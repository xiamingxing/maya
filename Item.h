// Item.h: interface for the Item class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__59D3FA6E_13EE_49E1_9995_9D1878750A76__INCLUDED_)
#define AFX_ITEM_H__59D3FA6E_13EE_49E1_9995_9D1878750A76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TableItem;

class Item  
{
public:
	Item();
	virtual ~Item();
	TableItem* GetTableItem(int num);
	BOOL ParseItemData(char* pBuf);
	void FillItemData(char* pBuf);

public:
	short			m_TNum;			// ���̺� ������ ��ȣ

	BYTE			m_ItemNum;		// �ش� item�� �ߺ���
	BYTE			m_Type;			// ������ ����
	BYTE			m_Arm;			// ������ġ
	BYTE			m_Uselevel;		// ��밡�� ����
	BYTE			m_UseType;		// ��밡�� �迭

	short			m_PicNum;		// �׸� ��ȣ
	short			m_Wg;			//  SP���ҷ�
	short			m_Dur;			// ������
	short			m_MaxAt;		// �ִ� ���ݷ�
	short			m_AtDelay;		// ���� ������

	BYTE			m_DmgX;			// ����� X
	BYTE			m_DmgY;			// �����Y
	BYTE			m_DmgZ;			// �����Z
	BYTE			m_Df;			// ����
	BYTE			m_At;			// ���� ���߰�
	BYTE			m_Crit;			// ũ��Ƽ�� Ȯ��
	BYTE			m_Range;		// �����Ÿ�
	BYTE			m_Out;			// �����
	BYTE			m_BullNum;		// źâ��
	BYTE			m_BullType;		// źâ����
	BYTE			m_StErr;		// �����̻�
	BYTE			m_StDf;			// �����̻� ���
	BYTE			m_StCure;		// �����̻� ġ��

	BYTE			m_Hp;			// ȸ��
	BYTE			m_Pp;			// ȸ��
	short			m_Sp;;			// ȸ��

	DWORD			m_Cost;			// ����
	char			m_Name[ITEM_NAME_LENGTH];
};

#endif // !defined(AFX_ITEM_H__59D3FA6E_13EE_49E1_9995_9D1878750A76__INCLUDED_)
