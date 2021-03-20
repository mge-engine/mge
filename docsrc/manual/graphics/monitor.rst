***************
Monitor Support
***************

Today many systems have more than one monitor attached. Also, emergence
of 4K and higher monitors make it necessary to adjust elements of the
presentation to the actual physical resolution.

MGE allows querying properties of attached monitors, can combines them in
the :any:`mge::monitor` class.

.. doxygenclass:: mge::monitor
    :members:

A monitor has a *video mode* set, and a list of supported video modes.

.. doxygenclass:: mge::video_mode
    :members: