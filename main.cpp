#include <iostream>

#include "unix.h"

tl::expected<tl::monostate, error> print_file_mod(std::filesystem::path const& path) {
    auto stat = TRY(stat_file(path));

    std::cout << stat << " " << path.string() << "\n";

    return tl::monostate();
}

tl::expected<tl::monostate, error> print_if_writable(std::filesystem::path const& path, user_identity const& user) {
    auto stat = TRY(stat_file(path));
    auto access = get_access(stat, user);
    if (access & file_mod_one::write && stat.kind != file_kind::other) {
        std::cout << stat.kind << " " << path.string() << "\n";
    }
}

tl::expected<tl::monostate, error> real_main() {
    namespace fs = std::filesystem;
    fs::path path = "/home/dcnick3/trash/aurora-test/test_dir";

    auto user = TRY(parse_identity("dcnick3", "dcnick3"));
    std::cout << user << std::endl;

    for (const fs::directory_entry& dir_entry :
            fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied))
    {
        // ignore errors
        print_if_writable(dir_entry.path(), user);
    }

    return tl::monostate();
}

int main() {

    auto r = real_main();

    if (!r.has_value())
        std::cout << "Error: " << r.error() << std::endl;

    return 0;
}
