/**
 * Resizes a specified bitmap image
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }

    // remember filenames
    float factor = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //Save original dimensions
    LONG orgWidth = bi.biWidth;
    LONG orgHeight = bi.biHeight;
   
    //Create new dimensions
    bi.biWidth = (bi.biWidth * factor);
    bi.biHeight = (bi.biHeight * factor);
   
    // determine new padding for scanlines
    int new_padding = (4 - ((bi.biWidth * sizeof(RGBTRIPLE))) % 4) % 4;
   
    bi.biSizeImage = (((bi.biWidth * factor)+ new_padding) * (bi.biHeight * factor)) * 3;
   
    //Create new file size 
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER 
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int old_padding = (4 - ((orgWidth * sizeof(RGBTRIPLE))) % 4) % 4;
    

    if(factor >= 1)
    {
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(orgHeight); i < biHeight; i++)
    {
        
        
        for(int z = 0; z < factor; z++)
        {

            // iterate over pixels in scanline
            for (int j = 0; j < orgWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile, factor number of times
                for(int i = 0; i < factor; i++)
                {   
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }    
                //Insert new padding into new file
                for (int k = 0; k < new_padding; k++)
                {
                    fputc(0x00, outptr);
                }    
            
            //If this is not the last replication - go back to beginning of infile line 
            if(z != (factor - 1))
                fseek(inptr, -(orgWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
        }
        // skip over old padding, if any
        fseek(inptr, old_padding, SEEK_CUR);

    }
    }

    else
    {
        




    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
