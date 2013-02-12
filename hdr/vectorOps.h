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

float euclideanDistance(const float x1, const float y1, const float x2, const float y2);
float manhattanDistance(const float x1, const float y1, const float x2, const float y2);
float dotProduct(const float* arr1, const float* arr2, const int length);
float* scaleVector(float* vector, const int length, const float scalar);
float* addVectors( const float* vector1, const float* vector2, float* sumVector, const int length);

#endif
