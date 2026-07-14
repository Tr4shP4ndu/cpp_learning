// 51-casting — the four named C++ casts, each saying what you mean.
#include <iostream>

struct Animal {
    virtual ~Animal() = default;
    virtual const char* name() const { return "animal"; }
};
struct Dog : Animal {
    const char* name() const override { return "dog"; }
    void bark() const { std::cout << "woof\n"; }
};

int main() {
    // static_cast: well-defined, compile-time-checked conversions.
    double pi = 3.99;
    int truncated = static_cast<int>(pi);   // 3 — explicit, searchable
    std::cout << "static_cast<int>(3.99) = " << truncated << '\n';

    // dynamic_cast: safe downcast in a polymorphic hierarchy. Returns nullptr
    // when the object is not actually the target type.
    Animal* a = new Dog;
    if (Dog* d = dynamic_cast<Dog*>(a)) d->bark();       // succeeds
    Animal base;
    if (dynamic_cast<Dog*>(&base) == nullptr)
        std::cout << "base is not a Dog (dynamic_cast returned nullptr)\n";

    // const_cast: add or strip const. Only legal to write through it if the
    // object was not originally const.
    const int locked = 42;
    const int* cp = &locked;
    std::cout << "const_cast reads: " << *const_cast<int*>(cp) << '\n';

    // reinterpret_cast: reinterpret the bits. Powerful and dangerous — here,
    // inspect an int's address as raw bytes. Rarely what you want.
    int value = 0x41;
    auto* bytes = reinterpret_cast<unsigned char*>(&value);
    std::cout << "first byte of value = " << static_cast<int>(bytes[0]) << '\n';

    delete a;
    return 0;
}
