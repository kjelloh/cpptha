#!/bin/zsh

# Exit the script on any command failure
set -e

conan install . --settings=compiler.cppstd=23 --build=missing
cmake --preset conan-release
echo "Toolchain initialization complete."
echo "You can now build the project using:"
echo "  > 'cmake --build .' inside the 'build' directory."
echo "  > 'cmake --build build/Release' from here"
echo "Or use the provided 'run.zsh' script to build and run the project."
