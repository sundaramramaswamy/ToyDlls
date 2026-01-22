# C++ is not a binary standard

- C++ is a language standard
- Lacks an ABI, standard calling convention, naming (mangling), etc.
- Different toolchains do different things
    + Even between their own versions

# How to build and consume cross-toolchain DLLs?

1. COM: a binary standard
    - Standardizes
        + Object (vtable) layout
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

# Reference

- [About DLLs][]: Comprehensive reference
- [Walkthrough: Creating and Using a DLL][create-use]
- Loading a DLL
    + [Load-Time Dynamic Linking][load-start] - implicit (at app start)
    + [Run-time Dynamic Linking][load-on-demand] - explicit (on-demand in an app code path)
    + [DLL Search Order][search-order]
- [Importing and exporting symbols to/from a DLL][import-export]

[about dlls]: https://learn.microsoft.com/en-us/windows/win32/dlls/about-dynamic-link-libraries
[import-export]: https://learn.microsoft.com/en-us/cpp/build/importing-and-exporting?view=msvc-170
[load-start]: https://learn.microsoft.com/en-us/windows/win32/dlls/load-time-dynamic-linking
[load-on-demand]: https://learn.microsoft.com/en-us/windows/win32/dlls/run-time-dynamic-linking
[create-use]: https://learn.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-170
[search-order]: https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order

# See Also

- [Build Fundamentals - Slide Deck][build-fundamentals]
    + Covers basic C++ building
    + Differences b/w static and dynamic libraries
- [C++ Object Layout - Nimrod’s Coding Blog][cpp-obj-layout]

[build-fundamentals]: https://legends2k.github.io/build-fundamentals/
[cpp-obj-layout]: https://nimrod.blog/posts/what-does-cpp-object-layout-look-like/
