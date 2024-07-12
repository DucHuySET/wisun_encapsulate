#include <stdint.h>
#include <stddef.h>
#include "coap_header.h"
/**Header file of CoAP, header file of CoAP Header is COAP_HEADER_HEADER */
#ifndef COAP_HEADER
#define COAP_HEADER

typedef struct COAP_PACKET
{
    COAP_HEADER_PACKET coap_header;
    uint8_t *coap_payload;
    size_t coap_payload_length;
} COAP_PACKET;

size_t coap_packet_to_bytes(const COAP_PACKET *packet, uint8_t *result);
void coap_packet_from_bytes(COAP_PACKET *packet, uint8_t *data);

#endif