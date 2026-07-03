#include "animals.hpp"
#include <iostream>

Animal::Animal(std::string name) : name_(std::move(name)) {}

const std::string& Animal::name() const { return name_; }

void Animal::eat() const {
    std::cout << name_ << " is eating\n";
}

// A Dog constructor forwards the name to the Animal base constructor.
Dog::Dog(std::string name) : Animal(std::move(name)) {}

void Dog::bark() const {
    // A Dog can use name_ because it's protected in the base.
    std::cout << name_ << " says woof!\n";
}
