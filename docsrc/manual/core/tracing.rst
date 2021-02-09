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

Severity
========

Trace messages can have a different severities, indicated by a trace level.

+----------+------------------------------+
| Severity | Purpose                      |
+==========+==============================+
| ERROR    | Severe malfunction message.  |
+----------+------------------------------+
| WARNING  | Potential problem.           |
+----------+------------------------------+
| INFO     | General information message. |
+----------+------------------------------+
| DEBUG    | Detailed debug information.  |
+----------+------------------------------+

.. cpp:enum-class:: mge::trace_level

    Log message severity. Note the shortcuts :any:`trace_level::ERROR`
    and :any:`trace_level::DEBUG` are only defined if the environment
    does not define an ``ERROR`` or ``DEBUG`` macro.

Trace Topic
===========

Trace topics partition trace output into different categories. Trace can be
enabled or disabled per category. There exists a pseudo-category for enabling
and disabling trace at a global level.

Defining a Trace Topic
----------------------

.. doxygendefine:: MGE_DEFINE_TRACE
    :project: mge

Using a Trace Topic
-------------------

.. doxygendefine:: MGE_USE_TRACE
    :project: mge

Trace Topic Reference
---------------------

.. doxygenclass:: mge::trace_topic
    :project: mge
    :members: