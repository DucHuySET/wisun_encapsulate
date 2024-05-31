#include <stdint.h>

#ifndef WPAN_API_HEADER
#define WPAN_API_HEADER 1

#define WPAN_MAX_BUFFER_SIZE 1300 // equal to spinel max frame size

struct wpan_api
{
    int use_hdlc;
};

void pack(uint8_t *buffer, const char *format, ...);
void unpack(const uint8_t *buffer, const char *format, ...);
void encode_i(uint8_t *result, int data);
void encode_packet(uint8_t * result ,int command_id, uint8_t *payload, int tid);
void transact(int command_id, uint8_t* payload, int tid);
void prop_change_value(int cmd, int prop_id, uint8_t* value, char *encode_format, int tid);
void prop_get_value(int prop_id, int tid);

#endif