/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include "dictionary.h"
#include "linkedliststring.h"
#include "hash.h" //Source: http://www.azillionmonkeys.com/qed/hash.html
#include <stdlib.h>
#include <ctype.h>

#define INDEX_SIZE 200000
/**
 * Returns true if word is in dictionary else false.
 */

node* dictionary_array[INDEX_SIZE]  = {NULL}; //Global dictionary
int counter;
bool loaded;

bool check(const char *word)
{
    
    char wt[LENGTH+2];
    memset(wt, 0 , sizeof(wt));

    for(int i = 0; i < strlen(word); i++)
        wt[i] = tolower(word[i]);

    uint32_t hashvalue = SuperFastHash(wt, strlen(wt)) % INDEX_SIZE;
    
    if(dictionary_array[hashvalue] != NULL) 
        return search_list(dictionary_array[hashvalue], wt);
    else
        return false;

}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //Declare variables
    uint32_t hashvalue;
    char word[LENGTH+2];
    FILE* dict_file;
    dict_file = fopen(dictionary, "r");
    //Handle error
    if(dict_file == NULL)
    {
        return false;
    }

    //Begin Load
    while(fgets(word, LENGTH + 2, dict_file))
    { 
        
        word[strlen(word) - 1] = '\0';
    
        //Hash
        hashvalue = SuperFastHash(word,strlen(word)) % INDEX_SIZE;       
        
        //Add to dictionary
        if(dictionary_array[hashvalue] == NULL)
            dictionary_array[hashvalue] = create(word);
        else
        {
            if(!search_list(dictionary_array[hashvalue], word))
                append(dictionary_array[hashvalue], word);
        }
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
 
    for(int i = 0; i < INDEX_SIZE; i++)
    {
        if(dictionary_array[i] == NULL)
            continue;
        else
            discard_list(dictionary_array[i]);
        
    }
    
    loaded = false;
    return true;
}
