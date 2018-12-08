#pragma once
#include "afxdtctl.h"
#include "ATLComTime.h"
#include "afxwin.h"
#include "afxcmn.h"


// CHistoryView ��ȭ �����Դϴ�.

class CHistoryView : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryView)

public:
	CHistoryView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHistoryView();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HISTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_vHistory;
	afx_msg void OnBnClickedButtonHistoryOk();
	CProgressCtrl m_progressHistory;
	CStatic m_historyCheck1;
	CStatic m_historyCheck2;
	CStatic m_historyCheck3;
	CStatic m_historyCheck4;
	CStatic m_historyCheck5;
	CStatic m_historyCheck6;
	CStatic m_historyCheck7;
	CStatic m_historyCheck8;
	CStatic m_historyPercent;
	CStatic* m_arrayHistoryCheck[8];
};
