.. _mgecore_exception:

******************
Exception Handling
******************

Base Exception Class
====================

MGE defines a general exception class, :cpp:texpr:`mge::exception` to
manage exceptions and attached additional information.

.. doxygenclass:: mge::exception
   :project: mge
   :members:

Specific Exception Classes
==========================

There are is a number of classes defined for more specific exceptions,
listed below:

.. doxygenclass:: mge::bad_cast
   :project: mge

.. doxygenclass:: mge::duplicate_element
   :project: mge

.. doxygenclass:: mge::illegal_argument
   :project: mge

.. doxygenclass:: mge::illegal_state
   :project: mge

.. doxygenclass:: mge::no_such_element
   :project: mge

.. doxygenclass:: mge::not_yet_implemented
   :project: mge

.. doxygenclass:: mge::null_pointer
   :project: mge

.. doxygenclass:: mge::out_of_range
   :project: mge

.. doxygenclass:: mge::runtime_exception
   :project: mge



Throwing Exceptions
===================

To throw an exception, the :cpp:any:`MGE_THROW`
or the :cpp:any:`MGE_THROW_WITH_CAUSE` macro can be used.

This macro adds the following information to the exception:

- the source file of the exception throw location
- the source line number of the exception throw location
- the function of the exception throw location
- a stack backtrace of the throw location
- the exception class type name

Additional information can be added to the exception using the ``<<``
operator:

.. code-block:: c++

   MGE_THROW(mge::runtime_exception)
       << "there is an " << 331 << " in my hoovercraft";

.. doxygendefine:: MGE_THROW
   :project: mge

.. doxygendefine:: MGE_THROW_WITH_CAUSE
   :project: mge