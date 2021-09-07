
// ImageTool.h: ImageTool 응용 프로그램의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 주 기호입니다.


// CImageToolApp:
// 이 클래스의 구현에 대해서는 ImageTool.cpp을(를) 참조하세요.
//
class IppDib;
class CImageToolApp : public CWinAppEx
{
public:
	CImageToolApp() noexcept;

public:
	IppDib* m_pNewDib;
	CMultiDocTemplate* m_pImageDocTemplate;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnWindowCloseall();
};

extern CImageToolApp theApp;

// 전역 함수 선언
void AfxNewBitmap(IppDib& dib); // 객체를 참조형으로 너겨받아 그와 동일한 DIB 객체를 생성하여 새 창으로 띄운다.
void AfxPrintInfo(CString message); // 출력창 문자열 추가 함수
void AfxPrintInfo(LPCTSTR lpszFormat, ...); // 출력창 문자열 추가 함수

