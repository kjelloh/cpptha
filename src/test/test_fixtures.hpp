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
            // Create test directory under current working directory/cpptha_test
            // This ensures tests run in workspace when using run.zsh --test
            test_base_dir = std::filesystem::current_path() / "cpptha_test";
            std::filesystem::create_directories(test_base_dir);
            
            // Create test-specific subdirectory based on test name
            auto test_info = ::testing::UnitTest::GetInstance()->current_test_info();
            std::string test_name = std::string(test_info->test_suite_name()) + "_" + std::string(test_info->name());
            temp_dir = test_base_dir / test_name;
            std::filesystem::create_directories(temp_dir);
            
            // Copy meh library files to test directory (needed for some tests)
            copy_meh_library();
        }
        
        void TearDown() override {
            // Clean up test-specific directory but preserve cpptha_test for inspection
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
        
        std::filesystem::path test_base_dir;  // cpptha_test directory
        std::filesystem::path temp_dir;      // test-specific subdirectory
        
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