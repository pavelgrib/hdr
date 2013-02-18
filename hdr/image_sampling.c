//
//  image_sampling.c
//  hdr
//
//  Created by paul on 2/17/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#include "image_sampling.h"


void generateCDFSamples(const FP_IMG* emImage, FP_IMG* sampledImage, const int numSamples) {
    unsigned int height, width, numComponents, i, j, k, idx;
    height = emImage->height;
    width = emImage->width;
    numComponents = emImage->numComponents;
    
    // first get luminance values for image:
    float* luminance = calculateLuminance(emImage);
    
    // next need to create 2D CDF for luminance values
    
    
}

float* calculateLuminance(const FP_IMG* img) {
    unsigned int height = img->height, width = img->width, nComp = img->numComponents;
    unsigned int i, j, k, idx;
    float* luminance = (float*) calloc(sizeof(float), height*width);
    float lumAvg;
    for ( i = 0; i < height; ++i ) {
        for ( j = 0; j < width; ++j ) {
            lumAvg = 0.0;
            for ( k = 0; k < nComp; ++k ) {
                idx = (i*width + j)*nComp + k;
                lumAvg += (img->data[idx] - lumAvg) / (k+1);
            }
        }
        luminance[i*img->width + j] = lumAvg;
    }
    return luminance;
}