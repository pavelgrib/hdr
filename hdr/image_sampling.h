//
//  image_sampling.h
//  hdr
//
//  Created by paul on 2/17/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#ifndef hdr_image_sampling_h
#define hdr_image_sampling_h

#include "img.h"
#include "loadPNM.h"

// generate numSamples samples according to 2D CDF of emImage
// create 5x5 green (0,1,0) neighborhood around sampled points
void generateCDFSamples(const FP_IMG* emImage, FP_IMG* sampledImage, const int numSamples);

// generate numSamples samples according to Phong BRDF model of emImage
// create 5x5 green (0,1,0) neighborhood around sampled points saved in sampledImage
void generatePhongSamples(const FP_IMG* emImage, FP_IMG* sampledImage, const int numSamples);

// preprocessing image to get average of RGB channels at each pixel
// allocates memory for sizeof(float) * height * width on heap; returns the pointer
float* calculateLuminance(const FP_IMG* img);

float* calculate2DCDF(const float* img,
                      const int height,
                      const int width,
                      const int nBuckets);

#endif
