cmake_minimum_required(VERSION 3.15)
project(cpptha CXX)

find_package(CLI11 REQUIRED)
find_package(GTest REQUIRED)
find_package(pegtl REQUIRED)

add_executable(cpptha 
    src/cpptha.cpp 
    src/main.cpp
    src/parse/meta_parser.cpp
    src/test/test_runner.cpp
    src/test/test_atomics.cpp
    src/test/test_integrations.cpp
    src/test/test_fixtures.cpp
)

target_link_libraries(cpptha 
    CLI11::CLI11
    GTest::gtest
    GTest::gtest_main
    taocpp::pegtl
)

install(TARGETS cpptha DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
