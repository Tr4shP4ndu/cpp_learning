---
name: mycelium-docs
description: >-
  Mycelium documentation discipline — write small, focused docs under
  docs/<area>/, one topic per file, with mermaid diagrams for flow and
  architecture, Flawless brand styling, explicit naming, and normal (not
  normal) prose aimed at the next developer. Trigger whenever the user asks to
  "write docs", "document this", "add a readme", "explain how X works in the
  docs", "update the documentation", "add a diagram", "draw the architecture",
  or is creating or restructuring anything under docs/ in the Mycelium repo.
  Pairs with mycelium-library-api when documenting a library's public surface.
version: 0.1.0
---

# Mycelium — Documentation

Self-contained documentation discipline for this repo. The core rules are
below.

## Core rules

- One topic per file; split past ~200 lines.
- One folder per area under `docs/<area>/` (architecture, design, apps,
  product…), each with a short index. Co-locate near the concept; keep
  areas decoupled — cross-link, don't inline.
- Lead with purpose (what + why) before detail. Explicit names, define
  acronyms on first use, real correct examples.
- Mermaid for diagrams (`flowchart`, `sequenceDiagram`, `stateDiagram-v2`,
  `erDiagram`). Keep each small.
- Flawless brand styling: defer to the `flawless-*` skills for palette and
  voice; this skill owns structure. Note if brand spec is unavailable.
- Docs are normal prose. Active voice, present tense.
- Update the doc in the same change as the code it describes.

## Example skeleton

````markdown
# Read Node

Reads image sequences and remaps the timeline. Used by the Reviewer to
present on-disk frames at a user-chosen start frame.

## Flow

\```mermaid
flowchart LR
disk[(EXR on disk)] --> read[Read node] --> viewer[Viewer]
\```

## Inputs

- `start_at` — Nuke-style first-frame remap. 0 = native numbering.
````

## Hand-offs

- Documenting a reusable package's API → `mycelium-library-api`.
- Brand colors / tone / messaging → `flawless-*` skills.
