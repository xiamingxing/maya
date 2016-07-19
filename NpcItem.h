// NpcItem.h: interface for the CNpcItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCITEM_H__56D8779F_271E_4B39_92CF_3DA33366FBA8__INCLUDED_)
#define AFX_NPCITEM_H__56D8779F_271E_4B39_92CF_3DA33366FBA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNpcItem  
{
public:

	int **m_ppItem;
	int m_nRow;
	int m_nField;

	CNpcItem();
	~CNpcItem();
};

struct drop_novelity {
	char name[64];//��Ʒ����
	byte code1;//��Ʒ����
	byte code2;
	int per;
};
struct drop_info {
	char name[64]; //��������
	byte code1;//��������
	byte code2;
	int money;//��Ǯ����
	int DropLeechdom;//��ҩƷ����
	int DropNovelity;//����Ʒ����
	int n; //��Ʒ��������
	struct drop_novelity novelity[256];
};
struct convert_table{ //100����Ʒ���ɱ�
	char src_name[64];
	SHORT Ssid;
	char dst_name[64];
	SHORT Dsid;
	BOOL have;
};

#endif // !defined(AFX_NPCITEM_H__56D8779F_271E_4B39_92CF_3DA33366FBA8__INCLUDED_)
