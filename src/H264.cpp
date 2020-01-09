#include "H264.h"

H264::H264()
    : m_pNaluBuff(nullptr), m_nBuffLen(0), m_pDataBuff(nullptr)
    , m_nDataLen(0), m_eNaluType(NalType::unknow), m_nWidth(0), m_nHeight(0)
{}

H264::~H264()
{}

void H264::SetBuff(char* nal_str, uint32_t nLen)
{
    m_pNaluBuff = nal_str;
    m_nBuffLen = nLen;
    ParseNalu();
}

void H264::ParseNalu()
{
    m_eNaluType = NalType::unknow;

    if(m_pNaluBuff[0] == 0 && 
       m_pNaluBuff[1] == 0 &&
       m_pNaluBuff[2] == 0 &&
       m_pNaluBuff[3] == 1 )
    {
        m_pDataBuff = m_pNaluBuff + 4;
        m_nDataLen = m_nBuffLen - 4;
    }
    else if(m_pNaluBuff[0] == 0 &&
            m_pNaluBuff[1] == 0 &&
            m_pNaluBuff[2] == 1)
    {
        m_pDataBuff = m_pNaluBuff + 3;
        m_nDataLen = m_nBuffLen - 3;
    }
    else 
        return;
    
    nal_head* nal_head_ = reinterpret_cast<nal_head*>(m_pDataBuff);
    if(nal_head_->nal_type != NalType::sps_Nal      && 
       nal_head_->nal_type != NalType::pps_Nal      &&
       nal_head_->nal_type != NalType::idr_Nal      &&
       nal_head_->nal_type != NalType::pdc_Nal      &&
       nal_head_->nal_type != NalType::b_Nal)
    {
        m_eNaluType = NalType::other;
        return;
    }
    
    m_eNaluType = static_cast<NalType>(nal_head_->nal_type);
}

NalType H264::NaluType()
{
    return m_eNaluType;
}