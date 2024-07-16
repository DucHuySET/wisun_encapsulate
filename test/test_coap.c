
#include "../lib/coap/coap.h"
#include <string.h>
#include <stdio.h>

int main()
{
    COAP_PACKET coap_packet;
    COAP_HEADER_PACKET coap_header;
    coap_header_pack(&coap_header, COAP_MSG_TYPE_CONFIRMABLE, COAP_DEFAULT_TKL, COAP_MSG_CODE_REQUEST_POST, 0x1234, 0x12345678, NULL);
    COAP_OPTION coap_option_0;
    set_coap_option_string(&coap_option_0, COAP_OPTION_URI_PATH, "led");
    COAP_OPTION_LIST coap_option_list;
    coap_option_list.head = NULL;
    coap_option_list.option_count = 0;
    insert_coap_option_to_list(&coap_option_list, &coap_option_0);
    coap_header.coap_option_list = &coap_option_list;

    uint8_t payload[] = {0x00, 0x01};
    coap_packet_pack(&coap_header, payload, 2, &coap_packet);

    uint8_t packet_encoded[MAX_COAP_PACKET_LENGTH];
    size_t packet_size = coap_packet_to_bytes(&coap_packet, packet_encoded);
    printf("Packet size: %d\n, 04_50 \n", packet_size);
    for (int i = 0; i < packet_size; i++)
    {
        printf("%02X ", packet_encoded[i]);
    }
    return 0;
}