#include "wpan_api.h"
#include "../hdlc/hdlc.h"
#include "buffer_handle.h"
#include "spinel.h"
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief
/// @param result
/// @param command_id
/// @param payload
/// @param tid
void encode_packet(uint8_t *result, int command_id, uint8_t *payload, int tid)
{
    /*Encode the given payload as a Spinel frame.*/
    uint8_t header[SPINEL_HEADER_SIZE];
    // pack(header, "B", tid);
    char *pack_format = SPINEL_DATATYPE_UINT8_S;
    spinel_datatype_pack(header, SPINEL_HEADER_SIZE, pack_format, tid);
    uint8_t cmd_encoded[SPINEL_MAX_CMD_SIZE];
    spinel_packed_uint_encode(cmd_encoded, SPINEL_MAX_CMD_SIZE, command_id);
    memset(result, 0, sizeof(result));
    writeByteArrayToBuffer(result, header, spinel_strnlen(header, SPINEL_HEADER_SIZE));
    writeByteArrayToBuffer(result, cmd_encoded, spinel_strnlen(cmd_encoded, SPINEL_MAX_CMD_SIZE));
    writeByteArrayToBuffer(result, payload, spinel_strnlen(payload, SPINEL_FRAME_MAX_SIZE - SPINEL_MAX_CMD_SIZE - SPINEL_HEADER_SIZE));
}

/* Default tid: SPINEL_HEADER_DEFAULT
Default: use_hdlc*/
void transact(int command_id, uint8_t *payload, int tid)
{
    uint8_t pkt[SPINEL_FRAME_MAX_SIZE];
    memset(pkt, 0, SPINEL_FRAME_MAX_SIZE);
    encode_packet(pkt, command_id, payload, tid);
    uint8_t hdlc_encoded_pkt[HDLC_MAX_FRAME_SIZE];
    memset(hdlc_encoded_pkt, 0, HDLC_MAX_FRAME_SIZE);
    hdlc_encode(pkt, hdlc_encoded_pkt);
    // TODO: send hdlc_encoded_pkt to NCP

    printf("transact: \n");
    for (size_t i = 0; i < strlen(hdlc_encoded_pkt); i++)
    {
        printf("%02X", hdlc_encoded_pkt[i]);
    }
}

/// @brief
/// @param cmd
/// @param prop_id
/// @param encode_format default value is 'C'
/// @param tid default value is SPINEL_HEADER_DEFAULT
void prop_change_value(int cmd, int prop_id, uint8_t *value, char *encode_format, int tid)
{
    uint8_t payload[SPINEL_FRAME_MAX_SIZE - SPINEL_MAX_CMD_SIZE - SPINEL_HEADER_SIZE]; // 1 octet of header
    // encode_i(prop_id_encoded, prop_id);
    spinel_packed_uint_encode(payload, SPINEL_MAX_EXI_INT_SIZE, prop_id);
    if (value != NULL && encode_format != NULL)
    {
        uint8_t value_encoded[SPINEL_FRAME_MAX_SIZE - SPINEL_MAX_CMD_SIZE - SPINEL_HEADER_SIZE - SPINEL_MAX_EXI_INT_SIZE];
        spinel_datatype_pack(value_encoded, sizeof(value_encoded), encode_format, value);
        strcat(payload, value_encoded);
    }
    // pack(value_encoded, encode_format, value);
    transact(cmd, payload, tid);
}
/// @brief
/// @param prop_id
/// @param tid default value is SPINEL_HEADER_DEFAULT
void prop_get_value(int prop_id, int tid)
{   
    prop_change_value(SPINEL_CMD_PROP_VALUE_GET, prop_id, NULL, NULL, tid);
}

