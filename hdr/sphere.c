//
//  sphere.c
//  hdr
//
//  Created by paul on 3/1/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#include "sphere.h"


xyzVector* reflection(const xyzVector spherePoint, const xyzVector view) {
    xyzVector* result = (xyzVector* ) malloc(sizeof(xyzVector));
    float ndotv;
    ndotv = dot3( spherePoint, view);
    ndotv *= 2.0;
    scale( spherePoint, ndotv, result );
    sub( *result, view, result );
    return result;
}

float dist( const xyzVector p1, const xyzVector p2) {
    float sum = pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2) + pow(p1.z-p1.z, 2);
    return sqrt(sum);
}

float dot3( const xyzVector v1, const xyzVector v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void add( const xyzVector v1, const xyzVector v2, xyzVector* result) {
    result->x = v1.x + v2.x;
    result->y = v1.y + v2.y;
    result->z = v1.z + v2.z;
}
void sub( const xyzVector v1, const xyzVector v2, xyzVector* result) {
    result->x = v1.x - v2.x;
    result->y = v1.y - v2.y;
    result->z = v1.z - v2.z;
}
void scale(const xyzVector vector, const float scalar, xyzVector* v2) {
    v2->x = vector.x * scalar;
    v2->y = vector.y * scalar;
    v2->z = vector.z * scalar;
}

void shadeSphere(const FP_IMG* latlong, const int diameter) {
    unsigned int i, j, idx;
    xyzVector spherePt;
    polarVector latlongPt;
    for ( i = 0; i < diameter; ++i ) {
        for ( j = 0; j < diameter; ++j ) {
            
        }
    }
}