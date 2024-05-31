/*----import section----*/
#include <stdio.h>
#include <string.h>

#include <stdint.h>

#include "../lib/hdlc/hdlc.h"

/*----define section*/
#define HDLC_MAX_FRAME_SIZE 4096
#define MAX_INPUT_SIZE 2040

#define MALLOC_TRIM 10

int token = 0;

/*----main----*/
int main()
{
    /*---get input section---*/
    // FILE *filePointer;
    // filePointer = fopen("test/hdlc_input.txt", "r");
    // if (filePointer == NULL)
    // {
    //     printf("cannot open input file\n");
    //     return 1;
    // }

    // uint8_t inputStr[MAX_INPUT_SIZE];
    uint8_t encode_pack[HDLC_MAX_FRAME_SIZE];
    uint8_t decode_pack[MAX_INPUT_SIZE];
    memset(encode_pack, 0, MAX_INPUT_SIZE);
    memset(decode_pack, 0, MAX_INPUT_SIZE);
    // while (fgets(inputStr, sizeof(inputStr), filePointer) != NULL)
    // {
    //     printf("%s\n", inputStr);
    //     hdlc_encode(inputStr, encode_pack);
    //     int i = 0;
    //     for (i = 0; i < strlen(encode_pack); i++)
    //     {
    //         printf("%02X", encode_pack[i]);
    //     }
    //     printf("\ndecode: \n");

    //     hdlc_decode(encode_pack, decode_pack, strlen(encode_pack));
    //     printf("\n%s\n", decode_pack);
    // }

    uint8_t inputStr[5] = {0x81,0x03,0x36,0x7e,0x7d};
    hdlc_encode(inputStr, encode_pack);
    int i = 0;
    for (i = 0; i < strlen(encode_pack); i++)
    {
        printf("%02X", encode_pack[i]);
    }
    printf("\ndecode: \n");
    hdlc_decode(encode_pack, decode_pack, strlen(encode_pack));
    i = 0;
    for (i = 0; i < strlen(decode_pack); i++)
    {
        printf("%02X", decode_pack[i]);
    }
    printf("\nend\n");
    return 0;
}