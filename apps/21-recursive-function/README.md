# 21-recursive-function

- A recursive function calls itself to solve a problem by breaking it into smaller subproblems. Use it when a problem has a natural self-similar structure (trees, divide-and-conquer, math recurrences).

## Anatomy of recursion

- Base case: when to stop recursing (must be reachable).
- Recursive case: reduce the problem size and call the function again.
- Progress: each step must move toward the base case.

## Example: factorial

Mathematical definition:
- n! = n × (n−1)! for n > 1
- 1! = 1, 0! = 1

// Defensive version with input check
long long factorial(int n) {
  if (n < 0) throw std::invalid_argument("negative factorial");
  if (n == 0 || n == 1) return 1;          // base case
  return static_cast<long long>(n) * factorial(n - 1); // recursive case
}

## Example: Fibonacci

Naive recursion matches the definition but is exponential due to repeated work. Prefer memoization or iteration.

// Memoized Fibonacci (top-down)
long long fib(int n, std::vector<long long>& memo) {
  if (n < 0) throw std::invalid_argument("negative fib");
  if (n <= 1) return n;            // base cases: 0, 1
  auto& v = memo[n];
  if (v != -1) return v;
  v = fib(n - 1, memo) + fib(n - 2, memo); // recursive case
  return v;
}

int main() {
  std::cout << factorial(5) << "\n"; // 120

  int n = 45;
  std::vector<long long> memo(n + 1, -1);
  std::cout << fib(n, memo) << "\n";  // fast thanks to memoization
}

## Tail recursion

- A function is tail-recursive if the recursive call is the last operation.
- C++ compilers are not required to optimize tail recursion; do not rely on it to avoid stack usage.

## Pitfalls and tips

- Missing or incorrect base case leads to infinite recursion and stack overflow.
- Ensure progress toward the base case on every path.
- Consider iteration when it’s simpler and avoids recursion depth limits.
- For overlapping subproblems (like Fibonacci), use memoization (top-down) or dynamic programming (bottom-up).
- Be careful with global state or side effects; recursion multiplies them.


Bit simpler way

```cpp
#include <iostream>

int coutdown(int n){
  // Base Case
  if(n==0){
    std::cout << "0...Blast Off!!!" << std:endl;
    return 0;
  }
  // Recursive case
  std::cout << n << std::endl;
  return countdown(n-1)l
}

int main(){
  countdown(5);
  return 0;
}
```

Call Stack
```cpp
#include <iostream>

int coutdown(int n){
  // Base Case
  if(n==0){
    std::cout << "0...Blast Off!!!" << std:endl;
    return 0;
  }
  // Recursive case
  std::cout << n << std::endl;
  return countdown(n-1)l
}

int main(){
  countdown(500000);
  return 0;
}
```

- Call Stack: When a function is called in C++, it gets added to a special area of memory called the call stack. This stack keeps track of each function call and stores important information like:
  - The return address, which tells the program where to go after the function finishes.
  - The current values of variables, like the argument values passed to the function.
- Stack Frame: Each function call creates a new stack frame, which is a block of memory that holds the function's data (like the return address and the values of its variables).
- Recursion and Growing the Stack: When a recursive function (like a countdown function) calls itself, a new stack frame is created for each recursive call. For example:
  - First, the function countdown(5) runs, creating a stack frame.
  - Then it calls countdown(4), creating another stack frame, and so on.
  - This process continues until a stopping condition is met, such as countdown(0).
- Stack Limitations: As the stack grows with each recursive call, it can take up a lot of memory. If the recursion goes too deep (for example, if it keeps calling itself many thousands or millions of times), the stack might run out of space. When this happens, the program might crash due to a stack overflow error.
- Heap vs. Stack: The stack is just one part of the memory that your program uses. Another part is the heap, where dynamically allocated memory lives. If the recursive calls grow too large, they might overflow into other parts of memory, causing errors.
- Key Point: Be careful with recursive functions in C++. If you don’t have a clear stopping condition, or if you call the function too many times, you might run into stack overflow issues.

Other example - Fibonacci Sequence

- The Fibonacci sequence is another common example of recursion. It is defined as:
  - F(0)=0
  - F(1)=1
  - F(n)=F(n−1)+F(n−2) for 𝑛>1

```cpp
#include <iostream>

// Recursive function to calculate Fibonacci numbers
int fibonacci(int n) {
    if (n <= 1) { // Base case: F(0) = 0 and F(1) = 1 - Base Case: When n is 0 or 1, the function returns n.
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2); // Recursive call - Recursive Case: The function calls itself with n - 1 and n - 2, The recursive calls continue until the base cases are reached.
    }
}

int main() {
    int number = 6;
    std::cout << "Fibonacci of " << number << " is: " << fibonacci(number) << std::endl;
    return 0;
}
```
- Pros and Cons of Recursive Functions
  - Pros:
    - Simpler Code: Recursive solutions can be more straightforward and easier to understand for problems that have a recursive nature.
    - Natural Representation: Recursion is a natural way to represent problems like tree traversal, factorial, Fibonacci, etc.
  - Cons:
    - Performance: Recursive functions can be inefficient if they involve repeated calculations (like the Fibonacci example).
    - Memory Overhead: Each recursive call adds a new frame to the call stack, which can lead to a stack overflow if the recursion is too deep.
- Best Practices for Recursive Functions
  - Always include a base case to stop the recursion.
  - Optimize your code to avoid redundant calculations (e.g., use memoization for the Fibonacci sequence).
  - Use recursion for problems that are naturally recursive, like tree traversal, combinatorial problems, and divide-and-conquer algorithms.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=21-recursive-function
  - make run app=21-recursive-function

Binary path: build/21-recursive-function/bin/21-recursive-function

Alternative (from inside this folder):
  - cd app/21-recursive-function
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
