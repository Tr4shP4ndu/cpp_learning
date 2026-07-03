#include "account.hpp"

Account::Account(double initial)
    : balance_(initial < 0 ? 0 : initial) {}   // guard the invariant at creation

bool Account::deposit(double amount) {
    if (amount <= 0) return false;              // reject invalid input
    balance_ += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || amount > balance_) return false;  // can't overdraw
    balance_ -= amount;
    return true;
}

double Account::balance() const {
    return balance_;
}
