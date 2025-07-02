#include "test_integrations.hpp"
#include "test_fixtures.hpp"
#include <gtest/gtest.h>
#include <iostream>

namespace tests::integrations {
    
    // Dummy integration test using gtest - always passes
    TEST(IntegrationTests, DummyWorkflowAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_STREQ("hello", "hello");
    }
    
    bool run_all() {
        std::cout << "Running integration tests..." << std::endl;
        
        // Run gtest for this namespace
        ::testing::InitGoogleTest();
        int result = RUN_ALL_TESTS();
        
        return result == 0;
    }
}