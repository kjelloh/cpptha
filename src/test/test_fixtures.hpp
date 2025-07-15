#pragma once

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace tests::fixtures {
    
    // Base fixture for meta-transform tests that need meh library support
    class MetaTransformFixture : public ::testing::Test {
    protected:
        void SetUp() override {
            // Create temporary test directory
            temp_dir = std::filesystem::temp_directory_path() / "cpptha_test_build";
            std::filesystem::create_directories(temp_dir);
            
            // Copy meh library files to test directory
            copy_meh_library();
        }
        
        void TearDown() override {
            // Clean up temporary directory
            std::filesystem::remove_all(temp_dir);
        }
        
        // Copy meh library files to the specified build directory
        void copy_meh_library_to(const std::filesystem::path& build_dir) {
            std::filesystem::create_directories(build_dir);
            
            // Copy meh.hpp
            std::filesystem::copy_file(
                get_meh_source_path() / "meh.hpp",
                build_dir / "meh.hpp",
                std::filesystem::copy_options::overwrite_existing
            );
            
            // Copy meh.cpp
            std::filesystem::copy_file(
                get_meh_source_path() / "meh.cpp", 
                build_dir / "meh.cpp",
                std::filesystem::copy_options::overwrite_existing
            );
        }
        
        // Get the path to the meh source directory
        std::filesystem::path get_meh_source_path() const {
            // Navigate from test directory to src/meh
            std::filesystem::path current = std::filesystem::current_path();
            return current / "src" / "meh";
        }
        
        std::filesystem::path temp_dir;
        
    private:
        void copy_meh_library() {
            copy_meh_library_to(temp_dir);
        }
    };
    
    // Utility function to copy meh library to any meta_transform build directory
    inline void copy_meh_library_to_build_dir(const std::filesystem::path& build_dir) {
        std::filesystem::create_directories(build_dir);
        
        // Get the path to the meh source directory
        std::filesystem::path current = std::filesystem::current_path();
        std::filesystem::path meh_source = current / "src" / "meh";
        
        // Copy meh.hpp
        std::filesystem::copy_file(
            meh_source / "meh.hpp",
            build_dir / "meh.hpp",
            std::filesystem::copy_options::overwrite_existing
        );
        
        // Copy meh.cpp
        std::filesystem::copy_file(
            meh_source / "meh.cpp", 
            build_dir / "meh.cpp",
            std::filesystem::copy_options::overwrite_existing
        );
    }
    
}