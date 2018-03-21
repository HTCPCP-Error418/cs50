// Title:   mario.c
// Author:  Zachary Miller

// Purpose:
// When run, this program will prompt the user for a non-negative integer no greater than 23.
// This program will then create two half-pyramids (a la Mario) to the hight specified by the
// user, with a two block space in the middle.

/* Example:
**  ./mario.c
**  Height: 4
**     #  #
**    ##  ##
**   ###  ###
**  ####  ####
*/

#include <stdio.h>
#include <cs50.h>

int get_height(void);
void print_pyramids(int height);

int main(void){
    int height = get_height();

    //check if number is positive and no greater than 23
    while (height > 23 || height < 0) {
        //number is not valid
//        printf("The number you entered does not meet the neccesary criteria...\n");   //flair that check50 doesn't like
        height = get_height();
    }
//    printf("Height is: %i\n",height);       //debug
    //if height is 0, don't do anything...
    if (height == 0) {
        printf("");
    } else {
        //if height is between 1 and 23, make the pyramids
        print_pyramids(height);
    }
}

int get_height(void) {
    //prompt user for height, 'get_int()' should check if type is int
//    int height = get_int("Please enter a positive number no greater than 23: ");  //flair that check50 doesn't like
    int height = get_int("Height: ");
//    printf("Height in get_height: %i\n", height);     //debug
    return height;
}

void print_pyramids(int height) {
//    char gap = "  ";      //for middle
//    char space = " ";     //for spaces
//    char block = "#";     //for blocks
    int total_space = height - 1;   //counter for needed spaces
    int total_block = 1;            //counter for needed blocks

    //when finished, block_no will be the same number as height
    while (total_block <= height) {
        int current_space = 0;      //counter for current number of spaces
        int current_block = 0;      //counter for current number of blocks

        while (current_space < total_space) {
            printf(" ");    //print spaces (left half)
            current_space++;
        }
        //after spaces are printed, reset the current_space counter
        current_space = 0;

        while (current_block < total_block) {
            printf("#");    //print blocks (left half)
            current_block++;
        }
        //after blocks are printed, reset the current_block counter
        current_block = 0;
        //print gap between pyramids
        printf("  ");

        while (current_block < total_block) {
            printf("#");    //print blocks (right half)
            current_block++;
        }
        //after blocks are printed, reset the current_block counter
        current_block = 0;

//check50 doesn't want spaces printed at the end, but I like it, so it's here...
//        while (current_space < total_space) {
//            printf(" ");    //print spaces (right half)
//            current_space++;
//        }
//        //after spaces are printed, reset the current_space counter
//        current_space = 0;
        //line is done, start a new line and increment counters
        printf("\n");
        total_block++;
        total_space--;
    }
}