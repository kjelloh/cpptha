#pragma once

#include <tao/pegtl.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <optional>

namespace cpptha::parse {

    // Grammar rules for meta scope parsing
    struct meta_keyword : tao::pegtl::string<'m','e','t','a'> {};
    struct open_brace : tao::pegtl::one<'{'> {};
    struct close_brace : tao::pegtl::one<'}'> {};
    
    // Forward declaration for recursive rule
    struct balanced_braces;
    
    // Content inside braces: either balanced braces or any non-brace character
    struct brace_content : tao::pegtl::sor<balanced_braces, tao::pegtl::not_one<'{', '}'>> {};
    
    // Balanced braces: { + content + }
    struct balanced_braces : tao::pegtl::seq<open_brace, tao::pegtl::star<brace_content>, close_brace> {};
    
    // Meta content: any content that doesn't include unmatched closing braces
    struct meta_content : tao::pegtl::star<brace_content> {};
    
    // Complete meta scope: "meta" + "{" + content + "}"
    struct meta_scope : tao::pegtl::seq<
        meta_keyword,               // "meta"
        open_brace,                 // "{"
        meta_content,               // Content inside braces
        close_brace                 // "}"
    > {};

    // Result of parsing operation
    struct ParseResult {
        std::string source_content;                    // Original file/input content
        std::vector<std::string_view> meta_scopes;     // Found meta scopes
        std::vector<size_t> scope_end_positions;       // End positions of each scope
        
        // Helper: Calculate start position of scope at index i
        size_t get_start_position(size_t index) const {
            return scope_end_positions[index] - meta_scopes[index].size();
        }
        
        // Helper: Get scope range for replacement
        std::pair<size_t, size_t> get_scope_range(size_t index) const {
            size_t start = get_start_position(index);
            size_t end = scope_end_positions[index];
            return {start, end};
        }
    };

    // Parser action to capture scopes and their end positions
    template<typename Rule> 
    struct action {};

    template<> 
    struct action<meta_scope> {
        template<typename ActionInput>
        static void apply(const ActionInput& in, ParseResult& result) {
            // Capture the matched scope content
            result.meta_scopes.emplace_back(in.string_view());
            
            // Store end position (current position + scope size)
            result.scope_end_positions.emplace_back(in.position().byte + in.string_view().size());
        }
    };

    class MetaParser {
    public:
        // Parse file and return result with original content + found scopes
        std::optional<ParseResult> parse_file(const std::string& file_path);
        
        // Parse content and return result with provided content + found scopes  
        std::optional<ParseResult> parse_content(std::string_view content);
        
        // Get last error message if parsing failed
        const std::string& get_last_error() const { return last_error_; }
        
    private:
        std::string last_error_;
        
        void set_error(const std::string& error) { last_error_ = error; }
    };

}