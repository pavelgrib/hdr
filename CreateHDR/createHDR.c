//
//  main.c
//  CreateHDR
//
//  Created by paul on 2/18/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#include <stdio.h>
#include "hdr.h"
#include "img.h"

int main(int argc, const char * argv[])
{


    if ( argc !== 2 ) {
        fprintf(stderr, "Wrong numbe of inputs.\n Format of call: ./CreateHDR <path-to-images>")
        break;
        return 1;
    }
    
    FP_IMG* hdr = (FP_IMG*) malloc(sizeof(FP_IMG));
    makeHDRImage(argv[1], hdr);
//    WritePFM(<#const char *filename#>, <#FP_IMG *image#>)
    
    return 0;
    
    
}

