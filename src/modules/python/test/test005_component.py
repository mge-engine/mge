# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
import unittest
import mge

class TestComponent(unittest.TestCase):

    def test_registered_components_exists(self):
        self.assertTrue(callable(mge.registered_components))

    def test_registered_components_returns_list(self):
        result = mge.registered_components()
        self.assertIsInstance(result, list)

    def test_registered_components_not_empty(self):
        result = mge.registered_components()
        self.assertGreater(len(result), 0)

    def test_registered_components_contains_strings(self):
        result = mge.registered_components()
        for item in result:
            self.assertIsInstance(item, str)

    def test_registered_implementations_exists(self):
        self.assertTrue(callable(mge.registered_implementations))

    def test_registered_implementations_wrong_arg_count(self):
        with self.assertRaises(TypeError):
            mge.registered_implementations()

    def test_registered_implementations_returns_list(self):
        components = mge.registered_components()
        self.assertGreater(len(components), 0)
        result = mge.registered_implementations(components[0])
        self.assertIsInstance(result, list)

    def test_registered_implementations_contains_strings(self):
        components = mge.registered_components()
        for comp in components:
            impls = mge.registered_implementations(comp)
            for impl in impls:
                self.assertIsInstance(impl, str)
