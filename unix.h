#pragma once

#include "common.h"

struct user_identity {
    uint32_t uid;
    uint32_t gid;
};


inline std::ostream &operator<<(std::ostream &os, user_identity const &m) {
    return os << "user_identity { UID: " << m.uid << ", GID: " << m.gid << " }";
}

tl::expected<user_identity, error> parse_identity(std::string const& username, std::string const& group_name);