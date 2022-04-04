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

enum class file_mod_one {
    read    = 0x1,
    write   = 0x2,
    execute = 0x4
};

inline file_mod_one operator|(file_mod_one lhs, file_mod_one rhs) {
    return static_cast<file_mod_one>(
            static_cast<std::underlying_type<file_mod_one>::type>(lhs) |
            static_cast<std::underlying_type<file_mod_one>::type>(rhs)
    );
}

inline file_mod_one& operator|=(file_mod_one& lhs, file_mod_one rhs) {
    lhs = lhs | rhs;
    return lhs;
}

inline bool operator&(file_mod_one lhs, file_mod_one rhs) {
    return (static_cast<std::underlying_type<file_mod_one>::type>(lhs) &
            static_cast<std::underlying_type<file_mod_one>::type>(rhs)) != 0;
}

inline std::ostream &operator<<(std::ostream &os, file_mod_one const &m) {
    return os
        << ((m & file_mod_one::read) ? "r" : "-")
        << ((m & file_mod_one::write) ? "w" : "-")
        << ((m & file_mod_one::execute) ? "x" : "-");
}

struct file_mod {
    file_mod_one owner;
    file_mod_one group;
    file_mod_one world;
};

inline std::ostream &operator<<(std::ostream &os, file_mod const &m) {
    return os << m.owner << m.group << m.world;
}

enum class file_kind {
    regular,
    directory,
    other
};

inline std::ostream &operator<<(std::ostream &os, file_kind const &m) {
    switch (m) {
        case file_kind::regular:
            return os << "-";
        case file_kind::directory:
            return os << "d";
        default:
            return os << "?";
    }
}

struct file_stat {
    file_mod mod;
    file_kind kind;
    uint32_t owner_uid;
    uint32_t owner_gid;
};

inline std::ostream &operator<<(std::ostream &os, file_stat const &m) {
    return os << m.kind << m.mod << " " << m.owner_uid << " " << m.owner_gid;
}


tl::expected<file_stat, error> stat_file(std::filesystem::path const& path);
