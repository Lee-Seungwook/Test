// LineStyleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "LineStyleDlg.h"
#include "afxdialogex.h"


// CLineStyleDlg 대화 상자

IMPLEMENT_DYNAMIC(CLineStyleDlg, CDialogEx)

CLineStyleDlg::CLineStyleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LINE_STYLE, pParent)
	, m_nLineStyle(0)
{

}

CLineStyleDlg::~CLineStyleDlg()
{
}

void CLineStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// DDX_Control(pDX, IDC_COMBO_STYLE, m_StyleCombo);
	DDX_Radio(pDX, IDC_RADIO_SOLID, m_nLineStyle);
}


BEGIN_MESSAGE_MAP(CLineStyleDlg, CDialogEx)
END_MESSAGE_MAP()


// CLineStyleDlg 메시지 처리기


BOOL CLineStyleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
