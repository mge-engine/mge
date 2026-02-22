****************
Asset Management
****************

Asset Type
==========

Without additional metadata information, assets are just plain files.
One of this meta information is the asset type, which defines a category
of the asset, such as an image or mesh.

.. doxygenclass:: mge::asset_type
    :members:

Asset
=====

An asset is a resource with additional meta information, such as the asset type.
Assets are identified by a unique path.

.. doxygenclass:: mge::asset
    :members:

Asset Properties
----------------

Assets can have associated metadata stored as properties. These properties
provide additional information about the asset beyond its raw data.

Checking for Properties
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

    mge::asset a("/images/photo.jpg");
    if (a.has_properties()) {
        auto props = a.properties();
        // Access property values
    }

Common Use Cases
~~~~~~~~~~~~~~~~

Properties can store:

- Image dimensions and color space information
- Model vertex/face counts and bounding boxes  
- Audio sample rates and channel counts
- Custom application-specific metadata

Properties are managed by the ``asset_access`` implementation and may be
automatically populated by asset handlers during loading.

Asset Literals
==============

For convenience, the asset library provides user-defined literals to construct
assets and asset types directly from string constants.

Asset Type Literal
------------------

The ``_at`` literal creates an ``asset_type`` from a string in MIME type format:

.. code-block:: cpp

    using namespace mge::literals;
    
    auto image_type = "image/png"_at;
    auto model_type = "model/obj"_at;
    auto shader_type = "application/x-spirv"_at;

Asset Path Literal
------------------

The ``_asset`` literal creates an ``asset`` from a path string:

.. code-block:: cpp

    using namespace mge::string_literals;
    
    auto texture = "/textures/brick.png"_asset;
    auto model = "/models/character.obj"_asset;

Exception Classes
=================

The asset library defines specific exception types for error handling.

Asset Not Found
---------------

Thrown when an asset cannot be located at the specified path.

.. doxygenclass:: mge::asset_not_found

Asset Corrupted
---------------

Thrown when an asset exists but its data is corrupted or cannot be parsed.

.. doxygenclass:: mge::asset_corrupted

Example Usage
-------------

.. code-block:: cpp

    try {
        mge::asset a("/images/missing.png");
        if (!a.exists()) {
            // Handle non-existent asset
        }
        auto data = a.load();
    } catch (const mge::asset_not_found& e) {
        // Asset path not found
        std::cerr << "Asset not found: " << e.what() << std::endl;
    } catch (const mge::asset_corrupted& e) {
        // Asset data is corrupted
        std::cerr << "Asset corrupted: " << e.what() << std::endl;
    }


