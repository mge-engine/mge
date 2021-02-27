*****************
Keyboard Handling
*****************

Constants for pressed keys are defined in the `key` enumeration.

..  cpp:enum-class:: mge::key

    Key codes for keys. These are of course to numerous to list
    here, the enumeration follows the following guidelines:

    - all enum values are uppercase
    - key codes for keys that produce an ASCII char are equal to that character,
      e.g `mge::key::A` has a value of 0x41
    - function keys are named `F<n>` for n in [1,12]
    - names of special keys are as usually written on a PC keyboard
      (`PAUSE`, `HOME` and so on)
    - Key pad keys are prefixed with `KP_` (like `KP_1`, `KP_ENTER`).

Modifiers are captured in the modifier set.

..  cpp:type:: mge::modifier = mge::enum_set<mge::modifier_value, mge::modifier_value::ALT>

    Modifiers.

..  cpp:enum-class:: mge::modifier_value

    Codes for modifier keys.

    ..  cpp:enumerator:: CONTROL

        Control key.

    ..  cpp:enumerator:: SHIFT

        Shift key.

    ..  cpp:enumerator:: ALT

        Alt key.
