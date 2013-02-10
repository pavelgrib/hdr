#ifndef HDR_H
#define HDR_H

#include "loadPNM.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <strings.h>
#include <stdbool.h>
#include "img.h"
#include "loaders.h"
#include <math.h>

#define PI 3.14159265358979323
#define HISTOGRAM_SIZE 1000
#define MAX_HDR_PIXEL_VALUE 1.0
#define MAX_PPM_PIXEL_VALUE 255
#define TM_MIN 3.0

#define LOWER_FP_TOLERANCE 0.005
#define UPPER_FP_TOLERANCE 0.92


void makeHDRImage(const char* imagesDirectory, FP_IMG* hdrImage);
void toneMap(FP_IMG* inputImage, FP_IMG* outputImage);
void brightenPFM(FP_IMG* inputImage, FP_IMG* outputImage, const float scale);

float w(const float z);
float hdrValue(FP_IMG* images[], const int numImages, const int dataIndex);

bool isPFM(const struct dirent* pent);
int countPFMImages(const char* imagesDirectory);
void concatenate(const char* str1, const char* str2, char* buffer);

void printHistogram(void* histogram, unsigned int size);

#endif