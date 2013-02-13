//
//  loaders.h
//  hdr
//
//  Created by paul on 2/9/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#ifndef hdr_loaders_h
#define hdr_loaders_h

#include "loadPNM.h"

void CreateAndSavePFM(const char* image_out);
void LoadAndSavePPM(const char* image_in, const char* image_out);
void LoadAndSavePFM(const char *image_in, const char *image_out);
void LoadPPMAndSavePFM(const char* image_in, const char* image_out);
void LoadPFMAndSavePPM(const char* image_in, const char* image_out);
void LoadPFMAndSaveClampedPPM(const char* image_in, const char* image_out, const float upperClamp);
void analyzePPM(const char* imageToAnalyze);

void cleanupFP(FP_IMG* fpImage);
void cleanup(IMG* image);

#endif
