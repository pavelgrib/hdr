//
//  Pixel.h
//  hdr
//
//  Created by Paul Gribelyuk on 2/20/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#ifndef hdr_Pixel_h
#define hdr_Pixel_h

typedef struct {
	unsigned int row;
    unsigned int col;
    char value;
} Pixel;

typedef struct {
	unsigned int row;
    unsigned int col;
    float value;
} PixelFP;

#endif
