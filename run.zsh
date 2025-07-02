#!/bin/zsh

# Parse arguments: use Debug as default, accept Release or Debug
BUILD_TYPE=${1:-Debug}

# Validate build type
if [[ "$BUILD_TYPE" != "Release" && "$BUILD_TYPE" != "Debug" ]]; then
    echo "Error: BUILD_TYPE must be 'Release' or 'Debug'"
    echo "Usage: $0 [Release|Debug] [app_args...]"
    exit 1
fi

# Shift to remove BUILD_TYPE from arguments, leaving app arguments
shift

# Define the build directory and workspace directory
BUILD_DIR="build/$BUILD_TYPE"
WORKSPACE_DIR="workspace"

echo "Running $BUILD_TYPE build..."

# Create the workspace directory if it doesn't exist
echo "Creating workspace directory..."
mkdir -p "$WORKSPACE_DIR"

# Build the project using cmake
echo "Building the project with cmake..."
cmake --build "$BUILD_DIR"

# Check if the build was successful
if [[ $? -ne 0 ]]; then
    echo "Build failed. Exiting."
    exit 1
fi

# Copy the built binary (cpptha) to the workspace directory
echo "Copying the 'cpptha' binary to the workspace directory..."
cp "$BUILD_DIR/cpptha" "$WORKSPACE_DIR/"

# Change to the workspace directory
cd "$WORKSPACE_DIR"

./cpptha "$@"
