#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/udp/udp.h"

// Example usage
int main()
{
    uint8_t payload[] = {
        0x48, 0x02, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00,
        0x12, 0x34, 0x56, 0x78, 0xB3, 0x6C, 0x65, 0x64,
        0xFF, 0x00, 0x01};
    size_t payload_length = sizeof(payload) / sizeof(payload[0]);

    UDPHeader header;
    header.src_port = COAP_PORT;
    header.dst_port = COAP_PORT;
    header.payload_length = payload_length;
    header.checksum = 0x0000;

    UDPDatagram datagram;
    datagram.header = header;
    datagram.payload = payload;
    datagram.payload_length = payload_length;

    // Convert UDPDatagram to bytes
    size_t datagram_length = udp_header_length() + datagram.payload_length;
    uint8_t datagram_bytes[datagram_length];
    udp_datagram_to_bytes(&datagram, datagram_bytes);

    for (size_t i = 0; i < datagram_length; i++)
    {
        printf("%02X ", datagram_bytes[i]);
    }

    // Convert bytes back to UDPDatagram
    UDPDatagram new_datagram;
    udp_datagram_from_bytes(datagram_bytes, datagram_length, &new_datagram);

    // Print out the results for verification
    printf("Source Port: %u\n", new_datagram.header.src_port);
    printf("Destination Port: %u\n", new_datagram.header.dst_port);
    printf("Payload Length: %u\n", new_datagram.header.payload_length);
    printf("Checksum: 0x%04x\n", new_datagram.header.checksum);
    printf("Payload: ");
    for (size_t i = 0; i < new_datagram.payload_length; ++i)
    {
        printf("0x%02x ", new_datagram.payload[i]);
    }
    printf("\n");

    // Free allocated memory
    free(new_datagram.payload);

    return 0;
}
