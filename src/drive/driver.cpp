#include "driver.hpp"
#include "../parse/meta_parser.hpp"
#include "../parse/meta_to_cpp_parse.hpp"
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
        
        try {
            // Read input file
            std::ifstream file(input_file);
            if (!file) {
                std::cerr << "Error: Cannot open input file: " << input_file << std::endl;
                return 1;
            }
            
            std::ostringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();
            file.close();
            
            // Parse meta-scopes
            parse::MetaParser parser;
            auto parse_result = parser.parse_content(content);
            
            if (!parse_result) {
                std::cerr << "Error: Failed to parse meta-scopes: " << parser.get_last_error() << std::endl;
                return 2;
            }
            
            std::string transformed_content = content;
            
            if (options.verbose) {
                std::cout << "Found " << parse_result->meta_scopes.size() << " meta-scope(s)" << std::endl;
            }
            
            // Process all meta-scopes (from last to first to preserve positions)
            for (int i = parse_result->meta_scopes.size() - 1; i >= 0; --i) {
                if (options.verbose) {
                    std::cout << "Processing meta-scope " << (i + 1) << "..." << std::endl;
                }
                
                // Get the complete meta-scope as string
                std::string meta_scope_str(parse_result->meta_scopes[i]);
                
                // Process the complete meta-scope
                std::string replacement = process_scope(meta_scope_str, options);
                
                // Replace the meta-scope with the processed result
                auto range = parse_result->get_scope_range(i);
                transformed_content = transformed_content.substr(0, range.first) + replacement + transformed_content.substr(range.second);
            }
            
            // Write output file (or stdout if no output file specified)
            if (output_file.empty()) {
                // No output file specified, write to stdout
                std::cout << transformed_content;
            } else {
                std::ofstream output(output_file);
                if (!output) {
                    std::cerr << "Error: Cannot create output file: " << output_file << std::endl;
                    return 3;
                }
                
                output << transformed_content;
                output.close();
            }
            
            if (options.verbose) {
                std::cout << "Processing complete. Output written to: " << output_file << std::endl;
            }
            
            return 0;
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 4;
        }
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
        // Parse the meta-content using our new parser
        parse::MetaToCppParser parser;
        auto parse_result = parser.parse(content);
        
        if (!parse_result.success) {
            std::cerr << "Error parsing meta-content: " << parse_result.error_message << std::endl;
            return content; // fallback to original content
        }
        
        // Transform parsed constructs to cpptha representation
        std::ostringstream result;
        result << "meta_tha env{};\n";
        
        for (const auto& construct : parse_result.constructs) {
            if (construct.keyword == "struct") {
                if (construct.identifier) {
                    result << "env += struct_tha(\"" << construct.identifier.value() 
                           << "\", \"" << construct.body << "\");\n";
                }
            } else if (construct.keyword == "meta") {
                // Meta constructs are processed recursively
                result << meta_to_cpptha_repr(construct.body);
            }
        }
        
        return result.str();
    }
    
    std::string generate_shared_lib_source(const std::string& cpptha_repr) {
        std::ostringstream source;
        size_t content_size = cpptha_repr.size();
        
        source << "// Generated C++/C hybrid shared library source\n";
        source << "// Exposes C API, implements in C++\n";
        source << "// Content size: " << content_size << " bytes\n";
        source << "\n";
        
        // C API Declaration
        source << "extern \"C\" {\n";
        source << "    const char* defacto_string();\n";
        source << "}\n";
        source << "\n";
        
        // C++ Implementation using raw string literal
        source << "// C++ implementation using raw string literal\n";
        source << "static const char* cpptha_content = R\"CPPTHA_DELIMITER(\n";
        source << cpptha_repr;  // Raw content, no escaping needed!
        source << ")CPPTHA_DELIMITER\";\n";
        source << "\n";
        source << "const char* defacto_string() {\n";
        source << "    return cpptha_content;\n";
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