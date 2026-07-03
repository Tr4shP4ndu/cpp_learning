// shapes.hpp — one base-class pointer, many runtime behaviors (polymorphism).
#pragma once
#include <string>

// Abstract base class: it declares an interface but has no complete objects.
class Shape {
public:
    virtual ~Shape() = default;          // virtual destructor: REQUIRED for a
                                         // polymorphic base, so `delete base`
                                         // calls the derived destructor.

    virtual double area() const = 0;     // `= 0` -> pure virtual -> abstract.
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double radius);
    double area() const override;        // `override`: compiler checks it really
    std::string name() const override;   // overrides a base virtual.
private:
    double radius_;
};

class Square : public Shape {
public:
    explicit Square(double side);
    double area() const override;
    std::string name() const override;
private:
    double side_;
};
