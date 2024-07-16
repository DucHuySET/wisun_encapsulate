

#include "ipv6.h"
#include "../udp/udp.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

uint16_t calculate_checksum(const uint8_t *data, size_t length)
{
    uint32_t checksum = 0;

    // Create halfwords from data bytes
    for (size_t i = 0; i < length; i += 2)
    {
        uint16_t halfword = (data[i] << 8);
        if (i + 1 < length)
        {
            halfword |= data[i + 1];
        }
        checksum += halfword;
        checksum = (checksum & 0xFFFF) + (checksum >> 16);
    }

    checksum ^= 0xFFFF;

    if (checksum == 0)
    {
        return 0xFFFF;
    }
    else
    {
        return (uint16_t)checksum;
    }
}

// Function to convert 32-bit integer from host byte order to network byte order (big-endian)
uint32_t htonl_custom(uint32_t hostlong)
{
    // Determine the endianness of the system
    union
    {
        uint32_t i;
        uint8_t c[4];
    } test = {0x01020304};

    // If the system is little-endian, swap bytes
    if (test.c[0] == 0x04)
    {
        return ((hostlong & 0xFF000000) >> 24) |
               ((hostlong & 0x00FF0000) >> 8) |
               ((hostlong & 0x0000FF00) << 8) |
               ((hostlong & 0x000000FF) << 24);
    }

    // Otherwise, the system is big-endian, return as is
    return hostlong;
}

// Function to convert IPv6PseudoHeader to byte array
void ipv6_psuedo_header_to_bytes(const struct IPv6PseudoHeader *header, uint8_t *data)
{
    int index = 0;
    for (int i = 0; i < 16; ++i)
    {
        data[index++] = header->source_address[i];
    }
    for (int i = 0; i < 16; ++i)
    {
        data[index++] = header->destination_address[i];
    }

    // Use custom htonl function
    uint32_t payload_length_n = htonl_custom(header->payload_length);
    for (int i = 0; i < sizeof(uint32_t); ++i)
    {
        data[index++] = ((uint8_t *)&payload_length_n)[i];
    }

    data[index++] = header->next_header;
}

// Function to convert IPv6Header to byte array
void ipv6_header_to_bytes(const struct IPv6Header *header, uint8_t *data)
{
    int index = 0;

    // First byte: version (4 bits) + traffic class (upper 4 bits)
    data[index++] = ((header->version & 0x0F) << 4) | ((header->traffic_class >> 4) & 0x0F);

    // Second byte: traffic class (lower 4 bits) + flow label (upper 4 bits)
    data[index++] = ((header->traffic_class & 0x0F) << 4) | ((header->flow_label >> 16) & 0x0F);
    data[index++] = (header->flow_label >> 8) & 0xFF;
    data[index++] = header->flow_label & 0xFF;

    // Payload length (2 bytes)
    data[index++] = (header->payload_length >> 8) & 0xFF;
    data[index++] = header->payload_length & 0xFF;

    // Next header + hop limit (2 bytes)
    data[index++] = header->next_header;
    data[index++] = header->hop_limit;

    // Source address (16 bytes)
    memcpy(&data[index], header->source_address, 16);
    index += 16;

    // Destination address (16 bytes)
    memcpy(&data[index], header->destination_address, 16);
}

// Function to build IPv6Header from byte array
void ipv6_header_from_bytes(const uint8_t *data, struct IPv6Header *header)
{
    int index = 0;

    // First byte: version (4 bits) + traffic class (upper 4 bits)
    header->version = (data[index] >> 4) & 0x0F;
    header->traffic_class = ((data[index] & 0x0F) << 4) | ((data[index + 1] >> 4) & 0x0F);
    index++;

    // Second byte: traffic class (lower 4 bits) + flow label (upper 4 bits)
    header->flow_label = ((data[index] & 0x0F) << 16) | (data[index + 1] << 8) | data[index + 2];
    index += 3;

    // Payload length (2 bytes)
    header->payload_length = (data[index] << 8) | data[index + 1];
    index += 2;

    // Next header + hop limit (2 bytes)
    header->next_header = data[index++];
    header->hop_limit = data[index++];

    // Source address (16 bytes)
    memcpy(header->source_address, &data[index], 16);
    index += 16;

    // Destination address (16 bytes)
    memcpy(header->destination_address, &data[index], 16);
}

void ipv6_packet_init(struct IPv6Packet *packet, struct IPv6Header ipv6_header, void *upper_layer_protocol, void *extension_headers)
{
    packet->ipv6_header = ipv6_header;
    packet->upper_layer_protocol = upper_layer_protocol;
    packet->extension_headers = extension_headers;
    packet->has_tunneled_ipv6_header = 0; // Initialize to false
    // Logic to check for tunneled IPv6 header and set has_tunneled_ipv6_header and tunneled_ipv6_header accordingly
}

int calculate_ipv6_checksum(struct IPv6Packet *packet)
{
    int saved_checksum = ((struct UDPDatagram *)packet->upper_layer_protocol)->header.checksum;
    ((struct UDPDatagram *)packet->upper_layer_protocol)->header.checksum = 0;

    char *upper_layer_protocol_bytes = (char *)packet->upper_layer_protocol;

    ((struct UDPDatagram *)packet->upper_layer_protocol)->header.checksum = saved_checksum;

    char source_address[40];
    char destination_address[40];
    if (packet->has_tunneled_ipv6_header)
    {
        strncpy(source_address, packet->tunneled_ipv6_header.source_address, sizeof(source_address) - 1);
        strncpy(destination_address, packet->tunneled_ipv6_header.destination_address, sizeof(destination_address) - 1);
    }
    else
    {
        strncpy(source_address, packet->ipv6_header.source_address, sizeof(source_address) - 1);
        strncpy(destination_address, packet->ipv6_header.destination_address, sizeof(destination_address) - 1);
    }

    struct IPv6PseudoHeader pseudo_header = {
        source_address,
        destination_address,
        sizeof(upper_layer_protocol_bytes),
        IPV6_NEXT_HEADER_UDP,
    };
    uint8_t data_needed_for_checksum[sizeof(pseudo_header) + sizeof(upper_layer_protocol_bytes)];
    memcpy(data_needed_for_checksum, &pseudo_header, sizeof(pseudo_header));
    memcpy(data_needed_for_checksum + sizeof(pseudo_header), upper_layer_protocol_bytes, sizeof(upper_layer_protocol_bytes));
    return calculate_checksum(data_needed_for_checksum, sizeof(upper_layer_protocol_bytes) + sizeof(upper_layer_protocol_bytes));
}

int ipv6_packet_to_bytes(struct IPv6Packet *packet, uint8_t *data)
{
    int index = 0;

    // Convert IPv6 header to bytes
    ipv6_header_to_bytes(&packet->ipv6_header, &data[index]);
    index += sizeof(struct IPv6Header);

    // Convert upper layer protocol to bytes
    if (packet->upper_layer_protocol != NULL)
    {
        if (packet->has_tunneled_ipv6_header)
        {
            ipv6_header_to_bytes(&packet->tunneled_ipv6_header, &data[index]);
            index += sizeof(struct IPv6Header);
        }
        else
        {
            udp_datagram_to_bytes((struct UDPDatagram *)packet->upper_layer_protocol, &data[index]);
            index += sizeof(struct UDPDatagram);
        }
    }

    // Convert extension headers to bytes
    // Not implemented here

    // Calculate checksum
    uint16_t checksum = calculate_ipv6_checksum(packet);

    // Add checksum to the end of the packet
    data[index++] = checksum >> 8;
    data[index++] = checksum & 0xFF;
    return index;
}