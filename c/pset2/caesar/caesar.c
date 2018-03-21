//Title: Caesar
//Author: Zachary Miller

//Purpose:
// This program accepts one Command Line Argument (a non-negative integer). If a valid CLA is not
// provided, this program will print an error message and main will return 1.
//
// When run with a valid CLA, this program will prompt the user for a plaintext string to encrypt.
// This program will then implement Caesar's cipher to encrypt the string (preserving case) with
// the number provided as the CLA, outputting the ciphertext to the terminal.

#include <stdio.h>  //printf()
#include <cs50.h>   //get_string()
#include <string.h> //strlen()
#include <ctype.h>  //islower()
#include <stdlib.h> //atoi()


int main(int argc, string argv[]) {
    int key = 0;                //make variable for key and set it to the CLA
    string pt = "";             //make variable to store plaintext string

    //check if one CLA, assume non-negative int
    if (argc != 2) {
        printf("[!] ERR: Incorrect number of arguments passed.\n");
        printf("[!] Usage: %s [key]\n",argv[0]);
        printf("[!]     Key should be a non-negative integer.\n");
        exit(1);
    } else {
        //CLA is valid
        key = atoi(argv[1]) % 26;                //set key to CLA
        pt = get_string("plaintext:  ");         //get plaintext string
        char ct[strlen(pt)];                     //make array to hold ciphertext string
        int int_pt = 0;                          //make variable to hold int value of char

        //encrypt string
        for (int i = 0; i < strlen(pt); i++) {

            if (isalpha(pt[i])) {                //skip over non-alpha characters
                int_pt = (int) pt[i];            //use int value of character

                if (islower(pt[i])) {
                    int_pt += key;               //add key to character
                    if (int_pt > 122) {          //if character is no longer a-z
                        ct[i] = int_pt - 26;     //loop around the alphabet
                    } else {                     //character is still a-z
                        ct[i] = int_pt;
                    }
                } else {                         //character is uppercase
                    int_pt += key;               //add key to character
                    if (int_pt > 90) {           //if character is no longer A-Z
                        ct[i] = int_pt - 26;     //loop around the alphabet
                    } else {                     //character is still A-Z
                        ct[i] = int_pt;
                    }
                }
            } else {                                 //character is non-alpha
                ct[i] = pt[i];
            }
        }

        //print ciphertext
        printf("ciphertext: ");
        for (int i = 0; i < strlen(pt); i++) {
            printf("%c",(char) ct[i]);
        }
        printf("\n");                       //print newline
    }
    exit(0);
}

