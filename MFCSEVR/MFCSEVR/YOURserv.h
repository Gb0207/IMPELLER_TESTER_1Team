#pragma once
#include <afxsock.h>
#include "CPPserv.h"

class YOURserv : public CSocket
{
private:
    CPPserv m_cpp;

public:
    virtual void OnAccept(int nErrorCode);
};

