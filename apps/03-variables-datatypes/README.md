# 03-variables-datatypes

- Variables store data values. C++ is statically typed, meaning you must declare the type of variable.
- Signed vs. Unsigned Primitive Data Types
  - Signed Data Types:
    - Definition: Signed data types can represent both positive and negative values. They reserve one bit (the most significant bit) to store the sign of the number.
    - Range: The range of values for signed data types typically includes negative numbers, zero, and positive numbers. For example:
      - int (32 bits): The range is from -2,147,483,648 to 2,147,483,647.
      - short (16 bits): The range is from -32,768 to 32,767.
      - long (64 bits): The range is from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807.
    - Usage: Use signed data types when you need to store values that can be negative or when negative numbers are a valid part of the data being represented (e.g., temperatures, financial data).
  - Unsigned Data Types:
    - Definition: Unsigned data types can only represent non-negative values (zero and positive numbers) since they do not reserve a bit for the sign.
    - Range: The range of values for unsigned data types is typically larger in the positive range compared to their signed counterparts, as all bits are used to represent the number. For example:
      - unsigned int (32 bits): The range is from 0 to 4,294,967,295.
      - unsigned short (16 bits): The range is from 0 to 65,535.
      - unsigned long (64 bits): The range is from 0 to 18,446,744,073,709,551,615.
      - Usage: Use unsigned data types when you are certain that the values will never be negative (e.g., counting objects, indexing arrays, and representing memory addresses).

## Build and run (from repository root)

Run these from the repository root:
  - make build app=03-variables-datatypes
  - make run app=03-variables-datatypes

Binary path: build/03-variables-datatypes/bin/03-variables-datatypes

Alternative (from inside this folder):
  - cd app/03-variables-datatypes
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
