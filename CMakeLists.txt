cmake_minimum_required(VERSION 3.15)
project(cpptha CXX)



add_executable(cpptha src/cpptha.cpp src/main.cpp)



install(TARGETS cpptha DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
