// #include <stdio.h>
// #include <stdint.h>

// unsigned char *pack()
// {
//     unsigned char *tmp = "wpan api";
//     return tmp;
// }

// int main()
// {
//     uint16_t myVariable = 42;
//     unsigned char tmp = 42;
//     int data = 102324;
//     while (data)

//     {
//         uint8_t value = data & 0x7F;
//         data >>= 7;
//         if (data)
//         {
//             value |= 0x80;
//         }
//         uint8_t *value_tmp;
//         pack(value_tmp, "B", value);
//     }

//     // Print in 0x format
//     printf("Value in 0x format: 0x%02X\n", tmp);

//     unsigned char *buffer;
//     buffer = pack();

//     printf("\n%s\n", buffer);

//     return 0;
// }



#include <stdint.h>
#include <stdio.h>

#define COAP_VERSION 1 // Thay đổi giá trị này theo đúng giá trị mong muốn
#define COAP_TYPE_CON 0 // Thay đổi giá trị này theo đúng giá trị mong muốn

typedef struct {
    //1st byte
    uint8_t coap_version;
    uint8_t coap_type;
    uint8_t token_length;
    //2nd byte
    uint8_t coap_code;
    //3rd-4th byte
    uint16_t coap_msg_id;
} COAP_HEADER_PACKET;

int main() {
    COAP_HEADER_PACKET packet = {
        .coap_version = COAP_VERSION,
        .coap_type = COAP_TYPE_CON,
        .token_length = 0,
        .coap_code = 0,
        .coap_msg_id = 0
    };

    // In giá trị các thành viên của struct
    printf("CoAP Version: %u\n", packet.coap_version);
    printf("CoAP Type: %u\n", packet.coap_type);
    printf("Token Length: %u\n", packet.token_length);
    printf("CoAP Code: %u\n", packet.coap_code);
    printf("CoAP Msg ID: %u\n", packet.coap_msg_id);

    return 0;
}
