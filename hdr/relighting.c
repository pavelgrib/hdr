//
//  relighting.c
//  hdr
//
//  Created by paul on 2/10/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#include "relighting.h"

void generateRelitSphere(const char* lightingFilename, FP_IMG* relitImage, const int diameter, const char* ibl_dir) {
    unsigned int i, radius = diameter/2;
    FP_IMG* iblPFMImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    loadPFM(lightingFilename, iblPFMImage);
    printf("%d, %d\n", iblPFMImage->height, iblPFMImage->width);
    float* sphere = initSphere(radius);
    relitImage->data = sphere;
    relitImage->width = 2*radius+1;
    relitImage->height = 2*radius+1;
    relitImage->numComponents = NUM_COMP;
    for ( i = 0; i < NUM_COMP; ++i ) {
        iblPFMImage->avgVal[i] = PI*radius*radius;
        iblPFMImage->maxVal[i] = 1.0;
        iblPFMImage->minVal[i] = 0;
    }

    FP_IMG* reflectanceImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    reflectanceImage->data = generateReflectionMap(sphere, relitImage->height, relitImage->width);
    reflectanceImage->height = relitImage->height;
    reflectanceImage->width = relitImage->width;
    reflectanceImage->numComponents = NUM_COMP;

    char reflectanceFilepathPFM[1024];
    char reflectanceFilepathPPM[1024];
    concatenate(ibl_dir, "reflectance.pfm", reflectanceFilepathPFM);
    concatenate(ibl_dir, "reflectance.ppm", reflectanceFilepathPPM);
    WritePFM(reflectanceFilepathPFM, reflectanceImage);
    LoadPFMAndSavePPM(reflectanceFilepathPFM, reflectanceFilepathPPM);
}

float* initSphere(const int diameter) {
    unsigned int i, j, k, midx = diameter/2, midy = diameter/2;
    float* sphere = (float*) malloc(sizeof(float) * diameter * diameter * NUM_COMP);
    
    for ( i = 0; i < diameter; ++i ) {
        for ( j = 0; j < diameter; ++j ) {
            for ( k = 0; k < NUM_COMP; ++k ) {
//                    printf("distance between (%d, %d) and (%d, %d) is: %f\n",
//                           midx, midy, i, j, distance(midx, midy, i, j));
                if ( euclideanDistance(midx, midy, i, j) > diameter/2 )
                    sphere[(i*diameter+j)*NUM_COMP + k] = MIN_HDR_PIXEL_VALUE;
                else {
                    sphere[(i*diameter+j)*NUM_COMP + k] = MAX_HDR_PIXEL_VALUE;
                }
            }
        }
    }
    return sphere;
}

float* generateReflectionMap(const float* original, const int height, const int width) {
    unsigned int i, j, k;
    float* reflectionMap = (float*) malloc(sizeof(float) * height * width * NUM_COMP);
    float n[NUM_COMP], v[NUM_COMP];
    float* r;
    float theta, phi; //, x, y, z;
    
    // assumption that v = [0, 0, 1]
    v[0] = v[1] = 0;
    v[2] = 1;
    for ( i = 0; i < height; ++i ) {
        theta = linearTransform(i, 0, width, 0, 2*PI);
        for ( j = 0; j < width; ++j ) {
            // need to calculate n as a function of i and j here
            phi = linearTransform(j, 0, height, 0, PI);

            n[0] = sinf(theta) * sinf(phi);
            n[1] = cosf(phi);
            n[2] = sinf(phi) * cosf(theta);

            r = addVectors( scaleVector(n, NUM_COMP, 2*dotProduct(n, v, NUM_COMP)), 
                scaleVector(v, NUM_COMP, -1.0), r, NUM_COMP );
            
            for ( k = 0; k < NUM_COMP; ++k )
                reflectionMap[(i*height + j)*NUM_COMP + k] = r[k];
        }
    }
    return reflectionMap;    
}

float linearTransform(const float location, const float fromOld, const float toOld, const float fromNew, const float toNew) {
    return ((location - fromOld) / (toOld-fromOld)) * (toNew-fromNew) + fromNew;
}

void shadeSphere(const char* image_in, FP_IMG* image) {
    
}

