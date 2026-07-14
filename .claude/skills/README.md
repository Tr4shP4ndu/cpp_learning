# Skills

A suite of generic, reusable Claude Code skills. Each skill is language- and
project-agnostic; project-specific rules live in `<skill>/projects/<project>.md`
and load only when you're working in that repo.

## Skills

| Skill | Use for |
|-------|---------|
| `code` | Writing code in a specific language/framework. Router → `code/references/<lang>/` (python, fastapi, typescript, nestjs, javascript, react, react-native, nextjs, cpp, csharp, java, game). |
| `review` | Broad-scope code / PR review: correctness, security, performance, maintainability, tests. |
| `refactor` | Restructure & simplify existing code — rule-of-three, size caps, extract-to-reusable-unit. |
| `debug` | Root-cause-first debugging: evidence at boundaries, reproduce, one change at a time. |
| `docs` | Docstrings, OpenAPI/JSDoc, doc portals, guides, diagrams. |
| `ship` | Stage → gate → house-style commit → push or PR. |
| `minimal` | The least-code / fewest-words discipline. The other skills' Output rules point here. |
| `api-designer` | REST/GraphQL design, OpenAPI, versioning, pagination. |
| `architecture-designer` | System design, ADRs, technology trade-offs, scalability. |
| `database-optimizer` | Slow queries, execution plans, indexes, partitioning. |
| `devops-engineer` | Dockerfiles, CI/CD, Kubernetes, Terraform, incident runbooks. |
| `ml-pipeline` | Experiment tracking, training orchestration, feature stores, MLOps. |

## Conventions

- **Generic first.** Top-level skills carry no project specifics. Repo rules go
  in `<skill>/projects/<project>.md` and are read only in that repo. To onboard a
  new project, add a `projects/<name>.md` — never fork a whole `<project>-*` skill.
- **`code` is a router.** Language guides sit under `code/references/<lang>/index.md`
  (+ topic files), loaded on demand. Add a language by adding a folder.
- **`minimal` owns terseness/YAGNI.** Every skill's `## Output` stanza reads
  "Follows the `minimal` skill — laziest solution that works, fewest words."
  Keep that single source; don't restate its rules elsewhere.
- **Each skill has `## Output` + `## Project overrides` stanzas.**
