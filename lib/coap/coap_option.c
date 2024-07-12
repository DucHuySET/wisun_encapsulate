
#include <coap_option.h>

void set_coap_option_empty(COAP_OPTION *option, uint16_t number)
{
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_EMPTY;
    option->coap_option_length = 0;
}

void set_coap_option_opaque(COAP_OPTION *option, uint16_t number, const uint8_t *value, uint8_t length)
{
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_OPAQUE;
    option->coap_option_length = length;
    memcpy(option->coap_option_value.opaque, value, length);
}

void set_coap_option_uint(COAP_OPTION *option, uint16_t number, uint32_t value)
{
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_UINT;
    option->coap_option_length = sizeof(uint32_t);
    option->coap_option_value.uint_value = value;
}

void set_coap_option_string(COAP_OPTION *option, uint16_t number, const char *value)
{
    option->coap_option_number = number;
    option->coap_option_type = COAP_OPTION_STRING;
    option->coap_option_length = strlen(value);
    strncpy(option->coap_option_value.string, value, MAX_OPTION_VALUE_LENGTH);
}

size_t coap_option_list_to_bytes(COAP_OPTION_LIST *list_op, uint8_t *ouput)
{
    size_t size = 0;
    uint8_t previous_coap_option_number = 0;
    int i;
    for (i = 0; i < list_op->option_count; i++)
    {
        uint8_t option_number = list_op->list[i].coap_option_number;
        uint8_t option_delta = option_number - previous_coap_option_number;
        if (option_delta < 13)
        {
            ouput[size] = option_delta << 4;
        }
        else
        {
            ouput[size] = 0xC0; /** 0xC0 = 13 */
            ouput[size + 1] = option_delta - 13;
        }

        uint8_t option_value_length = list_op->list[i].coap_option_length;
        if (option_value_length < 13)
        {
            ouput[size] |= option_value_length;
        }
        else
        {
            ouput[size] |= 0x0C;
            ouput[size + 2] = option_value_length - 13;
        }
        size++;
        if (option_number >= 13)
            size++;
        if (option_value_length >= 13)
            size++;

        uint8_t option_value_type = list_op->list[i].coap_option_type;
        switch (option_value_type)
        {
        case COAP_OPTION_EMPTY:
            continue;
        case COAP_OPTION_OPAQUE:
            continue;
        case COAP_OPTION_UINT:
            ouput[size] = list_op->list[i].coap_option_value.uint_value;
            size++;
            continue;
        case COAP_OPTION_STRING:
            strncpy((char *)&ouput[size], list_op->list[i].coap_option_value.string, option_value_length);
            size += option_value_length;
            continue;
        default:
            break;
        }
    }
    return size;
}

int insert_coap_option_to_list(COAP_OPTION_LIST *list, COAP_OPTION *option)
{
    if (list->option_count >= MAX_OPTION_QUANTITY)
    {
        return -1;
    }

    if (list->option_count == 0)
    {
        list->list[0] = *option;
        list->option_count++;
        return list->option_count;
    }
    int i;
    for (i = list->option_count - 1; (i >= 0 && list->list[i].coap_option_number > option->coap_option_number); i--)
    {
        list->list[i + 1] = list->list[i];
    }
    list->list[i] = *option;
    list->option_count += 1;

    return list->option_count;
}