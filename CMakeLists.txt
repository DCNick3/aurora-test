cmake_minimum_required(VERSION 3.13)
project(aurora_test)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG        b9be5b9444772324459989177108a6a65b8b2769
)
FetchContent_MakeAvailable(cli11)

# no tests please
set(EXPECTED_BUILD_TESTS OFF CACHE BOOL "")
FetchContent_Declare(
        expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected
        GIT_TAG        96d547c03d2feab8db64c53c3744a9b4a7c8f2c5
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
)
FetchContent_MakeAvailable(expected)

add_executable(mycheckperm
    main.cpp
    unix.cpp
)
target_link_libraries(mycheckperm
    expected
    CLI11::CLI11
)
