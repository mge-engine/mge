# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
import unittest
import mge


class TestSmoke(unittest.TestCase):

    def test_simple_construct(self):
        """Tests default construction"""
        v = mge.fvec2()

    def test_kwargs_unsupported(self):
        """Tests that keyword args are currently unsupported"""
        with self.assertRaises(SystemError):
            v = mge.fvec2(bubba=17)

    def test_default_field_access(self):
        """Test a default constructor"""
        v = mge.fvec2()
        self.assertEqual(0.0, v.x)
        self.assertEqual(0.0, v.y)

    def test_set_field_successful(self):
        """Test setting a field successfully"""
        p = mge.point()
        p.x = 17
        self.assertEqual(17, p.x)

    def test_set_field_unsuccessful(self):
        """Test setting a field successfully"""
        p = mge.point()
        with self.assertRaises(ValueError):
            p.x = 'hossa'

    def test_application_type_exists(self):
        """Test existence of application type"""
        x = mge.application
        self.assertEqual("<class 'application'>", repr(x))

    def test_application_subtype_exists(self):
        """Test existence of application type"""
        x = mge.application.input_listener
        self.assertEqual(
            "<class 'class std::function<void __cdecl(void)>'>", repr(x))

    def test_application_similar_subtype_exists(self):
        """Test existence of application type"""
        x = mge.application.input_listener
        y = mge.application.quit_listener
        assert x is not None and y is not None

    def test_method_call(self):
        """Test simple method call"""
        vf = mge.vertex_format(mge.data_type.FLOAT, 3)
        assert vf.binary_size() == 12


if __name__ == "main":
    unittest.main()
