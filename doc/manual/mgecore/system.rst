.. _mgecore_system:

*****************
System Facilities
*****************

System Identification
=====================

While MGE strives for portability, it acknowledges it is sometimes
necessary to identify the nature of the environment. So, some
identification macros are provided, in :file:`mge/config.hpp`.

Compiler
--------

.. c:macro:: MGE_COMPILER_MSVC

    Defined if the compiler is Microsoft Visual C++.

.. c:macro:: MGE_COMPILER_GNUC

    Defined if the compiler is GNU C++.

.. c:macro:: MGE_COMPILER_MINGW

    Defined if the compiler is MinGW (in this case also
    `MGE_COMPILER_GNUC`` is defined).`

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

Clock
=====

A raw clock that measures time span since program start is offered
in :file:`mge/core/clock.hpp`.

.. doxygenclass:: mge::clock
    :project: mge
    :members:

Memory Management
=================

C-Style Memory Management
-------------------------

``new`` and ``delete`` Operators
--------------------------------

Allocation Statistics
---------------------