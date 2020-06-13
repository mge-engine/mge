============
Installation
============


Building from source
====================

Prerequisites
-------------

MGE depends ony a number of tools and packages, which need to be installed
before building.

Software Packages
~~~~~~~~~~~~~~~~~

MGE uses the following software packages, which are supposed to be in
the PATH of the command line used to build. We usually strive to use a
most recent tool chain and environment, and deviating from that may
result in issues:

Git

CMake

Python 3

Sphinx and Breathe



Third-Party Libraries
~~~~~~~~~~~~~~~~~~~~~

MGE makes use of several third party libraries for common functionality,
like storing data, decoding image and mesh formats, or subsystems of the
engine.

Third-party libraries are mostly managed by using *Vcpkg*. An own
for of the global Vcpkg repository is managed by MGE. This allows
associating a snapshot of third-party libraries to a certain release
of MGE.

The following third-party vcpkg packages are used by MGE:

* sqlite3
*


