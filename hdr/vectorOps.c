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

float* addVectors( const float* vector1, const float* vector2, float* sumVector, const int length) {
    unsigned int i;
    for ( i = 0; i < length; ++ i ) sumVector[i] = vector1[i] + vector2[i];
    return sumVector;
}