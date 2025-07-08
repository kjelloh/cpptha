#include "driver.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <dlfcn.h>

namespace cpptha {
    
    int process_file(const std::string& input_file, 
                     const std::string& output_file, 
                     const Options& options) {
        
        if (options.verbose) {
            std::cout << "C++ Processing: " << input_file << std::endl;
            std::cout << "Preprocess only: " << (options.preprocess_only ? "yes" : "no") << std::endl;
            std::cout << "Max iterations: " << options.max_iterations << std::endl;
        }
        
        // TODO: Implement meta-collapse processing architecture
        std::cout << "cpptha::process_file: Meta-collapse architecture not yet implemented" << std::endl;
        
        return 0;
    }
    
    std::string process_scope(const std::string& meta_scope_content, const Options& options) {
        if (options.verbose) {
            std::cout << "Processing meta-scope: " << meta_scope_content.substr(0, 50) << "..." << std::endl;
        }
        
        // Step 1: Transform meta-scope to cpptha representation
        std::string cpptha_repr = meta_to_cpptha_repr(meta_scope_content);
        
        // Step 2: Generate C++ source code
        std::string source_code = generate_shared_lib_source(cpptha_repr);
        
        // Step 3: Create temp directory and write source files
        std::filesystem::path source_dir = create_shared_lib_folder_and_source(source_code);
        
        // Step 4: Compile to shared library
        std::filesystem::path lib_file = source_dir / "meta_transform.so";
        if (!compile_shared_library(source_dir, lib_file)) {
            throw std::runtime_error("Failed to compile shared library");
        }
        
        // Step 5: Load and execute defacto_string function
        std::string result = load_and_execute_defacto_string(lib_file);
        
        // Note: We don't clean up the build directory so user can inspect the files
        if (options.verbose) {
            std::cout << "Build files preserved in: " << source_dir << std::endl;
        }
        
        return result;
    }
    
    std::string meta_to_cpptha_repr(const std::string& content) {
        // For now, just return the content as-is
        // TODO: Implement actual meta-scope to cpptha representation transformation
        return content;
    }
    
    std::string generate_shared_lib_source(const std::string& cpptha_repr) {
        std::ostringstream source;
        source << "#include <string>\n";
        source << "\n";
        source << "extern \"C\" {\n";
        source << "    const char* defacto_string() {\n";
        source << "        // Generated from cpptha representation: " << cpptha_repr.substr(0, 30) << "...\n";
        source << "        static std::string result = \"" << cpptha_repr << "\";\n";
        source << "        return result.c_str();\n";
        source << "    }\n";
        source << "}\n";
        return source.str();
    }
    
    std::filesystem::path create_shared_lib_folder_and_source(const std::string& source_code) {
        // Create build directory in current working directory
        std::filesystem::path build_dir = std::filesystem::current_path() / "cpptha_build";
        std::filesystem::create_directories(build_dir);
        
        // Create unique meta-scope processing directory
        std::filesystem::path meta_dir = build_dir / ("meta_" + std::to_string(std::time(nullptr)));
        std::filesystem::create_directories(meta_dir);
        
        // Write source file
        std::filesystem::path source_file = meta_dir / "meta_transform.cpp";
        std::ofstream file(source_file);
        if (!file) {
            throw std::runtime_error("Failed to create source file: " + source_file.string());
        }
        file << source_code;
        file.close();
        
        return meta_dir;
    }
    
    bool compile_shared_library(const std::filesystem::path& source_dir, 
                               const std::filesystem::path& output_lib) {
        std::filesystem::path source_file = source_dir / "meta_transform.cpp";
        
        // Build compiler command
        std::ostringstream cmd;
        cmd << "g++ -shared -fPIC -std=c++17 -o " 
            << output_lib.string() << " " 
            << source_file.string();
        
        // Execute compiler
        int result = std::system(cmd.str().c_str());
        return result == 0;
    }
    
    std::string load_and_execute_defacto_string(const std::filesystem::path& lib_file) {
        // Load shared library
        void* handle = dlopen(lib_file.c_str(), RTLD_LAZY);
        if (!handle) {
            throw std::runtime_error("Failed to load shared library: " + std::string(dlerror()));
        }
        
        // Get function pointer
        typedef const char* (*defacto_string_func)();
        defacto_string_func defacto_string = (defacto_string_func) dlsym(handle, "defacto_string");
        
        if (!defacto_string) {
            dlclose(handle);
            throw std::runtime_error("Failed to find defacto_string function: " + std::string(dlerror()));
        }
        
        // Call function and get result
        const char* result_cstr = defacto_string();
        std::string result(result_cstr);
        
        // Clean up
        dlclose(handle);
        
        return result;
    }
    
}