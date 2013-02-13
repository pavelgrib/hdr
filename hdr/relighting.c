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
    float n[NUM_COMP], v[NUM_COMP], minVal[NUM_COMP], maxVal[NUM_COMP], r[NUM_COMP];
    float theta, phi, x, y, z;
//    printImageData(original, 127, height, 20, width, 3, 1);
    // assumption that v = [0, 0, 1]
    v[0] = v[1] = 0.0;
    v[2] = 1.0;
    minVal[0] = minVal[1] = minVal[2] = 1.0;
    maxVal[0] = maxVal[1] = maxVal[2] = -1.0;
    for ( i = 0; i < diameter; ++i ) {
        for ( j = 0; j < diameter; ++j ) {
            idx = (i*diameter + j)*NUM_COMP;
            x = 2.0*j/((float)(diameter-1)) - 1.0;
            y = 1.0 - 2.0*i/((float)(diameter-1));
            z = sqrt(1.0 - x*x - y*y);
            if ( sqrt(x*x + y*y) <= 1.0 ) {
                r[0] = r[1] = r[2] = 0.0;
                theta = acos(y) / PI;
                phi = (atan2( z, -x) + PI) / (2*PI);
                n[0] = x;//sin(theta) * sin(phi);
                n[1] = y;//cos(theta);
                n[2] = z;//sin(theta) * cos(phi);
                float ndotv = 0.0;
                for ( int k = 0; k < NUM_COMP; ++k) ndotv += v[k]*n[k];

                for ( int k = 0; k < NUM_COMP; ++k) r[k] = 2.0*ndotv*n[k] - v[k];
            
                for ( k = 0; k < NUM_COMP; ++k ) {
                    original[idx + k] = r[k];
                    if ( r[k] < minVal[k] ) minVal[k] = r[k];
                    if ( r[k] > maxVal[k] ) maxVal[k] = r[k];
                }
                if ( DEBUG_IBL ) {
                    if ( i == 255 && j == 255 ) {
                        printf("%f %f %f vs %f %f %f\n", n[0], n[1], n[2], r[0], r[1], r[2]);
                    }
                }
            }
        }
    }

    // making it fit on the 0 to 1 range
    for ( i = 0; i < diameter; ++i ) {
        for ( j = 0; j < diameter; ++j ) {
            for ( k = 0; k < NUM_COMP; ++k ) {
                x = 2.0*j/((float)(diameter-1)) - 1.0;
                y = 1.0 - 2.0*i/((float)(diameter-1));
                if (sqrt(x*x + y*y) <= 1.0 ) {
                    idx = (i*diameter + j)*NUM_COMP + k;
                    original[idx] = (original[idx] - minVal[k])/(maxVal[k] - minVal[k]);
                }
            }
        }
    }

    if ( DEBUG_IBL ) {
        int outsideRange = 0;
        for ( i = 0; i < diameter*diameter*NUM_COMP; ++i ) {
            outsideRange += (original[i]<0);
        }
        printf("pixels outside of range: %d\n", outsideRange);

        printImageData(minVal, 1, 1, NUM_COMP, NUM_COMP, 0, 0);
        printImageData(maxVal, 1, 1, NUM_COMP, NUM_COMP, 0, 0);
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