#include "meta_parser.hpp"
#include <tao/pegtl/file_input.hpp>
#include <fstream>
#include <sstream>

namespace cpptha::parse {

    std::optional<ParseResult> MetaParser::parse_file(const std::string& file_path) {
        try {
            // Read file content
            std::ifstream file(file_path);
            if (!file.is_open()) {
                set_error("Could not open file: " + file_path);
                return std::nullopt;
            }
            
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();
            
            // Parse the content
            return parse_content(content);
            
        } catch (const std::exception& e) {
            set_error("Error reading file '" + file_path + "': " + e.what());
            return std::nullopt;
        }
    }

    std::optional<ParseResult> MetaParser::parse_content(std::string_view content) {
        try {
            ParseResult result;
            result.source_content = std::string(content);
            
            // Create PEGTL input from string
            tao::pegtl::memory_input input(result.source_content, "input");
            
            // Parse all meta scopes using star combinator to find all occurrences
            tao::pegtl::parse<tao::pegtl::star<tao::pegtl::sor<meta_scope, tao::pegtl::any>>, action>(input, result);
            
            return result;
            
        } catch (const std::exception& e) {
            set_error("Parse error: " + std::string(e.what()));
            return std::nullopt;
        }
    }

}