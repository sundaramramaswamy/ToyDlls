# Remember

1. C++ is a language standard
    - Not a binary standard
    - Lacks an ABI
    - Different toolchains do different things
2. How to consume a C++ DLL cross-toolchain?
    1. Use C at DLL boundary
        - C ABI is a _de facto_ standard b/w toolchains
    2. Use COM
        - Standard binary interface