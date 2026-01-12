# comments

Comments explain intent, clarify tricky code, and document public APIs. Keep them accurate and concise—prefer explaining "why" over restating the obvious "what".

## Types of comments

- Single-line: starts with // and runs to end of line.
  // This increments the counter
  count++; // trailing explanatory comment

- Block (multi-line): delimited by /* and */.
  /*
    This block explains the algorithm steps:
    1) parse input
    2) validate
    3) execute
  */

- Documentation (Doxygen-style):
  /// Brief one-line description
  /**
   * Detailed description spanning multiple lines.
   * @param x input value
   * @return processed result
   */

## Best practices

- Explain intent, invariants, pre/post-conditions, and non-obvious decisions.
- Keep comments near the code they explain; update them when code changes.
- Prefer self-documenting names; don’t narrate every line.
- Use block comments for file/header overviews; single-line for short notes.
- For temporary code toggles, prefer preprocessor guards with rationale:
  #if 0
  // Disabled pending bug #123
  oldImplementation();
  #endif

## Pitfalls

- C++ block comments do not nest: /* outer /* inner */ outer */ is invalid.
- Don’t comment out large chunks permanently; rely on version control.
- Avoid misleading comments that contradict the code.
- Don’t expose secrets or credentials in comments.

## Small example

// Compute factorial iteratively (clarify the approach, not the syntax)
int factorial(int n) {
  // Precondition: n >= 0 (assert in debug builds if necessary)
  int result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i; // grows quickly; beware overflow
  }
  return result;
}

## Build and run (from repository root)

Run these from the repository root:
  - make build app=comments
  - make run app=comments

Binary path: build/comments/bin/comments

Alternative (from inside this folder):
  - cd app/comments
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
