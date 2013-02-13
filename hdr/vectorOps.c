//
//  vectorOps.c
//  hdr
//
//  Created by paul on 2/11/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#include "vectorOps.h"

float euclideanDistance(const float x1, const float y1, const float x2, const float y2) {
    return sqrtf((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

float manhattanDistance(const float x1, const float y1, const float x2, const float y2) {
    return fabs(x1 - x2 ) + fabs( y1 - y2 );
}

float dotProduct(const float* arr1, const float* arr2, const int length) {
    unsigned int i;
    float sum = 0.0;
    for ( i = 0; i < length; ++i ) sum += arr1[i]*arr2[i];
    return sum;
}

float* scaleVector(float* vector, const int length, const float scalar) {
    unsigned int i;
    for ( i = 0; i < length; ++i ) vector[i] *= scalar;
    return vector;
}

void addVectors( const float* vector1, const float* vector2, float* sumVector, const int length) {
    unsigned int i;
    for ( i = 0; i < length; ++ i ) sumVector[i] = vector1[i] + vector2[i];
}

void printImageData(const void* data, const int height, const int maxHeight, const int width,
                    const int maxWidth, const int numChannels, const int channel)
{
    unsigned int i, j, idx;
    printf("------------ printing channel %d of array of size %d by %d -----------\n",
           channel, maxHeight, maxWidth);
    for ( i = 0; i < height; ++i) {
        for ( j = 0; j < width; ++j) {
            idx = (i*maxWidth + j)*numChannels + channel;
            printf("%0.02f ", ((float*)data)[idx]);
        }
        printf("\n");
    }
    printf("------------------------ end printing -----------------------------\n");
}

void printImage(const FP_IMG* image, const int maxHeight, const int maxWidth, const int channel) {
    printImageData(image->data, image->height, maxHeight,
                   image->width, maxWidth, image->numComponents, channel);
}

//float arrayMax(const float* array, const int length) {
//    float maxVal = array[0];
//    int i;
//    for ( i = 0; i < length; ++i )
//        maxVal = (array[i] > maxVal ? array[i] : ;);
//    
//    
//}

float arrayAverage(const float* array, const int length) {
    float avg = 0.0, count = 0.0;
    int i;
    for ( i = 0; i < length; ++i ) {
        avg += (array[i] - avg) / (++count);
    }
    return avg;
}

void arrayStats(const float* array, const int length, float* avg, float* min, float* max, int numChannels) {
    float count[numChannels];
    float localAvg[numChannels];
    float localMin[numChannels];
    float localMax[numChannels];
    int i = 0, k = 0, idx;
    
    for ( k = 0; k < numChannels; ++k ) {
        localAvg[k] = 0;
        count[k] = 0.0;
        localMin[k] = localMax[k] = array[0];
    }
    
    for ( k = 0; i < length/4; ++i ) {
        for ( ; k < 4; ++k ) {
            idx = i*numChannels + k;
            localAvg[k] += ( array[idx] - localAvg[k]) / (++count[k]);
            localMin[k] = (array[idx] < localMin[k] ? array[idx] : localMin[k]);
            localMax[k] = (array[idx] > localMax[k] ? array[idx] : localMax[k]);
        }
    }
    
    for ( k = 0; k < numChannels; ++k ){
        max[k] = localMax[k];
        min[k] = localMin[k];
        avg[k] = localAvg[k];
    }
}
