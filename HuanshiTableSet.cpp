
#include "stdafx.h"
#include "server.h"
#include "HuanshiTableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CHuanshiTableSet, CRecordset)

CHuanshiTableSet::CHuanshiTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CHuanshiTableSet)
	m_sChangeValue = 0;
	m_sRandom = 0;
	m_sSubType = 0;
	m_strText = _T("");
	m_tLevel = 0;
	m_tNeedClass = 0;
	m_tSid = 0;
	m_tUpgrade = 0;
	m_tWearInfo = 0;
	m_nFields = 9;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CHuanshiTableSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=drgame;UID=drgame;PWD=drgame");
}

CString CHuanshiTableSet::GetDefaultSQL()
{
	return _T("[dbo].[HUAN_STNO]");
}

void CHuanshiTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CEBodyTableSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[sChangeValue]"), m_sChangeValue);
	RFX_Int(pFX, _T("[sRandom]"), m_sRandom);
	RFX_Int(pFX, _T("[sSubType]"), m_sSubType);
	RFX_Text(pFX, _T("[strText]"), m_strText);
	RFX_Byte(pFX, _T("[tLevel]"), m_tLevel);
	RFX_Byte(pFX, _T("[tNeedClass]"), m_tNeedClass);
	RFX_Byte(pFX, _T("[tSid]"), m_tSid);
	RFX_Byte(pFX, _T("[tUpgrade]"), m_tUpgrade);
	RFX_Byte(pFX, _T("[tWearInfo]"), m_tWearInfo);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CHuanshiTableSet diagnostics

#ifdef _DEBUG
void CHuanshiTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CHuanshiTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
