// DdayAddDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"

#include "MainFrm.h"
#include "WeeklyPlanner.h"
#include "DdayAddDlg.h"
#include "afxdialogex.h"
#include "WeeklyPlannerView.h"

// CDdayAddDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDdayAddDlg, CDialogEx)

CDdayAddDlg::CDdayAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DDAY_ADD_DIALOG, pParent)
	, pView(NULL)
{

}

CDdayAddDlg::~CDdayAddDlg()
{
}

void CDdayAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnDdayOK);
	DDX_Control(pDX, IDC_EDIT_DDAY_TITLE, m_editNewDdayTitle);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_ctrlDdayCal);
}


BEGIN_MESSAGE_MAP(CDdayAddDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_DDAY_MEMO, &CDdayAddDlg::OnEnChangeEditDdayMemo)
	ON_BN_CLICKED(IDOK, &CDdayAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDdayAddDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDdayAddDlg �޽��� ó�����Դϴ�.




//BOOL CDdayAddDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
//	{
//		if ((GetDlgItem(IDC_EDIT_DDAY_MEMO) == GetFocus()))
//		{
//			CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_DDAY_MEMO);
//			int nLen = edit->GetWindowTextLength();
//			edit->SetSel(nLen, nLen);
//			edit->ReplaceSel(_T("\r\n"));
//		}
//	}
//
//
//	return CDdayAddDlg::PreTranslateMessage(pMsg);
//}


void CDdayAddDlg::OnEnChangeEditDdayMemo()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CDdayAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	GetDlgItem(IDOK)->SendMessage(WM_KILLFOCUS, NULL);
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();
	pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	m_hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_DDAY));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_hBit);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDdayAddDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);
	//pView ���� WeeklyPlannerView�� m_timeNewDday, m_strNewDdayTitle, m_strNewDdayMemo��  �� ����
	CTime tSelected, nowTime;
	CString strDdayTitle, strDday, strTmp, strforQ;
	CListCtrl* ctrlDdayList = &(pView->m_ctrlDdayList);

	int nCount =ctrlDdayList->GetItemCount()+1;
	int index;
	nowTime = CTime::GetCurrentTime();
	m_ctrlDdayCal.GetCurSel(tSelected);
	strDdayTitle.Empty();
	strDday.Empty();
	strTmp.Empty();

	
	CTimeSpan dDay = tSelected - nowTime;
	//*******************
	int m_dDay = dDay.GetTotalSeconds();
	if (dDay > 0)
		strDday.Format(_T("D-%d"), abs(m_dDay / (24 * 60 * 60) + 1));
	else if(abs(m_dDay / (24 * 60 * 60)) == 0)
		strDday.Format(_T("D-Day"));
	else
		strDday.Format(_T("D+%d"), abs(m_dDay / (24 * 60 * 60)));
	pView->m_strNewDdayDate = strDday;
	//********************

	strTmp = tSelected.Format(_T("%Y-%m-%d"));
	pView->m_timeNewDday = strTmp;

	m_editNewDdayTitle.GetWindowText(strDdayTitle);
	pView->m_strNewDdayTitle = strDdayTitle;
	
	index = ctrlDdayList->InsertItem(nCount, strTmp);
	//ctrlDdayList->SetItem(index, 0, LVIF_TEXT, strDday, 0, 0, 0, NULL);
	ctrlDdayList->SetItem(index, 1, LVIF_TEXT, strDdayTitle, 0, 0, 0, NULL);
	ctrlDdayList->SetItem(index, 2, LVIF_TEXT, strDday, 0, 0, 0, NULL);
	////////////////////////////////////////////////////////////

	strforQ.Format(L"INSERT INTO dday VALUES ('%s', '%s')", strTmp, strDdayTitle);

	SQLHSTMT h_statement_forDday;


	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, pView->m_odbc->GetMh_odbc(), &h_statement_forDday)) {
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		RETCODE ret = SQLExecDirect(h_statement_forDday, (SQLWCHAR*)(const wchar_t *)strforQ, SQL_NTS);

		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		}

		SQLEndTran(SQL_HANDLE_ENV, pView->m_odbc->GetMh_odbc(), SQL_COMMIT);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forDday);

	}

	CDialogEx::OnOK();


}


BOOL CDdayAddDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{

		if ((GetDlgItem(IDC_EDIT_DDAY_MEMO) == GetFocus()))
		{
			CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_DDAY_MEMO);
			int nLen = edit->GetWindowTextLength();
			edit->SetSel(nLen, nLen);
			edit->ReplaceSel(_T("\r\n"));
			return true;
		}


	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDdayAddDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}


//void CDdayAddDlg::OnDestroy()
//{
//	CDialogEx::OnDestroy();
//
//	pView->UpdateData(TRUE);
//
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//}


void CDdayAddDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
					   // ��ũ���� ȣȯ�Ǵ� DC����.
	HDC hMemDC = CreateCompatibleDC(dc);
	// ȣȯDC�� ��Ʈ���� ����.
	SelectObject(hMemDC, m_hBitmap);
	// �޸� DC���� ��ũ�� DC�� �̹��� ����
	BitBlt(dc, 0, 0, m_hBit.bmWidth, m_hBit.bmHeight, hMemDC, 0, 0, SRCCOPY);
	// �޸� DC����
	DeleteDC(hMemDC);

}


HBRUSH CDdayAddDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (nCtlColor)
	{
	case (CTLCOLOR_STATIC):
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	}


	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}
