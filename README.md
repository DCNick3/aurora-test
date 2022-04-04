## overview

I decided to use C++ with CMake build system for the task

First of all, I've written C++ wrappers for Linux C functions I needed to get uid & gid by usernames along with file permissions.

In there I used [TartanLlama/expected](https://github.com/TartanLlama/expected) library which is an implementation of https://wg21.link/P0323R11 proposal to handle errors nicely.

This piece of code is located in `unix.cpp` file

The algorithm itself is located in `main.cpp` file. It uses `std::filesystem::recursive_directory_iterator` to traverse the directory tree and functions from `unix.cpp` to get the info necessary for deciding whether the user can write the file or not.

Finally, I use [CLIUtils/CLI11](https://github.com/CLIUtils/CLI11) to parse the command line arguments and provide a nice help message

## how to build & run

Build should as easy as this one-liner:

```
rm -rf cmake-build-debug/ && mkdir -p cmake-build-debug/ && cd cmake-build-debug/ && cmake .. && cmake --build .; cd ..
```

Cmake will download the required libraries and build the program

After this it can be ran as

```
sudo ./cmake-build-debug/mycheckperm -u username -g groupname -p /home/myname/testdir/
```