#include "MemberOrderCheck.h"
#include <clang-tidy/ClangTidy.h>
#include <clang-tidy/ClangTidyCheck.h>
#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <iostream>
using namespace clang::ast_matchers;

namespace clang::tidy::readability {

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

  for (const auto *Decl : Record->decls()) {
    AccessSpecifier CurrentAccess = Decl->getAccess();

    // Skip if no access specifier
    if (CurrentAccess == AS_none)
      continue;

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
