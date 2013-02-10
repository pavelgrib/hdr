#ifndef IMG_H
#define IMG_H

typedef struct {
	unsigned char* data;
	unsigned int height;
	unsigned int width;
	unsigned int numComponents;
	float* maxVal;
	float* minVal;
	float* avgVal;
} IMG;

typedef struct {
	float* data;
	unsigned int height;
	unsigned int width;
	unsigned int numComponents;
	unsigned int relativeExposure;
	float* maxVal;
	float* minVal;
	float* avgVal;
} FP_IMG;

#endif