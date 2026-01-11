# Basic C++ Concepts

- [Basic C++ Concepts](#basic-c-concepts)
    - [Hello World Program](#hello-world-program)
    - [Comments](#comments)
    - [Variables and DataTypes](#variables-and-datatypes)
    - [Constants](#constants)
    - [Scope](#scope)
    - [Namespaces](#namespaces)
    - [Typedef and Aliases](#typedef-and-aliases)
    - [Arrays](#arrays)
      - [std::array](#stdarray)
    - [For-Loop](#for-loop)
      - [Range-Based For-Loop](#range-based-for-loop)
      - [While Loop](#while-loop)
      - [Do-While Loop](#do-while-loop)
      - [std::fill](#stdfill)
      - [Break Statement](#break-statement)
      - [Continue Statement](#continue-statement)
    - [Functions](#functions)
      - [Void Functions](#void-functions)
      - [Functions with Parameters](#functions-with-parameters)
      - [Function Overloading](#function-overloading)
      - [Recursive Function](#recursive-function)
  - [Address of Operator(\&)](#address-of-operator)
  - [Value Semantics](#value-semantics)
    - [References](#references)
    - [Dereferencing](#dereferencing)
  - [Dynamic Memory Allocation or HEP](#dynamic-memory-allocation-or-hep)

#### std::fill

- std::fill is a function from the <algorithm> library used to fill a range (such as an array or container) with a specific value.

```cpp
#include <iostream>
#include <array>
#include <algorithm> // for std::fill

int main() {
    std::array<int, 5> numbers; // Uninitialized array

    // Fill array with the value 42
    std::fill(numbers.begin(), numbers.end(), 42);

    for (int num : numbers) {
        std::cout << "Array element: " << num << std::endl;
    }

    return 0;
}
```

#### Break Statement
- The break statement is used to immediately exit a loop, regardless of its condition. When break is encountered, the loop stops executing and control is transferred to the first statement following the loop.

```cpp
#include <iostream>

int main() {
    for (int i = 0; i < 10; ++i) {
        if (i == 5) {
            std::cout << "Break at i = " << i << std::endl;
            break; // Exit the loop when i equals 5
        }
        std::cout << "Current value of i: " << i << std::endl;
    }
    std::cout << "Loop exited." << std::endl;
    return 0;
}
```

#### Continue Statement

- The continue statement is used to skip the current iteration of the loop and proceed to the next iteration. When continue is encountered, the remaining code inside the loop for that iteration is skipped, and the loop proceeds with the next iteration.

```cpp
#include <iostream>

int main() {
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            continue; // Skip the rest of the loop body for even numbers
        }
        std::cout << "Odd number: " << i << std::endl;
    }
    return 0;
}
```

### Functions

- Functions are reusable blocks of code that perform specific tasks. Functions help to structure your program, avoid repetition, and make code more modular and readable.
- Components of a Function:
  - Return type: The data type of the value the function returns (e.g., int, double, void).
  - Function name: A unique identifier to call the function.
  - Parameters: Optional inputs that the function accepts (also called arguments).
  - Function body: The block of code that defines what the function does.
  - Return statement: Specifies the value to return (if applicable).


```cpp
#include <iostream>

// Function declaration
int add(int num1, int num2) {
    return num1 + num2; // Return the sum of num1 and num2
}
int main() {
    std::cout << "The sum of " << num1 << " and " << num2 << " is: " << add(1, 2) << std::endl;
    return 0;
}
```

Forward declaration

```cpp
#include <iostream>
// Foward Declared
// Function declaration
int add(int a, int b);

int main() {
    std::cout << "The sum of " << num1 << " and " << num2 << " is: " << add(1, 2) << std::endl;
    return 0;
}

int add(int num1, int num2) {
    return num1 + num2; // Return the sum of num1 and num2
}
```

#### Void Functions

- Void functions do not return a value.
- Useful for performing tasks that do not need to send back a result.

```cpp
#include <iostream>

void printMessage() {
    std::cout << "Hello, this is a void function!" << std::endl;
}

int main() {
    printMessage(); // Call to void function
    return 0;
}
```

#### Functions with Parameters

- Functions can take parameters to process and return a value based on them.

```cpp
#include <iostream>

int multiply(int x, int y) {
    return x * y;
}

int main() {
    int product = multiply(4, 5);
    std::cout << "The product is: " << product << std::endl;
    return 0;
}
```

#### Function Overloading

- You can create multiple functions with the same name but different parameter lists.
- The correct function is chosen based on the arguments provided during the function call.

```cpp
#include <iostream>

// Function to add integers
int add(int x, int y) {
    return x + y;
}

// Overloaded function to add doubles
double add(double x, double y) {
    return x + y;
}

int main() {
    std::cout << "Integer sum: " << add(3, 4) << std::endl;
    std::cout << "Double sum: " << add(3.5, 4.5) << std::endl;
    return 0;
}
```

#### Recursive Function

- Recursive functions are functions that call themselves in order to solve a problem. This technique is useful for tasks that can be broken down into smaller, similar tasks. The key to writing a recursive function is to include a base case that stops the recursion and a recursive case that reduces the problem in each step.
- Components of a Recursive Function
  - Base Case: The condition under which the recursion stops. It prevents the function from calling itself indefinitely.
  - Recursive Case: The part of the function where it calls itself with a smaller or simpler argument.
- Example: Factorial of a Number
  - Let's look at a classic example of recursion: calculating the factorial of a number.
  - Mathematical Definition
  - The factorial of a number 𝑛 (written as 𝑛!) is defined as:
    - 𝑛!=𝑛×(𝑛−1)!for 𝑛>0
    - 0!=1

```cpp
#include <iostream>

// Recursive function to calculate factorial
int factorial(int n) {
    if (n <= 1) { // Base Case: When n is 0 or 1, the function returns 1.
        return 1;
    } else {
        return n * factorial(n - 1); // Recursive call - Recursive Case: The function calls itself with n - 1, reducing the problem size by one in each step.

        /*
        For example, to calculate factorial(5), the recursive calls look like this:
        5 * factorial(4)
        4 * factorial(3)
        3 * factorial(2)
        2 * factorial(1)
        The base case is reached at factorial(1), which returns 1.
        */
    }
}

int main() {
    int number = 5;
    std::cout << "Factorial of " << number << " is: " << factorial(number) << std::endl;
    return 0;
}
```

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

## Address of Operator(&)

- The address-of operator (&) in C++ is used to obtain the memory address of a variable. This operator is helpful when dealing with pointers, as it allows you to directly access the address where the variable is stored in memory.


```cpp
#include <iostream>

// Function definition
void foo() {
    int x = 72; // Local variable inside the function
}

int main() {
    int x = 42;               // Integer variable
    float y = 72.0;           // Float variable
    char a = 'a';             // Character variable
    signed char b = 'b';      // Signed character variable
    unsigned char c = 'c';    // Unsigned character variable

    // Printing the addresses of the variables
    std::cout << "Address of x: " << &(x) << std::endl;            // Example output: Address of x: 0x7ffd8d4e1234
    std::cout << "Size of x: " << sizeof(x) << " bytes" << std::endl; // Example output: Size of x: 4 bytes
    std::cout << "Address of y: " << &(y) << std::endl;            // Example output: Address of y: 0x7ffd8d4e1238
    std::cout << "Address of a: " << (void*)&(a) << std::endl;     // Example output: Address of a: 0x7ffd8d4e123c
    std::cout << "Address of b: " << (void*)&(b) << std::endl;     // Example output: Address of b: 0x7ffd8d4e123d
    std::cout << "Address of c: " << (void*)&(c) << std::endl;     // Example output: Address of c: 0x7ffd8d4e123e

    // Printing the addresses of the functions
    std::cout << "Address of foo function: " << (void*)&foo << std::endl;   // Example output: Address of foo function: 0x55e2b7f10270
    std::cout << "Address of main function: " << (void*)&main << std::endl; // Example output: Address of main function: 0x55e2b7f10290

    return 0;
}
```

- &x, &y, &a, &b, and &c are used to get the memory addresses of the variables x, y, a, b, and c.
- The expression (void*)&a is used to explicitly cast the address of a to a void*, which ensures that it is displayed as a memory address.
&foo and &main are used to get the addresses of the functions foo and main.

## Value Semantics

- Pass by Value is a method of passing arguments to a function where the function receives a copy of the argument's value, not the actual variable itself. Changes made to the parameter inside the function do not affect the original variable outside the function.

- When you pass an argument to a function by value:
  - A copy of the argument is created.
  - The function operates on this copy, not the original data.
  - Any changes to the parameter inside the function will not be reflected in the original variable outside the function.


```cpp
#include <iostream>

// Function that modifies the value of its parameter
void PassByValue(int number) {
    number = 100;  // This change affects only the local copy of the variable
    std::cout << "Inside function, number = " << number << std::endl;  // Output: 100
}

int main() {
    int num = 42;
    std::cout << "Before function call, num = " << &num << std::endl;
    std::cout << "Before function call, num = " << num << std::endl;   // Output: 42

    // Call the function with pass by value
    PassByValue(num);

    std::cout << "After function call, num = " << &num << std::endl;
    std::cout << "After function call, num = " << num << std::endl;    // Output: 42
    return 0;
}
```


### References

- References in C++ are used to create an alias for another variable. They provide a way to access the same memory location as the original variable using a different name. References are often used in function parameters to allow modification of the original data or to avoid copying large data structures.

```cpp
#include <iostream>
#include <typeinfo>  // Required for typeid()

int main() {
    int x = 42;           // Original variable
    int& ref = x;         // Reference to x
    // "int&" is the full type for a reference type

    ref = 43;  // Changing the value through the reference

    // Printing the value, type, and address of x
    std::cout << "x value     : " << x << std::endl;          // Output: 43
    std::cout << "x type      : " << typeid(x).name() << std::endl;  // Output: int
    std::cout << "x address   : " << &x << std::endl;         // Address of x

    // Printing the value, type, and address of ref
    std::cout << "ref value   : " << ref << std::endl;        // Output: 43
    std::cout << "ref type    : " << typeid(ref).name() << std::endl;  // Output: int
    std::cout << "ref address : " << &ref << std::endl;       // Same address as x

    return 0;
}
```

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
void PassByValue(std::vector<int> arg) {
    arg[0] = 1;  // Modifies the copy of the vector
}

void PassByReference(std::vector<int>& alias) {
    alias[0] = 1;  // Modifies the original vector
}

int main() {
    std::vector<int> x(1000000000);  // Create a vector with 1 billion elements
    std::fill(std::begin(x), std::end(x), 1);  // Fill the vector with 1s

    // Call our function
    PassByValue(x);  // Pass the vector by value (creates a copy) Slower
    // PassByReference(x);  // Pass the vector by reference (no copy) Faster

    return 0;
}
```

### Dereferencing

- Dereferencing a pointer means accessing the value stored at the memory location that the pointer points to. In other words, it allows you to read or modify the value of the variable the pointer refers to.
- The * operator is used to dereference a pointer.

```cpp
#include <iostream>

int main() {
    int num = 10;
    int* ptr = &num;  // Pointer to num

    std::cout << "Value of num: " << num << std::endl;              // Output: 10
    std::cout << "Address of num (&num): " << &num << std::endl;    // Prints the address of num
    std::cout << "Pointer value (ptr): " << ptr << std::endl;       // Prints the same address as &num
    std::cout << "Dereferenced pointer (*ptr): " << *ptr << std::endl;  // Output: 10

    // Modify the value of num using the pointer
    *ptr = 20;
    std::cout << "New value of num after dereferencing: " << num << std::endl;  // Output: 20

    return 0;
}
```

## Dynamic Memory Allocation or HEP


- Dynamic memory allocation is a way to allocate memory at runtime, which means the amount of memory required does not need to be known at compile time. In C++, dynamic memory is managed using the operators new and delete.
- *new* Operator
  - The new operator is used to allocate memory dynamically on the heap (free store).
  - It returns a pointer to the allocated memory.
  - For basic types, it initializes the allocated memory to a specific value.
