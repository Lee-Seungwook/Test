#include "pch.h"
#include "ListenSocket.h"
#include "ChildSocket.h"
#include "ChattingServerDlg.h"

#include "ChatClienLibarary/ServerListenAPI.h"
#pragma comment( lib, "Debug/ChatClienLibrary.lib" )

void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CChildSocket* pChild = new CChildSocket;

	BOOL check = Accept(*pChild);

	if (check == FALSE) {
		delete pChild;
		AfxMessageBox(_T("접속허용실패"));
		return;
	}

	pChild->SetListenSocket(this);
	m_ptrChildSocketList.AddTail(pChild);

	CServerListenAPI API;
	API.APIAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}


void CListenSocket::CloseClientSocket(CSocket* pChild)
{
	// TODO: 여기에 구현 코드 추가.
	CServerListenAPI API;
	API.APICloseClientSocket(pChild, m_ptrChildSocketList);
}


void CListenSocket::BroadCast(char* pszBuffer, int len)
{
	// TODO: 여기에 구현 코드 추가.
	CServerListenAPI APIL;
	APIL.APIBroadCast(pszBuffer, len, m_ptrChildSocketList);
}

