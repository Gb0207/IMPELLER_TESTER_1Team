#pragma once

// UserSock 명령 대상

class UserSock : public CSocket
{
public:
	UserSock();
	virtual ~UserSock();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


