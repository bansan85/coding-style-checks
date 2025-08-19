#include <clang-tidy/ClangTidy.h>
#include <clang-tidy/ClangTidyCheck.h>
#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang::ast_matchers;

namespace clang::tidy::readability {

class MemberOrderCheck : public ClangTidyCheck {
public:
  MemberOrderCheck(StringRef Name, ClangTidyContext *Context);

  void registerMatchers(MatchFinder *Finder) override;

  void check(const MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::readability

extern "C" clang::tidy::ClangTidyModule *clangTidyModuleFactory();
