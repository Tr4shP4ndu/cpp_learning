---
name: mycelium-library-api
description: >-
  Mycelium reusable-library and API design — carve shared code into versioned,
  importable units under libraries/ (Python myc-*) and packages/ (TS myc-*)
  with an explicit public surface, underscore-private internals, stable
  signatures, and strict downward-only dependencies (apps → packages →
  libraries) so teammates can import instead of copy. Trigger whenever the user
  asks to "extract this into a library", "make this reusable", "share this
  code", "create a package", "design the API", "what's the public interface",
  "this belongs in a library", or is adding or restructuring anything under
  libraries/ or packages/ in the Mycelium repo. Pairs with mycelium-docs for
  the library README and mycelium-minimal to avoid premature extraction.
version: 0.1.0
---

# Mycelium — Library & API Design

Self-contained library and API design discipline for this repo. The core
rules are below.

## Core rules

- Extract when reused by 2+ consumers or it's a clear domain boundary.
  One-app-once → keep in the app (don't pre-extract).
- Explicit public surface in one entry point (`__init__.py` / `index.ts`).
  `_`-prefixed modules are private; callers never import them.
- Public signature change = confirmation gate; teammates are consumers.
- Dependencies point down only: apps → packages → libraries. No library
  imports an app or hidden global state. Data in, data out.
- Each library owns its manifest, README (one usage example), types,
  constants, and tests for the public surface.

## Where things go

Python: `myc-frame-io` (EXR), `myc-colorspace` (OCIO), `myc-image-utils`,
`myc-engine`, `myc-nodes`, `myc-models`, `myc-providers`. Frontend:
`myc-viewer`, `myc-store`, `myc-ui`, `myc-node-editor`, `myc-roto*`,
`myc-tracker`, `myc-property-forms`. Find the owning unit before adding new
code; new capability with no home → new `myc-*`, not a bolt-on.

## Hand-offs

- README / diagrams for the library → `mycelium-docs`.
- "Do we even need this library yet?" → `mycelium-minimal`.
- Pre-commit gate → `mycelium-check`.
