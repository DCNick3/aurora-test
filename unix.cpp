
#include "unix.h"

// I want to isolate POSIX includes not to pollute the precious C++ global namespace

#include <memory>

#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

static tl::expected<passwd, error> wrap_getpwnam_r(std::string const& username) {
    size_t bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)          /* Value was indeterminate */
        bufsize = 16384;        /* Should be more than enough */

    // don't you DARE to forget to free it
    std::vector<char> buf(bufsize);

    struct passwd  pwd{};
    struct passwd* result = nullptr;

    int s = getpwnam_r(username.c_str(), &pwd, buf.data(), bufsize, &result);
    if (result == nullptr) {
        if (s == 0)
            return tl::make_unexpected(error("User not found in the database"));
        else {
            return tl::make_unexpected(error::posix("getpwnam_r", strerror(s)));
        }
    }

    return pwd;
}

static tl::expected<group, error> wrap_getgrnam_r(std::string const& group_name) {
    size_t bufsize = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (bufsize == -1)          /* Value was indeterminate */
        bufsize = 16384;        /* Should be more than enough */

    // don't you DARE to forget to free it
    std::vector<char> buf(bufsize);

    struct group  pwd{};
    struct group* result = nullptr;

    int s = getgrnam_r(group_name.c_str(), &pwd, buf.data(), bufsize, &result);
    if (result == nullptr) {
        if (s == 0)
            return tl::make_unexpected(error("Group not found in the database"));
        else {
            return tl::make_unexpected(error::posix("getgrnam_r", strerror(s)));
        }
    }

    return pwd;
}

tl::expected<user_identity, error> parse_identity(std::string const& username, std::string const& group_name) {
    auto user_info = TRY(wrap_getpwnam_r(username));
    auto group_info = TRY(wrap_getgrnam_r(group_name));

    uint32_t uid = user_info.pw_uid;
    uint32_t gid = group_info.gr_gid;

    return user_identity {uid, gid};
}

tl::expected<file_stat, error> stat_file(std::filesystem::path const& path) {
    struct stat buf {};
    int r = lstat(path.c_str(), &buf);
    if (r != 0) {
        return tl::make_unexpected(error::posix("lstat of " + path.string(), strerror(errno)));
    }

    file_stat res {};

    auto mod = buf.st_mode;

    if (S_ISDIR(mod))
        res.kind = file_kind::directory;
    else if (S_ISREG(mod))
        res.kind = file_kind::regular;
    else
        res.kind = file_kind::other;

    if (mod & S_IRUSR)
        res.mod.owner |= file_mod_one::read;
    if (mod & S_IWUSR)
        res.mod.owner |= file_mod_one::write;
    if (mod & S_IXUSR)
        res.mod.owner |= file_mod_one::execute;

    if (mod & S_IRGRP)
        res.mod.group |= file_mod_one::read;
    if (mod & S_IWGRP)
        res.mod.group |= file_mod_one::write;
    if (mod & S_IXGRP)
        res.mod.group |= file_mod_one::execute;

    if (mod & S_IROTH)
        res.mod.world |= file_mod_one::read;
    if (mod & S_IWOTH)
        res.mod.world |= file_mod_one::write;
    if (mod & S_IXOTH)
        res.mod.world |= file_mod_one::execute;

    res.owner_uid = buf.st_uid;
    res.owner_gid = buf.st_gid;

    return res;
}