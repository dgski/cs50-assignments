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
    bi.biSizeImage = ((bi.biWidth * factor) * (bi.biHeight * factor)) * 3;
   
    //Create new file size 
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER 
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (orgWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(orgHeight); i < biHeight; i++)
    {
        int counter = 0;
        // iterate over pixels in scanline
        for (int j = 0; j < orgWidth * factor; j++)
        {
            printf("J = %d\n", j);
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            printf("READ PIXEL: %d,%d,%d\n", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);



            // write RGB triple to outfile, factor number of times
            for(int i = 0; i < factor; i++)
            {   
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                printf("WRITING PIXEL: %d,%d,%d\n", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);
            }

            printf("j + orgWidth = %d, orgWidth * factor = %f\n", j + orgWidth, orgWidth * factor);
            
            
            counter++; 
            //Go to beginning of line if not last entry
            if(counter  == (orgWidth) )
            {
               printf("seeking back\n");
               fseek(inptr, -(orgWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
               counter = 0;
            }
            else
            {
                printf("moving on to next line\n");
            }
            
        }


        
        
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
