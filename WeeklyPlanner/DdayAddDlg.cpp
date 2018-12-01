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
	//  DDX_Control(pDX, IDC_EDIT_DDAY_MEMO, m_editDdayMemo);
	DDX_Control(pDX, IDOK, m_btnDdayOK);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_ctrlDdayCal);
	DDX_Control(pDX, IDC_EDIT_DDAY_TITLE, m_editNewDdayTitle);
	DDX_Control(pDX, IDC_EDIT_DDAY_MEMO, m_editNewDdayMemo);
}


BEGIN_MESSAGE_MAP(CDdayAddDlg, CDialogEx)
	//ON_EN_CHANGE(IDC_EDIT2, &CDdayAddDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT_DDAY_MEMO, &CDdayAddDlg::OnEnChangeEditDdayMemo)
	ON_BN_CLICKED(IDOK, &CDdayAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDdayAddDlg::OnBnClickedCancel)
//	ON_WM_DESTROY()
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

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDdayAddDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//pView ���� WeeklyPlannerView�� m_timeNewDday, m_strNewDdayTitle, m_strNewDdayMemo��  �� ����
	CTime tSelected;
	CString strDdayTitle, strDdayMemo, strTmp;
	CListCtrl* ctrlDdayList = &(pView->m_ctrlDdayList);

	int nCount =ctrlDdayList->GetItemCount();
	nCount++;


	m_ctrlDdayCal.GetCurSel(tSelected);
	pView->m_timeNewDday = tSelected;

	m_editNewDdayTitle.GetWindowText(strDdayTitle);
	strTmp.Format(_T("%s"), strDdayTitle);	

	ctrlDdayList->InsertItem(nCount, strTmp);
	ctrlDdayList->SetItem(nCount, 1, LVIF_TEXT,strDdayTitle, 0, 0, 0, 0);
	ctrlDdayList->SetItem(nCount, 2, LVIF_TEXT, strDdayTitle, 0, 0, 0, 0);

	CDialogEx::OnOK();
	UpdateData(TRUE);

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
