# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import unittest
import mge


class TestInheritance(unittest.TestCase):

    def test_inheritance(self):
        class app(mge.application):
            pass


if __name__ == "main":
    unittest.main()
