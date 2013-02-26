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
	return x, y, z

def thetaPhiFromLatlong( i, j, latlongData ):
	theta = i * math.pi / latlongData.shape[0]
	phi = j * 2 * math.pi / latlongData.shape[1]
	return theta, phi

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
		luminance = np.average(imageData, axis=2)
		d_theta = math.pi / float(luminance.shape[0])
		d_phi = 2*math.pi / float(luminance.shape[1])
		for idx, v in np.ndenumerate(luminance):
			theta = d_theta * idx[0]
			d_omega = math.sin(theta) * d_theta * d_phi
			self.integral += v * d_omega

	def monteCarloSum(self, sampleValues, spherePixel):
		self.mcSum = 0.0
		theta = spherePoint[0] * 2 * math.pi / (self.diameter + 1)
		for s in sampleValues:
			self.mcSum += cos(theta) * (s / norm(s)) / math.pi

	def mapSamplesOntoSphere(self, sampler, numSamples):
		if not self.data:
			self.emptySphere()
		if not self.integral:
			self.precomputeIntegral(sampler.npData)

		samples = sampler.cdfSamples(numSamples)
		sampleValues = sampler.sampleValues()
		for i, v in np.ndenumerate(self.data[self.data!=0]):
			theta, phi = thetaPhiFromLatlong((i[0], i[1]), npData)
			x,y,z = cartesianCoord( (theta, phi) )
			rgbValue = self.integral * monteCarloSum(sampleValues, (i))

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

