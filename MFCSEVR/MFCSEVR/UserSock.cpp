// UserSock.cpp: 구현 파일
//

#include "pch.h"
#include "MFCSEVR.h"
#include "UserSock.h"
#include "iostream"
#include <afx.h>


// UserSock

UserSock::UserSock()
{
}

UserSock::~UserSock()
{
}


// UserSock 멤버 함수

/*함수이름: OnReceive(int nErrorCode)
기능: OnReceive 가상 함수 생성으로 수신받은 정수형 데이터 저장 및 콘솔 출력 함수
반환값: 없음*/
void UserSock::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);

	int data;
	Receive(&data, sizeof(int));
	std::cout << "From Python(result true:1 false:0) : " << data << std::endl;

}
/*함수이름: OnSend(int nErrorCode)
기능: OnSend 가상 함수 생성으로 송신할 파일 불러와 파일 사이즈와 파일 데이터 송신 후 콘솔 출력 함수
반환값: 없음*/
void UserSock::OnSend(int nErrorCode)
{
	CSocket::OnSend(nErrorCode);

	CFile pfile;
	pfile.Open(_T("C:/Users/lms/Documents/Cmfc_cnn_python/XX/cast_O (52).jpeg"),
		CFile::modeRead);

	int nFileData = pfile.GetLength(); //파일 사이즈
	Send(&nFileData, sizeof(nFileData)); //파일 사이즈 송신
	std::cout << "file size: " << nFileData << std::endl;

	char buf[28998];
	pfile.Read(buf, sizeof(buf));
	Send(&buf, sizeof(buf));
	std::cout << "file send" << std::endl;
	pfile.Close();
}

void UserSock::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	ShutDown(2);
	Close();
}
