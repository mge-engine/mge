.. _mgecore_archive:

*****************
Handling Archives
*****************

Archives are packed files of resources which on their own implement
kind of read-only file system. There exist different types of archives,
so individual archive formats are implemented as plug-in.

Archive Access
--------------

Access to an archive is managed by an instance of the
:cpp:texpr:`mge::archive` class.

.. doxygenclass:: mge::archive
   :project: mge
   :members:

Implementing Archive Access
---------------------------

The :cpp:texpr:`mge::archive` class is actually a wrapper
around the implementation of the :cpp:texpr:`mge::archive_access`
interface. This object is created by an instance of
:cpp:texpr:`mge::archive_access_factory`. Each factory handles a
path - like a mount point, and covers the creation of access
objects below that mount point.

.. doxygenclass:: mge::archive_access_factory
   :project: mge
   :members:

To implement a specific kind of archives, both :cpp:texpr:`mge::archive_access_factory`
and :cpp:texpr:`mge::archive_access` interfaces need to be implemented,
and the factory must be registered using the component mechanism.

.. doxygenclass:: mge::archive_access
   :project: mge
   :members:
