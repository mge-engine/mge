*****************
Utilities
*****************

General Utilities
=================

Executable Name
---------------

.. doxygenfunction:: mge::executable_name

Type Name
---------

.. doxygenfunction:: mge::type_name(const std::type_info &ti)

.. doxygenfunction:: mge::type_name()

.. doxygenfunction:: mge::base_type_name(const std::type_info &ti)

.. doxygenfunction:: mge::base_type_name()

.. doxygenfunction:: mge::namespace_name(const std::type_info &ti)

.. doxygenfunction:: mge::namespace_name()

Data structures
===============

String Pool
-----------

A string pool allows a set of constant and non-changing strings to be managed.
It is useful if there is more than one occurence of a string in a context.

.. doxygenclass:: mge::string_pool
    :members:



Programming Helpers
===================

Reference Declaration
---------------------

As a convention, in MGE, `type_ref` is an alias for `std::shared_ptr<type>`.
This declaration can be eased using the macro :any:`MGE_DECLARE_REF`

.. doxygendefine:: MGE_DECLARE_REF

Singleton
---------

To implement the singleton pattern, a helper class :any:`mge::singleton`
can be used:

.. doxygenclass:: mge::singleton
    :members:
