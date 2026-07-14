# 47-encapsulation

## Concept
Encapsulation means hiding an object's data behind a controlled interface. Data
is `private`; the outside world can only touch it through `public` methods that
enforce the rules. This lets a class guarantee its own **invariants** (facts
that must always be true — here, "balance is never negative").

## Files
```
47-encapsulation/
  include/account.hpp   <- private balance_, public deposit/withdraw/balance
  src/account.cpp       <- the methods that enforce the invariant
  src/main.cpp          <- can only go through the public interface
```

## Line-by-line
- `private: double balance_;` — no outside code can read or write it directly.
- `deposit` / `withdraw` validate their input and reject anything that would
  break the invariant (negative amounts, overdrafts).
- `balance()` is a read-only **accessor** (getter); there's deliberately no
  setter — you change the balance only through meaningful operations.
- The commented `acct.balance_ = -999;` in `main` is a **compile error** — that's
  encapsulation doing its job.

## `public` vs `private`
- `public` — the interface; callable from anywhere.
- `private` — internals; accessible only within the class's own methods.
- (`protected`, for subclasses, appears in lesson 48.)
A `class` defaults to `private`; a `struct` defaults to `public` — the only real
difference between the two keywords.

## Why it matters
Once data is private and validated at every entry point, whole classes of bugs
become *impossible* rather than merely unlikely. You can also change the internal
representation later without breaking callers, because they only depend on the
public interface.

## Common pitfalls
- **Getter/setter for everything** defeats the purpose — if every field has a
  public setter with no validation, you've just made the data public with extra
  steps. Expose *operations*, not raw fields.
- Returning a non-const reference/pointer to a private member leaks control of
  it back out.

## Build it step by step
1. **Hide the data.** In `account.hpp`, put `balance_` under `private:` and give
   a constructor plus a `balance()` getter. In `main`, construct and print.
2. **Guard construction.** In `account.cpp`, clamp a negative initial balance to
   0 — the first invariant.
3. **Add operations.** Add `deposit`/`withdraw` that reject invalid amounts and
   overdrafts, then try an overdraw in `main` and confirm it's refused. Try to
   write `acct.balance_ = -1;` and watch it fail to compile.

## Build & run
```sh
make run app=47-encapsulation
```

## Try it yourself
Uncomment the `acct.balance_ = -999;` line and build — see the compiler reject
it. Then add a `bool transfer(Account& to, double amount)` method.
