/*----import section----*/
#include<stdio.h>
#include<string.h>

/*----define section*/
#define MAX_INPUT 100


/*----main----*/
void main(){
    while (1){
        char inputStr[MAX_INPUT];
        scanf("%s", &inputStr);
        int lenCount = 0;
        char tmp[] = "qwerty";
        lenCount = strlen(inputStr);
        printf("%d\n", &lenCount);
    }
}