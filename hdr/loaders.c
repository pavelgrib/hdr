//
//  loaders.c
//  hdr
//
//  Created by paul on 2/9/13.
//  Copyright (c) 2013 paul. All rights reserved.
//

#include "loaders.h"

void CreateAndSavePFM(const char* image_out) {
	FP_IMG* image = (FP_IMG*) malloc(sizeof(FP_IMG));
	image->width = 511; // set size of image to 511x511 pixels
	image->height = 511;
	image->numComponents = 3;
	
	image->data = (float*) malloc(sizeof( float ) * image->width
                                  * image->height * image->numComponents );
	unsigned int i, j, k;
	for ( i = 0 ; i < image->height ; ++i ) {
		for ( j = 0 ; j < image->width ; ++j ) {
			for ( k = 0 ; k < image->numComponents ; ++k ) {
				unsigned int index = i * image->width * image->numComponents
                + j * image->numComponents + k; //index within the image
				//set image to white
				image->data[index] = 1.0f; // R
				image->data[index + 1] = 1.0f; // G
				image->data[index + 2] = 1.0f; // B
			}
		}
	}
	WritePFM(image_out, image);
}

void LoadAndSavePPM(const char* image_in, const char* image_out) {
	IMG* image = (IMG*) malloc(sizeof(IMG));
	unsigned int i, j, k, index;
	unsigned char* img_in = loadPNM(image_in, image);
	image->data = (unsigned char*) malloc( sizeof(unsigned char) * image->width
                                          * image->height * image->numComponents );
    
	for ( i = 0 ; i < image->height ; ++i ) {
		for ( j = 0 ; j < image->width ; ++j ) {
			for ( k = 0 ; k < image->numComponents ; ++k ) {
				index = i * image->width * image->numComponents
                + j * image->numComponents + k; //index within the image
                
				image->data[index] = img_in[index]; // R
				image->data[index + 1] = img_in[index + 1]; // G
				image->data[index + 2] = img_in[index + 2]; // B
			}
		}
	}
	WritePNM(image_out, image);
}

void LoadAndSavePFM(const char *image_in, const char *image_out) {
	FP_IMG* image = (FP_IMG*) malloc(sizeof(FP_IMG));
	unsigned int i, j, k, index;
	float* img_in = loadPFM(image_in, image);
	image->data = (float*) malloc(sizeof(float) * image->width * image->height
                                  * image->numComponents);
    
	for ( i = 0 ; i < image->height ; ++i ) {
		for ( j = 0 ; j < image->width ; ++j ) {
			for ( k = 0 ; k < image->numComponents ; ++k ) {
				index = i * image->width * image->numComponents
                + j * image->numComponents + k; /* index within the image */
				image->data[index] = img_in[index];  // R
				image->data[index + 1] = img_in[index + 1]; // G
				image->data[index + 2] = img_in[index + 2]; // B
			}
		}
	}
	WritePFM(image_out, image);
}

void LoadPPMAndSavePFM(const char* image_in, const char* image_out) {
	IMG* ppmImage = NULL;
	FP_IMG* pfmImage = (FP_IMG*) malloc(sizeof(FP_IMG));
	unsigned int i, j, k, index;
    loadPNM(image_in, ppmImage);
	pfmImage->data = (float*) malloc( sizeof(float) * pfmImage->width
        * ppmImage->height * ppmImage->numComponents );
    
	for ( i = 0 ; i < ppmImage->height ; ++i ) {
		for ( j = 0 ; j < ppmImage->width ; ++j ) {
			for ( k = 0 ; k < ppmImage->numComponents ; ++k ) {
				index = (i*ppmImage->width + j)*ppmImage->numComponents + k;
                
				pfmImage->data[index] = (float) (ppmImage->data[index])/255.0f; // R
//				pfmImage->data[index + 1] = (float) (ppmImage->data[index + 1])/255.0f; // G
//				pfmImage->data[index + 2] = (float) (ppmImage->data[index + 2])/255.0f; // B
			}
		}
	}
	WritePFM(image_out, pfmImage);
}

void LoadPFMAndSavePPM(const char* image_in, const char* image_out) {
	IMG* ppmImage = (IMG*) malloc(sizeof(IMG));
	FP_IMG* pfmImage = (FP_IMG*) malloc(sizeof(FP_IMG));
	unsigned int i, j, k, index;
	
	loadPFM(image_in, pfmImage);
	ppmImage->data = (unsigned char*) malloc(sizeof(unsigned char) *
                                             pfmImage->width * pfmImage->height * pfmImage->numComponents);
	ppmImage->width = pfmImage->width;
	ppmImage->height = pfmImage->height;
	ppmImage->numComponents = pfmImage->numComponents;
    ppmImage->avgVal = (float*)malloc(sizeof(float) * pfmImage->numComponents);
    ppmImage->minVal = (float*)malloc(sizeof(float) * pfmImage->numComponents);
    ppmImage->maxVal = (float*)malloc(sizeof(float) * pfmImage->numComponents);
    
    for ( k = 0; k < pfmImage->numComponents; ++k ) {
        ppmImage->avgVal[k] = pfmImage->avgVal[k];
        ppmImage->minVal[k] = pfmImage->minVal[k];
        ppmImage->maxVal[k] = pfmImage->maxVal[k];
    }
	for ( i = 0 ; i < ppmImage->height ; ++i ) {
		for ( j = 0 ; j < ppmImage->width ; ++j ) {
			for ( k = 0 ; k < ppmImage->numComponents ; ++k ) {
				index = (i * pfmImage->width + j) * pfmImage->numComponents + k;
				ppmImage->data[index] = (unsigned char) (pfmImage->data[index]*255.0f); //R
//				ppmImage->data[index + 1] = (unsigned char) (pfmImage->data[index + 1]*255.0f);//G
//				ppmImage->data[index + 2] = (unsigned char) (pfmImage->data[index + 2]*255.0f);//B
//                if ( i == 255 && j == 255 ) {
//                    printf("%d %f\n", ppmImage->data[index], pfmImage->data[index]);
//                }
			}
		}
	}
	WritePNM(image_out, ppmImage);
    cleanupFP(pfmImage);
    cleanup(ppmImage);
}

void analyzePPM(const char* imageToAnalyze) {
	IMG* ppmImage = (IMG*) malloc(sizeof(IMG));
	unsigned int i, j, k, height, width, numComponents, idx;
	loadPNM(imageToAnalyze, ppmImage);
	height = ppmImage->height;
	width = ppmImage->width;
	numComponents = ppmImage->numComponents;
	printf("(w, h, c): (%d, %d, %d)\n", width, height, numComponents);
	
	int dataItem;
	int avg[numComponents], min[numComponents], max[numComponents], count[numComponents];
	int histogram[numComponents][256];
    for ( k = 0; k < numComponents; ++k ) {
		avg[k] = count[k] = min[k] = max[k] = 0;
        for ( i = 0; i < 256; ++i ) histogram[k][i] = 0;
	}
    
    for ( i = 0; i < height; ++i ) {
		for ( j = 0; j < width; ++j ) {
			for ( k = 0; k < numComponents; ++k ) {
				idx = (i*width + j)*numComponents + k;
				dataItem = ppmImage->data[idx];
				histogram[k][dataItem]++;
				avg[k] += (dataItem - avg[k]) / (++count[k]);
				if ( dataItem > max[k] ) max[k] = dataItem;
				if ( dataItem < min[k] ) min[k] = dataItem;
				// if ( idx < 10 ) printf("%d", ppmImage->data[idx]);
			}
		}
	}
	for ( k = 0; k < numComponents; ++k) {
		printf("avg: %d; count: %d min: %d max: %d\n", avg[k], count[k], min[k], max[k]);
	}
    
	printf("\n");
	printf("attempt at HDR:\n");
    
	int cdf[3][256];
	for ( k = 0; k < numComponents; ++k ) {
		cdf[k][0] = histogram[k][0];
		for ( i = 1; i < 256; ++i ) {
			cdf[k][i] += histogram[k][i];
            printf("(%d, %d): %d\n", k, i, histogram[k][i]);
		}
	}
    
	unsigned char* newDataItems = (unsigned char *)malloc(sizeof(unsigned char)*height*width*numComponents);
	for ( i = 0; i < height; ++i ) {
		for ( j = 0; j < width; ++j ) {
			for ( k = 0; k < numComponents; ++k ) {
				idx = (i*width + j)*numComponents + k;
				dataItem = ppmImage->data[idx];
				newDataItems[idx] = cdf[k][dataItem];
			}
		}
	}
	IMG* newImage = (IMG*) malloc(sizeof(IMG));
	newImage->data = newDataItems;
	newImage->height = height;
	newImage->width = width;
	newImage->numComponents = numComponents;
	WritePNM("/Users/paul/Desktop/try_hader.ppm", newImage);
    
    IMG* anotherImage = (IMG*) malloc(sizeof(IMG));
    loadPNM("/Users/paul/Desktop/try_hader.ppm", anotherImage);
    

    // reset all the stats to zero
    for ( k = 0; k < numComponents; ++k ) {
		avg[k] = count[k] = min[k] = max[k] = 0;
        for ( i = 0; i < 256; ++i ) histogram[k][i] = 0;
	}
    
    for ( i = 0; i < height; ++i ) {
		for ( j = 0; j < width; ++j ) {
			for ( k = 0; k < numComponents; ++k ) {
				idx = (i*width + j)*numComponents + k;
				dataItem = anotherImage->data[idx];
                avg[k] += (dataItem - avg[k]) / (++count[k]);
                histogram[k][dataItem]++;
			}
		}
	}
    
	for ( k = 0; k < numComponents; ++k ) {
		cdf[k][0] = histogram[k][0];
		for ( i = 1; i < 256; ++i ) {
			cdf[k][i] = cdf[k][i-1] + histogram[k][i];
            printf("(%d, %d): %d\n", k, i, histogram[k][i]);
		}
	}
    
	printf("(h, w, c): (%d, %d, %d)\n", anotherImage->height, anotherImage->width, anotherImage->numComponents);
    
    for ( k = 0; k < numComponents; ++k) {
		printf("new loaded image\n avg: %d; count: %d\n", avg[k], count[k]);
	}

    cleanup(ppmImage);
    cleanup(anotherImage);
    cleanup(newImage);
}

void cleanupFP(FP_IMG* fpImage) {
    free(fpImage->data);
    free(fpImage->maxVal);
    free(fpImage->minVal);
    free(fpImage->avgVal);
    free(fpImage);
}

void cleanup(IMG* image) {
    free(image->data);
    free(image->maxVal);
    free(image->minVal);
    free(image->avgVal);
    free(image);
}