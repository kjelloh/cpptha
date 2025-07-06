# cpptha
C++ meta-code preprocessor for scripts based on the 'Tha' type system model embedded in C++ source code

The name is is riff on 'cpp' for the C++ language and 'Tha' as the working name of the meta layered data model of the C++ type system.

The seed for the 'Tha model' is described in YouTube playlist ['C++ Tha Tha'](https://www.youtube.com/watch?v=zdF_evtjJGg&list=PLl2MXgNWEbwE34Cxjx9uW-wdq7RyZ_uQj)

# The Tha type system model
It basically defines meta tiers with a 'meta' relation from a lower tier to a higher tier. And a 'defacto' relation from a higher tier to a lower tier.

The 'meta' relation defines an 'aboutness' that defines 'what' instantiates a 'thing' on the lower tier using 'things' on the higher tier. The 'defacto' relation defines the same 'aboutness' but from the higher tier to the lower one.

```sh
--------------- Meta tier N+1 ---------------
   ^                               |
   |                               |   
  meta                          defacto
   |                               |
   |                               v
--------------- Meta tier N ---------------
```

# Tha C++ meta-code

The Tha C++ meta-code is based on introduced meta-scopes 'meta' and 'defacto'.

Inside a meta-scope the C++ code is 'meta-shifted' to be 'about' the objects that exists in the outer (defacto) scope.

```sh
meta {
  // C++ code here is interpreted as to be 'about' the code in the outer scope
}
```

In the opposite way, inside a 'defacto-scope' the code represents what the surrounding scope (meta) 'creates'. That is, the 'defacto' code that the meta tiers talks about.

```sh
defacto {
  // C++ code here is interpreted as the actual (defacto) code the surroudning scope creates or is 'about'
}
```

# Semantics

The cpptha is a command line application that takes a C++ source code file and processes the embedded C++ meta code until it is transformed into plain C++ source code. It then applies a C++ compiler to the resulting C++ code to output a binary.

# Operation

The application starts by preprocessing the C++ source file to obtain a single translation using as defined by the C++/C preprocessor.

It will then process the translation unit until all meta-scopes has been transformed into plain C++ source code. *Note that 'defacto-scopes' can only exist withing meta-scopes. So when all-meta-scopes has been processed, so have also all defacto-scopes.*

As the final stage the application will apply a C++ compiler to the resulting C++ source code.

# Command line

```sh
>cpptha with_meta_code.cpp
```

The result will be some compilation failure or a compiled binary.

# PyroTha Docs

* [C++ 'Tha' Meta Language](CppThaMetaLanguage.md)
* [C++ 'Tha' Meta Language Compiler](CppThaCompiler.md)

# Initial version 0.0

An empty repository to create a C++ CMake and Conan package manager consuming software development environment.

How this project has been setup

This project is setup as a conan package consumer combined with cmake support for tool chain build and application.

For conan command references see https://docs.conan.io/2/reference/commands.html

## conan new cmake_exe -d name=cpptha -d version=0.0

For conan command reference see https://docs.conan.io/2/reference/commands/new.html

```sh
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % conan new cmake_exe -d name=cpptha -d version=0.0
File saved: CMakeLists.txt
File saved: conanfile.py
File saved: src/cpptha.cpp
File saved: src/cpptha.h
File saved: src/main.cpp
File saved: test_package/conanfile.py
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % 
```

# Build / Development

The build environment is designed as a conan package manager consumer project using cmake as the tool chain generator and executor.

* [Development Progress](../CLAUDE.md) - Architecture and progress notes (AI-generated, developer-readable)

## Conan/CMake Framework

This project uses Conan 2.x with CMake for cross-platform dependency management and builds. The setup leverages:

- **Conan**: Package manager for C++ dependencies with automatic toolchain generation
- **CMake**: Build system with preset support for consistent configuration
- **Cross-platform builds**: Conan/CMake defaults should enable builds on Windows (MSVC), macOS (Xcode/Clang), and Linux (GCC/Clang)

The framework generates CMake presets automatically via `conan install`, eliminating manual toolchain configuration.

## Helper Scripts

Two zsh shell scripts automate the build workflow:

- **[init_tool_chain.zsh](../init_tool_chain.zsh)** - Initializes toolchain and generates CMake presets
- **[run.zsh](../run.zsh)** - Builds and runs the application

**Note**: These scripts require a zsh shell environment.

### Script Usage

Both scripts accept optional build type arguments (defaults to Debug):

```zsh
# Initialize toolchain
./init_tool_chain.zsh [Release|Debug]

# Build and run
./run.zsh [Release|Debug] [app_args...]
```

Examples:
```zsh
./init_tool_chain.zsh Debug    # Generate Debug preset
./init_tool_chain.zsh Release  # Generate Release preset
./run.zsh Debug               # Build and run Debug version
./run.zsh Release --help      # Build and run Release with arguments
```

## CLion Integration

CLion integrates seamlessly with this Conan/CMake setup using the generated presets:

### Setup Steps

1. **Open project**: CLion automatically detects CMakeLists.txt
2. **Preset detection**: CLion reads CMakeUserPresets.json generated by `conan install`
3. **Configuration**: 
   - File → Settings → Build, Execution, Deployment → CMake
   - CLion should show available presets: `conan-debug`, `conan-release`
   - Remove any auto-created profiles and use the Conan presets

### Workflow

1. Run `./init_tool_chain.zsh Debug` to generate Debug preset
2. Run `./init_tool_chain.zsh Release` to generate Release preset  
3. CLion automatically detects both presets in CMakeUserPresets.json
4. Select desired preset in CLion's CMake settings
5. CLion builds using your `build/Debug` or `build/Release` folders

### Cache Management

CLion maintains its own project model cache separate from CMake's build cache:

- **CMake's cache**: Located in `build/Debug/CMakeCache.txt` and `build/Release/CMakeCache.txt`
- **CLion's cache**: Internal IDE cache for code completion, navigation, and project structure

**When cache gets out of sync:**
- Adding new Conan dependencies (like gtest)
- Modifying CMakeLists.txt significantly
- Changing preset configurations

**Manual refresh:**
- **Tools → CMake → Reset Cache and Reload Project** (recommended solution)

**Common symptoms of stale cache:**
- "Could NOT find [Package]" errors in CLion while CLI builds work fine
- Missing code completion for new dependencies
- CLion showing incorrect build targets

### Benefits

- **Consistent toolchain**: Same Conan-generated configuration for CLI and IDE
- **No config tampering**: CLion doesn't modify your CMakeLists.txt or conanfile.py
- **Preset integration**: Both CLI and CLion use identical build configurations
- **Cross-platform**: Works the same way on all supported platforms

## Script 'init_tool_chain.zsh' execution log example

```zsh
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % ./init_tool_chain.zsh 

======== Input profiles ========
Profile host:
[settings]
arch=armv8
build_type=Release
compiler=apple-clang
compiler.cppstd=23
compiler.libcxx=libc++
compiler.version=16
os=Macos

Profile build:
[settings]
arch=armv8
build_type=Release
compiler=apple-clang
compiler.cppstd=gnu17
compiler.libcxx=libc++
compiler.version=16
os=Macos


======== Computing dependency graph ========
Graph root
    conanfile.py (cpptha/0.0): /Users/kjell-olovhogdahl/Documents/GitHub/cpptha/conanfile.py

======== Computing necessary packages ========

======== Installing packages ========

======== Finalizing install (deploy, generators) ========
conanfile.py (cpptha/0.0): Calling generate()
conanfile.py (cpptha/0.0): Generators folder: /Users/kjell-olovhogdahl/Documents/GitHub/cpptha/build/Release/generators
conanfile.py (cpptha/0.0): CMakeToolchain generated: conan_toolchain.cmake
conanfile.py (cpptha/0.0): CMakeToolchain: Preset 'conan-release' added to CMakePresets.json.
    (cmake>=3.23) cmake --preset conan-release
    (cmake<3.23) cmake <path> -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release
conanfile.py (cpptha/0.0): CMakeToolchain generated: /Users/kjell-olovhogdahl/Documents/GitHub/cpptha/build/Release/generators/CMakePresets.json
conanfile.py (cpptha/0.0): CMakeToolchain generated: /Users/kjell-olovhogdahl/Documents/GitHub/cpptha/CMakeUserPresets.json
conanfile.py (cpptha/0.0): Generating aggregated env files
conanfile.py (cpptha/0.0): Generated aggregated env files: ['conanbuild.sh', 'conanrun.sh']
Install finished successfully
Preset CMake variables:

  CMAKE_BUILD_TYPE="Release"
  CMAKE_POLICY_DEFAULT_CMP0091="NEW"
  CMAKE_TOOLCHAIN_FILE:FILEPATH="generators/conan_toolchain.cmake"

-- Using Conan toolchain: /Users/kjell-olovhogdahl/Documents/GitHub/cpptha/build/Release/generators/conan_toolchain.cmake
-- Conan toolchain: Defining libcxx as C++ flags: -stdlib=libc++
-- Conan toolchain: C++ Standard 23 with extensions OFF
-- The CXX compiler identification is AppleClang 16.0.0.16000026
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (12.2s)
-- Generating done (0.0s)
-- Build files have been written to: /Users/kjell-olovhogdahl/Documents/GitHub/cpptha/build/Release
Toolchain initialization complete.
You can now build the project using:
  > 'cmake --build .' inside the 'build' directory.
  > 'cmake --build build/Release' from here
Or use the provided 'run.zsh' script to build and run the project.
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % 
```

## Script 'run.zsh' execution log example (version 0.0)

```sh
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % ./run.zsh example.cpp
Creating workspace directory...
Building the project with cmake...
[100%] Built target cpptha
Copying the 'cpptha' binary to the workspace directory...
cpptha/0.0: Hello World Release!
  cpptha/0.0: __aarch64__ defined
  cpptha/0.0: __cplusplus202302
  cpptha/0.0: __GNUC__4
  cpptha/0.0: __GNUC_MINOR__2
  cpptha/0.0: __clang_major__16
  cpptha/0.0: __apple_build_version__16000026
cpptha/0.0 test_package
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % 
```

## Files dynamically created by conan and cmake (That are NOT tracked by git)

This listing was created on macOS in zsh-shell using two 'find' calls and a final 'diff' call.

```sh
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % find . -type f | sort > before.txt

kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % ./init_tool_chain.zsh 

...

kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % ./run.zsh 

...

kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % find . -type f | sort > after.txt
```

Now the 'diff' on 'before' and 'after' yields the folling listing. It is basically the **CMakeUserPresets.json** file and population of new sub-directories in **./build/Release/**.

The 'run.zsh' script also creates and updates the './workspace' folder with the built application.

```sh
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % diff before.txt after.txt

42a43
> ./CMakeUserPresets.json
44a46
> ./after.txt
45a48,86
> ./build/Release/CMakeCache.txt
> ./build/Release/CMakeFiles/3.31.6/CMakeCXXCompiler.cmake
> ./build/Release/CMakeFiles/3.31.6/CMakeDetermineCompilerABI_CXX.bin
> ./build/Release/CMakeFiles/3.31.6/CMakeSystem.cmake
> ./build/Release/CMakeFiles/3.31.6/CompilerIdCXX/CMakeCXXCompilerId.cpp
> ./build/Release/CMakeFiles/3.31.6/CompilerIdCXX/CMakeCXXCompilerId.o
> ./build/Release/CMakeFiles/CMakeConfigureLog.yaml
> ./build/Release/CMakeFiles/CMakeDirectoryInformation.cmake
> ./build/Release/CMakeFiles/Makefile.cmake
> ./build/Release/CMakeFiles/Makefile2
> ./build/Release/CMakeFiles/TargetDirectories.txt
> ./build/Release/CMakeFiles/cmake.check_cache
> ./build/Release/CMakeFiles/cpptha.dir/DependInfo.cmake
> ./build/Release/CMakeFiles/cpptha.dir/build.make
> ./build/Release/CMakeFiles/cpptha.dir/cmake_clean.cmake
> ./build/Release/CMakeFiles/cpptha.dir/compiler_depend.make
> ./build/Release/CMakeFiles/cpptha.dir/compiler_depend.ts
> ./build/Release/CMakeFiles/cpptha.dir/depend.make
> ./build/Release/CMakeFiles/cpptha.dir/flags.make
> ./build/Release/CMakeFiles/cpptha.dir/link.txt
> ./build/Release/CMakeFiles/cpptha.dir/progress.make
> ./build/Release/CMakeFiles/cpptha.dir/src/cpptha.cpp.o
> ./build/Release/CMakeFiles/cpptha.dir/src/cpptha.cpp.o.d
> ./build/Release/CMakeFiles/cpptha.dir/src/main.cpp.o
> ./build/Release/CMakeFiles/cpptha.dir/src/main.cpp.o.d
> ./build/Release/CMakeFiles/progress.marks
> ./build/Release/Makefile
> ./build/Release/cmake_install.cmake
> ./build/Release/cpptha
> ./build/Release/generators/CMakePresets.json
> ./build/Release/generators/cmakedeps_macros.cmake
> ./build/Release/generators/conan_toolchain.cmake
> ./build/Release/generators/conanbuild.sh
> ./build/Release/generators/conanbuildenv-release-armv8.sh
> ./build/Release/generators/conandeps_legacy.cmake
> ./build/Release/generators/conanrun.sh
> ./build/Release/generators/conanrunenv-release-armv8.sh
> ./build/Release/generators/deactivate_conanbuild.sh
> ./build/Release/generators/deactivate_conanrun.sh
51a93
> ./workspace/cpptha
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/cpptha % 
```