// GuildHouseWar.h: interface for the CGuildHouseWar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDHOUSEWAR_H__410F61B7_AB0D_4522_8787_6DB5B8C76BBE__INCLUDED_)
#define AFX_GUILDHOUSEWAR_H__410F61B7_AB0D_4522_8787_6DB5B8C76BBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_GUILD_HOUSE_NUM	7
#define MAX_GUILD_APPLY_NUM 1

class COM;
class USER;

struct UserList
{
	int		uid;
	TCHAR	strUserName[CHAR_NAME_LENGTH + 1];	
};

struct	ApplyGuildHouseWar
{
	long	lUsed;				// ����ȭ�� ���߱�����...	
	long	lGuild;				// ����ȣ	
	int		iCurValue;			// ���� ����ġ
	DWORD	dwTimer;			// ���۽� ����
	DWORD	dwIntervalTick;		// 5�и��� üũ

	int		iGuildLevel;		// ���� �� �ջ� ����	

	int		iGuildNum;			// �� ���� �ο�
	UserList	arUserList[MAX_GUILD_HOUSE_USER];	// �������� ���� �ε���

	CWordArray	arNpcList;		// NPC ����Ʈ	(�ʱ⿡ ����)
	int			iWarZone;		// �ʱ⿡ ���� ����(���� Ʋ���� �ִ�)
};

struct TopGuildList
{
	long	lGuild;
	int		iLastValue;

	int		iGuildNum;
	int		iGuildLevel;
	DWORD	dwLastTime;
	TCHAR	strGuildName[CHAR_NAME_LENGTH + 1];
};

//typedef CTypedPtrArray <CPtrArray, ApplyGuildHouseWar*>	GuildHouseWarList;

class CGuildHouseWar  
{
public:
	int		m_iCity;	// �ش� ���ø� ����		
public:	
	void ResetGuildHouseRank();
	void SendTownPotalMsg(COM *pCom);
	//void SetUserListToWarEnd(USER** user_array);	
	void SetUserListToWarEnd();	

	void SetUserListToWarEnd(COM *pCom);
	void SetNpcListToWarEnd();
	void SwapTopGuildList(TopGuildList &s, TopGuildList &d);
	BOOL CheckGuildMopEnd();
	void CheckGuildUserEnd();
	void CheckGuildHouseRank();
	BOOL CheckGuildHouseWarEnd(USER** user_array);
	void InitGuild();
						// �ϳ��� ���ÿ� �ϳ��� ����� �� ����		
	ApplyGuildHouseWar	m_CurrentGuild;//[MAX_GUILD_APPLY_NUM];
	TopGuildList		m_TopList[MAX_GUILD_HOUSE_NUM];	// ���� ��帮��Ʈ	

	CGuildHouseWar();
	virtual ~CGuildHouseWar();

};

#endif // !defined(AFX_GUILDHOUSEWAR_H__410F61B7_AB0D_4522_8787_6DB5B8C76BBE__INCLUDED_)
