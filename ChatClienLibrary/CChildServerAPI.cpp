#include "pch.h"
#include "afxdialogex.h"


#ifndef _DLL_CHILD_
#define _DLL_CHILD_
#endif // !_DLL_SERVER_

#include "CChildServerAPI.h"

void CChildServerAPI::APIReceive(char *szBuffer, CString& tmp, CString& strIPAddress, int len)
{
	CChattingServerDlg* pMain = (CChattingServerDlg*)AfxGetMainWnd();
	tmp.Format(_T("[%s] : %s"), strIPAddress, szBuffer);
	pMain->m_ListServer.AddString(tmp);
	pMain->m_ListServer.SetCurSel(pMain->m_ListServer.GetCount() - 1);

}