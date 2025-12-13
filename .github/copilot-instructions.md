# Project Overview

This is a C++ project using cmake and vcpkg for dependency management.

# Coding Standards

* Use consistent naming conventions
    - everything lowercase, with words separated by underscores
    - methods should be verbs
    - classes should be nouns
    - methods returning values must not use "get" or "is" prefix, but just the name of the returned value
* Use style as defined in the `.clang-format` file

# Checking Changes

* Always execute 'cmake --build build' to build the project, do a build always to verify a change
* When checking an error, use 'cmake --build build -j 1' to do a single-threaded build for easier debugging
* After building, run the tests using 'ctest --output-on-failure --test-dir build' to ensure everything is working correctly in build subdirectory.
