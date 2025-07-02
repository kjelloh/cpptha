cmake_minimum_required(VERSION 3.15)
project(cpptha CXX)

find_package(CLI11 REQUIRED)

add_executable(cpptha src/cpptha.cpp src/main.cpp)

target_link_libraries(cpptha CLI11::CLI11)

install(TARGETS cpptha DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
