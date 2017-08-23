/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
        //Return false if n is non-positive
        if(n < 0)
            return false;

        //Get index of center of array
    	int middle = n / 2;   
    	
        if(values[middle] == value)
            return true;

        if(values[middle] > value)
            return false; //top half of array

        if(values[middle] < value)
            return false; //botton half of array
        
      
        return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    return;
}
