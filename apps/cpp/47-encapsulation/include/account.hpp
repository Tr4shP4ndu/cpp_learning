// account.hpp — hide data behind a controlled public interface.
#pragma once

class Account {
public:
    explicit Account(double initial);

    // Public interface: the ONLY way to change the balance. These methods
    // enforce the class invariant "balance is never negative".
    bool deposit(double amount);
    bool withdraw(double amount);
    double balance() const;   // read-only accessor (getter)

private:
    // Private data: unreachable from outside, so no code can set a bad balance.
    double balance_;
};
