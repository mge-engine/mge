.. _mgecore_file:

***********
File System
***********

The file system abstraction is somewhat a remainder to the time when
``std::filesystem``, which was introduced by C++17 did not exist.
It somewhat complements that functionality, and provides also helpers
for ease read and write of binary data.

File Access
-----------

Basic access is provided by the :any:`mge::file` class.

.. doxygenclass:: mge::file
    :project: mge
    :members:

File errors are signalled by two exceptions, :any:`mge::file_not_found`
and :any:`mge::filesystem_error`.

.. doxygenclass:: mge::file_not_found
    :project: mge
    :members:

.. doxygenclass:: mge::filesystem_error
    :project: mge
    :members:


Implementing File Access
------------------------

File access is handled by an implementation of the :any:`mge::file_access`
interface.

.. doxygenclass:: mge::file_access
    :project: mge
    :members:

Such a file access object is internally created by the factory, which implements
:any:`mge::file_access_factory`:

.. doxygenclass:: mge::file_access_factory
    :project: mge
    :members:

A file access factory implementing the native file system exists always.

Input and Output Streams
------------------------

It is a common task to read and write data from and to files or other sources,
MGE provides some helper classes - :any:`mge::input_stream` and
:any:`mge::output_stream`.

.. doxygenclass:: mge::input_stream
    :project: mge
    :members:

.. doxygenclass:: mge::output_stream
    :project: mge
    :members:

An error in streams is usually signalled by an :any:`mge::io_error`.

.. doxygenclass:: mge::io_error
    :project: mge

