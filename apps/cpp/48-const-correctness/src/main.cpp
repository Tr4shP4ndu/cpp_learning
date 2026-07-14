// 48-const-correctness — const member functions and why they matter.
#include <iostream>

class Account {
public:
    explicit Account(double balance) : balance_(balance) {}

    // A const member function promises NOT to modify the object. It can be
    // called on a const Account; a non-const method cannot.
    double balance() const { return balance_; }

    // Non-const: it mutates, so it may only be called on a non-const object.
    void deposit(double amount) { balance_ += amount; }

private:
    double balance_;
};

// Taking the object by const& means only its const methods are callable here —
// the compiler guarantees this function can't secretly change the account.
void print_balance(const Account& a) {
    std::cout << "balance = " << a.balance() << '\n';  // ok: balance() is const
    // a.deposit(10);  // would NOT compile: deposit() is non-const
}

int main() {
    Account acc(100.0);
    acc.deposit(50.0);        // fine: acc is non-const
    print_balance(acc);       // passed as const&

    const Account frozen(999.0);
    std::cout << "frozen: " << frozen.balance() << '\n';  // only const methods
    // frozen.deposit(1.0);   // would NOT compile: frozen is const
    return 0;
}
