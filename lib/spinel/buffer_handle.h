
#ifndef BUFFER_HANDLE_H
#define BUFFER_HANDLE_H

#include <stdint.h>
#include <stddef.h>

void writeByteArrayToBuffer(uint8_t *buffer, uint8_t *value, size_t size);
size_t spinel_strnlen(const char *s, size_t maxlen);

#endif