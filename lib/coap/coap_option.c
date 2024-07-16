
#include "coap_option.h"
#include <string.h>

void set_coap_option_empty(COAP_OPTION *option, uint16_t number)
{
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_EMPTY;
    option->coap_option_length = 0;
    option->next = NULL;
}

void set_coap_option_opaque(COAP_OPTION *option, uint16_t number, const uint8_t *value, uint8_t length)
{
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_OPAQUE;
    option->coap_option_length = length;
    memcpy(option->coap_option_value.opaque, value, length);
    option->next = NULL;
}

void set_coap_option_uint(COAP_OPTION *option, uint16_t number, uint32_t value)
{
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_UINT;
    option->coap_option_length = sizeof(uint32_t);
    option->coap_option_value.uint_value = value;
    option->next = NULL;
}

void set_coap_option_string(COAP_OPTION *option, uint16_t number, const char *value)
{
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_STRING;
    option->coap_option_length = strlen(value);
    strncpy(option->coap_option_value.string, value, MAX_OPTION_VALUE_LENGTH);
    option->next = NULL;
}

size_t coap_option_list_to_bytes(const COAP_OPTION_LIST *list_op, uint8_t *output)
{
    if (list_op->option_count == 0)
    {
        return 0;
    }
    size_t size = 0;
    uint8_t previous_coap_option_number = 0;
    COAP_OPTION *cursor = list_op->head;
    while (cursor)
    {
        uint8_t option_number = cursor->coap_option_number;
        uint8_t option_delta = option_number - previous_coap_option_number;
        if (option_delta < 13)
        {
            output[size] = option_delta << 4;
        }
        else
        {
            output[size] = 0xC0; /** 0xC0 = 13 */
            output[size + 1] = option_delta - 13;
        }

        uint8_t option_value_length = cursor->coap_option_length;
        if (option_value_length < 13)
        {
            output[size] |= option_value_length;
        }
        else
        {
            output[size] |= 0x0C;
            output[size + 2] = option_value_length - 13;
        }
        size++;
        if (option_delta >= 13)
            size++;
        if (option_value_length >= 13)
            size++;

        uint8_t option_value_type = cursor->coap_option_type;
        switch (option_value_type)
        {
        case COAP_OPTION_EMPTY:
            break;
        case COAP_OPTION_OPAQUE:
            memcpy(&output[size], cursor->coap_option_value.opaque, option_value_length);
            size += option_value_length;
            break;
        case COAP_OPTION_UINT:
            output[size] = cursor->coap_option_value.uint_value;
            size++;
            break;
        case COAP_OPTION_STRING:
            strncpy((char *)&output[size], cursor->coap_option_value.string, option_value_length);
            size += option_value_length;
            break;
        default:
            break;
        }

        previous_coap_option_number = option_number;
        cursor = cursor->next;
    }
    return size;
}

int insert_coap_option_to_list(COAP_OPTION_LIST *list, COAP_OPTION *option)
{
    if (list->option_count >= MAX_OPTION_QUANTITY)
    {
        return -1;
    }
    COAP_OPTION *new_option = (COAP_OPTION *)malloc(sizeof(COAP_OPTION));
    if (new_option == NULL)
    {
        return -1;
    }
    memcpy(new_option, option, sizeof(COAP_OPTION));
    new_option->next = NULL;

    if (list->head == NULL || list->head->coap_option_number > option->coap_option_number)
    {
        new_option->next = list->head;
        list->head = new_option;
        list->option_count++;
        return list->option_count;
    }
    COAP_OPTION *cursor = list->head;
    while (cursor->next != NULL && cursor->next->coap_option_number <= option->coap_option_number)
    {
        cursor = cursor->next;
    }

    new_option->next = cursor->next;
    cursor->next = new_option;
    list->option_count++;

    return list->option_count;
}