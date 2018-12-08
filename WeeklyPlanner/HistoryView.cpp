// HistoryView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WeeklyPlanner.h"
#include "HistoryView.h"
#include "afxdialogex.h"
#include "WeeklyPlannerView.h"


// CHistoryView ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CHistoryView, CDialogEx)

CHistoryView::CHistoryView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HISTORY, pParent)
	, m_vHistory(COleDateTime::GetCurrentTime())
{
		
}

CHistoryView::~CHistoryView()
{
}

void CHistoryView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER_HISTORY, m_ctrlHistory);
	//  DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HISTORY, m_vHistory);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER_HISTORY, m_ctrlHistory);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HISTORY, m_vHistory);
	DDX_Control(pDX, IDC_LIST_HISTORY, m_listHistory);
	DDX_Control(pDX, IDC_PROGRESS_HISTORY, m_progressHistory);
}


BEGIN_MESSAGE_MAP(CHistoryView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_OK, &CHistoryView::OnBnClickedButtonHistoryOk)
END_MESSAGE_MAP()


// CHistoryView �޽��� ó�����Դϴ�.


void CHistoryView::OnBnClickedButtonHistoryOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// �̷��� �ϸ� ������ ��¥�� int������ �޾ƿ�. 
	UpdateData();
	int Year = m_vHistory.GetYear();
	int Month = m_vHistory.GetMonth();
	int day = m_vHistory.GetDay();

	// CString���� ��ȯ�ε� �ʿ� ������ ������ ��.
	CString date;
	date.Format(_T("%d-%d-%d"), Year, Month, day);

	// ���⿡�� ��� �ҷ��� �� ����Ʈ �ڽ��� ��������� �˴ϴ�.
	// �޼��� �κп��� ���α׷����ٴ� �� üũ �ߴ��� ���ߴ��� �κ� ���� ���?



	//m_listHistory.AddString(date);
	UpdateData(FALSE);

}
