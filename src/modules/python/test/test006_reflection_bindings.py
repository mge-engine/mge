# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
import unittest
import mge

class TestReflectionBindings(unittest.TestCase):

    def test_rgba_color_constructor(self):
        c = mge.rgba_color(0.1, 0.2, 0.3, 0.4)
        self.assertIsNotNone(c)

    def test_rgba_color_fields_read(self):
        c = mge.rgba_color(0.1, 0.2, 0.3, 0.4)
        self.assertAlmostEqual(c.r, 0.1, places=3)
        self.assertAlmostEqual(c.g, 0.2, places=3)
        self.assertAlmostEqual(c.b, 0.3, places=3)
        self.assertAlmostEqual(c.a, 0.4, places=3)

    def test_rgba_color_fields_write(self):
        c = mge.rgba_color(0.0, 0.0, 0.0, 0.0)
        c.r = 1.0
        c.g = 0.5
        c.b = 0.25
        c.a = 0.75
        self.assertAlmostEqual(c.r, 1.0, places=3)
        self.assertAlmostEqual(c.g, 0.5, places=3)
        self.assertAlmostEqual(c.b, 0.25, places=3)
        self.assertAlmostEqual(c.a, 0.75, places=3)

    def test_key_enum_exists(self):
        self.assertIsNotNone(mge.key)

    def test_key_enum_values(self):
        self.assertIsNotNone(mge.key.ESCAPE)
        self.assertIsNotNone(mge.key.SPACE)
        self.assertIsNotNone(mge.key.ENTER)
        self.assertIsNotNone(mge.key.A)
        self.assertIsNotNone(mge.key.Z)
        self.assertIsNotNone(mge.key.F1)
        self.assertIsNotNone(mge.key.F12)

    def test_key_action_enum_exists(self):
        self.assertIsNotNone(mge.key_action)

    def test_key_action_enum_values(self):
        self.assertIsNotNone(mge.key_action.PRESS)
        self.assertIsNotNone(mge.key_action.RELEASE)
        self.assertIsNotNone(mge.key_action.REPEAT)

    def test_data_type_enum_exists(self):
        self.assertIsNotNone(mge.data_type)

    def test_data_type_enum_values(self):
        self.assertIsNotNone(mge.data_type.HALF)
        self.assertIsNotNone(mge.data_type.FLOAT)
        self.assertEqual(mge.data_type.HALF, 11)

    def test_window_type_exists(self):
        self.assertIsNotNone(mge.window)

    def test_render_context_type_exists(self):
        self.assertIsNotNone(mge.render_context)

    def test_pass_type_exists(self):
        # 'pass' is a Python keyword; access via getattr
        pass_type = getattr(mge, 'pass')
        self.assertIsNotNone(pass_type)

    def test_application_type_exists(self):
        self.assertIsNotNone(mge.application)

    def test_vertex_format_constructor(self):
        vf = mge.vertex_format(mge.data_type.FLOAT, 3)
        self.assertIsNotNone(vf)

    def test_vertex_format_binary_size(self):
        vf = mge.vertex_format(mge.data_type.FLOAT, 3)
        self.assertEqual(vf.binary_size(), 12)
