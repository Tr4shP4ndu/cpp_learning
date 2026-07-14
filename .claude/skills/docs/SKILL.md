---
name: docs
description: Use when writing or updating any technical documentation — "write docs", "document this", "add a readme", "docstrings", "add a docstring to this function", "API documentation", "OpenAPI/Swagger spec", "JSDoc", "build a doc portal", "add a diagram", "draw the architecture", "getting started guide", "tutorial", or "update the documentation". Covers inline docs, API specs, doc sites, and user guides across languages.
metadata:
  domain: quality
  role: specialist
  scope: implementation
  output-format: code
---

# Docs

One entry point for technical documentation: inline docs (docstrings, JSDoc),
API specs (OpenAPI/Swagger, GraphQL), documentation sites, and user guides.
Detect what's being asked, load the matching reference, then write docs that are
accurate, tested, and easy to maintain.

## Output

Docs/artifact first, then ≤3 short lines: what was skipped, when to add it.
Follows the `minimal` skill — laziest solution that works, fewest words.

## Workflow

1. **Detect** language, framework, and doc format from the code and request.
   Match the surrounding style rather than asking when the convention is clear.
2. **Analyze** what's undocumented — public functions, classes, endpoints.
3. **Document** in one consistent format: parameter types, returns, raised
   errors. Skip verbose docs for obvious getters/setters.
4. **Validate** every code example runs:
   - Python: `python -m doctest file.py`; `pytest --doctest-modules` for a module
   - TS/JS: `tsc --noEmit` confirms typed examples compile
   - OpenAPI: `npx @redocly/cli lint openapi.yaml`
   - Fix and re-validate before reporting.

## Quick reference

Google-style docstring — `Args:` / `Returns:` / `Raises:`. NumPy-style —
`Parameters`/`Returns`/`Raises` under dashed headers. JSDoc — `@param`,
`@returns`, `@throws`, `@example`. See the topic files for full patterns.

## Reference guide

Load only what the task touches:

| Topic | Read |
|-------|------|
| Python docstrings (Google, NumPy, Sphinx) | `references/python-docstrings.md` |
| TypeScript / JSDoc patterns | `references/typescript-jsdoc.md` |
| FastAPI / Django API docs | `references/api-docs-fastapi-django.md` |
| NestJS / Express API docs | `references/api-docs-nestjs-express.md` |
| Coverage reports | `references/coverage-reports.md` |
| Doc sites, static generators, search, testing | `references/documentation-systems.md` |
| OpenAPI 3.1, portals, GraphQL, WebSocket, gRPC, SDKs | `references/interactive-api-docs.md` |
| Getting-started guides, tutorials, troubleshooting, FAQs | `references/user-guides-tutorials.md` |

## Principles

- Document all public functions/classes with types, returns, and errors.
- Never write inaccurate or untested documentation.
- Match the existing format; don't invent a new one per file.
- Update docs in the same change as the code they describe.

## Project overrides

If this skill has a `projects/<project>.md` matching the repo you're in, read it
too — it carries project-specific rules that override the generic guidance.
