#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "udp.h"

// Function to convert UDPHeader to bytes
void udp_header_to_bytes(const UDPHeader *header, uint8_t *output)
{
    output[0] = header->src_port >> 8;
    output[1] = header->src_port & 0xFF;
    output[2] = header->dst_port >> 8;
    output[3] = header->dst_port & 0xFF;
    output[4] = header->payload_length >> 8;
    output[5] = header->payload_length & 0xFF;
    output[6] = header->checksum >> 8;
    output[7] = header->checksum & 0xFF;
}

// Function to convert bytes to UDPHeader
void udp_header_from_bytes(const uint8_t *data, UDPHeader *header)
{
    header->src_port = (data[0] << 8) | data[1];
    header->dst_port = (data[2] << 8) | data[3];
    header->payload_length = (data[4] << 8) | data[5];
    header->checksum = (data[6] << 8) | data[7];
}

// Function to convert UDPDatagram to bytes
void udp_datagram_to_bytes(const UDPDatagram *datagram, uint8_t *output)
{
    uint8_t header_bytes[8];
    udp_header_to_bytes(&datagram->header, header_bytes);
    memcpy(output, header_bytes, 8);
    if (datagram->payload_length > 0)
    {
        memcpy(output + 8, datagram->payload, datagram->payload_length);
    }
}

// Function to convert bytes to UDPDatagram
void udp_datagram_from_bytes(const uint8_t *data, size_t data_length, UDPDatagram *datagram)
{
    // Extract UDP header
    udp_header_from_bytes(data, &datagram->header);

    // Extract payload
    datagram->payload_length = data_length - 8;
    datagram->payload = (uint8_t *)malloc(datagram->payload_length);
    if (datagram->payload != NULL)
    {
        memcpy(datagram->payload, data + 8, datagram->payload_length);
    }
}

// Function to get the length of UDPHeader
size_t udp_header_length()
{
    return 8;
}
