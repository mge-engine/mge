# Project Structure and Conventions
## Used Tools and Languages
### C++
MGE is written in C++. The subset of C++20 supported by Microsoft Visual C++ defines the language support level in use. Once Microsoft Visual C++ fully supports C++20 the project will stick to it.
### CMake
CMake is used to configure and build MGE.
### Ninja
Ninja is the default build tool.
### Vcpkg
[Vcpkg](https://github.com/microsoft/vcpkg) is used to manage third-party libraries and components. To be able to support stable versions, vcpkg is forked into the mge-engine
Github organization (https://github.com/mge-engine/vcpkg).
### Clang-format
Clang-format is used to achieve a consistent formatting of the source code.
## Conventions
### Namespace
All code resides in name space `mge`.
### Names
#### Case
* Names of classes, structures, methods and variables are in lower case letters.
* Names are written in _snake_case_.
* Names of enumeration values, constants and global values use upper case letters.
#### Simplicity
* Common prefixes and suffixes like _get_, _set_, _type_ should be avoided.
* If a method or type can be described by a name used in the C++ standard library, that name shall be used if possible.
* Names should be mentioned in the [Name Glossary](NameGlossary.md) to foster conceptual integrity.
* Functional interfaces should be avoided in favor of function types.
### Scope
The smallest possible scopes shall be used:
* Global enum (not enum classes) shall be avoided.
* Internal implementation in public headers should be placed in the nameless namespace.
* If internal implementation has to be spread over files for technical reasons, the `detail` namespace is used.

## Development Practices
* Headless functionality should be accompanied with a unit test.
* Tests for functionality that requires a screen device should be placed in an appropriately marked test, if testing is possible.
* A sample shall be added for more sophisticated functionality.
* Parts deemed critical should be thoroughly benchmarked.

