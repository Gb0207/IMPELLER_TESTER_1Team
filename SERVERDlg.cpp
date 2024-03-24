
// SERVERDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SERVER.h"
#include "SERVERDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <windows.h>
#include <process.h>
#include <WinSock2.h>
#include <thread>
#include <fstream>
#include <mysql.h>
#include <cstring>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#define PORT 11999
#define BUFSIZE 1024

#define DB_HOST "127.0.0.1"	// 서버 아이피
#define DB_USER "root"	// DB 접속계정
#define DB_PASS "1234"	// DB 계정암호
#define DB_NAME "world"	// DB 이름

MYSQL* connection = NULL, conn;
MYSQL_RES* sql_result;
MYSQL_ROW sql_row;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CSERVERDlg 대화 상자

CSERVERDlg::CSERVERDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSERVERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
}

BEGIN_MESSAGE_MAP(CSERVERDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SERVERON, &CSERVERDlg::OnBnClickedBtnServeron)
END_MESSAGE_MAP()

// CSERVERDlg 메시지 처리기

BOOL CSERVERDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 100, 100, 0, 0, SWP_NOSIZE);

	DatabaseOn();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSERVERDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSERVERDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CSERVERDlg::DatabaseOn()
{
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	if (connect == NULL)
	{
		CString s = _T("");
		s = mysql_error(&conn);
		AfxMessageBox(s, MB_OK);
		return;
	}
	else
	{
		AfxMessageBox(_T("DB 연결 성공"));
	}
	// MySQL과 비주얼 스튜디오가 사용하는 문자셋이 달라서 한글 깨짐 방지를 위해 아래 코드 3줄 추가
	mysql_query(connection, "set session character_set_connection=euckr;");
	mysql_query(connection, "set session character_set_results=euckr;");
	mysql_query(connection, "set session character_set_client=euckr;");
}
void CSERVERDlg::Database_update(bool result)
{
	int n_iStat; //쿼리요청 후 결과 성공 실패
	char n_chStr[1024];
	/*CREATE TABLE result_table(
	number INT PRIMARY KEY AUTO_INCREMENT,
	filename TEXT NOT NULL,
	result INT NOT NULL,
	date DATE NOT NULL,
	reason INT NOT NULL
	);*/
	if (result == false) {
		strcpy_s(n_chStr,"INSERT INTO result_table SET result = 0, date = now() reason = 0");
	}
	else
	{
		strcpy_s(n_chStr,"INSERT INTO result_table SET result = 1, date = now() reason = 0");

	char* n_chQuery = n_chStr;
	n_iStat = mysql_query(&conn, n_chQuery);
	if (n_iStat != 0) { //쿼리 요철 확인
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(&conn));
		AfxMessageBox(_T("DB update error"));
	}
	else {
		AfxMessageBox(_T("DB update success"));
	}

	sql_result = mysql_store_result(&conn); // 결과 확인하기
	// 결과 출력
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
		printf("%s %s %s\n", sql_row[0], sql_row[1], sql_row[2]);
	}
	mysql_free_result(sql_result); //결과 비우기
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSERVERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSERVERDlg::OnBnClickedBtnServeron()
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup() error"));
		return;
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(PORT);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("bind() error"));
		return;
	}

	if (listen(hServSock, SOMAXCONN) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("listen() error"));
		return;
	}

	AfxMessageBox(_T("Server on"));

	// 클라이언트 연결 수락을 스레드로 이동
	std::thread acceptThread(&CSERVERDlg::AcceptThread, this, hServSock);
	acceptThread.detach(); // 스레드 분리

	return;
}

void CSERVERDlg::AcceptThread(SOCKET hServSock)
{
	SOCKADDR_IN clntAdr;
	int clntAdrSz;

	while (true) {
		clntAdrSz = sizeof(clntAdr);
		SOCKET hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);
		if (hClntSock == INVALID_SOCKET) {
			AfxMessageBox(_T("accept() error"));
			break;
		}
		m_clntSocks[clntCnt] = hClntSock;
		clntCnt++;
		// AfxMessageBox(_T("Client connected"));

		// 각 클라이언트와 통신을 위한 스레드 생성
		std::thread clientThread(&CSERVERDlg::ClientThread, this, hClntSock);
		clientThread.detach(); // 스레드 분리
	}

	//closesocket(hServSock);
	//WSACleanup();
}

void CSERVERDlg::ClientThread(SOCKET hClntSock)
{
	int imageSize = 0;
	char* imageBuffer = nullptr;

	// 이미지 크기 수신
	int recvSize = recv(hClntSock, reinterpret_cast<char*>(&imageSize), sizeof(imageSize), 0);
	if (recvSize != sizeof(imageSize))
	{
		AfxMessageBox(_T("Failed to receive image size"));
		closesocket(hClntSock);
		return;
	}

	// 이미지 데이터를 저장할 버퍼 동적 할당
	imageBuffer = new char[imageSize];
	if (!imageBuffer)
	{
		AfxMessageBox(_T("Memory allocation failed"));
		closesocket(hClntSock);
		return;
	}

	// 이미지 데이터 수신
	recvSize = recv(hClntSock, imageBuffer, imageSize, 0);
	if (recvSize != imageSize)
	{
		AfxMessageBox(_T("Failed to receive complete image"));
		delete[] imageBuffer;
		closesocket(hClntSock);
		return;
	}

	std::ofstream outputFile("captured_image.jpg", std::ios::out | std::ios::binary);
	if (outputFile.is_open())
	{
		outputFile.write(imageBuffer, imageSize);
		outputFile.close();
	}
	else
	{
		AfxMessageBox(_T("Failed to save image"));
		return;
	}
	//파이썬 함수 호출
	bool n_bResult = Python_cint();
	Database_update(n_bResult);

	CRect rect;
	m_picture.GetWindowRect(rect);
	CDC* dc;
	dc = m_picture.GetDC();

	CImage image;

	image.Load(_T("captured_image.jpg"));

	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	ReleaseDC(dc);
	delete[] imageBuffer;

	// AfxMessageBox(_T("Success to receive image"));
}
/*함수이름: Python_cint()
기능: 파이썬 클라이언트와 파일 및 문자열 송수신 함수
반환 값: bool 제품이 정상인지 아닌지 반환*/
bool CSERVERDlg::Python_cint()
{
	//m_clntSocks[0] = 파이썬
	CString file = _T("C:/Users/lms/Documents/Cmfc_cnn_python/XX/cast_O (130).jpeg");
	CFile openfile;

	if (openfile.Open(file, CFile::modeRead)) {
		ULONGLONG openFileSize = openfile.GetLength();
		send(m_clntSocks[0], reinterpret_cast<char*>(&openFileSize), sizeof(openFileSize), 0);

		char* openFilebuffer = new char[openFileSize];
		UINT bytesRead = openfile.Read(openFilebuffer, openFileSize);

		send(m_clntSocks[0], reinterpret_cast<char*>(openFilebuffer), openFileSize, 0);
		//전송
	}
	else {
		AfxMessageBox(_T("None file"));
	}

	char n_result[BUFSIZE];
	recv(m_clntSocks[0], n_result, BUFSIZE-1, 0);
	if (n_result != NULL) {
		std::cout << n_result << std::endl;
		AfxMessageBox(_T("Recv result"));
	}
	else
	{
		AfxMessageBox(_T("Recv None result"));
	}

	bool n_bResult = FALSE;
	if (n_result[0] == 't') {
		n_bResult = TRUE;
		AfxMessageBox(_T("True"));
	}
	else
	{
		AfxMessageBox(_T("False"));
	}
		
	return n_bResult;
}