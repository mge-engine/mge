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

.. doxygendefine:: MGE_COMPILER_MSVC
    :project: mge
.. doxygendefine:: MGE_COMPILER_GNUC
    :project: mge
.. doxygendefine:: MGE_COMPILER_MINGW
    :project: mge
.. doxygendefine:: MGE_COMPILER_CLANG
    :project: mge

Operating System
----------------

.. doxygendefine:: MGE_OS_LINUX
    :project: mge
.. doxygendefine:: MGE_OS_WINDOWS
    :project: mge
.. doxygendefine:: MGE_OS_MACOSX
    :project: mge
.. doxygendefine:: MGE_OS_UNIX
    :project: mge

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
