#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int ImpUse(void){
	printf("Usage: ./vigenere k\n");
	return 1;	
}


int GetCipher(char key){
	
	if(key >= 'A' && key <= 'Z')
		key = key - 65;
	else
		key = key - 97;

	return key;	
}

char Encrypt(char character, int key){
	

	
	if(character < 91 && character + key > 'Z') //If Uppercase and wrapping
		key -= 26;	

	if(character > 96 && character + key > 'z') //If Lowercase and wrapping
		key -= 26;
	
	
	return character + key;



}



int main(int argc, char* argv[]){

	//Improper amount of command line arguments
	if(argc != 2)
	{
		return ImpUse();
	}

	//Argument contains digits	
	for(int i = 0; i < strlen(argv[1]); i++)
		if(isdigit(argv[1][i]))
			return ImpUse();
	
	//Set Key
	char* key = argv[1];
	
	//Get message
	printf("plaintext: ");
	char* plaintext = malloc(500);
	fgets(plaintext, 500, stdin);
	
	//Prepare encrypted message
	char* ciphertext = malloc(500);
	
	



	int x = 0; //cipher key
	for(int i = 0; i < strlen(plaintext) -1; i++)
	{
		
			
		// Not a letter	
		if(plaintext[i] < 'A' ||  (plaintext[i] > 'Z' && plaintext[i] < 'a') || plaintext[i] > 'z')
		{
			ciphertext[i] = plaintext[i];
		}

		else
		{
			
			//Encrypt
			int ciphernumber = GetCipher(key[x]);
			ciphertext[i] = Encrypt(plaintext[i], ciphernumber);
			x++;			
			//Wrap around key	
			if(x == strlen(key))
				x = 0;
		}	
	}



	printf("\nciphertext: %s\n", ciphertext);
		
	free(plaintext);
	free(ciphertext);
	return 0;
}
