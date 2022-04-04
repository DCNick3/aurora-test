#include <iostream>

#include "unix.h"


int main() {
    auto r = parse_identity("dcnick3", "dcnick3");

    if (r.has_value()) {
        std::cout << r.value() << std::endl;
    } else {
        std::cout << "Error: " << r.error() << std::endl;
    }

    std::filesystem::path path = "/home/dcnick3/trash/aurora-test/test_dir/dicks";
    auto r1 = stat_file(path);

    if (r1.has_value()) {
        std::cout << r1.value() << " " << path.string() << std::endl;
    } else {
        std::cout << "Error: " << r1.error() << std::endl;
    }

    return 0;
}
