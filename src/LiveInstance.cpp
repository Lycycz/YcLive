#include "LiveInstance.h"
#include "ps.h"
#include "rtp.h"

CLiveInstance::CLiveInstance()
{
    m_pRtpParser = new Rtp();
}

void CLiveInstance::RtpRecv(char* pBuf, long nLen)
{
    Rtp* rtpAnalyzer = static_cast<Rtp*>(m_pRtpParser);
    rtpAnalyzer->InputBuffer(pBuf, nLen);
}
    