// HighboostDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "HighboostDlg.h"
#include "afxdialogex.h"


// CHighboostDlg 대화 상자

IMPLEMENT_DYNAMIC(CHighboostDlg, CDialogEx)

CHighboostDlg::CHighboostDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HIGHBOOST, pParent)
	, m_fAlpha(1.80f)
{

}

CHighboostDlg::~CHighboostDlg()
{
}

void CHighboostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ALPHA_EDIT, m_fAlpha);
	DDV_MinMaxFloat(pDX, m_fAlpha, 1.0, FLT_MAX);
	DDX_Control(pDX, IDC_ALPHA_SLIDER, m_sliderAlpha);
}


BEGIN_MESSAGE_MAP(CHighboostDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_ALPHA_EDIT, &CHighboostDlg::OnChangeAlphaEdit)
END_MESSAGE_MAP()


// CHighboostDlg 메시지 처리기


BOOL CHighboostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 슬라이더 컨트롤의 초기화
	m_sliderAlpha.SetRange(10, 250);
	m_sliderAlpha.SetTicFreq(20);
	m_sliderAlpha.SetPageSize(20);
	m_sliderAlpha.SetPos(static_cast<int>(m_fAlpha * 50));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CHighboostDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 슬라이드바에서 발생한 WM_HSCROLL 메시지인 경우 처리
	if (m_sliderAlpha.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderAlpha.GetPos();
		m_fAlpha = (pos / 50.f);
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CHighboostDlg::OnChangeAlphaEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderAlpha.SetPos(static_cast<int>(m_fAlpha * 50));
}
