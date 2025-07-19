#include "MehWorkspace.hpp"
#include <iostream>
#include <fstream>
#include <ctime>

namespace cpptha {

    MehWorkspace::MehWorkspace(const std::filesystem::path& working_dir) 
        : working_dir(working_dir) {
    }

    void MehWorkspace::setup_for_source(const std::string& source_code) {
        create_workspace_directory();
        copy_meh_library_files();
        write_source_file(source_code);
    }

    void MehWorkspace::create_workspace_directory() {
        // Create build directory in working directory
        std::filesystem::path build_dir = working_dir / "meh_build";
        std::filesystem::create_directories(build_dir);
        
        // Create unique meta-scope processing directory
        workspace_dir = build_dir / ("meh_" + std::to_string(std::time(nullptr)));
        std::filesystem::create_directories(workspace_dir);
    }

    void MehWorkspace::copy_meh_library_files() {
        // Get meh source directory relative to working directory
        std::filesystem::path meh_source_dir = working_dir / "src" / "meh";
        
        // Copy meh.hpp
        std::filesystem::copy_file(
            meh_source_dir / "meh.hpp",
            workspace_dir / "meh.hpp",
            std::filesystem::copy_options::overwrite_existing
        );
        
        // Copy meh.cpp
        std::filesystem::copy_file(
            meh_source_dir / "meh.cpp",
            workspace_dir / "meh.cpp", 
            std::filesystem::copy_options::overwrite_existing
        );
    }

    void MehWorkspace::write_source_file(const std::string& source_code) {
        std::filesystem::path source_file = workspace_dir / "meta_transform.cpp";
        std::ofstream file(source_file);
        if (!file) {
            throw std::runtime_error("Failed to create source file: " + source_file.string());
        }
        file << source_code;
        file.close();
    }

}