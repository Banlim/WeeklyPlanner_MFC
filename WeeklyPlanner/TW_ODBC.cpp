#include "stdafx.h"
#include "TW_ODBC.h"

#pragma comment(lib, "odbc32.lib")

// �����͸� �������� �ִ� ������ �����Ѵ�.
#define MAX_COUNT 100

// ����Ž���⸦ �̿��Ͽ� ODBC �����ϱ�
// ��Ű�� �̸�, ���̵�, �н����� ���
#define DBSchemas L"WeeklyPlanner"
#define DBID L"haey"
#define DBPassword L""


struct TodoList {
	wchar_t date[25];
	int isDone;
	wchar_t todolist[100];
};


TW_ODBC::TW_ODBC()
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
			}
			else {
				// ���ӿ� ������ ���, �����ߴ� �޸𸮸� �����Ѵ�.
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


void TW_ODBC::ImportData()
{
	AfxMessageBox(_T("TEST"));

	// table ��������
	wchar_t query_str[256] = L"SELECT * FROM todolist";
	// �о�� �������� ���¸� ����� ����
	unsigned short record_state[MAX_COUNT];
	// �о�� �����͸� ������ ����
	TodoList raw_data[MAX_COUNT];
	CString str;
	// ���� �������� ������ ������ ����
	unsigned long int record_num = 0;

	//�����͸� ������ �迭�� �ʱ�ȭ �Ѵ�.
	memset(raw_data, 0, sizeof(raw_data));

	HSTMT h_statement;
	RETCODE ret_code;
	// Query ���� ���� �޸𸮸� �Ҵ��Ѵ�.
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
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
		
		// ���̺��� ������ �����͸� �Ӽ����� raw_data ������ �����ϱ� ���ؼ�
		// �Ӽ����� ������ �޸� ��ġ�� �����Ѵ�.
		SQLBindCol(h_statement, 1, SQL_WCHAR, raw_data[0].date, sizeof(wchar_t) * 20, NULL);
		SQLBindCol(h_statement, 2, SQL_SMALLINT, &raw_data[0].isDone, sizeof(int), NULL);
		SQLBindCol(h_statement, 3, SQL_WCHAR, &raw_data[0].todolist, sizeof(wchar_t) * 20, NULL);
		// SQL ��ɹ��� �����Ѵ�.
		ret_code = SQLExecDirect(h_statement, (SQLWCHAR*)query_str, SQL_NTS);
		// SQL ��ɹ��� ���� ����� ���� �����͸� ListBox�� �߰��Ѵ�.
		while (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			// ������ ������ŭ �ݺ��ϸ鼭 �۾��Ѵ�.
			for (unsigned long int i = 0; i < record_num; i++) {
				CString tem;
				tem.Format(_T("for : record_num : %d"), record_num);
				AfxMessageBox(tem);

				// ������ �����Ͱ� ������ ������ �ƴ϶�� �ش� �Ӽ�����
				// ���ļ� ���ڿ��� �����ϰ� AfxMessageBox�� ����Ѵ�.
				if (record_state[i] != SQL_ROW_DELETED && record_state[i] != SQL_ROW_ERROR) {
					str.Format(L"%s, %d, %s", raw_data[i].date, raw_data[i].isDone, raw_data[i].todolist);
					AfxMessageBox(str);
					//m_data_list.InsertString(-1, str);
				}
			}
		}
		// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}
