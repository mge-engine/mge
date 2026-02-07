***************************
Command Line and User Input
***************************

MGE provides utilities for parsing command-line arguments and handling
interactive console input.

Program Options
===============

The :cpp:class:`mge::program_options` class provides a flexible framework for
defining and parsing command-line options. It supports named options, positional
arguments, and option values with type safety.

.. doxygenclass:: mge::program_options
    :members:

.. doxygenclass:: mge::program_options::options
    :members:

Unknown Option Exception
-------------------------

.. doxygenclass:: mge::unknown_option

Line Editor
===========

The :cpp:class:`mge::line_editor` class provides interactive line input with
prompt support for console applications. Only one line editor instance can
exist at a time.

.. doxygenclass:: mge::line_editor
    :members:
