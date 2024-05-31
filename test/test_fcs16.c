#include "../lib/hdlc/fcs_computation.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int n = 3;
    // Cấp phát mảng động
    unsigned char *p = malloc(n * sizeof(unsigned char));

    // Khởi tạo giá trị cho các phần tử
    p[0] = 0x05;
    p[1] = 0xE6;
    p[2] = 0xA3;

    // In ra giá trị các phần tử
    for (int i = 0; i < n; ++i)
    {
        printf("%02X ", p[i]);
    }
    testFcs16(p, strlen(p));
    // Giải phóng bộ nhớ
    free(p);
    return 0;
}