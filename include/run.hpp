#pragma once

#include <string>

// forward declaration to avoid circular dependency.
// it's possible because I'm only using pointers and not complete types
class Context;

class RuntimeResult;

std::unique_ptr<const RuntimeResult> run(const std::string&, const std::string&, std::shared_ptr<Context>);
