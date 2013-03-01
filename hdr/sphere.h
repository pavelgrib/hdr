//
//  sphere.h
//  hdr
//
//  Created by paul on 3/1/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#ifndef hdr_sphere_h
#define hdr_sphere_h

#include "vector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

xyzVector* reflection(const xyzVector spherePoint, const xyzVector view);

float dist( const xyzVector p1, const xyzVector p2);
float dot3( const xyzVector v1, const xyzVector v2);
void add( const xyzVector v1, const xyzVector v2, xyzVector* result);
void sub( const xyzVector v1, const xyzVector v2, xyzVector* result);
void scale(const xyzVector vector, const float scalar, xyzVector* v2);

void shadeSphere(const xyzVector atPoint, const float* withData);

#endif