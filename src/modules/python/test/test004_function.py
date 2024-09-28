# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge

class TestFunction(unittest.TestCase):
    
    def test_function_exists(self):
        t = mge.is_debugger_present

    def test_function_call_keywords_unsupported(self):
        with self.assertRaises(TypeError):
            mge.is_debugger_present(foo=1)