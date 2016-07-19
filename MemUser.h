// MemUser.h: interface for the CMemUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMUSER_H__A2D30C97_9428_4023_80A4_664B2B403E04__INCLUDED_)
#define AFX_MEMUSER_H__A2D30C97_9428_4023_80A4_664B2B403E04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemDefine.h"


class CMemGuildHouseRank
{
public:
	CMemGuildHouseRank();
	~CMemGuildHouseRank();

	BYTE	tSid;	
	int		lGuild;
	BYTE	iGuildNum;
	short	iLastValue;
	short	iGuildLevel;
	DWORD	dwLastTime;
};

class CMemFortress
{
public:
	CMemFortress();
	~CMemFortress();

	short	m_sFortressID;
	int		m_iGuildSid;

	short	m_sTaxRate;
	DWORD	m_dwStoreDN;
	BYTE	m_tWarType;

	TCHAR	m_AttackList[_GUILDLIST_DB];
	TCHAR	m_RepairList[_GUILDREPAIRLIST_DB];
};

class CMemStore
{
public:
	CMemStore();
	~CMemStore();

	short	m_sStoreID;						// ���� �ε���
	int		m_iGuildSid;					// ������ ������ ��� �ε���

	short	m_sTaxRate;						// ����
	DWORD	m_dwStoreDN;					// ���� ��
	BYTE	m_tWarType;						// ����� �ñ� ���� �÷���

	TCHAR	m_AttackList[_GUILDLIST_DB];	// ������� ��û�� ��� ����Ʈ
};

class CMemBank
{
public:
	CMemBank();
	~CMemBank();

	int		m_uid;

	TCHAR	m_strUserID[CHAR_NAME_LENGTH + 1];	// ���̵�(ĳ���� �̸�)
	TCHAR	m_UserBankItem[_BANK_DB];		// ���೻ ������
	DWORD	m_dwBankDN;						// User Bank DN
};

class CMemAccountBank
{
public:
	CMemAccountBank();
	~CMemAccountBank();

	int		m_uid;

	TCHAR	m_strAccount[CHAR_NAME_LENGTH + 1];	// ���̵�(ĳ���� �̸�)
	TCHAR	m_AccountBankItem[_ACCOUNT_BANK_DB];// ���೻ ������
	DWORD	m_dwBankDN;							// User Bank DN
};

typedef struct _ITEMLOG
{
	CTime	t;
	short	m_sLogType;
	char	m_strUserId[CHAR_NAME_LENGTH + 1];		// ���� ��ü
	char	m_strEtc[CHAR_NAME_LENGTH + 1];			// ������

	short	m_sLevel;
	short	m_sSid;
	short	m_sDuration;
	short	m_sBullNum;
	short	m_sCount;
	BYTE	m_tMagic[11];
	BYTE	m_tIQ;

	UINT64	m_iItemSerial;

	DWORD	m_dwResultDN;
	DWORD	m_dwInOutDN;
	DWORD	m_dwRelateDN;
} ITEMLOG;

class CMemUser  
{
public:
	CMemUser();
	~CMemUser();

	CMemBank	m_UB;
	CMemAccountBank m_AB;

	int			m_uid;					// Server User id

	TCHAR		m_strAccount[CHAR_NAME_LENGTH + 1];
	TCHAR		m_strUserID[CHAR_NAME_LENGTH + 1];	// ���̵�(ĳ���� �̸�)
	
	//-- yskang 0.1 ��忡�� ȣĪ�ο�....
	TCHAR m_strLoveName[LOVE_NAME_LENGTH+1];

	DWORD		m_dwSaveTime;			// �� 4, �� 4, �� 5, �� 5, �� 6, �� 6 BYTE	

	short		m_sSTR;					// ��
	short		m_sCON;					// �ǰ�
	short		m_sDEX;					// ��ø��
	short		m_sVOL;					// ����
	short		m_sWIS;					// ����
	
	DWORD		m_iSkin;				// �Ǻλ�
	DWORD		m_iHair;				// �Ӹ���
	short		m_sGender;				// ����
	TCHAR		m_strFace[10];			// �󱼸��

	int			m_curx;					// ���� X ��ǥ
	int			m_cury;					// ���� Y ��ǥ
	int			m_curz;					// ���� ��

	DWORD		m_dwBuddy;				// �����ȣ
	long		m_dwGuild;				// ����ȣ

	DWORD	m_dwExp;				// ��������ġ

	short		m_sPA;					// PA Point
	short		m_sSkillPoint;			// Skill Point
	
	DWORD		m_dwXP;					// X Point

	short		m_sMaxHP;				// �ִ� HP
	short		m_sHP;					// ���� HP
	short		m_sMaxPP;				// �ִ� PP
	short		m_sPP;					// ���� PP
	short		m_sMaxSP;				// �ִ� SP
	short		m_sSP;					// ���� SP

	DWORD		m_dwDN;					// ������

	short		m_sCityRank;			// �ù� ���
	short		m_sKillCount;			// PK�� Ƚ��

	short		m_sLevel;				// �迭����
	BYTE		m_byClass;				// Ŭ����

	TCHAR		m_strSkill[_SKILL_DB];	// User Skill DB
	TCHAR		m_strItem[_ITEM_DB];	// User Item DB
	TCHAR		m_strPsi[_PSI_DB];		// User Psionic DB
	TCHAR		m_strHaveEvent[_EVENT_DB];// User Event DB
	TCHAR		m_strTel[_TEL_DB];		// User Tel DB

	short		m_sChangeClass[_CHANGE_DB];		// ������ Ŭ�������� ����

	BYTE		m_tAbnormalKind;				// �����̻� ����
	DWORD		m_dwAbnormalTime;				// �����̻� �ɸ��� ���� �ð�
	
	TCHAR		m_strQuickItem[_QUICKITEM_DB];	// Quick Item Index
	
	BYTE		m_tIsOP;					// ������� �Ǵ�
											// 0: Normal User
											// 1: Game Operator
											// 2: Server Operator

	BYTE		m_bLive;				// �׾���? ��Ҵ�?

	BYTE		m_sTempSTR;				// ��
	BYTE		m_sTempCON;				// �ǰ�
	BYTE		m_sTempDEX;				// ��ø��
	BYTE		m_sTempVOL;				// ����
	BYTE		m_sTempWIS;				// ����

	int			m_iCityValue;			// ���� ���� ���Ⱚ

	DWORD		m_dwSaintTime;

	DWORD		m_dwHiExpTime;			// ����ġ 2�� ������ ������ ���� �ð� 
	DWORD		m_dwHtExpTime;    //������ʯʱ���¼
	DWORD		m_dwMagicFindTime;		// �������� 5�� ������ ������ ���� �ð�
	DWORD		m_dwMagicFtTime; //���뾧ʯʱ���¼
	DWORD		m_dwNoChatTime;			// ä���� ������ ���� �ð�
    DWORD		m_dwCloseTime; 
	DWORD		m_dwAutoMoney;
    DWORD		m_dwLingQu;
	DWORD		m_dwGuarDianTianShi;//�ػ���ʹ
	DWORD		m_dwShopPingDN;
	DWORD		m_dwVIPTime;//��Ա
	DWORD		m_dwZaiXianTime; //����ʱ��
	DWORD		m_dwBFindTime;//����
	DWORD       m_dwBHTime;
    DWORD       m_dwSJTime;

	
	BYTE		m_tPsiOneKind;			
	BYTE		m_tPsiTwoKind;
	BYTE		m_tPsiThreeKind;
	
	DWORD		m_dwPsiOneTime;
	DWORD		m_dwPsiTwoTime;
	DWORD		m_dwPsiThreeTime;
	//DWORD       m_dwBSTime;
	DWORD       m_dwDNMoney;
	DWORD       m_dwShTsTime;
	DWORD       m_isDoubleExp;//���3������ʱ��
	DWORD       m_isDoubleBAOLV;//���3������ʱ��

	char		m_arItemLogData[100][sizeof(ITEMLOG)];	// ������ �α�
	
	int			m_iMyServer;			// ���� ������ ���� �ε���
};

#endif // !defined(AFX_MEMUSER_H__A2D30C97_9428_4023_80A4_664B2B403E04__INCLUDED_)
