
#include "stdafx.h"
#include "TW_ODBC.h"

#include "MainFrm.h"
#include "WeeklyPlannerView.h"
#include "HistoryView.h"

#pragma comment(lib, "odbc32.lib")


// �����͸� �������� �ִ� ������ �����Ѵ�.
#define MAX_COUNT 8
#define MAX_SOUND 50

// ����Ž���⸦ �̿��Ͽ� ODBC �����ϱ�
// ��Ű�� �̸�, ���̵�, �н�����, ���������, DBĮ���� (��¥, ����) ���
//#define DBSchemas L"WeeklyPlanner"
//#define DBID L"haey"
//#define DBPassword L""
//#define DBSchemas L"WeeklyPlanner"
//#define DBID L"root"
//#define DBPassword L"1234"
#define DBContents Memo
#define DBDdaytitle Title
#define DBSchemas L"wp_db"
#define DBID L"WP_User"
#define DBPassword L"tndtlfeo"
//#define DBContents Todolist
//#define DBDdaytitle Title

struct TodoList {
	wchar_t date[25];
	bool isDone;
	wchar_t DBContents[100];
};

struct DdayData {
	wchar_t date[11];
	wchar_t Title[100];
};

struct profile {
	wchar_t path[256];
	wchar_t message[100];
};

struct sound {
	wchar_t soundPath[256];
	wchar_t soundName[100];
};

TW_ODBC::TW_ODBC()
	: record_num(0)
	, m_dbDataCnt(0)
	, record_sound(0)
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


int TW_ODBC::ImportData(CString strToday, CString strTomorrow)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	int checkEnabled = 0;
	CString tmp;
	tmp.Format(L"SELECT * FROM todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	wchar_t query_str_forDday[256] = L"SELECT * FROM dday";
	wchar_t query_str_forProfile[256] = L"SELECT * FROM profile";
	wchar_t query_str_forSound[256] = L"SELECT * FROM sound";

	// table ��������
	wchar_t* query_str = tmp.GetBuffer();

	// �о�� �������� ���¸� ����� ����
	unsigned short record_state[MAX_COUNT];
	unsigned short record_state_forDday;
	unsigned short record_state_forProfile[MAX_COUNT];
	unsigned short record_state_forSound[MAX_SOUND];


	// �о�� �����͸� ������ ����
	TodoList raw_data[MAX_COUNT];
	DdayData raw_data_forDday;
	profile raw_data_forProfile[100];
	sound raw_data_forSound[MAX_SOUND];


	CString str;
	CString strProfile;
	CString strSound;

	//�����͸� ������ �迭�� �ʱ�ȭ �Ѵ�.
	memset(raw_data, 0, sizeof(raw_data));
	memset(&raw_data_forDday, 0, sizeof(raw_data_forDday));
	memset(raw_data_forProfile, 0, sizeof(raw_data_forProfile));
	memset(raw_data_forSound, 0, sizeof(raw_data_forSound));

	HSTMT h_statement, h_statement_forDday, h_statement_forProfile, h_statement_forSound;
	RETCODE ret_code, ret_code_forDday, ret_code_forProfile, ret_code_forSound;
	// Query ���� ���� �޸𸮸� �Ҵ��Ѵ�.
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement) && SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement_forDday) && SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement_forProfile) && SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement_forSound)) {
		record_num = 0;
		record_sound = 0;
		// Query ���� ������ �� Ÿ�� �ƿ��� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);


		// ������ �����͸� ������ �޸��� ũ�⸦ �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data), 0);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data_forDday), 0);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data_forProfile), 0);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data_forSound), 0);


		// �����͸� ������ �� ���ü��� ���� ����� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);

		// �����͸� �������� �ִ� ������ �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_COUNT, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_COUNT, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_COUNT, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_SOUND, SQL_IS_UINTEGER);

		// ���� �������� ���¸� ������ ������ �ּҸ� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_STATUS_PTR, record_state, 0);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_ROW_STATUS_PTR, &record_state_forDday, 0);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_ROW_STATUS_PTR, record_state_forProfile, 0);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_ROW_STATUS_PTR, record_state_forSound, 0);

		// ���� �������� ������ ������ ������ �ּҸ� �����Ѵ�.
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_ROWS_FETCHED_PTR, &record_sound, 0);

		// ���̺��� ������ �����͸� �Ӽ����� raw_data ������ �����ϱ� ���ؼ�
		// �Ӽ����� ������ �޸� ��ġ�� �����Ѵ�.

		SQLBindCol(h_statement, 1, SQL_WCHAR, raw_data[0].date, sizeof(wchar_t) * 20, NULL);
		SQLBindCol(h_statement, 2, SQL_SMALLINT, &raw_data[0].isDone, sizeof(int), NULL);
		SQLBindCol(h_statement, 3, SQL_WCHAR, raw_data[0].DBContents, sizeof(wchar_t) * 100, NULL);

		SQLBindCol(h_statement_forDday, 1, SQL_WCHAR, raw_data_forDday.date, sizeof(wchar_t) * 11, NULL);
		SQLBindCol(h_statement_forDday, 2, SQL_WCHAR, raw_data_forDday.Title, sizeof(wchar_t) * 20, NULL);
		
		SQLBindCol(h_statement_forProfile, 1, SQL_WCHAR, raw_data_forProfile[0].path, sizeof(wchar_t) * 80, NULL);
		SQLBindCol(h_statement_forProfile, 2, SQL_WCHAR, raw_data_forProfile[0].message, sizeof(wchar_t) * 20, NULL);

		SQLBindCol(h_statement_forSound, 1, SQL_WCHAR, raw_data_forSound[0].soundPath, sizeof(wchar_t) * 80, NULL);
		SQLBindCol(h_statement_forSound, 2, SQL_WCHAR, raw_data_forSound[0].soundName, sizeof(wchar_t) * 80, NULL);

		// SQL ��ɹ��� �����Ѵ�.
		ret_code = SQLExecDirect(h_statement, (SQLWCHAR*)query_str, SQL_NTS);
		ret_code_forDday = SQLExecDirect(h_statement_forDday, (SQLWCHAR*)query_str_forDday, SQL_NTS);
		ret_code_forProfile = SQLExecDirect(h_statement_forProfile, (SQLWCHAR*)query_str_forProfile, SQL_NTS);
		ret_code_forSound = SQLExecDirect(h_statement_forSound, (SQLWCHAR*)query_str_forSound, SQL_NTS);

		if (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			// SQL ��ɹ��� ���� ����� ���� �����͸� ListBox�� �߰��Ѵ�.
			ret_code = SQLFetchScroll(h_statement, SQL_FETCH_PREV, 0);
			while (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {

				// ������ ������ŭ �ݺ��ϸ鼭 �۾��Ѵ�.
				// ������ �����Ͱ� ������ ������ �ƴ϶�� �ش� �Ӽ�����
				// ���ļ� ���ڿ��� �����ϰ� AfxMessageBox�� ����Ѵ�.
				if (record_state[0] != SQL_ROW_DELETED && record_state[0] != SQL_ROW_ERROR) {
					str.Format(L"%s, %d, %s", raw_data[0].date, raw_data[0].isDone, raw_data[0].DBContents);
					//������ �ҷ��� �� üũ����Ʈ�� �߰��ϱ� (���?)
					while (1) {
						if (pView->m_arrayTodoCheck[m_dbDataCnt]->IsWindowEnabled() == false) {
							break;
						}
						m_dbDataCnt++;
					}
					CString strContents;
					strContents.Format(_T("%s"), raw_data[0].DBContents);
					pView->m_arrayTodoCheck[m_dbDataCnt]->SetWindowText(strContents);

					if (raw_data[0].isDone == 1) {
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoCheck[m_dbDataCnt]->SetCheck(1);
						pView->m_bChecked[m_dbDataCnt] = false;
						pView->m_checkCnt++;
						checkEnabled++;
					}
					else if (raw_data[0].isDone == 0) {
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoCheck[m_dbDataCnt]->SetCheck(0);
						pView->m_bChecked[m_dbDataCnt] = false;
						checkEnabled++;
					}
					else {
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(false);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(false);
						pView->m_bChecked[m_dbDataCnt] = true;
					}
				}
			}
		}
		else {
			DataSaveTodolist();
		}

		while (ret_code_forDday = SQLFetchScroll(h_statement_forDday, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {

			CListCtrl* ctrlDdayList = &(pView->m_ctrlDdayList);

			for (unsigned int i = 0; i < record_num; i++) {
				if (record_state_forDday != SQL_ROW_DELETED && record_state_forDday != SQL_ROW_ERROR) {
					CTime nowTime, Ddate;
					CString str, strDday;
					CString stry, strm, strd;
					int year, month, day, index;

					AfxExtractSubString(stry, raw_data_forDday.date, 0, '-');
					AfxExtractSubString(strm, raw_data_forDday.date, 1, '-');
					AfxExtractSubString(strd, raw_data_forDday.date, 2, '-');
					year = _ttoi(stry);
					month = _ttoi(strm);
					day = _ttoi(strd);
					Ddate = CTime(year, month, day, 0, 0, 0, 0);
					str.Format(_T("%s-%s-%s"), stry, strm, strd);


					nowTime = CTime::GetCurrentTime();
					CTimeSpan dDay = Ddate - nowTime;

					//*******************
					int m_dDay = dDay.GetTotalSeconds();
					if (dDay > 0)
						strDday.Format(_T("D-%d"), abs(m_dDay / (24 * 60 * 60) + 1));
					else if (abs(m_dDay / (24 * 60 * 60)) == 0)
						strDday.Format(_T("D-Day"));
					else
						strDday.Format(_T("D+%d"), abs(m_dDay / (24 * 60 * 60)));
					pView->m_strNewDdayDate = strDday;



					index = ctrlDdayList->InsertItem(ctrlDdayList->GetItemCount(), str);
					ctrlDdayList->SetItem(index, 1, LVIF_TEXT, raw_data_forDday.Title, 0, 0, 0, NULL);
					ctrlDdayList->SetItem(index, 2, LVIF_TEXT, strDday, 0, 0, 0, NULL);


				}

			}
		}
		if (ret_code_forProfile = SQLFetchScroll(h_statement_forProfile, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			// SQL ��ɹ��� ���� ����� ���� �����͸� ListBox�� �߰��Ѵ�.
			ret_code_forProfile = SQLFetchScroll(h_statement_forProfile, SQL_FETCH_PREV, 0);
			while (ret_code_forProfile = SQLFetchScroll(h_statement_forProfile, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {

				// ������ ������ŭ �ݺ��ϸ鼭 �۾��Ѵ�.
				// ������ �����Ͱ� ������ ������ �ƴ϶�� �ش� �Ӽ�����
				// ���ļ� ���ڿ��� �����ϰ� AfxMessageBox�� ����Ѵ�.
				if (record_state_forProfile[0] != SQL_ROW_DELETED && record_state_forProfile[0] != SQL_ROW_ERROR) {
					strProfile.Format(L"%s, %s", raw_data_forProfile[0].path, raw_data_forProfile[0].message);

					//strProfile.Format(L"DELETE FROM weeklyplanner.profile WHERE path = '%s'", raw_data_forProfile[0].path);
					int i = 0;
					

						CString strMessage, strPath;
						strMessage.Format(_T("%s"), raw_data_forProfile[i].message);
						pView->m_EditMessage.SetWindowTextW(strMessage);

						strPath.Format(_T("%s"), raw_data_forProfile[i].path);
						//strPath.Replace(_T("\\"), _T("\\\\"));

						//AfxMessageBox(strPath);
						pView->m_strDefaultImagePath = strPath;
					
				}
			}
		}
		else {
			
			CString strPath = pView->m_strDefaultImagePath;
			CString strOld = pView->m_strOldPath;

			SaveProfilePath(strPath, strOld);
			
		}
		if (ret_code_forSound = SQLFetchScroll(h_statement_forSound, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			
			CString strPath, strName;
			ret_code_forSound = SQLFetchScroll(h_statement_forSound, SQL_FETCH_PREV, 0);

			int i = 0;
			//for (unsigned int i = 0; i < record_sound; i++) {
			while (ret_code_forSound = SQLFetchScroll(h_statement_forSound, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
				
				// ������ ������ŭ �ݺ��ϸ鼭 �۾��Ѵ�.
				// ������ �����Ͱ� ������ ������ �ƴ϶�� �ش� �Ӽ�����
				// ���ļ� ���ڿ��� �����ϰ� AfxMessageBox�� ����Ѵ�.
				if (record_state_forSound[0] != SQL_ROW_DELETED && record_state_forSound[0] != SQL_ROW_ERROR) {
					strSound.Format(L"%s, %s", raw_data_forSound[0].soundPath, raw_data_forSound[0].soundName);
					
					//CString* str;

					strName.Format(_T("%s"), raw_data_forSound[0].soundName);

					pView->m_soundSP.m_strSoundName[i] = strName;
					strPath.Format(_T("%s"), raw_data_forSound[0].soundPath);
					pView->m_soundSP.m_strSoundPath[i] = strPath;
					i++;

				}

			}
			
			pView->m_soundSP.m_nSoundIndex = i;
		}

		else {

			for (int i = 0; i < 9; i++) {
				pView->m_soundSP.m_strSoundName[i] = pView->SoundName[i];
				pView->m_soundSP.m_strSoundPath[i] = pView->SoundPath[i];
				//ttmp.Format(_T("%s"), strName[i]);
				//AfxMessageBox(ttmp);
			}
			pView->m_soundSP.m_nSoundIndex = 9;
			DataSaveSound(pView->SoundPath, pView->SoundName);
		}

		// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forDday);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forProfile);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forSound);

	}

	return checkEnabled;
}


void TW_ODBC::DataSaveTodolist()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	SQLHSTMT h_statement;
	CString str, strContents;
	int result = 0;
	char isDone = NULL;
	for (int i = 0; i < 8; i++) {
		pView->m_arrayTodoCheck[i]->GetWindowText(strContents);
		boolean isEnabled = pView->m_arrayTodoCheck[i]->IsWindowEnabled();
		if (isEnabled) {
			if (pView->m_arrayTodoCheck[i]->GetCheck() == BST_UNCHECKED)
				isDone = 0;
			else if (pView->m_arrayTodoCheck[i]->GetCheck() == BST_CHECKED)
				isDone = 1;
		}
		else {
			isDone = 2;
		}
		
		str.Format(L"INSERT INTO todolist VALUES (default,'%d', '%s')", isDone, strContents);
		const wchar_t* tmp = str;
		if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
			// Query ���� ������ �� Ÿ�� �ƿ��� �����Ѵ�.
			SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

			// SQL ��ɹ��� �����Ѵ�.
			RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);
			
			// ���������� �Ϸ�Ǿ����� üũ�Ѵ�.
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) result = 1;

			// ��� ������ �Ϸ�Ǿ��ٴ� ���� �����Ѵ�.
			SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
			// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
			SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
		}
		isDone = NULL;
	}
	
}


void TW_ODBC::DataRemoveTodolist(CString strToday, CString strTomorrow)
{
	SQLHSTMT h_statement;
	CString str;
	int result = 0;

	str.Format(L"DELETE FROM todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	const wchar_t* tmp = str;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query ���� ������ �� Ÿ�� �ƿ��� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
	
		// SQL ��ɹ��� �����Ѵ�.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);

		// ���������� �Ϸ�Ǿ����� üũ�Ѵ�.
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			result = 1;
		// ��� ������ �Ϸ�Ǿ��ٴ� ���� �����Ѵ�.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}


SQLHANDLE TW_ODBC::GetMh_Env()
{
	return mh_environment;
}


SQLHDBC TW_ODBC::GetMh_odbc()
{
	return mh_odbc;
}


void TW_ODBC::LoadHistoryTodolist(CString strToday, CString strTomorrow)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();
	CString strPercent;
	for (int i = 0; i < 8; i++)
		(pView->view).m_arrayHistoryCheck[i]->SetWindowText(_T(""));
	(pView->view).m_progressHistory.SetPos(0);
	strPercent.Format(_T("0"));
	(pView->view.m_historyPercent).SetWindowText(strPercent);

	int checkEnable = 0, writeList = 0, arrayCnt = 0;
	CString tmp, str;
	tmp.Format(L"SELECT * FROM todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	
	// table ��������
	wchar_t* query_str = tmp.GetBuffer();

	// �о�� �������� ���¸� ����� ����
	unsigned short record_state[MAX_COUNT];

	// �о�� �����͸� ������ ����
	TodoList raw_data[MAX_COUNT];

	//�����͸� ������ �迭�� �ʱ�ȭ �Ѵ�.
	memset(raw_data, 0, sizeof(raw_data));

	HSTMT h_statement;	RETCODE ret_code;

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


		// ���̺��� ������ �����͸� �Ӽ����� raw_data ������ �����ϱ� ���ؼ�
		// �Ӽ����� ������ �޸� ��ġ�� �����Ѵ�.

		SQLBindCol(h_statement, 1, SQL_WCHAR, raw_data[0].date, sizeof(wchar_t) * 20, NULL);
		SQLBindCol(h_statement, 2, SQL_SMALLINT, &raw_data[0].isDone, sizeof(int), NULL);
		SQLBindCol(h_statement, 3, SQL_WCHAR, &raw_data[0].DBContents, sizeof(wchar_t) * 100, NULL);

		// SQL ��ɹ��� �����Ѵ�.
		ret_code = SQLExecDirect(h_statement, (SQLWCHAR*)query_str, SQL_NTS);

		if (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			// SQL ��ɹ��� ���� ����� ���� �����͸� ListBox�� �߰��Ѵ�.
			ret_code = SQLFetchScroll(h_statement, SQL_FETCH_PREV, 0);
			while (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {

				if (record_state[0] != SQL_ROW_DELETED && record_state[0] != SQL_ROW_ERROR) {
					str.Format(L"%s, %d, %s", raw_data[0].date, raw_data[0].isDone, raw_data[0].DBContents);
					//������ �ҷ��� �� üũ����Ʈ�� �߰��ϱ� (���?)
					
					CString strContents;

					if (raw_data[0].isDone == 1) {
						strContents.Format(_T("< Complete >  %s"), raw_data[0].DBContents);
						(pView->view).m_arrayHistoryCheck[arrayCnt]->SetWindowText(strContents);
						checkEnable++;
						writeList++;
					}
					else if (raw_data[0].isDone == 0) {
						strContents.Format(_T("< Proceeding >  %s"), raw_data[0].DBContents);
						(pView->view).m_arrayHistoryCheck[arrayCnt]->SetWindowText(strContents);
						writeList++;
					}
					else {
						strContents.Format(_T("< No Events >"));
						(pView->view).m_arrayHistoryCheck[arrayCnt]->SetWindowText(strContents);
					}
				}
				arrayCnt++;
			}
			if (writeList == 0 || checkEnable == 0) {
				(pView->view).m_progressHistory.SetPos(0);
				strPercent.Format(_T("0"));
				(pView->view.m_historyPercent).SetWindowText(strPercent);
			}
			else {
				if (writeList == checkEnable) {
					(pView->view).m_progressHistory.SetPos(1000);
					strPercent.Format(_T("100"));
					(pView->view.m_historyPercent).SetWindowText(strPercent);
				}
				else {
					int percent = 1000 / writeList;
					percent = 1000 / writeList;
					(pView->view).m_progressHistory.SetPos(checkEnable*percent);
					percent = 100 / writeList;
					strPercent.Format(_T("%d"), percent);
					(pView->view.m_historyPercent).SetWindowText(strPercent);
				}
			}

		}
		else {
			CString strContents;
			strContents.Format(_T("< There is no record of this day. >"), raw_data[0].DBContents);
			(pView->view).m_arrayHistoryCheck[0]->SetWindowText(strContents);
		}
	}
}


void TW_ODBC::SaveProfilePath(CString strPath, CString strOldPath)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	SQLHSTMT h_statement;
	CString str, strMessage;
	int result = 0;

	strMessage = pView->m_strProfileMessage;
	strPath = pView->m_strDefaultImagePath;
	strOldPath = pView->m_strOldPath;

	strPath.Replace(_T("\\"), _T("\\\\"));

	str.Format(L"INSERT INTO profile VALUES ('%s', '%s')", strPath, strMessage);
	
	//AfxMessageBox(strPath);
	//AfxMessageBox(str);

	const wchar_t* tmp = str;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query ���� ������ �� Ÿ�� �ƿ��� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);


		// SQL ��ɹ��� �����Ѵ�.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);
		// ���������� �Ϸ�Ǿ����� üũ�Ѵ�.
		

		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			result = 1;
			//AfxMessageBox(_T("��������"));
		}

		// ��� ������ �Ϸ�Ǿ��ٴ� ���� �����Ѵ�.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}


void TW_ODBC::DeleteProfilePath(CString strPath, CString strOld)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	SQLHSTMT h_statement;
	CString str, strMessage, str2;
	int result = 0;

	//strPath = pView->m_strDefaultImagePath;
	strMessage = pView->m_strProfileMessage;
	//strOld = pView->m_strOldPath;

	//strPath.Replace(_T("\\"), _T("\\\\"));

	//str2.Format(L"SET SQL_SAFE_UPDATES = 0");
	str.Format(L"DELETE FROM profile");
	//AfxMessageBox(_T("��������"));

	const wchar_t* tmp = str;
	const wchar_t* tmp2 = str2;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query ���� ������ �� Ÿ�� �ƿ��� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// SQL ��ɹ��� �����Ѵ�.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);
		RETCODE ret2 = SQLExecDirect(h_statement, (SQLWCHAR *)tmp2, SQL_NTS);

		// ���������� �Ϸ�Ǿ����� üũ�Ѵ�.
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			result = 1;
			//AfxMessageBox(_T("����"));
		}
		if (ret2 == SQL_SUCCESS || ret2 == SQL_SUCCESS_WITH_INFO) {
			//AfxMessageBox(_T("���m"));
		}

		// ��� ������ �Ϸ�Ǿ��ٴ� ���� �����Ѵ�.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
	
}



void TW_ODBC::DeleteSound()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();
	CSoundPlayer pSound;

	SQLHSTMT h_statement;

	CString str, str2;
	int result = 0;

	//str2.Format(L"SET SQL_SAFE_UPDATES = 0");
	str.Format(L"DELETE FROM sound");


	const wchar_t* tmp = str;
	const wchar_t* tmp2 = str2;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query ���� ������ �� Ÿ�� �ƿ��� �����Ѵ�.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// SQL ��ɹ��� �����Ѵ�.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);
		RETCODE ret2 = SQLExecDirect(h_statement, (SQLWCHAR *)tmp2, SQL_NTS);

		// ���������� �Ϸ�Ǿ����� üũ�Ѵ�.
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			result = 1;

		}
		if (ret2 == SQL_SUCCESS || ret2 == SQL_SUCCESS_WITH_INFO) {

		}

		// ��� ������ �Ϸ�Ǿ��ٴ� ���� �����Ѵ�.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}


void TW_ODBC::DataSaveSound(CString SoundPath[50], CString SoundName[50])
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	SQLHSTMT h_statement;
	CString str, strPath, strName;
	int result = 0;
	int i = 0;

	//	pView->m_soundSP.m_strSoundPath = SoundPath;

	while (SoundPath[i] != "") {
		strPath = SoundPath[i];
		strName = SoundName[i];
		strPath.Replace(_T("\\"), _T("\\\\"));
		str.Format(L"INSERT INTO sound VALUES ('%s', '%s')", strPath, strName);

		const wchar_t* tmp = str;
		if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
			// Query ���� ������ �� Ÿ�� �ƿ��� �����Ѵ�.
			SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

			// SQL ��ɹ��� �����Ѵ�.
			RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);

			// ���������� �Ϸ�Ǿ����� üũ�Ѵ�.
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				result = 1;
			}

			// ��� ������ �Ϸ�Ǿ��ٴ� ���� �����Ѵ�.
			SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		}
		i++;

		// Query ���� ���� �Ҵ��� �޸𸮸� �����Ѵ�.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}
