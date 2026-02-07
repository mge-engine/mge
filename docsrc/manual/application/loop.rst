****************
Interaction Loop
****************

Game Loop Concept
=================

The interaction loop of a game application can be usually described by
the following flow::

    +-------------------------------------------------+
    |                                                 |
    +-> (!quit) -> (input) -> (update) -> (present) --+

While there is no **quit** signalled, the application processes the following
steps in a loop:

input
    Input is processed, state of input devices is retrieved.

update
    The state of the application is updated.

present
    The state of the application is presented at the screen.

Abstractions
============

The target of the loop is abstracted in the interface class ``mge::loop_target``:

..  doxygenclass:: mge::loop_target
    :members:

The loop itself is implemented by implementing a component of ``mge::loop`` interface:

..  doxygenclass:: mge::loop
    :members:

The loop context provides information about the current state of the loop:

..  doxygenclass:: mge::loop_context
    :members:

Simple Loop
===========

A simple loop is provided which does not use fixed step update, and also
does not provide any peek on presentation.::

    auto simple_loop = mge::loop::create("simple");

The simple loop processes the interaction loop as follows:

* ``update`` will be called with the delta time compared with
  previous frame
* ``present`` will always be called with a peek value of ``0.0``.

Fixed Step Loop
===============

A fixed step loop is provided which uses a fixed time step for updates::

    auto fixed_step_loop = mge::loop::create("fixed_step");

The fixed step loop processes the interaction loop as follows:

* ``update`` will be called with a fixed time step, potentially multiple 
  times per frame if the frame takes longer than the time step
* ``present`` will be called with a peek value indicating the interpolation
  factor between current and next state, allowing smooth rendering between
  fixed update steps
