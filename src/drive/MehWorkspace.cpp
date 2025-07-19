#include "MehWorkspace.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace cpptha {

    MehWorkspace::MehWorkspace(const std::filesystem::path& working_dir, bool keep_files) 
        : working_dir(working_dir), keep_files(keep_files) {
    }

    MehWorkspace::~MehWorkspace() {
        std::cout << "MehWorkspace destructor: keep_files=" << (keep_files ? "true" : "false") 
                  << ", workspace_dir=" << workspace_dir << std::endl;
        if (!keep_files && !workspace_dir.empty()) {
            std::cout << "MehWorkspace destructor: Cleaning up directory " << workspace_dir << std::endl;
            try {
                std::filesystem::remove_all(workspace_dir);
            } catch (const std::exception& e) {
                // Silently ignore cleanup errors to avoid throwing from destructor
            }
        } else {
            std::cout << "MehWorkspace destructor: Preserving directory " << workspace_dir << std::endl;
        }
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
        
        // Create unique meta-scope processing directory with date-time and milliseconds
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        std::ostringstream timestamp;
        timestamp << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
        timestamp << "_" << std::setfill('0') << std::setw(3) << ms.count();
        
        workspace_dir = build_dir / ("meh_" + timestamp.str());
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