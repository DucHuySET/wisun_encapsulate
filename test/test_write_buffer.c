#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "../lib/spinel/buffer_handle.h"

void writeByteArrayToBufferTest(uint8_t *buffer, uint8_t *value, size_t size)
{
    int len = strlen(buffer);
    // uint8_t *cursor = buffer+strlen(buffer);
    memcpy(buffer + len, value, size);
}

int main()
{

    uint8_t buffer[100];
    memset(buffer, 0, 100);
    uint8_t header[1];
    header[0] = 0x81;
    uint8_t payload[10];

    uint8_t tid[1];
    tid[0] = 0x68;
    printf("tid: %02X\n", *tid);
    printf("strlen(tid): %d\n", strlen(tid));
    memset(payload, 0, 10);
    payload[0] = 0x02;
    // strcat(buffer, header);
    writeByteArrayToBufferTest(buffer, header, strlen(header));
    // strcat(buffer, payload);
    writeByteArrayToBufferTest(buffer, payload, strlen(payload));
    // strcat(buffer, tid);
    printf("tid: %02X\n", *tid);
    printf("strlen(tid): %d\n", strlen(tid));
    writeByteArrayToBufferTest(buffer, tid, spinel_strnlen(tid, 1));

    for (size_t i = 0; i < strlen(buffer); i++)
    {
        printf("%02X", buffer[i]);
    }

    return 0;
}