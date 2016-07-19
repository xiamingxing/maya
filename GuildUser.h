// GuildUser.h: interface for the CGuildUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDUSER_H__1623FC3A_0913_46E0_B891_57E63EAD8845__INCLUDED_)
#define AFX_GUILDUSER_H__1623FC3A_0913_46E0_B891_57E63EAD8845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuildUser  
{
public:
	char	m_strUserId[CHAR_NAME_LENGTH + 1];
	long	m_lSid;						// ����ε���
	long	m_lUsed;					// ���⿡ ������ ���õǾ��ִ�.

	CGuildUser();
	virtual ~CGuildUser();
};

#endif // !defined(AFX_GUILDUSER_H__1623FC3A_0913_46E0_B891_57E63EAD8845__INCLUDED_)
