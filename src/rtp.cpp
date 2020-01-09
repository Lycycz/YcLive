#include "rtp.h"
#include "ps.h"

Rtp::Rtp(): m_frame_buf(nullptr), m_DoneSeq(0)
{
    m_frame_buf = new char[FRAME_MAX_SIZE];
    memset(m_frame_buf, '\0', FRAME_MAX_SIZE);
}

int Rtp::InputBuffer(char* pbuffer, int plen)
{
    if(pbuffer == nullptr || plen < RTP_HEADER_SIZE)
        return -1;
    RtpHeader* head = reinterpret_cast<RtpHeader*>(pbuffer);
    head->seq = util::EndianUshort(head->seq);
    head->ssrc = util::EndianUint(head->ssrc);
    head->ts = util::EndianUint(head->ts);

    /*
    if(head->pt != 96)
        return -1;
    */

    return InsertSortList(pbuffer, plen);
}
int Rtp::InsertSortList(char* packetbuffer, int packetsize) 
{
    RtpHeader *rtp_pkt_ = reinterpret_cast<RtpHeader*>(packetbuffer);
    if(rtp_pkt_->seq < m_DoneSeq)
        return ERROR_RTP_SEQ;

    int nHeaderSize;
    int nPayloadSize;
    int ret = ParseHeader(packetbuffer, packetsize, nHeaderSize, nPayloadSize);
    if(ret != ERROR_SUCCESS){
        return -1;
    }   

    rtp_pkt_element* pkt_ele_ = new rtp_pkt_element();
    pkt_ele_->data = packetbuffer;
    pkt_ele_->head_len = nHeaderSize;
    pkt_ele_->len = packetsize;
    pkt_ele_->payload_len = nPayloadSize;

    // map 按照 key 升序排列
    m_rtppktmap.insert(std::make_pair(rtp_pkt_->seq, pkt_ele_));

    pkt_ele_ = nullptr;
    RtpHeader* pHeader = nullptr;
    char* PsHead = nullptr;
    uint16_t seqLast = m_DoneSeq;

    if(rtp_pkt_->m != 0) {
        auto pkt_pos = m_rtppktmap.begin();
        auto rtp_frame_first = m_rtppktmap.begin();
        auto rtp_frame_last = m_rtppktmap.begin();

        for(;pkt_pos != m_rtppktmap.end(); pkt_pos++) {
            pkt_ele_ = pkt_pos->second;
            pHeader = reinterpret_cast<RtpHeader*>(pkt_ele_->data);
            PsHead = static_cast<char*>(pkt_ele_->data + pkt_ele_->head_len);

            if(!is_ps_header(PsHead)){
                break;
            }
            else {
                if(pkt_pos->first != m_DoneSeq + 1 )
                    break;
            }

            seqLast = pkt_pos->first;

            m_listrtpframe.clear();
            for(auto pos = rtp_frame_first; pos != rtp_frame_last; ++pos)
                m_listrtpframe.push_back(pos->second);
            ComposePsFream();
            m_DoneSeq = rtp_frame_last->first;
            
            for(auto pos = m_rtppktmap.begin(); pos != rtp_frame_last; )
            {
                DelRtpEle(pos->second);
                pos = m_rtppktmap.erase(pos);
            }
        }

    }


    // 每次进入缓存 判断
    /*
    auto pkt_pos = m_rtppktmap.begin();
    auto rtp_frame_first = m_rtppktmap.begin();
    auto rtp_frame_last = m_rtppktmap.begin();

    for(;pkt_pos != m_rtppktmap.end(); pkt_pos++) {
        pkt_ele_ = pkt_pos->second;
        pHeader = reinterpret_cast<RtpHeader*>(pkt_ele_->data);
        PsHead = reinterpret_cast<pack_start_code*>(pkt_ele_->data + pkt_ele_->head_len);

        
    }

    if(rtp_pkt_->m != 0) {

    }
    */

    return 0;
}
int Rtp::ParseHeader(char* pbuffer, int size, int& nHeaderSize, int& nPayloadSize)
{
    if(size < RTP_HEADER_SIZE){
        return ERROR_RTP_LENGTH;
    }
    RtpHeader *rtp_header_ = reinterpret_cast<RtpHeader*>(pbuffer);

    nHeaderSize = RTP_HEADER_SIZE;
    nPayloadSize = size - nHeaderSize;

    if(rtp_header_->v != RTP_VERSION)
        return ERROR_RTP_VERSION;

    if(rtp_header_->p) {
        int PaddingLength = pbuffer[size-1];
        if(PaddingLength + 12 > size)
        {
            return ERROR_RTP_LENGTH;
        }
        nPayloadSize -= PaddingLength;
    }

    return ERROR_SUCCESS;
}

int Rtp::DelRtpEle(rtp_pkt_element* pEle) {

    SAFE_DELETE_ARRAY(pEle->data);

    SAFE_DELETE(pEle);
    return 0;
}

int Rtp::ComposePsFream()
{
    int Pslen = 0;
    for(auto pkt: m_listrtpframe)
    {
        memcpy(m_frame_buf, pkt->data + pkt->head_len, pkt->payload_len);
        Pslen += pkt->payload_len;
        return -1;
    }
    
    return 0;
}