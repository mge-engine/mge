********************
Container Data Types
********************

MGE provides specialized container data structures optimized for specific use
cases in game engine development.

Small Vector
============

The :cpp:class:`mge::small_vector` template is a vector-like container that
uses stack allocation for small numbers of elements, avoiding heap allocation
overhead for common small-size use cases. When the number of elements exceeds
the specified size, it automatically transitions to heap allocation.

.. doxygenclass:: mge::small_vector
    :members:

Handle
======

The :cpp:class:`mge::handle` template provides a type-safe handle mechanism for
managing objects within a specific context. Handles consist of a context index,
flags, and an object index, enabling efficient object lookup through the
associated context.

.. doxygenclass:: mge::handle
    :members:

LRU Cache
=========

The :cpp:class:`mge::lru_cache` template implements a least-recently-used cache
that automatically evicts the oldest accessed items when the cache reaches its
maximum size.

.. doxygenclass:: mge::lru_cache
    :members:

Sequenced Objects
=================

The :cpp:class:`mge::sequenced` template provides automatic sequence numbering
for objects. Each instance receives a unique, monotonically increasing sequence
number that can be used for ordering or identification. Sequence numbers are
type-specific, so different template instantiations maintain separate sequences.

.. doxygenclass:: mge::sequenced
    :members:

Callback Map
============

The :cpp:class:`mge::callback_map` template manages a collection of callbacks
that can be invoked together. It provides safe insertion and removal of
callbacks, even during callback execution. Callbacks scheduled for removal
during execution are deferred until all current callbacks complete.

.. doxygenclass:: mge::callback_map
    :members:
