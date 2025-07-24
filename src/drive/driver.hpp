#pragma once
#include <string>
#include <filesystem>
#include "MehWorkspace.hpp"

namespace cpptha {
    struct Options {
        bool verbose;
        bool preprocess_only;
        bool keep_files;
        int max_iterations;
        
        Options() : verbose(false), preprocess_only(false), keep_files(false), max_iterations(100) {}
    };
    
    int process_file(const std::string& input_file, 
                     const std::string& output_file, 
                     const Options& options);
    
    // Meta-scope processing pipeline
    std::string process_scope(const std::string& scope_tha_code, const Options& options);
    
    // Pipeline step functions
    std::string tha_to_meh_model_repr(const std::string& content);
    std::string generate_mehtotha_lib_source(const std::string& meh_model_repr);
    MehWorkspace create_shared_lib_folder_and_source(const std::string& source_code, const Options& options);
    bool compile_shared_library(const MehWorkspace& workspace);
    std::string load_and_execute_shared_lib(const MehWorkspace& workspace);
}