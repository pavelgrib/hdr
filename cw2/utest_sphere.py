import numpy as np
from math import pi
from sphere import Sphere, SphereRenderer
import unittest

tol = 1e-10

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
        self.assertTrue(np.allclose([0,1,0], predictedR, atol=tol))


class TestSphereRenderer(unittest.TestCase):

    def setUp(self):
        self.renderer = SphereRenderer(511)

    def test_ShadeSphereAtPoint(self):
        pt = [0, 0]
        val = [3,4,5]
        self.renderer.shadeSphereAtPoint(pt, val)
        self.assertTrue(np.allclose(self.renderer.data[0,255,:], val, atol=tol))

if __name__ == '__main__':
    unittest.main()