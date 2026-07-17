// cli-todo — a tiny to-do list persisted to a text file.
// Concepts: argv, fstream file I/O, std::vector, std::string (lessons 20-24).
// Usage:  cli-todo add "buy milk"   |   cli-todo list   |   cli-todo done 1
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// The list lives under build/ (git-ignored) so the demo leaves no stray files.
static const char* kFile = "build/todo.txt";

static std::vector<std::string> load() {
    std::vector<std::string> items;
    std::ifstream in(kFile);
    std::string line;
    while (std::getline(in, line))
        if (!line.empty()) items.push_back(line);
    return items;
}

static void save(const std::vector<std::string>& items) {
    std::ofstream out(kFile);
    for (const std::string& i : items) out << i << '\n';
}

int main(int argc, char** argv) {
    std::vector<std::string> items = load();
    std::string cmd = (argc > 1) ? argv[1] : "list";

    if (cmd == "add" && argc > 2) {
        items.emplace_back(argv[2]);
        save(items);
        std::cout << "Added: " << argv[2] << '\n';
    } else if (cmd == "done" && argc > 2) {
        std::size_t n = std::stoul(argv[2]);
        if (n >= 1 && n <= items.size()) {
            std::cout << "Done: " << items[n - 1] << '\n';
            items.erase(items.begin() + static_cast<long>(n - 1));
            save(items);
        } else {
            std::cout << "No item " << n << '\n';
        }
    } else if (cmd == "list") {
        if (items.empty()) std::cout << "(no todos)\n";
        for (std::size_t i = 0; i < items.size(); ++i)
            std::cout << (i + 1) << ". " << items[i] << '\n';
    } else {
        std::cout << "usage: cli-todo [ add \"text\" | list | done N ]\n";
    }
    return 0;
}
