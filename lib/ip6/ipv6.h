#include <stddef.h>
#include <stdint.h>

#ifndef IPV6_HEADER
#define IPV6_HEADER
// Next headers for IPv6 protocols
#define IPV6_NEXT_HEADER_HOP_BY_HOP 0
#define IPV6_NEXT_HEADER_TCP 6
#define IPV6_NEXT_HEADER_UDP 17
#define IPV6_NEXT_HEADER_IP 41
#define IPV6_NEXT_HEADER_ICMP 58

// Array of upper layer protocols
static int UPPER_LAYER_PROTOCOLS[] = {
    IPV6_NEXT_HEADER_TCP,
    IPV6_NEXT_HEADER_UDP,
    IPV6_NEXT_HEADER_ICMP};

// ICMP Protocol codes
#define ICMP_ECHO_REQUEST 128
#define ICMP_ECHO_RESPONSE 129

// Default hop limit for IPv6
#define HOP_LIMIT_DEFAULT 64

uint16_t calculate_checksum(const uint8_t *data, size_t length);

/**Ipv6 Psuedo Header */
// Define the IPv6PseudoHeader structure
struct IPv6PseudoHeader
{
    uint8_t *source_address;
    uint8_t *destination_address;
    uint32_t payload_length;
    uint8_t next_header;
};

// Define the IPv6 header structure
typedef struct IPv6Header
{
    uint8_t version;
    uint8_t traffic_class;
    uint32_t flow_label;
    uint16_t payload_length;
    uint8_t next_header;
    uint8_t hop_limit;
    uint8_t source_address[16];
    uint8_t destination_address[16];
} IPv6Header;

void ipv6_header_to_bytes(const struct IPv6Header *header, uint8_t *data);
void ipv6_header_from_bytes(const uint8_t *data, struct IPv6Header *header);

typedef struct IPv6Packet
{
    struct IPv6Header ipv6_header;
    void *upper_layer_protocol; // Pointer to upper layer protocol structure (ICMPv6, UDP, etc.)
    void *extension_headers;    // Pointer to array of extension headers (not implemented here)
    int has_tunneled_ipv6_header;
    struct IPv6Header tunneled_ipv6_header;
} IPv6Packet;

void ipv6_packet_init(struct IPv6Packet *packet, struct IPv6Header ipv6_header, void *upper_layer_protocol, void *extension_headers);
int ipv6_packet_to_bytes(struct IPv6Packet *packet, uint8_t *data);

#endif