#include <stdio.h>
#include <stdlib.h>

int main(void){
	
	char* minutestring = malloc(20);
	printf("How long do you shower (in minutes)?");
	fgets(minutestring, 20, stdin);
	
	int minutes = atoi(minutestring);
	
	printf("Minutes: %d\n", minutes);
	int bottles = minutes * 12;
	printf("Bottles: %d\n", bottles);
 	
	return 0;
}
