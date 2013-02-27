import numpy as np
from scipy.linalg import norm, det
import PIL.Image as Image
from math import sin, cos, acos, atan2, pi
from imageIO import loadPFMFile

class Sphere:
	def __init__(self, center, diameter):
		self.diameter = diameter
		self.radius = diameter / 2.0
		self.center = center

	def insideSphere(self, xyzCoord):
		return norm(self.center - xyzCoord) < self.diameter / 2.0

	def normal(self, theta, phi):
		return self.xyzPoint(theta, phi) - self.center

	def reflection(self, view, theta, phi):
		n = self.normal(theta, phi)
		return 2.0 * np.dot(n, view) * n - view

	def xyzPoint(self, theta, phi):
		x = np.around( self.radius * sin(theta) * cos(phi), 12 )
		y = np.around( self.radius * cos(theta), 12 )
		z = np.around( self.radius * sin(theta) * sin(phi), 12 )
		return np.array([x,y,z])


class SphereRenderer:
	def __init__(self, diameter):
		self.sphere = Sphere([0,0,0], 2.0)
		self.data = np.zeros( (diameter, diameter, 3) )
		self.diameter = diameter

	def shadeSphereAtPoint(self, polarCoord, rgbValues):
		xyzCoord = self.sphere.xyzPoint(polarCoord[0], polarCoord[1])
		idx = self.mapXYToPixel(xyzCoord)
		self.data[idx[0], idx[1], :] = rgbValues
		

	def mapXYToPixel(self, xyCoord):
		""" x and y go from -1 to +1 """
		return map( int, [(self.diameter - 1) * (xyCoord[0] + 1) / 2.0, \
			(self.diameter - 1) * (1 - xyCoord[1]) / 2.0] )

	def mapLatlong(self, latlongFilePath):
		latlong = loadPFMFile( latlongFilePath )
		theta = np.linspace(0, pi, self.diameter-1)
		phi = np.linspace(0, 2*pi, self.diameter-1)
		xyzPoints = map(self.sphere.xyzPoint, theta, phi)
		print xyzPoints[1:10,1:10,1]

	# def mapLatLongPixels(self, latlong, diameter, data):
	# 	""" latlong is a n - by - m - by 3 numpy array """
	# 	dims = latlong.shape
	# 	theta = np.linspace(0, pi, llShape[0])
	# 	phi = np.linspace(0, 2*pi, llShape[1])
	# 	xPos = map( lambda x: int( diameter * (sin(x[0])*sin(x[1]))), \
	# 		zip(theta, phi) )
	# 	yPos = map( lambda x: int( diameter * (0.5 + 0.5*cos(x)) ), theta )
	# 	for i, v in np.ndenumerate(latlong): 
	# 		data[xPos[i[0]], yPos[i[1]], i[3]] = latlong[i]
	# 	return data

	if __name__=='__main__':
		renderer = SphereRenderer(511)
		latlongPath = '~/Dropbox/Imperial/advanced-graphics/GraceCathedral/grace_latlong.pfm'