.. _mgecore_configuration:

*************
Configuration
*************

Configuration Access
====================

Plain configuration access is provided by :any:`mge::configuration`.

.. doxygenclass:: mge::configuration
    :project: mge
    :members:

A special exception is thrown if a configuration key cannot be found.

.. doxygenclass:: mge::configuration_key_not_found
    :project: mge

Configuration access is usually done hierarchically:

.. code-block:: c++

    // access section "foo"
    configuration config("foo");
    auto bar_names = config.list_value("bars");
    for (const auto& b: bar_names) {
        std::string config_name("foo.bar.");
        config_name += b;
        // grab config for a single "bar" instance
        configuration bar_config(config_name);
        ...
    }


Configurable
============

A class that allows a configuration being applied usually implements
the :any:`mge::configurable` interface defined in
:file:`mge/core/configurable.hpp`.

.. doxygenclass:: mge::configurable
    :project: mge
    :members: