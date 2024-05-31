#include <stdio.h>
#include <stdint.h>

unsigned char *pack()
{
    unsigned char *tmp = "wpan api";
    return tmp;
}

int main()
{
    uint16_t myVariable = 42;
    unsigned char tmp = 42;
    int data = 102324;
    while (data)

    {
        uint8_t value = data & 0x7F;
        data >>= 7;
        if (data)
        {
            value |= 0x80;
        }
        uint8_t *value_tmp;
        pack(value_tmp, "B", value);
    }

    // Print in 0x format
    printf("Value in 0x format: 0x%02X\n", tmp);

    unsigned char *buffer;
    buffer = pack();

    printf("\n%s\n", buffer);

    return 0;
}
