#pragma once
#include <string>
#include <filesystem>

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
    
    // Meta-scope processing pipeline
    std::string process_scope(const std::string& meta_scope_content, const Options& options);
    
    // Pipeline step functions
    std::string meta_to_cpptha_repr(const std::string& content);
    std::string generate_shared_lib_source(const std::string& cpptha_repr);
    std::filesystem::path create_shared_lib_folder_and_source(const std::string& source_code);
    bool compile_shared_library(const std::filesystem::path& source_dir, 
                               const std::filesystem::path& output_lib);
    std::string load_and_execute_defacto_string(const std::filesystem::path& lib_file);
}