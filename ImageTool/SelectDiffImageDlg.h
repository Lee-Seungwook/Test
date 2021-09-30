#pragma once


// CSelectDiffImageDlg 대화 상자

class CSelectDiffImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDiffImageDlg)

public:
	CSelectDiffImageDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSelectDiffImageDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_DIFFIMG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboImage1;
	CComboBox m_comboImage2;
	void* m_pDoc1;
	void* m_pDoc2;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
