// guessing-game — guess a hidden number; the program says higher or lower.
// Concepts: <random>, while-loops, conditionals, std::cin (lessons 05, 07, 14).
// Pass a seed for a reproducible game:  guessing-game 42
#include <iostream>
#include <random>
#include <string>

int main(int argc, char** argv) {
    std::random_device rd;
    unsigned seed = (argc > 1) ? static_cast<unsigned>(std::stoul(argv[1])) : rd();
    std::mt19937 gen(seed);
    const int secret = std::uniform_int_distribution<int>(1, 100)(gen);

    std::cout << "I'm thinking of a number from 1 to 100. Guess it!\n";
    int guess = 0, tries = 0;
    while (std::cin >> guess) {
        ++tries;
        if (guess < secret) {
            std::cout << "Higher.\n";
        } else if (guess > secret) {
            std::cout << "Lower.\n";
        } else {
            std::cout << "Got it in " << tries << " tries! It was " << secret << ".\n";
            return 0;
        }
    }
    std::cout << "\nBye (the number was " << secret << ").\n";
    return 0;
}
