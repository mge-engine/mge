# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge


class TestComponent(unittest.TestCase):

    def test_register_methods_exist(self):
        import __mge__
        # Verify the component_registry instance exists
        self.assertTrue(hasattr(__mge__, 'register_component'))
        self.assertTrue(hasattr(__mge__, 'create_component'))
