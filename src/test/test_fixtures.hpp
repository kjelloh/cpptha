#pragma once

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <expected>

namespace tests::fixtures {
    
    // Global test environment to communicate settings across all tests
    class TestEnvironment : public ::testing::Environment {
    public:
        static TestEnvironment* GetInstance();
        void SetKeepTestFiles(bool keep) { keep_test_files_ = keep; }
        bool ShouldKeepTestFiles() const { return keep_test_files_; }
        
    private:
        bool keep_test_files_ = false;
    };

    // Fixture to build the meh library stand-alone
    // The fixture performs the build in to_built() method
    // and returns the compiler output as an std::expected
    class MehBuildFixture : public ::testing::Test {
    protected:
        void SetUp() override;
        void TearDown() override;

        std::expected<std::string,std::string> to_built();

    private:
        void copy_meh_library();
        std::filesystem::path test_base_dir;  // cpptha_test directory
        std::filesystem::path temp_dir;      // test-specific subdirectory

    };

    // Base fixture for meta-transform tests that need meh library support
    class MetaTransformFixture : public ::testing::Test {
    protected:
        void SetUp() override;
        void TearDown() override;
        
        // Copy meh library files to the specified build directory
        void copy_meh_library_to(const std::filesystem::path& build_dir);
        
        // Get the path to the meh source directory
        std::filesystem::path get_meh_source_path() const;
        
        std::filesystem::path test_base_dir;  // cpptha_test directory
        std::filesystem::path temp_dir;      // test-specific subdirectory
        
    private:
        void copy_meh_library();
    };
    
    // Utility function to copy meh library to any meta_transform build directory
    void copy_meh_library_to_build_dir(const std::filesystem::path& build_dir);
    
}