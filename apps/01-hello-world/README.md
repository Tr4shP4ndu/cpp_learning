# hello_world

The first program you should write in any language is "Hello, World!".

## Build and run (from repository root)

Run these from the repository root:
  - make build app=hello_world
  - make run app=hello_world

Binary path: build/hello_world/bin/hello_world

Alternative (from inside this folder):
  - cd app/hello_world
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
