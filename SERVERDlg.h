
// SERVERDlg.h: 헤더 파일
//

#pragma once
#include <iostream>

// CSERVERDlg 대화 상자
class CSERVERDlg : public CDialogEx
{
private:
	SOCKET m_clntSocks[100];
	int clntCnt = 0;

// 생성입니다.
public:
	CSERVERDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CStatic m_picture;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnServeron();
	void DatabaseOn();
	void AcceptThread(SOCKET hServSock);
	void ClientThread(SOCKET hClntSock);

	bool Python_cint(); //파이썬 클라이언트와 파일 및 문자열 송수신 함수
	void Database_result(std::string n_strImage); //정상 불량 정보 데이터베이스에 넣는 함수
	void Database_filename(std::string n_strImage); //클라이언트에게 수신 받은 이미지 저장할때 부여하는 파일 이름생성 및 데이터베이스에 넣는 함수
};