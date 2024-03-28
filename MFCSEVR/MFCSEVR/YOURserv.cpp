#include "pch.h"
#include "YOURserv.h"


void YOURserv::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	Accept(m_cpp);
}
