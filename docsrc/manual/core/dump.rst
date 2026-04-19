*****
Dumps
*****

MGE provides a crash dump facility that captures diagnostic information when
an unhandled exception or fatal error occurs. The dump is written as a
GitHub Flavored Markdown file containing a stack trace and additional
information contributed by dump info providers.

Crash Dump Handler
==================

The crash dump handler is automatically installed when an MGE application
starts (in :cpp:class:`mge::application`). On Windows, it registers an
unhandled exception filter via ``SetUnhandledExceptionFilter``. When a
crash occurs, the handler captures a stack trace from the faulting thread
and writes a Markdown crash dump file before the process terminates.

The handler is uninstalled when the application shuts down.

On non-Windows platforms, automatic crash handler installation is not
yet implemented; crash dumps can still be written manually.

Dump Class
==========

The :cpp:class:`mge::dump` class manages crash dump generation and handler
installation.

.. doxygenclass:: mge::dump
    :members:

Dump Output
===========

When a crash occurs, a Markdown file is created in the current working
directory. The filename follows the pattern
``<executable>-crash-<timestamp>.md``, where the timestamp has
millisecond precision (e.g. ``myapp-crash-20260419143021123.md``).

The dump document contains:

- **Table of Contents** with links to all sections
- **Overview** with executable name, local and UTC timestamps, and
  process ID
- **Stack Trace** captured at the point of the crash
- Additional sections from registered dump info providers

Example
-------

A crash dump file looks like this:

.. code-block:: markdown

   # Crash Dump

   ## Table of Contents

   - [Overview](#overview)
   - [Stack Trace](#stack-trace)
   - [Components](#components)
   - [Loaded Modules](#loaded-modules)
   - [Parameters](#parameters)
   - [Trace Configuration](#trace-configuration)

   ## Overview

   - Executable: myapp.exe
   - Timestamp: 2026-04-19 14:30:21.123 +0200
   - Timestamp (UTC): 2026-04-19 12:30:21.123 UTC
   - Process ID: 12345

   ## Stack Trace

   ...

Built-in Dump Info Providers
============================

MGE includes four dump info providers that are automatically included
in every crash dump:

Components
----------

Lists all registered component types and their implementations.
Displayed as a table with columns **Component** and
**Implementations**.

Loaded Modules
--------------

Lists all modules (plugins) that were loaded at the time of the
crash, with the file path of each module's shared library. Displayed
as a table with columns **Module** and **Library**.

Parameters
----------

Lists all configuration parameters that have a value set at the time
of the crash. Displayed as a table with columns **Parameter** (the
parameter path) and **Value** (the JSON-serialized value).

Trace Configuration
-------------------

Shows the global trace enable/disable state, whether trace output is
printed to stdout, and lists all trace topics that have specific
levels enabled (e.g. ``CORE (DEBUG, INFO)``).

Custom Dump Info Providers
==========================

Dump info providers are components that contribute additional diagnostic
sections to the crash dump. They are discovered and instantiated via the
:ref:`component system <mge-core-components>`.

.. doxygenclass:: mge::dump_info_provider
    :members:
    :protected-members:

Implementing a Dump Info Provider
---------------------------------

To add a custom section to crash dumps:

1. Derive from :cpp:class:`mge::dump_info_provider`.
2. Implement :cpp:func:`section_name` and :cpp:func:`dump_info`.
3. Register using :c:macro:`MGE_REGISTER_IMPLEMENTATION`.

.. code-block:: cpp

   class my_info_provider : public mge::dump_info_provider
   {
   public:
       my_info_provider() = default;
       my_info_provider(std::pmr::memory_resource* resource)
           : dump_info_provider(resource)
       {}

       std::pmr::string section_name() const override
       {
           return std::pmr::string("My Info", m_resource);
       }

       void dump_info(mge::markdown_document& doc) const override
       {
           doc.paragraph("Custom diagnostic information.");
       }
   };

   MGE_REGISTER_IMPLEMENTATION(my_info_provider,
                               mge::dump_info_provider);

The provider constructor receives a ``std::pmr::memory_resource*`` when
created through the component system with PMR support. Use ``m_resource``
for all PMR allocations within the provider.

Override :cpp:func:`has_info` to return ``false`` if the provider has
nothing to contribute; the section will then be omitted from the dump.

Manual Dump Creation
====================

A crash dump can also be created programmatically:

.. code-block:: cpp

   #include "mge/core/dump.hpp"

   mge::dump d("crash");
   d.write();

This writes a dump file with the current stack trace and all registered
provider sections.

Markdown Document
=================

The :cpp:class:`mge::markdown_document` class is a fluent builder for
composing GitHub Flavored Markdown documents. It is used internally by
the dump system and available for use by dump info providers.

All methods that modify the document return a reference to the document,
allowing method chaining:

.. code-block:: cpp

   mge::markdown_document doc;
   doc.heading(1, "Title")
      .paragraph("Some text.")
      .code_block("cpp", "int x = 42;")
      .horizontal_rule();

Block-level methods append content to the document:

- :cpp:func:`heading(level, text) <mge::markdown_document::heading>` --
  heading of given level (1-6)
- :cpp:func:`paragraph(text) <mge::markdown_document::paragraph>` --
  paragraph
- :cpp:func:`code_block(language, code) <mge::markdown_document::code_block>` --
  fenced code block with language tag
- :cpp:func:`code_block(code) <mge::markdown_document::code_block>` --
  fenced code block without language
- :cpp:func:`blockquote(text) <mge::markdown_document::blockquote>` --
  blockquote
- :cpp:func:`horizontal_rule() <mge::markdown_document::horizontal_rule>` --
  horizontal rule
- :cpp:func:`unordered_list(items) <mge::markdown_document::unordered_list>` --
  bulleted list
- :cpp:func:`ordered_list(items) <mge::markdown_document::ordered_list>` --
  numbered list
- :cpp:func:`table(headers, rows) <mge::markdown_document::table>` --
  table with headers and rows
- :cpp:func:`text(text) <mge::markdown_document::text>` --
  raw text appended as-is
- :cpp:func:`line(text) <mge::markdown_document::line>` --
  text followed by a newline
- :cpp:func:`blank_line() <mge::markdown_document::blank_line>` --
  empty line

Inline formatting methods return a formatted string for embedding
in other content:

- :cpp:func:`bold(text) <mge::markdown_document::bold>` -- ``**text**``
- :cpp:func:`italic(text) <mge::markdown_document::italic>` -- ``*text*``
- :cpp:func:`strikethrough(text) <mge::markdown_document::strikethrough>` --
  ``~~text~~``
- :cpp:func:`inline_code(text) <mge::markdown_document::inline_code>` --
  inline code formatting
- :cpp:func:`link(text, url) <mge::markdown_document::link>` --
  ``[text](url)``
- :cpp:func:`image(alt, url) <mge::markdown_document::image>` --
  ``![alt](url)``

Use :cpp:func:`str() <mge::markdown_document::str>` to obtain the
document as a string, or stream via ``operator<<``.

.. doxygenclass:: mge::markdown_document
    :members:

Markdown Formatting Wrapper
============================

The :cpp:class:`mge::markdown` template wraps a value for markdown-aware
formatting via ``fmt::format``. It is used internally to format types
like :cpp:class:`mge::stacktrace` as markdown code blocks in crash dumps.

.. code-block:: cpp

   mge::stacktrace st;
   auto formatted = fmt::format("{}", mge::markdown(st));

.. doxygenstruct:: mge::markdown
    :members:
