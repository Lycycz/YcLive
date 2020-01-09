#include "pes.h"

int Pes::InputBuffer(char* pBuf, int nLen)
{
    pes_header* pes = reinterpret_cast<pes_header*>(pBuf);
    PesType pestype = pes_type(pBuf);
    if(pestype != pes_video)
    {

        return -1;
    }
    optianol_pes_header* option = reinterpret_cast<optianol_pes_header*>
        (pBuf + sizeof(pes_header));
    if (option->PTS_DTS_flags != 2 && option->PTS_DTS_flags != 3 && option->PTS_DTS_flags != 0)
    {
        //Log::error("PTS_DTS_flags is 01 which is invalid PTS_DTS_flags");
        return -1;
    }
    
    int pesLen = util::EndianUshort(pes->pes_packet_length);

    char* pEsBuf = reinterpret_cast<char*>(option) + sizeof(optianol_pes_header) 
                + option->PES_header_data_length;
    int nEsLen = pesLen - sizeof(optianol_pes_header) + option->PES_header_data_length;

    return 0;
}   