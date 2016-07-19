#include "stdafx.h"
#include "ShopSystem.h"

ShopSystem::ShopSystem(void)
{
	m_QanBao_KG = 0;//Ԫ��ȯ,JZB������

	m_iAllTime = 0;    //ÿ����������Ԫ��
	m_iAllDnNum = 0;   //ÿ�������Ԫ��
	m_iZoneNum = 0;
	memset(m_iZone,0,sizeof(int) *255); //��ͼID
	m_iZoneTime = 0;  //ÿ����������Ԫ��
	m_iZoneDnNum = 0;   //ÿ�������Ԫ��
	memset(m_Item,0,sizeof(ShopItemData)*512);
	m_ItemNum = 0;

}

ShopSystem::~ShopSystem(void)
{
}
BOOL ShopSystem::ShopLoadConfig()
{
	char szTemp[255];

    m_QanBao_KG = GetPrivateProfileInt("����",
		"Ԫ��ȯJZB������",0,".\\Shop.ini");
	m_iAllDnNum = GetPrivateProfileInt("Ԫ��",
		"ȫ��Ԫ��",0,".\\Shop.ini");
	m_iAllTime = GetPrivateProfileInt("Ԫ��",
		"ȫ��ʱ��",0,".\\Shop.ini");
	m_iAllTime = 1000 * m_iAllTime;
	if (m_iAllTime == 0)
	{
		m_iAllTime = 10000000;
	}

	m_iZoneDnNum = GetPrivateProfileInt("Ԫ��",
		"��ͼԪ��",0,".\\Shop.ini");
	m_iZoneTime = GetPrivateProfileInt("Ԫ��",
		"��ͼʱ��",0,".\\Shop.ini");
	m_iZoneTime = 1000 * m_iZoneTime;
	if (m_iZoneTime == 0)
	{
		m_iZoneTime = 10000000;
	}
	m_iZoneNum = GetPrivateProfileInt("Ԫ��",
		"ZONENUM",0,".\\Shop.ini");
	for (int i=0; i<m_iZoneNum; i++)
	{
		sprintf(szTemp,"ZONE%02d",i);
		m_iZone[i] = GetPrivateProfileInt("Ԫ��",
			szTemp ,0,".\\Shop.ini");
	}
	
	m_ItemNum = GetPrivateProfileInt("��Ʒ",
		"����",0,".\\Shop.ini");

	for (int i=0; i<m_ItemNum; i++)
	{
		sprintf(szTemp,"��Ʒ%02d",i);
		m_Item[i].iSid = GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");

		sprintf(szTemp,"����%02d",i);
		m_Item[i].iNum = GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");
		sprintf(szTemp,"�۸�%02d",i);
		m_Item[i].iDn = GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");
		sprintf(szTemp,"��������%02d",i);
		int iLen = GetPrivateProfileString(
			"��Ʒ",
			szTemp,
			NULL ,
			m_Item[i].BuyName,
			20,
			".\\Shop.ini"
			);
		m_Item[i].BuyName[iLen] = '\0';

		sprintf(szTemp,"����%02d-1",i);
		m_Item[i].iAttribute[0]= GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");
		sprintf(szTemp,"����%02d-2",i);
		m_Item[i].iAttribute[1]= GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");
		sprintf(szTemp,"����%02d-3",i);
		m_Item[i].iAttribute[2]= GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");
		sprintf(szTemp,"����%02d-4",i);
		m_Item[i].iAttribute[3]= GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");
		sprintf(szTemp,"����%02d-5",i);
		m_Item[i].iAttribute[4]= GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");
		sprintf(szTemp,"��ɫ%02d",i);
		m_Item[i].iColor= GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");
		sprintf(szTemp,"����%02d",i);
		m_Item[i].iUpgradeNum= GetPrivateProfileInt("��Ʒ",
			szTemp ,0,".\\Shop.ini");

	}
	
	return TRUE;

}

BOOL ShopSystem::IsZone(int iZone)
{
	for (int i=0; i<m_iZoneNum; i++)
	{
		if (m_iZone[i] == iZone)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int ShopSystem::IsBuyItemName(char *name)
{
	for (int i=0; i<m_ItemNum; i++)
	{
		if(strcmp(m_Item[i].BuyName,name) == 0)
		{
			return i;
		}
	}
	return -1;
}
