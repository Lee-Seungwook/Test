#pragma once
#include "afxcmn.h"
#include "ClientSocket.h"
#include "IppImage/IppDib.h"
#include "IppImage/IppImage.h"


// CChattingClientDlg 대화 상자

class CChattingClientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChattingClientDlg)

public:
	CChattingClientDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CChattingClientDlg();
	CClientSocket m_Client;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_CLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_List;
	CString m_strData;
	CButton m_ButtonSend;
	CButton m_ButtonConnect;
	

	
	
	afx_msg void OnClickedButtonConnect();
	afx_msg void OnClickedButtonSend();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//CButton m_ButtonExcute;
//	CString m_ExeEdit;
	
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnLbnDblclkList();
//	afx_msg void OnClickedButtonExecute();
//	void SetImage(IppDib& dib);
	
//	void MakePreviewImage();
//	afx_msg void OnPaint();
};
