.. _mgecore_artifact:

******************
Artifact Inventory
******************

MGE has a number of components that are optional, and also uses a number of
third-party components. All these third-party components have a license,
which may apply usage restrictions or require attribution to the copyright
holder.

Using :any:`mge::artifact` and :any:`mge::license` will help you manage your
dependencies and properly handle them.

Artifact
--------

An artifact is a library or a piece of source code that is included
in the software. It has:

- an identifier, which is used to uniquely identify it and refer to it
  in dependencies
- a name of an author or owning organization that is used in attribution
- an URL of a web page used in the attribution
- a version
- a license

Programmatically, artifacts can be accessed using the class ``mge::artifact``:

.. doxygenclass:: mge::artifact
    :project: mge
    :members:

An artifact is usually defined by use of the :any:`MGE_ARTIFACT` macro.

.. doxygendefine:: MGE_ARTIFACT
    :project: mge

Usually, this is chained with calls to ``depends_on`` and ``used_by`` methods:

.. code-block:: c++
    MGE_ARTIFACT(foobar,
                "Foo Inc.",
                mge::version("1.2.3"),
                mge::license(mge::license_type::MIT))
        ->used_by("baz")
        ->depends_on("flugelduvel");

License
-------

The purpose of license mentioning when specifying an artifact is just to
monitor what licenses are used by components, without any implications
done by MGE directly. However it allows you to check the bill of materials
for external components used.

A license has a *type* which allows to describe well-known types of licenses,
or categories of licenses which come with their own text. Well known types are
for instance GPL, LGPL or MIT license.

.. doxygenclass:: mge::license
    :project: mge
    :members:

