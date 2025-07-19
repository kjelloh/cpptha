#pragma once
#include "MehWorkspace.hpp"
#include <filesystem>

namespace cpptha {

    // MehBuilder: Responsible for compilation using workspace
    class MehBuilder {
    public:
        explicit MehBuilder(const MehWorkspace& workspace);
        
        bool compile();
        
    private:
        const MehWorkspace& workspace;
        std::string build_g_plus_plus_command();
    };

}