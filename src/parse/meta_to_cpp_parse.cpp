#include "meta_to_cpp_parse.hpp"
#include <iostream>

namespace cpptha::parse {

    MetaToCppParseResult MetaToCppParser::parse(const std::string& content) {
        MetaToCppParseResult result;
        
        try {
            // Create input from string
            tao::pegtl::string_input<> input(content, "meta_content");
            
            // Parse with actions
            if (!tao::pegtl::parse<meta_cpp_grammar::content, meta_cpp_action>(input, result)) {
                set_error(result, "Failed to parse meta-cpp content");
                return result;
            }
            
            result.success = true;
            
        } catch (const std::exception& e) {
            set_error(result, "Parse exception: " + std::string(e.what()));
        }
        
        return result;
    }
    
    void MetaToCppParser::set_error(MetaToCppParseResult& result, const std::string& error) {
        result.success = false;
        result.error_message = error;
        result.constructs.clear();
    }

}