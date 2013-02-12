#include "hdr.h"


void makeHDRImage(const char* imagesDirectory, FP_IMG* hdrImage) {
	DIR* pdir = NULL;
	pdir = opendir( imagesDirectory );
	struct dirent* pent = NULL;
	int height, width, numComponents;
	int i, j, k;
	int dataIndex;
	float* hdrData;
    
	// allocating for the 7 images as an array of pointers to FP_IMG struct
	int numImages = countPFMImages(imagesDirectory);
	FP_IMG* fpImages[numImages];
	for ( i = 0; i < numImages; ++i ) {
		fpImages[i] = (FP_IMG*) malloc(sizeof(FP_IMG));
	}
	
	// reading the first file to determine height, width, numComponents
	while ( !isPFM(pent = readdir(pdir)) ) ;
	char fullFilePath[1024];
	concatenate( imagesDirectory, pent->d_name, fullFilePath );

	loadPFM(fullFilePath, fpImages[0]);
	fpImages[0]->relativeExposure = 1.0;
	height = fpImages[0]->height;
	width = fpImages[0]->width;
	numComponents = fpImages[0]->numComponents;
    
    // allocate on heap for future use
    float* avgPix = malloc(sizeof(float)*numComponents);
    float* minPix = malloc(sizeof(float)*numComponents);
    float* maxPix = malloc(sizeof(float)*numComponents);
    // allocate on stack since we don't care about storing counts
    float count[numComponents];
    
    for ( k = 0; k < numComponents; ++k ) avgPix[k]=count[k]=maxPix[k]=minPix[k]=0.0;
    
	hdrData = (float*) malloc(sizeof(float) * height * width * numComponents);

	int imageIndex = 1;
	while( (pent = readdir(pdir)) ) {
		if ( isPFM(pent) ) {
			concatenate( imagesDirectory, pent->d_name, fullFilePath );
			fpImages[imageIndex] = (FP_IMG*) malloc(sizeof(FP_IMG));
			loadPFM(fullFilePath, fpImages[imageIndex]);
			if ( fpImages[imageIndex]->height != height || 
				fpImages[imageIndex]->width != width || 
				fpImages[imageIndex]->numComponents != numComponents ) {
				printf("incorrect dimensions between image %d and the others (%d by %d vs %d by %d expected", 
					imageIndex, fpImages[imageIndex]->height, fpImages[imageIndex]->width, height, width);
				return;
			}
			fpImages[imageIndex]->relativeExposure = pow(4.0, (float)(imageIndex));
			imageIndex++;
		}
	}
	// at this point all the images are stored in the array of structs fpImages
	float hdrVal;
	for ( i = 0; i < height; ++i ) {
		for ( j = 0; j < width; ++j ) {
			for ( k = 0; k < numComponents; ++k ) {
				dataIndex = ( i * width + j) * numComponents + k;
				hdrVal = hdrValue(fpImages, numImages, dataIndex);
				avgPix[k] += ( hdrVal - avgPix[k] ) / (++count[k]);
				if ( maxPix[k] < hdrVal ) maxPix[k] = hdrVal;
				if ( minPix[k] > hdrVal ) minPix[k] = hdrVal;
				hdrData[dataIndex] = hdrVal;
			}
		}
	}
	hdrImage->height = height;
	hdrImage->width = width;
	hdrImage->numComponents = numComponents;
	hdrImage->data = hdrData;
	hdrImage->maxVal = maxPix;
	hdrImage->minVal = minPix;
	hdrImage->avgVal = avgPix;
    
    // since I malloc'd them here, I'll delete them here too
	for ( i = 0; i < numImages; ++i ) {
        cleanupFP(fpImages[i]);
	}
}

void toneMap(const FP_IMG* inputImage, FP_IMG* outputImage) {
	unsigned int i, j, k, idx, height, width, numComponents;
	height = inputImage->height;
	width = inputImage->width;
	numComponents = inputImage->numComponents;
    float stretch[numComponents];
	float* outputData = (float*) malloc(sizeof(float*) * height * width * numComponents);
    float* avg = (float*) malloc(sizeof(float) * numComponents);
    float count[numComponents];
    float* minTMPixel = (float*) malloc(sizeof(float) * numComponents);
    float* maxTMPixel = (float*) malloc(sizeof(float) * numComponents);

    for ( k = 0; k < numComponents; ++k ) {
        minTMPixel[k] = 0.0;
        maxTMPixel[k] = 1.0;
        avg[k] = 0.0;
        count[k] = 0;
        stretch[k] = 1.0 / (inputImage->maxVal[k] - inputImage->minVal[k]);
//        printf("%d stretch is %f\n", k, stretch[k]);
    }
    
    for ( i = 0; i < height; ++i ) {
        for ( j = 0; j < width; ++j ) {
            for ( k = 0; k < numComponents; ++k ) {
                idx = (i * width + j)*numComponents + k;
                outputData[idx] = fmin(inputImage->data[idx] * stretch[k], 1.0);
                avg[k] += (outputData[idx] - avg[k]) / (++count[k]);
			}
		}
    }

	outputImage->data = outputData;
	outputImage->width = width;
	outputImage->height = height;
	outputImage->numComponents = numComponents;
	outputImage->minVal = minTMPixel;
	outputImage->maxVal = maxTMPixel;
    outputImage->avgVal = avg;
}

void brightenPFM(const FP_IMG* inputImage, FP_IMG* outputImage, const float scale) {
    if ( scale <= 0 ) return;
    unsigned int i, j, k, idx, height, width, numComponents;
    height = inputImage->height;
    width = inputImage->width;
    numComponents = inputImage->numComponents;
    
    float count[numComponents];
    float* outData = (float*) malloc(sizeof(float) * height* width * numComponents);
    float* maxVal = (float*) malloc(sizeof(float) * numComponents);
    float* minVal = (float*) malloc(sizeof(float) * numComponents);
    float* avgVal = (float*) malloc(sizeof(float) * numComponents);
    
    for ( k = 0; k < numComponents; ++k ) avgVal[k]=count[k]=maxVal[k]=minVal[k]=0.0;
    
    for ( i = 0; i < height; ++i ) {
        for ( j = 0; j < width; ++j ) {
            for ( k = 0; k < numComponents; ++k ) {
                idx = (i*width + j)*numComponents + k;
                outData[idx] = fmin(inputImage->data[idx]*scale, 1.0);
            }
        }
    }
    
    outputImage->data = outData;
    outputImage->minVal = minVal;
    outputImage->maxVal = maxVal;
    outputImage->avgVal = avgVal;
    outputImage->height = height;
    outputImage->width = width;
    outputImage->numComponents = numComponents;
}

void applyGamma(FP_IMG* image, const float gamma) {
    int height, width, numComponents, i, j, k, idx;
    height = image->height;
    width = image->width;
    numComponents = image->numComponents;
    float* gammaData = (float*) malloc(sizeof(float) * width * height * numComponents);
    int count[numComponents];
    for ( k = 0; k < numComponents; ++k ) {
        count[k] = 0;
        image->avgVal[k] = 0;
        image->minVal[k] = pow(image->minVal[k], gamma);
        image->maxVal[k] = pow(image->maxVal[k], gamma);
    }
    
    for ( i = 0; i < height; ++i ) {
        for ( j = 0; j < width; ++j ) {
            for ( k = 0; k < numComponents; ++k ) {
                idx = (i*width + j)*numComponents + k;
                image->data[idx] = pow(image->data[idx], 1.0/gamma);
                image->avgVal[k] += ( gammaData[idx] - image->avgVal[k]) / (++count[k]);
            }
        }
    }
    
}

bool isPFM(const struct dirent* pent) {
	char* pfm = "pfm";
	if ( (pent)->d_namlen > 3 ) {
		return (strncasecmp( pfm, &pent->d_name[pent->d_namlen-3], 3) == 0);
	} else {
		return false;
	}
}

int countPFMImages(const char* imagesDirectory) {
	DIR* pdir = NULL;
	pdir = opendir( imagesDirectory );
	struct dirent* pent = NULL;
	int count = 0;
	while ( (pent = readdir( pdir )) ) if (isPFM(pent)) count++;
	return count;
}

float w(const float z) {
	return sin(PI * z * 2.0 - PI/2.0)/2.0 + 0.5;
	// if ( z < 0.5 ) return z;
	// else return 1 - z;
}

float hdrValue(FP_IMG* images[], const int numImages, const int dataIndex) {
	float val, relExp, sum = 0.0, sumWeights = 0.0;
    int i;

	for ( i = 0; i < numImages; ++i ) {
        val = fmax(fmin(images[i]->data[dataIndex], UPPER_FP_TOLERANCE), LOWER_FP_TOLERANCE);
            sumWeights += w(val);
	}
	
	for ( i = 0; i < numImages; ++i ) {
        val = fmax(fmin(images[i]->data[dataIndex], UPPER_FP_TOLERANCE), LOWER_FP_TOLERANCE);
//        val = images[i]->data[dataIndex];
//		val = fmin(val, UPPER_FP_TOLERANCE);
//        val = fmax(val, LOWER_FP_TOLERANCE);
        relExp = (float) images[i]->relativeExposure;
        sum += w(val) * log2( val/relExp );
	}

//    if ( dataIndex > 110 && dataIndex < 120 ) printf("%f\n", exp(sum/sumWeights));
	return exp(sum/sumWeights);
}

void concatenate(const char* str1, const char* str2, char* buffer) {
	strcpy(buffer, str1);
	strcat(buffer, str2);
}

void printHistogram(void* histogram, unsigned int size) {
	unsigned int i;
	for ( i = 0; i < size; ++i ) printf("%d\n", ((int*)histogram)[i]);
}