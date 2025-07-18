#include "MehBuilder.hpp"
#include <sstream>
#include <cstdlib>

namespace cpptha {

    MehBuilder::MehBuilder(const MehWorkspace& workspace) 
        : workspace(workspace) {
    }

    bool MehBuilder::compile() {
        std::string cmd = build_g_plus_plus_command();
        
        // Execute compiler
        int result = std::system(cmd.c_str());
        return result == 0;
    }

    std::string MehBuilder::build_g_plus_plus_command() {
        std::ostringstream cmd;
        cmd << "g++ -shared -fPIC -std=c++17 -o " 
            << workspace.get_lib_output_path().string() << " " 
            << workspace.get_source_file().string() << " " 
            << workspace.get_meh_cpp_file().string();
        
        return cmd.str();
    }

}