********************
Container Data Types
********************

MGE provides specialized container data structures optimized for specific use
cases in game engine development.

Small Vector
============

The ``mge::small_vector`` template is a vector-like container that
uses stack allocation for small numbers of elements, avoiding heap allocation
overhead for common small-size use cases. When the number of elements exceeds
the specified size, it automatically transitions to heap allocation.

.. cpp:class:: template<typename T, size_t S, class Alloc = std::allocator<T>> mge::small_vector

    A small vector to avoid allocation up to a certain size of elements.

    :tparam T: Element type
    :tparam S: Stack-allocated element capacity
    :tparam Alloc: Allocator type (default: std::allocator<T>)

Handle
======

The ``mge::handle`` template provides a type-safe handle mechanism for
managing objects within a specific context. Handles consist of a context index,
flags, and an object index, enabling efficient object lookup through the
associated context.

.. cpp:class:: template<typename Object, typename Context> mge::handle

    Handle to an object managed by a specific context.

    :tparam Object: Type of the managed object
    :tparam Context: Type of the context managing the object

LRU Cache
=========

The ``mge::lru_cache`` template implements a least-recently-used cache
that automatically evicts the oldest accessed items when the cache reaches its
maximum size.

.. cpp:class:: template<typename K, typename V> mge::lru_cache

    LRU cache implementation.

    :tparam K: Key type
    :tparam V: Value type

Sequenced Objects
=================

The ``mge::sequenced`` template provides automatic sequence numbering
for objects. Each instance receives a unique, monotonically increasing sequence
number that can be used for ordering or identification. Sequence numbers are
type-specific, so different template instantiations maintain separate sequences.

.. cpp:class:: template<typename T> mge::sequenced

    A sequenced object having a sequence number.

    :tparam T: The type that is sequenced, different types have different sequences

Callback Map
============

The :cpp:class:`mge::callback_map` template manages a collection of callbacks
that can be invoked together. It provides safe insertion and removal of
callbacks, even during callback execution. Callbacks scheduled for removal
during execution are deferred until all current callbacks complete.

.. doxygenclass:: mge::callback_map
    :members:
