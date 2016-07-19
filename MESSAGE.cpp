// MESSAGE.cpp: implementation of the MESSAGE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "MESSAGE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MESSAGE::MESSAGE()
{
	m_ID = 0;
	::ZeroMemory(m_MESSAGE_BODY, sizeof(m_MESSAGE_BODY));
}

MESSAGE::~MESSAGE()
{

}
