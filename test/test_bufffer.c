#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../lib/spinel/spinel.h"
#include "../lib/spinel/wpan_api.h"

int main(){
    uint8_t buffer[100];
    memset(buffer, 0, 100);
    buffer[0] = 0x81;
    buffer[1] = 0x02;
    buffer[2] = 0x00;
    buffer[3] = 0x68;
    buffer[4] = 0x02;
    spinel_packed_uint_encode(buffer, 1, 0);
    printf("strlen: %d\n", strlen(buffer));
    return 0;
}