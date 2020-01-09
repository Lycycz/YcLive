/*
parse rtp to ps 
*/
#pragma once
#include "common.h"
#include <map>
#include <list>
#include <utility>

#define RTP_HEADER_SIZE     12
#define RTP_VERSION         2
#define FRAME_MAX_SIZE      (1024*1024*3)

#define ERROR_SUCCESS                0
#define ERROR_RTP_LENGTH             1001
#define ERROR_RTP_VERSION            1002
#define ERROR_RTP_PT                 1003
#define ERROR_RTP_SEQ                1004
#define ERROR_RTP_MEMORY             1005
#define ERROR_RTP_WAIT               1006
#define ERROR_RTP_FORMAT             1007
#define ERROR_RTP_PARAM              1008
struct RtpHeader {
    unsigned char cc:4;	 /* CSRC count */
    unsigned char x:1;	 /* header extension flag */
    unsigned char p:1;	 /* padding flag */
    unsigned char v:2;	 /* packet type */
    unsigned char pt:7;	 /* payload type */
    unsigned char m:1;	 /* marker bit */
    unsigned short seq;	 /* sequence number */
    unsigned int ts;	 /* timestamp */
    unsigned int ssrc;	 /* synchronization source */
};

struct rtp_pkt_element {
    char*   data;
    int     len;
    int     head_len;
    int     payload_len;
    rtp_pkt_element():data(nullptr), len(0),
        head_len(0), payload_len(0)
    {}
};

typedef std::map<unsigned short, rtp_pkt_element*> RtpPktMap;
typedef std::list<rtp_pkt_element*> ListRtpFrame;
class Rtp {
public:
    Rtp();
    int InputBuffer(char* pbuffer, int plen);

private:
    int InsertSortList(char* packetbuffer, int packetsize);
    int ParseHeader(char* pbuffer, int size, int& nHeaderSize, int& nPayloadSize);
    int ComposePsFream();
    int DelRtpEle(rtp_pkt_element*);



private:
    char*           m_frame_buf;
    unsigned short  m_DoneSeq;
    // 按照seq排列的rtp packet 数据
    RtpPktMap       m_rtppktmap;
    // 可以连续组成一帧的 rtp packet 数据
    ListRtpFrame    m_listrtpframe;

};