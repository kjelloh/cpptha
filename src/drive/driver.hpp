#pragma once
#include <string>

namespace cpptha {
    struct Options {
        bool verbose;
        bool preprocess_only;
        int max_iterations;
        
        Options() : verbose(false), preprocess_only(false), max_iterations(100) {}
    };
    
    int process_file(const std::string& input_file, 
                     const std::string& output_file, 
                     const Options& options);
}