// GuildFortress.h: interface for the CGuildFortress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDFORTRESS_H__42E14E0C_5B21_430C_9A03_60CC86DD6BB3__INCLUDED_)
#define AFX_GUILDFORTRESS_H__42E14E0C_5B21_430C_9A03_60CC86DD6BB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNpc;
class COM;
class USER;

struct ApplyGuildList
{
	long	lUsed;
	long	lGuild;	
	TCHAR	strGuildName[CHAR_NAME_LENGTH + 1];
};

struct TargetNpcList
{
	long	lUsed;								// �ʱ� ���ý� NPC Thread���� ����ȭ�� ����
	short	sTargertID;
};

struct RepairNpcList
{
	short	sUid;								// sEZone�� ���õ� �����Ѱ�
	short	sNid;								// 
	short	sHP;								// ���� HP�� ����
};

struct MsgList
{
	int		iUsed;
	int		iMsgTime;
};

struct FortressCityList
{
	long	lUsed;
	long	lCityRank;
	TCHAR	strUserID[CHAR_NAME_LENGTH + 1];
};

class CGuildFortress  
{
public:
//	int		m_iCityDegree;
	int		m_sFortressID;	
	long	m_iGuildSid;
	DWORD	m_dwRepairCost;

	TCHAR	m_strGuildName[CHAR_NAME_LENGTH + 1];
	TCHAR	m_strMasterName[CHAR_NAME_LENGTH + 1];

	BYTE		m_tTaxRate;						  // ��Ʈ������ ������ �����鿡 ������ �����Ѵ�.
	CWordArray	arStoreTax;						  // ���� ����Ʈ	

	DWORD	m_iGuildDN;

	BYTE	m_tWarType;							  // 0: ����� ��¥ ���� 12�ð��� 1: �̹� ����� ��¥�� ����	
	BYTE	m_tGuildWar;						  // 0 : ����� �غ�ܰ�
											      // 1 : �������
												  // 2 : �Ϲ� �ܰ�
	SYSTEMTIME m_wLastWarTime;
	SYSTEMTIME m_wPlanWarTime;

	DWORD	m_dwStartTick;
	int		m_iStandingTime;
	int		m_iEndingTime;

	BOOL	m_bHaveGuild;						  // ���� ��忡 ���� Fortress = TRUE	
	long	m_lUsed;							  // ����� ���������� �ƴ��� ǥ��...
	long	m_lChangeUsed;						  // ������ �ٲ�� �������� ��� �����.	

	long	m_lListUsed;						  // ������ ���ý� ����ȭ�� ����
	long	m_lListCount;						  // ����Ʈ �ε���		
														
	ApplyGuildList	m_arAttackGuild[GUILDFORTRESS_ATTACK_MAX_NUM];// ������� �����ϴ� ��� ����Ʈ	
	TargetNpcList	m_arFortressTarget[FORTRESS_TARGET_MAX_NUM]; // 4���� N_Circle�����ΰ� �ִ�.
	RepairNpcList	m_arRepairDBList[GUILD_REPAIR_MAX_NUM]; // �������� ������ �ʿ䰡 �ִ� NPC�� ����
	int				m_iGuardList[GUILD_GUARD_MAX_NUM];

	CWordArray		m_arRepairNpcList;	
	CWordArray		m_arNpcList;
	CWordArray		m_arWarNpcList;
	CWordArray		m_arViolenceNpcList;

public:
	long			 m_lViolenceUsed;	
	FortressCityList m_arCityRankList[MAX_GUILD_USER];
	SYSTEMTIME		 m_wMopPartyTime;
	DWORD			 m_dwViolenceTick;

public:	
	MsgList	m_MsgTimer[GUILD_WAR_MAX_MSG_TIMER];
	int		m_iZone;							  // ���� ���� ��(Ÿ�� ��Ż�� �̿��Ѵ�)
	int		m_iPotalX;
	int		m_iPotalY;

public:
	void GetOutof(COM *pCom);
	void SetInitFortressViolence();
	BOOL SetNpcToFortressViolenceEnd(COM *pCom);
	void SetNpcToFortressViolenceBegin();//(COM *pCom);
	void AddGuildUserForFortressCityRank(TCHAR *strUserID, int sCityRank);
	int GetTotalCityValue();
	void ApplyTaxToStore();
	void SetInitFortress();
	void InitMemFortress(BYTE tGuildWarType);
	void UpdateInitFortress();
	void InitMsgTime();
	void StrFortressRepairList(CByteArray &Buf);
//	void StrFortressRepairList(TCHAR *pBuf);
	void FortressRepairListToStr(TCHAR *pBuf);
	void SetNpcToFortressWarEnd(COM *pCom);
	CNpc* GetNpc(int nid);
	void SetNpcToFortressWarBegin();
	void GuildGuardListToStr(TCHAR *pBuf);
	void GuildAttListToStr(TCHAR*pBuf);
	void StrGuildGuardList(TCHAR *pBuf);
	void StrToGuildAttList(TCHAR *pBuf, int iguild);

	CGuildFortress();
	virtual ~CGuildFortress();

};

#endif // !defined(AFX_GUILDFORTRESS_H__42E14E0C_5B21_430C_9A03_60CC86DD6BB3__INCLUDED_)
