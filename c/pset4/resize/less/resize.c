//Title:  resize (less)
//Author: Zachary Miller

//Purpose:
// Resizes input BMP file by a factor of "n" and outputs
// the new image to output file.

#include <stdio.h>
#include <stdlib.h> //atoi()
#include <string.h> //memcpy()

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [Resize Factor] [Input File] [Output File]\n",argv[0]);
        fprintf(stderr, "\tResize Factor must be an integer less than or equal to 100\n");
        return 1;
    }

    //get resize factor
    int mod = atoi(argv[1]);
    if (mod > 100 || mod <= 0) {
        fprintf(stderr, "[!] ERROR: Incorrect resize factor.\n");
        return 2;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "[!] ERROR: Could not open input file: %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "[!] ERROR: Could not create output file: %s.\n", outfile);
        return 4;
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
        fprintf(stderr, "[!] ERROR: Unsupported file format.\n");
        return 5;
    }


    //HEADER START ---

    //calculate padding for the infile
    int in_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //create outfile's BITMAPFILEHEADER
    BITMAPFILEHEADER out_bf = bf;
    //create outfile's BITMAPINFOHEADER
    BITMAPINFOHEADER out_bi = bi;

    //update BITMAPINFOHEADER
    out_bi.biHeight *= mod;
    out_bi.biWidth *= mod;

    // determine padding for scanlines
    int out_padding = (4 - (out_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //update biSizeImage
    out_bi.biSizeImage = ((abs(out_bi.biHeight) * out_bi.biWidth) * sizeof(RGBTRIPLE)) + (out_padding * abs(out_bi.biHeight));

    //update BITMAPFILEHEADER
    out_bf.bfSize = out_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    //write outfile's BITMAPFILEHEADER
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    //write outfile's BITMAPINFOHEADER
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    //HEADER END ---


    //IMAGE START ---

    //create buffer to format each scanline
    //create pointer to place data in the correct spots
    char* psl = NULL;
    //scanline_size = width(in bytes) + padding
    int scanline_size = (out_bi.biWidth * sizeof(RGBTRIPLE)) + out_padding;
    //allocate (and zero) memory, set to variable to give us a way to return to the beginning
    char *scanline = calloc(scanline_size, sizeof(char));

    //make sure malloc returns a pointer
    if (scanline == NULL) {
        fprintf(stderr, "[!] ERROR: Out of memory.\n");
        return 5;
    }

    //iterate over scanlines in input
    for (int i = 0, height = abs(bi.biHeight); i < height; i++) {
//        //debug
//        printf("\n\nLINE: %i\n",i);

        //reset placement pointer
        psl = scanline;

        //zero the buffer
        memset(psl, 0x00, scanline_size);

//        //debug
//        printf("line (0): ");
//        for (int z = 0; z < scanline_size; z++) {
//            printf("%#hhX ",scanline[z]);
//        }
//        printf("\n");


        //iterate over pixels in input's scanline
        for (int j = 0; j < in_bi.biWidth; j++) {

            //temp variable to store pixel
            RGBTRIPLE triple;

            //read value of pixel from infile to "triple"
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

//            //debug
//            printf("pixel: %#hhX %#hhX %#hhX\n",triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);

            //write pixel from "triple" to scanline buffer n times
            for (int k = 0; k < mod; k++) {
//                //debug
//                printf("writing pixel %i to buffer (%i time)\n",j, k);
                memcpy(psl, &triple, sizeof(RGBTRIPLE));
                //increment the pointer to not overwrite previous pixel
                psl += sizeof(RGBTRIPLE);
            }
        }

        //skip over infile padding and add padding to scanline buffer
        fseek(inptr, in_padding, SEEK_CUR);
        for (int l = 0; l < out_padding; l++) {
            //add padding 1 byte at a time
            memset(psl, 0x00, sizeof(BYTE));
            //increment the pointer by 1 byte to not overwrite last byte
            psl += sizeof(BYTE);
        }

//        //debug
//        printf("line: ");
//        for (int z = 0; z < scanline_size; z++) {
//            printf("%#hhX ",scanline[z]);
//        }
//        printf("\n");

        //write scanline n times
        for (int m = 0; m < mod; m++) {
//            //debug
//            printf("writing line %i to file (%i time)\n",i, m);
            fwrite(scanline, scanline_size, 1, outptr);
        }
    }

    //IMAGE END ---


    //free memory
    free(scanline);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
