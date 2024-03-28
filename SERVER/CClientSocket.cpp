#include "pch.h"
#include "SocketServer.h"
#include "CClientSocket.h"
#include "CListenSocket.h"
#include "SocketServerDlg.h"
#include <iostream>
#include <mysql.h>


CClientSocket::CClientSocket()
{

}

CClientSocket::~CClientSocket()
{

}

void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);

	CSocket::OnClose(nErrorCode);
}


void CClientSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	char strBuffer[1024];
	::ZeroMemory(strBuffer, sizeof(strBuffer));

	//int num;
	//Receive(&num, sizeof(num));
	//std::cout << num << std::endl;
	Receive(&strBuffer, sizeof(strBuffer));

	char cquery[100];
	sprintf_s(cquery, "SELECT file, status, reason FROM result_table WHERE date = '%s'", strBuffer);
	std::cout << cquery << std::endl;


	MYSQL* connection = NULL, conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	if (connect == NULL)
	{
		CString s = _T("");
		s = mysql_error(&conn);
		AfxMessageBox(s, MB_OK);
		return;
	}

	std::string message;
	std::string comma = ",";
	if (mysql_query(&conn, cquery) == 0)
	{
		sql_result = mysql_store_result(&conn);
		if (mysql_num_rows(sql_result) != 0)
		{
			MYSQL_ROW row;

			while ((row = mysql_fetch_row(sql_result)))
			{
				for (int i = 0; i < 3; i++)
				{
					message += (row[i] + comma);
				}
			}
			message.pop_back();
			std::cout << message.c_str() << std::endl;
			Send(message.c_str(), message.length());

			mysql_free_result(sql_result);
		}
		else
		{
			// 결과가 없는 경우
			message = "no" + comma;
			Send(message.c_str(), message.length());
			std::cout << message.c_str() << std::endl;
		}
	}
	else
	{
		// 쿼리 실행 오류
		message = "no" + comma;
		Send(message.c_str(), message.length());
		std::cout << message.c_str() << std::endl;
	}
}


//void CClientSocket::OnSend(int nErrorCode)
//{
//	CSocket::OnSend(nErrorCode);
//}
