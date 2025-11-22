****************
Version Tracking
****************

These tools provide utilities for semantic versioning and component tracking. 
They help developers manage version information and track components throughout
the application lifecycle.


Semantic Version
================

.. doxygenclass:: mge::semantic_version
    :members:


Package
=======

.. doxygenclass:: mge::package
    :members:

A package can be defined by using the :any:`MGE_DEFINE_PACKAGE` macro:

.. doxygendefine:: MGE_DEFINE_PACKAGE
