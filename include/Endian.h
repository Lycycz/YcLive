#pragma once
#include <stdint.h>

namespace util
{
static inline char* EndianChange(char* src, int bytes);

static inline unsigned short EndianUshort(unsigned short value);

static inline unsigned int EndianUint(unsigned int value);

static inline unsigned short EndianUshort(unsigned short value)
{
    return ((value & 0x00FF) << 8 ) | ((value & 0xFF00) >> 8);
}

static inline unsigned int EndianUint(unsigned int value) 
{
    return ((value & 0x000000FF) << 24) |  ((value & 0x0000FF00) << 8) 
        | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

}
