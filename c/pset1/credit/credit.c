// Title: credit.c
// Author: Zachary Miller

// Purpose:
// When run, this program will prompt for a credit card number from the user.
// This program will then use Luhn's Algorithm and card standards to determine
// the validity of the number and the card's issuer. This program will then
// output the issuer or "INVALID".

#include <stdio.h>
#include <cs50.h>

int get_length(long long cc_num);
int check_digits(long long cc_num, int length);
bool check_checksum(long long cc_num, int length);
int invalid(void);





int main(void) {
    int length = 0;         //variable for number of digits in cc_num
    int issuer = 0;         //variable for the card type

    //get credit card number
    long long cc_num = get_long_long("Number: ");
        //pset says not to sanitize input...

    //get number of digits in cc_num
    length = get_length(cc_num);

    //check issuer
    issuer = check_digits(cc_num, length);

    //number looks valid, caculate checksum
    if (check_checksum(cc_num, length)) {
        //number is valid
        switch (issuer) {   //didn't want to deal with a char array, did this instead
            case 1 :
                printf("VISA\n");
                break;
            case 2 :
                printf("AMEX\n");
                break;
            case 3 :
                printf("MASTERCARD\n");
                break;
        }
    } else {
        invalid();
    }
//    printf("%i\n",issuer);      //debug
    return 0;
}





int get_length(long long cc_num) {
    int length = 0;         //counter for number of digits in cc_num

    //find number of digits in cc_num
    while (cc_num > 0) {
        cc_num /= 10;       //remove one place from cc_num and count it
        length++;
    }
//    printf("Length: %i\n",length);    //debug

    //error checking to see if length is 13, 15, or 16 digits, invalid if not
    if (length != 13 && length != 15 && length != 16) {
        //length is not valid, execute invalid() and exit
        invalid();
    }
    //if length is valid this will execute
    return length;
}





int check_digits(long long cc_num, int length) {
    int issuer = 0;         //variable for issuer return

//check first one or two numbers in cc_num and determine issuer

    //check VISA, first digit 4
    if (length == 13) {
//        printf("Checking 13 digit VISA\n");           //debug
        if (cc_num / 1000000000000 == 4) {
            //valid VISA so far...
//            printf("valid 13 digit visa\n");          //debug
            issuer = 1;
        } else {
//            printf("invalid 13 digit visa\n");        //debug
            invalid();
        }
    }

    //check AMEX, first two are 34 or 37
    if (length == 15) {
//        printf("Checking 15 digit AMEX\n");           //debug
        if (cc_num / 10000000000000 == 34) {
            //valid AMEX so far...
//            printf("valid 15 digit amex\n");          //debug
            issuer = 2;
        } else if (cc_num / 10000000000000 == 37) {
            //valid AMEX so far...
//            printf("valid 15 digit amex\n");          //debug
            issuer = 2;
        } else {
//            printf("invalid 15 digit amex\n");        //debug
            invalid();
        }
    }

    //check MC and VISA
    if (length == 16) {
//        printf("Checking 16 digit MC or VISA\n");     //debug
        //check VISA, first digit 4
        if (cc_num / 1000000000000000 == 4) {
            //valid VISA so far...
//            printf("valid 16 digit visa\n");          //debug
            issuer = 1;
        } else {
            //check MC, first two are 51 - 55
            cc_num /= 100000000000000;       //two digits
            switch (cc_num) {
                case 51 :
                case 52 :
                case 53 :
                case 54 :
                case 55 :
                    //valid MC so far...
//                    printf("valid MC\n");             //debug
                    issuer = 3;
                    break;
                default :
//                    printf("invalid MC\n");           //debug
                    invalid();
            }
        }
    }
    return issuer;
}





/* Luhn's Algorithm:
** 1)   Get every other digit (starting with second to last) and multiply each one by two
** 1.2) Add the individual digits of every sum together
**
** 2)   Add non-doubled digits together
**
** 3)   [step 1 product] + [step 2 product] % 10 should be 0
*/

bool check_checksum(long long cc_num, int length) {
    long long tmp_cc = cc_num;                          //make temp variable to store cc_num
    int step_one_total = 0;                             //make variable for step one running total
    int step_two_total = 0;                             //make variable for step two running total
    int step_three_total = 0;                           //make variable for step three total
    int step_one_counter = length / 2;                  //make counter for first while loop
    int step_two_counter = length - step_one_counter;   //make counter for second while loop
    int current_digit = 0;                              //make variable to hold digit while working with it

    //step one
    while (step_one_counter > 0) {
        tmp_cc /= 10;                       //skip one digit
        current_digit = tmp_cc % 10;        //get last digit
        current_digit *= 2;                 //multiply digit by 2

        //if digit is multiple digits, separate and add to total, cannot be greater than 18
        if (current_digit >= 10) {
            step_one_total += current_digit % 10;   //add first number
            current_digit /= 10;                    //remove first number
            step_one_total += current_digit;
        } else {
            //current digit is only 1 number
            step_one_total += current_digit;
        }
        tmp_cc /= 10;                          //done working with number, remove it
        step_one_counter--;                    //decrement counter
    }

    //step two
    //get fresh tmp_cc to work with and reset counter
    tmp_cc = cc_num;

    //get every other number (starting with the last) and add them to step_two_total
    while (step_two_counter > 0) {
        current_digit = tmp_cc %10;         //get last digit
        step_two_total += current_digit;    //add digit to running total
//        printf("Current digit: %i\n",current_digit);      //debug
//        printf("Current total: %i\n\n",step_two_total);   //debug
        tmp_cc /= 100;                       //done working with number, remove it. Skip one number
        step_two_counter--;
    }

    //step three
    step_three_total = step_one_total + step_two_total; //add sums from other steps together
//    printf("Step one: %i\n",step_one_total);              //debug
//    printf("Step two: %i\n",step_two_total);              //debug
//    printf("Step three: %i\n",step_three_total);          //debug

    if (step_three_total % 10 == 0) {
        return true;
    } else {
        return false;
    }

}





int invalid(void) {
    printf("INVALID\n");
    exit(0);
}

