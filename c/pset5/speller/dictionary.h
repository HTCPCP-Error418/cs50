// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

//create structure for trie nodes
typedef struct _t_node {
    struct _t_node *next[27];           //next will be a letter or apostrophe (hence 27)
    bool EOW;                           //flag for the end of a word
} t_node;

// Prototypes
bool check(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);

void mem_err(void);
t_node *new_node(void);
void del_node(t_node *node);

#endif // DICTIONARY_H
