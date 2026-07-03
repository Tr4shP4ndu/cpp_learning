// buffer.hpp — a class that OWNS a resource: memory acquired in the
// constructor and released in the destructor. This is RAII.
#pragma once
#include <cstddef>

class Buffer {
public:
    explicit Buffer(std::size_t size);  // constructor: acquire the resource
    ~Buffer();                          // destructor: release it

    // This object owns raw memory, so a naive copy would double-free.
    // For this lesson we forbid copying (rule of three/five, covered later).
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    std::size_t size() const { return size_; }
    void set(std::size_t i, int value);
    int  get(std::size_t i) const;

private:
    std::size_t size_;
    int* data_;   // owned; freed in the destructor
};
