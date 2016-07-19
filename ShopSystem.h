#pragma once

struct ShopItemData 
{
	int iSid;
	int iNum;
	char BuyName[255];
	int iDn;
	int iAttribute[5];
	int iUpgradeNum;
	int iColor;
};
class ShopSystem
{
public:
	int m_QanBao_KG;//Ԫ��ȯ,JZB������
	int m_iAllTime;    //ÿ����������Ԫ��
	int m_iAllDnNum;   //ÿ�������Ԫ��
	int m_iZoneNum;
	int m_iZone[255]; //��ͼID
	int m_iZoneTime;  //ÿ����������Ԫ��
	int m_iZoneDnNum;   //ÿ�������Ԫ��
	ShopItemData m_Item[512];
	int m_ItemNum;

public:
	ShopSystem(void);
	~ShopSystem(void);
	BOOL ShopLoadConfig();
	BOOL IsZone(int iZone);
	int IsBuyItemName(char *name);
};
