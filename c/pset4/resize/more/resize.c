//TITLE:  resize (more)
//AUTHOR: Zachary Miller

//PURPOSE:
// This program accepts a float value, input file (a bitmap), and an output filename.
// This program then attempts to resize the input file by the provided float value
// and save the resized image to the output filename.

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memcpy()
#include <math.h>   //roundf()

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [Resize Factor] [Input File] [Output File]\n",argv[0]);
        fprintf(stderr, "\tResize Factor must be a float between 0.0 and 100.0\n");
        return 1;
    }

    //get resize factor, round to nearest 10ths
    float mod = roundf(atof(argv[1]) * 10) / 10;
    if (mod > 100.0 || mod <= 0.0) {
        fprintf(stderr, "[!] ERROR: Invalid resize factor.\n");
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
        fprintf(stderr, "Could not create output file: %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER in_bf;
    fread(&in_bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER in_bi;
    fread(&in_bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (in_bf.bfType != 0x4d42 || in_bf.bfOffBits != 54 || in_bi.biSize != 40 ||
        in_bi.biBitCount != 24 || in_bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //calculate input file's padding
    int in_padding = (4 - (in_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    //HEADERS START ---

    //create outfile headers
    BITMAPFILEHEADER out_bf = in_bf;
    BITMAPINFOHEADER out_bi = in_bi;

    //update height and width in out_bi
    out_bi.biHeight = round(in_bi.biHeight * mod);
    out_bi.biWidth = round(in_bi.biWidth * mod);

    //calculate output file's padding
    int out_padding = (4 - (out_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //update output file's biSizeImage
    out_bi.biSizeImage = ((abs(out_bi.biHeight) * out_bi.biWidth) * 3) + (out_padding * abs(out_bi.biHeight));

    //update output file's bfSize
    out_bf.bfSize = out_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    //write updated headers to outfile
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    //HEADERS END ---


    //IMAGE START ---

    //create pointer to place data in the correct location
    char* psl = NULL;
    //calculate the size of the outfile scanline
    int scanline_size = (out_bi.biWidth * sizeof(RGBTRIPLE)) + out_padding;
    //create buffer for outfile scanline
    char *scanline = calloc(scanline_size, sizeof(char));
    //make sure buffer was created
    if (scanline == NULL) {
        fprintf(stderr, "[!] Insufficient memory\n");
        return 5;
    }

    //flag that all pixels in line need to be repeated
    int all = 0;
    //flag that image needs to be smaller
    int small = 0;
    //get number of pixels/lines to repeat
    int repeat = 0;
    //if image needs to be smaller
    if (mod < 1.0) {
        repeat = round(in_bi.biWidth / out_bi.biWidth);
        small = 1;
    } else {
        //image needs to get bigger (non whole number)
        repeat = out_bi.biWidth % in_bi.biWidth;
        //image needs to get bigger (whole number)
        if (repeat == 0) {
            repeat = out_bi.biWidth / in_bi.biWidth;
            all = 1;
        }
    }

/*    //debug (print variable info)
    printf("Mod: %f\n",mod);
    printf("All: %i\n",all);
    printf("Small: %i\n",small);
    printf("Repeat: %i\n",repeat);
    printf("File: %lu\n",sizeof(BITMAPFILEHEADER));
    printf("Info: %lu\n",sizeof(BITMAPINFOHEADER));
    printf("Buffer size: %i\n\n",scanline_size);

    printf("Original height: %i\n",in_bi.biHeight);
    printf("Original width:  %i\n",in_bi.biWidth);
    printf("Original biSI:   %i\n",in_bi.biSizeImage);
    printf("Original bfSize: %i\n",in_bf.bfSize);
    printf("Original pad:    %i\n\n",in_padding);

    printf("New height: %i\n",out_bi.biHeight);
    printf("New width:  %i\n",out_bi.biWidth);
    printf("New biSI:   %i\n",out_bi.biSizeImage);
    printf("New bfsize: %i\n",out_bf.bfSize);
    printf("New pad:    %i\n\n",out_padding);
*/

    // iterate over infile's scanlines
    for (int i = 0, height = abs(in_bi.biHeight); i < height; i++)
    {
        //debug
//        printf("\nOn line: %i\n",i);

        //reset scanline location pointer
        psl = scanline;
        //zero the buffer
        memset(psl, 0x00, scanline_size);

        //for each pixel in infile's scanline
        for (int j = 0; j < in_bi.biWidth; j++)
        {
            //debug
//            printf("On pixel: %i\n",j);

            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            //first [repeat] pixels need to be repeated one time
            if (all == 0 && small == 0 && j <= repeat) {
                //debug
//                printf("Repeating %i pixels one time\n",repeat);

                for (int k = 0; k < 2; k++) {
                    memcpy(psl, &triple, sizeof(RGBTRIPLE));
                    psl += sizeof(RGBTRIPLE);
                }
            //all pixels need to be repeated [repeat] times
            } else if (all == 1) {
                //debug
//                printf("Repeating all pixels %i times\n",repeat);

                for (int k = 0; k < repeat; k++) {
                    memcpy(psl, &triple, sizeof(RGBTRIPLE));
                    psl += sizeof(RGBTRIPLE);
                }
            //image needs to be smaller, put every other pixel
            } else if (small == 1) {
                if (j % repeat == 0) {
                    //debug
//                    printf("Writing every %i pixels\n",repeat);
                    memcpy(psl, &triple, sizeof(RGBTRIPLE));
                    psl += sizeof(RGBTRIPLE);
                }
            //no pixels need to be repeated, put once
            } else {
                //debug
//                printf("No need to repeat\n");

                memcpy(psl, &triple, sizeof(RGBTRIPLE));
                psl += sizeof(RGBTRIPLE);
            }
        }
        //skip over infile padding and add outfile padding
        fseek(inptr, in_padding, SEEK_CUR);
        for (int l = 0; l < out_padding; l++) {
            //add padding 1 byte at a time
            memset(psl, 0x00, sizeof(char));
            psl += sizeof(char);
        }
        //should now have complete scanline, write to file
        //first [repeat] lines need to be repeated once
        if (all == 0 && small == 0 && i <= repeat) {
            //debug
//            printf("Repeating %i lines one time\n",repeat);

            for (int m = 0; m < 2; m++) {
                fwrite(scanline, scanline_size, 1, outptr);
            }
        //all lines need to be repeated [repeat] times
        } else if (all == 1 && small == 0) {
            //debug
//            printf("Repeating all lines %i times\n",repeat);

            for (int m = 0; m < repeat; m++) {
                fwrite(scanline, scanline_size, 1, outptr);
            }
        //image needs to be smaller, put every other line
        } else if (all == 0 && small == 1) {
            if(i % repeat == 0) {
                //debug
//                printf("Writing every %i lines\n",repeat);

                fwrite(scanline, scanline_size, 1, outptr);
            }
        //no lines need to be repeated, put once
        } else {
            //debug
//            printf("No need to repeat\n");

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
