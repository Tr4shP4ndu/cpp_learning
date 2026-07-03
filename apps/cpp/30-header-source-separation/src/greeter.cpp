// greeter.cpp — the IMPLEMENTATION: the single definition of greet().
#include "greeter.hpp"  // include our own header so declaration & definition match

std::string greet(const std::string& name) {
    return "Hello, " + name + "!";
}
