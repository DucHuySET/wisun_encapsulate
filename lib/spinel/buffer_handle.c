#include "buffer_handle.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

void writeByteArrayToBuffer(uint8_t *buffer, uint8_t *value, size_t size)
{
    int len = strlen(buffer);
    // uint8_t *cursor = buffer+strlen(buffer);
    memcpy(buffer + len, value, size);
}

size_t spinel_strnlen(const char *s, size_t maxlen)
{
    size_t ret;

    for (ret = 0; (ret < maxlen) && (s[ret] != 0); ret++)
    {
        // Empty loop.
    }

    return ret;
}