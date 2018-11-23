
// WeeklyPlannerView.h : CWeeklyPlannerView Ŭ������ �������̽�
//

#pragma once


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
};

#ifndef _DEBUG  // WeeklyPlannerView.cpp�� ����� ����
inline CWeeklyPlannerDoc* CWeeklyPlannerView::GetDocument() const
   { return reinterpret_cast<CWeeklyPlannerDoc*>(m_pDocument); }
#endif

