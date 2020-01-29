.. _mgecore_log:

*******
Logging
*******

Log
===

Log Severity
------------

Log messages can have a different severities.

+----------+------------------------------+
| Severity | Purpose                      |
+==========+==============================+
| ERROR    | Severe malfunction message.  |
+----------+------------------------------+
| WARNING  | Potential proplem.           |
+----------+------------------------------+
| INFO     | General information message. |
+----------+------------------------------+
| DEBUG    | Detailed debug information.  |
+----------+------------------------------+

.. cpp:enum-class:: mge::log_severity

    Log message severity. Note the shortcuts :any:`log_severity::ERROR`
    and :any:`log_severity::DEBUG` are only defined if the environment
    does not define an ``ERROR`` or ``DEBUG`` macro.

Log Sinks
---------

Calls to the log will be dispatched to log sinks, which handle
individual log records. A log sink usually defines a log destination
and performs formatting of the log record using a log formatter.

Console Log Sink
................

The console log sink prints the formatted record to the
standard output console. It takes one configuration option:

formatter
    Name of formatter instance used for formatting the log record. |

File Log Sink
.............

The file log sink prints log into a file. It accepts the following
configuration parameters:

formatter
    Name of formatter instance used for formatting the log record. |
logfile_name
    Name of the log file.

Custom Log Sink implementation
..............................

To implement a custom log sink, you need to implement the :any:`mge::log_sink`
and register it as implementation.

.. doxygenclass:: mge::log_sink
    :project: mge
    :members:

A log sink takes a log record for each log call to publish:

.. doxygenstruct:: mge::log_record
    :project: mge
    :members:


Log Formatter
-------------

Text Formatter
..............

Custom Log Formatter
....................

A custom log formatter can be implemented by subclassing :any:`mge::log_formatter`
class and register it as an implementation.

.. doxygenclass:: mge::log_formatter
    :project: mge
    :members:

Log Configuration
-----------------

Log Sink Configuration
......................

Log configuration is stored in the section ``log`` of the
configuration. The ``sinks`` key defines the sinks configured that
receive log records.

Each sink has its configuration stored at the key ``log.sink.<key>``.
There is at least one entry, ``log.sink.<key>.class`` which
defines the component to instantiate for the log sink.

Log Level Configuration
.......................

The keys below ``log.level`` define the enabled log level, i.e.
the severity that enables a log entry to be logged.
``log.level.ALL`` defines the global level. The severity ``ALL``
includes all log severites.


Log Usage
---------

Log Definition
..............

Log instances for a certain topic are defined using :any:`MGE_DEFINE_LOG`
helper macro.

.. doxygendefine:: MGE_DEFINE_LOG
    :project: mge

Declare Log Usage
.................

Any source file where you want to log to a certain topic need to have
log usage declared. Depending on whether log usage is in the declaring
library, or in a different library, either :any:`MGE_USE_LOG` or
:any:`MGE_USE_IMPORTED_LOG` is used.

.. doxygendefine:: MGE_USE_LOG
    :project: mge

.. doxygendefine:: MGE_USE_IMPORTED_LOG
    :project: mge

Writing Log Messages
....................

There are different helper macros provide for writing into the
log depending on the severity. Each invocation will write an
individual log entry, usually written on a different output
line.

So, a typical log usage looks like:

.. code-block:: c++

    // top of file,
    MGE_USE_LOG(FOOBAR);
    ...
    // logging, note no end of line is necessary
    MGE_DEBUG_LOG(FOOBAR) << "Debug message goes here";
    MGE_DEBUG_LOG(FOOBAR) << "Another debug message goes here";

.. doxygendefine: MGE_DEBUG_LOG
    :project: mge

.. doxygendefine: MGE_INFO_LOG
    :project: mge

.. doxygendefine: MGE_WARNING_LOG
    :project: mge

.. doxygendefine: MGE_ERROR_LOG
    :project: mge


Gists
=====

A *gist* is a core snipped of information, reduced to the minimum.
While normal output using the ``<<`` operator should print a complete
object, including all details, a gist should print its essentials
without too much clutter. Facilities for gist implementation are
provided in :file:`mge/core/gist.hpp`.

To implement a gist output two ways are provided:

1. implement a ``void gist(std::ostream&) const`` method in
   your class that prints the gist output to the provided
   stream. The method needs to have public access.

2. implement a ``std::ostream& operator <<(std::ostream&, const mge::gist_type<T>&)``
   operator, the :any:`mge::gist_type` object has a member ``value`` that
   points to the actual object whose gist is requested

To use the ``gist`` just wrap the :any:`mge::gist` function around
the object you want to print:

.. code-block:: c++

    mge::buffer b(100);
    // will print "buffer(100)" due to gist implementation
    std::cout << mge::gist(b) << std::endl;


.. doxygenfunction:: mge::gist
    :project: mge

If you choose the second variant of implementation, a helper macro is
provided to ease implementation.

.. code-block:: c++

    struct foobar {};
    using foobar_list = std::vector<foobar>;

    MGE_GIST_OUTPUT(foobar_list)
    {
        os << "foobar_list(" << g.value->size() << ");
    }

    ...
    foobar_list f(42);
    std::cout << mge::gist(f) << std::endl;

.. doxygendefine:: MGE_GIST_OUTPUT
    :project: mge

