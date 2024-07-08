#include <stdint.h>
#include <stddef.h>

#ifndef COAP_HEADER
#define COAP_HEADER

// 0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |Ver| T |  TKL  |      Code     |          Message ID           |
#define COAP_VERSION 0x01

typedef enum coap_msg_type_e {
    COAP_MSG_TYPE_CONFIRMABLE       = 0, /**< Reliable Request messages */
    COAP_MSG_TYPE_NON_CONFIRMABLE   = 1, /**< Non-reliable Request and Response messages */
    COAP_MSG_TYPE_ACKNOWLEDGEMENT   = 2, /**< Response to a Confirmable Request  */
    COAP_MSG_TYPE_RESET             = 3  /**< Answer a Bad Request */
} coap_msg_type_e;

typedef enum coap_msg_code_e {
    COAP_MSG_CODE_EMPTY                                 = 0,
    COAP_MSG_CODE_REQUEST_GET                           = 1,
    COAP_MSG_CODE_REQUEST_POST                          = 2,
    COAP_MSG_CODE_REQUEST_PUT                           = 3,
    COAP_MSG_CODE_REQUEST_DELETE                        = 4,

    COAP_MSG_CODE_RESPONSE_CREATED                      = 65,
    COAP_MSG_CODE_RESPONSE_DELETED                      = 66,
    COAP_MSG_CODE_RESPONSE_VALID                        = 67,
    COAP_MSG_CODE_RESPONSE_CHANGED                      = 68,
    COAP_MSG_CODE_RESPONSE_CONTENT                      = 69,
    COAP_MSG_CODE_RESPONSE_CONTINUE                     = 95,
    COAP_MSG_CODE_RESPONSE_BAD_REQUEST                  = 128,
    COAP_MSG_CODE_RESPONSE_UNAUTHORIZED                 = 129,
    COAP_MSG_CODE_RESPONSE_BAD_OPTION                   = 130,
    COAP_MSG_CODE_RESPONSE_FORBIDDEN                    = 131,
    COAP_MSG_CODE_RESPONSE_NOT_FOUND                    = 132,
    COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED           = 133,
    COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE               = 134,
    COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_INCOMPLETE    = 136,
    COAP_MSG_CODE_RESPONSE_PRECONDITION_FAILED          = 140,
    COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE     = 141,
    COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT   = 143,
    COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR        = 160,
    COAP_MSG_CODE_RESPONSE_NOT_IMPLEMENTED              = 161,
    COAP_MSG_CODE_RESPONSE_BAD_GATEWAY                  = 162,
    COAP_MSG_CODE_RESPONSE_SERVICE_UNAVAILABLE          = 163,
    COAP_MSG_CODE_RESPONSE_GATEWAY_TIMEOUT              = 164,
    COAP_MSG_CODE_RESPONSE_PROXYING_NOT_SUPPORTED       = 165
} coap_msg_code_e;

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

#define MAX_OPTION_LIST 16

typedef struct
{
    // 1st byte
    uint8_t coap_version;
    uint8_t coap_type;
    uint8_t token_length;
    // 2nd byte
    uint8_t coap_code;
    // 3rd-4th byte
    uint16_t coap_msg_id;
    COAP_OPTION coap_option_list[MAX_OPTION_LIST];
} COAP_HEADER_PACKET;

void coap_header_to_bytes (const COAP_HEADER_PACKET *packet, uint8_t *result);
void coap_header_from_bytes(COAP_HEADER_PACKET *packet, uint8_t *data);

typedef uint64_t coap_token;



typedef struct COAP_PACKET
{
};

#endif