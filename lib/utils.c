#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 10

// Function to trim a sentence into an array of strings
char** trimSentence(const char* sentence, int* wordCount) {
    char** words = (char**)malloc(MAX_WORDS * sizeof(char*));

    if (words == NULL) {
        fprintf(stderr, "Memory allocation for array of words failed!\n");
        exit(1);
    }

    // Initialize word count
    *wordCount = 0;

    // Tokenize the sentence
    char* token = strtok((char*)sentence, " ");
    
    while (token != NULL) {
        // Allocate memory for each word
        words[*wordCount] = (char*)malloc((strlen(token) + 1) * sizeof(char));

        if (words[*wordCount] == NULL) {
            fprintf(stderr, "Memory allocation for word failed\n");
            exit(1);
        }

        // Copy the word into the array
        strcpy(words[*wordCount], token);

        (*wordCount)++;

        // Get the next token
        token = strtok(NULL, " ");
    }

    return words;
}

