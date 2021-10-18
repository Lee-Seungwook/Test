#pragma once
#include <afxsock.h>
#include "..\ImageTool\ChattingClientDlg.h"

#ifdef  _DLL_CLIENT_
#define DLL_CLIENT _declspec(dllexport)
#else
#define DLL_CLIENT __declspec(dllimport)
#endif //  _DLL_CLIENT_

class DLL_CLIENT ClientAPI : public CSocket
{
public:
	void APIClose(int nErrorCode);
	void APIReceive(char *szBuffer, CString& tmp);
};