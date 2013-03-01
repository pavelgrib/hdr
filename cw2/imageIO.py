#!/usr/bin/bash
import numpy as np
import array
import PIL.Image as Image
from struct import pack

def loadPFMFile(filepath):
	pfmFile = open(filepath, 'rb')
	pfmFile.readline()
	dims = map(int, pfmFile.readline().split())
	numChannels = 3
	pfmFile.readline()
	data = np.fromfile(pfmFile, dtype=np.float32, count=dims[0] * dims[1] * 3)
	npData = np.array(data)
	npData.shape = (dims[1], dims[0], numChannels)
	npData = npData[::-1]
	pfmFile.close()
	return npData

def writePFMFile(filepath, data):
	f = open(filepath, 'wb')
	f.write('FP')
	f.write('\n')
	f.write(str(data.shape[1]) + ' ' + str(data.shape[0]) + '\n')
	f.write('-1.0\n')
	rev = data[::-1].astype(np.float32)
	rev.tofile(f)
	f.flush()
	f.close()

def loadPPMFile( filepath ):
	im = Image.open( filepath )
	rawdata = np.array(im.getdata())
	rawdata.shape = (im.size[1], im.size[0], 3)
	del im
	return rawdata

def upTo255(x):
	return 255 * min(1.0, x)

def toPPM(pfmData):
	temp = np.all(pfmData>=1, axis=2)
	toChange = np.where( temp==True )
	pfmData[toChange[0], toChange[1], :] = 1.0
	
	return (pfmData * 255.0).astype(np.uint8)

def writePPMFile( filepath, data ):
	img = Image.fromarray(np.uint8(toPPM(data)))
	img.save(filepath)