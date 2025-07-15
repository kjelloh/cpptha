#!/bin/zsh

# Read current preset from file, default to Debug if file doesn't exist
if [[ -f "preset.txt" ]]; then
    BUILD_TYPE=$(cat preset.txt)
    echo "Using preset from preset.txt: $BUILD_TYPE"
else
    BUILD_TYPE="Debug"
    echo "Warning: preset.txt not found, defaulting to Debug"
    echo "Run './init_tool_chain.zsh [Debug|Release]' first to set preset"
fi

# Validate build type
if [[ "$BUILD_TYPE" != "Release" && "$BUILD_TYPE" != "Debug" ]]; then
    echo "Error: Invalid preset in preset.txt: $BUILD_TYPE"
    echo "Run './init_tool_chain.zsh [Debug|Release]' to fix"
    exit 1
fi

# All script arguments are passed directly to cpptha (no shift needed)

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

# Copy meh library files to workspace/src/meh
echo "Copying meh library files to workspace..."
mkdir -p "$WORKSPACE_DIR/src/meh"
cp "src/meh/meh.hpp" "$WORKSPACE_DIR/src/meh/"
cp "src/meh/meh.cpp" "$WORKSPACE_DIR/src/meh/"

# Change to the workspace directory
cd "$WORKSPACE_DIR"

./cpptha "$@"
