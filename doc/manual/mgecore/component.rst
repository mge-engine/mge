.. _mgecore_component:

**********
Components
**********

Components and implementations are the base of MGEs plugin system.

A *component* is the definition of an interface and registered under a name
as component. Implementations of that interface can be registered, and an
implementation can be selected when creating an instance.

Component Base Class
--------------------

.. doxygenclass:: mge::component
   :project: mge
   :members:


Registering Components and Implementations
------------------------------------------


Modules
-------

Usually, implementations of interfaces reside in modules,
i.e. shared libraries.