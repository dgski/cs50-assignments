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

	printf("%s\n\n", salt);




	for(int a = 'A'; a < 91; a++)
	{
		pass[3] = a;
			
		for(int b = 64; b < 91; b++) 	
		{	
			if(b == 64)
				;
			else
				pass[2] = b;



			for(int c = 64; c < 91; c++)
			{
				if(b == 64)
					;
				else	
					pass[1] = c;
				
				for(int d = 64; d < 91; d++)
				{

					if(b == 64)
						;
					else
						pass[0] = d;
					printf("%s\n",pass);

				}	
			}
		}
		
	}



	
	//If given hash matches generated hash
	if (hash == crypt(pass,salt))
	{
		printf("%s\n", pass);
	}
	
	
	printf("\n");

	//Print out cracked password
	printf("%s\n", pass);

	return 0;
}
