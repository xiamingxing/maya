// EventItemTableSet.cpp : implementation file
//

#include "stdafx.h"
#include "server.h"
#include "EventItemTableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventItemTableSet

IMPLEMENT_DYNAMIC(CEventItemTableSet, CRecordset)

CEventItemTableSet::CEventItemTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CEventItemTableSet)
	m_sSid = 0;
	m_strSerialNum = _T("");
	m_tEnd = 0;
	m_tGiveFlag = 0;
	m_tType = 0;
	m_tUsed = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CEventItemTableSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=drgamenew;UID=drgame;PWD=drgame");
}

CString CEventItemTableSet::GetDefaultSQL()
{
	return _T("[dbo].[EVENT_ITEM]");
}

void CEventItemTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CEventItemTableSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[sSid]"), m_sSid);
	RFX_Text(pFX, _T("[strSerialNum]"), m_strSerialNum);
	RFX_Byte(pFX, _T("[tEnd]"), m_tEnd);
	RFX_Byte(pFX, _T("[tGiveFlag]"), m_tGiveFlag);
	RFX_Byte(pFX, _T("[tType]"), m_tType);
	RFX_Byte(pFX, _T("[tUsed]"), m_tUsed);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CEventItemTableSet diagnostics

#ifdef _DEBUG
void CEventItemTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CEventItemTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
