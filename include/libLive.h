/**
 * ���ļ���rtspģ��Ψһ���⵼����ͷ�ļ�
 */
#pragma once
#include <string>
#ifdef RTSP_EXPORTS
#define RTSP_API __declspec(dllexport)
#else
#define RTSP_API
#endif

enum NalType;
enum flv_tag_type;

/**
 * ���ݻص������ӿ�
 * ���ϲ�ʵ��һ���̳иýӿڵ���������RTP�����������
 */
struct IlibLiveCb
{
    /**
     * FLV���ݴ����ӿ�
     */
    virtual void push_flv_frame(int tag_type, char* frames, int frames_size) = 0;

    /**
     * TS���ݴ����ӿ�
     */
    virtual void push_ts_stream(char* pBuff, int nBuffSize) = 0;

    /**
     * h264���������ӿ�
     */
    virtual void push_h264_stream(NalType eType, char* pBuff, int nBuffSize) = 0;

    /**
     * rtsp���ն˽���
     */
    virtual void stop() = 0;

    bool        m_bFlv;
    bool        m_bTs;
    bool        m_bH264;
    IlibLiveCb():m_bFlv(false),m_bTs(false),m_bH264(false){}
};

/**
 * 解析接口
 */
struct IAnalyzer
{
    /**
     * 插入一个PES包
     * @param[in] pBuf PES帧
     * @param[in] nLen PES帧长度
     * @return 0成功 -1失败
     */
    virtual int InputBuffer(char* pBuf, long nLen) = 0;
};

struct RTSP_API IlibLive
{
    virtual ~IlibLive(){}

    /**
     * ����һ��ӵ�б��ӿڹ��ܵ�ʵ��
     * return ʵ����ָ�룬ע��Ҫ���������ͷ�
     */
    static IlibLive* CreateObj();

    /**
     * ����flvͷ
     */
    static bool MakeFlvHeader(char** ppBuff, int* pLen);

    /**
     * ���ñ��ؼ�����IP�Ͷ˿�
     * @param[in] strIP ����IP
     * @param[in] nPort ������UDP�˿�
     */
    virtual void SetLocalAddr(std::string strIP, int nPort) = 0;

    /**
     * ���û���֡����
     * @param[in] nPacketNum ֡��������,��ֵԽ���ӳ�Խ�󣬵���Ӧ�Ը��������״��
     */
    virtual void SetCatchPacketNum(int nPacketNum) = 0;

    /** 
     * ����UDP�˿ڼ��� 
     */
    virtual void StartListen() = 0;

    /**
     * ���ûص���������
     * @param[in] pHandle �ص���������ָ��
     */
    virtual void SetCallback(IlibLiveCb* pHandle) = 0;
};
