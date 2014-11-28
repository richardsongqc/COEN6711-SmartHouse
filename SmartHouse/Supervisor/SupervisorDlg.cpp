
// SupervisorDlg.cpp : implementation file
//

#include "stdafx.h"
#include <SerialProtocol.h>
#include "Supervisor.h"
#include "Buffer.h"
#include "SupervisorDlg.h"
#include "afxdialogex.h"
#include <Winspool.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSupervisorDlg dialog



CSupervisorDlg::CSupervisorDlg(CWnd* pParent /*=NULL*/) : 
	CDialogEx(CSupervisorDlg::IDD, pParent),
	m_uOldHiHumid (0),
	m_uOldLoHumid (0),
	m_uOldHiTemp  (0),
	m_uOldLoTemp  (0),
	m_uOldLocker  (0),
	m_uOldFan	  (0),
	m_uOldPIR	  (0),
	m_nOldDistance(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	MsgQueue.clear();
}

void CSupervisorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OPEN_PORT, m_btnOpenPort);
	DDX_Control(pDX, IDC_COMBO_BAUD_RATE, m_cboBaudRate);
	DDX_Control(pDX, IDC_COMBO_PORT, m_cboPort);
	DDX_Control(pDX, IDC_EDIT_PARAM_DISTANCE, m_edtParamDistance);
	DDX_Control(pDX, IDC_EDIT_PARAM_HUMID, m_edtParamHumid);
	DDX_Control(pDX, IDC_EDIT_PARAM_TEMPERATURE, m_edtParamTemperature);
	DDX_Control(pDX, IDC_STATIC_FAN, m_lblFan);
	DDX_Control(pDX, IDC_STATIC_HUMID, m_lblHumid);
	DDX_Control(pDX, IDC_STATIC_LOCKER, m_lblLocker);
	DDX_Control(pDX, IDC_STATIC_PIR, m_lblPIR);
	DDX_Control(pDX, IDC_STATIC_TEMPERATURE, m_lblTemperature);
	DDX_Control(pDX, IDC_STATIC_ULTRASONIC_DISTANCE, m_lblDistance);
}

BEGIN_MESSAGE_MAP(CSupervisorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PORT, &CSupervisorDlg::OnBnClickedButtonOpenPort)
	ON_MESSAGE(ON_COM_RECEIVE, &CSupervisorDlg::OnComReceive)
END_MESSAGE_MAP()


// CSupervisorDlg message handlers

BOOL CSupervisorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_cboPort.ResetContent();
	vector<int> listPort = GetSerialPortList();
	for (vector<int>::iterator iter = listPort.begin();
		iter != listPort.end();
		iter++)
	{
		CString str;
		str.Format(TEXT("COM%d"), *iter);
		m_cboPort.AddString(str);
	}
	m_cboPort.SetCurSel(0);

	m_cboBaudRate.AddString(TEXT("9600  "));
	m_cboBaudRate.AddString(TEXT("14400 "));
	m_cboBaudRate.AddString(TEXT("19200 "));
	m_cboBaudRate.AddString(TEXT("38400 "));
	m_cboBaudRate.AddString(TEXT("57600 "));
	m_cboBaudRate.AddString(TEXT("115200"));
	m_cboBaudRate.AddString(TEXT("128000"));
	m_cboBaudRate.AddString(TEXT("256000"));
	m_cboBaudRate.SetCurSel(4);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSupervisorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSupervisorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSupervisorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

vector<int> CSupervisorDlg::GetSerialPortList()
{
	vector<int>		listPort;
	listPort.clear();

	CString          strSerialList[256];  // 临时定义 256 个字符串组

	LPBYTE pBite = NULL;

	DWORD pcbNeeded = 0;  // bytes received or required

	DWORD pcReturned = 0;  // number of ports received

	// 获取端口信息，能得到端口信息的大小 pcbNeeded

	EnumPorts(NULL, 2, pBite, 0, &pcbNeeded, &pcReturned);

	pBite = new BYTE[pcbNeeded];

	// 枚举端口，能得到端口的具体信息 pBite 以及端口的的个数 pcReturned

	EnumPorts(NULL, 2, pBite, pcbNeeded, &pcbNeeded, &pcReturned);

	PORT_INFO_2 *pPort;

	pPort = (PORT_INFO_2*)pBite;

	for ( DWORD i = 0; i < pcReturned; i++)
	{
		CString str = pPort[i].pPortName;
		// 串口信息的具体确定
		if (str.Left(3) == TEXT("COM"))
		{
			CString temp = str.Right(str.GetLength() - 3);// 下面两行注释获取串口序号用
			listPort.push_back(_ttoi(temp.Left(temp.GetLength()- 1)));

		}

	}

	if (pBite)
	{
		delete[] pBite;
	}

	return listPort;
}

void CSupervisorDlg::OnBnClickedButtonOpenPort()
{
	// TODO: Add your control notification handler code here
	CString strPort;
	m_cboPort.GetLBText(m_cboPort.GetCurSel(), strPort);
	
	CString str = strPort.Right(strPort.GetLength() - 3);
	int nPort = _ttoi(str);
	
	m_cboBaudRate.GetLBText(m_cboBaudRate.GetCurSel(), str);
	int nBaudRate = _ttoi(str);

	if (!m_port.open(nPort, nBaudRate))
	{
		MessageBox(L"open fail", strPort, MB_OK);
	}
	else
	{
		m_port.set_hwnd(this->GetSafeHwnd());
	}
}

void CSupervisorDlg::ProcessMsg()
{
	unsigned char szBuf[256] = { 0 };
	while (MsgQueue.size() > 0)
	{
		CString str;
		CBuffer buffer = *(MsgQueue.begin());
		U8 * p = buffer.GetBuffer();
		U8 uLen = *( p + 1 );
		memcpy(szBuf, ((MsgQueue.begin())->GetBuffer()), uLen);
		switch (szBuf[0])
		{
		default:						break;
		case CMD_PIR: 	break;
		case CMD_ULTRASONIC_RANGE:
			break;
		case CMD_RELAY_1:
			if (szBuf[2] == 0)
			{
				m_lblLocker.SetWindowText(TEXT("OFF"));
			}
			else
			{
				m_lblLocker.SetWindowText(TEXT("ON"));
			}
			break;
		case CMD_RELAY_2:
			if (szBuf[2] == 0)
			{
				m_lblFan.SetWindowText(TEXT("OFF"));
			}
			else
			{
				m_lblFan.SetWindowText(TEXT("ON"));
			}
			break;
		case CMD_DHT:
			if (0 != szBuf[2])
			{
				str.Format(TEXT("%d.%d"), szBuf[2], szBuf[3]);
				m_lblHumid.SetWindowText(str);

				m_uOldHiHumid = szBuf[2];
				m_uOldLoHumid = szBuf[3];
			}

			if (0 != szBuf[4])
			{
				str.Format(TEXT("%d.%d"), szBuf[4], szBuf[5]);
				m_lblTemperature.SetWindowText(str);

				m_uOldHiTemp = szBuf[4];
				m_uOldLoTemp = szBuf[5];
			}
			break;
		case RSP_PIR: 	break;
		case RSP_ULTRASONIC_RANGE: 	break;
		case RSP_RELAY_1: 	break;
		case RSP_RELAY_2: 	break;
		case RSP_DHT: 	break;
		}

		MsgQueue.pop_front();
	}

}

afx_msg LRESULT CSupervisorDlg::OnComReceive(WPARAM wParam, LPARAM lParam)
{
	unsigned char szBuf[256] = { 0 };
	int len = m_port.read((char*)szBuf, sizeof(szBuf));
	
	int i = 0;

	while (i < len)
	{
		int nCurLen = szBuf[i + 1];
		CBuffer buffer( szBuf+i, (U8)(nCurLen + 2));

		MsgQueue.push_back(buffer);

		if (szBuf[i + nCurLen + 3] == 0xFF &&
			szBuf[i + nCurLen + 4] == 0xFF && ( 
			szBuf[i + nCurLen + 5] >0 ) )
		{
			i += nCurLen + 4;
		}
		else
		{
			break;
		}
	}



	ProcessMsg();

	return 0;
}
