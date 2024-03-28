#include "pch.h"
#include "CConnectSocket.h"
#include "SocketClientDlg.h"
#include <vector>
#include <string>
#include <sstream>

CConnectSocket::CConnectSocket()
{

}

CConnectSocket::~CConnectSocket()
{

}

void CConnectSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR: Disconnected from server"));
	::PostQuitMessage(0);
}


//void CConnectSocket::OnReceive(int nErrorCode)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	char strBuffer[1024];
//	::ZeroMemory(strBuffer, sizeof(strBuffer));
//
//	Receive(strBuffer, sizeof(strBuffer));
//
//	std::vector<std::string> tokens;
//	std::stringstream ss(strBuffer);
//	std::string token;
//	int nCnt = 0;
//	while (std::getline(ss, token, ','))
//	{
//		tokens.push_back(token);
//		nCnt++;
//	}
//
//	CSocketClientDlg dlg;
//	dlg.m_list.DeleteAllItems();
//
//	int normalCnt = 0;
//	int defectCnt = 0;
//
//	int index = 0;
//	for (int i = 0; i < nCnt / 3; i++)
//	{
//		if (strcmp(tokens[0].c_str(), "no") == 0)
//		{
//			AfxMessageBox(_T("No data on selected date"));
//			break;
//		}
//		CString strindex;
//		strindex.Format(_T("%d"), index + 1);
//
//		dlg.m_list.InsertItem(index, strindex, 0);
//		for (int j = 0; j < 3; j++)
//		{
//			dlg.m_list.SetItemText(index, j + 1, CString(tokens[(i * 3) + j].c_str()));
//			if (j == 1 && strcmp(tokens[(i * 3) + j].c_str(), "normal") == 0)
//			{
//				normalCnt++;
//			}
//			if (j == 1 && strcmp(tokens[(i * 3) + j].c_str(), "defect") == 0)
//			{
//				defectCnt++;
//			}
//		}
//		index++;
//	}
//
//	CSocket::OnReceive(nErrorCode);
//}
