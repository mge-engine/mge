# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge

class TestRenderSystem(unittest.TestCase):
    @unittest.skip("Temporarily disabled, need application instance")
    def test_create(self):
        s = mge.render_system.create()
        self.assertIsNotNone(s)