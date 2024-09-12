import unittest
import mge

class TestEnum (unittest.TestCase):
    
    def test_enum_type_available(self):
        tl = mge.trace_level

    def test_enum_field(self):
        val = mge.trace_level.DEBUG
        self.assertEqual(val, 1)

