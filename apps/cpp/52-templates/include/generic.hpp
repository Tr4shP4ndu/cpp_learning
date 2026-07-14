// generic.hpp — write code once that works for MANY types (templates).
#pragma once

// A function template. `T` is a placeholder the compiler fills in per call:
// maximum(3, 4) -> T=int; maximum(1.5, 0.5) -> T=double. One source, many
// concrete functions generated on demand.
template <typename T>
T maximum(const T& a, const T& b) {
    return (a > b) ? a : b;
}

// A class template. Box<int> and Box<std::string> are distinct types built from
// the same blueprint.
template <typename T>
class Box {
public:
    explicit Box(T value) : value_(value) {}
    const T& get() const { return value_; }
    void set(const T& v) { value_ = v; }
private:
    T value_;
};

// Templates are DEFINED in the header: the compiler needs the full definition
// at each point of use to generate the concrete version. (That's why there's
// no generic.cpp.)
