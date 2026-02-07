*************
Configuration
*************

Configuration is achieved by declaration of _parameters_ and later
their evaluation at runtime.

Parameter Definition
====================

Parameter Interface
-------------------

An untyped parameter is defined by an instance of
the :any:`mge::basic_parameter` class.

.. doxygenclass:: mge::basic_parameter
    :members:

Typed Parameter
---------------
The parameter interface is further refined into the :any:`mge::parameter`
template class. It allows to retrieve the parameter value as a typed
value.

The following classes are supported by default:

* numeric types like `int`, `float`, `double`
* strings, i.e. `std::string`
* containers of supported types, like `std::vector` or `std::list`
* maps mapping strings to supported types, like `std::map` or `std::unordered_map`
* :any:`mge::properties` for simple key-value pairs

.. doxygenclass:: mge::parameter
    :members:

Helper Macros
-------------

There are macros defined to ease the definition of a parameter, and to access its
value. Note that a defined parameter is not exported from the module, parameters
are intentionally meant to be value within their compilation unit only.

.. doxygendefine:: MGE_DEFINE_PARAMETER

.. doxygendefine:: MGE_PARAMETER

Configuration Management
========================

The configuration is controlled by the class :any:`mge::configuration`.

.. doxygenclass:: mge::configuration
    :members:

For loading the configuration, the following process is used:

#. :any:`mge::executable_name` is used to retrieve the base name
   of the configuration
#. a valid configuration file is looked up, in json format, named
   `<executable_name>.json`
#. The file is read, and the parameters registered are populated.

If a configuration was loaded, the same file type is used when storing it.
If it was not loaded before, a `<executable_name>.json` file is created.

Configuration Exception
========================

.. doxygenclass:: mge::bad_configuration
