#!/usr/bin/bash

import numpy as np
from scipy.linalg import norm, det
import PIL.Image as Image
import math, sys
import matplotlib.pyplot as plt
import sampler

def sphericalCoord( cartesianCoord ):
	theta = math.acos( cartesianCoord[1] )
	phi = math.atan2( cartesianCoord[2], cartesianCoord[0] )
	r = math.sqrt( cartesianCoord**2 + cartesianCoord**2 + cartesianCoord**2 )
	return (theta, phi, r)

def cartesianCoord( sphericalCoord ):
	theta = sphericalCoord[0]
	phi = sphericalCoord[1]
	x = math.sin( theta ) * math.cos( phi )
	y = math.sin( theta ) * math.sin( phi )
	z = math.cos( theta )
	return (x, y, z)

def reflectedVector(v, n):
	return np.dot(n, v) * n - v

def distance(pointA, pointB):
	return sqrt( (pointA[0] - pointB[0])**2 + (pointA[1] - pointB[1])**2 )

def normalizePixel(pixel):
	n = norm(pixel)
	return (pixel[0]/n, pixel[1]/n, pixel[2]/n)

class SampledSphere:
	def __init__(self, sphereDiameter):
		self.diameter = sphereDiameter
		self.imageData = sampler.npData

	def emptySphere(self):
		self.data = np.ones(shape=(self.diameter, self.diameter, 3))
		r = self.diameter / 2
		center = (r, r)

		for index in np.ndindex(self.diameter, self.diameter):
			if distance(index, (r, r)) > r:
				self.data[index[0], index[1], :] = 0

	def precomputeIntegral(self, imageData):
		self.integral = 0.0
		for idx, v in np.ndenumerate(imageData):
			self.integral += v / N

	def mapSamples(self, samples):
		for sample in samples:


	def saveSphereImage(self, filepath, asFloatingPoint):
		pass

if __name__ == '__main__':
	workingdir = '/Users/paul/github/hdr'
	grace_filepath = '/Users/paul/Dropbox/Imperial/advanced-graphics/GraceCathedra/grace_latlong.pfm'
	sampler = FPSampler(grace_filepath)
	for i in [64, 256, 1024]:
		s = SampledSphere(511)
		s.emptySphere()
		s.precomputeIntegral()
		mcSamples = sampler.cdfSamples(i)
		values = sampler.imageValues(mcSamples)
		s.mapSamples(samples)
		s.saveSphereImage(workingdir+'sampledSphere_' + str(i) + '.pfm', True)
		s.saveSphereImage(workingdir+'sampledSphere_' + str(i) + '.pfm', False)

