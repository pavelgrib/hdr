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


int main(int count, char** argv) {
	char* imgdir = "/Users/paul/Dropbox/Imperial/advanced-graphics/HW1/CO417-HW1/Memorial/";

	FP_IMG* hdrImage = (FP_IMG*) malloc(sizeof(FP_IMG));
	FP_IMG* brighterHDRImage = (FP_IMG*) malloc(sizeof(FP_IMG));

	makeHDRImage(imgdir, hdrImage);
	char fileOutputNamePFM[1024];
	char fileOutputNamePPM[1024];
	char toneMappedFilenamePFM[1024];
	char toneMappedFilenamePPM[1024];
	char toneMappedBrightenedFilenamePFM[1024];
	char toneMappedBrightenedFilenamePPM[1024];
	concatenate(imgdir, "memorial_hdr.pfm", fileOutputNamePFM);
	concatenate(imgdir, "memorial_hdr.ppm", fileOutputNamePPM);
	WritePFM(fileOutputNamePFM, hdrImage);
    LoadPFMAndSavePPM(fileOutputNamePFM, fileOutputNamePPM);
	FP_IMG* toneMappedHDR = (FP_IMG*) malloc(sizeof(FP_IMG));
    
	toneMap(hdrImage, toneMappedHDR);
    brightenPFM(hdrImage, brighterHDRImage, 100);
	concatenate(imgdir, "memorial_tonemap.pfm", toneMappedFilenamePFM);
	concatenate(imgdir, "memorial_tonemap.ppm", toneMappedFilenamePPM);
    concatenate(imgdir, "memorial_tm_brighter.pfm", toneMappedBrightenedFilenamePFM);
    concatenate(imgdir, "memorial_tm_brighter.ppm", toneMappedBrightenedFilenamePPM);
	WritePFM(toneMappedFilenamePFM, toneMappedHDR);
	LoadPFMAndSavePPM(toneMappedFilenamePFM, toneMappedFilenamePPM);
    WritePFM(toneMappedBrightenedFilenamePFM, brighterHDRImage);
    LoadPFMAndSavePPM(toneMappedBrightenedFilenamePFM, toneMappedBrightenedFilenamePPM);
	printf("done!\n");

	return 0;
}