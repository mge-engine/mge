******************
Exception Handling
******************

MGE defines a base exception class. This basic class allows to attach additional
informations to the exception, such as location information and stack traces.

Base Exception class
====================

.. doxygenclass:: mge::exception
    :members:

Throwing Exceptions
===================

The macro `MGE_THROW` can help in composing an exception.

.. doxygendefine:: MGE_THROW

If this is an exception translation, i.e. a catched exception is translated into
a different error, using `MGE_THROW_WITH_CAUSE` is more appropriate:

.. doxygendefine:: MGE_THROW_WITH_CAUSE

To attach extra info like the called function, the macro `MGE_CALLED_FUNCTION`
can be used like this:

.. code-block:: c++

    MGE_THROW(mge::exception)
        << MGE_CALLED_FUNCTION(glGenBuffers)
        << "Cannot generate buffers";

.. doxygendefine:: MGE_CALLED_FUNCTION

Standard Exceptions
===================

For a lot of use cases, standard exceptions are provided.

.. doxygenclass:: mge::illegal_state

.. doxygenclass:: mge::illegal_argument

.. doxygenclass:: mge::out_of_range

.. doxygenclass:: mge::duplicate_element

.. doxygenclass:: mge::bad_cast

.. doxygenclass:: mge::no_such_element

.. doxygenclass:: mge::runtime_exception

.. doxygenclass:: mge::not_yet_implemented

.. doxygenclass:: mge::null_pointer

.. doxygenclass:: mge::out_of_memory

.. doxygenclass:: mge::numeric_overflow

.. doxygenclass:: mge::not_implemented

.. doxygenclass:: mge::pure_virtual_method