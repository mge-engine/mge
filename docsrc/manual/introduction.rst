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
MGE is still in its very infancy, and not ready to be an alternative
to established engines and environments.

MGE makes use of STL and Boost, which are a sometimes shyed away from
- for various reasons - in game development. MGE makes heavy use of these
libraries but uses own data structures and algorithms where the scope of these
generic support libraries ends.

During the course of implementing a graphics and game engine, some
general-purpose components have been created that may be of use outside
the domain of game programming:

- The core library combines general purpose tools and utilities like
  facilities for tracing and error reporting, configuration or
  componentization.

The general design philosphy in MGE is that everything should stand
for itself, and *manager* or other *uber-objects* are to be avoided.

This also allows also going as low-level as wanted when using MGE, and
not being tied to a render loop or update architecture.


About this manual
#################

The first half of the manual, the user guide part, describes how to
install, configure and use MGE and its tools.

The second half, the refeence part, features reference chapters for
each main library of MGE:

- Chapter :ref:`mgecore` describes the core library, which contains
  basic facilities and utilities.



