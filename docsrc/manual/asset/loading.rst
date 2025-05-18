*************
Asset Loading
*************

Configuration
=============

Assets are identified by a path, which is used to look up the asset in a
virtual filesystem managed by the asset subsystem.

Asset access factories are mounted at specific directories and are responsible
for loading assets having paths that start with the directory they are mounted.

The asset subsystem is configured in the configuration by the `asset/repository`
parameter, which manages a list of mounted asset repositories.

The following properties have to be specified for each mounted repository:

type
    The type defines the factory used to load the assets.
mount_point
    The directory where the asset repository is mounted.

Additional properties specific to the type can be specified too,
as can be seen in following example:

.. code-block:: json

    "asset": {
        "repositories": [
            {
                "type": "file",
                "mount_point": "/",
                "directory": "."
            },
            {
                "type": "file",
                "mount_point": "/assets"
                "directory": "assets"
            },
            {
                "type": "zip",
                "mount_point": "/zipassets",
                "file": "assets.zip"
            }
        ]
    }

Supported Asset Sources
=======================

The following asset sources are built into the asset subsystem:

File Asset Source
-----------------

The file asset source loads assets from the filesystem. It is configured with
the following properties:

directory
    The directory from which to load assets.

Zip Asset Source
----------------

The zip asset source loads assets from a zip file. It is configured with the
following properties:

file
    The zip file from which to load assets.

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

Asset Loaders
=============

Asset loaders are responsible for loading assets from their respective sources.
They are implemented as classes that inherit from the `asset_loader` interface.

One asset loader handles a number of types of assets, such as images, meshes,
shaders, etc. The asset loader is responsible for loading the asset data into
memory and providing an interface to access the loaded asset.

In addition, an asset loader can be used also to improve a generic asset type
like e.g. 'text/plain' by providing a more specific type, such as 'model/obj'
by further inspection of the asset data.

.. doxygenclass:: mge::asset_loader
    :members:
