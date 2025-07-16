#include "cpptha.h"
#include "test/test_runner.hpp"
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
    bool run_tests = false;
    bool keep_test_files = false;
    bool preprocess_only = false;
    
    // Test flag - overrides everything else
    app.add_flag("--test", run_tests, "Run all tests (ignores other arguments)");
    app.add_flag("-k,--keep-test-files", keep_test_files, "Preserve test files after running tests");
    
    // Build info flag - overrides everything else
    app.add_flag("--build-info", show_build_info, "Show build and compiler information (ignores other arguments)");
    
    // Regular file processing arguments
    app.add_option("file", input_file, "C++ source/header file to process")
        ->check(CLI::ExistingFile);
    
    app.add_flag("-v,--verbose", verbose, "Enable verbose output");
    app.add_flag("-E", preprocess_only, "Preprocess only (do not compile)");
    
    CLI11_PARSE(app, argc, argv);
    
    // Test mode - exit early after running tests
    if (run_tests) {
        return tests::run_all(keep_test_files) ? 0 : 1;
    }
    
    // Build info mode - exit early after showing information
    if (show_build_info) {
        cpptha_print_build_info();        
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
    
    cpptha_config_t config = {0};
    config.verbose = verbose;
    config.preprocess_only = preprocess_only;
    config.max_iterations = 100;
    
    int result = cpptha_process_file(input_file.c_str(), nullptr, &config);
    if (result != 0) {
        std::cerr << "Error: Failed to process file: " << input_file << std::endl;
        return result;
    }
    
    return 0;
}
