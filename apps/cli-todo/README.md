# cli-todo

A minimal command-line to-do list that **persists to a file** — the classic
"my first useful program." Add items, list them, mark them done; the list
survives between runs.

**Builds on:** strings (20), vectors (21), files-and-filesystem (23), structs
(43), argv handling.

## Run
```sh
make project name=cli-todo ARGS='add "buy milk"'
make project name=cli-todo ARGS='add "write C++"'
make project name=cli-todo ARGS=list
make project name=cli-todo ARGS='done 1'
```
The list is stored in `build/todo.txt` (git-ignored).

## Try extending it
- A `clear` command, or `done` by matching text instead of number.
- Store a "done" flag per item instead of deleting (introduce a small `struct`).
- Swap the flat file for one line per field, or a tiny CSV.
