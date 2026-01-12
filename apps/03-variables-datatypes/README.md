# 03-variables-datatypes

- Variables store values with explicit types. C++ is statically typed: a variable’s type is known at compile time.

## Declaring and initializing

- Declaration defines a name and a type: `int count;` (uninitialized!)
- Value initialization: `int count{};` initializes to zero; `int count = 0;` is equivalent for scalars.
- Uniform initialization with braces prevents narrowing: `int x{3.14}; // error: narrows`
- Avoid `int x();` at global scope; that declares a function (the most vexing parse).

## Fundamental types (overview)

- Integers: `short`, `int`, `long`, `long long` (sizes are implementation-defined).
- Fixed-width integers (<cstdint>): `int8_t`, `int16_t`, `int32_t`, `int64_t`, and unsigned variants; use when exact width matters.
- Size/index: `size_t` (unsigned); differences: `ptrdiff_t` (signed for pointer diffs).
- Floating point: `float` (≈7 digits), `double` (≈15), `long double` (platform-specific).
- Boolean: `bool` (`true`/`false`).
- Character types: `char` (signedness is implementation-defined), `signed char`, `unsigned char`, wide and Unicode: `wchar_t`, `char8_t` (UTF‑8), `char16_t`, `char32_t`.

## Signed vs unsigned

- Signed represent negative and positive values (two’s complement on most platforms).
- Unsigned represent only non-negative values; same width yields a larger max.
- Choose unsigned for bitmasks and sizes (e.g., `size_t`), signed for arithmetic that can go negative.

Examples (typical 32-bit int):
- `int`: −2,147,483,648 .. 2,147,483,647
- `unsigned int`: 0 .. 4,294,967,295

## Literals and suffixes

- Integer bases: `42` (dec), `0x2A` (hex), `0b101010` (binary), `052` (octal).
- Digit separators: `1'000'000`.
- Integer suffixes: `u` (unsigned), `l`/`ll` (long/long long), combos like `42ull`.
- Floating suffixes: `f` (float), `l` (long double) e.g., `1.0f`, `1.0L`.
- String/char prefixes: `u8"..."`, `u"..."`, `U"..."`, `L"..."`. C++14 `"s"` creates `std::string` when `using namespace std::string_literals;`.

## Type inference

- `auto` deduces type from initializer: `auto n = 1; // int`
- `auto&`, `const auto&` for references without copying; `auto&&` for forwarding.
- `decltype(expr)` yields the declared type of an expression.

## Conversions and narrowing

- Implicit conversions may lose information (double -> int) or change meaning (signed <-> unsigned). Prefer explicit `static_cast` when narrowing.
- Avoid mixing signed and unsigned in comparisons; it can lead to surprises.

## Small examples

int a{};              // zero-initialized
auto b = 123'456u;    // unsigned literal with separator
std::int64_t big = 9'000'000'000LL; // from <cstdint>
double pi = 3.14159;  // floating point
char8_t smile[] = u8"🙂"; // UTF-8 code units (C++20)

## Pitfalls and tips

- Uninitialized locals have indeterminate values; always initialize.
- Use `size_t` for sizes/indices; cast carefully when interfacing with signed values.
- Beware overflow/underflow; prefer wider types or checked arithmetic when needed.
- `char` may be signed or unsigned; for raw bytes use `std::byte` or `unsigned char`.
- Prefer fixed-width types for I/O, serialization, and cross-platform boundaries.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=03-variables-datatypes
  - make run app=03-variables-datatypes

Binary path: build/03-variables-datatypes/bin/03-variables-datatypes

Alternative (from inside this folder):
  - cd app/03-variables-datatypes
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
