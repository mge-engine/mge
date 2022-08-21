import unittest
import mge


class TestSmoke(unittest.TestCase):

    def test_simple_construct(self):
        """Tests default construction"""
        v = mge.fvec2()

    def test_kwargs_unsupported(self):
        """Tests that keyword args are currently unsupported"""
        with self.assertRaises(SystemError):
            v = mge.fvec2(bubba=17)


if __name__ == "main":
    unittest.main()
