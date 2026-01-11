#include <iostream>
#include <vector>

// Using typedef to create aliases for existing data types
// typedef std::vector<std::pair<std::string, int>> pairlist_t;
typedef std::string text_t;
typedef int number_t;

int main() {
    /*
      Typedef:
      - Reserved keyword used to create an alias for another data type.
      - Creates a new identifier for an existing type.
      - Enhances readability and reduces typos.
      - Useful when there is a clear benefit, such as long or complex types.
      - In modern C++, 'using' is preferred over 'typedef', especially with templates.
    */

    //pairlist_t pairlist;  // Example of a typedef alias for a complex type
    text_t firstName = "Tr4shP4ndu";
    number_t age = 21;

    std::cout << firstName << '\n';
    std::cout << age << '\n';

    return 0;
}
