#include "MemberOrderCheck.h"
#include <clang-tidy/ClangTidy.h>
#include <clang-tidy/ClangTidyCheck.h>
#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <iostream>
using namespace clang::ast_matchers;

namespace clang::tidy::readability {

namespace {
enum MemberCategory {
  MC_TypesAndAliases = 1,       // typedef, using, enum, nested classes, friend types
  MC_NonStaticDataMembers = 2,  // non-static data members (structs only)
  MC_StaticConstants = 3,       // static const/constexpr data members
  MC_FactoryFunctions = 4,      // static functions returning class type
  MC_ConstructorsAndAssign = 5, // constructors, assignment operators
  MC_Destructor = 6,            // destructor
  MC_OtherFunctions = 7,        // all other functions
  MC_OtherDataMembers = 8       // all other data members
};

MemberCategory getMemberCategory(const Decl *D, bool isStruct) {
  if (isa<TypedefNameDecl>(D) || isa<EnumDecl>(D) || isa<CXXRecordDecl>(D) ||
      isa<FriendDecl>(D))
    return MC_TypesAndAliases;

  if (const auto *Field = dyn_cast<FieldDecl>(D)) {
    if (isStruct)
      return MC_NonStaticDataMembers;
  }

  if (const auto *Var = dyn_cast<VarDecl>(D)) {
    if (Var->isStaticDataMember() && Var->getType().isConstQualified())
      return MC_StaticConstants;
    return MC_OtherDataMembers;
  }

  if (const auto *Method = dyn_cast<CXXMethodDecl>(D)) {
    if (Method->isStatic()) {
      QualType RetType = Method->getReturnType();
      if (const auto *RT = RetType->getAsCXXRecordDecl()) {
        if (RT == Method->getParent())
          return MC_FactoryFunctions;
      }
    }

    if (isa<CXXConstructorDecl>(Method) || Method->isCopyAssignmentOperator() ||
        Method->isMoveAssignmentOperator())
      return MC_ConstructorsAndAssign;

    if (isa<CXXDestructorDecl>(Method))
      return MC_Destructor;

    return MC_OtherFunctions;
  }

  if (isa<FunctionDecl>(D) || isa<FunctionTemplateDecl>(D))
    return MC_OtherFunctions;

  return MC_OtherDataMembers;
}
} // namespace

MemberOrderCheck::MemberOrderCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {}

void MemberOrderCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxRecordDecl(isDefinition()).bind("record"), this);
}

void MemberOrderCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Record = Result.Nodes.getNodeAs<CXXRecordDecl>("record");
  if (!Record || Record->isUnion())
    return;

  AccessSpecifier LastAccess = AS_none;

  bool isStruct = Record->isStruct();
  MemberCategory LastCategory = MC_TypesAndAliases;

  for (const auto *Decl : Record->decls()) {
    // Skip implicit declarations (compiler-generated constructors, etc.)
    if (Decl->isImplicit())
      continue;

    AccessSpecifier CurrentAccess = Decl->getAccess();

    assert(CurrentAccess != AS_none);

    // Check order: public -> protected -> private
    if ((LastAccess == AS_protected && CurrentAccess == AS_public) ||
        (LastAccess == AS_private &&
         (CurrentAccess == AS_public || CurrentAccess == AS_protected))) {
      diag(Decl->getLocation(), "member access specifiers should be in order: "
                                "public, protected, private")
          << FixItHint::CreateInsertion(Decl->getLocation(),
                                        "// TODO: reorder members\n");
    }

    LastAccess = CurrentAccess;

    // Skip access specifier declarations (public:, protected:, private:)
    if (isa<AccessSpecDecl>(Decl)) {
      LastCategory = MC_TypesAndAliases;
      continue;
    }

    MemberCategory CurrentCategory = getMemberCategory(Decl, isStruct);

    // Check member order within the same access level
    if (CurrentCategory < LastCategory) {
      diag(Decl->getLocation(),
           "members should be ordered: types/aliases, "
           "data members (structs), static constants, "
           "factory functions, constructors/assignment, "
           "destructor, other functions, other data members")
          << FixItHint::CreateInsertion(Decl->getLocation(),
                                        "// TODO: reorder members\n");
    }

    LastCategory = std::max(LastCategory, CurrentCategory);
  }
}

} // namespace clang::tidy::readability

namespace {

class ReadabilityModule : public clang::tidy::ClangTidyModule {
public:
  void addCheckFactories(
      clang::tidy::ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<clang::tidy::readability::MemberOrderCheck>(
        "readability-member-order");
  }
};

} // namespace

namespace clang::tidy {
// Register the module
static ClangTidyModuleRegistry::Add<ReadabilityModule>
    X("coding-style-module", "Adds readability-related checks.");

} // namespace clang::tidy
