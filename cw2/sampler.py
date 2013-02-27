#!/usr/bin/bash

import PIL.Image as Image
import math, sys, functools
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from imageIO import loadPFMFile, writePFMFile

def upTo255(x):
	if x >= 1.0:
		return int(255)
	else:
		return int(255 * x)

class FPSampler:
	def __init__(self, filepath):
		self.filepath = filepath
		self.npData = loadPFMFile( filepath )
		self.height =  self.npData.shape[0]
		self.width = self.npData.shape[1]

	def cdfSamples(self, numSamples):
		lum = np.average(self.npData, axis=2)
		sinTheta = map(math.sin, np.linspace(0, math.pi, self.npData.shape[0]))
		scaledLum = (lum.T * sinTheta).T

		xUniform = np.random.rand(numSamples)
		yUniform = np.random.rand(numSamples)

		rowAvg = np.average(scaledLum, axis = 1).reshape(512)

		rowCDF = np.zeros(rowAvg.shape[0] + 1)
		rowCDF[1:] = np.cumsum(rowAvg)
		s = rowCDF[-1]
		rowCDF = rowCDF / s

		samples = []
		x = np.searchsorted(rowCDF, xUniform)
	
		for (i, u) in enumerate(yUniform):
			yCDF = np.zeros(scaledLum.shape[1])
			yCDF = np.cumsum(scaledLum[x[i], :])
			s = yCDF[-1]
			yCDF = yCDF / s
			y = np.searchsorted(yCDF, u)
			samples.append((x[i],y))
		return samples

	def phongSamples(self, numSamples, exponent):
		xUniform = np.random.rand(numSamples)
		yUniform = np.random.rand(numSamples)

		def thetaFunc(u):
			return int(2.0 * math.acos(math.pow(1 - u, 1.0/(exponent + 1))) / math.pi * self.height)

		theta = map(thetaFunc, xUniform)
		phi = map(int, yUniform * self.width)
		samples = zip(theta, phi)
		return samples

	def applySamplesToImage(self, samples):
		newImageData = self.npData.copy()
		for v in samples:
			i = v[0]
			j = v[1]
			newImageData[(i-2):(i+2),(j-2):(j+2),0] = newImageData[(i-2):(i+2),(j-2):(j+2),2] = 0
			newImageData[(i-2):(i+2),(j-2):(j+2),1] = 1.0
		return newImageData

	def toPPM(self, pfmData):
		f = np.vectorize(upTo255)
		return f(pfmData)

	def imageValues(self, samples):
		values = list(len(samples))
		for i, sample in enumerate(samples):
			values[i] = self.npData[sample[0], sample[1]]

if __name__ == '__main__':
	gracedir = '/Users/paul/Dropbox/Imperial/advanced-graphics/GraceCathedral/'
	workingdir = '/Users/paul/github/hdr/cw2/'
	sampler = FPSampler(gracedir + 'grace_latlong.pfm')
	for i in [64, 256, 1024]:
		mc_samples = sampler.cdfSamples(i)
		mc_sampled_image = sampler.applySamplesToImage(mc_samples)
		mc_ppm = sampler.toPPM(mc_sampled_image)
		writePFMFile(workingdir + 'grace_cdf' + str(i) + '.pfm', mc_sampled_image)
		img = Image.fromarray(np.uint8(mc_ppm))
		img.save('grace_cdf_' + str(i) + '.ppm')
		for j in [1, 10, 50, 200]:
			phong_samples = sampler.phongSamples(i, j)
			phong_sampled_image = sampler.applySamplesToImage(phong_samples)
			phong_ppm = sampler.toPPM(phong_sampled_image)
			writePFMFile(workingdir + 'phong_' + str(i) + '_' + str(j) + '.pfm', phong_sampled_image)
			img = Image.fromarray(np.uint8(phong_ppm))
			img.save('grace_phong_' + str(i) + '_' + str(j) + '.ppm')
