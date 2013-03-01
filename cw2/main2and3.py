#!/usr/bin/bash
import PIL.Image as Image
import numpy as np
from sampler import FPSampler
from imageIO import loadPFMFile, writePFMFile, writePPMFile

def main():
	gracedir = '/Users/paul/github/hdr/GraceCathedral/'
	workingdir = '/Users/paul/github/hdr/cw2/'
	sampler = FPSampler(gracedir + 'grace_latlong.pfm')
	for i in [64, 256, 1024]:
		mc_samples = sampler.cdfSamples(i)
		mc_sampled_image = sampler.applySamplesToImage(mc_samples)
		mc_ppm = sampler.toPPM(mc_sampled_image)
		writePFMFile(workingdir + 'grace_cdf' + str(i) + '.pfm', mc_sampled_image)
		writePPMFile('grace_cdf_' + str(i) + '.ppm', mc_ppm)
		for j in [1, 10, 50, 200]:
			phong_samples = sampler.phongSamples(i, j)
			phong_sampled_image = sampler.applySamplesToImage(phong_samples)
			phong_ppm = sampler.toPPM(phong_sampled_image)
			writePFMFile(workingdir + 'phong_' + str(i) + '_' + str(j) + '.pfm', phong_sampled_image)
			writePPMFile('grace_phong_'+str(i)+'_'+str(j)+'.ppm', phong_ppm)