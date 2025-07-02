#include "cpptha.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    CLI::App app{"cpptha - C++ meta-code preprocessor"};
    
    std::string input_file;
    bool verbose = false;
    bool show_build_info = false;
    
    // Build info flag - overrides everything else
    app.add_flag("--build-info", show_build_info, "Show build and compiler information (ignores other arguments)");
    
    // Regular file processing arguments
    app.add_option("file", input_file, "C++ source/header file to process")
        ->check(CLI::ExistingFile);
    
    app.add_flag("-v,--verbose", verbose, "Enable verbose output");
    
    CLI11_PARSE(app, argc, argv);
    
    // Build info mode - exit early after showing information
    if (show_build_info) {
        cpptha_print_build_info();
        
        std::vector<std::string> packages;
        packages.push_back("CLI11");
        cpptha_print_package_info(packages);
        
        return 0;  // Exit without processing any file
    }
    
    // Regular file processing mode - require input file
    if (input_file.empty()) {
        std::cerr << "Error: Input file is required when not using --build-info" << std::endl;
        std::cout << app.help() << std::endl;
        return 1;
    }
    
    if (verbose) {
        std::cout << "Processing file: " << input_file << std::endl;
        std::cout << "File size: " << std::filesystem::file_size(input_file) << " bytes" << std::endl;
    }
    
    std::cout << "Input file: " << input_file << std::endl;
    
    return 0;
}
