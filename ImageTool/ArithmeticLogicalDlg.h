#pragma once


// CArithmeticLogicalDlg 대화 상자

class CArithmeticLogicalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CArithmeticLogicalDlg)

public:
	CArithmeticLogicalDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CArithmeticLogicalDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARITHMETIC_LOGICAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboImage1;
	CComboBox m_comboImage2;
	int m_nFunction;
	void* m_pDoc1; // 첫 번째 콤보 박스에서 선택한 도큐먼트의 주소를 저장
	void* m_pDoc2; // 두 번째 콤보 박스에서 선택한 도큐먼트의 주소를 저장
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
