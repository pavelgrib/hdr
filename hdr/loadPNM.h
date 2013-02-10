#ifndef __LOADPNM_H__
#define __LOADPNM_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "img.h"

unsigned char* loadPNM( const char* filename, IMG* image );

void WritePNM( const char* filename, IMG* image );

float* loadPFM( const char* filename, FP_IMG* image );

void WritePFM( const char* filename, FP_IMG* image );

#endif