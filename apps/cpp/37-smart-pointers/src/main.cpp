// 37-smart-pointers — pointers that free their memory automatically (RAII).
#include <iostream>
#include <memory>   // std::unique_ptr, std::shared_ptr, make_unique/make_shared
#include <utility>  // std::move

// unique_ptr owns its object alone. When it goes out of scope, the object is
// deleted automatically — no manual `delete`, no leaks.
std::unique_ptr<int> makeAnswer() {
    return std::make_unique<int>(42);  // preferred over `new`
}

int main() {
    // --- unique_ptr: single owner ---
    std::unique_ptr<int> a = makeAnswer();
    std::cout << "*a = " << *a << "\n";   // use like a normal pointer

    // Can't copy a unique_ptr (that would mean two owners). You MOVE ownership.
    std::unique_ptr<int> b = std::move(a);
    std::cout << "moved: a is " << (a ? "set" : "null") << ", *b = " << *b << "\n";
    // `b` is destroyed at end of main -> the int is freed automatically.

    // --- shared_ptr: multiple owners, reference-counted ---
    auto s1 = std::make_shared<int>(7);
    auto s2 = s1;   // both share ownership; use_count() == 2
    std::cout << "shared value = " << *s2
              << ", owners = " << s1.use_count() << "\n";
    // The int is freed only when the LAST shared_ptr is gone.

    return 0;
}
