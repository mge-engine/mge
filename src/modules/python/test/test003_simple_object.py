import unittest
import mge

class TestSimpleObject(unittest.TestCase):
    
    def test_simple_object(self):
        v = mge.semantic_version()
        self.assertEqual(v.major(), 0)
        self.assertEqual(v.minor(), 0)
        self.assertEqual(v.patch(), 0)
