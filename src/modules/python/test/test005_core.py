# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge

class TestCore(unittest.TestCase):

    def test_buffer(self):
        b = mge.buffer()
    
    def test_buffer_from_bytes(self):
        #b = mge.buffer(b"Hello, World!")
        #self.assertEqual(13, b.size())
        #self.assertEqual(b"Hello, World!", bytes(b))
        pass