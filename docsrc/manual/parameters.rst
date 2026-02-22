MGE uses a configuration parameter system to control runtime behavior. Parameters are defined using the ``MGE_DEFINE_PARAMETER`` macro and can be configured via JSON configuration files.

Parameters are organized into sections corresponding to different subsystems.

.. list-table::
   :header-rows: 1
   :widths: 20 25 15 40

   * - Name
     - Type
     - Default
     - Description
   * - **Core Parameters**
     - 
     - 
     - 
   * - ``module_path``
     - ``std::string``
     - 
     - Paths from which to load modules
   * - **Asset Parameters**
     - 
     - 
     - 
   * - ``repositories``
     - ``std::vector<std::map<std::string, std::string>>``
     - 
     - Asset configuration
   * - **Graphics Parameters**
     - 
     - 
     - 
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
   * - **DirectX 11 Parameters**
     - 
     - 
     - 
   * - ``debug``
     - ``bool``
     - ``false``
     - Enable DirectX 11 debug mode
   * - ``software_device``
     - ``bool``
     - ``false``
     - Use the DirectX 11 software device
   * - **DirectX 12 Parameters**
     - 
     - 
     - 
   * - ``debug``
     - ``bool``
     - ``false``
     - Enable DirectX 12 debug mode
   * - ``warp``
     - ``bool``
     - ``false``
     - Enable DirectX 12 WARP (Windows Advanced Rasterization Platform) device
   * - **OpenGL Parameters**
     - 
     - 
     - 
   * - ``debug``
     - ``bool``
     - ``false``
     - Enable OpenGL debug
   * - **Vulkan Parameters**
     - 
     - 
     - 
   * - ``debug``
     - ``bool``
     - ``true``
     - Enable Vulkan debug mode
   * - ``stop_on_validation_error``
     - ``bool``
     - ``true``
     - Stop on Vulkan validation errors
