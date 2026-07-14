// animals.hpp — a derived class REUSES and EXTENDS a base class.
#pragma once
#include <string>

// Base class: the common part.
class Animal {
public:
    explicit Animal(std::string name);
    const std::string& name() const;
    void eat() const;              // shared behavior all animals inherit

protected:
    // `protected` = accessible to this class AND its subclasses, but not the
    // outside world.
    std::string name_;
};

// Dog "is-a" Animal: it inherits name_/name()/eat() and adds its own.
class Dog : public Animal {
public:
    explicit Dog(std::string name);
    void bark() const;             // behavior specific to Dog
};
