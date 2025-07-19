#pragma once
#include "MehWorkspace.hpp"
#include <filesystem>

namespace cpptha {

    // MehBuilder: Responsible for compilation using workspace
    class MehBuilder {
    public:
        explicit MehBuilder(const MehWorkspace& workspace);
        
        bool compile();
        bool compile(const std::string& filename);
        
    private:
        const MehWorkspace& workspace;
        std::string build_g_plus_plus_command();
        std::string build_g_plus_plus_command(const std::string& filename);
    };

}