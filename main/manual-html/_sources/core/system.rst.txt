*******************
System Abstractions
*******************

Some abstractions of common system functionality is provided, if that
abstraction is not already covered by the standard library.

System Error
============

The system error captures the last - or a provided - system
error and also resolves its message.

..  doxygenclass:: mge::system_error
    :members:

Shared Library
==============

Shared libraries can be loaded and inspected using the :any:`mge::shared_library`
class:

..  doxygenclass:: mge::shared_library
    :members:
