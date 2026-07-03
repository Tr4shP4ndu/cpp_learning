// 50-ranges-and-algorithms — compose operations over sequences (C++20 ranges).
#include <iostream>
#include <vector>
#include <algorithm>   // std::ranges::sort, std::ranges::count_if
#include <ranges>      // std::views::filter, transform, take

int main() {
    std::vector<int> nums{5, 3, 8, 1, 9, 2, 7};

    // Classic algorithms have a ranges version that takes the whole container
    // directly — no begin()/end() pair.
    std::ranges::sort(nums);
    std::cout << "sorted: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    auto evens = std::ranges::count_if(nums, [](int n) { return n % 2 == 0; });
    std::cout << "evens: " << evens << "\n";

    // Views are LAZY, composable pipelines. Nothing is computed until iterated,
    // and no intermediate containers are built.
    auto pipeline = nums
        | std::views::filter([](int n) { return n % 2 == 1; })  // keep odds
        | std::views::transform([](int n) { return n * 10; })   // scale
        | std::views::take(2);                                  // first two

    std::cout << "odd*10, first two: ";
    for (int n : pipeline) std::cout << n << " ";
    std::cout << "\n";
    return 0;
}
