// calc — evaluate an arithmetic expression (+ - * / and parentheses).
// A recursive-descent parser: the grammar's structure IS the call structure.
// Concepts: recursion, string parsing, exceptions/error handling (lessons 30, 20, 61).
// Usage:  calc "1 + 2 * (3 - 4)"     (or pipe an expression on stdin)
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

struct Parser {
    const std::string& s;
    std::size_t i = 0;
    explicit Parser(const std::string& str) : s(str) {}

    void skip() { while (i < s.size() && std::isspace((unsigned char)s[i])) ++i; }
    bool eat(char c) { skip(); if (i < s.size() && s[i] == c) { ++i; return true; } return false; }

    // expr := term (('+' | '-') term)*
    double expr() {
        double v = term();
        for (;;) {
            if (eat('+'))      v += term();
            else if (eat('-')) v -= term();
            else return v;
        }
    }
    // term := factor (('*' | '/') factor)*
    double term() {
        double v = factor();
        for (;;) {
            if (eat('*')) v *= factor();
            else if (eat('/')) {
                double d = factor();
                if (d == 0) throw std::runtime_error("division by zero");
                v /= d;
            } else return v;
        }
    }
    // factor := number | '(' expr ')' | '-' factor
    double factor() {
        skip();
        if (eat('(')) {
            double v = expr();
            if (!eat(')')) throw std::runtime_error("expected ')'");
            return v;
        }
        if (eat('-')) return -factor();
        std::size_t start = i;
        while (i < s.size() && (std::isdigit((unsigned char)s[i]) || s[i] == '.')) ++i;
        if (i == start) throw std::runtime_error("expected a number");
        return std::stod(s.substr(start, i - start));
    }
};

int main(int argc, char** argv) {
    std::string input;
    if (argc > 1) {
        for (int k = 1; k < argc; ++k) {   // join args so `calc 1 + 2` also works
            if (k > 1) input += ' ';
            input += argv[k];
        }
    } else {
        std::getline(std::cin, input);      // or pipe it in (avoids shell quoting)
    }

    try {
        Parser p(input);
        double result = p.expr();
        p.skip();
        if (p.i != input.size()) throw std::runtime_error("unexpected trailing input");
        std::cout << result << '\n';
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
