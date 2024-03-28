#pragma once
#include <afxsock.h>

#define DB_HOST "127.0.0.1"	// 서버 아이피
#define DB_USER "root"	// DB 접속계정
#define DB_PASS "1234"	// DB 계정암호
#define DB_NAME "defect_test"	// DB 이름

class CClientSocket :
    public CSocket
{
public:
    CClientSocket();
    virtual ~CClientSocket();

    CAsyncSocket* m_pListenSocket;
    void SetListenSocket(CAsyncSocket* pSocket);
    virtual void OnClose(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
    //virtual void OnSend(int nErrorCode);
};

