#include "pch.h"
#include "MYclnt.h"
#include "iostream"

void MYclnt::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	char buf[4];
	Receive(&buf, sizeof(buf));
	std::cout << "Main serv recv : " << buf << std::endl;
	Close();
}


void MYclnt::OnSend(int nErrorCode)
{
	CSocket::OnSend(nErrorCode);
	
}


void MYclnt::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	ShutDown(2);
	Close();
}
