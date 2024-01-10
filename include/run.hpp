#pragma once

#include <string>

// forward declaration to avoid circular dependency.
// it's possible because we're only using pointers and not complete types
class Context;

class RuntimeResult;

const RuntimeResult* run(const std::string&, const std::string&, Context* ctx);
