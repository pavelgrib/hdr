//
//  vectorOps.h
//  hdr
//
//  Created by paul on 2/11/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#ifndef vector_ops_h
#define vector_ops_h

#include <math.h>
#include <stdio.h>
#include "img.h"

float euclideanDistance(const float x1, const float y1, const float x2, const float y2);
float manhattanDistance(const float x1, const float y1, const float x2, const float y2);
float dotProduct(const float* arr1, const float* arr2, const int length);
float* scaleVector(float* vector, const int length, const float scalar);
void addVectors( const float* vector1, const float* vector2, float* sumVector, const int length);

void printImageData(const void* data, const int height, const int maxHeight, const int width,
                    const int maxWidth, const int numChannels, const int channel);
void printImage(const FP_IMG* image, const int maxHeight, const int maxWidth, const int channel);

#endif
