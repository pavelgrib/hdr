/*//////////////////////////////////////////////////////////////////////////
Author: Abhijeet Ghosh
Year: 2013
//////////////////////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hdr.h"
#include "loadPNM.h"
#include "loaders.h"
#include "relighting.h"


int main(int count, char** argv) {

    printf("----------------------------- HDR ----------------------------------\n\n");
	char* imgdir = "/Users/paul/Dropbox/Imperial/advanced-graphics/HW1/CO417-HW1/Memorial/";

    // applying HDR to original images
    char fileOutputNamePFM[1024];
	char fileOutputNamePPM[1024];
	concatenate(imgdir, "memorial_hdr.pfm", fileOutputNamePFM);
	concatenate(imgdir, "memorial_hdr.ppm", fileOutputNamePPM);
	FP_IMG* hdrImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    makeHDRImage(imgdir, hdrImage);
	WritePFM(fileOutputNamePFM, hdrImage);
    LoadPFMAndSavePPM(fileOutputNamePFM, fileOutputNamePPM);

    // applying linear tone-mapping to HDR image
    char toneMappedFilenamePFM[1024];
	char toneMappedFilenamePPM[1024];
    concatenate(imgdir, "memorial_tonemap.pfm", toneMappedFilenamePFM);
	concatenate(imgdir, "memorial_tonemap.ppm", toneMappedFilenamePPM);
	FP_IMG* toneMappedHDR = (FP_IMG*) malloc(sizeof(FP_IMG));
	toneMap(hdrImage, toneMappedHDR);
	WritePFM(toneMappedFilenamePFM, toneMappedHDR);
	LoadPFMAndSavePPM(toneMappedFilenamePFM, toneMappedFilenamePPM);
    
    // applying brightening to tone-mapped
    char toneMappedBrightenedFilenamePFM[1024];
	char toneMappedBrightenedFilenamePPM[1024];
    concatenate(imgdir, "memorial_tm_brighter.pfm", toneMappedBrightenedFilenamePFM);
    concatenate(imgdir, "memorial_tm_brighter.ppm", toneMappedBrightenedFilenamePPM);
    FP_IMG* brighterHDRImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    brightenPFM(toneMappedHDR, brighterHDRImage, 500);
    WritePFM(toneMappedBrightenedFilenamePFM, brighterHDRImage);
    LoadPFMAndSavePPM(toneMappedBrightenedFilenamePFM, toneMappedBrightenedFilenamePPM);

    // applying gamma to brighened image
    char gammaAppliedPFM[1024];
    char gammaAppliedPPM[1024];
    concatenate(imgdir, "memorial_gamma.pfm", gammaAppliedPFM);
    concatenate(imgdir, "memorial_gamma.ppm", gammaAppliedPPM);
    applyGamma(brighterHDRImage, 3);
    WritePFM(gammaAppliedPFM, brighterHDRImage);
    LoadPFMAndSavePPM(gammaAppliedPFM, gammaAppliedPPM);
    
    
    printf("----------------------------- IBL ----------------------------------\n\n");
    
    char* grace_imgdir = "/Users/paul/Dropbox/Imperial/advanced-graphics/HW1/CO417-HW1/GraceCathedral/";
    char ibl_original[1024];
    char iblPPM[1024];
    char iblPFM[1024];
    concatenate(grace_imgdir, "grace_latlong.pfm", ibl_original);
    FP_IMG* relitImage = (FP_IMG*) malloc(sizeof(FP_IMG));
    printf("%d, %d\n", relitImage->height, relitImage->width);
    generateRelitSphere(ibl_original, relitImage, 511, grace_imgdir);
    concatenate(grace_imgdir, "grace_relit.pfm", iblPFM);
    concatenate(grace_imgdir, "grace_relit.ppm", iblPPM);
    printf("%d, %d\n", relitImage->height, relitImage->width);
    WritePFM(iblPFM, relitImage);
    LoadPFMAndSavePPM(iblPFM, iblPPM);
    
    printf("----------------------------- done ---------------------------------\n\n");

	return 0;
}




















