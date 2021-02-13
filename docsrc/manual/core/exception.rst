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
