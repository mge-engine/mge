# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge

class TestApplication(mge.application):
    def __init__(self):

    def setup(self):
        pass

    def async_setup(self):
        pass

    def teardown(self):
        pass

    def run(self):
        pass

mge.component.register("test_application", TestApplication)

class TestRenderSystem(unittest.TestCase):
    @unittest.skip("Temporarily disabled, need application instance")
    def test_create(self):
        s = mge.render_system.create()
        self.assertIsNotNone(s)