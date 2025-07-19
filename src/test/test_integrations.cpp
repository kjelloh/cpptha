#include "test_integrations.hpp"
#include "test_fixtures.hpp"
#include "../meh/meh.hpp"
#include "../drive/MehWorkspace.hpp"
#include "../drive/MehBuilder.hpp"
#include "../drive/SharedBinarySourcerer.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace tests::integrations {
    
    // Dummy integration test using gtest - always passes
    TEST(IntegrationTests, DummyWorkflowAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_STREQ("hello", "hello");
    }

    namespace meh {
        // Namespace alias for the fixture
        using MehBuildFixture = tests::fixtures::MehBuildFixture;
        TEST(MehBuildFixture, CompileMehLibrary) {
            // Test that MehWorkspace and MehBuilder can successfully build meh.cpp
            
            // Create a simple test source code that uses meh library
            // std::string test_code = 
            //     "    meh::meta_tha env{};\n"
            //     "    env += meh::struct_tha(\"TestStruct\", \"int x; int y;\");\n"
            //     "    static std::string result = env.to_string();\n"
            //     "    return result.c_str();";
            std::string test_code = ::meh::test::meh_test_code;
            std::string test_source = cpptha::SharedBinarySourcerer::for_test_code(test_code).generate();
            
            // Create MehWorkspace and setup with test source (respect keep_files setting)
            bool keep_files = tests::fixtures::TestEnvironment::GetInstance()->ShouldKeepTestFiles();
            cpptha::MehWorkspace workspace(std::filesystem::current_path(), keep_files);
            workspace.setup_for_source(test_source);
            
            // Show where MehWorkspace operates
            std::cout << "MehWorkspace build directory: " << workspace.get_workspace_dir() << std::endl;
            std::cout << "keep_files flag: " << (keep_files ? "true" : "false") << std::endl;
            if (keep_files) {
                std::cout << "MehWorkspace files will be preserved (--keep-test-files flag)" << std::endl;
            }
            
            // Use MehBuilder to compile
            cpptha::MehBuilder builder(workspace);
            bool compile_success = builder.compile();
            
            // Assert that compilation succeeded
            EXPECT_TRUE(compile_success) << "MehBuilder failed to compile meh.cpp with test source";
            
            // Verify the shared library was created
            EXPECT_TRUE(std::filesystem::exists(workspace.get_lib_output_path())) 
                << "Shared library was not created at: " << workspace.get_lib_output_path();
        }
    }

    // Namespace alias for the fixture
    using MetaTransformFixture = tests::fixtures::MetaTransformFixture;

    // Test that the MetaTransformFixture can copy meh library files
    TEST_F(MetaTransformFixture, CanCopyMehLibraryFiles) {
            // Verify that meh.hpp was copied
            EXPECT_TRUE(std::filesystem::exists(temp_dir / "meh.hpp"));
            
            // Verify that meh.cpp was copied  
            EXPECT_TRUE(std::filesystem::exists(temp_dir / "meh.cpp"));
            
            // Verify the content has the expected classes
            std::ifstream hpp_file(temp_dir / "meh.hpp");
            std::string hpp_content((std::istreambuf_iterator<char>(hpp_file)),
                                    std::istreambuf_iterator<char>());
            
            EXPECT_TRUE(hpp_content.find("class struct_tha") != std::string::npos);
            EXPECT_TRUE(hpp_content.find("class meta_tha") != std::string::npos);
        }
        
        // Test cpptha -E preprocessing functionality
    TEST_F(MetaTransformFixture, CppthaPreprocessOnlyMode) {
            // Create test input file with meta-scope
            std::filesystem::path input_file = temp_dir / "test_input.cpp";
            std::ofstream input(input_file);
            input << "#include <iostream>\n"
                    "\n"
                    "int main() {\n"
                    "    std::cout << \"Hello before meta\" << std::endl;\n"
                    "    \n"
                    "    meta{ struct A {}; }\n"
                    "    \n"
                    "    std::cout << \"Hello after meta\" << std::endl;\n"
                    "    return 0;\n"
                    "}\n";
            input.close();
            
            // Create output file path
            std::filesystem::path output_file = temp_dir / "test_output.cpp";
            
            // Get path to cpptha executable (assume it's in current directory)
            std::filesystem::path cpptha_exe = std::filesystem::current_path() / "cpptha";
            
            // Run cpptha -E on the input file (output goes to stdout, redirect to file)
            std::string command = cpptha_exe.string() + " -E " + 
                                input_file.string() + " > " + output_file.string();
            
            int result = std::system(command.c_str());
            EXPECT_EQ(result, 0) << "cpptha -E command failed";
            
            // Verify output file was created
            EXPECT_TRUE(std::filesystem::exists(output_file));
            
            // Read and verify output content
            std::ifstream output(output_file);
            std::string output_content((std::istreambuf_iterator<char>(output)),
                                      std::istreambuf_iterator<char>());
            
            // For now, since we have transparent transformation, 
            // the output should contain the transformed meta-scope content
            EXPECT_TRUE(output_content.find("#include <iostream>") != std::string::npos);
            EXPECT_TRUE(output_content.find("Hello before meta") != std::string::npos);
            EXPECT_TRUE(output_content.find("Hello after meta") != std::string::npos);
            EXPECT_TRUE(output_content.find("return 0;") != std::string::npos);
            
            // The meta-scope should be transformed/replaced
            // New implementation executes the code and returns the actual C++ result
            EXPECT_TRUE(output_content.find("struct A{  }") != std::string::npos);
            
            std::cout << "Test directory: " << temp_dir << std::endl;
            std::cout << "Input file: " << input_file << std::endl; 
            std::cout << "Preprocessed output:\n" << output_content << std::endl;
        }

        bool run_all() {
            std::cout << "Running integration tests..." << std::endl;
            
            // Run gtest with filter for integration tests only
            ::testing::GTEST_FLAG(filter) = "IntegrationTests.*:MetaTransformFixture.*:MehBuildFixture.*";
            int result = RUN_ALL_TESTS();
            
            return result == 0;
        }
    
} // end namespace tests::integrations
