#pragma once

#ifdef _WIN32
#define RUNE_API __declspec(dllexport)
#else
#define RUNE_API __attribute__((visibility("default")))
#endif

#include "Node.hpp"

struct Test : public Node {
    int number = 5;

    Test() {
        _name = "Test";
    }

    ~Test() {
        std::println("Tschüss dies war ein test {}", this->number);
    }

    int test(int new_number = 0) {
        std::println("Hallo dies ist ein test {}", this->number);
        this->number = new_number;
        return this->number;
    }

    void _update() override {
        std::println("Dies ist ein update");
    }
};

extern "C" {
RUNE_API Test *create() {
    return new Test();
}
}