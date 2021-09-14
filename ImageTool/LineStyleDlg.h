#pragma once


// CLineStyleDlg 대화 상자

class CLineStyleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLineStyleDlg)

public:
	CLineStyleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLineStyleDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINE_STYLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CComboBox m_StyleCombo;
	int m_nLineStyle;
};
