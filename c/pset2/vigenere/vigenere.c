//Title:  vigenere.c
//Author: Zachary Miller

//Purpose:
// This program accepts one string as a command-line argument (the key). When run, this program will
// prompt the user for a plaintext string. This program will then implement the Vigenere cipher on
// the provided string, using the key supplied as a command-line argument. This program will then
// output the ciphertext string.

#include <stdio.h>  //printf()
#include <cs50.h>   //get_string()
#include <string.h> //strlen()
#include <ctype.h>  //islower()

void vigenere(string key);
int error(string name);


int main(int argc, string argv[]) {
    string key = "";                //make variable for key

    //error checking for key
    if (argc != 2) {                //check if only one cla is passed
        error(argv[0]);
    } else {                        //one cla is passed, check if it is all alpha
        key = argv[1];       //set the key
        for (int i = 0; i < strlen(key); i++) {
            if (isalpha(key[i]) == false) {
                error(argv[0]);
            }
        }
    }

    //key passed error checking, convert key to lowercase and encrypt
    for (int i = 0; i < strlen(key); i++) {
        key[i] = (char) tolower(key[i]);    //to lower only allows one letter?
    }
    vigenere(key);


    exit(0);
}

void vigenere(string key) {
    int key_array[strlen(key)];                          //make array to hold int values for key char
    int key_len = strlen(key);                           //make variable to hold key length
    int key_pos = 0;                                     //make counter for position in key
    int pt_int = 0;                                      //make variable to hold int of current pt char
    string pt_str = get_string("plaintext:  ");          //get plaintext string
    char ct_array[strlen(pt_str)];                        //create array to hold ct string

    //fill key_array with int values
    for (int i = 0; i < key_len; i++) {
        key_array[i] = (int) key[i] - 97;                //make 'a' = 0 and 'z' = 25
    }

    //begin encrypting pt string with key_array and saving to ct_array
    for (int i = 0; i < strlen(pt_str); i++) {
        if (key_pos >= key_len) {                         //loop around key if the pt string is longer
            key_pos = 0;
        }

//        printf("Current Key Pos: %i\n",key_pos);           //debug


        if (isalpha(pt_str[i])) {                        //only manipulate alpha characters
            pt_int = (int) pt_str[i];                    //get int value of current pt char
            pt_int += key_array[key_pos];                //add current key value to pt_int

            if (islower(pt_str[i])) {                    //if current pt char is lowercase
                if (pt_int > 122) {                      //check if need to loop around lowercase alphabet
                    ct_array[i] = pt_int - 26;           //loop around and add to ct string
                } else {
                    ct_array[i] = pt_int;                //no need to loop, add to ct string
                }
            } else {                                     //current pt char is uppercase
                if (pt_int > 90) {                       //check if need to loop around uppercase alphabet
                    ct_array[i] = pt_int - 26;           //loop around and add to ct string
                } else {
                    ct_array[i] = pt_int;                //no need to loop, add to ct string
                }
            }
            key_pos++;                                   //increment key position counter if key was used
        } else {                                         //if current pt char is non-alpha, skip it
            ct_array[i] = pt_str[i];
        }
    }

    //print ct string
    printf("ciphertext: ");
    for (int i = 0; i < strlen(pt_str); i++) {
        printf("%c",(char) ct_array[i]);
    }
    printf("\n");
}


int error(string name) {
    printf("Usage: %s [key]\n",name);
    printf("    key must be one word of only alpha characters\n");
    exit(1);
}