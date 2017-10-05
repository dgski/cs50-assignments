#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"


typedef struct node 
{
    char value[LENGTH + 2];;
    struct node* next;
} node;

node* create(char* input_string)
{
    node* temp = malloc(sizeof(node));
    memset(temp->value, '0', sizeof(temp->value));
    strcpy(temp->value, input_string);
    temp->next = NULL;
    return temp;
}

node* prepend(node* head, char* input_value)
{
    node* temp = create(input_value);;
    temp->next = head;
    head = temp;
    return head;
}

node* append(node* head, char* input_value)
{
    node* cursor = head;

    while(cursor->next != NULL)
        cursor = cursor->next;

    node* temp = create(input_value);
    cursor->next = temp;
    
    return head;
}

node* remove_front(node* head)
{
    node* cursor = head;
    head = head->next;
    free(cursor);
    return head;
}

node* remove_end(node* head)
{
    node* cursor = head;
    node* back = NULL;    
    
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }

    if(back != NULL)
        back->next = NULL;

    free(cursor);
    
    return head;
}

void print_list(node* head)
{
    node* temp = head;

    while(temp != NULL)
    {
        printf("%s ",temp->value);
        temp = temp->next;
    }
    printf("\n");

}

void discard_list(node* head)
{
    //Single element sized list
    if(head->next == NULL)
    {
        free(head);
        return;
    }
    
    node* temp = head;
    temp = temp->next;

    while(head != NULL)
    {
        
        free(head);
        head = temp;
        if(temp != NULL)
            temp = temp->next;
    }
}

bool search_list(node* head, const char* input_value)
{
    
    if(!strcmp(input_value, head->value))
        return true;

	if(head->next != NULL)
		return search_list(head->next, input_value);

	return false;

}

