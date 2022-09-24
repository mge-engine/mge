************
Introduction
************

What is MGE?
############

MGE stands for *Modern Game Engine*. It is called *modern* because
it is a newcomer and relies on modern concepts, such as a recent
C++ standard.

There exist likely already too many game and graphics engines,
so why create another one? The primary root was personal enjoyment
in programming and education about the workings of graphics and games.
MGE is still in its very infancy, and not ready to be an alternative
to established engines and environments.

MGE makes use of STL and `Boost <https://www.boost.org/>`_, which are a
sometimes shyed away from  for various reasons - in game development. MGE
makes heavy use of these libraries but uses own data structures and algorithms
where the scope of these generic support libraries ends.

During the course of implementing a graphics and game engine, some
general-purpose components have been created that may be of use outside
the domain of game programming:

- The core library combines general purpose tools and utilities like
  facilities for tracing and error reporting, configuration or
  componentization.

- The graphics library delivers an abstraction of a modern presentation
  pipeline, using concepts like shader programs, parallel queues and
  command buffers. It is a low level abstraction of these concepts.

- The input library makes it easy to process user input, from various
  devices including gamepads and joysticks.

- The application library provides a framework for the common game application,
  supporting concepts like single-threaded or multi-threaded simulation and
  render loop.

- The scripting library encapsulates the integration of scripting languages.
  Integrations for Python and Lua are included.

The general design philosphy in MGE is that everything should stand
for itself, and *manager* or other *uber-objects* are to be avoided.

This also allows also going as low-level as wanted when using MGE, and
not being tied to a render loop or update architecture, although common
implementations of these concepts are provided.


About this manual
#################

The first half of the manual, the user guide part, describes how to
install, configure and use MGE and its tools.

The second half, features reference chapters for each main library of MGE:

- :ref:`mge-core` describes the core library, which contains
  basic facilities and utilities.

- :ref:`mge-math` describes the mathematics library, which contains
  common mathematics utilities for graphics programming.

- :ref:`mge-graphics` describes the graphics library, which handles
  all technical means to put things on the screen.

- :ref:`mge-input` describes the input library, which is used to manager
  input devices and their state.

- :ref:`mge-application` describes the application library, which provides
  facilities to structure application utilizing the MGE toolkit.

- :ref:`mge-asset` describes the asset library, which provides
  facilities to load and manage assets.

- :ref:`mge-script` describes the scripting language integration facilities.
