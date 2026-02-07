MGE uses a configuration parameter system to control runtime behavior. Parameters are defined using the ``MGE_DEFINE_PARAMETER`` macro and can be configured via JSON configuration files.

Parameters are organized into sections corresponding to different subsystems.

Core Parameters
===============

.. list-table::
   :header-rows: 1
   :widths: 20 20 20 40

   * - Name
     - Type
     - Default
     - Description
   * - ``module_path``
     - ``std::string``
     - 
     - Paths from which to load modules

Asset Parameters
================

.. list-table::
   :header-rows: 1
   :widths: 20 30 10 40

   * - Name
     - Type
     - Default
     - Description
   * - ``repositories``
     - ``std::vector<std::map<std::string, std::string>>``
     - 
     - Asset configuration

Graphics Parameters
===================

.. list-table::
   :header-rows: 1
   :widths: 20 20 20 40

   * - Name
     - Type
     - Default
     - Description
   * - ``render_system``
     - ``std::string``
     - ``"opengl"``
     - Render system implementation
   * - ``frame_debugger``
     - ``std::string``
     - ``""``
     - Frame debugger implementation
   * - ``record_frames``
     - ``bool``
     - ``false``
     - Record frames using frame debugger
   * - ``vsync``
     - ``bool``
     - ``false``
     - Honor vertical sync in screen update

DirectX 11 Parameters
=====================

.. list-table::
   :header-rows: 1
   :widths: 20 20 20 40

   * - Name
     - Type
     - Default
     - Description
   * - ``debug``
     - ``bool``
     - ``false``
     - Enable DirectX 11 debug mode
   * - ``software_device``
     - ``bool``
     - ``false``
     - Use the DirectX 11 software device

DirectX 12 Parameters
=====================

.. list-table::
   :header-rows: 1
   :widths: 20 20 20 40

   * - Name
     - Type
     - Default
     - Description
   * - ``debug``
     - ``bool``
     - ``false``
     - Enable DirectX 12 debug mode
   * - ``warp``
     - ``bool``
     - ``false``
     - Enable DirectX 12 WARP (Windows Advanced Rasterization Platform) device

OpenGL Parameters
=================

.. list-table::
   :header-rows: 1
   :widths: 20 20 20 40

   * - Name
     - Type
     - Default
     - Description
   * - ``debug``
     - ``bool``
     - ``false``
     - Enable OpenGL debug

Vulkan Parameters
=================

.. list-table::
   :header-rows: 1
   :widths: 20 20 20 40

   * - Name
     - Type
     - Default
     - Description
   * - ``debug``
     - ``bool``
     - ``true``
     - Enable Vulkan debug mode
   * - ``stop_on_validation_error``
     - ``bool``
     - ``true``
     - Stop on Vulkan validation errors
