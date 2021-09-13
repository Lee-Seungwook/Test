#pragma once


// CThickDlg 대화 상자

class CThickDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThickDlg)

public:
	CThickDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CThickDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THICK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	int m_nThick;
	int m_nThick;
};
