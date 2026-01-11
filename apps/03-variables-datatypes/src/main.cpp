#include <iostream>

int main(){

  // Declare and initialize signed primitive data types
  int signedIntVar = -10;                 // Signed integer
  short signedShortVar = -20;             // Signed short integer
  long signedLongVar = -30000;            // Signed long integer
  long long signedLongLongVar = -4000000; // Signed long long integer

  // Declare and initialize unsigned primitive data types
  unsigned int unsignedIntVar = 10;                 // Unsigned integer
  unsigned short unsignedShortVar = 20;             // Unsigned short integer
  unsigned long unsignedLongVar = 30000;            // Unsigned long integer
  unsigned long long unsignedLongLongVar = 4000000; // Unsigned long long integer

  // Declare and initialize other primitive data types
  float floatVar = 10.5f;              // Floating-point type
  double doubleVar = 20.99;             // Double-precision floating-point type
  char charVar = 'A';                   // Character type
  bool boolVar = true;                  // Boolean type

  // Use sizeof to get the size of each data type in bytes
  std::cout << "Size of signed int: " << sizeof(signedIntVar) << " bytes" << std::endl;                 // Size of signed int: 4 bytes
  std::cout << "Size of unsigned int: " << sizeof(unsignedIntVar) << " bytes" << std::endl;             // Size of unsigned int: 4 bytes
  std::cout << "Size of signed short: " << sizeof(signedShortVar) << " bytes" << std::endl;             // Size of signed short: 2 bytes
  std::cout << "Size of unsigned short: " << sizeof(unsignedShortVar) << " bytes" << std::endl;         // Size of unsigned short: 2 bytes
  std::cout << "Size of signed long: " << sizeof(signedLongVar) << " bytes" << std::endl;               // Size of signed long: 8 bytes
  std::cout << "Size of unsigned long: " << sizeof(unsignedLongVar) << " bytes" << std::endl;           // Size of unsigned long: 8 bytes
  std::cout << "Size of signed long long: " << sizeof(signedLongLongVar) << " bytes" << std::endl;      // Size of signed long long: 8 bytes
  std::cout << "Size of unsigned long long: " << sizeof(unsignedLongLongVar) << " bytes" << std::endl;  // Size of unsigned long long: 8 bytes

  std::cout << "Size of float: " << sizeof(floatVar) << " bytes" << std::endl;    // Size of float: 4 bytes
  std::cout << "Size of double: " << sizeof(doubleVar) << " bytes" << std::endl;  // Size of double: 8 bytes
  std::cout << "Size of char: " << sizeof(charVar) << " bytes" << std::endl;      // Size of char: 1 byte
  std::cout << "Size of bool: " << sizeof(boolVar) << " bytes" << std::endl;      // Size of bool: 1 byte

  // Displaying values of the variables
  std::cout << "\nVariable values:" << std::endl;
  std::cout << "signedIntVar: " << signedIntVar << std::endl;           // signedIntVar: -10
  std::cout << "signedShortVar: " << signedShortVar << std::endl;       // signedShortVar: -20
  std::cout << "signedLongVar: " << signedLongVar << std::endl;         // signedLongVar: -30000
  std::cout << "signedLongLongVar: " << signedLongLongVar << std::endl; // signedLongLongVar: -4000000

  std::cout << "unsignedIntVar: " << unsignedIntVar << std::endl;           // unsignedIntVar: 10
  std::cout << "unsignedShortVar: " << unsignedShortVar << std::endl;       // unsignedShortVar: 20
  std::cout << "unsignedLongVar: " << unsignedLongVar << std::endl;         // unsignedLongVar: 30000
  std::cout << "unsignedLongLongVar: " << unsignedLongLongVar << std::endl; // unsignedLongLongVar: 4000000

  std::cout << "floatVar: " << floatVar << std::endl;    // floatVar: 10.5
  std::cout << "doubleVar: " << doubleVar << std::endl;  // doubleVar: 20.99
  std::cout << "charVar: " << charVar << std::endl;      // charVar: A
  std::cout << "boolVar: " << boolVar << std::endl;      // boolVar: 1 (true)

  // string (objects that represent a sequence of text)
  std::string name = "Tr4shP4ndu";
  std::string day = "Friday";
  std::string food = "Pizza";
  std::string address = "123 Fake St.";

  std::cout << "This is your Name: " << name << '\n';       // This is your Name: Tr4shP4ndu
  std::cout << "This is the Day: " << day << '\n';          // This is the Day: Friday
  std::cout << "This is the Food: " << food << '\n';        // This is the Food: Pizza
  std::cout << "This is the Address: " << address << '\n';  // This is the Address: 123 Fake St.

  return 0;
}
