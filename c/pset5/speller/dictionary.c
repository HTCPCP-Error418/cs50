// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include <stdio.h>

#include "dictionary.h"

//global variables
t_node *root = NULL;                    //root node of trie
int word_counter = 0;                   //counter for number of words loaded


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //create traversal pointer
    t_node *trav = root;

    //traverse through the trie until trav is null or the end of the word is reached
    for (int i = 0; i < strlen(word); i++) {
        int letter = 0;                                 //variable for next index to go to

        if (isalpha(word[i])) {
            letter = tolower(word[i]) - 97;         //make all letters lowercase (case insensitive)
        } else {
            //it's an apostrophe (index 26)
            letter = 26;
        }
        trav = trav->next[letter];
        if (!trav) {
            //if there is no next node, word is not in dictionary
            return false;
        }
    }

    //at the end of the word, check the EOW flag
    if (trav->EOW) {
        //EOW flag is true, word is in dictionary
        return true;
    }

    //EOW flag is false at the end of the word, word is not in dictionary
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary and check pointer
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL) {
        fprintf(stderr, "Error loading dictionary: %s\n",dictionary);
        return false;
    }

    //setup root node of trie
    root = new_node();

    //--- GET WORD ---
    //make array and index counter for word
    char word[LENGTH + 1];                  //account for longest word + '\0'
    int index = 0;

    //get word one character at a time (to remove \n and keep any special characters)
    for (int c = fgetc(inptr); c != EOF; c = fgetc(inptr)) {
        if (isalpha(c) || (c == '\'' && index > 0)) {
            //is alpha or apostrophe
            word[index] = c;
            index++;

            //make sure word isn't too long
            if (index > LENGTH) {
                //get the rest of the word and reset the index (effectively skip the word)
                while ((c = fgetc(inptr)) != EOF && isalpha(c));
                index = 0;
            }
        } else if (isdigit(c)) {
            //skip words with numbers
            while ((c = fgetc(inptr)) != EOF && isalnum(c));
            index = 0;
        } else if (index > 0) {
            //not a letter, apostrophe, or number, but at the end of a word, must be space
            word[index] = '\0';             //found whole word, terminate

            //--- LOAD WORD ---
            //make traversal pointer
            t_node *trav = root;

            //load word into trie, making nodes where needed
            for (int counter = 0; counter < strlen(word); counter++) {
                int letter = 0;                                         //variable for next index to go to

                if (isalpha(word[counter])) {
                    letter = ((int) word[counter]) - 97;            //get index of letter ('a' == 0)
                } else {
                    //it's an apostrophe (index 27)
                    letter = 26;
                }

                //if next node doesn't exist, make it
                if (trav->next[letter] == NULL) {
                    trav->next[letter] = new_node();
                }

                //move traversal pointer to the next word
                trav = trav->next[letter];
            }

            //at the end of the word, set EOW flag on node
            trav->EOW = true;

            //increment word counter
            word_counter++;

            //reset index for word array
            index = 0;
        }
    }

    //close dictionary
    fclose(inptr);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    del_node(root);

    return true;
}

//return a pointer to a new trie node
//  Set EOW to false, next pointers to NULL and prev pointer to previous node
t_node *new_node(void) {
    t_node *node = malloc(sizeof(t_node));
    if (node == NULL) {
        mem_err();
    }

    for(int i = 0; i < 27; i++) {
        node->next[i] = NULL;
    }

    node->EOW = false;

    return node;
}

//delete nodes recursively?
void del_node(t_node *node) {
    //recursive case
    for (int i = 0; i < 27; i++) {
        if (node->next[i]) {
            del_node(node->next[i]);
        }
    }

    //base case
    free(node);
}


//if there is an error allocating memory, exit with a code of 1
void mem_err(void) {
    fprintf(stderr, "Error allocating memory\n");
    exit(1);
}