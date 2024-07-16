#include <stdint.h>
#include <stddef.h>

#ifndef COAP_HEADER
#define COAP_HEADER

#define COAP_PORT 5683
// Define UDPHeader structure
typedef struct
{
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t payload_length;
    uint16_t checksum;
} UDPHeader;

// Define UDPDatagram structure
typedef struct UDPDatagram
{
    UDPHeader header;
    uint8_t *payload;
    size_t payload_length;
} UDPDatagram;

void udp_header_to_bytes(const UDPHeader *header, uint8_t *output);
void udp_header_from_bytes(const uint8_t *data, UDPHeader *header);
void udp_datagram_to_bytes(const UDPDatagram *datagram, uint8_t *output);
void udp_datagram_from_bytes(const uint8_t *data, size_t data_length, UDPDatagram *datagram);
size_t udp_header_length();

#endif