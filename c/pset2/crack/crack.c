//Title:  crack.c
//Author: Zachary Miller

//Purpose:
// This program will accept one command-line argument (a DES-hashed password). When run, this program
// will attempt to crack the password hash. If the crack is successful, this program will output the
// plaintext password and exit.

#define _XOPEN_SOURCE   //for crypt

#include <stdio.h>  //printf()
#include <cs50.h>   //string data type
#include <string.h> //strcmp()
#include <unistd.h> //for crypt()


int error(string prog_name);                        //basic error/usage prompt
void crack(string given_hash);                      //generate guess hash


int main(int argc, string argv[]) {
    //check if only one cla is given
    if (argc != 2) {
        error(argv[0]);
    } else {
        crack(argv[1]);                             //pass hash to crack function
    }
    exit(0);
}



void crack(string given_hash) {
    char dict[52];                                           //create master dictionary (a-zA-Z\0)
    char pass[6] = {'\0','\0','\0','\0','\0','\0'};          //create variable to hold password guess
    char salt[2];                                            //create array for salt
    string guess_hash = "";

/* Note:
**      pass[] needs to be max length of the passwords +1.
**      if the last '\0' gets overwritten, it won't end the
**      string and the password guess will have all of dict[]
**      added to the end.
*/

    //populate dict with lowercase
    for (int i = 0, j = 97; i <= 25; i++, j++) {
        dict[i] = j;
    }
    //populate dict with uppercase
    for (int i = 26, j = 65; i <= 51; i++, j++) {
        dict[i] = j;
    }

    //separate salt from hash
    salt[0] = given_hash[0];
    salt[1] = given_hash[1];

    //generate password guess (there has to be a way to make this one block, try it later)
    //try one character passwords
    for (int i = 0; i < 52; i++) {
        pass[0] = dict[i];
        guess_hash = crypt(pass, salt);
//        printf("Pass: %s\n",pass);                              //debug
//        printf("Guess: %s\n",guess_hash);                       //debug
//        printf("Given: %s\n\n",given_hash);                     //debug
        //compare guess_hash and given_hash
        if (strcmp(guess_hash, given_hash) == 0) {
            printf("%s\n",pass);
            exit(0);
        }
    }
    //try two character passwords
    for (int i = 0; i < 52; i++) {
        pass[0] = dict[i];
        for (int j = 0; j < 52; j++) {
            pass[1] = dict[j];
            guess_hash = crypt(pass, salt);
//            printf("Pass: %s\n",pass);                          //debug
//            printf("Guess: %s\n",guess_hash);                   //debug
//            printf("Given: %s\n\n",given_hash);                 //debug
            //compare guess_hash and given_hash
            if (strcmp(guess_hash, given_hash) == 0) {
                printf("%s\n",pass);
                exit(0);
            }
        }
    }
    //try three character passwords
    for (int i = 0; i < 52; i++) {
        pass[0] = dict[i];
        for (int j = 0; j < 52; j++) {
            pass[1] = dict[j];
            for(int k = 0; k < 52; k++) {
                pass[2] = dict[k];
                guess_hash = crypt(pass, salt);
//                printf("Pass: %s\n",pass);                      //debug
//                printf("Guess: %s\n",guess_hash);               //debug
//                printf("Given: %s\n\n",given_hash);             //debug
                if (strcmp(guess_hash, given_hash) == 0) {
                    printf("%s\n",pass);
                    exit(0);
                }
            }
        }
    }
    //try four character passwords
    for (int i = 0; i < 52; i++) {
        pass[0] = dict[i];
        for (int j = 0; j < 52; j++) {
            pass[1] = dict[j];
            for (int k = 0; k < 52; k++) {
                pass[2] = dict[k];
                for (int l = 0; l < 52; l++) {
                    pass[3] = dict[l];
                    guess_hash = crypt(pass, salt);
//                    printf("Pass: %s\n",pass);                  //debug
//                    printf("Guess: %s\n",guess_hash);           //debug
//                    printf("Given: %s\n\n",given_hash);         //debug
                    if (strcmp(guess_hash, given_hash) == 0) {
                        printf("%s\n",pass);
                        exit(0);
                    }
                }
            }
        }
    }
    //try five character passwords
    for (int i = 0; i < 52; i++) {
        pass[0] = dict[i];
        for (int j = 0; j < 52; j++) {
            pass[1] = dict[j];
            for (int k = 0; k < 52; k++) {
                pass[2] = dict[k];
                for (int l = 0; l < 52; l++) {
                    pass[3] = dict[l];
                    for (int m = 0; m < 52; m++) {
                        pass[4] = dict[m];
                        guess_hash = crypt(pass, salt);
                        if (strcmp(guess_hash, given_hash) == 0) {
                            printf("%s\n",pass);
                            exit(0);
                        }
                    }
                }
            }
        }
    }

    printf("Password not found.\n");
}


int error(string prog_name) {
    printf("Usage: %s [Password Hash]\n",prog_name);
    printf("    The password must be hashed with DES\n");
    exit(1);
}