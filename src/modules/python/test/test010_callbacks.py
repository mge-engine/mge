# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
import unittest
import gc
import mge


class TestCallbacks(unittest.TestCase):

    def tearDown(self):
        gc.collect()

    def test_add_redraw_listener_exists(self):
        app = mge.application()
        self.assertTrue(hasattr(app, 'add_redraw_listener'))

    def test_add_redraw_listener_accepts_callable(self):
        class CallbackApp(mge.application):
            listener_registered = False

            def setup(self):
                def on_redraw(cycle, delta):
                    pass
                self.add_redraw_listener(on_redraw)
                CallbackApp.listener_registered = True

        CallbackApp.__mge_name__ = "test_py_callback_app"
        mge.register_component(mge.application, CallbackApp)
        app = mge.create_component(mge.application, "test_py_callback_app")
        app.setup()
        self.assertTrue(CallbackApp.listener_registered)

    def test_add_redraw_listener_lambda(self):
        class LambdaApp(mge.application):
            registered = False

            def setup(self):
                self.add_redraw_listener(lambda cycle, delta: None)
                LambdaApp.registered = True

        LambdaApp.__mge_name__ = "test_py_lambda_app"
        mge.register_component(mge.application, LambdaApp)
        app = mge.create_component(mge.application, "test_py_lambda_app")
        app.setup()
        self.assertTrue(LambdaApp.registered)
