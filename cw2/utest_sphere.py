import numpy as np
from math import pi
from sphere import Sphere, SphereRenderer
import unittest
import imageIO

tol = 1e-11
testdir = '/Users/paul/github/hdr/tests/'

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

    def test_mapXYToPixel(self):
        xyCoord = [0,1]
        pixIdx = self.renderer.mapXYToPixel(xyCoord)
        self.assertTrue( np.allclose( [255, 0], pixIdx))

    def test_ShadeSphereAtPoint(self):
        pt = [pi/2, 0]
        val = [3,4,5]
        self.renderer.shadeSphereAtPoint(pt, val)
        self.assertTrue(np.allclose(self.renderer.data[510,255,:], val, atol=tol))

class TestImageReadWRite(unittest.TestCase):
    def setUp(self):
        self.ppmFilepath = '/Users/paul/github/hdr/GraceCathedral/grace_latlong.ppm'
        self.pfmFilepath = '/Users/paul/github/hdr/GraceCathedral/grace_latlong.pfm'

    def test_readPFM(self):
        d = imageIO.loadPFMFile( self.pfmFilepath )
        self.assertTrue( d.shape == (512, 1024, 3) )
        self.assertTrue( np.average(d) >= 0.01 and np.average(d) <= 0.99 )

    def test_writePFM(self):
        d = np.random.uniform( size = (512, 1024, 3) )
        imageIO.writePFMFile( testdir + 'writing.pfm', d )
        loadedData = imageIO.loadPFMFile( testdir + 'writing.pfm' )
        self.assertTrue( np.allclose( d, loadedData ) )

    def test_readPPM(self):
        d = imageIO.loadPPMFile( self.ppmFilepath )
        self.assertTrue( d.shape == (512, 1024, 3) )
        self.assertTrue( np.average(d) >= 1 and np.average(d) <= 254 )

    def test_writePPM(self):
        d = np.random.uniform( size = (512, 1024, 3) )
        dppm = imageIO.toPPM(d)
        imageIO.writePFMFile( testdir + 'writing.ppm', dppm )
        loadedData = imageIO.loadPFMFile( testdir + 'writing.ppm' )
        self.assertTrue( np.array_equal( dppm, loadedData ) )        


if __name__ == '__main__':
    unittest.main()