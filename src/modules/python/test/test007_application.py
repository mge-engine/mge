# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
import unittest
import mge

class TestApplication(unittest.TestCase):

    def test_application_type_exists(self):
        self.assertIsNotNone(mge.application)

    def test_application_main_exists(self):
        self.assertTrue(callable(mge.application.main))

    def test_application_main_rejects_too_few_args(self):
        with self.assertRaises(TypeError):
            mge.application.main()

    def test_application_main_rejects_too_many_args(self):
        with self.assertRaises(TypeError):
            mge.application.main("a", "b")

    def test_application_main_rejects_keywords(self):
        with self.assertRaises(TypeError):
            mge.application.main(name="foo")

    def test_application_instance_static_exists(self):
        self.assertTrue(callable(mge.application.instance))
