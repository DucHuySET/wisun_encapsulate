#ifndef SPINEL_DATATYPE_PACK_H
#define SPINEL_DATATYPE_PACK_H

#ifndef NULL
#define NULL 0L
#endif

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

typedef int spinel_ssize_t;
typedef unsigned int spinel_size_t;
typedef uint8_t spinel_tid_t;
typedef bool *spinel_bool_t;

typedef struct
{
    uint8_t bytes[8];
} spinel_eui64_t;

typedef struct
{
    uint8_t bytes[8];
} spinel_net_xpanid_t;

typedef struct
{
    uint8_t bytes[16];
} spinel_net_pskc_t;

typedef struct
{
    uint8_t bytes[6];
} spinel_eui48_t;

typedef struct
{
    uint8_t bytes[16];
} spinel_ipv6addr_t;

typedef struct
{
    va_list obj;
} va_list_obj;

#define SPINEL_MAX_PACK_LENGTH 32767

typedef char spinel_datatype_t;

enum
{
    SPINEL_DATATYPE_NULL_C = 0,
    SPINEL_DATATYPE_VOID_C = '.',
    SPINEL_DATATYPE_BOOL_C = 'b',
    SPINEL_DATATYPE_UINT8_C = 'C',
    SPINEL_DATATYPE_INT8_C = 'c',
    SPINEL_DATATYPE_UINT16_C = 'S',
    SPINEL_DATATYPE_INT16_C = 's',
    SPINEL_DATATYPE_UINT32_C = 'L',
    SPINEL_DATATYPE_INT32_C = 'l',
    SPINEL_DATATYPE_UINT64_C = 'X',
    SPINEL_DATATYPE_INT64_C = 'x',
    SPINEL_DATATYPE_UINT_PACKED_C = 'i',
    SPINEL_DATATYPE_IPv6ADDR_C = '6',
    SPINEL_DATATYPE_EUI64_C = 'E',
    SPINEL_DATATYPE_EUI48_C = 'e',
    SPINEL_DATATYPE_DATA_WLEN_C = 'd',
    SPINEL_DATATYPE_DATA_C = 'D',
    SPINEL_DATATYPE_UTF8_C = 'U', //!< Zero-Terminated UTF8-Encoded String
    SPINEL_DATATYPE_STRUCT_C = 't',
    SPINEL_DATATYPE_ARRAY_C = 'A',
};

#define SPINEL_DATATYPE_NULL_S ""
#define SPINEL_DATATYPE_VOID_S "."
#define SPINEL_DATATYPE_BOOL_S "b"
#define SPINEL_DATATYPE_UINT8_S "C"
#define SPINEL_DATATYPE_INT8_S "c"
#define SPINEL_DATATYPE_UINT16_S "S"
#define SPINEL_DATATYPE_INT16_S "s"
#define SPINEL_DATATYPE_UINT32_S "L"
#define SPINEL_DATATYPE_INT32_S "l"
#define SPINEL_DATATYPE_UINT64_S "X"
#define SPINEL_DATATYPE_INT64_S "x"
#define SPINEL_DATATYPE_UINT_PACKED_S "i"
#define SPINEL_DATATYPE_IPv6ADDR_S "6"
#define SPINEL_DATATYPE_EUI64_S "E"
#define SPINEL_DATATYPE_EUI48_S "e"
#define SPINEL_DATATYPE_DATA_WLEN_S "d"
#define SPINEL_DATATYPE_DATA_S "D"
#define SPINEL_DATATYPE_UTF8_S "U"

spinel_ssize_t spinel_packed_uint_decode(const uint8_t *bytes, spinel_size_t len, unsigned int *value_ptr);
spinel_ssize_t spinel_packed_uint_size(unsigned int value);
spinel_ssize_t spinel_packed_uint_encode(uint8_t *bytes, spinel_size_t len, unsigned int value);
const char *spinel_next_packed_datatype(const char *pack_format);
static spinel_ssize_t spinel_datatype_vunpack_(bool in_place,
                                               const uint8_t *data_in,
                                               spinel_size_t data_len,
                                               const char *pack_format,
                                               va_list_obj *args);
spinel_ssize_t spinel_datatype_unpack_in_place(const uint8_t *data_in,
                                               spinel_size_t data_len,
                                               const char *pack_format,
                                               ...);
spinel_ssize_t spinel_datatype_unpack(const uint8_t *data_in, spinel_size_t data_len, const char *pack_format, ...);
spinel_ssize_t spinel_datatype_vunpack_in_place(const uint8_t *data_in,
                                                spinel_size_t data_len,
                                                const char *pack_format,
                                                va_list args);
spinel_ssize_t spinel_datatype_vunpack(const uint8_t *data_in, spinel_size_t data_len, const char *pack_format, va_list args);
static spinel_ssize_t spinel_datatype_vpack_(uint8_t *data_out,
                                             spinel_size_t data_len_max,
                                             const char *pack_format,
                                             va_list_obj *args);
spinel_ssize_t spinel_datatype_pack(uint8_t *data_out, spinel_size_t data_len_max, const char *pack_format, ...);
spinel_ssize_t spinel_datatype_vpack(uint8_t *data_out,
                                     spinel_size_t data_len_max,
                                     const char *pack_format,
                                     va_list args);

#ifndef strnlen
static size_t spinel_strnlen(const char *s, size_t maxlen)
{
    size_t ret;

    for (ret = 0; (ret < maxlen) && (s[ret] != 0); ret++)
    {
        // Empty loop.
    }

    return ret;
}
#else
#define spinel_strnlen strnlen
#endif

#endif