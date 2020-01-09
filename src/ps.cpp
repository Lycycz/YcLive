#pragma once
#include "ps.h"
#include "common.h"

int Ps::ParseHeader(char* pBuf, int nLen, int& nHeadLen)
{
    int Pos = 0;
    /*
    program_stream_pack_header* ps = reinterpret_cast<program_stream_pack_header*>
        (pBuf);
    if(!is_ps_header(ps))
    {
        return -1;
    }

    int nStuffLen = ps->pack_stuffing_length;

    Pos += (14 + nStuffLen);
    if(Pos + 18 > nLen)
    {
        return -1;
    }

    program_stream_pack_bb_header* sh = reinterpret_cast<program_stream_pack_bb_header*>
        (pBuf + Pos);

    if(is_sh_header(sh))
    {
        int shLen = util::EndianUshort(sh->header_length);
        Pos += 
    }
    */
    int ret;
    while(nLen >= sizeof(pack_start_code))
    {
        Header_Type header_type = parse_header_type(pBuf);
        switch(header_type){
            // 000001BA
            case Ps_Head:
            {
                ret = ParsePsHeader(pBuf, nLen, Pos);
                if(ret < 0)
                {
                    //"parse ps header error"
                    return -1;
                }
            }
            // 000001BB
            case Sh_Head:
            {
                ret = ParseShHeader(pBuf, nLen, Pos);
                if(ret < 0)
                {
                    //"parse sh header error"
                    return -1;
                }
            }
            // 000001BC
            case Psm_Head:
            {
                ret = ParsePsmHeader(pBuf, nLen, Pos);
                if(ret < 0)
                {
                    //"parse psm header error"
                    return -1;
                }
            }
            default:
                break;

        }
    }
    return 0;
}

int Ps::ParsePsHeader(char* pBuf, int nLen, int& pos)
{
    program_stream_pack_header* header = reinterpret_cast<program_stream_pack_header*>
        (pBuf + pos);
    pos += (14 + header->pack_stuffing_length);
    if(pos + 18 > nLen)
        return -1;
    return 0;
}

int Ps::ParseShHeader(char* pBuf, int nLen, int& pos)
{
    program_stream_pack_bb_header* header = reinterpret_cast<program_stream_pack_bb_header*>
        (pBuf + pos);
    pos += (6 + util::EndianUshort(header->header_length));
    if(pos + 36 > nLen)
        return -1;
    return 0;
}

int Ps::ParsePsmHeader(char* pBuf, int nLen, int& pos) 
{
    program_stream_pack_map_header* header = reinterpret_cast<program_stream_pack_map_header*>
        (pBuf + pos);
    pos += (6 + util::EndianUshort(header->header_length));
    return 0;
}

int Ps::ParsePes(char* pBuf, int nLen)
{
    return 0;
}