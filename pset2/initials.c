#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


int main(void){

	//Array to hold user name
	char *input = malloc(50);
	
	//Get user input
	fgets(input, 50, stdin);

	//Array to hold initials
	char *initials = malloc(10);
	int x = 0;

	//For loop iterated over string
	for(int i = 0; i < strlen(input); i++)
	{
		
		//If space then continue to next letter
		if(input[i] == ' ')
		{
			continue;
		}

		//If first character or last character is space then add to array
		if(i == 0 || input[i - 1] == ' ')
		{
			initials[x] = input[i];
			x++;
		}
	}

	//Print initials
	printf("%s\n", initials);

	//Free memory
	free(input);
	free(initials);

	
	return 0;

}
