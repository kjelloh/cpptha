#pragma once
#include <filesystem>
#include <string>

namespace cpptha {

    // MehWorkspace: Responsible for folder creation and population
    class MehWorkspace {
    public:
        explicit MehWorkspace(const std::filesystem::path& working_dir, bool keep_files = false);
        ~MehWorkspace();
        
        // Setup workspace with source code
        void setup_for_source(const std::string& source_code);
        
        // Accessors for build process
        const std::filesystem::path& get_workspace_dir() const { return workspace_dir; }
        std::filesystem::path get_source_file() const { return workspace_dir / "meta_transform.cpp"; }
        std::filesystem::path get_meh_cpp_file() const { return workspace_dir / "meh.cpp"; }
        std::filesystem::path get_lib_output_path() const { return workspace_dir / "meta_transform.so"; }
        
    private:
        std::filesystem::path working_dir;
        std::filesystem::path workspace_dir;
        bool keep_files;
        
        void create_workspace_directory();
        void copy_meh_library_files();
        void write_source_file(const std::string& source_code);
    };

}