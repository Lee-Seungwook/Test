// GaussianDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "GaussianDlg.h"
#include "afxdialogex.h"


// CGaussianDlg 대화 상자

IMPLEMENT_DYNAMIC(CGaussianDlg, CDialogEx)

CGaussianDlg::CGaussianDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAUSSIAN, pParent)
{

}

CGaussianDlg::~CGaussianDlg()
{
}

void CGaussianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGaussianDlg, CDialogEx)
END_MESSAGE_MAP()


// CGaussianDlg 메시지 처리기

// 8.2....