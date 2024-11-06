/**
 * @file include/runtime/functions/io.hpp
 * This file contains the declaration of the input/output functions
 */

#pragma once

#include <runtime/types.hpp>

/**
 * Print function that take multiples arguments and print them
 */
RuntimeValue* printFunction(const std::vector<RuntimeValue*>& args);

/**
 * Read function that read a line from the standard input
 */
RuntimeValue* readFunction(const std::vector<RuntimeValue*>& args);