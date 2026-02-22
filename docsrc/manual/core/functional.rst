**********************************
Functional Programming Patterns
**********************************

MGE provides utilities that enable functional programming patterns and idioms
in C++.

Closures
========

The closure system provides a mechanism to create function pointers that can
bind to C++ lambdas and member functions, using libffi for dynamic code
generation. This enables interoperability with C APIs that expect function
pointers.

Base Class
----------

.. doxygenclass:: mge::closure_base
    :members:

Closure Template
----------------

.. doxygenclass:: mge::closure
    :members:

Scope Guards
============

The :cpp:class:`mge::on_leave` template implements the scope guard pattern,
executing a callable when the object leaves scope. This is useful for resource
cleanup and ensuring actions are performed regardless of how a scope is exited.

.. cpp:class:: template<typename Callable> mge::on_leave

    Execute a callable on leaving the current scope.

    :tparam Callable: Type of callable to execute

Example usage:

.. code-block:: cpp

    {
        FILE* f = fopen("file.txt", "r");
        mge::on_leave cleanup([f]() { fclose(f); });
        // ... work with file ...
    } // cleanup is called automatically

Visitor Pattern Helper
======================

The :cpp:class:`mge::overloaded` struct template simplifies the visitor pattern
by combining multiple lambda expressions or functors into a single overload set.
This is particularly useful with ``std::variant`` and ``std::visit``.

.. doxygenstruct:: mge::overloaded
    :members:

Example usage:

.. code-block:: cpp

    std::variant<int, float, std::string> v = 42;
    std::visit(mge::overloaded{
        [](int i) { std::cout << "int: " << i; },
        [](float f) { std::cout << "float: " << f; },
        [](const std::string& s) { std::cout << "string: " << s; }
    }, v);
