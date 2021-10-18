#include "UIThread.h"
#include "ChattingClientDlg.h"
IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

BOOL CUIThread::InitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	CChattingClientDlg* dlg = new CChattingClientDlg();
	m_pMainWnd = dlg;
	INT_PTR nResult = dlg->DoModal();
	return TRUE;
}


