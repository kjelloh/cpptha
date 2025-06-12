#C++ 'Tha' Meta Language Compiler

The C++ 'Tha' Meta Language Compiler parses 'meta scopes' into C++ source code that can be compiled into dll. This dll is then executed to return new meta code as text. But this returned meta code has one level of meta collapsed. Or put another way, when the dll is executed, selected snippets of the input meta code is transformed into 'defacto' code.

The basic idea is thus to process C++ meta code using C++ itself, with all the bells and whistles available in the form of the C++ library and compiler tooling.

The meta language 'compiler' performs the following steps on a C++ file.

1. Apply C++ pre-processing to turn the C++ file into a transaltion unit.
2. Extract all meta-scopes from the transaltion unit (marking where each scope goes)
3. Transform selected meta-scopes into C++ source code that can be compiled into a shared binary.
4. Load the shared binary and execute it to return 'compiled' meta code.
5. Insert the compiled meta code for each meta scope back into the translation unit.
6. Repeat from (1) until no meta scopes remains (or compilation fails)

##Example 0

```c++
// ...
// C++ translation unit
// ...
meta {
  // Assume we are at meta level 1 - then the following code is valid

  /**
    * Built in 'int' is now semantically 'a value'
    * So the meta syntax is C++ for a 'named value' (in this case a constant named 'int')
    */
  auto my_int = int; // my_int is a copy if 'int'
}
```

The C++ 'Tha' Meta Language compiler transforms this meta scope into a C++ function in a shared binary.

```c++
import std;
#include "cpptha/MYLIB_API.hpp
#include "cpptha/built_in.hpp"
#include "cpptha/environment.hpp"

auto eval() {
  // Meta scope 'code' modified to compile as ordinary C++

  auto my_int = built_in::Int; // cpptha provides an Int object
  return my_int;

}

MYLIB_API std::string defacto() {
  return cpptha::to_string(eval()); // The cpptha::Int knows how to represent itself 'one meta level down'

  // For this example: "using my_int = int"
}

```
