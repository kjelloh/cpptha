#include "test_atomics.hpp"
#include "test_fixtures.hpp"
#include "../parse/meta_parser.hpp"
#include <gtest/gtest.h>
#include <iostream>

namespace tests::atomics {

    // Dummy atomic test using gtest - always passes
    TEST(AtomicTests, DummyAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_EQ(1, 1);
    }

    namespace parse {


        // Test basic meta scope parsing
        TEST(MetaParserTest, ParseSingleMetaScope) {
            cpptha::parse::MetaParser parser;
            std::string input = "some code meta{int x = 5;} more code";

            auto result = parser.parse_content(input);

            ASSERT_TRUE(result.has_value());
            EXPECT_EQ(result->source_content, input);
            ASSERT_EQ(result->meta_scopes.size(), 1);
            EXPECT_EQ(result->meta_scopes[0], "meta{int x = 5;}");
            EXPECT_EQ(result->scope_end_positions.size(), 1);

            // Check position calculation
            auto [start, end] = result->get_scope_range(0);
            EXPECT_EQ(start, 10);  // Position of 'm' in "meta"
            EXPECT_EQ(end, 26);    // Position after '}'
        }

        // Test multiple meta scopes
        TEST(MetaParserTest, ParseMultipleMetaScopes) {
            cpptha::parse::MetaParser parser;
            std::string input = "meta{first} and meta{second} end";

            auto result = parser.parse_content(input);

            ASSERT_TRUE(result.has_value());
            ASSERT_EQ(result->meta_scopes.size(), 2);
            EXPECT_EQ(result->meta_scopes[0], "meta{first}");
            EXPECT_EQ(result->meta_scopes[1], "meta{second}");

            // Check positions
            auto [start1, end1] = result->get_scope_range(0);
            auto [start2, end2] = result->get_scope_range(1);
            EXPECT_EQ(start1, 0);
            EXPECT_EQ(end1, 11);
            EXPECT_EQ(start2, 16);
            EXPECT_EQ(end2, 28);
        }

        // Test nested braces in meta scope
        TEST(MetaParserTest, ParseNestedBraces) {
            cpptha::parse::MetaParser parser;
            std::string input = "meta{if(true){int x;}} after";

            auto result = parser.parse_content(input);

            ASSERT_TRUE(result.has_value());
            ASSERT_EQ(result->meta_scopes.size(), 1);
            EXPECT_EQ(result->meta_scopes[0], "meta{if(true){int x;}}");
        }

        // Test no meta scopes
        TEST(MetaParserTest, NoMetaScopes) {
            cpptha::parse::MetaParser parser;
            std::string input = "regular C++ code without meta scopes";

            auto result = parser.parse_content(input);

            ASSERT_TRUE(result.has_value());
            EXPECT_EQ(result->meta_scopes.size(), 0);
            EXPECT_EQ(result->scope_end_positions.size(), 0);
        }

        // Test empty input
        TEST(MetaParserTest, EmptyInput) {
            cpptha::parse::MetaParser parser;
            std::string input = "";

            auto result = parser.parse_content(input);

            ASSERT_TRUE(result.has_value());
            EXPECT_EQ(result->meta_scopes.size(), 0);
            EXPECT_EQ(result->source_content, "");
        }

    }
    
    bool run_all() {
        std::cout << "Running atomic tests..." << std::endl;
        
        // Run gtest with filter for atomic tests only
        ::testing::GTEST_FLAG(filter) = "AtomicTests.*:MetaParserTest.*";
        int result = RUN_ALL_TESTS();
        
        return result == 0;
    }
}

