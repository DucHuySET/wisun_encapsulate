#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Function to convert IPv6 address string to byte array
int convert_to_ipaddress(const char *ip_str, uint8_t *ip_addr) {
    // IPv6 address has 8 groups of 4 hex digits, separated by ':'
    // Example IPv6 address: "2001:0db8:85a3:0000:0000:8a2e:0370:7334"

    // Initialize the byte array to zero
    memset(ip_addr, 0, 16);

    int addr_index = 0; // Index for ip_addr array
    int str_index = 0;  // Index for ip_str

    // Iterate through the IPv6 address string
    while (*ip_str) {
        // Read one group of hexadecimal digits
        char hex[5]; // 4 hex digits + 1 terminator
        int hex_index = 0;

        // Read characters until ':' or end of string
        while (*ip_str && *ip_str != ':') {
            if (hex_index < 4) {
                hex[hex_index++] = *ip_str;
            }
            ip_str++;
        }
        hex[hex_index] = '\0'; // Null-terminate the hex string

        // Convert hex string to integer
        unsigned int value = 0;
        sscanf(hex, "%x", &value);

        // Store the integer value in the ip_addr array
        ip_addr[addr_index++] = (uint8_t)((value >> 8) & 0xFF); // High byte
        ip_addr[addr_index++] = (uint8_t)(value & 0xFF);        // Low byte

        // Skip over the ':' separator
        if (*ip_str == ':') {
            ip_str++;
        }
    }

    return 0; // Success
}


// Function to convert byte array IPv6 address to string
int convert_from_ipaddress(const uint8_t *ip_addr, char *ip_str) {
    // IPv6 address string format: "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx"
    // Each group is represented by 4 hex digits

    int str_index = 0; // Index for ip_str
    for (int i = 0; i < 16; i += 2) {
        // Convert each byte pair to hex string
        sprintf(&ip_str[str_index], "%02x%02x:", ip_addr[i], ip_addr[i + 1]);
        str_index += 5; // Move to next group
    }

    // Remove the last ':'
    ip_str[str_index - 1] = '\0';

    return 0; // Success
}
