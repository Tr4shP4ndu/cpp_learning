#include <iostream>
#include "account.hpp"

int main() {
    Account acct(100.0);
    std::cout << "start:    " << acct.balance() << "\n";

    acct.deposit(50.0);
    std::cout << "deposit:  " << acct.balance() << "\n";   // 150

    bool ok = acct.withdraw(1000.0);   // rejected: would overdraw
    std::cout << "overdraw attempt succeeded? " << ok
              << ", balance still " << acct.balance() << "\n";

    // acct.balance_ = -999;  // COMPILE ERROR: balance_ is private — that's the
    //                        // point. No outside code can break the invariant.
    return 0;
}
