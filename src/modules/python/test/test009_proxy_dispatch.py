# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
import unittest
import gc
import mge


class TestProxyDispatch(unittest.TestCase):

    def tearDown(self):
        gc.collect()

    def test_register_component_exists(self):
        self.assertTrue(callable(mge.register_component))

    def test_create_component_exists(self):
        self.assertTrue(callable(mge.create_component))

    def test_register_component_arg1_must_be_type(self):
        with self.assertRaises(TypeError):
            mge.register_component(42, mge.application)

    def test_register_component_arg1_must_have_proxy(self):
        # rgba_color is a class but has no proxy
        class DerivedColor(mge.rgba_color):
            pass
        with self.assertRaises(TypeError):
            mge.register_component(mge.rgba_color, DerivedColor)

    def test_register_component_arg2_must_be_subclass(self):
        class NotApp:
            pass
        with self.assertRaises(TypeError):
            mge.register_component(mge.application, NotApp)

    def test_setup_override_fires(self):
        class SetupApp2(mge.application):
            def setup(self):
                SetupApp2.setup_called = True

        SetupApp2.__mge_name__ = "test_py_setup_app2"
        SetupApp2.setup_called = False

        mge.register_component(mge.application, SetupApp2)
        app = mge.create_component(mge.application, "test_py_setup_app2")
        self.assertIsNotNone(app)
        app.setup()
        self.assertTrue(SetupApp2.setup_called)

    def test_teardown_override_fires(self):
        class TeardownApp(mge.application):
            def teardown(self):
                TeardownApp.teardown_called = True

        TeardownApp.__mge_name__ = "test_py_teardown_app"
        TeardownApp.teardown_called = False

        mge.register_component(mge.application, TeardownApp)
        app = mge.create_component(mge.application, "test_py_teardown_app")
        app.teardown()
        self.assertTrue(TeardownApp.teardown_called)

    def test_no_override_calls_base(self):
        class BareApp(mge.application):
            pass

        BareApp.__mge_name__ = "test_py_bare_app"
        mge.register_component(mge.application, BareApp)
        app = mge.create_component(mge.application, "test_py_bare_app")
        # Should call base application::setup() without error
        app.setup()

    def test_create_component_instance_is_application(self):
        class CheckApp(mge.application):
            pass

        CheckApp.__mge_name__ = "test_py_check_app"
        mge.register_component(mge.application, CheckApp)
        app = mge.create_component(mge.application, "test_py_check_app")
        self.assertIsInstance(app, mge.application)
