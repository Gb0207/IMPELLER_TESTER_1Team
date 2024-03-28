#include "pch.h"
#include "SocketServer.h"
#include "CListenSocket.h"
#include "CClientSocket.h"
#include "SocketServerDlg.h"

CListenSocket::CListenSocket()
{

}

CListenSocket::~CListenSocket()
{

}

void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CClientSocket* pClient = new CClientSocket;
	CString str;

	if (Accept(*pClient))
	{
		pClient->SetListenSocket(this);
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pClient)
{
	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);

	if (pos != NULL)
	{
		if (pClient != NULL)
		{
			pClient->ShutDown();
			pClient->Close();
		}

		CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd();
		CString str1, str2;
		UINT indx = 0, posNum;
		pMain->clientList->SetCurSel(0);

		while (indx < pMain->clientList->GetCount())
		{
			posNum = (int)m_ptrClientSocketList.Find(pClient);
			pMain->clientList->GetText(indx, str1);
			str2.Format(_T("%d"), posNum);

			if (str1.Find(str2) != -1)
			{
				AfxMessageBox(str1 + str2);
				pMain->clientList->DeleteString(indx);
				break;
			}
			indx++;
		}
		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
}