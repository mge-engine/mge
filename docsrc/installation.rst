============
Installation
============


Building from source
====================

Prerequisites
-------------

MGE depends ony a number of tools and packages, which need to be installed
before building.


Essential Software Packages
~~~~~~~~~~~~~~~~~~~~~~~~~~~

MGE uses the following software packages, which are supposed to be in
the PATH of the command line used to build. We usually strive to use a
most recent tool chain and environment, and deviating from that may
result in issues:

Git
    Git is a distributed version-control system for tracking changes in
    source code. The source code of MGE is managed using git, and also
    the build process fetches third-party components using git.

CMake
    CMake is a cross-platform build system or build configurator. MGE provides
    a set of CMake scripts which can be used with cmake to set up a native
    build system for your platform.

Python 3
    Several helper scripts use Python, like the extraction tools to generate
    the Vulkan API helper functions.

Optional Software Packages
~~~~~~~~~~~~~~~~~~~~~~~~~~

Ninja
    While cmake can be used to generate Visual Studio project files, or also
    a build environment suitable for *nmake*, ninja is the preferred build tool.


Third-Party Libraries
~~~~~~~~~~~~~~~~~~~~~

MGE makes use of several third party libraries for common functionality,
like storing data, decoding image and mesh formats, or subsystems of the
engine.

Third-party libraries are mostly managed by using *Vcpkg*. An own
for of the global Vcpkg repository is managed by MGE. This allows
associating a snapshot of third-party libraries to a certain release
of MGE, and also patches to components independently of the vcpkg patch
and release strategy.




