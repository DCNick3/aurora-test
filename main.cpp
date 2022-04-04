#include <iostream>

#include "unix.h"


int main() {
    auto r = parse_identity("dcnick3", "dcnick3");

    if (r.has_value()) {
        std::cout << r.value() << std::endl;
    } else {
        std::cout << "Error: " << r.error() << std::endl;
    }

    return 0;
}
