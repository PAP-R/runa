#pragma once

#ifdef _WIN32
#define RUNE_API __declspec(dllexport)
#else
#define RUNE_API __attribute__((visibility("default")))
#endif

#include <print>

struct ITest {
    virtual ~ITest() {}
    virtual int test(int new_number) = 0;
};
