*******
Tracing
*******

MGE defines a tracing using a system of trace topics having active trace levels
and trace sinks configured.

- A trace level defines whether a trace message is informational or describes a
  severe error.
- A trace sink is a mechanism to write trace records to - like a file or the
  console.
- A trace topic is a named category of trace, which allows an active level to
  be set, and trace sinks attached as destination of trace records
- A trace formatter is a  mechanism to create text from a trace record.
- A trace record is created for each trace entry, trace records capture the
  trace message and details on its origin.

To use tracing, :file:`mge/core/trace.hpp` needs to be included.

In addition to use configuration to enable and disable trace, the following
environment variables can be used:


.. envvar:: MGE_TRACE_TO_STDOUT

   Set to "1" to print all trace to standard output.

.. envvar:: MGE_ENABLE_TRACE

   Set to "1" to enable all trace.


Severity
========

Trace messages can have a different severities, indicated by a trace level.

+----------+-----------------------------------+
| Severity | Purpose                           |
+==========+===================================+
| ERROR    | Severe malfunction message.       |
+----------+-----------------------------------+
| WARNING  | Potential problem.                |
+----------+-----------------------------------+
| INFO     | General information message.      |
+----------+-----------------------------------+
| DEBUG    | Detailed debug information.       |
+----------+-----------------------------------+
| FATAL    | Fatal message, program will exit. |
+----------+-----------------------------------+

.. doxygenenum:: mge::trace_level


Trace Topic
===========

Trace topics partition trace output into different categories. Trace can be
enabled or disabled per category. There exists a pseudo-category for enabling
and disabling trace at a global level.

Defining a Trace Topic
----------------------

In a source file, a trace topic must be defined. This is best done using
the predefined macro :any:`MGE_DEFINE_TRACE`.

.. doxygendefine:: MGE_DEFINE_TRACE

Using a Trace Topic
-------------------

If you just want to use a trace topic for tracing, a forward declaration
must be done. This is done using :any:`MGE_USE_TRACE`, or :any:`MGE_USE_IMPORTED_TRACE`
in case the topic is defined in another dynamic library.

.. doxygendefine:: MGE_USE_TRACE

.. doxygendefine:: MGE_USE_IMPORTED_TRACE

If you need to inspect a trace topic directly, the following macros can be used:

.. doxygendefine:: MGE_TRACE_TOPIC

.. doxygendefine:: MGE_NS_TRACE_TOPIC

Trace Topic Reference
---------------------

.. doxygenclass:: mge::trace_topic
    :members:

Writing Trace Messages
======================

A :any:`mge::trace` object covers the trace writing process.

.. doxygenclass:: mge::trace
    :members:

For composing trace, various macros can be used:

.. doxygendefine:: MGE_TRACE

.. doxygendefine:: MGE_DEBUG_TRACE

.. doxygendefine:: MGE_INFO_TRACE

.. doxygendefine:: MGE_WARNING_TRACE

.. doxygendefine:: MGE_ERROR_TRACE

.. doxygendefine:: MGE_FATAL_TRACE

As an example, writing trace into a trace topic `TEST` looks like this:

.. code-block:: c++

    MGE_ERROR_TRACE(TEST) << "This is a test of error trace";

Note that a line break isn't needed at the end.

Sometimes creating traces may involve heavier processing one wants to avoid if
a particular trace is disabled. A number of check macros is available for this
purpose:

.. doxygendefine:: MGE_DEBUG_TRACE_ENABLED

.. doxygendefine:: MGE_INFO_TRACE_ENABLED

.. doxygendefine:: MGE_WARNING_TRACE_ENABLED

.. doxygendefine:: MGE_ERROR_TRACE_ENABLED

.. doxygendefine:: MGE_FATAL_TRACE_ENABLED

During development one may want to add temporary debug trace, which is purely
for isolating the problem, and which should be easy to identify. A
:any:`MGE_XDEBUG` statement may fulfil this purpose. Trace messages
issued by this statement are prefixed with 'XDEBUG:' and thus are easy to
grep for. Note that the released code of MGE does not contain any 'XDEBUG'
trace.

.. doxygendefine:: MGE_XDEBUG

Trace Processing
================

Trace Record
------------

A trace record describes a trace event. Note that a trace record contains
temporary information and must not be stored - except if the fields are
materialized.

.. doxygenstruct:: mge::trace_record
    :members:

Trace Sink
----------

A trace sink defines the destination of trace records.

.. doxygenclass:: mge::trace_sink
    :members:

Two variants are implemented, a sink for temporary record storage and a sink that
writes trace records to a stream.

.. doxygenclass:: mge::memory_trace_sink
    :members:

.. doxygenclass:: mge::stream_trace_sink
    :members:

Trace Formatter
---------------

To write trace to a stream, it is formatted. This is achieved by implementations
of the :any:`mge::trace_formatter` interface.

.. doxygenclass:: mge::trace_formatter
    :members:

A simple trace formatter is provided.

.. doxygenclass:: mge::simple_trace_formatter
    :members:
