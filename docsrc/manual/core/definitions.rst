***********
Definitions
***********

System Identification
=====================

While MGE strives for portability, it acknowledges it is sometimes
necessary to identify the nature of the environment. So, some
identification macros are provided, in :file:`mge/config.hpp`.

.. note::
    While MGE targets currently only Microsoft Visual Studio,
    some other definitions are provided, however no implementation exist for
    other    operating systems, compilers, and CPU architectures

Compiler
--------

.. c:macro:: MGE_COMPILER_MSVC

    Defined if the compiler is Microsoft Visual C++.

.. c:macro:: MGE_COMPILER_GNUC

    Defined if the compiler is GNU C++.

.. c:macro:: MGE_COMPILER_MINGW

    Defined if the compiler is MinGW (in this case also
    `MGE_COMPILER_GNUC` is defined).`

.. c:macro:: MGE_COMPILER_CLANG

    Defined if the compiler is ``clang``.

Operating System
----------------

.. c:macro:: MGE_OS_LINUX

    Defined if the operating system is Linux.

.. c:macro:: MGE_OS_WINDOWS

    Defined if the operating system is Microsoft Windows.

.. c:macro:: MGE_OS_MACOSX

    Defined if the operating system is Mac OS X.

.. c:macro:: MGE_OS_UNIX

    Defined if the operating system is a Unix-style system, like
    Linux and Mac OS X.

Platform Namespace
------------------

By convention, platform specific code resides under a specific namespace,
as follows:

+-------------------+------------+
| Platform          | Namespace  |
+===================+============+
| Microsoft Windows | ``win32``  |
+-------------------+------------+
| Linux             | ``linux``  |
+-------------------+------------+
| Mac OS X          | ``macos``  |
+-------------------+------------+

Attributes
==========

Attributes are additional tags on declarations that influence how the compiler
interprets them (and what conditions need to be warned on).

Common Attributes
-----------------

.. c:macro:: MGE_NO_RETURN

    Attribute that describes a function does not return
    (`[[noreturn]] <https://en.cppreference.com/w/cpp/language/attributes/noreturn>`_
    in C++11).

.. c:macro:: MGE_NO_INLINE

    Attribute forcing a function not to be inlined.

Symbol Visiblity
----------------

The file :file:`mge/dllexport.hpp` defines some macros used for symbol visiblity.

.. doxygendefine:: MGE_DLLEXPORT
    :project: mge

.. doxygendefine:: MGE_DLLIMPORT
    :project: mge

Utilities
=========

.. c:macro:: MGE_FUNCTION_SIGNATURE

    Expands to the function signature as text.

