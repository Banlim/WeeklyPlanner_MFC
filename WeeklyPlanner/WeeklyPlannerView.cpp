		
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
END_MESSAGE_MAP()

// CWeeklyPlannerView ����/�Ҹ�

CWeeklyPlannerView::CWeeklyPlannerView()
	: CFormView(IDD_WEEKLYPLANNER_FORM)
	, m_bModifyBtn(false)
	, m_nTodoDone(0)
	, m_todoEnd(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

	m_bChecked[8] = { true };

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
	DDX_Control(pDX, IDC_DDAY_ADD_BUTTON, m_btnaddDday);
	DDX_DateTimeCtrl(pDX, IDC_TODO_START, m_todoStart);
	DDX_DateTimeCtrl(pDX, IDC_TODO_END, m_todoEnd);
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
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_TodoAchivePrgs.SetRange(0, 1000);

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
{UpdateTodoProgressBar(&m_TodoCheck1);}



void CWeeklyPlannerView::OnClickedTodoCheckbox2()
{UpdateTodoProgressBar(&m_TodoCheck2);}


void CWeeklyPlannerView::OnClickedTodoCheckbox3()
{UpdateTodoProgressBar(&m_TodoCheck3);}


void CWeeklyPlannerView::OnClickedTodoCheckbox4()
{UpdateTodoProgressBar(&m_TodoCheck4);}


void CWeeklyPlannerView::OnClickedTodoCheckbox5()
{UpdateTodoProgressBar(&m_TodoCheck5);}


void CWeeklyPlannerView::OnClickedTodoCheckbox6()
{UpdateTodoProgressBar(&m_TodoCheck6);}


void CWeeklyPlannerView::OnClickedTodoCheckbox7()
{UpdateTodoProgressBar(&m_TodoCheck7);}


void CWeeklyPlannerView::OnClickedTodoCheckbox8()
{UpdateTodoProgressBar(&m_TodoCheck8);}

void CWeeklyPlannerView::UpdateTodoProgressBar(CButton* m_checkBtn)
{
	if (m_checkBtn->GetCheck() == BST_CHECKED) {
		m_nTodoDone++;
		m_TodoAchivePrgs.SetPos(m_TodoAchivePrgs.GetPos() + 125);
	}
	else if (m_checkBtn->GetCheck() == BST_UNCHECKED) {
		m_nTodoDone--;
		m_TodoAchivePrgs.SetPos(m_TodoAchivePrgs.GetPos() - 125);

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
	strStart.Format(_T("%d:%d"), m_todoStart.GetHour(), m_todoStart.GetMinute());
	strEnd.Format(_T("%d:%d"), m_todoEnd.GetHour(), m_todoEnd.GetMinute());
	str += strStart; str += " ~ "; str += strEnd;
	str += "  :  ";
	str += m_strTodomemo;

	int i = 0;
	while(1) {
		if (m_bChecked[i]) {
			m_bChecked[i] = false;
			break;
		}
		if (i == 7)
			break;
		i++;
	}
	if (i == 0)
		m_TodoCheck1.SetWindowText(str);
	else if (i == 1)
		m_TodoCheck2.SetWindowText(str);
	else if (i == 2)
		m_TodoCheck3.SetWindowText(str);
	else if (i == 3)
		m_TodoCheck4.SetWindowText(str);
	else if (i == 4)
		m_TodoCheck5.SetWindowText(str);
	else if (i == 5)
		m_TodoCheck6.SetWindowText(str);
	else if (i == 6)
		m_TodoCheck7.SetWindowText(str);
	else if (i == 7)
		m_TodoCheck8.SetWindowText(str);
	else
		AfxMessageBox(_T("������ �߰��� �� �����ϴ�."));


	//��ư Ŭ�� �� ���� ����
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ADD_TODO_MEMO);
	pEdit->SetWindowText(_T(" "));

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
	m_TodoCheck1.SetWindowText(_T("_______________________________"));
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[1] = true;
	m_TodoCheck2.SetWindowText(_T("_______________________________"));
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[2] = true;
	m_TodoCheck3.SetWindowText(_T("_______________________________"));
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[3] = true;
	m_TodoCheck4.SetWindowText(_T("_______________________________"));
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[4] = true;
	m_TodoCheck5.SetWindowText(_T("_______________________________"));
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[5] = true;
	m_TodoCheck6.SetWindowText(_T("_______________________________"));
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[6] = true;
	m_TodoCheck7.SetWindowText(_T("_______________________________"));
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[7] = true;
	m_TodoCheck8.SetWindowText(_T("_______________________________"));
}
