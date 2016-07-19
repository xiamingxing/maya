// Store.h: interface for the CStore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STORE_H__88D5E249_0673_4E53_BB8E_DA847DB3CFF1__INCLUDED_)
#define AFX_STORE_H__88D5E249_0673_4E53_BB8E_DA847DB3CFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SharedMemory.h"
#include "MemUser.h"

struct GuildStoreInfo
{
	long	m_iGuildSid;		// �ش� ��� �ε���
	TCHAR	m_strGuildName[CHAR_NAME_LENGTH + 1]; // �ش� ��� �̸�
	TCHAR	m_strMasterName[CHAR_NAME_LENGTH + 1];// �ش� ��� ¯ �̸�
	DWORD	m_iGuildDN;							  // ������ �ݾ�	

	BYTE	m_tWarType;							  // 0: ����� ��¥ ���� 12�ð��� 1: �̹� ����� ��¥�� ����	
	BYTE	m_tGuildWar;						  // 0 : ����� �غ�ܰ�
											      // 1 : �������
												  // 2 : �Ϲ� �ܰ�
	SYSTEMTIME m_wLastWarTime;
	SYSTEMTIME m_wPlanWarTime;
};

struct AttckGuildList
{
	TCHAR GuildName[CHAR_NAME_LENGTH + 1];
	long	iGuildSid;
};

//typedef CTypedPtrArray <CPtrArray, AttckGuildList*>		AttGuildArray;

class CStore  
{
public:
	void GuildListToStr(TCHAR *pBuf);
	void StrToGuildList(TCHAR *pBuf);
	void InitStoreInfo(int sid);
	BOOL IsCanRepair(short sSid);
	int	 IsCanBuyPsi(CStore *pStore, int iClass);
	BOOL IsCanSell(short sSid);

public:
	BOOL IsCanBuy(short sSid);
	void SetGuildStoreTex();
	BOOL UpdateGuildStore();
	short	m_sStoreID;			// ���� ID
	BYTE	m_tStoreType;		// ����Ÿ�� - 0: �Ϲ� ������, 1: ���̿���, 2: ��ī�� 

	BOOL	m_bHaveGuild;		// ���� ��忡 ���� �����̴�	TRUE	
	long	m_lUsed;			// ����� ���������� �ƴ��� ǥ��...
	long	m_lAreaUsed;		// ����� ���������� ǥ����
	DWORD	m_dwStartTick;

	int		m_sRate;			// �ü�

//	GuildStoreInfo	m_StoreInfo;
	long	m_iGuildSid;		// �ش� ��� �ε���
	TCHAR	m_strGuildName[CHAR_NAME_LENGTH + 1]; // �ش� ��� �̸�
	TCHAR	m_strMasterName[CHAR_NAME_LENGTH + 1];// �ش� ��� ¯ �̸�
	DWORD	m_iGuildDN;							  // ������ �ݾ�	

	BYTE	m_tWarType;							  // 0: ����� ��¥ ���� 12�ð��� 1: �̹� ����� ��¥�� ����	
	BYTE	m_tGuildWar;						  // 0 : ����� �غ�ܰ�
											      // 1 : �������
												  // 2 : �Ϲ� �ܰ�
	SYSTEMTIME m_wLastWarTime;
	SYSTEMTIME m_wPlanWarTime;

	int		m_iStandingTime;
	long	m_lListUsed;						  // ������� �����ϴ� ��� ����Ʈ	
	long	m_lListCount;
	int		m_arAttackGuild[GUILD_ATTACK_MAX_NUM]; // �ִ� 20���� ��尡 ���￡ �����Ҽ� �ִ�.

	CWordArray	m_arItems;			// ������ ID Array
	CWordArray	m_arSellList;		// �� �������� �����ϴ� ������ ���
	CWordArray	m_arRepairList;		// �� �������� �����ϴ� ������ ���

	CWordArray	m_arNpcList;

	CStore();
	~CStore();
};

#endif // !defined(AFX_STORE_H__88D5E249_0673_4E53_BB8E_DA847DB3CFF1__INCLUDED_)
