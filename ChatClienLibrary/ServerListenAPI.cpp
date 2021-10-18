#include "pch.h"
#include "afxdialogex.h"

#ifndef  _DLL_SERVER_
#define _DLL_SERVER_
#endif // ! _DLL_SERVER_

#include "ServerListenAPI.h"

void CServerListenAPI::APIAccept()
{
	CChattingServerDlg* pMain = (CChattingServerDlg*)AfxGetMainWnd();
	pMain->m_ListServer.AddString(_T("서버 접속 허용"));
	pMain->m_ListServer.SetCurSel(pMain->m_ListServer.GetCount() - 1);
}

void CServerListenAPI::APICloseClientSocket(CSocket* (&pChild), CPtrList &m_ptrChildSocketList)
{
	POSITION pos;
	pos = m_ptrChildSocketList.Find(pChild);
	if (pos != NULL) {
		pChild->ShutDown();
		pChild->Close();
	}
	m_ptrChildSocketList.RemoveAt(pos);
	delete pChild;
}

void CServerListenAPI::APIBroadCast(char* pszBuffer, int len, CPtrList &m_ptrChildSocketList)
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	while (pos != NULL)
	{
		pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);
		if (pChild != NULL)
			pChild->Send(pszBuffer, len * 2);
	}
}