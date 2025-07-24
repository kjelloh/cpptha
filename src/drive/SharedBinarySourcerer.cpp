#include "SharedBinarySourcerer.hpp"
#include <sstream>

namespace cpptha {

    SharedBinarySourcerer::SharedBinarySourcerer(const std::string& injected_code) 
        : injected_code(injected_code) {
    }

    std::string SharedBinarySourcerer::generate() const {
        std::ostringstream source;
        
        source << generate_header();
        source << generate_function_wrapper();
        
        return source.str();
    }

    SharedBinarySourcerer SharedBinarySourcerer::for_mehtotha_repr(const std::string& meh_model_repr) {
        // For cpptha representation, we inject the generated meh library calls
        std::ostringstream code;
        code << "    // Execute the cpptha representation code\n";
        code << "    " << meh_model_repr;
        code << "\n";
        code << "    // Return the result of env.to_string()\n";
        code << "    static std::string result = env.to_string();\n";
        code << "    return result.c_str();";
        
        return SharedBinarySourcerer(code.str());
    }

    SharedBinarySourcerer SharedBinarySourcerer::for_test_code(const std::string& test_code) {
        // For test code, inject the test code directly (should already include return statement)
        return SharedBinarySourcerer(test_code);
    }

    std::string SharedBinarySourcerer::generate_header() const {
        std::ostringstream header;
        
        // Header comments
        header << "// Generated C++/C hybrid shared library source\n";
        header << "// Exposes C API, implements in C++\n";
        header << "// Content size: " << injected_code.size() << " bytes\n";
        header << "\n";
        
        // C API Declaration
        header << "extern \"C\" {\n";
        header << "    const char* defacto_string();\n";
        header << "}\n";
        header << "\n";
        
        // Include meh library for meta_tha and struct_tha classes
        header << "#include \"meh.hpp\"\n";
        header << "\n";
        
        return header.str();
    }

    std::string SharedBinarySourcerer::generate_function_wrapper() const {
        std::ostringstream wrapper;
        
        // C++ Implementation that executes injected code
        wrapper << "const char* defacto_string() {\n";
        wrapper << "using namespace meh;";
        wrapper << injected_code;
        wrapper << "\n";
        wrapper << "}\n";
        
        return wrapper.str();
    }

}