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
* Execute these commands from the root of the repository.

# Commit Messages

* Use present tense ("Add feature" not "Added feature")
* Be concise yet descriptive
* Use the following structure:
    - A short summary of the change (50 characters or less)
    - A blank line
    - A detailed description of the change, if necessary (wrap at 72 characters)
* Do not mention that test passes in the commit message unless the commit is specifically about fixing or adding tests.
