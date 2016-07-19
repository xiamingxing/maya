// MESSAGE.h: interface for the MESSAGE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__6DE52814_F08F_4A8E_B63B_A5166B4924CB__INCLUDED_)
#define AFX_MESSAGE_H__6DE52814_F08F_4A8E_B63B_A5166B4924CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MESSAGE  
{
public:
	MESSAGE();
	virtual ~MESSAGE();

	short	m_ID;
	TCHAR	m_MESSAGE_BODY[500];
};

#endif // !defined(AFX_MESSAGE_H__6DE52814_F08F_4A8E_B63B_A5166B4924CB__INCLUDED_)
