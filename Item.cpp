// Item.cpp: implementation of the Item class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "Item.h"
#include "extern.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern TableItemArray  g_DBItemArray;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Item::Item()
{

}

Item::~Item()
{

}

BOOL Item::ParseItemData(char *pBuf)
{
	int index = 0;

	m_TNum = GetShort(pBuf, index);
	m_Dur  = GetShort(pBuf, index);
	m_BullNum = GetByte(pBuf, index);
	m_ItemNum = GetByte(pBuf, index);
	m_Cost = GetDWORD(pBuf, index);

	TableItem* tItem = GetTableItem( m_TNum );	if( !tItem ) return FALSE;

//	m_Type = tItem->;			// ������ ����
	m_Arm = tItem->m_Arm;			// ������ġ
	m_Uselevel = tItem->m_UseLevel;		// ��밡�� ����
	m_UseType = tItem->m_UseType;		// ��밡�� �迭

	m_PicNum = tItem->m_PicNum;		// �׸� ��ȣ
	m_Wg = tItem->m_Wg;			//  SP���ҷ�
	m_Dur = tItem->m_Dur;			// ������
	m_MaxAt = tItem->m_MaxAt;		// �ִ� ���ݷ�
	m_AtDelay = tItem->m_AtDelay;		// ���� ������

	m_DmgX = tItem->m_DmgX;			// ����� X
	m_DmgY = tItem->m_DmgY;			// �����Y
	m_DmgZ = tItem->m_DmgZ;			// �����Z
	m_Df = tItem->m_Df;			// ����
	m_At = tItem->m_At;			// ���� ���߰�
	m_Crit = tItem->m_Crit;			// ũ��Ƽ�� Ȯ��
	m_Range = tItem->m_Range;		// �����Ÿ�
	m_Out = tItem->m_Out;			// �����
	m_BullNum = tItem->m_BullNum;		// źâ��
	m_BullType = tItem->m_BullType;		// źâ����
	m_StErr = tItem->m_StErr;		// �����̻�
	m_StDf = tItem->m_StDf;			// �����̻� ���
	m_StCure = tItem->m_StCure;		// �����̻� ġ��

	m_Hp = tItem->m_HP;			// ȸ��
	m_Pp = tItem->m_PP;			// ȸ��
	m_Sp = tItem->m_SP;			// ȸ��

	return TRUE;
}

TableItem* Item::GetTableItem(int num)
{
	if( num < 0 || num >= 1000 ) return NULL;

	return g_DBItemArray[num];
}

void Item::FillItemData(char *pBuf)
{
	int index = 0;

	SetShort(pBuf, m_TNum, index);
	SetShort(pBuf, m_Dur, index);
	SetByte(pBuf, m_BullNum, index);
	SetByte(pBuf, m_ItemNum, index);

	SetDWORD(pBuf, m_Cost, index);
}
