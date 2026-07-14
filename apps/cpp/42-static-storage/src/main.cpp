// 42-static-storage — the `static` keyword and where variables live.
#include <iostream>

// A static LOCAL is initialized once and persists across calls — it lives in
// static storage, not on the stack. Perfect for a call counter.
int next_id() {
    static int counter = 0;   // initialized once; survives between calls
    return ++counter;
}

struct Widget {
    // A static MEMBER is shared by every instance: one variable for the class,
    // not one per object.
    static int living;
    Widget()  { ++living; }
    ~Widget() { --living; }
};
int Widget::living = 0;   // a static member needs one definition out of line

int main() {
    std::cout << next_id() << ' ' << next_id() << ' ' << next_id() << '\n'; // 1 2 3

    int stack_var = 5;            // automatic storage: lives on the stack
    int* heap_var = new int(9);   // dynamic storage: lives on the heap (lesson 40)
    std::cout << "stack=" << stack_var << " heap=" << *heap_var << '\n';
    delete heap_var;

    {
        Widget a, b;
        std::cout << "living widgets = " << Widget::living << '\n';  // 2
    }
    std::cout << "after scope, living = " << Widget::living << '\n'; // 0
    return 0;
}
