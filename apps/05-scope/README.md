# 05-scope

- **Block scope** refers to the visibility of variables declared within a specific block of code, which is defined by curly braces {}. Variables declared inside a block are only accessible within that block, and they are destroyed once the block is exited.
- Key Points:
  - Local Variables: Variables declared inside a block are local to that block. They cannot be accessed outside of it.
  - Lifetime: The lifetime of a block-scoped variable lasts only as long as the block is executing.
  - Nested Blocks: You can have nested blocks, and a variable in an outer block is accessible in inner blocks, but an inner block variable cannot be accessed outside its own block.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=05-scope
  - make run app=05-scope

Binary path: build/05-scope/bin/05-scope

Alternative (from inside this folder):
  - cd app/05-scope
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
