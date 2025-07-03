#C++ Tha Meta Language

```c++
meta {
  // In a meta-scope we 'lift' one level up from the outer meta level
  // So if we are at meta level 0 with 'named values' like variables, constants etc.,
  // Then here we have 'types' as 'named values'.

  // Assume we are at meta level 1 - then the following code is valid

  // * C++ meta levels are now shifted one level 'up'
  // * C++ meta level 0 'named values' is now meta level -1 (invisible)
  // * C++ meta level 1 'types' is now meta level 0 ('named values')
  // * C++ meta level 2 'type constructors' is now metal level 1 (types)
  // * C++ meta level 3 (e.g. 'template' as constructor of type constructor) is now meta level 2 (type constructor)
}
```

In a first level meta scope the C++ type system 'named values' becomes invisible and C++ types 'becomes' semantically 'named values'. We can use syntax valid for C++ 'named values' (variables, constants etc.) on type names.

```c++
meta {
  // Assume we are at meta level 1 - then the following code is valid

  /**
    * Built in 'int' is now semantically 'a value'
    * So the meta syntax is C++ for a 'named value' (in this case a constant named 'int')
    */
  auto my_int = int; // my_int is a copy if 'int'
  auto const& my_int_ref = int; // My int is in effect a c++ type alias (a reference to 'int'
}
```

In a first level meta scope the C++ type system 'type constructors' (e.g., 'struct', 'union', 'template instances') 'becomes' semantically 'types'. We can use syntax for C++ types on built in 'struct', 'union' and defined template instances.

```c++
meta {
  // Assume we are at meta level 1 - then the following code is valid

  /**
    * Built in 'struct' is now semantically a 'type' (one meta level above C++ types).
    * Now 'struct' in C++ takes a list of 'type-name' pairs with optional value initializer.
    * This does not change by us being in meta level 1. It is the 'struct' object that interprets 
    * its initialiser list so we must write it 'as normal'.
    */

  struct A;     // Declared 'named value' A
  A = struct {} // A Defined

  /**
    * Now 'struct' in C++ takes a list of 'type-name' pairs with optional value initializer.
    * This does not change by us being in meta level 1. It is the 'struct' object that interprets 
    * its initialiser list so we must write it 'as normal'. So the syntax looks familiar at this meta level.
    */

  auto B  = struct {
    int x;
    bool b {false};
  };
}
```
