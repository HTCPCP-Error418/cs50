#define _XOPEN_SOURCE   //for crypt

#include <stdio.h>  //printf()
#include <cs50.h>   //string data type
#include <string.h> //strcmp()
#include <unistd.h> //for crypt()

int main(int argc, string argv[]) {
    if (argc != 2) {
        exit(1);
    }
    string pass = argv[1];
    string hash = crypt(pass, "50");

    printf("Pass: %s\n",pass);
    printf("Hash: %s\n",hash);
    exit(0);
}