#include <stddef.h>
#include <stdint.h>
#ifndef COAP_OPTION_HEADER
#define COAP_OPTION_HEADER

typedef enum coap_option_number_e {
    COAP_OPTION_IF_MATCH        = 1,
    COAP_OPTION_URI_HOST        = 3,
    COAP_OPTION_ETAG            = 4,
    COAP_OPTION_IF_NONE_MATCH   = 5,
    COAP_OPTION_OBSERVE         = 6,
    COAP_OPTION_URI_PORT        = 7,
    COAP_OPTION_LOCATION_PATH   = 8,
    COAP_OPTION_URI_PATH        = 11,
    COAP_OPTION_CONTENT_FORMAT  = 12,
    COAP_OPTION_MAX_AGE         = 14,
    COAP_OPTION_URI_QUERY       = 15,
    COAP_OPTION_ACCEPT          = 17,
    COAP_OPTION_LOCATION_QUERY  = 20,
    COAP_OPTION_BLOCK2          = 23,
    COAP_OPTION_BLOCK1          = 27,
    COAP_OPTION_SIZE2           = 28,
    COAP_OPTION_PROXY_URI       = 35,
    COAP_OPTION_PROXY_SCHEME    = 39,
    COAP_OPTION_SIZE1           = 60,
} coap_option_number_e;

typedef enum coap_content_format_e {
    //default is Charset-utf8
    COAP_CT_TEXT_PLAIN          = 0,
    COAP_CT_LINK_FORMAT         = 40, // application/link-format
    COAP_CT_XML                 = 41, // application/xml
    COAP_CT_OCTET_STREAM        = 42, // application/octet-stream
    COAP_CT_EXI                 = 47, // application/exi
    COAP_CT_JSON                = 50, // application/json
} coap_content_format_e;

#define MAX_OPTION_VALUE_LENGTH 269  // Giới hạn độ dài tối đa của giá trị tùy chọn, có thể thay đổi 13, 269, lớn hơn

typedef enum {
    COAP_OPTION_EMPTY,
    COAP_OPTION_OPAQUE,
    COAP_OPTION_UINT,
    COAP_OPTION_STRING
} coap_option_type_t;

typedef struct {
    uint16_t coap_option_number;  // Số tùy chọn
    coap_option_type_t coap_option_type;  // Loại dữ liệu của tùy chọn
    uint8_t coap_option_length;   // Độ dài của giá trị tùy chọn
    union {
        uint8_t opaque[MAX_OPTION_VALUE_LENGTH];
        uint32_t uint_value;
        char string[MAX_OPTION_VALUE_LENGTH];
    } coap_option_value;
} COAP_OPTION;

void set_coap_option_empty(COAP_OPTION *option, uint16_t number);
void set_coap_option_opaque(COAP_OPTION *option, uint16_t number, const uint8_t *value, uint8_t length);
void set_coap_option_uint(COAP_OPTION *option, uint16_t number, uint32_t value);
void set_coap_option_string(COAP_OPTION *option, uint16_t number, const char *value);

#define MAX_OPTION_QUANTITY 20

typedef struct
{
    COAP_OPTION list[MAX_OPTION_QUANTITY];
    int option_count;
} COAP_OPTION_LIST;

size_t coap_option_list_to_bytes (COAP_OPTION_LIST *list_op, uint8_t *ouput);
void coap_option_list_from_bytes (COAP_OPTION_LIST *list, uint8_t *input);
/**Return option count after insert*/
int insert_coap_option_to_list(COAP_OPTION_LIST *list, COAP_OPTION *option);

#define MAX_OPTION_LIST 16
#define MAX_OPTION_LENGTH 256

#endif