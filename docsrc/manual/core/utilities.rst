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
