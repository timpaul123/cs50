/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy resize_by: n infile outfile\n");
        return 1;
    }
    
    int resize_factor = atoi(argv[1]);
    
    // ensure resize factor is non-negative, whole number below 100
    if(resize_factor < 1 || resize_factor > 100)
    {
        printf("The resize factor must be between 1 and 100");
        return 2;
    }
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }
    
    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    // create outfile's header info from infiles
    BITMAPINFOHEADER resized_bi = bi;
    BITMAPFILEHEADER resized_bf = bf;
    
    // resize outfile's header data by n
    resized_bi.biWidth *= resize_factor;
    resized_bi.biHeight *= resize_factor;
    
    // resize outfiles SizeImage using a formula from google
	resized_bi.biSizeImage = ((((resized_bi.biWidth * resized_bi.biBitCount) + 31) & ~31) / 8) * abs(resized_bi.biHeight);

	// determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int resized_padding =  (4 - (resized_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // resize outfiles Size header info, formula provided by google
    resized_bf.bfSize = 54 + resized_bi.biWidth * abs(resized_bi.biHeight) * 3 + abs(resized_bi.biHeight) *  resized_padding;
     
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }   
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&resized_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&resized_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // repeat each row n times to resize vertically and horizontally
        for(int j = 0; j < resize_factor; j++)
        {
            // first read the infile's pixels, one at a time
            for(int k = 0; k < bi.biWidth; k++)
            {
                // storage for an RGBT pixel
                RGBTRIPLE triple;
    
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // now write that pixel n times to resize it
                for(int l = 0; l < resize_factor; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
                
            }// end of row (before padding)
            
            //skip over any padding
            fseek(inptr, padding, SEEK_CUR);
            
            // add any padding
            for(int m = 0; m < resized_padding; m++)
            {
                fputc(0x00, outptr);
            }
            
            // set the file pointer to the start of the original infile row, thus re-copying that row 'n' times to the outfile
            fseek(inptr, -(bi.biWidth *  3 + padding), SEEK_CUR);
            
        }
                
        // move to the next row in the infile
        fseek(inptr, bi.biWidth * 3 + padding, SEEK_CUR);
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
