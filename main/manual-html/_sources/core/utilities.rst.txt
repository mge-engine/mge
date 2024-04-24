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

Debugger Breakpoint
-------------------

Sometimes it may be useful to programmatically raise a debug breakpoint,
i.e. stop execution when inside a debuggger. This is abstracted by
:any:`mge::call_debugger`

.. doxygenfunction:: mge::call_debugger