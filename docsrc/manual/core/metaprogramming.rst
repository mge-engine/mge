*****************************
Template Metaprogramming
*****************************

MGE provides type traits, concepts, and template metaprogramming utilities that
enable compile-time introspection and type manipulation.

Callable Detection
==================

Type Trait
----------

.. doxygenclass:: mge::is_callable
    :members:

Variable Template
-----------------

.. doxygenvariable:: mge::is_callable_v

Concept
-------

.. doxygenconcept:: mge::callable

Function Introspection
======================

The :cpp:struct:`mge::function_traits` template provides compile-time
introspection of callable types, extracting information about return type,
argument count (arity), and individual argument types.

.. doxygenstruct:: mge::function_traits
    :members:

Example usage:

.. code-block:: cpp

    auto lambda = [](int x, float y) -> double { return x + y; };
    using traits = mge::function_traits<decltype(lambda)>;
    
    static_assert(traits::arity == 2);
    static_assert(std::is_same_v<traits::result_type, double>);
    static_assert(std::is_same_v<traits::arg<0>::type, int>);
    static_assert(std::is_same_v<traits::arg<1>::type, float>);

Container Type Traits
=====================

Container Concept
-----------------

.. doxygenconcept:: mge::is_container

The :cpp:concept:`mge::is_container` concept identifies types that satisfy
the requirements of a standard container, including iterator support, size
queries, and element access. String types are explicitly excluded.

Sequence Container Concept
---------------------------

.. doxygenconcept:: mge::is_sequence_container

The :cpp:concept:`mge::is_sequence_container` concept identifies sequence
containers that support element insertion and removal operations.

Associative Container Concept
------------------------------

.. doxygenconcept:: mge::is_associative_container

The :cpp:concept:`mge::is_associative_container` concept identifies associative
containers (like :cpp:any:`std::map`) that support key-based element access.

Shared Pointer Detection
=========================

Type Trait
----------

.. doxygenstruct:: mge::is_shared_ptr
    :members:

Variable Template
-----------------

.. doxygenvariable:: mge::is_shared_ptr_v

Type Pack Utilities
===================

Nth Type Extraction
-------------------

.. doxygentypedef:: mge::nth_type

The :cpp:type:`mge::nth_type` alias template extracts the type at index N from
a template parameter pack.

Example usage:

.. code-block:: cpp

    using type = mge::nth_type<1, int, float, double>;
    static_assert(std::is_same_v<type, float>);
