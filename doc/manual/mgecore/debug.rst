.. _mgecore_debug:

**********************
Debugging and Analysis
**********************

Crash
=====

Sometimes it is necessary to just terminate the application as no recovery
from errors is possible. The :any:`mge::crash` functions provide this
functionality:

.. doxygenfunction:: mge::crash
    :project: mge

.. doxygenfunction:: mge::crash(const char *message)
    :project: mge

.. doxygenfunction:: mge::crash(Args... args)
    :project: mge

A :any:`MGE_CRASH_ASSERT` macro is defined to help to catch unrecoverable
errors:

.. doxygendefine:: MGE_CRASH_ASSERT
    :project: mge

Debug Break
===========

For debugging, it is useful to programatically break into the attached debugger.
The function :any:`mge::debug_break` achieves that.

.. doxygenfunction:: mge::debug_break()
    :project: mge
