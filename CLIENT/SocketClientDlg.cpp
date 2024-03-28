
// SocketClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SocketClient.h"
#include "SocketClientDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

const double pi = 3.1415926535;

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


// CSocketClientDlg 대화 상자



CSocketClientDlg::CSocketClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CSocketClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CSocketClientDlg::OnBnClickedBtnSearch)
END_MESSAGE_MAP()


// CSocketClientDlg 메시지 처리기

BOOL CSocketClientDlg::OnInitDialog()
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
	SetWindowPos(NULL, 100, 500, 0, 0, SWP_NOSIZE);
	m_Socket.Create();
	if (m_Socket.Connect(_T("127.0.0.1"), 21000) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		PostQuitMessage(0);
		return FALSE;
	}
	else
		AfxMessageBox(_T("Success to connect Server"));

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, _T("no"), 0, 30);
	m_list.InsertColumn(1, _T("file"), 0, 110);
	m_list.InsertColumn(2, _T("status"), 0, 50);
	m_list.InsertColumn(3, _T("reason"), 0, 60);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketClientDlg::OnPaint()
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSocketClientDlg::OnBnClickedBtnSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strdate;
	GetDlgItemTextW(IDC_DATETIMEPICKER1, strdate);

	char cdate[100];
	sprintf_s(cdate, "%S", (LPCTSTR)strdate);
	m_Socket.Send((LPVOID)cdate, strlen(cdate));

	char strBuffer[1024];
	int nLen = m_Socket.Receive(strBuffer, sizeof(strBuffer));
	strBuffer[nLen] = 0;

	std::cout << strBuffer << std::endl;

	std::vector<std::string> tokens;
	std::stringstream ss(strBuffer);
	std::string token;
	int nCnt = 0;
	while (std::getline(ss, token, ','))
	{
		tokens.push_back(token);
		nCnt++;
	}

	m_list.DeleteAllItems();
	int normalCnt = 0;
	int defectCnt = 0;

	int index = 0;
	for (int i = 0; i < nCnt / 3; i++)
	{
		if (strcmp(tokens[0].c_str(), "no") == 0)
		{
			AfxMessageBox(_T("No data on selected date"));
			break;
		}
		CString strindex;
		strindex.Format(_T("%d"), index + 1);

		m_list.InsertItem(index, strindex, 0);
		for (int j = 0; j < 3; j++)
		{
			m_list.SetItemText(index, j + 1, CString(tokens[(i * 3) + j].c_str()));
			if (j == 1 && strcmp(tokens[(i * 3) + j].c_str(), "normal") == 0)
			{
				normalCnt++;
			}
			if (j == 1 && strcmp(tokens[(i * 3) + j].c_str(), "defect") == 0)
			{
				defectCnt++;
			}
		}
		index++;
	}

	if (strcmp(tokens[0].c_str(), "no") != 0)
		DrawPieChart(normalCnt, defectCnt);
	else
	{
		CRect rect(1, 1, 253, 253);
		GetDlgItem(IDC_PICTURE)->GetDC()->FillSolidRect(&rect, RGB(240, 240, 240));
	}
}

void CSocketClientDlg::DrawPieChart(int normalCnt, int defectCnt)
{
	CClientDC dc(this);
	CRect rect;
	GetDlgItem(IDC_PICTURE)->GetWindowRect(&rect);
	ScreenToClient(&rect);


	double total = normalCnt + defectCnt;
	double normalrate = (normalCnt / total) * 100;
	double defectrate = (defectCnt / total) * 100;

	int centerX = rect.Width() / 2;
	int centerY = rect.Height() / 2;

	int Xpos = static_cast<int>(centerX * cos((360 * normalrate / 100) * pi / 180)) + rect.CenterPoint().x;
	int Ypos = static_cast<int>(centerY * sin((360 * normalrate / 100) * pi / 180)) + rect.CenterPoint().y;

	int normalX = static_cast<int>(centerX / 2 * cos((360 * normalrate / 2 / 100) * pi / 180)) + rect.CenterPoint().x;;
	int normalY = static_cast<int>(centerY / 2 * sin((360 * normalrate / 2 / 100) * pi / 180)) + rect.CenterPoint().y;;

	int defectX = static_cast<int>(centerX / 2 * cos((360 * (normalrate + defectrate / 2) / 100) * pi / 180)) + rect.CenterPoint().x;
	int defectY = static_cast<int>(centerY / 2 * sin((360 * (normalrate + defectrate / 2) / 100) * pi / 180)) + rect.CenterPoint().y;

	CBrush greenbrush, redbrush;
	greenbrush.CreateSolidBrush(RGB(0, 255, 0));
	redbrush.CreateSolidBrush(RGB(255, 0, 0));

	if (normalCnt != 0)
	{
		dc.SelectObject(&greenbrush);
		dc.Ellipse(&rect);
	}
	else
	{
		dc.SelectObject(&redbrush);
		dc.Ellipse(&rect);
	}

	if (defectCnt != 0 && normalCnt != 0)
	{
		dc.SelectObject(&redbrush);
		dc.Pie(rect.TopLeft().x, rect.TopLeft().y, rect.BottomRight().x, rect.BottomRight().y, rect.CenterPoint().x, rect.CenterPoint().y, Xpos, Ypos);
	}

	CString strnormalrate, strdefectrate;
	strnormalrate.Format(_T("%.1f%%"), normalrate);
	strdefectrate.Format(_T("%.1f%%"), defectrate);

	dc.SetBkMode(TRANSPARENT);
	if (normalCnt != 0)
	{
		dc.TextOutW(normalX, normalY, _T("normal"));
		dc.TextOutW(normalX, normalY + 15, strnormalrate);
	}
	if (defectCnt != 0)
	{
		dc.TextOutW(defectX, defectY, _T("defect"));
		dc.TextOutW(defectX, defectY + 15, strdefectrate);
	}
}