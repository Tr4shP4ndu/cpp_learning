#include "buffer.hpp"
#include <iostream>
#include <stdexcept>

// Constructor. The `: size_(size), data_(new int[size])` part is the MEMBER
// INITIALIZER LIST — it initializes members before the body runs.
Buffer::Buffer(std::size_t size)
    : size_(size), data_(new int[size]{}) {   // {} value-initializes to zero
    std::cout << "Buffer(" << size_ << ") acquired\n";
}

// Destructor runs automatically when the object's lifetime ends.
Buffer::~Buffer() {
    delete[] data_;
    std::cout << "Buffer(" << size_ << ") released\n";
}

void Buffer::set(std::size_t i, int value) {
    if (i >= size_) throw std::out_of_range("Buffer::set");
    data_[i] = value;
}

int Buffer::get(std::size_t i) const {
    if (i >= size_) throw std::out_of_range("Buffer::get");
    return data_[i];
}
