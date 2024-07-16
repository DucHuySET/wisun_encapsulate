#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../lib/ip6/ipv6.h"
#include "../lib/udp/udp.h"
#include "../lib/ip6/ipv6_converter.h"
#include <stdlib.h>

int main()
{
    IPv6Packet packet;
    IPv6Header header;
    header.version = 6;
    header.traffic_class = 0;
    header.flow_label = 0;
    header.payload_length = 0;
    header.next_header = IPV6_NEXT_HEADER_UDP;
    header.hop_limit = HOP_LIMIT_DEFAULT;
    uint8_t *source_address = (uint8_t *)malloc(16);
    uint8_t *destination_address = (uint8_t *)malloc(16);
    convert_to_ipaddress("2020:abcd::212:4b00:29bd:ab4b", source_address);
    convert_to_ipaddress("2020:abcd::212:4b00:29bd:ab4b", destination_address);
    memcpy(header.source_address, source_address, 16);
    memcpy(header.destination_address, destination_address, 16);

    UDPDatagram udp_datagram;
    udp_datagram.header.src_port = COAP_PORT;
    udp_datagram.header.dst_port = COAP_PORT;
    udp_datagram.header.payload_length = 0;
    udp_datagram.header.checksum = 0;
    udp_datagram.payload = NULL;
    udp_datagram.payload_length = 0;

    ipv6_packet_init(&packet, header, &udp_datagram, NULL);

    uint8_t data[54];
    size_t data_len = ipv6_packet_to_bytes(&packet, data);

    for (size_t i = 0; i < data_len; i++)
    {
        printf("%02x ", data[i]);
    }

    return 0;
}
