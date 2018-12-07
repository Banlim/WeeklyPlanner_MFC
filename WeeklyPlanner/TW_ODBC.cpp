
#include "stdafx.h"
#include "TW_ODBC.h"
#include "MainFrm.h"
#include "WeeklyPlannerView.h"

#pragma comment(lib, "odbc32.lib")


// �����͸� �������� �ִ� ������ �����Ѵ�.
#define MAX_COUNT 8

// ����Ž���⸦ �̿��Ͽ� ODBC �����ϱ�
// ��Ű�� �̸�, ���̵�, �н�����, ��������� ���
#define DBSchemas L"wp_db"
#define DBID L"WP_User"
#define DBPassword L"tndtlfeo"
#define DBContents Memo


struct TodoList {
	wchar_t date[25];
	bool isDone;
	wchar_t DBContents[100];
};


TW_ODBC::TW_ODBC()
	: record_num(0)
	, m_dbDataCnt(0)
{

	// ODBC ����̹��� ������ ���� �⺻ ������ �����Ѵ�.
	SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, SQL_IS_INTEGER);

	// ODBC ����� ����ϱ� ���� ȯ���� �����Ѵ�
	// ������ ȯ�� ������ ���� �ڵ� ���� mh_environment�� �����Ѵ�.
	if (SQL_ERROR != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mh_environment)) {
		// ����� ODBC ���� ������ �����Ѵ�.
		SQLSetEnvAttr(mh_environment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
		SQLSetEnvAttr(mh_environment, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, SQL_IS_INTEGER);

		// ODBC �Լ��� ����ϱ� ���� ������ �����Ѵ�.
		// �� ������ ���� �ڵ� ���� mh_odbc�� �����Ѵ�.
		if (SQL_ERROR != SQLAllocHandle(SQL_HANDLE_DBC, mh_environment, &mh_odbc)) {
			RETCODE ret_code = SQLConnect(mh_odbc,
				// ������ DSN ����
				(SQLWCHAR *)DBSchemas, SQL_NTS,
				// ���ӿ� ����� ID
				(SQLWCHAR *)DBID, SQL_NTS,
				// ���ӿ� ����� Password
				(SQLWCHAR *)DBPassword, SQL_NTS);
			if (ret_code == SQL_SUCCESS || ret_code == SQL_SUCCESS_WITH_INFO) {
				// ODBC�� ����Ͽ� �����ͺ��̽� ������ ���������� ������ ���
				m_connect_flag = 1;
				AfxMessageBox(_T("�����ͺ��̽� ���� ���� ����"));
			}
			else {
				// ���ӿ� ������ ���, �����ߴ� �޸𸮸� �����Ѵ�.
				AfxMessageBox(_T("�����ͺ��̽� ���� ���� ����"));

				if (mh_odbc != SQL_NULL_HDBC)
					SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
				if (mh_environment != SQL_NULL_HENV)
					SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
			}
		}
	}
	
	if (m_connect_flag)
		AfxMessageBox(_T("TRUE"));
}


TW_ODBC::~TW_ODBC()
{
	// ������ ����Ǿ� �ִ� ��쿡�� �����Ѵ�. ������ ������ �����ߴٸ�
	// ������ �������� �Ʒ��� ������ �����Ǿ��� �����̴�.
	if (m_connect_flag == 1) {
		if (mh_odbc != SQL_NULL_HDBC)
			SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
		if (mh_environment != SQL_NULL_HENV)
			SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
	}
}


int TW_ODBC::ImportData()
{
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();
	int cnt = -1;
	AfxMessageBox(_T("TEST"));

	// table ��������
	wchar_t query_str[256] = L"SELECT * FROM todolist";

	// �о�� �������� ���¸� ����� ����
	unsigned short record_state[MAX_COUNT];
	// �о�� �����͸� ������ ����
	TodoList raw_data[MAX_COUNT];
	CString str;
	// ���� �������� ������ ������ ����
	//unsigned long int record_num = 0;
	//record_num = 0 ;

	//�����͸� ������ �迭�� �ʱ�ȭ �Ѵ�.
	memset(raw_data, 0, sizeof(raw_data));

	HSTMT h_statement;
	RETCODE ret_code;
	// Query ���� ���� �޸𸮸� �Ҵ��Ѵ�.
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		record_num = 0;
		// Query ���� ������ �� Ÿ�� �ƿ��� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// ������ �����͸� ������ �޸��� ũ�⸦ �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data), 0);

		// �����͸� ������ �� ���ü��� ���� ����� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);

		// �����͸� �������� �ִ� ������ �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_COUNT, SQL_IS_UINTEGER);

		// ���� �������� ���¸� ������ ������ �ּҸ� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_STATUS_PTR, record_state, 0);

		// ���� �������� ������ ������ ������ �ּҸ� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);

		
		CString a;
		a.Format(_T("���� �� ������ �� %d"), record_num);
		AfxMessageBox(a);
		
		// ���̺��� ������ �����͸� �Ӽ����� raw_data ������ �����ϱ� ���ؼ�
		// �Ӽ����� ������ �޸� ��ġ�� �����Ѵ�.

		SQLBindCol(h_statement, 1, SQL_WCHAR, raw_data[0].date, sizeof(wchar_t) * 20, NULL);
		SQLBindCol(h_statement, 2, SQL_SMALLINT, &raw_data[0].isDone, sizeof(int), NULL);
		SQLBindCol(h_statement, 3, SQL_WCHAR, &raw_data[0].DBContents, sizeof(wchar_t) * 20, NULL);

		// SQL ��ɹ��� �����Ѵ�.
		ret_code = SQLExecDirect(h_statement, (SQLWCHAR*)query_str, SQL_NTS);

		// SQL ��ɹ��� ���� ����� ���� �����͸� ListBox�� �߰��Ѵ�.
		while (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {


			// ������ ������ŭ �ݺ��ϸ鼭 �۾��Ѵ�.
				// ������ �����Ͱ� ������ ������ �ƴ϶�� �ش� �Ӽ�����
				// ���ļ� ���ڿ��� �����ϰ� AfxMessageBox�� ����Ѵ�.
			if (record_state[0] != SQL_ROW_DELETED && record_state[0] != SQL_ROW_ERROR) {

				str.Format(L"%s, %d, %s", raw_data[0].date, raw_data[0].isDone, raw_data[0].DBContents);
				AfxMessageBox(_T("��ȣ��"+str));


				//������ �ҷ��� �� üũ����Ʈ�� �߰��ϱ� (���?)
				while (1) {
					if (pView->m_arrayTodoCheck[m_dbDataCnt]->IsWindowEnabled() == false) {
						break;
					}
					m_dbDataCnt++;
				}

				CString strContents;
				strContents.Format(_T("%s"), raw_data[0].Memo);

				pView->m_arrayTodoCheck[m_dbDataCnt]->SetWindowText(strContents);
				pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(true);
				pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(true);

				if (raw_data[0].isDone == true) {
						pView->m_arrayTodoCheck[m_dbDataCnt]->SetCheck(1);
						pView -> m_checkCnt++;
				}

				cnt = m_dbDataCnt;
				}	

		}



		// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}

	return cnt+1;
}
