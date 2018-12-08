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
//	CDateTimeCtrl m_ctrlHistory;
//	COleDateTime m_vHistory;
//	CDateTimeCtrl m_ctrlHistory;
	COleDateTime m_vHistory;
	afx_msg void OnBnClickedButtonHistoryOk();
	CListBox m_listHistory;
	CProgressCtrl m_progressHistory;
};
