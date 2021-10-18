// ChattingServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "ChattingServerDlg.h"
#include "afxdialogex.h"


// CChattingServerDlg 대화 상자

IMPLEMENT_DYNAMIC(CChattingServerDlg, CDialogEx)

CChattingServerDlg::CChattingServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_SERVER, pParent)
{

}

CChattingServerDlg::~CChattingServerDlg()
{
}

void CChattingServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListServer);
}


BEGIN_MESSAGE_MAP(CChattingServerDlg, CDialogEx)
	ON_WM_DESTROY()
	
	ON_BN_CLICKED(IDOK, &CChattingServerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChattingServerDlg 메시지 처리기


BOOL CChattingServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	WSADATA wd;
	memset(&wd, 0, sizeof(wd));
	WSAStartup(MAKEWORD(2, 2), &wd);

	m_pListenSocket = new CListenSocket;

	if (m_pListenSocket->Create(7000, SOCK_STREAM))
	{
		if (m_pListenSocket->Listen())
		{
		}
		else
		{
			AfxMessageBox(_T("Failed to LISTEN"));
		}
	}
	else
	{
		AfxMessageBox(_T("Failed to create socket for listen."));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CChattingServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	POSITION pos;
	pos = m_pListenSocket->m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	while (pos != NULL)
	{
		pChild = (CChildSocket*)m_pListenSocket->m_ptrChildSocketList.GetNext(pos);
		if (pChild != NULL)
		{
			pChild->ShutDown();
			pChild->Close();
			delete pChild;
		}
	}
	m_pListenSocket->ShutDown();
	m_pListenSocket->Close();
}

void CChattingServerDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// OnDestroy();
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

