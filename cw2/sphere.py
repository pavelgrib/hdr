import numpy as np
from scipy.linalg import norm, det
import PIL.Image as Image
from math import sin, cos, acos, atan2, pi

class SphereRenderer:

	def __init__(self, diameter):
		self.diameter = diameter
		self.radius = diameter / 2
		self.center = (radius, radius)
		self.data = np.zeros( (diameter, diameter) )


	def mapLatLongPixels(self, latlong):
		""" latlong is a n - by - m - by 3 numpy array """
		llShape = latlong.shape
		theta = np.linspace(0, pi, llShape[0])
		phi = np.linspace(0, 2*pi, llShape[1])
		xPos = map( lambda x: int( self.diameter * (0.5 + 0.5*(sin(x[0])*sin(x[1])))), zip(theta, phi) )
		yPos = map( lambda x: int( self.diameter * (0.5 + 0.5*cos(x)) ), theta )
		self.data[xPos, yPox, :] = latlong


def mapLatLongPixels(latlong, diameter, data):
	""" latlong is a n - by - m - by 3 numpy array """
	llShape = latlong.shape
	theta = np.linspace(0, pi, llShape[0])
	phi = np.linspace(0, 2*pi, llShape[1])
	xPos = map( lambda x: int( diameter * (sin(x[0])*sin(x[1]))), zip(theta, phi) )
	yPos = map( lambda x: int( diameter * (0.5 + 0.5*cos(x)) ), theta )
	for i, v in np.ndenumerate(latlong): 
		data[xPos[i[0]], yPos[i[1]], i[3]] = latlong[i]
	return data

