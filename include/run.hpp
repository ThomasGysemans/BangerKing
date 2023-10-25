#pragma once

#include <string>

// forward declaration to avoid circular dependency.
// it's possible because we're only using a pointer to an instance of "Context"
class Context;

void run(const std::string&, const std::string&, const Context* ctx);
