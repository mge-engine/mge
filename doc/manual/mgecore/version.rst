.. _mgecore_version:

******************
Version Management
******************

MGE uses *semantic versioning* with three levels with different
guarantees for version changes on each level.

- The first level, the *major version* is change on major, possibly
  incompatible changes to the library, which may require changes
  to existing projects.
- The second level, the *minor version* is changed on releasing new
  functionality. While being source-code compatible, it may require
  recompilation of a project.
- The third level, the *patch level* is changed on every release within
  a minor version release, and usually should be binary compatible
  with a previous patch level.

MGE Version
===========

The version of MGE can be identified by several different constants,
all defined in :file:`mge/version.hpp`.

.. doxygendefine:: MGE_VERSION_MAJOR
   :project: mge

.. doxygendefine:: MGE_VERSION_MINOR
   :project: mge

.. doxygendefine:: MGE_PATCHLEVEL
   :project: mge

.. doxygendefine:: MGE_VERSION_STRING
   :project: mge

Working with Versions
=====================

MGE provides the class :cpp:texpr:`mge::version` to manage semantic versions:

.. doxygenclass:: mge::version
   :project: mge
   :members:

.. cpp:function:: std::ostream& operator <<(std::ostream& os, const version& v)

   Print operator.

   :returns: ``os`` argument

   :param os:   target output stream
   :param v:    version to print

