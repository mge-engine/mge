*************************
Asset Loading and Storing
*************************

Configuration
=============

Assets are identified by a path, which is used to look up the asset in a
virtual filesystem managed by the asset subsystem.

Asset access factories are mounted at specific directories and are responsible
for loading assets having paths that start with the directory they are mounted.

Configuration File
------------------

The asset subsystem is configured in the executable configuration under the
``asset`` section, using the ``repositories`` parameter which manages a list
of mounted asset repositories.

The configuration is read from the application's JSON configuration file and
can be dynamically updated at runtime through the configuration system.

Required Repository Properties
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Each repository entry must specify:

type
    The type defines the factory used to load the assets (e.g., ``"file"``, ``"zip"``).
    
mount_point
    The virtual path where the asset repository is mounted (e.g., ``"/"``, ``"/assets"``).

Optional Repository Properties
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

mode
    Access mode for the repository. Valid values:
    
    - ``"r"`` - Read-only access (default)
    - ``"w"`` - Write-only access
    - ``"rw"`` - Read-write access

Type-Specific Properties
~~~~~~~~~~~~~~~~~~~~~~~~~

Additional properties specific to each repository type can be specified.
These properties are passed to the asset source implementation during
configuration.

For ``file`` type:
    ``directory`` - Filesystem path to the directory containing assets

For ``zip`` type:
    ``file`` - Path to the ZIP archive file

Configuration Example
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: json

    {
        "asset": {
            "repositories": [
                {
                    "type": "file",
                    "mount_point": "/",
                    "mode": "r",
                    "directory": "."
                },
                {
                    "type": "file",
                    "mount_point": "/assets",
                    "mode": "r",
                    "directory": "assets"
                },
                {
                    "type": "file",
                    "mount_point": "/temp",
                    "mode": "rw",
                    "directory": "/tmp/game_assets"
                },
                {
                    "type": "zip",
                    "mount_point": "/zipassets",
                    "file": "assets.zip"
                }
            ]
        }
    }

Dynamic Reconfiguration
~~~~~~~~~~~~~~~~~~~~~~~

The asset configuration supports dynamic updates through the configuration
system's change handler mechanism. When the ``asset.repositories`` parameter
is modified, the mount table is automatically reconfigured:

- Unchanged mount points are preserved
- Modified mount points are remounted with new settings
- Removed mount points are automatically unmounted

This allows runtime modification of asset sources without restarting the
application.

The configuration parameter can be accessed programmatically:

.. code-block:: cpp

    #include "mge/core/configuration.hpp"
    
    // Access the repositories parameter
    auto& repos = MGE_PARAMETER(asset, repositories);
    
    // Modification triggers automatic reconfiguration
    // through the registered change handler

Configuration Validation
~~~~~~~~~~~~~~~~~~~~~~~~

The asset configuration system validates repository entries during loading:

- **Missing required properties**: Repository entries without ``mount_point``
  or ``type`` are logged as errors and skipped.
  
- **Invalid access mode**: An invalid ``mode`` value throws an ``illegal_argument``
  exception. Valid values are ``"r"``, ``"w"``, or ``"rw"``.
  
- **Invalid type**: Attempting to mount an unknown asset source type throws
  an ``illegal_state`` exception.

Error messages are logged through the ``ASSET`` trace topic. Enable asset
tracing to diagnose configuration issues:

.. code-block:: bash

    # Windows PowerShell
    $env:MGE_TRACE_ENABLED=1
    $env:MGE_TRACE_TOPIC_ASSET=1
    $env:MGE_TRACE_TO_STDOUT=1

Path Resolution
~~~~~~~~~~~~~~~

When an asset is accessed by path, the mount table resolves it to the
appropriate asset source using the following algorithm:

1. Find the longest mount point that is a prefix of the requested asset path
2. Check that the access mode is compatible with the requested operation
   (load requires READ or READ_WRITE, store requires WRITE or READ_WRITE)
3. Delegate to the matched asset source's ``access()`` method

For example, with mount points ``/`` and ``/assets/textures``:

- ``/assets/textures/brick.png`` → uses ``/assets/textures`` mount
- ``/assets/models/box.obj`` → uses ``/`` mount  
- ``/config.json`` → uses ``/`` mount

This allows hierarchical organization of asset sources with more specific
mount points taking precedence over general ones.

Supported Asset Sources
=======================

The following asset sources are built into the asset subsystem:

File Asset Source
-----------------

The file asset source loads assets from the filesystem. It is configured with
the following properties:

directory
    The directory from which to load assets.

Access Mode
~~~~~~~~~~~

Asset sources can be mounted in different access modes:

READ
    Read-only access to assets (default).
WRITE
    Write-only access (for storing assets).
READ_WRITE
    Both reading and writing allowed.

Example:

.. code-block:: cpp

    mge::properties p;
    p.set("directory", "./assets");
    
    // Mount read-only (default)
    mge::asset::mount("/", "file", p);
    
    // Mount with read-write access
    mge::asset::mount("/temp", "file", 
                      mge::asset_source::access_mode::READ_WRITE, p);

Zip Asset Source
----------------

The zip asset source loads assets from a zip file. It is configured with the
following properties:

file
    The zip file from which to load assets.

.. note::
    Zip asset source implementation status may vary. Check source code for
    current availability.

Implementing an Asset Source
============================

To implement a new asset source, you need to create a class that implements
the `asset_source` interface and register it as a component.

.. doxygenclass:: mge::asset_source
    :members:

Furthermore, you need to implement the `asset_access` interface for an individual
asset.

.. doxygenclass:: mge::asset_access
    :members:

Asset Handlers
==============

Asset handlers are responsible for loading and storing assets from and to their 
respective sources. They are implemented as classes that inherit from 
the `asset_handler` interface. Different asset handlers that handle the same
type for load and store can be registered independently.

One asset handler handles a number of types of assets, such as images, meshes,
shaders, etc. The asset handler is responsible for loading the asset data into
memory and providing an interface to access the loaded asset, and to store an 
object in a a specific format back to the asset.

In addition, an asset handler can be used also to improve a generic asset type
like e.g. 'text/plain' by providing a more specific type, such as 'model/obj'
by further inspection of the asset data.

.. doxygenclass:: mge::asset_handler
    :members:

Built-in Asset Handlers
-----------------------

Octet Stream Handler
~~~~~~~~~~~~~~~~~~~~

The ``application_octet_stream_handler`` provides generic handling for binary
assets. It loads assets as raw byte buffers and is used as a fallback when no
more specific handler is available.

Handled types:
    ``application/octet-stream``

Loaded as:
    ``std::shared_ptr<mge::buffer>``

Example:

.. code-block:: cpp

    using namespace mge::literals;
    
    mge::asset shader("/shaders/fragment.spv");
    auto data = shader.load();
    auto buffer = std::any_cast<std::shared_ptr<mge::buffer>>(data);

Type Improvement
~~~~~~~~~~~~~~~~

Asset handlers can improve generic asset types by inspecting the asset data.
For example, a generic ``text/plain`` type might be improved to ``model/obj``
by checking for OBJ file markers.

The type improvement mechanism works as follows:

1. Asset source provides initial type (often via file extension or MIME magic)
2. Handlers are queried via ``can_improve()`` to see if they can refine the type
3. If a handler can improve the type, ``improve()`` is called to get the refined type

Implementation Details
----------------------

Type Detection
~~~~~~~~~~~~~~

The asset library uses ``libmagic`` for automatic type detection based on file
content. This allows assets to be correctly identified even without relying on
file extensions.

Handler Registration
~~~~~~~~~~~~~~~~~~~~

Asset handlers register themselves as component implementations:

.. code-block:: cpp

    class my_handler : public mge::asset_handler {
        // Implementation...
    };
    
    MGE_REGISTER_IMPLEMENTATION(my_handler, mge::asset_handler, my_handler_id);

Practical Examples
==================

Loading Assets
--------------

.. code-block:: cpp

    using namespace mge::literals;
    
    // Load an image
    mge::asset image_asset("/textures/brick.png");
    auto image_data = image_asset.load();
    auto image = std::any_cast<mge::image_ref>(image_data);
    
    // Load a model
    mge::asset model_asset("/models/teapot.obj");
    EXPECT_EQ("model/obj"_at, model_asset.type());
    auto mesh = std::any_cast<mge::mesh_ref>(model_asset.load());

Storing Assets
--------------

.. code-block:: cpp

    using namespace mge::literals;
    
    // Create or modify an image
    mge::image_ref my_image = create_image();
    
    // Store as PNG
    mge::asset output("/output/result.png");
    output.store("image/png"_at, my_image);

Mounting and Unmounting
------------------------

.. code-block:: cpp

    // Mount filesystem directory
    mge::properties fs_props;
    fs_props.set("directory", "./game_assets");
    mge::asset::mount("/game", "file", fs_props);
    
    // Mount with write access
    mge::properties temp_props;
    temp_props.set("directory", std::filesystem::temp_directory_path().string());
    mge::asset::mount("/temp", "file", 
                      mge::asset_source::access_mode::READ_WRITE, 
                      temp_props);
    
    // Use assets
    mge::asset level_data("/game/levels/level1.dat");
    // ...
    
    // Clean up
    mge::asset::unmount("/temp");
    mge::asset::unmount("/game");

Programmatic vs. Configuration File Mounting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Asset sources can be mounted in two ways:

1. **Configuration file**: Repositories listed in the ``asset.repositories``
   configuration parameter are automatically mounted at application startup
   and can be dynamically reconfigured.

2. **Programmatic mounting**: Using ``mge::asset::mount()`` and 
   ``mge::asset::unmount()`` provides runtime control over mount points,
   useful for temporary asset sources or conditional mounting.

Both methods can be used together. Programmatic mounts are independent of
the configuration file and persist until explicitly unmounted or until
application shutdown.

Locating Assets by Name
------------------------

The ``locate()`` method finds assets by name rather than explicit path,
useful for font families or other named resources:

.. code-block:: cpp

    using namespace mge::literals;
    
    // Find a font by family name
    auto font = mge::asset::locate("Arial", "font/truetype"_at);

