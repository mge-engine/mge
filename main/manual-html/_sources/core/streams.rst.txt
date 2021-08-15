*******
Streams
*******

MGE defines an input and outüut stream base, which allows an easy implementation
of custom data sources or targets. These streams also each provide a method to
retrieve a :any:`std::istream` or :any:`std::ostream`.

Input Stream
============

.. doxygenclass:: mge::input_stream
    :members:

Output Stream
=============

.. doxygenclass:: mge::output_stream
    :members:
