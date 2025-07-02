#include "test_atomics.hpp"
#include "test_fixtures.hpp"
#include <gtest/gtest.h>
#include <iostream>

namespace tests::atomics {
    
    // Dummy atomic test using gtest - always passes
    TEST(AtomicTests, DummyAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_EQ(1, 1);
    }
    
    bool run_all() {
        std::cout << "Running atomic tests..." << std::endl;
        
        // Run gtest for this namespace
        ::testing::InitGoogleTest();
        int result = RUN_ALL_TESTS();
        
        return result == 0;
    }
}