/*
parse ps to pes
 */
#pragma once
#include "libLive.h"
#include "stdint.h"
struct pack_start_code {
    unsigned char start_code[3];
    unsigned char stream_id[1];
};
struct program_stream_pack_header_content
{
    /** 8 */
    unsigned char system_clock_reference_base21 : 2;     // SCR 29..28
    unsigned char marker_bit : 1;                        // '1'
    unsigned char system_clock_reference_base1 : 3;      // SCR 31..30
    unsigned char fix_bit : 2;                           // '01'
    /** 8 */
    unsigned char system_clock_reference_base22;         // SCR 27..20
    /** 8 */
    unsigned char system_clock_reference_base31 : 2;     // SRC 14..13
    unsigned char marker_bit1 : 1;                       // '1'
    unsigned char system_clock_reference_base23 : 5;     // SRC 19..15
    /** 8 */
    unsigned char system_clock_reference_base32;         // SRC 12..5
    /** 8 */
    unsigned char system_clock_reference_extension1 : 2; // SRCE 8..7
    unsigned char marker_bit2 : 1;                       // '1'
    unsigned char system_clock_reference_base33 : 5;     // SRC 4..0
    /** 8 */
    unsigned char marker_bit3 : 1;                       // '1'
    unsigned char system_clock_reference_extension2 : 7; // SRCE 6..0
    /** 8 */
    unsigned char program_mux_rate1;                     // PMR 21..14
    /** 8 */
    unsigned char program_mux_rate2;                     // PMR 13..6
    /** 8 */
    unsigned char marker_bit5 : 1;
    unsigned char marker_bit4 : 1;
    unsigned char program_mux_rate3 : 6;                 // PMR 0..5
};
struct program_stream_pack_header
{
	pack_start_code                         PackStart;          // 4
    program_stream_pack_header_content      PackContent;        // 9
    unsigned char                           pack_stuffing_length : 3;
    unsigned char                           reserved : 5;
};
struct program_stream_pack_bb_header_content
{
    uint32_t marker_bit1 : 1;                           //1  bslbf
    uint32_t rate_bound : 22;                           // 速率界限字段
    uint32_t marker_bit2 : 1;                           //1 bslbf
    /** 8 */
    uint32_t audio_bound : 6;                           // 音频界限字段
    uint32_t fixed_flag : 1;                            // 置'1'时表示比特率恒定的操作；置'0'时，表示操作的比特率可变。
    uint32_t CSPS_flag : 1;                             // 置'1'时，节目流符合2.7.9中定义的限制
    /** 8 */
    uint16_t system_audio_lock_flag : 1;                // 系统音频锁定标志字段
    uint16_t system_video_lock_flag : 1;                // 系统视频锁定标志字段
    uint16_t marker_bit3 : 1;                           // bslbf
    uint16_t video_bound : 5;                           // 视频界限字段
    /** 8 */
    uint16_t packet_rate_restriction_flag : 1;          // 分组速率限制标志字段
    uint16_t reserved_bits : 7;                         // '1111111'
    /** 8*6 不定长，stream_id P-STD_buffer_bound_scale P-STD_buffer_size_bound */
    unsigned char reserved[6];
};
struct program_stream_pack_bb_header
{
    uint8_t                                 system_header_start_code[4];
    uint16_t                                header_length;
    program_stream_pack_bb_header_content   PackBBContent;
};

struct program_stream_pack_map_header_content
{
    unsigned char program_stream_map_version : 5;      // 节目流映射版本字段
    unsigned char reserved1 : 2;                       // 保留
    unsigned char current_next_indicator : 1;          // 置'1'时表示传送的节目流映射当前是可用的。置'0'时表示传送的节目流映射还不可用，但它将是下一个生效的表
    /** 8 */
    unsigned char marker_bit : 1;
    unsigned char reserved2 : 7;
    /** 8*2 */
    unsigned char program_stream_info_length[2];       // 节目流信息长度字段
    /** 8*2 */
    unsigned char elementary_stream_map_length[2];     // 基本流映射长度字段
    /** 8*4 这几个为一组，由elementary_stream_map_length确定有几组 */
    unsigned char stream_type;
    unsigned char elementary_stream_id;
    unsigned char elementary_stream_info_length[2];
    /** 8*4 */
    unsigned char CRC_32[4];
    /** 8*16 */
    unsigned char reserved[16];
};

struct program_stream_pack_map_header
{
    uint8_t                                 start_code[4];
    uint16_t                                header_length;
    program_stream_pack_map_header_content  PackMapContent;
};

enum Header_Type {
    Ps_Head     = 0,
    Sh_Head     = 1,
    Psm_Head    = 2,
    Other_Head  = 5,
    Err_Head    
};

Header_Type inline parse_header_type(char* pack){
    if(pack[0] == '\x00' && 
       pack[1] == '\x00' && 
       pack[2] == '\x01' ) 
    {
        switch(pack[3])
        {
        case ('\xBA'):
            return Ps_Head;
        case ('\xBB'):
            return Sh_Head;
        case ('\xBC'):
            return Psm_Head;
        default:
            return Other_Head;
        }
    }
    else
        return Err_Head;

}
bool inline is_ps_header(char* pack){
    if(pack[0] == '\x00' && 
       pack[1] == '\x00' && 
       pack[2] == '\x01' &&
       pack[3] == '\xBA')
        return true;
    return false;
}

bool inline is_sh_header(program_stream_pack_bb_header* pack){
    if(pack->system_header_start_code[0] == 0 &&
       pack->system_header_start_code[1] == 0 &&
       pack->system_header_start_code[2] == 0 &&
       pack->system_header_start_code[3] == 0xBB 
    )
        return true;
    return false;
}

class Ps {
public:
    Ps();
    ~Ps();
    int inPutBuffer(char* pBuf, int nLen);

private:
    int ParseHeader(char* pBuf, int nLen, int& nHeadLen);
    int ParsePsHeader(char* pBuf, int nLen, int& pos);
    int ParseShHeader(char* pBuf, int nLen, int& pos);
    int ParsePsmHeader(char* pBuf, int nLen, int& pos);
    int ParsePes(char* pBuf, int nLen);

private:

};