// greeter.hpp — the INTERFACE: what exists, not how it works.
#pragma once  // include guard: ensures this header is pasted in only once
              // per translation unit, even if included multiple times.

#include <string>

// A DECLARATION: signature only, no body. Callers include this header to learn
// the function exists; the body lives in greeter.cpp.
std::string greet(const std::string& name);
