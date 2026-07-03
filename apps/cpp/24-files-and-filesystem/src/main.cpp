// 24-files-and-filesystem — write a file, read it back, inspect its path.
#include <iostream>
#include <fstream>     // std::ofstream / std::ifstream — file streams
#include <filesystem>  // std::filesystem::path (C++17)
#include <string>

namespace fs = std::filesystem;

int main() {
    // Build a path in the OS temp directory. path handles separators for you.
    fs::path file = fs::temp_directory_path() / "cpp_learning_demo.txt";

    // Writing: an ofstream is just like std::cout, but aimed at a file.
    {
        std::ofstream out(file);          // opens (and creates) the file
        if (!out) { std::cerr << "cannot open for writing\n"; return 1; }
        out << "line one\n" << "line two\n";
    }   // the stream closes automatically here (RAII) — file is flushed & saved

    // Reading: an ifstream reads back line by line.
    std::cout << "reading " << file << ":\n";
    {
        std::ifstream in(file);
        std::string line;
        while (std::getline(in, line)) {
            std::cout << "  " << line << "\n";
        }
    }

    // Filesystem queries: size, existence, and path pieces.
    std::cout << "exists   : " << fs::exists(file) << "\n";
    std::cout << "size     : " << fs::file_size(file) << " bytes\n";
    std::cout << "filename : " << file.filename().string() << "\n";

    fs::remove(file);   // clean up
    return 0;
}
