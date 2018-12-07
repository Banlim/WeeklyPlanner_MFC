
// WeeklyPlannerView.cpp : CWeeklyPlannerView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "WeeklyPlanner.h"
#endif

#include "WeeklyPlannerDoc.h"
#include "WeeklyPlannerView.h"
#include "SoundPlayer.h"
#include "DdayAddDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWeeklyPlannerView

IMPLEMENT_DYNCREATE(CWeeklyPlannerView, CFormView)

BEGIN_MESSAGE_MAP(CWeeklyPlannerView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_MESSAGE_MODIFY_BUTTON, &CWeeklyPlannerView::OnClickedMessageModifyButton)
	ON_EN_CHANGE(IDC_MESSAGE, &CWeeklyPlannerView::OnEnChangeMessage)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX1, &CWeeklyPlannerView::OnClickedTodoCheckbox1)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX2, &CWeeklyPlannerView::OnClickedTodoCheckbox2)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX3, &CWeeklyPlannerView::OnClickedTodoCheckbox3)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX4, &CWeeklyPlannerView::OnClickedTodoCheckbox4)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX5, &CWeeklyPlannerView::OnClickedTodoCheckbox5)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX6, &CWeeklyPlannerView::OnClickedTodoCheckbox6)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX7, &CWeeklyPlannerView::OnClickedTodoCheckbox7)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX8, &CWeeklyPlannerView::OnClickedTodoCheckbox8)
	ON_EN_CHANGE(IDC_ADD_TODO_MEMO, &CWeeklyPlannerView::OnEnChangeAddTodoMemo)
	ON_BN_CLICKED(IDC_ADD_TODO_BUTTON, &CWeeklyPlannerView::OnBnClickedAddTodoButton)
	ON_BN_CLICKED(IDC_DDAY_ADD_BUTTON, &CWeeklyPlannerView::OnClickedDdayAddButton)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TODO_START, &CWeeklyPlannerView::OnDtnDatetimechangeTodoStart)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TODO_END, &CWeeklyPlannerView::OnDtnDatetimechangeTodoEnd)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON1, &CWeeklyPlannerView::OnClickedTodoDeleteButton1)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON2, &CWeeklyPlannerView::OnClickedTodoDeleteButton2)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON3, &CWeeklyPlannerView::OnClickedTodoDeleteButton3)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON4, &CWeeklyPlannerView::OnClickedTodoDeleteButton4)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON5, &CWeeklyPlannerView::OnClickedTodoDeleteButton5)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON6, &CWeeklyPlannerView::OnClickedTodoDeleteButton6)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON7, &CWeeklyPlannerView::OnClickedTodoDeleteButton7)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON8, &CWeeklyPlannerView::OnClickedTodoDeleteButton8)
	ON_BN_CLICKED(IDC_BUTTON_PROFILE_OPEN, &CWeeklyPlannerView::OnBnClickedButtonProfileOpen)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PROFILE_DELETE, &CWeeklyPlannerView::OnClickedButtonProfileDelete)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_DDAY_LIST_CNTL, &CWeeklyPlannerView::OnLvnItemchangedDdayListCntl)
ON_NOTIFY(LVN_INSERTITEM, IDC_DDAY_LIST_CNTL, &CWeeklyPlannerView::OnInsertitemDdayListCntl)
ON_BN_CLICKED(IDC_DDAY_DELETE_BUTTON, &CWeeklyPlannerView::OnBnClickedDdayDeleteButton)
ON_NOTIFY(NM_CLICK, IDC_DDAY_LIST_CNTL, &CWeeklyPlannerView::OnClickDdayListCntl)
ON_BN_CLICKED(IDC_PAUSE_SONG, &CWeeklyPlannerView::OnClickedPauseSong)
ON_BN_CLICKED(IDC_PREV_SONG, &CWeeklyPlannerView::OnClickedPrevSong)
ON_BN_CLICKED(IDC_NEXT_SONG, &CWeeklyPlannerView::OnClickedNextSong)
ON_BN_CLICKED(IDC_SONGS, &CWeeklyPlannerView::OnBnClickedSongs)
ON_BN_CLICKED(IDC_BUTTON_SOUND_DELETE, &CWeeklyPlannerView::OnBnClickedButtonSoundDelete)
END_MESSAGE_MAP()

// CWeeklyPlannerView ����/�Ҹ�

CWeeklyPlannerView::CWeeklyPlannerView()
	: CFormView(IDD_WEEKLYPLANNER_FORM)
	, m_nTodoDone(0)
	, m_todoEnd(0)
	, m_checkCnt(0)
	, m_strProfilePath(_T(""))
	, m_timeNewDday(_T(""))
	, m_strNewDdayTitle(_T(""))
	, m_strNewDdayDate(_T(""))
	, m_bModifyBtn(false)
	, m_nDdayListSelectedItem(0)
	, percent(1000)
	, m_nPlayPause(0)
	, m_nPlayIndex(0)
	, m_nSizeProfileX(0)
	, m_nSizeProfileY(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_odbc = new TW_ODBC();
	//
	m_bChecked[8] = { true };
	//
	m_arrayTodoCheck[0] = &m_TodoCheck1;
	m_arrayTodoCheck[1] = &m_TodoCheck2;
	m_arrayTodoCheck[2] = &m_TodoCheck3;
	m_arrayTodoCheck[3] = &m_TodoCheck4;
	m_arrayTodoCheck[4] = &m_TodoCheck5;
	m_arrayTodoCheck[5] = &m_TodoCheck6;
	m_arrayTodoCheck[6] = &m_TodoCheck7;
	m_arrayTodoCheck[7] = &m_TodoCheck8;
	//
	m_arrayTodoBtn[0] = &m_TodoBtn1;
	m_arrayTodoBtn[1] = &m_TodoBtn2;
	m_arrayTodoBtn[2] = &m_TodoBtn3;
	m_arrayTodoBtn[3] = &m_TodoBtn4;
	m_arrayTodoBtn[4] = &m_TodoBtn5;
	m_arrayTodoBtn[5] = &m_TodoBtn6;
	m_arrayTodoBtn[6] = &m_TodoBtn7;
	m_arrayTodoBtn[7] = &m_TodoBtn8;
}

CWeeklyPlannerView::~CWeeklyPlannerView()
{
}

void CWeeklyPlannerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_MODIFY_BUTTON, m_btnMessagemodify);
	DDX_Control(pDX, IDC_MESSAGE, m_EditMessage);
	DDX_Control(pDX, IDC_PROFILE_PHOTO, m_ProfilePhoto);
	DDX_Control(pDX, IDC_TODO_CHECKBOX1, m_TodoCheck1);
	DDX_Control(pDX, IDC_TODO_ACHIVE_PROGRESS, m_TodoAchivePrgs);
	DDX_Control(pDX, IDC_TODO_CHECKBOX2, m_TodoCheck2);
	DDX_Control(pDX, IDC_TODO_CHECKBOX3, m_TodoCheck3);
	DDX_Control(pDX, IDC_TODO_CHECKBOX4, m_TodoCheck4);
	DDX_Control(pDX, IDC_TODO_CHECKBOX5, m_TodoCheck5);
	DDX_Control(pDX, IDC_TODO_CHECKBOX6, m_TodoCheck6);
	DDX_Control(pDX, IDC_TODO_CHECKBOX7, m_TodoCheck7);
	DDX_Control(pDX, IDC_TODO_CHECKBOX8, m_TodoCheck8);
	DDX_Control(pDX, IDC_ADD_TODO_BUTTON, m_btnaddTodo);
	DDX_DateTimeCtrl(pDX, IDC_TODO_START, m_todoStart);
	DDX_DateTimeCtrl(pDX, IDC_TODO_END, m_todoEnd);
	DDX_Control(pDX, IDC_TODO_START, m_cTodoStart);
	DDX_Control(pDX, IDC_TODO_END, m_cTodoEnd);
	DDX_Control(pDX, IDC_DDAY_LIST_CNTL, m_ctrlDdayList);
	DDX_Control(pDX, IDC_DDAY_DELETE_BUTTON, m_btnDeleteDday);
	DDX_Control(pDX, IDC_DDAY_ADD_BUTTON, m_btnAddDday);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON1, m_TodoBtn1);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON2, m_TodoBtn2);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON3, m_TodoBtn3);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON4, m_TodoBtn4);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON5, m_TodoBtn5);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON6, m_TodoBtn6);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON7, m_TodoBtn7);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON8, m_TodoBtn8);
	DDX_Control(pDX, IDC_PAUSE_SONG, m_btnPlayPause);
	DDX_Control(pDX, IDC_PLAYLIST, m_soundPlayList);
	DDX_Control(pDX, IDC_MUSIC_INFO, m_strSongName);
}

BOOL CWeeklyPlannerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CWeeklyPlannerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	
	m_TodoAchivePrgs.SetRange(0, 1000);
	m_cTodoStart.SetFormat(_T("tt  HH: mm"));
	m_cTodoEnd.SetFormat(_T("tt  HH: mm"));
	// set using the SYSTEMTIME
	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));
	// DB���� ���� ��¥ �ҷ��ͼ� ������ ������ �� �־����� �Ѵ�.
	sysTime.wYear = 1998;
	sysTime.wMonth = 4;
	sysTime.wDay = 3;
	VERIFY(m_cTodoStart.SetTime(&sysTime));
	VERIFY(m_cTodoEnd.SetTime(&sysTime));

	UpdateData(TRUE);
	/*CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "�ּҰ��� �о�� �� �����ϴ�.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, 180, 180, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);*/


	//List Control ����
	CRect list_rect;
	m_ctrlDdayList.GetWindowRect(&list_rect);
	m_ctrlDdayList.EnableScrollBar(FALSE);
	LPWSTR list[3] = { _T("���� ����"), _T("����"),_T("���� ��¥") };
	double nWidth[3] = { list_rect.Width()*0.3, 0.0 ,list_rect.Width()*0.25};
	nWidth[1] = list_rect.Width() - nWidth[0] - nWidth[2]-3;
	m_ctrlDdayList.GetHeaderCtrl()->EnableWindow(FALSE);

	//List Column ����
	for (int i = 0; i < 3; i++) {
		m_ctrlDdayList.InsertColumn(i, list[i], LVCFMT_CENTER, nWidth[i]);
	}
	m_ctrlDdayList.SetExtendedStyle(m_ctrlDdayList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	((CButton*)GetDlgItem(IDC_DDAY_DELETE_BUTTON))->EnableWindow(FALSE);
	//((CButton*)GetDlgItem(IDC_DDAY_MODIFY_BUTTON))->EnableWindow(FALSE);

	// �� ũ�� ���
	this->GetWindowRect(winRect);
	m_nSizeProfileX = (int)winRect.right * 0.115;
	m_nSizeProfileY = (int)winRect.bottom * 0.23;

	//SoundList ����
	m_soundPlayList.InsertColumn(0, L"����", LVCFMT_CENTER, 600);
	for (int i = 0; i < m_soundSP.m_nSoundIndex; i++)
	{
		m_soundPlayList.InsertItem(i, m_soundSP.m_strSoundName[i]);
	}


	m_odbc->ImportData();
}

void CWeeklyPlannerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWeeklyPlannerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWeeklyPlannerView ����

#ifdef _DEBUG
void CWeeklyPlannerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWeeklyPlannerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWeeklyPlannerDoc* CWeeklyPlannerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeeklyPlannerDoc)));
	return (CWeeklyPlannerDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeeklyPlannerView �޽��� ó����


void CWeeklyPlannerView::OnClickedMessageModifyButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bModifyBtn) { //���� �϶�
		m_EditMessage.EnableWindow(true);
		m_btnMessagemodify.SetWindowText(_T("����"));
		m_bModifyBtn = true;

	}
	else {
		m_EditMessage.EnableWindow(false);
		m_btnMessagemodify.SetWindowText(_T("����"));

		m_bModifyBtn = false;
	}

}


void CWeeklyPlannerView::OnEnChangeMessage()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CWeeklyPlannerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{

		if ((GetDlgItem(IDC_MESSAGE) == GetFocus()))
		{
			CEdit* edit = (CEdit*)GetDlgItem(IDC_MESSAGE);
			int nLen = edit->GetWindowTextLength();
			edit->SetSel(nLen, nLen);
			edit->ReplaceSel(_T("\r\n"));
		}
		if ((GetDlgItem(IDC_ADD_TODO_MEMO) == GetFocus())) {
			CEdit* edit = (CEdit*)GetDlgItem(IDC_ADD_TODO_MEMO);
			int nLen = edit->GetWindowTextLength();
			edit->SetSel(nLen, nLen);
			edit->ReplaceSel(_T("\r\n"));
		}

	}

	return CFormView::PreTranslateMessage(pMsg);
}




void CWeeklyPlannerView::OnClickedTodoCheckbox1()
{
	if (m_TodoCheck1.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck1.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck1);
}



void CWeeklyPlannerView::OnClickedTodoCheckbox2()
{
	if (m_TodoCheck2.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck2.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck2);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox3()
{
	if (m_TodoCheck3.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck3.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck3);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox4()
{
	if (m_TodoCheck4.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck4.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck4);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox5()
{
	if (m_TodoCheck5.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck5.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck5);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox6()
{
	if (m_TodoCheck6.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck6.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck6);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox7()
{
	if (m_TodoCheck7.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck7.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck7);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox8()
{
	if (m_TodoCheck8.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck8.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck8);
}

void CWeeklyPlannerView::UpdateTodoProgressBar(CButton* m_checkBtn)
{
	int i = 8, j = 0, cnt = 0;
	while (i--) {
		if (!m_bChecked[j])
			cnt++;
		j++;
	}
	
	if (cnt == 0 || m_checkCnt == 0) {
		m_TodoAchivePrgs.SetPos(0);
	}
	else {
		if (cnt == m_checkCnt) {
			m_TodoAchivePrgs.SetPos(1000);
		}
		else {
			percent = 1000 / cnt;
			m_TodoAchivePrgs.SetPos(m_checkCnt*percent);
		}
	}
}


void CWeeklyPlannerView::OnEnChangeAddTodoMemo()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CWeeklyPlannerView::OnBnClickedAddTodoButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str, strStart, strEnd, m_strTodomemo;
	GetDlgItemText(IDC_ADD_TODO_MEMO, m_strTodomemo);
	if (m_todoStart.GetMinute() < 10)
		strStart.Format(_T("%d:0%d"), m_todoStart.GetHour(), m_todoStart.GetMinute());
	else
		strStart.Format(_T("%d:%d"), m_todoStart.GetHour(), m_todoStart.GetMinute());
	if (m_todoEnd.GetMinute() < 10)
		strEnd.Format(_T("%d:0%d"), m_todoEnd.GetHour(), m_todoEnd.GetMinute());
	else
		strEnd.Format(_T("%d:%d"), m_todoEnd.GetHour(), m_todoEnd.GetMinute());
	str += strStart; str += " ~ "; str += strEnd;
	str += "  :  ";
	str += m_strTodomemo;

	int i = 0;
	while (1) {
		if (m_bChecked[i]) {
			m_bChecked[i] = false;
			break;
		}
		if (i == 8) {
			break;
		}
		i++;
	}

	if (i <= 7) {
		m_arrayTodoCheck[i]->SetWindowText(str);
		m_arrayTodoCheck[i]->EnableWindow(true);
		m_arrayTodoBtn[i]->EnableWindow(true);
		UpdateTodoProgressBar(m_arrayTodoCheck[i]);
	}
	else
		AfxMessageBox(_T("������ �߰��� �� �����ϴ�."));

	//��ư Ŭ�� �� ���� ����
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ADD_TODO_MEMO);
	pEdit->SetWindowText(_T(""));

	// DB�� �ø���
}


void CWeeklyPlannerView::OnClickedDdayAddButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CDdayAddDlg m_ddayaddDlg;
	m_ddayaddDlg.DoModal();
}


void CWeeklyPlannerView::OnDtnDatetimechangeTodoStart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	*pResult = 0;
}


void CWeeklyPlannerView::OnDtnDatetimechangeTodoEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	*pResult = 0;
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[0] = true;
	MoveCheckboxStr(1);
	m_arrayTodoCheck[7]->SetWindowText(_T("_______________________________"));
	m_bChecked[7] = true;
	m_arrayTodoCheck[7]->EnableWindow(false);
	UpdateTodoProgressBar(&m_TodoCheck1);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[1] = true;
	MoveCheckboxStr(2);
	UpdateTodoProgressBar(&m_TodoCheck2);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[2] = true;
	MoveCheckboxStr(3);
	UpdateTodoProgressBar(&m_TodoCheck3);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[3] = true;
	MoveCheckboxStr(4);
	UpdateTodoProgressBar(&m_TodoCheck4);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[4] = true;
	MoveCheckboxStr(5);
	UpdateTodoProgressBar(&m_TodoCheck5);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[5] = true;
	MoveCheckboxStr(6);
	UpdateTodoProgressBar(&m_TodoCheck6);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[6] = true;
	MoveCheckboxStr(7);
	UpdateTodoProgressBar(&m_TodoCheck7);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[7] = true;
	m_TodoCheck8.SetWindowText(_T("_______________________________"));
	m_arrayTodoCheck[7]->SetCheck(false);
	m_arrayTodoCheck[7]->EnableWindow(false);
	m_arrayTodoBtn[7]->EnableWindow(false);
	UpdateTodoProgressBar(&m_TodoCheck8);
}

void CWeeklyPlannerView::MoveCheckboxStr(int i)
{
	if (m_arrayTodoCheck[i-1]->GetCheck() == BST_CHECKED)
		m_checkCnt--;
	while (i<8) {
		CString str;
		m_arrayTodoCheck[i]->GetWindowText(str);
		m_arrayTodoCheck[i - 1]->SetWindowText(str);
		m_bChecked[i - 1] = m_bChecked[i];
		if (m_arrayTodoCheck[i]->GetCheck() == BST_UNCHECKED)
			m_arrayTodoCheck[i - 1]->SetCheck(false);
		else if (m_arrayTodoCheck[i]->GetCheck() == BST_CHECKED)
			m_arrayTodoCheck[i - 1]->SetCheck(true);
		boolean isEnabled = m_arrayTodoCheck[i]->IsWindowEnabled();
		m_arrayTodoCheck[i - 1]->EnableWindow(isEnabled);
		isEnabled = m_arrayTodoBtn[i]->IsWindowEnabled();
		m_arrayTodoBtn[i - 1]->EnableWindow(isEnabled);
		i++;
	}
	m_arrayTodoCheck[7]->SetWindowText(_T("_______________________________"));
	m_bChecked[7] = true;
	m_arrayTodoCheck[7]->EnableWindow(false);
	m_arrayTodoBtn[7]->EnableWindow(false);
}

void CWeeklyPlannerView::OnBnClickedButtonProfileOpen()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//dimx, dimy : image size;
	
	static TCHAR BASED_CODE szFilter[] = _T("�̹��� ����(*.bmp, *.jpg, *.png)|*.bmp; *.jpg; *.png|�������(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		m_strProfilePath = dlg.GetPathName();

		CImage profileImage;
		profileImage.Load(m_strProfilePath);

		CDC *screenDC = GetDC();
		CDC mDC;
		mDC.CreateCompatibleDC(screenDC);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(screenDC, m_nSizeProfileX, m_nSizeProfileY);
		CBitmap *bmp = mDC.SelectObject(&bitmap);
		mDC.SetStretchBltMode(HALFTONE);
		profileImage.StretchBlt(mDC.m_hDC, 0, 0, m_nSizeProfileX, m_nSizeProfileY, 0, 0, profileImage.GetWidth(), profileImage.GetHeight(), SRCCOPY);
		mDC.SelectObject(bmp);

		((CStatic*)GetDlgItem(IDC_PROFILE_PHOTO))->SetBitmap((HBITMAP)bitmap.Detach());
		ReleaseDC(screenDC);

		//CImage img;
		//img.Load(_T("res\\1.png"));
		//img.BItBlt(pDC, x, y);

	}
}

void CWeeklyPlannerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CFormView::OnPaint()��(��) ȣ������ ���ʽÿ�.

	//������ ����Ʈ ���
	CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "�ּҰ��� �о�� �� �����ϴ�.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, m_nSizeProfileX, m_nSizeProfileY, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);
}



void CWeeklyPlannerView::OnClickedButtonProfileDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "�ּҰ��� �о�� �� �����ϴ�.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, m_nSizeProfileX, m_nSizeProfileY, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);
	
}


//void CWeeklyPlannerView::OnLvnItemchangedDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	NM_LISTVIEW * pNMListView = (NM_LISTVIEW *)pNMHDR;
//	int iColumn = 2;
//
//	int nItem = m_ctrlDdayList.GetItemCount();
//	CString ** arStr = new CString*[nItem];
//	for (int i = 0; i < nItem; i++) {
//		arStr[i] = new CString(m_ctrlDdayList.GetItemText(i, iColumn));
//		m_ctrlDdayList.SetItemData(i, (LPARAM)arStr[i]);
//	}
//
//	//ù��° �μ��� ������ ���� �Լ�, �ι�°�μ��� ���� ���
//	m_ctrlDdayList.SortItems(CompareFunc, (LPARAM)m_bSortAscending);
//
//	for (int i = 0; i < nItem; i++) {
//		delete arStr[i];
//	}
//	delete[]arStr;
//
//	UpdateData(TRUE);
//
//	*pResult = 0;
//}


//void CWeeklyPlannerView::OnInsertitemDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	NM_LISTVIEW * pNMListView = (NM_LISTVIEW *)pNMHDR;
//	int iColumn = 2;
//
//	int nItem = m_ctrlDdayList.GetItemCount();
//	CString ** arStr = new CString*[nItem];
//	for (int i = 0; i < nItem; i++) {
//		arStr[i] = new CString(m_ctrlDdayList.GetItemText(i, iColumn));
//		m_ctrlDdayList.SetItemData(i, (LPARAM)arStr[i]);
//	}
//
//	//ù��° �μ��� ������ ���� �Լ�, �ι�°�μ��� ���� ���
//	m_ctrlDdayList.SortItems(CompareFunc, (LPARAM)m_bSortAscending);
//
//	for (int i = 0; i < nItem; i++) {
//		delete arStr[i];
//	}
//	delete[]arStr;
//
//	UpdateData(TRUE);
//
//	*pResult = 0;
//
//}


void CWeeklyPlannerView::OnLvnItemchangedDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CWeeklyPlannerView::OnInsertitemDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	bool m_bListValid = m_ctrlDdayList.GetItemCount();
	m_btnDeleteDday.EnableWindow(m_bListValid);
//	m_btnModifyDday.EnableWindow(m_bListValid);
	UpdateData(TRUE);

	*pResult = 0;
}


void CWeeklyPlannerView::OnBnClickedDdayDeleteButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_nDdayListSelectedItem >= 0) {
		m_ctrlDdayList.DeleteItem(m_nDdayListSelectedItem);
		m_nDdayListSelectedItem = -1;
	}
	if (m_ctrlDdayList.GetItemCount() == 0) {
		m_btnDeleteDday.EnableWindow(FALSE);
	}
	UpdateData(TRUE);
}

void CWeeklyPlannerView::OnClickDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_nDdayListSelectedItem = pNMItemActivate->iItem;

	*pResult = 0;
}


void CWeeklyPlannerView::OnClickedPauseSong()
{
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_nPlayPause == 0)
	{
		//AfxMessageBox(_T("��������"));
		m_soundSP.SoundPlay(m_nPlayIndex);
		m_nPlayPause = 1;
		m_btnPlayPause.SetWindowText(_T("��"));
		m_strSongName.SetWindowText(m_soundSP.m_strSoundName[m_nPlayIndex]);

	}
	else
	{
		//AfxMessageBox(_T("������������"));
		m_soundSP.SoundStop();
		m_nPlayPause = 0;
		m_btnPlayPause.SetWindowText(_T("��"));
		m_strSongName.SetWindowText(m_soundSP.m_strSoundName[m_nPlayIndex]);
	}

}


void CWeeklyPlannerView::OnClickedPrevSong()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_soundSP.SoundStop();
	if (m_nPlayIndex <= 0) {
		m_nPlayIndex = m_soundSP.m_nSoundIndex - 1;
	}
	
	else 
	{
		m_nPlayIndex -= 1;
	}
		
	m_soundSP.SoundPlay(m_nPlayIndex);
	m_strSongName.SetWindowText(m_soundSP.m_strSoundName[m_nPlayIndex]);

}


void CWeeklyPlannerView::OnClickedNextSong()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_soundSP.SoundStop();
	//CString tmp;
	//tmp.Format(_T("%d"), m_nPlayIndex);
	//AfxMessageBox(tmp);
	if (m_nPlayIndex >= m_soundSP.m_nSoundIndex-1) {
		
		m_nPlayIndex = 0;
	}
	else {
		m_nPlayIndex += 1;
	}
	//CString tmp;
	//tmp.Format(_T("%d"), m_soundSP.m_nSoundIndex);
	//AfxMessageBox(tmp);
	m_soundSP.SoundPlay(m_nPlayIndex);
	m_strSongName.SetWindowText(m_soundSP.m_strSoundName[m_nPlayIndex]);
}


void CWeeklyPlannerView::OnBnClickedSongs()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	static TCHAR BASED_CODE szFilter[] = _T("wav ����(*.wav)|*.wav|�������(*.*)|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, szFilter);

	CString SoundPath;
	CString fileName;
	
	POSITION Pos;

	CFileFind finder;
	int m_nIndex = m_soundSP.m_nSoundIndex;

	if (fileDlg.DoModal() == IDOK)
	{
		Pos = fileDlg.GetStartPosition();
		while (Pos)
		{
			SoundPath = fileDlg.GetNextPathName(Pos);
			
			m_soundSP.m_strSoundPath[m_nIndex] = SoundPath;

			finder.FindFile(SoundPath);
			finder.FindNextFile();
			m_soundSP.m_strSoundName[m_nIndex] = finder.GetFileTitle();
			m_soundPlayList.InsertItem(m_soundPlayList.GetItemCount(), finder.GetFileTitle());
			m_nIndex += 1;

			finder.Close();
		}
		m_soundSP.m_nSoundIndex = m_nIndex;
	}

}


void CWeeklyPlannerView::OnBnClickedButtonSoundDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	POSITION pos = m_soundPlayList.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int nitem = m_soundPlayList.GetNextSelectedItem(pos);
		
		m_soundSP.m_strSoundPath[nitem] = "";
		m_soundSP.m_strSoundName[nitem] = "";
		for (int i = nitem; i < m_soundSP.m_nSoundIndex; i++)
		{
			if (i + 1 == m_soundSP.m_nSoundIndex)
				break;
			m_soundSP.m_strSoundPath[i] = m_soundSP.m_strSoundPath[i + 1];
			m_soundSP.m_strSoundPath[i + 1] = "";
			m_soundSP.m_strSoundName[i] = m_soundSP.m_strSoundName[i + 1];
			m_soundSP.m_strSoundName[i + 1] = "";
		}
		m_soundSP.m_nSoundIndex -= 1;
		//CString tmp;
		//tmp.Format(_T("%s"), m_soundSP.m_strSoundPath[nitem]);
		//AfxMessageBox(tmp);
		//tmp.Format(_T("%s"), m_soundSP.m_strSoundName[nitem]);
		//AfxMessageBox(tmp);
		m_soundPlayList.DeleteItem(nitem);
		pos = m_soundPlayList.GetFirstSelectedItemPosition();
		
	
	}
}
