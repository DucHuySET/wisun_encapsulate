#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "fcs_computation.h"
#include "hdlc.h"

Decoder_state mDecoderState = {.mState = kStateCompleted, .mDecodedLength = 0, .mFcs = 0};

uint16_t pppfcs16(uint16_t fcs, uint8_t cp)
{
    fcs = (fcs >> 8) ^ fcstab[(fcs ^ cp) & 0xff];
    return fcs;
}

void writeAndUpdateState(int8_t *result, uint8_t byte)
{
    mDecoderState.mFcs = pppfcs16(mDecoderState.mFcs, byte);
    writeByte(result, byte);
    mDecoderState.mDecodedLength++;
    mDecoderState.mState = kStateNotCompleted;
}

void writeByte(uint8_t *result, uint8_t byte)
{
    // TODO: implement check result buffer remain length
    result[mDecoderState.mDecodedLength] = byte;
}

void encode_byte(unsigned char byte, unsigned char *packet, int *index)
{
    if (byte == HDLC_ESCAPE || byte == HDLC_FLAG)
    {
        packet[(*index)++] = HDLC_ESCAPE;
        packet[(*index)++] = byte ^ 0x20;
    }
    else
    {
        packet[(*index)++] = byte;
    }
}

void hdlc_decode(const uint8_t *aData, uint8_t *result, int aLength)
{
    int ptr = 0;
    while (ptr < aLength)
    {
        uint8_t byte = aData[ptr];
        switch (mDecoderState.mState)
        {
        case kStateCompleted:
            if (byte == HDLC_FLAG)
            {
                mDecoderState.mState = kStateNotCompleted;
                mDecoderState.mDecodedLength = 0;
                mDecoderState.mFcs = HDLC_FCS_INIT;
            }
            break;
        case kStateNotCompleted:
            switch (byte)
            {
            case HDLC_ESCAPE:
                ptr++;
                byte = aData[ptr];
                byte ^= 0x20;
                writeAndUpdateState(result, byte);
                break;
            case HDLC_FLAG:
                if (mDecoderState.mDecodedLength > 0)
                {
                    if ((mDecoderState.mDecodedLength >= FCS_SIZE) && (mDecoderState.mFcs == HDLC_FCS_GOOD))
                    {
                        // Remove the FCS from the frame.
                        result[mDecoderState.mDecodedLength - FCS_SIZE]  = '\0';
                    }
                }

                mDecoderState.mDecodedLength = 0;
                mDecoderState.mFcs = HDLC_FCS_INIT;
                break;
            default:
                writeAndUpdateState(result, byte);
                break;
            }
        }
        ptr++;
    }
}

void hdlc_encode(const uint8_t *aData, uint8_t *result) // TODO: add uint8_t result array
{
    uint16_t fcs = HDLC_FCS_INIT;
    int index = 0;

    result[index++] = HDLC_FLAG;

    for (int i = 0; aData[i] != '\0'; i++)
    {
        fcs = pppfcs16(fcs, aData[i]);
        encode_byte(aData[i], result, &index);
    }

    fcs ^= 0xFFFF;
    encode_byte(fcs & 0xFF, result, &index);
    encode_byte(fcs >> 8, result, &index);

    result[index++] = HDLC_FLAG;
}