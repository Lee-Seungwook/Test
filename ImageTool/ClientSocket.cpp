#include "pch.h"
#include "ClientSocket.h"
#include "ChattingClientDlg.h"



#include "ChatClienLibarary/ClientAPI.h"
#pragma comment( lib, "ChatClienLibrary.lib")

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ClientAPI APIC;
	APIC.APIClose(nErrorCode);
	CSocket::OnClose(nErrorCode);
}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	char szBuffer[1024];
	::ZeroMemory(szBuffer, 1024);
	if (Receive(szBuffer, 1024) > 0)
	{
		// tmp.Format(_T("%s"), szBuffer);
		
		// CChattingClientDlg* pMain = (CChattingClientDlg*)AfxGetMainWnd();

		ClientAPI APIR;
		APIR.APIReceive(szBuffer, tmp);
	}
	CSocket::OnReceive(nErrorCode);
}
