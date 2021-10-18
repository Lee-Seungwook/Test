// ChattingClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "ImageToolDoc.h"
#include "ChattingClientDlg.h"
#include "afxdialogex.h"
#include "ConnectDlg.h"
#include "UIThread.h"
#include "IppImage/IppImage.h"
#include "IppImage/IppConvert.h"
#include "IppImage/IppEnhance.h"
#include "IppGeometry.h"
#include "IppColor.h"



// CChattingClientDlg 대화 상자

IMPLEMENT_DYNAMIC(CChattingClientDlg, CDialogEx)

CChattingClientDlg::CChattingClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_CLIENT, pParent)
	, m_strData(_T(""))
	
{

}

CChattingClientDlg::~CChattingClientDlg()
{
}

void CChattingClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strData);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_ButtonSend);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ButtonConnect);
	
}


BEGIN_MESSAGE_MAP(CChattingClientDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CChattingClientDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChattingClientDlg::OnClickedButtonSend)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CChattingClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()





// CChattingClientDlg 메시지 처리기


void CChattingClientDlg::OnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CConnectDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_Client.Create();
		m_Client.Connect(dlg.m_strAddress, 7000);
		m_ButtonSend.EnableWindow(TRUE);
		m_ButtonConnect.EnableWindow(FALSE);
	}
}


void CChattingClientDlg::OnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_Client.Send((LPCTSTR)m_strData, m_strData.GetLength() * 2);
	m_strData = _T("");

	UpdateData(FALSE);
}





BOOL CChattingClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	WSADATA wd;
	memset(&wd, 0, sizeof(wd));
	WSAStartup(MAKEWORD(2, 2), &wd);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CChattingClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CChattingClientDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnDestroy();
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}


