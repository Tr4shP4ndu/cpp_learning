// 48-move-semantics — transfer resources instead of copying them.
#include <iostream>
#include <vector>
#include <string>
#include <utility>   // std::move

// A type that owns a big resource (a vector). We log copy vs move so you can
// see which one happens.
class Widget {
public:
    explicit Widget(std::string name, std::size_t n)
        : name_(std::move(name)), data_(n, 0) {}

    // Copy constructor: DUPLICATE the data (expensive for large data).
    Widget(const Widget& other)
        : name_(other.name_), data_(other.data_) {
        std::cout << "  copy  " << name_ << " (" << data_.size() << " elems)\n";
    }

    // Move constructor: STEAL the data from a soon-to-die object (cheap:
    // just pointer swaps). `noexcept` lets containers use it safely.
    Widget(Widget&& other) noexcept
        : name_(std::move(other.name_)), data_(std::move(other.data_)) {
        std::cout << "  move  " << name_ << " (stole the buffer)\n";
    }

    std::size_t size() const { return data_.size(); }

private:
    std::string name_;
    std::vector<int> data_;
};

int main() {
    Widget a("A", 1000);

    std::cout << "copy-construct b from a:\n";
    Widget b = a;                 // lvalue -> copy constructor

    std::cout << "move-construct c from a:\n";
    Widget c = std::move(a);      // std::move casts a to rvalue -> move ctor
    // `a` is now in a valid-but-unspecified state; don't rely on its contents.

    std::cout << "b.size()=" << b.size() << ", c.size()=" << c.size() << "\n";
    return 0;
}
