#pragma comment(lib, "libuv.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libuv.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Userenv.lib")

#include "uv.h"
#include <iostream>
#include "LiveInstance.h"

struct udp_recv_loop_t {
    uv_loop_t   uvLoop;           // udp���յ�loop
    uv_udp_t    uvRtpSocket;      // rtp����
    uv_timer_t  uvTimeOver;       // ���ճ�ʱ��ʱ��
    uv_async_t  uvAsync;          // �ⲿ�߳�֪ͨ����loop
    bool        running;          // loop�߳��Ƿ�����ִ��
    int         uvHandleNum;      // ������uv���������Щ���ͨ��uv_close�ر�
    std::string remoteIP;         // ���ͷ�IP
    int         remotePort;       // ���ͷ��˿�
    int         port;
    void       *user;             // �û�����
};

static void echo_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    *buf = uv_buf_init((char*)malloc(suggested_size), suggested_size);
}

static void on_read(uv_udp_t *handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr*addr, unsigned flags) {
    udp_recv_loop_t* loop = (udp_recv_loop_t*)handle->data;
    if(nread < 0) {
        free(buf->base);
    }
    char sender[17] = { 0 };
    uv_ip4_name((const struct sockaddr_in*) addr, sender, 16);
    
    fprintf(stderr, "Recv from %s\n", sender);

    CLiveInstance* pLive = new CLiveInstance();
    pLive->RtpRecv(buf->base, nread);
}

int main() {
    uv_udp_t recv_socket;
    uv_loop_t* loop = uv_default_loop();
    int ret = uv_udp_init(loop, &recv_socket);
    struct sockaddr_in recv_addr;
    uv_ip4_addr("0.0.0.0", 6000, &recv_addr);
    uv_udp_bind(&recv_socket, (const struct sockaddr *)&recv_addr, UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket, echo_alloc, on_read);

    uv_run(loop, UV_RUN_DEFAULT);
}
