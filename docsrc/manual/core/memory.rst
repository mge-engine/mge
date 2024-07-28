*****************
Memory Management
*****************

MGE integrates the _jemalloc_ library for general purpose memory management.

For C-style memory management, the following functions are provided:

.. doxygenfunction:: mge::malloc
.. doxygenfunction:: mge::realloc
.. doxygenfunction:: mge::free

For C++-style memory management, a ``std::pmr::memory_resource`` is provided:

.. doxygenclass:: mge::memory_resource
    :members:

    