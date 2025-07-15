#pragma once

#include <tao/pegtl.hpp>
#include <string>
#include <optional>
#include <vector>

namespace cpptha::parse {

    // Grammar for parsing meta-cpp constructs
    namespace meta_cpp_grammar {
        
        // Keywords for our meta-cpp-language (reduced subset)
        struct struct_kw : tao::pegtl::string<'s','t','r','u','c','t'> {};
        struct meta_kw : tao::pegtl::string<'m','e','t','a'> {};

        struct keyword : tao::pegtl::sor<struct_kw, meta_kw> {};
        struct optional_identifier : tao::pegtl::opt<tao::pegtl::identifier> {};
        struct whitespace : tao::pegtl::star<tao::pegtl::space> {};
        struct open_brace : tao::pegtl::one<'{'> {};
        struct close_brace : tao::pegtl::one<'}'> {};

        // Balanced braces for body content
        struct balanced_braces;
        struct brace_content : tao::pegtl::sor<balanced_braces, tao::pegtl::not_one<'{', '}'>> {};
        struct balanced_braces : tao::pegtl::seq<open_brace, tao::pegtl::star<brace_content>, close_brace> {};
        struct body : tao::pegtl::star<brace_content> {};

        // Complete pattern
        struct construct : tao::pegtl::seq<
            keyword,
            whitespace,
            optional_identifier,
            whitespace,
            open_brace,
            body,
            close_brace
        > {};

        // Parser for multiple constructs
        struct content : tao::pegtl::star<
            tao::pegtl::sor<
                construct,
                tao::pegtl::any
            >
        > {};
    }

    // Result structure for parsed constructs
    struct ConstructParts {
        std::string keyword;
        std::optional<std::string> identifier;  // nullopt if no identifier
        std::string body;
        
        ConstructParts() = default;
        ConstructParts(const std::string& kw, const std::optional<std::string>& id, const std::string& b)
            : keyword(kw), identifier(id), body(b) {}
    };

    // Parse result containing all found constructs
    struct MetaToCppParseResult {
        std::vector<ConstructParts> constructs;
        std::string error_message;
        bool success;
        
        MetaToCppParseResult() : success(true) {}
    };

    // Parser action to capture constructs
    template<typename Rule> 
    struct meta_cpp_action {};

    template<> 
    struct meta_cpp_action<meta_cpp_grammar::keyword> {
        template<typename Input>
        static void apply(const Input& in, MetaToCppParseResult& result) {
            // Start a new construct
            result.constructs.emplace_back();
            result.constructs.back().keyword = in.string();
        }
    };

    template<> 
    struct meta_cpp_action<meta_cpp_grammar::optional_identifier> {
        template<typename Input>
        static void apply(const Input& in, MetaToCppParseResult& result) {
            if (!result.constructs.empty() && !in.string().empty()) {
                result.constructs.back().identifier = in.string();
            }
        }
    };

    template<> 
    struct meta_cpp_action<meta_cpp_grammar::body> {
        template<typename Input>
        static void apply(const Input& in, MetaToCppParseResult& result) {
            if (!result.constructs.empty()) {
                result.constructs.back().body = in.string();
            }
        }
    };

    // Parser class
    class MetaToCppParser {
    public:
        // Parse meta-cpp content and return constructs
        MetaToCppParseResult parse(const std::string& content);
        
    private:
        void set_error(MetaToCppParseResult& result, const std::string& error);
    };

}