#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h> 

#include "../lib/spinel/spinel_datatype_pack.h"

// void _pack(uint8_t *buffer, const char *format, ...)
// {
//     va_list args;
//     va_start(args, format);

//     for (int i = 0; format[i] != '\0'; i++)
//     {
//         switch (format[i])
//         {
//         case 'B': // 8-bit unsigned integer (byte)
//         {
//             uint8_t value = (uint8_t)va_arg(args, int);
//             printf("%02X\n", *buffer);
//             *buffer = value;
//             buffer += sizeof(uint8_t);
//         }
//         break;
//         case 'H': // 16-bit unsigned integer (short)
//         {
//             uint16_t value = (uint16_t)va_arg(args, int);
//             *((uint16_t *)buffer) = value;
//             buffer += sizeof(uint16_t);
//         }
//         break;
//         case 'I': // 32-bit unsigned integer (int)
//         {
//             uint32_t value = va_arg(args, uint32_t);
//             *((uint32_t *)buffer) = value;
//             buffer += sizeof(uint32_t);
//         }
//         break;
//         case 'S': // String (null-terminated)
//         {
//             const char *str = va_arg(args, const char *);
//             size_t strLen = strlen(str);
//             strcpy(buffer, str);
//             buffer += strLen + 1; // Include the null terminator
//         }
//         break;
//             // Add cases for other data types as needed
//         }
//     }
//     buffer += sizeof(uint8_t);
//     *buffer = '\0';

//     va_end(args);
// }

// void _unpack(const char *buffer, const char *format, ...)
// {
//     va_list args;
//     va_start(args, format);

//     for (int i = 0; format[i] != '\0'; i++)
//     {
//         switch (format[i])
//         {
//         case 'B': // 8-bit unsigned integer (byte)
//         {
//             uint8_t *dest = va_arg(args, uint8_t *);
//             *dest = *buffer;
//             buffer += sizeof(uint8_t);
//         }
//         break;
//         case 'H': // 16-bit unsigned integer (short)
//         {
//             uint16_t *dest = va_arg(args, uint16_t *);
//             *dest = *((uint16_t *)buffer);
//             buffer += sizeof(uint16_t);
//         }
//         break;
//         case 'I': // 32-bit unsigned integer (int)
//         {
//             uint32_t *dest = va_arg(args, uint32_t *);
//             *dest = *((uint32_t *)buffer);
//             buffer += sizeof(uint32_t);
//         }
//         break;
//         case 'S': // String (null-terminated)
//         {
//             char *dest = va_arg(args, char *);
//             strcpy(dest, buffer);
//             buffer += strlen(buffer) + 1; // Move past the null terminator
//         }
//         break;
//             // Add cases for other data types as needed
//         }
//     }

//     va_end(args);
// }

int main()
{   
    const char * format = "S";
    uint8_t  input = 134;
    // Example usage
    uint8_t packedBuffer[100];
    memset(packedBuffer, 0, 100);
    // pack(packedBuffer, "S", "Hello, World! ++++");
    // _pack(packedBuffer, "B", 0x4E);
    spinel_datatype_pack(packedBuffer, 100, "L", 4294967285);

    // Unpack the data
    uint8_t unpackedByte[100];
    memset(&unpackedByte, 0, 100);

    printf("%02X\n", packedBuffer);
    // _unpack(packedBuffer, "B", unpackedByte);
    uint32_t unpackedNum;
    spinel_datatype_unpack(packedBuffer, 100, "L", &unpackedNum);

    // Access the unpacked data
    printf("Unpacked values: %lu\n", unpackedNum);
    // int i = 0;
    // for (i = 0; i < strlen(unpackedByte); i++)
    // {
    //     printf("%llu", unpackedByte[i]);
    // }

    return 0;
}
