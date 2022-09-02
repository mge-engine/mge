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

    def test_default_field_access(self):
        """Test a default constructor"""
        v = mge.fvec2()
        self.assertEqual(0.0, v.x)
        self.assertEqual(0.0, v.y)

    def test_set_field_successful(self):
        """Test setting a field successfully"""
        p = mge.point()
        p.x = 17
        self.assertEqual(17, p.x)

    def test_set_field_unsuccessful(self):
        """Test setting a field successfully"""
        p = mge.point()
        with self.assertRaises(ValueError):
            p.x = 'hossa'


if __name__ == "main":
    unittest.main()
