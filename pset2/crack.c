#define _XOPEN_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char * argv[]){


	//Ensure proper usage	
	if(argc != 2)
	{
		printf("Usage: ./crack hash\n");
		return 1;
	}

	//Initiate variables
	char* hash = argv[1]; //make hash equal to given value
	char* pass = malloc(5); //Allocate memory to store password guesses
	char salt[3]; // Salt array creation
	salt[0] = hash[0]; // Take salt from has string (first two characters)
	salt[1] = hash[1];


	//Simple phrase generator - not super effiecient - doesnt skip character in between lowercase and uppercase
	for(int a = 'A'; a < 'z' + 1; a++)
	{
		for(int b = 64; b < 'z' + 1; b++)
		{
			for(int c = 64; c < 'z' + 1; c++)
			{
				for(int d = 64; d < 'z' + 1; d++)
				{
					
					pass[0] = a;	
					
					if(d == 64)
						pass[1] = (char) 0;
					else
						pass[1] = d;
					if(c == 64)
						pass[2] = (char) 0;
					else
						pass[2] = c;	
					if(b == 64)
						pass[3] = (char) 0; 
					else
						pass[3]=  b;
					
					//If given hash matches generated hash
					if (!strcmp(hash, crypt(pass,salt)))
					{
						printf("%s\n", pass);
						free(pass);
						return 0;
					}
				}
			}
		}
	}

	

	return 1;
}

