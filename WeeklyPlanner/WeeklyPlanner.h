
// WeeklyPlanner.h : WeeklyPlanner ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CWeeklyPlannerApp:
// �� Ŭ������ ������ ���ؼ��� WeeklyPlanner.cpp�� �����Ͻʽÿ�.
//

class CWeeklyPlannerApp : public CWinAppEx
{
public:
	CWeeklyPlannerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWeeklyPlannerApp theApp;
