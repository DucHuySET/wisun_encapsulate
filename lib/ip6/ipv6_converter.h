#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Function to convert IPv6 address string to byte array
int convert_to_ipaddress(const char *ip_str, uint8_t *ip_addr);
int convert_from_ipaddress(const uint8_t *ip_addr, char *ip_str);