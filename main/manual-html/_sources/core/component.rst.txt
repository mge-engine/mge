**********
Components
**********

Components and implementations are the base of MGEs plugin system.

A *component* is the definition of an interface and registered under a name
as component. Implementations of that interface can be registered, and an
implementation can be selected when creating an instance.

Component Base Class
====================

.. doxygenclass:: mge::component_base
    :members:


Registering Components and Implementations
==========================================

A component is registered using the macro :any:`MGE_REGISTER_COMPONENT`
when the component class is defined. An implementation is registered
using the :any:`MGE_REGISTER_IMPLEMENTATION` macro.

.. doxygendefine:: MGE_REGISTER_COMPONENT

.. doxygendefine:: MGE_REGISTER_IMPLEMENTATION

Modules
=======

Modules are dynamically loadable shared libraries that provide component
implementations. They integrate into the MGE plugin system by registering
implementations during module initialization.

Module Class
------------

.. doxygenclass:: mge::module
    :members:

Loading Modules
---------------

Modules are typically loaded automatically at application startup using
:cpp:func:`mge::module::load_all`, which discovers and loads modules from
configured directories. Modules can also be loaded individually by
constructing a :cpp:class:`mge::module` instance with the path to the
shared library.

