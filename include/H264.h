#pragma once
#include "stdint.h"
enum NalType
{
    unknow = 0,
    b_Nal   = 1,  // B Slice,非关键帧
    dpa_Nal = 2,
    dpb_Nal = 3,
    pdc_Nal = 4,
    idr_Nal = 5,  // IDR ,关键帧
    sei_Nal = 6,  // SEI,补充增强信息
    sps_Nal = 7,  // SPS,序列参数集
    pps_Nal = 8,  // PPS,图像参数集
    aud_Nal = 9,
    filler_Nal = 12,
    other,        // 其他类型
};

struct nal_head {
    uint8_t     nal_type : 5;
    uint8_t     nal_ref_ide : 2;
    uint8_t     for_bit : 1;
};

class H264
{
public:
    H264();
    ~H264();
    void SetBuff(char* nal_str, uint32_t nLen);
    NalType NaluType();
private:
    void ParseNalu();

private:
    char*       m_pNaluBuff;
    uint32_t    m_nBuffLen;
    char*       m_pDataBuff;
    uint32_t    m_nDataLen;
    NalType     m_eNaluType;
    int32_t     m_nWidth;
    int32_t     m_nHeight;
    double      m_nFps;
};