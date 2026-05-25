# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
import unittest
import gc
import mge


class TestProxySubclass(unittest.TestCase):

    def tearDown(self):
        gc.collect()

    def test_application_construct(self):
        app = mge.application()
        self.assertIsNotNone(app)

    def test_application_construct_gc(self):
        app = mge.application()
        del app
        gc.collect()

    def test_application_subclass_construct(self):
        class MyApp(mge.application):
            pass

        app = MyApp()
        self.assertIsNotNone(app)

    def test_application_subclass_isinstance(self):
        class MyApp(mge.application):
            pass

        app = MyApp()
        self.assertIsInstance(app, mge.application)

    def test_application_subclass_gc(self):
        class MyApp(mge.application):
            pass

        app = MyApp()
        del app
        gc.collect()

    def test_application_is_quit_initially_false(self):
        app = mge.application()
        self.assertFalse(app.is_quit())

    def test_application_set_quit(self):
        app = mge.application()
        app.set_quit()
        self.assertTrue(app.is_quit())

    def test_application_subclass_nooverride_calls_base(self):
        # Subclass without setup override: calling app.setup() must go
        # through the C++ proxy dispatch, find no Python override, and
        # fall back to the base implementation without crashing.
        class BareApp(mge.application):
            pass

        app = BareApp()
        app.setup()

    def test_application_subclass_nooverride_teardown(self):
        class BareApp(mge.application):
            pass

        app = BareApp()
        app.teardown()

    def test_application_subclass_keyword_args_rejected(self):
        class MyApp(mge.application):
            pass

        with self.assertRaises(TypeError):
            MyApp(name="foo")
