#include "UIServerThread.h"
#include "ChattingServerDlg.h"

IMPLEMENT_DYNCREATE(CUIServerThread, CWinThread)


BOOL CUIServerThread::InitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CChattingServerDlg* dlg = new CChattingServerDlg();
	m_pMainWnd = dlg;
	INT_PTR nResult = dlg->DoModal();
	return TRUE;
}
