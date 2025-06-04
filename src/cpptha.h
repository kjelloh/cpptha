#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define CPPTHA_EXPORT __declspec(dllexport)
#else
  #define CPPTHA_EXPORT
#endif

CPPTHA_EXPORT void cpptha();
CPPTHA_EXPORT void cpptha_print_vector(const std::vector<std::string> &strings);
