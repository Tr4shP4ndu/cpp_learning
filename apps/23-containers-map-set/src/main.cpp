// 23-containers-map-set — key/value lookup (map) and unique membership (set).
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <string>

int main() {
    // std::map<Key, Value>: sorted by key, lookup ~O(log n).
    std::map<std::string, int> ages;
    ages["Ada"] = 36;
    ages["Alan"] = 41;
    ages["Grace"] = 45;

    // find() returns an iterator; == end() means "not present".
    if (auto it = ages.find("Ada"); it != ages.end()) {
        std::cout << "Ada is " << it->second << "\n";  // ->first key, ->second value
    }

    // Iterating a map yields keys in sorted order.
    for (const auto& [name, age] : ages) {
        std::cout << name << " => " << age << "\n";
    }

    // std::unordered_map: hash table, average O(1) lookup, no ordering.
    std::unordered_map<std::string, int> counts;
    ++counts["apple"];
    ++counts["apple"];
    std::cout << "apple count = " << counts["apple"] << "\n";

    // std::set: a sorted collection of unique values.
    std::set<int> seen{3, 1, 2, 3, 1};   // duplicates dropped
    std::cout << "set size = " << seen.size() << " (";
    for (int v : seen) std::cout << v << " ";   // prints 1 2 3, sorted
    std::cout << ")\n";
    return 0;
}
