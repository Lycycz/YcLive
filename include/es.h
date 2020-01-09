/*
es -> h264
*/
#include "H264.h"

int inline is_nal_head(char* pack){
    if(pack[0] == '\x00' && pack[1] == '\x00' && pack[2] == '\x01')
        return 3;
    if(pack[0] == '\x00' && pack[1] == '\x00' && pack[2] == '\x00' && pack[3] == '\x01')
        return 4;
    else
        return -1;
}

class Es {
public:
    Es();
    ~Es();

    int InputBuffer(char* pBuf, int nLen);

private:
};