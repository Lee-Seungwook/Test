#pragma once


// CHighboostDlg 대화 상자

class CHighboostDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHighboostDlg)

public:
	CHighboostDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHighboostDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIGHBOOST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_fAlpha;
	CSliderCtrl m_sliderAlpha;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeAlphaEdit();
};
