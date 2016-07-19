// YhuoIni.cpp: implementation of the YhuoIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "YhuoIni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

YhuoIni::YhuoIni()
{
	
	//[WEBURL]
	::ZeroMemory(WEB_URL, sizeof(WEB_URL));
}

YhuoIni::~YhuoIni()
{

}
