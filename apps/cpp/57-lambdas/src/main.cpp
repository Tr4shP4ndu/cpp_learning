// 55-lambdas — define a small function inline, right where it's used.
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // A lambda is an anonymous function object.
    //   [captures](params) { body }
    auto square = [](int x) { return x * x; };
    std::cout << "square(5) = " << square(5) << "\n";

    // Capture by value [=] copies outer variables; by reference [&] refers to
    // them. Here we capture one variable explicitly by value.
    int threshold = 3;
    auto over = [threshold](int x) { return x > threshold; };

    std::vector<int> nums{1, 2, 3, 4, 5};

    // Lambdas shine as arguments to standard algorithms.
    auto count = std::count_if(nums.begin(), nums.end(), over);
    std::cout << "count > " << threshold << " : " << count << "\n";   // 2

    // Sort descending with an inline comparator.
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a > b; });
    std::cout << "sorted desc: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    // Capture by reference to accumulate into an outer variable.
    int total = 0;
    std::for_each(nums.begin(), nums.end(), [&total](int x) { total += x; });
    std::cout << "total = " << total << "\n";   // 15
    return 0;
}
