# 60-concurrency

## Concept
Concurrency is doing several things at once. C++ gives you `std::thread` to run
work in parallel, and the tools to do it **safely**: a `std::mutex` to serialize
access to shared data, `std::atomic` for lock-free counters, and `std::async` /
`std::future` to run a task and collect its result later. The hard part isn't
starting threads — it's avoiding **data races** on shared state.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `std::thread` in a vector, `emplace_back(worker, i)` — spawn several threads;
  `t.join()` waits for each to finish before `main` continues.
- `std::lock_guard<std::mutex> lock(io_mutex)` — RAII lock: holds the mutex for
  the scope so only one thread writes to `std::cout` at a time (interleaved
  output is otherwise a race).
- `std::atomic<int> counter{0}` with `++counter` from two threads — atomic
  increments have no data race, so the result is deterministically 2000.
- `std::async(std::launch::async, lambda)` returns a `std::future`; `.get()`
  blocks until the task finishes and hands back its return value.

## Why this matters
Shared mutable state touched by multiple threads without synchronization is a
**data race** — undefined behavior, and the source of the nastiest bugs (they
vanish under a debugger). Mutexes and atomics are how you make sharing correct;
`future` lets you get a result back without manual signaling.

## Common pitfalls
- Forgetting to `join()` (or `detach()`) a thread — its destructor calls
  `std::terminate`.
- A plain `int counter` bumped from two threads is a data race even for `++` —
  use `std::atomic` or a mutex. (Build with `-fsanitize=thread` to catch these.)
- **Deadlock**: two threads each holding one lock and waiting for the other.
  Always acquire multiple locks in a consistent order.

## Build & run
```sh
make run-cpp-app-60-concurrency
make build-cpp-app-60-concurrency STD=c++11   # threads/atomics/futures exist since C++11
```

## Try it yourself
Remove the `lock_guard` and run a few times — watch the thread output interleave
into garbage. Then replace `std::atomic<int>` with a plain `int` and confirm the
counter no longer reliably reaches 2000.
