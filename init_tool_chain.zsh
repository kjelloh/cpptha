#!/bin/zsh

# Exit the script on any command failure
set -e

# Parse arguments: use Debug as default, accept Release or Debug
BUILD_TYPE=${1:-Debug}

# Validate build type
if [[ "$BUILD_TYPE" != "Release" && "$BUILD_TYPE" != "Debug" ]]; then
    echo "Error: BUILD_TYPE must be 'Release' or 'Debug'"
    echo "Usage: $0 [Release|Debug]"
    exit 1
fi

echo "Initializing toolchain for $BUILD_TYPE build..."

# Install dependencies for the specified build type (also updates cmake presets)
conan install . --settings=compiler.cppstd=23 --settings=build_type=$BUILD_TYPE --build=missing

echo "Toolchain initialization complete for $BUILD_TYPE build."
echo "You can now build the project using:"
echo "  > 'cmake --build build/$BUILD_TYPE' from here"
echo "Or use the provided 'run.zsh' script to build and run the project."
