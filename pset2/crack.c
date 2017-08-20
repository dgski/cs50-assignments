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
	



	/*
	printf("1 CHARACTER\n");
	for(int a = 'A'; a < 91; a++)
	{

		pass[0] = a;	
		printf("%s\n", pass);


	}
	

	printf("2 CHARACTERS\n");
	for(int a = 'A'; a < 91; a++)
	{
		for(int b = 'A'; b < 91; b++)
		{

			pass[0] = a;	
			pass[1] = b;

			printf("%s\n", pass);


		}
	}


	printf("3 CHARACTERS\n");
	for(int a = 'A'; a < 91; a++)
	{
		for(int b = 'A'; b < 91; b++)
		{
			for(int c = 'A'; c < 91; c++)
			{

				pass[0] = a;	
				pass[2] = b;
				pass[1] = c;	

				printf("%s\n", pass);


			}
		}
	}
	

	printf("4 CHARACTERS\n");
	for(int a = 'A'; a < 91; a++)
	{
		for(int b = 'A'; b < 91; b++)
		{
			for(int c = 'A'; c < 91; c++)
			{
				for(int d = 'A'; d < 91; d++)
				{

					pass[0] = a;	
					pass[3] = b;
					pass[2] = c;	
					pass[1]=  d;

					printf("%s\n", pass);


				}
			}
		}
	}
	*/

	
	printf("4 CHARACTERS\n");
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

					printf("%s\n", pass);
					
					printf("G.HASH: %s\n", hash);
					printf("U.HASH: %s\n", crypt(pass,salt));
									
					//If given hash matches generated hash
					if (hash == crypt(pass,salt))
					{
						printf("YOUR PASSWORD!");
						return 0;
					}
	



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
