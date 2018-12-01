// DdayAddDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "MainFrm.h"
#include "WeeklyPlanner.h"
#include "DdayAddDlg.h"
#include "afxdialogex.h"
#include "WeeklyPlannerView.h"

// CDdayAddDlg 대화 상자입니다.

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


// CDdayAddDlg 메시지 처리기입니다.




//BOOL CDdayAddDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CDdayAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDOK)->SendMessage(WM_KILLFOCUS, NULL);
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();
	pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDdayAddDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//pView 통해 WeeklyPlannerView의 m_timeNewDday, m_strNewDdayTitle, m_strNewDdayMemo에  값 전달
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


//void CDdayAddDlg::OnDestroy()
//{
//	CDialogEx::OnDestroy();
//
//	pView->UpdateData(TRUE);
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//}
