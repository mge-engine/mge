***********
Concurrency
***********

MGE uses multi-threading to achieve concurrency.

Threading Utilities
===================

Thread
------

The thread class enhances the `std::thread` interface by some convenience
methods.

.. doxygenclass:: mge::thread
    :members:

Thread Group
------------

Threads can be organized in thread groups. A thread is assigned to a thread
group om construction, and the group cannot be changed.

.. doxygenclass:: mge::thread_group
    :members:

Synchronization
---------------

The mutex class enhances `std::mutex` by a name member that is used for
additional diagnostic purposes.

.. doxygenclass:: mge::mutex
    :members:

