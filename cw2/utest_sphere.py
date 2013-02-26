import numpy as np
from math import pi
from sphere import Sphere
import unittest

class TestSphereFunctions(unittest.TestCase):

    def setUp(self):
        self.sphere = Sphere(np.array([0,0,0]), 2.0)

    def test_sphere_size(self):
        self.assertEqual(self.sphere.diameter, 2)

    def test_inSphere(self):
        x = [11, 11, 11]
        self.assertFalse(self.sphere.insideSphere(x))

    def test_xyzPoint(self):
        theta = 0
        phi = 0
        predictedXYZ = self.sphere.xyzPoint(theta, phi)
        self.assertTrue(np.array_equal([0,1,0], predictedXYZ))

    def test_reflection(self):
        v = [0,0,1]
        theta = pi/4.0
        phi = pi/2.0
        predictedR = self.sphere.reflection(v, theta, phi)
        self.assertTrue(np.allclose([0,1,0], predictedR, atol=1e-8))

if __name__ == '__main__':
    unittest.main()