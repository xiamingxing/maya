// MAP.h: interface for the MAP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAP_H__9D08629D_3A60_11D3_BE0B_00105A6AE68D__INCLUDED_)
#define AFX_MAP_H__9D08629D_3A60_11D3_BE0B_00105A6AE68D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class	ItemList
{
public:
	short	uid[ITEM_USER_RIGHT_NUM];// �켱 ���� 3��
	BYTE	SuccessRate[ITEM_USER_RIGHT_NUM];// �켱 ���� 3��
	DWORD	dwTime;
//	SYSTEMTIME  ThrowTime;

public:
	BYTE	tType;			// ������(1)���� ��(0)���� ����
	short	sLevel;			// �����Ӽ��� ������ ������ ��ȭ�Ѵ�.(2001_07_13)	
	short	sSid;
	short	sDuration;
	short	sBullNum;
	short	sCount;
	BYTE	tMagic[11];
	BYTE	tIQ;

	DWORD	dwMoney;		// ���̸�... 

	__int64 iItemSerial;	// ������ ������ȣ

	ItemList()
	{
		dwTime = 0;
		uid[0] = uid[1] = uid[2] = -1;
		SuccessRate[0] = SuccessRate[1] = SuccessRate[2] = 0;
	};
};

class BodyInfo				// ��ü ó�� Class
{
public:
	int			iId;		// ��ü�� ������ �ִ� ��ȣ
	char		cName[CHAR_NAME_LENGTH+1];	// ��ü�� �̸�
	DWORD		dTime;		// ��ü�� �� �ð�
};

typedef CTypedPtrArray <CPtrArray, ItemList*> ItemInfoArray;

typedef CTypedPtrArray <CPtrArray, int*> ZoneUserArray;
typedef CTypedPtrArray <CPtrArray, BodyInfo*> BodyArray;

class MapInfo					// �� ��ǥ�� ����
{
public:
	LONG	m_bMove;			// �̵��Ӽ�(0 : �̵�����, ELSE : �̵��Ұ�)  
	LONG	m_lUser;			// ��������								
	short	m_sEvent;			// ����ǥ�� �̺�Ʈ ��ȣ
	WORD	m_dwType;			// ���ÿ� �ʵ� ����


	DWORD			m_FieldDN;	// �ʿ� ���̴� ��								����
	ItemInfoArray	m_Item;		// �ʿ� ���̴� ������ ����Ʈ					����

	ItemList		*m_pItem;
	int				iIndex;		// ������ �迭 �ε���
	LONG			m_FieldUse;	// ���� �ʿ� �������� ������ �� (1)	
								// �ƴϸ� (0)
	MapInfo();
	virtual ~MapInfo();
};

class MoveCell
{
public:
	void ConvertMapFromATM(MapInfo **tmap);
	void LoadSubInfo(CArchive &ar);
	MoveCell();
	~MoveCell();

	void RemoveData();
	void Initialize(int cx, int cy);
	void ConvertMap(MapInfo **tmap);

	void Load(CArchive& ar);
	void LoadSize(CArchive &ar);

	CSize	m_vDim;				// Cell���� ũ��
	int		**m_pMoveInfo;		// �� Cell���� ����
};

class MAP  
{
public:
	void SetType(int x, int y, int nType);
	void LoadSubInfo(CArchive &ar);
	BOOL LoadAtm(char *szFileName);
	void Load(CArchive& ar);
	void InitTargetMap(int cx, int cy);		// �� Size Initialize
	BOOL LoadImf(char *szFileName);
	BOOL LoadImfSize(char* szFileName);
	BOOL LoadMapByNum(int num);				// ��ȣ�� ���� �� Load
	BOOL LoadMapSizeByNum(int num);
/*

	void Load(CArchive& ar);
	void InitTargetMap(int cx, int cy);		// �� Size Initialize
	BOOL LoadImf(char *szFileName);
	BOOL LoadMapByNum(int num);				// ��ȣ�� ���� �� Load
*/
	MoveCell		m_vMoveCell;			// IMF���� ���� �б� ���

	CSize			m_sizeMap;				// ���� ũ��
	MapInfo			**m_pMap;
	int				m_Zone;					// ���� �ð� �ִ� �� ��ȣ

	ZoneUserArray	m_ZoneUserArray;

	MAP();
	virtual ~MAP();

protected:
	void RemoveMapData();
};

#endif // !defined(AFX_MAP_H__9D08629D_3A60_11D3_BE0B_00105A6AE68D__INCLUDED_)
