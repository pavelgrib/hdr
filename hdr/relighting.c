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
    float* sphere = (float*) calloc(sizeof(float), diameter * diameter * NUM_COMP);
    FP_IMG* iblPFMImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    loadPFM(lightingFilename, iblPFMImage);
    initSphere(sphere, diameter);
    
    FP_IMG* reflectanceImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    generateReflectionMap(sphere, diameter);
    reflectanceImage->data = sphere;
    
    if ( DEBUG_IBL_DATA ) printImageData(reflectanceImage->data, 50, 511, 20, 511, 3, 1);
    
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
    
    float* emSphere = (float*) calloc(sizeof(float), diameter * diameter * NUM_COMP);
    initSphere(emSphere, diameter);
    shadeSphere(emSphere, diameter, iblPFMImage);

    float* imgAvg = (float*)calloc(sizeof(float), NUM_COMP);
    float* imgMax = (float*)calloc(sizeof(float), NUM_COMP);
    float* imgMin = (float*)calloc(sizeof(float), NUM_COMP);
//    int len = iblPFMImage->height*iblPFMImage->width*iblPFMImage->numComponents;
//    arrayStats(iblPFMImage->data, len, &imgAvg, &imgMin, &imgMax, &imgMinAt, &imgMaxAt);
    int len = diameter*diameter*3;
    relitImage->data = emSphere;
    arrayStats(emSphere, len, imgAvg, imgMin, imgMax, NUM_COMP);
    printf("%f %f %f\n", imgMin[0], imgMax[0], imgAvg[0]);
    relitImage->width = diameter;
    relitImage->height = diameter;
    relitImage->numComponents = NUM_COMP;
    relitImage->avgVal = (float*) malloc(sizeof(float) * NUM_COMP);
    relitImage->maxVal = (float*) malloc(sizeof(float) * NUM_COMP);
    relitImage->minVal = (float*) malloc(sizeof(float) * NUM_COMP);
    relitImage->avgVal = imgAvg;
    relitImage->maxVal = imgMin;
    relitImage->minVal = imgMax;
    
    cleanupFP(iblPFMImage);
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
    if ( DEBUG_IBL_DATA ) printImageData(sphere, 50, diameter, 20, diameter, 3, 1);
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

    // making it fit on the 0 to 1 range --- not necessary hopefully
//    for ( i = 0; i < diameter; ++i ) {
//        for ( j = 0; j < diameter; ++j ) {
//            for ( k = 0; k < NUM_COMP; ++k ) {
//                x = 2.0*j/((float)(diameter-1)) - 1.0;
//                y = 1.0 - 2.0*i/((float)(diameter-1));
//                if (sqrt(x*x + y*y) <= 1.0 ) {
//                    idx = (i*diameter + j)*NUM_COMP + k;
//                    original[idx] = (original[idx] - minVal[k])/(maxVal[k] - minVal[k]);
//                }
//            }
//        }
//    }

    if ( DEBUG_IBL ) {
//        int outsideRange = 0;
//        for ( i = 0; i < diameter*diameter*NUM_COMP; ++i ) {
//            outsideRange += (original[i]<0);
//        }
//        printf("pixels outside of range: %d\n", outsideRange);
//
//        printImageData(minVal, 1, 1, NUM_COMP, NUM_COMP, 0, 0);
//        printImageData(maxVal, 1, 1, NUM_COMP, NUM_COMP, 0, 0);
    }
}

float linearTransform(const float location, const float fromOld,
                      const float toOld, const float fromNew,
                      const float toNew)
{
    return ((location - fromOld) / (toOld-fromOld)) * (toNew-fromNew) + fromNew;
}

void shadeSphere(float* original, const int diameter, const FP_IMG* inputImage) {
    unsigned int i, j, k, idx, originalIndex, u, w;
    float n[NUM_COMP], v[NUM_COMP], minVal[NUM_COMP], maxVal[NUM_COMP], r[NUM_COMP];
    float theta, phi, x, y, z;
//    printImageData(inputImage->data, 512, 512, 25, 1024, 3, 2);
    
    float imgAvg, imgMax, imgMin;
    int len = 512*1024*3;
    
    arrayStats(inputImage->data, len, &imgAvg, &imgMin, &imgMax, NUM_COMP);
    if ( DEBUG_IBL) printf("avg: %f min: %f max: %f\n\n", imgAvg, imgMin, imgMax);

    // assumption that v = [0, 0, 1]
    v[0] = v[1] = 0.0;
    v[2] = 1.0;
    minVal[0] = minVal[1] = minVal[2] = 1.0;
    maxVal[0] = maxVal[1] = maxVal[2] = -1.0;
    int used = 0;
    for ( i = 0; i < diameter; ++i ) {
        for ( j = 0; j < diameter; ++j ) {
            idx = (i*diameter + j)*NUM_COMP;
            x = 2.0*j/((float)(diameter)) - 1.0;
            y = 1.0 - 2.0*i/((float)(diameter));
            z = sqrt(1.0 - x*x - y*y);
            
            if ( sqrt(x*x + y*y) <= 1.0 ) {
                r[0] = r[1] = r[2] = 0.0;
                n[0] = x;
                n[1] = y;
                n[2] = z;
                float ndotv = 0.0;
                for ( int k = 0; k < NUM_COMP; ++k) ndotv += v[k]*n[k];
                
                for ( int k = 0; k < NUM_COMP; ++k) r[k] = 2.0*ndotv*n[k] - v[k];
                
                theta = acos(r[1]);
                phi = atan2( r[2], r[0]);
                u = theta / PI * inputImage->height;
                w = (0.5 + phi / (2*PI)) * (inputImage->width);
                for ( k = 0; k < NUM_COMP; ++k ) {
                    originalIndex = (int) ((u * inputImage->width + w) * inputImage->numComponents + k);
                    original[idx + k] = inputImage->data[originalIndex];
                    used++;
                    if ( original[idx+k] < minVal[k] ) minVal[k] = original[idx+k];
                    if ( original[idx+k] > maxVal[k] ) maxVal[k] = original[idx+k];
                }
//                if ( DEBUG_IBL ) {
                    if ( i == 120 && j == 60 ) {
                        printf("%f %f %f vs %f %f %f\n\n", r[0], r[1], r[2],
                               original[idx], original[idx+1], original[idx+2]);
//                    }
                }
            }
        }
    }
    
    printf("%f pixels used\n", ((float) used) / ((float) 511*511*3));
//    printImageData(original, 50, 511, 20, 511, 3, 0);
}