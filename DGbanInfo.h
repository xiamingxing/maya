// DGbanInfo.h: interface for the DGbanInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGBANINFO_H__9011C27F_CB7A_4E3F_AB8B_389F8AFAC67E__INCLUDED_)
#define AFX_DGBANINFO_H__9011C27F_CB7A_4E3F_AB8B_389F8AFAC67E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DGbanInfo  
{
public:
	DGbanInfo();
	virtual ~DGbanInfo();
	TCHAR		m_strUserID[CHAR_NAME_LENGTH + 1];
	TCHAR		m_DGBAN_BODY[50];

};

#endif // !defined(AFX_DGBANINFO_H__9011C27F_CB7A_4E3F_AB8B_389F8AFAC67E__INCLUDED_)
