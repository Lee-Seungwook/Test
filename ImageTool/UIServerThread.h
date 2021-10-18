#pragma once

#include "stdafx.h"
class CUIServerThread :
	public CWinThread
{
public:
	DECLARE_DYNCREATE(CUIServerThread)
	virtual BOOL InitInstance();
};

