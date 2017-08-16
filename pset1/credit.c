#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function Determines Card Type
char*  CardType(int length, char* input){

	//Visa
	if((length == 16 || length == 13) && input[0] == '4')
		return "VISA\n";
	//Mastercard
	else if(length == 16)
		return "MASTERCARD\n";
	//AMEX
	else if(length == 15)
		return "AMEX\n";
	//INVALID
	else
		return "INVALID\n";

}


//Main Function
int main(void){
	

	//Variable declaration
	char* input = malloc(20);
	int length;
	char* type = malloc(10);	


	//User number input
	printf("Please enter your credit card number (Research Purposes, we swear ;):");
	while(1)
	{
		fgets(input, 20, stdin);
		length = strlen(input) - 1;

		if(length == 13 || length == 15 || length == 16)
			break;

		printf("Please enter a valid credit card number:");

	}

	//Determine type of card
	//TEMPORARY -DOES NOT DETERMINE
	type = CardType(length, input);
	printf("Type: %s",type);


	//Convert into longlong
	long long number = atoll(input);

	

	//Determine validity
	printf("Determining validity!\n");


	//Sum of start at second last one times 2
	long long sum1 = 0;

	for(int i = length - 2; i>=0; i = i - 2)
	{
		sum1 += (input[i] - '0') * 2;	
		printf("%c ", input[i]);
	}
	printf("\nSum1: %lld\n", sum1);

	//Sum of starting at last one
	long long  sum2 = 0;
	for(int i = length - 1; i >= 0; i = i - 2)
	{	
		printf("%c ",input[i]);	
		sum2 += (input[i] -  '0');
	}
	printf("\nSum2: %lld\n", sum2);


	long long totalsum = sum1 + sum2;
	printf("TOTALSUM: %lld\n", totalsum);
	if(totalsum % 10 == 0)
		printf("VALID CARD");
	else
		printf("NOPE");


	printf("Long Long: %lld\n", number);
	printf("%s", input);
	printf("%d",length);
	printf("\n");

}
