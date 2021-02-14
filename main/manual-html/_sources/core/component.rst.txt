**********
Components
**********

Components and implementations are the base of MGEs plugin system.

A *component* is the definition of an interface and registered under a name
as component. Implementations of that interface can be registered, and an
implementation can be selected when creating an instance.

Component Base Class
====================

.. doxygenclass:: mge::component
    :members:


Registering Components and Implementations
==========================================

A component is registered using the macro :any:`MGE_REGISTER_COMPONENT`
when the component class is defined. An implementation is registered
using the :any:`MGE_REGISTER_IMPLEMENTATION` macro.

.. doxygendefine:: MGE_REGISTER_COMPONENT

.. doxygendefine:: MGE_REGISTER_IMPLEMENTATION

