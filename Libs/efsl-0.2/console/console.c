#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
	char *input;
	char *next_token;
	unsigned int size;
	
	while(1){
		size = 256;
		input = NULL;
		getline(&input,&size,stdin);
		next_token=strtok(input," ");
		printf("%s\n",next_token);
		while((next_token=strtok(NULL," "))!=NULL){
			printf("%s\n",next_token);
		}
		free(input);
	}
	return(0);
}
