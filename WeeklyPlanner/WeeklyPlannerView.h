
// WeeklyPlannerView.h : CWeeklyPlannerView Ŭ������ �������̽�
//

#pragma once
#include "afxwin.h"
#include "atlimage.h"
#include "afxcmn.h"
#include "atltime.h"
#include "afxdtctl.h"

class CWeeklyPlannerView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CWeeklyPlannerView();
	DECLARE_DYNCREATE(CWeeklyPlannerView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_WEEKLYPLANNER_FORM };
#endif

// Ư���Դϴ�.
public:
	CWeeklyPlannerDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CWeeklyPlannerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnMessagemodify;
	afx_msg void OnClickedMessageModifyButton();
	bool m_bModifyBtn;
	CEdit m_EditMessage;
	afx_msg void OnEnChangeMessage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_ProfilePhoto;
	CImage m_ProfileImg;
	afx_msg void OnClickedTodoCheckbox1();
	int m_nTodoDone;
	CButton m_TodoCheck1;
	CProgressCtrl m_TodoAchivePrgs;
	void UpdateTodoProgressBar(CButton* m_checkBtn);
	CButton m_TodoCheck2;
	CButton m_TodoCheck3;
	CButton m_TodoCheck4;
	CButton m_TodoCheck5;
	CButton m_TodoCheck6;
	CButton m_TodoCheck7;
	CButton m_TodoCheck8;
	afx_msg void OnClickedTodoCheckbox2();
	afx_msg void OnClickedTodoCheckbox3();
	afx_msg void OnClickedTodoCheckbox4();
	afx_msg void OnClickedTodoCheckbox5();
	afx_msg void OnClickedTodoCheckbox6();
	afx_msg void OnClickedTodoCheckbox7();
	afx_msg void OnClickedTodoCheckbox8();
	CButton m_btnaddTodo;
	afx_msg void OnEnChangeAddTodoMemo();
	afx_msg void OnBnClickedAddTodoButton();
	CButton m_btnaddDday;
	afx_msg void OnClickedDdayAddButton();
	CDateTimeCtrl m_todoStart;
	CDateTimeCtrl m_todoEnd;
	bool m_bChecked[8];
	afx_msg void OnDtnDatetimechangeTodoStart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeTodoEnd(NMHDR *pNMHDR, LRESULT *pResult);
//	CEdit m_Todomemo;
};

#ifndef _DEBUG  // WeeklyPlannerView.cpp�� ����� ����
inline CWeeklyPlannerDoc* CWeeklyPlannerView::GetDocument() const
   { return reinterpret_cast<CWeeklyPlannerDoc*>(m_pDocument); }
#endif

