/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include "dictionary.h"
#include <stdlib.h>
#include <ctype.h>
#include "tries.h"
#include <string.h>
#include <stdio.h>

/**
 * Returns true if word is in dictionary else false.
 */

node* head;
int counter;
bool loaded; 

bool check(const char *word)
{
    
    char wt[LENGTH+2];
    memset(wt, 0 , sizeof(wt));

    for(int i = 0; i < strlen(word); i++)
        wt[i] = tolower(word[i]);
    
    if(search_trie(head, (char*)wt)) 
        return true;
    else
        return false;

}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //Declare variables
    char word[LENGTH+2];
    FILE* dict_file;
    dict_file = fopen(dictionary, "r");
    //Handle error
    if(dict_file == NULL)
    {
        return false;
    }

    //Create head node
    head = create();

    //Begin Load
    while(fgets(word, LENGTH + 2, dict_file))
    { 
        
        printf("Loading: %s\n", word);
        word[strlen(word) - 1] = '\0';
        insert_word(head,word);
        counter++;
    }
        //Signal success 
        loaded = true;
        return true; 
}
    

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(loaded)
        return counter;
    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    
    if(!loaded)
        return false;
 
    free_trie(head);
    
    loaded = false;
    return true;
}
