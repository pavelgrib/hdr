//
//  relighting.h
//  hdr
//
//  Created by paul on 2/10/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#ifndef hdr_relighting_h
#define hdr_relighting_h

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "img.h"
#include "loadPNM.h"
#include "loaders.h"
#include "hdr.h"
#include "vectorOps.h"

#define NUM_COMP 3
#define DEBUG_IBL false

void initSphere(float* sphere, const int radius);
void generateReflectionMap(float* original, const int diameter);
void shadeSphere(const char* image_in, FP_IMG* image);

void generateRelitSphere(const char* lightingFilename, FP_IMG* relitImage, const int diameter, const char* ibl_dir);

float linearTransform(const float location, const float fromOld, const float toOld, const float fromNew, const float toNew);

#endif
