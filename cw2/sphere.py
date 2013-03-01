import numpy as np
from scipy.linalg import norm, det
import PIL.Image as Image
from math import sin, cos, acos, atan2, pi, fabs, sqrt
from imageIO import loadPFMFile

tol = 1e-8


def latlongToAngular(latlong, heightIdx, widthIdx):
	height = float(latlong.shape[0])
	width = float(latlong.shape[1])
	theta = heightIdx / (height - 1) * pi
	phi = ((width - 1) - widthIdx) / (width - 1) * 2.0 * pi
	return np.array([theta, phi])

class Sphere:
	def __init__(self, center, diameter):
		self.diameter = diameter
		self.radius = diameter / 2.0
		self.center = center

	def insideSphere(self, xyzCoord):
		return norm(self.center - xyzCoord) <= self.radius

	def normal(self, theta, phi):
		return self.xyzPoint(theta, phi) - self.center

	def reflection(self, view, normal):
		return 2.0 * np.dot(normal, view) * normal - view

	def xyzPoint(self, theta, phi):
		x = np.around( self.radius * sin(theta) * cos(phi), 12 )
		y = np.around( self.radius * cos(theta), 12 )
		z = np.around( self.radius * sin(theta) * sin(phi), 12 )
		return np.array([x,y,z])

	def sphericalCoord(self, xyzPoint):
		if fabs(norm( xyzPoint - self.center ) - radius) < tol:
			theta = acos( xyzPoint[1] )
			phi = atan2( xyzPoint[2], xyzPoint[0] )
			return np.array([theta, phi])
		else:
			return -1



class SphereRenderer:
	def __init__(self, diameter):
		self.sphere = Sphere([0,0,0], 2.0)
		self.data = np.zeros( (diameter, diameter, 3), dtype=np.float32 )
		self.pixelDiameter = diameter
		self.radius = 1.0

	def insideRenderedSphere(self, xyCoord ):
		return norm( xyCoord ) <= self.radius

	def shadeSphereAtPoint(self, polarCoord, rgbValues):
		xyzCoord = self.sphere.xyzPoint(polarCoord[0], polarCoord[1])
		idx = self.mapXYToPixel(xyzCoord)
		self.data[idx[0], idx[1], :] = rgbValues
		
	def getXYZfromXY(self, xyCoord):
		x = xyCoord[0]
		y = xyCoord[1]
		z = sqrt( 1.0 - x*x - y*y )
		return np.array( [x, y, z] )

	def mapXYToPixel(self, xyCoord):
		""" x and y go from -1 to +1 """
		return map( int, [(self.diameter - 1) * (xyCoord[0] + 1) / 2.0, \
			(self.diameter - 1) * (1 - xyCoord[1]) / 2.0] )

	def mapLatlong(self, latlong, view = [0, 0, 1]):
		""" for each point inside the rendered sphere, calculate where it reflects 
            to on the latlong map from the view of [0,0,1] """
		# determining which points in self.data are to be shaded
		# this is really bad, needs to be vectorized for performance
		# right now, takes a few seconds for 512 x 1024
		d = self.pixelDiameter
		for i in range(d):
			for j in range(d):
				y = 1.0 - 2.0 * float(i) / (d - 1)
				x = 2.0 * ((d - 1) - float(j)) / (d - 1) - 1.0
				if self.insideRenderedSphere( [x, y] ):
					z = sqrt( round(1.0 - x*x - y*y, 10) )
					r = self.sphere.reflection( view, np.array([x, y, z]) )
					[theta, phi] = self.sphere.sphericalCoord( r )
					llx = int( (latlong.shape[0] - 1) * theta / pi )
					lly = int( (latlong.shape[1] - 1) * (pi/2 + phi) / (2*pi) )
					self.data[i, j, :] = latlong[llx, lly, :]

	def mapLatlongSamples(self, latlong, samples):
		integral = np.average(latlong)
		d = self.pixelDiameter
		for i in range(d):
			print i
			for j in range(d):
				y = 1.0 - 2.0 * float(i) / (d - 1)
				x = 2.0 * ((d - 1) - float(j)) / (d - 1) - 1.0
				if self.insideRenderedSphere( [x, y] ):
					z = sqrt( round(1.0 - x*x - y*y, 10) )
					val = np.zeros(3)
					normalized = map( lambda x: x / norm(x), latlong[zip(*samples)] )
					theta = np.array(zip(*samples)[0]) * pi / (latlong.shape[0] - 1)
					val= np.sum( np.cos(theta) * np.array(normalized).T / pi)
					self.data[i,j,:] = val