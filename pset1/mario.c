#include <stdio.h>
#include <stdlib.h>

void printline(int size,int row);


int main(void){

	char* s_size = malloc(8);
	int i_size;
	
	printf("Please enter the size of the mario pyramid (max 23):");
	while(1)
	{
		fgets(s_size, 8, stdin);
		i_size = atoi(s_size);

		if(i_size < 23 || i_size > 0)
			break;

		printf("Please enter a valid number:");
	}

	for(int row = 1; row <= i_size; row++)
		printline(i_size,row);

	return 0;

}

void printline(int size, int row){

	int out_o = size - row;

	for(int i = 0; i < out_o; i++)
		printf(" ");
	
	for(int i = 0; i < row; i++)
		printf("X");
	
	printf("  ");

	for(int i = 0; i < row; i++)
		printf("X");
	
	for (int i = 0; i< out_o; i++)
		printf(" ");

	printf("\n");
}

