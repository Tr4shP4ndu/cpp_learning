# __PROJECT_NAME__

Your own project — it lives in `apps/__PROJECT_NAME__/`, alongside the `c/` and
`cpp/` lesson trees. Unlike the numbered lessons, a project owns its own
`Makefile` and can have any structure you want.

## Build & run
```sh
make project       name=__PROJECT_NAME__   # build + run (from the repo root)
make project-build name=__PROJECT_NAME__   # build only
```

Or from inside this folder, drive the Makefile directly:
```sh
make build
make run ARGS="whatever your program takes"
make clean
```

## Layout
Start with `src/`. Add `include/`, `assets/`, more source folders — whatever your
project needs. Just keep the `build` and `run` targets in this Makefile working,
and the workspace can build you.

The workspace passes `CXX`, `CC`, `STD`, `CSTD`, `BUILD_TYPE` (and `ARGS` for
`run`) down to this Makefile as overridable defaults.
