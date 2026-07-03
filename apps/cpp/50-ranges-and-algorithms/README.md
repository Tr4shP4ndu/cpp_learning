# 50-ranges-and-algorithms

## Concept
The `<algorithm>` header has long offered reusable operations (`sort`,
`count_if`, `find`) that work via iterator pairs. C++20 **ranges** make them
nicer: call them on a whole container, and compose lazy **views**
(filter → transform → take) into readable pipelines with no intermediate copies.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `std::ranges::sort(nums)` — the ranges version takes the container directly,
  instead of `std::sort(nums.begin(), nums.end())`.
- `std::ranges::count_if(nums, pred)` — same convenience for predicates.
- The `|` pipeline chains **views**:
  - `views::filter(pred)` — keep matching elements,
  - `views::transform(fn)` — map each element,
  - `views::take(n)` — first `n`.
- Views are **lazy**: nothing runs until the `for` loop pulls elements through,
  and no temporary vectors are allocated between stages.

## Why this matters
- **Readability:** the pipeline reads top-to-bottom as "odds, times ten, first
  two."
- **Efficiency:** lazy evaluation means one pass, no intermediate containers.
- It builds directly on lambdas (lesson 49) and the algorithm/iterator ideas
  from lesson 20.

## Common pitfalls
- **C++20** required for ranges; the views pipeline needs `<ranges>`.
- A view **doesn't own** its data — it refers to the underlying container. Don't
  let the container die while a view over it is alive.
- Some views are single-pass; re-iterating or storing them has caveats.

## Build it step by step
1. **A ranges algorithm.** Call `std::ranges::sort(nums)` (whole container, no
   `begin()/end()`) and print the result.
2. **A predicate.** Add `std::ranges::count_if(nums, ...)` with a lambda.
3. **A pipeline.** Compose `nums | std::views::filter(...) |
   std::views::transform(...) | std::views::take(2)` and iterate it — note
   nothing computes until the loop pulls values.

## Build & run
```sh
make run app=50-ranges-and-algorithms
make build app=50-ranges-and-algorithms STD=c++17   # ranges are C++20 — fails
```

## Try it yourself
Add `std::views::reverse` to the pipeline, or compute the sum of the pipeline's
results with `std::ranges::fold_left` (C++23) or a manual loop.
