#include "coap.h"
#include <string.h>

size_t coap_packet_to_bytes(const COAP_PACKET *packet, uint8_t *result)
{
    size_t size = 0;
    uint8_t header_bytes[MAX_HEADER_LENGTH];
    size_t header_size = coap_header_to_bytes(&packet->coap_header, header_bytes);
    memcpy(result, header_bytes, header_size);
    size += header_size;

    result[size] = 0xFF; /*Payload mark*/
    size++;

    memcpy(result + size, packet->coap_payload, packet->coap_payload_length);
    size += packet->coap_payload_length;
    return size;
}

void coap_packet_pack(COAP_HEADER_PACKET *header, uint8_t *payload, size_t payload_length, COAP_PACKET *packet)
{
    packet->coap_header = *header;
    packet->coap_payload = payload;
    packet->coap_payload_length = payload_length;
}
