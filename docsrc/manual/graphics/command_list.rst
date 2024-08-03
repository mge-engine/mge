*************
Command Lists
*************

Command lists capture state changes on a graphics pipeline.

They are used to record commands that will be sent to the GPU. In a multi-threaded
application, command lists can be recorded on multiple threads and then submitted
to the GPU on a single thread.

.. doxygenclass:: mge::command_list
    :members:

Often, recording will be done in the context of the current frame, or 
a target frame buffer. In this case, a frame command list should be used
for performance reasons.

.. doxygenclass:: mge::frame_command_list