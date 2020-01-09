/*
parse pes 
*/
#pragma once
#include <stdint.h>
#include "common.h"
enum PesType
{
    pes_unknow,
    pes_video,
    pes_audio,
    pes_jump    //需要跳过的pes
};

#pragma pack(1)
struct pes_header {
    uint8_t                     start_code[3];
    uint8_t                     stream_id; 
    uint16_t                    pes_packet_length;
};

struct optianol_pes_header {
    unsigned char original_or_copy : 1;
    unsigned char copyright : 1;
    unsigned char data_alignment_indicator : 1;
    unsigned char PES_priority : 1;
    unsigned char PES_scrambling_control : 2;
    unsigned char fix_bit : 2;

    unsigned char PES_extension_flag : 1;
    unsigned char PES_CRC_flag : 1;
    unsigned char additional_copy_info_flag : 1;
    unsigned char DSM_trick_mode_flag : 1;
    unsigned char ES_rate_flag : 1;
    unsigned char ESCR_flag : 1;
    unsigned char PTS_DTS_flags : 2;

    unsigned char PES_header_data_length;
};
#pragma pack()

bool inline is_pes_header(char* pack)
{
    if(pack[0] == 0 && pack[1] == 0 && pack[2] == 1)
    {
        if(pack[3] == '\xC0' || pack[3] == '\xE0')
            return true;
    }
    return false;
}

PesType inline pes_type(char* pack)
{
    if(pack[0] == 0 && pack[1] == 0 && pack[2] == 1)
    {
        switch(pack[3]){
            case ('\xC0'):
                return PesType::pes_audio;
            case ('\xE0'):
                return PesType::pes_video;
            case ('\xBC'):
            case ('\xBD'):
            case ('\xBE'):
            case ('\xBF'):
            case ('\xF0'):
            case ('\xF1'):
            case ('\xF2'):
            case ('\xF8'):
                return PesType::pes_jump;
            default:
                return PesType::pes_unknow;
        }
    }
    return PesType::pes_unknow;
}

class Pes{
public:
    Pes();
    ~Pes();

    int InputBuffer(char* pBuf, int nLen);

private:
};