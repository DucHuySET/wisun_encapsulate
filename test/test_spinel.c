#include "../lib/spinel/wpan_api.h"
#include "../lib/spinel/spinel.h"
#include <stdio.h>
#include <string.h>

int main()
{
    struct wpan_api wpan;
    wpan.use_hdlc = 1;
    printf("start\n");
    uint8_t *payload = "payload";
    // uint8_t packet_encoded[SPINEL_FRAME_MAX_SIZE];
    // encode_packet(packet_encoded, SPINEL_CMD_PROP_VALUE_GET, payload, SPINEL_HEADER_DEFAULT);
    // for (size_t i = 0; i < strlen(packet_encoded); i++)
    // {
    //     printf("%02X", packet_encoded[i]);
    // }
    // transact(SPINEL_CMD_PROP_VALUE_GET, payload, SPINEL_HEADER_DEFAULT);
    // prop_change_value(SPINEL_CMD_PROP_VALUE_GET, SPINEL_PROP_LAST_STATUS, NULL, NULL, SPINEL_HEADER_DEFAULT);
    prop_get_value(SPINEL_PROP_NET_STATE, SPINEL_HEADER_DEFAULT);
    printf("\n");
    return 0;
}