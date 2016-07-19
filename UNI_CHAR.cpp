// UNI_CHAR.cpp: implementation of the UNI_CHAR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UNI_CHAR.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// UNI_CHAR.cpp: implementation of the UNI_CHAR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UNI_CHAR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//char* UNI_CHAR::UniCodeC[19];

//char* UNI_CHAR::UniCodeV[21];
//char* UNI_CHAR::UniCodeJ[28];


char *UNI_CHAR::UniCodeC[19]={"��","��","��","��","��","��","��","��","��","��","��","��",
	"��","��","��","��","��","��","��"};
	//�����ڵ� �ѱ� �߼� ����ǥ
char* UNI_CHAR::UniCodeV[21]={"��","��","��","��","��","��","��","��","��","��","��","��",
	"��","��","��","��","��","��","��","��","��"};
	//�����ڵ� �ѱ� ���� ����ǥ 
char* UNI_CHAR::UniCodeJ[28]={" ","��","��","��","��","��","��","��","��","��","��","��","��",
	"��","��","��","��","��","��","��","��","��","��","��","��","��","��","��"}; 




UNI_CHAR::UNI_CHAR()
{
/* 	char *tmpUniCodeC[19]={"��","��","��","��","��","��","��","��","��","��","��","��",
		"��","��","��","��","��","��","��"};
		//�����ڵ� �ѱ� �߼� ����ǥ
	 char* tmpUniCodeV[21]={"��","��","��","��","��","��","��","��","��","��","��","��",
		"��","��","��","��","��","��","��","��","��"};
		//�����ڵ� �ѱ� ���� ����ǥ 
	 char* tmpUniCodeJ[28]={" ","��","��","��","��","��","��","��","��","��","��","��","��",
		"��","��","��","��","��","��","��","��","��","��","��","��","��","��","��"}; 

	 // �ʱ�ȭ�� ��� �ؾ� ���� �𸣰ڴ�..
	 // �׷��� �ϴ� ���ڸ� ������ �Ǵ��ϱ� ������..
	 */
}

UNI_CHAR::~UNI_CHAR()
{

}


BOOL UNI_CHAR::CheckString(CString str)
{
	BYTE *cComp;
	int nLength = str.GetLength();
	cComp = (BYTE*)str.GetBuffer(nLength);

	for (int i=0; i < nLength; i++) 
	{
#ifdef KOR_VER			
		int nCode = 949;//korea
#endif
#ifdef CHN_VER
		int nCode = 936; //china
#endif
#ifdef ENG_VER
		int nCode = 1252; //English
#endif
		if ( IsDBCSLeadByteEx(nCode,str.GetAt(i)) ) { // �ѱ��ڼ��� ���.
			ASSERT(i < nLength-1);
#ifdef KOR_VER			
			CVJ cvj;
			cvj = UNIDivChar(((LPSTR)(LPCTSTR)str)+i);
			if (!CheckUNIChar(cvj)) {
				return FALSE;
			}
#endif
#ifdef CHN_VER
		BYTE cTemp[2];

		memcpy(cTemp,cComp+i,sizeof(BYTE)*2);
		int nLen = MultiByteToWideChar(CP_ACP, 0,(char*)cTemp, -1, NULL, NULL);
		LPWSTR lpszW = new WCHAR[nLen];
		MultiByteToWideChar(CP_ACP, 0, (char*)cTemp, -1, lpszW, nLen);

		WORD wTemp = *(WORD *)lpszW;		
	
		delete[] lpszW;
		if(wTemp < 0x4e00 || wTemp > 0x9fa5){		
			return FALSE;
		}
#endif
			i ++;
		} else { // ���� �ڼ��� ���.
			if((cComp[i] >= 48 && cComp[i] <= 57) || 
			   (cComp[i] >= 64 && cComp[i] <= 90) ||
			   (cComp[i] >= 97 && cComp[i] <= 122)) 
			{
				
			}
			else{
				return FALSE;
			}
			//TRACE("(%c) \n", temp.GetAt(i));
		}
	}

	return TRUE;
}



BOOL UNI_CHAR::CheckUNIChar(CVJ cvj)
{
	BOOL rev = TRUE;
	if (cvj.C>=0 && cvj.C<19) {
		//TRACE("(%s) ", UniCodeC[cvj.C]);
	}
	else {
		//TRACE("(-) ");
		rev = FALSE;
	}

	if (cvj.V>=0 && cvj.V<21) {
		//TRACE("(%s) ", UniCodeV[cvj.V]);
	}
	else {
		//TRACE("(-) ");
		rev = FALSE;
	}

	if (cvj.J>=0 && cvj.J<28) {
		//TRACE("(%s) \n", UniCodeJ[cvj.J]);
	}
	else {
		//TRACE("(-) \n");
	}

	return rev;
}


UNI_CHAR::CVJ UNI_CHAR::UNIDivChar(char *pDivWord)
{
	CVJ Result; //��ȯ�� �ʼ� �߼� ���� ���� �ѱ� ����
	int cCode,vCode,jCode; //�����ڵ� �ѱۿ����� �ʼ�/�߼�/������ �ڵ尪
	char inputSyllable[3]; //�Ѿ�� ���ڸ� �����Ͽ� �̿�
	
	strncpy(inputSyllable,pDivWord,2);//�ܾ��� ù���ڸ� ����
	inputSyllable[2]=NULL;//strncpy�� NULL�� ������ �����Ƿ� ������ �־���
	
	//�����ڵ�� ��ȯ, �ϼ��� �ڵ带 ����ؾ��ϸ� �� �ּ���(�ڵ����̺� �ʿ�)
	int nLen = MultiByteToWideChar(CP_ACP, 0,inputSyllable, -1, NULL, NULL);
	LPWSTR lpszW = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, inputSyllable, -1, lpszW, nLen);
	
	
	cCode =(lpszW[0] - 0xAC00) / (588); //588=21*28(�ʼ�) 
	vCode =((lpszW[0] - 0xAC00) % (588)) / 28; //588=21*28(�߼�)
	jCode =(lpszW[0] - 0xAC00) % 28; //(����)
		
	Result.C=cCode;
	Result.V=vCode;
	Result.J=jCode;
	
	
	delete []lpszW;
	return Result;
}

