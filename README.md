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
