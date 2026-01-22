# C++ is not a binary standard

- C++ is a language standard
- Lacks an ABI, standard calling convention, naming (mangling), etc.
- Different toolchains do different things
    + Even between their own versions

# How to build and consume cross-toolchain DLLs?

1. COM: a binary standard
    - Standardizes
        + Object layout
        + Calling convention
        + Object lifetime model (`AddRef` / `Release`)
    - Runtime discovery (`QueryInterface`)
    - Works across languages!
2. Use C types/functions at DLL boundary
    - C ABI is a _de facto_ standard b/w toolchains
    - Cross-platform

# COM DLL Authoring

1. Write the IDL
    - Contract for users of your library
    - Declare interfaces and coclasses
    - Never break it; keep adding newer interfaces (`ID2D1Image1`, `ID2D1Image2`, … remember?)
2. Run MIDL
    - Generates the C++ header for you to fill up
3. Write the C++ implementation
    - Library implementation
        + DLL entry points
        + Can-unload logic
        + Class factories
    - Concrete class implementation
        + Actual logic like `add`, `sub`, etc.
