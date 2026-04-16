*****
Dumps
*****

MGE provides a crash dump facility that captures diagnostic information when
an unhandled exception or fatal error occurs. The dump is written as a
GitHub Flavored Markdown file containing a stack trace and additional
information contributed by dump info providers.

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
``<executable>-<purpose>-<timestamp>.md``.

The dump document contains:

- **Table of Contents** with links to all sections
- **Crash Dump** heading with general information (executable name,
  timestamps, process ID)
- **Stack Trace** as a code block
- Additional sections from registered dump info providers

Dump Info Providers
===================

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

Markdown Document
=================

The :cpp:class:`mge::markdown_document` class is a fluent builder for
composing GitHub Flavored Markdown documents. It is used internally by
the dump system and available for use by dump info providers.

.. doxygenclass:: mge::markdown_document
    :members:
