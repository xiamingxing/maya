// Remodeling02TableSet.cpp : implementation file
//

#include "stdafx.h"
#include "server.h"
#include "Remodeling02TableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemodeling02TableSet

IMPLEMENT_DYNAMIC(CRemodeling02TableSet, CRecordset)

CRemodeling02TableSet::CRemodeling02TableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRemodeling02TableSet)
	m_sPlan = 0;
	m_sRid = 0;
	m_sSid = 0;
	m_strText = _T("");
	m_tNeedClass = 0;
	m_tRandom1 = 0;
	m_tRandom2 = 0;
	m_tReplace = 0;
	m_tWearInfo = 0;
	m_nFields = 9;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CRemodeling02TableSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=drgame;UID=drgame;PWD=drgame");
}

CString CRemodeling02TableSet::GetDefaultSQL()
{
	return _T("[dbo].[REMODELING_02]");
}

void CRemodeling02TableSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRemodeling02TableSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[sPlan]"), m_sPlan);
	RFX_Int(pFX, _T("[sRid]"), m_sRid);
	RFX_Int(pFX, _T("[sSid]"), m_sSid);
	RFX_Text(pFX, _T("[strText]"), m_strText);
	RFX_Byte(pFX, _T("[tNeedClass]"), m_tNeedClass);
	RFX_Byte(pFX, _T("[tRandom1]"), m_tRandom1);
	RFX_Byte(pFX, _T("[tRandom2]"), m_tRandom2);
	RFX_Byte(pFX, _T("[tReplace]"), m_tReplace);
	RFX_Byte(pFX, _T("[tWearInfo]"), m_tWearInfo);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRemodeling02TableSet diagnostics

#ifdef _DEBUG
void CRemodeling02TableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRemodeling02TableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
