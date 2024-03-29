#pragma once
#include <afxsock.h>
class MYclnt : public CSocket
{
public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

