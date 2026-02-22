*****************************
Template Metaprogramming
*****************************

MGE provides type traits, concepts, and template metaprogramming utilities that
enable compile-time introspection and type manipulation.

Callable Detection
==================

Type trait and concept for detecting if a type is callable.

.. cpp:class:: template<typename T> mge::is_callable

    Type trait to check if a type is callable.

    .. cpp:member:: static constexpr bool value

        True if T is callable, false otherwise.

.. cpp:var:: template<typename T> inline constexpr bool mge::is_callable_v

    Helper variable template to check if a type is callable.

.. cpp:concept:: template<typename T> mge::callable

    Concept to check if a type is callable.

Function Introspection
======================

The ``mge::function_traits`` template provides compile-time
introspection of callable types, extracting information about return type,
argument count (arity), and individual argument types.

.. cpp:struct:: template<typename T> mge::function_traits

    Function traits for compile-time introspection.

    .. cpp:member:: static constexpr auto arity

        Number of arguments.

    .. cpp:type:: result_type

        Return type of the function.

    .. cpp:struct:: template<size_t I> arg

        Argument type accessor.

        .. cpp:type:: type

            Type of the I-th argument.

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
containers (like ``std::map``) that support key-based element access.

Shared Pointer Detection
=========================

.. cpp:struct:: template<typename T> mge::is_shared_ptr

    Type trait to detect std::shared_ptr types.

    .. cpp:member:: static constexpr bool value

        True if T is std::shared_ptr, false otherwise.

.. cpp:var:: template<typename T> inline constexpr bool mge::is_shared_ptr_v

    Helper variable template for shared pointer detection.

Type Pack Utilities
===================

Nth Type Extraction
-------------------

.. cpp:type:: template<std::size_t N, typename... Ts> mge::nth_type

    Extract the type at index N from a template parameter pack.

Example usage:

.. code-block:: cpp

    using type = mge::nth_type<1, int, float, double>;
    static_assert(std::is_same_v<type, float>);
