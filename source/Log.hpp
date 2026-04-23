#pragma once

#include <print>
#include <set>

namespace Log {
    enum Severity {
        INFO,
        WARNING,
        ERROR,
    };

    inline std::set<Severity> printSeverity = {INFO, WARNING, ERROR};

    inline std::string severity_string(Severity severity) {
        switch (severity) {
            case INFO:
                return "Info";
            case WARNING:
                return "Warning";
            case ERROR:
                return "Error";
        }
    }

    template <typename... Args>
    const void print(const std::format_string<Args...> &fmt, Args &&...args) {
        std::print(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    const void print(Severity severity, const std::format_string<Args...> &fmt, Args &&...args) {
        if (printSeverity.contains(severity)) {
            Log::print("[{}]: {}", severity_string(severity), std::format(fmt, std::forward<Args>(args)...));
        }
    }

    template <typename... Args>
    const void println(const std::format_string<Args...> &fmt, Args &&...args) {
        Log::print("{}\n", std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    const void println(Severity severity, const std::format_string<Args...> &fmt, Args &&...args) {
        if (printSeverity.contains(severity)) {
            Log::print(severity, "{}\n", std::format(fmt, std::forward<Args>(args)...));
        }
    }
} // namespace Log