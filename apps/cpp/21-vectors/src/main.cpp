// 22-vectors — std::vector: a resizable array that manages its own memory.
#include <iostream>
#include <vector>

int main() {
    // Unlike std::array, a vector's size can change at run time.
    std::vector<int> nums{10, 20, 30};

    nums.push_back(40);   // grow by one
    nums.push_back(50);

    std::cout << "size     : " << nums.size() << "\n";
    std::cout << "front    : " << nums.front() << "\n";
    std::cout << "back     : " << nums.back() << "\n";

    for (int n : nums) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    nums.pop_back();      // remove the last element
    std::cout << "after pop_back, size = " << nums.size() << "\n";

    // .at() is bounds-checked; [] is not.
    std::cout << "nums.at(1) = " << nums.at(1) << "\n";
    return 0;
}
