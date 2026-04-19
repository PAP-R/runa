#pragma once

#include <print>

namespace Log {
    template <typename... Args>
    const void print(const std::format_string<Args...> &fmt, Args &&...args) {
        std::print(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    const void println(const std::format_string<Args...> &fmt, Args &&...args) {
        std::println(fmt, std::forward<Args>(args)...);
    }
} // namespace Log