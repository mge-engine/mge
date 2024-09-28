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