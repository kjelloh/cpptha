#pragma once
#include "meh_workspace.hpp"
#include <filesystem>

namespace cpptha {

    // MehBuild: Responsible for compilation using workspace
    class MehBuild {
    public:
        explicit MehBuild(const MehWorkspace& workspace);
        
        bool compile();
        
    private:
        const MehWorkspace& workspace;
        std::string build_g_plus_plus_command();
    };

}