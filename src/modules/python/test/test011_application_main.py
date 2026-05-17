# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
import unittest
import mge


class TestApplicationMain(unittest.TestCase):

    def test_main_static_method_exists(self):
        self.assertTrue(hasattr(mge.application, 'main'))

    def test_main_runs_and_calls_setup(self):
        class QuickApp(mge.application):
            setup_called = False

            def setup(self):
                QuickApp.setup_called = True
                self.set_quit()

        QuickApp.__mge_name__ = "test_py_quick_app"
        mge.register_component(mge.application, QuickApp)
        rc = mge.application.main("test_py_quick_app")
        self.assertTrue(QuickApp.setup_called)
        self.assertEqual(rc, 0)

    def test_main_calls_teardown(self):
        class TearApp(mge.application):
            teardown_called = False

            def setup(self):
                self.set_quit()

            def teardown(self):
                TearApp.teardown_called = True

        TearApp.__mge_name__ = "test_py_tear_app"
        mge.register_component(mge.application, TearApp)
        rc = mge.application.main("test_py_tear_app")
        self.assertTrue(TearApp.teardown_called)
        self.assertEqual(rc, 0)
