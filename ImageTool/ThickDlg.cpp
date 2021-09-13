// ThickDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "ThickDlg.h"
#include "afxdialogex.h"


// CThickDlg 대화 상자

IMPLEMENT_DYNAMIC(CThickDlg, CDialogEx)

CThickDlg::CThickDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THICK, pParent)
	// , m_nThick(0)
	, m_nThick(0)
{

}

CThickDlg::~CThickDlg()
{
}

void CThickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// DDX_LBIndex(pDX, IDC_LIST, m_nThick);
	DDX_Text(pDX, IDC_EDIT, m_nThick);
}


BEGIN_MESSAGE_MAP(CThickDlg, CDialogEx)
END_MESSAGE_MAP()


// CThickDlg 메시지 처리기
