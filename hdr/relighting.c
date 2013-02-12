//
//  relighting.c
//  hdr
//
//  Created by paul on 2/10/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#include "relighting.h"

void generateRelitSphere(const char* lightingFilename, FP_IMG* relitImage,
                         const int diameter, const char* ibl_dir)
{
    unsigned int i;
    float* sphere = (float*) calloc(sizeof(float), diameter * diameter * NUM_COMP);
    FP_IMG* iblPFMImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    loadPFM(lightingFilename, iblPFMImage);
    initSphere(sphere, diameter);
    
    FP_IMG* reflectanceImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    generateReflectionMap(sphere, diameter);
    reflectanceImage->data = sphere;

    if ( DEBUG_IBL ) printImageData(reflectanceImage->data, 511, 511, 20, 511, 3, 1);
    
    reflectanceImage->height = diameter;
    reflectanceImage->width = diameter;
    reflectanceImage->numComponents = NUM_COMP;
    char reflectanceFilepathPFM[1024];
    char reflectanceFilepathPPM[1024];
    concatenate(ibl_dir, "reflectance.pfm", reflectanceFilepathPFM);
    concatenate(ibl_dir, "reflectance.ppm", reflectanceFilepathPPM);
    WritePFM(reflectanceFilepathPFM, reflectanceImage);
    LoadPFMAndSavePPM(reflectanceFilepathPFM, reflectanceFilepathPPM);

    // doing the re-lighting here
    relitImage->data = sphere;
    relitImage->width = diameter;
    relitImage->height = diameter;
    relitImage->numComponents = NUM_COMP;
    for ( i = 0; i < NUM_COMP; ++i ) {
        iblPFMImage->avgVal[i] = PI*diameter*diameter/4.0;
        iblPFMImage->maxVal[i] = 1.0;
        iblPFMImage->minVal[i] = 0;
    }
}

void initSphere(float* sphere, const int diameter) {
    unsigned int i, j, k, midx = diameter/2, midy = diameter/2;
    for ( i = 0; i < diameter; ++i ) {
        for ( j = 0; j < diameter; ++j ) {
            for ( k = 0; k < NUM_COMP; ++k ) {
                if ( euclideanDistance(midx, midy, i, j) <= diameter/2 )
                    sphere[(i*diameter+j)*NUM_COMP + k] = MAX_HDR_PIXEL_VALUE;
            }
        }
    }
    if ( DEBUG_IBL ) printImageData(sphere, diameter, diameter, 20, diameter, 3, 1);
}

void generateReflectionMap(float* original, const int diameter) {
    unsigned int i, j, k, idx;
    float n[NUM_COMP], v[NUM_COMP], r[NUM_COMP];
    float theta, phi; //, x, y, z;
//    printImageData(original, 127, height, 20, width, 3, 1);
    // assumption that v = [0, 0, 1]
    v[0] = v[1] = 0;
    v[2] = 1;
    for ( i = 0; i < diameter; ++i ) {
        for ( j = 0; j < diameter; ++j ) {
            // need to calculate n as a function of i and j here
            idx = (i*diameter + j)*NUM_COMP;
            if ( original[idx]!= 0 || original[idx+1]!=0 || original[idx+2]!=0 ) {
                phi = linearTransform(j, 0, diameter, 0, PI);
                theta = linearTransform(i, 0, diameter, -PI, PI);
                n[0] = sin(theta) * sin(phi);
                n[1] = cos(phi);
                n[2] = sin(phi) * cos(theta);
//                printImageData(n, 1, 3, 1, 1);
                addVectors( scaleVector(n, NUM_COMP, 2*dotProduct(n, v, NUM_COMP)),
                    scaleVector(v, NUM_COMP, -1.0), r, NUM_COMP );
            
                for ( k = 0; k < NUM_COMP; ++k )
                    original[(i*diameter + j)*NUM_COMP + k] = r[k];
            }
        }
    }
}

float linearTransform(const float location, const float fromOld,
                      const float toOld, const float fromNew,
                      const float toNew)
{
    return ((location - fromOld) / (toOld-fromOld)) * (toNew-fromNew) + fromNew;
}

void shadeSphere(const char* image_in, FP_IMG* image) {
    
}

