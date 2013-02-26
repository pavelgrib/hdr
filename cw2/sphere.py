import numpy as np
from scipy.linalg import norm, det
import PIL.Image as Image
from math import sin, cos, acos, atan2, pi


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
		x = self.radius * sin(theta) * cos(phi)
		y = self.radius * cos(theta)
		z = self.radius * sin(theta) * sin(phi)
		return np.array([x,y,z])


class SphereRenderer:
	def __init__(self, diameter):
		self.diameter = diameter
		self.radius = diameter / 2
		self.center = (radius, radius)
		self.data = np.zeros( (diameter, diameter) )

	def shadeSphereAtPoint(self, point, rgbValue):
		data[point, :] = rgbValue

	def mapXYToPixel(self, xyCoord):
		""" x and y go from -1 to +1 """
		result = np.zeros(2)
		result[0] = diameter * (y + 1) / 2.0
		result[1] = diameter * (x + 1) / 2.0
		return result

	def mapLatLongPixels(self, latlong, diameter, data):
		""" latlong is a n - by - m - by 3 numpy array """
		dims = latlong.shape
		theta = np.linspace(0, pi, llShape[0])
		phi = np.linspace(0, 2*pi, llShape[1])
		xPos = map( lambda x: int( diameter * (sin(x[0])*sin(x[1]))), \
			zip(theta, phi) )
		yPos = map( lambda x: int( diameter * (0.5 + 0.5*cos(x)) ), theta )
		for i, v in np.ndenumerate(latlong): 
			data[xPos[i[0]], yPos[i[1]], i[3]] = latlong[i]
		return data

