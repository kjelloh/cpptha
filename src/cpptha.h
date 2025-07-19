#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define CPPTHA_EXPORT __declspec(dllexport)
#else
  #define CPPTHA_EXPORT
#endif

typedef struct {
    bool verbose;
    bool preprocess_only;
    bool keep_files;
    int max_iterations;
} cpptha_config_t;
CPPTHA_EXPORT int cpptha_process_file(const char* input_file, const char* output_file, const cpptha_config_t* config);
CPPTHA_EXPORT void cpptha_print_build_info();

