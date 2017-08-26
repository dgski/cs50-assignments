/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include "helpers.h"
#include <string.h>
#include <stdio.h>


//Implementation of binary search
bool bin_search(int value, int values[], int s, int e)
{   
    
    //Array size 0 - Not in list
    if(e <= s)
        return false;
    
    //Find midpoint
    int m = s + ((e - s + 1) / 2);

    //This is our value
    if(values[m] == value)
        return true;
    
    //Value is bigger
    if(values[m] < value)
        return bin_search(value, values, m + 1 , e);

    //Value is smaller
    if(values[m] > value)
        return bin_search(value, values, s, m - 1);
    
    return false;
}



/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{    
    if(n < 0)
        return false;
    
    return bin_search(value, values, 0, n - 1);
}

/**
 * Sorts array of n values.
 *Counting sort implementation
*/
void sort(int values[], int n)
{
    //Index array
    int keys[65536];
    memset(keys, 0, sizeof(keys));
    
    //Tally values in index array
    for(int i = 0; i < n; i++)
    {
        if(values[i] == 0)
            keys[values[i]] = 1;
        else
            keys[values[i]] += 1;
    }
  
    //Place values back into values array
    int c = 0;
    for(int i = 0; i < 65536; i++)
    {
        for(int y = 0; y < keys[i]; y++)
        {
            values[c++] = i;
        }
    }

    return;
}
