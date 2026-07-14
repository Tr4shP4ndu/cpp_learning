// 57-variant-tuple — fixed heterogeneous bundles and type-safe unions (C++17).
#include <iostream>
#include <string>
#include <tuple>
#include <variant>

int main() {
    // std::tuple: a fixed-size bundle of values of different types.
    std::tuple<int, std::string, double> row{1, "alice", 9.5};

    // Structured bindings unpack it into named variables.
    auto [id, name, score] = row;
    std::cout << "row: " << id << ' ' << name << ' ' << score << '\n';

    // std::get accesses by index (or by type, if unambiguous).
    std::cout << "name via get<1>: " << std::get<1>(row) << '\n';

    // std::variant: holds exactly one of a fixed set of types, and knows which.
    std::variant<int, std::string> value = 42;
    std::cout << "holds int? " << std::holds_alternative<int>(value) << '\n';

    value = std::string{"now a string"};

    // std::visit dispatches on whatever the variant currently holds.
    std::visit([](const auto& v) { std::cout << "visit: " << v << '\n'; }, value);
    return 0;
}
