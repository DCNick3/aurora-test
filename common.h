#pragma once

#include <filesystem>
#include <cstdint>
#include <tl/expected.hpp>

class error {
    std::string _message;
public:
    explicit error(std::string s)
        : _message(std::move(s))
    {}

    inline static error posix(const std::string& doing_what, const std::string& strerror) {
        return error(doing_what + ": " + strerror);
    }

    [[nodiscard]] inline std::string const& message() const { return _message; }
};


inline std::ostream &operator<<(std::ostream &os, error const &m) {
    return os << m.message();
}


// this uses non-standard extension (statement expression), but makes life SO MUCH more easy
#define TRY(expr) ({ auto&& x = expr; if (!x) return tl::make_unexpected(x.error()); std::move(*x); })