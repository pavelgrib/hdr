#!/usr/bin/bash
import numpy as np
import array

def loadPFMFile(filepath):
	pfmFile = open(filepath, 'rb')
	pfmFile.readline()
	dims = pfmFile.readline()
	width = int(dims.split()[0])
	height = int(dims.split()[1])
	numChannels = 3
	pfmFile.readline()
	data = array.array('f')
	data.read(pfmFile, width * height * 3)
	npData = np.array(data)
	npData.shape = (height, width, numChannels)
	npData = npData[::-1]
	pfmFile.close()
	return npData

def writePFMFile(filepath, data):
	f = open(filepath, 'wb')
	f.write('FP\0x0a\0x0a')
	f.write(str(data.shape[0]) + ' ' + str(data.shape[1]) + '\0x0a')
	f.write('-1.0\0x0a')
	rev = data[::-1].astype(np.float32)
	rev.tofile(f)
	f.flush()
	f.close()