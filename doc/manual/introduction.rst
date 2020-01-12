************
Introduction
************

What is MGE?
############

MGE stands for *Modern Game Engine*. It is called *modern* because
it is a newcomer and relies on modern concepts, such as the latest
C++ standard.

There exist likely already too many game and graphics engines,
so why create another one? The primary root was personal enjoyment
in programming and education about the workings of graphics and games.
While MGE is still rough and unpolished, it can stand as a free and
open alternative to existing systems.

MGE makes use of STL and Boost, which are not always used - for various
reasons - in game development. MGE makes heavy use of these libraries
but used other methods and algorithms where appropriate or needed.

During the course of implementing a graphics and game engine, some
general-purpose components have been created that may be of use outside
the domain of game programming:

- The core library combines general purpose tools and utilities like
  facilities for tracing and error reporting, configuration or
  componentization.
- The entity component system is a general purpose mechanism, although
  commonly used in game development.
- The script library and plugins allow adding a scripting language,
  also a commonly needed functionality.

The general design philosphy in MGE is that everything should stand
for itself, and *manager* or other *uber-objects* are to be avoided.
This also allows also going as low-level as wanted when using MGE.


About this manual
#################

The first half of the manual, the user guide part, describes how to
install, configure and use MGE and its tools.

The second half, the refeence part, features reference chapters for
each main library of MGE:

- Chapter :ref:`mgecore` describes the core library, which contains
  basic facilities and utilities.

- Chapter :ref:`mgegraphics` describes the general graphics API provided
  by MGE.


