# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge


class TestComponent(unittest.TestCase):

    def test_registry(self):
        import __mge__
        # Verify the component_registry instance exists
        self.assertTrue(hasattr(__mge__, 'component_registry'), 'component_registry should exist in __mge__ module')
        registry = __mge__.component_registry
        self.assertIsNotNone(registry, 'component_registry instance should not be None')
