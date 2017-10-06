#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node
{
	bool is_word;
	struct node* letters[27];
} node;

node* create()
{
	node* temp = malloc(sizeof(node));
	temp->is_word = false;
	for(int i = 0; i < 27; i++)
		temp->letters[i] = NULL;

	return temp;
}


void flag_is_word(node* element)
{
	element->is_word = true;
}


node* insert_letter(node* element, char letter)
{	
	int let_index = letter - 96;
	if(letter == 39)
		let_index = 0;

	if(element->letters[let_index] != NULL)
		return element->letters[let_index];


	node* temp = create();

	element->letters[let_index] = temp;

	return element->letters[let_index];
}


void insert_raw_letter(node* element, int index)
{	
	node* temp = create();

	element->letters[index] = temp;
}
bool search_trie(node* head, char* search_word)
{
	for (int i = 0; i <= strlen(search_word); i++)
	{

		if(i == strlen(search_word))
		{	
			if(head->is_word)
				return true;
			else
				return false;
		}

		int let_index = search_word[i]- 96;	
		if(search_word[i] == 39)
			let_index = 0;

		if(head->letters[let_index] != NULL)
		{
			head = head->letters[let_index];
			continue;
		}
		else
			return false;

	}

    return false;
}

void insert_word(node* head, char* input_word)
{
	
	for (int i = 0; i < strlen(input_word); i++)
	{	

		//Insert letter
		head = 	insert_letter(head, input_word[i]);

		
		//If last letter of word flag 
		if(i == strlen(input_word) - 1)
		{
			flag_is_word(head);
			break;
		}
	}
}


void free_trie(node* head)
{
	for(int i = 0; i < 27; i++)
	{
		if(head->letters[i] != NULL)
			free_trie(head->letters[i]);
	}

	free(head);
}
