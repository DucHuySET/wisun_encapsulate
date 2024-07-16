#include "coap_header.h"
#include <string.h>

void coap_header_pack(COAP_HEADER_PACKET *packet, coap_msg_type_e type,
                      uint8_t token_length, coap_msg_code_e code, uint16_t msg_id,
                      coap_token token, COAP_OPTION_LIST *option_list)
{
    packet->coap_version = COAP_VERSION;
    packet->coap_type = type;
    packet->token_length = token_length;
    packet->coap_code = code;
    packet->coap_msg_id = msg_id;
    packet->coap_token_ = token;
    packet->coap_option_list = option_list;
}

size_t coap_header_to_bytes(const COAP_HEADER_PACKET *packet, uint8_t *result)
{
    size_t size = 0;
    result[0] = (packet->coap_version << 6) | (packet->coap_type << 4) | (packet->token_length);
    result[1] = packet->coap_code;
    result[2] = (uint8_t)(packet->coap_msg_id >> 8);
    result[3] = (uint8_t)(packet->coap_msg_id & 0xFF);
    size += 4;
    /*Pack coap token 8 byte*/
    for (int i = 0; i < packet->token_length; i++)
    {
        result[4 + i] = (uint8_t)(packet->coap_token_ >> (8 * (packet->token_length - i - 1)) & 0xFF);
    }
    size += packet->token_length;
    uint8_t option_bytes[MAX_OPTION_LENGTH];
    size_t option_bytes_length = coap_option_list_to_bytes(packet->coap_option_list, option_bytes);
    memcpy(result + size, option_bytes, option_bytes_length);
    size += option_bytes_length;
    return size;
}

void coap_header_from_bytes(COAP_HEADER_PACKET *packet, uint8_t *data)
{
    packet->coap_version = (data[0] >> 6) & 0x03; // 2 bits for version
    packet->coap_type = (data[0] >> 4) & 0x03;    // 2 bits for type
    packet->token_length = data[0] & 0x0F;        // 4 bits for token length

    packet->coap_code = data[1];

    packet->coap_msg_id = (data[2] << 8) | data[3];
}