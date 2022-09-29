# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
import mge
import sys


class blackscreen(mge.application):

    def setup(self):
        self.render_system = mge.render_system.create()
        self.window = self.render_system.create_window()

        def on_close():
            self.set_quit()

        self.window.set_close_listener(on_close)

        def on_key(keycode, action, modifier):
            if (action == mge.key_action.PRESS and keycode == mge.key.ESCAPE):
                self.set_quit()

        self.window.set_key_action_handler(on_key)

        self.clear_commands = self.window.render_context().create_command_list()
        self.clear_commands.clear(mge.rgba_color(0.0, 0.0, 0.0, 1.0))

        def redraw(cycle, delta):
            self.clear_commands.execute()
            self.window.render_context().swap_chain().present()

        self.add_redraw_listener(redraw)
        self.window.show()


if __name__ == "__main__":
    mge.script.component.register_implementation(blackscreen)
    mge.application.main(sys.argv)
