# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge
import __mge__

class TestComponent(unittest.TestCase):

    def test_registry(self):
        r = __mge__.__component__registry__()
        self.assertIsNotNone(r)
