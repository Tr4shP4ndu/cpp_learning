---
name: code
description: Use when writing, refactoring, or debugging code in any specific language or framework — Python, FastAPI, TypeScript, NestJS, JavaScript, React, React Native, Next.js, C++, C#, .NET, Java, Spring, or game engines (Unity/Unreal). Routes to the matching language/framework guide loaded on demand. Trigger terms include those languages/frameworks, type hints, async, hooks, server components, Pydantic, SQLAlchemy, Spring Boot, EF Core, ECS, and any "write/build/fix X in <language>" request.
metadata:
  domain: language
  role: router
  scope: implementation
  output-format: code
---

# Code

One entry point for language- and framework-specific coding style. Detect what
you're writing, load that guide, then write code that matches it.

## Output

Code first, then ≤3 short lines: what you skipped, when to add it. Follows the `minimal` skill — laziest solution that works, fewest words.

## Routing

Figure out the language/framework from the file extension, imports, or the
request, then read the matching guide's `index.md` (and any topic file it points
to). Load only what the task touches — a hooks question needs
`react/references/hooks-patterns.md`, not the whole folder.

| You're writing… | Read |
|-----------------|------|
| Python (general, typing, async, pytest) | `references/python/index.md` |
| FastAPI / Pydantic / async SQLAlchemy API | `references/fastapi/index.md` |
| TypeScript (generics, utility types, tRPC) | `references/typescript/index.md` |
| NestJS backend (modules, guards, DI) | `references/nestjs/index.md` |
| Vanilla JavaScript / Node.js | `references/javascript/index.md` |
| React (components, hooks, RSC) | `references/react/index.md` |
| React Native / Expo | `references/react-native/index.md` |
| Next.js App Router | `references/nextjs/index.md` |
| C++ (modern C++20/23, templates, perf) | `references/cpp/index.md` |
| C# / .NET / ASP.NET Core / Blazor | `references/csharp/index.md` |
| Java / Spring Boot / WebFlux | `references/java/index.md` |
| Game dev (Unity, Unreal, ECS) | `references/game/index.md` |

Each `index.md` has its own reference table for deeper topics; follow it.

Group hint: a Python API touches both `python/` and `fastapi/`; a TS backend
touches `typescript/` and `nestjs/`; React work may touch `react/`,
`react-native/`, and `nextjs/`. Pull from siblings when the task spans them.

## Project overrides

If this skill has a `projects/<project>.md` matching the repo you're in, read it
too — it carries project-specific rules that override the generic guides.
