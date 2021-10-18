#pragma once
#include "ListenSocket.h"
#include "ChildSocket.h"

// CChattingServerDlg 대화 상자

class CChattingServerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChattingServerDlg)

public:
	CChattingServerDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CChattingServerDlg();
	CListenSocket* m_pListenSocket;
	CChildSocket m_Server;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_SERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListServer;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();

};
