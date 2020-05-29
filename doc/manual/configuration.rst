.. _configuration:

**********************
Configuration Settings
**********************

Configuring an MGE Application
==============================

Configuration Database
----------------------

The configuration database of a program is stored in a file
:file:`mge_config_{program}.db` for each program *program*.
While this is a normal `sqlite3` database, it is not
encouraged to use the sqlite3 command tool for modifications.

Overriding Configuration Settings
----------------------------------


Configuration Settings
======================

Core
----

Logging
.......

The setting ``log.sinks`` contains the log sinks that are used as target
of log files. Each sink has its own config entry as ``log.sink.{name}``.

Each sink supports the following settings

+-------------------------------+---------------------------------------------+
| Key                           | Description                                 |
+===============================+=============================================+
| ``log.sink.{name}.formatter`` | Implementation name of log record formatter |
+-------------------------------+---------------------------------------------+
| ``log.sink.{name}.class``     | Implementation name of log sink             |
+-------------------------------+---------------------------------------------+

Logging is controlled by levels, the minimum level is configured for each
topic using the configuration key ``log.level.{topic}``. The pseudo
topic ``ALL`` controls the global settings. Possible log level settings
are ``DEBUG``, ``INFO``, ``ERROR`` and ``ALL``.


Graphics
--------

Render System
.............

The default render system can be set using the ``render.system`` setting,
supported are the following values:

+-----------+------------------------+
| Key       | Used Rendering Backend |
+===========+========================+
| opengl    | OpenGL 3.1+            |
+-----------+------------------------+
| directx11 | Direct3D 11            |
+-----------+------------------------+
| directx12 | Direct3D 12            |
+-----------+------------------------+
| vulkan    | Vulkan                 |
+-----------+------------------------+