*************
Command Buffers
*************

Command buffers capture state changes on a graphics pipeline.

They are used to record commands that will be sent to the GPU. In a multi-threaded
application, command buffers can be recorded on multiple threads and then submitted
to the GPU on a single thread.

.. doxygenclass:: mge::command_buffer
    :members:

