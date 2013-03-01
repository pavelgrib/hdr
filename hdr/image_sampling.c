//
//  image_sampling.c
//  hdr
//
//  Created by paul on 2/17/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#include "image_sampling.h"


void generateCDFSamples(const FP_IMG* emImage, FP_IMG* sampledImage, const int numSamples) {
    unsigned int height, width, numComponents, i, j, k;
    height = emImage->height;
    width = emImage->width;
    numComponents = emImage->numComponents;
    // make sampledImage same as the environment map to begin with
    sampledImage->data = (float*) calloc(sizeof(float), height*width*numComponents);
    sampledImage->minVal = (float*) malloc(sizeof(float) * numComponents);
    sampledImage->maxVal = (float*) malloc(sizeof(float) * numComponents);
    sampledImage->avgVal = (float*) malloc(sizeof(float) * numComponents);
    memcpy(sampledImage->data, emImage->data, sizeof(float)*height*width*numComponents);
    memcpy(sampledImage->avgVal, emImage->avgVal, sizeof(float)*numComponents);
    memcpy(sampledImage->minVal, emImage->minVal, sizeof(float)*numComponents);
    memcpy(sampledImage->maxVal, emImage->maxVal, sizeof(float)*numComponents);
    sampledImage->height = height;
    sampledImage->width = width;
    sampledImage->numComponents = numComponents;
    
    // first get luminance values for image:
    float* luminance = calculateLuminance(emImage);
    // scale luminance by "solid angle term" sin(theta)
    float theta;
    for ( i = 0; i < height; ++i ) {
        theta = (height - 1 - i) * PI / height;
        for ( j = 0; j < width; ++j ) {
            luminance[i*width + j] *= sin(theta);
        }
    }

    // treat the luminance as f(x) --> first average each row:
    float* rowAvg = (float*) calloc(sizeof(float), height);
    for ( i = 0; i < height; ++i ) {
        for ( j = 0; j < width; ++j ) {
            rowAvg[i] += (luminance[i*width + j] - rowAvg[i])/(j+1);
        }
        
    }
    // now sample numSamples values and color the picture in 5x5 blocks in green
    int x, y;
    for ( k = 0; k < numSamples; ++k ) {
        y = sample( rowAvg, height );
        x = sample( &(emImage->data[y * width * numComponents]), width );
        
        // now color the image with a 5x5 grid
        int lowerX = (int)fmax(0, x-2), upperX = (int)fmin(x+2, width);
        int lowerY = (int)fmax(0, y-2), upperY = (int)fmin(y+2, height);
        printf("%d, %d\n", x, y);
        for ( i = lowerY; i < upperY; ++i ) {
            for ( j = lowerX; j < upperX; ++j ) {
                sampledImage->data[(i*width + j)*numComponents] = 0;
                sampledImage->data[(i*width + j)*numComponents + 1] = 1.0;
                sampledImage->data[(i*width + j)*numComponents + 2] = 0;
            }
        }
    }
    
    free(luminance);
    free(rowAvg);
}

float* calculateLuminance(const FP_IMG* img) {
    unsigned int height = img->height, width = img->width, nComp = img->numComponents;
    unsigned int i, j, k;
    float* luminance = (float*) calloc(sizeof(float), height*width);
    float lumAvg;
    for ( i = 0; i < height; ++i ) {
        for ( j = 0; j < width; ++j ) {
            lumAvg = 0.0;
            for ( k = 0; k < nComp; ++k ) {
                lumAvg += (img->data[(i*width + j)*nComp + k] - lumAvg) / (k+1);
            }
            luminance[i*width + j] = lumAvg;
        }
    }
    return luminance;
}

void generatePhongSamples(const FP_IMG* emImage, FP_IMG* sampledImage, const int numSamples) {
//    unsigned int height = emImage->height, width = emImage->width, nComp = emImage->numComponents;
//    unsigned int i, j, k;
}

int sample(float* data, unsigned int length) {
    float avg = 0.0;
    float* cdf = (float*) calloc(sizeof(float), length+1);
    int result, i = 1;
    float u = (float)rand() / ((float) RAND_MAX);
    
    // calculate cumulative density function and the average pixel value
    for ( ; i <= length; ++i ) {
        avg += (data[i-1] - avg) / i;
        cdf[i] = cdf[i-1] + data[i-1]/length;
    }
    
    // normalize cdf by the average
    for ( i = 1; i <= length; ++i ) {
        cdf[i] /= avg;
    }
    
    result = index_lower(cdf, 0, length, u);
    free(cdf);
    return result;
}

// assumes data is sorted as in the case of a CDF
int index_lower(float* data, unsigned int startOffset, unsigned int length, float item) {
    if ( data[startOffset + length/2] > item && length > 1 ) {
        return index_lower(data, startOffset, length - length/2, item);
    } else if ( data[startOffset + length/2] < item && length > 1) {
        return index_lower(data, startOffset + length/2, length - length/2, item);
    } else if ( length == 1 ) {
        return startOffset;
    } else if (data[startOffset + length/2] == item) {
        int idx = startOffset + length/2;
        while ( data[idx] == item ) {
            idx--;
        }
        return idx + 1;
    }
    
    return -1;
}



