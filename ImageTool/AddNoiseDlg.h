﻿#pragma once


// CAddNoiseDlg 대화 상자

class CAddNoiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddNoiseDlg)

public:
	CAddNoiseDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAddNoiseDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_NOISE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nAmount;
	int m_nNoiseType;
	virtual BOOL OnInitDialog();
};
