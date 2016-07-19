// OnlineShop.h: interface for the OnlineShop class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once

class CRMBExchangeShop  
{
public:
	CRMBExchangeShop();
	virtual ~CRMBExchangeShop();
public:
	int m_rmbCode;
	short m_iSid;
	TCHAR m_iSname[20];
	BYTE  m_tIQ;
	int m_iNum;
	short m_sx1;
	short m_sx2;
	short m_sx3;
	short m_sx4;
	short m_sx5;
	short m_upgrade;
	short m_sx6;
	short m_sx7;
	short m_sx8;
	short m_sx9;
	short m_sx10;
	TCHAR m_iText[200];
};

#endif