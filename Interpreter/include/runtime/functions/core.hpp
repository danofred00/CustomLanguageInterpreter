/**
 * @file runtime/functions/core.hpp
 * Contains some core functions
 */

#pragma

#include <vector>
#include <runtime/types.hpp>

/**
 * This is use to check the type of a value
 * EX: type(1) = number; type('str') = str
 */
RuntimeValue * typeFunction(std::vector<RuntimeValue *> & args);