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
