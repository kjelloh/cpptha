#pragma once
#include <string>

namespace cpptha {

    // SharedBinarySourcerer: Responsible for generating meta_transform.cpp source code
    class SharedBinarySourcerer {
    public:
        // Constructor takes the code to inject into the defacto_string function body
        explicit SharedBinarySourcerer(const std::string& injected_code);
        
        // Generate the complete source code
        std::string generate() const;
        
        // Static factory methods for common patterns
        static SharedBinarySourcerer for_cpptha_repr(const std::string& cpptha_repr);
        static SharedBinarySourcerer for_test_code(const std::string& test_code);
        
    private:
        std::string injected_code;
        
        std::string generate_header() const;
        std::string generate_function_wrapper() const;
    };

}