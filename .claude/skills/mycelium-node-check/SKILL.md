---
name: mycelium-node-check
description: >-
  Validate a Mycelium custom node (BaseNode subclass) against the project's
  node conventions: registered with @node_registry.register, one node per
  file, NODE_TYPE namespaced as <category>.<op>, required class metadata set
  (DISPLAY_NAME, CATEGORY, DESCRIPTION, COLOR), define_io + execute
  implemented, heavy compute factored into _helpers/_run siblings, no
  swallowed errors, IS_LONG_RUNNING/bake protocol used correctly. Trigger
  when the user adds, edits, reviews, or asks to check a node, a BaseNode
  subclass, anything under a nodes builtin/<category>/ directory, or says
  "check this node", "is this node correct", "review my node", "does this
  follow the node pattern". Reports issues; auto-applies only deterministic
  tool fixes.
---

# Mycelium Node Check

Validates a `BaseNode` subclass against the Mycelium node conventions
(below) and the real contract in
`libraries/myc-engine/src/myc_engine/base.py`.

## The contract (from BaseNode)

A node MUST:

- **Subclass `BaseNode`** and be decorated `@node_registry.register`.
- Set **`NODE_TYPE`** — namespace `<category>.<op>`, lowercase, e.g.
  `autocomp.grain_curve`, `color.brightness`. Must be unique.
- Set the metadata class attrs: **`DISPLAY_NAME`**, **`CATEGORY`**,
  **`DESCRIPTION`**, **`COLOR`** (BaseNode defaults exist but a real node
  should set its own — empty `DISPLAY_NAME`/`DESCRIPTION` is a smell).
- Implement **`define_io(self)`** — declare ports via `add_input` /
  `add_output` with `DataType`, tooltips, sensible defaults/min/max.
- Implement **`execute(self, **kwargs)`\*\* — return a dict mapping output
  names to values; receive inputs as kwargs.

Optional but check for correctness:

- **`TAGS`** for per-app scoping (e.g. `"reviewer"`).
- **`IS_LONG_RUNNING` / `EXECUTION_MODE`** — heavy/thread-offloaded work is
  BAKE; if set, the **bake protocol** (`bake`, `serve_frame`) should be
  implemented coherently. Don't set `IS_LONG_RUNNING=True` on a node with
  no bake path.
- **`PASSTHROUGH_INPUT`** — set to the input _name_ for Nuke-style
  mute-passthrough (e.g. `"B"` on a Merge).

## Conventions to enforce

- **One node per file** under `.../builtin/<category>/<op>.py`. Two
  registered node classes in one file → split.
- **Heavy compute factored out** — pure helpers go in `_<topic>_helpers.py`
  / `_run.py` siblings (the codebase does this: `autocomp/_run.py`,
  `pipeline/_yaml.py`) so the node file stays readable and under the 300/500
  cap (→ hand to `mycelium-file-guard` if over).
- **No swallowed errors** — validation failures should raise
  `NodeValidationError` with a specific message (see `grain_curve.py`), not
  `except: pass` or a generic "something went wrong"
  (error handling, `mycelium-refactor`).
- **`NODE_TYPE` matches the directory category** where the codebase follows
  that (e.g. a file under `builtin/autocomp/` namespaced `autocomp.*`).

## How to run

1. Identify the node file(s) — the diff, or the path the user names.
2. Read the class + its `define_io`/`execute` (read narrow).
3. Check each contract/convention point above; verify `NODE_TYPE`
   uniqueness with a grep across the nodes tree.
4. Report: `path:line — issue → fix`. Group must-fix (breaks contract) vs
   nits.

## Auto-fix boundary — tool-driven ONLY

Apply without asking: `uv run ruff check --fix` on the file. Everything
else — adding missing metadata, splitting two nodes apart, restructuring
bake logic, factoring helpers — is **proposed**, since it's a judgment
edit that crosses the confirmation gates. After any approved edit, verify
the node still imports/registers: `make lint` and a scoped
`uv run pytest libraries/myc-nodes -q` if tests exist.

## Example skeleton (the shape to check against)

```python
@node_registry.register
class BrightnessNode(BaseNode):
    NODE_TYPE = "color.brightness"      # <category>.<op>, unique
    DISPLAY_NAME = "Brightness"
    CATEGORY = "color"
    DESCRIPTION = "Scale pixel values."
    COLOR = "#4488cc"

    def define_io(self) -> None:
        self.add_input("image", DataType.IMAGE)
        self.add_input("gain", DataType.FLOAT, default=1.0)
        self.add_output("image", DataType.IMAGE)

    def execute(self, image, gain):     # heavy math → _helpers.py
        return {"image": apply_gain(image, gain)}
```

```
read.py:1 — two registered classes in one file → split (one node per file)
read.py:40 — DISPLAY_NAME empty → set it (metadata smell)
read.py:88 — IS_LONG_RUNNING=True but no bake/serve_frame → fix protocol
```
