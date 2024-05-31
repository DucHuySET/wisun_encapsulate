#include "spinel_datatype_pack.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

spinel_ssize_t spinel_packed_uint_decode(const uint8_t *bytes, spinel_size_t len, unsigned int *value_ptr)
{
    spinel_ssize_t ret   = 0;
    unsigned int   value = 0;

    unsigned int i = 0;

    do
    {
        if (len < sizeof(uint8_t))
        {
            ret = -1;
            break;
        }

        value |= (unsigned int)(bytes[0] & 0x7F) << i;
        i += 7;
        ret += sizeof(uint8_t);
        bytes += sizeof(uint8_t);
        len -= sizeof(uint8_t);
    } while ((bytes[-1] & 0x80) == 0x80);

    if ((ret > 0) && (value_ptr != NULL))
    {
        *value_ptr = value;
    }

    return ret;
}

spinel_ssize_t spinel_packed_uint_size(unsigned int value)
{
    spinel_ssize_t ret;

    if (value < (1 << 7))
    {
        ret = 1;
    }
    else if (value < (1 << 14))
    {
        ret = 2;
    }
    else if (value < (1 << 21))
    {
        ret = 3;
    }
    else if (value < (1 << 28))
    {
        ret = 4;
    }
    else
    {
        ret = 5;
    }

    return ret;
}

spinel_ssize_t spinel_packed_uint_encode(uint8_t *bytes, spinel_size_t len, unsigned int value)
{
    const spinel_ssize_t encoded_size = spinel_packed_uint_size(value);

    if ((spinel_ssize_t)len >= encoded_size)
    {
        spinel_ssize_t i;

        for (i = 0; i != encoded_size - 1; ++i)
        {
            *bytes++ = (value & 0x7F) | 0x80;
            value    = (value >> 7);
        }

        *bytes++ = (value & 0x7F);
    }

    return encoded_size;
}

const char *spinel_next_packed_datatype(const char *pack_format)
{
    int depth = 0;

    do
    {
        switch (*++pack_format)
        {
        case '(':
            depth++;
            break;

        case ')':
            depth--;

            if (depth == 0)
            {
                pack_format++;
            }

            break;
        }
    } while ((depth > 0) && *pack_format != 0);

    return pack_format;
}

static spinel_ssize_t spinel_datatype_vunpack_(bool           in_place,
                                               const uint8_t *data_in,
                                               spinel_size_t  data_len,
                                               const char *   pack_format,
                                               va_list_obj *  args)
{
    spinel_ssize_t ret = 0;

    // Buffer length sanity check
    // require_action(data_len <= SPINEL_MAX_PACK_LENGTH, bail, (ret = -1, errno = EINVAL));

    for (; *pack_format != 0; pack_format = spinel_next_packed_datatype(pack_format))
    {
        if (*pack_format == ')')
        {
            // Don't go past the end of a struct.
            break;
        }

        switch ((spinel_datatype_t)pack_format[0])
        {
        case SPINEL_DATATYPE_BOOL_C:
        {
            bool *arg_ptr = va_arg(args->obj, spinel_bool_t);
            // require_action(data_len >= sizeof(uint8_t), bail, (ret = -1, errno = EOVERFLOW));

            if (arg_ptr)
            {
                *arg_ptr = data_in[0] != 0;
            }

            ret += sizeof(uint8_t);
            data_in += sizeof(uint8_t);
            data_len -= sizeof(uint8_t);
            break;
        }

        case SPINEL_DATATYPE_INT8_C:
        case SPINEL_DATATYPE_UINT8_C:
        {
            uint8_t *arg_ptr = va_arg(args->obj, uint8_t *);
            // require_action(data_len >= sizeof(uint8_t), bail, (ret = -1, errno = EOVERFLOW));

            if (arg_ptr)
            {
                *arg_ptr = data_in[0];
            }

            ret += sizeof(uint8_t);
            data_in += sizeof(uint8_t);
            data_len -= sizeof(uint8_t);
            break;
        }

        case SPINEL_DATATYPE_INT16_C:
        case SPINEL_DATATYPE_UINT16_C:
        {
            uint16_t *arg_ptr = va_arg(args->obj, uint16_t *);
            // require_action(data_len >= sizeof(uint16_t), bail, (ret = -1, errno = EOVERFLOW));

            if (arg_ptr)
            {
                *arg_ptr = (uint16_t)((data_in[1] << 8) | data_in[0]);
            }

            ret += sizeof(uint16_t);
            data_in += sizeof(uint16_t);
            data_len -= sizeof(uint16_t);
            break;
        }

        case SPINEL_DATATYPE_INT32_C:
        case SPINEL_DATATYPE_UINT32_C:
        {
            uint32_t *arg_ptr = va_arg(args->obj, uint32_t *);
            // require_action(data_len >= sizeof(uint32_t), bail, (ret = -1, errno = EOVERFLOW));

            if (arg_ptr)
            {
                *arg_ptr = (uint32_t)((data_in[3] << 24) | (data_in[2] << 16) | (data_in[1] << 8) | data_in[0]);
            }

            ret += sizeof(uint32_t);
            data_in += sizeof(uint32_t);
            data_len -= sizeof(uint32_t);
            break;
        }

        case SPINEL_DATATYPE_INT64_C:
        case SPINEL_DATATYPE_UINT64_C:
        {
            uint64_t *arg_ptr = va_arg(args->obj, uint64_t *);
            // require_action(data_len >= sizeof(uint64_t), bail, (ret = -1, errno = EOVERFLOW));

            if (arg_ptr)
            {
                uint32_t l32 = (uint32_t)((data_in[3] << 24) | (data_in[2] << 16) | (data_in[1] << 8) | data_in[0]);
                uint32_t h32 = (uint32_t)((data_in[7] << 24) | (data_in[6] << 16) | (data_in[5] << 8) | data_in[4]);

                *arg_ptr = ((uint64_t)l32) | (((uint64_t)h32) << 32);
            }

            ret += sizeof(uint64_t);
            data_in += sizeof(uint64_t);
            data_len -= sizeof(uint64_t);
            break;
        }

        case SPINEL_DATATYPE_IPv6ADDR_C:
        {
            // require_action(data_len >= sizeof(spinel_ipv6addr_t), bail, (ret = -1, errno = EOVERFLOW));

            if (in_place)
            {
                spinel_ipv6addr_t *arg = va_arg(args->obj, spinel_ipv6addr_t *);
                if (arg)
                {
                    memcpy(arg, data_in, sizeof(spinel_ipv6addr_t));
                }
            }
            else
            {
                const spinel_ipv6addr_t **arg_ptr = va_arg(args->obj, const spinel_ipv6addr_t **);
                if (arg_ptr)
                {
                    *arg_ptr = (const spinel_ipv6addr_t *)data_in;
                }
            }

            ret += sizeof(spinel_ipv6addr_t);
            data_in += sizeof(spinel_ipv6addr_t);
            data_len -= sizeof(spinel_ipv6addr_t);
            break;
        }

        case SPINEL_DATATYPE_EUI64_C:
        {
            // require_action(data_len >= sizeof(spinel_eui64_t), bail, (ret = -1, errno = EOVERFLOW));

            if (in_place)
            {
                spinel_eui64_t *arg = va_arg(args->obj, spinel_eui64_t *);
                if (arg)
                {
                    memcpy(arg, data_in, sizeof(spinel_eui64_t));
                }
            }
            else
            {
                const spinel_eui64_t **arg_ptr = va_arg(args->obj, const spinel_eui64_t **);
                if (arg_ptr)
                {
                    *arg_ptr = (const spinel_eui64_t *)data_in;
                }
            }

            ret += sizeof(spinel_eui64_t);
            data_in += sizeof(spinel_eui64_t);
            data_len -= sizeof(spinel_eui64_t);
            break;
        }

        case SPINEL_DATATYPE_EUI48_C:
        {
            // require_action(data_len >= sizeof(spinel_eui48_t), bail, (ret = -1, errno = EOVERFLOW));

            if (in_place)
            {
                spinel_eui48_t *arg = va_arg(args->obj, spinel_eui48_t *);
                if (arg)
                {
                    memcpy(arg, data_in, sizeof(spinel_eui48_t));
                }
            }
            else
            {
                const spinel_eui48_t **arg_ptr = va_arg(args->obj, const spinel_eui48_t **);
                if (arg_ptr)
                {
                    *arg_ptr = (const spinel_eui48_t *)data_in;
                }
            }

            ret += sizeof(spinel_eui48_t);
            data_in += sizeof(spinel_eui48_t);
            data_len -= sizeof(spinel_eui48_t);
            break;
        }

        case SPINEL_DATATYPE_UINT_PACKED_C:
        {
            unsigned int * arg_ptr = va_arg(args->obj, unsigned int *);
            spinel_ssize_t pui_len = spinel_packed_uint_decode(data_in, data_len, arg_ptr);

            // Range check
            // require_action(NULL == arg_ptr || (*arg_ptr < SPINEL_MAX_UINT_PACKED), bail, (ret = -1, errno = ERANGE));

            // require(pui_len > 0, bail);

            // require(pui_len <= (spinel_ssize_t)data_len, bail);

            ret += pui_len;
            data_in += pui_len;
            data_len -= (spinel_size_t)pui_len;
            break;
        }

        case SPINEL_DATATYPE_UTF8_C:
        {
            size_t len;

            // Make sure we have at least one byte.
            // require_action(data_len > 0, bail, (ret = -1, errno = EOVERFLOW));

            // Add 1 for zero termination. If not zero terminated,
            // len will then be data_len+1, which we will detect
            // in the next check.
            len = spinel_strnlen((const char *)data_in, data_len) + 1;

            // Verify that the string is zero terminated.
            // require_action(len <= data_len, bail, (ret = -1, errno = EOVERFLOW));

            // Verify the string follows valid UTF8 encoding.
            // require_action(spinel_validate_utf8(data_in), bail, (ret = -1, errno = EINVAL));

            if (in_place)
            {
                char * arg     = va_arg(args->obj, char *);
                size_t len_arg = va_arg(args->obj, size_t);
                if (arg)
                {
                    // require_action(len_arg >= len, bail, (ret = -1, errno = ENOMEM));
                    memcpy(arg, data_in, len);
                }
            }
            else
            {
                const char **arg_ptr = va_arg(args->obj, const char **);
                if (arg_ptr)
                {
                    *arg_ptr = (const char *)data_in;
                }
            }

            ret += (spinel_size_t)len;
            data_in += len;
            data_len -= (spinel_size_t)len;
            break;
        }

        case SPINEL_DATATYPE_DATA_C:
        case SPINEL_DATATYPE_DATA_WLEN_C:
        {
            spinel_ssize_t pui_len       = 0;
            uint16_t       block_len     = 0;
            const uint8_t *block_ptr     = data_in;
            void *         arg_ptr       = va_arg(args->obj, void *);
            unsigned int * block_len_ptr = va_arg(args->obj, unsigned int *);
            char           nextformat    = *spinel_next_packed_datatype(pack_format);

            if ((pack_format[0] == SPINEL_DATATYPE_DATA_WLEN_C) || ((nextformat != 0) && (nextformat != ')')))
            {
                pui_len = spinel_datatype_unpack(data_in, data_len, SPINEL_DATATYPE_UINT16_S, &block_len);
                block_ptr += pui_len;

                // require(pui_len > 0, bail);
                // require(block_len < SPINEL_FRAME_MAX_SIZE, bail);
            }
            else
            {
                block_len = (uint16_t)data_len;
                pui_len   = 0;
            }

            // require_action((spinel_ssize_t)data_len >= (block_len + pui_len), bail, (ret = -1, errno = EOVERFLOW));

            if (in_place)
            {
                // require_action(NULL != block_len_ptr && *block_len_ptr >= block_len, bail, (ret = -1, errno = EINVAL));
                memcpy(arg_ptr, block_ptr, block_len);
            }
            else
            {
                const uint8_t **block_ptr_ptr = (const uint8_t **)arg_ptr;
                if (NULL != block_ptr_ptr)
                {
                    *block_ptr_ptr = block_ptr;
                }
            }

            if (NULL != block_len_ptr)
            {
                *block_len_ptr = block_len;
            }

            block_len += (uint16_t)pui_len;
            ret += block_len;
            data_in += block_len;
            data_len -= block_len;
            break;
        }

        case 'T':
        case SPINEL_DATATYPE_STRUCT_C:
        {
            spinel_ssize_t pui_len    = 0;
            uint16_t       block_len  = 0;
            spinel_ssize_t actual_len = 0;
            const uint8_t *block_ptr  = data_in;
            char           nextformat = *spinel_next_packed_datatype(pack_format);

            if ((pack_format[0] == SPINEL_DATATYPE_STRUCT_C) || ((nextformat != 0) && (nextformat != ')')))
            {
                pui_len = spinel_datatype_unpack(data_in, data_len, SPINEL_DATATYPE_UINT16_S, &block_len);
                block_ptr += pui_len;

                // require(pui_len > 0, bail);
                // require(block_len < SPINEL_FRAME_MAX_SIZE, bail);
            }
            else
            {
                block_len = (uint16_t)data_len;
                pui_len   = 0;
            }

            // require_action((spinel_ssize_t)data_len >= (block_len + pui_len), bail, (ret = -1, errno = EOVERFLOW));

            actual_len = spinel_datatype_vunpack_(false, block_ptr, block_len, pack_format + 2, args);

            // require_action(actual_len > -1, bail, (ret = -1, errno = EOVERFLOW));

            if (pui_len)
            {
                block_len += (uint16_t)pui_len;
            }
            else
            {
                block_len = (uint16_t)actual_len;
            }

            ret += block_len;
            data_in += block_len;
            data_len -= block_len;
            break;
        }

        case '.':
            // Skip.
            break;

        case SPINEL_DATATYPE_ARRAY_C:
        default:
            // Unsupported Type!
            ret   = -1;
            // errno = EINVAL;
            goto bail;
        }
    }

    return ret;

bail:
    return ret;
}

spinel_ssize_t spinel_datatype_unpack_in_place(const uint8_t *data_in,
                                               spinel_size_t  data_len,
                                               const char *   pack_format,
                                               ...)
{
    spinel_ssize_t ret;
    va_list_obj    args;
    va_start(args.obj, pack_format);

    ret = spinel_datatype_vunpack_(true, data_in, data_len, pack_format, &args);

    va_end(args.obj);
    return ret;
}

spinel_ssize_t spinel_datatype_unpack(const uint8_t *data_in, spinel_size_t data_len, const char *pack_format, ...)
{
    spinel_ssize_t ret;
    va_list_obj    args;
    va_start(args.obj, pack_format);

    ret = spinel_datatype_vunpack_(false, data_in, data_len, pack_format, &args);

    va_end(args.obj);
    return ret;
}

spinel_ssize_t spinel_datatype_vunpack_in_place(const uint8_t *data_in,
                                                spinel_size_t  data_len,
                                                const char *   pack_format,
                                                va_list        args)
{
    spinel_ssize_t ret;
    va_list_obj    args_obj;
    va_copy(args_obj.obj, args);

    ret = spinel_datatype_vunpack_(true, data_in, data_len, pack_format, &args_obj);

    va_end(args_obj.obj);
    return ret;
}

spinel_ssize_t spinel_datatype_vunpack(const uint8_t *data_in,
                                       spinel_size_t  data_len,
                                       const char *   pack_format,
                                       va_list        args)
{
    spinel_ssize_t ret;
    va_list_obj    args_obj;
    va_copy(args_obj.obj, args);

    ret = spinel_datatype_vunpack_(false, data_in, data_len, pack_format, &args_obj);

    va_end(args_obj.obj);
    return ret;
}

static spinel_ssize_t spinel_datatype_vpack_(uint8_t *     data_out,
                                             spinel_size_t data_len_max,
                                             const char *  pack_format,
                                             va_list_obj * args)
{
    spinel_ssize_t ret = 0;

    // Buffer length sanity check
    // require_action(data_len_max <= SPINEL_MAX_PACK_LENGTH, bail, (ret = -1, errno = EINVAL));

    if (data_out == NULL)
    {
        data_len_max = 0;
    }

    for (; *pack_format != 0; pack_format = spinel_next_packed_datatype(pack_format))
    {
        if (*pack_format == ')')
        {
            // Don't go past the end of a struct.
            break;
        }

        switch ((spinel_datatype_t)*pack_format)
        {
        case SPINEL_DATATYPE_BOOL_C:
        {
            bool arg = (bool)va_arg(args->obj, int);
            ret += sizeof(uint8_t);

            if (data_len_max >= sizeof(uint8_t))
            {
                data_out[0] = (arg != false);
                data_out += sizeof(uint8_t);
                data_len_max -= sizeof(uint8_t);
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_INT8_C:
        case SPINEL_DATATYPE_UINT8_C:
        {
            uint8_t arg = (uint8_t)va_arg(args->obj, int);
            ret += sizeof(uint8_t);

            if (data_len_max >= sizeof(uint8_t))
            {
                data_out[0] = arg;
                data_out += sizeof(uint8_t);
                data_len_max -= sizeof(uint8_t);
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_INT16_C:
        case SPINEL_DATATYPE_UINT16_C:
        {
            uint16_t arg = (uint16_t)va_arg(args->obj, int);
            ret += sizeof(uint16_t);

            if (data_len_max >= sizeof(uint16_t))
            {
                data_out[1] = (arg >> 8) & 0xff;
                data_out[0] = (arg >> 0) & 0xff;
                data_out += sizeof(uint16_t);
                data_len_max -= sizeof(uint16_t);
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_INT32_C:
        case SPINEL_DATATYPE_UINT32_C:
        {
            uint32_t arg = (uint32_t)va_arg(args->obj, int);
            ret += sizeof(uint32_t);

            if (data_len_max >= sizeof(uint32_t))
            {
                data_out[3] = (arg >> 24) & 0xff;
                data_out[2] = (arg >> 16) & 0xff;
                data_out[1] = (arg >> 8) & 0xff;
                data_out[0] = (arg >> 0) & 0xff;
                data_out += sizeof(uint32_t);
                data_len_max -= sizeof(uint32_t);
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_INT64_C:
        case SPINEL_DATATYPE_UINT64_C:
        {
            uint64_t arg = va_arg(args->obj, uint64_t);

            ret += sizeof(uint64_t);

            if (data_len_max >= sizeof(uint64_t))
            {
                data_out[7] = (arg >> 56) & 0xff;
                data_out[6] = (arg >> 48) & 0xff;
                data_out[5] = (arg >> 40) & 0xff;
                data_out[4] = (arg >> 32) & 0xff;
                data_out[3] = (arg >> 24) & 0xff;
                data_out[2] = (arg >> 16) & 0xff;
                data_out[1] = (arg >> 8) & 0xff;
                data_out[0] = (arg >> 0) & 0xff;
                data_out += sizeof(uint64_t);
                data_len_max -= sizeof(uint64_t);
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_IPv6ADDR_C:
        {
            spinel_ipv6addr_t *arg = va_arg(args->obj, spinel_ipv6addr_t *);
            ret += sizeof(spinel_ipv6addr_t);

            if (data_len_max >= sizeof(spinel_ipv6addr_t))
            {
                *(spinel_ipv6addr_t *)data_out = *arg;
                data_out += sizeof(spinel_ipv6addr_t);
                data_len_max -= sizeof(spinel_ipv6addr_t);
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_EUI48_C:
        {
            spinel_eui48_t *arg = va_arg(args->obj, spinel_eui48_t *);
            ret += sizeof(spinel_eui48_t);

            if (data_len_max >= sizeof(spinel_eui48_t))
            {
                *(spinel_eui48_t *)data_out = *arg;
                data_out += sizeof(spinel_eui48_t);
                data_len_max -= sizeof(spinel_eui48_t);
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_EUI64_C:
        {
            spinel_eui64_t *arg = va_arg(args->obj, spinel_eui64_t *);
            ret += sizeof(spinel_eui64_t);

            if (data_len_max >= sizeof(spinel_eui64_t))
            {
                *(spinel_eui64_t *)data_out = *arg;
                data_out += sizeof(spinel_eui64_t);
                data_len_max -= sizeof(spinel_eui64_t);
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_UINT_PACKED_C:
        {
            uint32_t       arg = va_arg(args->obj, uint32_t);
            spinel_ssize_t encoded_size;

            // Range Check
            // require_action(arg < SPINEL_MAX_UINT_PACKED, bail, {
            //     ret   = -1;
            //     errno = EINVAL;
            // });

            encoded_size = spinel_packed_uint_encode(data_out, data_len_max, arg);
            ret += encoded_size;

            if ((spinel_ssize_t)data_len_max >= encoded_size)
            {
                data_out += encoded_size;
                data_len_max -= (spinel_size_t)encoded_size;
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_UTF8_C:
        {
            const char *string_arg     = va_arg(args->obj, const char *);
            size_t      string_arg_len = 0;

            if (string_arg)
            {
                string_arg_len = strlen(string_arg) + 1;
            }
            else
            {
                string_arg     = "";
                string_arg_len = 1;
            }

            ret += (spinel_size_t)string_arg_len;

            if (data_len_max >= string_arg_len)
            {
                memcpy(data_out, string_arg, string_arg_len);

                data_out += string_arg_len;
                data_len_max -= (spinel_size_t)string_arg_len;
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case SPINEL_DATATYPE_DATA_WLEN_C:
        case SPINEL_DATATYPE_DATA_C:
        {
            const uint8_t *arg           = va_arg(args->obj, const uint8_t *);
            uint32_t       data_size_arg = va_arg(args->obj, uint32_t);
            spinel_ssize_t size_len      = 0;
            char           nextformat    = *spinel_next_packed_datatype(pack_format);

            if ((pack_format[0] == SPINEL_DATATYPE_DATA_WLEN_C) || ((nextformat != 0) && (nextformat != ')')))
            {
                size_len = spinel_datatype_pack(data_out, data_len_max, SPINEL_DATATYPE_UINT16_S, data_size_arg);
                // require_action(size_len > 0, bail, {
                //     ret   = -1;
                //     errno = EINVAL;
                // });
            }

            ret += (spinel_size_t)size_len + data_size_arg;

            if (data_len_max >= (spinel_size_t)size_len + data_size_arg)
            {
                data_out += size_len;
                data_len_max -= (spinel_size_t)size_len;

                if (data_out && arg)
                {
                    memcpy(data_out, arg, data_size_arg);
                }

                data_out += data_size_arg;
                data_len_max -= data_size_arg;
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case 'T':
        case SPINEL_DATATYPE_STRUCT_C:
        {
            spinel_ssize_t struct_len = 0;
            spinel_ssize_t size_len   = 0;
            char           nextformat = *spinel_next_packed_datatype(pack_format);

            // require_action(pack_format[1] == '(', bail, {
            //     ret   = -1;
            //     errno = EINVAL;
            // });

            // First we figure out the size of the struct
            {
                va_list_obj subargs;
                va_copy(subargs.obj, args->obj);
                struct_len = spinel_datatype_vpack_(NULL, 0, pack_format + 2, &subargs);
                va_end(subargs.obj);
            }

            if ((pack_format[0] == SPINEL_DATATYPE_STRUCT_C) || ((nextformat != 0) && (nextformat != ')')))
            {
                size_len = spinel_datatype_pack(data_out, data_len_max, SPINEL_DATATYPE_UINT16_S, struct_len);
                // require_action(size_len > 0, bail, {
                //     ret   = -1;
                //     errno = EINVAL;
                // });
            }

            ret += size_len + struct_len;

            if (struct_len + size_len <= (spinel_ssize_t)data_len_max)
            {
                data_out += size_len;
                data_len_max -= (spinel_size_t)size_len;

                struct_len = spinel_datatype_vpack_(data_out, data_len_max, pack_format + 2, args);

                data_out += struct_len;
                data_len_max -= (spinel_size_t)struct_len;
            }
            else
            {
                data_len_max = 0;
            }

            break;
        }

        case '.':
            // Skip.
            break;

        default:
            // Unsupported Type!
            ret   = -1;
            // errno = EINVAL;
            goto bail;
        }
    }

bail:
    return ret;
}

spinel_ssize_t spinel_datatype_pack(uint8_t *data_out, spinel_size_t data_len_max, const char *pack_format, ...)
{
    int         ret;
    va_list_obj args;
    va_start(args.obj, pack_format);

    ret = spinel_datatype_vpack_(data_out, data_len_max, pack_format, &args);

    va_end(args.obj);
    return ret;
}

spinel_ssize_t spinel_datatype_vpack(uint8_t *     data_out,
                                     spinel_size_t data_len_max,
                                     const char *  pack_format,
                                     va_list       args)
{
    int         ret;
    va_list_obj args_obj;
    va_copy(args_obj.obj, args);

    ret = spinel_datatype_vpack_(data_out, data_len_max, pack_format, &args_obj);

    va_end(args_obj.obj);
    return ret;
}