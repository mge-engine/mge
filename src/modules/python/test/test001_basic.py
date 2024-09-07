import unittest

class TestBasic(unittest.TestCase):
    def test_helper_module(self):
        """Test helper module"""
        import __mge__
        self.assertTrue(__mge__ is not None)
    def test_usual_modules(self):
        """Test usual modules"""
        import mge
        self.assertTrue(mge is not None)
        import std
        self.assertTrue(std is not None)
        
if __name__ == "main":
    unittest.main()
