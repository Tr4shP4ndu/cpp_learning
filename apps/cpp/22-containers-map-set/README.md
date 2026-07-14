# 23-containers-map-set

## Concept
Beyond sequences (`vector`), you constantly need two other shapes: **key → value
lookup** (a map / dictionary) and **a set of unique values**. The standard
library provides both, in ordered (tree-based) and unordered (hash-based)
flavors.

## Minimal example
See `src/main.cpp`.

## The containers
- `std::map<K, V>` — keys kept **sorted**; lookup/insert O(log n). Iterating
  yields keys in order.
- `std::unordered_map<K, V>` — a **hash table**; average O(1) lookup, no ordering.
  Prefer it when you don't need sorted keys.
- `std::set<T>` — sorted collection of **unique** values (duplicates ignored).
  `std::unordered_set<T>` is its hash-based sibling.

## Line-by-line
- `ages["Ada"] = 36;` — `operator[]` inserts the key if missing, then assigns.
- `ages.find("Ada")` — returns an iterator; compare to `.end()` to test presence
  without inserting.
- `it->first` / `it->second` — a map element is a `pair`: `.first` is the key,
  `.second` the value.
- `for (const auto& [name, age] : ages)` — structured bindings unpack each pair.
- `++counts["apple"];` — the counting idiom: a missing key value-initializes to
  0, then increments.

## Common pitfalls
- **`operator[]` inserts.** `if (m["x"] == ...)` on a missing key silently adds
  `"x"` with a default value. Use `.find()` or `.contains()` (C++20) to *test*
  without inserting.
- `unordered_map` gives no ordering — don't rely on iteration order.
- Custom key types need a comparison (`<`) for `map`/`set`, or a hash for the
  unordered versions.

## Build & run
```sh
make run app=23-containers-map-set
```

## Try it yourself
Count the letters in a word: loop over a `std::string` and do
`++counts[ch]` into an `unordered_map<char,int>`, then print each count.
