**********
Asset Tool
**********

The ``mgeassettool`` is a command line utility for managing assets 
for use with the MGE library. 

Usage
=====

.. code-block:: bash

    mgeassettool [OPTIONS] COMMAND [COMMAND OPTIONS] [ARGS]


Common Options
=============

``-h, --help``
    Show help message and exit.

``-v, --version``
    Show version information and exit.

Commands
========

``info`` command
-----------------

    Show information about the asset.

    .. code-block:: bash

        mgeassettool info [-h] [-m path=mount-point] [asset-path]

    Options:

        ``-h, --help``
            Show help message and exit.

        ``-m mount-point=expression``
            Configure a mount point with an expression.

        ``asset-path``
            The path to the asset to show information about. 

