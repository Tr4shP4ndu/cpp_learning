#include "animals.hpp"

int main() {
    Dog d("Rex");

    d.eat();   // inherited from Animal
    d.bark();  // Dog's own method

    return 0;
}
