#include "es.h"

int Es::InputBuffer(char* pBuf, int nLen)
{
    int pos = 0;
    uint8_t nNalType = 0;
    int Buf_begin_pos = 0;
    char* Buf_begin = pBuf;

    while(pos < nLen)
    {
        char* pPos = pBuf + pos;
        int offset = is_nal_head(pPos);
        if(!offset){
            pos++;
            continue;
        }
        nal_head* header = reinterpret_cast<nal_head*>(pPos + offset);
        nNalType = header->nal_type;
        if(pos > Buf_begin_pos)
        {}
        Buf_begin_pos = pos;
        Buf_begin = pPos;
        pos += offset;
    }
    return 0;
}