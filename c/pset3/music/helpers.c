// Helper functions for music

#include <cs50.h>
#include <string.h>     //strlen()
#include <math.h>       //pow(), round()

#include <stdio.h>      //TESTING

#include "helpers.h"


// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int num = (int) fraction[0] - 48;         //variable for numerator (0 = 48 in ASCII)
    int den = (int) fraction[2] - 48;         //variable for denominator (0 = 48 in ASCII)
    int mult;                                 //variable for numerator multiplier

    mult = 8 / den;

//    printf("[DURATION] Fraction: %s\n",fraction);
//    printf("\tNum: %i\n",num);
//    printf("\tDen: %i\n",den);
//    printf("\tMult: %i\n",mult);
//    printf("\tEighths: %i\n",mult * num);


    return mult * num;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    char tone = note[0];            //variable for letter from string
    char accidental = '\0';         //variable for accidental from string
    int octave = 0;                 //variable for octave from string (4 = 52 in ASCII)
    float semitones = 0;            //variable for number of semitones from A4

    //set semitones for each letter (I can't figure out how to do it mathematically)
    switch (tone) {
        case 'A' :
            semitones = 0;
            break;
        case 'B' :
            semitones = 2;
            break;
        case 'C' :
            semitones = -9;
            break;
        case 'D' :
            semitones = -7;
            break;
        case 'E' :
            semitones = -5;
            break;
        case 'F' :
            semitones = -4;
            break;
        case 'G' :
            semitones = -2;
    }

    //calculate octaves and adjust for #/b
    if (strlen(note) == 3) {            //adjust for accidentals (format is "A#4")
        accidental = note[1];
        if (accidental == '#') {
            semitones++;                //adjust for sharp
        } else {
            semitones--;                //adjust for flat
        }
        octave = (int) note[2] - 52;    //adjust for octaves
        semitones += octave * 12;
    } else {
        octave = (int) note[1] - 52;    //adjust for octaves
        semitones += octave * 12;
    }


//    printf("\n[FREQUENCY] Note: %s\n",note);
//    printf("\tTone: %c\n",tone);
//    printf("\tAccidental: %c\n",accidental);
//    printf("\tSemitones: %f\n",semitones);
//    printf("\tOctave: %i\n",octave);
//    printf("\tPower: %f\n",(semitones / 12));


    return round(pow(2, (semitones / 12)) * 440);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (strcmp(s, "") == 0) {
        return true;
    } else {
        return false;
    }
}
