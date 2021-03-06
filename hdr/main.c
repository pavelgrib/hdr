/*//////////////////////////////////////////////////////////////////////////
Author: Abhijeet Ghosh
Year: 2013
//////////////////////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "hdr.h"
#include "loadPNM.h"
#include "loaders.h"
#include "relighting.h"
#include "vectorOps.h"
#include "image_sampling.h"

int main(int count, char** argv) {
    unsigned int i;
    // all for CW1
//    printf("----------------------------- HDR ----------------------------------\n\n");
//	char* imgdir = "/Users/paul/Dropbox/Imperial/advanced-graphics/HW1/CO417-HW1/Memorial/";
//
//    // applying HDR to original images
//    char fileOutputNamePFM[1024];
//	char fileOutputNamePPM[1024];
//	concatenate(imgdir, "memorial_hdr.pfm", fileOutputNamePFM);
//	concatenate(imgdir, "memorial_hdr.ppm", fileOutputNamePPM);
//	FP_IMG* hdrImage = (FP_IMG*) malloc(sizeof(FP_IMG));
//    makeHDRImage(imgdir, hdrImage);
//	WritePFM(fileOutputNamePFM, hdrImage);
//    LoadPFMAndSavePPM(fileOutputNamePFM, fileOutputNamePPM);
//
//    // applying linear tone-mapping to HDR image
//    char toneMappedFilenamePFM[1024];
//	char toneMappedFilenamePPM[1024];
//    concatenate(imgdir, "memorial_tonemap.pfm", toneMappedFilenamePFM);
//	concatenate(imgdir, "memorial_tonemap.ppm", toneMappedFilenamePPM);
//	FP_IMG* toneMappedHDR = (FP_IMG*) malloc(sizeof(FP_IMG));
//	toneMap(hdrImage, toneMappedHDR);
//	WritePFM(toneMappedFilenamePFM, toneMappedHDR);
//	LoadPFMAndSavePPM(toneMappedFilenamePFM, toneMappedFilenamePPM);
//    
//    // applying brightening to tone-mapped
//    char toneMappedBrightenedFilenamePFM[1024];
//	char toneMappedBrightenedFilenamePPM[1024];
//    concatenate(imgdir, "memorial_tm_brighter.pfm", toneMappedBrightenedFilenamePFM);
//    concatenate(imgdir, "memorial_tm_brighter.ppm", toneMappedBrightenedFilenamePPM);
//    FP_IMG* brighterHDRImage = (FP_IMG*) malloc(sizeof(FP_IMG));
//    brightenPFM(toneMappedHDR, brighterHDRImage, 5000);
//    WritePFM(toneMappedBrightenedFilenamePFM, brighterHDRImage);
//    LoadPFMAndSavePPM(toneMappedBrightenedFilenamePFM, toneMappedBrightenedFilenamePPM);
//
//    // applying gamma to brighened image
//    char gammaAppliedPFM[1024];
//    char gammaAppliedPPM[1024];
//    concatenate(imgdir, "memorial_gamma.pfm", gammaAppliedPFM);
//    concatenate(imgdir, "memorial_gamma.ppm", gammaAppliedPPM);
//    applyGamma(brighterHDRImage, 2.2);
//    WritePFM(gammaAppliedPFM, brighterHDRImage);
//    LoadPFMAndSavePPM(gammaAppliedPFM, gammaAppliedPPM);
//    
//    
//    printf("------------------------------ IBL ----------------------------------\n\n");
//    
//    char* grace_imgdir = "/Users/paul/Dropbox/Imperial/advanced-graphics/HW1/CO417-HW1/GraceCathedral/";
//    char ibl_original[1024];
//    concatenate(grace_imgdir, "grace_latlong.pfm", ibl_original);
//    FP_IMG* relitImage = (FP_IMG*) malloc(sizeof(FP_IMG));
//    generateRelitSphere(ibl_original, relitImage, 511, grace_imgdir);
//    
//    char EMSpherePFM[1024];
//    char EMSpherePPM[1024];
//    concatenate(grace_imgdir, "latlong_em.pfm", EMSpherePFM);
//    concatenate(grace_imgdir, "latlong_em.ppm", EMSpherePPM);
//    WritePFM(EMSpherePFM, relitImage);
//    LoadPFMAndSaveClampedPPM(EMSpherePFM, EMSpherePPM, 1.0);
////    printImageData((void*)relitImage->data, 511, 511, 3, 1);
//    
//    printf("\n----------------------------- done --------------------------------\n\n");

    
    // all for CW2
    srand((unsigned int) time(NULL));
    char* graceEMDir ="/Users/paul/Dropbox/Imperial/advanced-graphics/GraceCathedral/";
    char graceEMPath[1024];
    concatenate(graceEMDir, "grace_latlong.pfm", graceEMPath);
    char pfmFileName[1024], ppmFileName[1024];
    char ppmPath[1024], pfmPath[1024];
    FP_IMG* grace_em = (FP_IMG*) malloc(sizeof(FP_IMG));
    

    loadPFM(graceEMPath, grace_em);

//    WritePFM("/Users/paul/Destop.test.pfm", grace_em);
//    LoadPFMAndSavePPM("/Users/paul/Destop.test.pfm", "/Users/paul/Destop.test.ppm");
    
    FP_IMG* cdf_sampled = (FP_IMG*) malloc(sizeof(FP_IMG));
    FP_IMG* phong_sampled = (FP_IMG*) malloc(sizeof(FP_IMG));
    
    int nums[] = {64, 256, 1024};
    for ( i = 0; i < 3; ++i ) {
        char buff[20], buff2[20];
        sprintf(buff, "%d.pfm", nums[i]);
        sprintf(buff2, "%d.ppm", nums[i]);
        generateCDFSamples(grace_em, cdf_sampled, nums[i]);
        concatenate("grace_cdf_sampled", buff, pfmFileName);
        concatenate(graceEMDir, pfmFileName, pfmPath);
        concatenate("grace_cdf_sampled", buff2, ppmFileName);
        concatenate(graceEMDir, ppmFileName, ppmPath);
        WritePFM(pfmPath, cdf_sampled);
        LoadPFMAndSavePPM(pfmPath, ppmPath);
        generatePhongSamples(grace_em, phong_sampled, nums[i]);
    }
    
    cleanupFP(cdf_sampled);
    cleanupFP(grace_em);
    
    
    
    
    
    
    
    
    
	return 0;
}