// NpcTable.h: interface for the CNpcTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCTABLE_H__C9E9FA22_E50D_4EAD_980F_2A89E9EDC094__INCLUDED_)
#define AFX_NPCTABLE_H__C9E9FA22_E50D_4EAD_980F_2A89E9EDC094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNpcTable  
{
public:
	int		m_sSid;				// MONSTER(NPC) Serial ID
	int		m_sPid;				// MONSTER(NPC) Picture ID
	TCHAR	m_strName[20];		// MONSTER(NPC) Name

	int		m_sSTR;				// ��
	int		m_sDEX;				// ��ø
	int		m_sVOL;				// ����
	int		m_sWIS;				// ����

	int		m_sMaxHP;			// �ִ� HP
	int		m_sMaxPP;			// �ִ� PP
	
	BYTE	m_byClass;			// ����迭
	BYTE	m_byClassLevel;		// ����迭 ����

	int		m_sExp;				// ����ġ

	int	m_byAX;				// ���ݰ� X
	int	m_byAY;				// ���ݰ� Y
	int	m_byAZ;				// ���ݰ� Z

	int		m_iDefense;		// ��
	BYTE	m_byRange;			// �����Ÿ�

	int		m_sAI;				// �ΰ����� �ε���
	int		m_sAttackDelay;		// ���ݵ�����
	BYTE	m_byVitalC;			// ��ü������ ũ��Ƽ��
	BYTE	m_byWildShot;		// ���� ����
	BYTE	m_byExcitedRate;	// ��� ����
	BYTE	m_byIronSkin;		// ���
	BYTE	m_byReAttack;		// �ݰ�
	BYTE	m_bySubAttack;		// �����̻� �߻�(�ΰ�����)
	BYTE	m_byState;			// ���� (NPC) �����̻�
	BYTE	m_byPsi;			// ���̿��� ����
	BYTE	m_byPsiLevel;		// ���̿��з���

	BYTE	m_bySearchRange;	// �� Ž�� ����
	int		m_sSpeed;			// �̵��ӵ�	

	int		m_sInclination;		// ����	(�������� �ùε������� �ش�.)
	BYTE	m_byColor;			// �����ް� �Ϲݱ�
	int		m_sStandTime;		// ���ִ� �ð�
	BYTE	m_tNpcType;			// NPC Type
								// 0 : Monster
								// 1 : Normal NPC

	int		m_sFamilyType;		// ������̿��� �������踦 �����Ѵ�.
	BYTE	m_tItemPer;			// �������� ������ Ȯ��
	BYTE	m_tDnPer;			// ���� ������Ȯ��
	
/*
//	int		m_sCON;				// �ǰ�
//	int		m_sINT;				// ����
	BYTE	m_byCritical;		// ũ��Ƽ�ù߻� ����
	BYTE	m_byDamageC;		// ������ũ��Ƽ�� ����
	BYTE	m_byDelayC;			// ������ũ��Ƽ�� ����
	BYTE	m_byOutRange;		// ������ ��� ����
	BYTE	m_byBlowRate;		// ��Ÿ ����
	BYTE	m_byCGuard;			// ũ��Ƽ�ð���	����
	int		m_sEvent;			// �̺�Ʈ ��ȣ
	int		m_sHaveItem;		// ���Ͱ� �� ��� �� ������ �ε���
*/
	CNpcTable();
	virtual ~CNpcTable();
};

#endif // !defined(AFX_NPCTABLE_H__C9E9FA22_E50D_4EAD_980F_2A89E9EDC094__INCLUDED_)
