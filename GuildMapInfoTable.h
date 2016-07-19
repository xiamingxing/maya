// GuildMapInfoTable.h: interface for the CGuildMapInfoTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMAPINFOTABLE_H__497347F6_4FFA_4CA4_8E49_7447C9977C10__INCLUDED_)
#define AFX_GUILDMAPINFOTABLE_H__497347F6_4FFA_4CA4_8E49_7447C9977C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuildMapInfoTable  
{
public:
	CGuildMapInfoTable();
	virtual ~CGuildMapInfoTable();

public:
	short	m_sMapIndex;
	BYTE	m_tPkMode;

	short	m_sStoreID;			// ���� ��ȣ
	short   m_sStoreIndex;		// ���� �迭 ���� �ε���
	short	m_sStoreZone;		// ���� �� �����̴�.
};

#endif // !defined(AFX_GUILDMAPINFOTABLE_H__497347F6_4FFA_4CA4_8E49_7447C9977C10__INCLUDED_)
