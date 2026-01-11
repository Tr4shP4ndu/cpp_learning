# 06-namespaces

-  A namespace is a declarative region that provides a scope to the identifiers (names of types, functions, variables, etc.) inside it. Namespaces help avoid name conflicts in large projects by allowing entities with the same name to coexist as long as they are in different namespaces.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=06-namespaces
  - make run app=06-namespaces

Binary path: build/06-namespaces/bin/06-namespaces

Alternative (from inside this folder):
  - cd app/06-namespaces
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
