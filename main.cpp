#include "unix.h"

#include <iostream>

#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

tl::expected<tl::monostate, error> print_file_mod(std::filesystem::path const& path) {
    auto stat = TRY(stat_file(path));

    std::cout << stat << " " << path.string() << "\n";

    return tl::monostate();
}

tl::expected<tl::monostate, error> print_if_writable(std::filesystem::path const& path, user_identity const& user) {
    auto stat = TRY(stat_file(path));
    auto access = get_access(stat, user);
    auto can_write = access & file_mod_one::write || user.uid == 0; // root can do anything!
    if (can_write && stat.kind != file_kind::other) {
        std::cout << stat.kind << " " << path.string() << "\n";
    }
    return tl::monostate();
}

tl::expected<tl::monostate, error> real_main(std::filesystem::path const& path, std::string const& username, std::string const& group_name) {
    namespace fs = std::filesystem;

    auto user = TRY(parse_identity(username, group_name));

    for (const fs::directory_entry& dir_entry :
            fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied))
    {
        // ignore errors
        print_if_writable(dir_entry.path(), user);
    }

    return tl::monostate();
}

int main(int argc, char** argv) {
    CLI::App app{"A tool to find all files/directories writable by a user & group combination"};
    app.option_defaults()->required();

    std::filesystem::path root_path;
    app.add_option("-p", root_path, "Path to directory to start the traversal from");

    std::string username;
    app.add_option("-u", username, "Username to check the writability against");

    std::string group_name;
    app.add_option("-g", group_name, "Group name to check the writability against");

    CLI11_PARSE(app, argc, argv);

    auto r = real_main(root_path, username, group_name);

    if (!r.has_value())
        std::cout << "Error: " << r.error() << std::endl;

    return 0;
}
