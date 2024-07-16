#include <stdint.h>
#include <stddef.h>
#include "coap_option.h"

#ifndef COAP_HEADER_HEADER
#define COAP_HEADER_HEADER
#define MAX_HEADER_LENGTH 256
#define COAP_DEFAULT_TKL 8
// 0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |Ver| T |  TKL  |      Code     |          Message ID           |
#define COAP_VERSION 0x01

typedef enum coap_msg_type_e
{
    COAP_MSG_TYPE_CONFIRMABLE = 0,     /**< Reliable Request messages */
    COAP_MSG_TYPE_NON_CONFIRMABLE = 1, /**< Non-reliable Request and Response messages */
    COAP_MSG_TYPE_ACKNOWLEDGEMENT = 2, /**< Response to a Confirmable Request  */
    COAP_MSG_TYPE_RESET = 3            /**< Answer a Bad Request */
} coap_msg_type_e;

typedef enum coap_msg_code_e
{
    COAP_MSG_CODE_EMPTY = 0,
    COAP_MSG_CODE_REQUEST_GET = 1,
    COAP_MSG_CODE_REQUEST_POST = 2,
    COAP_MSG_CODE_REQUEST_PUT = 3,
    COAP_MSG_CODE_REQUEST_DELETE = 4,

    COAP_MSG_CODE_RESPONSE_CREATED = 65,
    COAP_MSG_CODE_RESPONSE_DELETED = 66,
    COAP_MSG_CODE_RESPONSE_VALID = 67,
    COAP_MSG_CODE_RESPONSE_CHANGED = 68,
    COAP_MSG_CODE_RESPONSE_CONTENT = 69,
    COAP_MSG_CODE_RESPONSE_CONTINUE = 95,
    COAP_MSG_CODE_RESPONSE_BAD_REQUEST = 128,
    COAP_MSG_CODE_RESPONSE_UNAUTHORIZED = 129,
    COAP_MSG_CODE_RESPONSE_BAD_OPTION = 130,
    COAP_MSG_CODE_RESPONSE_FORBIDDEN = 131,
    COAP_MSG_CODE_RESPONSE_NOT_FOUND = 132,
    COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED = 133,
    COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE = 134,
    COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_INCOMPLETE = 136,
    COAP_MSG_CODE_RESPONSE_PRECONDITION_FAILED = 140,
    COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE = 141,
    COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT = 143,
    COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR = 160,
    COAP_MSG_CODE_RESPONSE_NOT_IMPLEMENTED = 161,
    COAP_MSG_CODE_RESPONSE_BAD_GATEWAY = 162,
    COAP_MSG_CODE_RESPONSE_SERVICE_UNAVAILABLE = 163,
    COAP_MSG_CODE_RESPONSE_GATEWAY_TIMEOUT = 164,
    COAP_MSG_CODE_RESPONSE_PROXYING_NOT_SUPPORTED = 165
} coap_msg_code_e;

typedef uint64_t coap_token;

typedef struct
{
    // 1st byte
    uint8_t coap_version;
    coap_msg_type_e coap_type;
    uint8_t token_length;
    // 2nd byte
    coap_msg_code_e coap_code;
    // 3rd-4th byte
    uint16_t coap_msg_id;
    coap_token coap_token_;
    COAP_OPTION_LIST * coap_option_list;
} COAP_HEADER_PACKET;

void coap_header_pack(COAP_HEADER_PACKET *packet, coap_msg_type_e type,
                      uint8_t token_length, coap_msg_code_e code,
                      uint16_t msg_id, coap_token token,
                      COAP_OPTION_LIST *option_list);

size_t coap_header_to_bytes(const COAP_HEADER_PACKET *packet, uint8_t *result);
void coap_header_from_bytes(COAP_HEADER_PACKET *packet, uint8_t *data);

#endif
