#include "coap.h"

void set_coap_option_empty(COAP_OPTION *option, uint16_t number) {
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_EMPTY;
    option->coap_option_length = 0;
}

void set_coap_option_opaque(COAP_OPTION *option, uint16_t number, const uint8_t *value, uint8_t length) {
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_OPAQUE;
    option->coap_option_length = length;
    memcpy(option->coap_option_value.opaque, value, length);
}

void set_coap_option_uint(COAP_OPTION *option, uint16_t number, uint32_t value) {
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_UINT;
    option->coap_option_length = sizeof(uint32_t);
    option->coap_option_value.uint_value = value;
}

void set_coap_option_string(COAP_OPTION *option, uint16_t number, const char *value) {
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_STRING;
    option->coap_option_length = strlen(value);
    strncpy(option->coap_option_value.string, value, MAX_OPTION_VALUE_LENGTH);
}

size_t coap_option_list_to_bytes(COAP_OPTION_LIST *list_op, uint8_t *ouput)
{   
    size_t size = 0;
    int i;
    for (i = 0; i < list_op->option_count ; i++){
        uint8_t option_number = list_op->list[i].coap_option_number;
        if (option_number < 13){
            ouput[size] = option_number << 4;
        }else {
            ouput[size] = 0xC0;
            ouput[size+1] = option_number - 13;
        }

        uint8_t option_value_length = list_op->list[i].coap_option_length;
        if (option_value_length < 13){
            ouput[size] |= option_value_length;
        } else {
            ouput[size] |= 0x0C;
            ouput[size+2] = option_value_length - 13;
        }
        size++;
        if (option_number >= 13) size++;
        if (option_value_length >= 13) size++;

        uint8_t option_value_type = list_op->list[i].coap_option_type;
        switch (option_value_type)
        {
        case COAP_OPTION_EMPTY:
            break;
        case COAP_OPTION_OPAQUE:
            
            break;
        default:
            break;
        }
    }
    return size;
}

void coap_header_to_bytes(const COAP_HEADER_PACKET *packet, uint8_t *result)
{
    result[0] = (packet->coap_version << 6) | (packet->coap_type << 4) | (packet->token_length);
    result[1] = packet->coap_code;
    result[2] = (uint8_t)(packet->coap_msg_id >> 8);
    result[3] = (uint8_t)(packet->coap_msg_id & 0xFF);
}

void coap_header_from_bytes(COAP_HEADER_PACKET *packet, uint8_t *data) {
    packet->coap_version = (data[0] >> 6) & 0x03; // 2 bits for version
    packet->coap_type = (data[0] >> 4) & 0x03;    // 2 bits for type
    packet->token_length = data[0] & 0x0F;        // 4 bits for token length

    packet->coap_code = data[1];

    packet->coap_msg_id = (data[2] << 8) | data[3];
}