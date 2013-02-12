#include "loadPNM.h"

unsigned char* loadPNM( const char* filename, IMG* image ) {
    FILE*         imgFile;
    char          buf[1024];
    char          type;
    int           i, j, k, l;
    // double        distance, x, y;
    unsigned char* imageData;
    // unsigned char* tmpData;
    unsigned int width, height, numComponents;
    // printf("%s\n", filename);
    imgFile = fopen( filename, "rb" );
    //  if( errno!= 0 )
    //    return NULL;
    // read PNM magic number (P1 to P6)
    if( fscanf( imgFile, "P%c \n", &type ) != 1 || type < '1' || type > '8') {
        fprintf(stderr, "Unknown filetype\n" );
        fclose( imgFile );
        return NULL;
    }
    // skip comments
    while( fscanf( imgFile, "#%[^\n]\n", buf ) ) ;
    // read width
    fscanf( imgFile, "%d", &width );
    /* skip comments */
    while( fscanf( imgFile, "#%[^\n]\n", buf ) ) ;
    /* read height */
    fscanf( imgFile, "%d", &height );
    /* skip comments */
    while( fscanf( imgFile, "#%[^\n]\n", buf ) ) ;
    /* skip max. component and exactly one whitespace */
    fscanf( imgFile, "%*d%*c" );
    printf("type is %c\n", type);
    switch( type ) {
        case '1': // ASCII bitmap
        case '4': // binary bitmap
            fprintf( stderr, "Bitmaps not implemented\n" );
            fclose( imgFile );
            return NULL;
        case '2': // ASCII greymap
            imageData = (unsigned char*) malloc( sizeof(unsigned char) * width * height * 3);
            for( i= 0 ; i < height ; i++ )
                for( j= 0 ; j < width ; j++ ) {
                    fscanf( imgFile, "%d", &l );
                    imageData[i*width+j]= l;
                }
            numComponents= 1;
            break;
        case '3': // ASCII RGB
            imageData = (unsigned char*) malloc( sizeof(unsigned char) * width * height * 3);
            for( i = 0 ; i < height ; i++ )
                for( j = 0 ; j < width ; j++ )
                    for( k = 0 ; k < 3 ; k++ ) {
                        fscanf( imgFile, "%d", &l );
                        imageData[(i*width+j)*3+k]= l;
                    }
            numComponents= 3;
            break;
        case '5': // binary greymap
            imageData = (unsigned char*) malloc( sizeof(unsigned char) * width * height );
            fread( imageData, 1, width*height, imgFile );
            numComponents= 1;
            break;
        case '6': // binary RGB
            imageData= (unsigned char*) malloc( sizeof(unsigned char) * width * height * 3);
            fread( imageData, 1, width*height*3, imgFile );
            numComponents= 3;
            break;
    }
    fclose( imgFile );
    image->data = imageData;
    image->height = height;
    image->width = width;
    image->numComponents = 3;
    return imageData;
}

void WritePNM( const char *filename, IMG* image ) {
    FILE*          fp;
    // char          buf[1024];
    // char          type;
    // int           i, j, k, l;
    // double        distance, x, y;
  

    // Write PGM image file with filename "file"

    // The PGM file format for a GREYLEVEL image is:
    // P5 (2 ASCII characters) <CR>
    // two ASCII numbers for nx ny (number of rows and columns <CR>
    // 255 (ASCII number) <CR>
    // binary pixel data with one byte per pixel

    // The PGM file format for a COLOR image is:
    // P6 (2 ASCII characters) <CR>
    // two ASCII numbers for nx ny (number of rows and columns <CR>
    // 255 (ASCII number) <CR>
    // binary pixel data with three bytes per pixel (one byte for each R,G,B)
    fp=fopen(filename,"wb");

    // write the first ASCII line with the file type
	if( image->numComponents == 1 ) fprintf(fp,"P5\n"); //greylevel image
    else if( image->numComponents == 3 ) fprintf(fp,"P6\n");  // color image

    // write image dimensions
    fprintf(fp,"%d %d\n", image->width, image->height);

	// write '255' to the next line
    fprintf(fp,"255\n");
    fwrite(image->data, sizeof(unsigned char), image->width * image->height * image->numComponents, fp);
    fclose(fp);
}

float* loadPFM( const char *filename, FP_IMG* image ) {
    FILE*         imgFile;
    char          buf[1024];
    char          type;
    int           i, j, k;
    float*        imageData;
    float*        tmpData;
    unsigned int  height, width, numComponents=3;
    unsigned int  avgCount[numComponents];
    float*        avgPixel = malloc(sizeof(float)*numComponents);
    float*        maxPixel = malloc(sizeof(float)*numComponents);
    float*        minPixel = malloc(sizeof(float)*numComponents);

    for ( k = 0; k < numComponents; ++k ) {
        avgPixel[k]=maxPixel[k]=minPixel[k]=0.0;
        avgCount[k] = 0;
    }
    imgFile= fopen( filename, "rb" );
    fscanf( imgFile, "PF \n", &type );
    // skip comments
    while( fscanf( imgFile, "#%[^\n]\n", buf ) );
    // read width
    fscanf( imgFile, "%d", &width );
    image->width = width;
    /* skip comments */
    while( fscanf( imgFile, "#%[^\n]\n", buf ) );
    /* read height */
    fscanf( imgFile, "%d", &height );
    image->height = height;
    /* skip comments */
    while( fscanf( imgFile, "#%[^\n]\n", buf ) );
    /* skip max. component and exactly one whitespace */
    fscanf( imgFile, "%*f%*c" );
    imageData= (float*) malloc(sizeof(float) * width * height * 3) ;
    fread( imageData, sizeof(float), width * height * 3, imgFile );
    
    image->numComponents = numComponents;

    fclose( imgFile );
    
    tmpData = (float*) malloc(sizeof(float) * width * 3) ;
    
    //invert image for reading!!!
    for(i = 0; i < height/2; i++ ) {
        for(j = 0; j < width; j++ ) {
            for(k = 0; k < 3; k++ ) {
                int indexS = (i*width + j)*3 + k;
                int indexD = (height-1 - i)*width*3 + j*3 + k;
                
                tmpData[j*3 + k] = imageData[indexS];
                imageData[indexS] = imageData[indexD];
                imageData[indexD] = tmpData[j*3 + k];
                
                // keeping track of pixel stats
                avgPixel[k] += (imageData[indexD] - avgPixel[k]) / (++avgCount[k]);
                avgPixel[k] += (imageData[indexS] - avgPixel[k]) / (++avgCount[k]);
                
                if ( imageData[indexS] > maxPixel[k] ) maxPixel[k] = imageData[indexS];
                if ( imageData[indexD] > maxPixel[k] ) maxPixel[k] = imageData[indexD];
                if ( imageData[indexS] < minPixel[k] ) minPixel[k] = imageData[indexS];
                if ( imageData[indexD] < minPixel[k] ) minPixel[k] = imageData[indexD];
            }
        }
    }
    image->avgVal = avgPixel;
    image->minVal = minPixel;
    image->maxVal = maxPixel;
//    printf("(min, avg, max) pixels: (%f, %f, %f)\n", minPixel[0], avgPixel[0], maxPixel[0]);
    free(tmpData);
    image->data = imageData;

    return imageData;
}

void WritePFM( const char* filename, FP_IMG* image ) {
    FILE          *fp;
    int           i;
    // char          buf[1024];
    // char          type;
    // double        distance, x, y;
    // float max=1.0f;
    // char space=' ';
    
    // Write PGM image file with filename "file"
    
    // The PGM file format for a GREYLEVEL image is:
    // P5 (2 ASCII characters) <CR>
    // two ASCII numbers for nx ny (number of rows and columns <CR>
    // 255 (ASCII number) <CR>
    // binary pixel data with one byte per pixel
    
    // The PGM file format for a COLOR image is:
    // P6 (2 ASCII characters) <CR>
    // two ASCII numbers for nx ny (number of rows and columns <CR>
    // 255 (ASCII number) <CR>
    // binary pixel data with three bytes per pixel (one byte for each R,G,B)
    
    fp = fopen(filename, "wb");
    fputc('P', fp);
	fputc('F', fp);
	fputc(0x0a, fp);
    
	fprintf(fp, "%d %d", image->width, image->height);
	fputc(0x0a, fp);
    
	fprintf(fp, "%f", -1.0f);
	fputc(0x0a, fp);
    
	// write image data upside down!!!
    // since we flipped the image upside-down when we read it
 	// we have to write it upside-down so it's stored the right way
    for( i = image->height - 1; i >= 0; --i ) {
        fwrite( &image->data[i * image->width * image->numComponents],
               sizeof(float), image->width * image->numComponents, fp );
    }
	fclose(fp);
}
