#include <iostream>

namespace first {
    int x = 1;
}

namespace second {
    int x = 2;
}

using std::cout;
using std::string;

int main() {
    // using namespace first;

    /*
      Namespace:
      - Provides a solution for preventing name conflicts in large projects.
      - Allows identically named entities to coexist as long as they are in different namespaces.
      - Each entity needs a unique name within its namespace.
    */

    int x = 0;

    string name = "Bro!";
    cout << "Hello " << name << '\n';
    cout << "This is Value X without namespace: " << x << '\n'; // Output is 0 since it's using the local x
    cout << "This is Value X with namespace first: " << first::x << '\n'; // Output is 1, using first namespace
    cout << "This is Value X with namespace second: " << second::x << '\n'; // Output is 2, using second namespace

    return 0;
}
