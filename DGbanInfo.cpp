// DGbanInfo.cpp: implementation of the DGbanInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "DGbanInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DGbanInfo::DGbanInfo()
{
	::ZeroMemory(m_strUserID, sizeof(m_strUserID));
	::ZeroMemory(m_DGBAN_BODY, sizeof(m_DGBAN_BODY));
}

DGbanInfo::~DGbanInfo()
{

}
