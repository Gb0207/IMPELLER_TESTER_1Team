#include "pch.h"
#include "MFCSEVR.h"
#include "CPPserv.h"
#include "iostream"
#include <mysql.h>
#include <afx.h>


void CPPserv::OnReceive(int nErrorCode)
{
	std::cout << "Onrecv---------------------- " << std::endl;

	char strBuffer[1024];
	int nLen = Receive(&strBuffer, sizeof(strBuffer));
	strBuffer[nLen] = 0;

	std::cout << "clnt recv : "<< strBuffer << std::endl;


	// 구분자로 조회라고 왔을 때 아래 함수로 호출0
	if (strBuffer[0] == '2' && strBuffer[1] == '0')
	{
		char cquery[100];
		sprintf_s(cquery, "SELECT filename, result, reason FROM result_table WHERE date = '%s'", strBuffer);
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
	else
	{
		char buf[4] = { 'a', };
		Send(&buf, sizeof(buf));
		std::cout << "cpp send : " << buf[0] << std::endl;
	}
}


//void CPPserv::OnSend(int nErrorCode)
//{
//	CSocket::OnSend(nErrorCode);
//	char buf[4] = {'a',};
//	Send(&buf, sizeof(buf));
//	std::cout << "cpp send" << buf[0] << std::endl;
//}


void CPPserv::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	ShutDown(2);
	Close();
}
