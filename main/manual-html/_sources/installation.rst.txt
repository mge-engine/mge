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

MGE depends on a number of tools and packages, which need to be installed
before building. The following subsections detail the requirements for each
supported platform.

Microsoft Windows
=================

Microsoft Visual C++ 2022
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
    used as a possible graphics backend of MGE. The SDK can be installed using
    the provided helper script:

    .. code-block::

        C:\mge>python tools\ci\vulkan.py

`Ninja <https://ninja-build.org/>`_
    While cmake can be used to generate Visual Studio project files, or also
    a build environment suitable for *nmake*, ninja is the preferred build tool.
    Install via Chocolatey:

    .. code-block::

        C:\>choco install ninja

.. note::
   It's useful to use a package manager like `Scoop <https://scoop.sh/>`_ or
   `Chocolatey <https://chocolatey.org/>`_ for installing the required tools,
   to keep them up to date and install updates easily.

Linux
=====

MGE is currently supported only on Debian/Ubuntu based distributions.

GCC 14 Compiler
    MGE requires GCC 14 or later. For Ubuntu:

    .. code-block::

        $ sudo apt-get install gcc-14 g++-14

`Git <https://git-scm.com/>`_
    Git is typically included in most distributions or can be installed via
    the package manager:

    .. code-block::

        $ sudo apt-get install git  # Debian/Ubuntu

`CMake <https://cmake.org/>`_
    Install CMake from your distribution's package manager. For Ubuntu, using
    snap is recommended:

    .. code-block::

        $ sudo snap install cmake --classic

`Python 3 <https://www.python.org/>`_
    Python 3 is typically pre-installed on most Linux distributions.

`Vulkan SDK <https://vulkan.lunarg.com/sdk/home>`_
    Download and install the Vulkan SDK from the LunarG website, or install
    via package manager if available.

`Ninja <https://ninja-build.org/>`_
    Install Ninja build system:

    .. code-block::

        $ sudo apt-get install ninja-build  # Debian/Ubuntu

Build Tools
    Install automake and related tools, they are required for building some of the third-party dependencies:

    .. code-block::

        $ sudo apt-get install automake autoconf autoconf-archive libtool

Mac OS X
========

Xcode Command Line Tools
    Install the Xcode Command Line Tools:

    .. code-block::

        $ xcode-select --install

`Homebrew <https://brew.sh/>`_
    Homebrew is recommended as a package manager for macOS. Install it if
    you don't have it already.

`Git <https://git-scm.com/>`_
    Git is included with Xcode Command Line Tools, or install via Homebrew:

    .. code-block::

        $ brew install git

`CMake <https://cmake.org/>`_
    Install CMake via Homebrew:
    
    .. code-block::

        $ brew install cmake

`Python 3 <https://www.python.org/>`_
    Install Python 3 via Homebrew:

    .. code-block::

        $ brew install python3

`Vulkan SDK <https://vulkan.lunarg.com/sdk/home>`_
    Download and install the Vulkan SDK from the LunarG website. On Mac OS X,
    Vulkan is implemented via MoltenVK which translates Vulkan to Metal.

`Ninja <https://ninja-build.org/>`_

    Install Ninja build system:
    .. code-block::

        $ brew install ninja

Automake and related tools
    Install automake and related tools via Homebrew:

    .. code-block::

        $ brew install automake autoconf autoconf-archive libtool


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

Building the third-party packages is done when configuring the build.

Configuring Documentation Tools
===============================

If you need to build the documentation from source, a few additional packages
are required to be installed. You can usually skip this, as documentation is
provided at the `MGE github.io site <https://mge-engine.github.io/mge/main/manual-html/index.html>`_.

The tools needed are `doxygen` and `sphinx`.

Doxygen can be installed from the download, or by using one of the previously
mentioned package managers.

Sphinx can be installed using the supplied helper script:

.. code-block::

    C:\mge>python3 tools\ci\sphinx.py

This will create a so called 'virtualenv' in the mge directory, and
install the needed python packages there.

Configuring the Build
=====================

Microsoft Windows
-----------------

Ensure you are in a properly configured Visual Studio environment. The MSVC
compiler and environment need to be set up:

.. code-block::

    C:\>SET INCLUDE=
    C:\>SET LIB=
    C:\>CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    C:\>SET CXX=cl.exe
    C:\>SET CC=cl.exe
    C:\>SET VCPKG_ROOT=C:\mge\vcpkg
    C:\>SET PATH=%VCPKG_ROOT%;%PATH%

If building documentation, activate the Sphinx environment:

.. code-block::

    C:\mge>sphinx\Scripts\activate.bat

Set up the Vulkan SDK:

.. code-block::

    C:\mge>SET VULKAN_SDK=C:\VulkanSDK\1.4.309.0

Configure the build using CMake:

.. code-block::

    C:\mge>cmake --preset=default

This configures the build for the `RelWithDebInfo` configuration and installs
all dependencies using `vcpkg`. It may take a while.

Linux
-----

Set the compiler to GCC 14:

.. code-block::

    $ export CC=gcc-14
    $ export CXX=g++-14
    $ export VCPKG_ROOT=$HOME/mge/vcpkg

Configure the build using CMake:

.. code-block::

    $ cmake --preset=default


This configures the build and installs all dependencies using `vcpkg`. It may
take a while.

Mac OS X
--------

Set up the vcpkg root:

.. code-block::

    $ export VCPKG_ROOT=$HOME/mge/vcpkg

Configure the build using CMake:

.. code-block::

    $ cmake --preset=default


This configures the build and installs all dependencies using `vcpkg`. It may
take a while.

Build MGE
=========

Microsoft Windows
-----------------

Build the project using CMake:

.. code-block::

    C:\mge>cmake --build build --parallel 8 --target all

Linux
-----

Build the project using CMake:

.. code-block::

    $ cmake --build build --parallel 8 --target all

Mac OS X
--------

Build the project using CMake:

.. code-block::

    $ cmake --build build --parallel 4 --target all

Test MGE
========

After building, run the unit tests to ensure a clean build:

Microsoft Windows
-----------------

.. code-block::

    C:\mge\build>ctest --verbose

Linux
-----

.. code-block::

    $ cd build
    $ ctest --verbose

Mac OS X
--------

.. code-block::

    $ cd build
    $ ctest --verbose

All unit test programs start with `test_` and are located in the main build
directory, and can also be executed individually for debugging.

Building Documentation
======================

The documentation build can also be started by calling `cmake`:

.. code-block::

    C:\mge>cmake --build build --target documentation

The generated documentation is in the folder `docsrc/manual/manual-html` of
the build directory.
