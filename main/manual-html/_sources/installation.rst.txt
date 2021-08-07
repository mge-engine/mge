************
Installation
************

SDK and Engine Installation
###########################

.. attention::
   Due to its early state, MGE does not yet provide an installation package
   yet. Please build the MGE engine and libraries from source.

Building From Source
####################

Prerequisites
*************

MGE depends ony a number of tools and packages, which need to be installed
before building:

Microsoft Visual C++ 2019
    MGE currently is built using this compile tool chain.
    The *Community Edition* that can be obtained at
    `https://visualstudio.microsoft.com <https://visualstudio.microsoft.com/>`_
    is sufficient.

`Git <https://git-scm.com/>`_
    Git is a distributed version-control system for tracking changes in
    source code. The source code of MGE is managed using git, and also
    the build process fetches third-party components using git.

`CMake <https://cmake.org/>`_
    CMake is a cross-platform build system or build configurator. MGE provides
    a set of CMake scripts which can be used with cmake to set up a native
    build system for your platform.

`Python 3 <https://www.python.org/>`_
    Several helper scripts use Python, like the extraction tools to generate
    the Vulkan API helper functions.

`Vulkan SDK <https://vulkan.lunarg.com/sdk/home>`_
    Vulkan is a low-overhead, cross-platform graphics and computing API. It is
    used as a possible graphics backend of MGE.

.. note::
   It's useful to use a package manager like `Scoop <https://scoop.sh/>`_ or
   `Chocolatey <https://chocolatey.org/>`_ for installing the required tools,
   to keep them up to date and install updates easily.

Optional Software Packages
**************************

`Ninja <https://ninja-build.org/>`_
    While cmake can be used to generate Visual Studio project files, or also
    a build environment suitable for *nmake*, ninja is the preferred build tool.

Build Steps
***********

Obtaining the Source
====================

Before compiling MGE, you actually need to get the sources. Sources of MGE are
managed in `Github <https://github.com/mge-engine/mge>`_. While you can download
MGE as an archive, you usually want to obtain the sources by cloning the git
repository:

.. code-block::

    C:\>git clone https://github.com/mge-engine/mge.git

After obtaining the sources, you can continue with configuring third-party
libraries.

Configuring Third-Party Libraries
=================================

Third-party libraries are managed by the `vcpkg` tool. The MGE project maintains
an own fork of `vcpkg`, to tag specific versions of vcpkg for usage with MGE.

This tagging is achieved by a helper script, which ensures that
- vcpkg is retrieved from the MGE mirror
- the matching version of vcpkg is used
- all used packages are installed

To setup the libraries, just change into the source code directory and execute

.. code-block::

    C:\mge>python3 tools\ci\vcpkg.py

which will checkout `vcpkg`, bootstrap it, and install all packages. This may
take a while.

Configuring Documentation Tools
===============================

If you need to build the documentation from source, a few additional packages
are required to be installed. You can usually skip this, as documentation is
provided at the `MGE github.io site <https://mge-engine.github.io/mge/main/manual-html/index.html>`_.

The tools needed are `doxygen` and `sphinx`.

Doxygen can be installed from the download, or by using one of the previously
mentioned package managers.

Sphinx is usually installed into your local python installation using the `pip`
package manager:

.. code-block::

    C:\>python -m pip install sphinx breathe sphinx_rtd_theme

You may want to use a `virtualenv` to separate these packages from your
system-provided Python installation.

Please make sure the installed tools are in the PATH before configuring the build.

Configuring the Build
=====================

To configure, you need to ensure that you are in a `cmd` environment that
has the correct settings

.. code-block::

    C:\>SET INCLUDE=
    C:\>SET LIB=
    C:\>call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    C:\>set CC=cl
    C:\>set CXX=cl

Note the file for setting the Microsoft Visual Studio command prompt may be at
a different location, depending on your installation.

Make sure `cmake` and Python 3 are also in the PATH and can be called.

MGE does not support so-called *in-source* builds, so you need to create a new
directory that is used for the build:

.. code-block::

    C:\mge>mkdir build
    C:\mge>cd build

Within the `build` directory, use `cmake` to configure the build:

.. code-block::

    C:\mge\build>cmake -GNinja .. -DCMAKE_BUILD_TYPE=RelWithDebInfo

CMake allows to configure different build types, the commonly chosen one is
`RelWithDebInfo` - release with debug informations.

Build MGE
=========

The build can simply be started after successful configuring by also
calling `cmake`:

.. code-block::

    C:\mge\build>cmake --build . --target all

Again, this may take a while.

Test MGE
========

After building, you may want to run the unit-tests to ensure a clean build, or
also to ensure proper quality if you have made a change to the source code:

The tests can be simply started by running `ctest`:

.. code-block::

    C:\mge\build>ctest

All unit test programs start with `test_` and are located in the main build
directory, and can be also simply executed and debugged on their own.

Building Documentation
======================

The documentation build can also be started by calling `cmake`:

.. code-block::

    C:\mge\build>cmake --build . --target documentation

The generated documentation is in the folder `docsrc/manual/manual-html` of
the build directory.
