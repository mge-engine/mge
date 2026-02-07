*****************
Utilities
*****************

General Utilities
=================

Executable Name and Path
------------------------

.. doxygenfunction:: mge::executable_name

.. doxygenfunction:: mge::executable_path

Type Name
---------

.. doxygenfunction:: mge::type_name(const std::type_info &ti)

.. doxygenfunction:: mge::type_name()

.. doxygenfunction:: mge::base_type_name(const std::type_info &ti)

.. doxygenfunction:: mge::base_type_name()

.. doxygenfunction:: mge::namespace_name(const std::type_info &ti)

.. doxygenfunction:: mge::namespace_name()

Stack Backtrace
---------------

A stack backtrace can be useful in diagnostics to determine the position of an
error. To retrieve a stack backtrace programatically, :any:`mge::stacktrace`
can be used.

.. doxygenclass:: mge::stacktrace
    :members:

A stacktrace can be printed using the standard `<<` stream operator.

Data structures
===============

String Pool
-----------

A string pool allows a set of constant and non-changing strings to be managed.
It is useful if there is more than one occurence of a string in a context.

.. doxygenclass:: mge::string_pool
    :members:

Property Map
------------

A property map allows to manage key-value pairs of strings. 

.. doxygenclass:: mge::properties
    :members:

Property Object
---------------

The property object provides a base class for objects that support property
management with type-safe accessors.

.. doxygenclass:: mge::property_object
    :members:

Enumeration Utilities
=====================

Enum Set
--------

The :cpp:class:`mge::enum_set` template provides a type-safe bitset indexed
by enum values.

.. doxygenclass:: mge::enum_set
    :members:

Magic Enum Integration
----------------------

MGE integrates the magic_enum library to provide reflection capabilities for
enumerations. The following utilities are available in the :cpp:any:`mge`
namespace:

Type Traits:

- :cpp:any:`mge::is_scoped_enum` - Check if enum is scoped
- :cpp:any:`mge::is_scoped_enum_v` - Variable template version
- :cpp:any:`mge::is_unscoped_enum` - Check if enum is unscoped
- :cpp:any:`mge::is_unscoped_enum_v` - Variable template version
- :cpp:any:`mge::underlying_type` - Get underlying type of enum
- :cpp:any:`mge::underlying_type_t` - Alias template for underlying type

Enum Functions:

- :cpp:any:`mge::enum_cast` - Convert string to enum value
- :cpp:any:`mge::enum_name` - Get name of enum value
- :cpp:any:`mge::enum_type_name` - Get name of enum type
- :cpp:any:`mge::enum_contains` - Check if value is valid enum
- :cpp:any:`mge::enum_count` - Get number of enum values
- :cpp:any:`mge::enum_values` - Get array of all enum values
- :cpp:any:`mge::enum_entries` - Get array of name-value pairs
- :cpp:any:`mge::enum_index` - Get index of enum value
- :cpp:any:`mge::enum_integer` - Convert enum to integer

Flags Functions:

- :cpp:any:`mge::enum_flags_cast` - Convert string to enum flags
- :cpp:any:`mge::enum_flags_name` - Get name of enum flags
- :cpp:any:`mge::enum_flags_contains` - Check flags contain value

To Underlying
-------------

Convert enum values to their underlying integer type.

.. doxygenfunction:: mge::to_underlying

Cleanup Handlers
================

AtExit
------

Register callbacks to be executed at program termination.

.. doxygenclass:: mge::atexit
    :members:

Programming Helpers
===================

Reference Declaration
---------------------

As a convention, in MGE, `type_ref` is an alias for `std::shared_ptr<type>`.
This declaration can be eased using the macro :any:`MGE_DECLARE_REF`

.. doxygendefine:: MGE_DECLARE_REF

Noncopyable
-----------

A noncopyable object is one that has its copy constructor and
assignment operatpr deleted.

..  doxygenclass:: mge::noncopyable
    :members:

Patterns and Algorithms
=======================

Singleton
---------

To implement the singleton pattern, a helper class :any:`mge::singleton`
can be used:

..  doxygenclass:: mge::singleton
    :members:

Container Element Check
-----------------------

:any:`mge::contains` can be used to check whether an element is in a
container. Be aware this may have different complexity, depending on
the container.

.. doxygenfunction:: mge::contains(const Container &, const Element &)

Callback Map
------------

A callback map is used to manage callbacks or _listeners_. It manages
a map of keys and callbacks, where the callback key can be used to remove
a callback again, as function objects can't be compared.

.. doxygenclass:: mge::callback_map
    :members:

Copy of Structures
------------------

Plain structures, and POD types do not define an assignment operator.
Fumbling around with `memcpy` and `sizeof` may be a source of error,
so this is encapsulated in :any:`mge::copy_struct`.

.. doxygenfunction:: mge::copy_struct

Clear Memory of Structures
--------------------------

Unlike constructed objects, plain structures can be uninitialized and
it may be necessary to fill them with zeros. Again, handling
`sizeof` can be a source of error, so this is encapsulated in
:any:`mge::zero_memory`.

.. doxygenfunction:: mge::zero_memory

Checked Type Cast
-----------------

Type casting may produce unexpected results on overflows, i.e. an overflow
is usually not detected but produces a weird value. A checked cast provides
check logic that prevents overflows and unexpected truncation of values.

.. doxygenfunction:: mge::checked_cast

Debugging
---------

C++26 proposes an API to interact with the debugger, i.e.
- raise a breakpoint
- check if a debugger is attached
- check if a debugger is attached and break
The proposed API is implemented in the ``mge`` namespace.

.. doxygenfunction:: mge::breakpoint

.. doxygenfunction:: mge::breakpoint_if_debugging 

.. doxygenfunction:: mge::is_debugger_present

Iterator Utilities
------------------

Convert iterator to index within container.

.. doxygenfunction:: mge::iterator_index

Lexical Cast
------------

Convert between types using string representation.

.. doxygenfunction:: mge::lexical_cast
