// NpcTable.cpp: implementation of the CNpcTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "NpcTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcTable::CNpcTable()
{
	m_sSid				= 0;				// MONSTER(NPC) Serial ID
	m_sPid				= 0;				// MONSTER(NPC) Picture ID
	::ZeroMemory(m_strName, sizeof(m_strName));		// MONSTER(NPC) Name

	m_sSTR				= 0;				// ��
	m_sDEX				= 0;				// ��ø
	m_sVOL				= 0;				// ����
	m_sWIS				= 0;				// ����
	m_sMaxHP			= 0;				// �ִ� HP
	m_sMaxPP			= 0;				// �ִ� PP

	m_byClass			= 0;				// ����迭
	m_byClassLevel		= 0;				// ����迭 ����
	m_sExp				= 0;				// ����ġ
	m_byAX				= 0;				// ���ݰ� X
	m_byAY				= 0;				// ���ݰ� Y
	m_byAZ				= 0;				// ���ݰ� Z
	
	m_iDefense			= 0;				// ��
	m_byRange			= 0;
	m_sAI				= 0;				// �ΰ����� �ε���
	m_sAttackDelay		= 0;				// ���ݵ�����
	m_byVitalC			= 0;				// ��ü������ ũ��Ƽ��
	m_byWildShot		= 0;				// ���� ����
	m_byExcitedRate		= 0;				// ��� ����
	m_byIronSkin		= 0;
	m_byReAttack		= 0;
	m_bySubAttack		= 0;				// �����̻� �߻�(�ΰ�����)
	m_byState			= 0;				// ���� (NPC) �����̻�
	m_byPsi				= 0;				// ���̿��� ����
	m_byPsiLevel		= 0;				// ���̿��з���
	m_bySearchRange		= 0;				// �� Ž�� ����
	m_sSpeed			= 0;				// �̵��ӵ�	
	m_sInclination		= 0;
	m_byColor			= 0;
	
	m_sStandTime		= 0;
	m_tNpcType			= 0;				// NPC Type

	m_sFamilyType		= 0;		// ������̿��� �������踦 �����Ѵ�.
	m_tItemPer			= 0;			// �������� ������ Ȯ��
	m_tDnPer			= 0;			// ���� ������Ȯ��
}

CNpcTable::~CNpcTable()
{

}
