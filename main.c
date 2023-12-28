/*----import section----*/
#include<stdio.h>
#include<string.h>

#include "lib/utils.h"

/*----define section*/
#define MAX_INPUT 100
#define MALLOC_TRIM 10


/*----main----*/
void main(){
    while (1){
        /*---get input section---*/
        char inputStr[MAX_INPUT];
        fgets(inputStr, sizeof(inputStr), stdin);
        size_t len = strlen(inputStr);
        if (len > 0 && inputStr[len - 1] == '\n') {
            inputStr[len - 1] = '\0'; // Remove the newline character
            len--;
        }

        /*---trim setion---*/
        int wordCount = 0;
        char** cmd_array = trimSentence(inputStr, &wordCount);

        int i = 0;
        for(i; i<wordCount; i++){
            printf("%s\n", cmd_array[i]);
        }
        
    }
}