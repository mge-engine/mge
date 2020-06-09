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

Within the namespace ``mge``, the usual C-style memory management functions
as known from ``<stdlib.h>`` are defined:

.. doxygenfunction:: mge::malloc
    :project: mge

.. doxygenfunction:: mge::realloc
    :project: mge

.. doxygenfunction:: mge::calloc
    :project: mge

.. doxygenfunction:: mge::free
    :project: mge


``new`` and ``delete`` Operators
--------------------------------

MGE defines its own ``new`` and ``delete`` operators, which itself call the C
style memory management replacements. These operators are defined in
:file:`mge/core/new.hpp`.


Allocation Statistics
---------------------

There exist a few basic statistics that can be gathered about memory
allocation, also defined in :file:`mge/core/new.hpp`.

.. doxygenfunction:: mge::allocation_count
    :project: mge

.. doxygenfunction:: mge::thread_allocation_count
    :project: mge

An :any:`mge::allocation_count_scope` instance can be used to get a more
fine-grained number of allocations.

.. doxygenclass:: mge::allocation_count_scope
    :project: mge
    :members:

Executable Name
===============

The function :any:`mge::executable_name` can be used to retrieve the
name of the executable. It is defined in :file:`mge/core/executable_name.hpp`.

.. doxygenfunction:: mge::executable_name
    :project: mge