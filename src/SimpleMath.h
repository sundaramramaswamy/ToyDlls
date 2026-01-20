#pragma once

#ifdef SIMPLEMATH_EXPORTS
#define SIMPLEMATH_API __declspec(dllexport)
#else
#define SIMPLEMATH_API __declspec(dllimport)
#endif

extern "C" SIMPLEMATH_API int add(int a, int b);
extern "C" SIMPLEMATH_API int sub(int a, int b);
