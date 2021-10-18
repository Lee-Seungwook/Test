#pragma once
#include "stdafx.h"
#include "IppImage/IppDib.h"


class CUIThread :
	public CWinThread
{
public:
	DECLARE_DYNCREATE(CUIThread)
	
	

	virtual BOOL InitInstance();
	
};

