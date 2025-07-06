#include <iostream>
#include "cpptha.h"
#include "drive/driver.hpp"

int cpptha_process_file(const char* input_file, const char* output_file, const cpptha_config_t* config) {
    // C API validation
    if (!input_file) {
        return 1; // Missing input file
    }
    if (!config) {
        return 2; // Missing config
    }
    
    // Convert C types to C++ types
    cpptha::Options options;
    options.verbose = config->verbose;
    options.preprocess_only = config->preprocess_only;
    options.max_iterations = config->max_iterations;
    
    std::string cpp_input_file(input_file);
    std::string cpp_output_file = output_file ? std::string(output_file) : "";
    
    // Bridge to C++ implementation
    return cpptha::process_file(cpp_input_file, cpp_output_file, options);
}

void cpptha_print_build_info(){
    #ifdef NDEBUG
    std::cout << "cpptha/0.0: Hello World Release!\n";
    #else
    std::cout << "cpptha/0.0: Hello World Debug!\n";
    #endif

    // ARCHITECTURES
    #ifdef _M_X64
    std::cout << "  cpptha/0.0: _M_X64 defined\n";
    #endif

    #ifdef _M_IX86
    std::cout << "  cpptha/0.0: _M_IX86 defined\n";
    #endif

    #ifdef _M_ARM64
    std::cout << "  cpptha/0.0: _M_ARM64 defined\n";
    #endif

    #if __i386__
    std::cout << "  cpptha/0.0: __i386__ defined\n";
    #endif

    #if __x86_64__
    std::cout << "  cpptha/0.0: __x86_64__ defined\n";
    #endif

    #if __aarch64__
    std::cout << "  cpptha/0.0: __aarch64__ defined\n";
    #endif

    // Libstdc++
    #if defined _GLIBCXX_USE_CXX11_ABI
    std::cout << "  cpptha/0.0: _GLIBCXX_USE_CXX11_ABI "<< _GLIBCXX_USE_CXX11_ABI << "\n";
    #endif

    // MSVC runtime
    #if defined(_DEBUG)
        #if defined(_MT) && defined(_DLL)
        std::cout << "  cpptha/0.0: MSVC runtime: MultiThreadedDebugDLL\n";
        #elif defined(_MT)
        std::cout << "  cpptha/0.0: MSVC runtime: MultiThreadedDebug\n";
        #endif
    #else
        #if defined(_MT) && defined(_DLL)
        std::cout << "  cpptha/0.0: MSVC runtime: MultiThreadedDLL\n";
        #elif defined(_MT)
        std::cout << "  cpptha/0.0: MSVC runtime: MultiThreaded\n";
        #endif
    #endif

    // COMPILER VERSIONS
    #if _MSC_VER
    std::cout << "  cpptha/0.0: _MSC_VER" << _MSC_VER<< "\n";
    #endif

    #if _MSVC_LANG
    std::cout << "  cpptha/0.0: _MSVC_LANG" << _MSVC_LANG<< "\n";
    #endif

    #if __cplusplus
    std::cout << "  cpptha/0.0: __cplusplus" << __cplusplus<< "\n";
    #endif

    #if __INTEL_COMPILER
    std::cout << "  cpptha/0.0: __INTEL_COMPILER" << __INTEL_COMPILER<< "\n";
    #endif

    #if __GNUC__
    std::cout << "  cpptha/0.0: __GNUC__" << __GNUC__<< "\n";
    #endif

    #if __GNUC_MINOR__
    std::cout << "  cpptha/0.0: __GNUC_MINOR__" << __GNUC_MINOR__<< "\n";
    #endif

    #if __clang_major__
    std::cout << "  cpptha/0.0: __clang_major__" << __clang_major__<< "\n";
    #endif

    #if __clang_minor__
    std::cout << "  cpptha/0.0: __clang_minor__" << __clang_minor__<< "\n";
    #endif

    #if __apple_build_version__
    std::cout << "  cpptha/0.0: __apple_build_version__" << __apple_build_version__<< "\n";
    #endif

    // SUBSYSTEMS

    #if __MSYS__
    std::cout << "  cpptha/0.0: __MSYS__" << __MSYS__<< "\n";
    #endif

    #if __MINGW32__
    std::cout << "  cpptha/0.0: __MINGW32__" << __MINGW32__<< "\n";
    #endif

    #if __MINGW64__
    std::cout << "  cpptha/0.0: __MINGW64__" << __MINGW64__<< "\n";
    #endif

    #if __CYGWIN__
    std::cout << "  cpptha/0.0: __CYGWIN__" << __CYGWIN__<< "\n";
    #endif
}