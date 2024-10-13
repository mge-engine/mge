# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge

class TestObject(unittest.TestCase):
    
    def test_type_exists(self):
        t = mge.semantic_version

    def test_type_call_keywords_unsupported(self):
        with self.assertRaises(TypeError):
            mge.semantic_version(foo=1)

    def test_type_error_not_matching_constructor(self):
        with self.assertRaises(TypeError):
            mge.semantic_version(1,2,3,4,5,6,7)

    def test_simple_construct(self):
        """Simple construct without verification of result"""
        x = mge.semantic_version(1,2,3)

    def test_simple_construct_verify(self):
        x = mge.semantic_version(1,2,3)
        self.assertEqual(x.major(), 1)
        self.assertEqual(x.minor(), 2)
        self.assertEqual(x.patch(), 3)
    
    def test_field_access(self):
        v = mge.fvec2
        v.x = 123
        v.y = 456
        self.assertEqual(v.x, 123)
        self.assertEqual(v.y, 456)
