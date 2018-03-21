//TITLE:  recover.c
//AUTHOR: Zachary Miller

//PURPOSE:
// This program will accept 1 argument (a forensic image) and search the
// first 4 bytes of every block for a JPEG file header. If a header is
// found, this program will extract all data between that header and the
// next found header, then write it to a JPEG file.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void write_jpeg(int jpeg_start, int jpeg_end, int file_num, FILE *inptr, int last, int file_size);


int main(int argc, char *argv[]) {
    int file_size = 0;           //variable to hold total size of the file
    int blocks = 0;              //variable to hold total blocks in the file
    int jpeg_start = 0;          //variable to hold start of a JPEG file (block number)
    int file_num = 0;            //variable to hold current file number (for filename)
    int last = 0;                //flag for last image in file

    //check if 1 CLA is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [Forensic Image File]\n",argv[0]);
        return 1;
    }

    //remember filename
    char *infile = argv[1];

    //open provided forensic image file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL) {
        fprintf(stderr, "Could not open input file: %s\n",infile);
        return 2;
    }
    //get size of file, then reset to the beginning
    fseek(inptr, 0, SEEK_END);
    file_size = ftell(inptr);
    rewind(inptr);

    //get total number of blocks in the file
    blocks = ceil(file_size / 512);


    //debug
//    printf("Size:   %i\n",file_size);
//    printf("Blocks: %i\n",blocks);

    //while blocks still need to be checked
    for (int i = 0; i < blocks; i++) {      //CHANGE "10" TO "BLOCKS" IN FINAL VERSION
        //temp storage variable
        unsigned char header[4];

        //move file pointer to beginning of next block
        fseek(inptr, i * 512, SEEK_SET);

//        //debug
//        printf("Current Block: %i\n",i);

        //read first 4 bytes of current block into buffer
        for (int j = 0; j < 4; j++) {
            fread(&header[j], sizeof(char), 1, inptr);
        }

//        //debug --- print first 4 bytes of block
//        for (int k = 0; k < 4; k++) {
//            printf("Byte %i: %#hhx\n",k, header[k]);
//        }

        //check header[] for JPEG header
        if (header[0] == 0xFF) {
            if (header[1] == 0xD8) {
                if (header[2] == 0xFF) {
                    if (header[3] >= 0xE0 && header[3] <= 0xEF) {
                            //if the header is found, check if it is the first one
                            if (jpeg_start != 0) {
//                                //debug
//                                printf("Next header found in block %i\n",i);
//                                printf("executing write_jpeg(%i,%i,%i)\n",jpeg_start, i, file_num);

                                //if this is not the first header found, write the file
                                write_jpeg(jpeg_start, i, file_num, inptr, last, file_size);

                                //move the "jpeg_start" value to point to new header
                                jpeg_start = i;

                                //increment file number
                                file_num++;

//                                //debug
//                                printf("jpeg_start now: %i\n",jpeg_start);
//                                printf("file_num now: %i\n",file_num);

                            } else {
                                //first header found, set jpeg_start
                                jpeg_start = i;

//                                //debug
//                                printf("first header found in block %i, setting jpeg_start\n",i);
//                                printf("jpeg_start = %i\n",jpeg_start);
                            }
                    }
                }
            }
        //if there are no more headers (last image in file)
        } else if (i == blocks - 1) {
            //write last image
            last = 1;
            write_jpeg(jpeg_start, i, file_num, inptr, last, file_size);
        }

        //reset file pointer to beginning of file
        rewind(inptr);
    }


    //close file
    fclose(inptr);

    return 0;
}


//write jpeg to file, jpeg_start = first header; jpeg_end = next header
void write_jpeg(int jpeg_start, int jpeg_end, int file_num, FILE *inptr, int last, int file_size) {
    //convert start and end from blocks to bytes
    jpeg_start *= 512;
    //if last file, end needs to be calculated differently
    if (last == 0) {
        jpeg_end = (jpeg_end * 512);
    } else {
        //set parameters for last image in file
        jpeg_end = file_size;
    }

    int jpeg_size = jpeg_end - jpeg_start;
    char file_name[8];
    sprintf(file_name, "%03i.jpg",file_num);

//    //debug
//    printf("\nfile_name: %s\n\n",file_name);

    //create buffer to store image
    char* buf = calloc(jpeg_size, sizeof(char));
    if (buf == NULL) {
        fclose(inptr);
        fprintf(stderr, "Insufficient memory\n");
        exit(5);
    }

    //open new JPEG file and write recovered data
    FILE *outptr = fopen(file_name, "w");

    //move to jpeg_start
    fseek(inptr, jpeg_start, SEEK_SET);

    //read data to buffer
    fread(buf, jpeg_size, 1, inptr);

    //write buffer to file
    fwrite(buf, jpeg_size, 1, outptr);

    //debug
//    printf("jpeg_start: %i\n",jpeg_start);
//    printf("jpeg_end:   %i\n",jpeg_end);
//    printf("jpeg_size:  %i\n\n",jpeg_size);
//    for (int i = 0; i < 4; i++) {
//        printf("Byte %i, %#hhx\n",i, buf[i]);
//    }
//    printf("\n");

    //free buffer
    free(buf);

    //close JPEG file
    fclose(outptr);
}