#include <stdint.h>
#include <stdio.h>

#define COAP_VERSION 1  // Example value, change as needed
#define COAP_TYPE_CON 0 // Example value, change as needed

typedef struct
{
    uint8_t coap_version; // 2 bits
    uint8_t coap_type;    // 2 bits
    uint8_t token_length; // 4 bits
    uint8_t coap_code;    // 8 bits
    uint16_t coap_msg_id; // 16 bits
} COAP_HEADER_PACKET;

void pack_coap_header(COAP_HEADER_PACKET *packet, uint8_t *output)
{
    // Combine the first byte
    output[0] = (packet->coap_version << 6) | (packet->coap_type << 4) | (packet->token_length);

    // Second byte
    output[1] = packet->coap_code;

    // Third and fourth bytes (Message ID)
    output[2] = (uint8_t)(packet->coap_msg_id >> 8);   // High byte
    output[3] = (uint8_t)(packet->coap_msg_id & 0xFF); // Low byte
}

int main()
{
    COAP_HEADER_PACKET packet = {
        .coap_version = COAP_VERSION,
        .coap_type = COAP_TYPE_CON,
        .token_length = 0,
        .coap_code = 0,
        .coap_msg_id = 0x1234 // Example Message ID
    };

    uint8_t packed_data[4];
    pack_coap_header(&packet, packed_data);

    // Print packed data for verification
    for (int i = 0; i < 4; i++)
    {
        printf("Byte %d: 0x%02X\n", i, packed_data[i]);
    }

    return 0;
}
