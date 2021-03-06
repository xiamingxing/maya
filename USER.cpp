// USER.cpp: implementation of the USER class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "USER.h"
#include "COM.h"
#include "CircularBuffer.h"
#include "BufferEx.h"
#include "Item.h"
#include "Hyper.h"
#include "Mcommon.h"
#include "Search.h"
#include "ServerDlg.h"
#include "Mcommon.h"
#include "UserManager.h"
#include "UNI_CHAR.h"
#include "RoyalRumble.h"

#include "EventZone.h"

#include "SharedQueue.h"
#include "ShopSystem.h"
#define _CHINA
#define D_WORD_XOR1MUL                          ((WORD)0x009d)
#define D_WORD_XOR3BASE                         ((WORD)0x086d)
#define D_WORD_XOR3MUL                          ((WORD)0x087b)
#define D_XORTABLE_LENGTH                       8
//char G_BYTE_DOII_XorTable[D_XORTABLE_LENGTH] = { 0x31, 0x54, 0x76, 0x58, 0x42, 0x22, 0x12, 0x53 };//标准日服
BYTE G_BYTE_DOII_XorTable[D_XORTABLE_LENGTH] = { 0x73,0x4F,0x3D,0x4E,0x99,0x66,0x88,0x11 };
//BYTE G_BYTE_DOII_XorTable[D_XORTABLE_LENGTH] = { 0x45,0x31,0x12,0x91,0x73,0x92,0x18,0x43 };//国服标准KEY


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Extern.h"		//	寇何函荐 急攫

extern CUserManager *g_pUQM;
extern CSearch *g_pUserList;
extern CServerDlg *g_pMainDlg;
extern TCHAR *g_szBabyName[];

// zinee 02/11/26 for Mail Agent
extern CSharedQueue		g_MailAgentQueue;

//extern CRITICAL_SECTION m_CS_ReceiveData[];
extern CRITICAL_SECTION m_CS_FileWrite;
extern CRITICAL_SECTION m_CS_EventItemLogFileWrite;
extern CPtrList RecvPtrList[];
extern long nRecvDataLength[];

// IKING 2002.1.
extern CRITICAL_SECTION m_CS_LoginData;
extern CPtrList RecvLoginData;
extern long nLoginDataCount;
extern CRITICAL_SECTION m_CS_LogoutData;
extern CPtrList RecvLogoutData;
extern long nLogoutDataCount;				
extern CString winName;	//PK大赛胜利者名字
extern CString ShaRenName;	//杀人狂大赛
extern DWORD ShaRenNum;
extern BOOL ShaRenEnd;

extern ShopSystem g_Shop;
extern CPtrList RecvSqlData;
extern CRITICAL_SECTION m_CS_SqlData;
extern long nSqlDataCount;	
extern int g_ChatEnable[];

extern CHATDATAPACKET *g_WaitRecvDataChat[DATA_BUF_SIZE_FOR_THREAD+1][AUTOMATA_THREAD+1];
extern int g_nChatDataHead[];
extern int g_nChatDataTail[];

extern BYTE g_ServerId;

extern TCHAR g_arServerIPAddr[16];
extern CString g_strARKRRWinner;
extern DWORD g_ThreadErr[];

extern CEventZone	g_QuestEventZone;

extern long	g_PotionViewOnOff;
//
/// 鞠龋拳 包访
// 霸烙 付促 促福霸..
//T_KEY private_key = 0x1234567890123456;
//T_KEY private_key = 0x4584263589548694;
//
//T_KEY private_key = 0xd9b4f1a524657845;
////////////////////////////////////////////////////////////////////// 
//	傈开函荐
//
int g_8x[]	= {-2, -1, 0, 1, 2, 1, 0, -1};
int g_8y[]	= {0, -1, -2, -1, 0, 1, 2, 1};
int g_16x[]	= {-4, -3, -2, -1, 0, 1, 2, 3, 4, 3, 2, 1, 0, -1, -2, -3};
int g_16y[]	= {0, -1, -2, -3, -4, -3, -2, -1, 0, 1, 2, 3, 4, 3, 2, 1};

POINT g_rPt[8][6] = 
{
	{{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {100, 100}, {100, 100}},
	{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}},
	{{0, -1}, {1, -1}, {0, 0}, {1, 0}, {100, 100}, {100, 100}},
	{{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {-1, 1}, {0, 1}},
	{{0, -1}, {1, -1}, {0, 0}, {1, 0}, {0, 1}, {1, 1}},
	{{-1, 0}, {0, 0}, {-1, 1}, {0, 1}, {100, 100}, {100, 100}},
	{{-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}},
	{{0, 0}, {1, 0,}, {0, 1}, {1, 1}, {100, 100}, {100, 100}},
};

//short		g_sHPConst[CLASS_NUM] = {6, 3, 4, 4}; // 拜捧, 瘤莆捞, 档八, 醚扁
//short		g_sPPConst[CLASS_NUM] = {1, 5, 3, 5};
//short		g_sSPConst[CLASS_NUM] = {2, 2, 2, 2};
//
//short		g_sHPLV[CLASS_NUM]	= {5, 3, 4, 3};
//short		g_sPPLV[CLASS_NUM]	= {1, 4, 2, 3};
//short		g_sSPLV[CLASS_NUM]	= {1, 1, 1, 1};
//
//short		g_sHPAdd[CLASS_NUM]	= {10, 30, 40, 35};
//short		g_sPPAdd[CLASS_NUM]	= {20, 10, 20, 20};
//short		g_sSPAdd[CLASS_NUM]	= {20, 25, 35, 25};

//新增仲裁
short		g_sHPConst[NEW_CLS_NUM] = {6, 3, 4, 4,4};	 // 各职业体质转生命值
short		g_sPPConst[NEW_CLS_NUM] = {1, 5, 3, 5,6};	 // 各职业智慧转精神值
short		g_sSPConst[NEW_CLS_NUM] = {2, 2, 2, 2,2};	 // 各职业体质转体力值

short		g_sHPLV[NEW_CLS_NUM]	= {5, 3, 4, 3,3};		//各职业级别转生命值
short		g_sPPLV[NEW_CLS_NUM]	= {1, 4, 2, 3,8};		//各职业级别加精神值
short		g_sSPLV[NEW_CLS_NUM]	= {1, 1, 1, 1,1};		//各职业级别加体力值

short		g_sHPAdd[NEW_CLS_NUM]	= {10, 30, 40, 35,35};	//各职业附加生命值
short		g_sPPAdd[NEW_CLS_NUM]	= {20, 10, 20, 20,30};	//各职业附加精神值
short		g_sSPAdd[NEW_CLS_NUM]	= {20, 25, 35, 25,25};	//各职业附加体力值

int			g_iAccessoriSlot[MAX_ACCESSORI] = {2, 3, 6, 7}; //首饰位置
int			g_iGuardianiSlot[GUARDIAN_SLOT] = {38};

extern short		g_sHPConst[NEW_CLS_NUM];
extern short		g_sPPConst[NEW_CLS_NUM];
extern short		g_sSPConst[NEW_CLS_NUM];

extern short		g_sHPLV[NEW_CLS_NUM];
extern short		g_sPPLV[NEW_CLS_NUM];
extern short		g_sSPLV[NEW_CLS_NUM];

extern short		g_sHPAdd[NEW_CLS_NUM];
extern short		g_sPPAdd[NEW_CLS_NUM];
extern short		g_sSPAdd[NEW_CLS_NUM];

extern CRoyalRumble	g_RR;
//////////////////////////////////////////////////////////////////////
// 各职业拳,法,剑,枪(可以装置的装备的class类别)16,31表示通用所有职业
//8是拳,4是法,2是剑,1是枪,16是仲裁
//10 拳,剑  衣裤
//21法,枪,仲裁  衣裤
//拳
int g_BrawlClass[USE_CLASS_TYPE] = {8,10,31};
//法
int g_StaffClass[USE_CLASS_TYPE] = {4,21,31};
//剑
int g_EdgedClass[USE_CLASS_TYPE] = {2,10,31};
//枪
int g_FireArmsClass[USE_CLASS_TYPE] = {1,21,31};
//仲裁
int g_JudgeClass[USE_CLASS_TYPE] = {16,21,31};

///////////////////////////////////////////////////////////////////////
//	概流窍绊 胶懦捞 辑肺 概莫捞 救登绢辑 概流加己阑 胶懦扁霖栏肺 牧滚飘茄促.
//								拜捧		瘤莆捞				档八			醚扁			  公拌凯
BYTE g_DynamicSkillInfo[MAX_SKILL_MAGIC_NUM] = 
{ 
	8,7,6,9,  0,
	13,12,10,11,  0,  
	16,15,14,17,  0, 
	21,19,18,20,  0, 
	0,0,0,0,0,
	25,26,27,28,};

BOOL g_bDebug = FALSE;

int	g_CityRank[5][3] =		// 殿鞭窍遏矫 皑家荐摹
{
	{{3200}, {2950}, {3800}},
	{{1800}, {2300}, {3800}},
	{{1100}, {1870}, {2700}},
	{{1333}, {1870}, {2600}},
	{{1333}, {1870}, {2600}},
};
							// 阿 殿鞭寸 穿利 鞘夸荐摹//大于-59413 为霸王  小于等于-36242为土匪
//int	g_CityRankValue[] = {-59413, -36242, -21899, -4112, -332, 0, 500, 4000, 26300, 43900, 64000,2000000000,2100000000};
int	g_CityRankValue[] = {-59413, -36242, -21899, -4112, -332, 0, 500, 4000, 26300, 43900, 64000,160000,210000};
					//林冈  瘤莆捞 档八  醚扁
//int	g_ClassWeight[] = {150,  100,   150,  130,  120};		// 负重
int	g_ClassWeight[] = {6500,  5000,   6500,  5000, 5000};

bool isDoubleExp = false; //服务器双倍
bool isDoubleExpup4 = false; //服务器4倍

BOOL  g_sanBaoLv = FALSE;

BOOL  g_sanJingYan = FALSE;

//int g_ItemHsUpgrade[] = {9000,8400,7600,6800,5700,4700,3200,1700,800,100};//幻石升级的成功率
int g_ItemHsUpgrade[] = {9500,8500,7000,5000,4000,3000,2000,1000,500,100};//幻石升级的成功率

int	g_HuanShiJiLv[] = {9500,8000,7000,6000,1/*3000*/}; //装备上打幻石的成功率
//武器
int g_ItemAttUpgrade[] = {10000, 10000, 9000, 6000, 4000, 2000, 1, 1, 1, 1};
//装备
int g_ItemDefUpgrade[] = {10000, 10000, 9000, 6000, 4000, 3000, 1, 1, 1, 1};
//百级
int g_ItemDefUpgrade_[] = {10000, 10000, 7000, 5000, 3000, 2500, 1500, 1000, 800,800,700,400,100,50,10};
//首饰
int g_ItemAccessoriUpgrade[] = {9500, 9500, 10000};//最后一位10000，成功率100%，钻石取消

int g_ItemBlessingUpgrade[] = {5000, 8500, 9500};
int g_ItemNormalDownUpgrade[] = {6000, 10000};
/***************************************
功能:转存数据包
返回值: 无返值
日期: 2006年12月19日
版本:1.00
****************************************/
void SaveDump(char* lpTarget, int len)
{
	
	int i;
	
	FILE *fp = fopen("c:\\send_rev_dump.log", "a+");
	if(fp != NULL) 
	{
		for(i=0;i<len;i++){
			fprintf(fp, _T("%2.2x "), (unsigned char)lpTarget[i]);       //   用十六进制的形式存储.   
		}
		fprintf(fp, "\n"); 
		fclose(fp);
	}
	
}
/***************************************
功能:初始化KEY
返回值: 无返值
日期: 2006年12月19日
版本:1.00
*****************************************/
void USER::init_encode(__int64 key2)
{
	int i;
	char *p;
	p=(char*)&key2;
	for(i=0; i<D_XORTABLE_LENGTH; i++){
    	key[i] = p[i] ^ G_BYTE_DOII_XorTable[i];
   }
}

/***************************************
功能:封包加密AND解密
返回值: 无返值
日期: 2006年12月19日
版本:1.00
*****************************************/
void USER::Encode_Decode(char* lpTarget, char* lpSource, int nLen,int f)
{
//memcpy(lpTarget,lpSource,nLen);
//	return ;
	BYTE	l_BYTE_Xor1 = (WORD)nLen * D_WORD_XOR1MUL, l_BYTE_Xor3;

	WORD	l_WORD_Xor3 = D_WORD_XOR3BASE, nCir;


	for (nCir=0; nCir<(WORD)nLen; nCir++) 
	{
		l_BYTE_Xor3    = HIBYTE(l_WORD_Xor3);
		lpTarget[nCir] = lpSource[nCir] ^ l_BYTE_Xor1 ^ key[nCir % D_XORTABLE_LENGTH] ^ l_BYTE_Xor3;
		l_WORD_Xor3   *= D_WORD_XOR3MUL;
	}
// 	if(f==1)
// 		SaveDump(lpSource,nLen);
// 	else
// 		SaveDump(lpTarget,nLen);
	
}

  //////////////////////////////////////////////////////////////////////////////////////////////////////
//	秦寸 鸥涝俊 措茄 惑怕捞惑 矫埃 沥焊甫 努府绢 茄促.
//
void USER::ClearAbnormalTime(DWORD dwType)
{
	DWORD dwCurrTime = GetTickCount();

	switch(dwType)
	{
	case ABNORMAL_TIME:
		m_tAbnormalKind = ABNORMAL_BYTE_NONE;
		m_dwAbnormalTime = 0;
		m_dwLastAbnormalTime = dwCurrTime;

		ClearAbnormalGroup(ABNORMAL_CLEAR_STATUS);
		break;

	case PSI_ONE_TIME:
		m_dwLastHasteTime	= dwCurrTime;			// 付瘤阜栏肺 庆捞胶飘啊 吧赴矫埃
		m_dwLastShieldTime	= dwCurrTime;			// 付瘤阜栏肺 蒋靛甫 敬 矫埃
		m_dwLastDexUpTime	= dwCurrTime;			// 付瘤阜栏肺 DEX UP 阑 敬 矫埃
		m_dwLastMaxHPUpTime	= dwCurrTime;			// 付瘤阜栏肺 MAX HP UP 阑 敬 矫埃

		m_dwLastFastRunTime			= dwCurrTime;	// 付瘤阜栏肺 菩胶飘繁阑 敬 矫埃
		m_dwLastMindShockTime		= dwCurrTime;	// 付瘤阜栏肺 付牢靛 碱农甫 敬 矫埃
		m_dwLastPsiShieldTime		= dwCurrTime;	// 付瘤阜栏肺 荤捞坷葱蒋靛甫 敬矫埃
		m_dwLastPiercingShieldTime = dwCurrTime;	// 付瘤阜栏肺 乔绢教蒋靛甫 敬 矫埃
		m_dwLastBigShieldTime       = dwCurrTime;

		m_dwHasteTime		= 0;					// Haste
		m_dwShieldTime		= 0;					// Shield
		m_dwDexUpTime		= 0;					// Dex Up
		m_dwWISUpTime		= 0;					// 智力 Up
        m_dwXiShengTime		= 0;				// 牺牲护法
		m_dwMaxHPUpTime		= 0;					// Max HP Up

		m_dwFastRunTime			= 0;				// Fast Run
		m_dwMindShockTime		= 0;				// Mind Shock
		m_dwPsiShieldTime		= 0;				// Psionic Shield
		m_dwPiercingShieldTime	= 0;				// Piercing Shield
		m_dwBigShieldTime   = 0;


		
		ClearAbnormalGroup(ABNORMAL_CLEAR_PSI_ONE);
		if(m_bNecklaceOfShield) AddAbnormalInfo(ABNORMAL_SHIELD);	// 荐龋狼 格吧捞

		break;

	case PSI_TWO_TIME:
		m_dwLastAdamantineTime	= dwCurrTime;	// 付瘤阜栏肺 Adamantine阑 敬 矫埃
		m_dwLastMightyWeaponTime= dwCurrTime;	// 付瘤阜栏肺 MightyWeapon阑 敬 矫埃
		m_dwLastBerserkerTime	= dwCurrTime;	// 付瘤阜栏肺 Berserker甫 敬 矫埃
		
		m_dwAdamantineTime		= 0;			// Adamantine
		m_dwMightyWeaponTime	= 0;			// Mighty Weapon
		m_dwBerserkerTime		= 0;			// Berserker

		ClearAbnormalGroup(ABNORMAL_CLEAR_PSI_TWO);
		break;

	case PSI_THREE_TIME:
		m_dwLastMindGuardTime	= dwCurrTime;	// 付瘤阜栏肺 Mind Guard 甫 敬矫埃
		m_dwMindGuardTime		= 0;			// Mind Guard
		
		m_dwFANTAnTime		= 0;					// 超级反弹时间
		m_dwFENGLiTime		= 0;					// 超级锋利时间
		m_dwYINENgTime		= 0;					// 超级异能时间
		m_dwDUOCHONgTime		= 0;					// 超级多重时间
		m_dwDUOCHONgTime		= 0;					// 超级多重时间

        ClearAbnormalGroup(ABNORMAL_CLEAR_PSI_THREE);
		break;
    case TRANS:
		m_dwLastTransTime = dwCurrTime;
		m_dwTransTime = 0;

		m_dwLastLhslTime = dwCurrTime;//灵魂锁链时间
		m_dwLhslTime = 0;//灵魂锁链时间
		ClearStateGroup(ABNORMAL_CLEAR_STATUS1);
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction ...
//
USER::USER()
{	
	m_pCB		= new CCircularBuffer(20000);		// Socket俊辑 Data甫 罐酒辑 阶酒滴绰 滚欺
	m_pCBCore	= new CCircularBuffer(20000);		// Socket俊辑 罐篮 Data吝俊 蜡瓤茄 菩哦阑 榜扼郴绢 历厘窍绰 滚欺

	m_pSharedMemory = NULL;
	m_pMD			= NULL;

	// IKING 2002.1.
	m_server		= 0;
	m_dwLastDownExp	= 0;
	m_bPseudoString = FALSE;//yskang 0.1 龋莫[局莫]捞 啊楼牢瘤 柳楼牢瘤 备盒^^
	m_nGuildUserInfoType = 0x00;//yskang 0.2
	ZeroMemory(m_strLoveName,sizeof(m_strLoveName));

	InitializeCriticalSection(&m_CS_ExchangeItem);
	InitializeCriticalSection(&m_CS_ShopItem);
	JiaoXiMiMa[0]=0;
}

USER::~USER()
{
	int i = 0;
	EnterCriticalSection( &m_CS_ExchangeItem );
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)
	{
		if( m_arExchangeItem[i] )
		{
			delete m_arExchangeItem[i];
			m_arExchangeItem[i] = NULL;
		}
	}
	m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ExchangeItem );

	EnterCriticalSection( &m_CS_ShopItem );
	for(i = 0; i < m_arShopItem.GetSize(); i++)
	{
		if( m_arShopItem[i] )
		{
			delete m_arShopItem[i];
			m_arShopItem[i] = NULL;
		}
	}
	m_arShopItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ShopItem );

	if(m_pCB) delete m_pCB;
	if(m_pCBCore) delete m_pCBCore;

	DeleteCriticalSection(&m_CS_ExchangeItem);
	DeleteCriticalSection(&m_CS_ShopItem);
}

int USER::Init( int bufCreateFlag )
{
	CBSocket::Init(bufCreateFlag);

	m_ConnectionSuccessTick = 0;

	if ( m_Sid == -1 )
	{
		m_Sid = 0;
		m_iModSid = AUTOMATA_THREAD;
		m_uid = m_Sid;
	}
	else
	{
		m_iModSid = m_Sid % AUTOMATA_THREAD;
		m_uid = m_Sid;
	}
	
	m_isCryptionFlag = 0;
	m_Recv_value = 0;
	m_Send_value = 0;
	m_public_key = 0;

	SetPublicKey();
	init_encode(m_public_key);
	/*m_cryptfn.SetPublicKey(m_public_key );	
	m_cryptfn.SetPrivateKey(private_key);*/

	// 辑摹 备炼 檬扁拳 窍扁...
	if ( m_uid >= 0 )
		g_pUserList->SetUserUid( m_uid,this );

	m_Recv_value = 0;
	m_UserStatus = USER_JUST_CONNECTED;
	m_pCom = g_pMainDlg->GetCOM();
	m_UserFlag = FALSE;

	InitUser();

	m_ConnectionSuccessTick = GetTickCount();

	// 粮 change IP, Port...
	m_strZoneIP = "";
	m_nZonePort	= -1;
	


	return 1;
}

BOOL USER::InitUser()
{
	// Compress Init
	memset( m_CompBuf, NULL, 10240 );		// 拘绵且 单捞磐甫 葛栏绰 滚欺
	m_iCompIndex = 0;						// 拘绵且 单捞磐狼 辨捞
	m_CompCount = 0;						// 拘绵且 单捞磐狼 俺荐

	// Memory DB Data Init
	m_pSharedMemory = NULL;
	m_pMD = NULL;

	// Speed Hack Check 函荐 檬扁拳
	m_iTickCount = 0;

	m_bSpeedHackEnble = FALSE;//新加变速检测

	m_dwServerTickBasic = 0;
	m_dwServerTick = 0;
	m_dwClientTick = 0;


	m_ZoneIndex = 0;
	m_bWhisper	= TRUE;						// 庇富 倾侩
	m_bLive		= USER_DEAD;				// Live Or Dead

	m_NowZoneChanging = FALSE;				// 粮 眉牢瘤且锭 TRUE;

	m_bBuddyMode = FALSE;					// Buddy Off
//	m_bTradeMode	= FALSE;				// Trade Off
	m_bTradeWaiting = FALSE;				// Trade 夸没饶 搬苞甫 扁促府绰 吝
	m_bNowTrading	= FALSE;				// 泅犁 芭贰吝
	m_bExchangeOk	= FALSE;				// Exchange 铰倡
	m_bTradeItem	= FALSE;				// Item阑 棵妨初澜(X)
	m_iTradeUid		= -1;

	m_bPShopOpen	= FALSE;				// Personal shop open or closed
	m_bPShopView	= FALSE;				// Personal shop being viewed
	m_iPShopViewuid = -1;					// Personal shop view uid
	m_bViewingAShop	= FALSE;				// User viewing a personal shop

	m_bNoItemMove	= FALSE;

	m_bRefuse		= FALSE;				// 扁夯 可记篮 酒捞袍 罐扁 
	m_iMaxWeight	= 0;					// 甸荐乐绰 公霸
	m_iCurWeight	= 0;
        
	isGuildRen = false;
	m_dwGuild = -1;							// 辨靛 牢郸胶	
	m_sGuildVersion = -1;					// 辨靛 巩剧 滚怜
	m_bGuildMaster = FALSE;					// 辨靛 付胶磐
	m_bRepresentationGuild = FALSE;			// 鼻茄 措青 咯何	 
	::ZeroMemory(m_strGuildName, sizeof(m_strGuildName)); // 辨靛 捞抚

	m_bFieldWarApply = FALSE;
	m_FieldWarGMUid = 0;					// 鞘靛傈老锭 惑措规 辨付 uid
	m_dwFieldWar = 0;						// 鞘靛傈 矫累捞搁 惑措祈 辨靛 锅龋啊 甸绢柯促.

	::ZeroMemory(m_strChar, sizeof(m_strChar));
	::ZeroMemory(m_strTradeUser, sizeof(m_strTradeUser));
	::ZeroMemory(m_PersonalShopName, sizeof(m_PersonalShopName));

	m_sTempSTR = 0;					
	m_sTempCON = 0;					
	m_sTempDEX = 0;					
	m_sTempINT = 0;					
	m_sTempVOL = 0;					
	m_sTempWIS = 0;	
	
	m_tTempClass = 255;

	m_nHavePsiNum = 0;						// 焊蜡 荤捞坷葱 荐
	m_tDir = 0;								// 蜡历啊 焊绊 乐绰 规氢
	m_tIsOP = 0;							// 款康磊牢瘤 魄窜
	m_bLogOut = FALSE;							// Logout 吝牢啊 魄窜
	m_bZoneLogOut = FALSE;

	int i = 0;										// 馆拜胶懦 贸府甫 困秦 檬扁拳		
	for(i = 0; i < COUNTERATTACK_MAX_NUM; i++)
	{
		m_CounterAttackList[i].iDamage = 0;
		m_CounterAttackList[i].iNpcUid = -1;
		m_CounterAttackList[i].lSid = 0;
		m_CounterAttackList[i].dwLastAttackTime = GetTickCount();
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++) // 芭贰吝 牢亥俊 官差绰 酒捞袍 荐甫 眉农困秦
	{ 
		m_TradeItemNum[i].sSid = -1; 
		m_TradeItemNum[i].sNum = 0; 
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++) // 酒捞袍 浇吩甫 檬扁拳
	{ 
		m_UserItem[i].sLevel = 0;			 // 捞 酒捞袍阑 荤侩且荐 乐绰 饭骇
		m_UserItem[i].sSid = -1; 
		m_UserItem[i].sCount = 0;
		m_UserItem[i].sBullNum = 0;
		m_UserItem[i].sDuration = -1;
		m_UserItem[i].tIQ = 0;
		for(int j = 0; j < MAGIC_NUM; j++) m_UserItem[i].tMagic[j] = 0;
		m_UserItem[i].iItemSerial = 0;
	}

	m_bMakeBuddy = FALSE;			// 炉牢瘤 眉农 
	m_bNowBuddy = FALSE;			// 泅犁 滚叼吝牢瘤
	for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
	{
		m_MyBuddy[i].uid = -1;
		::ZeroMemory(m_MyBuddy[i].m_strUserID, sizeof(m_MyBuddy[i].m_strUserID));		
	}

	for(i = 0; i < MAX_GUILD_USER; i++)
	{
		m_MyGuildUser[i].sLevel = 0;	// 立加吝牢 辨靛盔
		::ZeroMemory(m_MyGuildUser[i].GuildUser, sizeof(m_MyGuildUser[i].GuildUser));		
	}

	for(i = 0; i < MAGIC_COUNT; i++)		// 概流 浇吩阑 檬扁拳
	{
		m_DynamicUserData[i] = 0;
	}

	for(i = 0; i < EBODY_MAGIC_COUNT; i++)	// EBody 概流 浇吩 檬扁拳
	{	
		m_DynamicEBodyData[i] = 0;
	}

	for(i = 0; i < HUANSHI_MAGIC_COUNT; i++)	// EBody 概流 浇吩 檬扁拳
	{	
		m_DynamicMagicItem[i] = 0;
	}
    
	for(i = 0; i < MS_MAGIC_COUNT; i++)	// 程序卡
	{	
		m_DynamicSuit[i] = 0;
	}

	InitTelList(m_strTeleport);
	InitEventSlot();

//	m_bLive = USER_LIVE;
	m_nCharNum = 0;

	DWORD dwCurrTime = GetTickCount();

	m_dwLastSpeedTime = dwCurrTime;
	m_dwLastMoveAndRun = dwCurrTime;
	m_dwLastAttackTime = dwCurrTime;
	m_dwLastPsiAttack = dwCurrTime;
	m_dwLastMaxAttack = dwCurrTime;
	m_dwCastDelay = 0;
	m_dwLastTimeCount = dwCurrTime;
	m_dwLastAbnormalTime = dwCurrTime;
	m_dwLastHPTime = dwCurrTime;
	m_dwLastPsiDamagedTime = 0;
    m_dwLastUseSpecialTime = 0;//限制疯狂加血
	m_dwLastHasteTime = dwCurrTime;
	m_dwLastShieldTime = dwCurrTime;
	m_dwLastDexUpTime = dwCurrTime;
	m_dwLastMaxHPUpTime = dwCurrTime;
	m_dwLastBigShieldTime = dwCurrTime;
	m_dwLastCallHuFaTime = dwCurrTime;
	m_dwLastXiShendTime = dwCurrTime;
	m_dwLastWISUpTime = dwCurrTime;
	m_dwLastFANTAnTime = dwCurrTime;
	m_dwLastFENGLiTime = dwCurrTime;
	m_dwLastYINENgTime = dwCurrTime;
	m_dwLastDUOCHONgTime = dwCurrTime;

	m_dwLastFastRunTime			= dwCurrTime;
	m_dwLastMindShockTime		= dwCurrTime;
	m_dwLastMindGuardTime		= dwCurrTime;
	m_dwLastPsiShieldTime		= dwCurrTime;
	m_dwLastPiercingShieldTime	= dwCurrTime;

	m_dwLastUpdateTime = dwCurrTime;	//
	m_dwLastSaveTime   = dwCurrTime;

	//m_dwBSTime=0;			//狼熊变的时间
    m_dwShTsTime=0;
	m_isDoubleExp=0;
	m_isDoubleBAOLV=0;
	m_ShouhuTime=dwCurrTime;
	//m_dwBSLaseTime=dwCurrTime;// 狼熊变的开始时间
    m_dwShTsLaseTime=dwCurrTime;// 守护天使时间
	m_isDoubleExpLase=dwCurrTime;// 3倍经验时间
	m_isDoubleBAOLVLase=dwCurrTime;//3倍掉宝时间
	m_dwLastItem44Time = dwCurrTime;
	//显视攻击目标血量
	m_ShowHP=0;
	m_Bmws=0; //避免误伤
    m_lRequireNum = 0;
	m_ionlineTime = dwCurrTime; //上线时间。挂机AKR
	m_dwShopTime = dwCurrTime;
	
	m_tAbnormalKind		= 0;
	m_dwAbnormalTime	= 0;

	m_dwHasteTime		= 0;
	m_dwShieldTime		= 0;
	m_dwDexUpTime		= 0;
	m_dwWISUpTime		= 0;
	m_dwFANTAnTime		= 0; //超级反弹
	m_dwFENGLiTime		= 0; //超级锋利
	m_dwYINENgTime		= 0; //超级异能
	m_dwDUOCHONgTime		= 0;
	m_dwXiShengTime		= 0;
	m_dwMaxHPUpTime		= 0;
	m_dwAdamantineTime	= 0;
	m_dwMightyWeaponTime= 0;
	m_dwBerserkerTime	= 0;

	m_dwFastRunTime			= 0;
	m_dwMindShockTime		= 0;
	m_dwMindGuardTime		= 0;
	m_dwPsiShieldTime		= 0;
	m_dwPiercingShieldTime	= 0;
	m_dwBigShieldTime   = 0;

	m_dwLastHiExpTime		= dwCurrTime;
	m_dwLastHtExpTime		= dwCurrTime;
	m_dwLastMagicFindTime	= dwCurrTime;
	m_dwLastMagicFtTime		= dwCurrTime;
	m_dwLastNoChatTime		= dwCurrTime;
    m_dwLastCloseTime		= dwCurrTime;
	m_dwLastAutoMoney		= dwCurrTime;
	m_dwLastLingQu  		= dwCurrTime;
	m_dwLastGuarDianTianShi = dwCurrTime;
	m_dwLastVIPTime			= dwCurrTime;
	m_dwLastUPZaiXianTime   = dwCurrTime;
	m_dwLastBFindTime		= dwCurrTime;
	m_dwLastBHTime		    = dwCurrTime;
	m_dwLastSJTime		= dwCurrTime;

	m_dwHiExpTime		= 0;	// 版氰摹 2硅
	m_dwHtExpTime		= 0;//幻想灵石
	m_dwMagicFindTime	= 0;	// 概流蛮胶 5硅
	m_dwMagicFtTime		= 0;
	m_dwNoChatTime		= 0;	// 盲陛
    m_dwCloseTime	    = 0;
	m_dwAutoMoney	    = 0;
	m_dwLingQu  	    = 0;
	m_dwGuarDianTianShi = 0;//守护天使
	m_dwShopPingDN	= 0;
	m_dwVIPTime	        = 0;
	m_dwZaiXianTime	    = 0;
	m_dwUPZaiXianTime	= 60*1000;
	m_dwBFindTime	    = 0;
	m_dwBHTime	    = 0;
	m_dwSJTime	    = 0;
	m_dwTransTime = 0;//狂暴机甲时间
    m_dwLhslTime = 0;//灵魂锁链时间
	m_MItemLock	= FALSE;////交易密码
	
	 m_dwGuardianTime = 60000; //机甲耐久时间 1分
//	m_dwRandNum = 0; //抽奖钥匙0次
	m_dwLastShoutChatTime	= dwCurrTime + 6 * 1000;	//最后一次呐喊时间

	m_dwNoDamageTime		= 0;
	m_dwLastNoDamageTime	= dwCurrTime;

    m_PKStart	= FALSE;  //PK竞赛是否开始
	m_PKWin		= FALSE;  //PK胜利

	m_dwAbnormalInfo	= ABNORMAL_NONE;
	m_dwAbnormalInfo_	= ABNORMAL_NONE;

	m_state = STATE_DISCONNECTED;

	m_ItemFieldInfoCount = 0;
	memset( m_ItemFieldInfoBuf, NULL, 8192 );
	m_ItemFieldInfoIndex = 0;

	InitUserItem();
	InitGuildItem();			// 函荐甫 柄昌捞 技殴茄促.
	InitAccountBankItem();		// 函荐甫 柄昌捞 技殴茄促.

	m_bPkStatus = FALSE;		// 泪矫悼救 墨坷惑怕牢瘤甫 汲沥
	m_bPkKeyDown = FALSE;		// 泪矫悼救 墨坷惑怕牢瘤甫 汲沥
	m_dwPkStatusTime = 0;		// 墨坷己氢老锭 瘤加 矫埃甫 魄沥
	m_iCityValue = 0;			// 泅犁 穿利 己氢蔼
	m_sCityRank = 0;			// 矫刮 殿鞭
	m_sKillCount = 0;			// PK穿利 冉荐

	m_sMagicSTR = 0;				// 概流加己苞 E_Boady蔼俊狼茄 函拳等蔼
	m_sMagicCON = 0;
	m_sMagicDEX = 0;
	m_sMagicVOL = 0;
	m_sMagicWIS = 0;

	m_sMagicMaxHP = 0;
	m_sMagicMaxPP = 0;
	m_sMagicMaxSP = 0;
	m_Hit=0;
	m_Avoid=0;
	m_SKill=0;
//	m_tPsiAbnormal = 0;

	EnterCriticalSection( &m_CS_ExchangeItem );
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)
	{
		if(m_arExchangeItem[i] != NULL)
		{
			delete m_arExchangeItem[i];
			m_arExchangeItem[i] = NULL;
		}
	}
	m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ExchangeItem );
	
	EnterCriticalSection( &m_CS_ShopItem );
	for(i = 0; i < m_arShopItem.GetSize(); i++)
	{
		if ( m_arShopItem[i] != NULL )
		{
			delete m_arShopItem[i];
			m_arShopItem[i] = NULL;
		}
	}
	m_arShopItem.RemoveAll();

	LeaveCriticalSection( &m_CS_ShopItem );

	for( i = 0; i < m_arItemLog.GetSize(); i++ )
	{
		if( m_arItemLog[i] )
		{
			delete m_arItemLog[i];
		}
	}
	m_arItemLog.RemoveAll();

	m_dwSaintTime = 0;
	m_dwSaintTimeCount = GetTickCount();

	m_lDeadUsed = 0;

	//家葛己 咀辑辑府侩 函荐 檬扁拳
	m_bRingOfLife		= 0;
	m_bNecklaceOfShield	= 0;
	m_bEarringOfProtect	= 0;
        m_bCBOff     	= 0;
	m_dwFANTAnTime		= 0; //超级反弹
	m_dwFENGLiTime      = 0; //超级锋利
	m_dwYINENgTime      = 0; //超级异能
	m_dwDUOCHONgTime		= 0;
	//宠物
	m_tBabyCall = 0;					//初始未生宝宝
	m_sBabyID = 0;
	m_sBabyLevel = 0;
	m_sBabyStatus = 0;
	m_sBabyNormal = 0;
	m_nBabyExp = 0;
	m_nBabyNextLevelExp = 0;
	m_sFealty = 0;
	m_sMaxHungry = 0;
	m_sHungry = 0;
	m_tBabySkill[0] = 0;
	m_tBabySkill[1] = 0;
	m_tBabySkill[2] = 0;
	m_tBabySkill[3] = 0;
		//新职业
		m_dwJudgePsiState = JUDGE_PSI_NONE;
		m_dwJudgePsiExState = JUDGE_PSI_NONE;
		m_tHuFaType = 0;					//护法类型
		m_dwLastCallHuFaTime = 0;			//上次护法召唤时间
		m_dwHuFaLastAttack = 0;				//上次护法召唤时间
		m_dwLevealHuFaTime = 0;				//护法剩余时间
		m_dwWISUpTime = 0;					//智力上升剩余时间
		m_dwXiShengTime = 0;				//牺牲剩余时间
		m_nHuFaMaxHP = 0;
		m_bSessionOnline = false;
        m_nHuFaHP = 0;
		m_xHuFa = -1;
	    m_yHuFa = -1;
		//m_bSessionOnline = false;

       return TRUE;
}

void USER::InitEventSlot()
{
	for( int i = 0; i < m_arEventNum.GetSize(); i++)
	{
		if( m_arEventNum[i] )
		{
			delete m_arEventNum[i];
			m_arEventNum[i] = NULL;
		}
	}
	m_arEventNum.RemoveAll();
}

void USER::InitUserItem()
{
	int i, j;

	for(i = 0; i < TOTAL_ITEM_NUM; i++) // 酒捞袍 浇吩甫 檬扁拳
	{ 
		m_UserItem[i].sLevel = 0;	
		m_UserItem[i].sSid = -1; 
		m_UserItem[i].sCount = 0;
		m_UserItem[i].sBullNum = 0;
		m_UserItem[i].sDuration = 0;
		m_UserItem[i].tIQ = 0;
		for(j = 0; j < MAGIC_NUM; j++) m_UserItem[i].tMagic[j] = 0;
		m_UserItem[i].iItemSerial = 0;
	}

	for(i = 0; i < MAGIC_COUNT; i++)
	{
		m_DynamicUserData[i] = 0;
	}

	for(i = 0; i < EBODY_MAGIC_COUNT; i++)	// EBody 概流 浇吩 檬扁拳
	{	
		m_DynamicEBodyData[i] = 0;
	}

	for(i = 0; i < HUANSHI_MAGIC_COUNT; i++)	// EBody 概流 浇吩 檬扁拳
	{	
		m_DynamicMagicItem[i] = 0;
	}

	for(i = 0; i < MS_MAGIC_COUNT; i++)	// 程序卡
	{	
		m_DynamicSuit[i] = 0;
	}
}

void USER::InitUserBankItem()
{
	int i, j;
	
	m_dwBankDN = 0;

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++) // 篮青 酒捞袍 浇吩甫 檬扁拳
	{ 
		m_UserBankItem[i].sLevel = 0; 
		m_UserBankItem[i].sSid = -1; 
		m_UserBankItem[i].sCount = 0;
		m_UserBankItem[i].sBullNum = 0;
		m_UserBankItem[i].sDuration = 0;
		m_UserBankItem[i].tIQ = 0;
		for(j = 0; j < MAGIC_NUM; j++) m_UserBankItem[i].tMagic[j] = 0;

		m_UserBankItem[i].iItemSerial = 0;
	}
}

void USER::InitAccountBankItem()
{
	int i, j;
	
	m_dwAccountBankDN = 0;

	for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++) // 拌沥 篮青 酒捞袍 浇吩甫 檬扁拳
	{ 
		m_AccountBankItem[i].sLevel = 0; 
		m_AccountBankItem[i].sSid = -1; 
		m_AccountBankItem[i].sCount = 0;
		m_AccountBankItem[i].sBullNum = 0;
		m_AccountBankItem[i].sDuration = 0;
		m_AccountBankItem[i].tIQ = 0;
		for(j = 0; j < MAGIC_NUM; j++) m_AccountBankItem[i].tMagic[j] = 0;

		m_AccountBankItem[i].iItemSerial = 0;
	}
}

void USER::InitGuildItem()
{
	int i, j;
	
	m_dwGuildDN = 0;

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++) // 篮青 酒捞袍 浇吩甫 檬扁拳
	{ 
		m_GuildItem[i].sLevel = 0; 
		m_GuildItem[i].sSid = -1; 
		m_GuildItem[i].sCount = 0;
		m_GuildItem[i].sBullNum = 0;
		m_GuildItem[i].sDuration = 0;
		m_GuildItem[i].tIQ = 0;
		for(j = 0; j < MAGIC_NUM; j++) m_GuildItem[i].tMagic[j] = 0;

		m_GuildItem[i].iItemSerial = 0;
	}
}

//////////////////////////////////////////////////////////////////////
//	Inline Fn...
//

USER* USER::GetUser(int uid)
{
	if( uid < 0 || uid + USER_BAND >= NPC_BAND ) return NULL;

	//if(uid >= MAX_USER)
	//{
	//	TRACE("######### USER::GetUser() : Invalid UID = %d\n" , uid);
	//	return NULL;
	//}

	return m_pCom->GetUserUid(uid);
}

USER* USER::GetUser(TCHAR* id)
{
	if(!strlen(id)) return NULL;
	
	CString szSource = id;
	USER *pUser = NULL;

	for( int i = 0; i < MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if ( pUser->m_state == STATE_DISCONNECTED || pUser->m_state == STATE_LOGOUT ) continue;

		if ( !szSource.CompareNoCase(pUser->m_strUserID) ) return pUser;
	}

	return NULL;
}
/*
inline int USER::GetUid(int x, int y )
{
	MAP* pMap = g_zone[m_ZoneIndex];
	return pMap->m_pMap[x][y].m_lUser;
}

inline BOOL USER::SetUid(int x, int y, int id)
{
	MAP* pMap = g_zone[m_ZoneIndex];

	if(!pMap) return FALSE;
	if(pMap->m_pMap[x][y].m_bMove != 0) return FALSE;
	if(pMap->m_pMap[x][y].m_lUser != 0 && pMap->m_pMap[x][y].m_lUser != id ) return FALSE;
/*
	long lUid = m_uid + USER_BAND;
	long temp = pMap->m_pMap[x][y].m_lUser;

	if(InterlockedCompareExchange(&pMap->m_pMap[x][y].m_lUser, (LONG)0, lUid) == lUid)
	{
		long tt = pMap->m_pMap[x][y].m_lUser;
		return TRUE;
	}
*/
/*	pMap->m_pMap[x][y].m_lUser = id;

	return TRUE;
}
*/
//-------------------------------------------------------------------------------//

CNpc* USER::GetNpc(int nid)
{
	if( nid < 0 || nid >= g_arNpc.GetSize() ) return NULL;

	return g_arNpc[nid];
}

CNpc* USER::GetNpc(TCHAR* pNpcName)
{
	CNpc* pNpc = NULL;

	int nSize = g_arNpc.GetSize();

	for( int i = 0; i < g_arNpc.GetSize(); i++)
	{
		pNpc = g_arNpc[i];
		if( !pNpc ) continue;

		if( _tcscmp(pNpc->m_strName, pNpcName) == 0)
		{
			return pNpc;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
//	Communication Part
int USER::NoEncodeSend(int length, char *pBuf)
{
	if ( m_SockFlag != 1 ) return 0;

	int rv = CBSocket::B_Send( length, pBuf );

	if ( m_SockFlag == 0 || rv == -2 )
	{
		//SockCloseProcess();
		return rv;
	}

	return length;
}

void USER::Send(TCHAR *pBuf, int nLength)
{
	char pTBuf[RECEIVE_BUF_SIZE+1];

	if ( m_SockFlag != 1 ) return;

	if ( nLength > PACKET_DATA_SIZE ) return;

	if(nLength >= SEND_BUF_SIZE - 6)
	{
		TRACE("#### Too Big Send Data!!! (%s, %d, size = %d)\n", __FILE__, __LINE__, nLength);
		return;
	}

//	::ZeroMemory(pTBuf, SEND_BUF_SIZE);
	TCHAR tibuf[SEND_BUF_SIZE+1];
	::ZeroMemory(pTBuf, SEND_BUF_SIZE);
	::ZeroMemory(tibuf, SEND_BUF_SIZE);

	int index = 0;
	MYSHORT slen;

	pTBuf[index++] = (char) PACKET_START1;		// 菩哦 矫累阑 舅府绰 2 Byte
	pTBuf[index++] = (char) PACKET_START2;

//	slen.i = nLength;
	if ( m_isCryptionFlag == 1 )
	{
		m_Send_value++;							// 菩哦俊 老访 锅龋甫 何咯茄促.
		DWORD nSendOrder = m_Send_value;
		m_Send_value &= 0x00ffffff;
		nSendOrder &= 0x00ffffff;

		memcpy( tibuf, &nSendOrder, sizeof(DWORD) );
		memcpy( &tibuf[4], pBuf, nLength );
		nLength += sizeof(DWORD);

		slen.i = nLength + 1;						//鞠龋拳咯何甫 唱鸥郴绰 目盖靛 器窃
	}
	else slen.i = nLength;

	pTBuf[index++] = (char) (slen.b[0]);		// 菩哦 辨捞甫 Short(2Byte)肺 角绢 焊辰促
	pTBuf[index++] = (char) (slen.b[1]);		// 2Byte扼绊 窍咯 1Byte * 256 + 2Byte啊 酒丛

//	memcpy(pTBuf + index, pBuf, nLength);
	// 鞠龋拳...
	if ( m_isCryptionFlag == 1 )
	{
		pTBuf[index++] = ENCRYPTION_PKT;
		memcpy(pTBuf + index, tibuf, nLength);
#ifdef _CHINA
		Encode_Decode((char *)pTBuf + index, (char *)tibuf, nLength,1);
#else
		//m_cryptfn.JvEncryptionFast( nLength, (BYTE*)tibuf, (BYTE*)pTBuf + index );
#endif
	}
	else
	{
		memcpy(pTBuf + index, pBuf, nLength);
	}
	
	index += nLength;

	pTBuf[index++] = (char) PACKET_END1;		
	pTBuf[index++] = (char) PACKET_END2;


	NoEncodeSend(index, pTBuf);

}

/*
void USER::ProcessData()
{
	if ( m_UserStatus == 0 && m_UserFlag == FALSE ) return;

	if(!m_BufCount) return;

	int len = 0;
	BOOL bEncryption = FALSE;

	m_pCB->PutData(m_pBuf, m_BufCount);		// 罐篮 Data甫 滚欺俊 持绰促

	while (PullOutCore(len, bEncryption))
	{
		if( len <= 0 ) break;

		Parse( len, m_RData, bEncryption );

		bEncryption = FALSE;
	}
}
*/

void USER::ProcessData(char *pIBuf, int length)
{
	if ( m_UserStatus == 0 && m_UserFlag == FALSE ) return;

	if(!length) return;

	int len = 0;
	BOOL bEncryption = FALSE;

	m_pCB->PutData(pIBuf, length);		// 罐篮 Data甫 滚欺俊 持绰促

	while (PullOutCore(len, bEncryption))
	{
		if( len <= 0 ) break;

		Parse( len, m_RData, bEncryption );

		bEncryption = FALSE;
	}
}

BOOL USER::PullOutCore(int &length, BOOL &bEncryption)
{
	BYTE		*pTmp;
	int			len = 0, sPos = 0, ePos = 0;
	BOOL		foundCore = FALSE;
	MYSHORT		slen;

	len = m_pCB->GetValidCount();

	if(len <= 0 || len >= RECEIVE_BUF_SIZE) return FALSE;

	pTmp = new BYTE[len];
	m_pCB->GetData((char*)pTmp, len);

	for (int i = 0; i < len && !foundCore; i++)
	{
		if (i + 2 >= len) break;

		if (pTmp[i] == PACKET_START1 && pTmp[i + 1] == PACKET_START2)
		{
			sPos = i + 2;

			slen.b[0] = pTmp[sPos];
			slen.b[1] = pTmp[sPos + 1];

			length = (int)slen.i;

			if (length <= 0) goto cancelRoutine;
			if (length > len || length > CORE_BUFF_SIZE) goto cancelRoutine;

			if((BYTE)pTmp[sPos + 2] == ENCRYPTION_PKT)
			{
				bEncryption = TRUE;
			}

			ePos = sPos + length + 2;
			if((ePos + 2) > len ) goto cancelRoutine;

			if (pTmp[ePos] == PACKET_END1 && pTmp[ePos+1] == PACKET_END2)
			{
				::ZeroMemory(m_RData, CORE_BUFF_SIZE+1);

				if(bEncryption) 
#ifdef _CHINA
					Encode_Decode((char *)m_RData, (char *)(pTmp+sPos+3), length -1,0);
#else
					//m_cryptfn.JvDecryptionFast( length -1, (BYTE *)(pTmp+sPos+3), (BYTE *)m_RData );
#endif
				else				::CopyMemory((void *)m_RData, (const void *)(pTmp+sPos+2), length);
//				if(bEncryption) 	::CopyMemory((void *)m_RData, (const void *)(pTmp+sPos+3), length - 1);
//				else				::CopyMemory((void *)m_RData, (const void *)(pTmp+sPos+2), length);

				m_RData[length] = 0;

				foundCore = TRUE;
				break;
			}
			else
			{
				m_pCB->HeadIncrease(3);
				break;
			}
		}
	}
	if (foundCore) m_pCB->HeadIncrease(6 + length); //6: header 2+ end 2+ length 2

	delete[] pTmp;

	return foundCore;

cancelRoutine:
	if(pTmp) delete[] pTmp;
	return foundCore;
}
/*
BOOL USER::PullOutCore(char *&data, int &length)
{
	BYTE		*pTmp;
	int			len = 0, sPos = 0, ePos = 0;
	BOOL		foundCore = FALSE;
	MYSHORT		slen;

	len = m_pCB->GetValidCount();

	if(len == 0 || len < 0)
	{
		data = NULL;
		return FALSE;
	}

	pTmp = new BYTE[len];
	m_pCB->GetData((char*)pTmp, len);

	for (int i = 0; i < len && !foundCore; i++)
	{
		if (i + 2 >= len) break; 

		if (pTmp[i] == PACKET_START1 && pTmp[i + 1] == PACKET_START2)
		{
			sPos = i + 2;

			slen.b[0] = pTmp[sPos];
			slen.b[1] = pTmp[sPos + 1];

			length = (int)slen.i;

			if (length <= 0) goto cancelRoutine;
			if (length > len) goto cancelRoutine;
			ePos = sPos + length + 2;

			if((ePos + 2) > len ) goto cancelRoutine;

			if (pTmp[ePos] == PACKET_END1 && pTmp[ePos+1] == PACKET_END2)
			{
				data = new char[length+1];
				::CopyMemory((void *)data, (const void *)(pTmp+sPos+2), length);
				data[length] = 0;
				foundCore = TRUE;
				break;
			}
			else 
			{
				data = NULL;
				m_pCB->HeadIncrease(3);
				break;
			}
		}
	}
	if (foundCore) m_pCB->HeadIncrease(6 + length); //6: header 2+ end 2+ length 2

	delete[] pTmp;

	return foundCore;

cancelRoutine:
	data = NULL;
	delete[] pTmp;
	return foundCore;
}
*/
//void USER::Parse(int len, TCHAR *pBuf)
void USER::Parse(int len, TCHAR *pBuf, BOOL &bEncryption)
{
/*	int index = 0;
	BYTE command = GetByte(pBuf+ index, index);
\ 
	UserTimer();
*/
	 
	int index = 0;
	DWORD tempVer = 0;
	DWORD dwValue = 0;

	BYTE command = 0;

	if(bEncryption)
	{
		dwValue = GetDWORD(m_RData + index, index);

		tempVer = dwValue - m_Recv_value;

		if(tempVer > 100) return;		// 菩哦 鞠龋拳 烙矫 抛胶飘侩烙

		m_Recv_value = dwValue;
	}

	command = GetByte(m_RData, index);
	//心跳直接返回
	if(CLIENT_TIMER == command)
		return ;
	if(command == 0)
		bEncryption = TRUE;

	 UserTimer();
	// Before Game Start...
	if(command == CHECK_ALIVE_REQ) CheckServerTest();
	else if(command == ENCRYPTION_START_REQ) EncryptionStartReq(pBuf + index);
	else if(command == ANNOUNCE_NOTICE) AnnounceNotice(pBuf + index);

	///////////////////////////
		CBufferEx TempBuf;
		//TempBuf.Add(SET_SPEED_MONSTER);
		//TempBuf.Add(m_uid + USER_BAND);
		//TempBuf.Add(150); //移动速度
		//SendInsight(TempBuf, TempBuf.GetLength());
		//SightRecalc();
  
	 ////////////////////////
	if( !bEncryption ) return;		// 鞠龋拳啊 救登绢 乐栏搁 公矫..


	g_ThreadErr[m_iModSid] = command;

	if(command != CLIENT_TICK) 
	{
		m_dwNoDamageTime = 0;
		m_dwLastNoDamageTime = GetTickCount();
	}
	//if(CLIENT_TIMER == 0x06)
	//	return;
	// Before Game Start...
	switch( command )
	{
//	case SESSION_LOGIN_REQ:
//		SessionLoginReq(pBuf + index);
//		break;
	//case 0:
//		GameStart_(pBuf + index+14);
	//	break;
	
//	case NEW_ACCOUNT_REQ:
//		NewAccountReq(pBuf + index);
//		break;
	
	case ACCOUNT_LOGIN_REQ:
		AccountLoginReq(pBuf + index);
//		AccountLoginReqWithThread(pBuf+index);
//		AccountLoginReqWithDbServer(pBuf+index);
		break;

	case NEW_CHAR_REQ:
		NewCharReq(pBuf + index);
//		NewCharReqWithThread(pBuf + index);
		break;

	case CHECK_ID_REQ:
		CheckIDReq(pBuf + index);
		break;

	case REROLL_REQ:
		RerollReq(pBuf + index);
		break;

	case DELETE_CHAR_REQ:
		DeleteCharReq(pBuf + index);
//		DeleteCharReqWithThread(pBuf + index);
		break;

	case GAME_START_REQ:
		GameStart(pBuf + index);
//		GameStartWithThread(pBuf + index);
//		GameStartWithDbServer(pBuf + index);
		break;

	case ZONE_LOGIN_REQ:
		ZoneLoginReq(pBuf + index);
		break;

	case LOGOUT_REQ:
		SoftClose();
//		LogOut();
		break;

	case CLIENT_TICK:
		if ( g_ChatEnable[m_iModSid] == TRUE )
			CheckSpeedHack();
		return;
		break;

//	case CHECK_ALIVE_REQ:			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 抛胶飘 内靛烙
//		CheckServerTest();
//		break;
//	case ANNOUNCE_NOTICE:
//		AnnounceNotice(pBuf + index);
//		break;
	default:
		break;
	}

	if( g_bShutDown )
	{
		SoftClose();
		return;
	}

	// Game Start...
	switch( command )
	{
	/*	case PALYER_ONLINE_REQ:
		if(m_bLive == USER_DEAD) return;		// 酒贰狼 菩哦篮 蜡历啊 混酒乐阑 版快父 贸府
		if(m_bNowTrading == TRUE) { SendSystemMsg("交易状态下无法启动离线挂机",SYSTEM_ERROR,TO_ME); return;} 		// Trade 吝俊绰 谍窿阑 给茄促.
		if(m_bNoItemMove == TRUE) return;		// 酒捞袍阑 框流阑 荐 绝绰 惑怕捞搁 府畔
		if(m_bViewingAShop == TRUE) return;		// A User is viewing another users shop
		if(m_state != STATE_GAMESTARTED) return;
		if( m_bZoneLogOut ) return;
		if( m_bNowBuddy == TRUE ){ SendSystemMsg("组队状态下无法启动离线挂机",SYSTEM_ERROR,TO_ME); return;} 

		if(IsCity()== false )
		{
			SendSystemMsg("请在安全区域启动离线挂机功能！",SYSTEM_ERROR,TO_ME);

			break;
		}
		m_bSessionOnline = true;
			SendSystemMsg("成功启动离线挂机功能，如需取消离线请重新登陆一次游戏", SYSTEM_SPECIAL, TO_ME);

		break;*/
		
	case CHAT_REQ:
//		if ( g_ChatEnable[m_iModSid] == TRUE )	ChatReq(pBuf + index);
		if ( g_ChatEnable[m_iModSid] == TRUE ) ChatReqWithThread(len-1, pBuf + index);
		break;

	case LIVE_REQ:
		if(m_bPShopOpen == TRUE) return;

		LiveReq(pBuf + index);
		break;

	case REVIVAL_REQ://神之泪..复活
		if(m_bPShopOpen == TRUE) return;

		RevivalReq( pBuf + index );
		break;

	case USERINFO_REQ:
		if(m_bPShopOpen == TRUE) return;

		UserInfoReq(pBuf + index);
		break;

	case CLIENT_TIMER:					// 老窜 SP甫 雀汗茄促.
//		UserTimer();
//		return;
		break;

	case ITEM_TRADE_REQ:
		if(m_bPShopOpen == TRUE) return;

		ItemTradeReq(pBuf + index);
		break;

	case EXCHANGE_REQ:
		if(m_bPShopOpen == TRUE) return;

		ExchangeReq(pBuf + index);
		break;

	case RESTART_REQ:
		RestartReq(pBuf + index);
//		RestartReqWithThread(pBuf + index);
		break;

	case HELP_REQ:
		HelpStatusOpenReq();
		break;

	case SCREEN_CAPTURE_REQ:
		ScreenCaptureReq( );
		break;

	case EXPRESSION_REQ:
		if(m_bPShopOpen == TRUE) return;

		ExpressionReq( pBuf + index );
		break;

	case EXPRESSION_OPEN:
		if(m_bPShopOpen == TRUE) return;

		ExpressionOpen( );
		break;

	case PERSONAL_SHOP_REQ:    ///打开商店请求
		if(m_bNowTrading == TRUE) return;		// 
		if(m_bNoItemMove == TRUE) return;//
		PersonalShopReq( pBuf + index );
		break;


//	case SERVER_TICK_REQ:
//		SendServerTick();
//		break;

	default:
		break;
	}

	if(m_bLive == USER_DEAD) return;		// 酒贰狼 菩哦篮 蜡历啊 混酒乐阑 版快父 贸府
	if(m_bNowTrading == TRUE) return;		// Trade 吝俊绰 谍窿阑 给茄促.
	if(m_bNoItemMove == TRUE) return;		// 酒捞袍阑 框流阑 荐 绝绰 惑怕捞搁 府畔
	if(m_bPShopOpen == TRUE) return;		// User has Personal shop open
	if(m_bViewingAShop == TRUE) return;		// A User is viewing another users shop
	if(m_state != STATE_GAMESTARTED) return;
	if( m_bZoneLogOut ) return;
	//////////////////////////////////////////////////////
		
	if(m_PersonalShopName[0] = '\0')
	{
			m_bPShopView = 0;
			m_iPShopViewuid = -1;
			m_bPShopOpen = FALSE; // Set FALSE because User now has no personal shop

			EnterCriticalSection( &m_CS_ShopItem );
			for(int i = 0; i < m_arShopItem.GetSize(); i++)
			{
				if ( m_arShopItem[i] != NULL )
				{
					delete m_arShopItem[i];
					m_arShopItem[i] = NULL;
				}
			}
			m_arShopItem.RemoveAll();
			LeaveCriticalSection( &m_CS_ShopItem );

			::ZeroMemory(m_PersonalShopName, sizeof(m_PersonalShopName));
	}
///////////////////////////////////////////////////////////////////////////////////////////////


	switch( command )
	{
	case CLIENT_EVENT:
		ClientEvent(pBuf+index);
		break;

	case BANK_ITEM_MOVE_REQ:
        if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE) return;  //个人仓库
		BankItemMoveReq(pBuf + index);
		break;

	case BUY_ITEM:
		BuyItem(pBuf + index);
		break;

	case BUY_PSY_REQ:
		BuyPsi(pBuf + index);
		break;

	case SELL_ITEM:
		SellItem(pBuf + index);
		break;

	case REPAIR_ITEM:
		RepairItem(pBuf + index);
		break;

	case MOVE_CHAT_REQ:
		MoveChatReq(pBuf + index);
		break;

	case CHANGE_DIR_REQ:
		ChangeDir(pBuf + index);
		break;

	case MOVE_FIRST_REQ:
		MoveFirstReq(pBuf + index);
		break;
	case MOVE_REQ:
		MoveReq(pBuf + index);
		break;
	case MOVE_END_REQ:
		MoveEndReq(pBuf + index);
		break;

	case RUN_FIRST_REQ:
		RunFirstReq(pBuf + index);
		
	///////////////////////////
		
		TempBuf.Add(SET_SPEED_MONSTER);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add(130); //移动速度
		SendInsight(TempBuf, TempBuf.GetLength());
		SightRecalc();
   
	 ////////////////////////
		
		break;
	case RUN_REQ://移动请求
		if(m_dwLhslTime != 0 && (!IsCity()))
			{
				SendEventMsg( "灵魂锁链状态下(5秒)无法移动.");
				return;
		}		
		RunReq(pBuf + index);
		
	///////////////////////////
		
		//TempBuf.Add(SET_SPEED_MONSTER);
		//TempBuf.Add(m_uid + USER_BAND);
		//TempBuf.Add(150); //移动速度
		//SendInsight(TempBuf, TempBuf.GetLength());
		//SightRecalc();	
   
	 ////////////////////////
		break;
	case RUN_END_REQ:
		RunEndReq(pBuf + index);
			
	///////////////////////////
		
		//TempBuf.Add(SET_SPEED_MONSTER);
		//TempBuf.Add(m_uid + USER_BAND);
		//TempBuf.Add(130); //移动速度
		//SendInsight(TempBuf, TempBuf.GetLength());
		//SightRecalc();
   
	 ////////////////////////
		break;
	
	case FIND_USER_REQ:
		FindUserReq(pBuf + index);
		break;

	case BBS_NEXT:
		BBSNext(pBuf + index);
//		BBSNextWithThread(pBuf + index);
		break;

	case BBS_READ:
		BBSRead(pBuf + index);
//		BBSReadWithThread(pBuf + index);
		break;

	case BBS_WRITE:
		BBSWrite(pBuf + index);
//		BBSWriteWithThread(pBuf + index);
		break;

	case BBS_EDIT:
		BBSEdit(pBuf + index);
//		BBSEditWithThread(pBuf + index);
		break;

	case BBS_DELETE:
		BBSDelete(pBuf + index);
//		BBSDeleteWithThread(pBuf + index);
		break;

	case PSI_ATTACK_REQ:
		if(m_dwCloseTime > 0) return; //坐牢不能使用魔法
		PsiAttack(pBuf + index);
		break;

	case ATTACK_REQ:
		Attack(pBuf + index);
		break;

	case TRADE_MODE_REQ:
		SetTradeMode(pBuf + index);
		break;

	case ITEM_MOVE_REQ: //移动装备或者物品 穿得装备等.
		ItemMoveReq(pBuf + index);
		break;

	case ITEM_GIVE_REQ:
		ItemGiveReq(pBuf + index);
		break;

	case ITEM_LOAD_REQ:
		ItemLoadReq(pBuf + index);
		break;

	case ITEM_USE_REQ:
		ItemUseReq(pBuf + index);
		break;

	case CHANGE_WEAPON_REQ:
		ChangeWeaponReq(pBuf + index);
		break;

	case SKILL_ONOFF_REQ:
		SkillOnOffReq(pBuf + index);
		break;

	case PSI_CHANGE_REQ:
		PsiChangeReq(pBuf + index);
		break;

	case USER_STATUS_SKILL_UP_REQ:
		UserStatusSkillUp(pBuf + index);
		break;

	case BUDDY_MODE_REQ: 
		SetBuddyMode(pBuf + index);
		break;

	case UPGRADE_ITEM_REQ:
//		SendSystemMsg( IDS_USER_UNDER_CONSTRUCTION, SYSTEM_NORMAL, TO_ME);
//		UpgradeItemReq(pBuf + index);
		if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
	{
		SendSystemMsg("请先解除密码保护系统!",SYSTEM_ERROR,TO_ME);
		return;
	}
		UpgradeItemReqBegin(pBuf + index);
		break;

	case GUILD_OPEN_REQ:
		NewGuild(pBuf + index);
//		NewGuildWithThread(pBuf + index);
		break;

	case GUILD_WAREHOUSE_CLOSE:
		GuildWharehouseCloseReq();
		break;

	case GUILD_ADD_REQ:
		GuildAddReq(pBuf + index);
		break;

	case GUILD_MARK_ADD_REQ:
		GuildMarkAddReq(pBuf + index);
//		GuildMarkAddReqWithThread(pBuf + index);
		break;

	case GUILD_ITEM_MOVE_REQ:
		GuildItemMoveReq(pBuf + index);
//		GuildItemMoveReqWithThread(pBuf + index);
		break;

	case GUILD_USER_INFO_REQ:
		GuildUserInfoReq(pBuf + index);
		break;

	case GUILD_TAX_REQ:
		GetStoreTax(pBuf + index);
		break;

	case GUILD_WAR_REQ:
		GetGuildWarScheme(pBuf + index);
		break;

	case GUILD_WAR_APPLY_REQ:
		GuildWarApplyReq(pBuf + index);
		break;

	case GUILD_FIELD_WAR_RESULT:
		SendGuildWarFieldApplyResult(pBuf + index);
		break;

	case GUILD_HOUSE_APPLY_REQ:
		MassZoneMove(pBuf + index);
		break;

	case TELEPORT_REQ:
		TeleportReq();
		break;

	case TELEPORT_EDIT_REQ: 
		TelportEdit(pBuf + index);
		break;

	case TELEPORT_COPY_REQ:
		TeleportCopyReq(pBuf + index);
		break;

	case TELEPORT_COPY_RESULT:
		TeleportCopyResult(pBuf + index);
		break;

	case EVENT_UPGRADE_ITEM_REQ:
		EventUpgradeItemReq(pBuf + index);
		break;

	case ADDRESS_WINDOW_OPEN:
	case ADDRESS_WINDOW_OPEN_NEW:
		RecvAddress( pBuf + index );
		break;

	case PRE_REG_USER_EVENT_REQ:
		RecvEventItemSerial( pBuf + index );
		break;

	case PRE_REG_GAMEROOM_EVENT_REQ:
		RecvEventItemSerialForGameRoom( pBuf + index );
		break;

	case ACCOUNT_BANK_ITEM_MOVE_REQ:  //综合仓库
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE) return;
		AccountBankItemMoveReq( pBuf + index );
		break;

	case DRESSING:
		m_bNoItemMove = TRUE;
		DressingReq( pBuf + index );
		m_bNoItemMove = FALSE;
		break;

	case SELECT_OVER100_SKILL:
		SelectOver100Skill( pBuf+index );
		break;

	case GUILD_INVITE_REQ:
		SendGuildInviteReq( pBuf+index );
		break;

	case GUILD_INVITE_RESULT:
		GuildInviteResult( pBuf+index );
		break;

	case QUESTWINDOW_OPEN_REQ://ctrl+Q打开任务窗口请求
	//	QuestWindowOpenReq( pBuf + index);
		NpcXING();//打开5009SAY
		break;

	case LOVE_NAME_REQ://--yskang 0.1
		LoveName(pBuf+index);
		break;

	// Mail system by zinee 02-11-15
	case MAIL_INFO_REQ:
		MailInfoReq( pBuf+index );
		break;

	case MAIL_SEND_REQ:
		MailSendReq( pBuf+index );
		break;

	case MAIL_LIST_REQ:
		MailListReq( pBuf+index );
		break;

	case MAIL_CONTENTS_REQ:
		MailContentReq( pBuf+index );
		break;

	case MAIL_DELETE_REQ:
		MailDeleteReq( pBuf+index );
		break;

	case EBODY_IDENTIFY_REQ:
		EBodyIdentifyReq( pBuf + index );
		break;

	case PK_BUTTON_REQ:
		TogglePKButton( pBuf + index );
		break;

	case TRANSFORM_REQ:
		TransformReq( pBuf + index  );
		break;
	case 68: //化妆变身
		Face_painting(pBuf + index,len);
		break;
	//case 30: //光电板
	//	DianGuangBan(pBuf + index);
	//	break;
		
	default:
		break;
	}
	//TempBuf.Add(SET_SPEED_MONSTER);
	//	TempBuf.Add(m_uid + USER_BAND);
	//	TempBuf.Add(130); //移动速度
	//	SendInsight(TempBuf, TempBuf.GetLength());
	//	SightRecalc();
}


//////////////////////////////////////////////////////////////////////
//化妆变身 m_iSkin m_iHair m_sGender m_strFace
//////////////////////////////////////////////////////////////////////
void USER::Face_painting(TCHAR *pBuf,int len)
{
	int index=0,i;
	byte sGender;

	int slot=GetShort(pBuf, index); //使用物品位置
	if(m_UserItem[slot].sSid!=1000 && m_UserItem[slot].sSid!=1004 && m_UserItem[slot].sSid!=1018)
		return ;
	for(i=0;i<10;i++){
		m_strFace[i]=GetByte(pBuf, index);
	}
	sGender=GetByte(pBuf, index);
	if(sGender==1){
		m_sGender=m_sGender+1;
		if(m_sGender>1)
			m_sGender=0;
	}
	
	SendMyInfo(TO_INSIGHT, INFO_MODIFY);

	CBufferEx	TempBuf;
	TempBuf.Add((byte)68);
	Send(TempBuf, TempBuf.GetLength());

	CBufferEx	TempBuf1;
	ReSetItemSlot(&m_UserItem[slot]);
	TempBuf1.Add(ITEM_GIVE_RESULT);
	TempBuf1.Add((BYTE)0x1);
	TempBuf1.Add((BYTE)slot);
	TempBuf1.Add(m_UserItem[slot].sLevel);
	TempBuf1.Add(m_UserItem[slot].sSid);
	TempBuf1.Add(m_UserItem[slot].sDuration);
	TempBuf1.Add(m_UserItem[slot].sBullNum);
	TempBuf1.Add(m_UserItem[slot].sCount);
	for(int j = 0; j < MAGIC_NUM; j++) TempBuf1.Add(m_UserItem[slot].tMagic[j]);
	TempBuf1.Add(m_UserItem[slot].tIQ);
	Send(TempBuf1, TempBuf1.GetLength());

}
//////////////////////////////////////////////////////////////////////
//电光板
//////////////////////////////////////////////////////////////////////
void USER::DianGuangBan(TCHAR *pBuf)
{
//	SaveDump(pBuf,len);
	/*CBufferEx	TempBuf,TempBuf1;
	int index=1;
	

	int slot=GetShort(pBuf, index); //使用物品位置
	if(slot<10||slot>34)
		return ;
	if(m_UserItem[slot].sSid!=1021 ||m_UserItem[slot].sDuration<=0)
		return ;
	SendDuration(slot, 1);
	if(m_UserItem[slot].sDuration<=0){
		ReSetItemSlot(&m_UserItem[slot]);
		TempBuf1.Add(ITEM_GIVE_RESULT);
		TempBuf1.Add((BYTE)0x1);
		TempBuf1.Add((BYTE)slot);
		TempBuf1.Add(m_UserItem[slot].sLevel);
		TempBuf1.Add(m_UserItem[slot].sSid);
		TempBuf1.Add(m_UserItem[slot].sDuration);
		TempBuf1.Add(m_UserItem[slot].sBullNum);
		TempBuf1.Add(m_UserItem[slot].sCount);
		for(int j = 0; j < MAGIC_NUM; j++) TempBuf1.Add(m_UserItem[slot].tMagic[j]);
		TempBuf1.Add(m_UserItem[slot].tIQ);
		Send(TempBuf1, TempBuf1.GetLength());
	//	SaveDump(TempBuf1,TempBuf1.GetLength());

	}
	TempBuf.Add((byte)0x1f);
	TempBuf.Add((byte)1);
	TempBuf.Add((byte)0x25);
	TempBuf.Add((DWORD)0);
	TempBuf.AddString(m_strUserID);
	TempBuf.AddString(pBuf+4);
	
	SendAll(TempBuf, TempBuf.GetLength());*/

    int index = 0;
	short tSlot = GetShort(pBuf, index);
	if(tSlot < 10 || tSlot > 33) return;
	int dgnum = g_arDGArray.GetSize();
	int len = strlen(pBuf);
	if(len > 45) return;
	if(m_UserItem[tSlot].sSid != 1021) return;
	if(dgnum < 200)
	{
		CString strDG = _T("");
		TCHAR *dginfo = pBuf;
		DGbanInfo *DGB = new DGbanInfo;
		if (DGB == NULL) return;
		strcpy(DGB->m_strUserID,m_strUserID);	
		strcpy(DGB->m_DGBAN_BODY, dginfo+3);		
		g_arDGArray.Add(DGB);
		strDG.Format("你的消息排在第%d条",dgnum+1);
		SendSystemMsg(strDG.GetBuffer(strDG.GetLength()), SYSTEM_NORMAL, TO_ME);
		SendDGDuration(1021,tSlot);
	}
	else
	{ 
		SendSystemMsg( "电光板最多200人排队！", SYSTEM_NORMAL, TO_ME);
	}


}

///////////////////////////////////////////////////////////////////////////////////
//	Session DB俊 Account 啊 乐绰瘤 犬牢窍绰 Stored Proc
//
BOOL USER::CheckSessionLoginReq(TCHAR *strAccount, TCHAR *strPassword, int& nCount)
{
	nCount = 0;
	
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	BOOL bQuerySuccess = TRUE;
	
	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LoginReq (\'%s\', \'%s\',?)}"), strAccount, strPassword);
	
	SQLSMALLINT	sCount = 0;
	SQLINTEGER sCountInd;

	hstmt = NULL;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
//		g_DBSession[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLBindParameter(hstmt,1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sCount, 0, &sCountInd);
	if (retcode!=SQL_SUCCESS) 
	{
//		g_DBSession[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);
			if (retcode !=SQL_SUCCESS && retcode !=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
		}
	}
	else if (retcode==SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
	}
	else if (retcode==SQL_NO_DATA)
	{
		bQuerySuccess = FALSE;
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

	if( !bQuerySuccess ) 
	{
//		BREAKPOINT();
		g_DBSession[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	nCount = sCount;

	g_DBSession[m_iModSid].ReleaseDB(db_index);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//	捞固 鞍篮 Account 肺 立加茄 蜡历啊 乐绰瘤 八荤茄促.
//
BOOL USER::IsDoubleAccount(char *account,USER **pTempUser)

{
	if( !strlen( account ) ) return TRUE;

	CString strSource = account;

	// IKING 2001.1.
	USER *pUser = NULL;
	for( int i = 0; i < MAX_USER; i++)
	{
		if(i == m_uid) continue;

		pUser = m_pCom->GetUserUid(i);
		if( pUser == NULL ) continue;

		if( !strSource.CompareNoCase(pUser->m_strAccount))
		{
			if( pUser->m_state != STATE_DISCONNECTED && pUser->m_state != STATE_LOGOUT)
			{
				if(pTempUser !=NULL)
				{
					*pTempUser = pUser;
				}

				pUser->SendSystemMsg( IDS_USER_DOUBLE_ACCOUNT, SYSTEM_SPECIAL, TO_ME);				
				pUser->SoftClose();
				return TRUE;
			}
		}
	}
	//

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//	Session 俊 Login 沁阑 锭狼 贸府
//
void USER::SessionLoginReq(TCHAR *pBuf)
{
	int			index = 0, nCount;
	TCHAR		strAccount[ACCOUNT_LENGTH+1], strPassword[PASSWORD_LENGTH+1];
	BYTE		result = FAIL, error_code = 0;
	CBufferEx	TempBuf;

	::ZeroMemory(strAccount, sizeof(strAccount));
	::ZeroMemory(strPassword, sizeof(strPassword));

	if(!GetVarString(sizeof(strAccount), strAccount, pBuf, index)) return;
	if(!GetVarString(sizeof(strPassword), strPassword, pBuf, index)) return;
	
	if( IsDoubleAccount( strAccount ) )
	{
		TRACE( "DOUBLE ACCOUNT - %s\n", strAccount );
		error_code = ERR_3;
		goto result_send;
	}

	if(CheckSessionLoginReq(strAccount, strPassword, nCount) == FALSE)
	{
		error_code = UNKNOWN_ERR;
		goto result_send;
	}
	
	if(nCount == 0) result = SUCCESS;
	else error_code = (BYTE)nCount;

result_send:

	TempBuf.Add(SESSION_LOGIN_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		SoftClose();
	}
	else Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////
//	Session 俊 New Account 甫 父电促.
//
void USER::NewAccountReq(TCHAR *pBuf)
{
	int		index = 0, nRet = -1;
	BYTE	result = FAIL;

	TCHAR	szUserID	[12 + 1];
	TCHAR	szPassword	[12 + 1];
	TCHAR	szRealName	[40 + 1];
	TCHAR	szSocNo		[15 + 1];
	TCHAR	szAddress	[80 + 1];
	TCHAR	szTel		[20 + 1];
	TCHAR	szEmail		[30 + 1];
	TCHAR	szQuiz01	[50 + 1];
	TCHAR	szAnswer01	[50 + 1];

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];
	BOOL			bQuerySuccess = TRUE;
	CDatabase*		pDB = NULL;
	int				db_index = 0;
	SQLSMALLINT		sRet = 0;
	SQLINTEGER		sRetInd;

	if(!GetVarString(12, szUserID,			pBuf, index)) goto result_send;
	if(!GetVarString(12, szPassword,		pBuf, index)) goto result_send;
	if(!GetVarString(40, szRealName,		pBuf, index)) goto result_send;
	if(!GetVarString(15, szSocNo,			pBuf, index)) goto result_send;
	if(!GetVarString(80, szAddress,			pBuf, index)) goto result_send;
	if(!GetVarString(20, szTel,				pBuf, index)) goto result_send;
	if(!GetVarString(30, szEmail,			pBuf, index)) goto result_send;
	if(!GetVarString(50, szQuiz01,			pBuf, index)) goto result_send;
	if(!GetVarString(50, szAnswer01,		pBuf, index)) goto result_send;	
	
	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{? = call NewAccount (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')}"), 
		szUserID, szPassword, szRealName, szSocNo, szAddress, szTel, szEmail, szQuiz01, szAnswer01);
	
	hstmt = NULL;
	
	pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		//g_DBSession[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}

	retcode = SQLBindParameter(hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &sRetInd);
	if (retcode!=SQL_SUCCESS) 
	{
		g_DBSession[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);
			if (retcode !=SQL_SUCCESS && retcode !=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
		}
	}
	else if (retcode==SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBSession[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}
	else if (retcode==SQL_NO_DATA)
	{
		bQuerySuccess = FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

	if(bQuerySuccess) nRet = sRet;

	g_DBSession[m_iModSid].ReleaseDB(db_index);

result_send:
		
	if(bQuerySuccess == TRUE && nRet == 0) result = SUCCESS;

	index = 0;
	SetByte(m_TempBuf, NEW_ACCOUNT_RESULT, index);
	SetByte(m_TempBuf, result, index);
	Send(m_TempBuf, index);
}

///////////////////////////////////////////////////////////////////////////////////
//	Session 俊辑 牢刘累诀阑 芭摹绊 某腐磐甫 急琶窍扁 困秦 Login
//
void USER::AccountLoginReq(TCHAR *pBuf)
{
/*	if( g_bShutDown ) return;

	int		index = 0;
	BYTE	result = FAIL, error_code = 0;
	int		old_index = 0;
	TCHAR	szTemp[8096];

	TRACE("AccountLoginReq Request...Check");

#ifdef _GETVARSTRING_DEBUG
	char strFn[128];
	sprintf( strFn, "AccountLoginReq1" );
	int	nIDLength = GetVarString(m_strAccount, pBuf, sizeof(BYTE), index, strFn);
#endif
#ifndef _GETVARSTRING_DEBUG
	int	nIDLength = GetVarString(m_strAccount, pBuf, sizeof(BYTE), index);
#endif

	if(nIDLength == 0 || nIDLength > ACCOUNT_LENGTH)
	{
		error_code = 1;
		goto result_send;
	}

	if(IsDoubleAccount(m_strAccount))
	{
		error_code = ERR_2;
		goto result_send;
	}

	if(!LoadCharData(m_strAccount))
	{
		// Load Character Data Fail...
		error_code = UNKNOWN_ERR;
		goto result_send;
	}
	else
	{
		m_state = STATE_CONNECTED;
		result = SUCCESS;
	}

result_send:

	index = 0;
	SetByte(m_TempBuf, ACCOUNT_LOGIN_RESULT, index );
	SetByte(m_TempBuf, result, index );

	old_index = index;
	::CopyMemory(szTemp, m_TempBuf, old_index);

	if(result == SUCCESS)
	{
		SetByte(m_TempBuf, (BYTE)m_nCharNum, index);
	}
	else
	{
		SetByte(m_TempBuf, error_code, index);
		Send( m_TempBuf, index );
		SoftClose();
		return;
	}

	if(m_nCharNum != 0 && result == SUCCESS)
	{
		for(int i = 0; i < 3; i++)
		{
			if(m_strChar[i][0])
			{
				SetByte(m_TempBuf, (BYTE)i, index);
				if(!SendCharInfo(m_strChar[i], m_TempBuf, index))
				{
					result = FAIL;
					error_code = UNKNOWN_ERR;
					index = 0;
					SetByte(m_TempBuf, ACCOUNT_LOGIN_RESULT, index );
					SetByte(m_TempBuf, result, index );
					SetByte(m_TempBuf, error_code, old_index);
					Send(m_TempBuf, old_index);
					SoftClose();
					return;
				}
			}
		}
	}

//	UpdateCurrentUserTable();				// 技记俊 辑滚 IP甫 喀但茄促.

	Send(m_TempBuf, index);
*/
	if( g_bShutDown ) return;

	int		index = 0;
	BYTE	result = FAIL, error_code = 0;
	int		old_index = 0;
	TCHAR	szTemp[8096];

//	TRACE("AccountLoginReq Request...Check");

	int	nIDLength = GetVarString(ACCOUNT_LENGTH + 1, m_strAccount, pBuf, index);

	if(nIDLength == 0 || nIDLength > ACCOUNT_LENGTH)	
	{
		error_code = 1;
		goto result_send;
	}

	if(IsDoubleAccount(m_strAccount))
	{
		error_code = ERR_2;
		goto result_send;
	}

	if(!LoadCharData(m_strAccount))
	{
		// Load Character Data Fail...
		error_code = UNKNOWN_ERR;
		goto result_send;
	}
	else 
	{
		m_state = STATE_CONNECTED;
		result = SUCCESS;
	}

result_send:

	index = 0;
	SetByte(m_TempBuf, ACCOUNT_LOGIN_RESULT, index );
	SetByte(m_TempBuf, result, index );

	old_index = index;
	::CopyMemory(szTemp, m_TempBuf, old_index);

	if(result == SUCCESS)
	{
		SetByte(m_TempBuf, (BYTE)m_nCharNum, index);
	}
	else
	{
		SetByte(m_TempBuf, error_code, index);
		Send( m_TempBuf, index );
		SoftClose();
		return;
	}

	if(m_nCharNum != 0 && result == SUCCESS)
	{
		for(int i = 0; i < 3; i++)
		{
			if(m_strChar[i][0])	
			{
				SetByte(m_TempBuf, (BYTE)i, index);
				if(!SendCharInfo(m_strChar[i], m_TempBuf, index))
				{
					result = FAIL;
					error_code = UNKNOWN_ERR;
					index = 0;
					SetByte(m_TempBuf, ACCOUNT_LOGIN_RESULT, index );
					SetByte(m_TempBuf, result, index );
					SetByte(m_TempBuf, error_code, old_index);
					Send(m_TempBuf, old_index);
					SoftClose();
					return;
				}
			}
		}
	}

//	UpdateCurrentUserTable();				// 技记俊 辑滚 IP甫 喀但茄促.

	Send(m_TempBuf, index);
}

void USER::ZoneLoginReq(TCHAR *pBuf)
{
	int			index = 0;
	TCHAR		szPw[PASSWORD_LENGTH + 1];
	TCHAR		szID[CHAR_NAME_LENGTH+1];
	BYTE		result = FAIL, error_code = 0;
	CPoint		pt(-1, -1);
	CBufferEx	TempBuf;
	int			iMemory = 0;
	int			iMemoryAccountBank = 0;

	TRACE("ZoneLoginReq Request...Check\n");

	int nIDLength, nPwLength, nCharLength;
	int iMyServer = -1;
	USER *pDoubleUser = NULL;

	nIDLength = GetVarString( sizeof( m_strAccount ), m_strAccount, pBuf, index);
	if(nIDLength == 0 || nIDLength > CHAR_NAME_LENGTH)	
	{
		error_code = ERR_1;
		goto result_send;
	}
	nPwLength = GetVarString( sizeof( szPw ), szPw, pBuf, index);
	if(nPwLength == 0 || nPwLength > CHAR_NAME_LENGTH)	
	{
		error_code = ERR_1;
		goto result_send;
	}
	nCharLength = GetVarString( sizeof( szID ), szID, pBuf, index);
	if(nCharLength == 0 || nCharLength > CHAR_NAME_LENGTH)	
	{
		error_code = ERR_1;
		goto result_send;
	}
	
	// IKING 2002.1.
	iMyServer = GetShort(pBuf, index);

	m_iMyServer = iMyServer;

	if( !IsMyDBServer( m_iMyServer ) )
	{
		m_iMyServer = -1;
		error_code = 255;
		goto result_send;
	}
	//

	if(nIDLength == 0 || nIDLength > CHAR_NAME_LENGTH)	
	{
		error_code = ERR_1;
		goto result_send;
	}

	//鞍篮 酒捞叼啊 乐栏搁 角菩 茄促...
	pDoubleUser = GetUser( szID );
	if( pDoubleUser != NULL ) 
	{  
		//double fors test
		error_code = ERR_1;
		goto result_send;
/*		if( pDoubleUser->m_state != STATE_DISCONNECTED && pDoubleUser->m_state != STATE_LOGOUT )
		{
			error_code = ERR_5;
			TempBuf.Add(GAME_START_RESULT);
			TempBuf.Add(FAIL);
			TempBuf.Add(error_code);
			Send(TempBuf, TempBuf.GetLength());

			pDoubleUser->SendSystemMsg( IDS_USER_DOUBLE_CHAR, SYSTEM_SPECIAL, TO_ME);
			pDoubleUser->SoftClose();
			return;
		} */
	}

	g_pMainDlg->BridgeServerUserZoneLogIn( m_uid, m_strAccount, szID );
	return;


	InitUser();
//	InitMemoryDB(m_uid);
	iMemory = CheckMemoryDB( szID );
//	iMemory = 0;		// 酒第 鞍篮 巴档 绝菌绊, 拌沥 鞍篮 巴档 绝菌促.
//						// 酒第,篮青,烹芒 葛滴 DB俊辑 啊廉客具 茄促.

//	iMemory = 1;		// 酒第尔 拌沥捞 鞍篮 巴篮 乐菌绊, 拌沥父 鞍绊 酒第啊 促弗巴篮 绝菌促.
//						// 酒第,篮青,烹芒 葛滴 MemoryDB俊辑 啊廉吭促. DB俊辑 啊廉坷瘤 臼绰促.

//	iMemory = 1;		// 酒第尔 拌沥捞 鞍篮 巴捞 乐菌绊, 拌沥父 鞍绊 酒第啊 促弗巴档 乐菌促.
//						// 酒第,篮青,烹芒 葛滴 MemoryDB俊辑 啊廉吭促. DB俊辑 啊廉坷瘤 臼绰促.

//	iMemory = 2;		// 酒第尔 拌沥捞 鞍篮 巴篮 绝菌绊, 拌沥父 鞍篮 巴捞 乐菌促.


	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserData( szID ) )
		{
			error_code = ERR_2;
			goto result_send;
		}
	}

	/*
	if( !IsZoneInThisServer(m_curz) )
	{
		ChangeServer(m_curz);
		
		ReInitMemoryDB();

		SoftClose();
		return;
	}
	*/

	m_state = STATE_CONNECTED;

	// zone, zoneindex肺 谅钎 函券...
//	ZoneChangeInfoSet(m_curz, m_curx, m_cury);

	pt = FindNearAvailablePoint_S(m_curx, m_cury);	// DB俊 历厘等 谅钎啊 框流老 荐 乐绰 谅钎牢瘤 魄窜
	if(pt.x == -1 || pt.y == -1) 
	{
		error_code = ERR_4;
		goto result_send;
	}

//	CheckUserLevel();					// 公丰眉氰饭骇牢 25饭骇阑 逞菌阑 版快 

	m_curx = pt.x; m_cury = pt.y;
	SetUid(m_curx, m_cury, m_uid + USER_BAND );		// 蜡历 困摹沥焊 悸泼
	m_presx = -1;
	m_presy = -1;

	pt = ConvertToClient(m_curx, m_cury);

	GetMagicItemSetting();							// 泅犁 酒捞袍吝 概流 加己, 饭骇函悼阑 悼利 函荐俊 馆康茄促.

	m_UserFlag = TRUE;

	result = SUCCESS;

	m_ConnectionSuccessTick = 0;


	InitMemoryDB(m_uid);

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
		if( !LoadAccountBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
	}

	// 粮 眉牢瘤 犬牢棺 函荐 檬扁拳...
	m_strZoneIP = "";
	m_nZonePort	= -1;

	SetPsiAbnormalStatus();
    SetStatus();

result_send:
	TempBuf.Add(GAME_START_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		ReInitMemoryDB();

		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	
	SendCharData();						// 蜡历狼 惑技 沥焊甫 焊辰促.

	TempBuf.Add((short)m_curz);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);

	TempBuf.Add(m_tDir);

	TempBuf.Add((BYTE)g_GameTime);
	TempBuf.Add((BYTE)g_GameMinute);

	Send(TempBuf, TempBuf.GetLength());

	SetGameStartInfo();	

	SendSystemMsg( IDS_USER_OPERATOR_MAIL1, SYSTEM_NORMAL, TO_ME);
//	SendSystemMsg( IDS_USER_OPERATOR_MAIL2, SYSTEM_NORMAL, TO_ME);
}

void USER::AccountLoginReqWithThread(TCHAR *pBuf)
{
	char id[MAX_ID+1];
	int index = 0;
	BYTE	result = FAIL, error_code = 0;

	int	nIDLength = GetVarString(sizeof(id), id, pBuf, index);

	if(nIDLength == 0 || nIDLength > ACCOUNT_LENGTH)	
	{
		error_code = 1;
		index = 0;
		SetByte(m_TempBuf, ACCOUNT_LOGIN_RESULT, index );
		SetByte(m_TempBuf, result, index );
		SetByte(m_TempBuf, error_code, index);
		Send( m_TempBuf, index );
		SoftClose();
		return;
	}

	LOGINOUTTHREADDATA *pLIOTD;
	pLIOTD = new LOGINOUTTHREADDATA;
	pLIOTD->CODE = ACCOUNT_LOGIN_REQ;
	pLIOTD->UID = m_uid;
	memset(pLIOTD->ID, NULL, CHAR_NAME_LENGTH+sizeof(int)+1);
	memcpy(pLIOTD->ID, pBuf, index );

	EnterCriticalSection( &m_CS_LoginData );
	RecvLoginData.AddTail(pLIOTD);
	nLoginDataCount = RecvLoginData.GetCount();
	LeaveCriticalSection( &m_CS_LoginData );
}

///////////////////////////////////////////////////////////////////////////////////
//	Character 沥焊甫 啊廉柯促.
//
BOOL USER::LoadCharData(TCHAR *id)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	BOOL bFind = FALSE;
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_CHAR_DATA (\'%s\')}")/*, m_iMyServer*/, id );
	
	SQLCHAR		strChar[3][CHAR_NAME_LENGTH+1];
	SQLINTEGER	strCharInd[3];

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	for( int i = 0; i < 3; i++)
	{
		::ZeroMemory(strChar[i], CHAR_NAME_LENGTH+1);
		strCharInd[i] = SQL_NTS;
	}

	retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt);
	if( retcode != SQL_SUCCESS )
	{
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			SQLGetData( hstmt, 1, SQL_C_CHAR, strChar[0], CHAR_NAME_LENGTH, &strCharInd[0] );
			SQLGetData( hstmt, 2, SQL_C_CHAR, strChar[1], CHAR_NAME_LENGTH, &strCharInd[1] );
			SQLGetData( hstmt, 3, SQL_C_CHAR, strChar[2], CHAR_NAME_LENGTH, &strCharInd[2] );
		}
		else 
		{
			DisplayErrorMsg(hstmt);
			retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		//BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	m_nCharNum = 0;
	int nSize = sizeof(m_strChar);
	::ZeroMemory(m_strChar, sizeof(m_strChar));

	_tcscpy( m_strChar[0], (LPCTSTR)strChar[0]);		if( strlen( m_strChar[0] ) ) m_nCharNum++;
	_tcscpy( m_strChar[1], (LPCTSTR)strChar[1]);		if( strlen( m_strChar[1] ) ) m_nCharNum++;
	_tcscpy( m_strChar[2], (LPCTSTR)strChar[2]);		if( strlen( m_strChar[2] ) ) m_nCharNum++;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	Client俊 Character 扁夯 沥焊甫 焊辰促.
//
BOOL USER::SendCharInfo(TCHAR *strChar, TCHAR* pBuf, int& index)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	BOOL			bFind = FALSE;
	TCHAR			szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call SEND_CHAR_DATA (\'%s\')}"), strChar );
	
	SQLCHAR		strCharID[CHAR_NAME_LENGTH + 1], strFace[10];
	SQLINTEGER	strCharInd = SQL_NTS;

	SQLSMALLINT	sSTR, sCON, sDEX, sVOL, sWIS, sGender;
	SQLINTEGER	sInd;

	SDWORD		dSkin, dHair;

	SQLSMALLINT	sLevel;
	SQLCHAR		byClass;

	SQLSMALLINT	sHP, sPP, sSP;

	SQLCHAR		strSkill[_SKILL_DB], strItem[_ITEM_DB];

	::ZeroMemory(strCharID, sizeof(strCharID));
	::ZeroMemory(strFace, sizeof(strFace));
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));

	sSTR = sCON = sDEX = sVOL = sWIS = sGender = 0;
	dSkin = dHair = 0;

	byClass = 0;
	sLevel = 1;

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			int i = 1;

			SQLGetData( hstmt, i++,	SQL_C_CHAR,		strCharID,	sizeof(strCharID),	&strCharInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sSTR,		sizeof(sSTR),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sCON,		sizeof(sCON),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sDEX,		sizeof(sDEX),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sVOL,		sizeof(sVOL),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sWIS,		sizeof(sWIS),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_ULONG,	&dSkin,		sizeof(dSkin),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_ULONG,	&dHair,		sizeof(dHair),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sGender,	sizeof(sGender),	&sInd);

			SQLGetData( hstmt, i++, SQL_C_BINARY,	strFace,	sizeof(strFace),	&sInd);

			SQLGetData( hstmt, i++,	SQL_C_TINYINT,	&byClass,	sizeof(byClass),	&sInd);

			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sLevel,	sizeof(sLevel),		&sInd);

			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sHP,		sizeof(sHP),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sPP,		sizeof(sPP),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sSP,		sizeof(sSP),		&sInd);

			SQLGetData( hstmt, i++,	SQL_C_BINARY,	strSkill,	sizeof(strSkill),	&sInd);
			SQLGetData( hstmt, i++,	SQL_C_BINARY,	strItem,	sizeof(strItem),	&sInd);
		}
		else
		{
			DisplayErrorMsg(hstmt);
			retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);			
			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	TCHAR strID[CHAR_NAME_LENGTH+1];
	_tcscpy(strID, (TCHAR*)strCharID);
	short nSTR = sSTR; 
	short nCON = sCON;
	short nDEX = sDEX;
	short nVOL = sVOL;
	short nWIS = sWIS;

	short	sMaxHP = 0;
	short	sMaxPP = 0;
	short	sMaxSP = 0;

	DWORD dwSkin = dSkin;
	DWORD dwHair = dHair;

	BYTE  cGender = (BYTE)sGender;

	CBufferEx TempBuf;
	TempBuf.AddString(strID);
	TempBuf.Add(nSTR);
	TempBuf.Add(nCON);
	TempBuf.Add(nDEX);
	TempBuf.Add(nVOL);
	TempBuf.Add(nWIS);
	TempBuf.Add(dwSkin);
	TempBuf.Add(dwHair);
	TempBuf.Add(cGender);

	TempBuf.AddData((LPTSTR)strFace, 10);

	TempBuf.Add(byClass);
	TempBuf.Add(sLevel);
	TempBuf.Add((short)sHP);
	TempBuf.Add((short)sPP);
	TempBuf.Add((short)sSP);

	sMaxHP	= g_sHPConst[byClass] * nCON + g_sHPLV[byClass] * (sLevel - 1) + g_sHPAdd[byClass];
	sMaxPP	= g_sPPConst[byClass] * nWIS + g_sPPLV[byClass] * (sLevel - 1) + g_sPPAdd[byClass];
	sMaxSP	= g_sSPConst[byClass] * nCON + g_sSPLV[byClass] * (sLevel - 1) + g_sSPAdd[byClass];

	TempBuf.Add((short)sMaxHP);
	TempBuf.Add((short)sMaxPP);
	TempBuf.Add((short)sMaxSP);

	// Parse Skill
	CHAR szSkillBuf[3 * TOTAL_SKILL_NUM];
	::ZeroMemory(szSkillBuf, sizeof(szSkillBuf));
	if(!GetSkillNameLevel(byClass, (LPTSTR)strSkill, szSkillBuf)) return FALSE;

	// Parse Item
	TCHAR szItemBuf[2 * EQUIP_ITEM_NUM];		// 2 = Item Pid, 10 = Totoal Equip Slot Count
	::ZeroMemory(szItemBuf, sizeof(szItemBuf));

	if(!GetEquipItemPid((LPTSTR)strItem, szItemBuf))
	{
		return FALSE;
	}

	TempBuf.AddData(szSkillBuf, 3 * TOTAL_SKILL_NUM);
	TempBuf.AddData(szItemBuf, 2* EQUIP_ITEM_NUM);

	::CopyMemory(pBuf + index, TempBuf, TempBuf.GetLength());
	index += TempBuf.GetLength();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
//	货肺款 某腐磐甫 父电促.
//
void USER::NewCharReq(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[4096];
	
	SDWORD			sFaceLen = 10;
	SDWORD			sSkillLen	= _SKILL_DB;
	SDWORD			sPsiLen		= _PSI_DB;
	SDWORD			sItemLen	= _ITEM_DB;

	int db_index = 0;
	CDatabase* pDB;
	int				i;

	const	int	nInitMax = 25; 

	int		SumPoint = 0;
	int		index = 0;
	TCHAR	szAccount[ACCOUNT_LENGTH + 1], szName[CHAR_NAME_LENGTH + 1], szFace[10];
	BYTE	result = FAIL, error_code = 0;
	BYTE	nCharNum	= 0;
	int		nTempLength = 0;

	CString szTemp		= _T("");
	short	nSTR		= 0;
	short	nCON		= 0;
	short	nDEX		= 0;
	short	nVOL		= 0;
	short	nWIS		= 0;

	short	sHP = 0;
	short	sPP = 0;
	short	sSP = 0;

	DWORD	dwSkin		= 0;
	DWORD	dwHair		= 0;

	BYTE	cGender		= 0;
	BYTE	byClass		= 0;

	if(!GetVarString(sizeof(szAccount), szAccount, pBuf, index)) {error_code = 1; goto result_send; }
	nCharNum = GetByte(pBuf, index);
	if(!GetVarString(sizeof(szName), szName, pBuf, index)) {error_code = 2; goto result_send; }

	nTempLength = _tcslen(szAccount);
	if(!szAccount[0] ||  nTempLength > ACCOUNT_LENGTH)
	{
		error_code = 1;
		goto result_send;
	}

	if(m_nCharNum >= 3 || nCharNum > 2 || nCharNum < 0)
	{
		error_code = 2;
		goto result_send;
	}

	nTempLength = _tcslen(szName);
	if(nTempLength == 0 || nTempLength > CHAR_NAME_LENGTH) 
	{
		error_code = 8;
		goto result_send;
	}

	szTemp = szName;

	if(!UNI_CHAR::CheckString(szTemp))
	{
		result = FAIL;
		error_code = 8;
		index = 0;
		SetByte(m_TempBuf, NEW_CHAR_RESULT, index);
		SetByte(m_TempBuf, result, index);
		SetByte(m_TempBuf, error_code, index);
		Send(m_TempBuf, index);
		return;
	}
/*	if(szTemp.Find(' ') != -1 || szTemp.Find('\'') != -1 || szTemp.Find('"') != -1 || szTemp.Find('.') != -1 || szTemp.Find(',') != -1)
	{
		result = FAIL;
		error_code = 8;
		index = 0;
		SetByte(m_TempBuf, NEW_CHAR_RESULT, index);
		SetByte(m_TempBuf, result, index);
		SetByte(m_TempBuf, error_code, index);
		Send(m_TempBuf, index);
		return;
	}
*/
	if(IsReservedID(szName))
	{
		result = FAIL;
		error_code = 7;
		index = 0;
		SetByte(m_TempBuf, NEW_CHAR_RESULT, index);
		SetByte(m_TempBuf, result, index);
		SetByte(m_TempBuf, error_code, index);
		Send(m_TempBuf, index);
		return;
	}

	switch(m_iMyServer)
	{
	case	0:	// 抛胶飘
		break;
	case	1:	// 捞墨风胶
		strcat( szName, "[I]" );
		break;
	case	2:	// 郡颇档福
		strcat( szName, "[E]" );
		break;
	case	3:	// 啊匙瘤酒
		strcat( szName, "[K]" );
		break;
	case	4:	// 鞘赋器
		strcat( szName, "[F]" );
		break;
	case	5:	// 酒胶飘
		strcat( szName, "[A]" );
		break;
	case	6:	// 弊尔叼俊
		strcat( szName, "[G]" );
		break;
	case	7:	// 其技快胶
		strcat( szName, "[P]" );
		break;
	case	8:	// 剐扼叼
		strcat( szName, "[M]" );
		break;
	case	9:	// 风捞匙飘
		strcat( szName, "[R]" );
		break;
	case	10:	// 鸥福箕
		strcat( szName, "[T]" );
		break;
	default:
		result = FAIL;
		error_code = 255;
		index = 0;
		SetByte(m_TempBuf, NEW_CHAR_RESULT, index);
		SetByte(m_TempBuf, result, index);
		SetByte(m_TempBuf, error_code, index);
		Send(m_TempBuf, index);
		return;
	}

	// 粮犁窍绰 ID牢瘤 八荤
	if(IsExistCharId(szName))
	{
		error_code = 7;
		goto result_send;
	}

	nSTR		= (short)m_sTempSTR;
	nCON		= (short)m_sTempCON;
	nDEX		= (short)m_sTempDEX;
	nVOL		= (short)m_sTempVOL;
	nWIS		= (short)m_sTempWIS;

	sHP = 0;
	sPP = 0;
	sSP = 0;

	dwSkin		= GetDWORD(pBuf, index);
	dwHair		= GetDWORD(pBuf, index);

	cGender		= GetByte (pBuf, index);

	::ZeroMemory(szFace, sizeof(szFace));
	GetString(szFace, pBuf, 10, index);
	
	byClass		= GetByte(pBuf, index);
	if(byClass != m_tTempClass) 
	{
		error_code = 10;
		goto result_send;
	}

	if(byClass < 0 || byClass > 3)
	{
		error_code = 10;
		goto result_send;
	}

	//-------------[ 檬扁 瓷仿摹 八荤 ]-----------------------------//
	if(SumPoint > 65)		// 檬扁 瓷仿摹狼 钦篮 65
	{
		error_code = 9;
		goto result_send;
	}

	if(nSTR < 1 || nSTR > nInitMax ||
	   nCON < 1 || nCON > nInitMax ||
	   nDEX < 1 || nDEX > nInitMax ||
	   nVOL < 1 || nVOL > nInitMax ||
	   nWIS < 1 || nWIS > nInitMax)
	{
		error_code = 3;
		goto result_send;
	}

	//-----------[ 纳腐磐 Reroll 荐摹 檬扁拳 ] -------------------------------//
	m_sTempSTR = 0;					
	m_sTempCON = 0;					
	m_sTempDEX = 0;					
	m_sTempVOL = 0;					
	m_sTempWIS = 0;					

	m_tTempClass = 255;

	SumPoint = nSTR + nCON + nDEX + nVOL + nWIS;

	// 乔何祸, 赣府祸, 庆绢胶鸥老俊 措茄 八荤绰 眠饶俊... 

	pDB = g_DB[m_iModSid].GetDB( db_index );		// 困俊辑 葛电 俊矾蔼甫 眉农窍绊 唱辑 且寸
	if( !pDB ) return ;
	
	sHP = g_sHPConst[byClass] * nCON + g_sHPAdd[byClass];
	sPP = g_sPPConst[byClass] * nWIS + g_sPPAdd[byClass];
	sSP = g_sSPConst[byClass] * nCON + g_sSPAdd[byClass];
	
	// Skill, Item, Psionic
	TCHAR strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB];
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));

	sFaceLen = sizeof(szFace);
	sSkillLen = sizeof(strSkill);
	sItemLen = sizeof(strItem);
	sPsiLen = sizeof(strPsi);

	MakeEmptySkill(strSkill, byClass);
	MakeInintItem(strItem, byClass);
	MakeEmptyPsi(strPsi);

	// Stored Procedure Call
	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call NEW_CHAR (\'%s\', %d, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, ?, ?, ?, ?, %d, %d, %d)}")/*, m_iMyServer*/, 
						szAccount, nCharNum, szName,
						nSTR, nCON, nDEX, nVOL, nWIS,
						dwSkin, dwHair,
						cGender,
						byClass, 
						sHP, sPP, sSP);

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if(retcode != SQL_SUCCESS )
	{
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}
	
	i = 1;
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(szFace),		0, (TCHAR*)szFace,		0, &sFaceLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strSkill),	0, (TCHAR*)strSkill,	0, &sSkillLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strPsi),		0, (TCHAR*)strPsi,		0, &sPsiLen );
	
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_NEED_DATA || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else if( retcode == SQL_ERROR )
	{
		DisplayErrorMsg( hstmt );
		error_code = UNKNOWN_ERR;

		g_DB[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt );
	g_DB[m_iModSid].ReleaseDB(db_index);

	if(!LoadCharData(m_strAccount))
	{
		error_code = UNKNOWN_ERR;

		g_DB[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}
	else result = SUCCESS;

result_send:
	TCHAR FailBuf[8192];
	int nOldIndex = 0;
	index = 0;
	
	SetByte(m_TempBuf, NEW_CHAR_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result != SUCCESS)
	{
		SetByte(m_TempBuf, error_code, index);
		::CopyMemory(FailBuf, m_TempBuf, index);
		nOldIndex = index;
		Send(m_TempBuf, index);
		return;
	}

	SetByte(m_TempBuf, nCharNum, index);

	if(!SendCharInfo(szName, m_TempBuf, index))
	{
		Send(FailBuf, nOldIndex);
		return;
	}

	Send(m_TempBuf, index);
}

void USER::NewCharReqWithThread(TCHAR *pBuf)
{
	int index = 0;
	TCHAR	szAccount[ACCOUNT_LENGTH + 1], szName[CHAR_NAME_LENGTH + 1], szFace[11];

	if(!GetVarString(sizeof(szAccount), szAccount, pBuf, index)) return;
	BYTE	nCharNum		= GetByte(pBuf, index);
	if(!GetVarString(sizeof(szName), szName, pBuf, index))return;

	DWORD	dwSkin		= GetDWORD(pBuf, index);
	DWORD	dwHair		= GetDWORD(pBuf, index);
	BYTE	cGender		= GetByte (pBuf, index);
	
	GetString(szFace, pBuf, 10, index);

	BYTE	byClass		= GetByte(pBuf, index);

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = NEW_CHAR_REQ_WITH_THREAD;
	Datalength = index;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, 0, Datalength+1);
	memcpy(pData, pBuf, Datalength);
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );
}

////////////////////////////////////////////////////////////////////////////////
//	扁夯荐摹蔼阑 积己茄促.
//
void USER::RerollReq(TCHAR *pBuf)
{
	int index = 0;
	BYTE tClass = GetByte(pBuf, index);
	BYTE tBasic[] = {13, 12, 11, 10, 9};
	
	int i = 0;
	int nSum = 0;
	int nRemain = 0;
	int nRand = 0;

	for(i = 0; i < 5; i++) nSum += tBasic[i];

	nRemain = 65 - nSum;
	while(nRemain > 0)
	{
		for(i = 0; i < 5; i++)
		{
			nRand = myrand(0, 100);
			if(nRand > 50) nRand = 1;
			else nRand = 0;

			if(nRand == 1 && tBasic[i] < 25) 
			{
				tBasic[i]++;
				nRemain--;
			}
			if(nRemain <= 0) break;
		}
	}

	i = 0;
	switch(tClass)
	{
	case 0:			// 拜捧
		m_sTempCON = tBasic[i++];
		m_sTempDEX = tBasic[i++];
		m_sTempSTR = tBasic[i++];
		m_sTempVOL = tBasic[i++];
		m_sTempWIS = tBasic[i++];
		break;
	case 1:			// 瘤莆捞
		m_sTempWIS = tBasic[i++];
		m_sTempVOL = tBasic[i++];
		m_sTempCON = tBasic[i++];
		m_sTempDEX = tBasic[i++];
		m_sTempSTR = tBasic[i++];
		break;
	case 2:			// 档八
		m_sTempSTR = tBasic[i++];
		m_sTempCON = tBasic[i++];
		m_sTempDEX = tBasic[i++];
		m_sTempWIS = tBasic[i++];
		m_sTempVOL = tBasic[i++];
		break;
	case 3:			// 醚扁
		m_sTempDEX = tBasic[i++];
		m_sTempCON = tBasic[i++];
		m_sTempWIS = tBasic[i++];
		m_sTempVOL = tBasic[i++];
		m_sTempSTR = tBasic[i++];
		break;
	}

	index = 0;
	SetByte(m_TempBuf, REROLL_RESULT, index);
	SetByte(m_TempBuf, m_sTempSTR, index);
	SetByte(m_TempBuf, m_sTempCON, index);
	SetByte(m_TempBuf, m_sTempDEX, index);
	SetByte(m_TempBuf, m_sTempVOL, index);
	SetByte(m_TempBuf, m_sTempWIS, index);

	Send(m_TempBuf, index);
}

/////////////////////////////////////////////////////////////////////////////////
//	捞固 粮犁窍绰 某腐磐牢瘤 八荤茄促.
//
BOOL USER::IsExistCharId(TCHAR *id)
{
	/*
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	BOOL bFind = FALSE;
	
	memset( szSQL, 0x00, 1024 );
	_sntprintf(szSQL, sizeof(szSQL), TEXT("SELECT strUserId FROM GAMEUSER WHERE strUserId = \'%s\'"), id );
	
	hstmt = NULL;

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			bFind = TRUE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return TRUE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if(!bFind ) return FALSE;

	return TRUE;
	*/

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	SQLSMALLINT	sRet;
	SQLINTEGER sRetInd = SQL_NTS;

	sRet = 0;

	memset( szSQL, 0x00, 1024 );
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call IS_EXIST_CHAR (\'%s\', ?)}"), id);
	
	hstmt = NULL;

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}

	retcode = SQLBindParameter(hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &sRetInd);
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
//		retcode = SQLFetch( hstmt );
//
//		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
//		{
//		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return TRUE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( sRet == 0 ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////
//	某腐磐甫 昏力茄促.
//
void USER::DeleteCharReq(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	SQLHSTMT	hstmt = NULL;

	int		index = 0;
	TCHAR	szAccount[ACCOUNT_LENGTH+1], szChar[CHAR_NAME_LENGTH+1];
	TCHAR	szJumin[20], szIP[128];
	BYTE	result = FAIL, error_code = 0;

	::ZeroMemory(szJumin, sizeof(szJumin));
	::ZeroMemory(szIP, sizeof(szIP));

	int db_index = 0;
	CDatabase* pDB = NULL;

	int nAccountLength = 0, nCharLength = 0, nJuminCount = 0;
	BYTE CharIndex = 0;

	nAccountLength = GetVarString(sizeof(szAccount), szAccount, pBuf, index);
	if(nAccountLength == 0 || nAccountLength > ACCOUNT_LENGTH) 
	{
		error_code = 1;
		goto result_send;
	}

	nCharLength	= GetVarString(sizeof(szChar), szChar, pBuf, index);
	if(nCharLength == 0 || nCharLength > CHAR_NAME_LENGTH) 
	{
		error_code = 2;
		goto result_send;
	}

	CharIndex = GetByte(pBuf, index);

	nJuminCount	= GetVarString(sizeof(szJumin), szJumin, pBuf, index);
	if(nJuminCount == 0 || nJuminCount > 15) 
	{
		error_code = 5;
		goto result_send;
	}
	
//	GetVarString(szIP, pBuf, sizeof(BYTE), index);	泅犁 IP绰 老窜 公矫茄促.


	if(CharIndex < 0 || CharIndex > 2)
	{
		error_code = 3;
		goto result_send;
	}
	
	if(m_strChar[CharIndex][0] == 0) 
	{
		error_code = 3;
		goto result_send;
	}

	if(_stricmp(szChar, m_strChar[CharIndex]) != 0)
	{
		error_code = 2;
		goto result_send;
	}

	if(CheckJuminNumber(szAccount, szJumin) == FALSE)
	{
		error_code = 6;		// !!Check
		goto result_send;
	}

	if(!IsExistCharId(szChar)) 
	{
		error_code = 3;
		goto result_send;
	}
	
	pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	SQLRETURN		retcode;
	TCHAR			szSQL[1024];
	
	memset(szSQL, 0x00, 1024);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{? = call DEL_CHAR (\'%s\', \'%s\', %d)}")/*, m_iMyServer*/, szAccount, szChar, CharIndex);
	
	SQLSMALLINT	sRet;
	SQLINTEGER sRetInd;	

	sRet = 0;
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
	{
		error_code = 4;

		//g_DB[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}
	
	retcode = SQLBindParameter(hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &sRetInd);
	if (retcode!=SQL_SUCCESS) 
	{
		error_code = 4;
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}
	
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);
			if (retcode !=SQL_SUCCESS && retcode !=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
		}
	}
	else 
	{
		error_code = 4;
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}
	
	if (hstmt!=NULL) SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if(sRet == 0) result = SUCCESS;
	else if(sRet == 100)
	{
		error_code = 7;
		goto result_send;
	}

	if( !LoadCharData(szAccount) )
	{
		// 犁肺爹 角菩 - 单捞磐俊 捞惑
		error_code = UNKNOWN_ERR;
		goto result_send;
	}

result_send:
	index = 0;
	SetByte(m_TempBuf, DELETE_CHAR_RESULT, index);
	SetByte(m_TempBuf, result, index);
	if(result != SUCCESS) SetByte(m_TempBuf ,error_code, index);
	else SetByte(m_TempBuf, CharIndex, index);

	Send(m_TempBuf, index);
}

void USER::DeleteCharReqWithThread(TCHAR *pBuf)
{
	int		index = 0;
	BYTE	result = FAIL, error_code = 0;
	TCHAR	szAccount[20], szChar[30];
	TCHAR	szJumin[20], szIP[128];
	int		nAccountLength =0, nCharLength = 0, nJuminCount = 0;
	BYTE	CharIndex = 0;

	::ZeroMemory(szJumin, sizeof(szJumin));
	::ZeroMemory(szIP, sizeof(szIP));

	nAccountLength	= GetVarString(sizeof(szAccount), szAccount, pBuf, index);
	if(nAccountLength == 0 || nAccountLength > ACCOUNT_LENGTH) 
	{
		error_code = 1;
		goto result_send;
	}

	nCharLength	= GetVarString(sizeof(szChar), szChar, pBuf, index);
	if(nCharLength == 0 || nCharLength > CHAR_NAME_LENGTH) 
	{
		error_code = 2;
		goto result_send;
	}

	CharIndex = GetByte(pBuf, index);

	nJuminCount	= GetVarString(sizeof(szJumin), szJumin, pBuf, index);
	if(nJuminCount == 0 || nJuminCount > 15) 
	{
		error_code = 5;
		goto result_send;
	}

	if(!GetVarString(sizeof(szIP), szIP, pBuf, index)) return;

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = DELETE_CHAR_REQ;
	Datalength = index;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, 0, Datalength+1);
	memcpy(pData, pBuf, Datalength);
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );
	return;

result_send:
	index = 0;
	SetByte(m_TempBuf, DELETE_CHAR_RESULT, index);
	SetByte(m_TempBuf, result, index);
	if(result != SUCCESS) SetByte(m_TempBuf ,error_code, index);
	else SetByte(m_TempBuf, CharIndex, index);

	Send(m_TempBuf, index);
}
///////////////////////////////////////////////////////////////////////////////
//加入:角色登陆后检测账户.账户错误或为空则踢下线
//
BOOL USER::CheckInvalidatePassword(char *strAccount,char *strPassword,char *szid)
{
	//检测密码
	{/*
		SQLHSTMT		hstmt;
		SQLRETURN		retcode;
		TCHAR			szSQL[1024];

		BOOL bQuerySuccess = TRUE;

		memset(szSQL, 0x00, 1024);
		_sntprintf(szSQL, sizeof(szSQL), TEXT("select struserid,strpasswd from ngscuser where struserid='%s'and strpasswd='%s'"), strAccount, strPassword);

		hstmt = NULL;

		int db_index = -1;
		BOOL bPasswordIsValidate = FALSE;
		try
		{
			CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
			if( !pDB ) return FALSE;

			retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
			if (retcode!=SQL_SUCCESS)
			{
				//		g_DBSession[m_iModSid].ReleaseDB(db_index);
				return FALSE;
			}
			retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
			if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				while (TRUE)
				{
					retcode = SQLFetch(hstmt);
					if (retcode !=SQL_SUCCESS && retcode !=SQL_SUCCESS_WITH_INFO)
					{
						break;
					}
					bPasswordIsValidate = TRUE;
				}
			}else		
				if (retcode==SQL_ERROR)
				{
					bQuerySuccess = FALSE;
					DisplayErrorMsg(hstmt);
				}
				else if (retcode==SQL_NO_DATA)
				{
					bQuerySuccess = FALSE;
				}

				if (hstmt!=NULL) 
					SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

				if( !bQuerySuccess ) 
				{
					//		BREAKPOINT();
					g_DBSession[m_iModSid].ReleaseDB(db_index);
					return FALSE;
				}
		}
		catch(...)
		{
			TRACE("数据库异常\r\n");
		}
		g_DBSession[m_iModSid].ReleaseDB(db_index);
		//密码校验失败
		if(bPasswordIsValidate == FALSE)
			return FALSE;
			*/
	}
	BOOL bCharIsValidate = FALSE;
	//检测帐号与角色是否绑定
	{
		SQLHSTMT		hstmt;
		SQLRETURN		retcode;
		TCHAR			szSQL[1024];

		BOOL bQuerySuccess = TRUE;

		memset(szSQL, 0x00, 1024);
		_sntprintf(szSQL, sizeof(szSQL), 
			TEXT("select strchar01,strchar02,strchar03 from chars where straccount='%s'"), strAccount);

		hstmt = NULL;

		int db_index = -1;
		char szChar01[40]="",szChar02[40]="",szChar03[40]="",szChar04[40]="";
		int i;
		try
		{
			CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
			if( !pDB ) return FALSE;

			retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
			if (retcode!=SQL_SUCCESS)
			{
				//		g_DBSession[m_iModSid].ReleaseDB(db_index);
				return FALSE;
			}
			retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
			SQLINTEGER ivd;
			i = 0;
			if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				while (TRUE)
				{
					retcode = SQLFetch(hstmt);
					if (retcode !=SQL_SUCCESS && retcode !=SQL_SUCCESS_WITH_INFO)
					{
						break;
					}
					SQLGetData( hstmt, i++, SQL_C_CHAR, szChar01, sizeof(szChar01), &ivd );
					SQLGetData( hstmt, i++, SQL_C_CHAR, szChar02, sizeof(szChar02), &ivd );
					SQLGetData( hstmt, i++, SQL_C_CHAR, szChar03, sizeof(szChar03), &ivd );
					SQLGetData( hstmt, i++, SQL_C_CHAR, szChar04, sizeof(szChar04), &ivd );
				}
			}else
				if (retcode==SQL_ERROR)
				{
					bQuerySuccess = FALSE;
					DisplayErrorMsg(hstmt);
				}
				else if (retcode==SQL_NO_DATA)
				{
					bQuerySuccess = FALSE;
				}

				if (hstmt!=NULL) 
					SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

				if( !bQuerySuccess ) 
				{
					//		BREAKPOINT();
					g_DB[m_iModSid].ReleaseDB(db_index);
					return FALSE;
				}
				//对角色进行判断
				{
					if(!strcmp(szChar01,szid)||!strcmp(szChar02,szid)||!strcmp(szChar03,szid)||!strcmp(szChar04,szid))
						bCharIsValidate = TRUE;
				}
		}
		catch(...)
		{
			TRACE("数据库异常\r\n");
		}
		g_DB[m_iModSid].ReleaseDB(db_index);
	}
	return bCharIsValidate;
}
///////////////////////////////////////////////////////////////////////////////
//	霸烙 矫累
//
void USER::GameStart(TCHAR *pBuf)
{
	if( g_bShutDown ) return;
    
	int			index = 0;
	BYTE		result = FAIL, error_code = 0;
	TCHAR		szID[CHAR_NAME_LENGTH+1];
	CPoint		pt(-1, -1);
	CBufferEx	TempBuf;
	int			iMemory = 0;
	int			iMemoryAccountBank = 0;
	USER *pUser = NULL;


	//TCHAR		bankstr[_BANK_DB];
	//TCHAR		accountbankstr[_ACCOUNT_BANK_DB];
	
	USER *pDoubleUser = NULL;

	int	nIDLength = GetVarString(sizeof(szID), szID, pBuf, index);

	if(nIDLength == 0 || nIDLength > CHAR_NAME_LENGTH)	
	{
		error_code = ERR_1;
		goto result_send;
	}

	int iMyServer, nAccountID;
	TCHAR		szAccountID[ACCOUNT_LENGTH+1];
#ifdef _CHINA
	iMyServer=1;
	index=index+2;
#else
	iMyServer = GetShort(pBuf, index);
#endif

	m_iMyServer = -1;

	if( !IsMyDBServer( iMyServer ) )
	{
		error_code = 255;
		goto result_send;
	}

	m_iMyServer = iMyServer;

	nAccountID = GetVarString(sizeof(szAccountID), szAccountID, pBuf, index);
	
	if(nAccountID == 0 || nAccountID > ACCOUNT_LENGTH)
	{
		error_code = 1;
		goto result_send;
	}
	
	
	strcpy(m_strAccount,szAccountID);

	/*	if(IsDoubleAccount(m_strAccount))
	{
		error_code = ERR_1;
		goto result_send;
	}*/
	if(IsDoubleAccount(m_strAccount,&pUser))
	{
		if(pUser&& pUser->m_bSessionOnline)
		{
			pUser->m_bSessionOnline = false;
			pUser->LogOut();
		}

		error_code = ERR_1;
		goto result_send;
	}

	//鞍篮 酒捞叼啊 乐栏搁 角菩 茄促...
	pDoubleUser = GetUser( szID );
	if( pDoubleUser != NULL ) 
	{
// fors test double char
		error_code = ERR_1;
		goto result_send;
/*		if( pDoubleUser->m_state != STATE_DISCONNECTED && pDoubleUser->m_state != STATE_LOGOUT )
		{
			error_code = ERR_5;
			TempBuf.Add(GAME_START_RESULT);
			TempBuf.Add(FAIL);
			TempBuf.Add(error_code);
			Send(TempBuf, TempBuf.GetLength());

			pDoubleUser->SendSystemMsg( IDS_USER_DOUBLE_CHAR, SYSTEM_SPECIAL, TO_ME);
			pDoubleUser->SoftClose();
			return;
		} */
	}
//////检测帐号与角色是否匹配开始///////////////////////
	//增加对密码检测
	{
		TCHAR szPwd[40] = "";
		/*不检测密码
		int nPwdLen;
		nPwdLen = GetVarString(sizeof(szPwd), szPwd, pBuf, index);
		if(nPwdLen == 0 || nPwdLen > PASSWORD_LENGTH)
		{
			error_code = 1;
			goto result_send;
		}*/
		if(!CheckInvalidatePassword(szAccountID,szPwd,szID))
		{
			error_code = 6;
			goto result_send;
		}
	}
	QuJiaoXiMiMa(szAccountID);
//////检测帐号与角色是否匹配结束/////////////////////////////////////
	g_pMainDlg->BridgeServerUserLogIn( m_uid, szAccountID, szID );

	// alisia - 咯扁辑 谗绢具 茄促. 搬苞甫 罐酒辑 促澜阑 柳青茄促.
	return;



	InitUser();
//	InitMemoryDB(m_uid);
	iMemory = CheckMemoryDB( szID );




	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserData( szID ) )
		{
			error_code = ERR_2;
			goto result_send;
		}
	}
	

	if( !IsZoneInThisServer(m_curz) )
	{
		ChangeServer(m_curz);
		
		ReInitMemoryDB();

		SoftClose();
		return;
	}
//	SetZoneIndex(m_curz);

	m_state = STATE_CONNECTED;

	pt = FindNearAvailablePoint_S(m_curx, m_cury);	// DB俊 历厘等 谅钎啊 框流老 荐 乐绰 谅钎牢瘤 魄窜
	if(pt.x == -1 || pt.y == -1) 
	{
		error_code = ERR_4;
		goto result_send;
	}

	CheckUserLevel();								// 公丰眉氰饭骇牢 25饭骇阑 逞菌阑 版快 

	m_curx = pt.x; m_cury = pt.y;
	SetUid(m_curx, m_cury, m_uid + USER_BAND );		// 蜡历 困摹沥焊 悸泼
	m_presx = -1;
	m_presy = -1;

	pt = ConvertToClient(m_curx, m_cury);

//	SetUserToMagicUser();							// 泅犁 瓷仿摹甫 悼利 蜡历函荐俊 歹茄促.
	GetMagicItemSetting();							// 泅犁 酒捞袍吝 概流 加己, 饭骇函悼阑 悼利 函荐俊 馆康茄促.

	m_UserFlag = TRUE;

	result = SUCCESS;

	m_ConnectionSuccessTick = 0;


	InitMemoryDB(m_uid);

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
		if( !LoadAccountBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
	}

/*
	if( iMemory == 0 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}

//		iMemoryAccountBank = CheckMemoryAccountBankDB( m_strAccount );

//		if( iMemoryAccountBank == 0 )
//		{
//			if( !LoadAccountBank() )
//			{
//				error_code = ERR_2;
//				result = FAIL;
//				goto result_send;
//			}
//		}
//		else
//		{
//			UserAccountBankItemToStr( accountbankstr );
//			SetMemAccountBank( accountbankstr );
//		}
	}
*/

result_send:
	TempBuf.Add(GAME_START_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		ReInitMemoryDB();

		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
//		SockCloseProcess();
		return;
	}
	
	SendCharData();						// 蜡历狼 惑技 沥焊甫 焊辰促.

	//TRACE( "Initial Point : %d %d\n", m_curx, m_cury);
	
	TempBuf.Add((short)m_curz);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);

	TempBuf.Add(m_tDir);

	TempBuf.Add((BYTE)g_GameTime);
	TempBuf.Add((BYTE)g_GameMinute);

	Send(TempBuf, TempBuf.GetLength());

	SetGameStartInfo();	

	SendSystemMsg( IDS_USER_OPERATOR_MAIL1, SYSTEM_NORMAL, TO_ME);
//	SendSystemMsg( IDS_USER_OPERATOR_MAIL2, SYSTEM_NORMAL, TO_ME);
}

void USER::GameStartWithThread(TCHAR *pBuf)
{
	int			index = 0;
	BYTE		result = FAIL;
	TCHAR		szID[CHAR_NAME_LENGTH+1];
	CBufferEx	TempBuf;

	int	nIDLength = GetVarString(sizeof(szID), szID, pBuf, index);
	if ( nIDLength == 0 || nIDLength > CHAR_NAME_LENGTH )
	{
		ReInitMemoryDB();

		TempBuf.Add(GAME_START_RESULT);
		TempBuf.Add(result);
		TempBuf.Add(ERR_1);
		Send(TempBuf, TempBuf.GetLength());
		SoftClose();
		return;
	}

	LOGINOUTTHREADDATA *pLIOTD;
	pLIOTD = new LOGINOUTTHREADDATA;
	pLIOTD->CODE = GAME_START_REQ;
	pLIOTD->UID = m_uid;
	memset(pLIOTD->ID, NULL, CHAR_NAME_LENGTH+sizeof(int)+1);
	memcpy(pLIOTD->ID, pBuf, index );

	EnterCriticalSection( &m_CS_LoginData );
	RecvLoginData.AddTail(pLIOTD);
	nLoginDataCount = RecvLoginData.GetCount();
	LeaveCriticalSection( &m_CS_LoginData );
}

///////////////////////////////////////////////////////////////////////////
//	Game Start 矫俊 鞘夸茄 沥焊 悸泼
//
void USER::SetGameStartInfo()
{
	// Game捞 Start 瞪锭 鞘夸茄 沥焊 傈价...
//	SendQuickChange();					// 趣矫 捞傈 立加俊辑 酒捞袍 殿废捞 秒家等巴捞 乐绰瘤 舅酒夯促.
	SendUserStatusSkill();

	SendWeatherInMoveZone();			// 粮俊辑 朝揪函拳甫 舅酒夯促. 
	SetWeather(1, 0);					// 粮俊辑 矫埃

	m_state = STATE_GAMESTARTED;

//	SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	SightRecalc();						// 郴 矫具救狼 沥焊甫 唱俊霸 焊辰促.

//	LoadUserBank();
//	LoadAccountBank();
	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	if( rank != ANGEL_RANK )		// 绢骂 函沁电 技牢飘啊 酒聪扼搁
	{
		m_dwSaintTime = 0;
	}

	UpdateMemUserAll(TRUE);

	m_dwServerTick = GetTickCount();

	m_dwLastNoDamageTime = GetTickCount();
	m_dwNoDamageTime = NO_DAMAGE_TIME;
	//TRACE("######### Game Start ##################\n");
}

///////////////////////////////////////////////////////////////////////////
//	厘馒茄 酒捞袍吝 概流捞 乐栏搁 概流 加己阑 馆康茄促.(函荐蔼阑 函悼)
//	
void USER::GetMagicItemSetting()
{
	int i, j;

	int nCount = 4;										// 概流, 饭绢老锭 曼炼 冉荐
	int magic_slot = 0;
	int dynamic_slot = 0;
	BOOL bOldShield = CheckAbnormalInfo(ABNORMAL_SHIELD);
	CBufferEx TempBuf;

	for(i = 0; i < MAGIC_COUNT; i++) m_DynamicUserData[i] = 0;
	for(i = 0; i < EBODY_MAGIC_COUNT; i++) m_DynamicEBodyData[i] = 0;
	for(i = 0; i < HUANSHI_MAGIC_COUNT; i++) m_DynamicMagicItem[i]= 0;
	for(i = 0; i < 10; i++) m_DynamicMagicItem[i]= 0;
	for(i = 0; i < MS_MAGIC_COUNT; i++)	m_DynamicSuit[i] = 0;// 程序卡
	for(i = 0;i<2;i++) m_Dynamic140[i]=0;//140技能
	m_nHPPoint = 0;

	for(i = 0; i < EQUIP_ITEM_NUM; i++)					// 傈眉 酒捞袍 浇吩甫 促 八荤茄促.
	{
		if(m_UserItem[i].sSid >= 0)			
		{												// 咯扁俊辑 概流俊 馆康等 酒捞袍 饭骇蔼阑 悸泼茄促.				
			if((m_UserItem[i].tIQ > NORMAL_ITEM && m_UserItem[i].tIQ <= REMODEL_MAGIC_ITEM) || m_UserItem[i].tIQ==12 || m_UserItem[i].tIQ==18)	// 促 概流 加己捞 嘿绢 乐栏搁...
			{
				if(m_UserItem[i].tIQ == UNIQUE_ITEM) nCount = MAGIC_NUM;
				if(m_UserItem[i].tIQ == 12 || m_UserItem[i].tIQ==18)
				{
					if(m_UserItem[i].tMagic[5] <= 6) nCount = (MAGIC_NUM - 4);
					else if(m_UserItem[i].tMagic[5] >= 7) nCount = MAGIC_NUM;
				}
														// 概流, 饭绢绰 4俺 浇吩父 唱赣瘤绰 诀臂窍绰单 靖
				if(i != LEFT_HAND)
				{
					for(j = 0; j < nCount; j++)			// 浇吩 荐父怒 倒赴促. 
					{				
						magic_slot = m_UserItem[i].tMagic[j];
						if(magic_slot < 0 || magic_slot >= g_arMagicItemTable.GetSize()) continue;

						//属性是否与装备符合
						if(IsMagicVSItem(m_UserItem[i].sSid,magic_slot) == FALSE&&(i == 0||i == 1||i==8||i==9))//帽子,衣服,裤子,鞋子
						{
							TRACE("IsMagicVSItem(%d,%d)\r\n",m_UserItem[i].sSid,magic_slot);
							continue;
						}					

						dynamic_slot = g_arMagicItemTable[magic_slot]->m_sSubType;
						if(dynamic_slot < 0 || dynamic_slot >= MAGIC_COUNT) continue;

						m_DynamicUserData[dynamic_slot] += g_arMagicItemTable[magic_slot]->GetMagicItemValue();
						if(j >=6)//百及装备属性生效
						{													
							magic_slot = m_UserItem[i].tMagic[j];							
							if(magic_slot < 3 || magic_slot >= g_arHuanshiTable.GetSize()) continue;
					
							dynamic_slot = g_arHuanshiTable[magic_slot]->m_sSubType; //属性类型
							if(dynamic_slot < 0 || dynamic_slot >= MAGIC_COUNT) continue;						
							m_DynamicMagicItem[dynamic_slot] += g_arHuanshiTable[magic_slot]->GetMagicItemValue1();
						}					
					}
				}
			}

			if(i == BODY || i == PANTS)	// 80 乏 捞惑狼 癌渴俊绰 漂荐 瓷仿摹啊 乐促.
			{
				if(m_byClass == 3)
				{
					if(m_UserItem[i].sSid < g_arItemTable.GetSize())
					{
						if(g_arItemTable[m_UserItem[i].sSid]->m_byRLevel == 80 && g_arItemTable[m_UserItem[i].sSid]->m_byClass == 1)
						{
							m_DynamicUserData[MAGIC_MAX_HP_UP] += g_arItemTable[m_UserItem[i].sSid]->m_byMPP;
						}
					}
				}
			}
             //////////////////////////////////NEO版本需要开放的
						///是否为10改以上,右手武器装备增加技能点,装备增加血百分比
			if (o_yehuoini[0]->bbzh == 1)
			{
			BYTE bupg = m_UserItem[i].tMagic[ITEM_UPGRADE_COUNT];
			if(bupg>0)
			{
				if(i == RIGHT_HAND)
				{
					bupg += GetGuardianAddWeapUpgrade();//检测是否佩带武器守护
                    if(Is130Wearp(m_UserItem[i].sSid)&&bupg>=5)
					{
						AddMagicPoint((bupg-4)*3);
					}else if(bupg>=10)
					{
						AddMagicPoint((bupg-9)*3);
					}

				}
				else if(i == BODY || i == PANTS||i == HEAD || i == SHOES)
				{
					//130级从3改开始加3%,100级从8改开始改2%血,70级以下从10开始加2%血
					if(Is130Item(m_UserItem[i].sSid)&&bupg>=3)
					{
						AddMaxHPPoint((bupg-2)*3);
					}else if(Is100Item(m_UserItem[i].sSid)&&bupg>=8)
					{
						AddMaxHPPoint((bupg-7)*2);
					}
					else if(bupg>=10)
					{
						AddMaxHPPoint((bupg-9)*2);
					}
				}
			}
		 }
	  }
  }


	nCount = 5;
	for(i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++)	// E-Body Magic 加己 眠啊
	{
		if(m_UserItem[i].sSid >= 0)			
		{					
			//改数不合法直接删除
			if(m_UserItem[i].tMagic[5]>15)
			{
				ReSetItemSlot(&m_UserItem[i]);
				continue;
			}			
			if(m_UserItem[i].tIQ == MAGIC_ITEM || m_UserItem[i].tIQ == SUPER_EBODY_ITEM)//蓝色和超级机械15
			{
				for(j = 0; j < nCount; j++)			// 浇吩 荐父怒 倒赴促. 
				{				
					magic_slot = m_UserItem[i].tMagic[j];
					if(magic_slot < 0 || magic_slot >= g_arEBodyTable.GetSize()) continue;
					
					dynamic_slot = g_arEBodyTable[magic_slot]->m_sSubType;
					if(dynamic_slot < 0 || dynamic_slot >= EBODY_MAGIC_COUNT) continue;
					
					m_DynamicEBodyData[dynamic_slot] += g_arEBodyTable[magic_slot]->m_sChangeValue;
				}
			}
			BYTE upg = m_UserItem[i].tMagic[5];
			{
			if(m_UserItem[i].sSid==987 ||m_UserItem[i].sSid==1270)
			{
				//最终伤增加 前9改*6,后面*9加一点技能点
				if(m_UserItem[i].sSid==987)
				{
					if(upg<=9)
					{
					m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= upg*6;
					}else{
					m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= (upg-9)*9+9*6;
								//加技能点
						AddMagicPoint(upg-9);
					  }
				}else if(m_UserItem[i].sSid==1270)						
					{//超级机械加42基础,再加1-7改每改的加12最终伤害,8改开始加15最终伤害,再加一点技能点
						if(upg<=7)
						{
							m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= 12*upg+42;
						}
						else
						{
							m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= 15*(upg-7)+7*12+42;
							AddMagicPoint(upg-7);
						}
					}
				}else
				{ 
						//超级机械 14伤害减少,1-7改每改减少3最终攻击减少,加10血,8改开始加4点攻击力减少,15点血
						//m_DynamicMagicItem[5]=m_DynamicMagicItem[5] + m_UserItem[i].tMagic[5]*2;
					if(IsSuperEbodyItem(m_UserItem[i].sSid))
					{
						if(upg<=7)
						{
							m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= upg*3+14;
								//加10点血
							m_DynamicUserData[MAGIC_MAX_HP_UP]+=upg*10;
						}else
						{
							m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= (upg-7)*4+7*3+14;
								//加15点血
							m_DynamicUserData[MAGIC_MAX_HP_UP]+=(upg-7)*15+70;
						}

				    }else
					{
							//最终伤减少前9改*2,后*3加hp5点非5%
							if(upg<=9)
							{
								m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= upg*2;
							}else
							{
								m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= (upg-9)*3+9*2;
								//加5点血
								m_DynamicUserData[MAGIC_MAX_HP_UP]+=(upg-9)*5;
							}
						}
				
			   	}
			}
		 }
	  }

  
	//统计守护属性
    GetMagicItemSetting_38();
	GetSkillSetting_130();
	GetSkillSetting_140();
	GetSkillSetting();
	GetJiSetting();
	//宠物技能
	GetBabySetting();
	
   //百级上的防魔防 血 回避幻石。
    m_DynamicUserData[MAGIC_DEFENSE_UP] +=m_DynamicMagicItem[H_DEFINE];
	m_DynamicUserData[MAGIC_PSI_RESIST_UP] +=m_DynamicMagicItem[H_PSI_DEFINE];
	m_DynamicUserData[MAGIC_MAX_HP_UP] +=m_DynamicMagicItem[H_HP];
	m_DynamicUserData[MAGIC_AVOID_RATING_UP] +=m_DynamicMagicItem[H_HUIBI];	



	// 家葛己 厩技荤府 眉农
	m_bRingOfLife		= FALSE;
	m_bNecklaceOfShield	= FALSE;
	m_bEarringOfProtect = FALSE;
	m_bCBOff	    	= FALSE;
        for(i = 0; i < MAX_ACCESSORI; i++)
	{
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_RING_OF_LIFE)		m_bRingOfLife = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_NECKLACE_OF_SHIELD)	m_bNecklaceOfShield = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_PROTECT)	m_bEarringOfProtect = TRUE;
	        if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_FEIFEI)	m_bCBOff = TRUE;
        }

	if(m_bNecklaceOfShield == TRUE) 
	{
		AddAbnormalInfo(ABNORMAL_SHIELD);
		if(bOldShield == FALSE)
		{
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);

			SendInsight(TempBuf, TempBuf.GetLength());
		}
	}
	else
	{
		if(bOldShield == TRUE && m_dwShieldTime == 0)
		{
			DeleteAbnormalInfo(ABNORMAL_SHIELD);
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);

			SendInsight(TempBuf, TempBuf.GetLength());
		}
	}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>翅膀戒子
	    if(isGuildRen == TRUE || m_bCBOff == TRUE) 
	{
	//m_dwAbnormalInfo_	= 448;
	m_dwAbnormalInfo_ |= ABNORMAL_POISON;
	FreedomCB();
		}
	else 
	{
		if(isGuildRen == FALSE || m_bCBOff == FALSE)
		{
	m_dwAbnormalInfo_	= ABNORMAL_NONE;
	m_dwAbnormalInfo_ |= ABNORMAL_POISON;
	FreedomCB();
		}
	}	
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if(m_dwMaxHPUpTime)	SetUserToMagicUser(1);	// 蜡历狼 扁夯瓷仿摹 函悼蔼阑 历厘茄促.
	else SetUserToMagicUser();	
	//if(m_iSkin == 1 || m_iSkin == 2) m_sDramageUp = true;
	//CheckMagicItemMove();
}
void USER::GetJiSetting() //计算机甲
{	
	int i = 0;
	int max_count = 2;
	bool isatt = false;
	if(m_UserItem[39].sSid != 1184 && m_UserItem[39].sBullNum <=0 )	return ;
	if(m_UserItem[39].tIQ!=13)	return ;
	if(m_iSkin != 5 && m_iSkin != 8 && m_iSkin != 170) return;
		////////////////////////////////////////////////机甲重新计算
	if(m_UserItem[39].tMagic[7] == 3) //机甲加抗魔
	m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=3;

	if(m_UserItem[39].tMagic[7] == 2)  //机甲加防御
	 m_DynamicUserData[MAGIC_DEFENSE_UP]+=4;
    //if(m_dwFaNuTime != 0)
	//{
	//	m_DynamicUserData[MAGIC_DAMAGE_UP]+=50;//(short)(m_DynamicGuardianData[MAGIC_DAMAGE_UP]*0.1);
	//	m_DynamicUserData[MAGIC_DEFENSE_UP]+=30;//(short)(m_DynamicGuardianData[MAGIC_DEFENSE_UP]*0.1);
	//}	
	int att_def = m_UserItem[39].sCount;
	int count = m_UserItem[39].tMagic[5];
	if(att_def < 0 || att_def > 2)	return;
	if(att_def == 1) max_count = 3;
	else if(att_def ==2)
	{
		max_count = 5;
		isatt = true;
	}
	if(max_count == 0)	return;

	short s_2 = 12;
	short s_3 = 6;
	short s_4 = 70;
	short s_5 = 10;
	short s_6 = 1;
        if(m_dwTransTime != 0){
		if(isatt) //攻击模式  3G外观  狂暴中
		{
			if(count == 1){
				s_2 = 15;	s_3 = 10;	s_4 = 50;	s_5 = 25;	s_6 = 3;
			}
			
			else if(count == 2 )
			{
				if( m_UserItem[39].tMagic[7] == 7) //3G外观
				{
				s_2 = 65;	s_3 = 45;	s_4 = 280;	s_5 = 80;	s_6 = 4;
				
				}
				else //正常外观
			 {
                s_2 = 20;	s_3 = 15;	s_4 = 80;	s_5 = 40;	s_6 = 4;
				
				}
			}
			else
			{
				/*s_2 = 10;	s_3 = 5;	s_4 = 30;*/	s_5 = 18;	s_6 = 2;
			}
		}else{ //防御模式    狂暴中
			if(count == 1){
				s_2 = 30;	s_3 = 20;	s_4 = 150;				
			}
			else if(count == 2)
			{
				if( m_UserItem[39].tMagic[7] == 7)
				
				{
				s_2 = 90;	s_3 = 60;	s_4 = 400;	s_5 = m_DynamicUserData[MAGIC_DAMAGE_UP]+= 39,m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+= 39;	
			}

				else
			{
			    s_2 = 45;	s_3 = 30;	s_4 = 200;	
			}
		 }
			else
			{
				s_2 = 20;	s_3 = 10;	s_4 = 100;				
			}		
		}
	}else{
		if(isatt){  
			if(count == 1)
			{
				s_2 = 10;	s_3 = 6;	s_4 = 30;	s_5 = 18;	s_6 = 2;
			}

			
		    else if(count == 2 )//攻击模式  3G外观
			{
				if( m_UserItem[39].tMagic[7] == 7)
				{
				s_2 = 45;	s_3 = 30;	s_4 = 200;	s_5 = 50;	s_6 = 3;
			}
				else
	         {
              s_2 = 15;	s_3 = 10;	s_4 = 50;	s_5 = 25;	s_6 = 3;
			  
				}
			}
			else
			{
				s_2 = 6;	s_3 = 3;	s_4 = 20;	s_5 = 10;	s_6 = 1;
			}
		}
		
		
		else{
			if(count == 1){
				s_2 = 20;	s_3 = 12;	s_4 = 100;				
			}
			else if(count == 2 )
			{
				if( m_UserItem[39].tMagic[7] == 7)
				{ 
				   s_2 = 60;	s_3 = 40;	s_4 = 300;	s_5 = m_DynamicUserData[MAGIC_DAMAGE_UP]+= 24,m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+= 24;
				}
				else 
				{

				s_2 = 30;	s_3 = 20;	s_4 = 150;		
				
				}
			}
			
			else
			{
				s_2 = 12;	s_3 = 6;	s_4 = 70;				
			}		
		}		
	}
	
	int Top4sx = 0;
	int damage_up = GetMaxDamage(m_byClass); //最大攻击	
	short psiattack_up = GetUserSpellAttack(false);//mo gong
	for(i = 0; i < max_count; i++)
	{
		if(m_UserItem[39].tMagic[i] != 0)	Top4sx = m_UserItem[39].tMagic[i];
		if(Top4sx >= 1 && Top4sx <= 18)
		{
			switch(Top4sx)
			{
				case 1://移动速度阶段
				case 7:
				case 13:
					break;
				case 2:	case 8:	case 14://防御	
					m_DynamicUserData[MAGIC_DEFENSE_UP]+= s_2;						
					break;
				case 3:	case 9: case 15:
					m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=s_3;
					break;
				case 4:	case 10: case 16:// HP1%					
					m_DynamicUserData[MAGIC_MAX_HP_UP] += s_4;							
					break;
			     case 5:	case 11: case 17: //损伤					
					m_DynamicUserData[MAGIC_DAMAGE_UP]+= s_5; //机甲加损伤
					m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+= s_5; //法师机甲加损伤
					break;
			//	case 6:	case 12: case 18:
			//		m_DynamicUserData[MAGIC_DAMAGE_UP] += (short)(damage_up*s_6/100);
			//		m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (short)(damage_up*s_6/100);
			//		break;					

			}
		}
	}
	int mingzhong = 0;
	int huibi = 0;
	short sMaxDamage = 0;
	for(i = 8; i < 11; i++)
	{
		int shuxing = m_UserItem[39].tMagic[i];
		if(shuxing >=1 && shuxing <= 20)
		{
			switch(shuxing)
			{		
				case 1:				
				case 2:				
				case 3:
				case 4:
				case 5:			
				case 6://命中1 - 6
					mingzhong = GetHitRate(m_UserItem[RIGHT_HAND].sSid, m_byClass);
					m_DynamicUserData[MAGIC_ATTACK_RATING_UP]+=  (short)(mingzhong * (double)(shuxing * 1 / 100.0));				
					break;
				case 7: 
				case 8:			
				case 9:				
				case 10:			
				case 11:			
				case 12://回避1- 6
					huibi = GetAvoid();
					m_DynamicUserData[MAGIC_AVOID_RATING_UP]+= (short)(huibi * (double)((shuxing-6) * 1 / 100.0));	
					break;
				case 13:
				case 14:			
				case 15:				
				case 16://攻击百分比增加-4 6 8 
					sMaxDamage = GetMaxDamage(m_byClass);				
					m_DynamicUserData[MAGIC_DAMAGE_UP]+= (short)(sMaxDamage * (double)((shuxing-12) * 2 / 100.0));	
					m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+= (short)(sMaxDamage * (double)((shuxing-12) * 2 / 100.0));
					break;
				case 17:			
				case 18:
				case 19:
				case 20://无视防御力3 6 9 12
					//dVital_ = (shuxing-16) * 3 / 100.0;
					break;
				
			}
		}
	}
}

//统计守护属性.
void USER::GetMagicItemSetting_38()
{
	if(m_UserItem[TOTAL_ITEM_NUM-2].sSid==-1)
		return ;
	if(m_UserItem[TOTAL_ITEM_NUM-2].sDuration<=0)
		return ;
	if(m_UserItem[TOTAL_ITEM_NUM-2].tIQ!=9) //看看是不是守护属性 不是返回
		return ;
	switch (m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]){//计算守护本身属性
			case 1://1 损伤增加2
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+2;
				break;
			case 2:// 2 损伤增加3
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+3;
				break;
			case 3:// 3 损伤增加5
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+5;
				break;
			case 4://4  武器等级增加1
				if(m_byClass != 1) m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+16;
				if(m_byClass == 1) m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+= 4;
				break;
			case 5://5  武器等级增加2
				if(m_byClass != 1) m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+32;
				if(m_byClass == 1) m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+= 8;
				break;
			case 6:// 6 武器等级增加3
				if(m_byClass != 1) m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+48;
				if(m_byClass == 1) m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+= 16;
				break;
			case 7:// 7 将对方生命2转化成自己生命
				break;
			case 8://8将对方生命5转化成自己生命
				break;
			case 9://9 将对方生命10转化成自己生命
				break;
			case 10:// 10对周围造成2%伤害
				break;
			case 11://11 对周围造成5%伤害
				break;
			case 12://12 对周围造成10%伤害
				break;
			case 13://13 防御增加3
					m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+3;
				break;
			case 14://14 防御增加6
					m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+6;
				break;
			case 15://15 防御增加10
					m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+10;
				break;
			case 16://16魔法防御增加3
				m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+3;
				break;
			case 17:// 17魔法防御增加6
				m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+6;
				break;
			case 18:// 18魔法防御增加10
				m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+10;
				break;
			case 19://19将敌人对自己攻击伤害2%反射到敌人身上
				break;
			case 20://20将敌人对自己攻击伤害3%反射到敌人身上
				break;
			case 21://21将敌人对自己攻击伤害5%反射到敌人身上
				break;
			case 22://22 打怪经验值提高2%
				break;
			case 23:// 23打怪经验值提高3%
				break;
			case 24://24 打怪经验值提高5%
				break;
			case 25:// 25 金币提高10%
				break;
			case 26:// 26 金币提高20%
				break;
			case 27:// 27 金币提高50%
				break;
			case 28:// 28 所有技能增加1
				m_DynamicUserData[MAGIC_ALL_SKILL_UP]=m_DynamicUserData[MAGIC_ALL_SKILL_UP]+1;
				break;
			case 29://29所有技能增加2
				m_DynamicUserData[MAGIC_ALL_SKILL_UP]=m_DynamicUserData[MAGIC_ALL_SKILL_UP]+2;
				break;
			case 30://30所有技能增加3
				m_DynamicUserData[MAGIC_ALL_SKILL_UP]=m_DynamicUserData[MAGIC_ALL_SKILL_UP]+3;
				break;
			case 31://31掉落物品提高2%
				break;
			case 32:// 32掉落物品提高5%
				break;
			case 33://33掉落物品提高10%
				break;
			case 34://34攻击力增加10
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+10;
				break;
			case 35:// 35攻击力增加20
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+20;
				break;
			case 36://36攻击力增加30
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+30;
				break;
			case 37://37吸取对方生命10
				break;
			case 38:// 38吸取对方生命20
				break;
			case 39://39吸取对方生命30
				break;
			case 40://40生命增加10
				m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+10;
				break;
			case 41://41生命增加20
				m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+20;
				break;
			case 42://  42生命增加50
				m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+50;
				break;
				
	}
	for(int i=0;i<3;i++){
		switch (m_UserItem[TOTAL_ITEM_NUM-2].tMagic[1+i]){//计算守护666属性
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6://每次加点3防
				m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+m_UserItem[TOTAL_ITEM_NUM-2].tMagic[1+i]*3;
				break;
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12://每级加15血
				m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[1+i]-6)*15;
				break;
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18://每级增加5抗AGIC_DEFENSE_UP
				m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[1+i]-12)*5;
				break;
		 default:
		 	break;
		}

	}
	
	
}
// 获取武器守护增加滴武器改数
int USER::GetGuardianAddWeapUpgrade()
{
	if(m_UserItem[TOTAL_ITEM_NUM-2].sSid==-1)
		return 0;
	if(m_UserItem[TOTAL_ITEM_NUM-2].sDuration<=0)
		return 0;
	if(m_UserItem[TOTAL_ITEM_NUM-2].tIQ != 9) 
		return 0;
	switch (m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0])
	{
	case 4://4  武器等级增加
		return 1;
		break;
	case 5://5  武器等级增加
		return 2;
		break;
	case 6:// 6 武器等级增加
		return 3;
		break;
	}
	return 0;
}	

//计算130技能
void USER::GetSkillSetting_130()
{
	m_Hit=0;
	m_Avoid=0;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if( m_dwBFindTime > 0 && m_iSkin == 1	)		//狼变加攻击
	{
		m_Hit +=20;                                                                             //命中
		m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+=15;     //魔法损伤
		m_DynamicUserData[MAGIC_DAMAGE_UP]+=15;       //损伤
		
	}
	//if((m_UserItem[39].tMagic[7] == 7) && (m_iSkin == 8 || m_iSkin == 170))
     //m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+150;

	if( m_dwBFindTime > 0 	)		//熊变加防御
	//if( m_dwBFindTime > 0 && m_iSkin == 2	)
	{
		m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+100;		//生命增加200
        m_DynamicUserData[MAGIC_DEFENSE_UP]+=15;      //防御
		m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=15;    //抗魔
	}
    if(  m_UserItem[1].sSid == 1489)
	{
		CBufferEx TempBuf;
		TempBuf.Add(SET_SPEED_MONSTER);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add(300); //移动速度
		SendInsight(TempBuf, TempBuf.GetLength());
		SightRecalc();
	}
	if(m_dwGuild> 0)
    {
		m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+50;		

	}
	if(m_dwAutoMoney>=0 && m_dwAutoMoney<=10)
			{
			for (int i=1;i<11;i++)
               {
				if(m_dwAutoMoney == i)		
	             {
					  if(m_byClass == STAFF)
						  {
					
					  m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+=10*i;
					      }
					  else{
						  m_DynamicUserData[MAGIC_DAMAGE_UP]+=10*i;
						  }
				 }
              }
			 } 
	BYTE Szb1=m_UserItem[0].tMagic[5];
	BYTE Szb2=m_UserItem[1].tMagic[5];
	BYTE Szb3=m_UserItem[8].tMagic[5];
	BYTE Szb4=m_UserItem[9].tMagic[5];
	
	    if(Szb1>=7 && Szb2>=7 && Szb3>=7 && Szb4>=7  )     //7改加回避
			{
		     if(Szb1==7|| Szb2==7|| Szb3==7|| Szb4==7)
					{ 
		       m_DynamicUserData[MAGIC_AVOID_RATING_UP]+=10;  
		             }
	        }
		 if(Szb1>=8 && Szb2>=8 && Szb3>=8 && Szb4>=8  )    //8改加回避加魔抗
			{
				if(Szb1==8|| Szb2==8|| Szb3==8|| Szb4==8)
					{ 
		            m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=25;
		            m_DynamicUserData[MAGIC_AVOID_RATING_UP]+=10;

					}

	        }
		 if(Szb1>=9 && Szb2>=9 && Szb3>=9 && Szb4>=9  )
			{
			if(Szb1==9|| Szb2==9|| Szb3==9|| Szb4==9)
					{ 	
		           m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+50;		
	               m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=25;
		           m_DynamicUserData[MAGIC_AVOID_RATING_UP]+=10;
                    }
	        }
		 if(Szb1>=10 && Szb2>=10 && Szb3>=10 && Szb4>=10 )
			{
		      m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+100;		
	          m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=25;
		      m_DynamicUserData[MAGIC_AVOID_RATING_UP]+=10;

	        }
		 if(m_dwGuild != -1)
		     {
			 m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+50;
		     }//有军团的，多给50血
		 if(m_UserItem[2].sSid==690 && m_UserItem[3].sSid==730 )
			{
				if( m_UserItem[6].sSid==743 || m_UserItem[7].sSid==743)
                {
		      m_DynamicUserData[MAGIC_DAMAGE_UP]+=30;		
	          m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+=20;
		      AddMagicPoint(5);
                }
	        }

	if (m_byClass == BRAWL ) //修复仲裁
	m_DynamicUserData[MAGIC_PHY_ATTACK_DOWN]+= m_sLevel* 2;
	
	if (m_byClass == FIREARMS ) 
	m_DynamicUserData[MAGIC_PHY_ATTACK_DOWN]+= m_sLevel* 3;

	if (m_dwSJTime !=0 ) AddMagicPoint(10); //神迹药水
	 
		

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int maxslevel = 40;
	int iCount,sLevel;
	int nClassPos ;
	if(m_byClass == JUDGE)
	{
		nClassPos = 16;
	}else
	{
		nClassPos = 3*m_byClass;
	}

	//机甲的技能程序卡属性生效
    if(m_UserItem[39].sSid != 0 && m_iSkin > 2)
//	if(m_UserItem[39].sSid != 0 && (m_iSkin == 5 || m_iSkin == 8 || m_iSkin == 170) )
	{
		int SuitValue = 0;
		if(m_UserItem[39].tMagic[10] != 0) SuitValue = m_UserItem[39].tMagic[10];
		if(SuitValue >= 21 && SuitValue <= 44)
		{		
			switch(SuitValue)
			{
				    case 21:
						m_DynamicSuit[QS0_130_0] += 1;
						break;
					case 22://斗技能增加1-2
						m_DynamicSuit[QS0_130_0] += 2;
						break;
					case 23:
						m_DynamicSuit[QS0_130_1] += 1;
						break;
					case 24://格斗职业HP增加 1-2
						m_DynamicSuit[QS0_130_1] += 2;
						break;
					case 25:
						m_DynamicSuit[QS0_130_2] += 1;
						break;
					case 26://格斗技能增加 1-2
						m_DynamicSuit[QS0_130_2] += 2;
						break;
					case 27:
						m_DynamicSuit[FS1_130_3] += 1;
						break;
					case 28://魔法职业技能增加 1 -2
						m_DynamicSuit[FS1_130_3] += 2;
						break;
					case 29:
						m_DynamicSuit[FS1_130_4] += 1;
						break;
					case 30://魔法职业HP增加 1-2
						m_DynamicSuit[FS1_130_4] += 2;
						break;
					case 31:
						m_DynamicSuit[FS1_130_5] += 1;
						break;
					case 32://魔法职业回避率增加 1 - 2
						m_DynamicSuit[FS1_130_5] += 2;
						break;
					case 33:
					m_DynamicSuit[JS2_130_6] += 1;
						break;
					case 34://刀剑职业HP增加 1
						m_DynamicSuit[JS2_130_6] += 2;
						break;
					case 35:
						m_DynamicSuit[JS2_130_7] += 1;
						break;
					case 36://刀剑职业技能增加 1
						m_DynamicSuit[JS2_130_7] += 2;
						break;
					case 37:
						m_DynamicSuit[JS2_130_8] += 1;
						break;
					case 38://刀剑职业魔法增加 1
						m_DynamicSuit[JS2_130_8] += 2;
						break;
					case 39:
					m_DynamicSuit[QS3_130_9] += 1;
						break;
					case 40://枪械职业HP增加 1?
						m_DynamicSuit[QS3_130_9] += 2;
						break;
					case 41:
						m_DynamicSuit[QS3_130_10] += 1;
						break;
					case 42://枪械职业魔法增加 
						m_DynamicSuit[QS3_130_10] += 2;
						break;
					case 43:
						m_DynamicSuit[QS3_130_11] += 1;
						break;
					case 44: //枪械职业回避率增加 1
						m_DynamicSuit[QS3_130_11] += 2;
						break;
			}
		}
	}	

	for (int i=nClassPos;i<(nClassPos+3);i++)
	{
		switch(i)
		{
			case 0: ////致命之聪 //增加命中
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+= m_DynamicSuit[QS0_130_0];
				if(sLevel>0&&sLevel<=maxslevel){
					m_Hit=(sLevel-1)*10+30;
				}
				break;
			case 1:////生命之光 增加生命
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+= m_DynamicSuit[QS0_130_1];
				if(sLevel>0&&sLevel<=maxslevel){
					iCount=(sLevel-1)*20+150;
					m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+iCount;

				}
				break;
			case 2: 	// 超级罡气   改为 第一级。增加 30抗 30防 以后每级 增加 3点抗 3点防
					sLevel=m_UserSkill_[i].tLevel;
					sLevel+= m_DynamicSuit[QS0_130_2];
				if(sLevel>0&&sLevel<=maxslevel){
					iCount=(sLevel-1)*6+30;
					m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+iCount;
					m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+iCount;
				}
				break;
			case 3://	//辅助 魔法  无效

				break;
			case 4:	//生命之光 增加生命
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+= m_DynamicSuit[FS1_130_4];
				if(sLevel>0&&sLevel<=maxslevel){
					iCount=(sLevel-1)*12+90;
					m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+iCount;
				}
				break;
			case 5://灵敏之慧
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+=m_DynamicSuit[FS1_130_5];
				if(sLevel>0&&sLevel<=maxslevel){
					m_Avoid=(sLevel-1)*4+24;
				}
				break;
			case 6:	//生命之光 增加生命
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+=m_DynamicSuit[JS2_130_6];
				if(sLevel>0&&sLevel<=maxslevel){
					iCount=(sLevel-1)*16+120;
					m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+iCount;
				}
				break;
			case 7:	//护体罡气  第一级 增加防 40  每级增加1防
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+=m_DynamicSuit[JS2_130_7];
				if(sLevel>0&&sLevel<=maxslevel){
					iCount=(sLevel-1)*1+40;
					m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+iCount;
				}
				break;
			case 8:		// 魔法抗斥  第一级 增加抗 42  每级增加3抗
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+=m_DynamicSuit[JS2_130_8];
				if(sLevel>0&&sLevel<=maxslevel){
					iCount=(sLevel-1)*3+42;
					m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+iCount;
				}
				break;
			case 9:	//生命之光   第一级 生命+105 以每级增加14点生命
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+=m_DynamicSuit[QS3_130_9];
				if(sLevel>0&&sLevel<=maxslevel){
					iCount=(sLevel-1)*14+105;
					m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+iCount;
				}
				break;
			case 10:	// 魔法抗斥  第一级 增加抗 45  每级增加5抗
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+=m_DynamicSuit[QS3_130_10];
				if(sLevel>0&&sLevel<=maxslevel){
					iCount=(sLevel-1)*8+65;
					m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+iCount;
				}
				break;
			case 11://灵敏之慧    第一级 增加 50   每级增加7点
				sLevel=m_UserSkill_[i].tLevel;
				sLevel+=m_DynamicSuit[QS3_130_11];
				if(sLevel>0&&sLevel<=maxslevel){
					m_Avoid=(sLevel-1)*7+50;
				}
				break;
			case SKILL_130_TI_ZHI_QIANG_HUA://仲裁体质强化 15体质为起点,每级加二点体质 
				{
					sLevel=m_UserSkill_[i].tLevel;
					if(sLevel>0&&sLevel<=20)
					{
						m_DynamicUserData[MAGIC_CON_UP]+=(sLevel-1)*2+15;
					}
					break;
				}
			case SKILL_130_REN_MI_FU_HA:	//仲裁生命护法
				{
					break;
				}
			case SKILL_130_MO_FU_KANG_CHI_4://仲裁抗斥 37魔法抗斥 每级加4点
				{
					sLevel=m_UserSkill_[i].tLevel;
					if(sLevel>0&&sLevel<=20)
					{
						iCount=(sLevel-1)*4+37;
						m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=iCount;
					}
					break;
				}
    

			default:
				break;

		}

	}
   

	
	 if(m_byClass == BRAWL) //如果是拳
	{
		m_Hit +=15; //命中
		
	}
	if(m_byClass == STAFF) //如果是法生命加100
	{
	 m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+100;
	}
	if(m_byClass == EDGED) //如果是刀
	{
		m_Hit +=25;
		
		//m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+o_yehuoini[0]->daohp;
	}
	if(m_byClass == FIREARMS) //如果是枪
	{
		m_Hit +=0;
		
	//	m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+o_yehuoini[0]->qianghp;
	}
	if(m_byClass == JUDGE) //如果是仲裁
	{
		m_Hit +=150;
		//m_Avoid +=o_yehuoini[0]->zhonghb;
		m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+300;
	}
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
    if( (m_UserItem[2].sSid == 733) && (m_UserItem[3].sSid == 735) && (m_UserItem[6].sSid == 818)  && (m_UserItem[7].sSid == 818) )      //月神套
	{
        AddMagicPoint(1);
     }
  }
//计算140技能
void USER::GetSkillSetting_140()
{
	if(m_sLevel<140)
		return ;
	short sLevel;
	for (int i=0;i<2;i++)
	{
		switch(m_UserSkill140[i].sSid)
		{
		case 0x0c://最终伤害减少
			{
				//最终伤害力减少30,36,43,51,60,70,81,93,106,120
				static int def[]={0,30,36,43,51,60,70,81,93,106,120};
				sLevel = m_UserSkill140[i].tLevel;
				if(sLevel>0&&sLevel<=10)
				{
					m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+=def[sLevel];
				//	m_DynamicMagicItem[5]+=def[sLevel];

				}
				break;
			}
		case 0x0d://最终伤害猎杀增加
			{
				//最终伤害猎杀增加18,27,36,48,60,75,90,108,126,147
				static int dam[]={0,18,27,36,48,60,75,90,108,126,147};
				sLevel = m_UserSkill140[i].tLevel;
				if(sLevel>0&&sLevel<=10)
					m_Dynamic140[0]+=dam[sLevel];
				break;
			}
		case 0x0e://未知
			break;
		case 0x0f://定时
			break;
		}
	}
}
////////////幻石属性////////////
int USER::GetMagicItem_100(byte tMagic)
{
	if (tMagic<3||tMagic>52)
		return 0;
	int iDynamic=(tMagic-3)/10;
	int isLevel= (tMagic-3)%10+1;
	int iValue;
	switch(iDynamic){
		case 0://防+5
			iValue=5*isLevel; break;
		case 1://魔法防+5
			iValue=5*isLevel; break;
		case 2://23 生命增加10
			iValue=10*isLevel; break;
		case 3://回+4 不开放
			iValue=0; break;
		case 4://魔法伤减少 10
			iValue=10*isLevel; break;
		case 5://最终伤减少 5
			iValue=5*isLevel; break;
		case 6://物理伤减少 10
			iValue=10*isLevel; break;
							 
	}
	m_DynamicMagicItem[iDynamic]=m_DynamicMagicItem[iDynamic]+iValue;
	return iValue;

}

///////////////////////////////////////////////////////////////////////////
//	Log Out
//
void USER::LogOut()
{
	if(m_bLogOut == TRUE) return;
	if(m_state != STATE_GAMESTARTED) return;	// 捞寇 STATE_CONNECTED殿 老锭绰 秦寸 檬扁拳啊 皋葛府俊绝栏骨肺 力寇秦具茄促.

	m_iHair=0;
	
	m_bLogOut = TRUE;
//	USER *pUser = NULL;
												// 老馆 鞘靛傈 吝捞扼搁..		
	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)		
	{
		if(m_bGuildMaster) 
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// 亲汗
		}
	}

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // 促弗 蜡历甸篮 够窍唱 眉农..

	if(m_tQuestWar == GUILD_WARRING) g_QuestEventZone.CheckUserStateInEventZone(m_curz);//CheckQuestEventZoneWarEnd();

	if(m_bNowBuddy == TRUE)				// 滚叼吝捞搁 烹焊茄促.
	{
		for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(m_MyBuddy[i].uid == m_uid + USER_BAND) SendBuddyUserLeave(i);
		}
	}

	// 芭贰吝捞搁 芭贰秒家 贸府
	if(m_bNowTrading == TRUE) 
	{
		BYTE result = 0x00;
		USER *pTradeUser = NULL;
		if(m_iTradeUid != -1)	pTradeUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTradeUser != NULL)	pTradeUser->SendExchangeFail(result, (BYTE)0x05);
	}

	// 肺锯烦喉 贸府
	ExitRoyalRumble();

	// 焊龋内靛 眠啊
	int nRet1 = 0, nRet2 = 0, nRet3 = 0;
	nRet1 = UpdateMemBankDataOnly();
	nRet3 = UpdateMemAccountBankDataOnly();
	nRet2 = UpdateUserData(TRUE);

	if(nRet1 == 1 && nRet2 == 1 && nRet3 == 1) ReInitMemoryDB();
	else
	{
		if(nRet1 == 0)
		{
			UpdateMemBankDataOnly();
		}
		if(nRet2 == 0)
		{
			UpdateUserData(TRUE);
		}
		if(nRet3 == 0)
		{
			UpdateMemAccountBankDataOnly();
		}
		ReInitMemoryDB();
	}

//	SessionLogOut();			// CURRENTUSER TABLE俊辑 肺弊甫 瘤款促.
	// 焊龋内靛 ( x, y, z蔼捞 裹困甫 逞绰蔼栏肺 函且荐乐促 )
	MAP* pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) goto go_result;

	pMap = g_zone[m_ZoneIndex];		if( !pMap ) goto go_result;

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) goto go_result;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) goto go_result;

	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == USER_BAND + m_uid ) 
		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);

go_result:
	SendMyInfo(TO_INSIGHT, INFO_DELETE);

	m_state = STATE_LOGOUT;

	g_pMainDlg->BridgeServerUserLogOut( m_uid, m_strUserID );
}

void USER::ZoneLogOut(int save_z, int save_x, int save_y)
{
	if(m_bLogOut == TRUE) return;
	if(m_state != STATE_GAMESTARTED) return;	// 捞寇 STATE_CONNECTED殿 老锭绰 秦寸 檬扁拳啊 皋葛府俊绝栏骨肺 力寇秦具茄促.

	m_bLogOut = TRUE;
//	USER *pUser = NULL;
												// 老馆 鞘靛傈 吝捞扼搁..		
	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)		
	{
		if(m_bGuildMaster) 
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// 亲汗
		}
	}

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // 促弗 蜡历甸篮 够窍唱 眉农..

	if(m_bNowBuddy == TRUE)				// 滚叼吝捞搁 烹焊茄促.
	{
		for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(m_MyBuddy[i].uid == m_uid + USER_BAND) SendBuddyUserLeave(i);
		}
	}

	// 芭贰吝捞搁 芭贰秒家 贸府
	if(m_bNowTrading == TRUE) 
	{
		BYTE result = 0x00;
		USER *pTradeUser = NULL;
		if(m_iTradeUid != -1)	pTradeUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTradeUser != NULL)	pTradeUser->SendExchangeFail(result, (BYTE)0x05);
	}

	// 肺锯烦喉 贸府
	ExitRoyalRumble();

	int backup_x, backup_y, backup_z;

	backup_z = m_curz;
	backup_x = m_curx;
	backup_y = m_cury;

	m_curz = save_z;
	m_curx = save_x;
	m_cury = save_y;

	// 焊龋内靛 眠啊
	int nRet1 = 0, nRet2 = 0, nRet3 = 0;
	nRet1 = UpdateMemBankDataOnly();
	nRet3 = UpdateMemAccountBankDataOnly();
	nRet2 = UpdateUserData(TRUE);

	if(nRet1 == 1 && nRet2 == 1 && nRet3 == 1) ReInitMemoryDB();
	else
	{
		if(nRet1 == 0)
		{
			UpdateMemBankDataOnly();
		}
		if(nRet2 == 0)
		{
			UpdateUserData(TRUE);
		}
		if(nRet3 == 0)
		{
			UpdateMemAccountBankDataOnly();
		}
		ReInitMemoryDB();
	}

	m_curz = backup_z;
	m_curx = backup_x;
	m_cury = backup_y;

//	SessionLogOut();			// CURRENTUSER TABLE俊辑 肺弊甫 瘤款促.
	// 焊龋内靛 ( x, y, z蔼捞 裹困甫 逞绰蔼栏肺 函且荐乐促 )
	MAP* pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) goto go_result;

	pMap = g_zone[m_ZoneIndex];		if( !pMap ) goto go_result;

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) goto go_result;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) goto go_result;

	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == USER_BAND + m_uid ) 
		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);

go_result:
	SendMyInfo(TO_INSIGHT, INFO_DELETE);

	m_state = STATE_LOGOUT;
}

///////////////////////////////////////////////////////////////////////////
//	Session俊 Log甫 巢扁扁 困秦辑  by zi_gi 2002-03-27
//
void USER::SessionLogOut()
{
	return;
/*	
	if(m_bSessionLogOut) return;
		
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UM_LOGOUT ( \'%s\', %d )}"), m_strAccount, 1 );
//	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call GAMESERVER_LOGOUT_DROIYAN ( \'%s\', %d )}"), m_strAccount, 1 );
	
	hstmt = NULL;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return;
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		return;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
	}

	else if (retcode==SQL_ERROR)
	{
//		DisplayErrorMsg(hstmt);
	}
	else if (retcode==SQL_NO_DATA)
	{
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

	g_DBSession[m_iModSid].ReleaseDB(db_index);

	m_bSessionLogOut = TRUE;

	TRACE("CURRENUSER TABLE Success. \n");
*/
	return;
}
BOOL USER::UpdateUserName(TCHAR* newName)//0509
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];

	SQLINTEGER	sReturn;
	SQLINTEGER	iReturnIndex;

	sReturn			= -1;
	iReturnIndex	= SQL_NTS;

	if(IsReservedID(newName))
	{
		SendSystemMsg("该名字中包含非法字符!请换个名字!",SYSTEM_ERROR,TO_ME);
		return FALSE;
	}
///////////////////角色名带后缀开始//////////////////////////////////////////////////

	CString strTemp = m_strUserID;
	unsigned int iPos = strTemp.Find("[");
	if (iPos <= 0 || iPos > strlen(m_strUserID)-3)
	{
		SendSystemMsg("检测到角色名后缀异常,请联系管理员解决!",SYSTEM_ERROR,TO_ME);
		return FALSE;
	}
	int nLength = strlen(newName);

	newName[nLength] =  m_strUserID[iPos];
	newName[nLength+1] =  m_strUserID[iPos+1];
	newName[nLength+2] =  m_strUserID[iPos+2];
	newName[nLength+3] = '\0';
	nLength += 3;
////////////////////角色名带后缀结束///////////////////////////////////////////////////

//////////////////////////////////检测角色名包含后缀是否重复开始///////////////////
	if( IsExistCharId( newName ) == TRUE )
	{
		SendSystemMsg("该角色名已存在!",SYSTEM_ERROR,TO_ME);
		return FALSE;
	}
//////////////////////////////////检测角色名包含后缀是否重复结束///////////////////
	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_NAME (\'%s\',\'%s\', ? )}"), 
		m_strUserID,	newName); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if(retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		int i=1;
		retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT,SQL_C_LONG, SQL_INTEGER, 0,		0, &sReturn,			0, &iReturnIndex);
		if (retcode!=SQL_SUCCESS) 
			return FALSE;

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			memcpy( m_strUserID, newName, CHAR_NAME_LENGTH );
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
		}
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	BYTE bReturn = (BYTE)sReturn;
	if(bReturn!=0)
	{
		SendSystemMsg("该名字中包含非法字符!请换个名字!",SYSTEM_ERROR,TO_ME);
		return FALSE;
	}
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////
//	努贰胶狼 蜡历沥焊甫 DB肺 诀单捞飘 茄促.
//
BOOL USER::UpdateUserData(BOOL bLogOut)
{
	if(!bLogOut) if(UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strFace[10], strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB], strTel[_TEL_DB];
	TCHAR			strQuickItem[_QUICKITEM_DB];
	TCHAR			strHaveEvent[_EVENT_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));

	::ZeroMemory(strFace, sizeof(strFace));
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));	
	::ZeroMemory(strTel, sizeof(strTel));
	::ZeroMemory(strHaveEvent, sizeof(strHaveEvent));
	
	::ZeroMemory(strQuickItem, sizeof(strQuickItem));
	
    ::CopyMemory(strFace, m_strFace, sizeof(m_strFace));

	UserSkillToDBStr(strSkill);
	UserItemToStr(strItem);
	UserPsiToStr(strPsi);
	UserTelToStr(strTel);
	UserHaveEventDataToStr(strHaveEvent);

	DWORD dwCurTime = ConvertCurTimeToSaveTime();			// 泅犁 矫埃 滚怜阑 悸泼

	SDWORD sFaceLen		= sizeof(strFace);
	SDWORD sSkillLen	= sizeof(strSkill);
	SDWORD sItemLen		= sizeof(strItem);
	SDWORD sPsiLen		= sizeof(strPsi);
	SDWORD sQuickLen	= sizeof(strQuickItem);
	SDWORD sEventLen	= sizeof(strHaveEvent);
	SDWORD sTelLen		= sizeof(strTel);	
	
    m_tPsiOneKind = m_tPsiTwoKind = m_tPsiThreeKind = 0;
	m_dwPsiOneTime = m_dwPsiTwoTime = m_dwPsiThreeTime = 0;

	// Psionic One
	if(m_dwHasteTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_HASTE;
		m_dwPsiOneTime = m_dwHasteTime;
	}
	if(m_dwShieldTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_SHIELD;
		m_dwPsiOneTime = m_dwShieldTime;
	}
	if(m_dwDexUpTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_DEXUP;
		m_dwPsiOneTime = m_dwDexUpTime;
	}
	if(m_dwMaxHPUpTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_HPUP;
		m_dwPsiOneTime = m_dwMaxHPUpTime;
	}
	if(m_dwFastRunTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_FAST_RUN;
		m_dwPsiOneTime = m_dwFastRunTime;
	}
	if(m_dwMindShockTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_MIND_SHOCK;
		m_dwPsiOneTime = m_dwMindShockTime;
	}
	if(m_dwPsiShieldTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_PSI_SHIELD;
		m_dwPsiOneTime = m_dwPsiShieldTime;
	}
	if(m_dwBigShieldTime != 0)
	{
		m_tPsiOneKind = 30;
		m_dwPsiOneTime = m_dwBigShieldTime;
	}
	if(m_dwPiercingShieldTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_PIERCING_SHIELD;
		m_dwPsiOneTime = m_dwPiercingShieldTime;
	}

	// Psionic Two
	if(m_dwAdamantineTime != 0) 
	{
		m_tPsiTwoKind = PSIONIC_ADAMANTINE;
		m_dwPsiTwoTime = m_dwAdamantineTime;
	}
	if(m_dwMightyWeaponTime != 0) 
	{
		m_tPsiTwoKind = PSIONIC_MIGHTYWEAPON;
		m_dwPsiTwoTime = m_dwMightyWeaponTime;
	}
	if(m_dwBerserkerTime != 0) 
	{
		m_tPsiTwoKind = PSIONIC_BERSERKER;
		m_dwPsiTwoTime = m_dwBerserkerTime;
	}

	// Psionic Three
	if(m_dwMindGuardTime != 0) 
	{
		m_tPsiThreeKind = PSIONIC_MIND_GUARD;
		m_dwPsiThreeTime = m_dwMindGuardTime;
	}
	   /////////////////////加字段
    _sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_DATA (\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,\
		?, %d,%d,%d, %d, %d,%d,  %d,%d,  %d, \
		%d,%d,%d,%d,%d,%d, %d,%d,%d,%d, \
		?,?,?,?, %d,%d,\
		%d, %d, ?, %d, %d,\
		?, %d,\
		%d, %d, %d, %d, %d, %d,%d, %d, %d,%d,%d,%d,%d,%d,%d,\
		\'%s\', \
		%d,%d,		%d,%d,		%d,%d)}"), 
		m_strUserID,	m_sSTR,	m_sCON,	m_sDEX,	m_sVOL,	m_sWIS,	m_iSkin, m_iHair, m_sGender,	
		m_curz,	m_curx,	m_cury,		m_dwBuddy,		m_dwGuild, m_dwExp,		m_sPA, m_sSkillPoint,	m_dwXP,
		m_sMaxHP, m_sHP, m_sMaxPP, m_sPP, m_sMaxSP,	m_sSP,		m_dwDN,	m_sCityRank, m_sLevel,	m_byClass,

		m_tAbnormalKind, m_dwAbnormalTime,

		m_bLive, m_iCityValue, m_sKillCount, dwCurTime, 
		m_dwSaintTime, 
		m_dwHiExpTime, m_dwHtExpTime, m_dwMagicFindTime, m_dwMagicFtTime, m_dwNoChatTime, m_dwCloseTime, m_dwAutoMoney,m_dwLingQu, m_dwGuarDianTianShi, m_dwShopPingDN,m_dwVIPTime,m_dwZaiXianTime,m_dwBFindTime,m_dwBHTime,m_dwSJTime,
		m_strLoveName, //--yskang 0.1 
		m_tPsiOneKind, m_dwPsiOneTime,		m_tPsiTwoKind, m_dwPsiTwoTime,		m_tPsiThreeKind, m_dwPsiThreeTime); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if(retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strFace),	0, (TCHAR*)strFace,		0, &sFaceLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strSkill),	0, (TCHAR*)strSkill,	0, &sSkillLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strPsi),		0, (TCHAR*)strPsi,		0, &sPsiLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strQuickItem),	0, (TCHAR*)strQuickItem,	0, &sQuickLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strHaveEvent),	0, (TCHAR*)strHaveEvent,	0, &sEventLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strTel),			0, (TCHAR*)strTel,			0, &sTelLen );

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
		}
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
    
	if( !bQuerySuccess ) return FALSE;
    
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//	剧率蜡历狼 酒捞袍捞悼俊 狼茄 蜡历沥焊甫 DB肺 诀单捞飘 茄促.
//
BOOL USER::UpdateUserExchangeData(USER *pUser)
{ 
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;
	if(UpdateMemUserAll() && pUser->UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB], strUserItem[_ITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strUserItem, sizeof(strUserItem));

	UserItemToStr(strItem);
	pUser->UserItemToStr(strUserItem);

	SDWORD sItemLen			= sizeof(strItem);
	SDWORD sUserItemLen		= sizeof(strUserItem);

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_EXCHANGE_DATA (\'%s\',%d,?,\'%s\',%d,?)}"), 
		m_strUserID, m_dwDN, pUser->m_strUserID, pUser->m_dwDN); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if(retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strUserItem),	0, (TCHAR*)strUserItem,		0, &sUserItemLen );

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
		}
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//	急琶等 某腐磐狼 沥焊甫 啊廉柯促.
//
BOOL USER::LoadUserData(TCHAR *szID)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];
	DWORD			dwMaxDN = 0;
	BYTE			tPoint = 0;
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_USER_DATA (\'%s\')}"), szID);
	
	SQLSMALLINT sSTR, sCON, sDEX, sVOL, sWIS;
	SQLSMALLINT sBasicSTR, sBasicCON, sBasicVOL, sBasicDEX, sBasicWIS;
	SQLUINTEGER	iSkin, iHair, iExp, iXP, iDN;
	SQLINTEGER iCityValue, iGuild;
	SQLSMALLINT	sGender;
	SQLCHAR		strFace[10];
	SQLSMALLINT	sLZ, sLX, sLY, sPA;
	SQLUINTEGER iBuddy;
	SQLSMALLINT	sMaxHP, sHP, sMaxPP, sPP, sMaxSP, sSP, sCityRank, sKillCount;
	SQLSMALLINT sLevel;
	SQLCHAR		byClass, bLive;
	
	SQLSMALLINT	sSkillPoint;

	SQLCHAR		strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB], strHaveEvent[_EVENT_DB], strTel[_TEL_DB];
	
	SQLCHAR		strQuickItem[_QUICKITEM_DB];	// 狞 酒捞袍篮 歹捞惑 历厘窍瘤 臼绰促.

	SQLCHAR		tAbnormalKind;
	SQLUINTEGER iAbnormalTime;

	//--yskang 0.1
	SQLCHAR		strDbLoveName[LOVE_NAME_LENGTH];

	SQLSMALLINT	sChangeClass[CLASS_NUM];

	SQLCHAR		tIsOP;

	SQLUINTEGER iSaintTime;		// 技牢飘 乔纳捞 墨款飘 临捞扁 困秦
	SQLINTEGER	sInd;            //////////// 加字段

	SQLUINTEGER iHiExpTime, iHtExpTime, iMagicFindTime, iMagicFtTime, iNoChatTime, iCloseTime, iAutoMoney,iLingQu, iGunTianShi, iShopPingDN, iVIPTime, iZaiXianTime, iBFindTime, iBHTime, iSJTime;	// 版氰摹2硅, 概流犬啦5硅, 盲陛矫埃

	SQLCHAR		tPsiOneKind, tPsiTwoKind, tPsiThreeKind;			// Haste, Shield, Dex Up, Max HP Up, Fast Run, Mind Shock, Psi Shield, Piercing Shield
	SQLUINTEGER	iPsiOneTime, iPsiTwoTime, iPsiThreeTime;			// Adamantine, MightyWeapon, Berserker
																	// Mind Guard

	sSTR = sCON = sDEX = sVOL = sWIS = 0;
	sBasicSTR = sBasicCON = sBasicVOL = sBasicWIS = sBasicDEX = 0;
	sLZ = sLX = sLY = 1;
	sPA = 0;
	iBuddy = iGuild = 0;
	iExp = iXP = iDN = iCityValue = sKillCount = 0;
	sMaxHP = sHP = sMaxPP = sPP = sMaxSP = sSP = 0;
	sCityRank = 0;
	sLevel = 1;
	byClass = tIsOP = bLive = 0;

	tAbnormalKind = 0;
	iAbnormalTime = 0;

	iSaintTime = 0;   ////加字段

	iHiExpTime = iHtExpTime = iMagicFindTime = iMagicFtTime = iNoChatTime = iCloseTime = iAutoMoney = iLingQu = iGunTianShi = iShopPingDN = iVIPTime = iZaiXianTime = iBFindTime = iBHTime = iSJTime = 0;

	tPsiOneKind = tPsiTwoKind = tPsiThreeKind = 0;
	iPsiOneTime = iPsiTwoTime = iPsiThreeTime = 0;

	sSkillPoint = 0;

	::ZeroMemory(strFace, sizeof(strFace));
	::ZeroMemory(strSkill, sizeof(strSkill));
//	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));
	::ZeroMemory(strTel, sizeof(strTel));
	::ZeroMemory(strHaveEvent, sizeof(strHaveEvent));
	
	::ZeroMemory(strDbLoveName,sizeof(strDbLoveName));

	memset(strItem, -1, sizeof(strItem));
	memset(strQuickItem, -1, sizeof(strQuickItem));		// 泅犁 狞酒捞袍篮 荤侩窍瘤 臼绰促. 唱吝俊 促弗侩档肺 荤侩窍妨绊 叼厚俊辑 瘤快柳 臼澜

	for(i = 0; i < TOTAL_PSI_NUM; i++)					// 檬扁拳甫 怖 秦具 茄促.
	{
		m_UserPsi[i].sSid = -1;
		m_UserPsi[i].tOnOff = FALSE;
	}

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load User Data !!\n");
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			int i = 1;
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sSTR, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sCON, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sDEX, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sVOL, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sWIS, sizeof(SQLSMALLINT), &sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iSkin,		sizeof(iSkin),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iHair,		sizeof(iHair),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sGender,	sizeof(sGender),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_BINARY,	strFace,	sizeof(strFace),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sLZ,		sizeof(sLZ),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sLX,		sizeof(sLX),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sLY,		sizeof(sLY),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iBuddy,	sizeof(iBuddy),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SLONG,	&iGuild,	sizeof(iGuild),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iExp,		sizeof(iExp),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sPA,		sizeof(sPA),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sSkillPoint,	sizeof(sSkillPoint),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iXP,		sizeof(iXP),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sMaxHP,	sizeof(sMaxHP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sHP,		sizeof(sHP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sMaxPP,	sizeof(sMaxPP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sPP,		sizeof(sPP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sMaxSP,	sizeof(sMaxSP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sSP,		sizeof(sSP),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iDN,		sizeof(iDN),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sCityRank,	sizeof(sCityRank),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sLevel,	sizeof(sLevel),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&byClass,	sizeof(byClass),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_BINARY,	strSkill,	sizeof(strSkill),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY,	strPsi,		sizeof(strPsi),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY,	strItem,	sizeof(strItem),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sChangeClass[0],	sizeof(sChangeClass[0]),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sChangeClass[1],	sizeof(sChangeClass[1]),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sChangeClass[2],	sizeof(sChangeClass[2]),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sChangeClass[3],	sizeof(sChangeClass[3]),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tAbnormalKind,	sizeof(tAbnormalKind),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iAbnormalTime,	sizeof(iAbnormalTime),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_BINARY,	strQuickItem,	sizeof(strQuickItem),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tIsOP,			sizeof(tIsOP),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&bLive,		sizeof(bLive),		&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicSTR, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicCON, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicDEX, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicVOL, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicWIS, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SLONG,  &iCityValue,sizeof(iCityValue),&sInd );

			SQLGetData( hstmt, i++, SQL_C_BINARY, strHaveEvent,	sizeof(strHaveEvent), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sKillCount,	sizeof(sKillCount),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY, strTel,		sizeof(strTel), &sInd );
			
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iSaintTime,sizeof(iSaintTime),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iHiExpTime,		sizeof(iHiExpTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iHtExpTime,		sizeof(iHtExpTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iMagicFindTime,	sizeof(iMagicFindTime),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iMagicFtTime,		sizeof(iMagicFtTime),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iNoChatTime,		sizeof(iNoChatTime),	&sInd );
            SQLGetData( hstmt, i++, SQL_C_ULONG,	&iCloseTime,		sizeof(iCloseTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iAutoMoney,		sizeof(iAutoMoney),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iLingQu,		    sizeof(iLingQu),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iGunTianShi,		sizeof(iGunTianShi),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iShopPingDN,		sizeof(iShopPingDN),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iVIPTime,		    sizeof(iVIPTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iZaiXianTime,		sizeof(iZaiXianTime),	&sInd ); ///加字段
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iBFindTime,		sizeof(iBFindTime),	    &sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iBHTime,		    sizeof(iBHTime),	    &sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iSJTime,		    sizeof(iSJTime),	    &sInd );

			
			SQLGetData( hstmt, i++, SQL_C_CHAR,	strDbLoveName,sizeof(strDbLoveName),	&sInd );	//-- yskang 0.1

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tPsiOneKind,	sizeof(tPsiOneKind),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iPsiOneTime,	sizeof(iPsiOneTime),	&sInd );			

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tPsiTwoKind,	sizeof(tPsiTwoKind),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iPsiTwoTime,	sizeof(iPsiTwoTime),	&sInd );			

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tPsiThreeKind,	sizeof(tPsiThreeKind),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iPsiThreeTime,	sizeof(iPsiThreeTime),	&sInd );	

		}
		else
		{
			DisplayErrorMsg(hstmt);
			retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		//	BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	// Copy User Data
	strcpy(m_strUserID, szID);	// User ID

	ZeroMemory(m_strLoveName,sizeof(m_strLoveName));
	
	if(strlen((char *)strDbLoveName)<1)//-称号显示等级
	{
		
		m_bPseudoString=TRUE;
		strcpy(m_strLoveName,"");
	}
	else
	{
		//-----------------------------------------------------------
		//--yskang 0.6 蜡丰 荤侩磊俊霸父 龋莫 何咯
		if(m_iDisplayType !=5 && m_iDisplayType !=6)
		
			strcpy(m_strLoveName,(char *)strDbLoveName);//--yskang 0.1
		   
		
		else
			strcpy(m_strLoveName,"");
		//------------------------------------------------------------
	}
      //strcpy(strTemp1,strTemp);

	 // sprintf(strTemp,"@9[Lv.%d]%s",m_sLevel,strTemp1);//称号显示等级     
		  

	m_sSTR = sSTR;				// 塞
	m_sCON = sCON;				// 扒碍
	m_sDEX = sDEX;				// 刮酶
	m_sVOL = sVOL;				// 狼瘤
	m_sWIS = sWIS;				// 瘤驱

	m_sTempSTR = (BYTE)sBasicSTR;		// 檬扁 费蔼甫 罐酒柯促.(饭骇促款老锭 靖)
	m_sTempCON = (BYTE)sBasicCON;
	m_sTempDEX = (BYTE)sBasicDEX;
	m_sTempVOL = (BYTE)sBasicVOL;
	m_sTempWIS = (BYTE)sBasicWIS;

	m_iSkin = iSkin;			// 乔何祸
	m_iHair = iHair;			// 赣府祸

	m_curz = sLZ;				// Zone
	m_curx = sLX;				// 困摹 x
	m_cury = sLY;				// 困摹 y

	m_sGender = sGender;		// 己喊

	::CopyMemory(m_strFace, strFace, 10);	// 赣府葛剧

	m_dwBuddy = iBuddy;			// Buddy
	m_dwGuild = iGuild;			// Guild
	m_tFortressWar = GUILD_WAR_AFFTER;
	m_tGuildWar = GUILD_WAR_AFFTER;
	m_tGuildHouseWar = GUILD_WAR_AFFTER;
	m_tQuestWar = GUILD_WAR_AFFTER;

	if( m_dwGuild >= 1 )
	{
		if( !GetAbsentGuildInfo( (int)m_dwGuild ) )
		{
			m_dwGuild = -1;
		}
	}

	if(m_dwGuild >= 1)			// 辨靛俊 措茄 单捞磐甫 檬扁拳茄促.
	{
		SetGuildVariable();
	}

	CheckGuildWar();			// 父距 傍己傈 吝捞扼搁 葛电 蜡历俊霸 舅赴促.(瘤陛篮 傍己傈父)

	m_bFieldWarApply = FALSE;
	m_FieldWarGMUid = 0;		// 鞘靛傈老锭 惑措规 辨付 uid
	m_dwFieldWar = 0;			// 鞘靛傈 矫累捞搁 惑措祈 辨靛 锅龋啊 甸绢柯促.

	m_dwExp	  = iExp;			// Experience

	m_sPA = sPA;				// PA Point

	m_sSkillPoint = sSkillPoint;// Skill Point

	m_dwXP = iXP;				// X Point
	
	m_sLevel	= sLevel;		// 饭骇

	if(iExp > GetNextLevel(sLevel)) iExp = GetNextLevel(sLevel);	// 救傈 内靛.
	m_byClass	= byClass;		// 努贰胶

	m_sMaxHP	= g_sHPConst[m_byClass] * m_sCON + g_sHPLV[m_byClass] * (m_sLevel - 1) + g_sHPAdd[m_byClass];
	m_sMaxPP	= g_sPPConst[m_byClass] * m_sWIS + g_sPPLV[m_byClass] * (m_sLevel - 1) + g_sPPAdd[m_byClass];
	m_sMaxSP	= g_sSPConst[m_byClass] * m_sCON + g_sSPLV[m_byClass] * (m_sLevel - 1) + g_sSPAdd[m_byClass];

	m_bLive		= bLive;		// 磷菌绰瘤, 混疽绰瘤?

	m_sHP		= sHP;			// 泅犁 HP
	m_sPP		= sPP;			// 泅犁 PP
	m_sSP		= sSP;			// 泅犁 SP

	if(m_sHP <= 0 || m_bLive == USER_DEAD) 
	{
		m_bLive = USER_DEAD;
		SetZoneIndex(m_curz);
		IsDeadUser();		// 磷篮 蜡历搁 付阑肺...

		m_bLive = USER_LIVE;
		m_sHP = m_sMaxHP;
	}
	else SetZoneIndex(m_curz);

	if(iShopPingDN < 0) iShopPingDN = 0;
	m_dwShopPingDN = iShopPingDN;
	
	if(iDN < 0) iDN = 0;

	m_dwDN		= iDN;			// 家瘤陛

	m_iCityValue = iCityValue;	// 矫刮 穿利蔼
	m_sKillCount = sKillCount;	// PK穿利 冉荐

	for(i = 0; i < 11; i++)
	{
		if(m_iCityValue < g_CityRankValue[i])// 穿利摹蔼捞 扁霖 汲沥蔼焊促 累篮瘤 魄窜茄促.
		{
			if(i < 6) { m_sCityRank = i - CITY_RANK_INTERVAL; }
			else { m_sCityRank = i -1 - CITY_RANK_INTERVAL; }
			break;
		}
	}

	if(m_iCityValue >= g_CityRankValue[10]) m_sCityRank = 5;

	::CopyMemory(m_strSkill, strSkill, sizeof(m_strSkill));		// 胶懦
	::CopyMemory(m_strItem, strItem, sizeof(m_strItem));		// 酒捞袍
	::CopyMemory(m_strPsi, strPsi, sizeof(m_strPsi));			// 荤捞坷葱
//SaveDump((char*)strItem,sizeof(strItem));
	StrToUserSkill((LPTSTR)strSkill);
	StrToUserItem((LPTSTR)strItem);
	StrToUserPsi((LPTSTR)strPsi);
	StrToHaveEventData((LPTSTR)strHaveEvent);
	StrToUserTel((LPSTR)strTel);


	m_dwExpNext	= GetNextLevel(m_sLevel);						// 促澜饭骇捞 登扁困茄 版氰摹

	for(i = 0; i < TOTAL_SKILL_NUM; i++)						// 胶懦 己傍啦 
	{
		GetSkillInfo(i, m_UserSkillInfo[i]);
	}
//	m_UserChangeSkillInfo;										// 傈流捞 乐阑版快 ?(5)胶懦甫 历厘

	for(i = 0; i < CLASS_NUM; i++)
	{
		m_sChangeClass[i] = sChangeClass[i];
	}

	m_tAbnormalKind = tAbnormalKind;							// 惑怕捞惑 沥焊
	m_dwAbnormalTime = iAbnormalTime;
	m_dwLastAbnormalTime = GetTickCount();

	// Quick Item 沥焊绰 瘤陛 静瘤 臼绰促.
//	int index = 0;		
//	for(i = TOTAL_ITEM_NUM; i < TOTAL_ITEM_SLOT_NUM; i++)
//	{
//		m_UserItem[i].sSid = GetShort((char*)strQuickItem, index);
//	}

//	dwMaxDN = m_sLevel * MAX_LEVEL_DN;									// 家瘤陛捞 捞惑茄瘤 眉农
//	if( m_dwDN > dwMaxDN ) m_dwDN = dwMaxDN; 
	m_tDir = myrand(0, 7);												// 泅犁 焊绊乐绰 规氢阑 伐待栏肺 檬扁拳茄促.
		
	m_tIsOP = tIsOP;
	if(m_tIsOP == 255)//yskang debug 叼厚俊 盔牢 葛甫 泅惑栏肺 255啊 甸绢啊绊 乐澜 棱阑锭 鳖瘤 内靛 蜡瘤
		m_tIsOP = 0;
	if(tIsOP == 1) AddAbnormalInfo(OPERATION_MODE);						// 扁夯捞 捧疙葛靛烙...

	CheckInvalidGuildZone();

	DWORD dwCurrTime = GetTickCount();
	m_dwLastSpeedTime = dwCurrTime;
	m_dwLastMoveAndRun = dwCurrTime;
	m_dwLastAttackTime = dwCurrTime;
	m_dwLastPsiAttack = dwCurrTime;
	m_dwLastPsiDamagedTime = 0;
	m_dwLastUseSpecialTime = 0;
	m_dwCastDelay = 0;
	m_dwLastTimeCount = dwCurrTime;
	m_dwLastAbnormalTime = dwCurrTime;
	m_dwLastHPTime = dwCurrTime;

	m_dwLastHasteTime = dwCurrTime;
	m_dwLastShieldTime = dwCurrTime;
	m_dwLastDexUpTime = dwCurrTime;
	m_dwLastMaxHPUpTime = dwCurrTime;
	m_dwLastCallHuFaTime = dwCurrTime;
	m_dwLastXiShendTime = dwCurrTime;
	m_dwLastWISUpTime = dwCurrTime;
	m_dwLastFANTAnTime = dwCurrTime;
	m_dwLastFENGLiTime = dwCurrTime;
	m_dwLastYINENgTime = dwCurrTime;
	m_dwLastDUOCHONgTime = dwCurrTime;

	m_dwNoDamageTime = 0;
	m_dwLastNoDamageTime = dwCurrTime;

	m_dwSaintTime = iSaintTime;		// 技牢飘 乔纳捞 墨款飘 临咯林扁

	m_dwLastHiExpTime		= dwCurrTime;	
	m_dwLastHtExpTime		= dwCurrTime;
	m_dwLastMagicFindTime	= dwCurrTime;
	m_dwLastMagicFtTime		= dwCurrTime;
	m_dwLastNoChatTime		= dwCurrTime;
    m_dwLastCloseTime		= dwCurrTime;
	m_dwLastAutoMoney		= dwCurrTime;
	m_dwLastLingQu  		= dwCurrTime;
    m_dwLastGuarDianTianShi = dwCurrTime;
	m_dwLastVIPTime			= dwCurrTime;
	m_dwLastUPZaiXianTime   = dwCurrTime;
	m_dwLastBFindTime		= dwCurrTime;
	m_dwLastBHTime		= dwCurrTime;
	m_dwLastSJTime		= dwCurrTime;
	
	m_dwHiExpTime		= iHiExpTime;		// 版氰摹 2硅
	m_dwHtExpTime		= iHtExpTime;		// 3bei
	m_dwMagicFindTime	= iMagicFindTime;
	m_dwMagicFtTime		= iMagicFtTime;
	m_dwNoChatTime		= iNoChatTime;
	m_dwCloseTime		= iCloseTime;
	m_dwAutoMoney		= iAutoMoney;
	m_dwLingQu  		= iLingQu;
	m_dwGuarDianTianShi	= iGunTianShi;//VIP
	m_dwShopPingDN		= iShopPingDN;
	m_dwVIPTime			= iVIPTime;	
	m_dwZaiXianTime		= iZaiXianTime;
	m_dwBFindTime		= iBFindTime;
	m_dwBHTime		= iBHTime;
	m_dwSJTime		= iSJTime;
	
	m_dwLastAdamantineTime		= dwCurrTime;	
	m_dwLastMightyWeaponTime	= dwCurrTime;
	m_dwLastBerserkerTime		= dwCurrTime;

	m_dwLastFastRunTime			= dwCurrTime;
	m_dwLastMindShockTime		= dwCurrTime;
	m_dwLastPsiShieldTime		= dwCurrTime;
	m_dwLastPiercingShieldTime	= dwCurrTime;

	m_dwLastMindGuardTime		= dwCurrTime;

	SetPsiOne(tPsiOneKind, iPsiOneTime);
	SetPsiTwo(tPsiTwoKind, iPsiTwoTime);
	SetPsiThree(tPsiThreeKind, iPsiThreeTime);
	
	SetPsiAbnormalStatus();
    SetStatus();
	GetRecoverySpeed();													// 蜡历 努贰胶狼 雀汗加档甫 搬沥茄促.

	return TRUE;
}

///////////////////////////////////////////////////////////////////////
//	秦寸 蜡历啊 角青茄 捞亥飘甫 历厘茄促.
//
void USER::StrToHaveEventData(TCHAR *pBuf)
{
	int index = 0;
	int eventnum;

	InitEventSlot();

	if( !pBuf[0] ) return;

	short HaveEventNum = GetShort( pBuf, index );

	if(HaveEventNum > MAX_EVENT_NUM) HaveEventNum = MAX_EVENT_NUM;

	int* pEventNum;

	for(int i = 0; i < HaveEventNum; i++)
	{
		eventnum = GetShort( pBuf, index );

		pEventNum = new int;

		*(pEventNum) = eventnum;

		m_arEventNum.Add( pEventNum );
	}
}

///////////////////////////////////////////////////////////////////////
//	秦寸 蜡历啊 角青茄 捞亥飘甫 DB俊 历厘茄促.
//
void USER::UserHaveEventDataToStr(TCHAR *pBuf)
{
	int index = 0;
	int num = m_arEventNum.GetSize();

	if(num >= MAX_EVENT_NUM) num = MAX_EVENT_NUM;

	SetShort( pBuf, num, index );

	int temp_int;

	for( int i = 0; i < num; i++)
	{
		temp_int = *(m_arEventNum[i]);

		SetShort( pBuf, temp_int, index );
	}
}

///////////////////////////////////////////////////////////////////////
//	秦寸 蜡历俊 辨靛啊 乐促搁 辨靛俊 鞘夸茄 函荐甫 檬扁拳茄促.
//
void USER::SetGuildVariable()
{	
	int nLen = 0;
	int iUserIndex = -1;
	CGuild *pGuild = NULL;
//	CGuildUser *pGuildUser = NULL;

	pGuild = GetGuild(m_dwGuild);

	m_tGuildWar = GUILD_WAR_AFFTER;
	m_tFortressWar = GUILD_WAR_AFFTER;

	if(!pGuild)	m_dwGuild = -1;									// 辨靛盔捞扼搁
	else
	{
		iUserIndex = pGuild->GetUser(m_strUserID);

		if(iUserIndex < 0) m_dwGuild = -1;
		else													// 角力 辨靛俊 加窍搁
		{
			m_bRepresentationGuild = FALSE;						// 困烙 NO! 

			if(strcmp(pGuild->m_strMasterName, m_strUserID) == 0) m_bGuildMaster = TRUE; //辨付牢瘤 眉农
			else if(pGuild->m_lSubMaster == 1)
			{
				if(strcmp(pGuild->m_strSubMasterName, m_strUserID) == 0) m_bRepresentationGuild = TRUE; //辨付牢瘤 眉农
			}
			
			if(strcmp(g_arGuildData[m_dwGuild]->m_strUsedUser, m_strUserID) == 0)		// 秦寸 辨靛芒绊甫 捞侩窍绰瘤 眉农
			{
				g_arGuildData[m_dwGuild]->m_iUsedUser = -1;
				::ZeroMemory(g_arGuildData[m_dwGuild]->m_strUsedUser, sizeof(g_arGuildData[m_dwGuild]->m_strUsedUser));
				InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0); 
			}
			
			m_sGuildVersion = pGuild->m_sVersion;				// 辨靛滚怜 
			strcpy(m_strGuildName, pGuild->m_strGuildName);		// 辨靛捞抚
		}
	}

	ReleaseGuild();			// 秦力...	
}

///////////////////////////////////////////////////////////////////////
//	贸澜 霸烙俊 立加且锭 秦寸 辨靛啊 辨傈阑 窍搁 磊悼栏肺 曼啊窍档废茄促.
//
//  瘤陛篮 傍己傈父 窍档废... 
void USER::CheckGuildWar()
{
	int i, j;
	short nCount = 1;

	BOOL bWar = FALSE;

	CGuildFortress *pFort = NULL;

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		pFort = g_arGuildFortress[i];

		if(!pFort) continue;

		bWar = FALSE;
		nCount = 1;
		CBufferEx TempBuf, TempBufData;

		if(pFort->m_lUsed == 1)									// 秦寸 瘤开捞 辨傈 吝捞搁...
		{
			if(m_dwGuild >= 1)
			{
				if(pFort->m_iGuildSid == m_dwGuild) 
				{
					bWar = TRUE;
					BeginFortressWar();	
				}
				else
				{
					for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
					{
						if(pFort->m_arAttackGuild[j].lGuild <= 0) continue;

						if(pFort->m_arAttackGuild[j].lGuild == m_dwGuild)
						{
							bWar = TRUE;
							BeginFortressWar();			// 辨靛 傈阑 矫累茄促.
							break;
						}
					}
				}

				TempBufData.Add(pFort->m_iGuildSid);			// 规绢 辨靛			

				for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)		// 傍拜 辨靛...
				{
					if(pFort->m_arAttackGuild[j].lGuild > 0)
					{
						TempBufData.Add(pFort->m_arAttackGuild[j].lGuild);
						nCount++;
					}
				}
			}

			TempBuf.Add(GUILD_WAR);
			TempBuf.Add((BYTE)0x01);						// 辨靛傈 矫累...
			TempBuf.Add((BYTE)GUILD_FOTRESS_WARRING);		// 傍己傈...
			TempBuf.Add((short)pFort->m_sFortressID);

			if(bWar)
			{
				TempBuf.Add((BYTE)0x01);
				TempBuf.Add(nCount);							// 醚 辨靛荐
				TempBuf.AddData(TempBufData, TempBufData.GetLength());
			}
			else TempBuf.Add((BYTE)0x00);

			Send(TempBuf, TempBuf.GetLength());
		}
	}
}

///////////////////////////////////////////////////////////////////////
//	蜡历 努贰胶俊 舅嘎绰 雀汗 加档甫 备茄促.
//
void USER::GetRecoverySpeed()
{
	int weight = 0;

	m_iHPIntervalTime = 0;		// HP 雀汗 矫埃 埃拜
	m_iSPIntervalTime = 0;		// SP 雀汗 矫埃 埃拜
	m_iPPIntervalTime = 0;		// PP 雀汗 矫埃 埃拜

	m_iHPRate = 1;
	m_iPPRate = 1;
	m_iSPRate = 1;

	m_iHPIntervalTime = 15000;	// Init default	
	m_iSPIntervalTime = 15000;
	m_iPPIntervalTime = 15000;

	if(CheckRecoverTableByClass())
	{
		m_iHPRate = g_arRecoverTable[m_byClass]->m_byHPDivide;
		m_iPPRate = g_arRecoverTable[m_byClass]->m_byPPDivide;
		m_iSPRate = g_arRecoverTable[m_byClass]->m_bySPDivide;

		m_iHPIntervalTime = (int)(g_arRecoverRateTable[m_byClass]->m_sHpSpeedupTime - (double)(g_arRecoverRateTable[m_byClass]->m_byHpSpeedupLevel/30));
		m_iSPIntervalTime = (int)(g_arRecoverRateTable[m_byClass]->m_sSpSpeedupTime - (double)(g_arRecoverRateTable[m_byClass]->m_bySpSpeedupLevel/30));
		m_iPPIntervalTime = (int)(g_arRecoverRateTable[m_byClass]->m_sPpSpeedupTime - (double)(g_arRecoverRateTable[m_byClass]->m_byPpSpeedupLevel/30));
	}
	
	weight = (int)((double)m_iMaxWeight * 0.7 + 0.5);		// 公霸 力茄栏肺 牢茄 雀汗 加档甫 词...
	if(m_iCurWeight >= weight) 
	{
		m_iHPIntervalTime = m_iHPIntervalTime * 2;
		m_iSPIntervalTime = m_iSPIntervalTime * 2;
		m_iPPIntervalTime = m_iPPIntervalTime * 2;
	}
}

///////////////////////////////////////////////////////////////////////
//	蜡历 沥焊狼 Zone 捞 辑滚啊 寸寸窍绊 乐绰 粮牢瘤 八荤茄促.
//
BOOL USER::IsZoneInThisServer(int zone)
{
	int temp_zone = -1;

	for(int i = 0; i < g_zone.GetSize(); i++)
	{
		if( g_zone[i]->m_Zone == zone )	return TRUE;
		//TRACE("地图代码 %d \n",g_zone[i]->m_Zone);
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
//	概俺函荐狼 Zone 捞 绢蠢 辑滚俊辑 包厘窍绰瘤甫 茫酒辑 
//	技记狼 ZONEINFO Table 阑 诀单捞飘 茄促.
//
//	给茫绰 版快绰 唱吝俊 内爹
void USER::ChangeServer(int zone)
{
	int server_index = -1;
	int i, j;
	BOOL found = FALSE;
	BOOL bFoundDBIndex = FALSE;

	for( i = 0; i < g_server.GetSize(); i++)
	{
		if(found) break;

		if(g_server[i])
		{
			if( g_server[i]->m_sDBIndex != -1 )
			{
				bFoundDBIndex = FALSE;

				for( j = 0; j < g_arMyServer.GetSize(); j++ )
				{
					if( *(g_arMyServer[j]) == g_server[i]->m_sDBIndex )
					{
						bFoundDBIndex = TRUE;
						break;
					}
				}

				if( !bFoundDBIndex ) continue;
			}

			for( j = 0; j < g_server[i]->m_zone.GetSize(); j++)
			{
				if(found) break;

				if(g_server[i]->m_zone[j])
				{
					if( *(g_server[i]->m_zone[j]) == zone)
					{
						server_index = i;
						found = TRUE;
					}
				}
			}
		}
	}

	if(server_index == -1) 
	{
		TRACE("Can't find valid point...\n");
		return;// FALSE;
	}
/*
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call CHANGE_SERVER (\'%s\',%d)}"), m_strUserID, server_index);

	int db_index = 0;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
//		bQuerySuccess = FALSE;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
	}
	else if (retcode==SQL_ERROR)
	{
		DisplayErrorMsg(hstmt);
		if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBSession[m_iModSid].ReleaseDB(db_index);
*/
}

///////////////////////////////////////////////////////////////////////////
//	Zone Index 甫 悸泼茄促.
//
void USER::SetZoneIndex(int zone)
{
	for(int i = 0; i < g_zone.GetSize(); i++)
	{
		if( g_zone[i]->m_Zone == zone )
		{
			m_ZoneIndex = i;
			break;
		}
	}
}

//######################################################################################
//	Send Functions

////////////////////////////////////////////////////////////////////////////////////////
//
void USER::SightRecalc()
{
	int sx, sy;
	sx = m_curx / SIGHT_SIZE_X;
	sy = m_cury / SIGHT_SIZE_Y;

	int dir_x = 0;
	int dir_y = 0;

	if( sx == m_presx && sy == m_presy ) return;
	
	if( m_presx == -1 || m_presy == -1 )
	{
		dir_x = 0;
		dir_y = 0;
	}
	else if( m_presx == -2 || m_presy == -2 )
	{
		dir_x = DIR_OUTSIDE;
		dir_y = DIR_OUTSIDE;
	}
	else
	{
		if( sx > m_presx && abs(sx-m_presx) == 1 )		dir_x = DIR_H;
		if( sx < m_presx && abs(sx-m_presx) == 1 )		dir_x = DIR_L;
		if( sy > m_presy && abs(sy-m_presy) == 1 )		dir_y = DIR_H;
		if( sy < m_presy && abs(sy-m_presy) == 1 )		dir_y = DIR_L;
		if( abs(sx-m_presx) > 1 )						dir_x = DIR_OUTSIDE;
		if( abs(sy-m_presy) > 1 )						dir_y = DIR_OUTSIDE;
	}

//	TRACE("Sight changed....[%d] [%d] -> [%d] [%d] (%d %d)\n", m_presx, m_presy, sx, sy, dir_x, dir_y);

	int prex = m_presx;
	int prey = m_presy;
	m_presx = sx;
	m_presy = sy;

	SendUserInfoBySightChange(dir_x, dir_y, prex, prey);
}

/////////////////////////////////////////////////////////////////////////////////////
//	矫具函版栏肺 牢茄 蜡历沥焊 傈价
//
void USER::SendUserInfoBySightChange(int dir_x, int dir_y, int prex, int prey)
{
	int min_x = 0, min_y = 0;
	int max_x = 0, max_y = 0;

	int sx = m_presx;
	int sy = m_presy;

	int modify_index = 0;
	TCHAR modify_send[10000];	::ZeroMemory(modify_send, sizeof(modify_send));
	FillUserInfo(modify_send, modify_index, INFO_MODIFY);

	int delete_index = 0;
	TCHAR delete_send[4096];	::ZeroMemory(delete_send, sizeof(delete_send));
	FillUserInfo(delete_send, delete_index, INFO_DELETE);

	if( prex == -1 || prey == -1 )
	{
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
//		SendItemFieldInfoToMe();
		SendCompressedRangeInfoToMe();
		return;
	}
	if( dir_x == DIR_OUTSIDE || dir_y == DIR_OUTSIDE )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);	// 款康磊 单捞磐绰 盔玫豪技.
//		SendItemFieldInfoToMe();
		SendCompressedRangeInfoToMe();
		return;
	}
	if( dir_x > 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx+1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);
//		SendItemFieldInfoToMe();
	}
	if( dir_y > 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy+1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);
//		SendItemFieldInfoToMe();
	}
	if( dir_x < 0 )
	{
		min_x = (prex+1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);
//		SendItemFieldInfoToMe();
	}
	if( dir_y < 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey+1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);
//		SendItemFieldInfoToMe();
	}

	SendCompressedRangeInfoToMe();
}

////////////////////////////////////////////////////////////////////////////////////
//	焊尘 矫具 沥焊甫 拘绵窍咯 焊辰促
//
void USER::SendCompressedRangeInfoToMe()
{
	if( !m_CompCount || m_CompCount < 0 || !m_iCompIndex || m_iCompIndex < 0 )
	{
		m_CompCount = 0;
		m_iCompIndex = 0;
		return;
	}

	m_CompMng.FlushAddData();

	m_CompMng.AddData( m_CompBuf, m_iCompIndex );

	m_CompMng.PreCompressWork();
	m_CompMng.Compress();

	int comp_data_len = m_CompMng.GetCompressedDataCount();
	int org_data_len = m_CompMng.GetUnCompressDataLength();
	DWORD crc_value = m_CompMng.GetCrcValue();

	CBufferEx	TempBuf;

	TempBuf.Add( SIGHT_INFO_COMPRESSED );
	TempBuf.Add( (short)comp_data_len );
	TempBuf.Add( (short)org_data_len );
	TempBuf.Add( crc_value );
	TempBuf.Add( (short)m_CompCount );

	char* packet = m_CompMng.GetExtractedBufferPtr();

	TempBuf.AddData( packet, comp_data_len );

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL)
	{
		m_CompCount = 0;
		m_iCompIndex = 0;
		m_CompMng.FlushAddData();
		return;
	}

	pNewData->flag = SEND_USER;
	pNewData->len = TempBuf.GetLength();

	::CopyMemory(pNewData->pBuf, TempBuf, TempBuf.GetLength());

	pNewData->uid = m_uid;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);

	if(pNewData) delete pNewData;
	//

	m_CompCount = 0;
	m_iCompIndex = 0;
	m_CompMng.FlushAddData();
}

////////////////////////////////////////////////////////////////////////////////////
//	漂沥 蜡历 肚绰 NPC狼 沥焊甫 努扼捞攫飘俊辑 夸没且 版快 贸府
//
void USER::UserInfoReq(TCHAR *pBuf)
{
	int index = 0;
	int nUid = GetInt(pBuf, index);
	
	if(nUid < 0 || nUid >= INVALID_BAND) return;
	
	if(nUid >= USER_BAND && nUid < NPC_BAND)
	{
		USER* pSendUser = GetUser(nUid - USER_BAND);	
//		if(pSendUser == NULL || pSendUser->m_curz != m_curz || pSendUser->m_state != STATE_GAMESTARTED) return;
		if( pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED ) return;

		if( !IsInSight( pSendUser->m_curx, pSendUser->m_cury, pSendUser->m_curz ) ) return;
		
		SendUserInfo(pSendUser, INFO_MODIFY);
	}
	if(nUid >= NPC_BAND && nUid < INVALID_BAND)
	{
		CNpc* pNpc = GetNpc(nUid - NPC_BAND);
		if(pNpc == NULL) return;

		if( !IsInSight( pNpc->m_sCurX, pNpc->m_sCurY, pNpc->m_sCurZ ) ) return;

		SendNpcInfo(pNpc, INFO_MODIFY);
	}
}

BOOL USER::IsInSight(int x, int y, int z)
{
	if( m_curz != z ) return FALSE;

	int min_x, max_x;
	int min_y, max_y;

	int sx = m_curx / SIGHT_SIZE_X;
	int sy = m_cury / SIGHT_SIZE_Y;
	
	min_x = (sx-1)*SIGHT_SIZE_X; if( min_x < 0 ) min_x = 0;
	max_x = (sx+2)*SIGHT_SIZE_X;
	min_y = (sy-1)*SIGHT_SIZE_Y; if( min_y < 0 ) min_y = 0;
	max_y = (sy+2)*SIGHT_SIZE_Y;
	
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return FALSE;
	
	if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
	if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;

	if( min_x > x || max_x < x ) return FALSE;
	if( min_y > y || max_y < y ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	NPC 沥焊甫 焊辰促.
//
void USER::SendNpcInfo(CNpc *pNpc, BYTE tMode /* INFO_MODIFY */)
{
	if( !pNpc ) return;

	CBufferEx TempBuf;
	TempBuf.Add((BYTE)NPC_INFO);
	TempBuf.Add((BYTE)tMode);
	TempBuf.Add((short)(pNpc->m_sNid + NPC_BAND));
	
	if(tMode != INFO_MODIFY)
	{
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add((short)pNpc->m_sPid);
    TempBuf.AddString(pNpc->m_strName);

	CPoint pt = ConvertToClient(pNpc->m_sCurX, pNpc->m_sCurY);
	if(pt.x == -1 || pt.y == -1) { pt.x = 1; pt.y = 1; }
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);

//	if(pNpc->m_NpcState == NPC_DEAD) TempBuf.Add((BYTE)0x00);
	if(pNpc->m_sHP <= 0) TempBuf.Add((BYTE)0x00);
	else TempBuf.Add((BYTE)0x01);

	TempBuf.Add(pNpc->m_tNpcType);	// NPC Type

	TempBuf.Add((int)pNpc->m_sMaxHP);	// NPC 泅犁 惑怕甫 钎矫窍扁 困秦(Client 祸 备盒)	
	TempBuf.Add((int)pNpc->m_sHP);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add(pNpc->m_sClientSpeed);
	TempBuf.Add(pNpc->m_byColor);

	if(pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR)
	{
		TempBuf.Add(pNpc->m_sDimension);
	}
	TempBuf.Add(pNpc->m_sQuestSay);
	
	Send(TempBuf, TempBuf.GetLength());
}

int USER::MakeRangeInfoToMe(CNpc* pNpc, BYTE tMode, TCHAR *pData)
{
	if( !pNpc ) return 0;
	CPoint pt = ConvertToClient( pNpc->m_sCurX, pNpc->m_sCurY );
	if( pt.x == -1 || pt.y == -1 ) return 0;

	int index = 0;

	SetByte( pData, (BYTE)NPC_INFO, index );
	SetByte( pData, tMode, index );
	SetShort( pData, (short)(pNpc->m_sNid + NPC_BAND), index );

	if( tMode != INFO_MODIFY )
	{
		return index;
	}

	SetShort( pData, (short)pNpc->m_sPid, index );
	SetByte( pData, (BYTE)strlen( pNpc->m_strName ), index);
	SetString( pData, pNpc->m_strName, strlen( pNpc->m_strName ), index );

	SetShort( pData, (short)pt.x, index );
	SetShort( pData, (short)pt.y, index );

	if( pNpc->m_sHP == 0 ) SetByte( pData, 0X00, index );
	else SetByte( pData, 0X01, index );

	SetByte( pData, pNpc->m_tNpcType, index );

	SetInt( pData, (int)pNpc->m_sMaxHP, index );
	SetInt( pData, (int)pNpc->m_sHP, index );
	SetByte( pData , 0x00, index);
	SetByte( pData , 0x00, index);
	SetByte( pData , 0x00, index);
	SetByte( pData , 0x00, index);
	SetByte( pData , 0x00, index);
	SetByte( pData , 0x00, index);
	SetByte( pData , 0x00, index);
	SetByte( pData , 0x00, index);
	SetShort( pData, pNpc->m_sClientSpeed, index );
	//int iRandom = myrand(1, 6);  //刷怪颜色随机
	int iRandom = pNpc->m_byColor; 

	if(pNpc->m_tNpcType == 0)
	{
		SetByte( pData, (BYTE)iRandom, index );	
	}else{		
		SetByte( pData, pNpc->m_byColor, index );	
	}
	if(pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR) 
	{
		SetShort( pData, pNpc->m_sDimension, index);
	}
	SetShort(pData, pNpc->m_sQuestSay, index);
////////////////NPC显示要塞团图标//////////////////////
	if((pNpc->m_tNpcType != 1) && (pNpc->m_tNpcType != 3))
		return index;
	//要塞所有者
	CGuildFortress *pGuild= GetGuildWarWin();
	if(pGuild== NULL)
	{
		SetInt(pData,-1,index);
		SetByte(pData,0,index);
		SetInt(pData,0,index);
	}else
	{
		SetInt(pData,pGuild->m_iGuildSid,index);
		SetVarString(pData,pGuild->m_strGuildName,strlen(pGuild->m_strGuildName),index);
		SetInt(pData,0,index);
	}
    return index;
}
///////判断要塞军团///////////
CGuildFortress *USER::GetGuildWarWin()
{
	CGuildFortress* pFort = NULL;
	int Fortress[] = {o_yehuoini[0]->jtbh};
	for(int i = 0; i < 3;i++)
	{
		pFort = GetFortress(Fortress[i]);
		//ASSERT(pFort);
		if(pFort == NULL)
			continue;
		if(pFort->m_iGuildSid <= 0)
			continue;	
		return pFort;
	}
	return NULL;
}

void USER::AddRangeInfoToMe(CNpc *pNpc, BYTE tMode)
{
	int index = 0;
	TCHAR pData[1024];

	index = MakeRangeInfoToMe( pNpc, tMode, pData );

	if( index )
	{
		m_CompCount++;

		SetShort( m_CompBuf, index, m_iCompIndex );				// 父甸绢柳 蜡历 沥焊狼 辨捞
		::CopyMemory( m_CompBuf+m_iCompIndex, pData, index );	// 父甸绢柳 沥焊甫 拘绵且 滚欺俊 汗荤
		m_iCompIndex += index;

		if( m_iCompIndex >= 8000 )
		{
			SendCompressedRangeInfoToMe();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	漂沥 蜡历狼 蜡历 沥焊甫 焊辰促.
//
void USER::SendUserInfo(USER *pUser, BYTE tMode	/*INFO_MODIFY*/)
{
	if( !pUser ) return;

	CBufferEx	TempBuf;
	int i;

	CPoint pos = ConvertToClient( pUser->m_curx, pUser->m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }

	TempBuf.Add(USER_INFO);
	TempBuf.Add(tMode);
	TempBuf.Add(pUser->m_uid + USER_BAND);

	if(tMode != INFO_MODIFY)
	{
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.AddString(pUser->m_strUserID);

	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);

	TempBuf.Add(pUser->m_iSkin);
	TempBuf.Add(pUser->m_iHair);
	TempBuf.Add((BYTE)pUser->m_sGender);
	TempBuf.AddData(pUser->m_strFace, 10);

	for( i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(pUser->m_UserItem[i].sSid);

	TempBuf.Add(pUser->m_sHP);
	TempBuf.Add(pUser->m_sMagicMaxHP);

	TempBuf.Add(pUser->m_tDir);
	TempBuf.Add(pUser->m_dwAbnormalInfo);				// 惑怕捞惑 沥焊绰 菊栏肺 m_dwAbnormalInfo 窍唱父 曼炼茄促. JJS07 2002.10.14
	TempBuf.Add(pUser->m_dwAbnormalInfo_);	
	TempBuf.Add((DWORD)0);
	TempBuf.Add((DWORD)0);
	//国服此处多16 byte 0
	for(int zero = 0;zero<16;zero++)
		TempBuf.Add((BYTE)0x00);

	TempBuf.Add(pUser->m_sCityRank);

	TempBuf.Add((int)pUser->m_dwGuild);					//&&&&&&&&&&&& Test Code
	TempBuf.AddString(pUser->m_strGuildName);			// 辨靛 捞抚阑 眠啊
	TempBuf.Add(pUser->m_sGuildVersion);

	TempBuf.Add(pUser->m_byClass);
	TempBuf.Add((BYTE)pUser->m_bPkStatus);
	TempBuf.AddString(pUser->m_strLoveName); //守护显示

	for(i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) TempBuf.Add(pUser->m_UserItem[i].sSid);	// EBody
	if(m_UserItem[TOTAL_ITEM_NUM-2].sSid!=-1&&m_UserItem[TOTAL_ITEM_NUM-2].sDuration!=0){
		TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]));
		TempBuf.Add((BYTE)0x00);
	}else{
		TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]);
		TempBuf.Add((BYTE)0xff);
	}
	TempBuf.AddString(pUser->m_PersonalShopName);
	//国服此处多5byte 0x0a 0x00 0x00 0x00 0x00
//	TempBuf.Add((BYTE)0x01);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)pUser->m_tBabyCall);
	if(pUser->m_tBabyCall)
	{
		TempBuf.Add(g_szBabyName[pUser->m_sBabyID%2],strlen(g_szBabyName[pUser->m_sBabyID%2]));
		TempBuf.Add(pUser->m_sBabyID);
	}
	Send(TempBuf, TempBuf.GetLength());

	SendHuFaInfo(pUser,TO_ME);

}

int USER::MakeRangeInfoToMe(USER* pUser, BYTE tMode, TCHAR *pData)
{
	int nLen = 0;
	int i = 0;

	if( !pUser ) return 0;

	CPoint pos = ConvertToClient( pUser->m_curx, pUser->m_cury );
	if( pos.x == -1 || pos.y == -1 ) return 0;

	int index = 0;

	SetByte( pData, USER_INFO, index );
	SetByte( pData, tMode, index );
	SetInt(  pData, pUser->m_uid + USER_BAND, index );

	if( tMode != INFO_MODIFY )
	{
		return index;
	}

	SetByte( pData, (BYTE)strlen( pUser->m_strUserID ), index );
	SetString( pData, pUser->m_strUserID, strlen( pUser->m_strUserID ), index );

	SetShort( pData, pos.x, index );
	SetShort( pData, pos.y, index );

	SetDWORD( pData, pUser->m_iSkin, index );
	SetDWORD( pData, pUser->m_iHair, index );
	SetByte( pData, (BYTE)pUser->m_sGender, index );

	::CopyMemory( pData+index, pUser->m_strFace, 10 );
	index += 10;

	for( i = 0; i < EQUIP_ITEM_NUM; i++ ) SetShort( pData, pUser->m_UserItem[i].sSid, index );

	SetShort( pData, pUser->m_sHP, index );
	SetShort( pData, pUser->m_sMagicMaxHP, index );

	SetByte( pData, pUser->m_tDir, index );

	SetDWORD( pData, pUser->m_dwAbnormalInfo, index);// 惑怕捞惑 沥焊
///////////////////////////////////测试VIP头顶NEW显示////////////////////////// NEO版本     测试闪光
	if (o_yehuoini[0]->bbzh == 0)
 {
	//if(pUser->m_dwGuarDianTianShi > 0)
	 if(isGuildRen == TRUE || m_bCBOff == TRUE )  
	{
		pUser->AddHuFaStatus(New_Status_22);// IP显示特殊符号以及头上带NEW
		pUser->SendJudgePsiStatus();
	}
	else
	{
		pUser->DelHuFaStatus(New_Status_22);
		pUser->SendJudgePsiStatus();
	}
}

///////////////////////////////////测试VIP头顶NEW显示//////////////////////////
	SetDWORD( pData, pUser->m_dwAbnormalInfo_, index);
	SetDWORD( pData, 0, index);
	SetDWORD( pData, 0, index);

	/*SetByte( pData, 0x00, index );
	SetByte( pData, 0x00, index );
	SetByte( pData, 0x00, index );
	SetByte( pData, 0x00, index );
	SetByte( pData, 0x00, index );
	SetByte( pData, 0x00, index );
	SetByte( pData, 0x00, index );
	SetByte( pData, 0x00, index );
	*/
	//国服此处多16 byte 0
	for(int zero = 0;zero<16;zero++)
		SetByte(pData, (BYTE)0x00, index);

	SetShort( pData, pUser->m_sCityRank, index );
	SetInt( pData, pUser->m_dwGuild, index );

	nLen = strlen(pUser->m_strGuildName);
	if(nLen <= 0) nLen = 1;

	SetByte( pData, (BYTE)nLen, index );
	SetString( pData, pUser->m_strGuildName, nLen, index );		// Add Guild Name
	SetShort( pData, pUser->m_sGuildVersion, index );

	SetByte( pData, pUser->m_byClass, index );
	SetByte( pData, pUser->m_bPkStatus, index );
	//-- yskang 0.1 眠啊登绰 菩哦.... 龋莫[局莫]
	nLen = strlen(pUser->m_strLoveName);
	if(nLen < 1) nLen =1;
	SetByte( pData, (BYTE)nLen, index );
	SetString(pData,pUser->m_strLoveName,nLen,index);

	for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) 
		SetShort( pData, pUser->m_UserItem[i].sSid, index );
	//SetByte( pData, 0x00, index );
	//SetByte( pData, 0x00, index );
	if(pUser->m_UserItem[TOTAL_ITEM_NUM-2].sSid!=-1&&pUser->m_UserItem[TOTAL_ITEM_NUM-2].sDuration!=0){
			SetByte(pData,(BYTE)(pUser->m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]),index);
			SetByte( pData, 0x00, index );
		}else{
			SetByte(pData,(BYTE)(pUser->m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]),index);
			SetByte( pData, 0xff, index );
		}
		
	SetByte(pData, strlen(pUser->m_PersonalShopName), index);
	SetString(pData, pUser->m_PersonalShopName, strlen(pUser->m_PersonalShopName), index);
	//国服此处多5byte 0x0a 0x00 0x00 0x00 0x00
	SetByte(pData,(BYTE)0x01,index);
	SetByte(pData,(BYTE)0x00,index);
	SetByte(pData,(BYTE)0x00,index);
	SetByte(pData,(BYTE)0x00,index);
	SetByte(pData,(BYTE)pUser->m_tBabyCall,index);
	if(pUser->m_tBabyCall)
	{
		int tmp = strlen(g_szBabyName[pUser->m_sBabyID%2]);
		SetByte(pData, tmp, index);
		SetString(pData, g_szBabyName[pUser->m_sBabyID%2],tmp, index);
		SetShort(pData,pUser->m_sBabyID,index);
	}

//SaveDump(pData,index);
	SendHuFaInfo(pUser,TO_ME);

	return index;
}

void USER::AddRangeInfoToMe(USER *pUser, BYTE tMode)
{
	int index = 0;
	TCHAR pData[1024];

	index = MakeRangeInfoToMe( pUser, tMode, pData );

	if( index )
	{
		m_CompCount++;

		SetShort( m_CompBuf, index, m_iCompIndex );				// 父甸绢柳 蜡历 沥焊狼 辨捞
		::CopyMemory( m_CompBuf+m_iCompIndex, pData, index );	// 父甸绢柳 沥焊甫 拘绵且 滚欺俊 汗荤
		m_iCompIndex += index;

		if( m_iCompIndex >= 8000 )
		{
			SendCompressedRangeInfoToMe();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//功能:发送人物数据
//返回值:无
// 版本 1.00
//日期 2006.12.21
void USER::SendCharData()
{
	int i = 0;
	CBufferEx TempBuf;
	
	TempBuf.Add(CHAR_DATA);
	TempBuf.AddString(m_strUserID);
	TempBuf.Add(m_sMagicSTR);
	TempBuf.Add(m_sMagicCON);
	TempBuf.Add(m_sMagicDEX);
	TempBuf.Add(m_sMagicVOL);
	TempBuf.Add(m_sMagicWIS);

	TempBuf.Add((short)1);  // 烙矫 内靛

	TempBuf.Add(m_iSkin);
	TempBuf.Add(m_iHair);
	TempBuf.Add((BYTE)m_sGender);
	TempBuf.AddData(m_strFace, 10);

	TempBuf.Add(m_dwExp);
	TempBuf.Add(m_dwXP);

	TempBuf.Add(m_sSkillPoint);
	TempBuf.Add(m_sPA);
    TempBuf.Add(m_sSkillPoint_);

	TempBuf.Add(m_sMagicMaxHP);
	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sMagicMaxPP);
	TempBuf.Add(m_sPP);
	TempBuf.Add(m_sMagicMaxSP);
	TempBuf.Add(m_sSP);

	TempBuf.Add(m_dwDN);
	TempBuf.Add(m_sCityRank);
	
    TempBuf.Add(m_sLevel);
	TempBuf.Add(m_byClass);

	TCHAR strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB],strSkill_[12*3];
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strSkill_, sizeof(strSkill_));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));

	UserSkillToStr((LPTSTR)strSkill);
	TempBuf.AddData(strSkill, USER_SKILL_LEN);		// Skill

	//国服此处有5个未知技能法师的值0x19,0x1a,0x1b,0x1c,0x18
	//CHAR szSkillExBuf[3 * 5]={0x19,0x00,0x00,0x1a,0x00,0x00,0x1b,0x00,0x00,0x1c,0x00,0x00,0x18,0x00,0x00};
	//TempBuf.AddData(szSkillExBuf, 5*3);
	TCHAR JudgeSkill[3*5];
	::ZeroMemory(JudgeSkill, sizeof(JudgeSkill));
	UserJudgeSkill_ToStr(JudgeSkill);
	TempBuf.AddData(JudgeSkill, 5*3);
	//
	int index=0;
	for(i = 0; i < 12; i++)
	{
		SetShort(strSkill_, m_UserSkill_[i].sSid,		index);
		SetByte (strSkill_, m_UserSkill_[i].tLevel,	index);
	}
	TempBuf.AddData(strSkill_,12*3);

/*	//国服此处又有3个未知技能点数法id为0x10,0x11,0x12
	CHAR szUnknow[3 * 3]={0x10,0x00,0x00,0x11,0x00,0x00,0x12,0x00,0x00};
	TempBuf.AddData(szUnknow, 3*3);*/
	TCHAR Skill140[3*3];
	::ZeroMemory(Skill140, sizeof(Skill140));
	User140SkillToStr(Skill140);
	TempBuf.AddData(Skill140, 3*3);

	//此处又有6 byte 未知值0
	char bUnknow[6]={0x00,0x00,0x00,0x00,0x00,0x00};
	TempBuf.AddData(bUnknow, 6);

	TempBuf.Add((BYTE)m_nHavePsiNum);				// Psionic
	UserPsiToStr((LPTSTR)strPsi);
	if(m_nHavePsiNum > 0)
		TempBuf.AddData(strPsi, m_nHavePsiNum * _PSI_SIZE);

	UserItemToStrForSend( (LPTSTR)strItem );
	TempBuf.AddData(strItem, USER_ITEM_LEN);		// Item

	TempBuf.Add(m_dwExpNext);// Next Exp
	
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);

	//国服在此为32个0,日服为16个0
	for(int zero = 0;zero<32;zero++)
	{
		TempBuf.Add((BYTE)0x00);
	}

	TempBuf.Add((int)m_dwGuild);					//&&&&&&&&&&&&&& Test Code
	TempBuf.AddString(m_strGuildName);			// 辨靛 捞抚阑 眠啊
	TempBuf.Add(m_sGuildVersion);

	if(m_dwGuild >= 1)
	{
		//TempBuf.Add((BYTE)0xF4);
		//TempBuf.Add((BYTE)0x02);
		//TempBuf.Add((BYTE)0x41);
	//	TempBuf.Add((BYTE)0x00);
	//	TempBuf.Add((BYTE)0x00);
	//	TempBuf.Add((BYTE)0x00);
	}
	else
	{
		//TempBuf.Add((BYTE)0x00);
		//TempBuf.Add((BYTE)0x00);
		//TempBuf.Add((BYTE)0x00);
	}
	//负重
	TempBuf.Add((short)m_iMaxWeight);
	TempBuf.Add((short)m_iCurWeight);

/*	TempBuf.Add((short)-1);//龙心
	TempBuf.Add((short)-1);//狮吼

    TempBuf.Add((BYTE)0x84); // Finito Added
	TempBuf.Add((BYTE)0x99); // Finito Added
	TempBuf.Add((BYTE)0x2c); // Finito Added
	TempBuf.Add((BYTE)0x01); // Finito Added
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);*///原来的设置
	TempBuf.Add((BYTE)0xFF);
	TempBuf.Add((short)-1);		 
	TempBuf.Add((BYTE)0x84); 
	TempBuf.Add((BYTE)0x99);//现在设置解决龙心显示问题
	//国服此处添加了4个0,韩服只增加2个
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
//SaveDump(TempBuf,TempBuf.GetLength());
	CBufferEx TempBuf1;

	TempBuf1.Add( (short)(TempBuf.GetLength()) );
	TempBuf1.AddData( TempBuf, TempBuf.GetLength() );

	// 拘绵 霖厚 
	m_CompMng.FlushAddData();

	m_CompMng.AddData( TempBuf1, TempBuf1.GetLength() );

	m_CompMng.PreCompressWork();
	m_CompMng.Compress();

	int comp_data_len = m_CompMng.GetCompressedDataCount();
	int org_data_len = m_CompMng.GetUnCompressDataLength();
	DWORD crc_value = m_CompMng.GetCrcValue();

	CBufferEx SendBuf;

	SendBuf.Add( SIGHT_INFO_COMPRESSED );
	SendBuf.Add( (short)comp_data_len );
	SendBuf.Add( (short)org_data_len );
	SendBuf.Add( crc_value );
	SendBuf.Add( (short)1 );

	char* packet = m_CompMng.GetExtractedBufferPtr();

	SendBuf.AddData( packet, comp_data_len );
	
	TRACE( "%d -> %d\n", org_data_len, comp_data_len );

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL)
	{
		m_CompMng.FlushAddData();
		return;
	}

	pNewData->flag = SEND_USER;
	pNewData->len = SendBuf.GetLength();

	::CopyMemory(pNewData->pBuf, SendBuf, SendBuf.GetLength());

	pNewData->uid = m_uid;

	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	//

	m_CompMng.FlushAddData();

}

//////////////////////////////////////////////////////////////////////////////////
//	功能: 发送自己人物数据
//  返回值: 无
//  版本: 1.00 (牛牛更新)
///////////////////////////////////////////////////////////////////////////////////
void USER::SendMyInfo(BYTE towho, BYTE type)
{
	CBufferEx	TempBuf;
	int i;

	TempBuf.Add(USER_INFO);
	TempBuf.Add(type);
	TempBuf.Add(m_uid + USER_BAND);

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }

/*	if(type == INFO_MODIFY)
	{
	*/
		TempBuf.AddString(m_strUserID);
		
		TempBuf.Add((short)pos.x);
		TempBuf.Add((short)pos.y);
		
		TempBuf.Add(m_iSkin);
		TempBuf.Add(m_iHair);
		TempBuf.Add((BYTE)m_sGender);
		TempBuf.AddData(m_strFace, 10);
		
		for(i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(m_UserItem[i].sSid);

		TempBuf.Add(m_sHP);
		TempBuf.Add(m_sMagicMaxHP);

		TempBuf.Add(m_tDir);

		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		TempBuf.Add((DWORD)0);
		TempBuf.Add((DWORD)0);
		//国服此处多16 byte 0
		for(int zero = 0;zero<16;zero++)
			TempBuf.Add((BYTE)0x00);
        
		TempBuf.Add(m_sCityRank);

		TempBuf.Add((int)m_dwGuild);
		TempBuf.AddString(m_strGuildName);
		TempBuf.Add(m_sGuildVersion);
		TempBuf.Add(m_byClass);
		TempBuf.Add((BYTE)m_bPkStatus);
		TempBuf.AddString(m_strLoveName);

		for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) 
			TempBuf.Add(m_UserItem[i].sSid);	// EBody
		if(m_UserItem[TOTAL_ITEM_NUM-2].sSid!=-1&&m_UserItem[TOTAL_ITEM_NUM-2].sDuration!=0){
			//BYTE code=;
			//static BYTE code=0x00;
			TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]));
			//code++;
			TempBuf.Add((BYTE)0x00);
		}else{
			//TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]);
			TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]);
			TempBuf.Add((BYTE)0xff);
		}
		TempBuf.AddString(m_PersonalShopName);
//////////////////////////////////////////////////////////////////////
		//国服此处多5byte 0x0a 0x00 0x00 0x00 0x00
		//韩服在此添加宠物
		TempBuf.Add((BYTE)0x01);
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add((BYTE)m_tBabyCall);
	    if(m_tBabyCall)
	    {
		   TempBuf.Add(g_szBabyName[m_sBabyID%2],strlen(g_szBabyName[m_sBabyID%2]));
		   TempBuf.Add(m_sBabyID);
	    }
//SaveDump(TempBuf,TempBuf.GetLength());
	switch(towho)
	{
	case TO_ALL:
		SendAll(TempBuf, TempBuf.GetLength());
		break;

	case TO_ME:
		Send(TempBuf, TempBuf.GetLength());
		break;

	case TO_ZONE:
		SendZone(TempBuf, TempBuf.GetLength());
		break;

	case TO_INSIGHT:
	default:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;
	}
	if(type == INFO_DELETE)
		SendHuFaPostion(HUFA_HIDE);
	else
		SendHuFaInfo(this,towho);
}

///////////////////////////////////////////////////////////////////////////////
//	葛电 蜡历俊霸 单捞磐 傈价
///////////////////////////////////////////////////////////////////////////////
void USER::SendAll(TCHAR *pBuf, int nLength)
{
	if( nLength <= 0 || nLength >= SEND_BUF_SIZE ) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if( !pNewData ) return;

	pNewData->flag = SEND_ALL;
	pNewData->len = nLength;

	::CopyMemory( pNewData->pBuf, pBuf, nLength );

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	
}

///////////////////////////////////////////////////////////////////////////////
//Zone------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
void USER::SendZone(TCHAR *pBuf, int nLength)
{
	if( nLength <= 0 || nLength >= SEND_BUF_SIZE ) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if( !pNewData ) return;

	pNewData->flag = SEND_ZONE;
	pNewData->len = nLength;

	::CopyMemory( pNewData->pBuf, pBuf, nLength );

	pNewData->z = m_curz;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	//

	/*
	USER* pUser = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = m_pCom->user_array[i];

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_curz) continue;

		pUser->Send(pBuf, nLength);
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////
//	漂沥康开郴俊 乐绰 蜡历俊霸 单捞磐 傈价
///////////////////////////////////////////////////////////////////////////////
void USER::SendRange(TCHAR *pBuf, int nLength, CRect rect)
{
	if( nLength <= 0 || nLength >= SEND_BUF_SIZE) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if( !pNewData ) return;

	pNewData->flag = SEND_RANGE;
	pNewData->len = nLength;

	::CopyMemory( pNewData->pBuf, pBuf, nLength );

	pNewData->uid = 0;
	pNewData->z = m_curz;
	pNewData->rect = rect;
	pNewData->zone_index = m_ZoneIndex;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	//
}

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
void USER::SendInsight(TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;
	
	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_INSIGHT;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = m_curx;
	pNewData->y = m_cury;
	pNewData->z = m_curz;
	pNewData->zone_index = m_ZoneIndex;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
}

void USER::SendExactScreen(TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;
	
	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_SCREEN;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = m_curx;
	pNewData->y = m_cury;
	pNewData->z = m_curz;
	pNewData->zone_index = m_ZoneIndex;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	//
}

//////////////////////////////////////////////////////////////////////////////
//	矫具函版饶 巢酒 乐绰 矫具狼 蜡历俊霸 单捞磐 傈价
//
void USER::SendRemainSight(TCHAR *pBuf, int nLength, CPoint pt)
{
/*	USER* pSendUser = NULL;
	CCellUidArray arUid;
	int nDirection = g_cell[m_nCellZone]->GetDirection(pt, m_ptCell);

	int nD = nDirection - 1;
	int xx = 0, yy = 0, i, j;
	int nUid = 0, nSize = 0;
	int cx, cy;

	for(i = 0; i < 6; i++)
	{
		if(g_rPt[nD][i].x == 100) break;
		
		// Add ...
		xx = g_rPt[nD][i].x;
		yy = g_rPt[nD][i].y;
		
		cx = pt.x + xx;
		cy = pt.y + yy;
		if(cx < 0 || cx >= g_cell[m_nCellZone]->m_sizeCell.cx) continue;
		if(cy < 0 || cy >= g_cell[m_nCellZone]->m_sizeCell.cy) continue;
		
		g_cell[m_nCellZone]->m_pCell[cx][cy].Get(arUid);
		nSize = arUid.GetSize();
		
		for(j = 0; j < nSize; j++)
		{
			nUid = arUid.GetAt(j);
			if(nUid >= USER_BAND && nUid < NPC_BAND)
			{
				pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
				if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_curz) continue;
				
				pSendUser->Send(pBuf, nLength);
			}
		}
	}

	arUid.RemoveAll();
*/
}

//////////////////////////////////////////////////////////////////////////////
//	茄 拳搁郴狼 蜡历俊霸 单捞磐 傈价
//
void USER::SendScreen(TCHAR *pBuf, int nLength)
{
	USER* pUser = NULL;
	int dx, dy;

	for(int i = 0; i< MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_curz) continue;

		dx = abs(m_curx - pUser->m_curx);
		dy = abs(m_cury - pUser->m_cury);

		if((dx + dy) <= (SCREEN_Y * 2))
		{
			pUser->Send(pBuf, nLength);
		}
	}
}

//##############################################################################
//
//	PC 框流烙 包访 风凭
//
//##############################################################################

//////////////////////////////////////////////////////////////////////////////
//	规氢傈券 夸备甫 贸府茄促.
//
void USER::ChangeDir(TCHAR *pBuf)
{
	int		index = 0;
	BYTE	dir;

	dir = GetByte(pBuf, index);
	if(dir < 0 || dir >= 8) return;

	m_tDir = dir;

	CBufferEx TempBuf;
	TempBuf.Add(CHANGE_DIR_RESULT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_tDir);

//	SendInsight(TempBuf, TempBuf.GetLength());
	SendExactScreen(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	泅犁 焊绊乐绰 规氢甫 钎矫茄促. 
//
int USER::GetDirection(CPoint ptStart, CPoint ptEnd, int nDirCount)
{
	int nDir;					//	3 4 5
								//	2 8 6
								//	1 0 7
	int deltax = ptEnd.x - ptStart.x;
	int deltay = ptEnd.y - ptStart.y;

	if (deltax==0) {
		if (ptEnd.y>ptStart.y) nDir = DIR_DOWNRIGHT;		// ->		
		else nDir = DIR_UPLEFT;							// ->
	}
	else {
		double lean = (double)abs(deltay)/abs(deltax);

		if (ptEnd.y>ptStart.y) {
			if (ptEnd.x>ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_RIGHT;
					else if (lean>2.) nDir = DIR_DOWN;
					else nDir = DIR_RIGHT;		// -> 
				}
				else {
					if (lean<.2) nDir = DIR_RIGHT;
					else if (lean<.4) nDir = DIR_RIGHT+8;
					else if (lean<1) nDir = DIR_DOWNRIGHT;
					else if (lean<5) nDir = DIR_DOWNRIGHT+8;
					else			 nDir = DIR_DOWN;
				}

			}
			else if (ptEnd.x < ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_LEFT;
					else if (lean>2.) nDir = DIR_DOWN;
					else nDir = DIR_DOWN;		// -> 
				}
				else {
					if (lean<.2) nDir = DIR_LEFT;
					else if (lean<.4) nDir = DIR_LEFT+7;
					else if (lean<1) nDir = DIR_DOWNLEFT;
					else if (lean<5) nDir = DIR_DOWNLEFT+7;
					else			 nDir = DIR_DOWN;
				}
			}
		}
		else {
			if (ptEnd.x>ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_UPRIGHT;	// ->
					else if (lean>2.) nDir = DIR_UP;
					else nDir = DIR_UP;		// ->
				}
				else {
					if (lean<.2) nDir = DIR_RIGHT;
					else if (lean<.4) nDir = DIR_RIGHT+7;
					else if (lean<1) nDir = DIR_UPRIGHT;
					else if (lean<5) nDir = DIR_UPRIGHT+7;
					else			 nDir = DIR_UP;
				}
			}
			else if (ptEnd.x < ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_DOWNLEFT;		// -> 
					else if (lean>2.) nDir = DIR_UP;
					else nDir = DIR_LEFT;				// -> 2()
				} else {
					if (lean<.2) nDir = DIR_LEFT;
					else if (lean<.4) nDir = DIR_LEFT+8;
					else if (lean<1) nDir = DIR_UPLEFT;
					else if (lean<5) nDir = DIR_UPLEFT+8;
					else			 nDir = DIR_UP;
				}
			}
		}
	}

	return nDir;
/*if (deltax==0){
		if (ptEnd.y>ptStart.y) nDir = DIR_DOWN;		// ->DIR_DOWNRIGHT		
		else nDir = DIR_UP;							// ->DIR_UPLEFT
	}
	else {
		double lean = (double)abs(deltay)/abs(deltax);

		if (ptEnd.y>ptStart.y) {
			if (ptEnd.x>ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_RIGHT;
					else if (lean>2.) nDir = DIR_DOWN;
					else nDir = DIR_DOWNRIGHT;		// -> DIR_RIGHT
				}
				else {
					if (lean<.2) nDir = DIR_RIGHT;
					else if (lean<.4) nDir = DIR_RIGHT+8;
					else if (lean<1) nDir = DIR_DOWNRIGHT;
					else if (lean<5) nDir = DIR_DOWNRIGHT+8;
					else			 nDir = DIR_DOWN;
				}

			}
			else if (ptEnd.x < ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_LEFT;
					else if (lean>2.) nDir = DIR_DOWN;
					else nDir = DIR_DOWNLEFT;		// -> DIR_DOWN
				}
				else {
					if (lean<.2) nDir = DIR_LEFT;
					else if (lean<.4) nDir = DIR_LEFT+7;
					else if (lean<1) nDir = DIR_DOWNLEFT;
					else if (lean<5) nDir = DIR_DOWNLEFT+7;
					else			 nDir = DIR_DOWN;
				}
			}
		} else {
			if (ptEnd.x>ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_RIGHT;	// ->DIR_UPRIGHT
					else if (lean>2.) nDir = DIR_UP;
					else nDir = DIR_UPRIGHT;		// ->DIR_UP
				} else {
					if (lean<.2) nDir = DIR_RIGHT;
					else if (lean<.4) nDir = DIR_RIGHT+7;
					else if (lean<1) nDir = DIR_UPRIGHT;
					else if (lean<5) nDir = DIR_UPRIGHT+7;
					else			 nDir = DIR_UP;
				}
			}
			else if (ptEnd.x < ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_LEFT;		// -> DIR_DOWNLEFT
					else if (lean>2.) nDir = DIR_UP;
					else nDir = DIR_UPLEFT;				// -> 2(DIR_LEFT)
				}
				else {
					if (lean<.2) nDir = DIR_LEFT;
					else if (lean<.4) nDir = DIR_LEFT+8;
					else if (lean<1) nDir = DIR_UPLEFT;
					else if (lean<5) nDir = DIR_UPLEFT+8;
					else			 nDir = DIR_UP;
				}
			}
		}
	}
*/
//m_tDir =  nDir;
}
////////////////////////////////////////////////////////////////////////////////
//走路请求
////////////////////////////////////////////////////////////////////////////////
void USER::MoveFirstReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int will_x, will_y, x, y;
	int index = 0;
    DWORD checkTick;

	x = will_x = GetShort( pBuf, index );
	y = will_y = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );
    checkTick = GetDWORD(pBuf, index);
	//if(CheckMoveSpeedHack(checkTick)) return;//检测变速老代码
	//if(Move_C(will_x, will_y)) result = SUCCESS;//检测变速老代码
	//////////////////////////////////////////////////
	if(CheckMoveSpeedHack(610) == FALSE)//封变速新代码
	{
		if(Move_C(will_x, will_y)) result = SUCCESS;
	}
///////////////////////////////////////////////////////////	
	index = 0;
	SetByte(m_TempBuf, MOVE_FIRST_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		CPoint t = ConvertToClient(m_curx, m_cury);
		SetShort(m_TempBuf, t.x, index);
		SetShort(m_TempBuf, t.y, index);

		SendInsight(m_TempBuf, index);
		
		//TRACE("Move First Fail : uid = %d, UserName = %s, curx = %d, cury = %d\n", m_uid, m_strUserID, t.x, t.y);
		return;
	}
	

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, will_x, index);
	SetShort(m_TempBuf, will_y, index);

	SendInsight(m_TempBuf, index);

	//发送护法移动
//	SendHuFaInfo(this,TO_INSIGHT);
	SendHuFaMoveInfo(MOVE_FIRST_RESULT);
///////////////测试VIP走路显示头顶NEW/////////////////////////////   NEO版本  测试闪光
		if (o_yehuoini[0]->bbzh == 0)
 {
	//if(m_dwGuarDianTianShi > 0)
	 if(isGuildRen == TRUE || m_bCBOff == TRUE )  
	{
		AddHuFaStatus(New_Status_22);//VIP显示特殊符号以及头上带NEW
		SendJudgePsiStatus();
	}
	else
	{
		DelHuFaStatus(New_Status_22);
		SendJudgePsiStatus();
	}
 }

///////////////测试VIP走路显示头顶NEW/////////////////////////////

	SightRecalc();

//	UserTimer();		// SP甫 老沥 TICK付促 盲况霖促. 
}

//////////////////////////////////////////////////////////////////////////////////
//	PC 啊 框流捞绰 吝狼 贸府
//
void USER::MoveReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int will_x, will_y, x, y;
	int index = 0;
    DWORD checkTick;

	x = will_x = GetShort( pBuf, index );	// 框流捞妨绰 痢
	y = will_y = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );
    checkTick = GetDWORD(pBuf, index);
	//if(CheckMoveSpeedHack(checkTick)) return;//原来检测变速老代码

	//if(Move_C(x, y)) result = SUCCESS;//原来检测变速老代码
	//////////////////////////////////////////////////////////
	if(CheckMoveSpeedHack(610) == FALSE)//检测变速新代码
	{
		if(Move_C(will_x, will_y)) result = SUCCESS;
	}
/////////////////////////////////////////////////////////////	
	index = 0;
	SetByte(m_TempBuf, MOVE_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		CPoint t = ConvertToClient(m_curx, m_cury);
		SetShort(m_TempBuf, t.x, index);
		SetShort(m_TempBuf, t.y, index);

		SendInsight(m_TempBuf, index);
		
		//TRACE("Move Fail : uid = %d, UserName = %s, curx = %d, cury = %d\n", m_uid, m_strUserID, t.x, t.y);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, will_x, index);
	SetShort(m_TempBuf, will_y, index);
	
	SendInsight(m_TempBuf, index);
	//发送护法移动
//	SendHuFaInfo(this,TO_INSIGHT);
	SendHuFaMoveInfo(MOVE_RESULT);

    SightRecalc();

//	UserTimer();		// SP甫 老沥 TICK付促 盲况霖促. 
}
/////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////
void USER::MoveEndReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int end_x, end_y, x, y;
	int index = 0;
    DWORD checkTick;

	x = end_x = GetShort( pBuf, index );	// 框流捞妨绰 付瘤阜痢
	y = end_y = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );
    checkTick = GetDWORD(pBuf, index);
	//if(CheckMoveSpeedHack(checkTick)) return;//原来检测变速老代码

	//if(Move_C(x, y)) result = SUCCESS;//原来检测变速老代码
	//////////////////////////////////////////////////////
	if(CheckMoveSpeedHack(610) == FALSE)//封变速新代码
	{
		if(Move_C(x, y)) result = SUCCESS;
	}
///////////////////////////////////////////////////////////	
	index = 0;
	SetByte(m_TempBuf, MOVE_END_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		CPoint t = ConvertToClient(m_curx, m_cury);
		SetShort(m_TempBuf, t.x, index);
		SetShort(m_TempBuf, t.y, index);

		SendInsight(m_TempBuf, index);
		
		//TRACE("Move End Fail : uid = %d, UserName = %s, curx = %d, cury = %d\n", m_uid, m_strUserID, t.x, t.y);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, end_x, index);
	SetShort(m_TempBuf, end_y, index);
	
	SendInsight(m_TempBuf, index);
	//发送护法移动
	//SendHuFaInfo(this,TO_INSIGHT);
	SendHuFaMoveInfo(MOVE_END_RESULT);

    SightRecalc();

}

////////////////////////////////////////////////////////////////////////////////
//	漂沥 谅钎肺 捞悼 (款康磊葛靛)
//
void USER::MoveChatReq(TCHAR *pBuf, BOOL bPsi)
{
	if(bPsi == FALSE && m_tIsOP != 1) return;

	BYTE result = FAIL;
	int will_x, will_y;
	int index = 0;

	will_x = GetShort( pBuf, index );	// 框流捞妨绰 痢
	will_y = GetShort( pBuf, index );

	CPoint pt = FindNearAvailablePoint_C(will_x, will_y);
//	if(pt.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pt.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return;

	CPoint pc;
	if(pt.x != -1 && pt.y != -1) 
	{
		pc = ConvertToServer(pt.x, pt.y);
		if(pc.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pc.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return;

		if(pc.x < 0 || pc.y < 0) 
		{
			result = FAIL;
		}
		else
		{
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pc.x][pc.y].m_lUser, USER_BAND + m_uid);
			m_curx = pc.x;
			m_cury = pc.y;

			result = SUCCESS;
		}
	}

	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		// Move Chat 俊档 持绢拎具 窍唱?
//CPoint t = ConvertToClient(m_curx, m_cur_y);
//SetShort(m_TempBuf, t.x, index);
//SetShort(m_TempBuf, t.y, index);

		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, pt.x, index);
	SetShort(m_TempBuf, pt.y, index);
	Send(m_TempBuf, index);

	SendInsight(m_TempBuf, index);

	SightRecalc();
	//yskang 0.4
	if( m_tIsOP == 1 )
    {
	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	sprintf(strOP,"move_chat:x=%d,y=%d", pt.x,pt.y);
	WriteOpratorLog(strOP,CHAT_LOG);
	}
}
//////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
void USER::MopCatchReq(TCHAR *pBuf)
{
	int index = 0;
	TCHAR szNpcName[80];

	::ZeroMemory(szNpcName, sizeof(szNpcName));

	if(!GetVarString(sizeof(szNpcName), szNpcName, pBuf, index)) return;

	CNpc* pNpc = GetNpc(szNpcName);
	if(pNpc == NULL) return;

	BYTE result = FAIL;
	int will_x, will_y;

	will_x = pNpc->m_sCurX;	// 框流捞妨绰 痢
	will_y = pNpc->m_sCurY;

	CPoint pt = FindNearAvailablePoint_S(will_x, will_y);
	CPoint ptNew;
	if(pt.x != -1 && pt.y != -1) 
	{
		ptNew = ConvertToClient(pt.x, pt.y);
		if(ptNew.x != -1 && ptNew.y != -1)
		{
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + m_uid);
			m_curx = pt.x;
			m_cury = pt.y;

			result = SUCCESS;
		}
	}

	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		// Move Chat 俊档 持绢拎具 窍唱?
//^^	CPoint t = ConvertToClient(m_curx, m_cur_y);
//		SetShort(m_TempBuf, t.x, index);
//		SetShort(m_TempBuf, t.y, index);
//		TRACE("Move Chat Req Fail - %d : %d %d\n", m_uid, will_x, will_y);
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	Send(m_TempBuf, index);

	SendInsight(m_TempBuf, index);

	SightRecalc();

	//yskang 0.4
	if( m_tIsOP == 1 )
    {
	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	sprintf(strOP,"trace_npc:%s,x=%d,y=%d",szNpcName,ptNew.x,ptNew.y);
	WriteOpratorLog(strOP,CHAT_LOG);
	}
}

///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void USER::UserCallReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;
	CPoint pt, ptNew;
    CString moonname =_T("") ;

	moonname=m_strUserID;	
	if( m_tIsOP != 1 ) return;									// 款康磊啊 酒丛

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength == 0 || nLength > CHAR_NAME_LENGTH) return;		
	
	pUser = GetUser(szUserName);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	
	if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳
	
//	pt = FindNearAvailablePoint_S(m_curx, m_cury);
//	if(pt.x <= -1 && pt.y <= -1) return;						// 款康磊 林困肺 棵 谅钎啊 绝澜

	if(pUser->m_curz != m_curz)									// 蜡历客 款康磊啊 鞍篮粮俊 乐瘤 臼阑 版快
	{
		if( !IsZoneInThisServer(pUser->m_curz) ) return;		// 郴 辑滚救俊 乐绰粮捞 酒丛 府畔

		ptNew = ConvertToClient(m_curx, m_cury);
		BOOL bSuccess = pUser->ZoneChangeProcess(m_curz, ptNew.x, ptNew.y);	//^^ Check 夸噶
		
		if(bSuccess)
		{
//			pUser->SendZoneChange(bSuccess);
			pUser->SendWeatherInMoveZone();				// 捞悼 粮狼 朝揪函拳甫 舅赴促.
		}
		return;
	}
	else
	{													// 鞍篮 粮捞搁 捞悼栏肺...		
/*
		ptNew = ConvertToClient(pt.x, pt.y);
		if(ptNew.x == -1 || ptNew.y == -1) return;

		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_lUser, 0);
		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + pUser->m_uid);
		pUser->m_curx = pt.x;
		pUser->m_cury = pt.y;
		
		result = SUCCESS;
*/
		pt = pUser->FindNearAvailablePoint_S( m_curx, m_cury );
		ptNew = ConvertToClient( pt.x, pt.y );

		if( ptNew.x == -1 || ptNew.y == -1 ) return;

		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_lUser, 0);
		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + pUser->m_uid);
		pUser->m_curx = pt.x;
		pUser->m_cury = pt.y;
		
		result = SUCCESS;
	}
	
	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, pUser->m_uid + USER_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	pUser->Send(m_TempBuf, index);	// 蜡历俊霸绰 货肺款 谅钎蔼阑...
	
	pUser->SendInsight(m_TempBuf, index);
	
	pUser->SightRecalc();

	//yskang 0.4
	if( m_tIsOP == 1 )
    {
	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	sprintf(strOP,"call_user:%s,z=%d,x=%d,y=%d",pUser->m_strUserID, pUser->m_curz,pUser->m_curx,pUser->m_cury);
	WriteOpratorLog(strOP,CALL_USER_LOG);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
void USER::UserRemoteCallReq(TCHAR *pBuf)
{
	CBufferEx TempBuf;
	int will_x = 0, will_y = 0;
	BYTE result = FAIL, error_code = 0;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;
	CPoint pt, ptNew;

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;
	
	pUser = GetUser(szUserName);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	
	if( pUser->m_ZoneIndex < 0 || pUser->m_ZoneIndex >= g_zone.GetSize() ) 
	{
		m_curz = 1;
		m_curx = 192;
		m_cury = 803;
		SetZoneIndex(m_curz);
		return;
	}
	if(pUser->m_curx >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pUser->m_curx < 0)
	{
		m_curz = 1;
		m_curx = 192;
		m_cury = 803;
		SetZoneIndex(m_curz);
		return;
	}
	if(pUser->m_cury >= g_zone[m_ZoneIndex]->m_sizeMap.cy || pUser->m_cury < 0) 
	{
		m_curz = 1;
		m_curx = 192;
		m_cury = 803;
		SetZoneIndex(m_curz);
		return;
	}

	if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;		// 磊扁 磊脚篮 救凳

	will_x = GetShort( pBuf, index );
	will_y = GetShort( pBuf, index );

	pt = FindNearAvailablePoint_C(will_x, will_y);	// 府畔登绰 谅钎档 努扼捞攫飘 谅钎促
	if(pt.x == -1 || pt.y == -1) 
	{ 
		// 款康磊啊 款康阑 奖霸 窍扁 困秦辑绰 咯扁辑 款康磊俊霸 角菩皋技瘤甫 焊郴绰霸 亮阑巴 鞍促.
		return;
	}
	
//	ptNew = ConvertToClient(pt.x, pt.y);
//	if(ptNew.x == -1 || ptNew.y == -1) return;
	ptNew = pt;
	pt = ConvertToServer( pt.x, pt.y );
	if(pt.x == -1 || pt.y == -1) return;
	
	::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_lUser, 0);
	::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + pUser->m_uid);
	pUser->m_curx = pt.x;
	pUser->m_cury = pt.y;
	
	result = SUCCESS;

	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, pUser->m_uid + USER_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	pUser->Send(m_TempBuf, index);	// 蜡历俊霸绰 货肺款 谅钎蔼阑...
	
	SendInsight(m_TempBuf, index);
	
	SightRecalc();
	//yskang 0.4
	if( m_tIsOP == 1 )
    {
	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	sprintf(strOP,"Remote_User:%s,x=%d,y=%d", pUser->m_strUserID,ptNew.x,ptNew.y);
	WriteOpratorLog(strOP,CALL_USER_LOG);
	}
}

/*
//////////////////////////////////////////////////////////////////////////////////
//	泅犁 困摹甫 吝缴栏肺 25 伎吝 框流老 荐 乐绰 谅钎啊 乐绰瘤 魄窜
//
CPoint USER::FindNearAvailablePoint(int xpos, int ypos)
{
	int i;

	if(IsMovable(xpos, ypos)) return CPoint(xpos, ypos);	// 泅犁 困摹

	for(i = 0; i < sizeof(g_8x) / sizeof(g_8x[0]); i++)		// 林函 8规氢
	{
		if(IsMovable(xpos + g_8x[i], ypos + g_8y[i])) return CPoint(xpos + g_8x[i], ypos + g_8y[i]);
	}

	for(i = 0; i < sizeof(g_16x) / sizeof(g_16x[0]); i++)	// 林函 16规氢
	{
		if(IsMovable(xpos + g_16x[i], ypos + g_16y[i])) return CPoint(xpos + g_16x[i], ypos + g_16y[i]);
	}

	return CPoint(-1, -1);
}
*/

//////////////////////////////////////////////////////////////////////////////////
//	泅犁 困摹甫 吝缴栏肺 25 伎吝 酒捞袍甫 冻绢龙荐乐绰 谅钎甫 伐待栏肺 积己
//
CPoint USER::FindNearRandomPoint(int x, int y)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return CPoint(-1, -1);

	CPoint t;
	int i;
	int iX, iY;
	int rand_x = 1, rand_y = 1;

	MAP *pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return CPoint(-1, -1);
	if( !pMap->m_pMap ) return CPoint(-1, -1);
	if(x >= pMap->m_sizeMap.cx || x < 0 || y >= pMap->m_sizeMap.cy || y < 0) return CPoint(-1, -1);

	int dir[25][2];

	if(g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser == 0)
	{
		if( IsMovable_S( x, y ) )
		{
			if(g_zone[m_ZoneIndex]->m_pMap[x][y].iIndex == -1) return CPoint( x, y );
		}
	}

//	X					Y
	dir[0][0]  =  0;		dir[0][1] =  0;		// 
	dir[1][0]  = -1;		dir[1][1] =  0;		// 
	dir[2][0]  = -1;		dir[2][1] =  1;		// 
	dir[3][0]  =  0;		dir[3][1] =  1;		// 
	dir[4][0]  =  1;		dir[4][1] =  1;		// 

	dir[5][0]  =  1;		dir[5][1] =  0;		// 
	dir[6][0]  =  1;		dir[6][1] = -1;		// 
	dir[7][0]  =  0;		dir[7][1] = -1;		// 
	dir[8][0]  = -1;		dir[8][1] = -1;		// 
	dir[9][0]  = -2;		dir[9][1] = -1;		// 

	dir[10][0] = -2;		dir[10][1] =  0;	// 
	dir[11][0] = -2;		dir[11][1] =  1;	// 
	dir[12][0] = -2;		dir[12][1] =  2;	// 
	dir[13][0] = -1;		dir[13][1] =  2;	// 
	dir[14][0] =  0;		dir[14][1] =  2;	// 

	dir[15][0] =  1;		dir[15][1] =  2;	// 
	dir[16][0] =  2;		dir[16][1] =  2;	// 
	dir[17][0] =  2;		dir[17][1] =  1;	// 
	dir[18][0] =  2;		dir[18][1] =  0;	// 
	dir[19][0] =  2;		dir[19][1] = -1;	// 

	dir[20][0] =  2;		dir[20][1] = -2;	// 
	dir[21][0] =  1;		dir[21][1] = -2;	// 
	dir[22][0] =  0;		dir[22][1] = -2;	// 
	dir[23][0] = -1;		dir[23][1] = -2;	// 
	dir[24][0] = -2;		dir[24][1] = -2;	// 

	rand_x = myrand(1, 24, TRUE);
	rand_y = myrand(0, 1, TRUE);

	iX = dir[rand_x][rand_y] + x;
	iY = dir[rand_x][rand_y] + y;

	rand_x = iX; rand_y = iY;

	if( rand_x >= pMap->m_sizeMap.cx || rand_x < 0 || rand_y >= pMap->m_sizeMap.cy || rand_y < 0) return CPoint(-1, -1);

	if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].m_lUser == 0)
	{
		if( IsMovable_S( rand_x, rand_y ) )
		{
			if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].iIndex == -1) return CPoint( rand_x, rand_y );
		}
	}

	rand_x = x, rand_y = y;

	for( i = 1; i < 25; i++)
	{
		iX = rand_x + dir[i][0];
		iY = rand_y + dir[i][1];

		if( iX >= pMap->m_sizeMap.cx || iX < 0 || iY >= pMap->m_sizeMap.cy || iY < 0) continue;
		if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].m_lUser != 0) continue;	// 后 甘牢瘤 犬牢茄促.

		if( IsMovable_S( iX, iY ) )
		{
			if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].iIndex == -1) return CPoint( iX, iY );
		}
	}

	return CPoint(-1, -1);
}


//////////////////////////////////////////////////////////////////////////////////
//
BOOL USER::Move_C(int x, int y, int nDist /* = 1 */)
{
	CPoint t = ConvertToServer(x, y);
	if( t.x == -1 || t.y == -1 ) return FALSE;
	
//	if(!GetDistance(t.x, t.y, nDist)) return FALSE;
	// alisia
	if( abs( m_curx - t.x ) > nDist || abs( m_cury - t.y ) > nDist ) return FALSE;

	if(!IsMovable_S(t.x, t.y)) return FALSE;
	if(t.x == m_curx && t.y == m_cury) return FALSE;

		// jjs
	if(g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == 0 || g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == m_uid + USER_BAND)
	{
		//::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
		g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser = 0;
	}
	//::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[t.x][t.y].m_lUser, m_uid + USER_BAND);
	g_zone[m_ZoneIndex]->m_pMap[t.x][t.y].m_lUser = m_uid + USER_BAND;

	m_curx = t.x;
	m_cury = t.y;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	辑滚谅钎甫 扁霖栏肺 x, y 啊 捞悼且 荐 乐绰 痢牢瘤 魄窜
//
BOOL USER::IsMovable_S(int x, int y)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	if( !g_zone[m_ZoneIndex] ) return FALSE;
	if( !g_zone[m_ZoneIndex]->m_pMap ) return FALSE;
	if( x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || x < 0 || y >= g_zone[m_ZoneIndex]->m_sizeMap.cy || y < 0) return FALSE;

//	CPoint t = ConvertToClient( x, y );		// 框流捞妨绰 辑滚谅钎啊 努扼捞攫飘俊辑 给框流捞绰 谅钎搁 府畔
//	if( t.x == -1 || t.y == -1 ) return FALSE;

	if(g_zone[m_ZoneIndex]->m_pMap[x][y].m_bMove) return FALSE;

	int temp_uid = g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser;

	if( temp_uid != 0 && temp_uid != m_uid + USER_BAND)
	{
		if( temp_uid >= NPC_BAND && temp_uid < INVALID_BAND)
		{
			CNpc *pNpc = NULL;
			pNpc = GetNpc( temp_uid - NPC_BAND );

			if( pNpc == NULL )
			{
				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
			if( pNpc->m_sCurX != x || pNpc->m_sCurY != y )
			{
				if(pNpc->m_sDimension > 0) return FALSE;

				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
		}
		else if(temp_uid >= USER_BAND && temp_uid < NPC_BAND)
		{
			if(temp_uid - USER_BAND < 0 || temp_uid - USER_BAND >= MAX_USER)
			{
				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}

			USER *pUser = NULL;
			pUser = GetUser( temp_uid - USER_BAND );

			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)
			{
				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}

			if( pUser->m_curx != x || pUser->m_cury != y || pUser->m_state != STATE_GAMESTARTED )
			{
				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
		}

		return FALSE;
	}

	return TRUE;
}
BOOL USER::IsMovable_S(int zone, int x, int y)
{
	int ZoneIndex = -1;
	for(int j = 0; j < g_zone.GetSize(); j++)
	{
		if( g_zone[j]->m_Zone == zone )
		{
			ZoneIndex = j;
			break;
		}
	}
	if( ZoneIndex < 0 || ZoneIndex >= g_zone.GetSize() ) return FALSE;

	if( !g_zone[ZoneIndex] ) return FALSE;
	if( !g_zone[ZoneIndex]->m_pMap ) return FALSE;
	if( x >= g_zone[ZoneIndex]->m_sizeMap.cx || x < 0 || y >= g_zone[ZoneIndex]->m_sizeMap.cy || y < 0) return FALSE;

//	CPoint t = ConvertToClient( x, y );		// 框流捞妨绰 辑滚谅钎啊 努扼捞攫飘俊辑 给框流捞绰 谅钎搁 府畔
//	if( t.x == -1 || t.y == -1 ) return FALSE;

	if(g_zone[ZoneIndex]->m_pMap[x][y].m_bMove) return FALSE;

	int temp_uid = g_zone[ZoneIndex]->m_pMap[x][y].m_lUser;

	if( temp_uid != 0 && temp_uid != m_uid + USER_BAND)
	{
		if( temp_uid >= NPC_BAND && temp_uid < INVALID_BAND)
		{
			CNpc *pNpc = NULL;
			pNpc = GetNpc( temp_uid - NPC_BAND );

			if( pNpc == NULL )
			{
				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
			if( pNpc->m_sCurX != x || pNpc->m_sCurY != y )
			{
				if(pNpc->m_sDimension > 0) return FALSE;

				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
		}
		else if(temp_uid >= USER_BAND && temp_uid < NPC_BAND)
		{
			if(temp_uid - USER_BAND < 0 || temp_uid - USER_BAND >= MAX_USER)
			{
				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}

			USER *pUser = NULL;
			pUser = GetUser( temp_uid - USER_BAND );

			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)
			{
				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}

			if( pUser->m_curx != x || pUser->m_cury != y || pUser->m_state != STATE_GAMESTARTED )
			{
				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
		}

		return FALSE;
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//	泅犁 蜡历狼 困摹客 (xpos, ypos) 荤捞狼 芭府甫 拌魂
//
BOOL USER::GetDistance(int xpos, int ypos, int dist, int* ret)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
	if(xpos >= g_zone[m_ZoneIndex]->m_sizeMap.cx || xpos < 0 || ypos >= g_zone[m_ZoneIndex]->m_sizeMap.cy || ypos < 0) return FALSE;

	int dx = abs(xpos - m_curx);
	int dy = abs(ypos - m_cury);

	if(ret != NULL) *ret = (dx + dy) / 2;

	if((dx + dy) > (dist * 2)) return FALSE;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	贸澜 顿妨绊 且 锭狼 贸府
// 跑步请求
void USER::RunFirstReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	int wx_1, wy_1, x1, x2;
	int wx_2, wy_2, y1, y2;
//	int ex, ey;
	DWORD checkTick;

	CPoint ptOld(-1, -1);

	if(GetSP() <= 0) 
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_STAMINA, SYSTEM_NORMAL, TO_ME);
		goto fail_result;
	}

	x1 = wx_1 = GetShort( pBuf, index );
	y1 = wy_1 = GetShort( pBuf, index );
	x2 = wx_2 = GetShort( pBuf, index );
	y2 = wy_2 = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );

//	ex = GetShort( pBuf, index );
//	ey = GetShort( pBuf, index );

	checkTick = GetDWORD(pBuf, index);
	if(CheckRunSpeedHack(checkTick, x1 == x2)) return;

	if(IsMovable_C(x1, y1) == FALSE) goto fail_result;
	if(Move_C(x2, y2, 2) == FALSE)
	{
		if(Move_C(x1, y1, 2) == FALSE) goto fail_result;
		wx_2 = wy_2 = -1;	
	}
	
	DecSP(1);
	index = 0;
	result = SUCCESS;
	SetByte(m_TempBuf, RUN_FIRST_RESULT, index );
	SetByte(m_TempBuf, result, index);
	SetInt	(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, wx_1, index);
	SetShort(m_TempBuf, wy_1, index);
	SetShort(m_TempBuf, wx_2, index);
	SetShort(m_TempBuf, wy_2, index);
//	SetShort(m_TempBuf, ex, index);
//	SetShort(m_TempBuf, ey, index);

	SendInsight(m_TempBuf, index);

	//发送护法走路
	SendHuFaInfo(this,TO_INSIGHT);
////////////////////测试跑步显示VIP头顶NEW///////////////////////////  NEO版本    测试闪光
		if (o_yehuoini[0]->bbzh == 0)
{
	//if(m_dwGuarDianTianShi > 0)
	if(isGuildRen == TRUE || m_bCBOff == TRUE )  
	{
		AddHuFaStatus(New_Status_22);// IP显示特殊符号以及头上带NEW
		SendJudgePsiStatus();
	}
	else
	{
		DelHuFaStatus(New_Status_22);
		SendJudgePsiStatus();
	}
}

////////////////////测试跑步显示VIP头顶NEW///////////////////////////	
    SightRecalc();

	m_dwLastMoveAndRun = GetTickCount();

	SendSP();	//^^ 胶抛固呈 沥焊甫 鞍捞 持配废
	return;

fail_result:
	index = 0;

	ptOld = ConvertToClient(m_curx, m_cury);
	if(ptOld.x < 0 || ptOld.y < 0) return;

	SetByte(m_TempBuf, RUN_FIRST_RESULT, index );
	SetByte(m_TempBuf, FAIL, index);
	SetInt(m_TempBuf, m_uid + USER_BAND, index );
	SetShort(m_TempBuf, ptOld.x, index );
	SetShort(m_TempBuf, ptOld.y, index );

	SendInsight(m_TempBuf, index);
}

/////////////////////////////////////////////////////////////////////////////////////
//	顿绊 乐绰 吝狼 贸府
//
void USER::RunReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	int wx_1, wy_1, x1, y1;
	int wx_2, wy_2, x2, y2;
	DWORD checkTick;

	CPoint ptOld;

	if(GetSP() <= 0) 
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_STAMINA, SYSTEM_NORMAL, TO_ME);
		goto fail_result;
	}

	x1 = wx_1 = GetShort( pBuf, index );
	y1 = wy_1 = GetShort( pBuf, index );
	x2 = wx_2 = GetShort( pBuf, index );
	y2 = wy_2 = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );

	checkTick = GetDWORD(pBuf, index);
	if(CheckRunSpeedHack(checkTick, x1 == x2)) return;

	if(IsMovable_C(x1, y1) == FALSE) goto fail_result;
	if(Move_C(x2, y2, 2) == FALSE)
	{
		if(Move_C(x1, y1, 2) == FALSE) goto fail_result;
		wx_2 = wy_2 = -1;
	}

	DecSP(1);

	index = 0;
	result = SUCCESS;
	SetByte(m_TempBuf, RUN_RESULT, index );
	SetByte(m_TempBuf, result, index);

	SetInt	(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, wx_1, index);
	SetShort(m_TempBuf, wy_1, index);
	SetShort(m_TempBuf, wx_2, index);
	SetShort(m_TempBuf, wy_2, index);
	
	SendInsight(m_TempBuf, index);

	//发送护法走路
	SendHuFaInfo(this,TO_INSIGHT);
	
    SightRecalc();

	m_dwLastMoveAndRun = GetTickCount();
	SendSP();	//^^ 胶抛固呈 沥焊甫 鞍捞 持配废

	return;

fail_result:
	index = 0;
	ptOld = ConvertToClient(m_curx, m_cury);
	if(ptOld.x < 0 || ptOld.y < 0) return;

	SetByte(m_TempBuf, RUN_RESULT, index );
	SetByte(m_TempBuf, FAIL, index);
	SetInt(m_TempBuf, m_uid + USER_BAND, index );
	SetShort(m_TempBuf, ptOld.x, index );
	SetShort(m_TempBuf, ptOld.y, index );

	SendInsight(m_TempBuf, index);
}

//////////////////////////////////////////////////////////////////////////////////////
//	顿促啊 档馒痢俊 档崔沁阑 锭狼 贸府
//
void USER::RunEndReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	int wx_1, wy_1, x1, y1;
	int wx_2, wy_2, x2, y2;
	CPoint ptOld;
	DWORD checkTick;

	if(GetSP() <= 0) goto fail_result;

	x1 = wx_1 = GetShort( pBuf, index );
	y1 = wy_1 = GetShort( pBuf, index );
	x2 = wx_2 = GetShort( pBuf, index );
	y2 = wy_2 = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );

	checkTick = GetDWORD(pBuf, index);
	if(CheckRunSpeedHack(checkTick, x1 == x2)) return;

	if(x1 == x2 && y1 == y2)
	{
		if(Move_C(x1, y1, 2) == FALSE) goto fail_result;
		wx_2 = wy_2 = -1;
	}
	else
	{
		if(IsMovable_C(x1, y1) == FALSE) goto fail_result;
		if(Move_C(x2, y2, 2) == FALSE)
		{
			if(Move_C(x1, y1, 2) == FALSE) goto fail_result;
			wx_2 = wy_2 = -1;
		}
	}

	DecSP(1);

	index = 0;
	result = SUCCESS;
	SetByte(m_TempBuf, RUN_RESULT, index );
	SetByte(m_TempBuf, result, index);

	SetInt	(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, wx_1, index);
	SetShort(m_TempBuf, wy_1, index);
	SetShort(m_TempBuf, wx_2, index);
	SetShort(m_TempBuf, wy_2, index);
	
	SendInsight(m_TempBuf, index);

	//发送护法走路
	SendHuFaInfo(this,TO_INSIGHT);
	
    SightRecalc();

	m_dwLastMoveAndRun = GetTickCount();
	SendSP();	//^^ 胶抛固呈 沥焊甫 鞍捞 持配废

	return;

fail_result:
	index = 0;
	ptOld = ConvertToClient(m_curx, m_cury);
	if(ptOld.x < 0 || ptOld.y < 0) return;

	SetByte(m_TempBuf, RUN_END_RESULT, index );
	SetByte(m_TempBuf, FAIL, index);
	SetInt(m_TempBuf, m_uid + USER_BAND, index );
	SetShort(m_TempBuf, ptOld.x, index );
	SetShort(m_TempBuf, ptOld.y, index );

	SendInsight(m_TempBuf, index);
}

///////////////////////////////////////////////////////////////////////////////////
//	泅犁 SP 甫 掘绰促.
//
short USER::GetSP()
{
	BYTE	tWeaponClass = 255;

	int iSkillSid = 0;
	int iLevel = 0, iRandom = 0, iSP = 0;

	double dPlusSP = 1.0;
	int iAddSP = 0;

	DWORD dwDiffTime = GetTickCount() - m_dwLastMoveAndRun;

	if(dwDiffTime >= m_iSPIntervalTime)
	{
		if(IsCanUseWeaponSkill(tWeaponClass))	// 泅犁 磊脚捞 坷弗颊俊 电 公扁狼 胶懦捞 乐栏搁
		{
			int tClass = tWeaponClass * SKILL_NUM; 

			for(int i = tClass; i < tClass + SKILL_NUM; i++)			// 胶懦吝 SP雀汗 胶懦捞 乐栏搁 馆康茄促.
			{
				iSkillSid = m_UserSkill[i].sSid;

				if(iSkillSid == SKILL_SP_RECOVER_UP)					// 13 index
				{
					iLevel = m_UserSkill[i].tLevel;							

					// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
					if(g_DynamicSkillInfo[iSkillSid] >= MAGIC_COUNT) continue;
					if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
					//if(iLevel > SKILL_LEVEL) iLevel = SKILL_LEVEL;
					if(iSkillSid >= g_arSkillTable.GetSize()) continue;
					if(iLevel < 0 || iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)) iSP = 1;

					dPlusSP = 1.0 + (double)iSP * g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel) / 100.0;
				}
			}
		}
							// 焊沥蔼
		if(m_iSPRate <= 0) 
		{
			m_iSPRate = 1;
			TRACE("雀汗厚啦 俊矾\n");
		}

		if(IsCity() && CheckRecoverTableByClass()) iAddSP = g_arRecoverTable[m_byClass]->m_byTown;
		m_sSP += (int)(dPlusSP * (double)m_sMagicCON/m_iSPRate ) + iAddSP;
		if(m_sSP > m_sMagicMaxSP) m_sSP = m_sMagicMaxSP;

		m_dwLastMoveAndRun = GetTickCount();
	}

	return m_sSP;
}

BOOL USER::CheckRecoverTableByClass()
{
	if(m_byClass < 0 || m_byClass >= g_arRecoverTable.GetSize()) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	SP 甫 皑家矫挪促.
//
short USER::DecSP(short sDec)
{
	m_sSP -= sDec;
	if(m_sSP < 0) m_sSP = 0;

	return m_sSP;
}

////////////////////////////////////////////////////////////////////////////////////
//	SP 甫 努扼捞攫飘肺 焊辰促.
//
void USER::SendSP(BOOL bLevelDown)
{
	CBufferEx TempBuf;

	if(m_bLive == USER_DEAD && !bLevelDown) return;

	if(m_sSP > m_sMagicMaxSP) m_sSP = m_sMagicMaxSP;

	TempBuf.Add(SET_STAEMINA);
	TempBuf.Add(m_sSP);

	Send(TempBuf, TempBuf.GetLength());
}

//######################################################################
//
//	Character Chatting
//
//######################################################################


////////////////////////////////////////////////////////////////////////
//	盲泼 贸府(滚欺 辨捞甫 汲沥)
//
void USER::ChatReqWithLength(TCHAR *pBuf, int iLen)
{
	int index = 0;
	BYTE type = NORMAL_CHAT;
    CString moonname =_T("") ;

	type = GetByte(pBuf, index);

	if( pBuf[index+1] == '/' && type != WHISPER_CHAT )
	{
		ExecuteChatCommand( pBuf+index+2, iLen );
		return;
	}

	switch( type )
	{
	case NORMAL_CHAT:		// 老馆盲泼
		NormalChat(pBuf + index);
		break;

	case WHISPER_CHAT:		// 庇加富
		WhisperChat(pBuf + index);
		break;

	case SHOUT_CHAT:		// 8 拳搁 寇魔
		ShoutChat(pBuf + index);
		break;

	case BUDDY_INVITE: case BUDDY_OFF:	// Buddy 檬没, 呕硼
		BuddyUserChange(pBuf + index, type);
		break;
	case BUDDY_CHAT:		// Buddy 盲泼
		BuddyChat(pBuf + index);
		break;

	case ALL_CHAT:						// 傈眉 盲泼
		if(m_tIsOP != 1) break;			// 瘤陛篮 款康磊父 茄促.
		SendAllChat(pBuf + index);
		break;

	case WHISPER_OK:
	case WHISPER_DENY:
		WhisperOnOff(pBuf);
		break;

	case PK_COUNT_CHAT:
		SendPKCount();
		break;

	case MOP_CHAT:
		if(m_tIsOP != 1) break;
		MopCatchReq(pBuf + index);
		break;
	case MOP_CALL_CHAT:		// 17 各阑 家券	款康磊 辟贸肺... 
		if(m_tIsOP != 1) break;
		MopCallReq(pBuf + index);
		break;

	case CALL_CHAT:			// 15 蜡历甫 家券
		moonname=m_strUserID;	
		if( m_tIsOP != 1 ) break;
		UserCallReq(pBuf + index);
		break;

	case REMOTE_CHAT:		// 16 漂沥困摹肺 漂沥 蜡历甫 捞悼
		if(m_tIsOP != 1) break;
		UserRemoteCallReq(pBuf + index);
		break;

	case GUILD_CHAT:
		GuildChat(pBuf + index);
		break;

		case 37: //电光板
		DianGuangBan(pBuf + index);
		break;

//	case GUILD_INVITE:		// 18 辨靛 啊涝阑 狼汾茄促.	
//		GuildInvite(pBuf + index);
//		GuildInviteWithThread(pBuf + index);
//		break;
	case GUILD_OFF:			// 20 辨靛 呕硼
		GuildOff(pBuf + index);
//		GuildOffWithThread(pBuf + index);
		break;
	case GUILD_KICK_OUT:	// 21 辨靛盔 碍力 眠规
    	GuildKickOut(pBuf + index);
	//	GuildKickOutWithThread(pBuf + index);
		break;
	case GUILD_DISPERSE:
		GuildDisperse(pBuf + index);
//		GuildDisperseWithThread(pBuf + index);
		break;

	case ITEM_MODE_CHAT:	// 22 巢捞林绰 酒捞袍阑 铰倡, 芭何甫 汲沥.
		SetItemMode(pBuf + index);
		break;

	case GUILD_FIELD_WAR:
		SendGuildWarFieldApply(pBuf + index);
		break;

	case GUILD_SURRENDER:	// 26 鞘靛傈阑 亲汗茄促.
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// 亲汗
		}
		break;

	case COPY_TELEPORT:
		break;

	case GUILD_USER_CHECK:
		GuildUserAllInforChat();
		break;

	case GUILD_WAR_APPLY_LIST:
		SendFortressAttackGuildList(pBuf + index);
		break;

	case GUILD_FORTRESS_DOOR:
		OpenFortressDoor();
		break;

	case GUILD_DEPUTE_POWER:  //副团授权
		SendSystemMsg("授权对方", SYSTEM_NORMAL, TO_ME );
		DeputeGuildMasterOfPower(pBuf + index);
		break;

	case GUILD_STOP_POWER:  //终止授权
		SendSystemMsg("终止授权", SYSTEM_NORMAL, TO_ME );
		RemoveGuildMasterOfPower(pBuf + index);
		break;

	case GUILD_CALL_CHAT: //yskang 0.2 (35) 器飘府胶 辨付啊 蜡历家券
		GuildUserCallReq(pBuf + index);
		break;
	case GUILD_USER_INFO_CALL://yskang 0.2
		m_nGuildUserInfoType =0x03;
		GuildUserInfoReq(pBuf + index);
		break;
	}	
}

////////////////////////////////////////////////////////////////////////
//	盲泼 贸府
//
void USER::ChatReq(TCHAR *pBuf)
{
	int index = 0;
    CString moonname =_T("") ;
	BYTE type = NORMAL_CHAT;

	type = GetByte(pBuf, index);

	if( pBuf[index+1] == '/' && type != WHISPER_CHAT )
	{
		ExecuteChatCommand( pBuf+index+2 );
		return;
	}

	switch( type )
	{
	case NORMAL_CHAT:		// 老馆盲泼
		NormalChat(pBuf + index);
		break;

	case WHISPER_CHAT:		// 庇加富
		WhisperChat(pBuf + index);
		break;

	case SHOUT_CHAT:		// 8 拳搁 寇魔
		ShoutChat(pBuf + index);
		break;

	case BUDDY_INVITE: case BUDDY_OFF:	// Buddy 檬没, 呕硼
		BuddyUserChange(pBuf + index, type);
		break;
	case BUDDY_CHAT:		// Buddy 盲泼
		BuddyChat(pBuf + index);
		break;

	case ALL_CHAT:						// 傈眉 盲泼
		if(m_tIsOP != 1) break;			// 瘤陛篮 款康磊父 茄促.
		SendAllChat(pBuf + index);
		break;

	case WHISPER_OK:
	case WHISPER_DENY:
		WhisperOnOff(pBuf);
		break;

	case PK_COUNT_CHAT:
		SendPKCount();
		break;

	case MOP_CHAT:
		if(m_tIsOP != 1) break;
		MopCatchReq(pBuf + index);
		break;
	case MOP_CALL_CHAT:		// 17 各阑 家券	款康磊 辟贸肺... 
		if(m_tIsOP != 1) break;
		MopCallReq(pBuf + index);
		break;

	case CALL_CHAT:			// 15 蜡历甫 家券
		moonname=m_strUserID;	
		if( m_tIsOP != 1 ) break;
		UserCallReq(pBuf + index);
		break;

	case REMOTE_CHAT:		// 16 漂沥困摹肺 漂沥 蜡历甫 捞悼
		if(m_tIsOP != 1) break;
		UserRemoteCallReq(pBuf + index);
		break;

	case GUILD_CHAT:
		GuildChat(pBuf + index);
		break;
//	case GUILD_INVITE:		// 18 辨靛 啊涝阑 狼汾茄促.	
//		GuildInvite(pBuf + index);
//		GuildInviteWithThread(pBuf + index);
		break;
	case GUILD_OFF:			// 20 辨靛 呕硼
		GuildOff(pBuf + index);
//		GuildOffWithThread(pBuf + index);
		break;
	case GUILD_KICK_OUT:	// 21 辨靛盔 碍力 眠规
		GuildKickOut(pBuf + index);
//		GuildKickOutWithThread(pBuf + index);
		break;
	case GUILD_DISPERSE:
		GuildDisperse(pBuf + index);
//		GuildDisperseWithThread(pBuf + index);
		break;

	case ITEM_MODE_CHAT:	
		SetItemMode(pBuf + index);
		break;

	case GUILD_FIELD_WAR:
		SendGuildWarFieldApply(pBuf + index);
		break;

	case GUILD_SURRENDER:	// 26 鞘靛傈阑 亲汗茄促.
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// 亲汗
		}
		break;

	case COPY_TELEPORT:
		break;

	case GUILD_USER_CHECK:
		GuildUserAllInforChat();
		break;

	case GUILD_WAR_APPLY_LIST:
		SendFortressAttackGuildList(pBuf + index);
		break;

	case GUILD_FORTRESS_DOOR:
		OpenFortressDoor();
		break;

    case GUILD_DEPUTE_POWER:  //副团授权
		DeputeGuildMasterOfPower(pBuf + index);
		break;

	case GUILD_STOP_POWER:  //终止授权
		RemoveGuildMasterOfPower(pBuf + index);
		break;

	case GUILD_CALL_CHAT: //yskang 0.2 (35) 器飘府胶 辨付啊 蜡历家券
		GuildUserCallReq(pBuf + index);
		break;
	case GUILD_USER_INFO_CALL://yskang 0.2
		m_nGuildUserInfoType =0x03;
		GuildUserInfoReq(pBuf + index);
		break;
	}	
}

void USER::ChatReqWithThread(int length, TCHAR *pBuf)
{
	int head;
	BYTE *pData;
	CHATDATAPACKET *pCDP;

	pCDP = new CHATDATAPACKET;
	if ( pCDP == NULL )	return;

	pData = new BYTE[length+1];
	if ( pData == NULL )
	{
		delete pCDP;
		return;
	}

	memcpy(pData, pBuf, length);
	pCDP->pData = pData;
	pCDP->pData[length] = '\0';
	pCDP->dcount = length;
	pCDP->code = CHAT_REQ;
	pCDP->UID = m_uid;

	if ( m_iModSid < 0 || m_iModSid > AUTOMATA_THREAD )
	{
		delete pCDP;
		return;
	}

	head = g_nChatDataHead[m_iModSid];

	g_WaitRecvDataChat[head][m_iModSid] = pCDP;
	
	head++;
	head %= DATA_BUF_SIZE_FOR_THREAD;
	g_nChatDataHead[m_iModSid] = head;
}

/////////////////////////////////////////////////////////////////////////
//	夸没茄 PK Count甫 焊辰促. 
//
void USER::SendPKCount()
{
	CString strMsg = _T("");
	strMsg.Format( IDS_USER_PK_COUNT, m_sKillCount);
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);	
}

/////////////////////////////////////////////////////////////////////////
//	傈眉 矫胶袍 皋技瘤肺 朝赴促.
//
void USER::SendAllChat(TCHAR *pBuf)
{
	CBufferEx TempBuf;

	TempBuf.Add(SYSTEM_MSG);			// 款康磊啊 矫胶袍 皋技瘤甫 舅赴促.
//	TempBuf.Add(SYSTEM_NORMAL);
	TempBuf.Add(SYSTEM_ANNOUNCE);
	TempBuf.AddString(pBuf + 1);

	USER* pUser = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUser(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		pUser->Send(TempBuf, TempBuf.GetLength());
	}
	//yskang 0.4
	if( m_tIsOP == 1 )
    {
	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	sprintf(strOP,"GM公告:%s", pBuf);
	WriteOpratorLog(strOP,CHAT_LOG);
	}
}

/////////////////////////////////////////////////////////////////////////
//	刷怪物.
//
void USER::MopCallReq(TCHAR *pBuf)
{
	int index = 0;
	char szNpcName[255];

	if(!GetVarString(sizeof(szNpcName), szNpcName, pBuf, index)) return;

	CNpc* pNpc = GetNpc(szNpcName);
	if(pNpc == NULL)
	{
		SendSystemMsg( IDS_USER_INVALID_MONSTER_NAME, SYSTEM_NORMAL, TO_ME);
		return;
	}

	short x = GetShort(pBuf, index);
	short y = GetShort(pBuf, index);

	CPoint ptCell = ConvertToServer( x, y );
	ptCell = FindNearAvailablePoint_S(ptCell.x, ptCell.y);

	if(ptCell.x != -1 && ptCell.y != -1) 
	{
		GetNpcData(pNpc, ptCell.x, ptCell.y);
		//yskang 0.4
		if( m_tIsOP == 1 )
        {
		TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
		sprintf(strOP,"call_npc:%s,x=%d,y=%d", szNpcName,ptCell.x, ptCell.y);
		WriteOpratorLog(strOP,CHAT_LOG);
	    }
	}
	else SendSystemMsg( IDS_USER_INVALID_POINT_CALL_MONSTER, SYSTEM_NORMAL, TO_ME);
}

/////////////////////////////////////////////////////////////////////////
//	老馆 盲泼 贸府
//
void USER::NormalChat(TCHAR *pBuf)
{
	if(m_dwCloseTime > 0 )
    {
		SendSystemMsg( "监狱中,无法聊天!",SYSTEM_NORMAL,TO_ME);
		return;
	}
	CBufferEx TempBuf;
	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(NORMAL_CHAT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	TempBuf.AddString(pBuf + 1);

	SendScreen(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////
//	庇加富 贸府
//
void USER::WhisperChat(TCHAR *pBuf)
{
	CBufferEx TempMyBuf;
	CBufferEx TempYouBuf;
	BYTE result = FAIL, error_code = 0;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;

	if(m_sKillCount >= 100) 
	{
		SendSystemMsg( IDS_USER_TOO_LOW_CITY_VALUE, SYSTEM_ERROR, TO_ME);
		return;
	}

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // 肋给等 蜡历酒捞叼
	{
		error_code = ERR_6;
		goto result_send;
	}

	pUser = GetUser(szUserName);
	if(!pUser) // 庇富 惑措 绝澜
	{
//		error_code = ERR_3;
//		goto result_send;
		g_pMainDlg->BridgeServerWhisperChatReq( m_uid, m_strUserID, szUserName, pBuf + 1 + nLength + 1 );
		return;
	}

	if(pUser->m_state != STATE_GAMESTARTED)  // 泅犁 霸烙 柳青吝捞 酒丛...
	{
		error_code = ERR_3;
		goto result_send;
	}

	if(pUser->m_bWhisper == FALSE) // 惑措祈捞 庇富 芭何吝
	{
		error_code = ERR_2;
		goto result_send;
	}

	if(pUser->m_tIsOP == 1)					// 款康磊俊霸 庇富篮 救凳
	{
		error_code = ERR_3;
		goto result_send;
	}

	if(m_strUserID == pUser->m_strUserID)	// 磊扁磊脚俊霸 庇富 -> 府畔
	{
		return;
	}

/*	if(pUser->m_sKillCount >= 100) 
	{
		SendSystemMsg( IDS_USER_TOO_LOW_OTHER_CITY_VALUE, SYSTEM_ERROR, TO_ME);
		return;
	}
*/
	result = SUCCESS;

result_send:
	TempMyBuf.Add(CHAT_RESULT);
	TempMyBuf.Add(result);

	if(result != SUCCESS)
	{
		TempMyBuf.Add(error_code);
		Send(TempMyBuf, TempMyBuf.GetLength());
		return;
	}

	TempMyBuf.Add(WHISPER_CHAT);
	TempMyBuf.Add((int)0x00);								// 惑措规 酒捞叼甫 唱俊霸 焊辰促.(磊悼 历厘困秦)
	TempMyBuf.AddString(pUser->m_strUserID);
	TempMyBuf.AddString(pBuf + 1 + nLength + 1);			// pBuf == IDLen(1) + ID + MsgLen(1) + Msg
	Send(TempMyBuf, TempMyBuf.GetLength());
	
	TempYouBuf.Add(CHAT_RESULT);
	TempYouBuf.Add(result);
	TempYouBuf.Add(WHISPER_CHAT);
	TempYouBuf.Add((int)0x01);								// Client俊辑 辑肺 备盒窍扁困秦(惑措规 庇富 酒捞叼)
	TempYouBuf.AddString(m_strUserID);
	TempYouBuf.AddString(pBuf + 1 + nLength + 1);			// pBuf == IDLen(1) + ID + MsgLen(1) + Msg
	pUser->Send(TempYouBuf, TempYouBuf.GetLength());
	//yskang 0.4
	if( m_tIsOP == 1 )
    {
	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	sprintf(strOP,"Whisper_Chat:%s,%s", szUserName, pBuf + 1 + nLength + 1);
	WriteOpratorLog(strOP,CHAT_LOG);
	}
}

//////////////////////////////////////////////////////////////////////////
//	8拳搁 寇魔 贸府
//  呐喊聊天
void USER::ShoutChat(TCHAR *pBuf)
{
	USER* pUser = NULL;
	if(m_sLevel < 100)
	{
		SendSystemMsg( IDS_USER_TOO_LOW_LEVEL_FOR_SHOUT, SYSTEM_NORMAL, TO_ME);
		return;
	}
    
/*	if (GetTickCount() - m_dwLastShoutChatTime <= 6 * 1000)	//小于10秒不能呐喊
	{
		SendSystemMsg( "你说话太快了,歇会吧!请5秒后再呐喊!", SYSTEM_ERROR, TO_ME);
		return;
	}*/
	short sp = GetSP();

	if(sp < 10)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_STAMINA_FOR_SHOUT, SYSTEM_NORMAL, TO_ME);
		return;
	}
    
    if(m_dwCloseTime > 0 )
    {
		SendSystemMsg( "你没有资格呐喊!",SYSTEM_NORMAL,TO_ME);
		return;
	}
	
	if(m_sKillCount >= 100) 
	{
		SendSystemMsg( IDS_USER_TOO_LOW_CITY_VALUE, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(m_dwDN < 10000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}
	if( m_dwDN <= 10000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 10000;
	UpdateUserItemDN();						
	SendMoneyChanged();
	DecSP(10);			// 胶抛固唱 皑家
	CBufferEx TempBuf;
	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(SHOUT_CHAT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	TempBuf.AddString(pBuf + 1);	

	int min_x = m_curx - ( SIGHT_SIZE_X * 2 );
	int max_x = m_curx + ( SIGHT_SIZE_X * 2 );
	int min_y = m_cury - ( SIGHT_SIZE_Y * 2 );
	int max_y = m_cury + ( SIGHT_SIZE_Y * 2 );

	SendSP();
	//SendToRange( TempBuf, TempBuf.GetLength(), min_x, min_y, max_x, max_y );
	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUser(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		pUser->Send(TempBuf, TempBuf.GetLength());
	}
//	m_dwLastShoutChatTime = GetTickCount();
}

//////////////////////////////////////////////////////////////////////////
//	庇富 倾侩/芭何
//
void USER::WhisperOnOff(TCHAR *pBuf)
{
	int index = 0;
	BYTE type = GetByte(pBuf, index);

	if(type == WHISPER_OK) m_bWhisper = TRUE;
	else m_bWhisper = FALSE;

	CBufferEx TempBuf;

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(type);
	
	Send(TempBuf, TempBuf.GetLength());

	g_pMainDlg->BridgeServerUserWhisperOnOff( m_uid, (BYTE)m_bWhisper );
}

///////////////////////////////////////////////////////////////////////////
//	漂沥 蜡历啊 霸烙俊 立加窍绊 乐绰瘤 茫酒夯促.
//
void USER::FindUserReq(TCHAR *pBuf)
{
	CBufferEx TempBuf;
	BYTE result = 1;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // 肋给等 蜡历酒捞叼
	{
		result = ERR_1;
		goto result_send;
	}

	pUser = GetUser(szUserName);
	if(!pUser) // 惑措 绝澜
	{
		result = ERR_2;
		goto result_send;
	}

	result = 0;

result_send:
	TempBuf.Add(FIND_USER_RESULT);
	TempBuf.Add(result);
	TempBuf.AddString(szUserName);

	Send(TempBuf, TempBuf.GetLength());
}


////////////////////////////////////////////////////////////////////////////
//	鞍篮 Buddy 备己盔俊霸 盲泼阑 焊辰促.
//
void USER::BuddyChat(TCHAR *pBuf)
{
	int i;
	if(!m_bNowBuddy) 
	{
		SendSystemMsg( IDS_USER_MAKE_BUDDY_FIRST, SYSTEM_NORMAL, TO_ME);
		return;
	}

	USER *pUser = NULL;

	CBufferEx TempBuf;
	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(BUDDY_CHAT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	TempBuf.AddString(pBuf + 1);

	for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
	{
		if(m_MyBuddy[i].uid	>= USER_BAND && m_MyBuddy[i].uid <= NPC_BAND) 
		{
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(!pUser) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
			if(strcmp(pUser->m_strUserID, m_MyBuddy[i].m_strUserID) != 0) continue;

			pUser->Send(TempBuf, TempBuf.GetLength());
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	Buddy Mode 甫 On/Off 茄促
//
void USER::SetBuddyMode(TCHAR *pBuf)
{
/*	int index = 0;
	BOOL bMode = (BOOL)GetByte(pBuf, index);

	m_bBuddyMode = bMode;

	if(m_bBuddyMode == FALSE)	// 檬扁拳 茄促.
	{
		int iCount = m_MyBuddy.GetSize();
		for(int i = 0; i < m_MyBuddy.GetSize(); i++)
		{
			if(m_MyBuddy[i].Compare(m_strUserID) == 0) { SendBuddyUserLeave(i);	break; }
		}
	}

	CBufferEx TempBuf;

	TempBuf.Add(BUDDY_MODE_RESULT);
	TempBuf.Add((BYTE)m_bBuddyMode);

	Send(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////////
//	巢捞 林绰 酒捞袍甫 铰倡, 芭例甫 搬沥茄促. 
//
void USER::SetItemMode(TCHAR *pBuf)
{
	int index = 0;	

	BYTE tMode = GetByte(pBuf, index);

	if(tMode == 1)			// 酒捞袍 罐扁 芭何
	{
		m_bRefuse = TRUE;
		SendSystemMsg( IDS_USER_ITEM_RECEIVE_DENIED, SYSTEM_NORMAL, TO_ME);
		return;
	}
	else if(tMode == 0)		// 酒捞袍 罐扁 铰倡
	{
		m_bRefuse = FALSE;
		SendSystemMsg( IDS_USER_ITEM_RECEIVE_PERMITTED, SYSTEM_NORMAL, TO_ME);
		return;
	}
	
}

///////////////////////////////////////////////////////////////////////////
//	滚叼, 惑措规 酒捞袍林扁, 芭贰 备己夸家啊 登绰瘤 魄窜(炼扒 : 林函 1伎捞郴, 辑肺 付林航)
//
BOOL USER::IsThereUser(USER *pUser)
{
	if( !pUser ) return FALSE;

	int nLen = 0;
	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return FALSE;

	if( abs( m_tDir - pUser->m_tDir ) != 4 ) return FALSE;

	if( abs( m_curx - pUser->m_curx ) > 2 || abs( m_cury - pUser->m_cury ) > 2 ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//	滚叼 备己夸家啊 惯积且版快 焊辰促.
//
void USER::BuddyUserChange(TCHAR *pBuf, BYTE tType)
{
	int index = 0;
	int iCount = 0;
	int i, diffLevel = 0;
	char szUserName[255];

	USER* pUser = NULL;

//	if(m_bBuddyMode == FALSE) return;

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // 肋给等 蜡历酒捞叼
	{
		return;
	}

	pUser = GetUser(szUserName);
	if(!pUser) // 惑措 绝澜
	{
		CString strTemp = _T("");
		strTemp.Format( IDS_USER_NOT_CONNECTED_NOW, szUserName);
		SendSystemMsg((LPTSTR)(LPCTSTR)strTemp, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(tType == BUDDY_OFF)
	{
//		if(!pUser->m_bBuddyMode || !pUser->m_bNowBuddy) return;
		if(!pUser->m_bNowBuddy) return;
		if(!m_bMakeBuddy && strcmp(m_strUserID, pUser->m_strUserID) != 0)
		{
			SendSystemMsg( IDS_USER_HAVE_NOT_PERMISSION, SYSTEM_NORMAL, TO_ME);
			return;
		}

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(m_MyBuddy[i].uid == pUser->m_uid + USER_BAND) { SendBuddyUserLeave(i);	break; }
		}
		return;
	}
	
	if(tType == BUDDY_INVITE)
	{
		if(/*IsThereUser(pUser) == FALSE ||*/ strcmp(m_strUserID, pUser->m_strUserID) == 0)
		{
			SendSystemMsg( IDS_USER_INVALID_BUDDY_CONDITION, SYSTEM_NORMAL, TO_ME);
			return;
		}

/*		if(pUser->m_bBuddyMode == FALSE) // 惑措祈捞 滚叼 芭何吝
		{
			CString strTemp = _T("");
			strTemp.Format( IDS_USER_REQUEST_DENIED, pUser->m_strUserID);
			SendSystemMsg(strTemp.GetBuffer(strTemp.GetLength()), SYSTEM_NORMAL, TO_ME);
			return;
		}
*/
		
		diffLevel = abs(m_sLevel - pUser->m_sLevel);
		if( diffLevel > 20)					
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_BUDDY_LEVEL, pUser->m_strUserID);
			SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
			return;
		}
		
/*		if(m_strUserID == pUser->m_strUserID && m_bNowBuddy == TRUE)	// 磊扁磊脚俊霸 滚叼 -> 滚叼吝捞搁 呕硼
		{
			iCount = m_MyBuddy.GetSize();
			for(i = 0; i < m_MyBuddy.GetSize(); i++)
			{
				if(m_MyBuddy[i].Compare(m_strUserID) == 0) { SendBuddyUserLeave(i);	break; }
			}
			return;
		}*/
		
        if(pUser->m_bSessionOnline)
			{
			SendSystemMsg( "对方处于离线状态，无法组队", SYSTEM_ERROR, TO_ME);
			return;
		}
			
		if(m_curz == 67 || m_curz == 66)		//PK竞赛无法组队
		{
			SendSystemMsg( "在活动场地,无法组队", SYSTEM_ERROR, TO_ME);
			return;
		}

		if(pUser->m_bNowBuddy == TRUE) // 惑措祈捞 滚叼吝
		{
			CString strTemp = _T("");
			strTemp.Format( IDS_USER_BUDDY_ALREADY, pUser->m_strUserID);
			SendSystemMsg((LPTSTR)(LPCTSTR)strTemp, SYSTEM_NORMAL, TO_ME);
			return;
		}
										// 贸澜栏肺 惯扁牢捞 等促.
		if(!m_bMakeBuddy && !m_bNowBuddy && m_strUserID != pUser->m_strUserID) // 磊扁去磊父 父靛绰巴阑 规瘤窍扁困秦
		{
			CBufferEx TempBuf;

			for(i = 0; i < MAX_BUDDY_USER_NUM; i++)		// 贸澜 父甸锭绰 檬扁拳茄促.
			{
				m_MyBuddy[i].uid = -1;
				::ZeroMemory(m_MyBuddy[i].m_strUserID, sizeof(m_MyBuddy[i].m_strUserID));		
			}

			m_MyBuddy[0].uid = m_uid + USER_BAND; // 磊扁 酒捞叼啊 力老 刚历 客具 茄促.
			strcpy(m_MyBuddy[0].m_strUserID, m_strUserID);
			m_bMakeBuddy = TRUE;
			m_bNowBuddy = TRUE;

			TempBuf.Add(BUDDY_USER_CHANGE);
			TempBuf.Add((BYTE)0x01);
			TempBuf.Add((int)(m_uid + USER_BAND));
			TempBuf.AddString(m_strUserID);
			TempBuf.Add(m_iSkin);
			TempBuf.Add(m_iHair);
			TempBuf.Add((BYTE)m_sGender);
			TempBuf.AddData(m_strFace, 10);
			TempBuf.Add(m_sMagicMaxHP);
			TempBuf.Add(m_sHP);

			Send(TempBuf, TempBuf.GetLength());
		}

		if(m_bMakeBuddy == FALSE)	return;				// 炉捞 酒聪搁 唱啊扼~~

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(m_MyBuddy[i].uid == -1)
			{
				m_MyBuddy[i].uid = pUser->m_uid + USER_BAND;
				strcpy(m_MyBuddy[i].m_strUserID, szUserName);
				SendBuddyUserChange(BUDDY_JOIN, i);
				return;
			}
		}

		SendSystemMsg( IDS_USER_BUDDY_MAX_USER, SYSTEM_NORMAL, TO_ME);
	}
}

///////////////////////////////////////////////////////////////////////////
//
void USER::SendBuddyUserChange(BYTE type, int iCount)
{	
	CBufferEx TempBuf;

	USER *pInviteUser = NULL;
	USER *pUser = NULL;
	int i, j;
	
	if(type == BUDDY_JOIN)									// 炉捞 眠玫窍咯 啊涝	
	{
		pInviteUser = GetUser(m_MyBuddy[iCount].uid - USER_BAND);//.GetBuffer(20));

		if(pInviteUser == NULL) return;
		if(pInviteUser->m_state != STATE_GAMESTARTED) return;

		TempBuf.Add(BUDDY_USER_CHANGE);
		TempBuf.Add((BYTE)0x01);							// 啊涝
		TempBuf.Add((int)(pInviteUser->m_uid + USER_BAND));
		TempBuf.AddString(pInviteUser->m_strUserID);
		TempBuf.Add(pInviteUser->m_iSkin);					// 倔奔
		TempBuf.Add(pInviteUser->m_iHair);
		TempBuf.Add((BYTE)pInviteUser->m_sGender);
		TempBuf.AddData(pInviteUser->m_strFace, 10);
		TempBuf.Add(pInviteUser->m_sMagicMaxHP);					// 惑怕 
		TempBuf.Add(pInviteUser->m_sHP);

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				// 檬没等 蜡历狼 沥焊甫 唱赣瘤 蜡历俊霸
		{
			if(m_MyBuddy[i].uid == -1) continue;

			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);	// 府胶飘惑狼 蜡历甫 茫绰促.	
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
															// 炉篮 歹 眠啊且 鞘夸啊 绝促.
			if(i != 0 && i != iCount)					
			{
				for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
				{
					if(pUser->m_MyBuddy[j].uid == -1)
					{
						pUser->m_MyBuddy[j].uid = pInviteUser->m_uid + USER_BAND;
						strcpy(pUser->m_MyBuddy[j].m_strUserID, pInviteUser->m_strUserID);
						break;
					}
				}
			}
			if(i != iCount) pUser->Send(TempBuf, TempBuf.GetLength());
		}

		pInviteUser->m_bNowBuddy = TRUE;					// 啊涝 沁促绰 钎矫甫 悸泼

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				// 唱俊霸 扁粮 滚叼 蜡历 沥焊 葛滴甫
		{
			CBufferEx TempBuf2;
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;

			pInviteUser->m_MyBuddy[i].uid = m_MyBuddy[i].uid;
			strcpy(pInviteUser->m_MyBuddy[i].m_strUserID, m_MyBuddy[i].m_strUserID);

			TempBuf2.Add(BUDDY_USER_CHANGE);
			TempBuf2.Add((BYTE)0x01);
			TempBuf2.Add((int)(pUser->m_uid + USER_BAND));
			TempBuf2.AddString(pUser->m_strUserID);
			TempBuf2.Add(pUser->m_iSkin);
			TempBuf2.Add(pUser->m_iHair);
			TempBuf2.Add((BYTE)pUser->m_sGender);
			TempBuf2.AddData(pUser->m_strFace, 10);
			TempBuf2.Add((short)pUser->m_sMagicMaxHP);
			TempBuf2.Add((short)pUser->m_sHP);
			
			pInviteUser->Send(TempBuf2, TempBuf2.GetLength());
		}
	}
	else if(type == BUDDY_CHANGE)							// 惑怕 荐摹啊 函悼捞 乐阑锭
	{
		TempBuf.Add(BUDDY_USER_CHANGE);
		TempBuf.Add((BYTE)0x03);
		TempBuf.Add((int)(m_uid + USER_BAND));
		TempBuf.Add((short)m_sMagicMaxHP);
		TempBuf.Add((short)m_sHP);

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
			pUser->Send(TempBuf, TempBuf.GetLength());
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//	滚叼 备己盔 呕硼唱 碍硼老锭 阿 备己盔俊霸 焊辰促.
//
void USER::SendBuddyUserLeave(int iCount)//(/@@@ 咯扁 何盒篮 唱吝俊 弥利拳 窍磊)
{
	int i, j;
	int iNum = 0;
	USER *pUser = NULL;
	USER *pLeaveUser = NULL;

	CBufferEx TempBuf;	// 备己盔吝俊 唱哎锭

		BOOL bPan ;
	int iTemp;
	TCHAR cTemp[31];
	/////
go_head:
	bPan =1;


	pLeaveUser = GetUser(m_MyBuddy[iCount].uid - USER_BAND);

	if(pLeaveUser == NULL) goto go_result;
															// 炉捞 唱哎锭...	
	if(m_bMakeBuddy && strcmp(m_strUserID, pLeaveUser->m_strUserID) == 0)// 炉捞 唱哎锭
	{
		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				// 府胶飘俊 殿废等 蜡历绰 唱埃促. 
		{
			CBufferEx TempBuf;
			if(m_MyBuddy[i].uid == -1) continue;
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);

			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;

		//	TempBuf.Add(BUDDY_USER_CHANGE);
		//	TempBuf.Add((BYTE)0x02);
		//	TempBuf.Add((int)(pUser->m_uid + USER_BAND));
		//	pUser->Send(TempBuf, TempBuf.GetLength());

			if(m_uid != pUser->m_uid)						// 磊扁 磊脚俊霸绰 焊尘 鞘夸啊 绝促.
			{
								if (bPan)
				{
					iCount = i;
					iTemp = m_MyBuddy[i].uid; 
			        _tcscpy(cTemp,m_MyBuddy[i].m_strUserID );
					m_MyBuddy[i].uid = m_MyBuddy[0].uid;
					_tcscpy(m_MyBuddy[i].m_strUserID,m_MyBuddy[0].m_strUserID);
					m_MyBuddy[0].uid =  iTemp ;
					_tcscpy(m_MyBuddy[0].m_strUserID,cTemp );
					m_bMakeBuddy = 0;
					pUser->m_bMakeBuddy = 1;
					bPan = 0;
				}
				//pUser->m_bNowBuddy = FALSE;
				for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
				{
					if (pUser->m_MyBuddy[j].uid == iTemp )
					{
						pUser->m_MyBuddy[j].uid = pUser->m_MyBuddy[0].uid;
						_tcscpy(pUser->m_MyBuddy[j].m_strUserID,pUser->m_MyBuddy[0].m_strUserID );
						pUser->m_MyBuddy[0].uid =  iTemp ;
					    _tcscpy(pUser->m_MyBuddy[0].m_strUserID,cTemp );

						break;
					}


				///	pUser->m_MyBuddy[j].uid = -1;			// 친절히 상대방을 초기화 한다.
				//	::ZeroMemory(pUser->m_MyBuddy[j].m_strUserID, sizeof(pUser->m_MyBuddy[j].m_strUserID));
				}
			/*TempBuf.Add(BUDDY_USER_CHANGE);
			TempBuf.Add((BYTE)0x02);
			TempBuf.Add((int)(pUser->m_uid + USER_BAND));
			pUser->Send(TempBuf, TempBuf.GetLength());*/
				pUser->SendBuddyUserChange(BUDDY_CHANGE);
			}
		}
		goto go_head;

	}

	else if(m_bMakeBuddy)									// 碍硼牢 版快
	{		
		TempBuf.Add(BUDDY_USER_CHANGE);
		TempBuf.Add((BYTE)0x02);
		TempBuf.Add((int)(pLeaveUser->m_uid + USER_BAND));

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				// 昏力茄促.		
		{
			if(m_MyBuddy[i].uid == -1) continue;
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
			pUser->Send(TempBuf, TempBuf.GetLength());

			for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
			{
				if(pUser->m_MyBuddy[j].uid == pLeaveUser->m_uid + USER_BAND) 
				{
					pUser->m_MyBuddy[j].uid = -1;	
					::ZeroMemory(pUser->m_MyBuddy[j].m_strUserID, sizeof(pUser->m_MyBuddy[j].m_strUserID));
				}
			}
		}

		iNum = 0;
		pLeaveUser->m_bNowBuddy = FALSE;
		pLeaveUser->m_bMakeBuddy = FALSE;
		for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
		{
			if(m_MyBuddy[j].uid != -1) iNum++;
			pLeaveUser->m_MyBuddy[j].uid = -1;		// 炉篮 唱吝俊 昏力...	
			::ZeroMemory(pLeaveUser->m_MyBuddy[j].m_strUserID, sizeof(pLeaveUser->m_MyBuddy[j].m_strUserID));
		}

		if(iNum <= 1)						// 炉 去磊老锭绰 秦魂 
		{
			CBufferEx TempBuf2;
			TempBuf2.Add(BUDDY_USER_CHANGE);
			TempBuf2.Add((BYTE)0x02);
			TempBuf2.Add((int)(m_uid + USER_BAND));
			Send(TempBuf2, TempBuf2.GetLength());
			m_bNowBuddy = FALSE;
			m_bMakeBuddy = FALSE;
			for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
			{
				m_MyBuddy[j].uid = -1;		// 炉篮 唱吝俊 昏力...	
				::ZeroMemory(m_MyBuddy[j].m_strUserID, sizeof(m_MyBuddy[j].m_strUserID));
			}
		}
		return;	
	}
	else
	{
		TempBuf.Add(BUDDY_USER_CHANGE);
		TempBuf.Add((BYTE)0x02);
		TempBuf.Add((int)(pLeaveUser->m_uid + USER_BAND));

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				
		{
			if(m_MyBuddy[i].uid == -1) continue;
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;

			pUser->Send(TempBuf, TempBuf.GetLength());

			if(pUser->m_uid != pLeaveUser->m_uid)			// 唱啊绰单 磊扁俊霸 焊尘 鞘夸 绝促.
			{				
				for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
				{
					if(pUser->m_MyBuddy[j].uid == pLeaveUser->m_uid + USER_BAND) 
					{
						pUser->m_MyBuddy[j].uid = -1;	
						::ZeroMemory(pUser->m_MyBuddy[j].m_strUserID, sizeof(pUser->m_MyBuddy[j].m_strUserID));
						break;
					}
				}
			}
		}

		pUser = GetUser(m_MyBuddy[0].uid - USER_BAND);
		if(pUser != NULL)
		{
			iNum = 0;
			for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
			{
				if(pUser->m_MyBuddy[j].uid != -1) iNum++;
			}

			if(iNum <= 1)						// 炉 去磊老锭绰 秦魂 
			{
				CBufferEx TempBuf2;
				TempBuf2.Add(BUDDY_USER_CHANGE);
				TempBuf2.Add((BYTE)0x02);
				TempBuf2.Add((int)(pUser->m_uid + USER_BAND));
				pUser->Send(TempBuf2, TempBuf2.GetLength());
				pUser->m_bNowBuddy = FALSE;
				pUser->m_bMakeBuddy = FALSE;
				for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
				{
					m_MyBuddy[j].uid = -1;		// 炉篮 唱吝俊 昏力...	
					::ZeroMemory(m_MyBuddy[j].m_strUserID, sizeof(m_MyBuddy[j].m_strUserID));
				}
			}
		}
	}

go_result:
	m_bMakeBuddy = FALSE;	
	m_bNowBuddy = FALSE;	
	for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
	{
		m_MyBuddy[j].uid = -1;		// 炉篮 唱吝俊 昏力...	
		::ZeroMemory(m_MyBuddy[j].m_strUserID, sizeof(m_MyBuddy[j].m_strUserID));
	}
}

///////////////////////////////////////////////////////////////////////////
//	BBS 贸府风凭俊辑 俊矾啊 朝 版快 俊矾傈价
//
void USER::SendBBSError(BYTE tError)
{
	CBufferEx TempBuf;

	TempBuf.Add(BBS_ERROR);
	TempBuf.Add(tError);

	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////
//	秦寸 BBS甫 Open 茄促.
//
void USER::BBSOpen(int bbsnum)
{
	CBufferEx TempBuf;
	int index = 0;
	int rowcount = 0;
	int i = 0;

	BOOL bQuerySuccess = FALSE;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_READ_INDEX ( %d, -1 )}" ), bbsnum);

	SQLCHAR		strWriter[CHAR_NAME_LENGTH+1];		::ZeroMemory(strWriter, sizeof(strWriter));
	SQLCHAR		strTitle[50];		::ZeroMemory(strTitle, sizeof(strTitle));
	SQLCHAR		strTime[30];		::ZeroMemory(strTime, sizeof(strTime));

	SQLINTEGER	iIndex, iView;
	SQLINTEGER	iInd;

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To BBS Open(BBS:%d) !!\n", bbsnum);
//		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strWriter,	sizeof(strWriter),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iView,		sizeof(SQLINTEGER),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strTitle,	sizeof(strTitle),	&iInd );

				TempBuf.Add((int)(iIndex));
				TempBuf.AddString((LPTSTR)strWriter);
				TempBuf.AddString((LPTSTR)strTime);
				TempBuf.Add((int)iView);
				TempBuf.AddString((LPTSTR)strTitle);
				
				::ZeroMemory(strWriter, sizeof(strWriter));
				::ZeroMemory(strTitle, sizeof(strTitle));
				::ZeroMemory(strTime, sizeof(strTime));

				rowcount++;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else break;
		}
	}
	else if (retcode == SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}
	
	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess) return;

	index = 0;
	SetByte(m_TempBuf, BBS_OPEN, index);
	SetShort(m_TempBuf, bbsnum, index );
	SetShort(m_TempBuf, rowcount, index );
	SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), index);
	Send(m_TempBuf, index);
}

//////////////////////////////////////////////////////////////////////////////
//	秦寸 BBS俊辑 促澜 霸矫拱 沥焊甫 掘绰促.
//
void USER::BBSNext(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	CBufferEx TempBuf;
	int index = 0;
	int i = 1;

	int	nBBS = GetShort(pBuf, index);
	int	nStart = GetInt(pBuf, index);
	
	int rowcount = 0;
	BOOL bQuerySuccess = FALSE;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_READ_INDEX ( %d, %d )}" ), nBBS, nStart);

	SQLCHAR		strWriter[CHAR_NAME_LENGTH+1];		::ZeroMemory(strWriter, sizeof(strWriter));
	SQLCHAR		strTitle[50];		::ZeroMemory(strTitle, sizeof(strTitle));
	SQLCHAR		strTime[30];		::ZeroMemory(strTime, sizeof(strTime));

	SQLINTEGER	iIndex, iView;
	SQLINTEGER	iInd;

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To BBS Next(BBS = %d, Start = %d) !!\n", nBBS, nStart);

//		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strWriter,	sizeof(strWriter),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iView,		sizeof(SQLINTEGER),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strTitle,	sizeof(strTitle),	&iInd );

				TempBuf.Add((int)(iIndex));
				TempBuf.AddString((LPTSTR)strWriter);
				TempBuf.AddString((LPTSTR)strTime);
				TempBuf.Add((int)iView);
				TempBuf.AddString((LPTSTR)strTitle);
				
				::ZeroMemory(strWriter, sizeof(strWriter));
				::ZeroMemory(strTitle, sizeof(strTitle));
				::ZeroMemory(strTime, sizeof(strTime));

				rowcount++;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else
				break;
		}
	}
	else if (retcode == SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);	

	if( !bQuerySuccess) return;

	index = 0;
	SetByte(m_TempBuf, BBS_NEXT_RESULT, index);
	SetShort(m_TempBuf, nBBS, index );
	SetShort(m_TempBuf, rowcount, index );
	SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), index);
	Send(m_TempBuf, index);
}

void USER::BBSNextWithThread(TCHAR *pBuf)
{
	int index = 0;

	int	nBBS = GetShort(pBuf, index);
	int	nStart = GetInt(pBuf, index);

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = BBS_NEXT;
	Datalength = index;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, 0, Datalength+1);
	memcpy(pData, pBuf, Datalength);
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );	
}


//////////////////////////////////////////////////////////////////////////////
//	霸矫拱狼 郴侩阑 佬绰促.
//
void USER::BBSRead(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	CBufferEx TempBuf;
	int index = 0;
	int i = 1;

	int bbsnum = GetShort( pBuf, index );
	int readindex = GetInt( pBuf, index );

	BOOL bQuerySuccess = FALSE;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];		memset(szSQL, 0x00, sizeof(szSQL));
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_READ_CONTENT ( %d, %d )}" ), bbsnum, readindex );

	SQLCHAR		strWriter[CHAR_NAME_LENGTH+1];		memset( strWriter,	NULL, sizeof(strWriter));
	SQLCHAR		strTitle[51];		memset( strTitle,	NULL, sizeof(strTitle));
	SQLCHAR		strTime[31];		memset( strTime,	NULL, sizeof(strTime));
	SQLCHAR		strContent[5001];	memset( strContent, NULL, sizeof(strContent));

	SQLINTEGER	iIndex;
	SQLINTEGER	iInd;

//	SQLSMALLINT	sRet = -1;
//	SQLINTEGER	iRetInd;

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Read BBS (BBS:%d,Index:%d) !!\n", bbsnum, readindex);
		return;
	}

	/*
	retcode = SQLBindParameter(hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &iRetInd);
	if (retcode!=SQL_SUCCESS) 
	{
		TRACE("USER::BBSRead(), Line = %d, File = %s\n", __LINE__, __FILE__);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}
	*/

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strWriter,	sizeof(strWriter),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strTitle,	sizeof(strTitle),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strContent,	sizeof(strContent),	&iInd );

				TempBuf.Add((int)(iIndex));
				TempBuf.AddString((LPTSTR)strWriter);
				TempBuf.AddString((LPTSTR)strTitle);
				TempBuf.AddString((LPTSTR)strTime);
				TempBuf.AddLongString((LPTSTR)strContent, _tcslen((LPCTSTR)strContent));

				memset( strWriter,	NULL, sizeof(strWriter));
				memset( strTitle,	NULL, sizeof(strTitle));
				memset( strTime,	NULL, sizeof(strTime));
				memset( strContent, NULL, sizeof(strContent));

				break;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else break;
		}
	}
	else
	{				
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess )
	{
		if(g_bDebug) SendSystemMsg( IDS_USER_BBS_DELETED_ALREADY, SYSTEM_NORMAL, TO_ME);
		SendBBSError(BBS_CONTENTS_DELETE);
		return;
	}
	
	if( !bQuerySuccess ) return;

	index = 0;
	SetByte(m_TempBuf, BBS_READ_RESULT, index);
	SetShort(m_TempBuf, bbsnum, index );
	SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), index);
	Send(m_TempBuf, index);
}

void USER::BBSReadWithThread(TCHAR *pBuf)
{
	int index = 0;

	int bbsnum = GetShort( pBuf, index );
	int readindex = GetInt( pBuf, index );

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = BBS_READ;
	Datalength = index;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, 0, Datalength+1);
	memcpy(pData, pBuf, Datalength);
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );

}
//////////////////////////////////////////////////////////////////////////////
//	货肺款 霸矫拱阑 累己茄促.
//
void USER::BBSWrite(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	int i = 1;
//	CString strText = _T("");
	char strTitle[51];		::ZeroMemory(strTitle,		sizeof(strTitle));
	char strContent[5001];	::ZeroMemory(strContent,	sizeof(strContent));

	int bbsnum = GetShort(pBuf, index);

	if(m_tIsOP != 1 && ( bbsnum == 1 || bbsnum == 3 ) )		// 傍瘤霸矫魄 or 肺凯烦喉 霸矫魄
	{
		SendSystemMsg(IDS_USER_NEED_OPERATOR, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(!GetVarString(sizeof(strTitle), strTitle, pBuf, index)) return;			// 力格篮 50官捞飘 捞郴肺...	
	if(!GetVarLongString(sizeof(strContent), strContent, pBuf, index)) return;	// 郴侩篮 5000官捞飘 捞郴肺...
	
	SDWORD sTitleLen	= _tcslen(strTitle);
	SDWORD sContentLen	= _tcslen(strContent);
	SDWORD sIDLen		= _tcslen(m_strUserID);

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

//	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_WRITE ( %d, %s, ?, ? )}" ), bbsnum, m_strUserID);
	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_WRITE ( %d, ?, ?, ? )}" ), bbsnum );

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Write BBS (BBS:%d,Writer:%s,Title:%d) !!\n", bbsnum, m_strUserID, strTitle);

//		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	i = 1;
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)m_strUserID,	0, &sIDLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 50,		0, (TCHAR*)strTitle,	0, &sTitleLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 5000,	0, (TCHAR*)strContent,	0, &sContentLen );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else if (retcode == SQL_ERROR)
	{
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}
	
	SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	BBSOpen( bbsnum );
}

void USER::BBSWriteWithThread(TCHAR *pBuf)
{
	int index = 0;
	char strTitle[51];		::ZeroMemory(strTitle,		sizeof(strTitle));
	char strContent[5001];	::ZeroMemory(strContent,	sizeof(strContent));

	int bbsnum = GetShort(pBuf, index);

	if(m_tIsOP != 1 && bbsnum == 1)		// 傍瘤霸矫魄
	{
		SendSystemMsg(IDS_USER_NEED_OPERATOR, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(!GetVarString(sizeof(strTitle), strTitle, pBuf, index)) return;			// 力格篮 50官捞飘 捞郴肺...	
	if(!GetVarLongString(sizeof(strContent), strContent, pBuf, index)) return;	// 郴侩篮 5000官捞飘 捞郴肺...

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = BBS_WRITE;
	Datalength = index;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, 0, Datalength+1);
	memcpy(pData, pBuf, Datalength);
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );
}

//////////////////////////////////////////////////////////////////////////////
//	霸矫拱阑 荐沥茄促.
//
void USER::BBSEdit(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	int i = 1;

	char strTitle[51];		::ZeroMemory(strTitle,		sizeof(strTitle));
	char strContent[5001];	::ZeroMemory(strContent,	sizeof(strContent));

	int bbsnum = GetShort(pBuf, index);
	int editindex = GetInt( pBuf, index );

	if(m_tIsOP != 1 && ( bbsnum == 1 || bbsnum == 3 ) )		// 傍瘤霸矫魄 or 肺凯烦喉 霸矫魄
	{
		SendSystemMsg(IDS_USER_NEED_OPERATOR, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(!GetVarString(sizeof(strTitle), strTitle, pBuf, index)) return;
	if(!GetVarLongString(sizeof(strContent), strContent, pBuf, index)) return;

	SDWORD sTitleLen	= _tcslen(strTitle);
	SDWORD sContentLen	= _tcslen(strContent);
	SDWORD sIDLen		= _tcslen(m_strUserID);

	SQLSMALLINT	sRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_EDIT ( %d, %d, ?, ?, ?, ?)}" ), bbsnum, editindex );

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Edit BBS (BBS:%d,Writer:%s,Index:%d) !!\n", bbsnum, m_strUserID, editindex);

		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	i = 1;
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)m_strUserID,	0, &sIDLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 50,		0, (TCHAR*)strTitle,	0, &sTitleLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 5000,	0, (TCHAR*)strContent,	0, &sContentLen );
	SQLBindParameter( hstmt, i++ ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT,	0,		0, &sRet,				0, &iRetInd);

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if(sRet == 2)
	{
		if(g_bDebug) SendSystemMsg( IDS_USER_BBS_NO_PERMISSION, SYSTEM_NORMAL, TO_ME);
		SendBBSError(BBS_CONTENTS_DELETE);
		return;
	}
	if(sRet == 3)
	{
		if(g_bDebug) SendSystemMsg( IDS_USER_BBS_DELETED_ALREADY, SYSTEM_NORMAL, TO_ME);
		SendBBSError(BBS_CONTENTS_DELETE);
		return;
	}

	BBSOpen( bbsnum );
}

void USER::BBSEditWithThread(TCHAR *pBuf)
{
	int index = 0;

	char strTitle[51];		::ZeroMemory(strTitle,		sizeof(strTitle));
	char strContent[5001];	::ZeroMemory(strContent,	sizeof(strContent));

	int bbsnum = GetShort(pBuf, index);
	int editindex = GetInt( pBuf, index );

	if(m_tIsOP != 1 && ( bbsnum == 1 || bbsnum == 3 ) )		// 傍瘤霸矫魄 or 肺凯烦喉 霸矫魄
	{
		SendSystemMsg( IDS_USER_NEED_OPERATOR, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(!GetVarString(sizeof(strTitle), strTitle, pBuf, index)) return;
	if(!GetVarLongString(sizeof(strContent), strContent, pBuf, index)) return;

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = BBS_EDIT;
	Datalength = index;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, 0, Datalength+1);
	memcpy(pData, pBuf, Datalength);
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );
}

/////////////////////////////////////////////////////////
// MAIL request process fucntion
// zinee 02-11-18

void USER::MailInfoReq( TCHAR* pBuf )
{
	if ( pBuf == NULL ) return;

	TCHAR	TempBuf[10000];
	int		nTempBufPos;
	int		nBufferPos;
	BYTE	error;

	nTempBufPos = 0;
	error = MailGetInfo( TempBuf, nTempBufPos );
//	if( error == 0 ) error = MailGetList( TempBuf, -1, 20, nTempBufPos );

	nBufferPos = 0;
	SetByte(m_TempBuf, MAIL_INFO_RESULT, nBufferPos);
//	SetByte(m_TempBuf, error, nBufferPos );
	SetString(m_TempBuf, TempBuf, nTempBufPos, nBufferPos );
	Send(m_TempBuf, nBufferPos );
}


void USER::MailSendReq( TCHAR* pBuf )
{
	/*if ( pBuf == NULL ) return;

//	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
//	SQLRETURN	retcode;

	USER*	pRecvUser;

	short tSlot;

	int nBufferPos;
//	int	nDBIndex;

	BYTE error;

	SYSTEMTIME	st;
	ItemList	StampItem;

	QUEUE		Queue;
	CBufferEx	TempBuf, QTempBuf;

	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[15];
	BOOL		bRead;
	char		strMessage[1001];

	SQLINTEGER		nUserID = CHAR_NAME_LENGTH+1;		
	SQLINTEGER		nSendID = CHAR_NAME_LENGTH+1;
	SQLINTEGER		nTime = 15;
	SQLINTEGER		nRead = sizeof(bRead);
	SQLINTEGER		nMessage = 1001;

//	SQLSMALLINT		SQLType;
//	SQLSMALLINT		SQL_C_Type;
//	SQLUINTEGER		SQLColLength;
//	SQLINTEGER		SQLDataLength;

	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strMessage, sizeof(strMessage));
	::ZeroMemory(&Queue, sizeof(QUEUE) );

	nBufferPos	= 1;
	// Get (strUserID, strMessage) from buffer.
	if(!GetVarString(sizeof(strUserID), (char*)strUserID, pBuf, nBufferPos)) return;
	if(!GetVarLongString(sizeof(strMessage), strMessage, pBuf, nBufferPos)) return;

	// 罐绰 荤恩 酒捞叼啊 粮犁窍瘤 臼阑版快
	if( IsExistCharId( (char*)strUserID ) == FALSE )
	{
		error = 2;
		nBufferPos = 0;
		SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
		SetByte(m_TempBuf, error, nBufferPos );
		Send(m_TempBuf, nBufferPos);
		return;
	}

	::ZeroMemory( &StampItem, sizeof(ItemList) );
	StampItem.sSid = NORMAL_ITEM_STAMP;
	tSlot = (short)GetSameItem(StampItem, INVENTORY_SLOT);

	//蜡历 牢亥俊 酒捞袍捞 绝芭唱 郴备仿捞 0老 版快
	if( tSlot == -1 )

	{
		error = 1;
		nBufferPos = 0;
		SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
		SetByte(m_TempBuf, error, nBufferPos );
		Send(m_TempBuf, nBufferPos);
		return;
	}
	else if( m_UserItem[tSlot].sCount <= 0 )
	{
		error = 1;
		nBufferPos = 0;
		SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
		SetByte(m_TempBuf, error, nBufferPos );
		Send(m_TempBuf, nBufferPos);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////

	// strSendID, strTime, bRead
	strcpy( (char*)strSendID, m_strUserID );
	GetLocalTime(&st);
	_sntprintf((char*)strTime, sizeof(strTime), TEXT( "%02d/%02d/%02d %02d:%02d" ), 
				st.wYear%100, st.wMonth, st.wDay, st.wHour, st.wMinute );
	bRead = FALSE;

	QTempBuf.Add( (int)0 );
	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_INSERT ( \'%s\',\'%s\',\'%s\',%d,\'%s\' )}" ),strUserID, strSendID, strTime, bRead, strMessage);
	QTempBuf.AddData( szSQL, strlen( szSQL )+1 );


	if( g_MailAgentQueue.PutQueue( (QUEUE*)((LPTSTR)QTempBuf) ) == TRUE )
		error = 0;
	else error = 3;  // 皋老矫胶袍 苞何窍老 版快
	//////////////////////////////////////////////////////////////////////////////


	

	nBufferPos = 0;
	SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
	SetByte(m_TempBuf, error, nBufferPos );
	if( error == 0 )
	{
		// 快钎狼 公霸甫 窍唱 皑家矫挪促.
		m_UserItem[tSlot].sCount -= 1;
		if(m_UserItem[tSlot].sCount <= 0) m_UserItem[tSlot].sSid = -1;

		// 泅犁 公霸俊辑 快钎 茄俺 公霸甫 皑家矫挪促.
		m_iCurWeight -= g_arItemTable[NORMAL_ITEM_STAMP]->m_byWeight;
		if( m_iCurWeight < 0 ) m_iCurWeight = 0;

		SetShort( m_TempBuf, tSlot, nBufferPos );
		SetShort( m_TempBuf, m_UserItem[tSlot].sCount, nBufferPos );;
	}

	Send(m_TempBuf, nBufferPos);

	// 惑措规 蜡历啊 霸烙吝捞搁 货皋老捞 档馒沁促绊 舅妨淋
	if( error == 0 )
	{
		pRecvUser = GetUser((TCHAR*)strUserID);
		if( pRecvUser != NULL )
		{
			if(pRecvUser->m_state == STATE_GAMESTARTED 
				&& pRecvUser->m_tIsOP == 0)
			{
				nBufferPos = 0;
				SetByte(m_TempBuf, MAIL_INFO_RESULT, nBufferPos);
				pRecvUser->MailGetInfo( m_TempBuf, nBufferPos );
				pRecvUser->Send(m_TempBuf, nBufferPos );
			}
		}
	}*/
    ///////////////////////////////////////////////////////
if ( pBuf == NULL ) return;


	SQLHSTMT	hstmt = NULL;
	//SQLRETURN	retcode;
	TCHAR		szSQL[1024];	

	USER*	pRecvUser;

	short tSlot;

	int nBufferPos;

	BYTE error;

	SYSTEMTIME	st;
	ItemList    StampItem;

	QUEUE		Queue;
	CBufferEx	TempBuf, QTempBuf;
	USER            *pUser = NULL;
	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[10];
	BOOL		bRead;
	char		strMessage[1001];

	SQLINTEGER		nUserID = CHAR_NAME_LENGTH+1;		
	SQLINTEGER		nSendID = CHAR_NAME_LENGTH+1;
	SQLINTEGER		nTime = 10;
	SQLINTEGER		nRead = sizeof(bRead);
	SQLINTEGER		nMessage = 1001;


	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strMessage, sizeof(strMessage));
	::ZeroMemory(&Queue, sizeof(QUEUE) );

	nBufferPos	= 1;

	if(!GetVarString(sizeof(strUserID), (char*)strUserID, pBuf, nBufferPos)) return;
	if(!GetVarLongString(sizeof(strMessage), strMessage, pBuf, nBufferPos)) return;	
	int nLength = strlen( strMessage );

	if(nLength <= 0 || nLength > 50 - 14) 
	{
		SendSystemMsg("全服信息请控制在15字以内",SYSTEM_ERROR,TO_ME);
		return;
	}	  
     //	strcat((char*)strUserID ,"发送全服邮件：" );
	if (strcmp((char*)strUserID,"全服"))
	{
		if( IsExistCharId( (char*)strUserID ) == FALSE )
		{
			error = 2;
			nBufferPos = 0;
			SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
			SetByte(m_TempBuf, error, nBufferPos );
			Send(m_TempBuf, nBufferPos);
			return;
		} 
	}
	::ZeroMemory( &StampItem, sizeof(ItemList) );
	StampItem.sSid = NORMAL_ITEM_STAMP;
	tSlot = (short)GetSameItem(StampItem, INVENTORY_SLOT);

	if( tSlot == -1 )

	{
		error = 1;
		nBufferPos = 0;
		SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
		SetByte(m_TempBuf, error, nBufferPos );
		Send(m_TempBuf, nBufferPos);
		return;
	}
	else if( m_UserItem[tSlot].sCount <= 0 )
	{
		error = 1;
		nBufferPos = 0;
		SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
		SetByte(m_TempBuf, error, nBufferPos );
		Send(m_TempBuf, nBufferPos);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////

	// strSendID, strTime, bRead
	strcpy( (char*)strSendID, m_strUserID );
	GetLocalTime(&st);
	_sntprintf((char*)strTime, sizeof(strTime), TEXT( "%02d/%02d/%02d %02d:%02d" ), 
		st.wYear%100, st.wMonth, st.wDay, st.wHour, st.wMinute);
	bRead = 0;
//	SendSystemMsg("1",SYSTEM_NORMAL,TO_ME);
	QTempBuf.Add( (int)0 );
	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_INSERT ( \'%s\',\'%s\',\'%s\',%d,\'%s\' )}" ),strUserID, strSendID, strTime, bRead, strMessage);
	QTempBuf.AddData( szSQL, strlen( szSQL )+1 );
	if( g_MailAgentQueue.PutQueue( (QUEUE*)((LPTSTR)QTempBuf) ) == TRUE )
		error = 0;
	else error = 3;  // 皋老矫胶袍 苞何窍老 版快
	//////////////////////////////////////////////////////////////////////////////

	
	nBufferPos = 0;
	SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
	SetByte(m_TempBuf, error, nBufferPos );
	if( error == 0 )
	{
		m_UserItem[tSlot].sCount -= 1;
		if(m_UserItem[tSlot].sCount <= 0) m_UserItem[tSlot].sSid = -1;
		m_iCurWeight -= g_arItemTable[NORMAL_ITEM_STAMP]->m_byWeight;
		if( m_iCurWeight < 0 ) m_iCurWeight = 0;
		SetShort( m_TempBuf, tSlot, nBufferPos );
		SetShort( m_TempBuf, m_UserItem[tSlot].sCount, nBufferPos );;
	}
	Send(m_TempBuf, nBufferPos);
	if( error == 0)
	{
		if (strcmp((char*)strUserID,"全服"))//字符比较函数
		{
			pRecvUser = GetUser((TCHAR*)strUserID);
			if( pRecvUser != NULL )
			{
				if(pRecvUser->m_state == STATE_GAMESTARTED && pRecvUser->m_tIsOP == 0)
				{
					nBufferPos = 0;
					SetByte(m_TempBuf, MAIL_INFO_RESULT, nBufferPos);
					pRecvUser->MailGetInfo( m_TempBuf, nBufferPos );
					pRecvUser->Send(m_TempBuf, nBufferPos );
					//	pRecvUser->m_mailrecv = TRUE;
				}
			}
		} 
		else
		{			
			for(int i = 0; i < MAX_USER; i++)
			{	
				pUser = m_pCom->GetUserUid(i);
				if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
				nBufferPos = 0;
				SetByte(m_TempBuf, MAIL_INFO_RESULT, nBufferPos);
				pUser->MailGetInfo( m_TempBuf, nBufferPos );
				pUser->Send(m_TempBuf, nBufferPos );		
			}	

		}
	}
}

void USER::MailListReq( TCHAR* pBuf )
{
	if ( pBuf == NULL ) return;

	int nReadPos;
	int nStartIndex;//, nListCount;

	nReadPos	= 0;
	nStartIndex = GetInt(pBuf, nReadPos);


	nReadPos = 0;
	SetByte(m_TempBuf, MAIL_LIST_RESULT, nReadPos);
    MailGetList( m_TempBuf, nStartIndex, 20, nReadPos );//只显示20条
	//MailGetList( m_TempBuf, nStartIndex, 8, nReadPos );
//	SetShort(m_TempBuf, nListCount, nReadPos );
//	SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), nReadPos);
	Send(m_TempBuf, nReadPos);
}

int USER::MailGetList( char* pBuf, int nStartIndex, int nCount, int& index )
{
	/*if ( pBuf == NULL ) return -1;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	int i;
	int nListCount;
	int	nDBIndex;

	SQLINTEGER	nSQLIndex;

	SQLINTEGER  nUserIDSize;

	BOOL	bQuerySuccess;

	CBufferEx	TempBuf;

	SQLINTEGER	iIndex;
	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[15];
	BOOL		bRead;


	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));

	nDBIndex	= 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return -1;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return -1;
	}

	if( nStartIndex > 1 ) nStartIndex--;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_LIST ( %d, ? )}" ), nStartIndex );

	nUserIDSize = _tcslen(m_strUserID);

	i=1;
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30,	0, (TCHAR*)m_strUserID,	0, &nUserIDSize );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	nListCount	= 0;

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(nListCount < nCount)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strUserID,	sizeof(strUserID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strSendID,	sizeof(strSendID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_SHORT,	&bRead,		sizeof(SQL_C_TINYINT),	&nSQLIndex );

				TempBuf.Add((SQLINTEGER)(iIndex));
//				TempBuf.AddString((LPTSTR)strUserID);
				TempBuf.AddString((LPTSTR)strSendID);
				TempBuf.AddString((LPTSTR)(strTime));
				TempBuf.Add((BYTE)bRead);//加个(BYTE)解决邮件无法删除无法查看问题
				
				::ZeroMemory(strUserID, sizeof(strUserID));
				::ZeroMemory(strSendID, sizeof(strSendID));
				::ZeroMemory(strTime, sizeof(strTime));

				nListCount++;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else 
			{
				bQuerySuccess = FALSE;
				break;
			}
		}
	}
	else if (retcode == SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return -1;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);	

	if( !bQuerySuccess) return -1;

	SetShort( pBuf, (short)nListCount, index );
	if( nListCount > 0 ) SetString(pBuf, TempBuf, TempBuf.GetLength(), index);

	return nListCount;*/
//////////////////////////////////////////////////////////////////
if ( pBuf == NULL ) return -1;
 //  SendSystemMsg("MailGetList",SYSTEM_NORMAL,TO_ME);
	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	int i;
	int nListCount;
	int	nDBIndex;

	SQLINTEGER	nSQLIndex;

	SQLINTEGER  nUserIDSize;

	BOOL	bQuerySuccess;

	CBufferEx	TempBuf;

	SQLINTEGER	iIndex;
	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[30];
	BOOL		bRead;


	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));

	nDBIndex	= 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return -1;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return -1;
	}

	if( nStartIndex > 1 ) nStartIndex--;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_LIST ( %d, ? )}" ), nStartIndex );

	nUserIDSize = _tcslen(m_strUserID);

	i=1;
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30, 0, (TCHAR*)m_strUserID,0, &nUserIDSize );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	nListCount	= 0;

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(nListCount < nCount)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strUserID,	sizeof(strUserID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strSendID,	sizeof(strSendID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_SHORT,		&bRead,		sizeof(SQL_C_CHAR),	&nSQLIndex );

				TempBuf.Add((SQLINTEGER)(iIndex));

				if (strcmp((char*)strUserID,"全服")== FALSE)
				{
					char info[20];
					sprintf(info, "全服:%s",strTime); //将m_strUserID ,"发送全服邮件",strMessage拼接到info
				  //strcpy((char*)strSendID,"全服邮件" );//字符类型复制函数 。前面是目标变量，中间是复制长度，后面是源变量
					strcpy_s( (char*)strTime,30, info );
				}
				else
				{
					char info[20];
					sprintf(info, "私人:%s",strTime); //将m_strUserID ,"发送全服邮件",strMessage拼接到info
					strcpy_s( (char*)strTime,30, info );
			
				}
				TempBuf.AddString((LPTSTR)strSendID);  
				TempBuf.AddString((LPTSTR)(strTime));
				 
				//TempBuf.AddString((LPTSTR)strUserID);					
				TempBuf.Add((BYTE)bRead);

				::ZeroMemory(strUserID, sizeof(strUserID));
				::ZeroMemory(strSendID, sizeof(strSendID));
				::ZeroMemory(strTime, sizeof(strTime));

				nListCount++;
				
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else 
			{
				bQuerySuccess = FALSE;
				break;
			}
		}
	}
	else if (retcode == SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return -1;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);	

	if( !bQuerySuccess) return -1;

	SetShort( pBuf, (short)nListCount, index );
	if( nListCount > 0 ) SetString(pBuf, TempBuf, TempBuf.GetLength(), index);

	return nListCount;

}

BYTE USER::MailGetInfo( char* pBuf, int& index )
{
	/*if ( pBuf == NULL ) return -1;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	BYTE	error;

	int		i;
	int		nDBIndex;	

	SQLINTEGER	nNewMail = -1;
	SQLINTEGER	nCurMail = -1;

	SQLINTEGER	nUserIDSize;
	SQLINTEGER	nNewMailSize;
	SQLINTEGER	nCurMailSize;

	::ZeroMemory(szSQL, sizeof(szSQL));

	nDBIndex	= 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) 
	{
		error = 1;
		goto exit;
	}

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		error = 1;
		goto exit;
	}

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_INFO ( ?, ?, ? )}" ) );

	nUserIDSize = _tcslen(m_strUserID);
	nNewMailSize = SQL_NTS;
	nCurMailSize = SQL_NTS;

	i=1;
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30,	0, (TCHAR*)m_strUserID,	0, &nUserIDSize );
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0,	0, &nNewMail,	0, &nNewMailSize );
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0,	0, &nCurMail,	0, &nCurMailSize );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		error = 0;
	}
	else if (retcode == SQL_ERROR)
	{
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		error = 1;
		goto exit;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);	

exit:

	SetInt(pBuf, nNewMail, index);
	SetInt(pBuf, nCurMail, index);

	return error;*/
	////////////////////////////////////////////////
if ( pBuf == NULL ) return -1;
//   SendSystemMsg("邮件信息引导",SYSTEM_NORMAL,TO_ME);
	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	BYTE	error ;

	int		i;
	int		nDBIndex;	

	SQLINTEGER	nNewMail = -1;
	SQLINTEGER	nCurMail = -1;

	SQLINTEGER	nUserIDSize;
	SQLINTEGER	nNewMailSize;
	SQLINTEGER	nCurMailSize;

	::ZeroMemory(szSQL, sizeof(szSQL));
	nDBIndex	= 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) 
	{
		error = 1;
		goto exit;
	}

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		error = 1;
		goto exit;
	}

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_INFO ( ?, ?, ? )}" ) );

	nUserIDSize = _tcslen(m_strUserID);
	nNewMailSize = SQL_NTS;
	nCurMailSize = SQL_NTS;
	
	i=1;
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30,	0, (TCHAR*)m_strUserID,	0, &nUserIDSize );
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0,	0, &nNewMail,	0, &nNewMailSize );
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0,	0, &nCurMail,	0, &nCurMailSize );
    
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		error = 0;
	}
	else if (retcode == SQL_ERROR)
	{
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		error = 1;
		goto exit;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);	

exit:
	//CString msg;
	//msg.Format( "元宝数：%d 元宝数：%d", nNewMail,nCurMail);
	//SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
	SetInt(pBuf, nNewMail, index);
	SetInt(pBuf, nCurMail, index);
	return error;
}


void USER::MailContentReq( TCHAR* pBuf )
{
	/*if ( pBuf == NULL ) return;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;
	SQLINTEGER	nSQLIndex;

	int i;
	int nReadPos;
	int	nDBIndex;

	BYTE error;

	int nMailReqIndex;

	CBufferEx TempBuf;

	BOOL bQuerySuccess;

	SQLINTEGER	iIndex;
	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[15];
	SQLCHAR		bRead[20];
	SQLCHAR		strMessage[1001];

	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strMessage, sizeof(strMessage));

	nReadPos = 0;
	nMailReqIndex = GetInt( pBuf, nReadPos );

	nDBIndex = 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_MESSAGE ( %d )}" ), nMailReqIndex );
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strUserID,	sizeof(strUserID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strSendID,	sizeof(strSendID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		&bRead,		sizeof(SQL_C_CHAR),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strMessage,	sizeof(strMessage),	&nSQLIndex );

				TempBuf.Add((int)(iIndex));
				TempBuf.AddLongString((LPTSTR)strMessage, _tcslen((LPCTSTR)strMessage));

				::ZeroMemory(strUserID, sizeof(strUserID));
				::ZeroMemory(strSendID, sizeof(strSendID));
				::ZeroMemory(strTime, sizeof(strTime));
				::ZeroMemory(strMessage, sizeof(strMessage));

				break;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else 
			{
				bQuerySuccess = FALSE;
				break;
			}
		}
	}
	else
	{				
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);
	
	if( bQuerySuccess ) error = 0;
	else error = 1;
	
	nReadPos = 0;
	SetByte(m_TempBuf, MAIL_CONTENTS_RESULT, nReadPos);
	SetByte(m_TempBuf, error, nReadPos);
	if( error == 0 ) SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), nReadPos);

	Send(m_TempBuf, nReadPos);*/
/////////////////////////////////////////////////////////////////////////////////
//	SendSystemMsg("邮件内容请求",SYSTEM_NORMAL,TO_ME);
	if ( pBuf == NULL ) return;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;
	SQLINTEGER	nSQLIndex;
    SQLINTEGER  nUserIDSize;
	int i;
	int nReadPos;
	int	nDBIndex;

	BYTE error;

	int nMailReqIndex;

	CBufferEx TempBuf;

	BOOL bQuerySuccess;

	SQLINTEGER	iIndex;
	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[15];
	SQLCHAR		bRead[20];
	SQLCHAR		strMessage[1001];

	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strMessage, sizeof(strMessage));

	nReadPos = 0;
	nMailReqIndex = GetInt( pBuf, nReadPos );

	nDBIndex = 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_MESSAGE (%d)}" ), nMailReqIndex );
	nUserIDSize = _tcslen(m_strUserID);

	
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strUserID,	sizeof(strUserID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strSendID,	sizeof(strSendID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		&bRead,		sizeof(SQL_C_CHAR),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strMessage,	sizeof(strMessage),	&nSQLIndex );
		//		CString msg1;
		//		msg1.Format( "str1：%s",strMessage);
        //      SendSystemMsg((LPTSTR)(LPCTSTR)msg1, SYSTEM_NORMAL, TO_ME);

				TempBuf.Add((int)(iIndex));
				TempBuf.AddLongString((LPTSTR)strMessage, _tcslen((LPCTSTR)strMessage));
		//		CString msg;
		//		msg.Format( "str2：%s",strMessage);
		//		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);

				/*memset( strUserID,	NULL, sizeof(strUserID));
				memset( strSendID,	NULL, sizeof(strSendID));
				memset( strTime,	NULL, sizeof(strTime));
				memset( strMessage, NULL, sizeof(strMessage));*/
				::ZeroMemory(strUserID, sizeof(strUserID));
				::ZeroMemory(strSendID, sizeof(strSendID));
				::ZeroMemory(strTime, sizeof(strTime));
				::ZeroMemory(strMessage, sizeof(strMessage));

				break;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else 
			{
				bQuerySuccess = FALSE;
				break;
			}
		}
	}
	else
	{				
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);
	
	if( bQuerySuccess ) error = 0;
	else error = 1;
	
	nReadPos = 0;
	SetByte(m_TempBuf, MAIL_CONTENTS_RESULT, nReadPos);
	SetByte(m_TempBuf, error, nReadPos);
	if( error == 0 ) SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), nReadPos);

	Send(m_TempBuf, nReadPos);

}

void USER::MailDeleteReq( TCHAR* pBuf )
{
	if ( pBuf == NULL ) return;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	SQLINTEGER	nUserIDSize;
	SQLINTEGER		sReturn;
	SQLINTEGER	iReturnIndex;

	int nReadPos;
	int	nDBIndex;

	char pSuccessBuf[10000];
	int	nBufferCount;
	int nMailReqIndex;
	int	nMailNextIndex;
	int nMailListCount;

	int	i;
	BYTE error;

	CBufferEx TempBuf;

	::ZeroMemory( szSQL, sizeof(szSQL) );

	nReadPos = 0;
	nMailReqIndex = GetInt( pBuf, nReadPos );
	nMailNextIndex = GetInt( pBuf, nReadPos );

	nDBIndex = 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS ) return;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_DELETE ( %d, ?, ? )}" ), nMailReqIndex );

	nUserIDSize = _tcslen(m_strUserID);
	sReturn			= -1;
	iReturnIndex	= SQL_NTS;

	i=1;
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30,	0, (TCHAR*)m_strUserID,	0, &nUserIDSize );
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT,SQL_C_LONG, SQL_INTEGER, 0,		0, &sReturn,			0, &iReturnIndex);
	if (retcode!=SQL_SUCCESS) return;

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);

	error = (BYTE)sReturn;

	nReadPos = 0;
	SetByte(m_TempBuf, MAIL_DELETE_RESULT, nReadPos);
	SetByte(m_TempBuf, error, nReadPos);
	SetInt(m_TempBuf, nMailReqIndex, nReadPos);

	if( error == 0 )
	{
		nBufferCount = 0;
		MailGetInfo( pSuccessBuf, nBufferCount );
		SetString(m_TempBuf, pSuccessBuf, nBufferCount, nReadPos );

		nBufferCount = 0;
		nMailListCount = MailGetList( pSuccessBuf, nMailNextIndex, 1, nBufferCount );
		//SetShort(m_TempBuf, nMailListCount, nReadPos );
		SetString(m_TempBuf, pSuccessBuf, nBufferCount, nReadPos );
	}

	Send(m_TempBuf, nReadPos);
}



//------------------------------------------------------------
// 单捞磐 海捞胶甫 诀单捞飘 茄促.
//--yskang 0.1
//捞窃荐绰 静饭靛俊辑父 龋免钦聪促.
//------------------------------------------------------------
void USER::InsertLoveName(TCHAR *pBuf)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	BOOL			bSuccess = FALSE;
	TCHAR			szSQL[160];	
	BYTE error_code=0;
	USER *pUser = NULL;
	CDatabase* pDB = NULL;
	CBufferEx TempBuf;
	CBufferEx ResultTempBuf;
	int index = 0;
	int db_index = 0;
	int uid=0;
	uid = GetInt(pBuf, index);
	if(uid < 0 || uid >= INVALID_BAND) { error_code = ERR_2; goto go_result; }
	pUser = GetUser(uid - USER_BAND);								
	if(!pUser) { error_code = ERR_2; goto go_result; }// 蜡历啊 绝促.


	::ZeroMemory(szSQL, sizeof(szSQL));
	index = 0;
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call INSERT_LOVE_NAME (\'%s\', \'%s\',%d)}"), pUser->m_strUserID, pUser->m_strLoveName, pUser->m_dwGuild); 

	
	//pDB = g_DBNew[m_iModSid].GetDB( db_index );
	pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) { error_code = ERR_5; goto go_result; }

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		{ error_code = ERR_5; goto go_result; }

	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
		}
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	//g_DBNew[m_iModSid].ReleaseDB(db_index);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) { error_code = ERR_5; goto go_result; }

	//龋莫阑 皋葛府 叼厚俊 历厘茄促(?)
	lstrcpy(pUser->m_pMD->m_strLoveName , pUser->m_strLoveName);
	//龋莫阑 促弗 荤恩甸俊霸 舅赴促(鞍篮 胶农赴俊 乐绰荤恩甸俊 茄秦辑?)
	TempBuf.Add(LOVE_NAME);
	TempBuf.Add(uid);
	TempBuf.AddString(pUser->m_strLoveName);
	pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
	bSuccess = TRUE;
	m_bPseudoString=FALSE;//涝仿 己傍窍看栏骨肺 啊惑 龋莫捞 酒聪促.
go_result:
	ResultTempBuf.Add(LOVE_NAME_RESULT);
	if(bSuccess == FALSE)
	{
		ResultTempBuf.Add(FAIL);				// 角菩
		ResultTempBuf.Add(error_code);
		Send(ResultTempBuf,ResultTempBuf.GetLength());
		return;
	}
	ResultTempBuf.Add(SUCCESS);					// 己傍	
	Send(ResultTempBuf, ResultTempBuf.GetLength());
}



//////////////////////////////////////////////////////////////////////////////
//	霸矫拱阑 昏力茄促.
//
void USER::BBSDelete(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	int bbsnum = GetShort( pBuf, index );
	int delindex = GetInt( pBuf, index );

	if(m_tIsOP != 1 && ( bbsnum == 1 || bbsnum == 3 ) )		//傍瘤霸矫魄 or 肺凯烦喉 霸矫魄
	{
		SendSystemMsg( IDS_USER_NEED_OPERATOR, SYSTEM_NORMAL, TO_ME);
		return;
	}

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	SQLSMALLINT	sRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;

	SDWORD sIDLen		= _tcslen(m_strUserID);

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_DELETE (%d, %d, ?, ?)}" ), bbsnum, delindex );

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Delete BBS (BBS:%d, Index:%d) !!\n", bbsnum, delindex);
		return;
	}

	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)m_strUserID,	0, &sIDLen );
	retcode = SQLBindParameter(hstmt, 2 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &iRetInd);
	if (retcode!=SQL_SUCCESS) 
	{
		TRACE("USER::BBSDelete(), Line = %d, File = %s\n", __LINE__, __FILE__);
		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if( sRet != 1 )
	{
		if(g_bDebug) SendSystemMsg( IDS_USER_BBS_CANT_DELETE, SYSTEM_NORMAL, TO_ME);
		SendBBSError(BBS_CONTENTS_DELETE);
		return;
	}
	BBSOpen( bbsnum );
}

void USER::BBSDeleteWithThread(TCHAR *pBuf)
{
	int index = 0;
	int bbsnum = GetShort( pBuf, index );
	int delindex = GetInt( pBuf, index );

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = BBS_DELETE;
	Datalength = index;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, 0, Datalength+1);
	memcpy(pData, pBuf, Datalength);
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );
}

//##################################################################################
//
//	Battle
//
//##################################################################################

//////////////////////////////////////////////////////////////////////////////////
//	User 狼 傍拜仿阑 掘绢柯促.
//
int USER::GetAttack()
{
	// 唱吝俊绰 傈捧傍侥措肺 贸府窍档废 茄促.
	// 瘤陛篮 弊成 罚待栏肺 掘绢柯促.

	int nAttack = XdY(2, 80);
	return nAttack;
}

//////////////////////////////////////////////////////////////////////////////////
//	蜡历狼 规绢仿阑 掘绢柯促.
//  人物防御
int USER::GetDefense(short* sItemDefense)
{
	int nDefense = 0;

	short sHeadSid = m_UserItem[HEAD].sSid;
	short sBodySid = m_UserItem[BODY].sSid;
	short sPantsSid = m_UserItem[PANTS].sSid;
	short sShoesSid = m_UserItem[SHOES].sSid;

	short sHeadDefense = GetItemDefense(sHeadSid, HEAD);
	short sBodyDefense = GetItemDefense(sBodySid, BODY);
	short sPantsDefense = GetItemDefense(sPantsSid, PANTS);
	short sShoesDefense = GetItemDefense(sShoesSid, SHOES);

	if(m_UserItem[HEAD].sDuration <= 0) sHeadDefense = 0;
	if(m_UserItem[BODY].sDuration <= 0) sBodyDefense = 0;
	if(m_UserItem[PANTS].sDuration <= 0) sPantsDefense = 0;
	if(m_UserItem[SHOES].sDuration <= 0) sShoesDefense = 0;


	nDefense = sHeadDefense + sBodyDefense + sPantsDefense + sShoesDefense + m_DynamicUserData[MAGIC_DEFENSE_UP] \
			                  + (int)((double)m_sMagicCON * (double)m_DynamicEBodyData[EBODY_CON_TO_DEFENSE] / 100) ; 

	short sMaxLevel = g_arUserLevel.GetSize() + ADD_USER_LEVEL;//yskang temp m_sLevel篮 g_arUserLevel狼 硅凯农扁甫 逞辨荐 绝促.
	if(m_sLevel >= ADD_USER_LEVEL && m_sLevel<sMaxLevel ) nDefense += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDefense;

	nDefense += (int)((double) nDefense * (double)m_DynamicEBodyData[EBODY_DEFENSE_UP] / 100); 
	if(nDefense <= 0) nDefense = 0;

	/*if (m_byClass==0)//如果是拳
	{
		nDefense=nDefense + (int)(nDefense*0.2);
	}
	if (m_byClass==1)//如果是法师防增5%
	{
		nDefense=nDefense + (int)(nDefense*0.1);
	}
    if (m_byClass==2)//如果是
	{
		nDefense=nDefense + (int)(nDefense*0.1);
	}
	if (m_byClass==3)//如果是法师防增5%
	{
		nDefense=nDefense + (int)(nDefense*0.1);
	}
	if (m_byClass==4)//如果是法师防增5%
	{
		nDefense=nDefense + (int)(nDefense*0.1);
	}*/

	
	////////////////////////////////////////////////////
	

	return nDefense;
}

///////////////////////////////////////////////////////////////////////////////////
//	酒捞攫 胶懦
//
double USER::GetIronSkill()
{
	BYTE tWeaponClass = 0;
	
	int IronSkin = 0;
	int iLevel = 0;
	int nDefense = 0;
	int iSkillSid = 0;

	double nISkill = 0;

	if(IsCanUseWeaponSkill(tWeaponClass) == FALSE)		// 泅犁 磊脚捞 坷弗颊俊 电 公扁狼 胶懦阑 荤侩且 荐 绝栏搁 溜 盖颊捞搁
	{
		nDefense = (int)((double)m_sMagicCON/3 + 0.5);
		return nDefense; //酒捞攫 胶挪捞 绝栏搁 
	}

	int tClass = tWeaponClass * SKILL_NUM;
	// 磊扁 泅犁 胶懦吝... 
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_IRON) // 酒捞攫 胶挪(9)					// 0 index
		{			
			iLevel = m_UserSkill[i].tLevel;		// 鉴荐 胶懦 饭骇 
			if(iLevel <= 0) continue;
												// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			if(iLevel >= g_arSkillTable[iSkillSid]->m_arInc.GetSize()) return 0;
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

			IronSkin += g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel);
		}
	}

	nISkill = ((double)m_sMagicCON/3)*(1 + (double)IronSkin/100.0) + 0.5; // 刘啊啦

	if(nISkill <= 0) nISkill = 0;
	return nISkill;
}

///////////////////////////////////////////////////////////////////////////////////
//
void USER::SetDamage(int nDamage)
{
	if(nDamage <= 0) return;
	if(m_bLive == USER_DEAD) return;

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Test Code
	if( m_sHP <= (short)nDamage )
	{
		if(m_UserItem[39].sSid == 1184 && m_UserItem[39].sBullNum >= 300 && m_dwTransTime == 0 && m_iSkin == 8){	
			m_sHP = m_sMagicMaxHP;
			m_dwTransTime = 60000;//狂暴持续时间1分钟
			m_dwLastTransTime = GetTickCount();
			m_UserItem[39].sBullNum -= 300; //机甲耐久减少5点
			if(m_UserItem[39].sBullNum <0) m_UserItem[39].sBullNum = 0;
			m_iSkin = 170;			
			SendCharData();
			SendMyInfo(TO_INSIGHT, INFO_MODIFY);			
			CheckMagicItemMove();
			SendSystemMsg("Trans-AM启动,战神系统进入狂暴模式!", SYSTEM_ERROR, TO_ME);			
			AddStateInfo(STATE_18);
			CBufferEx TempBuf;
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);					
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);	
			Send(TempBuf, TempBuf.GetLength());			
		}
        else if(m_UserItem[39].sSid == 1184 && m_UserItem[39].sBullNum >= 300 && m_dwTransTime == 0 && m_iSkin == 5){	
			m_sHP = m_sMagicMaxHP;
			m_dwTransTime = 60000;//狂暴持续时间1分钟
			m_dwLastTransTime = GetTickCount();
			m_UserItem[39].sBullNum -= 300; //机甲耐久减少5点
			if(m_UserItem[39].sBullNum <0) m_UserItem[39].sBullNum = 0;
			m_iHair = 10015;			
			SendCharData();
			SendMyInfo(TO_INSIGHT, INFO_MODIFY);			
			CheckMagicItemMove();
			SendSystemMsg("Trans-AM启动,超级机甲系统进入狂暴模式!", SYSTEM_ERROR, TO_ME);			
			AddStateInfo(STATE_18);
			CBufferEx TempBuf;
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);					
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);	
			Send(TempBuf, TempBuf.GetLength());			
		}
		else if(InterlockedCompareExchange((LONG*)&m_lDeadUsed, (long)1, (long)0) == (long)0)
		{
			m_sHP = 0;
			Dead();
		}
		else return;
	}
	else
	{
		m_sHP -= (short)nDamage;
	}

	SendHP();	

	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	
/*	m_sHP -= (short)nDamage;

	if( m_sHP <= 0 )
	{
		if(InterlockedCompareExchange(&m_lDeadUsed, (LONG)1, 0) == 0)
		{
			m_sHP = 0;
			Dead();
		}
		else m_sHP = 1;
	}

	SendHP();
	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
*/
//	TRACE("USER(%d) HP = %d\n", m_uid, m_sHP);
}

////////////////////////////////////////////////////////////////////////////////////
//	荤噶贸府
//
void USER::Dead()
{
	DWORD dwCurrTime = GetTickCount();

	if(m_bLive == USER_DEAD) return;

	if(UpdateUserData() == FALSE)	// DB俊 盎脚捞 救登搁
	{
		m_bLive = USER_LIVE;
		m_sHP = 1;
		SendHP();
		TRACE("DEAD HP =1, UpdateUserData() Fail!!!!\n");
//		SendSystemMsg("UpdateUserData() Fail", SYSTEM_NORMAL, TO_ME);
		return;
	}

//	SetUid(m_curx, m_cury, 0);
	m_bLive = USER_DEAD;

	// 肺凯烦喉 贸府
	ExitRoyalRumble();

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 162; pos.y = 1452; }	// 捞何盒篮 唱吝俊 官操磊

	CBufferEx TempBuf;
	TempBuf.Add(DEAD);
	TempBuf.Add((short)(m_uid + USER_BAND));
	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);
 
	SendInsight(TempBuf, TempBuf.GetLength());
    //发送用户死亡
	if(m_tBabyCall ==1)//如果有宠物的角色死亡->则掉忠诚度
    {
	  if ( m_curz == 66 || m_curz == 67 || IsInPKC()) return;//活动地图不掉忠诚度
      if(m_sFealty > 4) m_sFealty -= 3;
         SendEventMsg("由于您死亡,造成宠物[忠诚度]减少[ 3 ]");
         SendBabyInfo();
    }
	//如果还有护法也同样死亡
	if(m_tHuFaType !=0&&m_nHuFaHP>0)
	{
		SendHuFaPostion(HUFA_DEA);
	}
	ClearAbnormalTime(ABNORMAL_TIME);
	ClearAbnormalTime(PSI_ONE_TIME);
	ClearAbnormalTime(PSI_TWO_TIME);
	ClearAbnormalTime(PSI_THREE_TIME);
	ClearAbnormalTime(TRANS);
	
	m_dwAbnormalInfo = ABNORMAL_NONE;
	m_dwAbnormalInfo_ = ABNORMAL_NONE;
	m_dwAbnormalInfo_ |= 1;
	
	FreedomCB();
	if(isGuildRen == TRUE || m_bCBOff == TRUE) 
	{
		m_dwAbnormalInfo_ |= ABNORMAL_POISON;
		FreedomCB();
	}
	else 
	{
		if(isGuildRen == FALSE || m_bCBOff == FALSE)
		{
	m_dwAbnormalInfo_	= ABNORMAL_NONE;
	m_dwAbnormalInfo_ |= ABNORMAL_POISON;
	FreedomCB();
		}
	}	

        if(m_tIsOP == 1) AddAbnormalInfo(OPERATION_MODE);
///////////////////人物死亡时清理各职业135状态//////////////////////////	
	if(m_sLevel>=135)
	{
		switch(m_byClass)		//判断职业
		{
			case 0:
				DelHuFaStatus(New_Status_12);
				break;
			case 1:
				DelHuFaStatus(New_Status_13);
				break;
			case 2:
				DelHuFaStatus(New_Status_14);
				break;
			case 3:
				DelHuFaStatus(New_Status_15);
				break;
			case 4:
				DelHuFaStatus(New_Status_15);
				break;
		}
		SendJudgePsiStatus();
		SendUserStatusSkill();
	}

	m_dwNoDamageTime	= 0;
	m_dwLastNoDamageTime= GetTickCount();

	SetPsiAbnormalStatus();
    SetStatus();
	
    SetUserToMagicUser(0);

	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)
	{
		if(m_bGuildMaster)			// 辨付搁 场车促.
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// 亲汗
		}
		else EndGuildWar(GUILD_FIELD_WARRING);			// 促矫 曼啊啊 救等促.
	}

	// 芭贰吝捞搁 芭贰秒家 贸府
	if(m_bNowTrading == TRUE) 
	{
		BYTE result = 0x00;
		SendExchangeFail(result, (BYTE)0x04);

		USER *pTradeUser = NULL;
		if(m_iTradeUid != -1)
		{
			pTradeUser = GetUser(m_iTradeUid - USER_BAND);
			if(pTradeUser != NULL)	
			{
				if(pTradeUser->m_state == STATE_GAMESTARTED) pTradeUser->SendExchangeFail(result, (BYTE)0x05);
			}
		}
	}

//	m_sHP = m_sMaxHP;

//	SendMyInfo(TO_ME, INFO_MODIFY);
//	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
}

///////////////////////////////////////////////////////////////////////////////////
//死亡减经验.和掉东西...
// yskang 0.8 add dead log strPKName 眠啊 
void USER::GetLevelDownExp(BOOL bPK, int enemyrank, BOOL bEventSummon,TCHAR *strPKName)
{
	//StartLevelDown = TRUE;
	BYTE tSlot = 0;
	CBufferEx TempBuf;

	int minusratio = GetMinusExpRatioByCityRank( enemyrank );
    if (!bPK && m_sLevel>=105) minusratio=minusratio / 2;    /////被怪杀不掉经验
	if (!bPK) minusratio=minusratio / 500;
    else minusratio=minusratio / 2;
	double minusresult = 0.01 * (double)minusratio;

	int skill[] = {0,0,0,0,0};
	int status[] = {0,0,0,0,0};
	int type = 0;
	DWORD minusExp = 0;
	DWORD preExp = 0;
	long lDiffExp = 0;
	int LevelDownType = 10;
	m_dwLastDownExp = 0;
	
    if(m_bPkStatus) 
	{
		m_dwPkStatusTime = 0;
		m_bPkStatus = FALSE;
	}
    if(m_curz == 66 || m_curz ==67 || m_curz == 45 || m_curz == 40 || m_curz == 43 || m_curz == 44 || m_curz == 58)		//杀人狂大赛和PK竞赛不掉经验//PKC不掉经验
		return;

	if(m_tGuildHouseWar == GUILD_WARRING) return;// Virtual Room俊辑绰 康氢捞 绝促.
	if(m_sLevel <= 5) return;
	if(CheckInvalidMapType() == 12) return;		// 措访厘

	if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING)
	{
		minusExp = (DWORD)m_dwExpNext/100;		//军团战中死亡掉10点
		LevelDownType = 10;//5%
	}
	if(bEventSummon || m_sCityRank == 6 || m_sCityRank == 7)							// 
	{
		minusExp = (DWORD)m_dwExpNext/100;		// 捞亥飘 魔傍 阁胶磐 老锭 1% 皑家
		LevelDownType = 1;
	}
	else
	{
//		minusExp = (DWORD)m_dwExpNext/10;		// 促澜 饭骇 版氰摹狼 10%阑 扁霖栏肺 皑家矫挪促.
		minusExp = (DWORD)((double) m_dwExpNext * minusresult );

		//--------------------------------------------------------------------------------------
		//yskang fortress buger 父距 傍己 瘤开捞扼搁 版氰摹 5%
		for(int i = 0; i < g_arGuildFortress.GetSize(); i++)
		{
			//break;
			if(!g_arGuildFortress[i]) continue;

			if(m_curz == g_arGuildFortress[i]->m_iZone)
			{
				minusExp = (DWORD)m_dwExpNext/20;		// 傍己傈老锭...惑痢 辨靛傈老锭...
				LevelDownType = 5;
				break;
			}
			int type = 0;
			type = CheckInvalidMapType();
			if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16)//傍己 瘤开捞促. 
			{
				minusExp = (DWORD)m_dwExpNext/20;		
				LevelDownType = 5;     
				break;
			}
			if(g_arGuildFortress[i]->m_lUsed == 1)
			{
				if(g_arGuildFortress[i]->m_sFortressID == 1000 && type==9)
				{
					minusExp = (DWORD)m_dwExpNext/20;		
					LevelDownType =5;
					break;
				}
				else if(g_arGuildFortress[i]->m_sFortressID == 1001 && type==11)
				{
					minusExp = (DWORD)m_dwExpNext/20;		// 傍己傈老锭...惑痢 辨靛傈老锭...
					LevelDownType = 5;
					break;
				}
				else if(g_arGuildFortress[i]->m_sFortressID == 1002 && type==16)
				{
					minusExp = (DWORD)m_dwExpNext/20;		// 傍己傈老锭...惑痢 辨靛傈老锭...
					LevelDownType=5;
					break;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------

	}
//	if(m_bRingOfLife == TRUE && minusExp > 0)	
	
		// 酒捞袍捞 函悼夌促绰 沥焊甫 舅妨拎具瘤
		m_bRingOfLife=false;
		m_bNecklaceOfShield=false;
		m_bEarringOfProtect=false;
		for(int i = 0; i < MAX_ACCESSORI; i++)
		{
			if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_RING_OF_LIFE)		m_bRingOfLife = TRUE;
			if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_NECKLACE_OF_SHIELD)	m_bNecklaceOfShield = TRUE;
			if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_PROTECT)	m_bEarringOfProtect = TRUE;
		}
	
		EnterCriticalSection( &m_CS_ExchangeItem );
	if(m_bRingOfLife == TRUE)	
	{
		SendAccessoriDuration(SID_RING_OF_LIFE);
		LiveReq(NULL, 1);
		SendSystemMsg(IDS_RING_OF_LIFE, SYSTEM_ANNOUNCE, TO_ME);
		m_bRingOfLife=false;
		LeaveCriticalSection( &m_CS_ExchangeItem );
		return;
	}
	LeaveCriticalSection( &m_CS_ExchangeItem );
	//--------------------------------------------------------------------------------------------------
	//--yskang 0.6 蜡丰 荤侩磊  橇府固决 - 版氰摹 皑家甫 歹 临咯 霖促. 焊烹荤侩磊狼 1/2荐霖栏肺 
	int tempExp = minusExp;
	if(m_iDisplayType != 5 && m_iDisplayType !=6 && LevelDownType !=1 )
	{
		if(LevelDownType == 5) 
			m_dwExp += (DWORD)m_dwExpNext/40; 
		else if(LevelDownType == 10)
			m_dwExp += (DWORD)((double)m_dwExpNext*minusresult/2.0);
		else
			m_dwExp += (DWORD)m_dwExpNext/20; 

		tempExp = minusExp/2;
	}
	//--------------------------------------------------------------------------------------------------

	lDiffExp = m_dwExp - minusExp;
	preExp = m_dwExp;

	m_dwLastDownExp	= tempExp;				// 何劝且 版快 捞版氰摹甫 扁霖栏肺 版氰摹甫 汗备茄促.

    if(m_sLevel > 10)	DropItemFromDeadUser(bPK, enemyrank);			//牛牛

	if(lDiffExp >= 0)							// 版氰摹父 皑家茄促.
	{
		type = 0;
		m_dwExp = lDiffExp;
		SendExp();
		UpdateUserDeadLog(minusExp, preExp,strPKName);//yskang 0.8 add dead log		// 蜡历 版氰摹 肺弊甫 SQL静饭靛俊辑 贸府 //肺弊 巢辨锭绰 沥惑利栏肺 焊捞霸 窍扁 困窍咯..tempMinus函荐啊 鞘夸.
		return;	
	}
	else										// 饭骇 促款捞 老绢抄促.
	{	
		type = 2;
		int sCount = 20;						// 弥措 角青 啊瓷茄 风橇 冉荐

		lDiffExp = (long)(m_dwExp * (double)100/minusExp);		// 10%皑家吝 倔付父怒狼 %啊 皑家登绢 乐绰瘤...

		m_sLevel = m_sLevel - 1;				// 饭骇皑家窍绊 皑家等 饭骇俊辑 巢篮%甫 歹猾促.. 

		//降级
		BabyUpgradeLevel();

//		long minus = g_arLevelUpTable[m_sLevel]->m_dwExp/10;
		
		long minus=0;
		if(LevelDownType ==5 ) 
			minus = (long)(g_arLevelUpTable[m_sLevel]->m_dwExp /20);//5;
		else if(LevelDownType == 1)
			minus = (long)(g_arLevelUpTable[m_sLevel]->m_dwExp /100);//1;
		else
			minus = (long)(g_arLevelUpTable[m_sLevel]->m_dwExp * minusresult);///10;

		minus = (long)(minus * (double)( 1.0 - (double)lDiffExp/100));	// 秦寸 饭骇俊 10%吝 哗具且 弥辆樊	
		m_dwExp = (DWORD)(g_arLevelUpTable[m_sLevel]->m_dwExp - minus);
												// Skill Point甫 皑茄促.
		if( m_sLevel < 99 )
		{
			if(g_arLevelUpTable[m_sLevel]->m_tBasicUp)
			{
				if(m_sSkillPoint > 0) m_sSkillPoint -= 1; 
				else 
				{ 
					int iSid = 0;
					int tClass = m_byClass * SKILL_NUM;
					sCount = 0;

					if(m_UserSkill[tClass].tLevel > 0)     { skill[sCount] = tClass; sCount++; }
					if(m_UserSkill[tClass + 1].tLevel > 0) { skill[sCount] = tClass + 1; sCount++; }
					if(m_UserSkill[tClass + 2].tLevel > 0) { skill[sCount] = tClass + 2; sCount++; }
					if(m_UserSkill[tClass + 3].tLevel > 0) { skill[sCount] = tClass + 3; sCount++; }
					if(m_UserSkill[tClass + 4].tLevel > 0) { skill[sCount] = tClass + 4; sCount++; }

	//				while(sCount > 0)					//	0~4, 5~9, 10~14, 15~19鳖瘤 胶懦吝
					if(sCount > 0)
					{
						sCount = sCount - 1;
						type = myrand(0, sCount, TRUE);

						m_UserSkill[skill[type]].tLevel -= 1;
					}			
				}										
			}
		}
		else
		{
			if(m_sLevel <119){//100 - 119 级之间 减1点百级技能
				int emptyskillslot;
				switch( m_byClass )
				{
					case	BRAWL:
						emptyskillslot = 4;
						break;
					case	STAFF:
						emptyskillslot = 9;
						break;
					case	EDGED:
						emptyskillslot = 14;
						break;
					case	FIREARMS:
						emptyskillslot = 19;
						break;
					case	JUDGE:
						emptyskillslot = 24;
						break;
					default:
						return;
				}
                ResetOver100LevelSkill( m_sLevel );
				/*if( emptyskillslot == -1 ) return;
				int skillsid = m_UserSkill[emptyskillslot].sSid;

				if( skillsid < 0 || skillsid >= g_arSkillTable.GetSize() ) { m_UserSkill[emptyskillslot].sSid =0; return;}
				if( g_arSkillTable[skillsid]->m_sSid == -1 ) return;
				m_UserSkill[emptyskillslot].tLevel = (BYTE)((m_sLevel % 100) + 1);
				if(m_UserSkill[emptyskillslot].tLevel>20)
					m_UserSkill[emptyskillslot].tLevel=20;*/
			}
			else if(m_sLevel <129){// 120 -129 减1点120技能转换后的技能
				int iIndex=m_byClass *SKILL_NUM;
				//新职业技能
				if(m_byClass == JUDGE)
				{
					iIndex =m_byClass *SKILL_NUM;
				}else
				{
					iIndex = 24;
				}
				int i;
				for(i = iIndex; i < iIndex + SKILL_NUM-1; i++)
				{
					if(m_UserSkill[i].sSid>=20)
					{
						m_UserSkill[i].tLevel--;
						if(m_UserSkill[i].tLevel>=20)
							m_UserSkill[i].tLevel=20;
					}
				}
			}
			
			
			else 
			{
				//150以上减一点150级技能
				//140级以上减一点140级技能
				if(m_sLevel>=149 && m_UserSkill140[1].tLevel>0 && o_yehuoini[0]->chaoneng == 1 )
				{
					m_UserSkill140[1].tLevel--;
					if(m_UserSkill140[1].tLevel == 0)
						m_UserSkill140[1].sSid = 0x00;
				}else if(m_sLevel>=139 && m_UserSkill140[0].tLevel > 0 && o_yehuoini[0]->chaoneng == 1)
				{
					m_UserSkill140[1].tLevel=0;
					m_UserSkill140[1].sSid = 0x00;

					m_UserSkill140[0].tLevel--;
					if(m_UserSkill140[0].tLevel == 0)
						m_UserSkill140[0].sSid = 0x00;
			 }
				//130 以上减1点 130技能
				else if (o_yehuoini[0]->chaoneng == 1)
			  {
				int count= m_UserSkill_[m_byClass*3].tLevel + m_UserSkill_[m_byClass*3+1].tLevel + m_UserSkill_[m_byClass*3+2].tLevel + m_sSkillPoint_;
				if(count== (m_sLevel-130+1))
					return;
			if(count>(m_sLevel-130+1))
			{
				if(m_sSkillPoint_ > 0){
					m_sSkillPoint_ -= 1;
				}else{
					int iSid = 0;
					int tClass = m_byClass * 3;
					sCount = 0;
					if(m_UserSkill_[tClass].tLevel > 0)     { skill[sCount] = tClass; sCount++; }
					if(m_UserSkill_[tClass + 1].tLevel > 0) { skill[sCount] = tClass + 1; sCount++; }
					if(m_UserSkill_[tClass + 2].tLevel > 0) { skill[sCount] = tClass + 2; sCount++; }
					if(sCount > 0)
					{
						sCount = sCount - 1;
						type = myrand(0, sCount, TRUE);
						m_UserSkill_[skill[type]].tLevel -= 1;
					}
				}
			 }
		  }
			//ResetOver100LevelSkill( m_sLevel );
			}
		}
			
		
		if(m_sPA > 0)	m_sPA -= 1;
		else									// 歹 捞惑 绝促搁 阶咯乐绰 器牢磐甫 皑茄促.
		{
			sCount = 0;
			if(m_sSTR > m_sTempSTR) { status[sCount] = 0; sCount++; } 
			if(m_sCON > m_sTempCON) { status[sCount] = 1; sCount++; }
			if(m_sDEX > m_sTempDEX) { status[sCount] = 2; sCount++; }
			if(m_sVOL > m_sTempVOL) { status[sCount] = 3; sCount++; }
			if(m_sWIS > m_sTempWIS) { status[sCount] = 4; sCount++; }

			if(sCount > 0)					// 0~4吝 伐待栏肺 猾促.
			{
				sCount = sCount - 1;
				type = myrand(0, sCount, TRUE);

				type = status[type];

				switch(type)
				{
				case USER_STR:
					if(m_sSTR > m_sTempSTR) m_sSTR -= 1;					
					break;
				case USER_CON:
					if(m_sCON > m_sTempCON) m_sCON -= 1;
					break;
				case USER_DEX:
					if(m_sDEX > m_sTempDEX) m_sDEX -= 1;
					break;
				case USER_VOL:
					if(m_sVOL > m_sTempVOL) m_sVOL -= 1; 
					break;
				case USER_WIS:
					if(m_sWIS > m_sTempWIS) m_sWIS -= 1; 
					break;
				default:
					break;
				}
			}

			SetUserToMagicUser();		// 瓷仿摹 皑家肺 函悼捞 老绢唱搁
		}

		if(m_sHP > m_sMagicMaxHP) m_sHP = m_sMagicMaxHP;
		if(m_sPP > m_sMagicMaxPP) m_sPP = m_sMagicMaxPP;
		if(m_sSP > m_sMagicMaxSP) m_sSP = m_sMagicMaxSP;
	}

	UpdateUserDeadLog(minusExp, preExp,strPKName);	//yskang 0.8 add dead log	// 蜡历 版氰摹 肺弊甫 SQL静饭靛俊辑 贸府

	UpdateUserData();

	m_dwExpNext = GetNextLevel(m_sLevel);	// 促澜 饭骇诀阑 困茄 弥措摹

	SendExp((BYTE)0x02);					// 饭骇诀 菩哦栏肺 Level, Exp, PA, MA, SkillPoint甫 盎脚
	SendUserStatusSkill(); 
	SendSystemMsg( IDS_USER_LEVEL_DOWN, SYSTEM_NORMAL, TO_ME);
}

///////////////////////////////////////////////////////////////////////////////////////
//	掉装备
//
void USER::DropItemFromDeadUser(BOOL bPK, int enemyrank)
{
	if(m_state != STATE_GAMESTARTED ) return;	// 促弗 蜡历俊辑 龋免且荐 乐栏骨肺...
	if (!bPK) return;
    //PK竞赛和杀人狂大赛不掉东西//PKC不掉东西
	if (m_curz == 66 || m_curz == 67 || m_curz == 45 || m_curz == 40 || m_curz == 43 || m_curz == 44 || m_curz == 58  )	return;
	if (m_bPShopOpen == TRUE || m_bNowTrading == TRUE)	return;
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1) return;//未解除保护掉东西类返回  交易密码
    if (m_bSessionOnline == true) return;
 	int i, sid = -1, invensid = -1, equipsid = -1/*,ebodysid = -1*/;
	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	int iRandom = myrand(1, 100);
	int iWhere = myrand(1, 100);
	//int iEjixie = myrand(1,100);

	BOOL bDrop = FALSE;
	BOOL bInven = FALSE;
	//BOOL bEbody = FALSE;

	CWordArray	arEquip, arInven/*, arEbody*/;

	CBufferEx	TempBuf;

	rank = GetDropItemRankByAttackerCityRank( enemyrank );

	if(CheckGuildWarArea())
	{
		rank -= 1;
		if( rank < 0 ) rank = 0;
	}

	switch(rank)							// 阿 殿鞭喊肺 犬伏捞 促福促.
	{
    case CHAONISE_RANK:
		if(iRandom < 100) { bDrop = TRUE; if(iWhere < 100) bInven = TRUE;  }
		break;
	case DEATH_RANK:
		if(iRandom < 90) { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }
		break;
	case EVIL_RANK:
		if(iRandom < 80) { bDrop = TRUE; if(iWhere < 80) bInven = TRUE;  }
		break;
	case VILLAIN_RANK:
		if(iRandom < 70) { bDrop = TRUE; if(iWhere < 70) bInven = TRUE;  }
		break;		
	case CAIN_RANK:
		if(iRandom < 50) { bDrop = TRUE; if(iWhere < 50) bInven = TRUE;  }
		break;

	case COMMOM_RANK:
		if(iRandom < 0) { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }
		break;

	case CITIZEN_RANK:
		if(iRandom < 0) { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }
		break;
	case VIUAL_RANK:
		if(iRandom < 0) { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }
		break;
	case WARRIOR_RANK:
		if(iRandom < 0) { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }
		break;
	case HERO_RANK:		
		if(iRandom < 0) { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }
		break;
	case SAINT_RANK:		
		
		if(iRandom < 0)  { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }	// 技牢飘绰 酒捞袍阑 冻备瘤 臼绰促. 缴狼包访
		break;
    //case GUARDANT_RANT:
    //    if(iRandom < 0)  { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }
	//    break;
	//case ANGEL_RANK://天使
	//    if(iRandom < 0)  { bDrop = TRUE; if(iWhere < 90) bInven = TRUE;  }
	//    break;
	
   case (ANGEL_RANK+1):
		bDrop = FALSE;
		break;

	default:
		if(iRandom < 90) { bDrop = TRUE; if(iWhere < 10) bInven = TRUE; }
		break;
	}

	if(bDrop)							// 促矫 弥利拳 窍磊...//@@@@@@@@@@@@@@@@@@@@@@@2
	{
		arEquip.RemoveAll();
		arInven.RemoveAll();
		//arEbody.RemoveAll();

		for(i = 0; i < EQUIP_ITEM_NUM; i++)
		{
			if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
			{
				if(g_arItemTable[m_UserItem[i].sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND) continue;

				arEquip.Add(i);
			}
		}

		for(i = EQUIP_ITEM_NUM; i < TOTAL_INVEN_MAX; i++)
		{
			if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
			{
				if(g_arItemTable[m_UserItem[i].sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND) continue;

				arInven.Add(i);
			}
		}
        iWhere = arInven.GetSize();
		if(iWhere > 0)
		{		 
			iRandom = myrand(1, iWhere);
			invensid = arInven[iRandom - 1];
		}

		iWhere = arEquip.GetSize();
		if(iWhere > 0)
		{
			iRandom = myrand(1, iWhere);
			equipsid = arEquip[iRandom - 1];
		}
        
		
        if(bInven) sid = invensid;
		
		else 
		{	
			sid = equipsid;	
		}
			
		if(rank <= CHAONISE_RANK)
		{
			if(equipsid >= 0) sid = equipsid;
			else              sid = invensid;
			
		}

		if(sid != -1 && m_UserItem[sid].sSid >= 0)
		{
			if(m_UserItem[sid].sSid < g_arItemTable.GetSize())
			{
				
					int index = 0;
					
					TCHAR tBuf[10];
					::ZeroMemory(tBuf, sizeof(tBuf));
					
					CPoint pos = ConvertToClient( m_curx, m_cury );
					if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }	// 唱吝俊 怖 茫酒辑 官操磊.
					
					SetShort(tBuf, pos.x, index);
					SetShort(tBuf, pos.y, index);
					
					SetByte(tBuf, (BYTE)sid, index);
					SetShort(tBuf, m_UserItem[sid].sCount, index);
					
					PKGiveItemToMap(tBuf, TRUE);//死亡掉东西显示

				//}

				if(!bInven) CheckMagicItemMove();		// 酒捞袍 函悼荤亲捞乐栏骨肺 眉农...

			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////
//	PP 甫 努扼捞攫飘肺 焊辰促.
//
void USER::SendPP(BOOL bLevelDown)
{
	CBufferEx TempBuf;

	if(m_bLive == USER_DEAD && !bLevelDown) return;

	TempBuf.Add(SET_PP);
	TempBuf.Add(m_sPP);
	TempBuf.Add((short)-1);//关闭龙心的显示

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////
//	泅犁 PP甫 掘绰促.
//
short USER::GetPP()
{
	BYTE	tWeaponClass = 255;

	int iSkillSid = 0;
	int iLevel = 0, iRandom = 0, iPP = 0;

	double dPlusPP = 1.0; 
	int iAddSP = 0;

	DWORD dwDiffTime = GetTickCount() - m_dwLastPsiTime;

	if(dwDiffTime >= m_iPPIntervalTime)
	{
		if(IsCanUseWeaponSkill(tWeaponClass))	// 泅犁 磊脚捞 坷弗颊俊 电 公扁狼 胶懦捞 乐栏搁
		{
			int tClass = tWeaponClass * SKILL_NUM; 

			for(int i = tClass; i < tClass + SKILL_NUM; i++)			// 胶懦吝 HP雀汗 胶懦捞 乐栏搁 馆康茄促.
			{
				iSkillSid = m_UserSkill[i].sSid;

				if(iSkillSid == SKILL_PP_RECOVER_UP)					// 5 index
				{
					iLevel = m_UserSkill[i].tLevel;		
					if(iLevel < 0) iLevel = 0;

					// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
					if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
					if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
					if(iSkillSid >= g_arSkillTable.GetSize()) continue;

					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)) iPP = 1;

					dPlusPP = 1.0 + iPP * (double)g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel) / 100.0;
				}
			}
		}
								// 焊沥蔼
		if(m_iPPRate <= 0) 
		{
			m_iPPRate = 1;
			TRACE("雀汗厚啦 俊矾\n");
		}

		if(IsCity() && CheckRecoverTableByClass()) iAddSP = g_arRecoverTable[m_byClass]->m_byTown;
		m_sPP += (int)(dPlusPP * (double)m_sMagicWIS/m_iPPRate ) + iAddSP;
		if(m_sPP > m_sMagicMaxPP) m_sPP = m_sMagicMaxPP;

		m_dwLastPsiTime = GetTickCount();
	}

	return m_sPP;
}

////////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 傍拜贸府
//
void USER::PsiAttack(TCHAR *pBuf)
{

#ifdef _ACTIVE_USER
		//if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
		if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	DWORD	dwExp = 0;
	int		nDamage = 0;
	int		nTempHP = 0;

	USER*	pUser = NULL;
	CNpc*	pNpc = NULL;

	BYTE	tWeaponClass = 0;
	BOOL	bCanUseSkill = FALSE;
	int		bSuccessSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};

	int		nPsiRange = 0;
	int		nTPosX	= 0;
	int		nTPosY	= 0;
	int		nDist	= 100;
	short	sNeedPP	= 25000;
	BYTE	tPsiRegi = 0;
	DWORD	dwPsiCast = 0;

	DWORD	dwCurrTick = GetTickCount();

	BOOL	bPsiSuccess = FALSE;

	int index = 0;
	int nTargetID = GetInt(pBuf, index );	// Target ID 甫 掘绰促.
	BYTE byPsi = GetByte(pBuf, index);		// Psionic sid 甫 掘绰促.
	int nPsiX = -1;							// Teleport且 困摹
	int nPsiY = -1;
	CPoint ptPsi(-1, -1);

	if(byPsi < 0 || byPsi >= TOTAL_PSI_NUM) return;
	if(IsHavePsionic(byPsi) == FALSE) return;						// 家蜡窍瘤 臼篮 荤捞坷葱捞搁 府畔	

	//直接判断仲裁必杀不能针对自己
	if((byPsi == PSI_BISA_0||byPsi == PSI_BISA_1||byPsi == PSI_BISA_2||byPsi == PSI_BISA_3 ||byPsi == PSI_BISA_4)&& nTargetID == m_uid+USER_BAND)
	{
		return ;
	}

	if(byPsi == PSI_TELEPORT)
	{
		// Teleport 且 困摹甫 掘绰促.
		nPsiX = GetInt(pBuf, index);
		nPsiY = GetInt(pBuf, index);

		// 炮饭器飘肺 捞悼且 荐 绝绰 磊府捞搁 努扼捞攫飘俊辑 焊郴霖 谅钎 弊措肺 傈价
		CPoint pt = FindNearAvailablePoint_C(nPsiX, nPsiY); 
		if(pt.x < 0 || pt.y < 0) 
		{
			pt.x = nPsiX;
			pt.y = nPsiY;
		}

		ptPsi = ConvertToServer(pt.x, pt.y);
		nDist = abs(m_curx - ptPsi.x) + abs(m_cury - ptPsi.y);
		if(nDist > SCREEN_X + SCREEN_Y) return;
	}
	else	// 措惑俊 狼茄 眉农 (焊碍且 巴)
	{
		if(nTargetID < USER_BAND || nTargetID >= INVALID_BAND) return;	// 肋给等 Target 捞搁 return
	}

	
	// 荤沥芭府 拌魂 ------------------------------------------------------------------------//
	nPsiRange = GetPsiRange(byPsi);
	if(nPsiRange > 0)
	{
		if(nTargetID >= USER_BAND && nTargetID < NPC_BAND)	// USER
		{
#ifdef _ACTIVE_USER
	//if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if( m_iDisplayType == 6) return; //yskang 0.5
#endif
			pUser = GetUser(nTargetID - USER_BAND);	// User Pointer 甫 掘绰促.
			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;// 肋给等 USER 捞搁 府畔
			if(pUser->m_lDeadUsed == 1) return;
			if(pUser->m_bPShopOpen == TRUE) return;							// User has personal shop
			//必杀不能在城市中使用,新职业辐射能
			if(m_byClass == JUDGE&&(byPsi == PSI_BISA_4||byPsi == PSI_JUDGE_FU_SHE_LEN)&& IsCity())
			{
				if(!IsInPKC())
					return ;
			}
			if(byPsi == PSI_BISA_4||byPsi == PSI_JUDGE_FU_SHE_LEN)
			{
				//两个中是否红名
				if(!(m_bPkKeyDown||pUser->m_bPkStatus||m_bPkStatus))
					return ;
			}
//			if(pUser->m_bLive == USER_DEAD)	return;		// Target User 啊 捞固 磷绢乐栏搁 府畔
//			if(pUser->m_sHP == 0) return;
			if(byPsi==23 || byPsi==25){//拳和剑的必杀距离只能是2
				if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, (2 + 2) ) ) return;
			}
			// 泅犁 茄 拳搁俊 乐绰瘤 魄窜. 窜 滚叼蜡历老 版快 滴拳搁鳖瘤 啊瓷
			if( !CheckBuddyMember(pUser->m_strUserID) )
			{
				if(!IsInSight(pUser->m_curx, pUser->m_cury, pUser->m_curz)) return;			
			}
			else
			{
				if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, (SCREEN_X + SCREEN_Y) ) ) return;
				// 鸥百捞 滚叼蜡历老 版快 荤沥芭府绰 叼厚抛捞喉 公矫窍绊 2拳搁鳖瘤 啊瓷
				nPsiRange = (SCREEN_X + SCREEN_Y);
			}

			if(pUser->m_dwNoDamageTime != 0) 
			{
				if((dwCurrTick - pUser->m_dwLastNoDamageTime) > pUser->m_dwNoDamageTime)		// 公利矫埃捞 钱副 矫埃捞搁
				{
					pUser->m_dwNoDamageTime = 0;
					pUser->m_dwLastNoDamageTime = GetTickCount();
				}
				else
				{
					pUser->m_dwNoDamageTime = pUser->m_dwNoDamageTime - (dwCurrTick - pUser->m_dwLastNoDamageTime);
					pUser->m_dwLastNoDamageTime = dwCurrTick;
					return;	
				}
			}

			if(CheckBuddyMember(pUser->m_strUserID) || !IsPKZone(pUser))
			{
				if(g_arPsiTable[byPsi]->m_tTarget != 1 && g_arPsiTable[byPsi]->m_tTarget != 2) return;

#ifndef _EVENT_RR 
				// 肺凯 烦喉 甘俊辑 IsPKZone捞 FALSE甫 府畔沁绊
				// 促弗 加己俊 辑肺 辑乐绰 巴捞扼搁 葛电 荤捞坷葱捞 角菩促.
				if(pUser->m_curz == g_RR.m_iZoneNum || m_curz == g_RR.m_iZoneNum ) 
				{
					if( CheckInvalidMapType() != pUser->CheckInvalidMapType() ) return;
				}
#else		// 烙矫肺 弥碍 捞亥飘锭巩俊
				if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
				{
					if( CheckInvalidMapType() != pUser->CheckInvalidMapType() ) return;				
				}
				if( pUser->m_curz == g_RR.m_iZoneNum || pUser->m_curz == 61 || pUser->m_curz == 62 || pUser->m_curz ==  63)
				{
					if( CheckInvalidMapType() != pUser->CheckInvalidMapType() ) return;				
				}
#endif
			}
			nTPosX = pUser->m_curx;
			nTPosY = pUser->m_cury;
		}
		else if(nTargetID >= NPC_BAND)				// NPC
		{
			pNpc = GetNpc(nTargetID - NPC_BAND);	// NPC Point 甫 掘绰促.
			if(pNpc == NULL) return;				// 肋给等 NPC 捞搁 府畔
			if(pNpc->m_NpcState == NPC_DEAD) return;
			if(pNpc->m_sHP == 0) return;// NPC 啊 捞固 磷绢 乐栏搁 府畔
			if(pNpc->m_byAX == 0 && pNpc->m_byAZ == 0 && pNpc->m_tNpcType == 0) return;			// 傍拜瓷仿捞 绝绰 阁胶磐(泅犁 盔籍)篮 老馆 傍拜捞 登瘤 臼绰促.
//			if(pNpc->m_tGuildWar == GUILD_WAR_AFFTER && ((pNpc->m_tNpcType == NPCTYPE_GUILD_NPC) || (pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR))
//			{
//				return;
//			}
			if(byPsi==23 || byPsi==25){//拳和剑的必杀距离只能是2
				if( m_curz != pNpc->m_sCurZ || !GetDistance( pNpc->m_sCurX, pNpc->m_sCurY, (2 + 2) ) ) return;
			}

			if(!IsInSight(pNpc->m_sCurX, pNpc->m_sCurY, pNpc->m_sCurZ)) return;

			nTPosX = pNpc->m_sCurX;
			nTPosY = pNpc->m_sCurY;
		}
		else return;

		if(GetDistance(nTPosX, nTPosY, nPsiRange, &nDist) == FALSE)		// 荤捞坷葱 矫傈 芭府 观捞搁 府畔
		{
			//if(g_bDebug)SendSystemMsg(_T("### Too Long Distance!!!"), SYSTEM_NORMAL, TO_ME);
			return;
		}
	}

	// 公扁狼 努贰胶客 秦寸 努贰胶狼 胶懦阑 镜 荐 乐绰瘤 八荤 ---------------//
	bCanUseSkill = IsCanUseWeaponSkill(tWeaponClass);	

	// 咯蜡 PP 眉农 ---------------------------------------------------------//
	sNeedPP = GetNeedPP(bSuccessSkill, tWeaponClass, byPsi);
	if(sNeedPP > m_sPP) 
	{
		SendPsiAttackResult(FAIL, nTargetID, byPsi);
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PP, SYSTEM_NORMAL, TO_ME);
		return;
	}

	// Casting Delay 贸府
	dwPsiCast = g_arPsiTable[byPsi]->m_sDelayTime;		// 某胶泼 掉饭捞

	int iLevel = 0;
	int iDecCast = 0;
	int iSkillSid = 0;
	double dEBodyDec = 0;

	//dwPsiCast -= (int)((double)m_DynamicEBodyData[EBODY_WIS_TO_CAST_DOWN] / 100 * m_sMagicWIS);	// WIS 狼 % 甫 某胶泼 皑家肺
	//if(dwPsiCast < 0) dwPsiCast = 0;

	dEBodyDec = (double)m_DynamicEBodyData[EBODY_CASTING_DELAY_DOWN] / 100 ;				// EBODY 俊 狼茄 某胶泼 矫埃 皑家
	if(dEBodyDec >= 1) dEBodyDec = 0.0;

	if(tWeaponClass == STAFF)	
	{													// 某胶泼 鸥烙 皑家贸府		
		if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
		{
			if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_sCTime > 0)
			{
				dwPsiCast -= g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_sCTime;
				if(dwPsiCast < 0) dwPsiCast = 0;
			}
		}

		for(int i = tWeaponClass * SKILL_NUM; i < tWeaponClass * SKILL_NUM + SKILL_NUM; i++)
		{
			iSkillSid = m_UserSkill[i].sSid;

			if(iSkillSid == SKILL_CASTING_DOWN) // 某胶泼鸥烙皑家		//法术大师修复
			{
				// 鉴荐 胶懦 饭骇 	
				
                iLevel = m_UserSkill[i].tLevel;	
				if(iLevel < 0) iLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iSkillSid >= g_arSkillTable.GetSize()) break;
				if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				if(iLevel >= g_arSkillTable[iSkillSid]->m_arInc.GetSize()) return;
				if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
				//if(iLevel >= 30) return;				
				iDecCast = g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel);
			}
		}
	}
	
	if(iDecCast > 0 || dEBodyDec > 0)
	{
		dwPsiCast = (DWORD)( dwPsiCast * (1 - ((double)iDecCast / 100)- dEBodyDec ));
	}
	if(byPsi!=7 && dwPsiCast < 450) return;
	//释放速度
	if(dwPsiCast < 450) dwPsiCast =450;
	if(!(byPsi==23||byPsi==24||byPsi==25||byPsi==26||PSI_BISA_4 == byPsi))
	{
		if(byPsi==7)
         {
			 m_dwCastDelay = 0;
         }
         else{
		m_dwCastDelay = dwPsiCast;
             }
		if(CheckPsiCastingDelay() == FALSE ) return;
	}else
	{
		m_dwCastDelay = g_arPsiTable[byPsi]->m_sDelayTime;
		if(dwCurrTick-m_dwLastMaxAttack<m_dwCastDelay) return;
		m_dwLastMaxAttack=dwCurrTick;
		if(m_dwXP<=0){
			SendSystemMsg( "战斗经验不足无法使用必杀.", SYSTEM_ERROR, TO_ME);
			return;
		}
		m_dwXP--;
		SendXP();
	}


	
	//荤捞坷葱 己傍
	if(IsException(nTargetID, byPsi, ptPsi) == FALSE) return;

	BOOL bPsiOK = PsionicProcess(nTargetID, byPsi, ptPsi, sNeedPP);	
	if(bPsiOK)
	{
		if(byPsi == 44){
			SendPsiAttackResult(SUCCESS, nTargetID, (BYTE)190);
		}else if(byPsi == 45){
			SendPsiAttackResult(SUCCESS, nTargetID, (BYTE)304);
		}else if(byPsi == 81)//烈焰风暴
		{
			SendPsiAttackResult(SUCCESS, nTargetID, 81);		
		}else if(byPsi == 54)//战神技能
		{
			SendPsiAttackResult(SUCCESS, nTargetID, (BYTE)289);
		}else{
			SendPsiAttackResult(SUCCESS, nTargetID, byPsi);		
		}
		if(byPsi == 35) SendMyInfo( TO_INSIGHT, INFO_MODIFY );		// PP 皑家 --------------------------------------------------------//
		m_sPP -= sNeedPP;
		//发送狮吼的..
		SendPP();
		m_dwLastPsiAttack = GetTickCount();
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 傍拜 搬苞俊 措茄 抗寇 惑炔阑 包府茄促.(抗 : 鸥款 器呕篮 坷流 蜡历父等促.)
//
BOOL USER::IsException(int iTargetID, BYTE tPsi, CPoint ptPsi)
{	
	USER *pUser = NULL;
	CNpc* pNpc = NULL;

	BOOL bUser = FALSE;
	BOOL bMon = FALSE;
	int type = 0, index = 0;

	if(iTargetID >= USER_BAND && iTargetID < NPC_BAND)	// USER
	{
		pUser = GetUser(iTargetID - USER_BAND);	// User Pointer 甫 掘绰促.
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;
		if(pUser->m_bLive == USER_DEAD)	return FALSE;		// Target User 啊 捞固 磷绢乐栏搁 府畔
		bUser = TRUE;
	}
	else if(iTargetID >= NPC_BAND)						// NPC
	{
		pNpc = GetNpc(iTargetID - NPC_BAND);		// NPC Point 甫 掘绰促.
		if(pNpc == NULL) return FALSE;				// 肋给等 NPC 捞搁 府畔
		if(pNpc->m_NpcState == NPC_DEAD) return FALSE;// NPC 啊 捞固 磷绢 乐栏搁 府畔
		bMon = TRUE;
	}
	else return FALSE;

	switch((int)tPsi)
	{
	// 拜捧拌凯 -------------------------------------------------------------------------------//
	case 0:	//Recovery	: SP 雀汗
		if(bUser) 
		{
			if(pUser->m_dwMindShockTime != 0) return FALSE;	// Mind Shock 惑怕俊辑绰 酒公 荤捞坷葱档 救吧赴促.
		}
		if(bMon) return FALSE;
		break;

	case 1:	
		if(bUser) 
		{
			if( pUser->m_dwDexUpTime != 0 || pUser->m_dwShieldTime != 0 || pUser->m_dwMaxHPUpTime != 0 || 
				pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE;	// 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		if(bMon) return FALSE;
		break;

	case PSIONIC_FAST_RUN:	// 捞悼加档 氢惑
		if(bUser) 
		{
			if( pUser->m_dwDexUpTime != 0 || pUser->m_dwShieldTime != 0 || pUser->m_dwMaxHPUpTime != 0 || 
				pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE;	
		}
		if(bMon) return FALSE;

	// 瘤莆捞 拌凯 ----------------------------------------------------------------------------//
	case 3:	// 鳃傅
		if(bUser) 
		{
			if(pUser->m_dwMindShockTime != 0) return FALSE;	// Mind Shock 惑怕俊辑绰 酒公 荤捞坷葱档 救吧赴促.
		}
		if(bMon) return FALSE;
		
		break;

	case 6:	// 蒋靛
		if(bUser) 
		{
			if( pUser->m_dwDexUpTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwHasteTime != 0 || pUser->m_dwMaxHPUpTime != 0 || pUser->m_dwMindShockTime != 0 ||
				pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwFastRunTime != 0 || pUser->m_dwPsiShieldTime != 0) return FALSE;	// 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		if(bMon) return FALSE;
		break;

	case 7: // 炮饭器飘 
/*
		type = CheckInvalidMapType();
//		type = ((g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_dwType & 0xFF00 ) >> 8);		
//		if(type > 1 && type < g_arMapTable.GetSize())		// 档矫甫 哈绢唱搁...
		if(type > 1 && type < 8)
		{
			index = g_arMapTable[type]->m_sStoreIndex;
			if(index < 0 || index >= g_arStore.GetSize()) return TRUE;

			if(g_arStore[index]->m_lUsed == 1)	return FALSE;  // 秦寸 加己 甘捞 辨靛傈 惑怕	
		}

*/
		type = CheckInvalidMapType();
		if(type == 8 || type == 10 || type == 15 || type == 17 || type == 9 || type == 11 || type == 16 || type == 18) 
		{
				SendEventMsg( "要塞战区内无法使用瞬移魔法.");
				return FALSE;
		}
		if(m_dwLhslTime != 0 && (!IsCity()))
		{
				SendEventMsg( "灵魂锁链状态下(5秒)无法使用瞬移魔法.");
				return FALSE;
		}
		if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
		if(ptPsi.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptPsi.x < 0) return FALSE;
		if(ptPsi.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy || ptPsi.y < 0) return FALSE;

		type = ((g_zone[m_ZoneIndex]->m_pMap[ptPsi.x][ptPsi.y].m_dwType & 0xFF00 ) >> 8);

		if(!CheckInvalidZoneState(type)) return FALSE;

		if(bUser) 
		{
			if(pUser->m_dwMindShockTime != 0) return FALSE;	// Mind Shock 惑怕俊辑绰 酒公 荤捞坷葱档 救吧赴促.
		}
		if(bMon) return FALSE;
		break;

	case PSIONIC_MIND_SHOCK:
		if(bUser) 
		{
			if(pUser->m_dwMindGuardTime != 0) return FALSE;	// Mind Guard 惑怕俊辑绰 Mind Shock 甫 吧荐 绝促.
		}
		else return FALSE;

		break;

	// 档八拌凯 ------------------------------------------------------------------------------//
	case 9:	// Ecasion	: 鉴埃雀乔
/*		type = CheckInvalidMapType();
//		type = ((g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_dwType & 0xFF00 ) >> 8);
//		if(type > 1 && type < g_arMapTable.GetSize())		// 档矫甫 哈绢唱搁...
		if(type > 1 && type < 8)
		{
			index = g_arMapTable[type]->m_sStoreIndex;
			if(index < 0 || index >= g_arStore.GetSize()) return TRUE;

			if(g_arStore[index]->m_lUsed == 1)	return FALSE;  // 秦寸 加己 甘捞 辨靛傈 惑怕	
		}
*/
		type = CheckInvalidMapType();
		if(!CheckInvalidZoneState(type)) return FALSE;

		if(bUser) 
		{
			if(pUser->m_dwMindShockTime != 0) return FALSE;	// Mind Shock 惑怕俊辑绰 酒公 荤捞坷葱档 救吧赴促.
		}
		if(bMon) return FALSE;
		break;

	case PSIONIC_PSI_SHIELD: // 荤捞坷葱 蒋靛
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0	|| pUser->m_dwMindShockTime != 0 || pUser->m_dwDexUpTime != 0 || 
				pUser->m_dwMaxHPUpTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwFastRunTime != 0) return FALSE; // 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		else return FALSE;

		break;

	case 30: // 荤捞坷葱 蒋靛
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0	|| pUser->m_dwMindShockTime != 0 || pUser->m_dwDexUpTime != 0 || 
				pUser->m_dwMaxHPUpTime != 0 || pUser->m_dwPsiShieldTime != 0|| pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwFastRunTime != 0) return FALSE; // 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		if(bMon) return FALSE;

		break;

	// 醚扁 -------------------------------------------------------------------------------------------//
	case 11: // Dex Up
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0	|| pUser->m_dwMindShockTime != 0 || 
				pUser->m_dwFastRunTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0) return FALSE; // 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		if(bMon) return FALSE;
		break;
	case 12: // Max HP Up
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0 || pUser->m_dwMindShockTime != 0 ||
				pUser->m_dwFastRunTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0) return FALSE; // 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		if(bMon) return FALSE;
		break;

	case PSIONIC_PIERCING_SHIELD:	// 乔绢教角靛
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0	|| pUser->m_dwMindShockTime != 0 || 
				pUser->m_dwFastRunTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0) return FALSE; // 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		else return FALSE;

		break;

	// 公拌凯 -----------------------------------------------------------------------------------------//
	case 13: // Adamantine
		if(bUser) 
		{
			if( pUser->m_dwMightyWeaponTime != 0 || pUser->m_dwBerserkerTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE; // 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		if(bMon) return FALSE;
		break;
	case 14: // Mighty Weapon
		if(bUser) 
		{
			if( pUser->m_dwAdamantineTime != 0 || pUser->m_dwBerserkerTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE; // 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		if(bMon) return FALSE;
		break;
	case 15: // Berserker
		if(bUser) 
		{
			if( pUser->m_dwAdamantineTime != 0 || pUser->m_dwMightyWeaponTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE; // 捞固 促弗 瓤苞啊 乐栏搁 利侩救凳
		}
		if(bMon) return FALSE;
		break;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 傍拜 搬苞甫 傈价茄促.
//
void USER::SendPsiAttackResult(BYTE result, int tuid, BYTE psi)
{
	USER*		pUser;
	CBufferEx TempBuf;

	if(m_tIsOP == 1) return;					// 款康磊 老锭绰 荤捞坷葱惑怕啊 函窍搁 救等促. (捧疙捞 钱妨辑...)

	pUser = NULL;

	// E-Body Skills
	if(psi == 23 || psi == 25 || psi == 26||PSI_BISA_4 == psi)
	{
		TempBuf.Add(ATTACK_RESULT);
		if(result != FAIL)
			TempBuf.Add((BYTE)0x06); // Ebody Attack
		else
			TempBuf.Add(result); // Ebody Attack
	}
	else
	{
		TempBuf.Add(PSI_ATTACK_RESULT);
		TempBuf.Add(result);
	}
	if(result == FAIL) 
	{
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	if(psi == 23  || psi == 25 || psi == 26||PSI_BISA_4 == psi)
	{
		;
	}
	else
	{
		TempBuf.Add(psi);				// Psionic sid
	}

	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(tuid);

	if(psi == 23 || psi == 24 || psi == 25 || psi == 26||PSI_BISA_4 == psi)
	{
		if(tuid >= NPC_BAND)
		{
			CNpc *pNpc;
			pNpc = GetNpc(tuid - NPC_BAND);
			//国服需要使用short -->int
			TempBuf.Add((int)pNpc->m_sHP);
			TempBuf.Add((int)pNpc->m_sMaxHP);
		}
		else if(tuid >= USER_BAND && tuid < NPC_BAND)
		{
			USER *pUser = NULL;
			pUser = GetUser(tuid - USER_BAND);
			TempBuf.Add((int)pUser->m_sHP);
			TempBuf.Add((int)pUser->m_sMagicMaxHP);
		}

	}

	// 弥碍 捞亥飘锭巩俊 款康评捞 夸没 02-11-1 
	if(m_curz == 61 || m_curz == 62 || m_curz == 63)
	{
		SendInsight(TempBuf, TempBuf.GetLength());
	}
	else
	{
		if(tuid >= USER_BAND && tuid < NPC_BAND)	// USER
			pUser = GetUser(tuid - USER_BAND);

		if( pUser != NULL )
		{
			if( ((pUser->m_curx < m_curx-16 && pUser->m_curx >= m_curx+16
				&& pUser->m_cury < m_cury-16 && pUser->m_cury >= m_cury+16)
				|| (( abs( m_curx - pUser->m_curx ) + abs( m_cury - pUser->m_cury ) ) > 16))
				&& pUser->m_curz == m_curz )
				pUser->Send(TempBuf, TempBuf.GetLength());
		}

		SendExactScreen(TempBuf, TempBuf.GetLength());	// 02-10-28 by Youn Gyu
	}
//	SendInsight(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////
//	傍拜贸府
//攻击
void USER::Attack(TCHAR* pBuf)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	if( m_UserItem[RIGHT_HAND].sSid == 669 || m_UserItem[RIGHT_HAND].sSid == 670 )		// 坷弗颊俊 盲笼侩 酒捞袍阑 厘馒沁促搁
	{
		Collect( pBuf );
		return;
	}

	USER*	pUser	= NULL;
	CNpc*	pNpc	= NULL;
	int		nTPosX	= 0;
	int		nTPosY	= 0;
	int		nAttackRange = 0;
	int		nDist	= 100;

	int Damage = 0;	//用于群攻伤害值变量

	int		nDamage = 0;
	int		nDefense = 0;
//	double	nIronSkin = 0;
	int		nFinalDamage = 0;

	BYTE	tWeaponClass = 0;
	BYTE	tTargetClass = 0;

	BOOL	bCanUseSkill = FALSE;
	int		iRandom = 100;

	int		nHit	= 0;
	int		nAvoid	= 0;
//	int		nWeaponHit	= 0;

	BOOL	bIsHit	= FALSE;
	double	nVCHit	= 0;
	double	nCGuard	= 0;
	BOOL	bIsCritical = FALSE;

	int		nInitDamage = 0;
	
	short	sItemDefense[4] = {0, 0, 0, 0};

	BOOL	bSuccessSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};
	BOOL	bTargetSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};
	
	BYTE	tActionIndex = HIT_NONE;
	BYTE	tNpcClass = 0;

	short	sOldDuration = 0, sNewDuration = 0;
	short	sOldBullNum = 0;

	DWORD	dwCurrTick = GetTickCount();

	int		iFireErrDec = 0;
//	double	determine = 0;
//	int iAttackDex = 0, iDefenseDex = 0;
	int	determine = 0;
	int iDexHitRate = 0, iLevelHitRate = 0;
	int iMyDex = 0, iYourDex = 0;

	CByteArray	arSkillAction1, arSkillAction2;							// Skill Action Array

	DWORD dwExp = 0;
	int index = 0;
	int nTargetID = GetShort(pBuf, index);								// 取得ServerID

	if(nTargetID < USER_BAND || nTargetID >= INVALID_BAND) return;		// 如果ServerID是错误 返回

	//判断玩家使用57类型//
	bCanUseSkill = IsCanUseWeaponSkill(tWeaponClass);	

	if(bCanUseSkill)//如果有57则技能生效
	{
		IsSkillSuccess(bSuccessSkill, tWeaponClass);					//查看技能是否成功
	}

	if(CheckAttackDelay(bSuccessSkill, tWeaponClass) == FALSE)			// Attack Delay Check
	{
//		SendAttackFail(ERR_SHORT_ATTACK_DELAY);
		return;
	}

	// 57耐久 -------------------------------------------------------------//
	if(tWeaponClass != 255)	// 盖颊捞 酒聪搁
	{
		if(m_UserItem[RIGHT_HAND].sDuration <= 0)
		{
//			SendAttackFail(ERR_BROKEN);
			SendSystemMsg( IDS_USER_ITEM_DAMAGED, SYSTEM_NORMAL, TO_ME);
			return;
		}
	}

	// Target 苞狼 芭府拌魂 ----------------------------------------------------//
	if(nTargetID >= USER_BAND && nTargetID < NPC_BAND)	//如果攻击是玩家
	{
		pUser = GetUser(nTargetID - USER_BAND);				// User Pointer 甫 掘绰促.
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;// 肋给等 USER 捞搁 府畔
		if(pUser->m_bLive == USER_DEAD || pUser->m_sHP == 0) return;	// Target User 啊 捞固 磷绢乐栏搁 府畔
		if(pUser->m_bPShopOpen == TRUE) return;							// User has personal shop
		if(CheckBuddyMember(pUser->m_strUserID)) return;	// 鞍篮 滚叼盔捞搁 角菩
		if(!m_MItemLock  && (!IsCity()) && o_yehuoini[0]->mimabaohu == 1){ SendSystemMsg( "解除[安全锁]后,才能攻击.", SYSTEM_ERROR, TO_ME);return;}  ///交易密码
		if((m_dwGuild == pUser->m_dwGuild) && (!IsCity()) && m_Bmws==1 ){ SendSystemMsg( "对方与您同属一个军团,请在内挂关闭避免误伤功能", SYSTEM_ERROR, TO_ME);return;}  ///避免误伤

	//	if(pUser->m_dwBHTime !=0 ) { SendEventMsg( "对方闭关保护中,无法攻击!.");return;}
	//	if (m_dwBHTime !=0)  { SendEventMsg( "您当前是在闭关保护中无法攻击玩家!."); return;}
		
        if(pUser->m_bSessionOnline == true && (!IsCity()))	return;
		if(m_UserItem[2].sSid == 23)
		{
				TSFengHao(pUser); //
                return;
		}

		if(!IsPKZone(pUser)) { SendAttackMiss(nTargetID); return; }	// PK粮捞 酒聪骨肺 轿匡荐绝促.
		
		if(pUser->m_dwNoDamageTime != 0)		
		{
			if((dwCurrTick - pUser->m_dwLastNoDamageTime) > pUser->m_dwNoDamageTime)		// 公利矫埃捞 钱副 矫埃捞搁
			{
				pUser->m_dwNoDamageTime = 0;
				pUser->m_dwLastNoDamageTime = GetTickCount();
			}
			else
			{
				pUser->m_dwNoDamageTime = pUser->m_dwNoDamageTime - (dwCurrTick - pUser->m_dwLastNoDamageTime);
				pUser->m_dwLastNoDamageTime = dwCurrTick;
				return;	
			}
		}

		nTPosX = pUser->m_curx;
		nTPosY = pUser->m_cury;
	}
	else if(nTargetID >= NPC_BAND)				//如果攻击是怪物
	{
		pNpc = GetNpc(nTargetID - NPC_BAND);	// NPC Point 甫 掘绰促.
		if(pNpc == NULL) return;				// 肋给等 NPC 捞搁 府畔
		if(pNpc->m_NpcState == NPC_DEAD || pNpc->m_sHP <= 0)
		{
//			pNpc->Dead(); 
//			pNpc->m_Delay = pNpc->SendDead(m_pCom, 0);//空手攻击控制
			return;// NPC 啊 捞固 磷绢 乐栏搁 府畔
		}
		
		

		if(pNpc->m_byAX == 0 && pNpc->m_byAZ == 0 && pNpc->m_tNpcType == 0) return;			// 傍拜瓷仿捞 绝绰 阁胶磐(泅犁 盔籍)篮 老馆 傍拜捞 登瘤 臼绰促.

		nTPosX = pNpc->m_sCurX;
		nTPosY = pNpc->m_sCurY;
	}

	nAttackRange = GetAttackRange(m_UserItem[RIGHT_HAND].sSid);//得到射程
	nAttackRange += 1;	// 拱府利牢 鸥拜阑啊且锭父 +1甫 秦霖促.(捞悼 加档锭巩俊 各阑 棱瘤 给秦辑 焊沥栏肺...)
	if(tWeaponClass == FIREARMS)
	{
		nAttackRange += m_DynamicUserData[MAGIC_RANGE_UP];//射程增加 
	}

	if(GetDistance(nTPosX, nTPosY, nAttackRange, &nDist) == FALSE)		// 傍拜啊瓷芭府 观捞搁 府畔
	{
		if(g_bDebug) SendSystemMsg(_T("距离太远了"), SYSTEM_NORMAL, TO_ME);
		return;
	}

	//是否还有能量包数-------------------------------------//
	if(tWeaponClass == EDGED || tWeaponClass == FIREARMS||tWeaponClass == JUDGE)
	{
		if(m_UserItem[RIGHT_HAND].sBullNum <= 0)
		{
			if(tWeaponClass == EDGED)
				SendSystemMsg( IDS_USER_SWORD_PACK_EMPTY, SYSTEM_NORMAL, TO_ME);
			else if(tWeaponClass == FIREARMS)
				SendSystemMsg( IDS_USER_BULLET_EMPTY, SYSTEM_NORMAL, TO_ME);
			else if(tWeaponClass == JUDGE)	//仲裁
				SendSystemMsg( "你的武器已经没有脉冲能量", SYSTEM_NORMAL, TO_ME);
			return;
		}
		else SetDecBullNum(RIGHT_HAND, 1);//攻击成功 能量包数减1
	}

	// 取得玩家命中-------------------------------------------------------//
	nHit = GetHitRate(m_UserItem[RIGHT_HAND].sSid, tWeaponClass);
	if(nHit < 0) nHit = 0;


	// 雀乔蔼/疙吝魄沥/单固瘤 拌魂/郴备档 拌魂/弥辆 傍拜矫埃 悸泼----------//
	if(nTargetID >= USER_BAND && nTargetID < NPC_BAND)	// USER
	{
		pUser = GetUser(nTargetID - USER_BAND);	// User Pointer 甫 掘绰促.
		
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;// 肋给等 蜡历捞搁 府畔
		if(pUser->m_bLive == USER_DEAD || pUser->m_sHP == 0) return;	// Target User 啊 捞固 磷绢乐栏搁 府畔

		// 位置是不是在一起? 能不能打到?
		if(!IsInSight(pUser->m_curx, pUser->m_cury, pUser->m_curz)) return;
		
		//取得被打玩家回避
		nAvoid = pUser->GetAvoid();
		
		// 疙吝咯何 魄窜
		iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5); //促剧己阑 焊厘窍扁 困秦(1, 100)
		
//		iDefenseDex = pUser->m_sMagicDEX;
//		iAttackDex = m_sMagicDEX;	
//		determine = 200 * ((double)iAttackDex / (iAttackDex + iDefenseDex)) * ((double)m_sLevel / (m_sLevel + pUser->m_sLevel));

		if(m_dwDexUpTime) 
			iMyDex = m_sMagicDEX + 10;
		else
			iMyDex = m_sMagicDEX;

		if(pUser->m_dwDexUpTime) iYourDex = pUser->m_sMagicDEX + 10;
		else iYourDex = pUser->m_sMagicDEX;

		iDexHitRate = (int)( 30.0 * ( (double)iMyDex/(iMyDex + iYourDex) ) + 15.0 );
		iLevelHitRate = (int)( 70.0 * ( (double)m_sLevel/(pUser->m_sLevel + m_sLevel) ) + 15.0 );

		determine = iDexHitRate + iLevelHitRate + nHit + m_Hit - (nAvoid+pUser->m_Avoid);

//		index = (int)(determine + nHit - nAvoid);

		if(determine < ATTACK_MIN) determine = ATTACK_MIN;			// 弥家
		else if(determine > ATTACK_MAX) determine = ATTACK_MAX;		// 弥措
        
		iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
		if(iRandom < determine)	bIsHit = TRUE;		// 疙吝
		else bIsHit = FALSE;

		//没有命中...发送攻击无效...
		if(bIsHit == FALSE)					
		{
			SendAttackMiss(nTargetID);
			return;
		}

		//计算防 ----------------------------------------------------------------//
		nDamage = GetNormalInitDamage(tWeaponClass, pUser, bIsCritical);//取得玩家的防.
		if(nDamage < 0) nDamage = 0;
		if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) nDamage += 10;		// 惑措祈捞 趁扁 捞惑捞搁 单固瘤 眠啊

		// 弥辆单固瘤
		nFinalDamage = GetFinalDamage(pUser, nDamage, tWeaponClass,bIsCritical);
		//nFinalDamage = nFinalDamage +m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP];//最终伤害增加
		//nFinalDamage = nFinalDamage-(pUser->m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+pUser->m_DynamicUserData[MAGIC_PHY_ATTACK_DOWN]);//最终伤害减少+物理伤害减少
     //   nFinalDamage = nFinalDamage +m_DynamicMagicItem[7];//最终伤害增加
	//	nFinalDamage=nFinalDamage-(pUser->m_DynamicMagicItem[5]+pUser->m_DynamicMagicItem[6]);
		if (m_byClass == 3)
			nFinalDamage = (int)(nFinalDamage * 0.5); 
		else
			nFinalDamage = (int)(nFinalDamage * 0.9);

		if(nFinalDamage < 0) nFinalDamage = 15;		// 规绢 内靛

		sNewDuration = pUser->SendDamagedItem(nFinalDamage); // 惑措规 郴备档 皑家甫 焊辰促. 

		// 傍拜螟 郴备档/饭萍唇蒲 皑家
		if(tWeaponClass != 255)	// 盖颊捞 酒囱版快
		{
			// 郴备档
			sOldDuration = m_UserItem[RIGHT_HAND].sDuration;
			sNewDuration = (int)myrand(1,10);
			if(tWeaponClass != FIREARMS) 
				SendDuration(RIGHT_HAND, sNewDuration);
			//if(tWeaponClass != FIREARMS) m_UserItem[RIGHT_HAND].sDuration -= sNewDuration;
			else
			{
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
				{
					if(iRandom < g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byErrorRate)
					{
						//iFireErrDec = 10 - GetBreakDec();
						iFireErrDec=4;
						if(iFireErrDec < 0) iFireErrDec = 0;

						//m_UserItem[RIGHT_HAND].sDuration -= iFireErrDec;
						SendDuration(RIGHT_HAND, iFireErrDec);
					}
				}
			}

//			if(m_UserItem[RIGHT_HAND].sDuration < 0) m_UserItem[RIGHT_HAND].sDuration = 0;
//			if(sOldDuration != m_UserItem[RIGHT_HAND].sDuration) SendDuration(RIGHT_HAND);
		}


	//	if (m_byClass == JUDGE )
	//	{
	//		nFinalDamage -= pUser->m_sLevel*2;
	//		if (nFinalDamage<0)nFinalDamage=15;
	//	}
	//m_DynamicUserData[MAGIC_PHY_ATTACK_DOWN]+= m_sLevel* 2;

		BOOL bIsLegal = IsLegalDefence_isdead(pUser);		// 老窜 傍拜窍搁 沥寸规困牢瘤 魄窜//yskang 0.7
		pUser->SetDamage(nFinalDamage);
		//SendDamageNum(0,nTargetID,nFinalDamage);   
		SendDamageNum(0,pUser->m_uid + USER_BAND,(short)nFinalDamage);//显血修改处
		pUser->SendDamageNum(0,m_uid+USER_BAND,(short)nFinalDamage);
        //if(m_ShowHP==1){
			CString strMsg;
			int iMaxHP=pUser->m_sMaxHP;
			if(m_sMagicMaxHP > m_sMaxHP) iMaxHP = pUser->m_sMagicMaxHP;
			strMsg.Format("%s(%d) HP= %d \\ %d",pUser->m_strUserID,pUser->m_uid + USER_BAND,pUser->m_sHP,iMaxHP);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		
		//}
	
	  
	  //if(pUser->m_bLive == FALSE)
		if(pUser->m_lDeadUsed == 1)
		{
			if ( m_curz == 66)
			{
				m_dwAutoMoney += 1;
				CString strMsg = _T("");
				if (m_dwAutoMoney <= 20)
				{
					//GiveDN(200000);
					GiveItemAll(724,10,0,0,0,0,0,0,0,0,0,0,0);
				//	SendMoneyChanged();
					UpdateUserItemDN();
					strMsg.Format( "当前杀人数是:[ %d ]个.成功击杀1玩家,奖励标志10个", m_dwAutoMoney);
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NPC, TO_ME);
				}
				else
				{
					strMsg.Format( "当前杀人数是:[ %d ]个.", m_dwAutoMoney);
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NPC, TO_ME);
				}
			}
			
			if(m_tBabyCall ==1)//如果有宠物
                {
					if ( m_curz == 66 || m_curz == 67 || IsInPKC()) return;//活动地图不加忠诚度
                    if(m_sFealty != 100)//忠诚度不是满的
					{
                        if((m_sFealty + 1) < 101) m_sFealty += 1;
						SendEventMsg("成功击杀1名玩家,宠物[忠诚度]增加[ 1 ]");
						SendBabyInfo();
					}  
				}
			
			CString strMsg = _T("");

			strMsg.Format( IDS_USER_ATTACK_FROM, m_strUserID,pUser->m_strUserID);
            int type = 0;
        	type = CheckInvalidMapType();
			if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING) //只有审请军团才掉市民等级	
        	if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16)  bIsLegal = TRUE; 
			int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
			if(bIsLegal == FALSE)//yskang 0.7
				IsChangeCityRank(m_sCityRank, pUser);
			pUser->GetLevelDownExp(USER_PK, tempRank, FALSE, m_strUserID);					// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
			
			if(m_curz != 40 && m_curz != 45 && m_curz != 58) {pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ALL);}
///////////////////////////杀人电光提醒/////////////////////////////////////////////////
	//		    char sendMsg[200] = "";
	////			strcat_s(sendMsg,"PK中, ");
	//			strcat_s(sendMsg,m_strUserID);
	//			strcat_s(sendMsg," 杀害了 ");
	//			strcat_s(sendMsg,pUser->m_strUserID);								
	//			CBufferEx	TempBuf;
	//			TempBuf.Add((byte)0x1f);
	//			TempBuf.Add((byte)1);
	//			TempBuf.Add((byte)0x25);
	//			TempBuf.Add((DWORD)0);
	//			TempBuf.AddString("PK战报");
	//			TempBuf.AddString(sendMsg);
	//			SendAll(TempBuf, TempBuf.GetLength());	
///////////////////////////杀人电光提醒//////////////////////////////////////////////////
		}

		// 傍拜 搬苞 傈价 ------------------------------------------------------------//
		//yskang 0.3 SendAttackSuccess(nTargetID, arSkillAction1, arSkillAction2, pUser->m_sHP, pUser->m_sMagicMaxHP);
		SendAttackSuccess(nTargetID,bIsCritical, pUser->m_sHP, pUser->m_sMagicMaxHP);//yskang 0.3
		m_dwLastAttackTime = GetTickCount();

/*		//yskang 0.4 款康磊 肺弊 扁废窍扁
		TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
		sprintf(strOP,"PKINFO:ACCOUNT%s : CharID%s",pUser->m_strAccount, pUser->m_strUserID);
		WriteOpratorLog(strOP,PKINFO_LOG);
*/
		return;
	}

	if(nTargetID >= NPC_BAND)				// NPC
	{
		pNpc = GetNpc(nTargetID - NPC_BAND);		// NPC Point 甫 掘绰促.
		
		if(pNpc == NULL) return;					// 肋给等 NPC 捞搁 府畔
		if(pNpc->m_NpcState == NPC_DEAD) return;
		if(pNpc->m_sHP == 0) return;

		if(pNpc->m_tNpcType == NPCTYPE_GUARD)					// 版厚捍捞搁 鸥百阑 秦寸 蜡历肺
		{
			pNpc->m_Target.id = m_uid + USER_BAND;
			pNpc->m_Target.x = m_curx;
			pNpc->m_Target.y = m_cury;
			pNpc->m_Target.failCount = 0;
			pNpc->Attack(m_pCom);

			return;
		}

		// 泅犁 茄 拳搁俊 乐绰瘤 魄窜
		if(!IsInSight(pNpc->m_sCurX, pNpc->m_sCurY, pNpc->m_sCurZ)) return;

		if(m_dwDexUpTime) iMyDex = m_sMagicDEX + 10;
		else iMyDex = m_sMagicDEX;

		iDexHitRate = (int)( 30.0 * ( (double)iMyDex/(iMyDex + pNpc->m_sDEX) ) + 15.0 );
//		iDexHitRate = (int)( 30.0 * ( (double)m_sMagicDEX/(m_sMagicDEX + pNpc->m_sDEX) ) + 15.0 );
		iLevelHitRate = (int)( 70.0 * ( (double)m_sLevel/(pNpc->m_byClassLevel + m_sLevel) ) + 15.0 );

		determine = iDexHitRate + iLevelHitRate + nHit + m_Hit;

		if(determine < ATTACK_MIN) determine = ATTACK_MIN;			// 弥家
		else if(determine > ATTACK_MAX) determine = ATTACK_MAX;		// 弥措

		iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5); //促剧己阑 焊厘窍扁 困秦(1, 100)

		if(iRandom < determine)	bIsHit = TRUE;		// 疙吝
		else bIsHit = FALSE;

		pUser = GetUser(m_uid);

		if(pNpc->m_tNpcType == NPCTYPE_GUILD_GUARD)	pNpc->ChangeTarget(pUser, m_pCom);

		// 傍拜 固胶
		if(bIsHit == FALSE)					
		{
			SendAttackMiss(nTargetID);
			return;
		}
		// NPC 规绢蔼 
		nDefense = pNpc->GetDefense();

		
		//yskang 0.3 nDamage = GetNormalInitDamage(tWeaponClass, NULL);// 檬扁 措固瘤
		nDamage = GetNormalInitDamage(tWeaponClass, NULL,bIsCritical);//yskang 0.3 檬扁 措固瘤
		if(nDamage < 0) nDamage = 0;

		
		if(bIsHit == TRUE)
		{
			nVCHit = 0.0;
			nVCHit = GetCriticalHit(tWeaponClass,0);
			if(nVCHit > 0.0) bIsCritical = TRUE; //yskang 0.3

			nFinalDamage = (int)((double)nDamage - ((double)nDefense * nVCHit));
			if(m_byClass == FIREARMS) // 醚扁率 单固瘤 皑家 - 烙矫内靛  
			{
				double dMin = (double)myrand(90, 100) / 100;
				nFinalDamage = (int)((double)nFinalDamage * dMin);
			}
			
			if(nFinalDamage < 0) nFinalDamage = 15;
		}
		
		// 傍拜螟 郴备档/饭萍唇蒲 皑家
		if(tWeaponClass != 255)	// 盖颊捞 酒囱版快
		{
			// 郴备档
			sNewDuration = (int)myrand(1,10);
			sOldDuration = m_UserItem[RIGHT_HAND].sDuration;

			if(tWeaponClass != FIREARMS) 
				SendDuration(RIGHT_HAND, sNewDuration);
			//if(tWeaponClass != FIREARMS) m_UserItem[RIGHT_HAND].sDuration -= sNewDuration;
			else
			{
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
				{
					if(iRandom < g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byErrorRate)
					{
						//iFireErrDec = 10 - GetBreakDec();
						iFireErrDec=4;
						if(iFireErrDec < 0) iFireErrDec = 0;

						//m_UserItem[RIGHT_HAND].sDuration -= iFireErrDec;
						SendDuration(RIGHT_HAND, iFireErrDec);
					}
				}
			}

//			if(m_UserItem[RIGHT_HAND].sDuration < 0) m_UserItem[RIGHT_HAND].sDuration = 0;
//			if(sOldDuration != m_UserItem[RIGHT_HAND].sDuration) SendDuration(RIGHT_HAND);
		}

		// Calculate Target HP -------------------------------------------------------//
		short sOldNpcHP = pNpc->m_sHP;
//发送对怪物伤害数值
		
	//	CString strMsg;
	//	strMsg.Format("您产生攻击是 %d,您对 %s 最终伤害是 %d",nDamage,pNpc->m_strName,nFinalDamage);
	//	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		nFinalDamage = nFinalDamage +m_DynamicMagicItem[7];
		//加上140级杀怪技能
		nFinalDamage += m_Dynamic140[0];
//		if(pNpc->SetDamage(nFinalDamage, m_strUserID, m_uid + USER_BAND, m_pCom) == FALSE)
		if(pNpc->SetDamage(nFinalDamage, m_uid + USER_BAND, m_pCom) == FALSE)
		{
			pNpc->SendExpToUserList(m_pCom); // 版氰摹 盒硅!!
			int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel); //怪物等级相差30级没经验
			if(diffLevel <= o_yehuoini[0]->djxz || pNpc->m_sEvent >= 30000 )
			{
				if (m_sLevel>= 70)
				m_dwShopPingDN += 1;//每杀死怪物增加杀怪物数
				if (m_isDoubleBAOLV > 0 || m_dwGuarDianTianShi > 0)
				{
					pNpc->SendDead(m_pCom,1,TRUE);
					 Diaoluo();
			    }
				else
				{
					pNpc->SendDead(m_pCom);
	             Diaoluo();		
				}
			}
			else
			{
				pNpc->SendDead(m_pCom,0);
				SendSystemMsg("您和当前怪物等级相差20,没有任何经验和道具掉落", SYSTEM_ERROR,TO_ME);
			}

			if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
			{
				CheckGuildHouseWarEnd();
			}
			else
			{
				if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(diffLevel < 40)
				{
					m_iCityValue=m_iCityValue+250;
					GetCityRank();
					CheckMaxValue(m_dwXP, 1);		// 各捞 磷阑锭父 1 刘啊!	
					SendXP();
					SendUserStatusSkill();
				}
			}
		}
		SendDamageNum(1,nTargetID,nFinalDamage);
        SendNpcHP(nTargetID,pNpc->m_sHP);
 
		

///////////////////////////物理群攻触发开始//////////////////////////////////////////////////////////	
	/*	if((m_UserItem[4].sSid == 1572) || (m_UserItem[4].sSid == 1573) || (m_UserItem[4].sSid == 1574) || 
			(m_UserItem[4].sSid == 1575) || (m_UserItem[4].sSid == 1576) || (m_UserItem[4].sSid == 1577) || 
			(m_UserItem[4].sSid == 1581) || (m_UserItem[4].sSid == 1582) || (m_UserItem[4].sSid == 1583) || 
			(m_UserItem[4].sSid == 1584) || (m_UserItem[4].sSid == 1585) || (m_UserItem[4].sSid == 1586)) 
		{*/
	   if(m_tBabyCall ==1)//首先得是有宠
       {
		 if(m_sFealty == 100)//宠忠诚度是100
         {
			int iRandom = myrand(0, 100);			
			if(iRandom > 45 && iRandom < 54) 
            {
     		       Damage =GetMaxDamage(m_byClass);
		       if(GetQunGongAttack( m_curx,  m_cury,  Damage)>=1)
			   {
			//    SendSystemMsg("由于你周围没有怪物,范围攻击释放失败!", SYSTEM_ERROR, TO_ME);
			   }
		    }
		  }
        }
////////////////////////////////物理群攻结束////////////////////////////////////////////////////////
	   if(m_ShowHP==1){
			CString strMsg;
			strMsg.Format("%s(%d) HP= %d \\ %d",pNpc->m_strName,pNpc->m_sNid + NPC_BAND,pNpc->m_sHP,pNpc->m_sMaxHP);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			
		}
		// 傍拜 搬苞 傈价
		SendAttackSuccess(nTargetID, bIsCritical, pNpc->m_sHP, pNpc->m_sMaxHP);//yskang 0.3

		m_dwLastAttackTime = GetTickCount();
	}
}


///////////////////////////////////////////////////////////////////////////////////
//	规绢螟 规绢备狼 酒捞袍 郴备档 函拳甫 魂免
//	
int USER::SendDamagedItem(int totalDamage)
{
	int iRandom = XdY(1, 1000);

	int iDur = (int)((double)totalDamage * 0.1/10 + 0.5);

	if(iRandom < 200) 
	{ 
//		m_UserItem[HEAD].sDuration -= iDur; 
//		if(m_UserItem[HEAD].sDuration < 0) m_UserItem[HEAD].sDuration = 0; 
		SendDuration(HEAD, iDur);
	}
	else if(iRandom < 450)
	{ 
//		m_UserItem[PANTS].sDuration -= iDur; 
//		if(m_UserItem[PANTS].sDuration < 0) m_UserItem[PANTS].sDuration = 0; 
		SendDuration(PANTS, iDur);
	}
	else if(iRandom < 850)
	{ 
//		m_UserItem[BODY].sDuration -= iDur; 
//		if(m_UserItem[BODY].sDuration < 0) m_UserItem[BODY].sDuration = 0; 
		SendDuration(BODY, iDur);
	}
	else
	{ 
//		m_UserItem[SHOES].sDuration -= iDur; 
//		if(m_UserItem[SHOES].sDuration < 0) m_UserItem[SHOES].sDuration = 0; 
		SendDuration(SHOES, iDur);
	}

	return iDur;
}


///////////////////////////////////////////////////////////////////////////////////
//	
int USER::GetCounterAttack()
{
	BYTE	tWeaponClass = 255;
	int		iLevel = 0;
	int		CounterAttack = 0;

/*	if(IsCanUseWeaponSkill(tWeaponClass) == FALSE)	// 泅犁 磊脚捞 坷弗颊俊 电 公扁狼 胶懦阑 荤侩且 荐 绝栏搁 溜 盖颊捞搁
	{
		return CounterAttack;
	}

	int tClass = tWeaponClass * SKILL_NUM; 

	
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		if(i == MAGIC_BACK_ATTACK) // 馆拜
		{
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel <= 0) return 0;
												// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			iLevel += m_DynamicUserData[m_DynamicSkillInfo[i]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(i >= g_arSkillTable.GetSize()) return 0;
			if(iLevel >= g_arSkillTable[i]->m_arSuccess.GetSize()) return 0;

			CounterAttack += g_arSkillTable[i]->m_arSuccess.GetAt(iLevel);
		}
	}

	CounterAttack += m_DynamicUserData[MAGIC_BACK_ATTACK];// 酒捞袍俊狼茄 磊眉 雀乔啦 刘啊
	
	CounterAttack = CounterAttack/100;
*/
	return (int)CounterAttack;	
}

///////////////////////////////////////////////////////////////////////////////////
//	傍拜 PK 措惑捞 鞍篮 滚叼 备己盔捞搁 角菩
//	
BOOL USER::CheckBuddyMember(TCHAR *pID)
{
	int nLen = 0;
	nLen = strlen(pID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return FALSE;

	for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
	{
		if(m_MyBuddy[i].uid < USER_BAND) continue;

		nLen = 0;
		nLen = strlen(m_MyBuddy[i].m_strUserID);
		if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) continue;

		if(strcmp(m_MyBuddy[i].m_strUserID, pID) == 0) return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//	iNpcExp : 牢磊蔼栏肺 版氰摹甫 啊柳促.
void USER::GetExp(DWORD iNpcExp)					
{
#ifdef _ACTIVE_USER
	if( m_iDisplayType == 6) return; //yskang 0.5
#endif

	int iRate = 0;
	short sTempLevel = 0;
	short sLevel = m_sLevel;
	DWORD dwExp = m_dwExp;
	DWORD dwExpNext = m_dwExpNext;
	int chgx = 0;

	if(iNpcExp <= 0) return;

	GetCityRank();								

	if(m_sLevel >= o_yehuoini[0]->zgdj) 
	{ 
		m_sLevel = o_yehuoini[0]->zgdj;
		CString strMsg = _T("");
		strMsg.Format( IDS_USER_HIGHEST_LEVEL, m_strUserID);

		UpdateHighEventLevelUser(sLevel);
		SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
		return; 
	}// 饭骇力茄 120
															
	CheckMaxValue((DWORD &)dwExp, (DWORD)iNpcExp);// Overflow牢瘤 八荤

	while(dwExp >= dwExpNext)					// Level UP
	{
		if(sLevel == o_yehuoini[0]->zgdj) { dwExp = dwExpNext; break; }
		if(sLevel < o_yehuoini[0]->zgdj)
		{
			sLevel += 1;
			//chgx = +1;
			
			if(sLevel == 100)
			{
//				CString strMsg = _T("");
//				strMsg.Format( IDS_USER_HIGHEST_LEVEL, m_strUserID);

				UpdateHighEventLevelUser(sLevel);
//				SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ME);
			}
		}

		dwExp = dwExp - dwExpNext;
		dwExpNext = GetNextLevel(sLevel);
		
	} 
   
	if((sLevel - m_sLevel) > 0)					// 饭骇诀老 版快
	{
		BYTE tType = 2;
		sTempLevel = m_sLevel;

		m_dwExp = dwExp;						// 泅犁 版氰摹
		m_dwExpNext = dwExpNext;				// 促澜 饭骇 夸备版氰摹
		m_sLevel = sLevel;						// 饭骇诀

		while(sTempLevel < sLevel)				// 穿利摹甫 霖促.
		{
			sTempLevel++;
			CheckMaxValue((short &)m_sPA, (short)1);		// 公炼扒 饭骇诀付促 1究 刘啊.

			if( sTempLevel < 100 )
			{
				if((sTempLevel - 1) >= g_arLevelUpTable.GetSize()) continue;

				if(g_arLevelUpTable[sTempLevel - 1]->m_tBasicUp)
				{
					CheckMaxValue((short &)m_sSkillPoint, (short)1);
				}
			}
			else	// 100乏 捞惑
			{
				ResetOver100LevelSkill( sTempLevel );
			}
			//升级
			BabyUpgradeLevel();
		}

		
		SetUserToMagicUser();  

		m_sHP = m_sMagicMaxHP;					// 菜盲款促.
		m_sPP = m_sMagicMaxPP;
		m_sSP = m_sMagicMaxSP;

//		SendHP();								// 饭骇诀老版快 菜盲况辑 焊辰促. (HP, PP)
//		SendPP();
//		SendSP(TRUE);
		SendExp(tType);							// 饭骇诀老版快		
		SendUserStatusSkill();
		
		
	}

#ifdef _ACTIVE_USER

		//if(m_iDisplayType == 6 && m_sLevel > 25)//yskang 0.5
		if(m_iDisplayType == 6) //yskang 0.5
		{			
			CString strMsg = _T("");
			strMsg.Format("蜡历丛捞 立加窍脚 眉氰魄 辑滚胶绰 老何 扁瓷惑 力距捞 乐嚼聪促.");
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);
		}
#endif
	
	m_dwExp = dwExp;							// 扁成 版氰摹父...

	SendExp();

	UpdateUserData();

		
}

//////////////////////////////////////////////////////////////////////////////////
// 滚叼甫 茄 惑怕老锭 眠啊版氰摹甫 掘绊 颇萍盔俊霸 眠啊 版氰摹甫 唱穿绢霖促
// zinee 02/12/09
void USER::GetExpBuddy(int iNpcExp )
{
	int		i;
	int		nBuddyUsers;
	int		nBuddyLevelSum;
	DWORD	dwBonusExp;
	double	fLevelAdvantage;

	USER*		pUser;
	BuddyList	NearBuddyUser[MAX_BUDDY_USER_NUM];


	if( !m_bNowBuddy ) GetExp( iNpcExp );

	nBuddyUsers = 0;
	nBuddyLevelSum = 0;
	memset( NearBuddyUser, -1, sizeof(BuddyList)*MAX_BUDDY_USER_NUM );
	int diffLevel = 0;

	// 泅犁 醚滚叼蜡历 荐客 辟贸(2拳搁)俊 乐绰 滚叼蜡历府胶飘, 饭骇钦 备窍扁.
	for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
	{
		if(m_MyBuddy[i].uid != -1 )
		{
			if( m_MyBuddy[i].uid != (m_uid + USER_BAND) )  // 郴 磊脚捞 酒囱 颇萍盔捞扼搁..
			{
				pUser = GetUser( m_MyBuddy[i].m_strUserID );
				if( pUser != NULL ) 
				{
					//滚叼蜡历客 鞍篮粮俊 乐绊 辟贸(2拳搁)俊 乐阑锭 滚叼版氰摹府胶飘俊 眠啊.
					if( m_curz == pUser->m_curz && GetDistance( pUser->m_curx, pUser->m_cury, (SCREEN_X + SCREEN_Y) ) &&  diffLevel <= 10)
					{
						NearBuddyUser[i].uid = m_MyBuddy[i].uid;
						strcpy( NearBuddyUser[i].m_strUserID, m_MyBuddy[i].m_strUserID );

						nBuddyLevelSum += pUser->m_sLevel;
					}
				}

			}

			nBuddyUsers++;
		}
	}

	// 滚叼盔 荐俊 蝶扼 焊呈胶 版氰摹. 2疙:10%, 3疙:15%, 4疙:20%
	dwBonusExp = (DWORD)((double)( iNpcExp * (nBuddyUsers*5) )/100 + 0.5);

	if( dwBonusExp != 0 && nBuddyLevelSum != 0 )	
	for( i=0; i< MAX_BUDDY_USER_NUM; i++ )
	{
		if( NearBuddyUser[i].uid != -1 ) 
		{
			pUser = GetUser( NearBuddyUser[i].m_strUserID );

			if( pUser != NULL )
			{
				// 盒硅登绰 版氰摹 = (眠啊 版氰摹 * 阿 滚叼盔狼 饭骇 /荤成茄 蜡历狼 饭骇) 
				//					* 阿 滚叼盔狼 饭骇 / 版氰摹甫 掘篮 滚叼盔狼 饭骇 醚钦
				if(pUser->m_bLive == USER_LIVE)
				{
					fLevelAdvantage = (double)pUser->m_sLevel/(double)m_sLevel;
					if( fLevelAdvantage > 1 ) fLevelAdvantage = 1;

/*					TRACE( "NpcExp:%d, bonus: %d, Adv: %f, Exp: %d, fExp: %d\n", iNpcExp, dwBonusExp, fLevelAdvantage,
					(int)(dwBonusExp*fLevelAdvantage*pUser->m_sLevel/nBuddyLevelSum),
					(int)((double)dwBonusExp*fLevelAdvantage*(double)pUser->m_sLevel/(double)nBuddyLevelSum) );							
*/


					pUser->GetExp( (int)(dwBonusExp*fLevelAdvantage*pUser->m_sLevel/nBuddyLevelSum+0.5) );
				}
			}
		}
	}

	GetExp( iNpcExp+dwBonusExp );

}

////////////////////////////////////////////////////////////////////////////////////
// 蜡历啊 HI 牢啊 滚叼惑怕牢啊俊 蝶扼 版氰摹甫 拌魂茄促.
// zinee 02/12/09
void USER::GetExpCommon( int iNpcExp )
{
	int exp;
    CString strMsg = _T("");
	
		/*if(m_sLevel<= 50)
	{
		iNpcExp=iNpcExp* o_yehuoini[0]->EXP_50;
	 }
	else if(m_sLevel<60)
	{
		iNpcExp=iNpcExp*o_yehuoini[0]->EXP_60;
	 }
	else if(m_sLevel<70)
	{
		iNpcExp=iNpcExp*o_yehuoini[0]->EXP_70;
	 }
	else if(m_sLevel<80)
	{
	 	iNpcExp=iNpcExp*o_yehuoini[0]->EXP_80;
	 }
	else if(m_sLevel<90)
	{
	 	iNpcExp=iNpcExp*o_yehuoini[0]->EXP_90;
	 }
	else if(m_sLevel<100)
	{
	 	iNpcExp=iNpcExp*o_yehuoini[0]->EXP_100;
	 }
	else if(m_sLevel<110)
	{
	 	iNpcExp=iNpcExp*o_yehuoini[0]->EXP_110;
	 }
	else if(m_sLevel<120)
	{
		iNpcExp=iNpcExp*o_yehuoini[0]->EXP_120;
 	 }
	else if(m_sLevel<130)
	{
		iNpcExp=iNpcExp*o_yehuoini[0]->EXP_130;
 	 }
	else if(m_sLevel<140)
	{
		iNpcExp=iNpcExp*o_yehuoini[0]->EXP_140;
 	 }
	else if(m_sLevel<150)
	{
		iNpcExp=iNpcExp*o_yehuoini[0]->EXP_150;
 	 }
	else if(m_sLevel<155)
	{
		iNpcExp=iNpcExp*o_yehuoini[0]->EXP_155;
    }
	else if(m_sLevel<160)
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_160;
 	 }
	else if(m_sLevel<165)
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_165;
 	 }
	else if(m_sLevel<170)
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_170;
 	 }
	else if(m_sLevel<175)
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_175;
 	 }
	else if(m_sLevel<180)
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_180;
 	 }
	else if(m_sLevel<185)
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_185;
 	 }
	else if(m_sLevel<190)
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_190;
 	 }
	else if(m_sLevel<195)
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_195;
 	 }
    else 
    {
		iNpcExp=iNpcExp/o_yehuoini[0]->EXP_200;
 	 }*/
	 if(m_sLevel<= 100){
		iNpcExp=iNpcExp*2;
	 }else if(m_sLevel<120){
		iNpcExp=iNpcExp*2;
	 }else if(m_sLevel<140){
		iNpcExp=iNpcExp*8;
	 }else if(m_sLevel<150){
	 	iNpcExp=iNpcExp*4;  
	 }else if(m_sLevel<160){
	 	iNpcExp=iNpcExp*2;  
	 }else if(m_sLevel<165){
	 	iNpcExp=iNpcExp;  
	 }else if(m_sLevel<170){
	 	iNpcExp=iNpcExp/2;  
	 }else if(m_sLevel<175){
	 	iNpcExp=iNpcExp/5; 
	 }else if(m_sLevel<180){
	    iNpcExp=iNpcExp/10; 
	 }else if(m_sLevel<190){
		iNpcExp=iNpcExp/20;
 	 }else if(m_sLevel<195){
		iNpcExp=iNpcExp/30;
 	 }else if(m_sLevel<197){
		iNpcExp=iNpcExp/4000;
	 }else{
		iNpcExp=iNpcExp/9000;
 	 }

//////////////////////////////////////////////////////////////////////////////////////////////////////
	 if(isDoubleExp) //服务器双倍经验
	 {
		 iNpcExp = iNpcExp * 2;
	 }

	 if(isDoubleExpup4) //服务器4倍经验
	 {
		 iNpcExp = iNpcExp * 4;
	 }

     
	 if(isGuildRen == TRUE || m_bCBOff == TRUE)//翅膀经验倍数
	 {
		 iNpcExp = (int)(iNpcExp * 1);
	 }
	 
	 if(m_isDoubleExp > 0 || g_sanJingYan == TRUE || m_dwGuarDianTianShi > 0 || m_dwVIPTime > 0 )//捐款和服务器3倍以及VIP3倍
	 {
		 iNpcExp = (int)(iNpcExp * 2);
	 }
	 if (m_UserItem[38].tMagic[0] == 22) //经验守护
	 {
       iNpcExp = (int)(iNpcExp * 1.2);
		
	 }
	  if (m_UserItem[38].tMagic[0] == 23) 
	 {
       iNpcExp = (int)(iNpcExp * 1.3);
		
	 }
	  if (m_UserItem[38].tMagic[0] == 24) 
	 {
       iNpcExp = (int)(iNpcExp * 1.5);
	 }

	
	 if( m_dwHiExpTime > 0 || m_dwBHTime > 0)//兴奋经验倍数 或者天使双倍
	 {
		exp = iNpcExp * 2;
		if( m_dwGuarDianTianShi > 0) //有兴奋时，并且是VIP，那么获得经验值增加30%属性 生效
		{
			exp += exp * m_DynamicUserData[MAGIC_EXP_3BEI_UP] / 100;
		}
	 }
	 else exp = iNpcExp;
	 //exp = (int)(exp * 0.8);////组队经验
	 if(m_bNowBuddy)	GetExpBuddy( exp );
	 else GetExp( exp );

	/* if( m_ShowHP == 1 )
	 {
		strMsg.Format("获得经验值:%d点.",exp);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg,SYSTEM_NORMAL,TO_ME);
	 }*/
}


///////////////////////////////////////////////////////////////////////////////////
//  版氰摹甫 焊辰促. (饭骇诀老锭 包访 荐摹甫 霖促)
//
void USER::SendExp(int tType)
{
	CBufferEx TempBuf;

	TempBuf.Add(LEVEL_UP);
	TempBuf.Add((BYTE)tType);
	TempBuf.Add(m_dwExp);

	if(tType == 2)								// 饭骇诀
	{		
		TempBuf.Add(m_dwExpNext);
		TempBuf.Add(m_sLevel);
		TempBuf.Add(m_sSkillPoint);
		TempBuf.Add(m_sPA);
	}

	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////////////
//	
//
void USER::UserStatusSkillUp(TCHAR *pBuf)
{
	int index = 0;
	int iPlusMinus = 1;
	BYTE tType = GetByte(pBuf, index);			// PA, MA 肚绰 Skill Level牢瘤 魄窜
	BYTE tKinds = GetByte(pBuf, index);			// 阿 辆幅甫 魄窜

	if(tType >= 4) return;
	if(tKinds < 0 || tKinds >= SKILL_COUNT) 
		return;
	
	int iNum = 0;

	CBufferEx TempBuf;

	if(tType == PA_MA_STATUS) 
	{
		if(m_sPA <= 0) return;

		switch(tKinds)
		{
		case USER_STR:
			iNum = m_sSTR;
			iNum += iPlusMinus;														// 歹窍芭唱 哗芭唱
			if(iNum <= 0 || iNum > 100) return;				// 0~100 荤捞父
			m_sSTR = iNum; m_sPA -= 1;
			break;

		case USER_CON:
			iNum = m_sCON;
			iNum += iPlusMinus;
			if(iNum <= 0 || iNum > 100) return;
			m_sCON = iNum; m_sPA -= 1;
			break;

		case USER_DEX:
			iNum = m_sDEX;
			iNum += iPlusMinus;
			if(iNum <= 0 || iNum > 100) return;
			m_sDEX = iNum; m_sPA -= 1;
			break;

		case USER_VOL:
			iNum = m_sVOL;
			iNum += iPlusMinus;
			if(iNum <= 0 || iNum > 100) return;
			m_sVOL = iNum; m_sPA -= 1;
			break;

		case USER_WIS:
			iNum = m_sWIS;
			iNum += iPlusMinus;
			if(iNum <= 0 || iNum > 100) return;
			m_sWIS = iNum; m_sPA -= 1;
			break;
		}
		//仲裁者生命不息
		if(tKinds == SKILL_JUDGE_RENMINBUXI)
		{
			GetMagicItemSetting();
		}
		SetUserToMagicUser();		// 扁夯瓷仿蔼捞 函沁栏骨肺 促矫 悸泼
									// 扁夯瓷仿摹 盒硅甫 扁撅
	}
	else if(tType == SKILL_STATUS)
	{
		if(CheckSkillClass(tKinds) != m_byClass) return;
		if(m_sSkillPoint <= 0 || m_sSkillPoint > 500) return;// 器牢飘啊 1捞惑
		if(m_UserSkill[tKinds].tLevel >= 20) return;

		m_UserSkill[tKinds].tLevel += 1;
		m_sSkillPoint -= 1;
		//仲裁者生命不息
		if(tKinds == SKILL_JUDGE_RENMINBUXI)
		{
			GetMagicItemSetting();
		}
		SetUserToMagicUser();
	}else if (tType == 0x3)
	{
		if(m_sLevel>=140&&(tKinds==0x0c||tKinds==0x0d||tKinds==0x0f))
		{
			//统计130加了多少技能点

			//140级技能12永远加140级技能,13永远加150级技能点
			if(m_sLevel>=150&&(tKinds==0x0d))
			{
				//防止130级前技能点全加至140技能上来
				if(m_UserSkill140[1].tLevel> m_sLevel-150)
				{
					return ;
				}
			}
			if(m_sLevel>=140&&(tKinds==0x0c))
			{
				//防止130级前技能点全加至140技能上来
				if(m_UserSkill140[0].tLevel+m_UserSkill140[1].tLevel> m_sLevel-140)
				{
					return ;
				}
			}
			switch(tKinds)
			{
			case 0x0c:
					//是否大于10
					if(m_UserSkill140[0].tLevel>=10)
						return ;
					m_UserSkill140[0].tLevel++;
					break;
			case 0x0d:
				//是否大于10
				if(m_UserSkill140[1].tLevel>=10)
					return ;
				m_UserSkill140[1].tLevel++;
				break;
			default:
				return ;
			}
			m_sSkillPoint_--;
			CheckMagicItemMove();
			UpdateUserData();
			return ;
		}
		if(m_sLevel <130)
			return;
		//if(tKinds/3!=m_byClass)
		//	return;
		if(m_sSkillPoint_ <= 0 || m_sSkillPoint_ > 70) 
			return;
		if(m_UserSkill_[tKinds].tLevel >= 20) return;
		m_UserSkill_[tKinds].tLevel += 1;
		m_sSkillPoint_ -= 1;
		CheckMagicItemMove();
		UpdateUserData();
		SetUserToMagicUser();
		return ;
	}
	else return;
	CheckMagicItemMove();
	SendUserStatusSkill();

}


///////////////////////////////////////////////////////////////////////////////////
//	蜡历狼 荐摹 函拳樊甫 舅赴促.
//  人物属性栏显示
void USER::SendUserStatusSkill()
{
	int i;
	CBufferEx TempBuf;
	BYTE tWeaponClass = 0;

	short	sTemp = 0;
	short	sMaxDamage = 0, sMaxDefense = 0;

	TempBuf.Add(USER_STATUS_SKILL_UP_RESULT);

	IsCanUseWeaponSkill(tWeaponClass);
	sMaxDamage = GetMaxDamage(tWeaponClass);//攻击力
	
	sTemp = (short)GetIronSkill();
	sMaxDefense = sTemp + GetDefense(); //防御力



	TempBuf.Add(m_sPA);
	TempBuf.Add(m_sMagicSTR);
	TempBuf.Add(m_sMagicCON);
	TempBuf.Add(m_sMagicDEX);
	TempBuf.Add(m_sMagicVOL);
	TempBuf.Add(m_sMagicWIS);

	if(m_sHP > m_sMagicMaxHP) m_sHP = m_sMagicMaxHP;
	if(m_sPP > m_sMagicMaxPP) m_sPP = m_sMagicMaxPP;
	if(m_sSP > m_sMagicMaxSP) m_sSP = m_sMagicMaxSP;

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sPP);
	TempBuf.Add(m_sSP);

	TempBuf.Add(m_sMagicMaxHP);
	TempBuf.Add(m_sMagicMaxPP);
	TempBuf.Add(m_sMagicMaxSP);

	TempBuf.Add(sMaxDamage);	// 弥措 傍拜仿
	TempBuf.Add(sMaxDefense);	// 弥措 规绢仿

	TempBuf.Add(m_sSkillPoint);

	TempBuf.Add(m_byClass);
	
	//基本技能5项
	//新职业技能
	//	0x19 0x00 0x00 
	//	0x1a 0x00 0x00 
	//	0x1b 0x00 0x00 
	//	0x1c 0x00 0x00 
	//	0x18 0x00 0x00 
	if(m_byClass == JUDGE)
	{
		for(i = 25;i<SKILL_COUNT;i++)
		{
			TempBuf.Add(m_UserSkill[i].sSid);
			TempBuf.Add(m_UserSkill[i].tLevel);
		}
		TempBuf.Add((short)m_UserSkill[24].sSid);
		TempBuf.Add(m_UserSkill[24].tLevel);
		//TCHAR ba[]={0x19,0x00,0x00,0x1a,0x00,0x00,0x1b,0x00,0x00,0x1c,0x00,0x00,0x18,0x00,0x00};
		//TempBuf.AddData(ba,15);
	}else
	{
		for( i = m_byClass * SKILL_NUM; i < m_byClass * SKILL_NUM + SKILL_NUM; i++)
		{
			TempBuf.Add(m_UserSkill[i].sSid);
			TempBuf.Add(m_UserSkill[i].tLevel);
		}
	}

	TempBuf.Add(m_sSkillPoint_);
	//新职业百级技能
	//0x10 0x00 0x00 0x11 0x00 0x00 0x12 0x00 0x00 
	if(m_byClass == JUDGE)
	{
		for(i = 16;i<SKILL_130_COUNT;i++)
		{
			TempBuf.Add((short)i);
			TempBuf.Add(m_UserSkill_[i].tLevel);
		}
	}else
	{
		for( i=m_byClass*3;i<m_byClass*3+3;i++)
		{
			TempBuf.Add((short)i);
			TempBuf.Add(m_UserSkill_[i].tLevel);
		}
	}
/*	//国服添加了6byte 0
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);*/
	//国服添加了6byte 0 140与150级技能
#ifdef _USE_Guo_Fu_
	for( i=0;i<2;i++)
	{
		TempBuf.Add(m_UserSkill140[i].sSid);
		TempBuf.Add(m_UserSkill140[i].tLevel);
	}
#endif

	short spellattack=GetUserSpellAttack(TRUE);//魔法攻击
	short spelldefence=GetUserSpellDefence();;//计算玩家自己的魔法防御
	TempBuf.Add(spellattack);
	TempBuf.Add(spelldefence);
	TempBuf.Add(m_sKillCount);  //显示PK值
	TempBuf.Add((short)0x00);   //显示年龄点数：无
	TempBuf.Add(m_iCityValue);//(m_iCityValue);  //显示市民等级经验
	//国服添加了2int,PK值与,市民等级
	TempBuf.Add((short)0x00);
	TempBuf.Add((short)m_sKillCount);
	TempBuf.Add((int)m_sCityRank);
        
	GetRecoverySpeed();	
	Send(TempBuf, TempBuf.GetLength());
	//护法
	if(m_byClass ==  JUDGE)
		SendJudgePsiStatus(TO_ME);
}
//////////////////////////////////
//计算玩家自己的魔法攻击
/////////////////////////////////
short USER::GetUserSpellAttack(BOOL bMax)
{
	int iPsiAttackUp =0 ;
	short sBasic	= 0;
    short iSid=m_UserItem[RIGHT_HAND].sSid;
    if (m_byClass == 1)  //法135级魔法加魔攻180到属性
	{
		if(m_dwYINENgTime != 0) sBasic += 100;
		if(sBasic < 0) sBasic = 0;
	}
//	if(m_sLevel >= ADD_USER_LEVEL) 
//		sBasic += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDamage;
	

	if(m_UserItem[RIGHT_HAND].tIQ != UNIQUE_ITEM)	
	{
		if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
		{
			if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byClass == STAFF_ITEM)
			{
				int iUpCount = m_UserItem[RIGHT_HAND].tMagic[ITEM_UPGRADE_COUNT];
				if(iUpCount > 0 && iUpCount <= MAX_ITEM_UPGRADE_COUNT)
				{
					int sSid = m_UserItem[RIGHT_HAND].sSid;

					//130级武器前6改13,后增加15                 //////////////////////////////////NEO版本需要开放的
					if(Is130Wearp(sSid))
					{
						if(iUpCount<=6)
						{
							iPsiAttackUp = (iUpCount * 13);
						}else
						{
							iPsiAttackUp = (6 * 13)+(iUpCount-6)*15;
						}
					}else 
					{//100级武器前9改+4,后增加+10
						if(iUpCount<=9)
						{
							iPsiAttackUp = (iUpCount * 4);
						}else
						{
							iPsiAttackUp = (4 * 9)+(iUpCount-(6))*10;
						}
					}

					//iPsiAttackUp = (iUpCount * ATTACK_UPGRADE_PSI_BAND);
				}
			}
		}
	}
    if(iSid==1225 || iSid==1226 || iSid==1227 || iSid==1578 || iSid==1579 || iSid==1580) sBasic+=10;
	iPsiAttackUp +=sBasic+ m_DynamicUserData[MAGIC_PSI_ATTACK_UP] + (int)((double)m_DynamicEBodyData[EBODY_VOL_TO_PSI_DAMAGE] / 100 * (double)m_sMagicVOL)+m_sMagicVOL/2;
	iPsiAttackUp += (int)((double)iPsiAttackUp * (double)m_DynamicEBodyData[EBODY_PSI_ATTACK_UP] / 100);
	if(m_sLevel>=70 )
		iPsiAttackUp += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDamage;
	return iPsiAttackUp;
}
//////////////////////////////////
//计算玩家自己的魔法防御
//////////////////////////////////
short USER::GetUserSpellDefence()
{
	int nDefense = 0;
	double iBasic = 0;			// 扁夯单固瘤
	iBasic = (int)((double)m_sMagicVOL/2 + 0.5);
	
	if(iBasic <= 0) iBasic = 0;
	
	int sBasic=m_sMagicVOL;
	//百级每改增加3点魔防
	sBasic += Get100ItemPsiDefense(m_UserItem[0].sSid,0);
	sBasic += Get100ItemPsiDefense(m_UserItem[1].sSid,1);
	sBasic += Get100ItemPsiDefense(m_UserItem[8].sSid,8);
	sBasic += Get100ItemPsiDefense(m_UserItem[9].sSid,9);

	//if(m_UserItem[0].tIQ==12)//帽子
	//{
	//	sBasic+=m_UserItem[0].tMagic[ITEM_UPGRADE_COUNT]*3;
	//	if(m_dwMagicFtTime > 0) sBasic+=3;
	//}
	//if(m_UserItem[1].tIQ==12)//衣服
	//{
	//	sBasic+=m_UserItem[1].tMagic[ITEM_UPGRADE_COUNT]*3;
	//	if(m_dwMagicFtTime > 0) sBasic+=3;
	//}
	//if(m_UserItem[8].tIQ==12)//裤子
	//{
	//	sBasic+=m_UserItem[8].tMagic[ITEM_UPGRADE_COUNT]*3;
	//	if(m_dwMagicFtTime > 0) sBasic+=3;
	//}
	//if(m_UserItem[9].tIQ==12)//鞋子
	//{
	//	sBasic+=m_UserItem[9].tMagic[ITEM_UPGRADE_COUNT]*3;
	//	if(m_dwMagicFtTime > 0) sBasic+=3;
	//}
	

int dItemResist =sBasic+ m_DynamicUserData[MAGIC_PSI_RESIST_UP] + (int)((double)m_DynamicEBodyData[EBODY_VOL_TO_PSI_RESIST] / 100 * (double)m_sMagicVOL);
    dItemResist+=(int)(dItemResist*(double)m_DynamicEBodyData[EBODY_PSI_RESIST_UP] / 100);//机械增加的魔抗
	return dItemResist;
}

///////////////////////////////////////////////////////////////////////////////////
//	秦寸 拌凯狼 胶懦牢瘤 魄窜.
//
int USER::CheckSkillClass(short sSid)
{
	if(sSid >= 0 && sSid < SKILL_NUM) return BRAWL;
	else if(sSid < (2 * SKILL_NUM)) return STAFF;
	else if(sSid < (3 * SKILL_NUM)) return EDGED;
	else if(sSid < TOTAL_SKILL_NUM) return FIREARMS;
	else if(sSid>=25&&sSid<=28)//仲裁
		return JUDGE;
	else return -1;
}


//////////////////////////////////////////////////////////////////////////////////
//	傍拜角菩 菩哦阑 焊辰促.
//
void USER::SendAttackFail(BYTE tErrorCode)
{
	CBufferEx TempBuf;
	
	TempBuf.Add(ATTACK_RESULT);
	TempBuf.Add(ATTACK_FAIL);
	TempBuf.Add(tErrorCode);
	
	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	傍拜固胶 菩哦阑 焊辰促.
//
void USER::SendAttackMiss(int tuid)
{
	CBufferEx TempBuf;
	
	TempBuf.Add(ATTACK_RESULT);
	TempBuf.Add(ATTACK_MISS);
	TempBuf.Add((int)(m_uid + USER_BAND));
	TempBuf.Add(tuid);

//	SendInsight(TempBuf, TempBuf.GetLength());
	SendExactScreen(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////////////
//	傍拜己傍 菩哦阑 焊辰促.
//
void USER::SendAttackSuccess(int tuid, BOOL bIsCritical, short sHP, short sMaxHP)//yskang 0.3 //void USER::SendAttackSuccess(int tuid, CByteArray& arAction1, CByteArray& arAction2, short sHP, short sMaxHP)
{
	CBufferEx TempBuf;
//	CByteArray arAction1;
//	CByteArray arAction2;
	
	TempBuf.Add(ATTACK_RESULT);
	//---------------------------------------------------------------
	//yskang 0.3 菩哦阑 临捞扁 困窍咯 胶懦咀记阑 窜鉴窍霸 傈价
	//窃荐 牢磊 磊眉甫 荐沥秦具 且鳖?
	//---------------------------------------------------------------
	if(bIsCritical == FALSE) //农府萍拿 傍拜捞 酒聪衬
	{
		TempBuf.Add(ATTACK_SUCCESS);//焊烹 傍拜
	}
	else
	{
		TempBuf.Add(CRITICAL_ATTACK_SUCCESS); //农府萍拿 傍拜
	}
	TempBuf.Add((int)(m_uid + USER_BAND));
	TempBuf.Add(tuid);
	//------------------------------------------------------------------

/*	BYTE tAction1 = (BYTE)arAction1.GetSize();
	BYTE tAction2 = (BYTE)arAction2.GetSize();
	int i = 0;

	TempBuf.Add(tAction1);
	if(tAction1 > 0)
	{
		for(i = 0; i < arAction1.GetSize(); i++)
		{
			TempBuf.Add(arAction1[i]);
		}
	}
	TempBuf.Add(tAction2);
	if(tAction2 > 0)
	{
		for(i = 0; i < arAction2.GetSize(); i++)
		{
			TempBuf.Add(arAction2[i]);
		}
	}
*/	
	TempBuf.Add((int)sHP);
	TempBuf.Add((int)sMaxHP);

	// 弥碍 捞亥飘锭巩俊 款康评捞 夸没 02-11-1 
	if(m_curz == 61 || m_curz == 62 || m_curz == 63) SendInsight(TempBuf, TempBuf.GetLength());
	else SendExactScreen(TempBuf, TempBuf.GetLength());	// 菩哦 何窍樊捞 腹酒辑 绊模巴烙 02-10-28 by Youn Gyu
//	SendInsight(TempBuf, TempBuf.GetLength());

	//护法攻击
	if(m_tHuFaType !=0&&m_nHuFaHP>0)
	{
		int nDamage = GetHuFaDamage();
		if(nDamage<15)
			nDamage = 15;

		sHP-=nDamage;
		if(sHP<=0)
			sHP = 0;
		//护法攻击目标去血
		if(tuid >= USER_BAND && tuid < NPC_BAND)
		{
			USER *pUser = GetUser(tuid-USER_BAND);
			if(pUser)
				pUser->SetDamage(nDamage);
			//用户掉血显示
			//SendDamageNum(0,tuid,nDamage);	
		}else
		{
			CNpc *pNpc = GetNpc(tuid-NPC_BAND);
			if(pNpc)
			{
				pNpc->SetHuFaFinalDamage(this,nDamage);
			}
			//npc掉血
			SendDamageNum(1,tuid,nDamage);	

			//SendNpcHP(tuid,sHP);
		}
		CBufferEx TempBuf2;

		TempBuf2.Add(ATTACK_RESULT);//国服已经修正
		if(bIsCritical == FALSE)
		{
			TempBuf2.Add(ATTACK_SUCCESS);
		}
		else
		{
			TempBuf2.Add(CRITICAL_ATTACK_SUCCESS);
		}
		TempBuf2.Add((int)(m_uid + USER_BAND+USER_HUFA_BAND));
		TempBuf2.Add(tuid);
#ifdef _USE_Guo_Fu_
		TempBuf2.Add((int)sHP);
		TempBuf2.Add((int)sMaxHP);

#else
		TempBuf2.Add((short)sHP);
		TempBuf2.Add((short)sMaxHP);
#endif
		if(m_curz == 61 || m_curz == 62 || m_curz == 63)
			SendInsight(TempBuf2, TempBuf2.GetLength());
		else 
			SendExactScreen(TempBuf2, TempBuf2.GetLength());
	}
}


///////////////////////////////////////////////////////////////////////////////////
//	鞍篮 粮救狼 蜡历甫 档矫俊辑 混赴促
//
BOOL USER::LiveCity(int x, int y, BYTE type)
{
//	CPoint ptNew;
//	CPoint pt = FindNearAvailablePoint_S(x, y);
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
	if(m_curx < 0 || m_cury < 0 || m_curx >= g_zone[m_ZoneIndex]->m_sizeMap.cx || m_cury >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return FALSE;
//	if(pt.x != -1 && pt.y != -1) 
//	{
//		if(!SetUid(pt.x, pt.y, m_uid + USER_BAND)) return FALSE;
		long lUid = m_uid + USER_BAND;

		if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, (long)0, (long)lUid) == (long)lUid)
		{
//			m_curx = pt.x;
//			m_cury = pt.y;
			m_curx = x;
			m_cury = y;

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, lUid);
			SightRecalc();
			return TRUE;
		}
//	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//	磷菌带 蜡历甫 混赴促.
//
void USER::LiveReq(TCHAR *pBuf, BYTE tOption)
{
	// tOption == 1 捞搁 积疙狼 馆瘤肺 混酒唱绰 版快烙
	// 捞版快俊绰 pBuf 蔼阑 眉农窍瘤 臼配废 茄促.
	
	// 林狼秦辑 荐沥秦 林档废 茄促.

	if(m_bLive == USER_LIVE) return;

	int i;
	int result = 0;
	int iPotalZone = -1;

	int index = 0, rand = 0;

	int x = 0;		// 鸥款器呕俊 历厘等 DB谅钎甫 扁霖栏肺 档矫俊辑 混酒巢	
	int y = 0;

	BOOL nResult = FALSE;
	CPoint	ptPotal, pt;	

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // 促弗 蜡历甸篮 够窍唱 眉农..
/*
	for(i = 0; i < g_TownPotal.GetSize(); i++)
	{
		if(g_TownPotal[i]->iZone == m_curz) { iPotalIndex = i; break; }
	}

	if(iPotalIndex < 0) { nResult = TRUE; goto go_result; }
*/
	pt = GetTownPotal(iPotalZone);
	x = pt.x;
	y = pt.y;

	if(iPotalZone < 0) 
	{ 
		if( m_curz == 56 || m_curz == 57 || m_curz == 58 || m_curz == 59 ) { nResult = TRUE; goto go_result; }

		x = g_TownPotal[0]->iPotalX;
		y = g_TownPotal[0]->iPotalY;
		iPotalZone = g_TownPotal[0]->iPotalZone;
	}
													// 粮 眉牢瘤俊辑 磊眉 谅钎甫 函券茄促.			
//	if(g_TownPotal[iPotalIndex]->iPotalZone != m_curz) ZoneMoveReq(g_TownPotal[iPotalIndex]->iPotalZone, x, y);
	if(iPotalZone != m_curz) ZoneMoveReq(iPotalZone, x, y);
	else 
	{	
		ptPotal = ConvertToServer(x, y);			

		i = SEARCH_TOWN_POTAL_COUNT;				// 谅钎甫 茫扁困茄 冉荐
		pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
		if(pt.x == -1 || pt.y == -1)
		{
			while(TRUE)								// 鸥款器呕 伐待 谅钎甫 掘绰促.
			{
				rand = myrand(-TOWN_POTAL_SIZE, TOWN_POTAL_SIZE);
				ptPotal.x += rand; ptPotal.y += rand;

				pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
				if(pt.x != -1 && pt.y != -1) break;

				i--;
				if(i <= 0) // 唱吝俊
				{
					pt = ConvertToServer(x, y);
					m_curx = pt.x; m_cury = pt.y;
					nResult = TRUE;
					goto go_result;
				}
			}
		}
		x = pt.x; y = pt.y;
		if(LiveCity(x, y) == FALSE)  { nResult = TRUE; goto go_result; }
	}

go_result:
	m_bLive = USER_LIVE;

	if(m_sMagicMaxHP > m_sMaxHP) m_sHP = m_sMagicMaxHP;
	else m_sHP = m_sMaxHP;
	
//	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0) EndGuildWar();	// 鞘靛傈 俊辑 秦寸 辨付狼 檬扁拳啊 狐瘤骨肺 促矫 茄促.

	if(nResult)
	{
		SoftClose();
		//Close();	// &&& 犬牢窍磊.(NULL肺 窍绰瘤...)
		return;		// 场鳖瘤 给茫栏搁 辑滚俊辑 酒辰促.
	}

	InterlockedExchange(&m_lDeadUsed, (LONG)0);
	CPoint pos = ConvertToClient( m_curx, m_cury );

	CBufferEx TempBuf;
	TempBuf.Add(USER_INFO);
	TempBuf.Add(INFO_MODIFY);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);

	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);

	TempBuf.Add(m_iSkin);
	TempBuf.Add(m_iHair);
	TempBuf.Add((BYTE)m_sGender);
	TempBuf.AddData(m_strFace, 10);

	for(i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(m_UserItem[i].sSid);

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sMagicMaxHP);

	TempBuf.Add(m_tDir);

//	TempBuf.Add((BYTE)m_strAbnormal[0]);			// Abnormal Info
//	TempBuf.Add(m_tPsiAbnormal);						//&&&&&&&&&&&& Test Code	

	TempBuf.Add(m_dwAbnormalInfo);					// 惑怕捞惑 沥焊
	TempBuf.Add(m_dwAbnormalInfo_);
	
	TempBuf.Add((DWORD)0);
	TempBuf.Add((DWORD)0);
	//国服此处多16 byte 0
	for(int zero = 0;zero<16;zero++)
		TempBuf.Add((BYTE)0x00);

	TempBuf.Add(m_sCityRank);
	TempBuf.Add((int)m_dwGuild);					//&&&&&&&&&&&& Test Code
	TempBuf.AddString(m_strGuildName);			// 辨靛 捞抚阑 眠啊
	TempBuf.Add(m_sGuildVersion);

	TempBuf.Add(m_byClass);
	TempBuf.Add((BYTE)m_bPkStatus);

	TempBuf.AddString(m_strLoveName);//yskang 0.1
	
	for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) TempBuf.Add(m_UserItem[i].sSid);	// EBody
	if(m_UserItem[TOTAL_ITEM_NUM-2].sSid!=-1&&m_UserItem[TOTAL_ITEM_NUM-2].sDuration!=0){
		TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]));
		TempBuf.Add((BYTE)0x00);
	}else{
		TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]);
		TempBuf.Add((BYTE)0xff);
	}
	TempBuf.AddString(m_PersonalShopName);
	//国服此处多5byte 0x0a 0x00 0x00 0x00 0x00
	TempBuf.Add((BYTE)0x01);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)m_tBabyCall);
	if(m_tBabyCall)
	{
		TempBuf.Add(g_szBabyName[m_sBabyID%2],strlen(g_szBabyName[m_sBabyID%2]));
		TempBuf.Add(m_sBabyID);
	}
	
	SendInsight(TempBuf, TempBuf.GetLength());

	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.

	SendHuFaInfo(this,TO_INSIGHT);

	//TRACE("%d锅 蜡历 混酒巢\n", m_uid );
}

/////////////////////////////////////////////////////////////////////////////////////////
// 磷篮 滚叼盔阑 何劝矫挪促.
//
void USER::RevivalReq( TCHAR *pBuf )
{
	int			i;
	int			nBufferPos;
	short		tSlot;
	BYTE		error;
	USER*		pUser;
	TCHAR		strUserID[31];
	ItemList	ReverseItem;
	CBufferEx	TempBuf, TempYourBuf;
	CPoint		ptRevival, ptClientRevival;


	nBufferPos = 0;
	if(!GetVarString(sizeof(strUserID), (char*)strUserID, pBuf, nBufferPos)) return;

	if(m_bLive == USER_LIVE) return;

	pUser = GetUser(strUserID);	// User Pointer 甫 掘绰促.

	// 何劝矫难林绰 蜡历啊 粮犁窍瘤 臼澜
	if( pUser == NULL )
	{
		error = 1;
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}

	if( !CheckBuddyMember(pUser->m_strUserID) )
	{
		error = 1;
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}

	::ZeroMemory( &ReverseItem, sizeof(ItemList) );
	ReverseItem.sSid = NORMAL_ITEM_REVERSE;
	tSlot = (short)pUser->GetSameItem(ReverseItem, INVENTORY_SLOT);

	//蜡历 牢亥俊 酒捞袍捞 绝芭唱 郴备仿捞 0老 版快
	if( tSlot == -1 )
	{
		error = 2;
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}
	else if( pUser->m_UserItem[tSlot].sCount <= 0 )
	{
		error = 2;
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}

	
	////////////////////////////////////////////////////////////////
	// 何劝困摹甫 茫酒辑 何劝矫挪促.

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // 促弗 蜡历甸篮 够窍唱 眉农..

	ptRevival = FindNearAvailablePoint_S( m_curx, m_cury );

	
	if( ptRevival.x == -1 || ptRevival.y == -1 )
	{
//		LiveReq( pBuf );
		TownPotal();
		//Close();	
		return;		
	}

	// 咯扁辑 LiveCity 风凭篮 泅犁困摹俊辑 何劝窃. 
	// 付阑俊辑 何劝窍绰巴捞 酒丛.( 内靛犁劝侩 ^^ )
	if( LiveCity( ptRevival.x, ptRevival.y ) == FALSE )
	{
		error = 3;								// 何劝且 厘家甫 给茫澜: 舅荐绝绰 俊矾
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}

	m_bLive = USER_LIVE;

	InterlockedExchange(&m_lDeadUsed, (LONG)0);
	ptClientRevival = ConvertToClient( m_curx, m_cury );

	if(m_sMagicMaxHP > m_sMaxHP) m_sHP = m_sMagicMaxHP;
	else m_sHP = m_sMaxHP;
//牛牛
	if( m_dwLastDownExp != 0 )
		GetExp( m_dwLastDownExp*50/100 );
		

	error = 0;

	TempBuf.Add( REVIVAL_RESULT );
	TempBuf.Add( error );

	// USER_INFO 俊辑 Mode 甫 猾 唱赣瘤 单捞磐 弊措肺 傈价
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);

	TempBuf.Add((short)ptClientRevival.x);
	TempBuf.Add((short)ptClientRevival.y);

	TempBuf.Add(m_iSkin);
	TempBuf.Add(m_iHair);
	TempBuf.Add((BYTE)m_sGender);
	TempBuf.AddData(m_strFace, 10);

	for( i = 0; i < EQUIP_ITEM_NUM; i++ ) TempBuf.Add(m_UserItem[i].sSid);

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sMagicMaxHP);

	TempBuf.Add(m_tDir);

	TempBuf.Add(m_dwAbnormalInfo);				// 惑怕捞惑 沥焊
	TempBuf.Add(m_dwAbnormalInfo_);
	/*TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	TempBuf.Add((BYTE)0x00);
	*/
	TempBuf.Add(m_sCityRank);

	TempBuf.Add((int)m_dwGuild);				//&&&&&&&&&&&& Test Code
	TempBuf.AddString(m_strGuildName);			// 辨靛 捞抚阑 眠啊
	TempBuf.Add(m_sGuildVersion);
	TempBuf.Add(m_byClass);
	TempBuf.Add((BYTE)m_bPkStatus);

	TempBuf.AddString(m_strLoveName);			//yskang 0.1

	for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) TempBuf.Add(m_UserItem[i].sSid);	// EBody
	if(m_UserItem[TOTAL_ITEM_NUM-2].sSid!=-1&&m_UserItem[TOTAL_ITEM_NUM-2].sDuration!=0){
		TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]));
		TempBuf.Add((BYTE)0x00);
	}else{
		TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]);
		TempBuf.Add((BYTE)0xff);
	}
	TempBuf.AddString(m_PersonalShopName);

	SendInsight(TempBuf, TempBuf.GetLength());

	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.


	//////////////////////////////////////////////////////////////////////////
	// 混妨霖 蜡历狼 府滚胶府柯 郴备档甫 憋绊 沥焊甫 焊辰促.

	// 府滚胶府柯狼 肮荐甫 皑家矫挪促.
	pUser->m_UserItem[tSlot].sCount -= 1;
	if(pUser->m_UserItem[tSlot].sCount <= 0) pUser->m_UserItem[tSlot].sSid = -1;

	// 泅犁 公霸俊辑 快钎 茄俺 公霸甫 皑家矫挪促.
	pUser->m_iCurWeight -= g_arItemTable[NORMAL_ITEM_STAMP]->m_byWeight;
	if( pUser->m_iCurWeight < 0 ) pUser->m_iCurWeight = 0;

	pUser->UpdateUserItemDN();

	error = 1;
	TempYourBuf.Add( ITEM_GIVE_RESULT );
	TempYourBuf.Add( error );
	TempYourBuf.Add( (BYTE)tSlot );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sLevel );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sSid );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sDuration );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sBullNum );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sCount );
	for(i = 0; i < MAGIC_NUM; i++) TempYourBuf.Add( pUser->m_UserItem[tSlot].tMagic[i]);
	TempYourBuf.Add( pUser->m_UserItem[tSlot].tIQ );

	pUser->Send(TempYourBuf, TempYourBuf.GetLength());

		//在BOSS洞中使用神之泪将自动回城
	if(m_curz==409){
		ReturnTown(1, 33);
		return;
	}


}


//######################################################################################
//	Cell Functions
//######################################################################################

////////////////////////////////////////////////////////////////////////////
//	CELL 狼 困摹 index setting
//
/*
BOOL USER::SetCellIndex(int zone, int xpos, int ypos)
{
	CPoint pt;

	for(int i = 0; i < g_cell.GetSize(); i++)
	{
		if( g_cell[i]->m_Zone == zone )
		{
			pt = g_cell[i]->GetCellIndex(xpos, ypos);
			if(pt.x < 0 || pt.y < 0) return FALSE;
			
			m_ptCell = pt;
			m_nCellZone = i;

			break;
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//	Cell 俊 蜡历甫 殿废茄促.
//
void USER::AddCellUid()
{
	g_cell[m_nCellZone]->AddCell(m_ptCell, m_uid + USER_BAND);
}

////////////////////////////////////////////////////////////////////////////
//	Cell 俊辑 蜡历甫 昏力茄促.
//
void USER::DeleteCellUid()
{
	g_cell[m_nCellZone]->DeleteCell(m_ptCell, m_uid + USER_BAND);
}

////////////////////////////////////////////////////////////////////////////
//	Cell 捞 函版灯阑 版快狼 贸府
//
void USER::ChangeCellUid(CPoint ptOld, CPoint ptNew)
{
	g_cell[m_nCellZone]->ChangeCell(ptOld, ptNew, m_uid + USER_BAND);
}
*/
//-------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////
//	胶懦抛捞喉俊辑 秦寸 胶懦酒捞叼俊措茄 胶懦疙阑 掘绢柯促.
//
BOOL USER::GetSkillName(int nSid, CString &strName)
{
/*	int nSize = g_arSkillTable.GetSize();

	if(nSid > nSize || nSid < 0) return FALSE;

	strName = g_arSkillTable[nSid]->m_strName;
*/
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
BOOL USER::GetSkillNameLevel(BYTE byClass, TCHAR* pSkill, TCHAR* pBuf)
{
	if(pSkill == NULL) return FALSE;

	SkillList UserSkill[TOTAL_SKILL_NUM];

	int i, index = 0;
	for(i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		UserSkill[i].sSid		= GetShort(pSkill, index);
		UserSkill[i].tLevel		= GetByte(pSkill, index);
	}

	index = 0;
	for(i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		SetShort(pBuf,UserSkill[i].sSid, index); 
		SetByte(pBuf, UserSkill[i].tLevel, index);
	}

//int nOffset = byClass * 5;
//	short sSid;
//	CString strSkillName;
/*	TCHAR	szTemp[SKILL_NAME_LENGTH];

	index = 0;
	for(i = 0; i < SKILL_NUM; i++)
	{
		sSid = UserSkill[nOffset + i].sSid;
		
//		if(!GetSkillName(sSid, strSkillName))return FALSE;

		SpaceCopy(szTemp, strSkillName, SKILL_NAME_LENGTH);

		SetString(pBuf, szTemp, SKILL_NAME_LENGTH, index);
		SetByte(pBuf, UserSkill[nOffset + i].tLevel, index);
	}
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//	ItemTable 俊辑 秦寸 酒捞袍酒捞叼俊 措茄 弊覆锅龋甫 啊廉柯促.
//
BOOL USER::GetEquipItemPid(TCHAR *pItemBuf, TCHAR *pBuf)
{
	if(pItemBuf == NULL) return FALSE;

	int nSize = g_arItemTable.GetSize();
	if(EQUIP_ITEM_NUM > nSize) return FALSE;

	ItemList UserItem[EQUIP_ITEM_NUM];

	int index = 0;
	int i, j;
	for(i = 0; i < EQUIP_ITEM_NUM; i++)
	{
		UserItem[i].sLevel		= GetShort(pItemBuf, index);
		UserItem[i].sSid		= GetShort(pItemBuf, index);
		UserItem[i].sDuration	= GetShort(pItemBuf, index);
		UserItem[i].sBullNum	= GetShort(pItemBuf, index);
		UserItem[i].sCount		= GetShort(pItemBuf, index);

		for(j = 0; j < MAGIC_NUM; j++) UserItem[i].tMagic[j] = GetByte(pItemBuf, index);

		UserItem[i].tIQ			= GetByte(pItemBuf, index);

		// 努扼捞攫飘俊 焊郴绰 厘馒 沥焊甫 父甸锭 矫府倔锅龋绰 焊郴瘤 臼栏骨肺 佬绢辑 滚赴促.
		for( j = 0; j < 8; j++ )
		{
			GetByte( pItemBuf, index );
		}
	}	
	
	short sSid = 0;
	short sPid = 0;

	index = 0;
	for(i = 0; i < EQUIP_ITEM_NUM; i++)
	{
		sSid = UserItem[i].sSid;
		if(sSid >= nSize) return FALSE;

		if(sSid != -1)
		{
			sPid = sSid;//g_arItemTable[sSid]->m_sPid;
		}
		else sPid = -1;
		
		SetShort(pBuf, sPid, index);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//	厚绢乐绰 胶懦沥焊甫 父电促.
//
void USER::MakeEmptySkill(TCHAR *pBuf, BYTE tClass)
{
	int index = 0;
	int i;

	for(i = 0; i < CLASS_NUM * SKILL_NUM; i++)
	{	
			SetShort(pBuf, i, index);				// sid
			SetByte(pBuf, 0, index);				// skill level
/*
		if(i == (tClass * 5))						// 林拌凯狼 疙吝/雀乔胶懦 窍唱绰 扁夯利栏肺 霖促.
			SetByte(pBuf, 1, index);				// skill level
		else 
			SetByte(pBuf, 0, index);		

		SetByte(pBuf, 0 , index);					// OnOff
*/	}
}

//////////////////////////////////////////////////////////////////////////
void USER::MakeInintItem(TCHAR *pBuf, BYTE tClass)
{
	int index = 0;
	int i, j;
	int sSid = 0, sSid2 = 0, sSid3 = 0, sSid4 = 0, sSubSid = 0;

	InitUserItem();

	for(i = 0; i < g_arInitItem.GetSize(); i++)
	{
		if(tClass == g_arInitItem[i]->tClass) 
		{ 
			sSid = g_arInitItem[i]->iSid; 
			sSubSid = g_arInitItem[i]->iSubSid;
			break; 
		}
	}

	m_UserItem[EQUIP_ITEM_NUM].sLevel = g_arItemTable[sSid]->m_byRLevel;
	m_UserItem[EQUIP_ITEM_NUM].sSid = g_arItemTable[sSid]->m_sSid;
	m_UserItem[EQUIP_ITEM_NUM].sDuration = g_arItemTable[sSid]->m_sDuration;
	m_UserItem[EQUIP_ITEM_NUM].sBullNum = g_arItemTable[sSid]->m_sBullNum;
	m_UserItem[EQUIP_ITEM_NUM].sCount = 1;
	for(j = 0; j < MAGIC_NUM; j++) m_UserItem[EQUIP_ITEM_NUM].tMagic[j] = 0;
	m_UserItem[EQUIP_ITEM_NUM].tIQ = 0;
	m_UserItem[EQUIP_ITEM_NUM].iItemSerial = 0;

	switch(tClass)
	{
	case EDGED:
		m_UserItem[EQUIP_ITEM_NUM].sBullNum = g_arItemTable[sSubSid]->m_sBullNum;
	break;

	case FIREARMS:
		m_UserItem[EQUIP_ITEM_NUM].sBullNum = g_arItemTable[sSubSid]->m_sBullNum;
/*		m_UserItem[EQUIP_ITEM_NUM + 1].sLevel = g_arItemTable[sSid]->m_byRLevel;
		m_UserItem[EQUIP_ITEM_NUM + 1].sSid = g_arItemTable[sSubSid]->m_sSid;
		m_UserItem[EQUIP_ITEM_NUM + 1].sDuration = g_arItemTable[sSubSid]->m_sDuration;
		m_UserItem[EQUIP_ITEM_NUM + 1].sBullNum = g_arItemTable[sSubSid]->m_sBullNum;
		m_UserItem[EQUIP_ITEM_NUM + 1].sCount = 1;
		for(j = 0; j < MAGIC_NUM; j++) m_UserItem[EQUIP_ITEM_NUM  +1].tMagic[j] = 0;
		m_UserItem[EQUIP_ITEM_NUM + 1].tIQ = 0;
*/		break;
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{	
		SetShort(pBuf, 0, index);		// Level
		SetShort(pBuf, -1, index);		// sid
		SetShort(pBuf, 0, index);		// BullNum
		SetShort(pBuf, 0, index);		// Count
		SetShort(pBuf, 0, index);		// Duration

		for(j = 0; j < MAGIC_NUM; j++) 
			SetByte(pBuf, 0, index);	// Magic Attribute

		SetByte(pBuf, 0, index);		// Item Quality

		for(j = 0; j < 8; j++ )			// Item Serial
		{
			SetByte(pBuf, 0, index );
		}
	}

	UserItemToStr(pBuf);
}

//////////////////////////////////////////////////////////////////////////
//	厚绢乐绰 荤捞坷葱 沥焊甫 父电促.
//
void USER::MakeEmptyPsi(TCHAR *pBuf)
{
	int index = 0;

	SetShort(pBuf, -1, index);		// sid
									// 荤捞坷葱 滚欺绰 -1甫 父朝锭鳖瘤 佬绰促. 唱赣瘤绰 葛滴 0肺 檬扁拳
	SetByte(pBuf, 0 , index);		// OnOff
}

//////////////////////////////////////////////////////////////////////////
//	PA or MA Point 甫 掘扁 困茄 促澜 版氰摹甫 备茄促.
//
DWORD USER::GetNextPAMAExp(DWORD dwExp)
{
	int i;
	int nSize = g_arPAMAExp.GetSize();

	for(i = 0; i < nSize; i++)
	{
		if(g_arPAMAExp[i]->m_dwPAMAExp > dwExp) return g_arPAMAExp[i]->m_dwPAMAExp;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//	促澜 饭骇捞 登扁 困茄 版氰摹甫 备茄促.
//
DWORD USER::GetNextLevel(int iCurrLevel)
{
	int nSize = g_arLevelUpTable.GetSize();

	if(iCurrLevel >= nSize) return 0;

	return (DWORD)(g_arLevelUpTable[iCurrLevel]->m_dwExp);
}

////////////////////////////////////////////////////////////////////////////
//	秦寸 胶懦狼 沥焊甫 备茄促. (泅犁绰 己傍啦父 备茄促.)
//
void USER::GetSkillInfo(int sid, BYTE& tRate)
{
/*	int i = 0;
	int nOnSkill = 0;

	if(m_UserSkill[sid].tOnOff == 0)
	{
		tRate = 0;
	}
	else
	{
		for(i = 0; i < SKILL_NUM; i++)
		{
			if(m_UserSkill[m_byClass * SKILL_NUM + i].tOnOff == 1) nOnSkill++;
		}

		tRate = (BYTE)((float)m_sWIS * 0.5 + m_UserSkill[sid].tLevel * 2 - nOnSkill * 25 + 50);
	}
*/
}

////////////////////////////////////////////////////////////////////////////
//	秦寸 胶懦狼 己傍啦阑 备茄促. (函版凳. 泅犁 救靖)
//
BYTE USER::GetSkillRate(BYTE byClass, int sid, int nDefusion)
{
	int nSelect = 0;
/*
	for(int i = 0; i < SKILL_NUM; i++)
	{
		if(m_UserSkill[byClass * SKILL_NUM + i].tOnOff == 1) nSelect++;
	}
*/
//!	BYTE byRate = (BYTE)((double)m_sWIS * 0.3 + (double)m_UserSkill[sid].sLevel * 3.5 - nSelect * 25 + 40 - nDefusion);
	BYTE byRate = 1;

	return byRate;
}

////////////////////////////////////////////////////////////////////////////
//	Trade Mode 甫 On/Off 茄促
//
void USER::SetTradeMode(TCHAR *pBuf)
{
/*	int index = 0;
	BOOL bMode = (BOOL)GetByte(pBuf, index);

	m_bTradeMode = bMode;

	CBufferEx TempBuf;

	TempBuf.Add(TRADE_MODE_RESULT);
	TempBuf.Add((BYTE)m_bTradeMode);

	Send(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////
//	酒捞袍阑 EQUIP, INVENTORY, QUICK 浇吩埃俊 捞悼 矫挪促.
//
void USER::ItemMoveReq(TCHAR *pBuf) //移动物品请求
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return;//yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int	index = 0;
	int i;
	short sOldSid = 0, sNewSid = 0;
	int nOldPos = 0, nNewPos = 0;
	BYTE byOldWear = 255, byNewWear = 255;

	BYTE tType = GetByte(pBuf, index);			// 老馆 酒捞袍苞 狞 酒捞袍苞狼 备盒

	if(tType == 1 || tType == 2) return;		// 狞酒捞袍 率篮 贸府窍瘤 臼绰促
	
	BYTE byOldSlot = GetByte(pBuf, index);		// 盔贰酒捞袍 困摹
	BYTE byNewSlot = GetByte(pBuf, index);		// 颗扁妨绰 酒捞袍 困摹

	if(byOldSlot < 0 || byOldSlot >= TOTAL_ITEM_NUM || byNewSlot < 0 || byNewSlot >= TOTAL_ITEM_NUM) 
	{
		SendItemMoveFail();
		return;
	}
	if (m_bNowTrading == TRUE || m_bPShopOpen == TRUE)
	{
        SendItemMoveFail();
		return;
	}
	if (m_bNowTrading == TRUE || m_bPShopOpen == TRUE) 
	   {
            CString strDate ="";
			SYSTEMTIME st;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
			TCHAR m_Log[500];
			sprintf_s(m_Log,"[ %s ]%s %d,%d 物品移动存在非法1 \r\n",strDate,m_strUserID,m_bNowTrading,m_bPShopOpen);
		    WriteUserShopLog(m_Log);
			SendItemMoveFail();
		    return;
	 }   
	
	
	sOldSid = m_UserItem[byOldSlot].sSid;
	sNewSid = m_UserItem[byNewSlot].sSid;
	
	if(sOldSid <= -1)							// 家胶啊 厚菌栏搁 角菩
	{
		SendItemMoveFail();
		return;
	}
	
	if(sOldSid >= 0 && sOldSid < g_arItemTable.GetSize()) 
	{
		byOldWear = g_arItemTable[sOldSid]->m_byWear;
	}
	
	if(sNewSid >= 0 && sNewSid < g_arItemTable.GetSize()) 
	{
		byNewWear = g_arItemTable[sNewSid]->m_byWear;
	}
	
	nOldPos = GetSlotClass(byOldSlot);//老的装备栏或机械栏或机甲栏 0 1 2
	nNewPos = GetSlotClass(byNewSlot); //新的装备栏或机械栏或机甲栏
	
	if(nOldPos == -1 || nNewPos == -1)			// 秦寸窍绰 浇吩捞 绝绰 版快
	{
		SendItemMoveFail();
		return;
	}

	short	sChangeSlot[3];
	for(i = 0; i < 3; i++)
	{
		sChangeSlot[i] = 255;
	}

	ItemList	TempItem;
	BOOL		bChangeEquip = FALSE;
	int			iSearchSlot = -1;
	int			iSameSlot = -1;
	BOOL		bEmpty01 = FALSE, bEmpty02 = FALSE;
	CPoint		ptEquipPos(-1, -1);

	int			iItemIndex = 0, iMagicIndex = 0;

	int			iEBodyPos = -1;
	
	switch (nOldPos)
	{
	case 0:						// EQUIP
		switch(nNewPos)
		{
		case 0:					// E -> E
			if(byOldWear != byNewWear)	// 取到的装备或武器类型
			{
				if(byOldWear != 1 && byOldWear != 8)
				{
					SendItemMoveFail();
					return;
				}
				if(
					(byOldSlot == 4 && byNewSlot != 5) ||
				    (byOldSlot == 5 && byNewSlot != 4) ||
				    (byOldSlot == 6 && byNewSlot != 7) ||
				    (byOldSlot == 7 && byNewSlot != 6)	)
				{
					SendItemMoveFail();
					return;
				}
			}
		
			iItemIndex = GetItemIndex(m_UserItem[byOldSlot].sSid);
			if(iItemIndex < 0) { SendItemMoveFail(); return; }
			m_UserItem[byOldSlot].sLevel = g_arItemTable[iItemIndex]->m_byRLevel;

			if(m_UserItem[byOldSlot].tIQ > NORMAL_ITEM)
			{
				if(m_UserItem[byOldSlot].tIQ == UNIQUE_ITEM) iItemIndex = MAGIC_NUM;	// 弥措 八祸 冉荐
				else iItemIndex = 4;								// 蜡聪农客 悸飘 酒捞袍阑 猾 酒捞袍篮 弥措 4俺鳖瘤父 浇吩 荤侩

				for(i = 0; i < iItemIndex; i++)
				{
					iMagicIndex = m_UserItem[byOldSlot].tMagic[i];
					if(iMagicIndex >= 0 && iMagicIndex < g_arMagicItemTable.GetSize())
					{
						m_UserItem[byOldSlot].sLevel += g_arMagicItemTable[iMagicIndex]->m_tLevel;
					}
				}
			}

			if(m_UserItem[byOldSlot].sLevel > m_sLevel) { SendItemMoveFail(); return; }

			if(!IsCanEquipItem((int)byOldSlot)) { SendItemMoveFail(); return; }

			TempItem = m_UserItem[byOldSlot];                   //身上装备交换
			m_UserItem[byOldSlot] = m_UserItem[byNewSlot];
			m_UserItem[byNewSlot] = TempItem;

			sChangeSlot[0] = byOldSlot;			
			sChangeSlot[1] = byNewSlot;

			if(byOldWear == 1 || byNewWear == 1) bChangeEquip = TRUE;
			break;
		case 1:		// E -> I
			if(byOldWear >= 9 || m_UserItem[byOldSlot].sCount > 1)		// if( count > 1 ) FAIL;
			{
				m_UserItem[byOldSlot].sCount = 1;
				SendItemMoveFail();
				return;
			}

			if(byOldWear >= 6)							// 郴备档啊 绝绰 版快 牢亥俊 般磨荐 乐促.	
			{
				iSameSlot = GetSameItem(m_UserItem[byOldSlot], INVENTORY_SLOT);	 
				if(iSameSlot != -1)						// 牢亥配府俊 鞍篮 酒捞袍捞 粮犁窍搁
				{
					if(m_UserItem[iSameSlot].sCount <= 0 || m_UserItem[iSameSlot].sCount >= _MAX_SHORT) { SendItemMoveFail(); return; }

					ReSetItemSlot(&m_UserItem[byOldSlot]);
					CheckMaxValue((short &)m_UserItem[iSameSlot].sCount, (short)1);

					sChangeSlot[0] = byOldSlot;			
					sChangeSlot[1] = iSameSlot;
					if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
					break;
				}
			}
														// 困 炼扒阑 力寇窍搁 公炼扒 后 浇吩俊 窍唱究 措涝.
			if(m_UserItem[byNewSlot].sSid == -1) iSearchSlot = byNewSlot;	
			else	iSearchSlot = GetEmptySlot(INVENTORY_SLOT);

			if(iSearchSlot == -1)						// 蜡历啊 夸没茄 瘤痢捞 厚菌栏搁..
			{
				SendItemMoveFail();
				return;
			}

			m_UserItem[iSearchSlot] = m_UserItem[byOldSlot];
//			ReSetItemSlot(byOldSlot);
			ReSetItemSlot(&m_UserItem[byOldSlot]);

			sChangeSlot[0] = byOldSlot;			
			sChangeSlot[1] = iSearchSlot;
			if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
			break;
		case 2:		// E -> EBody
			SendItemMoveFail();
			return;
		default:
			SendItemMoveFail();
			return;
		};
		break;
	case 1:			//INVENTORY
		switch(nNewPos)
		{
		case 0:		// I -> E
			if(byOldWear >= 1 && byOldWear <= 8)	// 厘厚芒栏肺 颗辨 荐 乐绰 酒捞袍捞搁
			{
				if(!IsCanEquipItem((int)byOldSlot)) 
				{ 
					SendItemMoveFail(); 
					return; 
				}

				if(m_UserItem[byOldSlot].sCount > 1)	// 家胶啊 1俺甫 檬苞窍搁
				{
					bEmpty01 = bEmpty02 = FALSE;

					GetEquipPos(byOldWear, ptEquipPos);

					if(ptEquipPos.x != -1)
					{
						if(m_UserItem[ptEquipPos.x].sSid == -1) bEmpty01 = TRUE;
					}
					if(ptEquipPos.y != -1)
					{
						if(m_UserItem[ptEquipPos.y].sSid == -1) bEmpty02 = TRUE;
					}

					iItemIndex = GetItemIndex(m_UserItem[byOldSlot].sSid);
					if(iItemIndex < 0) { SendItemMoveFail(); return; }
					m_UserItem[byOldSlot].sLevel = g_arItemTable[iItemIndex]->m_byRLevel;

					if(m_UserItem[byOldSlot].tIQ > NORMAL_ITEM)
					{
						if(m_UserItem[byOldSlot].tIQ == UNIQUE_ITEM) iItemIndex = MAGIC_NUM;	// 弥措 八祸 冉荐
						else iItemIndex = 4;								// 蜡聪农客 悸飘 酒捞袍阑 猾 酒捞袍篮 弥措 4俺鳖瘤父 浇吩 荤侩

						for(i = 0; i < iItemIndex; i++)
						{
							iMagicIndex = m_UserItem[byOldSlot].tMagic[i];
							if(iMagicIndex >= 0 && iMagicIndex < g_arMagicItemTable.GetSize())
							{
								m_UserItem[byOldSlot].sLevel += g_arMagicItemTable[iMagicIndex]->m_tLevel;
							}
						}
					}

					if(m_UserItem[byOldSlot].sLevel > m_sLevel) { SendItemMoveFail(); return; }

					if(bEmpty01)
					{
						m_UserItem[ptEquipPos.x] = m_UserItem[byOldSlot];
						m_UserItem[ptEquipPos.x].sCount = 1;

						m_UserItem[byOldSlot].sCount--;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)ptEquipPos.x;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}

					if(bEmpty02)
					{
						m_UserItem[ptEquipPos.y] = m_UserItem[byOldSlot];
						m_UserItem[ptEquipPos.y].sCount = 1;

						m_UserItem[byOldSlot].sCount--;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)ptEquipPos.y;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}

					if(byNewSlot == (BYTE)ptEquipPos.y) ptEquipPos.x = ptEquipPos.y;
					iSearchSlot = GetEmptySlot(INVENTORY_SLOT);
					iSameSlot = GetSameItem(m_UserItem[ptEquipPos.x], INVENTORY_SLOT);

					if(iSearchSlot == -1 && iSameSlot == -1) 
					{
						SendItemMoveFail();
						return;
					}
					
					if(iSameSlot != -1)
					{
						m_UserItem[iSameSlot].sCount++;

						m_UserItem[ptEquipPos.x] = m_UserItem[byOldSlot];
						m_UserItem[ptEquipPos.x].sCount = 1;

						m_UserItem[byOldSlot].sCount--;

						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)iSameSlot;
						sChangeSlot[2] = (BYTE)ptEquipPos.x;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}
					
					if(iSearchSlot != -1)
					{
						m_UserItem[iSearchSlot] = m_UserItem[ptEquipPos.x];
						
						m_UserItem[ptEquipPos.x] = m_UserItem[byOldSlot];
						m_UserItem[ptEquipPos.x].sCount = 1;
						
						m_UserItem[byOldSlot].sCount--;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)iSearchSlot;
						sChangeSlot[2] = (BYTE)ptEquipPos.x;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}
				}
				else									// 家胶啊 1俺搁
				{
					bEmpty01 = bEmpty02 = FALSE;

					GetEquipPos(byOldWear, ptEquipPos);

					if(ptEquipPos.x != -1)
					{
						if(m_UserItem[ptEquipPos.x].sSid == -1) bEmpty01 = TRUE;
					}
					if(ptEquipPos.y != -1)
					{
						if(m_UserItem[ptEquipPos.y].sSid == -1) bEmpty02 = TRUE;
					}

					iItemIndex = GetItemIndex(m_UserItem[byOldSlot].sSid);
					if(iItemIndex < 0) { SendItemMoveFail(); return; }
					m_UserItem[byOldSlot].sLevel = g_arItemTable[iItemIndex]->m_byRLevel;

					if(m_UserItem[byOldSlot].tIQ > NORMAL_ITEM)
					{
						if(m_UserItem[byOldSlot].tIQ == UNIQUE_ITEM) iItemIndex = MAGIC_NUM;	// 弥措 八祸 冉荐
						else iItemIndex = 4;								// 蜡聪农客 悸飘 酒捞袍阑 猾 酒捞袍篮 弥措 4俺鳖瘤父 浇吩 荤侩

						for(i = 0; i < iItemIndex; i++)
						{
							iMagicIndex = m_UserItem[byOldSlot].tMagic[i];
							if(iMagicIndex >= 0 && iMagicIndex < g_arMagicItemTable.GetSize())
							{
								m_UserItem[byOldSlot].sLevel += g_arMagicItemTable[iMagicIndex]->m_tLevel;
							}
						}
					}

					if(m_UserItem[byOldSlot].sLevel > m_sLevel) { SendItemMoveFail(); return; }

					if(bEmpty01)
					{
						TempItem = m_UserItem[byOldSlot];
						m_UserItem[byOldSlot] = m_UserItem[ptEquipPos.x];
						m_UserItem[ptEquipPos.x] = TempItem;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)ptEquipPos.x;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}
					if(bEmpty02)
					{
						TempItem = m_UserItem[byOldSlot];
						m_UserItem[byOldSlot] = m_UserItem[ptEquipPos.y];
						m_UserItem[ptEquipPos.y] = TempItem;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)ptEquipPos.y;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}

					TempItem = m_UserItem[byOldSlot];
					m_UserItem[byOldSlot] = m_UserItem[ptEquipPos.x];
					m_UserItem[ptEquipPos.x] = TempItem;
					
					sChangeSlot[0] = byOldSlot;			
					sChangeSlot[1] = (BYTE)ptEquipPos.x;
					
					if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
				}
			}
			else									// 厘厚芒栏肺 颗辨 荐 绝绰 酒捞袍捞搁
			{
				SendItemMoveFail();
				return;
			}
			break;
		case 1:		// I -> I
			TempItem = m_UserItem[byOldSlot];
			m_UserItem[byOldSlot] = m_UserItem[byNewSlot];
			m_UserItem[byNewSlot] = TempItem;
			
			sChangeSlot[0] = byOldSlot;			
			sChangeSlot[1] = byNewSlot;		
			break;
		case 2:		// I -> EBody (牢亥俊辑 EBody 浇吩栏肺 肺 颗扁扁)
			if(m_iDisplayType == 5)
			{
				SendSystemMsg("公丰 荤侩磊绰 EBody甫 荤侩且 荐 绝嚼聪促.", SYSTEM_NORMAL, TO_ME);
				SendItemMoveFail();
				return;
			}
			if ( o_yehuoini[0]->jxrw ==1 )
		   {
			if((byOldWear!=130) && FindEvent(16) == FALSE) 
			{
				SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
				SendItemMoveFail();
				return;
			}
		}
			//机械和守护
			if((byOldWear >= 122 && byOldWear <= 125)||(byOldWear==130))	// EBody 肺 颗辨 荐 乐绰 酒捞袍捞搁
			{
				if(byOldWear==130)
				{
					m_UserItem[byOldSlot].sLevel=40;
				}
				if(m_UserItem[byOldSlot].sCount != 1)	// 家胶啊 1俺啊 酒聪搁
				{
					SendItemMoveFail();
					return;
				}

				bEmpty01 = bEmpty02 = FALSE;
				iEBodyPos = GetEBodyPos(byOldWear);
				if(iEBodyPos == -1)
				{
					SendItemMoveFail();
					return;
				}
				
				iItemIndex = m_UserItem[byOldSlot].sSid;
				if(iItemIndex < 0 || iItemIndex >= g_arItemTable.GetSize()) 
				{
					SendItemMoveFail(); 
					return;
				}
				m_UserItem[byOldSlot].sLevel = g_arItemTable[iItemIndex]->m_byRLevel;
				
				if(m_UserItem[byOldSlot].tIQ == MAGIC_ITEM)	// EBody 绰 泅犁 葛滴 概流
				{
					iItemIndex = 5; // EBody 绰 可记捞 5俺
					
					for(i = 1; i < iItemIndex; i++)		// 海捞胶 可记篮 眉农窍瘤 臼绰促.
					{
						iMagicIndex = m_UserItem[byOldSlot].tMagic[i];
						if(iMagicIndex >= 0 && iMagicIndex < g_arEBodyTable.GetSize())
						{
							m_UserItem[byOldSlot].sLevel += g_arEBodyTable[iMagicIndex]->m_tLevel;
						}
					}
				}
				
				if(m_UserItem[byOldSlot].sLevel > m_sLevel) { SendItemMoveFail(); return; }
				
				TempItem = m_UserItem[byOldSlot];
				m_UserItem[byOldSlot] = m_UserItem[iEBodyPos];
				m_UserItem[iEBodyPos] = TempItem;
					
				sChangeSlot[0] = byOldSlot;			
				sChangeSlot[1] = iEBodyPos;
					
				bChangeEquip = TRUE;
				break;
			}
			else SendItemMoveFail();
			break;
		default:
			SendItemMoveFail();
			return;
		};
		break;

	case 2:			// EBody
		switch(nNewPos)
		{
		case 0:		// EBody -> Equip
			SendItemMoveFail();
			return;
			break;
		case 1:		// EBody -> Inven
			if(m_UserItem[byNewSlot].sSid == -1) iSearchSlot = byNewSlot;	
			else	iSearchSlot = GetEmptySlot(INVENTORY_SLOT);

			if(iSearchSlot == -1)	// 厚绢乐绰 浇吩阑 惯斑窍瘤 给沁栏搁
			{
				SendItemMoveFail();
				return;
			}

			m_UserItem[iSearchSlot] = m_UserItem[byOldSlot];
			ReSetItemSlot(&m_UserItem[byOldSlot]);

			sChangeSlot[0] = byOldSlot;			
			sChangeSlot[1] = iSearchSlot;
			bChangeEquip = TRUE;

			break;
		case 2:		// EBody -> EBody
			SendItemMoveFail();
			return;
		default:
			break;
		};
		break;
	default:
		SendItemMoveFail();
		return;
	};	

	BYTE nCount = 0;
	for(i = 0; i < 3; i++)
	{
		if(sChangeSlot[i] != 255) nCount++;
	}

	BYTE bySlot;
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(tType);

	if(bChangeEquip)
	{
		
		SendMyWeaponChangeInfo();
		//重新计算装备属性..
		CheckMagicItemMove();
		SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	}

	if(tType == 0)
	{
		TempBuf.Add(nCount);

		for(i = 0; i < 3; i++)
		{
			bySlot = (BYTE)sChangeSlot[i];
			
			if(bySlot == 255) break;
			TempBuf.Add(bySlot);
			TempBuf.Add(m_UserItem[bySlot].sLevel);
			TempBuf.Add(m_UserItem[bySlot].sSid);
			TempBuf.Add(m_UserItem[bySlot].sDuration);
			TempBuf.Add(m_UserItem[bySlot].sBullNum);
			TempBuf.Add(m_UserItem[bySlot].sCount);

			for(int j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

			TempBuf.Add(m_UserItem[bySlot].tIQ); 
		}
	}

	UpdateUserItemDN();

	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
//	秦寸 酒捞袍捞 厘馒瞪锭 概流加己俊 狼茄 蔼捞 官差绰瘤 眉农
//
void USER::CheckMagicItemMove()			//^^ 唱吝俊 弥利拳窍磊...
{
	int i, j,t=0;
	int nCount = 4;						// 概流, 饭绢老锭 曼炼 冉荐绰 4锅鳖瘤
	int iMagic = 0;
	int iMagicCount = 0;
	int ebody[5]={0,0,0,0,0};
	BOOL ebody_skill=0;
	BOOL bOldShield = CheckAbnormalInfo(ABNORMAL_SHIELD) || CheckAbnormalInfo(ABNORMAL_BIGSHIELD);
	CBufferEx TempBuf;

	for(i = 0; i < MAGIC_COUNT; i++) m_DynamicUserData[i] = 0;
	for(i = 0; i < EBODY_MAGIC_COUNT; i++) m_DynamicEBodyData[i] = 0;
	for(i = 0; i < HUANSHI_MAGIC_COUNT; i++) m_DynamicMagicItem[i]= 0;
    for(i = 0; i < 10; i++) m_DynamicMagicItem[i]= 0;
	for(i = 0; i < MS_MAGIC_COUNT; i++)	m_DynamicSuit[i] = 0;// 程序卡
	for(i = 0;i<2;i++) m_Dynamic140[i]=0;//140技能
	m_nHPPoint = 0;
	for(i = 0; i < EQUIP_ITEM_NUM; i++)
	{
		if(m_UserItem[i].sSid < 0 || m_UserItem[i].sSid >= g_arItemTable.GetSize()) continue;	// 规绢内靛

		if(i == BODY || i == PANTS)
		{
			if(m_byClass == 3)
			{
				if(g_arItemTable[m_UserItem[i].sSid]->m_byRLevel == 80 && g_arItemTable[m_UserItem[i].sSid]->m_byClass == 1)
				{
					m_DynamicUserData[MAGIC_MAX_HP_UP] += g_arItemTable[m_UserItem[i].sSid]->m_byMPP;
				}
			}
		}

		if(i == LEFT_HAND) continue;

		if((m_UserItem[i].tIQ >= REMODEL_ITEM && m_UserItem[i].tIQ <= REMODEL_MAGIC_ITEM)||m_UserItem[i].tIQ <= 12||m_UserItem[i].tIQ <= 18)  // 老窜 畴钢捞 酒聪搁...
		{
			switch(m_UserItem[i].tIQ)
			{
			case MAGIC_ITEM:
				nCount = 2;
				break;
			case RARE_ITEM:
				nCount = 4;
				break;
			case UNIQUE_ITEM:
				nCount = MAGIC_NUM;
				break;
			case SET_ITEM:
				nCount = 4;
				break;
			case REMODEL_ITEM:
			case REMODEL_MAGIC_ITEM:
				nCount = 3;
				break;
			case 12:
				nCount= MAGIC_NUM;
				break;
			case 18:
				nCount= MAGIC_NUM;
				break;
			default:
				nCount = 4;
				break;
			}
            if(m_UserItem[i].tIQ == MAGIC2_ITEM || m_UserItem[i].tIQ == 12 || m_UserItem[i].tIQ == 18)
			{
				if(m_UserItem[i].tMagic[5] <= 6) nCount = (MAGIC_NUM - 4);
				else if(m_UserItem[i].tMagic[5] >= 7) nCount = MAGIC_NUM;
			}
			for(j = 0; j < nCount; j++)
			{
                if(j < 6) //修复最终幻石 加释放
				{
				iMagic = m_UserItem[i].tMagic[j];
				if(iMagic < 0 || iMagic >= g_arMagicItemTable.GetSize()) continue;

				iMagicCount = g_arMagicItemTable[iMagic]->m_sSubType;
				if(iMagicCount < 0 || iMagicCount >= MAGIC_COUNT) continue;
				
				m_DynamicUserData[iMagicCount] += g_arMagicItemTable[iMagic]->GetMagicItemValue();
				}
			//	if(j >=6)//属性生效  修复最终幻石 加释放
				else 
				{										
					iMagic = m_UserItem[i].tMagic[j];
					if(iMagic < 3 || iMagic >= g_arHuanshiTable.GetSize()) continue;
					
					iMagicCount = g_arHuanshiTable[iMagic]->m_sSubType; //属性类型					
					if(iMagicCount < 0 || iMagicCount >= MAGIC_COUNT) continue;	
					
					m_DynamicMagicItem[iMagicCount] += g_arHuanshiTable[iMagic]->GetMagicItemValue1();	
				}	
			}
		}
		//////////////////////////////////NEO版本需要开放的

				if (o_yehuoini[0]->bbzh == 1)
	{

		//是否为10改以上,右手武器装备增加技能点,装备增加血百分比
		BYTE bupg = m_UserItem[i].tMagic[ITEM_UPGRADE_COUNT];
		if(bupg>0)
		{
			if(i == RIGHT_HAND)
			{
				bupg += GetGuardianAddWeapUpgrade();//检测是否佩带武器守护
				//武器 130 5改开始+3技能点,其他10改开始+3点技能
				if(Is130Wearp(m_UserItem[i].sSid)&&bupg>=5)
				{
					AddMagicPoint((bupg-4)*3);
				}else if(bupg>=10)
				{
					AddMagicPoint((bupg-9)*3);
				}

			}else if(i == BODY || i == PANTS||i == HEAD || i == SHOES)
			{
				//130级从3改开始加3%,100级从8改开始改2%血,70级以下从10开始加2%血
				if(Is130Item(m_UserItem[i].sSid)&&bupg>=3)
				{
					AddMaxHPPoint((bupg-2)*3);
				}else if(Is100Item(m_UserItem[i].sSid)&&bupg>=8)
				{
					AddMaxHPPoint((bupg-7)*2);
				}
				else if(bupg>=10)
				{
					AddMaxHPPoint((bupg-9)*2);
				}
			}
		}
	}	
}

//计算机械属性
	nCount = 5;
	BYTE tValue = 0;
	for(i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++)	// E-Body Magic 加己 眠啊
	{
		if(m_UserItem[i].sSid >= 0)			
		{
				
			if(m_UserItem[i].tIQ == MAGIC_ITEM || m_UserItem[i].tIQ == SUPER_EBODY_ITEM)//蓝色和超级机械15
			{
				for(j = 0; j < nCount; j++)				// 浇吩 荐父怒 倒赴促. 
				{				
					iMagic = m_UserItem[i].tMagic[j];
					if(iMagic < 0 || iMagic >= g_arEBodyTable.GetSize()) continue;
					
					iMagicCount = g_arEBodyTable[iMagic]->m_sSubType;
					if(iMagicCount < 0 || iMagicCount >= EBODY_MAGIC_COUNT) continue;
					
					tValue = (BYTE)g_arEBodyTable[iMagic]->m_sChangeValue;
					m_DynamicEBodyData[iMagicCount] += g_arEBodyTable[iMagic]->m_sChangeValue;
				}
				BYTE upg = m_UserItem[i].tMagic[5];
			{
			if(m_UserItem[i].sSid==987 ||m_UserItem[i].sSid==1270)
			{
				//最终伤增加 前9改*6,后面*9加一点技能点
				if(m_UserItem[i].sSid==987)
				{
					if(upg<=9)
					{
					m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= upg*6;
					}else{
					m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= (upg-9)*9+9*6;
								//加技能点
						AddMagicPoint(upg-9);
					  }
				}else if(m_UserItem[i].sSid==1270)						
					{//超级机械加42基础,再加1-7改每改的加12最终伤害,8改开始加15最终伤害,再加一点技能点
						if(upg<=7)
						{
							m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= 12*upg+42;
						}
						else
						{
							m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= 15*(upg-7)+7*12+42;
							AddMagicPoint(upg-7);
						}
					}
				}else
				{ 
						//超级机械 14伤害减少,1-7改每改减少3最终攻击减少,加10血,8改开始加4点攻击力减少,15点血
						//m_DynamicMagicItem[5]=m_DynamicMagicItem[5] + m_UserItem[i].tMagic[5]*2;
					if(IsSuperEbodyItem(m_UserItem[i].sSid))
					{
						if(upg<=7)
						{
							m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= upg*3+14;
								//加10点血
							m_DynamicUserData[MAGIC_MAX_HP_UP]+=upg*10;
						}else
						{
							m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= (upg-7)*4+7*3+14;
								//加15点血
							m_DynamicUserData[MAGIC_MAX_HP_UP]+=(upg-7)*15+70;
						}

				    }else
					{
							//最终伤减少前9改*2,后*3加hp5点非5%
							if(upg<=9)
							{
								m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= upg*2;
							}else
							{
								m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= (upg-9)*3+9*2;
								//加5点血
								m_DynamicUserData[MAGIC_MAX_HP_UP]+=(upg-9)*5;
							}
					}
			    }
			}
		}
	}
}

		
	
	//计算守护属性
	GetMagicItemSetting_38();
	GetSkillSetting_130();//计算130技能
	GetSkillSetting_140();
	GetSkillSetting();
	GetJiSetting();
	GetBabySetting();//宠物技能
	
    m_DynamicUserData[MAGIC_DEFENSE_UP] +=m_DynamicMagicItem[H_DEFINE];
	m_DynamicUserData[MAGIC_PSI_RESIST_UP] +=m_DynamicMagicItem[H_PSI_DEFINE];
	m_DynamicUserData[MAGIC_MAX_HP_UP] +=m_DynamicMagicItem[H_HP];
	m_DynamicUserData[MAGIC_AVOID_RATING_UP] +=m_DynamicMagicItem[H_HUIBI];	

	// 家葛己 厩技荤府 眉农
	m_bRingOfLife		= FALSE;
	m_bNecklaceOfShield	= FALSE;
	m_bEarringOfProtect = FALSE;
	m_bCBOff	    	= FALSE;
        for(i = 0; i < MAX_ACCESSORI; i++)
	{
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_RING_OF_LIFE)		m_bRingOfLife = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_NECKLACE_OF_SHIELD)	m_bNecklaceOfShield = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_PROTECT)	m_bEarringOfProtect = TRUE;
	        if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_FEIFEI)	m_bCBOff = TRUE;
        }

	if(m_bNecklaceOfShield == TRUE) 
	{
		AddAbnormalInfo(ABNORMAL_SHIELD);
		if(bOldShield == FALSE)
		{
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);

			SendInsight(TempBuf, TempBuf.GetLength());
//			SendExactScreen(TempBuf, TempBuf.GetLength());
		}
	}
	else
	{
		if(bOldShield == TRUE && m_dwShieldTime == 0)
		{
			DeleteAbnormalInfo(ABNORMAL_SHIELD);
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);
//			SendInsight(TempBuf, TempBuf.GetLength());
			SendExactScreen(TempBuf, TempBuf.GetLength());
		}
	}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>翅膀戒子
        if(isGuildRen == TRUE || m_bCBOff == TRUE) 
	{
	//m_dwAbnormalInfo_	= 448;
	m_dwAbnormalInfo_ |= ABNORMAL_POISON;
	FreedomCB();
		}
	else 
	{
		if(isGuildRen == FALSE || m_bCBOff == FALSE)
		{
	m_dwAbnormalInfo_	= ABNORMAL_NONE;
	m_dwAbnormalInfo_ |= ABNORMAL_POISON;
	FreedomCB();
	   }
	}	
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>	
	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP)) SetUserToMagicUser(1);	// MAX_HP_UP捞搁 拌加 蜡瘤矫难霖促.
	else									  SetUserToMagicUser();
  
	SendUserStatusSkill();
}

/////////////////////////////////////////////////////////////////////////////
//	秦寸 酒捞袍 浇吩狼 酒捞袍捞 蜡历 努贰胶俊辑 荤侩且荐 乐绰 公扁牢瘤 魄窜茄促.
//
BOOL USER::IsCanEquipItem(int iSlot)
{
	int i;
	BOOL bflag = FALSE;
	CString strMsg;

	int sid = m_UserItem[iSlot].sSid;
	int armClass = g_arItemTable[sid]->m_byClass;

	if(sid < 0 || sid >= g_arItemTable.GetSize()) return FALSE;

	short sNeedStr = g_arItemTable[sid]->m_byRstr;
	short sNeedDex = g_arItemTable[sid]->m_byRdex;
	short sNeedVol = g_arItemTable[sid]->m_byRvol;

	switch(m_byClass)
	{
	case BRAWL:
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_BrawlClass[i] == armClass)
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol) { bflag = TRUE; goto go_result;}
				else return TRUE; 
			}
		}		
		break;
	case STAFF:
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_StaffClass[i] == armClass) 
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol) { bflag = TRUE; goto go_result;}
				else return TRUE; 
			}
		}
		break;
	case EDGED:
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_EdgedClass[i] == armClass)
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol) { bflag = TRUE; goto go_result;}
				else return TRUE; 
			}
		}
		break;
	case FIREARMS:
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_FireArmsClass[i] == armClass)
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol) { bflag = TRUE; goto go_result;}
				else return TRUE; 
			}
		}
		break;
	case JUDGE://仲裁
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_JudgeClass[i] == armClass)
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol)
				{ 
					bflag = TRUE; 
					goto go_result;
				}
				else return TRUE; 
			}
		}
		break;

	}

go_result:
	if(bflag)
	{
		SendSystemMsg( IDS_USER_ITEM_TOO_LOW_ABILITY, SYSTEM_NORMAL, TO_ME);
		return FALSE;
	}

	strMsg.Format( IDS_USER_ITEM_OTHER_CLASS );
	
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	秦寸 酒捞袍 浇吩阑 檬扁拳茄促.
//
void USER::ReSetItemSlot(ItemList *pItem)
{
	if(!pItem) return;

	pItem->sLevel = 0;
	pItem->sSid		= -1;
	pItem->sDuration  = 0;
	pItem->sBullNum	= 0;
	pItem->sCount		= 0;
	::ZeroMemory(pItem->tMagic, MAGIC_NUM);
	pItem->tIQ = 0;
	pItem->iItemSerial = 0;

	pItem->dwTime = 0;
	pItem->uid[0] = pItem->uid[1] = pItem->uid[2] = -1;
	pItem->SuccessRate[0] = pItem->SuccessRate[1] = pItem->SuccessRate[2] = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void USER::SendItemMoveFail()
{
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add((BYTE)FAIL);

	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
//	酒捞袍捞 EQUIP, INVENTORY, EBody Slot 吝 绢叼俊 加窍绰瘤 魄窜茄促.
//	UpDate : 2001. 4. 18
int USER::GetSlotClass(int nSlot, int nType, BOOL flag)
{
	if(nSlot >= 0 && nSlot < EQUIP_ITEM_NUM) return 0;						// Equip
	else if(nSlot >= EQUIP_ITEM_NUM && nSlot < TOTAL_INVEN_MAX) return 1;	// Inven
	else if(nSlot >= TOTAL_INVEN_MAX && nSlot < TOTAL_ITEM_NUM) return 2;	// EBody
	else return -1;
}

//////////////////////////////////////////////////////////////////////////////
//	厚绢乐绰 酒捞袍 浇吩阑 茫绰促.
//
int USER::GetEmptySlot(int nSlot)
{
	int i = 0;

	switch(nSlot)
	{
	case EQUIP_SLOT:
		for(i = 0; i < EQUIP_ITEM_NUM; i++)
		{
			if(m_UserItem[i].sSid == -1) return i;
		}
		break;
	case INVENTORY_SLOT:
		for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
		{
			if(m_UserItem[i].sSid == -1) return i;
		}
		break;
	case BANK_SLOT:
		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
		{
			if(m_UserBankItem[i].sSid == -1) return i;
		}
		break;
	case ACCOUNT_BANK_SLOT:
		for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++)
		{
			if(m_AccountBankItem[i].sSid == -1) return i;
		}
		break;
	case GUILD_SLOT:
		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
		{
			if(m_GuildItem[i].sSid == -1) return i;
		}
		break;
	default:
		break;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//	鞍篮 辆幅狼 酒捞袍捞 乐绰瘤 茫绰促.
//
int USER::GetSameItem(ItemList Item, int nSlot)
{
	int i = 0;
	int j = 0;
	BOOL nFlag = FALSE;

	if(Item.sSid < 0 || Item.sSid >= g_arItemTable.GetSize()) return -1;
	if(g_arItemTable[Item.sSid]->m_sDuration > 0) return -1;		
	
	switch(nSlot)
	{
	case INVENTORY_SLOT:
		for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
		{
			if(m_UserItem[i].sSid == Item.sSid &&
			   m_UserItem[i].sDuration == Item.sDuration)  
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_UserItem[i].tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if(m_UserItem[i].tIQ == Item.tIQ) return i;
			}
            
		}
		break;
	case TRADE_SLOT:
		EnterCriticalSection( &m_CS_ExchangeItem );
		for(i = 0; i < m_arExchangeItem.GetSize(); i++)
		{
			if ( m_arExchangeItem[i] == NULL ) continue;

			if(m_arExchangeItem[i]->changedList.sSid == Item.sSid &&
			   m_arExchangeItem[i]->changedList.sDuration == Item.sDuration &&
			   m_arExchangeItem[i]->changedList.sBullNum == Item.sBullNum)
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_arExchangeItem[i]->changedList.tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if ( m_arExchangeItem[i]->changedList.tIQ == Item.tIQ)
				{
					LeaveCriticalSection( &m_CS_ExchangeItem );
					return i;
				}
			}
		}
		LeaveCriticalSection( &m_CS_ExchangeItem );
		break;
	case BANK_SLOT:
		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)				// 
		{
			if(m_UserBankItem[i].sSid == Item.sSid &&
			   m_UserBankItem[i].sDuration == Item.sDuration &&
			   m_UserBankItem[i].sBullNum == Item.sBullNum)  
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_UserBankItem[i].tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if(m_UserBankItem[i].tIQ == Item.tIQ) return i;
			}
		}
		break;
	case ACCOUNT_BANK_SLOT:
		for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++)				// 
		{
			if(m_AccountBankItem[i].sSid == Item.sSid &&
			   m_AccountBankItem[i].sDuration == Item.sDuration &&
			   m_AccountBankItem[i].sBullNum == Item.sBullNum)  
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_AccountBankItem[i].tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if(m_AccountBankItem[i].tIQ == Item.tIQ) return i;
			}
		}
		break;
	case GUILD_SLOT:
		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)				// 
		{
			if(m_GuildItem[i].sSid == Item.sSid &&
			   m_GuildItem[i].sDuration == Item.sDuration &&
			   m_GuildItem[i].sBullNum == Item.sBullNum)  
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_GuildItem[i].tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if(m_GuildItem[i].tIQ == Item.tIQ) return i;
			}
		}
		break;

	default:
		break;
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////
//	狞酒捞袍 殿废 秒家甫 茫酒辑 焊辰促. 
//
void USER::SendQuickChange()
{
/*	int i, j;
	int iChangeNum = 0;
	BOOL bChange = TRUE;
	int tChangeSlot[QUICK_ITEM];
	memset(tChangeSlot, -1, sizeof(tChangeSlot));

	int iQuick = TOTAL_ITEM_NUM;

	CBufferEx TempBuf;

	for(i = 0; i < QUICK_ITEM; i++)
	{
		if(m_UserItem[iQuick].sSid != -1)
		{
			for(j = EQUIP_ITEM_NUM; j < TOTAL_ITEM_NUM; j++)
			{
				if(m_UserItem[iQuick].sSid == m_UserItem[j].sSid) { bChange = FALSE; break; }
			}

			if(bChange == TRUE) 
			{ 
				m_UserItem[i+TOTAL_ITEM_NUM].sSid = -1; 
				tChangeSlot[i] = i; iChangeNum++; 
			}
		}
		iQuick++;
		bChange = TRUE;
	}

	if(iChangeNum == 0) return;

	short sCount = -1;
	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add((BYTE)0x01);			// 己傍
	TempBuf.Add((BYTE)0x02);			// 狞酒捞袍 殿废 秒家

	TempBuf.Add((BYTE)iChangeNum);

	for(i = 0; i < QUICK_ITEM; i++)
	{
		if(tChangeSlot[i] != -1)
		{
			TempBuf.Add((BYTE)i);
			TempBuf.Add(sCount);
		}
	}

	Send(TempBuf, TempBuf.GetLength());
*/
}

/////////////////////////////////////////////////////////////////////////////////
//	物品蓝使用加载...
//
void USER::ItemUseReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if( m_iDisplayType == 6) return; //yskang 0.5
#endif

	int iMode = 0;
	int index = 0;
	short sSid = 0;
	short sValue = 0;
	int iRandom = 0;

	short sSP = -1;	
	short sHP = -1;	
	short sPP = -1;	

	CString strMsg = _T("");

	BYTE tResult = SUCCESS;
	BOOL bRes = FALSE, bWeight = FALSE;
	int iInvenSlot = 0;

	DWORD dwDN = 0;

	int type = 0;

	BYTE tType = GetByte(pBuf, index);
	BYTE tSlot = GetByte(pBuf, index);
//	sSid = GetShort(pBuf, index);

	BYTE tUsePotion = 0;

	CBufferEx TempBuf,TempBuf1,TempBuf2, PotionBuf;
	if( GetTickCount()- m_dwLastUseSpecialTime < USE_ITEM_TIME)  //限制疯狂加血
	{
		return ;
	}

	TempBuf.Add(ITEM_USE_RESULT);
	
	/************************抗寇贸府****************************************/
	if(tType != 1 && tType != 2) { bRes = TRUE; goto go_result; } 
	if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bRes = TRUE; goto go_result; } 

	sSid = m_UserItem[tSlot].sSid;
            //判断是否机甲  双击机甲
	if(sSid == 1184){
	    if(m_UserItem[39].sSid == 1184){
		   SendSystemMsg("你身上已有件机甲,请卸下再装备.", SYSTEM_ERROR, TO_ME);
		   return;} 
	    Ebody39(tSlot);
		ReSetItemSlot(&m_UserItem[tSlot]);
		SendCharData();
		CheckMagicItemMove();
	 } 
     
    if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { bRes = TRUE; goto go_result; } 
    if(m_UserItem[tSlot].sSid == 1490)
	{
		UserBabyCall(tSlot);
		return ;
	}
																		// 鞍篮 加己 尝府 嘿咯初扁困秦 ITEM_MAX_USE_WEAR甫 荤侩
	if(g_arItemTable[sSid]->m_byWear < 100 || g_arItemTable[sSid]->m_byWear > ITEM_MAX_USE_WEAR) 
	{
		if(g_arItemTable[sSid]->m_byWear != 15) { bRes = TRUE; goto go_result; } // 捞亥飘 汗鼻...
	} 

	if(m_UserItem[tSlot].sCount <= 0) { bRes = TRUE; goto go_result; } 

	if(g_arItemTable[sSid]->m_sBullNum != 0)
	{
		if(m_UserItem[RIGHT_HAND].sSid < 0) 
		{
			strMsg.Format( IDS_USER_CANT_USE_THIS, g_arItemTable[sSid]->m_strName);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			bRes = TRUE; 
			goto go_result; 
		} 
		if(g_arItemTable[sSid]->m_byBullType != g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byBullType)
		{
			strMsg.Format( IDS_USER_CANT_USE_THIS, g_arItemTable[sSid]->m_strName);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			bRes = TRUE; 
			goto go_result;
		}
	}
    /************************角青 风凭***************************************/
	iMode = GetItemFieldNum((int)sSid);
	TempBuf.Add(tResult);
	TempBuf.Add((BYTE)iMode);

	switch(iMode)												//@@@@@@@@概流 馆康 鞘夸!!!!!
	{
	case QUICK_ITEM_SP:											//	SP			
		sValue = g_arItemTable[sSid]->m_sRepairSP;
		m_sSP =  m_sSP + sValue;
		if(m_sMagicMaxSP <= m_sSP) m_sSP = m_sMagicMaxSP;

		sSP = m_sSP;
		TempBuf.Add(sSP);										// 雀汗 荐摹
		TempBuf.Add(sHP);
		TempBuf.Add(sPP);
		TempBuf.Add((short)-1);
		bWeight = TRUE;
		tUsePotion = SP_POTION;
		break;

	case QUICK_ITEM_PP:											//	PP
		sValue = g_arItemTable[sSid]->m_sRepairPP;
		sValue += (short)((double)sValue * m_DynamicEBodyData[EBODY_PP_RECOVERY_UP] / 100);
		m_sPP =  m_sPP + sValue;
		if(m_sMagicMaxPP <= m_sPP) m_sPP = m_sMagicMaxPP;

		sPP = m_sPP;
		TempBuf.Add(sSP);										// 雀汗 荐摹
		TempBuf.Add(sHP);
		TempBuf.Add(sPP);
		TempBuf.Add((short)-1);
		bWeight = TRUE;
		tUsePotion = PP_POTION;
		break;

	case QUICK_ITEM_HP:											//	HP
		sValue = g_arItemTable[sSid]->m_sRepairHP;
		sValue += (short)((double)sValue * m_DynamicEBodyData[EBODY_HP_RECOVERY_UP] / 100);
		m_sHP =  m_sHP + sValue;
		if(m_sMagicMaxHP <= m_sHP) m_sHP = m_sMagicMaxHP;

		sHP = m_sHP;
		TempBuf.Add(sSP);										// 雀汗 荐摹
		TempBuf.Add(sHP);
		TempBuf.Add(sPP);
		TempBuf.Add((short)-1);
		bWeight = TRUE;
		tUsePotion = HP_POTION;

		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
		break;

	case QUICK_ITEM_ALLREPAIR:									//	SP, PP, HP 葛滴 雀汗
		m_sSP += g_arItemTable[sSid]->m_sRepairSP;
		m_sPP += g_arItemTable[sSid]->m_sRepairPP;
		m_sHP += g_arItemTable[sSid]->m_sRepairHP;

		if(m_sMagicMaxSP <= m_sSP) m_sSP = m_sMagicMaxSP;
		if(m_sMagicMaxPP <= m_sPP) m_sPP = m_sMagicMaxPP;
		if(m_sMagicMaxHP <= m_sHP) m_sHP = m_sMagicMaxHP;
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
		sSP = m_sSP; sHP = m_sHP; sPP = m_sPP;
		TempBuf.Add(sSP);										// 雀汗 荐摹
		TempBuf.Add(sHP);
		TempBuf.Add(sPP);
		TempBuf.Add((short)-1);

		bWeight = TRUE;
		tUsePotion = ALL_POTION;
		break;

	case QUICK_ITEM_BULL:										// 秦寸 醚扁 藕舅甫 盲框
		m_UserItem[RIGHT_HAND].sBullNum = g_arItemTable[sSid]->m_sBullNum;
		SendBullNum(RIGHT_HAND);
		bWeight = TRUE;
		break;
	case QUICK_ITEM_TELEPORT:
		sSid = GetByte(pBuf, index);
		GetHanyGate(tSlot, sSid);
		bWeight = TRUE;
		return;

	case QUICK_ITEM_POTAL:
		if(g_arItemTable[sSid]->m_sPid == 44)			//鉴埃 雀乔 酒捞袍
		{
			DWORD dwCurrTick = GetTickCount();
			if (dwCurrTick - m_dwLastItem44Time < 5*1000)
			{
				return ;
			}else{
				m_dwLastItem44Time = dwCurrTick;
			}
			type = 0, index = 0;
			int iRepeat = 0;
			int rank = m_sCityRank + CITY_RANK_INTERVAL;

			if(rank <= DEATH_RANK || m_sKillCount >= 100) 
			{
				SendSystemMsg( IDS_USER_TOO_LOW_CITY_VALUE, SYSTEM_ERROR, TO_ME);
				return;
			}
			if(m_dwCloseTime > 0 || m_curz==409 || m_curz==61) 
			{
				SendSystemMsg( "无法使用闪避药水.", SYSTEM_ERROR, TO_ME);
				return;
			}
		   if(m_dwLhslTime != 0 && (!IsCity()))
		  {
				SendEventMsg( "灵魂锁链状态下(5秒)无法使用闪避药水.");
				return;
		   }

			if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) { bRes = TRUE; goto go_result; }

			MAP *pMap = g_zone[m_ZoneIndex];
			if(!pMap) { bRes = TRUE; goto go_result; } 

			type = CheckInvalidMapType();
			if(type == 8 || type == 10 || type == 15 || type == 17 || type == 9 || type == 11 || type == 16 ||type == 18) 
		    {
				SendEventMsg( "要塞战区内无法使用闪避药水.");
				return;
		    } 
			if(!CheckInvalidZoneState(type)) { bRes = TRUE; goto go_result; } 

			do
			{
				int min_x = m_curx - 12; if(min_x < 0) min_x = 0;
				int max_x = m_curx + 12; if(max_x >= pMap->m_sizeMap.cx) max_x = pMap->m_sizeMap.cx - 1;
				int min_y = m_cury - 12; if(min_y < 0) min_y = 0;
				int max_y = m_cury + 12; if(max_y >= pMap->m_sizeMap.cy) max_y = pMap->m_sizeMap.cy - 1;
				
				int move_x = 0, move_y =0;
				
				move_x = myrand(min_x, max_x);
				move_y = myrand(min_y, max_y);

//				type = CheckInvalidMapType();
				type = ((g_zone[m_ZoneIndex]->m_pMap[move_x][move_y].m_dwType & 0xFF00 ) >> 8);
				
				if(!CheckInvakidZoneChangeState(type)) { bRes = TRUE; goto go_result; } 
	//			if(type > 1)		// 档矫甫 哈绢唱搁...
/*				if(type > 1 && type < 8)
				{
					index = g_arMapTable[type]->m_sStoreIndex;
					if(index < 0 || index >= g_arStore.GetSize()) { bRes = TRUE; goto go_result; } 
					if(g_arStore[index]->m_lUsed == 1)	{ bRes = TRUE; goto go_result; } 
				}
*/				
				if(Teleport(move_x, move_y) == FALSE) iRepeat++;
				else break;
				
			}while(iRepeat < 6);

			if( iRepeat >= 6 )
			{
				bRes = TRUE; goto go_result;
			}
		}
		else if (m_curz == 67 && g_arItemTable[sSid]->m_sPid == 85)			//PK竞赛无法使用时空药水
		{
			SendSystemMsg( "进入PK赛场后就无法使用时空药水!", SYSTEM_ERROR, TO_ME);
			return;
		}
		else					
		{
			if(m_tGuildHouseWar == GUILD_WARRING) { bRes = TRUE; goto go_result; } 
			if(IsCity()) { bRes = TRUE; goto go_result; } 
//			TownPotal();
			if(m_tQuestWar == GUILD_WARRING) g_QuestEventZone.CheckUserStateInEventZone(m_curz);

			if( !ReturnTown( tType, tSlot ) ) return;
		}

		bWeight = TRUE;
		break;

/*	case QUICK_ITEM_CONFUSE:									//  去鄂 摹丰
		break;
	case QUICK_ITEM_PALSY:										//  付厚 摹丰
		break;
	case QUICK_ITEM_POISON:										//  吝刀 摹丰
		break;
	case QUICK_ITEM_VISION:										//	矫仿 摹丰
		break;
	case QUICK_ITEM_ALLCURE:									//	葛电 惑怕捞惑 摹丰
		break;
*/
	case QUICK_ITEM_SALUTE:					// 气磷
		if(sSid == NORMAL_SALUTE_SID)			tUsePotion = SALUTE;
		else if(sSid == XMAS_SALUTE_SID)		tUsePotion = XMAS_SALUTE;
		else if(sSid == YEAR_SALUTE_SID)		tUsePotion = YEAR_SALUTE;
		else tUsePotion = SALUTE;

		bWeight = TRUE;
		break;

	case EVENT_ITEM_LOTTERY:
		dwDN = CheckLottery();		
		ReSetItemSlot(&m_UserItem[tSlot]);
		CheckMaxValue(m_dwDN, dwDN);
		MakeMoneyLog( dwDN, ITEMLOG_MONEY_LOTTERY );
		FlushItemLog( TRUE );
		bWeight = TRUE;
		break;

	case EVENT_ITEM_DEADPOISON:
		tUsePotion = DEAD_POTION;//圣药
		{			
			int skill_point = 0;
			if( m_sLevel <= 60 )
				skill_point = (int)( (double)m_sLevel / 2 + 0.5 );
			else if( m_sLevel > 60 && m_sLevel <= 99 )
				skill_point = (int)( 30 + (double)m_sLevel - 60 + 0.5 );
			else if( m_sLevel > 99 )
				skill_point = (int)( 30 + 99 - 60 + 0.5 );
			else
			{
				return;
			}
			if(sSid==1022)
			{
				switch (m_byClass){
					case 0://拳
						m_sSTR = 14;
						m_sCON = 15;
						m_sDEX = 10;
						m_sVOL = 9;	
						m_sWIS = 9;
						break;
					case 1:
						m_sSTR = 9;
						m_sCON = 10;
						m_sDEX = 9;
						m_sVOL = 17;
						m_sWIS = 12;
						break;
					case 2:
						m_sSTR = 15;
						m_sCON = 14;
						m_sDEX = 10;
						m_sVOL = 9;
						m_sWIS = 9;
						break;
					case 3:
						m_sSTR = 10;
						m_sCON = 12;
						m_sDEX = 17;
						m_sVOL = 9;
						m_sWIS = 9;
						break;
					case 4:
						m_sSTR = 10;
						m_sCON = 12;
						m_sDEX = 9;
						m_sVOL = 9;
						m_sWIS = 17;
						break;

				}
				m_sPA = (short)(m_sLevel-1)+8;
			}else
			{
				m_sSTR = m_sTempSTR;					// 塞
				m_sCON = m_sTempCON;					// 扒碍
				m_sDEX = m_sTempDEX;					// 刮酶己
				m_sVOL = m_sTempVOL;					// 狼瘤
				m_sWIS = m_sTempWIS;// 瘤驱
				m_sPA = (short)(m_sLevel-1);
			}
			DelMyEventNum(251);
			for(int i = 0; i < SKILL_COUNT; i++)
			{
				m_UserSkill[i].sSid=i;
				m_UserSkill[i].tLevel = 0;
			}
	
			m_UserSkill[4].sSid = 4;
			m_UserSkill[9].sSid = 9;
			m_UserSkill[14].sSid = 14;
			m_UserSkill[19].sSid = 19;
			m_UserSkill[24].sSid = 24;

//			CheckMaxValue((short &)m_sSkillPoint, (short)skill_point);
			m_sSkillPoint = (short)skill_point;
			if(m_sLevel >= 130 && o_yehuoini[0]->chaoneng == 1)
			{
				m_sSkillPoint_= m_sLevel - 129;
				if(m_sSkillPoint_ >70)
					m_sSkillPoint_=70;
				for(int i = 0; i < SKILL_130_COUNT; i++)
				{
					m_UserSkill_[i].sSid=i;
					m_UserSkill_[i].tLevel = 0;
				}
			}
			//140级技能
			if(m_sLevel>=140 && o_yehuoini[0]->chaoneng == 1)
			{
				for(int i = 0; i < 2; i++)
				{
					m_UserSkill140[i].sSid = 0;
					m_UserSkill140[i].tLevel = 0;
				}
			}
			//重新计算
			GetMagicItemSetting();
			//设置属性
			SetUserToMagicUser();

			UpdateUserData();

			SendUserStatusSkill();

			bWeight = TRUE;
		}
		break;

	case EVENTITEM_WEAR_MOON:
		if(MoonEvent(tSlot)) bWeight = TRUE;
		else bRes = TRUE;

		break;

	case EVENTITEM_WEAR_INVASION:	// 魔傍 捞亥飘侩 家券 酒捞袍
		if(InvaderSummon(tSlot)) bWeight = TRUE;
		else bRes = TRUE;
		break;

	case SPECIAL_POTION_WEAR:
		tUsePotion = UseSpecialPotion(tSlot);
		if(tUsePotion)
		{
			MakeItemLog( &m_UserItem[tSlot], ITEMLOG_SPECIAL_USE_POTION );
			FlushItemLog( TRUE );

			bWeight = TRUE;
		}
		else bRes = TRUE;

		break;

	case PSI_STONE_WEAR:
		if(UsePsiStone(tSlot) == TRUE)
		{
			MakeItemLog( &m_UserItem[tSlot], ITEMLOG_SPECIAL_USE_POTION );
			FlushItemLog( TRUE );

			bWeight = TRUE;
		}
		else bRes = TRUE;

		break;
	case 132://守护能源
		{
		if(m_UserItem[38].sSid <= 0) return;
		m_UserItem[38].sDuration += 300;
		if(m_UserItem[38].sDuration >= 1800) m_UserItem[38].sDuration = 1800;
	    TempBuf1.Add(ITEM_DURATION);
		TempBuf1.Add((byte)38);
		TempBuf1.Add(m_UserItem[38].sDuration);
		Send(TempBuf1, TempBuf1.GetLength());
		bWeight = TRUE;
		SendSystemMsg("成功充电,守护耐久增加300!", SYSTEM_NORMAL, TO_ME);
		}

		break;
	default:
		bRes = TRUE;
		break;
	}

go_result:

	if(bWeight) m_iCurWeight -= g_arItemTable[sSid]->m_byWeight;		// 敬父怒 公霸甫 临牢促.

	if(bRes)
	{
		tResult = FAIL;
		TempBuf2.Add(ITEM_USE_RESULT);
		TempBuf2.Add(tResult);
		Send(TempBuf2, TempBuf2.GetLength());
		return;
	}

	UpdateUserItemDN();							// 酒捞袍 函悼 眉农...

	GetRecoverySpeed();							// 雀汗加档 眉农...

	TempBuf.Add((BYTE)tSlot);

	if(iMode == EVENT_ITEM_LOTTERY) { TempBuf.Add(dwDN); Send(TempBuf, TempBuf.GetLength()); return; }

	m_UserItem[tSlot].sCount -= 1;		//
	if(m_UserItem[tSlot].sCount <= 0) { m_UserItem[tSlot].sSid = -1; sSid = -1; }//SendQuickChange(); }

	TempBuf.Add(m_UserItem[tSlot].sLevel);
	TempBuf.Add(sSid);
	TempBuf.Add(m_UserItem[tSlot].sDuration);
	TempBuf.Add(m_UserItem[tSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSlot].sCount);
	for(int i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tSlot].tMagic[i]);
	TempBuf.Add(m_UserItem[tSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());

	type = CheckInvalidMapType();
									
	if(CheckInvalidZoneState(type))		// 老馆 鞘靛捞搁
	{
		if(tUsePotion)
		{
			if(g_PotionViewOnOff == 1) return;	// 款康磊啊 拱距 瓤苞甫 焊捞瘤 臼档废 悸泼茄促

			PotionBuf.Add(USE_POTION);
			PotionBuf.Add(m_uid + USER_BAND);
			if(tUsePotion==101||tUsePotion==102){
				PotionBuf.Add((BYTE)0xf);
				PotionBuf.Add((int)(tUsePotion-100));
			
			}else{
				PotionBuf.Add((BYTE)tUsePotion);
			}
			if(m_curz == 61 || m_curz == 62 || m_curz == 63) SendInsight( PotionBuf, PotionBuf.GetLength() );
			else SendExactScreen( PotionBuf, PotionBuf.GetLength() );
		}
	}
m_dwLastUseSpecialTime = GetTickCount();
//	else Send(PotionBuf, PotionBuf.GetLength());	// 傍己傈 肚绰 鞘靛 惑痢傈老锭绰 焊郴瘤 臼绰促.

//	SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
}

/////////////////////////////////////////////////////////////////////////////////
//	汗鼻 寸梅咯何甫 眉农茄促. 
//
DWORD USER::CheckLottery()
{
	int type = myrand(1, 10);
	DWORD dwDN = 0;

	int iMax = 0;

	switch(type)
	{
	case 1:
		iMax = myrand(1, _MAX_INT);
		if(iMax == 1) dwDN = 10000000;
		break;
	case 2:
		iMax = myrand(1, 1000000000);
		if(iMax == 1) dwDN = 8000000;
		break;
	case 3:
		iMax = myrand(1, 100000000);
		if(iMax == 1) dwDN = 6000000;
		break;
	case 4:
		iMax = myrand(1, 10000000);
		if(iMax == 1) dwDN = 4000000;
		break;
	case 5:
		iMax = myrand(1, 1000000);
		if(iMax == 1) dwDN = 2000000;
		break;
	case 6:
		iMax = myrand(1, 100000);
		if(iMax == 1) dwDN = 1000000;
		break;
	case 7:
		iMax = myrand(1, 10000);
		if(iMax == 1) dwDN = 500000;
		break;
	case 8:
		iMax = myrand(1, 1000);
		if(iMax == 1) dwDN = 250000;
		break;
	case 9:
		iMax = myrand(1, 100);
		if(iMax == 1) dwDN = 100000;
		break;
	case 10:
		iMax = myrand(1, 10);
		if(iMax == 1) dwDN = 10000;
		break;
	}

	return dwDN;
}

/////////////////////////////////////////////////////////////////////////////////
//	酒捞袍 鞘靛俊辑 秦寸 Wear 锅龋甫 舅妨霖促.
//
int USER::GetItemFieldNum(int iNum)
{
	BYTE tCure = 0;
	BYTE tHP = 0;
	BYTE tPP = 0;
	BYTE tSP = 0;

	BYTE tResult = 0;

	switch(g_arItemTable[iNum]->m_byWear)
	{
	case 100:			// 摹丰力 辆幅..
//		return g_arItemTable[iNum]->m_bySubCure;
		break;
	case 101:			// 雀汗力 辆幅
		if(g_arItemTable[iNum]->m_bySubCure) { tCure = 1; tCure <<= 3; }
		if(g_arItemTable[iNum]->m_sRepairHP) { tHP = 1; tHP <<= 2; }
		if(g_arItemTable[iNum]->m_sRepairPP) { tPP = 1; tPP <<= 1; }
		if(g_arItemTable[iNum]->m_sRepairSP) tSP = 1; 

		tResult = tCure + tHP + tPP + tSP;
		break;

	case 102: case 103:	// 醚扁 档八 蒲
		tResult = QUICK_ITEM_BULL;
		break;

	case 104:			// 橇府俊胶 府柯, 扼捞靛
		tResult = QUICK_ITEM_POTAL;
		break;

	case 14:			// 
		break;

	case 15:			// 汗鼻
		tResult = EVENT_ITEM_LOTTERY;
		break;

	case 105:			// 勤叼霸捞飘
		tResult = QUICK_ITEM_TELEPORT;
		break;

	case 111:
		tResult = EVENT_ITEM_DEADPOISON;
		break;

	case 113:
		tResult = QUICK_ITEM_SALUTE;
		break;
		
	case 115:
		tResult = EVENTITEM_WEAR_MOON;
		break;

	case 116:
		tResult = SPECIAL_POTION_WEAR;
		break;

	case 117:
		tResult = PSI_STONE_WEAR;
		break;
	case 120:
		tResult = EVENTITEM_WEAR_INVASION;
		break;
	case 132:
		tResult= 132; //守护能源
		break;
	}

	return tResult;
/*
	if(g_arItemTable[iNum]->m_byWear == 13) return QUICK_ITEM_POTAL;
	if(g_arItemTable[iNum]->m_byWear == 15) return EVENT_ITEM_LOTTERY;
	if(g_arItemTable[iNum]->m_sBullNum)	return QUICK_ITEM_BULL;

	if(g_arItemTable[iNum]->m_bySubCure) { tCure = 1; tCure <<= 3; }
	if(g_arItemTable[iNum]->m_sRepairHP) { tHP = 1; tHP <<= 2; }
	if(g_arItemTable[iNum]->m_sRepairPP) { tPP = 1; tPP <<= 1; }
	if(g_arItemTable[iNum]->m_sRepairSP) tSP = 1; 

	tResult = tCure + tHP + tPP + tSP;
		
	if(tResult == 8) return g_arItemTable[iNum]->m_bySubCure;

	return tResult;
*/
/*
	SubCure : RepairHP : RepairPP : RepairSP : RETURN
		0		1			0			0		4
		0		0			1			0		2
		0		0			0			1		1
		1		0			0			0		m_bySubCure;
		0		1			1			1		7
*/
}

/////////////////////////////////////////////////////////////////////////////////
//	甘 肚绰 惑措 蜡历俊霸 酒捞袍阑...
//
void USER::ItemGiveReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int		index = 0;

	BYTE tType = GetByte(pBuf, index);
	BYTE tObject = GetByte(pBuf, index);
	if (m_bNowTrading == TRUE || m_bPShopOpen == TRUE) 
	   {
            CString strDate ="";
			SYSTEMTIME st;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
			TCHAR m_Log[500];
			sprintf_s(m_Log,"[ %s ]%s %d,%d 物品移动存在非法4 \r\n",strDate,m_strUserID,m_bNowTrading,m_bPShopOpen);
		    WriteUserShopLog(m_Log);
			SendItemMoveFail();
		    return;
	 }           

	if(tType == 1)		// 蜡历
	{
		if(tObject == 0) GiveMoneyToUser(pBuf + index);

		if(tObject == 1) GiveItemToUser(pBuf + index);
	}
	else if(tType == 0) // 甘
	{
		if(tObject == 0) GiveMoneyToMap(pBuf + index);

		if(tObject == 1) GiveItemToMap(pBuf + index);
	}

}

/////////////////////////////////////////////////////////////////////////////////
//	惑措 蜡历俊霸 捣阑...
//
void USER::GiveMoneyToUser(TCHAR *pBuf)
{
	int		index = 0;

	BOOL nFlag = TRUE;
	BYTE error_code = 0;

	BOOL bBuddyMember;
	int  nMeMapType, nYouMapType;

	CString strMsg;
	USER *pUser = NULL;

	int uid = GetInt(pBuf, index);
	///////////////交易密码
	if(!m_MItemLock  && o_yehuoini[0]->mimabaohu == 1 )
	{
		strMsg.Format("请先接触密码保护后在操作");
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);
		return;
	}
	///////////////////////

	if( (uid - USER_BAND) == m_uid ) return;

	pUser = GetUser(uid - USER_BAND);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	// 焊烹蜡历老 版快 茄 拳搁, 滚叼蜡历老 版快 滴拳搁阑 哈绢唱搁 酒捞袍捞唱 捣阑 临荐 绝促. zinee 02/12/10
	// 窜 滚叼盔捞 傍己瘤开俊 乐阑 版快绰 焊烹锭贸烦 茄拳搁栏肺 力茄等促.
	bBuddyMember = CheckBuddyMember(pUser->m_strUserID);

	nMeMapType = CheckInvalidMapType();
	nYouMapType = pUser->CheckInvalidMapType();

	if( !bBuddyMember )
	{
		if(!IsInSight(pUser->m_curx, pUser->m_cury, pUser->m_curz)) return;			
	}
	else 
	if( (nMeMapType == 8 || nMeMapType == 10 || nMeMapType == 15)
		||(nMeMapType == 9 || nMeMapType == 11 || nMeMapType == 16)
		||(nYouMapType == 8 || nYouMapType == 10 || nYouMapType == 15)
		||(nYouMapType == 9 || nYouMapType == 11 || nYouMapType == 16) )
	{
			if( !CheckInvalidZoneState(nMeMapType) || !pUser->CheckInvalidZoneState(nYouMapType) )
			{
				if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, 2 ) )
					return;
			}

	}
	else if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, (SCREEN_X + SCREEN_Y) ) ) return;

	// 肺凯烦喉 版扁厘捞扼搁
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int metype = CheckInvalidMapType();
		int type = pUser->CheckInvalidMapType();

		if( metype != type ) return;
	}
#else		// 烙矫肺 弥碍 捞亥飘锭巩俊
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int metype = CheckInvalidMapType();
		int type = pUser->CheckInvalidMapType();

		if( metype != type ) return;
	}
#endif

	DWORD	dwDN = GetDWORD(pBuf, index);

	DWORD dwMyDN = 0, dwYourDN = 0, dwTempDN = 0;

	if(pUser->m_bRefuse) { error_code = 0x03; goto go_result; }
	if(dwDN > m_dwDN) { error_code = 0x04; goto go_result; }
	if(dwDN <= 0 || dwDN == _MAX_DWORD) { error_code = 255; goto go_result; }

	dwYourDN = dwTempDN = pUser->m_dwDN;	// 归诀侩	
	dwMyDN = m_dwDN;

	if(!CheckMaxValueReturn(dwTempDN, dwDN))// 弥措 DWORD蔼阑 逞菌促.	
	{
		CheckMaxValue(dwTempDN, dwDN);		// 唱赣瘤绰 登倒妨 霖促.
		if( ((double)dwTempDN - (double)dwDN) < 0 ) return;

		dwDN = dwTempDN - dwDN;				// 角力肺 狐廉 唱埃捣
	}
	else dwTempDN += dwDN;					// 霖父怒 歹茄促.

	//----------------[ 陛咀阑 函悼矫挪促.]----------------------------------//
	pUser->m_dwDN = dwTempDN;				// 歹窍绊

	if(m_dwDN <= dwDN) m_dwDN = 0;
	else		       m_dwDN -= dwDN;							// 哗绊

	MakeMoneyLog( dwDN, ITEMLOG_MONEY_GIVE, pUser->m_strUserID, pUser->m_dwDN );

	// ----------------[ Update Databace ] ----------------------------------//
	if(UpdateUserExchangeData(pUser) == FALSE)
	{
		m_dwDN = dwMyDN;
		pUser->m_dwDN = dwYourDN;
		FlushItemLog( FALSE );
		return;
	}

	nFlag = FALSE;							// 炼扒甫 促 父练矫淖促.
	FlushItemLog( TRUE );

go_result:
	if(nFlag)
	{
		CBufferEx TempBuf;

		BYTE result = FAIL;
		TempBuf.Add(ITEM_GIVE_RESULT);
		TempBuf.Add(result);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return ;
	}

	UpdateUserItemDN();						// 酒捞袍 函悼 眉农...
	SendMoneyChanged();
	pUser->SendMoneyChanged();	

	strMsg = _T("");
	strMsg.Format( IDS_USER_DINA_RECEIVE_FROM, m_strUserID, dwDN);
	pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
}

/////////////////////////////////////////////////////////////////////////////////
//	惑措规俊霸 酒捞袍阑 霖促.
//
void USER::GiveItemToUser(TCHAR *pBuf)
{
	CBufferEx TempMyBuf, TempYourBuf;
	ItemList MyItem[TOTAL_ITEM_NUM], YourItem[TOTAL_ITEM_NUM];	// 牢亥 归诀侩 

	BOOL nFlag = TRUE;
//	BOOL bQuickChange = FALSE;

	BOOL bBuddyMember;

	int  nMeMapType, nYouMapType;

	int iWeight = 0;
	int iSlot = -1;
	int i = 0, index = 0; 
	BYTE error_code = 0;
	int sSid = -1;
	BYTE tSlot = 0;
	short sCount = 0;

	CString strMsg, strMsg1;

	USER *pUser = NULL;

	int uid = GetInt(pBuf, index);

	if( (uid - USER_BAND) == m_uid ) return;

	pUser = GetUser(uid - USER_BAND);
	//////////////////交易密码
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
	{
		strMsg.Format("请先解除密码保护后在操作");
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);
		return;
	}
	//////////////////////////
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_bRefuse) { error_code = 0x03; goto go_result; }			// 措惑蜡历啊 酒捞袍 罐扁 芭例 惑怕搁 俊矾贸府

#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return;				// 磊扁磊脚捞 公丰荤侩磊捞芭唱 措惑 蜡历啊 公丰荤侩磊捞搁 府畔
	if(pUser->m_iDisplayType == 6) return; 
#endif

	if(pUser->m_bNowTrading == TRUE || pUser->m_bNoItemMove == TRUE) return;	

	// 焊烹蜡历老 版快 茄 拳搁, 滚叼蜡历老 版快 滴拳搁阑 哈绢唱搁 酒捞袍捞唱 捣阑 临荐 绝促. zinee 02/12/10
	
	bBuddyMember = CheckBuddyMember(pUser->m_strUserID);

	nMeMapType = CheckInvalidMapType();
	nYouMapType = pUser->CheckInvalidMapType();

	if( !bBuddyMember )
	{
		if(!IsInSight(pUser->m_curx, pUser->m_cury, pUser->m_curz)) return;			
	}
	else if(  (nMeMapType == 8 || nMeMapType == 10 || nMeMapType == 15)		// 器飘府胶 郴何, 器飘府胶傈 康开
			||(nMeMapType == 9 || nMeMapType == 11 || nMeMapType == 16)
			||(nYouMapType == 8 || nYouMapType == 10 || nYouMapType == 15)
			||(nYouMapType == 9 || nYouMapType == 11 || nYouMapType == 16) )
	{
			if( !CheckInvalidZoneState(nMeMapType) || !pUser->CheckInvalidZoneState(nYouMapType) )
			{
				if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, 2 ) ) return;
			}

	}
	else if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, (SCREEN_X + SCREEN_Y) ) ) return;

	// 肺凯烦喉 版扁厘捞扼搁
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int metype = CheckInvalidMapType();
		int type = pUser->CheckInvalidMapType();

		if( metype != type ) return;
	}
#else		
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int metype = CheckInvalidMapType();
		int type = pUser->CheckInvalidMapType();

		if( metype != type ) return;
	}
#endif

	tSlot = GetByte(pBuf, index);
	if(tSlot < EQUIP_ITEM_NUM || tSlot >= EQUIP_ITEM_NUM + INVENTORY_NUM) { error_code = 0x05; goto go_result; }	// 牢亥俊 乐绰巴父 临荐 乐澜
	
	sCount = GetShort(pBuf, index);
	if(m_UserItem[tSlot].sCount < sCount || sCount <= 0) { error_code = 0x05; goto go_result; }	// 箭磊啊 厚沥惑利牢巴 眉农
	if(sCount > MAX_ITEM_NUM) { error_code = 0x02; goto go_result; }	// 牢亥狼 弥措 肮荐绰 255 (jjs07 03-01-03)

	sSid = m_UserItem[tSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { error_code = 0x05; goto go_result; }
	

	// 父距 酒捞袍捞 府滚胶府柯捞绊 秦寸滚叼蜡历啊 磷菌栏搁 何劝酒捞袍栏肺 荤侩.
	// 窜 傍己傈阑 窍绊乐绰 瘤开捞扼搁 何劝矫懦荐 绝促.
	if( sSid == NORMAL_ITEM_REVERSE && pUser->m_bLive == USER_DEAD && bBuddyMember )
	{
		if( CheckInvalidZoneState(nMeMapType) && pUser->CheckInvalidZoneState(nYouMapType) )
		{
			TempMyBuf.Add(REVIVAL_REQ);
			TempMyBuf.AddString( m_strUserID );
			pUser->Send(TempMyBuf, TempMyBuf.GetLength());
			return;
		}
		else 
			SendSystemMsg(IDS_CANNOT_REVERSE, SYSTEM_NORMAL, TO_ME);
			//SendSystemMsg( "器飘府胶傈矫 府滚胶府柯阑 荤侩且 荐 绝嚼聪促.", SYSTEM_NORMAL, TO_ME);
	}

	if( pUser->m_bLive == USER_DEAD ) return;

	if(g_arItemTable[sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
	{
		SendSystemMsg( IDS_USER_CANT_GIVE_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
		error_code = 0x05; 
		goto go_result;
	}
	// 郴备档啊 乐绰单 肮荐啊 1焊促 农搁 俊矾
	if(g_arItemTable[sSid]->m_sDuration > 0 && m_UserItem[tSlot].sCount >= 2)	{ error_code = 255; goto go_result; }	// if( count > 1 ) FAIL;

	iWeight = sCount * g_arItemTable[sSid]->m_byWeight;
	if(pUser->m_iMaxWeight <= pUser->m_iCurWeight + iWeight)
	{
		SendSystemMsg( IDS_USER_OVER_WEIGHT_OTHER, SYSTEM_ERROR, TO_ME);
		error_code = 0x05; 
		goto go_result;
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];	
		YourItem[i] = pUser->m_UserItem[i];
	}

//	if(g_arItemTable[sSid]->m_byWear <= 5 || g_arItemTable[sSid]->m_byWear == 15)// 郴备档啊 乐绢辑 肮荐绰 公炼扒 1
	if(g_arItemTable[sSid]->m_sDuration > 0)// 郴备档啊 乐绢辑 肮荐绰 公炼扒 1
	{
		iSlot = pUser->GetEmptySlot(INVENTORY_SLOT);
		if(iSlot < 0) { error_code = 0x02; goto go_result; }			// 惑措规 牢亥俊 傍埃捞 绝促.

		pUser->m_UserItem[iSlot].sLevel = m_UserItem[tSlot].sLevel;		// 刚历 惑措规 牢亥俊 盲况林绊...	
		pUser->m_UserItem[iSlot].sSid = m_UserItem[tSlot].sSid;		// 刚历 惑措规 牢亥俊 盲况林绊...	
		pUser->m_UserItem[iSlot].sCount = 1;
		pUser->m_UserItem[iSlot].sDuration = m_UserItem[tSlot].sDuration;
		pUser->m_UserItem[iSlot].sBullNum = m_UserItem[tSlot].sBullNum;
		pUser->m_UserItem[iSlot].tIQ = m_UserItem[tSlot].tIQ;

		for(i = 0; i < MAGIC_NUM; i++) pUser->m_UserItem[iSlot].tMagic[i] = m_UserItem[tSlot].tMagic[i];

		pUser->m_UserItem[iSlot].iItemSerial = m_UserItem[tSlot].iItemSerial;

		MakeItemLog( &(pUser->m_UserItem[iSlot]), ITEMLOG_GIVE, pUser->m_strUserID );
		ReSetItemSlot(&m_UserItem[tSlot]);							// 促澜 郴 牢亥甫 哗 霖促.		
	}
	else															// 郴备档啊 绝绰 酒捞袍篮 笛促 混旗毫具..(瓤啦己锭巩俊 蝶肺)		
	{
		iSlot = pUser->GetSameItem(m_UserItem[tSlot], INVENTORY_SLOT);
		if(iSlot < 0)
		{
			iSlot = pUser->GetEmptySlot(INVENTORY_SLOT);			// 惑措规 后浇吩阑 茫磊...
			if(iSlot < 0) { error_code = 0x02; goto go_result; }		// 惑措规 牢亥俊 傍埃捞 绝促.

			pUser->m_UserItem[iSlot].sLevel = m_UserItem[tSlot].sLevel;	// 刚历 惑措规 牢亥俊 盲况林绊...	
			pUser->m_UserItem[iSlot].sSid = m_UserItem[tSlot].sSid;	// 刚历 惑措规 牢亥俊 盲况林绊...	
			pUser->m_UserItem[iSlot].sCount = sCount;				// 后 浇吩捞骨肺 
			pUser->m_UserItem[iSlot].sDuration = m_UserItem[tSlot].sDuration;
			pUser->m_UserItem[iSlot].sBullNum = m_UserItem[tSlot].sBullNum;
			pUser->m_UserItem[iSlot].tIQ = m_UserItem[tSlot].tIQ;

			for(i = 0; i < MAGIC_NUM; i++) pUser->m_UserItem[iSlot].tMagic[i] = m_UserItem[tSlot].tMagic[i];

			pUser->m_UserItem[iSlot].iItemSerial = m_UserItem[tSlot].iItemSerial;
		}
		else
		{
//			if(pUser->m_UserItem[iSlot].sCount + sCount > MAX_ITEM_NUM) { error_code = 0x02; goto go_result; }
			pUser->m_UserItem[iSlot].sCount += sCount;				// 惑措规 刚历...
		}

		if((m_UserItem[tSlot].sCount - sCount) <= 0)				// 促澜 郴 牢亥甫 哗 霖促.		
		{															// 狞酒捞袍 函拳啊 乐栏搁		
//			if(g_arItemTable[sSid]->m_byWear >= 6) bQuickChange = TRUE;

			ReSetItemSlot(&m_UserItem[tSlot]);
		}
		else m_UserItem[tSlot].sCount -= sCount;

	}

	// ----------------[ Update Databace ] ----------------------------------//		
	if(UpdateUserExchangeData(pUser) == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)//诀单捞飘啊 角菩窍搁 狐廉唱柯促.
		{
			m_UserItem[i] = MyItem[i];
			pUser->m_UserItem[i] = YourItem[i];
		}
		FlushItemLog( FALSE );
		return;
	}

	nFlag = FALSE;

go_result:
	BYTE result = SUCCESS;
	TempMyBuf.Add(ITEM_GIVE_RESULT);

	if(nFlag)
	{
		result = FAIL;
		TempMyBuf.Add(result);
		TempMyBuf.Add(error_code);
		Send(TempMyBuf, TempMyBuf.GetLength());
		FlushItemLog( FALSE );
		return ;
	}

	FlushItemLog( TRUE );

	//	公霸甫 函悼矫挪促.
	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	pUser->m_iCurWeight += iWeight;

	UpdateUserItemDN();								// 酒捞袍 函悼 眉农...
	GetRecoverySpeed();								// 促矫 雀汗加档甫 拌魂
	pUser->GetRecoverySpeed();

	TempMyBuf.Add(result);							// 唱俊霸 焊辰促.
	TempMyBuf.Add(tSlot);
	TempMyBuf.Add(m_UserItem[tSlot].sLevel);
	TempMyBuf.Add(m_UserItem[tSlot].sSid);
	TempMyBuf.Add(m_UserItem[tSlot].sDuration);
	TempMyBuf.Add(m_UserItem[tSlot].sBullNum);
	TempMyBuf.Add(m_UserItem[tSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempMyBuf.Add(m_UserItem[tSlot].tMagic[i]);
	TempMyBuf.Add(m_UserItem[tSlot].tIQ);

	Send(TempMyBuf, TempMyBuf.GetLength());
//	if(bQuickChange) SendQuickChange();				// 狞酒捞袍 函悼捞乐栏搁...

													// 惑措规俊 函悼 荤亲阑 焊辰促.		
	TempYourBuf.Add(ITEM_GIVE_RESULT);
	TempYourBuf.Add(result);
	TempYourBuf.Add((BYTE)iSlot);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sLevel);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sSid);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sDuration);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sBullNum);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempYourBuf.Add(pUser->m_UserItem[iSlot].tMagic[i]);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].tIQ);

	strMsg = _T("");
	strMsg.Format( IDS_USER_ITEM_RECEIVE_FROM, g_arItemTable[sSid]->m_strName, m_strUserID); //唱吝俊 弥利拳_sid
	pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

	pUser->Send(TempYourBuf, TempYourBuf.GetLength());

	strMsg1 = _T("");
	strMsg1.Format( IDS_USER_ITEM_GIVE_TO, g_arItemTable[sSid]->m_strName, pUser->m_strUserID); //唱吝俊 弥利拳_sid
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_NORMAL, TO_ME);

//	SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
//	pUser->SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
}

/////////////////////////////////////////////////////////////////////////////////
//	甘俊 捣阑 滚覆...
//
void USER::GiveMoneyToMap(TCHAR *pBuf)
{
	CString strMsg;
	int		index = 0;
	long	lUsed = 0;
	CPoint	ptCell;
	int		iIsItem = 11;	// 伎俊 鞍篮 谅钎 殿废规瘤

	BOOL nFlag = TRUE;

	ItemList *pMapItem = NULL;

	DWORD dwMyDN = 0;
	BYTE error_code = 0;

	short sWill_X = GetShort(pBuf, index);
	short sWill_Y = GetShort(pBuf, index);

	DWORD	dwDN = GetDWORD(pBuf, index);
	////////////////////交易密码
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
	{
		strMsg.Format("请先解除密码保护后在操作");
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);
		return;
	}
	///////////////////////////////

	if(m_tGuildHouseWar == GUILD_WARRING || m_tQuestWar == GUILD_WARRING) 
	{
		SendSystemMsg( IDS_USER_CANT_THROW_IN_QUEST, SYSTEM_ERROR, TO_ME);
		return;
	}

	ptCell = ConvertToServer( sWill_X, sWill_Y );
	if(ptCell.x < 0 || ptCell.y < 0) { error_code = 0x04; goto go_result; }

	ptCell = FindNearRandomPoint(ptCell.x, ptCell.y);							// 泅犁 磊扁谅钎甫 力寇茄 24沫
	if(ptCell.x < 0 || ptCell.y < 0) { error_code = 0x04; goto go_result; }
	if(ptCell.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptCell.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) { error_code = 0x04; goto go_result; }

	if(dwDN > m_dwDN) { error_code = 0x04; goto go_result; }
	if(dwDN <= 0 || dwDN == _MAX_DWORD) { error_code = 255; goto go_result; }

	// 泅犁 乐绰 粮捞 肺凯烦喉 甘捞扼搁
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#else		// 烙矫肺 弥碍 捞亥飘锭巩俊
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#endif

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, (long)1, (long)0) == (long)0)
	{
		pMapItem = new ItemList;

		if(pMapItem != NULL)											// 皋葛府啊 力措肺 且寸 登菌促搁
		{																		
			pMapItem->tType = TYPE_MONEY;
			pMapItem->dwMoney = 0;

			dwMyDN = m_dwDN; 

			if(m_dwDN <= dwDN) m_dwDN = 0;
			else m_dwDN -= dwDN; 

			MakeMoneyLog( dwDN, ITEMLOG_MONEY_THROW );

			CheckMaxValue((DWORD &)pMapItem->dwMoney, (DWORD)dwDN);
			// ----------------[ Update Databace ] ----------------------------------//
//			if(UpdateUserData() == FALSE)
			if(UpdateUserItemDN() == FALSE)
			{
				m_dwDN = dwMyDN;
				::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);

				error_code = 255; goto go_result;
			}

			nFlag = FALSE;

			FlushItemLog( TRUE );

			//m_pCom->DelThrowItem();
			m_pCom->SetThrowItem( pMapItem, ptCell.x, ptCell.y, m_ZoneIndex );
		}
		else error_code = 255;

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);
	}

go_result:
	if(nFlag)
	{
		CBufferEx TempBuf;

		BYTE result = FAIL;
		TempBuf.Add(ITEM_GIVE_RESULT);
		TempBuf.Add(result);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());

		if(pMapItem) delete pMapItem;

		FlushItemLog( FALSE );

		return ;
	}

	UpdateUserItemDN();								// 酒捞袍 函悼 眉农...
	SendMoneyChanged();
}

/////////////////////////////////////////////////////////////////////////////////
//	甘俊 酒捞袍阑...
//  丢东西
void USER::GiveItemToMap(TCHAR *pBuf, BOOL bChao)
{
	CString strMsg;
	CBufferEx TempBuf;
	ItemList MyItem[TOTAL_ITEM_NUM];	// 牢亥 归诀侩 

	BOOL nFlag = TRUE;
//	BOOL bQuickChange = FALSE;

	int i, index = 0; 
	BYTE error_code = 0;

	CPoint	ptCell;
	int sSid = -1;

	short sWill_X = GetShort(pBuf, index);
	short sWill_Y = GetShort(pBuf, index);

	BYTE tSlot = GetByte(pBuf, index);
	short sCount = GetShort(pBuf, index);

	ItemList *pMapItem = NULL;
	//////////////////////////////////////////交易密码
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
		{
			strMsg.Format("请先解除密码保护后在操作");
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);
			//return;
			error_code = 0x05; goto go_result;
		}
	/////////////////////////////////////////////

	if(m_tGuildHouseWar == GUILD_WARRING || m_tQuestWar == GUILD_WARRING) 
	{
		SendSystemMsg( IDS_USER_CANT_THROW_IN_QUEST, SYSTEM_ERROR, TO_ME);
		return;
	}

	//if(m_UserItem[tSlot].tMagic[5]> 10)//避免幻石无法丢，设置1改以上的物品不能丢
	//{
	//	SendSystemMsg( "10改以上的物品无法丢弃!", SYSTEM_ERROR, TO_ME);
	//	return;
	//}

	// 郴 牢亥俊 绝促.
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) { error_code = 0x05; goto go_result; }
	if( !bChao && (tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) ) { error_code = 0x05; goto go_result; }

	sSid = m_UserItem[tSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { error_code = 0x05; goto go_result; }

	if(g_arItemTable[sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
	{
		SendSystemMsg( IDS_USER_CANT_THROW_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
		error_code = 0x05; goto go_result;
	}

	if(m_UserItem[tSlot].sCount < sCount || sCount <= 0) { error_code = 0x05; goto go_result; }

	ptCell = ConvertToServer( sWill_X, sWill_Y );
	ptCell = FindNearRandomPoint(ptCell.x, ptCell.y);							// 泅犁 磊扁谅钎甫 力寇茄 24沫
	if(ptCell.x < 0 || ptCell.y < 0) { error_code = 0x04; goto go_result; }
	if(ptCell.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptCell.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) { error_code = 0x04; goto go_result; }

	// 泅犁 乐绰 粮捞 肺凯烦喉 甘捞扼搁
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#else		// 烙矫肺 弥碍 捞亥飘锭巩俊
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#endif

	for(i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];		
	}

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, (long)1, (long)0) == (long)0)
	{
		pMapItem = new ItemList;

		if(pMapItem != NULL)											// 皋葛府啊 力措肺 且寸 登菌促搁
		{
			pMapItem->tType = TYPE_ITEM;
			pMapItem->sLevel = m_UserItem[tSlot].sLevel;
			pMapItem->sSid = m_UserItem[tSlot].sSid;
			pMapItem->sDuration = m_UserItem[tSlot].sDuration;
			pMapItem->sCount = sCount;
			pMapItem->sBullNum = m_UserItem[tSlot].sBullNum;
			pMapItem->tIQ = m_UserItem[tSlot].tIQ;
			for(i = 0; i < MAGIC_NUM; i++) pMapItem->tMagic[i] = m_UserItem[tSlot].tMagic[i];
			pMapItem->iItemSerial = m_UserItem[tSlot].iItemSerial;

			if(g_arItemTable[sSid]->m_sDuration <= 0)
			{
				if((m_UserItem[tSlot].sCount - sCount) <= 0) 
				{ 
					// 酒捞袍 肺弊甫 巢败具 且巴 鞍变 茄单... 般摹绰 酒捞袍捞扼, 林籍贸府窃
					//if( !bChao )	MakeItemLog( pMapItem, ITEMLOG_THROW );
					//else			MakeItemLog( pMapItem, ITEMLOG_DEAD_THROW );
					ReSetItemSlot(&m_UserItem[tSlot]);
				}
				else m_UserItem[tSlot].sCount -= sCount;
			}
			else
			{
				if( !bChao )	MakeItemLog( pMapItem, ITEMLOG_THROW );
				else			MakeItemLog( pMapItem, ITEMLOG_DEAD_THROW );
				ReSetItemSlot(&m_UserItem[tSlot]);
			}

			// ----------------[ Update Databace ] ----------------------------------//		
//			if(UpdateUserData() == FALSE)
			if(UpdateUserItemDN() == FALSE)
			{
				for(i = 0; i < TOTAL_ITEM_NUM; i++)//诀单捞飘啊 角菩窍搁 狐廉唱柯促.
				{
					m_UserItem[i] = MyItem[i];
				}
				::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);

				error_code = 255; 
				goto go_result; 
			}

			//m_pCom->DelThrowItem();
		//	m_pCom->SetThrowItem( pMapItem, ptCell.x, ptCell.y, m_ZoneIndex );//丢东西显示在地上
			if(m_tIsOP==1)
			{
				m_pCom->SetThrowItem( pMapItem, ptCell.x, ptCell.y, m_ZoneIndex );//丢地上的东西/物品显示在地上
			}
	
			nFlag = FALSE;
		}
		else error_code = 255;

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);
	}

go_result:
	BYTE result = SUCCESS;
	TempBuf.Add(ITEM_GIVE_RESULT);

	if(!pMapItem) nFlag = TRUE;

	if(nFlag)
	{
		result = FAIL;
		TempBuf.Add(result);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());

		if(pMapItem) delete pMapItem;

		FlushItemLog( FALSE );
		return ;
	}

	FlushItemLog( TRUE );

	m_iCurWeight -= g_arItemTable[pMapItem->sSid]->m_byWeight * sCount;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();			// 促矫 雀汗加档甫 拌魂

	TempBuf.Add(result);
	TempBuf.Add(tSlot);
	TempBuf.Add(m_UserItem[tSlot].sLevel);
	TempBuf.Add(m_UserItem[tSlot].sSid);
	TempBuf.Add(m_UserItem[tSlot].sDuration);
	TempBuf.Add(m_UserItem[tSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSlot].sCount);
	for(int j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[tSlot].tMagic[j]);

	TempBuf.Add(m_UserItem[tSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
}
/////////////////////////////////////////////////////////////////////////////////
//	甘俊 酒捞袍阑...
//  死亡掉东西显示
void USER::PKGiveItemToMap(TCHAR *pBuf, BOOL bChao)
{
	CBufferEx TempBuf;
	ItemList MyItem[TOTAL_ITEM_NUM];	// 牢亥 归诀侩 

	BOOL nFlag = TRUE;
//	BOOL bQuickChange = FALSE;

	int i, index = 0; 
	BYTE error_code = 0;

	CPoint	ptCell;
	int sSid = -1;

	short sWill_X = GetShort(pBuf, index);
	short sWill_Y = GetShort(pBuf, index);

	BYTE tSlot = GetByte(pBuf, index);
	short sCount = GetShort(pBuf, index);

	ItemList *pMapItem = NULL;

	if(m_tGuildHouseWar == GUILD_WARRING || m_tQuestWar == GUILD_WARRING) 
	{
		SendSystemMsg( IDS_USER_CANT_THROW_IN_QUEST, SYSTEM_ERROR, TO_ME);
		return;
	}

	// 郴 牢亥俊 绝促.
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) { error_code = 0x05; goto go_result; }
	if( !bChao && (tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) ) { error_code = 0x05; goto go_result; }

	sSid = m_UserItem[tSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { error_code = 0x05; goto go_result; }

	if(g_arItemTable[sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
	{
		SendSystemMsg( IDS_USER_CANT_THROW_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
		error_code = 0x05; goto go_result;
	}

	if(m_UserItem[tSlot].sCount < sCount || sCount <= 0) { error_code = 0x05; goto go_result; }

	ptCell = ConvertToServer( sWill_X, sWill_Y );
	ptCell = FindNearRandomPoint(ptCell.x, ptCell.y);							// 泅犁 磊扁谅钎甫 力寇茄 24沫
	if(ptCell.x < 0 || ptCell.y < 0) { error_code = 0x04; goto go_result; }
	if(ptCell.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptCell.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) { error_code = 0x04; goto go_result; }

	// 泅犁 乐绰 粮捞 肺凯烦喉 甘捞扼搁
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#else		// 烙矫肺 弥碍 捞亥飘锭巩俊
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#endif

	for(i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];		
	}

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, (long)1, (long)0) == (long)0)
	{
		pMapItem = new ItemList;

		if(pMapItem != NULL)											// 皋葛府啊 力措肺 且寸 登菌促搁
		{
			pMapItem->tType = TYPE_ITEM;
			pMapItem->sLevel = m_UserItem[tSlot].sLevel;
			pMapItem->sSid = m_UserItem[tSlot].sSid;
			pMapItem->sDuration = m_UserItem[tSlot].sDuration;
			pMapItem->sCount = sCount;
			pMapItem->sBullNum = m_UserItem[tSlot].sBullNum;
			pMapItem->tIQ = m_UserItem[tSlot].tIQ;
			for(i = 0; i < MAGIC_NUM; i++) pMapItem->tMagic[i] = m_UserItem[tSlot].tMagic[i];
			pMapItem->iItemSerial = m_UserItem[tSlot].iItemSerial;

			if(g_arItemTable[sSid]->m_sDuration <= 0)
			{
				if((m_UserItem[tSlot].sCount - sCount) <= 0) 
				{ 
					// 酒捞袍 肺弊甫 巢败具 且巴 鞍变 茄单... 般摹绰 酒捞袍捞扼, 林籍贸府窃
					//if( !bChao )	MakeItemLog( pMapItem, ITEMLOG_THROW );
					//else			MakeItemLog( pMapItem, ITEMLOG_DEAD_THROW );
					ReSetItemSlot(&m_UserItem[tSlot]);
				}
				else m_UserItem[tSlot].sCount -= sCount;
			}
			else
			{
				if( !bChao )	MakeItemLog( pMapItem, ITEMLOG_THROW );
				else			MakeItemLog( pMapItem, ITEMLOG_DEAD_THROW );
				ReSetItemSlot(&m_UserItem[tSlot]);
			}

			// ----------------[ Update Databace ] ----------------------------------//		
//			if(UpdateUserData() == FALSE)
			if(UpdateUserItemDN() == FALSE)
			{
				for(i = 0; i < TOTAL_ITEM_NUM; i++)//诀单捞飘啊 角菩窍搁 狐廉唱柯促.
				{
					m_UserItem[i] = MyItem[i];
				}
				::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);

				error_code = 255; 
				goto go_result; 
			}

			if (bChao)
			m_pCom->SetThrowItem( pMapItem, ptCell.x, ptCell.y, m_ZoneIndex );//死亡掉东西显示在地上
	
			nFlag = FALSE;
		}
		else error_code = 255;

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);
	}

go_result:
	BYTE result = SUCCESS;
	TempBuf.Add(ITEM_GIVE_RESULT);

	if(!pMapItem) nFlag = TRUE;

	if(nFlag)
	{
		result = FAIL;
		TempBuf.Add(result);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());

		if(pMapItem) delete pMapItem;

		FlushItemLog( FALSE );
		return ;
	}

	FlushItemLog( TRUE );

	m_iCurWeight -= g_arItemTable[pMapItem->sSid]->m_byWeight * sCount;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();			// 促矫 雀汗加档甫 拌魂

	TempBuf.Add(result);
	TempBuf.Add(tSlot);
	TempBuf.Add(m_UserItem[tSlot].sLevel);
	TempBuf.Add(m_UserItem[tSlot].sSid);
	TempBuf.Add(m_UserItem[tSlot].sDuration);
	TempBuf.Add(m_UserItem[tSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSlot].sCount);
	for(int j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[tSlot].tMagic[j]);

	TempBuf.Add(m_UserItem[tSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
}
/////////////////////////////////////////////////////////////////////////////////
//	甘 肚绰 各俊 乐绰 酒捞袍甫 掘绰促.
//
void USER::ItemLoadReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return;

	BYTE tType;				// 1 : 各
							// 2 : 甘
	int i, j;
	short sX = 0, sY = 0;
	short sSid = -1;

	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap) return;

	ItemList* pItem = NULL;

	int iWeight = 0;
	int mapIndex = -1;
	short Diff = 0;
	int index = 0;

	BYTE	error_code = UNKNOWN_ERR;

	DWORD dwDN = 0, dwMyDN = 0;

	BYTE result = SUCCESS;
	BYTE tRight = FAIL;

	CBufferEx TempBuf, TempBuf2;

	ItemList MyItem[TOTAL_ITEM_NUM];	// 牢亥 归诀侩 

	dwMyDN = m_dwDN;					// 捣 沥焊 归诀
	for(i = 0; i < TOTAL_ITEM_NUM; i++)	// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];		
	}

	tType = GetByte(pBuf, index);

	TempBuf.Add(ITEM_LOAD_RESULT);			// 菩哦狼 矫累

	if(tType == 2)						//@@@@@@@@@@@ 阂鞘夸茄 何盒...
	{
		sX = GetShort(pBuf, index);
		sY = GetShort(pBuf, index);

		CPoint ptCell = ConvertToServer( sX, sY );

		if(ptCell.x < 0 || ptCell.y < 0) { error_code = ERR_2; goto go_result; }
		if(ptCell.x >= pMap->m_sizeMap.cx || ptCell.y >= pMap->m_sizeMap.cy) { error_code = ERR_2; goto go_result; }

		if(GetDistance(ptCell.x, ptCell.y, 2) == FALSE) { error_code = ERR_1; goto go_result; }
				
		mapIndex = pMap->m_pMap[ptCell.x][ptCell.y].iIndex;
		if(mapIndex < 0) { error_code = ERR_6; goto go_result; }
		
		// IKING 2001.1.
		EnterCriticalSection( &m_pCom->m_critThrowItem );
		if ( m_pCom->m_ThrowItemArray[mapIndex] == NULL )
		{
			LeaveCriticalSection( &m_pCom->m_critThrowItem );
			error_code = ERR_6;
			goto go_result;
		}
		pItem = m_pCom->m_ThrowItemArray[mapIndex]->m_pItem;
		LeaveCriticalSection( &m_pCom->m_critThrowItem );
		//

		if( !pItem )
		{
			pMap->m_pMap[ptCell.x][ptCell.y].iIndex = -1;
			error_code = ERR_6;
			goto go_result;
		}

		//------------------------------------------------------------------------
		if( pItem->dwTime > 0 )
		{
			BYTE tNum = 0;
			DWORD curr = ConvertCurTimeToSaveTime();

			if( (curr - pItem->dwTime) <= 62 )
			{
				for(i = 0; i < ITEM_USER_RIGHT_NUM; i++)
				{
					if( pItem->uid[i] >= 0 && pItem->uid[i] + USER_BAND < NPC_BAND )
					{
						if(m_uid == pItem->uid[i]) { tRight = SUCCESS; break; }
					}
					else if( pItem->uid[i] <= -1 ) tNum += 1;
				}

				if(tRight)
				{
					int iRandom = myrand(0, 100);
					if(iRandom > pItem->SuccessRate[i])
					{
						error_code = ERR_6;
						goto go_result;
					}
				}
				else if( tNum != 3 ) 
				{
					error_code = ERR_3;
					goto go_result;
				}
			}
		}
/*		COleDateTime checkTime = COleDateTime(pItem->ThrowTime);
		
		if(checkTime.m_status == COleDateTime::valid)
		{
			CTime cur = CTime::GetCurrentTime();
			CTime last(pItem->ThrowTime);
			CTimeSpan DiffTime;
			DiffTime = cur - last;
			
			if( DiffTime.GetMinutes() <= 1)
			{
				for(i = 0; i < ITEM_USER_RIGHT_NUM; i++)
				{
					if( pItem->uid[i] >= 0 && pItem->uid[i] + USER_BAND < NPC_BAND )
					{
						if(m_uid == pItem->uid[i]) { tRight = SUCCESS; break; }
					}
				}

				if(tRight)
				{
					int iRandom = myrand(0, 100);
					if(iRandom > pItem->SuccessRate[i])
					{
						error_code = ERR_6;
						goto go_result;
					}
				}
				else
				{
					error_code = ERR_3;
					goto go_result;
				}
			}
		}
*/
		tType = pItem->tType;
		if(pItem->tType == TYPE_MONEY)
		{
			if(pItem->dwMoney <= 0 || pItem->dwMoney == _MAX_DWORD) goto go_result;
			if(m_dwDN == _MAX_DWORD) goto go_result;
		}
		else if(pItem->tType == TYPE_ITEM)
		{
			sSid = pItem->sSid;		// 殿废登瘤 臼篮 酒捞袍
			if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { error_code = ERR_6; goto go_result; }
			if(pItem->sCount < 0 || pItem->sCount > _MAX_SHORT) { error_code = ERR_6; goto go_result; }
//			if(g_arItemTable[sSid]->m_byWear < 6 && pItem->sCount >= 2) goto go_result;	// 郴备档乐绰巴篮 坷流 窍唱
			if(g_arItemTable[sSid]->m_sDuration > 0 && pItem->sCount >= 2) { error_code = ERR_6; goto go_result; }	// 郴备档乐绰巴篮 坷流 窍唱
			
/*			iWeight = g_arItemTable[sSid]->m_byWeight * pItem->sCount;
			if(m_iMaxWeight <= m_iCurWeight + iWeight) 
			{
				SendSystemMsg( IDS_USER_OVER_WEIGHT, SYSTEM_ERROR, TO_ME);
				goto go_result;
			}
*/		}
		else goto go_result;

		if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse,(long)1, (long)0) == (long)0)
		{
			if(pItem->tType == TYPE_MONEY)
			{
				if(!CheckMaxValueReturn(m_dwDN, pItem->dwMoney))
				{									// 窜, MAX蔼捞搁 瞒咀篮 鞘靛肺...
					DWORD iDN = 0;
					CheckMaxValue(m_dwDN, pItem->dwMoney);

					iDN = (DWORD)m_dwDN;
	/////////////////////////////Release编译修改处开始//////////////////////////////////
					dwDN = (DWORD)abs((int)iDN - (int)pItem->dwMoney);
	/////////////////////////////Release编译修改处结束//////////////////////////////////

	/////////////////////////////Debug编译修改处开始//////////////////////////////////
    //                dwDN = (DWORD)abs(iDN - (DWORD)pItem->dwMoney);
	/////////////////////////////Debug编译修改处结束//////////////////////////////////
				}

				CheckMaxValue(m_dwDN, pItem->dwMoney);

				MakeMoneyLog( pItem->dwMoney, ITEMLOG_MONEY_PICKUP );
			}
			else
			{
				index = GetSameItem(*pItem, INVENTORY_SLOT);

				if(index != -1)
				{					
					for(i = 1; i < pItem->sCount + 1; i++)
					{
						iWeight = g_arItemTable[pItem->sSid]->m_byWeight * i;
						if(m_iMaxWeight < m_iCurWeight + iWeight) 
						{
							iWeight = g_arItemTable[pItem->sSid]->m_byWeight * (i - 1);
							if(iWeight == 0)
							{
//								SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
								error_code = ERR_4;
								::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
								goto go_result;
							}
							Diff = i - 1;
							break;
/*							if(pItem->sCount >= i && iWeight > 0)
							{
								Diff = pItem->sCount - i; // 甘俊 巢败龙 肮荐甫 备窃
								pItem->sCount -= i;
								break;
							}
*/						}
						Diff = i;
					}

					if(m_iMaxWeight >= m_iCurWeight + iWeight) 
					{
						if(pItem->sCount > Diff && Diff >= 0)
						{
							Diff = pItem->sCount - Diff; // 甘俊 巢败龙 肮荐甫 备窃
							pItem->sCount -= Diff;
						}
						else Diff = 0;

						CheckMaxValue((short &)m_UserItem[index].sCount, pItem->sCount);
					}
					else
					{
//						SendSystemMsg(IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
						error_code = ERR_4;
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
						goto go_result;
					}
/*					Diff = MAX_ITEM_NUM - m_UserItem[index].sCount;
					if(Diff == 0) 
					{
						SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
						goto go_result;
					}
					//	甘俊 乐绰 荐樊捞 牢亥俊 甸绢哎 弥措焊促 腹栏搁				
					if(pItem->sCount > Diff && Diff >= 0)
					{
						Diff = pItem->sCount - Diff; // 甘俊 巢败龙 肮荐甫 备窃
						pItem->sCount -= Diff;
					}
					else Diff = 0;
*/
//					CheckMaxValue((short &)m_UserItem[index].sCount, pItem->sCount);
				}
				else
				{
					index = GetEmptySlot(INVENTORY_SLOT);
					if(index == -1) 
					{
//						SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
						error_code = ERR_5;
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
						goto go_result;
					} 

					for(i = 1; i < pItem->sCount + 1; i++)
					{
						iWeight = g_arItemTable[pItem->sSid]->m_byWeight * i;
						if(m_iMaxWeight < m_iCurWeight + iWeight) 
						{
							iWeight = g_arItemTable[pItem->sSid]->m_byWeight * (i - 1);
							if(iWeight == 0)
							{
//								SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
								error_code = ERR_4;
								::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
								goto go_result;
							}
							Diff = i;
							break;
						}
						Diff = i;
					}

					if(m_iMaxWeight >= m_iCurWeight + iWeight) 
					{
						if(pItem->sCount > Diff && Diff >= 0)
						{
							Diff = pItem->sCount - Diff; // 甘俊 巢败龙 肮荐甫 备窃
							pItem->sCount -= Diff;
						}
						else Diff = 0;

						CheckMaxValue((short &)m_UserItem[index].sCount, pItem->sCount);
					}
					else
					{
//						SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
						error_code = ERR_4;
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
						goto go_result;
					}				

					m_UserItem[index].sLevel = pItem->sLevel;
					m_UserItem[index].sSid = pItem->sSid;
					m_UserItem[index].sCount = pItem->sCount;
					m_UserItem[index].sDuration = pItem->sDuration;
					m_UserItem[index].sBullNum = pItem->sBullNum;
					m_UserItem[index].tIQ = pItem->tIQ;
					for(j = 0; j < MAGIC_NUM; j++) m_UserItem[index].tMagic[j] = pItem->tMagic[j];
					m_UserItem[index].iItemSerial = pItem->iItemSerial;
					for(j = 0; j < ITEM_USER_RIGHT_NUM; j++) m_UserItem[index].uid[j] = -1;
					m_UserItem[index].dwTime = 0;

					MakeItemLog( &(m_UserItem[index]), ITEMLOG_PICKUP );
				}
			}
			
//			if(UpdateUserData() == FALSE)
			if(UpdateUserItemDN() == FALSE)
			{
				m_dwDN = dwMyDN;
				for(i = 0; i < TOTAL_ITEM_NUM; i++)//诀单捞飘啊 角菩窍搁 狐廉唱柯促.
				{
					m_UserItem[i] = MyItem[i];
				}

				::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 
				error_code = ERR_7;
				goto go_result;
			}

			if(pItem->tType == TYPE_MONEY && dwDN > 0)
			{
				pItem->dwMoney = dwDN;
				SendItemFieldInfo(ITEM_INFO_PICKUP, TO_INSIGHT, pItem, sX, sY);

				// IKING 2001.1.
				EnterCriticalSection( &m_pCom->m_critThrowItem );
				if ( m_pCom->m_ThrowItemArray[mapIndex] != NULL )
				{
					m_pCom->m_ThrowItemArray[mapIndex]->m_pItem->dwMoney = dwDN;
				}
				LeaveCriticalSection( &m_pCom->m_critThrowItem );
			}
			else if(pItem->tType == TYPE_ITEM && Diff > 0)
			{
				pItem->sCount = Diff;
				SendItemFieldInfo(ITEM_INFO_PICKUP, TO_INSIGHT, pItem, sX, sY);

				// IKING 2001.1.
				EnterCriticalSection( &m_pCom->m_critThrowItem );
				if ( m_pCom->m_ThrowItemArray[mapIndex] != NULL )
				{
					m_pCom->m_ThrowItemArray[mapIndex]->m_pItem->sCount = Diff;
				}
				LeaveCriticalSection( &m_pCom->m_critThrowItem );
			}
			else
			{
				pItem->sSid = -1;
				pItem->dwMoney = 0;
				pItem->sCount = 0;
				SendItemFieldInfo(ITEM_INFO_PICKUP, TO_INSIGHT, pItem, sX, sY);
				pMap->m_pMap[ptCell.x][ptCell.y].iIndex = -1;

				// IKING 2001.1.
				EnterCriticalSection( &m_pCom->m_critThrowItem );
				if ( m_pCom->m_ThrowItemArray[mapIndex] != NULL )
				{
					if(m_pCom->m_ThrowItemArray[mapIndex]->m_pItem)
					{
						delete m_pCom->m_ThrowItemArray[mapIndex]->m_pItem;
						m_pCom->m_ThrowItemArray[mapIndex]->m_pItem = NULL;
						m_pCom->ThrowItemDel(m_pCom->m_ThrowItemArray[mapIndex]);
					}
				}
				LeaveCriticalSection( &m_pCom->m_critThrowItem );
				//
			}

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
		}
	}
	else goto go_result;

	// ----------------[ Send Packet ] ----------------------------------//
	
	if(tType == TYPE_ITEM)
	{
		TempBuf.Add(result);
		TempBuf.Add((BYTE)0x01);

		TempBuf.Add((BYTE)index);
		TempBuf.Add(m_UserItem[index].sLevel);
		TempBuf.Add(m_UserItem[index].sSid);
		TempBuf.Add(m_UserItem[index].sDuration);
		TempBuf.Add(m_UserItem[index].sBullNum);
		TempBuf.Add(m_UserItem[index].sCount);
		for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_UserItem[index].tMagic[j]);

		TempBuf.Add((BYTE)m_UserItem[index].tIQ);

		Send(TempBuf, TempBuf.GetLength());

		m_iCurWeight += iWeight;
		GetRecoverySpeed();					// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券
//		SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
	}
	else SendMoneyChanged();

	FlushItemLog( TRUE );
	return;

go_result:

	result = FAIL;
	TempBuf.Add(result);
	TempBuf.Add(error_code);
	Send(TempBuf, TempBuf.GetLength());
	//TRACE("凛扁 角菩\n");

	FlushItemLog( FALSE );
}

/////////////////////////////////////////////////////////////////////////////////
//	甘俊 乐绰 酒捞袍 沥焊甫 焊辰促.
//
void USER::SendItemFieldInfo(BYTE type, BYTE towho, ItemList *pItem, int x, int y)
{
	if(!pItem) return;

	CBufferEx TempBuf;

	TempBuf.Add(FIELD_ITEM_INFO);
	TempBuf.Add((short)0x01);
	TempBuf.Add(type);
	TempBuf.Add((short)x);
	TempBuf.Add((short)y);

	if(pItem->tType == TYPE_ITEM)	
	{
		TempBuf.Add(pItem->sSid);
		TempBuf.Add((DWORD)pItem->sCount);
		TempBuf.Add((BYTE)pItem->tIQ);
	}
	else
	{
		TempBuf.Add(TYPE_MONEY_SID);
		TempBuf.Add(pItem->dwMoney);
		TempBuf.Add((BYTE)0);
	}

	if(type == ITEM_INFO_PICKUP) TempBuf.Add((int)(m_uid + USER_BAND));

	switch( towho )
	{

	case TO_ALL:
		SendAll(TempBuf, TempBuf.GetLength());
		break;

	case TO_ME:
		Send(TempBuf, TempBuf.GetLength());
		break;

	case TO_ZONE:
		SendZone(TempBuf, TempBuf.GetLength());
		break;

	case TO_INSIGHT:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;

	default:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;

	}
}

/////////////////////////////////////////////////////////////////////////////////
//	鞘靛惑俊 捣 沥焊(凛绰 荤恩捞 穿备牢瘤 灌腮促.)
//
void USER::GetMapMoneyInfo(DWORD dwDN, CPoint pt, int uid)
{
/*	CBufferEx TempBuf;

	short nPosX = 0;
	short nPosY = 0;

	nPosX = (short)pt.x;
	nPosY = (short)pt.y;

	TempBuf.Add(FIELD_ITEM_INFO);

	if(uid == -1)
	{
		TempBuf.Add(MONEY_INFO_MODIFY);
		TempBuf.Add(nPosX);
		TempBuf.Add(nPosY);
		TempBuf.Add(dwDN);
	}

	if(uid >= 0)
	{
		TempBuf.Add(MONEY_PICK_UP);
		TempBuf.Add(nPosX);
		TempBuf.Add(nPosY);
		TempBuf.Add(uid);
		TempBuf.Add(dwDN);
	}

	Send(TempBuf, TempBuf.GetLength());
	*/
}

/////////////////////////////////////////////////////////////////////////////////
//	厘厚芒俊 甸绢 哎 荐 乐绰 困摹甫 备茄促.
//
void USER::GetEquipPos(BYTE byWear, CPoint &pt)
{
	pt.x = -1;
	pt.y = -1;

	if(byWear < 1 || byWear > 8) return;

	switch(byWear)
	{
	case 1:
		pt.x = 4; pt.y = 5;
		break;
	case 2:
		pt.x = 0; pt.y = -1;
		break;
	case 3:
		pt.x = 1; pt.y = -1;
		break;
	case 4:
		pt.x = 8; pt.y = -1;
		break;
	case 5:
		pt.x = 9; pt.y = -1;
		break;
	case 6:
		pt.x = 2; pt.y = -1;
		break;
	case 7:
		pt.x = 3; pt.y = -1;
		break;
	case 8:
		pt.x = 6; pt.y = 7;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//	E-Body 芒俊 甸绢哎 困摹甫  备茄促.
//
int USER::GetEBodyPos(BYTE byWear)
{
//	if((byWear < 122 || byWear > 125)||byWear!=130)return -1;

	switch(byWear)
	{
	case 122:
		return TOTAL_INVEN_MAX;
	case 123:
		return TOTAL_INVEN_MAX + 1;
	case 124:
		return TOTAL_INVEN_MAX + 2;
	case 125:
		return TOTAL_INVEN_MAX + 3;
	case 130:
		return TOTAL_INVEN_MAX + 4;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//	UserItem 俊 Buffer狼 郴侩阑 Copy 茄促. --> 泅犁 家瘤茄 酒捞袍 公霸甫 穿利茄促.
//
void USER::StrToUserItem(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	int temp = 0, value = 0;
	int iWear = -1;
	MYINT64 itemserial;

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{
		m_UserItem[i].sLevel	= GetShort(pBuf, index);
		m_UserItem[i].sSid		= GetShort(pBuf, index);
		m_UserItem[i].sDuration = GetShort(pBuf, index);
		m_UserItem[i].sBullNum	= GetShort(pBuf, index);
		m_UserItem[i].sCount	= GetShort(pBuf, index);

		if(m_UserItem[i].sCount <= 0) m_UserItem[i].sSid = -1;

		for(j = 0; j < MAGIC_NUM; j++) 
		{
			m_UserItem[i].tMagic[j] = GetByte(pBuf, index);
		}
		m_UserItem[i].tIQ = GetByte(pBuf, index);

		if( m_UserItem[i].sSid == 625 || m_UserItem[i].sSid == 626 )	// 磊宏费, 漠肺父
		{
			m_UserItem[i].sDuration = 0;
		}

		for( j = 0; j < 8; j++ )
		{
			itemserial.b[j] = GetByte( pBuf, index );
		}
		m_UserItem[i].iItemSerial = itemserial.i;

		for( j = 0; j < ITEM_USER_RIGHT_NUM; j++ )
		{
			m_UserItem[i].uid[j] = -1;
			m_UserItem[i].SuccessRate[j] = 0;
		}

		m_UserItem[i].dwTime = 0;

		// 酒捞袍 背券 滚弊 锭巩俊 疵绢抄 内靛 (瘤快柳 富巴) --------------------------------------------------->>
		if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
		{
			if(m_UserItem[i].sDuration > g_arItemTable[m_UserItem[i].sSid]->m_sDuration)
			{
				m_UserItem[i].sDuration = g_arItemTable[m_UserItem[i].sSid]->m_sDuration;
			}

			if(m_UserItem[i].sDuration > 0 && m_UserItem[i].tIQ != UNIQUE_ITEM)
			{
				if(m_UserItem[i].tMagic[ITEM_UPGRADE_COUNT] > 0)		// 6锅掳 俺炼 冉荐
				{
					iWear = g_arItemTable[m_UserItem[i].sSid]->m_byWear;

					if(iWear == ATTACK_ITEM)	// 傍拜侩 公扁
					{
						 if(m_UserItem[i].tMagic[ITEM_UPGRADE_COUNT] > MAX_ITEM_UPGRADE_COUNT + 1)		// 公炼扒 10诀捞惑捞搁 厚沥惑 酒捞袍烙
						 {
							 ReSetItemSlot(&m_UserItem[i]);
							 continue;
						 }

					}
					else if(iWear >= 2 && iWear <= 5)
					{
						 if(m_UserItem[i].tMagic[ITEM_UPGRADE_COUNT] > MAX_ITEM_UPGRADE_COUNT + 1)		// 公炼扒 10诀捞惑捞搁 厚沥惑 酒捞袍烙
						 {
							 ReSetItemSlot(&m_UserItem[i]);
							 continue;
						 }
					}
				}
			}

			if( m_UserItem[i].sSid == g_RR.m_iItemSid )				// 捞 酒捞袍捞 肺锯烦喉 惑前捞绊
			{
				if( g_strARKRRWinner.CompareNoCase( m_strUserID ) )	// 泅犁 铰磊啊 酒匆 版快
				{
					ReSetItemSlot( &(m_UserItem[i]) );					// 话绰促.
				}
			}
		}
		else	// 酒捞袍 抛捞喉俊 绝绰 酒捞袍篮 瘤款促.
		{
			ReSetItemSlot(&m_UserItem[i]);
		}
		//-----------------------------------------------------------------------------------------------------<<

		if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
		{
			if(m_UserItem[i].sCount > 0) m_iCurWeight += (g_arItemTable[m_UserItem[i].sSid]->m_byWeight * m_UserItem[i].sCount);
			else m_iCurWeight += g_arItemTable[m_UserItem[i].sSid]->m_byWeight;
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer 俊 UserItem 沥焊甫 Copy 茄促.
//
void USER::UserItemToStr(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	MYINT64 itemserial;

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{
		SetShort(pBuf, m_UserItem[i].sLevel,	index);
		SetShort(pBuf, m_UserItem[i].sSid,		index);
		SetShort(pBuf, m_UserItem[i].sDuration,	index);
		SetShort(pBuf, m_UserItem[i].sBullNum,	index);
		SetShort(pBuf, m_UserItem[i].sCount,	index);

		for(j = 0; j < MAGIC_NUM; j++) SetByte(pBuf, m_UserItem[i].tMagic[j], index);

		SetByte(pBuf, m_UserItem[i].tIQ, index);

		itemserial.i = m_UserItem[i].iItemSerial;

		for( j = 0; j < 8; j++ )
		{
			SetByte( pBuf, itemserial.b[j], index );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer 俊 蜡历俊霸 焊尘 UserItem 沥焊甫 Copy 茄促.
//
void USER::UserItemToStrForSend(TCHAR *pBuf)
{
	int index = 0;
	int i, j;

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{
		SetShort(pBuf, m_UserItem[i].sLevel,	index);
		SetShort(pBuf, m_UserItem[i].sSid,		index);
		SetShort(pBuf, m_UserItem[i].sDuration,	index);
		SetShort(pBuf, m_UserItem[i].sBullNum,	index);
		SetShort(pBuf, m_UserItem[i].sCount,	index);

		for(j = 0; j < MAGIC_NUM; j++) SetByte(pBuf, m_UserItem[i].tMagic[j], index);

		SetByte(pBuf, m_UserItem[i].tIQ, index);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	UserSkill 俊 Buffer 狼 郴侩阑 墨乔茄促.
//
void USER::StrToUserSkill(TCHAR *pBuf)
{
	int index = 0,i;
	for( i = 0; i < SKILL_COUNT; i++)
	{
		m_UserSkill[i].sSid=i;
		m_UserSkill[i].tLevel = 0;
	}
	for(i = 0; i < 20; i++)
	{
		m_UserSkill[i].sSid		= GetShort(pBuf, index);
		m_UserSkill[i].tLevel	= GetByte (pBuf, index);
//		m_UserSkill[i].tOnOff	= GetByte (pBuf, index);
	}
	m_sSkillPoint_=GetByte (pBuf, index);
	if(m_sSkillPoint_>70)
		m_sSkillPoint_ = 0;
	//读取130级技能
	for(i = 0; i < 12; i++)
	{
		m_UserSkill_[i].sSid=i;
		m_UserSkill_[i].tLevel	= GetByte (pBuf, index);
	}
	//读取熊变狼变时间
	//m_dwBSTime=GetDWORD(pBuf, index);
	//m_dwDNMoney = GetDWORD(pBuf,index);
   //读取守护天使时间
	m_dwShTsTime = GetDWORD(pBuf,index);
	//读取3倍经验时间
	m_isDoubleExp = GetDWORD(pBuf,index);
	//读取3倍掉宝时间
	m_isDoubleBAOLV = GetDWORD(pBuf,index);

	//if(m_dwBSTime>3600*1000) 
	//	m_dwBSTime=0;
	//if(m_dwBSTime==0)
	//	m_iSkin=0;
	//守护天使
    if(m_dwShTsTime>3600*24*1000) m_dwShTsTime=0;
	//3倍经验
    if(m_isDoubleExp>3600*2*1000) m_isDoubleExp=0;
	//3倍掉宝
    if(m_isDoubleBAOLV>3600*2*1000) m_isDoubleBAOLV=0;

	//读取新职业5技能
	for(i = 25;i<SKILL_COUNT;i++)
	{
		m_UserSkill[i].sSid		= GetShort(pBuf, index);
		m_UserSkill[i].tLevel	= GetByte (pBuf, index);
	}
	m_UserSkill[24].sSid	= GetShort(pBuf, index);
	m_UserSkill[24].tLevel	= GetByte (pBuf, index);

	//130级新加技能
	for(i = 16;i<19;i++)
	{
		//对此没有必要
		m_UserSkill_[i].sSid = i;//GetShort(pBuf,index);
		m_UserSkill_[i].tLevel	= GetByte (pBuf, index);
		//读取出错,必须重置
		if(m_UserSkill_[i].tLevel > 20 || m_sLevel <130)
			m_UserSkill_[i].tLevel = 0;
	}
    //140与150技能
	for(i = 0;i<2;i++)
	{
		m_UserSkill140[i].sSid = GetShort(pBuf,index);

		m_UserSkill140[i].tLevel	= GetByte (pBuf, index);
	}
	//读取出错,必须重置
	if(m_UserSkill140[0].tLevel > 0 && m_sLevel <140)
	{
		m_UserSkill140[0].sSid = 0;
		m_UserSkill140[0].tLevel = 0;
	}
	if(m_UserSkill140[1].tLevel > 0 && m_sLevel <150)
	{
		m_UserSkill140[1].sSid = 0;
		m_UserSkill140[1].tLevel = 0;
	}
	//对技能点值进行判断是否合法
	int killcount = m_UserSkill140[0].tLevel+m_UserSkill140[1].tLevel+m_UserSkill_[16].tLevel+m_UserSkill_[17].tLevel+m_UserSkill_[18].tLevel;
	if(killcount>m_sLevel-129)
	{
		//全部重置
		m_UserSkill140[0].tLevel = 0;
		m_UserSkill140[1].tLevel = 0;
		m_UserSkill_[16].tLevel = 0;
		m_UserSkill_[17].tLevel = 0;
		m_UserSkill_[18].tLevel = 0;
	}
	//护法相关存储
	m_tHuFaType = GetByte(pBuf,index);
	m_dwLevealHuFaTime = GetInt(pBuf,index);
	m_nHuFaHP = GetInt(pBuf,index);
	m_nHuFaMaxHP = GetInt(pBuf,index);
	//新职业技能相关存储
	m_dwXiShengTime = GetInt(pBuf,index);
	m_dwWISUpTime = GetInt(pBuf,index);
	//宠物数据存储
	m_tBabyCall = GetByte(pBuf,index);
	m_sBabyID = GetShort(pBuf,index);
	m_sBabyLevel = GetShort(pBuf,index);
	m_sBabyStatus = GetShort(pBuf,index);
	m_sBabyNormal = GetShort(pBuf,index);
	m_nBabyExp = GetInt(pBuf,index);
	m_nBabyNextLevelExp = GetInt(pBuf,index);
	m_sFealty = GetShort(pBuf,index);
	m_sMaxHungry = GetShort(pBuf,index);
	m_sHungry = GetShort(pBuf,index);
	m_tBabySkill[0] = GetByte(pBuf,index);
	m_tBabySkill[1] = GetByte(pBuf,index);
	m_tBabySkill[2] = GetByte(pBuf,index);
	m_tBabySkill[3] = GetByte(pBuf,index);
}	

///////////////////////////////////////////////////////////////////////////////
//	Buffer 俊 UserSkill 沥焊甫 Copy 茄促.
//
void USER::UserSkillToStr(TCHAR *pBuf)
{
	int	index = 0;
	for(int i = 0; i < 20; i++)
	{
		SetShort(pBuf, m_UserSkill[i].sSid,		index);
		SetByte (pBuf, m_UserSkill[i].tLevel,	index);
	}
}
void USER::UserSkillToDBStr(TCHAR *pBuf)
{
	int i;
	int	index = 0;
	for(i = 0; i < 20; i++)
	{
		SetShort(pBuf, m_UserSkill[i].sSid,		index);
		SetByte (pBuf, m_UserSkill[i].tLevel,	index);
	}
	SetByte (pBuf,(byte)m_sSkillPoint_,index);
	for(i = 0; i < 12; i++)
	{
		//SetShort(pBuf, m_UserSkill_[i].sSid,index);
		SetByte (pBuf, m_UserSkill_[i].tLevel,	index);
	}
	//存入熊变狼变时间
	
	


	//存入守护天使时间
	DWORD LostTime2=GetTickCount()-m_dwShTsLaseTime;
	if(LostTime2 >=m_dwShTsTime)
		m_dwShTsTime=0;
	else
	{
		m_dwShTsTime=m_dwShTsTime-LostTime2;
		m_dwShTsLaseTime=GetTickCount();
	}
	SetDWORD(pBuf,m_dwShTsTime,index);

	//存入捐款3倍经验时间
	DWORD LostTime3=GetTickCount()-m_isDoubleExpLase;
	if(LostTime3 >=m_isDoubleExp)
		m_isDoubleExp=0;
	else
	{
		m_isDoubleExp=m_isDoubleExp-LostTime3;
		m_isDoubleExpLase=GetTickCount();
	}
	SetDWORD(pBuf,m_isDoubleExp,index);

	//存入捐款3倍掉宝时间
	DWORD LostTime4=GetTickCount()-m_isDoubleBAOLVLase;
	if(LostTime4 >=m_isDoubleBAOLV)
		m_isDoubleBAOLV=0;
	else
	{
		m_isDoubleBAOLV=m_isDoubleBAOLV-LostTime4;
		m_isDoubleBAOLVLase=GetTickCount();
	}
	SetDWORD(pBuf,m_isDoubleBAOLV,index);

	//守护天使

	//另外5个新职业技能
	//0x19,0x1a,0x1b,0x1c,0x18
	for(i = 25;i<29;i++)
	{
		SetShort(pBuf, m_UserSkill[i].sSid,		index);
		SetByte (pBuf, m_UserSkill[i].tLevel,	index);
	}
	SetShort(pBuf, m_UserSkill[24].sSid,		index);
	SetByte (pBuf, m_UserSkill[24].tLevel,	index);

	//新职业130级技能
	//0x10,0x11,0x12
	for(i = 16;i<19;i++)
	{
		//SetShort(pBuf, i,	index);
		//读取出错,必须重置
		if(m_UserSkill_[i].tLevel > 20 || m_sLevel <130)
			m_UserSkill_[i].tLevel = 0;

		SetByte (pBuf, m_UserSkill_[i].tLevel,	index);
	}
	//140与150技能
	for( i = 0; i < 2; i++)
	{
		SetShort(pBuf, m_UserSkill140[i].sSid,	index);
		SetByte (pBuf, m_UserSkill140[i].tLevel,index);
	}
	//护法数据
	SetByte(pBuf,m_tHuFaType,index);
	SetInt(pBuf,m_dwLevealHuFaTime,index);
	SetInt(pBuf,m_nHuFaHP,index);
	SetInt(pBuf,m_nHuFaMaxHP,index);
	//新职业技能数据
	SetInt(pBuf,m_dwXiShengTime,index);
	SetInt(pBuf,m_dwWISUpTime,index);
    //宠物相关数据
	SetByte(pBuf,m_tBabyCall,index);
	SetShort(pBuf,m_sBabyID,index);
	SetShort(pBuf,m_sBabyLevel,index);
	SetShort(pBuf,m_sBabyStatus,index);
	SetShort(pBuf,m_sBabyNormal,index);
	SetInt(pBuf,m_nBabyExp,index);
	SetInt(pBuf,m_nBabyNextLevelExp,index);
	SetShort(pBuf,m_sFealty,index);
	SetShort(pBuf,m_sMaxHungry,index);
	SetShort(pBuf,m_sHungry,index);
	SetByte(pBuf,m_tBabySkill[0],index);
	SetByte(pBuf,m_tBabySkill[1],index);
	SetByte(pBuf,m_tBabySkill[2],index);
	SetByte(pBuf,m_tBabySkill[3],index);
}
///////////////////////////////////////////////////////////////////////////////
//	UserPsi 俊 Buffer 狼 郴侩阑 墨乔茄促.
//
void USER::StrToUserPsi(TCHAR *pBuf)
{
	int index = 0;

	m_nHavePsiNum = 0;
	for(int i = 0; i < TOTAL_PSI_NUM; i++)
	{

		m_UserPsi[i].sSid	= GetShort(pBuf, index);
		m_UserPsi[i].tOnOff	= GetByte (pBuf, index);
		
		if(m_UserPsi[i].sSid == -1)	break; 
		m_nHavePsiNum++;
	}
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer 俊 UserPsi 沥焊甫 Copy 茄促.
//
void USER::UserPsiToStr(TCHAR *pBuf)
{
	int	index = 0;

	for(int i = 0; i < TOTAL_PSI_NUM; i++)
	{
		SetShort(pBuf, m_UserPsi[i].sSid,	index);
		SetByte (pBuf, m_UserPsi[i].tOnOff,	index);
		if(m_UserPsi[i].sSid == -1) break;		
		// DB俊 Update且 版快
	}
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer 俊 UserSkillInfo 沥焊甫 Copy 茄促.
//
void USER::UserSkillInfoToStr(TCHAR *pBuf)
{
	int	index = 0;
	for(int i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		SetByte (pBuf, m_UserSkillInfo[i],	index);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	矫胶袍 皋技瘤甫 傈价茄促.
//
void USER::SendSystemMsg(TCHAR *pMsg, BYTE type, int nWho)
{
	CBufferEx TempBuf;

	TempBuf.Add(SYSTEM_MSG);
	TempBuf.Add(type);
	TempBuf.Add(pMsg, _tcslen(pMsg));

	switch(nWho)
	{
	case TO_ALL:
		SendAll(TempBuf, TempBuf.GetLength());
		break;

	case TO_ME:
		Send(TempBuf, TempBuf.GetLength());
		break;

	case TO_ZONE:
		SendZone(TempBuf, TempBuf.GetLength());
		break;

	case TO_INSIGHT:
	default:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;

	}
}

///////////////////////////////////////////////////////////////////////////////
//	Trade Request 己傍 贸府
//
void USER::SendTradeSuccess(int uid, TCHAR *pstrID)//(int uid)
{
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_TRADE_RESULT);
	TempBuf.Add(uid);
	TempBuf.AddString(pstrID);
	TempBuf.Add(SUCCESS);

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	Trade Request 角菩贸府
//
void USER::SendTradeFail(int uid, BYTE error_code)
{
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_TRADE_RESULT);
	TempBuf.Add(uid);
	TempBuf.AddString(m_strUserID);
	TempBuf.Add(FAIL);
	TempBuf.Add(error_code);

	Send(TempBuf, TempBuf.GetLength());
/*
	m_bTradeWaiting = FALSE;		// Trade 夸没吝
	m_bNowTrading = FALSE;			// 泅犁 芭贰吝
	m_bExchangeOk = FALSE;			// 背券铰倡
	m_bTradeItem = FALSE;			// 酒捞袍阑 窍唱扼档 棵啡栏搁...
	m_TradeMoney = 0;   			// 芭贰 吝牢 陛咀

	for(int i = 0; i < TOTAL_ITEM_NUM; i++) // 芭贰吝 牢亥俊 官差绰 酒捞袍 荐甫 眉农困秦
	{ 
		m_TradeItemNum[i].sSid = -1; 
		m_TradeItemNum[i].sNum = 0; 
	}
*/
}

/////////////////////////////////////////////////////////////////////////////////
//	芭贰脚没 贸府甫 茄促.
//
void USER::ItemTradeReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return;
#endif		

	int type = CheckInvalidMapType();
	if(!CheckInvalidZoneState(type)) return;
	if(m_bPShopOpen == TRUE) return; // 商店状态则无法交易
	BYTE result = FAIL;
	BYTE error_code = UNKNOWN_ERR;

	TCHAR szUserName[CHAR_NAME_LENGTH + 1];
	::ZeroMemory(szUserName, sizeof(szUserName));

	USER* pSendUser = NULL;


	int i, index = 0;
	BYTE tMode = 0;

	int uid = GetInt(pBuf, index);

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // 肋给等 蜡历酒捞叼
	{
		SendTradeFail(uid, ERR_4);
		return;
	}	

	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
	{
		SendSystemMsg( "请先解除密码保护系统!", SYSTEM_ERROR, TO_ME);
		SendTradeFail(uid, ERR_4);
		return;
	}

	tMode = GetByte(pBuf, index);

	pSendUser = GetUser(uid - USER_BAND);

	if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED)	// 肋给等 蜡历酒捞叼捞搁 俊矾
	{
		SendTradeFail(uid, ERR_4);
		return;
	}
	/////////////////////////bug/////////////////////////////////////////////////
	_tcscpy_s(m_strTradeUser, _T(""));


	for(i = 0; i < TOTAL_ITEM_NUM; i++) 
	{ 
		m_TradeItemNum[i].sSid = -1; 
		m_TradeItemNum[i].sNum = 0; 
	}

	EnterCriticalSection( &m_CS_ExchangeItem );
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)
	{
		if ( m_arExchangeItem[i] )
		{
			delete m_arExchangeItem[i];
			m_arExchangeItem[i] = NULL;
		}
	}
	m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ExchangeItem );

	//////////////////////////////////////////////////////////////////////////

	if(pSendUser->m_bNowTrading == TRUE || pSendUser->m_iTradeUid != -1)	//bug 交易bug
	{
		SendTradeFail(uid, ERR_3);
	
		pSendUser->SoftClose();
		SoftClose();

		return;
	}
	if(m_bNowTrading == TRUE || m_iTradeUid != -1)				//bug 交易bug
	{
		SendTradeFail(uid, ERR_3);

		SoftClose();
		return;
	}




	if(IsThereUser(pSendUser) == FALSE)
	{
		SendTradeFail(uid, ERR_2);
		return;
	}
	switch(tMode)
	{
	case 0:			// Trade 夸没阑 秒家窍绰 版快
		if(m_bTradeWaiting == TRUE && pSendUser->m_bNowTrading == FALSE)					
		{											// 惑措规 铰倡傈, 夸没焊郴绊 促矫 秒家 且锭		
			pSendUser->SendTradeFail(m_uid + USER_BAND, (BYTE)0x00);
			m_bTradeWaiting = FALSE;
			return;
		}
		if(pSendUser->m_bNowTrading == TRUE)		// 铰倡茄第 芭例老锭
		{
			SendTradeFail(uid, (BYTE)0x00);
			pSendUser->SendTradeFail(m_uid + USER_BAND, (BYTE)0x00);
			return;
		}
		return;
	case 1:			// Trade 夸没
		m_bTradeWaiting = TRUE;
		break;
	case 2:			// Trade 铰倡
		if(pSendUser->m_bTradeWaiting != TRUE)
		{
			SendTradeFail(uid, UNKNOWN_ERR);
			return;
		}
		_tcscpy(m_strTradeUser, pSendUser->m_strUserID);
		_tcscpy(pSendUser->m_strTradeUser, m_strUserID);

					// 芭贰 铰倡锭 檬扁拳 鞘夸
		m_TradeMoney = 0;
		pSendUser->m_TradeMoney = 0;

		m_iTradeUid = uid;
		pSendUser->m_iTradeUid = m_uid + USER_BAND;
		
		m_bNowTrading = TRUE;
		pSendUser->m_bNowTrading = TRUE;

		SendTradeSuccess(uid, pSendUser->m_strUserID);
		pSendUser->SendTradeSuccess(m_uid + USER_BAND, m_strUserID);
//		SendTradeSuccess(uid);
//		pSendUser->SendTradeSuccess(m_uid + USER_BAND);

		for(i = 0; i < TOTAL_ITEM_NUM; i++) 
		{
			m_TradeItemNum[i].sSid = m_UserItem[i].sSid;
			m_TradeItemNum[i].sNum = m_UserItem[i].sCount;
			pSendUser->m_TradeItemNum[i].sSid = pSendUser->m_UserItem[i].sSid;
			pSendUser->m_TradeItemNum[i].sNum = pSendUser->m_UserItem[i].sCount;
		}
		return;
	case 3:			// Trade 芭例
		SendTradeFail(uid, ERR_6);
		pSendUser->SendTradeFail(uid, ERR_1);
		return;
	default:
		SendTradeFail(uid, UNKNOWN_ERR);
		return;
	}

	CBufferEx TempBuf;

	TempBuf.Add(ITEM_TRADE_REQ);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	TempBuf.Add(tMode);	
	
	pSendUser->Send(TempBuf, TempBuf.GetLength());

	//yskang 0.4 款康磊 肺弊 扁废窍扁
	if( m_tIsOP == 1 )
    {
	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	sprintf(strOP,"交易记录:帐户%s :角色%s",pSendUser->m_strAccount, pSendUser->m_strUserID);
	WriteOpratorLog(strOP,ITEM_LOG);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	酒捞袍 背券夸没 贸府甫 茄促.
//
void USER::ExchangeReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1 )
	{
		SendSystemMsg( "请先解除密码保护系统!.", SYSTEM_ERROR, TO_ME);
		SendExchangeFail((BYTE)0x00, (BYTE)0x02);
		return;
	}
	if(m_bPShopOpen == TRUE) return;
	BOOL flag = FALSE;
	int j, iNum = 0;
	int index = 0;
	int uid = 0;
	BYTE byType = 0, bySlot = 0;
	DWORD dwCount = 0;
	BYTE result = 0;
	BYTE error_code = UNKNOWN_ERR;

	short sCount = 0;

	USER* pSendUser = NULL;
	USER* pTempUser = NULL;

//	if(m_bTradeMode == FALSE || m_bNowTrading != TRUE)
	if(m_bNowTrading != TRUE)
	{
		if(m_iTradeUid != -1) 	pSendUser = GetUser(m_iTradeUid - USER_BAND);
		if(pSendUser != NULL) 
		{
			SendExchangeFail((BYTE)0x00, (BYTE)0x02);
			pSendUser->SendExchangeFail((BYTE)0x00, (BYTE)0x02);
		}
		return;
	}

	ItemList	TempItem;
	ExchangeItem* pNewItem = NULL;

	uid		= GetInt(pBuf, index);
	byType	= GetByte(pBuf, index);

	CBufferEx TempBuf;
	CBufferEx TempMyBuf;

	if(uid < 0 || uid >= NPC_BAND)				// 肋给等 惑措祈 uid搁 芭贰 秒家
	{
		result = 0x00;
		error_code = ERR_1;
		SendExchangeFail(result, error_code);

		pTempUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTempUser) 
		{
			pTempUser->SendExchangeFail(result, UNKNOWN_ERR);
		}

		return;
	}

	pSendUser = GetUser(uid - USER_BAND);		// 肋给等 蜡历 
	if(m_bPShopOpen==TRUE||pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_bPShopOpen==TRUE)
	{
		result = 0x00;
		error_code = ERR_1;
		SendExchangeFail(result, error_code);

		pTempUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTempUser) 
		{
			pTempUser->SendExchangeFail(result, UNKNOWN_ERR);
		}

		return;
	}

	if(_tcscmp(m_strTradeUser, pSendUser->m_strUserID) != 0)		// 贸澜俊 芭贰窍带 蜡历啊 酒丛
	{
		result = 0x00;
		error_code = ERR_2;
		SendExchangeFail(result, error_code);

		pTempUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTempUser) 
		{
			pTempUser->SendExchangeFail(result, UNKNOWN_ERR);
		}

		return;
	}

	switch(byType)
	{
	case 0:				// 背券秒家
		result = 0x00;
		SendExchangeFail(result, (BYTE)0x04);
		pSendUser->SendExchangeFail(result, (BYTE)0x05);
		return;
	case 1:				// 背券铰倡
		if(m_bExchangeOk == FALSE) 
		{
			m_bExchangeOk = TRUE;

			index = 0;
			SetByte(m_TempBuf, EXCHANGE_REQ, index);
			SetInt(m_TempBuf, uid, index);
			SetByte(m_TempBuf, 0x01, index);
			Send(m_TempBuf, index);

			index = 0;
			SetByte(TempBuf, EXCHANGE_REQ, index);
			SetInt(TempBuf, m_uid + USER_BAND, index);
			SetByte(TempBuf, 0x02, index);
			pSendUser->Send(TempBuf, index);

		}
		if(m_bExchangeOk == TRUE && pSendUser->m_bExchangeOk == TRUE)
		{
			EnterCriticalSection( &m_CS_ExchangeItem );
			for(j = 0; j < m_arExchangeItem.GetSize(); j++)		// 公霸 力茄俊 吧府唱 眉农...
			{
				if ( m_arExchangeItem[j] )
				{
					index = 0;
					index = m_arExchangeItem[j]->changedList.sSid;
					if(index < 0 || index >= g_arItemTable.GetSize()) continue;

					if(g_arItemTable[index]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND) 
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( IDS_USER_CANT_BUYSELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
						pSendUser->SendSystemMsg( IDS_USER_CANT_BUYSELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
						return;
					}
					//////////////////////////////////交易密码
					if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( "物品保护未解除或对方未解除不能交易", SYSTEM_ERROR, TO_ME);//"无法交易任务物品."
						pSendUser->SendSystemMsg( "物品保护未解除或对方未解除不能交易", SYSTEM_ERROR, TO_ME);//"无法交易任务物品."
						return;
					}
					//////////////////////////////////////////

					bySlot = m_arExchangeItem[j]->bySlot;				
					sCount = m_arExchangeItem[j]->changedList.sCount;
					iNum += (g_arItemTable[index]->m_byWeight * sCount);
					if(pSendUser->m_iMaxWeight <= pSendUser->m_iCurWeight + iNum)
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( IDS_USER_OVER_WEIGHT_OTHER, SYSTEM_ERROR, TO_ME);
						pSendUser->SendSystemMsg( IDS_USER_OVER_WEIGHT_SAVE, SYSTEM_ERROR, TO_ME);
						return;
					}
				}
			}
			LeaveCriticalSection( &m_CS_ExchangeItem );


			EnterCriticalSection( &pSendUser->m_CS_ExchangeItem );
			for(j = 0; j < pSendUser->m_arExchangeItem.GetSize(); j++)		// 公霸 力茄俊 吧府唱 眉农...
			{			
				if ( pSendUser->m_arExchangeItem[j] )
				{
					index = 0;
					index = pSendUser->m_arExchangeItem[j]->changedList.sSid;
					if(index < 0 || index >= g_arItemTable.GetSize()) continue;

					if(g_arItemTable[index]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND) 
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( IDS_USER_CANT_BUYSELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
						pSendUser->SendSystemMsg( IDS_USER_CANT_BUYSELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
						return;
					}
					////////////////////////////////////////// 交易密码
					if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);						
						SendSystemMsg( "物品保护未解除或对方未解除不能交易", SYSTEM_ERROR, TO_ME);//"无法交易任务物品."
						pSendUser->SendSystemMsg( "物品保护未解除或对方未解除不能交易", SYSTEM_ERROR, TO_ME);//"无法交易任务物品."
						return;
					}
					///////////////////////////////////////////////////

					bySlot = pSendUser->m_arExchangeItem[j]->bySlot;
					sCount = pSendUser->m_arExchangeItem[j]->changedList.sCount;
					iNum += (g_arItemTable[index]->m_byWeight * sCount);
					if(m_iMaxWeight < m_iCurWeight + iNum)
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x06);
						pSendUser->SendExchangeFail(result, 0x07);
						SendSystemMsg( IDS_USER_OVER_WEIGHT_SAVE, SYSTEM_ERROR, TO_ME);
						pSendUser->SendSystemMsg( IDS_USER_OVER_WEIGHT_SAVE_OTHER, SYSTEM_ERROR, TO_ME);
						return;
					}
				}
			}
			LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

			RunExchange(pSendUser);
		}

		break;

	case 2:			// 鸥牢捞 背券 铰倡(辑滚父捞 且 荐 乐促)
		result = 0x00;
		error_code = UNKNOWN_ERR;
		SendExchangeFail(result, error_code);
		pSendUser->SendExchangeFail(result, UNKNOWN_ERR);
		return;

	case 3:			// 磊脚捞 酒捞袍 棵覆
		if(m_bExchangeOk == TRUE) return;		// 悼狼窍搁 歹捞惑狼 酒捞袍 眠啊甫 芭何茄促.

		bySlot	= GetByte(pBuf, index);
		sCount	= GetShort(pBuf, index);

		if(bySlot < EQUIP_ITEM_NUM || bySlot >= TOTAL_INVEN_MAX) //|| sCount > MAX_ITEM_NUM)
		{
			result = 0x00;
			error_code = ERR_3;

			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);

			return;
		}

		if(sCount <= 0 || sCount >= _MAX_SHORT)
		{
			result = 0x00;
			error_code = ERR_3;

			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);

			return;
		}

		if(m_TradeItemNum[bySlot].sSid < 0 || m_TradeItemNum[bySlot].sNum < sCount)
		{
			result = 0x00;
			error_code = ERR_3;

			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}
		m_bTradeItem = TRUE;		//酒捞袍阑 棵妨躇促. 
		ReSetItemSlot(&TempItem);	// 鞍篮 酒捞袍阑 般摹扁困秦 檬扁拳	

		iNum = m_TradeItemNum[bySlot].sNum;

		if(iNum <= 0 ) 
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code); //bug 交易
			return;
		}
		m_TradeItemNum[bySlot].sNum = iNum - sCount;
		if(m_TradeItemNum[bySlot].sNum < 0) 
			{
			m_TradeItemNum[bySlot].sNum = 0;
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code); //bug 交易
			return;
		}


		TempItem = m_UserItem[bySlot];
		TempItem.sCount = sCount;

		if(TempItem.sSid < 0 || TempItem.sSid >= g_arItemTable.GetSize())		// 促矫茄馆 犬牢
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}


		if(g_arItemTable[TempItem.sSid]->m_sDuration <= 0) //4月10日  BUG   
		{
			iNum = GetSameItem(TempItem, TRADE_SLOT);
			//牛牛ADD 
			if(iNum != -1 && iNum == bySlot)
			{
				if(m_arExchangeItem[iNum]->changedList.sCount + sCount >= _MAX_SHORT)
				{
					result = 0x00;
					error_code = ERR_3;
					SendExchangeFail(result, error_code);
					pSendUser->SendExchangeFail(result, error_code);
					return;
				}

				EnterCriticalSection( &m_CS_ExchangeItem );
				m_arExchangeItem[iNum]->changedList.sCount += sCount;
				LeaveCriticalSection( &m_CS_ExchangeItem );

				SendExchangeItem(pSendUser, bySlot, (DWORD)sCount);
				return;
			}
			iNum = pSendUser->GetSameItem(TempItem, INVENTORY_SLOT);
			if(iNum != -1)
			{
				if(pSendUser->m_UserItem[iNum].sCount + TempItem.sCount > MAX_ITEM_NUM)
				{
					result = 0x02;
					error_code = ERR_3;
					SendExchangeFail(result, error_code);
					pSendUser->SendExchangeFail(result, error_code);
					SendSystemMsg( IDS_USER_OVER_COUNT_SAVE_OTHER, SYSTEM_ERROR, TO_ME);
					return;
				}
			}
		}

		EnterCriticalSection( &m_CS_ExchangeItem );
		if(m_arExchangeItem.GetSize() >= INVENTORY_NUM)
		{
			LeaveCriticalSection( &m_CS_ExchangeItem );

			result = 0x00;
			SendExchangeFail(result, 0x06);
			pSendUser->SendExchangeFail(result, 0x07);
			return;
		}
		LeaveCriticalSection( &m_CS_ExchangeItem );

		pNewItem = new ExchangeItem;
		pNewItem->bySlot = bySlot;
		pNewItem->changedList.sLevel = m_UserItem[bySlot].sLevel;
		pNewItem->changedList.sSid = m_UserItem[bySlot].sSid;
		pNewItem->changedList.sDuration = m_UserItem[bySlot].sDuration;
		pNewItem->changedList.sBullNum = m_UserItem[bySlot].sBullNum;
		pNewItem->changedList.sCount = sCount;
		for(j =0; j < MAGIC_NUM; j++) pNewItem->changedList.tMagic[j] = m_UserItem[bySlot].tMagic[j];
		pNewItem->changedList.tIQ = m_UserItem[bySlot].tIQ;
		pNewItem->changedList.iItemSerial = m_UserItem[bySlot].iItemSerial;

		EnterCriticalSection( &m_CS_ExchangeItem );
		m_arExchangeItem.Add(pNewItem);
		LeaveCriticalSection( &m_CS_ExchangeItem );

		SendExchangeItem(pSendUser, bySlot, (DWORD)sCount);
		break;

	case 4:			// 鸥牢捞 酒捞袍 棵覆(辑滚父捞 临 荐 乐促)
		result = 0x00;
		error_code = UNKNOWN_ERR;
		SendExchangeFail(result, error_code);
		pSendUser->SendExchangeFail(result, UNKNOWN_ERR);
		return;

	case 5:			// 磊脚捞 捣 棵覆
		dwCount	= GetDWORD(pBuf, index);
		if(dwCount <= 0 || dwCount > m_dwDN)
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}

//		m_TradeMoney = m_TradeMoney + dwCount;
		CheckMaxValue((DWORD &)m_TradeMoney, (DWORD)dwCount);
		if(m_TradeMoney > m_dwDN)
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}

		bySlot = 255;
		SendExchangeItem(pSendUser, bySlot, m_TradeMoney);
//		SendExchangeItem(pSendUser, bySlot, dwCount);
		break;

	case 6:			// 鸥牢捞 捣 棵覆(辑滚父捞 临 荐 乐促)
	default:
		result = 0x00;
		error_code = UNKNOWN_ERR;
		SendExchangeFail(result, error_code);
		pSendUser->SendExchangeFail(result, UNKNOWN_ERR);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////////
//	酒捞袍 背券 角菩 菩哦阑 焊辰促.
//
void USER::SendExchangeFail(BYTE result, BYTE error_code)
{
	if ( m_UserStatus == 0 && m_UserFlag == FALSE ) return;

	USER* pTempUser = NULL;
	CBufferEx TempBuf, TempMyBuf;
	int i, iNum = 0;

	BYTE bySlot = 0;
	
	TempBuf.Add(EXCHANGE_RESULT);
	TempBuf.Add(result);
	TempBuf.Add(error_code);

	Send(TempBuf, TempBuf.GetLength());

	if((m_bTradeItem == TRUE || m_TradeMoney > 0) && result == 0)			// 父距 背券吝捞绊, 酒捞袍阑 棵妨躇阑版快 
	{
		EnterCriticalSection( &m_CS_ExchangeItem );
		iNum = m_arExchangeItem.GetSize();				// 棵妨初篮 酒捞袍 荐父怒 荤侩磊俊霸 促矫 舅妨淋
		if(iNum > 0)
		{
			TempMyBuf.Add(ITEM_MOVE_RESULT);
			TempMyBuf.Add(SUCCESS);
			TempMyBuf.Add((BYTE)0x00);
			TempMyBuf.Add((BYTE)iNum);

			for(i = 0; i < iNum; i++)
			{
				if ( m_arExchangeItem[i] == NULL ) continue;

				bySlot = m_arExchangeItem[i]->bySlot;

				TempMyBuf.Add(bySlot);	
				TempMyBuf.Add(m_UserItem[bySlot].sLevel);
				TempMyBuf.Add(m_UserItem[bySlot].sSid);
				TempMyBuf.Add(m_UserItem[bySlot].sDuration);
				TempMyBuf.Add(m_UserItem[bySlot].sBullNum);
				TempMyBuf.Add(m_UserItem[bySlot].sCount);

				for(int j =0; j < MAGIC_NUM; j++) TempMyBuf.Add(m_UserItem[bySlot].tMagic[j]);

				TempMyBuf.Add(m_UserItem[bySlot].tIQ); 
			}
			Send(TempMyBuf, TempMyBuf.GetLength());
		}
		LeaveCriticalSection( &m_CS_ExchangeItem );

		if(m_TradeMoney > 0) SendMoneyChanged();
	}

	if(result == 0 || result == 2)
	{
		_tcscpy(m_strTradeUser, _T(""));// 惑措规阑 犬牢窍扁 困茄 蜡历捞抚阑 瘤款促.
		m_bNowTrading = m_bTradeWaiting = FALSE;
		m_bExchangeOk = m_bTradeItem	= FALSE;// 背券吝 惑怕甫 檬扁拳
		m_iTradeUid = -1;
		m_TradeMoney = 0;				// 芭贰 陛咀阑 檬扁拳

		for(i = 0; i < TOTAL_ITEM_NUM; i++) // 芭贰吝 牢亥俊 官差绰 酒捞袍 荐甫 眉农困秦
		{ 
			m_TradeItemNum[i].sSid = -1; 
			m_TradeItemNum[i].sNum = 0; 
		}

		EnterCriticalSection( &m_CS_ExchangeItem );
		for(i = 0; i < m_arExchangeItem.GetSize(); i++)
		{
			if ( m_arExchangeItem[i] )
			{
				delete m_arExchangeItem[i];
				m_arExchangeItem[i] = NULL;
			}
		}
		m_arExchangeItem.RemoveAll();
		LeaveCriticalSection( &m_CS_ExchangeItem );
	}
}

///////////////////////////////////////////////////////////////////////////////
//	背券芒俊 酒捞袍 棵覆
//
void USER::SendExchangeItem(USER *pUser, BYTE bySlot, DWORD dwCount)
{
	int j;
	short sCount;
	short sSid = -1;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	CBufferEx TempBuf, TempMyBuf;

	//----------------------------[陛咀 ]------------------------------//
	if(bySlot == 255)
	{
		DWORD tempDN = 0;
		if(m_dwDN < dwCount) { tempDN = 0; m_dwDN = 0; }
		else  tempDN = m_dwDN - dwCount;

		TempBuf.Add(EXCHANGE_REQ);				
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add((BYTE)0x06);				// 惑措祈俊霸 捣阑 焊辰促.
		TempBuf.Add(dwCount);

		TempMyBuf.Add(EXCHANGE_REQ);			// 磊扁俊霸 焊辰促
		TempMyBuf.Add(pUser->m_uid + USER_BAND);
		TempMyBuf.Add((BYTE)0x05);
		TempMyBuf.Add(dwCount);
		TempMyBuf.Add(tempDN);
		goto go_result;
	}

	//----------------------------[惑措规]------------------------------//
	TempBuf.Add(EXCHANGE_REQ);					// 惑措祈俊霸 焊辰促.
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((BYTE)0x04);

	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add((short)dwCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ); 

	//----------------------------[磊扁 ]------------------------------//
	TempMyBuf.Add(EXCHANGE_REQ);				// 飘饭捞靛俊 棵扼哎 酒捞袍
	TempMyBuf.Add(pUser->m_uid + USER_BAND);	
	TempMyBuf.Add((BYTE)0x03);				
	
	TempMyBuf.Add(m_UserItem[bySlot].sLevel);
	TempMyBuf.Add(m_UserItem[bySlot].sSid);
	TempMyBuf.Add(m_UserItem[bySlot].sDuration);
	TempMyBuf.Add(m_UserItem[bySlot].sBullNum);
	TempMyBuf.Add((short)dwCount);
	for(j =0; j < MAGIC_NUM; j++) TempMyBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempMyBuf.Add(m_UserItem[bySlot].tIQ); 
	
	sCount = m_TradeItemNum[bySlot].sNum;// - (short)dwCount;//m_UserItem[bySlot].sCount - (short)dwCount;
	TempMyBuf.Add(bySlot);						
	
	TempMyBuf.Add(m_UserItem[bySlot].sLevel);

	if(sCount <= 0) TempMyBuf.Add(sSid);
	else			TempMyBuf.Add(m_UserItem[bySlot].sSid);

	TempMyBuf.Add(m_UserItem[bySlot].sDuration);
	TempMyBuf.Add(m_UserItem[bySlot].sBullNum);
	TempMyBuf.Add(sCount);
	for(j =0; j < MAGIC_NUM; j++) TempMyBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempMyBuf.Add(m_UserItem[bySlot].tIQ); 

go_result:
	pUser->Send(TempBuf, TempBuf.GetLength());
	Send(TempMyBuf, TempMyBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////////
//	角力 背券贸府甫 茄促.
//
void USER::RunExchange(USER* pUser)
{
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) 
	{
		SendExchangeFail((BYTE)0x00, ERR_1);
		return;
	}

	if ( pUser->m_UserStatus == 0 && pUser->m_UserFlag == FALSE )
	{
		SendExchangeFail((BYTE)0x00, ERR_1);
		return;
	}

	BYTE	result		= 0x01;				// 背券悼狼
	BYTE	tNumberMy	= m_arExchangeItem.GetSize();
	BYTE	tNumberYour	= pUser->m_arExchangeItem.GetSize();

	ItemList		MyItem[TOTAL_ITEM_NUM], YourItem[TOTAL_ITEM_NUM];
	ItemList		TempItem;

	ItemList*		pNewItemList = NULL;
	ItemListArray	arItemListMy, arItemListYour;
	CByteArray		arSlotMy, arSlotYour;

	int i = 0, j = 0;
	int iMyWeight = 0, iYourWeight = 0;

	for(i = 0; i < TOTAL_ITEM_NUM; i++)			// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
		YourItem[i] = pUser->m_UserItem[i];
	}

	iMyWeight = m_iCurWeight;							// 泅犁 酒捞袍 公霸甫 归诀
	iYourWeight = pUser->m_iCurWeight;

	//----------------------------[init values]------------------------------//
	CBufferEx	TempBuf;
	CBufferEx	TempBuf2;
	CBufferEx	TempBuf3;
	CBufferEx	TempBuf4;

	int			iWear = -1;
	int			nSizeYour = 0;
	int			nSizeMy = 0;
	BOOL		bIsOk = TRUE;
	int			iQuickChange = 0, yQuickChange = 0;
	int			iSameSlot = -1, iEmptySlot = -1;
	short		sSid = -1;
	BYTE		tSlot = 0;
	short		sCount = 0;
	DWORD		dwMyTempDN = 0, dwYourTempDN = 0;
	BOOL		bChangeMyEquip = TRUE, bChangeYourEquip = TRUE;
	//----------------------------[ User ]-----------------------------------//

	if((tNumberYour == 0 && tNumberMy == 0) && (m_TradeMoney <= 0 && pUser->m_TradeMoney <= 0))
	{
		SendExchangeFail(0x00, ERR_3);			// 笛促 酒捞袍, 捣 阑 救棵副版快
		pUser->SendExchangeFail(0x00, ERR_3);
		goto go_result;
	}
	
	EnterCriticalSection( &m_CS_ExchangeItem );
	//tNumberMy = m_arExchangeItem.GetSize();
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)				// 磊脚捞 林绰 酒捞袍 贸府
	{
		if ( m_arExchangeItem[i] == NULL ) continue;

		tSlot = (BYTE)m_arExchangeItem[i]->bySlot;

		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bIsOk = FALSE; break;}
		sCount = m_arExchangeItem[i]->changedList.sCount;
		if(sCount < 0)  { bIsOk = FALSE; break;}	// Add jjs07

		if(m_UserItem[tSlot].sSid < 0 || m_UserItem[tSlot].sSid >= g_arItemTable.GetSize())	{ bIsOk = FALSE; break;}
		
		m_iCurWeight -= (g_arItemTable[m_UserItem[tSlot].sSid]->m_byWeight * sCount);
		if(m_iCurWeight < 0) m_iCurWeight = 0;

		// 林绰 酒捞袍阑 肺彪茄促
		// 酒捞袍捞 函版登扁 傈俊 肺彪阑 巢变促
		MakeItemLog( &m_UserItem[tSlot], ITEMLOG_EXCHANGE_GIVE, pUser->m_strUserID );

		m_UserItem[tSlot].sCount -= sCount;
		if(m_UserItem[tSlot].sCount <= 0) 
		{
			m_UserItem[tSlot].sCount = 0;
			m_UserItem[tSlot].sSid = -1;
		}

		pNewItemList = new ItemList;
		pNewItemList->sSid = m_UserItem[tSlot].sSid;
		pNewItemList->sLevel = m_UserItem[tSlot].sLevel;
		pNewItemList->sBullNum = m_UserItem[tSlot].sBullNum;
		pNewItemList->sDuration = m_UserItem[tSlot].sDuration;
		pNewItemList->sCount = m_UserItem[tSlot].sCount;

		for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = m_UserItem[tSlot].tMagic[j];
		pNewItemList->tIQ = m_UserItem[tSlot].tIQ; 
		pNewItemList->iItemSerial = m_UserItem[tSlot].iItemSerial;

		arItemListMy.Add(pNewItemList);
		arSlotMy.Add(tSlot);
	}
	LeaveCriticalSection( &m_CS_ExchangeItem );

	EnterCriticalSection( &pUser->m_CS_ExchangeItem );
	//tNumberYour	= pUser->m_arExchangeItem.GetSize();
	for(i = 0; i < pUser->m_arExchangeItem.GetSize(); i++)		// 巢捞 林绰 酒捞袍 贸府
	{
		if ( pUser->m_arExchangeItem[i] == NULL ) continue;

		tSlot = (BYTE)pUser->m_arExchangeItem[i]->bySlot;
		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bIsOk = FALSE; break;}

		sCount = pUser->m_arExchangeItem[i]->changedList.sCount;
		if(sCount < 0) { bIsOk = FALSE; break;}		// Add jjs07

		ReSetItemSlot(&TempItem);
		TempItem = pUser->m_UserItem[tSlot];
		TempItem.sCount = sCount;

		if(TempItem.sSid < 0 || TempItem.sSid >= g_arItemTable.GetSize()) { bIsOk = FALSE; break;}
		iSameSlot = GetSameItem(TempItem, INVENTORY_SLOT);

		if(iSameSlot == -1)					// 鞍篮 辆幅狼 酒捞袍捞 绝阑 版快
		{
			iEmptySlot = GetEmptySlot(INVENTORY_SLOT);
			if(iEmptySlot == -1)			// 鞍篮 辆幅狼 酒捞袍档 绝绊 后浇吩档 绝阑 版快
			{
				bIsOk = FALSE;
				break;
			}

			// 后浇吩捞 乐阑 版快
			m_UserItem[iEmptySlot] = TempItem;

			MakeItemLog( &m_UserItem[iEmptySlot], ITEMLOG_EXCHANGE_RECEIVE, pUser->m_strUserID );

			pNewItemList = new ItemList;
			pNewItemList->sSid = m_UserItem[iEmptySlot].sSid;
			pNewItemList->sLevel = m_UserItem[iEmptySlot].sLevel;
			pNewItemList->sBullNum = m_UserItem[iEmptySlot].sBullNum;
			pNewItemList->sDuration = m_UserItem[iEmptySlot].sDuration;
			pNewItemList->sCount = m_UserItem[iEmptySlot].sCount;
	
			for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = m_UserItem[iEmptySlot].tMagic[j];
			pNewItemList->tIQ = m_UserItem[iEmptySlot].tIQ; 
			pNewItemList->iItemSerial = m_UserItem[iEmptySlot].iItemSerial;
			
			arItemListMy.Add(pNewItemList);
			arSlotMy.Add((BYTE)iEmptySlot);
		}
		else								// 鞍篮 辆幅狼 酒捞袍捞 乐阑 版快
		{
			m_UserItem[iSameSlot].sCount += TempItem.sCount;

			pNewItemList = new ItemList;
			pNewItemList->sSid = m_UserItem[iSameSlot].sSid;
			pNewItemList->sLevel = m_UserItem[iSameSlot].sLevel;
			pNewItemList->sBullNum = m_UserItem[iSameSlot].sBullNum;
			pNewItemList->sDuration = m_UserItem[iSameSlot].sDuration;
			pNewItemList->sCount = m_UserItem[iSameSlot].sCount;
		
			for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = m_UserItem[iSameSlot].tMagic[j];
			pNewItemList->tIQ = m_UserItem[iSameSlot].tIQ; 
			pNewItemList->iItemSerial = m_UserItem[iSameSlot].iItemSerial;

			arItemListMy.Add(pNewItemList);
			arSlotMy.Add((BYTE)iSameSlot);
		}

		if(TempItem.sSid >= 0) m_iCurWeight += g_arItemTable[TempItem.sSid]->m_byWeight * TempItem.sCount;
		if(m_iCurWeight < 0) m_iCurWeight = 0;
	}
	LeaveCriticalSection( &pUser->m_CS_ExchangeItem );

	if(bIsOk == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// Item Information Backup
		{
			m_UserItem[i] = MyItem[i];
			pUser->m_UserItem[i] = YourItem[i];
		}
		SendExchangeFail(0x00, ERR_6);
		pUser->SendExchangeFail(0x00, ERR_7);
		goto go_result;
	}

	//-----------------------------[Send User]----------------------------------//		
	EnterCriticalSection( &pUser->m_CS_ExchangeItem );
	//tNumberYour	= pUser->m_arExchangeItem.GetSize();
	for(i = 0; i < pUser->m_arExchangeItem.GetSize(); i++)			// 磊脚捞 林绰 酒捞袍 贸府
	{
		if ( pUser->m_arExchangeItem[i] == NULL ) continue;

		tSlot = (BYTE)pUser->m_arExchangeItem[i]->bySlot;
		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bIsOk = FALSE; break;}

		sCount = pUser->m_arExchangeItem[i]->changedList.sCount;
		if(sCount < 0) { bIsOk = FALSE; break;}		// Add jjs07

		if(pUser->m_UserItem[tSlot].sSid < 0 || pUser->m_UserItem[tSlot].sSid >= g_arItemTable.GetSize()) { bIsOk = FALSE; break;}

		pUser->m_iCurWeight -= g_arItemTable[pUser->m_UserItem[tSlot].sSid]->m_byWeight * sCount;
		if(pUser->m_iCurWeight < 0) pUser->m_iCurWeight = 0;

		pUser->m_UserItem[tSlot].sCount -= sCount;
		if(pUser->m_UserItem[tSlot].sCount <= 0)
		{
			pUser->m_UserItem[tSlot].sCount = 0;
			pUser->m_UserItem[tSlot].sSid = -1;
		}

		pNewItemList = new ItemList;
		pNewItemList->sSid = pUser->m_UserItem[tSlot].sSid;
		pNewItemList->sLevel = pUser->m_UserItem[tSlot].sLevel;
		pNewItemList->sBullNum = pUser->m_UserItem[tSlot].sBullNum;
		pNewItemList->sDuration = pUser->m_UserItem[tSlot].sDuration;
		pNewItemList->sCount = pUser->m_UserItem[tSlot].sCount;

		for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = pUser->m_UserItem[tSlot].tMagic[j];
		pNewItemList->tIQ = pUser->m_UserItem[tSlot].tIQ;
		pNewItemList->iItemSerial = pUser->m_UserItem[tSlot].iItemSerial;

		arItemListYour.Add(pNewItemList);
		arSlotYour.Add(tSlot);
	}
	LeaveCriticalSection( &pUser->m_CS_ExchangeItem );

	EnterCriticalSection( &m_CS_ExchangeItem );
	//tNumberMy = m_arExchangeItem.GetSize();
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)		// 巢捞 林绰 酒捞袍 贸府--> arItemListMy俊辑 罐酒具茄促.
	{
		if ( m_arExchangeItem[i] == NULL ) continue;

		tSlot = (BYTE)m_arExchangeItem[i]->bySlot;

		// 困俊辑 林绰 酒捞袍篮 函悼捞 绝栏骨肺 馆康茄促.
		TempItem.sLevel		= m_arExchangeItem[i]->changedList.sLevel;
		TempItem.sSid		= m_arExchangeItem[i]->changedList.sSid;
		TempItem.sCount		= m_arExchangeItem[i]->changedList.sCount;
		TempItem.sBullNum	= m_arExchangeItem[i]->changedList.sBullNum;
		TempItem.sDuration	= m_arExchangeItem[i]->changedList.sDuration;

		for(j =0; j < MAGIC_NUM; j++) TempItem.tMagic[j] = m_arExchangeItem[i]->changedList.tMagic[j];
		TempItem.tIQ = m_arExchangeItem[i]->changedList.tIQ;
		TempItem.iItemSerial = m_arExchangeItem[i]->changedList.iItemSerial;

		if(TempItem.sSid < 0 || TempItem.sSid >= g_arItemTable.GetSize()) { bIsOk = FALSE; break;}
		iSameSlot = pUser->GetSameItem(TempItem, INVENTORY_SLOT);

		if(iSameSlot == -1)					// 鞍篮 辆幅狼 酒捞袍捞 绝阑 版快
		{
			iEmptySlot = pUser->GetEmptySlot(INVENTORY_SLOT);
			if(iEmptySlot == -1)			// 鞍篮 辆幅狼 酒捞袍档 绝绊 后浇吩档 绝阑 版快
			{
				bIsOk = FALSE;
				break;
			}

			// 后浇吩捞 乐阑 版快
			pUser->m_UserItem[iEmptySlot] = TempItem;

			pNewItemList = new ItemList;
			pNewItemList->sLevel = pUser->m_UserItem[iEmptySlot].sLevel;
			pNewItemList->sSid = pUser->m_UserItem[iEmptySlot].sSid;
			pNewItemList->sBullNum = pUser->m_UserItem[iEmptySlot].sBullNum;
			pNewItemList->sDuration = pUser->m_UserItem[iEmptySlot].sDuration;
			pNewItemList->sCount = pUser->m_UserItem[iEmptySlot].sCount;

			for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = pUser->m_UserItem[iEmptySlot].tMagic[j];
			pNewItemList->tIQ = pUser->m_UserItem[iEmptySlot].tIQ; 
			pNewItemList->iItemSerial = pUser->m_UserItem[iEmptySlot].iItemSerial;
			
			arItemListYour.Add(pNewItemList);
			arSlotYour.Add((BYTE)iEmptySlot);
		}
		else								// 鞍篮 辆幅狼 酒捞袍捞 乐阑 版快
		{
			pUser->m_UserItem[iSameSlot].sCount += TempItem.sCount;
			
			pNewItemList = new ItemList;
			pNewItemList->sLevel = pUser->m_UserItem[iSameSlot].sLevel;
			pNewItemList->sSid = pUser->m_UserItem[iSameSlot].sSid;
			pNewItemList->sBullNum = pUser->m_UserItem[iSameSlot].sBullNum;
			pNewItemList->sDuration = pUser->m_UserItem[iSameSlot].sDuration;
			pNewItemList->sCount = pUser->m_UserItem[iSameSlot].sCount;

			for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = pUser->m_UserItem[iSameSlot].tMagic[j];
			pNewItemList->tIQ = pUser->m_UserItem[iSameSlot].tIQ; 
			pNewItemList->iItemSerial = pUser->m_UserItem[iSameSlot].iItemSerial;
			
			arItemListYour.Add(pNewItemList);
			arSlotYour.Add((BYTE)iSameSlot);
		}

		if(TempItem.sSid >= 0) pUser->m_iCurWeight += g_arItemTable[TempItem.sSid]->m_byWeight * TempItem.sCount;
	}
	LeaveCriticalSection( &m_CS_ExchangeItem );

	if(bIsOk == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// Item Information Backup
		{
			m_UserItem[i] = MyItem[i];
			pUser->m_UserItem[i] = YourItem[i];
		}
		SendExchangeFail(0x00, ERR_7);
		pUser->SendExchangeFail(0x00, ERR_6);
		goto go_result;
	}

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)// 扁利利栏肺 惑措祈捞 蜡历 府胶飘俊辑 昏力登菌栏搁...
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// Item Information Backup
		{
			m_UserItem[i] = MyItem[i];
		}
		SendExchangeFail(0x00, ERR_5);
		goto go_result;
	}


	//----------------[ 陛咀阑 函悼矫挪促.]----------------------------------//
	dwMyTempDN = m_dwDN;
	dwYourTempDN = pUser->m_dwDN;
	CheckMaxValue((DWORD &)m_dwDN, (DWORD)pUser->m_TradeMoney);

	MakeMoneyLog( pUser->m_TradeMoney, ITEMLOG_EXCHANGE_RECEIVE_MONEY, pUser->m_strUserID );

	if(m_dwDN <= m_TradeMoney) m_dwDN = 0;
	else m_dwDN = m_dwDN - m_TradeMoney;

	CheckMaxValue((DWORD &)pUser->m_dwDN, (DWORD)m_TradeMoney);

	if(pUser->m_dwDN <= pUser->m_TradeMoney) pUser->m_dwDN = 0;
	else  pUser->m_dwDN = pUser->m_dwDN - pUser->m_TradeMoney;

	MakeMoneyLog( m_TradeMoney, ITEMLOG_EXCHANGE_GIVE_MONEY, pUser->m_strUserID, pUser->m_dwDN );

	// ----------------[ Update Databace ] ----------------------------------//
	if(UpdateUserExchangeData(pUser) == FALSE)
	{
		m_iCurWeight = iMyWeight;
		pUser->m_iCurWeight = iYourWeight;

		m_dwDN = dwMyTempDN;
		pUser->m_dwDN = dwYourTempDN;
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// 惑措祈 诀单捞飘啊 角菩窍搁 狐廉唱柯促.
		{
			m_UserItem[i] = MyItem[i];
			pUser->m_UserItem[i] = YourItem[i];
		}
		SendExchangeFail(0x00, UNKNOWN_ERR);
		pUser->SendExchangeFail(0x00, UNKNOWN_ERR);
		goto go_result;
	}

	//-----------------[ Send Packet ]---------------------------------------//
	nSizeMy = arSlotMy.GetSize();

	TempBuf.Add(EXCHANGE_RESULT);
	TempBuf.Add((BYTE)0x01);
	TempBuf.Add(m_dwDN);
	TempBuf.Add((BYTE)nSizeMy);

	for(i = 0; i < nSizeMy; i++)
	{
		TempBuf.Add((BYTE)arSlotMy[i]);
		TempBuf.Add(arItemListMy[i]->sLevel);
		TempBuf.Add(arItemListMy[i]->sSid);
		TempBuf.Add(arItemListMy[i]->sDuration);
		TempBuf.Add(arItemListMy[i]->sBullNum);
		TempBuf.Add(arItemListMy[i]->sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)arItemListMy[i]->tMagic[j]);
		TempBuf.Add((BYTE)arItemListMy[i]->tIQ); 
	}

	Send(TempBuf, TempBuf.GetLength());

	nSizeYour = arSlotYour.GetSize();

	TempBuf2.Add(EXCHANGE_RESULT);
	TempBuf2.Add((BYTE)0x01);
	TempBuf2.Add(pUser->m_dwDN);
	TempBuf2.Add((BYTE)nSizeYour);

	for(i = 0; i < nSizeYour; i++)
	{
		TempBuf2.Add((BYTE)arSlotYour[i]);
		TempBuf2.Add(arItemListYour[i]->sLevel);
		TempBuf2.Add(arItemListYour[i]->sSid);
		TempBuf2.Add(arItemListYour[i]->sDuration);
		TempBuf2.Add(arItemListYour[i]->sBullNum);
		TempBuf2.Add(arItemListYour[i]->sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf2.Add((BYTE)arItemListYour[i]->tMagic[j]);
		TempBuf2.Add((BYTE)arItemListYour[i]->tIQ); 
	}

	pUser->Send(TempBuf2, TempBuf2.GetLength());

	// 狞酒捞袍篮 歹捞惑 辑滚俊辑 促风瘤 臼绰促. -------------------------------//
/*
	if(iQuickChange >= 1)
	{
		sCount = -1;
		TempBuf3.Add(ITEM_MOVE_RESULT);
		TempBuf3.Add((BYTE)0x01);
		TempBuf3.Add((BYTE)0x02);

		TempBuf3.Add((BYTE)iQuickChange);
		for(i = 0; i < QUICK_ITEM; i++)
		{
			if(MyTempQuickItem[i] != -1)
			{
				TempBuf3.Add((BYTE)i);
				TempBuf3.Add(sCount);
			}
		}
		Send(TempBuf3, TempBuf3.GetLength());
	}

	if(yQuickChange >= 1)
	{
		sCount = -1;
		TempBuf4.Add(ITEM_MOVE_RESULT);
		TempBuf4.Add((BYTE)0x01);
		TempBuf4.Add((BYTE)0x02);

		TempBuf4.Add((BYTE)yQuickChange);
		for(i = 0; i < QUICK_ITEM; i++)
		{
			if(YourTempQuickItem[i] != -1)
			{
				TempBuf4.Add((BYTE)i);
				TempBuf4.Add(sCount);
			}
		}
		pUser->Send(TempBuf4, TempBuf4.GetLength());
	}
*/
/*	//	捣 函拳啊 乐栏搁...^^
	if(pUser->m_TradeMoney > 0 || m_TradeMoney > 0)
	{
		SendMoneyChanged();
		pUser->SendMoneyChanged();
	}
*/
	GetRecoverySpeed();					//	公霸 函拳俊 蝶弗 雀汗加档甫 炼沥茄促.
	pUser->GetRecoverySpeed();

	SendSystemMsg( IDS_USER_EXCHANGE_COMPLETED, SYSTEM_NORMAL, TO_ME);
	if(bChangeMyEquip) SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	if(bChangeYourEquip) pUser->SendMyInfo(TO_INSIGHT, INFO_MODIFY);

	FlushItemLog( TRUE );

//	SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
//	pUser->SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
	//---------------[ Delete Memory ]------------------------------------//
go_result:
	for(i = 0; i < arItemListMy.GetSize(); i++)
	{
		if(arItemListMy[i] != NULL) delete arItemListMy[i];
	}
	arItemListMy.RemoveAll();

	for(i = 0; i < arItemListYour.GetSize(); i++)
	{
		if(arItemListYour[i] != NULL) delete arItemListYour[i];
	}
	arItemListYour.RemoveAll();

	EnterCriticalSection( &m_CS_ExchangeItem );
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)
	{
		if ( m_arExchangeItem[i] != NULL )
		{
			delete m_arExchangeItem[i];
			m_arExchangeItem[i] = NULL;
		}
	}
	m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ExchangeItem );

	EnterCriticalSection( &pUser->m_CS_ExchangeItem );
	for(i = 0; i < pUser->m_arExchangeItem.GetSize(); i++)
	{
		if(pUser->m_arExchangeItem[i] != NULL)
		{
			delete pUser->m_arExchangeItem[i];
			pUser->m_arExchangeItem[i] = NULL;
		}
	}
	pUser->m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &pUser->m_CS_ExchangeItem );

	//---------------[ 芭贰如利 檬扁拳 ]-------------------------------//
	m_TradeMoney = 0;
	pUser->m_TradeMoney = 0;
	m_bNowTrading = m_bExchangeOk = m_bTradeItem	= FALSE;
	pUser->m_bNowTrading = pUser->m_bExchangeOk = pUser->m_bTradeItem = FALSE;

	_tcscpy(m_strTradeUser, _T(""));		// 惑措规阑 犬牢窍扁 困茄 蜡历捞抚阑 瘤款促.
	_tcscpy(pUser->m_strTradeUser, _T(""));	// 惑措规阑 犬牢窍扁 困茄 蜡历捞抚阑 瘤款促.
	m_iTradeUid = -1;						// 惑措规 uid昏力
	pUser->m_iTradeUid = -1;				// 惑措规 uid昏力

	for(i = 0; i < TOTAL_ITEM_NUM; i++) // 芭贰吝 牢亥俊 官差绰 酒捞袍 荐甫 眉农困秦
	{ 
		m_TradeItemNum[i].sSid = -1; 
		m_TradeItemNum[i].sNum = 0; 
	}

	FlushItemLog( FALSE );
}

//////////////////////////////////////////////////////////////////////////////
//	粮 眉牢瘤 且锭 朝揪 函拳甫 舅赴促.
//
void USER::SendWeatherInMoveZone()
{
	int tType = 2;
	int iWeather = 1;

	// 捞悼窍妨绰 粮捞 厚啊棵荐 乐绰 瘤开捞搁	
	for(int i = 0; i < g_WeatherZone.GetSize(); i++)	
	{
		if(g_WeatherZone[i]->iZone == m_curz)
		{
			if(g_WeatherZone[i]->bRaining == TRUE) SetWeather(tType, iWeather); 
			return ;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//	秦寸 粮俊 乐绰 蜡历俊霸 朝揪 函拳甫 舅赴促.
//
//void USER::SetWeather(TCHAR *pBuf)
void USER::SetWeather(int tType, int tOnOff)
{
	CBufferEx TempBuf;
	TempBuf.Add(SET_CLIENT_WORLD);
	TempBuf.Add((BYTE)tType);

	if(tType == 2)	TempBuf.Add((BYTE)tOnOff);
	else			TempBuf.Add((BYTE)g_GameTime);

	Send(TempBuf, TempBuf.GetLength());
}

/*
//////////////////////////////////////////////////////////////////////////////
//	
//
void USER::SendWeatherToZone(BYTE tType, BYTE tOnOff)
{
	CBufferEx TempBuf;
	
	TempBuf.Add(SET_CLIENT_WORLD);

	TempBuf.Add(tType);
	TempBuf.Add(tOnOff);

	Send(TempBuf, TempBuf.GetLength());
}
*/
//##############################################################################
//	Character Event
//##############################################################################

////////////////////////////////////////////////////////////////////////////
//	Client NPC 俊辑 捞亥飘 荐青阑 夸没茄促.
//
void USER::NpcEvent(TCHAR *pBuf)
{
	int event_zone_index = -1;
	int i, j;
	CNpc*	pNpc = NULL;
	EVENT*	pEvent = NULL;

	int index = 0;
	int nUid = GetInt(pBuf, index);

	if(nUid < 0 || nUid >= INVALID_BAND) return;	// 肋给等 uid搁 府畔
	
	if(nUid >= USER_BAND && nUid < NPC_BAND)		// 蜡历俊 措茄 捞亥飘
	{
		return;
	}
	
	if(nUid >= NPC_BAND && nUid < INVALID_BAND)		// NPC 俊 措茄 捞亥飘
	{
		pNpc = GetNpc(nUid - NPC_BAND);
		if(!pNpc ) return;

		if(pNpc->m_sEvent <= 0 ) return;

		for(i = 0; i < g_event.GetSize(); i++)
		{
			if(g_event[i]->m_Zone == m_curz)
			{				
				event_zone_index = i;
				break;
			}
		}

		if(event_zone_index == -1) return;			// 泅犁 Zone 俊绰 define 等 捞亥飘啊 绝促.

		pEvent = g_event[event_zone_index];

		if(pEvent == NULL) return;
		if(pNpc->m_sEvent >= pEvent->m_arEvent.GetSize()) return;
		if(pEvent->m_arEvent[pNpc->m_sEvent] == NULL) return;

		if(!CheckEventLogic(pEvent->m_arEvent[pNpc->m_sEvent])) return;	// 炼扒八荤

		for(j = 0; j < pEvent->m_arEvent[pNpc->m_sEvent]->m_arExec.GetSize(); j++)
		{
			if(RunNpcEvent(pNpc, pEvent->m_arEvent[pNpc->m_sEvent]->m_arExec[j]) == NULL) break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	Client HyperText俊辑 捞亥飘 荐青阑 夸没茄促.
//
void USER::HyperTextEvent(TCHAR *pBuf)
{
	int event_zone_index = -1;
	int index = 0, i, j;
	EVENT*	pEvent = NULL;

	short sZone = GetShort(pBuf, index);
	short sEventNum = GetShort(pBuf, index);
	TRACE( "牛牛DEGBUG NPC 功能事件号 %d\n", sEventNum);
	if(sZone < 0 || sEventNum < 0) return;

	for(i = 0; i < g_event.GetSize(); i++)
	{
		if(g_event[i]->m_Zone == sZone)
		{			
			if(sZone != m_curz)
			{
				return;
//				if(sZone == 2 && (sEventNum >= 600 && sEventNum <= 700)) { event_zone_index = i; break; }
//				else return;
			}
			event_zone_index = i;
			break;
		}
	}

	if(event_zone_index == -1) return;			// 泅犁 Zone 俊绰 define 等 捞亥飘啊 绝促.
	
	pEvent = g_event[event_zone_index];

	if(pEvent == NULL) return;
	if(sEventNum >= pEvent->m_arEvent.GetSize()) return;
	if(pEvent->m_arEvent[sEventNum] == NULL) return;

	if(!CheckEventLogic(pEvent->m_arEvent[sEventNum])) return;	
	for(j = 0; j < pEvent->m_arEvent[sEventNum]->m_arExec.GetSize(); j++)
	{
		if(RunNpcEvent(NULL, pEvent->m_arEvent[sEventNum]->m_arExec[j]) == NULL) break;
	}
}

////////////////////////////////////////////////////////////////////////////
//	捞亥飘 涅胶飘肺 各捞 淬寸窍绊 乐绰 捞亥飘甫 角青茄促.
//
void USER::RunQuestEvent(CNpc *pNpc, int iZone, int iEventNum)
{
	if(!pNpc) return;

	int event_zone_index = -1;
	int index = 0, i, j;
	EVENT*	pEvent = NULL;

	if(iZone < 0) return;		// 捞亥飘 锅龋绰 1何磐 10000锅鳖瘤 
	if(iEventNum <= 0 || iEventNum > NPC_QUEST_MOP) return;

	for(i = 0; i < g_event.GetSize(); i++)
	{
		if(g_event[i]->m_Zone == iZone)
		{			
			if(iZone != m_curz)	return;

			event_zone_index = i;
			break;
		}
	}

	if(event_zone_index == -1) return;			// 泅犁 Zone 俊绰 define 等 捞亥飘啊 绝促.
	
	pEvent = g_event[event_zone_index];

	if(pEvent == NULL) return;
	if(iEventNum >= pEvent->m_arEvent.GetSize()) return;
	if(pEvent->m_arEvent[iEventNum] == NULL) return;

	if(!CheckEventLogic(pEvent->m_arEvent[iEventNum])) return;	// 炼扒八荤

	for(j = 0; j < pEvent->m_arEvent[iEventNum]->m_arExec.GetSize(); j++)
	{
		if(RunNpcEvent(pNpc, pEvent->m_arEvent[iEventNum]->m_arExec[j]) == NULL) break;
	}
}

////////////////////////////////////////////////////////////////////////////
//	Client 俊辑 捞亥飘 荐青阑 夸没茄促.
//
void USER::ClientEvent(TCHAR *pBuf)
{
	int index = 0;

	BYTE tType = GetByte(pBuf, index);

	if(tType == 1) NpcEvent(pBuf + index);

	if(tType == 2) HyperTextEvent(pBuf + index);

}

/////////////////////////////////////////////////////////////////////////
//	捞亥飘俊 炼扒捞 乐绰瘤 八荤茄促.
//
BOOL USER::CheckEventLogic(EVENT_DATA *pEventData)
{
	if(pEventData == NULL) return FALSE;

	BOOL bExact = FALSE;

	for( int i = 0; i < pEventData->m_arLogicElse.GetSize(); i++)
	{
		bExact = FALSE;

		LOGIC_ELSE* pLE = pEventData->m_arLogicElse[i];		if( !pLE ) return FALSE;

		switch( pLE->m_LogicElse )
		{
		case LOGIC_LEVEL:
			if( CheckLevel(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1]) ) bExact = TRUE; 
			break;
	    case LOGIC_VIP://NPC检测会员
			if( CheckVip(pLE->m_LogicElseInt[0] ) ) bExact = TRUE; 
			break;
		case LOGIC_VIP2://NPC检测会员2
			if( CheckVip2(pLE->m_LogicElseInt[0] ) ) bExact = TRUE; 
			break;
		case LOGIC_LING://
			if( CheckLing(pLE->m_LogicElseInt[0] ) ) bExact = TRUE; 
			break;
		case LOGIC_EXIST_EVENT:
			if( FindEvent( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_ABSENT_EVENT:
			if( !FindEvent( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_CLASS:
			if( CheckClass( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_EXIST_ITEM:
			if( FindItem( pLE->m_LogicElseInt[0]) >= pLE->m_LogicElseInt[1] ) bExact = TRUE;
			break;

		case LOGIC_EXIST_EVENT_ITEM:
			if( FindEventItem( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1] ) ) bExact = TRUE;
			break;
		case LOGIC_EXIST_SKILL:
			if( FindSkill( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_ABSENT_ITEM:
			if( !FindItem( pLE->m_LogicElseInt[0]) ) bExact = TRUE;
			break;

		case LOGIC_WEIGHT:
			if( CheckItemWeight( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2],
													pLE->m_LogicElseInt[3], pLE->m_LogicElseInt[4],
													pLE->m_LogicElseInt[5], pLE->m_LogicElseInt[6],
													pLE->m_LogicElseInt[7], pLE->m_LogicElseInt[8],
													pLE->m_LogicElseInt[9], pLE->m_LogicElseInt[10]	)) bExact = TRUE;
			break;

		case LOGIC_CHECK_ITEMSLOT:
			if( CheckItemSlot( pLE->m_LogicElseInt[0], pLE) ) bExact = TRUE;
			break;

		case LOGIC_EXIST_GM:
			if( CheckGuildMaster() ) bExact = TRUE;
			break;

		case LOGIC_OVER_GUILD_USER_COUNT:
			if( CheckOverGuildUserCount( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_ZONE_WHO:
			if( CheckZoneWho(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2]) ) bExact = TRUE;
			break;

		case LOGIC_KILL_GH:
			break;

		case LOGIC_MASS_ZONE_MOVE:
			break;

		case LOGIC_TERM:
			if( CheckTerm( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2]) ) bExact = TRUE; 
			break;
/*		case LOGIC_DN:		// 炼扒 DN焊促 腹阑版快
			if(m_dwDN >= (DWORD)pLE->m_LogicElseInt[0] ) bExact = TRUE;
			break;
*/
		case LOGIC_MONEY:
			if( CheckMoneyMinMax( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1] ) ) bExact = TRUE;
			break;

		case LOGIC_CHECK_CV:
			if( CheckUserCityValue( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1]) ) bExact = TRUE;
			break;

		case LOGIC_GUILD_HOUSE_NUMBER:
			if(CheckGuildHouseUser(pLE->m_LogicElseInt[0])) bExact = TRUE;
			break;

		case LOGIC_RAND:
			if( CheckRandom( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_RR_TIME:
			if( CheckRoyalRumbleEnterTime() ) bExact = TRUE;
			break;

		case LOGIC_RR_MAX_USER:
			if( CheckRoyalRumbleEnterMaxUser() ) bExact = TRUE;
			break;

		default:
			return FALSE;
		}

		if( !pLE->m_bAnd )		// OR 炼扒老 版快 bExact啊 TRUE捞搁 傈眉啊 TRUE捞促
		{
			if( bExact )
			{
				return TRUE;
			}
		}
		else					// AND 炼扒老 版快 bExact啊 FALSE捞搁 傈眉啊 FALSE捞促
		{
			if( !bExact )
			{
				return FALSE;
			}
		}
	}

	return TRUE;		// 酒公繁 炼扒捞 绝澜搁 TRUE 府畔
}

////////////////////////////////////////////////////////////////////////////
//	秦寸 蜡历啊 力茄 矫刮 殿鞭牢瘤 魄窜
//
BOOL USER::CheckUserCityValue(int min, int max)
{
	if(m_sCityRank >= min && m_sCityRank < max ) return TRUE;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//	秦寸 蜡历啊 力茄 饭骇俊 登绰瘤
//
BOOL USER::CheckLevel(int min, int max)   
{
	if(m_sLevel >= min && m_sLevel < max ) return TRUE;

//	SendSystemMsg( IDS_USER_NOT_THOS_LEVEL, SYSTEM_ERROR, TO_ME);

	return FALSE;
}
void USER::RobLINGE(int dn)        //修炼
{
	CString sysMsg;
	if(m_dwAutoMoney <= 0 &&dn == 1) 
		{
           if( m_dwXP<100)   //战斗经验
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
         if(m_dwDN < 10000000){
          SendSystemMsg("修炼需要1000万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 10000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 10000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();

			m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第一阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;

			
		}
	else if(m_dwAutoMoney == 1 &&dn == 2) 
		{
          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 15000000){
          SendSystemMsg("修炼需要1500万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 15000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 15000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第二阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else if(m_dwAutoMoney == 2 &&dn == 3) 

		{

          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 20000000){
          SendSystemMsg("修炼需要2000万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 20000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 20000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第三阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else if(m_dwAutoMoney == 3 &&dn == 4) 
		{
			
          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 25000000){
          SendSystemMsg("修炼需要2500万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 25000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 25000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第四阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else if(m_dwAutoMoney == 4 &&dn == 5) 
		{
			
          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 30000000){
          SendSystemMsg("修炼需要3000万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 30000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 30000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第五阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else if(m_dwAutoMoney == 5 &&dn == 6) 
		{
			
          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 35000000){
          SendSystemMsg("修炼需要3500万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 35000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 35000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第六阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else if(m_dwAutoMoney == 6 &&dn == 7) 
		{
			
          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 40000000){
          SendSystemMsg("修炼需要4000万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 40000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 40000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第七阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else if(m_dwAutoMoney == 7 &&dn == 8) 
		{
			
          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 45000000){
          SendSystemMsg("修炼需要4500万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 45000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 45000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第八阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else if(m_dwAutoMoney == 8 &&dn == 9) 
		{
			
          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 50000000){
          SendSystemMsg("修炼需要5000万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 50000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 50000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第九阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else if(m_dwAutoMoney == 9 &&dn == 10) 
		{
			
          if( m_dwXP<100)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}
		    if(m_dwDN < 55000000){
          SendSystemMsg("修炼需要5500万,你的金钱不足",SYSTEM_ERROR,TO_ME);
          return;
          }
          if( m_dwDN <= 55000000 ) m_dwDN = 0;
          else m_dwDN = m_dwDN - 55000000;
          UpdateUserItemDN(); 
          SendMoneyChanged();
           m_dwAutoMoney = dn;
         sysMsg.Format("恭喜玩家 %s 进行了第十阶段的修炼攻击力增加10. ",this->m_strUserID);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				return;
		}
	else
		{
			 SendSystemMsg( "请按每个阶段修炼，不能跳阶段修炼.",  SYSTEM_ERROR, TO_ME);
				return;
	    }

}
BOOL USER::CheckVip(int min)//NPC检测会员
{
	if((int)m_dwGuarDianTianShi >= min) return TRUE;

	
	return FALSE;
}
BOOL USER::CheckVip2(int min)//NPC检测会员2
{
	if((int)m_dwGuarDianTianShi == min) return TRUE;
        return FALSE;
}
BOOL USER::CheckLing(int min)
{
	if((int)m_dwLingQu >= min ) return TRUE;

//	SendSystemMsg( IDS_USER_NOT_THOS_LEVEL, SYSTEM_ERROR, TO_ME);

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////
//	秦寸 蜡历啊 辨付牢瘤 眉农
//
BOOL USER::CheckGuildMaster()
{
	if( m_bGuildMaster ) return TRUE;

	SendSystemMsg( IDS_USER_ONLY_GUILD_MASTER_USE, SYSTEM_ERROR, TO_ME);

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//	老沥 朝楼俊辑 角青登绰瘤 眉农
//
BOOL USER::CheckTerm(int start, int end, int say)
{
	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);

	if( guildTime.wDay >= start	&& guildTime.wDay <= end ) return TRUE;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//	弥措 20疙阑 捞悼 矫挪促.
//
BOOL USER::MassZoneMove(int cityNum)
{
/*	InterlockedExchange(&g_arGuildHouseWar[cityNum]->m_CurrentGuild.lUsed, 0);

	g_arGuildHouseWar[cityNum]->m_CurrentGuild.arUserList.RemoveAll();
	g_arGuildHouseWar[cityNum]->m_CurrentGuild.lGuild = 0;
	g_arGuildHouseWar[cityNum]->m_CurrentGuild.iCurValue = 0;
	g_arGuildHouseWar[cityNum]->m_CurrentGuild.dwTimer = 0;

*/
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//	蜡历啊 家蜡茄 Event吝 秦寸 Event啊 乐绰瘤 茫绰促.
//
BOOL USER::FindEvent(int event_num)
{
	//不对机械身体任务与魔法球任务作判断,202法大圈任务,116 5级任务
/*	if(event_num == 16 || event_num == 6 ||event_num == 202||event_num == 3)
		return TRUE;*/
	for(int i = 0; i < m_arEventNum.GetSize(); i++)
	{
		TRACE("任务号:%d \n", *(m_arEventNum[i]));
		if( *(m_arEventNum[i]) == event_num )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL USER::FindSkill(int Skill_num)
{
	int i;
	int iIndex=m_byClass *SKILL_NUM;
	
	if(m_sSkillPoint != 0)  //如果还有技能点没有加的话..也不行的....
		return false;
	for(i = iIndex; i < iIndex + SKILL_NUM; i++)
	{
		if(m_UserSkill[i].sSid==Skill_num)
			return true;
	}
	return false;
}
void USER::AddSkill (int Skill_num)
{
	m_SKill=Skill_num;
}
// 120技能转换
void USER::RunSkill120(int Skill)
{
	int i,count=0,sLevel=m_sLevel;
	int iIndex=m_byClass *SKILL_NUM;
	//仲裁职业的起点
	if(m_byClass == JUDGE)
	{
		iIndex = SKILL_JUDGE_QIANGHUAHUFA;
	}
	if(Skill!=20&&Skill!=21&&Skill!=22&&Skill!=23&&Skill!=24)
		return;
	for(i = iIndex; i < iIndex + SKILL_NUM; i++)
	{
		if(m_UserSkill[i].sSid>=20&&m_UserSkill[i].sSid<=24)
			count++;
	}
	if(count>=2)
		return ;

	for(i = iIndex; i < iIndex + SKILL_NUM; i++)
	{
		if(m_UserSkill[i].sSid==m_SKill)
			break;
	}
	if(i >= iIndex + SKILL_NUM)
		return;

	if(sLevel>130)
		sLevel=129;
	m_UserSkill[i].sSid=Skill;
	m_UserSkill[i].tLevel=m_UserSkill[i].tLevel/2;
	if(sLevel>=120)
		m_UserSkill[i].tLevel=m_UserSkill[i].tLevel+(sLevel-119);
	SendUserStatusSkill();
	return;

}
////////////////////////////////////////////////////////////////////////////
//	蜡历啊 家蜡茄 Item吝 秦寸 Item捞 乐绰瘤 茫绰促.
//
int USER::FindItem(int sid)
{
	if(sid < 0 || sid >= g_arItemTable.GetSize()) return FALSE;

	int i, j, iNum = -1;
	ItemList TempItem;

	ReSetItemSlot(&TempItem);

	TempItem.sSid		= g_arItemTable[sid]->m_sSid;
	TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
	TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

	for(i =0; i < MAGIC_NUM; i++) TempItem.tMagic[i] = 0;
	TempItem.tIQ = NORMAL_ITEM;
	if(sid==755){
		TempItem.tIQ=2;
		TempItem.tMagic[0]=137;
	}
	
//	iNum = GetSameItem(TempItem, INVENTORY_SLOT);
	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == TempItem.sSid)
		{
			if( m_UserItem[i].sSid != 796 )		// 捞亥飘 汗鼻老 版快 概流 锅龋唱 可记阑 眉农窍瘤 臼绰促.
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
				}
					
				if(m_UserItem[i].tIQ == TempItem.tIQ) { iNum = i; break; } 
			}
			else
			{
				iNum = i; break;
			}
		}
	}

	if(iNum != -1)	return m_UserItem[iNum].sCount;
	
	return 0;
}

BOOL USER::FindEventItem(int sid, int quality)
{
	if(sid < 0 || sid >= g_arItemTable.GetSize()) return FALSE;

	int i, j, iNum = -1;
	ItemList TempItem;

	ReSetItemSlot(&TempItem);

	TempItem.sSid		= g_arItemTable[sid]->m_sSid;
	TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
	TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

	for(i =0; i < MAGIC_NUM; i++) TempItem.tMagic[i] = 0;
	TempItem.tIQ = (BYTE)quality;
	
//	iNum = GetSameItem(TempItem, INVENTORY_SLOT);
	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == TempItem.sSid)
		{
			for(j = 0; j < MAGIC_NUM; j++)
			{
				if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
			}
				
			if(m_UserItem[i].tIQ == TempItem.tIQ) { iNum = i; break; } 
		}
	}

	if(iNum != -1)	return TRUE;
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
//	捞亥飘俊辑 酒捞袍阑 临锭 公霸 眉农
//
BOOL USER::CheckItemWeight(int say, int iSid1, int iNum1, int iSid2, int iNum2, int iSid3, int iNum3, int iSid4, int iNum4, int iSid5, int iNum5)
{
	int iWeight = 0;
//	int iMax = g_arItemTable.GetSize();

	if( iSid1 >= 0 && iSid1 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid1]->m_byWeight * iNum1;
	if( iSid2 >= 0 && iSid2 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid2]->m_byWeight * iNum2;
	if( iSid3 >= 0 && iSid3 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid3]->m_byWeight * iNum3;
	if( iSid4 >= 0 && iSid4 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid4]->m_byWeight * iNum4;
	if( iSid5 >= 0 && iSid5 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid5]->m_byWeight * iNum5;

	if(m_iMaxWeight < m_iCurWeight + iWeight) 
	{
//		SendNpcSay(NULL, say);
		SendSystemMsg( IDS_USER_OVER_WEIGHT_RECEIVE_ITEM, SYSTEM_NORMAL, TO_ME);
		return FALSE;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
//
BOOL USER::CheckItemSlot(int say, LOGIC_ELSE* pLE)
{
	int sid = 0;
	int iNum = -1;
	ItemList TempItem;

	for(int i = 1; i < 10; i += 2)
	{
		sid = pLE->m_LogicElseInt[i];

		if( sid < 0 || sid >= g_arItemTable.GetSize() ) continue;

		ReSetItemSlot(&TempItem);

		TempItem.sSid		= g_arItemTable[sid]->m_sSid;
		TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
		TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

		for(int j =0; j < MAGIC_NUM; j++) TempItem.tMagic[j] = 0;
		TempItem.tIQ = NORMAL_ITEM;
	
		iNum = GetSameItem(TempItem, INVENTORY_SLOT);
		if(iNum == -1)							// 鞍篮 酒捞袍捞 绝绊
		{
			iNum = GetEmptySlot(INVENTORY_SLOT);
			if(iNum == -1)						// 后 浇吩档 绝栏搁..
			{
//				SendNpcSay(NULL, say);
				SendSystemMsg( IDS_USER_INVEN_FULL_RECEIVE_ITEM, SYSTEM_NORMAL, TO_ME);
				return FALSE;
			}
		}
	}

	return TRUE;	
}

//////////////////////////////////////////////////////////////////////
//
//
BOOL USER::CheckClass(int iClass)
{
	if(m_byClass == iClass) return TRUE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
//	捞亥飘俊辑 秦寸 酒捞袍阑 化绰促.
//
void USER::RobItem(int sid, int num)
{
	if(sid < 0 || sid >= g_arItemTable.GetSize()) return;

	int i, j;
	int iSlot = -1;
	CBufferEx TempBuf;
//	BOOL bQuickChange = FALSE;
	BYTE result = SUCCESS;

	ItemList	TempItem;
	ReSetItemSlot(&TempItem);

	TempItem.sSid		= g_arItemTable[sid]->m_sSid;
	TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
	TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

	for(i =0; i < MAGIC_NUM; i++) TempItem.tMagic[i] = 0;
	TempItem.tIQ = NORMAL_ITEM;
	if(sid==755){
		TempItem.tIQ=2;
		TempItem.tMagic[0]=137;
	}

	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == TempItem.sSid)
		{
			if( m_UserItem[i].sSid != 796 )
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
				}
					
				if(m_UserItem[i].tIQ == TempItem.tIQ) { iSlot = i; break; } 
			}
			else
			{
				iSlot = i; break;
			}
		}
	}

	if(iSlot == -1) return;

	if(m_UserItem[iSlot].sSid < 0) return;
	if(m_UserItem[iSlot].sCount < num) return;

	if((m_UserItem[iSlot].sCount - num) <= 0)				// 促澜 郴 牢亥甫 哗 霖促.		
	{														// 狞酒捞袍 函拳啊 乐栏搁		
//		if(g_arItemTable[iSlot]->m_byWear >= 6) bQuickChange = TRUE;

		ReSetItemSlot(&m_UserItem[iSlot]);
	}
	else m_UserItem[iSlot].sCount -= num;

	int iWeight = num * g_arItemTable[iSlot]->m_byWeight;

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();			

	TempBuf.Add(ITEM_GIVE_RESULT);
	TempBuf.Add(result);
	TempBuf.Add((BYTE)iSlot);
	TempBuf.Add(m_UserItem[iSlot].sLevel);
	TempBuf.Add(m_UserItem[iSlot].sSid);
	TempBuf.Add(m_UserItem[iSlot].sDuration);
	TempBuf.Add(m_UserItem[iSlot].sBullNum);
	TempBuf.Add(m_UserItem[iSlot].sCount);
	for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[iSlot].tMagic[j]);

	TempBuf.Add(m_UserItem[iSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
//	if(bQuickChange) SendQuickChange();				// 狞酒捞袍 函悼捞乐栏搁...
}


//	巩拳 惑前鼻, 归拳痢 惑前鼻阑 窍唱究 哗林扁 困秦 父电 窃荐捞促.
//	泅犁 胶农赋飘肺 阂府绰 巴捞 酒聪扼 RecvAddress俊辑父 何弗促.
//	胶农赋飘肺 何福哥, 裹侩栏肺 荤侩窍绊 酵阑锭俊绰 荐沥捞 鞘夸且铂?
BOOL USER::EventRobItem(int item_index, int quality)
{
	int sid = item_index;
	int num = 1;
	if(sid < 0 || sid >= g_arItemTable.GetSize()) return FALSE;

	int i, j;
	int iSlot = -1;

	CBufferEx TempBuf;
	BYTE result = SUCCESS;

	ItemList	TempItem;
	ReSetItemSlot(&TempItem);

	TempItem.sSid		= g_arItemTable[sid]->m_sSid;
	TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
	TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

	for(i =0; i < MAGIC_NUM; i++) TempItem.tMagic[i] = 0;
	TempItem.tIQ = quality;
	
	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == TempItem.sSid)
		{
			for(j = 0; j < MAGIC_NUM; j++)
			{
				if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
			}
				
			if(m_UserItem[i].tIQ == TempItem.tIQ) { iSlot = i; break; } 
		}
	}

	if(iSlot == -1) return FALSE;

	if(m_UserItem[iSlot].sSid < 0) return FALSE;
	if(m_UserItem[iSlot].sCount < num) return FALSE;

	if((m_UserItem[iSlot].sCount - num) <= 0)				// 促澜 郴 牢亥甫 哗 霖促.		
	{														// 狞酒捞袍 函拳啊 乐栏搁		
//		if(g_arItemTable[iSlot]->m_byWear >= 6) bQuickChange = TRUE;

		ReSetItemSlot(&m_UserItem[iSlot]);
	}
	else m_UserItem[iSlot].sCount -= num;

	int iWeight = num * g_arItemTable[iSlot]->m_byWeight;

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();			// 促矫 雀汗加档甫 拌魂

	TempBuf.Add(ITEM_GIVE_RESULT);
	TempBuf.Add(result);
	TempBuf.Add((BYTE)iSlot);
	TempBuf.Add(m_UserItem[iSlot].sLevel);
	TempBuf.Add(m_UserItem[iSlot].sSid);
	TempBuf.Add(m_UserItem[iSlot].sDuration);
	TempBuf.Add(m_UserItem[iSlot].sBullNum);
	TempBuf.Add(m_UserItem[iSlot].sCount);
	for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[iSlot].tMagic[j]);

	TempBuf.Add(m_UserItem[iSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
//	if(bQuickChange) SendQuickChange();				// 狞酒捞袍 函悼捞乐栏搁...

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
//	捞亥飘俊辑 XP甫 棵妨霖促.
//
void USER::AddXPUp(int iNum)
{
	if(iNum <= 0) return;

	CheckMaxValue(m_dwXP, iNum);		// 各捞 磷阑锭父 1 刘啊!	
	SendXP();
}

////////////////////////////////////////////////////////////////////////////
//	NPC Event 狼 EXEC 疙飞阑 荐青茄促.
//
BOOL USER::RunNpcEvent(CNpc *pNpc, EXEC *pExec)
{
	int nDist = 0;
	int result = 0;
	short sID = 0;
	CBufferEx test;
	test.Add(sID);
	
	if(pNpc)
	{
		if(pNpc->m_sCurZ != m_curz) return TRUE;
	}

	switch( pExec->m_Exec )
	{
	case EXEC_RETURN:
		return FALSE;

	case EXEC_BBS_OPEN:
		BBSOpen( pExec->m_ExecInt[0] );
		break;

	case EXEC_ZONE_MOVE:			
		if(pNpc) nDist = abs(m_curx - pNpc->m_sCurX) + abs(m_cury - pNpc->m_sCurY);
		if(nDist > SCREEN_X + SCREEN_Y) return TRUE;
		ZoneMoveReq( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2] );
		break;

	case EXEC_RR_ENTER:
		if( !g_RR.Enter( this ) ) return TRUE;
		ZoneMoveReq( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2] );
		break;

	case EXEC_FT_MOVE:
		FortressMove(pNpc,  pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2], pExec->m_ExecInt[3], pExec->m_ExecInt[4] );
		break;

	case EXEC_GUARD_BOW:			// 版厚捍 牢荤窍扁
		GuardBow(pNpc);
		break;
    
	case EXEC_NPC_SAYING:
		NpcSaying(pNpc);
		break;

    case EXEC_NPC_DA:
		NpcDuBoDA(pExec->m_ExecInt[0],pExec->m_ExecInt[1]);		//赌博大
		break;
     
	case EXEC_NPC_XIAO:
		NpcDuBoXIAO(pExec->m_ExecInt[0],pExec->m_ExecInt[1]);		//赌博小
		break;

    case EXEC_JUANKUAN:
		SetServerDoubleExp3();
		break;
   
	case EXEC_BAOLV:
		SetServerDoubleBaoLv();
		break;

	case EXCE_GOPK://进入PK大赛
		CheckPKTime();
		break;
    
	case EXCE_GOSHAREN:
		CheckPKShaRen();
		break;

    case EXEC_OFF_JJ:	//卸下机甲
		offJJ();
		break;

	case EXEC_MOVE:					// 鞍篮粮救狼 漂喊 厘家肺 捞悼矫挪促.(唱吝俊 鞘夸茄瘤 绊刮!)
		EventMove( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2] );
		break;

	case EXEC_ROB_DN:
		RobDN( pExec->m_ExecInt[0] );
/*		if(m_dwDN < (DWORD)pExec->m_ExecInt[0])
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_DINA, SYSTEM_NORMAL, TO_ME);
			return FALSE;
		}
		m_dwDN -= pExec->m_ExecInt[0];
		SendMoneyChanged();	*/
		break;
    case EXEC_ROB_LING:
		RobLING( pExec->m_ExecInt[0] );
		break;

	case EXEC_SELL_OPEN:			// 酒捞袍 迫扁
		SellItemOpenReq( pExec->m_ExecInt[0] );
		break;

	case EXEC_BANK_OPEN:
	    	BankOpenReq();
		break;

	case EXEC_ACCOUNT_BANK_OPEN:
	    	AccountBankOpenReq( pExec->m_ExecInt[0] );
		break;

	case EXEC_GUILD_WAREHOUSE_OPEN:
		GuildWharehouseOpenReq();
//		GuildWharehouseOpenReqWithThread();

		break;

	case EXEC_REPAIR_OPEN:			// 酒捞袍 荐府窍扁
		RepairItemOpenReq( pExec->m_ExecInt[0] );
		break;

	case EXEC_STORE_OPEN:			// 酒捞袍 荤扁
		SendStoreOpen( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_FIELD_STORE_OPEN:		// 鞘靛俊辑 辨靛客 惑痢捞 父车阑锭...
		SendFieldStoreOpen( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2] );
		break;

	case EXEC_GUILD_SAY:
		SendFieldGuildSay( pNpc, pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_GUILD_TAX:
		SendStoreTax( pExec->m_ExecInt[0] );
		break;

	case EXEC_GUILD_SCHEME:
		SendGuildWarScheme( pExec->m_ExecInt[0] );
		break;

	case EXEC_GUILD_APPLY:
		GuildWarApply( pExec->m_ExecInt[0] );
		break;

	case EXEC_PSI_STORE_OPEN:
		SendPsiStoreOpen( pExec->m_ExecInt[0] );
		break;

	case EXEC_SAY:
		SendNpcSay( pNpc, pExec->m_ExecInt[0] );
		break;
    case EXEC_PKSAY:
		SendNpcPKSay(pNpc);
		break;
	case EXEC_SAYSHAREN:
		SendNpcShaRenSay(pNpc);
		break;

    case EXEC_KILL_BABY:	//杀宠物 
		{
		UserKillBaby(pExec->m_ExecInt[0],pExec->m_ExecInt[1],pExec->m_ExecInt[2]);
		break;
		}

	case EXEC_ALL_CURE:				// 葛电 hp, sp, pp甫 菜盲款促.
		SendAllCure( pExec->m_ExecInt[0],  pExec->m_ExecInt[1] );
		break;

/*	case EXEC_CARE:					// 葛电 惑怕捞惑甫 摹丰茄促.
		SendSystemMsg(IDS_USER_UNDER_CONSTRUCTION, SYSTEM_NORMAL, TO_ME);
		break;
*/
	case EXEC_SC_CHENGXU_KA://删除机甲程序卡
		EbodyFjCXcard( pExec->m_ExecInt[0] );
		break;

	case EXEC_GUILD_OPEN:
		GuildOpen();
		break;

	case EXEC_EBODY_OPEN:
//		SendEBodyOpen( pExec->m_ExecInt[0] );
		break;

	case EXEC_EBODY_DEFINE:
		EBodyIdentifyOpen( pExec->m_ExecInt[0] );
		break;
     case EXEC_ROB_LINGE://修炼
		RobLINGE( pExec->m_ExecInt[0] );
		break;

	case EXEC_SANPAI:
		Add4Pai( pExec->m_ExecInt[0] );//三排变四排
		break;

    case EXEC_ZUDUI:
		KuaiSuZuDui();//三排变四排
		break;

	case EXEC_UPGRADE_OPEN:
		UpgradeItemOpen(pExec->m_ExecInt[1]);
		break;

	case EXEC_ADD_EVENT:
		AddMyEventNum( pExec->m_ExecInt[0] );
		break;
	case EXEC_ADD_SKILL://120技能转换时..用的数据
		AddSkill(pExec->m_ExecInt[0]);
		break;

	case EXEC_DEL_EVENT:
		DelMyEventNum( pExec->m_ExecInt[0] );
		break;

	case EXEC_ROB_ITEM:
		RobItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;
    
	case EXEC_GO_EMO:
		CheckDevilTime();
		break;

	case EXEC_EVENT_ROB_ITEM:
//		EventRobItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_XP_UP:
		AddXPUp( pExec->m_ExecInt[0] );
		break;

	case EXEC_CHECK_GUILD_USER:
		SendGuildUserInfo( pExec->m_ExecInt[0] );
		break;

	case EXEC_CHECK_GUILD_RANK:
		SendGuildHouseRank( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_FT_REPAIR:
		SendRepairItem( pExec->m_ExecInt[0] );
		break;

	case EXEC_FT_PEOPLE:
		SendPeopleSay( pExec->m_ExecInt[0] );
		break;

	case EXEC_GIVE_ITEM:
		GiveItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;
   
    case EXEC_GIVE_MAGIC_ITEM:
		GiveMagicItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_GIVE_EVENT_ITEM:
		GiveEventItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2], pExec->m_ExecInt[3] );
		break;

	case EXEC_ADDRESS_WINDOW:
		AddressWindowOpen(pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
		break;
	case EXEC_INPUT_WINDOW:				
		EventItemSerialWindowOpen();
		break;

	case EXEC_INPUTGAME_WINDOW:
		EventItemSerialForGameRoomWindowOpen();
		break;

/*	case EXEC_ADD_GAMETIME_ITEM:
		GiveEventGameTime(pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
		break;
*/
	case EXEC_SC_FUSHU_PIN://删除机甲附属品
		EbodyFj39( pExec->m_ExecInt[0] );
		break;

	case EXEC_DRESSING:
		DressingWindowOpen();
		break;

	case EXEC_GIVE_DN://给决战币
		GiveDN( pExec->m_ExecInt[0] );
		break;

	case EXEC_GIVE_YB://给元宝
		GiveShopPingDN( pExec->m_ExecInt[0] );
		break;
    
	case EXEC_GIVE_HUFA://给护法
		{
		     CallHuFa( pExec->m_ExecInt[0] );
		     break;
		}

	case EXEC_SEND_MSG:
		SendEventMsg( pExec->m_ExecChar );
		break;

	case EXEC_CHANGE_UPGRADE_ACC:
		ChangeUpgradeAcc();
		break;

	case EXEC_APPLY_GUILD_RUN:
		ApplyGuildRun();
		break;

	case EXEC_MON_SAY:
		SendMonsterSay( pNpc, pExec->m_ExecInt[0], pExec->m_ExecChar );
		break;

	case EXEC_SUMMON:
		if(!ExcuteSingleQuestEvent( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2], pExec->m_ExecInt[3], 
								pExec->m_ExecInt[4], pExec->m_ExecInt[5], pExec->m_ExecInt[6])) return FALSE;
		break;

	case EXEC_RUN_EVENT:
		{
			EVENT* pEvent = GetEventInCurrentZone();	if( !pEvent ) break;

			if( !pEvent->m_arEvent[pExec->m_ExecInt[0]] ) break;

			if( !CheckEventLogic( pEvent->m_arEvent[pExec->m_ExecInt[0]] ) ) break;

			for( int i = 0; i < pEvent->m_arEvent[pExec->m_ExecInt[0]]->m_arExec.GetSize(); i++)
			{
				if( !RunNpcEvent( pNpc, pEvent->m_arEvent[pExec->m_ExecInt[0]]->m_arExec[i] ) )
				{
					return FALSE;
				}
			}
		}
		break;
	case EXEC_HELPERCURE:
		SendHelperCure( );
		break;
	case EXEC_GIVE_PSI:
		if(!GivePSI( pExec->m_ExecInt[0], pExec->m_ExecInt[1])) return FALSE;
		break;
	case EXEC_NPC_CHANGE_SKIN:  //脚本还原变身
		if(!ChangeSkin()) return FALSE;
		break;
	case EXEC_NPC_CHANGE_BH:  //脚本还原变身
		if(!ChangeBh()) return FALSE;
		break;
	case EXEC_100_1://100级防具转换
		break;
	case EXEC_100_2://解除属性限制
		break;
	case EXEC_100_3:// 添加及变更属性
		break;
	case EXEC_100_4://属性升级
		break;
	case EXEC_100_5://防具材料化
		break;
	case EXEC_100_6://解除材料化
		break;
	case RUN_SKILL:
		RunSkill120( pExec->m_ExecInt[0]);
		break;
	case EXEC_GIVE_MAGIC:////检查是否可以学习　魔法大０魔法 可以的话。就给一个
		GiveMagic_30();
		break;
	case EXEC_OPEN_YUANBAO:
		ViewYuanBaoShop(pExec->m_ExecInt[0]);
		break;
  /*  case EXCE_ON_WEB:
		if(!OpenOnWeb()) return FALSE;
		break;*/
	 case EXCE_ON_SHOP:
		if(!OpenOnShop()) return FALSE;
		break;
    case EXEC_GIVE_ITEM_ALL: //给予一个物品
		GiveAllItem(pExec->m_ExecInt[0],pExec->m_ExecInt[1],pExec->m_ExecInt[2],pExec->m_ExecInt[3],
		            pExec->m_ExecInt[4],pExec->m_ExecInt[5],pExec->m_ExecInt[6],pExec->m_ExecInt[7],pExec->m_ExecInt[8]);
		break;
	case EXEC_CHANGE_ATTRIBUTE_ITEM: //白浩换有属性浩
		ChangeAttributeItem(pExec->m_ExecInt[0],pExec->m_ExecInt[1],pExec->m_ExecInt[2],pExec->m_ExecInt[3],
		                    pExec->m_ExecInt[4],pExec->m_ExecInt[5],pExec->m_ExecInt[6],pExec->m_ExecInt[7],
		                    pExec->m_ExecInt[8],pExec->m_ExecInt[9],pExec->m_ExecInt[10],pExec->m_ExecInt[11],
							pExec->m_ExecInt[12],pExec->m_ExecInt[13],pExec->m_ExecInt[14]);
		break;
	case EXEC_TRANSFORM:
		TransformWindow( );
		break;
	case EXEC_CHANGE_JOB://转职
		{
			ChangeJob(pExec->m_ExecInt[0],pExec->m_ExecInt[1]);
			break;
		}
	case EXEC_STUDY_NEW_JOB_PSI://学习新职业魔法
		{
			StudyNewJobPsi(pExec->m_ExecInt[0]);
			break;
		}
	case EXEC_CAN_GIVE_NEWER_ITEM://能否给新手装备
		{
			if(!CanGiveNewerItem())
				return FALSE;
			break;
		}
	case EXEC_GIVE_NEWER_ITEM://新手装
		{
			GiveNewerItem(pExec->m_ExecInt[0],pExec->m_ExecInt[1],pExec->m_ExecInt[2],pExec->m_ExecInt[3],pExec->m_ExecInt[4],
				pExec->m_ExecInt[5],pExec->m_ExecInt[6],pExec->m_ExecInt[7],pExec->m_ExecInt[8],pExec->m_ExecInt[9]);
			break;
		}
	case EXEC_GIVE_NEWER_ITEM_FINISH:
		{
			GiveNewerItemFinish();
			break;
		}
	case EXEC_ONLINE_EXCHANGE://充值领取系统
		{
			OnlineExchange(pExec->m_ExecInt[0],pExec->m_ExecInt[1]);
			break;
		}

	default:
		break;
	}

	return TRUE;
}
void USER::GiveAllItem(int sid,int sCount,int iUp,int sIQ,int a1,int a2,int a3,int a4,int a5)
{
		CWordArray		arEmptySlot, arSameSlot;
		int iSlot = GetEmptySlot( INVENTORY_SLOT );

		if( iSlot < 0 ) return;

		if( sid < 0 || sid >= g_arItemTable.GetSize() ) return;


		ReSetItemSlot( &m_UserItem[iSlot]);

		m_UserItem[iSlot].sLevel = g_arItemTable[sid]->m_byRLevel;
		m_UserItem[iSlot].sSid = sid;
		m_UserItem[iSlot].sCount = sCount;
		m_UserItem[iSlot].sDuration = g_arItemTable[sid]->m_sDuration;
		m_UserItem[iSlot].sBullNum = g_arItemTable[sid]->m_sBullNum;
		m_UserItem[iSlot].tMagic[0] = a1;
		m_UserItem[iSlot].tMagic[1] = a2;
		m_UserItem[iSlot].tMagic[2] = a3;
		m_UserItem[iSlot].tMagic[3] = a4;
		m_UserItem[iSlot].tMagic[4] = a5;
		m_UserItem[iSlot].tMagic[5] = iUp;
		m_UserItem[iSlot].tIQ = sIQ;
		m_UserItem[iSlot].iItemSerial = 0;

	//	MakeItemLog( &m_UserItem[iSlot], ITEMLOG_EVENT_GIVE );

		arEmptySlot.Add(iSlot); 
		UpdateInvenSlot(&arEmptySlot, &arSameSlot);

}
void USER::GiveMagic_30()
{
	CBufferEx TempBuf;

	if(IsHavePsionic(6)==false){
		SendNpcSay( NULL, 665);
		return;
	}
	if(m_dwXP<8000){
		SendNpcSay( NULL, 666);
		return;
	}else{
		m_dwXP=m_dwXP-8000;
	}
	for(int j = 0; j < m_nHavePsiNum; j++)
	{
		if(m_UserPsi[j].sSid == 6) 
			m_UserPsi[j].sSid=30;
	}

	AddMyEventNum(202);
	SendNpcSay( NULL, 664);

	TempBuf.Add(BUY_PSY_RESULT);
	TempBuf.Add((byte)1);
	short sPsiCount = 1;
	TempBuf.Add(sPsiCount);
	for(int i = 0; i < sPsiCount; i++)
	{
		short sSid = (short)30;
		TempBuf.Add(sSid);
	}
	RobItem(1005,1);
	SendXP();
	Send(TempBuf, TempBuf.GetLength());


}
/////////////////////////////////////////////////////////////////////////
//	傍己傈矫 捞悼阑 力茄窍绊 巩磊眉啊 绝绢瘤绰 瓤苞锭巩...
//
void USER::FortressMove(CNpc *pNpc, int sid, int x1, int y1, int x2, int y2) // 观-->郴何, 郴何-->观 
{
//	BOOL bSuc = TRUE;
	int nDist = 0;

	if(!pNpc) return;

	nDist = abs(m_curx - pNpc->m_sCurX) + abs(m_cury - pNpc->m_sCurY);
	if(nDist > SCREEN_X + SCREEN_Y) return;

	CGuildFortress* pFort = NULL;
	pFort = GetFortress(sid);

	if(!pFort || pFort->m_iGuildSid <= 0) return;

	int type = CheckInvalidMapType();

	if(pNpc->m_tGuildWar == GUILD_WARRING && pNpc->m_NpcState == NPC_DEAD) return;

	if(pFort->m_iGuildSid != m_dwGuild)
	{
		SendSystemMsg( IDS_USER_NOT_THIS_GUILD_MEMBER, SYSTEM_ERROR, TO_ME);
		return;
	}
//	if(!pFort->m_bHaveGuild && pFort->m_lUsed == 0) bSuc = TRUE;

//	if(bSuc)
//	{
		switch(type)
		{
		case 8: case 10: case 15:case 17:   //不记得了
			EventMove( pNpc->m_sCurZ, x2, y2 );
			break;
		case 9: case 11: case 16:case 18:					
			EventMove( pNpc->m_sCurZ, x1, y1 );
			break;
		}		
		return;
//	}
//	else 
//	{
//		SendSystemMsg( IDS_USER_NOT_THOS_GUILD_MEMBER, SYSTEM_ERROR, TO_ME);
//	}
}

/////////////////////////////////////////////////////////////////////////
//	泅犁 辑乐绰 粮俊辑 惯积窍绰 捞亥飘甫 啊廉柯促.
//
EVENT* USER::GetEventInCurrentZone()
{
	int i;//, event_index = -1;
	EVENT *pEvent;

	for( i = 0; i < g_event.GetSize(); i++)
	{
		pEvent = g_event[i];
		if( !pEvent ) continue;
		if( pEvent->m_Zone == m_curz )
		{
//			event_index = i;
//			break;
			return g_event[i];
		}
	}

//	if(event_index == -1) return NULL;

//	return g_event[event_index];
	return NULL;
}

/////////////////////////////////////////////////////////////////////////
//	泅犁 荐青茄 捞亥飘甫 扁废茄促.
//
void USER::AddMyEventNum(int seventnum)
{
	int i;
	int* senum;

	if(MAX_EVENT_NUM <= m_arEventNum.GetSize()) return;

	for( i = 0; i < m_arEventNum.GetSize(); i++)
	{
		if( !m_arEventNum[i] ) continue;

		if( *(m_arEventNum[i]) == seventnum )
		{
			return;
		}
	}

	senum = new int;

	*senum = seventnum;

	m_arEventNum.Add( senum );
}

/////////////////////////////////////////////////////////////////////////
//
void USER::DelMyEventNum(int seventnum)
{
	int i;

	for( i = 0; i < m_arEventNum.GetSize(); i++)
	{
		if( !m_arEventNum[i] ) continue;

		if( *(m_arEventNum[i]) == seventnum )
		{
			delete m_arEventNum[i];
			m_arEventNum[i] = NULL;
			m_arEventNum.RemoveAt( i, 1 );
			DelMyEventNum( seventnum );
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//	版厚捍 牢荤 捞亥飘甫 矫累茄促.
//
void USER::GuardBow(CNpc *pNpc)
{
	if(!pNpc) return;
	if(m_sCityRank < 0) return;

	CString strMsg =_T("");
	int nMax = 0, index = 0;

	CBufferEx TempBuf, TempBuf2;

	CPoint startPt, pt;

	startPt.x = pNpc->m_sCurX;
	startPt.y = pNpc->m_sCurY;
	pt.x = m_curx;
	pt.y = m_cury;

	index = GetDirection(startPt, pt);		// 泅犁 规氢阑 钎矫茄促.;

	TempBuf2.Add(GUARD_BOW);
	TempBuf2.Add((int)pNpc->m_sNid + NPC_BAND);
	TempBuf2.Add((BYTE)index);

	SendScreen(TempBuf2, TempBuf2.GetLength());

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(NORMAL_CHAT);
	TempBuf.Add(pNpc->m_sNid + NPC_BAND);
	TempBuf.AddString(pNpc->m_strName);

	nMax = g_arNpcSayEventTable.GetSize();

	index = myrand(0, nMax - 1);

	strMsg = g_arNpcSayEventTable[index]->Say;
	strMsg.Replace("%s", m_strUserID);

	TempBuf.AddString((LPTSTR)(LPCTSTR)strMsg);

	SendScreen(TempBuf, TempBuf.GetLength());

}
void USER::NpcSaying(CNpc *pNpc)
{
	if(!pNpc) return;
	CString strMsg =_T("");
	CBufferEx TempBuf;
	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(NORMAL_CHAT);
	TempBuf.Add(pNpc->m_sNid + NPC_BAND);
	TempBuf.AddString(pNpc->m_strName);
	int error = RandChouJiang();
	if(error == 1) strMsg.Format("%s 恭喜,你抽中了.", m_strUserID);	
	else if(error == 2) strMsg.Format("%s 你的装备栏没有空间了,整理后再来吧.", m_strUserID);
	else if(error == 3)strMsg.Format("%s 元宝不足!参加抽奖要200元宝!", m_strUserID);
//	else strMsg.Format("%s 你当前不能抽奖,过会再来", m_strUserID);	

	TempBuf.AddString((LPTSTR)(LPCTSTR)strMsg);
	SendScreen(TempBuf, TempBuf.GetLength());
}
void USER::NpcXING()//时间查询显示设置
{
	CString sayStr;
	CBufferEx TempBuf, TempSayBuf , TempBuf3;
    int temptime = (3600 * 1000);
	int hour = 0,min = 0,day = 0;
//	sayStr.Format("%s",m_strUserID);//角色名字
//	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
//	sayStr.Format("%d",m_dwExp);//当前经验
//	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
//	sayStr.Format("%d",m_dwExpNext);//升级所需经验
//	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
//	if (m_dwShopPingDN > 0)
//	{
//	sayStr.Format("%d 个",m_dwShopPingDN);//显示元宝
//	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
//	}
//	else
//	{
//		sayStr.Format("0 个");
//		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
//	}
////////////////////////////////////////////////////////////////////////////////////////
	/*if (m_dwGuarDianTianShi > 0)
	{
	hour = m_dwGuarDianTianShi/temptime;	min = m_dwGuarDianTianShi%temptime/(60000);
	sayStr.Format("%d小时%d分",hour,min);//VIP时间
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}*/
	if (m_dwBHTime > 0)
	{
	hour = m_dwBHTime/temptime;	min = m_dwBHTime%temptime/(60000);
	sayStr.Format("%d小时%d分",hour,min);//天使双倍时间
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	if (m_dwVIPTime > 0)
	{
	hour = m_dwVIPTime/temptime;	min = m_dwVIPTime%temptime/(60000);
	sayStr.Format("%d小时%d分",hour,min);//幸福时光
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	
	
	if (m_dwHiExpTime > 0)
	{
	hour = m_dwHiExpTime/temptime;	min = m_dwHiExpTime%temptime/(60000);
	sayStr.Format("%d小时%d分",hour,min);//兴奋时间
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if (m_dwMagicFindTime > 0)
	{
	hour = m_dwMagicFindTime/temptime;   min = m_dwMagicFindTime%temptime/(60000);
	sayStr.Format("%d小时%d分",hour,min);//幸运
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if (m_dwMagicFtTime > 0)
	{
	hour = m_dwMagicFtTime/temptime;	min = m_dwMagicFtTime%temptime/(60000);
	sayStr.Format("%d小时%d分",hour,min);//幻想晶石
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if (m_dwHtExpTime > 0)
	{
	hour = m_dwHtExpTime/temptime;	min = m_dwHtExpTime%temptime/(60000);
	sayStr.Format("%d小时%d分",hour,min);//幻想灵石
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	/*if (m_isDoubleBAOLV > 0)//捐献时间
	{
	hour = m_isDoubleBAOLV/temptime;	min = m_isDoubleBAOLV%temptime/(60000);
    sayStr.Format("%d小时%d分",hour,min);//捐献时间
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}*/

	if (m_dwShopPingDN > 0)
	{
	sayStr.Format("%d 个",m_dwShopPingDN);//显示元宝
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else
	{
		sayStr.Format("0 个");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	
	
	if (m_dwBFindTime > 0)//变身时间
	{
	hour = m_dwBFindTime/temptime;	min = m_dwBFindTime%temptime/(60000);
    sayStr.Format("%d小时%d分",hour,min);
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );}
	else 
	{
		sayStr.Format("0小时0分");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	
	if (m_dwZaiXianTime > 0)//在线时间
	{
	hour = m_dwZaiXianTime/temptime;	min = m_dwZaiXianTime%temptime/(60000);
    sayStr.Format("%d小时%d分",hour,min);//VIP时间
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("0小时0分钟");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	/////////////////////
	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)5009);
	TempBuf.Add((BYTE)0x09);//发送的显示多少行
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
	
}
/////////////////////////////////////
void USER::shuxXING(int uid)//属性查询显示设置
{
	CString sayStr;
	CBufferEx TempBuf, TempSayBuf , TempBuf3;
	int lil = 0,tiz = 0,minj = 0 ,zhih = 0,zhil = 0,dengj = 0;

	USER *pUser = GetUser(uid- USER_BAND);
	
	if(pUser == NULL)
		return ;
	BYTE tWeaponClass = 0;
	short	sMaxDamage = 0, sMaxDefense = 0;
	short	sTemp = 0;
	IsCanUseWeaponSkill(tWeaponClass);
	sMaxDamage = pUser->GetMaxDamage(tWeaponClass);//攻击力
	
	sTemp = (short)GetIronSkill();
	sMaxDefense = sTemp + pUser->GetDefense(); //防御力

	short spellattack=pUser->GetUserSpellAttack(TRUE);//魔法攻击
	short spelldefence=pUser->GetUserSpellDefence();;//计算玩家自己的魔法防御

	if (pUser->m_sLevel > 0)
	{
	dengj = pUser->m_sLevel;	
	sayStr.Format("等级%d",dengj);//等级
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	if (pUser->m_sSTR > 0)
	{
	lil = pUser->m_sSTR;	
	sayStr.Format("力量%d",lil);//力量
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	

	if (pUser->m_sCON > 0)
	{
	tiz = pUser->m_sCON;	
	sayStr.Format("体质%d",tiz);//体质
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
		
	if (pUser->m_sDEX > 0)
	{
	minj = pUser->m_sDEX;	
	sayStr.Format("敏捷%d",minj);//敏捷
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	
	if (pUser->m_sVOL > 0)
	{
	zhih = pUser->m_sVOL;	
	sayStr.Format("智慧%d",zhih);//智慧
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	
	if (pUser->m_sWIS > 0)
	{
	zhil = pUser->m_sWIS;	
	sayStr.Format("智力%d",zhil);//智力
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	if (sMaxDamage > 0)
	{
	
	sayStr.Format("攻击%d",sMaxDamage);//物理攻击
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	if (sMaxDefense > 0)
	{
	
	sayStr.Format("防御%d",sMaxDefense);//物理防御
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	if (spellattack > 0)
	{
	
	sayStr.Format("攻击%d",spellattack);//魔法攻击
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	if (spelldefence > 0)
	{
	
	sayStr.Format("防御%d",spelldefence);//魔法防御
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	
	/////////////////////
	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)5111);
	TempBuf.Add((BYTE)0x0a);//发送的显示多少行
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
	
}
/////////////////////////////////////////////////////////////////////////////////////

void USER::NpcDuBoDA(int money,int type)
{
	CString sayStr;
	CString str;
	CBufferEx TempBuf, TempSayBuf , TempBuf3;
	if (type == 1)				//  赌博游戏币
	{
		if(m_dwDN >= (DWORD)money)
		{
			int result = myrand(1,7);
			if(result > 6) result =myrand(1,3);
			if(result >=4)
			{
				sayStr.Format("%d 游戏币",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("%d",result);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("大");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5002);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwDN += money;
				SendMoneyChanged();
				UpdateUserItemDN();
				str.Format( "玩家[ %s ]赌神发力,在赌场嬴了[ %d ]游戏币!", m_strUserID, money);
                SendSystemMsg(str.GetBuffer(str.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				for(int i = 0; i < 5; i++)
				{
					TempBuf3.Add(USE_POTION);		
					TempBuf3.Add(m_uid + USER_BAND);
					TempBuf3.Add((BYTE)5);	//中奖时带烟花效果
					Send( TempBuf3, TempBuf3.GetLength() );
				}
			}
			else
			{
				sayStr.Format("%d 游戏币",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("%d",result);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("小");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5003);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwDN -= money;
				SendMoneyChanged();
				UpdateUserItemDN();
			}
		}
		else
		{
			sayStr.Format("%d",money);
			TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
			TempBuf.Add(CLIENT_EVENT_SAY);
			TempBuf.Add((BYTE)SUCCESS);
			TempBuf.Add((short)5001);
			TempBuf.Add((BYTE)0x01);
			TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}
	}
	else				//  赌博元宝
	{
		if(m_dwShopPingDN >= (DWORD)money)
		{
			int result = myrand(1,7);
			if(result > 6) result =myrand(1,3);
			if(result >=4)
			{
				sayStr.Format("%d 元宝",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("%d",result);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("大");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5002);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwShopPingDN += money;
				SendUserStatusSkill();//更新点卷数量
				str.Format( "玩家[ %s ]牛气冲天,在赌场嬴了[ %d ]元宝!", m_strUserID, money);
                SendSystemMsg(str.GetBuffer(str.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				for(int i = 0; i < 5; i++)
				{
					TempBuf3.Add(USE_POTION);		
					TempBuf3.Add(m_uid + USER_BAND);
					TempBuf3.Add((BYTE)5);	//中奖时带烟花效果
					Send( TempBuf3, TempBuf3.GetLength() );
				}
			}
			else
			{
				sayStr.Format("%d 元宝",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("%d",result);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("小");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5003);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwShopPingDN -= money;
				SendUserStatusSkill();//更新元宝数量
			}
		}
		else
		{
			sayStr.Format("%d",money);
			TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
			TempBuf.Add(CLIENT_EVENT_SAY);
			TempBuf.Add((BYTE)SUCCESS);
			TempBuf.Add((short)5001);
			TempBuf.Add((BYTE)0x01);
			TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}

	}
	
}
void USER::NpcDuBoXIAO(int money,int type)
{
	CString sayStr;
	CString str;
	CBufferEx TempBuf, TempSayBuf ,TempBuf3;
	if(type == 1) //赌博游戏币
	{
		if(m_dwDN >= (DWORD)money)
		{
			int result = myrand(1,7);
			if(result > 6) result =myrand(4,6);
			if(result <= 3)
			{
				sayStr.Format("%d 游戏币",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("%d",result);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("小");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5002);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwDN += money;
				SendMoneyChanged();
				UpdateUserItemDN();
				str.Format( "玩家[ %s ]意气风发,在赌场嬴了[ %d ]游戏币!", m_strUserID, money);
                SendSystemMsg(str.GetBuffer(str.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				for(int i = 0; i < 5; i++)
				{
					TempBuf3.Add(USE_POTION);		
					TempBuf3.Add(m_uid + USER_BAND);
					TempBuf3.Add((BYTE)5);	//中奖时带烟花效果
					Send( TempBuf3, TempBuf3.GetLength() );
				}
			}
			else
			{
				sayStr.Format("%d 游戏币",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("%d",result);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("大");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5003);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwDN -= money;
				SendMoneyChanged();
				UpdateUserItemDN();
			}
		}
		else
		{
			sayStr.Format("%d",money);
			TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
			TempBuf.Add(CLIENT_EVENT_SAY);
			TempBuf.Add((BYTE)SUCCESS);
			TempBuf.Add((short)5001);
			TempBuf.Add((BYTE)0x01);
			TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}

	}
	else		//赌博点卷
	{
		if(m_dwShopPingDN >= (DWORD)money)
		{
			int result = myrand(1,7);
			if(result > 6) result =myrand(4,6);
			if(result <= 3)
			{
				sayStr.Format("%d 元宝",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("%d",result);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("小");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5002);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwShopPingDN += money;
				SendUserStatusSkill();
				str.Format( "玩家[ %s ]红光满面,在赌场嬴了[ %d ]元宝!", m_strUserID, money);
                SendSystemMsg(str.GetBuffer(str.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				for(int i = 0; i < 5; i++)
				{
					TempBuf3.Add(USE_POTION);		
					TempBuf3.Add(m_uid + USER_BAND);
					TempBuf3.Add((BYTE)5);	//中奖时带烟花效果
					Send( TempBuf3, TempBuf3.GetLength() );
				}
			}
			else
			{
				sayStr.Format("%d 元宝",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("%d",result);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("大");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5003);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwShopPingDN -= money;
				SendUserStatusSkill();
			}
		}
		else
		{
			sayStr.Format("%d",money);
			TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
			TempBuf.Add(CLIENT_EVENT_SAY);
			TempBuf.Add((BYTE)SUCCESS);
			TempBuf.Add((short)5001);
			TempBuf.Add((BYTE)0x01);
			TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}

	}
	
}
/////////////////////////////////////////////////////////////////////////

//
void USER::SendFieldGuildSay(CNpc *pNpc, int nStore, int say)
{
	int nChatNum = -1;
	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;

	if(nStore >= FORTRESS_BAND)						// 夸货俊 加茄 惑痢捞搁...
	{
		pFort = GetFortress(nStore);
		if(pFort == NULL) return;

		if(pNpc)
		{
			if(pNpc->m_tNpcType == NPCTYPE_GUILD_GUARD)
			{
				if(pFort->m_tWarType != GUILD_WAR_DECISION)
				{
					if(pFort->m_sFortressID == 1000) nChatNum = 145;
					else if(pFort->m_sFortressID == 1001) nChatNum = 165;
					else if(pFort->m_sFortressID == 1002) nChatNum = 318;
				}
				else
				{
					if(pFort->m_iGuildSid != m_dwGuild && m_bGuildMaster)
					{
						for(int i = 0; i < GUILDFORTRESS_ATTACK_MAX_NUM; i++)
						{
							if(pFort->m_arAttackGuild[i].lGuild == m_dwGuild)
							{
								if(pFort->m_sFortressID == 1000) nChatNum = 144;
								else if(pFort->m_sFortressID == 1001) nChatNum = 164;
								else if(pFort->m_sFortressID == 1002) nChatNum = 317;
							}
						}

						if(nChatNum == -1)
						{
							if(pFort->m_iGuildSid == 900000)
							{
								if(pFort->m_sFortressID == 1000) nChatNum = 146;
								else if(pFort->m_sFortressID == 1001) nChatNum = 166;
								else if(pFort->m_sFortressID == 1002) nChatNum = 319;
							}
							else 
							{
								if(pFort->m_sFortressID == 1000) nChatNum = 143;
								else if(pFort->m_sFortressID == 1001) nChatNum = 163;
								else if(pFort->m_sFortressID == 1002) nChatNum = 316;
							}
						}
					}
					else 
					{
						if(pFort->m_sFortressID == 1000) nChatNum = 144;
						else if(pFort->m_sFortressID == 1001) nChatNum = 164;
						else if(pFort->m_sFortressID == 1002) nChatNum = 317;
					}
				}
			}
			else
			{
				nChatNum = say;
			}
		}
		else
		{
			nChatNum = say;
		}
/*
		if(m_bGuildMaster)
		{
			if(pFort->m_iGuildSid == m_dwGuild) nChatNum = say;
			for(int i = 0; i < GUILDFORTRESS_ATTACK_MAX_NUM; i++)
			{
				if(pFort->m_arAttackGuild[i].lGuild == m_dwGuild) { nChatNum = 144; break; }
			}

			if(nChatNum == -1 && pFort->m_tWarType == GUILD_WAR_DECISION) nChatNum = 143;
			if(nChatNum == -1) nChatNum = 145;
		}
		else
		{
			if(pFort->m_tWarType == GUILD_WAR_DECISION)
			{
				if(pFort->m_iGuildSid == 900000)
				{
					if(pFort->m_sFortressID == 1000) nChatNum = 146;
					else if(pFort->m_sFortressID == 1001) nChatNum = 166;
				}
				else nChatNum = 144;
			}
			else 
			{
				SendSystemMsg( IDS_USER_NOT_YET_FIX_SCHEDULE, SYSTEM_NORMAL, TO_ME);
				return;
			}
		}
*/
		EditHyperFortressText(pFort, nChatNum);
	}
/*	else
	{
		pStore = GetStore(nStore);
		if(pStore == NULL) return;

		if(say == 1)								// 辨靛郴狼 惑痢林牢富...
		{
			if(pStore->m_sStoreID == 4)
			{
				if(strcmp(pStore->m_strMasterName, m_strUserID) == 0) nChatNum = 33;		// 
				else if(pStore->m_tWarType == GUILD_WAR_PREPARE) nChatNum = 36;
				else if(pStore->m_tWarType == GUILD_WAR_DECISION) nChatNum = 34;
			}
			else if(pStore->m_sStoreID == 5)
			{
				if(strcmp(pStore->m_strMasterName, m_strUserID) == 0) nChatNum = 43;		// 
				else if(pStore->m_tWarType == GUILD_WAR_PREPARE) nChatNum = 46;
				else if(pStore->m_tWarType == GUILD_WAR_DECISION) nChatNum = 44;
			}
			else if(pStore->m_sStoreID == 6)
			{
				if(strcmp(pStore->m_strMasterName, m_strUserID) == 0) nChatNum = 53;		// 
				else if(pStore->m_tWarType == GUILD_WAR_PREPARE) nChatNum = 56;
				else if(pStore->m_tWarType == GUILD_WAR_DECISION) nChatNum = 54;
			}
		}
		else
		{
			if(say == 37 && pStore->m_iGuildSid >= 900000) nChatNum = 30;	// 辨靛啊 贸澜 盒剧且锭 37锅 SAY客 辨靛锅龋啊 900000老锭肺...
			else if(say == 47 && pStore->m_iGuildSid >= 900000) nChatNum = 40;
			else if(say == 57 && pStore->m_iGuildSid >= 900000) nChatNum = 50;
			else nChatNum = say;// 唱赣柳 窜开 硅快狼 陋篮 富
		}

		EditHyperText(pStore, nChatNum);
	}
*/
}

/////////////////////////////////////////////////////////////////////////
//	版厚盔俊霸 辨靛 傈阑 脚没茄促.
//
void USER::GuildWarApply(int nStore)
{
	int nChatNum = -1;
	int nCount = 0;
	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;

	if(nStore >= FORTRESS_BAND)						// 夸货俊 加茄 惑痢捞搁...
	{
		pFort = GetFortress(nStore);
		if(pFort == NULL) return;

		if(pFort->m_tWarType == GUILD_WAR_DECISION) 
		{	
			if(m_bGuildMaster) 
			{
				CBufferEx TempBuf;

				TempBuf.Add(GUILD_WAR_APPLY_OPEN);
				TempBuf.Add((short)pFort->m_sFortressID);
				TempBuf.AddString(pFort->m_strGuildName);

				Send(TempBuf, TempBuf.GetLength());
				return;
			}		
		}

		nChatNum = 145;

		EditHyperFortressText(pFort, nChatNum);
	}

}

/////////////////////////////////////////////////////////////////////////
//	版厚盔俊霸 辨靛 傈阑 脚没茄促.
//
void USER::GuildWarApplyReq(TCHAR *pBuf)
{
	int index = 0;

	if(!m_bGuildMaster || m_dwGuild <= 0)
	{
		SendSystemMsg( IDS_USER_ONLY_GUILD_MASTER_WAR, SYSTEM_NORMAL, TO_ME);
		return;
	}

	CString strMsg;
	short sStore = GetShort(pBuf, index);

	if(sStore < 0) return;

	if(sStore >= FORTRESS_BAND)	GuildFortressWarApplyReq(sStore);		
//	else						GuildFieldStoreWarApplyReq(sStore);
}

void USER::GuildFieldStoreWarApplyReq(int sid)
{
/*	int i, j;

	CString strMsg;
	
	CStore *pStore = NULL;
	CGuildFortress *pFort = NULL;

	pStore = GetStore(sid);

	if(!pStore) return;

	if(pStore->m_lUsed == 1)
	{
		SendSystemMsg( IDS_USER_CANT_APPLY_IN_WAR, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(pStore->m_iGuildSid == m_dwGuild)
	{
		SendSystemMsg( IDS_USER_CANT_APPLY_TO_MY_GUILD, SYSTEM_NORMAL, TO_ME);
		return;
	}

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(g_arGuildFortress[i] == NULL) continue;

		pFort = g_arGuildFortress[i];

		if(pFort->m_iGuildSid == m_dwGuild)
		{
			SendSystemMsg( IDS_USER_CANT_APPLY_HAVE_FORTRESS, SYSTEM_NORMAL, TO_ME);
			return;
		}
	}

	for(i = 0; i < g_arStore.GetSize(); i++)
	{
		if(g_arStore[i] == NULL) continue;

		if(InterlockedCompareExchange((LONG*)&g_arStore[i]->m_lListUsed, (PVOID*)1, (PVOID*)0) == 0)
		{
			for(j = 0; j < GUILD_ATTACK_MAX_NUM; j++)
			{
				if(g_arStore[i]->m_arAttackGuild[j] == m_dwGuild)
				{
					strMsg.Format( IDS_USER_ALREADY_APPLY_OTHER_GUILD, g_arStore[i]->m_strGuildName);
					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);					
					InterlockedExchange(&g_arStore[i]->m_lListUsed, 0);
					return;
				}
			}
			InterlockedExchange(&g_arStore[i]->m_lListUsed, 0);
		}
	}

	if(pStore->m_tWarType == GUILD_WAR_DECISION)
	{
		for(i = 0; i < g_arStore.GetSize(); i++)
		{
			if(g_arStore[i] == NULL) continue;

			if(g_arStore[i]->m_sStoreID == sid)
			{
				if(g_arStore[i]->m_lListCount >= GUILD_ATTACK_MAX_NUM) 
				{
					strMsg.Format( IDS_USER_OVERFLOW_GUILD_WAR_ENEMY, pStore->m_strGuildName);
					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);					
					return;
				}

				if(InterlockedCompareExchange((LONG*)&g_arStore[i]->m_lListUsed, (PVOID*)1, (PVOID*)0) == 0)
				{
					g_arStore[i]->m_arAttackGuild[g_arStore[i]->m_lListCount] = m_dwGuild;
					InterlockedIncrement(&g_arStore[i]->m_lListCount);

					UpdateMemStoreGuildList(g_arStore[i]);		// 皋葛府DB俊 敬促.

					SendSystemMsg( IDS_USER_APPLIED_GUILD_WAR, SYSTEM_NORMAL, TO_ME);					

					InterlockedExchange(&g_arStore[i]->m_lListUsed, 0);
				}
			}
		}
	}
	else SendSystemMsg( IDS_USER_NOT_FIX_SCHEDULE, SYSTEM_NORMAL, TO_ME);
*/
}

void USER::GuildFortressWarApplyReq(int sid)
{
	int i, j, nLen = 0;

	CString strMsg;

	CGuild *pGuild = NULL;
	CGuildFortress *pFort = NULL;
	CGuildFortress *pTempFort = NULL;

	pFort = GetFortress(sid);

	if(!pFort) return;

	if(m_sLevel < 70)
	{
		SendSystemMsg( IDS_USER_APPLY_CONDITION_LEVEL, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(pFort->m_lUsed == 1)
	{
		SendSystemMsg( IDS_USER_CANT_APPLY_IN_WAR, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(pFort->m_iGuildSid == m_dwGuild)
	{
		SendSystemMsg( IDS_USER_CANT_APPLY_TO_MY_GUILD, SYSTEM_NORMAL, TO_ME);
		return;
	}

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) 
	{
		ReleaseGuild();				// 秦力...
		return;
	}

	j = 0;
	CGuildUser pGUser;

	for(i = 0; i < MAX_GUILD_USER; i++)
	{
		pGUser = pGuild->m_arUser[i];
		if(pGUser.m_lUsed != 0) j++;
	}

	ReleaseGuild();

	if(j < 1)//申请要塞所需军团人数
	{
		SendSystemMsg( IDS_USER_APPLY_CONDITION_GMEMBER, SYSTEM_NORMAL, TO_ME);
		return;
	}
/*
	for(i = 0; i < g_arGuildHouse.GetSize(); i++)
	{
		if(g_arGuildHouse[i]->iGuild == m_dwGuild)
		{
			SendSystemMsg( IDS_USER_CANT_APPLY_HAVE_HOUSE, SYSTEM_NORMAL, TO_ME);
			return;
		}
	}
*/
	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(g_arGuildFortress[i] == NULL) continue;

		pTempFort = g_arGuildFortress[i];

		if(pTempFort->m_iGuildSid == m_dwGuild)
		{
			SendSystemMsg( IDS_USER_CANT_APPLY_HAVE_FORTRESS, SYSTEM_NORMAL, TO_ME);
			return;
		}
	}

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(g_arGuildFortress[i] == NULL) continue;

		pTempFort = g_arGuildFortress[i];

		if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_lListUsed, (long)1, (long)0) == (long)0)
		{
			for(j = 0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
			{
				if(pTempFort->m_arAttackGuild[j].lGuild == m_dwGuild)
				{
					if(pTempFort->m_sFortressID == 1000) strMsg.Format( IDS_USER_ALREADY_LUINET_FORTRESS );
					else if(pTempFort->m_sFortressID == 1001) strMsg.Format( IDS_USER_ALREADY_SANAD_FORTRESS );
					else if(pTempFort->m_sFortressID == 1002) strMsg.Format( IDS_USER_ALREADY_ZAMELLYA_FORTRESS );

					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);					
					InterlockedExchange(&g_arGuildFortress[i]->m_lListUsed, 0);
					return;
				}
			}
			InterlockedExchange(&g_arGuildFortress[i]->m_lListUsed, 0);
		}
	}

	if(pFort->m_tWarType == GUILD_WAR_DECISION)
	{
		for(i = 0; i < g_arGuildFortress.GetSize(); i++)
		{
			if(g_arGuildFortress[i] == NULL) continue;

			if(g_arGuildFortress[i]->m_sFortressID == sid)
			{
				if(g_arGuildFortress[i]->m_lListCount >= GUILDFORTRESS_ATTACK_MAX_NUM) 
				{
					strMsg.Format( IDS_USER_OVERFLOW_GUILD_WAR_ENEMY, pFort->m_strGuildName);
					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);					
					return;
				}

				if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_lListUsed, (long)1, (long)0) == (long)0)
				{
/*					nLen = strlen(m_strGuildName);
					g_arGuildFortress[i]->m_arAttackGuild[g_arGuildFortress[i]->m_lListCount].lGuild = m_dwGuild;
					strncpy(g_arGuildFortress[i]->m_arAttackGuild[g_arGuildFortress[i]->m_lListCount].strGuildName, m_strGuildName, nLen);
					InterlockedIncrement(&g_arGuildFortress[i]->m_lListCount);
*/					
					if(UpdateFortress(g_arGuildFortress[i]) == FALSE)
					{
						SendSystemMsg( IDS_USER_APPLY_FAIL, SYSTEM_NORMAL, TO_ME);
						InterlockedExchange(&g_arGuildFortress[i]->m_lListUsed, 0);
						return;
					}

//					UpdateMemAttackListNpc(g_arGuildFortress[i]->m_sFortressID);// Agent俊绰 歹捞惑 历厘救茄促.
					pTempFort = g_arGuildFortress[i];

					for(j = 0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
					{
						if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_arAttackGuild[j].lUsed, (long)1, (long)0) == (long)0)
						{
							nLen = strlen(m_strGuildName);
							g_arGuildFortress[i]->m_arAttackGuild[j].lGuild = m_dwGuild;
							strncpy(g_arGuildFortress[i]->m_arAttackGuild[j].strGuildName, m_strGuildName, nLen);

							SendSystemMsg( IDS_USER_APPLY_COMPLETED, SYSTEM_NORMAL, TO_ME);
							break;
						}
					}										

					InterlockedExchange(&g_arGuildFortress[i]->m_lListUsed, 0);
				}
			}
		}
	}
	else SendSystemMsg( IDS_USER_NOT_FIX_SCHEDULE, SYSTEM_NORMAL, TO_ME);
}

/////////////////////////////////////////////////////////////////////////
//	鞍篮粮救狼 漂沥厘家肺 捞悼矫挪促.
//
void USER::EventMove(int zone, int moveX, int moveY)
{
	BYTE result = FAIL;
	int index = 0;

	if(zone != m_curz) return;

	CPoint pt = FindNearAvailablePoint_C(moveX, moveY);
	CPoint pc;
	if(pt.x != -1 && pt.y != -1) 
	{
		pc = ConvertToServer(pt.x, pt.y);
		if(pc.x < 0 || pc.y < 0 || pc.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pc.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) 
		{
			result = FAIL;
		}
		else
		{
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pc.x][pc.y].m_lUser, USER_BAND + m_uid);
			m_curx = pc.x;
			m_cury = pc.y;

			result = SUCCESS;
		}
	}

	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, pt.x, index);
	SetShort(m_TempBuf, pt.y, index);
	Send(m_TempBuf, index);

	m_dwNoDamageTime = NO_DAMAGE_TIME;			// 公利矫埃 悸泼
	m_dwLastNoDamageTime = GetTickCount();

	SendInsight(m_TempBuf, index);

	SightRecalc();
}

/////////////////////////////////////////////////////////////////////////
//	葛电 HP, PP, SP甫 菜盲款促.
//
void USER::SendAllCure(int iStoreID, int iCost)
{
	BYTE result = SUCCESS;

	CBufferEx TempBuf;

	DWORD dwCost = (DWORD)iCost;

	CStore *pStore = NULL;
	pStore = GetStore(iStoreID);

	if(!pStore) return;

	// 捞亥飘 困摹甫 八荤 ----------------------------------------//
	CPoint pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, CURE_BLOCK)) return;

	DWORD dwTax = (DWORD)( dwCost * ((double)pStore->m_sRate/100) );
	dwCost = (DWORD)(dwCost + dwTax + 0.999);			// 棵覆(DWORD 肺 父甸扁 困秦辑)

	UpdateFortressTax(iStoreID, dwTax);					// 器飘府胶 技陛捞搁 历厘...

	if(m_sHP >= m_sMagicMaxHP && m_sPP >= m_sMagicMaxPP && m_sSP >= m_sMagicMaxSP)
	{
		SendSystemMsg( IDS_USER_NEED_NOT_CURE, SYSTEM_NORMAL, TO_ME);
		result = FAIL;
		goto go_result;
	}

	if(dwCost > m_dwDN) 
	{ 
		SendSystemMsg( IDS_USER_NOT_ENOUGH_DINA_CURE, SYSTEM_NORMAL, TO_ME); 
		result = FAIL;
		goto go_result;
	}
	else m_dwDN = m_dwDN - dwCost;

	m_sHP = m_sMagicMaxHP;
	m_sPP = m_sMagicMaxPP;
	m_sSP = m_sMagicMaxSP;

go_result:
	TempBuf.Add(CURE_RESULT);

	if(result == FAIL) 
	{
		TempBuf.Add(result);			// 角菩
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add((BYTE)0x01);			// 雀汗

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sPP);
	TempBuf.Add(m_sSP);

	SendMoneyChanged();
	Send(TempBuf, TempBuf.GetLength());
	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.

	SendSystemMsg( IDS_USER_CURE_COMPLETED, SYSTEM_NORMAL, TO_ME);
}

/////////////////////////////////////////////////////////////////////////
//	Zone Change甫 矫累茄促.
//	//^^ 捞 窃荐 磊眉档 BOOL 屈侥捞绢具 且 巴 鞍促.
void USER::ZoneMoveReq(int zone, int moveX, int moveY)
{
	BOOL bSuccess = FALSE;							// 角菩, 己傍狼 搬苞蔼阑 历厘
	BOOL bFoundDBIndex = FALSE;

	int i, j;

	if( !IsZoneInThisServer(zone) )
	{
		SERVERINFO* pNewData;
		int iNum, sZone, find_flag = FALSE;
		CString strIP;
		int nPort;

		// 粮俊 秦寸窍绰 IP, PORT 茫扁...
		for( i = 0; i < g_server.GetSize() && find_flag == FALSE; i++)
		{
			BOOL bTrue =FALSE;
			pNewData = g_server[i];

			if ( pNewData == NULL ) continue;
																		// -1捞搁 葛滴啊 哎 荐 乐绰 辑滚捞聪鳖 眉农窍霸 窍绊
			if( pNewData->m_sDBIndex != -1 )							// 酒聪搁 郴啊 哎 荐 乐绰 辑滚牢瘤 眉农窍霸 茄促
			{
				if( pNewData->m_sDBIndex != m_iMyServer ) 
				{
					if(pNewData->m_sDBIndex == 11 &&(m_iMyServer == 1 || m_iMyServer == 2 ||m_iMyServer == 3 ||m_iMyServer == 4 ||m_iMyServer == 5))
						bTrue = TRUE;
					else if(pNewData->m_sDBIndex == 12 &&(m_iMyServer == 6 || m_iMyServer == 7 ||m_iMyServer == 8 ||m_iMyServer == 9 ||m_iMyServer == 10))
						bTrue = TRUE;
					else 
						continue;		// 捞 辑滚 沥焊啊 唱狼 单捞磐甫 啊瘤绊 乐绰 辑滚咯具 茄促.
				}
			}
			
			// -1捞芭唱 哎 荐 乐绰 辑滚扼绊 魄窜瞪锭
			iNum = pNewData->m_zone.GetSize();
			for ( j = 0; j < iNum; j++ )
			{
				sZone = *(pNewData->m_zone[j]);
				if ( sZone == zone )				// 郴啊 啊绊磊 窍绰 粮捞 乐绰瘤 眉农
				{
					find_flag = TRUE;				// 乐栏搁 弊 辑滚狼 林家甫 郴妨霖促.
					m_strZoneIP = strIP = pNewData->m_szAddr;
					m_nZonePort = nPort = pNewData->m_nPort;

					break;
				}
			}
		}

		if ( find_flag == TRUE )
		{
			TRACE("INTER COMPUTER ZONE CHANGE STARTED[]...\n");

			if( m_bZoneLogOut ) return;

			m_bZoneLogOut = TRUE;

			g_pMainDlg->BridgeServerUserZoneLogOut( m_uid, m_strUserID, zone, moveX, moveY );
			return;

			CPoint pt = ConvertToServerByZone( zone, moveX, moveY );

			if( pt.x < 0 || pt.y < 0 ) return;

			int save_z = zone;
			int save_x = pt.x;
			int save_y = pt.y;

			m_strZoneIP = strIP;
			m_nZonePort	= nPort;

			// 单捞鸥 历厘...
			TRACE("Zone Change Data Saved...\n");
			ZoneLogOut(save_z, save_x, save_y);
			//

			m_curz = zone;
			m_curx = save_x;
			m_cury = save_y;

			CBufferEx TempBuf;
			BYTE result = FAIL;
			
			TempBuf.Add(ZONE_CHANGE_RESULT);

			result = SUCCESS_ZONE_CHANGE;
			TempBuf.Add(result);
			TempBuf.AddString((char *)LPCTSTR(strIP));
			TempBuf.Add(nPort);

			Send(TempBuf, TempBuf.GetLength());
			return;
		}
		//

		SendZoneChange(FALSE);
		return;
	}

	// 鞍篮 辑滚埃狼 粮 函券...
	bSuccess = ZoneChangeProcess(zone, moveX, moveY);
	
	if(bSuccess)
	{
		SendWeatherInMoveZone();			// 捞悼 粮狼 朝揪函拳甫 舅赴促.
		SightRecalc();						// 郴 矫具救狼 沥焊甫 唱俊霸 焊辰促.
		SendHuFaInfo(this,TO_INSIGHT);

	}

	TRACE("SAVE-COMPUTER ZONE CHANGE...\n");
}

void USER::ZoneMoveMini(int zone, int moveX, int moveY)
{


	BOOL bSuccess = FALSE;							
	BOOL bFoundDBIndex = FALSE;

	int i, j;

	if( !IsZoneInThisServer(zone) )
	{
		SERVERINFO* pNewData;
		int iNum, sZone, find_flag = FALSE;
		CString strIP;
		int nPort;

		// 
		for( i = 0; i < g_server.GetSize() && find_flag == FALSE; i++)
		{
			BOOL bTrue =FALSE;
			pNewData = g_server[i];

			if ( pNewData == NULL ) continue;
																		// 
			if( pNewData->m_sDBIndex != -1 )							// 
			{
				if( pNewData->m_sDBIndex != m_iMyServer ) 
				{
					if(pNewData->m_sDBIndex == 11 &&(m_iMyServer == 1 || m_iMyServer == 2 ||m_iMyServer == 3 ||m_iMyServer == 4 ||m_iMyServer == 5))
						bTrue = TRUE;
					else if(pNewData->m_sDBIndex == 12 &&(m_iMyServer == 6 || m_iMyServer == 7 ||m_iMyServer == 8 ||m_iMyServer == 9 ||m_iMyServer == 10))
						bTrue = TRUE;
					else 
						continue;		// 
				}
			}
			
			// 
			iNum = pNewData->m_zone.GetSize();
			for ( j = 0; j < iNum; j++ )
			{
				sZone = *(pNewData->m_zone[j]);
				if ( sZone == zone )				// 
				{
					find_flag = TRUE;				// 
					m_strZoneIP = strIP = pNewData->m_szAddr;
					m_nZonePort = nPort = pNewData->m_nPort;

					break;
				}
			}
		}

		if ( find_flag == TRUE )
		{
			TRACE("INTER COMPUTER ZONE CHANGE STARTED[]...\n");

			if( m_bZoneLogOut ) return;

			m_bZoneLogOut = TRUE;

			g_pMainDlg->BridgeServerUserZoneLogOut( m_uid, m_strUserID, zone, moveX, moveY );
			return;

			CPoint pt = ConvertToServerByZone( zone, moveX, moveY );

			if( pt.x < 0 || pt.y < 0 ) return;

			int save_z = zone;
			int save_x = pt.x;
			int save_y = pt.y;

			m_strZoneIP = strIP;
			m_nZonePort	= nPort;

			// 
			TRACE("Zone Change Data Saved...\n");
			ZoneLogOut(save_z, save_x, save_y);
			//

			m_curz = zone;
			m_curx = save_x;
			m_cury = save_y;

			CBufferEx TempBuf;
			BYTE result = FAIL;
			
			TempBuf.Add(ZONE_CHANGE_RESULT);

			result = SUCCESS_ZONE_CHANGE;
			TempBuf.Add(result);
			TempBuf.AddString((char *)LPCTSTR(strIP));
			TempBuf.Add(nPort);

			Send(TempBuf, TempBuf.GetLength());
			return;
		}
		//

		SendZoneChange(FALSE);
		return;
	}

	// 
	bSuccess = ZoneChangeProcess(zone, moveX, moveY);
	
	if(bSuccess)
	{
		SendWeatherInMoveZone();			// 
		SightRecalc();						
	}

	TRACE("SAVE-COMPUTER ZONE CHANGE...\n");
}



/////////////////////////////////////////////////////////////////////////
//	粮眉牢瘤狼 角力 贸府甫 淬寸茄促.
//
BOOL USER::ZoneChangeProcess(int new_z, int moveX, int moveY)
{
	int i, rand;
	BYTE result = SUCCESS;

	MAP* pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	pMap = g_zone[m_ZoneIndex];		if( !pMap ) return FALSE;

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) return FALSE;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) return FALSE;

//	if( pMap->m_pMap[m_curx][m_cury].m_lUser != USER_BAND + m_uid ) return FALSE;

//	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser != USER_BAND + m_uid ) return FALSE;		
					
	CPoint ptPotal, pt;
	m_NowZoneChanging = TRUE;					// Zone Change 窍绊 乐促.

	/*************************[ Back Up ]***************************************/	
	int org_x = m_curx;
	int org_y = m_cury;
	int org_z = m_curz;

	/*************************[ Zone Change Init]*******************************/
	SetZoneIndex(new_z);

	ptPotal = ConvertToServer(moveX, moveY);		// 捞亥飘 谅钎绰 努扼捞攫飘 扁霖捞骨肺...

	if( ptPotal.x == -1 || ptPotal.y == -1 )
	{
		SetZoneIndex(org_z);
		return FALSE;
	}

	i = SEARCH_TOWN_POTAL_COUNT;				// 谅钎甫 茫扁困茄 冉荐
	
	pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);

	if(pt.x == -1 || pt.y == -1) 
	{
		while(TRUE)								// 鸥款器呕 伐待 谅钎甫 掘绰促.
		{
			rand = myrand(-3, 3);
			ptPotal.x += rand; ptPotal.y += rand;
			pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
			if(pt.x != -1 && pt.y != -1) break;
			i--;
			if(i <= 0) // 唱吝俊
			{ 
				pt.x = -1; pt.y = -1;
				break;
			}
		}
	}

	if(pt.x == -1 || pt.y == -1) 
	{
		SetZoneIndex(org_z);
		m_NowZoneChanging = FALSE;
		return FALSE;
	}
	else									// 甸绢哎荐绝绰瘤开捞搁...
	{
		int type = ((g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_dwType & 0xFF00 ) >> 8);
		if(!CheckInvakidZoneChangeState(type)) 
		{
			SetZoneIndex(org_z);
			m_NowZoneChanging = FALSE;
			return FALSE;
		}
	}

	SetZoneIndex(org_z);					// 捞傈 伎 蜡历俊霸  舅赴促.

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 
			(long)0, (long)(m_uid + USER_BAND)) == (long)m_uid + USER_BAND)
	{
//		SetUid(m_curx, m_cury, 0);				
		SendMyInfo(TO_INSIGHT, INFO_DELETE);
		//护法消失
		SendHuFaPostion(HUFA_HIDE);

		SetZoneIndex(new_z);						
		m_curx = pt.x;
		m_cury = pt.y;
		m_curz = new_z;
		SetUid(m_curx, m_cury, m_uid + USER_BAND );// 菊栏肺 捞悼且 磊府甫 犬焊, 困摹沥焊 悸泼

		SendZoneChange(TRUE);	// 粮眉牢瘤甫  刚历焊郴具 唱吝 NPC沥焊啊 client俊辑 混酒乐促.

		m_presx = -1;
		m_presy = -1;
		SightRecalc();
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		
		m_NowZoneChanging = FALSE;

		return TRUE;
	}
	else
	{
		SendMyInfo(TO_INSIGHT, INFO_DELETE);

		SetZoneIndex(new_z);						
		m_curx = pt.x;
		m_cury = pt.y;
		m_curz = new_z;
		SetUid(m_curx, m_cury, m_uid + USER_BAND );

		SendZoneChange(TRUE);

		m_presx = -1;
		m_presy = -1;
		SightRecalc();
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		
		m_NowZoneChanging = FALSE;

		SendHuFaInfo(this,TO_INSIGHT);

		return TRUE;

//		m_NowZoneChanging = FALSE;
//		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////
//	粮眉牢瘤甫 蜡历俊霸 焊辰促.
//
void USER::SendZoneChange(BOOL bResult)
{
	CBufferEx TempBuf;
	BYTE result = FAIL;
	
	TempBuf.Add(ZONE_CHANGE_RESULT);
	
	if(bResult == FALSE)
	{
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());			// 角菩甫 府畔
		return;
	}

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }	// 捞何盒篮 唱吝俊 官操磊

	result = SUCCESS;
	TempBuf.Add(result);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);
	TempBuf.Add((short)m_curz);

	Send(TempBuf, TempBuf.GetLength());

	m_dwNoDamageTime = NO_DAMAGE_TIME;
	m_dwLastNoDamageTime = GetTickCount();
}

/////////////////////////////////////////////////////////////////////////
//	NPC 措拳沥焊甫 焊辰促.
//
void USER::SendNpcSay(CNpc *pNpc, int nChatNum)
{
TRACE( "牛牛DEGBUG NPC SAY %d\n", nChatNum);
	CBufferEx TempBuf;

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)nChatNum);
	TempBuf.Add((BYTE)0x00);		


	Send(TempBuf, TempBuf.GetLength());
}
///////////////PK竞赛对话框//
void USER::SendNpcPKSay(CNpc *pNpc)
{

	CBufferEx TempBuf, TempSayBuf;
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)winName );

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)345);
	TempBuf.Add((BYTE)0x01);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}
///////////////杀人大赛对话框//
void USER::SendNpcShaRenSay(CNpc *pNpc)
{

	CBufferEx TempBuf, TempSayBuf;
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)ShaRenName );

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)5008);
	TempBuf.Add((BYTE)0x02);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}
//////////////////////////////////////////////////////////////////////////////////
//	档框富 捞亥飘甫 角青茄促.
//
void USER::HelpStatusOpenReq()
{
	SendNpcSay(NULL, 7);
}

//void USER::NpcSayOpen()
//{
//	SendNpcSay(NULL, 522);
//}

//////////////////////////////////////////////////////////////////////////////////
//	惑痢努贰胶狼 器牢磐甫 掘绰促.
//
CStore* USER::GetStore(int nStore)
{
	for(int i = 0; i < g_arStore.GetSize(); i++)
	{
		if(g_arStore[i] == NULL) continue;

		if(g_arStore[i]->m_sStoreID == (short)nStore)
		{
//			int temp = g_arStore[i]->m_sRate;
			return g_arStore[i];
		}
	}

	return NULL;	
}

//////////////////////////////////////////////////////////////////////////////////
//	夸货努贰胶狼 器牢磐甫 掘绰促.
//
CGuildFortress* USER::GetFortress(int nFort)
{
	for(int i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(g_arGuildFortress[i] == NULL) continue;

		if(g_arGuildFortress[i]->m_sFortressID == (short)nFort)
		{
			return g_arGuildFortress[i];
		}
	}

	return NULL;	
}

//////////////////////////////////////////////////////////////////////////////////
//	惑痢努贰胶狼 器牢磐甫 掘绰促.
//
CStore* USER::GetPsiStore(int nStore)
{
	for(int i = 0; i < g_arPsiStore.GetSize(); i++)
	{
		if(g_arPsiStore[i] == NULL) continue;

		if(g_arPsiStore[i]->m_sStoreID == (short)nStore)
		{
			return g_arPsiStore[i];
		}
	}

	return NULL;	
}

//////////////////////////////////////////////////////////////////////////////////
//	惑痢努贰胶狼 器牢磐甫 掘绰促.
//
CStore* USER::GetEbodyStore(int nStore)
{
	for(int i = 0; i < g_arEBodyStore.GetSize(); i++)
	{
		if(g_arEBodyStore[i] == NULL) continue;

		if(g_arEBodyStore[i]->m_sStoreID == (short)nStore)
		{
			return g_arEBodyStore[i];
		}
	}

	return NULL;	
}
//////////////////////////////////////////////////////////////////////////////////
//	惑痢阑 坷锹茄促.
//
void USER::SendStoreOpen(int nStore, int nRate)
{
	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg( IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);
		return;
	}

	CStore* pStore = NULL;

	pStore = GetStore(nStore);
	if(pStore == NULL) return;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Test Code
//	if(pStore->m_iGuildSid <= 0) pStore->m_sRate = (short)nRate;
	
	CBufferEx TempBuf;
	int i = 0;
	short sid = 0;
	DWORD dwCost = 0;

	TempBuf.Add(STORE_OPEN);
	TempBuf.Add((short)nStore);
	TempBuf.Add((short)pStore->m_sRate);
	TempBuf.Add((short)(pStore->m_arItems.GetSize()));

	for(i = 0; i < pStore->m_arItems.GetSize(); i++)
	{
		sid = (short)pStore->m_arItems[i];
									// 鞘靛 惑牢牢瘤绰 NPC啊 家加登绢 乐栏骨肺 魄窜
		if(pStore->m_arNpcList.GetSize() > 0)
		{
			int iCost = (int)(g_arItemTable[sid]->m_iDN * 0.9);
			dwCost = iCost + (int)( iCost * ((double)(pStore->m_sRate)/100) );
		}else if (pStore->m_sStoreID == 32 || pStore->m_sStoreID == 33)
		{
			int iCost = (int)(g_arItemTable[sid]->m_iDN); //关闭JZB商店和机甲商店市民等级价格优惠//显示价格
			dwCost = iCost + (int)( iCost * ((double)(pStore->m_sRate)/100) );
		}
		else dwCost = GetItemCostByCityRank(sid, pStore->m_sRate);

		TempBuf.Add(sid);
		TempBuf.Add(dwCost);
	}

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	辨靛啊 加茄 惑痢牢瘤 魄窜秦辑 惑痢阑 坷锹茄促.
//
void USER::SendFieldStoreOpen(int nStore, int nHave, int nEmpty)
{
	int nChatNum = -1;
	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;

	if(nStore >= FORTRESS_BAND)						// 夸货俊 加茄 惑痢捞搁...
	{
		pFort = GetFortress(nStore);
		if(pFort == NULL) return;

		if(pFort->m_lUsed == 1) 
		{
			SendSystemMsg(IDS_USER_CANT_USE_IN_GUILD_WAR, SYSTEM_NORMAL, TO_ME);
			return;
		}

		nChatNum = nEmpty;

		if(pFort->m_bHaveGuild)
		{
			if(pFort->m_iGuildSid > 0) nChatNum = nHave;
		}

		EditHyperFortressText(pFort, nChatNum);
	}
	else											// 酒聪搁 鞘靛俊 加茄 惑痢
	{
		pStore = GetStore(nStore);
		if(pStore == NULL) return;

		if(pStore->m_lUsed == 1) 
		{
			SendSystemMsg( IDS_USER_CANT_USE_IN_GUILD_WAR, SYSTEM_NORMAL, TO_ME);
			return;
		}

		nChatNum = nEmpty;

		if(pStore->m_bHaveGuild)
		{
			if(pStore->m_iGuildSid > 0) nChatNum = nHave;
		}
	
		EditHyperText(pStore, nChatNum);
	}
}

/////////////////////////////////////////////////////////////////////////
//	秦寸 SAY啊 惑痢俊 加茄 啊函利牢 巴牢瘤 魄窜窍咯 函拳矫挪促.
//
void USER::EditHyperText(CStore *pStore, int nNum)
{
	CString temp;

	int i, j, talknum = -1;
	BYTE tCount = 0;

	CHyper hyperText;
	
	if(!pStore) return;
	if(nNum < 0) return;

	CBufferEx TempBuf, TempSayBuf; 

	for(i = 0; i < g_arNpcChat.GetSize(); i++)
	{
		if(g_arNpcChat[i]->m_sCid == nNum)
		{
			talknum = i;
			break;
		}
	}

	if(talknum < 0) return;

	hyperText.GetHyperText(g_arNpcChat[talknum]->m_strTalk, g_arNpcChat[talknum]->m_sSize);
	hyperText.load();

	for(i = 0; i < hyperText.m_nCountLine; i++)
	{
		for(j = 0; j < hyperText.m_HyperText[i].m_strText.GetLength()-7; j++){

			temp = hyperText.m_HyperText[i].m_strText.Mid(j, 2);
			if(temp == "%S")
			{
				if("GM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))		// 辨靛 付胶磐捞抚阑 持绰促.
				{	
					tCount++;
					TempSayBuf.AddString(pStore->m_strMasterName);
//					hyperText.m_HyperText[i].m_strText.Replace("%SGM%", pStore->m_strMasterName);
				}
				else if("GN" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 辨靛 捞抚阑 持绰促.
				{
//					hyperText.m_HyperText[i].m_strText.Replace("%SGN%", pStore->m_strGuildName);
					tCount++;
					TempSayBuf.AddString(pStore->m_strGuildName);
				}
				else if("GL" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pStore->m_wPlanWarTime.wMonth);
//					hyperText.m_HyperText[i].m_strText.Replace("%SGL%", temp);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				else if("GD" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pStore->m_wPlanWarTime.wDay);
//					hyperText.m_HyperText[i].m_strText.Replace("%SGD%", temp);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				else if("GT" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pStore->m_wPlanWarTime.wHour);
//					hyperText.m_HyperText[i].m_strText.Replace("%SGT%", temp);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
/*				else if("STM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					if(iGHWarNum < 0 || iGHWarNum >= g_arGuildHouseWar.GetSize()) return;
					
					nTime = (int)g_arGuildHouseWar[iGHWarNum]->m_CurrentGuild.dwTimer/1000;
					temp.Format("%d", nTime);
					hyperText.m_HyperText[i].m_strText.Replace("%STM%", temp);
				}
*/			}		
		}
		
	}

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)nNum);
	TempBuf.Add(tCount);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());

	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////
//	秦寸 SAY啊 傍己傈俊 加茄 啊函利牢 巴牢瘤 魄窜窍咯 函拳矫挪促.
//
void USER::EditHyperFortressText(CGuildFortress *pFort, int nNum)
{
	CString temp;

	int i, j, talknum = -1;
	BYTE tCount = 0;

	CHyper hyperText;
	
	if(!pFort) return;
	if(nNum < 0) return;

	CBufferEx TempBuf, TempSayBuf; 

	for(i = 0; i < g_arNpcChat.GetSize(); i++)
	{
		if(g_arNpcChat[i]->m_sCid == nNum)
		{
			talknum = i;
			break;
		}
	}

	if(talknum < 0) return;

	hyperText.GetHyperText(g_arNpcChat[talknum]->m_strTalk, g_arNpcChat[talknum]->m_sSize);
	hyperText.load();

	for(i = 0; i < hyperText.m_nCountLine; i++)
	{
		for(j = 0; j < hyperText.m_HyperText[i].m_strText.GetLength()-7; j++){

			temp = hyperText.m_HyperText[i].m_strText.Mid(j, 2);
			if(temp == "%S")
			{
				if("GM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))		// 辨靛 付胶磐捞抚阑 持绰促.
				{	
					tCount++;
					TempSayBuf.AddString(pFort->m_strMasterName);
				}
				else if("GN" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	
				{
					tCount++;
					TempSayBuf.AddString(pFort->m_strGuildName);
				}
				else if("GL" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pFort->m_wPlanWarTime.wMonth);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				else if("GD" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pFort->m_wPlanWarTime.wDay);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				else if("GT" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pFort->m_wPlanWarTime.wHour);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				else if("GG" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pFort->m_dwRepairCost);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}

/*				else if("STM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					if(iGHWarNum < 0 || iGHWarNum >= g_arGuildHouseWar.GetSize()) return;
					
					nTime = (int)g_arGuildHouseWar[iGHWarNum]->m_CurrentGuild.dwTimer/1000;
					temp.Format("%d", nTime);
					hyperText.m_HyperText[i].m_strText.Replace("%STM%", temp);
				}
*/			}		
		}
		
	}

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)nNum);
	TempBuf.Add(tCount);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());

	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 惑痢狼 格废阑 焊辰促.
//
void USER::SendPsiStoreOpen(int iStoreID)
{
	CStore* pStore = NULL;

	pStore = GetPsiStore(iStoreID);
	if(pStore == NULL) return;

	CBufferEx TempBuf;

	short	sSid[TOTAL_PSI_NUM];// 烙矫肺 秦寸 荤捞坷葱阑 历厘茄促.
	memset(sSid, -1, sizeof(sSid));

	int i, j;
	int iStart, iEnd;
	int iClass = -1;
	BOOL bflag;	
	short sCount = 0;

	CWordArray arPsi;			
	CByteArray arChangeClass;
	arPsi.RemoveAll();
	arChangeClass.RemoveAll();

	arChangeClass.Add(m_byClass);
	arChangeClass.Add( NOCLASS ); // 公拌凯 荤捞坷葱阑 硅匡 荐 乐霸 茄促

	for(i = 0; i < _CHANGE_DB; i++)// 泅犁, 傈流 努贰胶甫 历厘茄促.
	{
		if(m_sChangeClass[i] > 0 && i != m_byClass) arChangeClass.Add(i);
	}

	iStart = arChangeClass.GetSize();

	for(i = 0; i < arChangeClass.GetSize(); i++)
	{
		iStart = pStore->IsCanBuyPsi(pStore, (int)arChangeClass[i]);

		if(iStart >= 0)			// 惑痢俊辑 秒鞭窍绰 格废吝 劳鳃荐 乐绰巴父 急喊
		{
			switch(arChangeClass[i])			
			{
			case BRAWL:			// 秦寸 努贰胶 肚绰 傈流 努贰胶俊 秦寸窍绰 荤捞坷葱 格废甫 历厘
				iEnd = iStart + BRAWL_PSI_NUM;
				break;
			case STAFF:			// 父距俊 捞固 嚼垫茄 荤捞坷葱捞 乐绰瘤 八荤
				iEnd = iStart + STAFF_PSI_NUM;	
				break;
			case EDGED:			// 乐促搁 捞固 嚼垫茄 荤捞坷葱篮 力寇
				iEnd = iStart + EDGED_PSI_NUM;	
				break;
			case FIREARMS:
				iEnd = iStart + FIREARMS_PSI_NUM;
				break;
			case NOCLASS:
				iEnd = iStart + NOCLASS_PSI_NUM;
				break;
			}

			for(j = iStart;  j < iEnd; j++) 
			{ 
				sSid[sCount] = (short)j; sCount++;
			}
			
		}
	}

	for(i = 0; i < sCount; i++)	// 劳鳃荐 乐绰 格废父 急喊 历厘茄促.
	{
		bflag = TRUE;
		for(j = 0; j < m_nHavePsiNum; j++) if(m_UserPsi[j].sSid == sSid[i]) { bflag = FALSE; break; }
		if(bflag) arPsi.Add(sSid[i]);
	}
/*
	for(i = 0; i < sCount; i++)	// 劳鳃荐 乐绰 格废父 急喊 历厘茄促.
	{
		bflag = TRUE;
		for(j = 0; j < m_nHavePsiNum; j++) if(m_UserPsi[j].sSid == sSid[i]) { bflag = FALSE; break; }
		if(bflag) arPsi.Add(sSid[i]);
	}
*/
	sCount = arPsi.GetSize();
	if(sCount == 0) 
	{
		arChangeClass.RemoveAll();
		arPsi.RemoveAll();
		SendNpcSay( NULL, 4 );		// say锅龋 4绰 "歹捞惑 硅匡荐 绝促"绰 皋技瘤烙
		return;
	}

	TempBuf.Add(BUY_PSY_OPEN_RESULT);
	TempBuf.Add((short)iStoreID);
	TempBuf.Add(sCount);

	for(i = 0; i < sCount; i++) TempBuf.Add(arPsi[i]);

	Send(TempBuf, TempBuf.GetLength());

	arChangeClass.RemoveAll();
	arPsi.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱甫 备涝茄促.
//
void USER::BuyPsi(TCHAR *pBuf)
{
	int i, j;
	int index = 0;
	int iClass = 0;

	BOOL bSame;
	BYTE tError = 1;	
	BYTE tResult = SUCCESS;

	short sStoreID;			// 惑痢 酒捞叼
	short sPsiCount;		// 醚 Psi 荐
	short sSid;

	DWORD dwDN = m_dwDN;
	DWORD dwXP = m_dwXP;

	CBufferEx TempBuf;
	CWordArray	arPsi;
	CByteArray arChangeClass;

	arChangeClass.RemoveAll();
	arPsi.RemoveAll();

	CStore* pStore = NULL;

	sStoreID = GetShort(pBuf, index);

	pStore = GetPsiStore(sStoreID);
	if(pStore == NULL) { tResult = FAIL; goto go_result; }

	sPsiCount = GetShort(pBuf, index);

	if(sPsiCount <= 0 || sPsiCount >= g_arPsiTable.GetSize()) return;

	arChangeClass.Add(m_byClass);
	arChangeClass.Add( NOCLASS );			//	傈流苞 惑包绝捞 公拌凯 荤捞坷葱阑 硅匡 荐 乐霸 秦霖促
	for(i = 0; i < _CHANGE_DB; i++)			//	傈流茄 版快客 贸澜牢 版快 惑痢俊辑 秒鞭茄 努贰胶客 鞍篮瘤 		
	{										//	舅酒具茄促.
		if(m_sChangeClass[i] > 0 && i != m_byClass) arChangeClass.Add(i);
	}
											
	for(i = 0; i < sPsiCount; i++)
	{
		bSame = FALSE;
		sSid = GetShort(pBuf, index);
		/****************************** 抗寇 贸府 *****************************/
		if(sSid < 0 || sSid >= g_arPsiTable.GetSize()) { tResult = FAIL; tError = 05; goto go_result; }

											//	捞固 嚼垫茄 荤捞坷葱篮 角菩
		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == sSid) { tResult = FAIL; tError = 06; goto go_result; }
		}

		iClass = CheckPsiClass(sSid);
		if(iClass < 0) { tResult = FAIL; goto go_result; }
											//	嚼垫 阂啊瓷茄 拌凯狼 荤捞坷葱档 角菩
		for(j = 0;  j < arChangeClass.GetSize(); j++)
		{
			if(arChangeClass[j] == iClass) bSame = TRUE;
		}									//	惑痢 荤捞坷葱捞 磊扁 努贰胶客 嘎绰霸 绝促.
		if(bSame == FALSE) { tResult = FAIL; tError = 01; goto go_result; }
											//	备涝饭骇捞 臭酒档 角菩
		if(g_arPsiTable[sSid]->m_tNeedLevel > m_sLevel) { tResult = FAIL; tError = 02; goto go_result; }
											//	XP啊 葛磊扼档 角菩
		if(g_arPsiTable[sSid]->m_iNeedXP > dwXP) { tResult = FAIL; tError = 03; goto go_result; }
											//	DN捞 葛磊扼档 角菩
		if(g_arPsiTable[sSid]->m_iNeedDN > dwDN) { tResult = FAIL; tError = 04; goto go_result; }
											//	弥措蔼阑 逞绢档 角菩
		if(m_nHavePsiNum >= TOTAL_PSI_NUM) { tResult = FAIL; tError = 05; goto go_result; }
		
		dwDN = dwDN - g_arPsiTable[sSid]->m_iNeedDN;
		dwXP = dwXP - g_arPsiTable[sSid]->m_iNeedXP;
		arPsi.Add(sSid);
	}

	/*************************** User Psi 殿废****************************/
	for(i = 0; i < arPsi.GetSize(); i++)
	{
		sSid = arPsi[i];
		m_UserPsi[m_nHavePsiNum].sSid = sSid;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
	}

	m_dwDN = dwDN; m_dwXP = dwXP; 
//	m_nHavePsiNum = m_nHavePsiNum + sPsiCount;

	/******************************** DB 殿废****************************/
//	InterlockedIncrement(&m_lRequireNum);		// DB俊 历厘且 扁雀啊 吭促. 
/*	if(UpdateUserData() == FALSE)
	{
		m_nHavePsiNum = m_nHavePsiNum - sPsiCount;

		for(i = m_nHavePsiNum; i < (m_nHavePsiNum + sPsiCount); i++)
		{
			m_UserPsi[i].sSid = -1;
			m_UserPsi[i].tOnOff = FALSE;
		}
		CheckMaxValue((DWORD &)m_dwDN, (DWORD)dwDN);
		CheckMaxValue((DWORD &)m_dwXP, (DWORD)dwXP);
		tResult = FAIL;
	}
*/
go_result:
	TempBuf.Add(BUY_PSY_RESULT);
	TempBuf.Add(tResult);

	if(tResult == FAIL) 
	{ 
		TempBuf.Add(tError);
		Send(TempBuf, TempBuf.GetLength()); 
		return; 
	}

	sPsiCount = arPsi.GetSize();

	TempBuf.Add(sPsiCount);

	for(i = 0; i < sPsiCount; i++)
	{
		sSid = (short)arPsi[i];
		TempBuf.Add(sSid);
	}

	SendXP();
	SendMoneyChanged();
	Send(TempBuf, TempBuf.GetLength());

	arPsi.RemoveAll();
	arChangeClass.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////
//	蜡历 巩狼茄 Psi serial id俊 秦寸窍绰 努贰胶甫 府畔茄促. 
//
int USER::CheckPsiClass(short sSid)
{
	if(sSid < BRAWL_PSI_NUM) return BRAWL;
	else if(sSid < BRAWL_PSI_NUM + STAFF_PSI_NUM) return STAFF;
	else if(sSid < BRAWL_PSI_NUM + STAFF_PSI_NUM + EDGED_PSI_NUM) return EDGED;
	else if(sSid < BRAWL_PSI_NUM + STAFF_PSI_NUM + EDGED_PSI_NUM + FIREARMS_PSI_NUM) return FIREARMS;
	else if(sSid < TOTAL_PSI_NUM) return NOCLASS;
	else return -1;
}

////////////////////////////////////////////////////////////////////////////////////
//	牢亥吝 后 浇吩阑 茫酒辑 措涝, 绝栏搁 -1 
//
int USER::GetEmptyInvenSlot(int *pEmpty)
{
	int i, j = 0;

	for(i = EQUIP_ITEM_NUM; i < TOTAL_INVEN_MAX; i++)
	{
		if(m_UserItem[i].sSid == -1) 
		{
			pEmpty[j] = i;
			j++;
		}
	}

	return j;
}

////////////////////////////////////////////////////////////////////////////////////
//	货肺款 酒捞袍甸阑 掘阑锭...(抗: 甘俊辑 酒捞袍阑 林匡锭, 各俊霸辑 掘阑锭, 惑痢俊辑 混锭)   
//
void USER::UpdateInvenSlot(CWordArray *arEmptySlot,CWordArray *arSameSlot, BYTE tType)
{
	int i, j;
	int iEmpty = 0;
	int iSame = 0;
	CBufferEx TempBuf;

	if(arEmptySlot != NULL) iEmpty = arEmptySlot->GetSize();
	if(arSameSlot != NULL) iSame = arSameSlot->GetSize();

	BYTE bySlot = 0;
	BYTE iCount = (BYTE)(iEmpty + iSame); 

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(tType);

	TempBuf.Add(iCount);

	for(i = 0; i < iEmpty; i++)
	{
		bySlot = (BYTE)arEmptySlot->GetAt(i);
		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}

	for(i = 0; i < iSame; i++)
	{
		bySlot = (BYTE)arSameSlot->GetAt(i);
		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	
	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////
//	家瘤陛捞 函版夌阑 版快 烹焊
//
void USER::SendMoneyChanged()
{
	CBufferEx TempBuf;

	TempBuf.Add(MONEY_CHANGED);
	TempBuf.Add((BYTE)0x01);
	TempBuf.Add(m_dwDN);

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	惑痢俊辑 酒捞袍阑 备涝茄促.
//
void USER::BuyItem(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if( m_iDisplayType == 6) return; //yskang 0.5
#endif

	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg(IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);
		return;
	}

	CStore* pStore = NULL;
	ItemListArray	arEmptyBuy, arSameBuy;
	CWordArray		arEmptySlot, arSameSlot;
	ItemList		MyItem[TOTAL_ITEM_NUM], SameItem[TOTAL_ITEM_NUM];
	int				pTotalEmpty[INVENTORY_NUM];

	int index = 0;
	int i, j, k; 
	short sid = -1, num = -1; 
	int iSlot, iEmptyNum = 0;
	int iWeight = 0;

	short sStoreID = GetShort(pBuf, index);
	short sCount = GetShort(pBuf, index);

	if(sStoreID < 0 || sCount <= 0 || sCount >= _MAX_SHORT) return;

	pStore = GetStore(sStoreID);
	if(pStore == NULL) return;

	if(sStoreID == 50)
	{
		for(int sc = 0; sc < sCount; sc++)
		{
			short ssid = GetShort(pBuf, index); //物品ID
			short nnum = GetShort(pBuf, index); //数量
			short sprice = -1;
			short spnum = -1;

			/*     int price;
		         DWORD jianchaqian = 0;
		         price =(DWORD)g_arOnlineShopTable[j]->m_price;
		         jianchaqian += ((price/100000000) * nnum);*/

			for(int i = 0; i < g_arOnlineShopTable.GetSize();i++)
			{
				 if(ssid == g_arOnlineShopTable[i]->m_iSid)
				{										
					sprice = g_arOnlineShopTable[i]->m_price;
					spnum = g_arOnlineShopTable[i]->m_iNum;
					break;
				}
			}
			if(ssid == -1 || /*nnum == -1 ||*/ sprice == -1 /*|| spnum == -1*/) return;
			if( nnum < 0 ) { nnum = 0;return;}  //封刷元宝BUG
            if( spnum < 0 ) { spnum = 0;return;}  //封刷元宝BUG
			if( nnum > 999 ) { nnum = 0;return;}  //封刷元宝BUG
		//	if(jianchaqian > 20) { return;}//买物品价格不能超过20E

			if(((int)m_dwShopPingDN - (sprice*nnum)) < 0)
			{		
			//	SendSystemMsg("您的元宝不足,购物失败!",SYSTEM_ERROR,TO_ME);
				SendEventMsg("您的元宝不足,购物失败!");
				return;
			}
			BuyOnlineShopItem(ssid,nnum);
		}
	}
	else
	{
	// 捞亥飘 困摹甫 八荤 -----------------------------------------//
	CPoint pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, BUY_BLOCK)) return;

	if(sCount > 255) return;				// 茄锅俊 混荐乐绰 弥措樊甫 碍力肺 沥茄促.(MAX_BYTE)

	for(i = 0; i < TOTAL_ITEM_NUM; i++)		
	{
		MyItem[i] = m_UserItem[i];
		SameItem[i] = m_UserItem[i];
	}

	// 函荐 檬扁拳 ------------------------------------------------//
	BOOL bFlag = FALSE;
	DWORD dwCost = 0;
	short sRate = pStore->m_sRate;

	DWORD dwTemp = m_dwDN;
//	double TempCost;
	DWORD dwTax = 0, dwBasicTax = 0;

	arSameSlot.RemoveAll();
	arSameBuy.RemoveAll();
	arEmptyBuy.RemoveAll();
	arEmptySlot.RemoveAll();

	ItemList	*pBuyItem = NULL;
	::ZeroMemory(pTotalEmpty, sizeof(pTotalEmpty));

	GetEmptyInvenSlot(pTotalEmpty);

	int iItemSize = g_arItemTable.GetSize();
	// 备涝格废 ---------------------------------------------------//
	for(i = 0; i < sCount; i++)
	{
		iSlot = -1;
		sid = GetShort(pBuf, index);
		num = GetShort(pBuf, index);
        if (num>10 && (sStoreID == 33))
		{
				SendEventMsg("每件商品一次最多只能买10个！");// "你没有足够的金钱(s)."
				goto go_result;
		}
	//	 if (num>100 && (sStoreID == 27))
	//	{
	//			SendEventMsg("每件商品一次最多只能买100个！");// "你没有足够的金钱(s)."
	//			goto go_result;
	//	}
		  if (num>999 && (sStoreID == 28))
		{
				SendEventMsg("每件商品一次最多只能买999个！");// "你没有足够的金钱(s)."
				goto go_result;
		}
		if (num>999 && (sStoreID == 1001))
		{
				SendEventMsg("每件商品一次最多只能买999个！");// "你没有足够的金钱(s)."
			   goto go_result;
		}
		if (num>999 && (sStoreID == 23))
		{
				SendEventMsg("每件商品一次最多只能买999个！");// "你没有足够的金钱(s)."
			    goto go_result;
				
		}
		if (num>10 && (sStoreID == 32))
		{
				SendEventMsg("机甲商品一次最多只能买10个！");// "你没有足够的金钱(s)."
			    goto go_result;
				
		}
		if(iEmptyNum > INVENTORY_NUM) { bFlag = TRUE; goto go_result; }
		if(sid < 0 || num <= 0 || sid > iItemSize || num >= _MAX_SHORT) { bFlag = TRUE; goto go_result; }

		if(!pStore->IsCanBuy(sid)) { bFlag = TRUE; goto go_result; }
		if( num < 0 ) { num = 0;return;}  //封刷钱BUG
//		if(g_arItemTable[sid]->m_byWear <= 5 || g_arItemTable[sid]->m_byWear == 15)	// 汗鼻 酒捞袍
		if(g_arItemTable[sid]->m_sDuration > 0)
		{
			for(j = 0; j < num; j++)
			{
				iSlot = pTotalEmpty[iEmptyNum];
				if(iSlot == 0) { bFlag = TRUE; goto go_result; }

				pBuyItem = NULL;
				pBuyItem = new ItemList;
				if(pBuyItem == NULL) { bFlag = TRUE; goto go_result; }

				ReSetItemSlot(pBuyItem);

				pBuyItem->sLevel = g_arItemTable[sid]->m_byRLevel;
				pBuyItem->sSid = sid;
				pBuyItem->sCount = 1;
				pBuyItem->sDuration = g_arItemTable[sid]->m_sDuration;
				pBuyItem->sBullNum = g_arItemTable[sid]->m_sBullNum;
				for(k =0; k< MAGIC_NUM; k++) pBuyItem->tMagic[k] = 0;
				pBuyItem->tIQ = 0;
				pBuyItem->iItemSerial = 0;

				arEmptyBuy.Add(pBuyItem);
				arEmptySlot.Add(iSlot);
				iEmptyNum++; pBuyItem = NULL;

				if(iEmptyNum > INVENTORY_NUM) { bFlag = TRUE; goto go_result; }
			}
		}
		else 
		{
//			if(num > MAX_ITEM_NUM) num = MAX_ITEM_NUM;
			pBuyItem = NULL;
			pBuyItem = new ItemList;
			if(pBuyItem == NULL) { bFlag = TRUE; goto go_result; }

			ReSetItemSlot(pBuyItem);

			pBuyItem->sLevel = g_arItemTable[sid]->m_byRLevel;
			pBuyItem->sSid = sid;
			pBuyItem->sCount = num;
			pBuyItem->sDuration = g_arItemTable[sid]->m_sDuration;
			pBuyItem->sBullNum = g_arItemTable[sid]->m_sBullNum;
			for(k =0; k< MAGIC_NUM; k++) pBuyItem->tMagic[k] = 0;
			pBuyItem->tIQ = 0;
			pBuyItem->iItemSerial = 0;

			iSlot = GetSameItem(*pBuyItem, INVENTORY_SLOT);
			if(iSlot != -1)	
			{ 
/*				if(SameItem[iSlot].sCount + num > MAX_ITEM_NUM)
				{
					num = MAX_ITEM_NUM - SameItem[iSlot].sCount;
					SameItem[iSlot].sCount = MAX_ITEM_NUM;
					if(num < 0) num = 0;
				}
*/				if(num != 0)
				{
					pBuyItem->sCount = num;
					arSameBuy.Add(pBuyItem); 
					arSameSlot.Add(iSlot); 
				}
			}
			else			
			{
				iSlot = pTotalEmpty[iEmptyNum];
				if(iSlot == 0) { bFlag = TRUE; goto go_result; }
				arEmptyBuy.Add(pBuyItem); 
				arEmptySlot.Add(iSlot); 
				iEmptyNum++;
			}			
		}

		iWeight += g_arItemTable[sid]->m_byWeight * num;

		if(pStore->m_arNpcList.GetSize() > 0)
		{
			int iCost = (int)(g_arItemTable[sid]->m_iDN * 0.9);
//			iCost += (int)( iCost * ((double)sRate/100) );

			dwCost += num * iCost;
		}else if (pStore->m_sStoreID == 32 || pStore->m_sStoreID == 33)
			{
				int iCost = (int)(g_arItemTable[sid]->m_iDN); //关闭JZB商店和机甲商店市民等级价格优惠//实际价格
				dwCost += num * iCost;
			}
		else dwCost += num * GetItemCostByCityRank(sid, 0);//sRate);
	}

	// 备涝厚侩 拌魂 棺 家瘤陛 八荤 -------------------------------//
	dwTax = (DWORD)( dwCost * ((double)sRate/100) );
	dwCost = (DWORD)(dwCost + dwTax + 0.999);			// 棵覆(DWORD 肺 父甸扁 困秦辑)
//	TempCost = (double)dwCost;
//	TempCost = TempCost + TempCost * (double)sRate / 100.0;			// 矫技利侩
//	dwCost = (DWORD)(TempCost + 0.999);			// 棵覆(DWORD 肺 父甸扁 困秦辑)

	
	if(dwCost > dwTemp)		// 捣捞 何练窍搁
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_DINA1, SYSTEM_ERROR, TO_ME);
		goto go_result;
	}
    
    if(m_iMaxWeight < m_iCurWeight + iWeight)
	{
		SendSystemMsg( IDS_USER_OVER_WEIGHT1, SYSTEM_ERROR, TO_ME);
		goto go_result;
	}

	// 备涝格废阑 牢亥俊 -------------------- --------------------//
	for(i = 0; i < arSameBuy.GetSize(); i++)
	{
		CheckMaxValue((short &)m_UserItem[arSameSlot[i]].sCount, (short)arSameBuy[i]->sCount); 
	}

	for(i = 0; i < arEmptyBuy.GetSize(); i++)
	{
		m_UserItem[arEmptySlot[i]].sLevel = arEmptyBuy[i]->sLevel;
		m_UserItem[arEmptySlot[i]].sSid = arEmptyBuy[i]->sSid;
		m_UserItem[arEmptySlot[i]].sCount = arEmptyBuy[i]->sCount;
		m_UserItem[arEmptySlot[i]].sBullNum = arEmptyBuy[i]->sBullNum;
		m_UserItem[arEmptySlot[i]].sDuration = arEmptyBuy[i]->sDuration;
		m_UserItem[arEmptySlot[i]].tIQ = arEmptyBuy[i]->tIQ;
		for(j =0; j < MAGIC_NUM; j++) m_UserItem[arEmptySlot[i]].tMagic[j] = arEmptyBuy[i]->tMagic[j];
		m_UserItem[arEmptySlot[i]].iItemSerial = arEmptyBuy[i]->iItemSerial;

		MakeItemLog( &(m_UserItem[arEmptySlot[i]]), ITEMLOG_BUY );
	}

	if(dwTemp <= dwCost) m_dwDN = 0;
	else m_dwDN = dwTemp - dwCost;					// 家瘤陛 函拳

	MakeMoneyLog( dwCost, ITEMLOG_BUY_MONEY );

	if(UpdateUserItemDN() == FALSE)
	{
		m_dwDN = dwTemp;						// 家瘤陛 券盔
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// 酒捞袍 沥焊 券盔
		{
			m_UserItem[i] = MyItem[i];
		}
			SendSystemMsg( IDS_USER_BUY_FAIL, SYSTEM_ERROR, TO_ME);
		FlushItemLog( FALSE );
		goto go_result;
	}

	if(dwTax < dwCost && pStore->m_bHaveGuild) 
	{
		pStore->m_iGuildDN += dwTax;	// 鞘靛 惑痢捞搁 辨靛芒绊 利赋
		UpdateMemStoreDN(sStoreID, pStore->m_iGuildDN);
	}
	else
	{
		UpdateFortressTax(sStoreID, dwTax); // 器飘府胶 技陛捞搁 历厘...
	}

	m_iCurWeight += iWeight;
	GetRecoverySpeed();

	UpdateInvenSlot(&arEmptySlot, &arSameSlot);
	SendMoneyChanged();

	FlushItemLog( TRUE );

go_result:
	if(bFlag) SendSystemMsg( IDS_USER_FULL_INVEN, SYSTEM_ERROR, TO_ME);

	// 皋葛府 秦力 -------------------- --------------------//
	for(i = 0; i < arSameBuy.GetSize(); i++)
	{
		if(arSameBuy[i] != NULL) delete arSameBuy[i];
	}
	arSameBuy.RemoveAll();
	arSameSlot.RemoveAll();
	for(i = 0; i < arEmptyBuy.GetSize(); i++)
	{
		if(arEmptyBuy[i] != NULL) delete arEmptyBuy[i];
	}
	arEmptyBuy.RemoveAll();
	arEmptySlot.RemoveAll();
  }
}
////////////////////////////////////////////////////////////////////////////////////
//	器飘府胶俊 家加等 惑痢捞搁 技陛阑 拌魂秦辑 利赋 
//
void USER::UpdateFortressTax(int iStoreID, DWORD dwCost)
{
	int i, j;

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i]) continue;

		for(j = 0; j < g_arGuildFortress[i]->arStoreTax.GetSize(); j++)
		{
			if(g_arGuildFortress[i]->arStoreTax[j] == iStoreID)
			{
				g_arGuildFortress[i]->m_iGuildDN += dwCost; 
//				InterlockedExchange(&g_arGuildFortress[i]->m_iGuildDN, (long)dwCost);
				UpdateMemStoreDN(g_arGuildFortress[i]->m_sFortressID, g_arGuildFortress[i]->m_iGuildDN);
				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	惑痢俊 迫 荐 乐绰 酒捞袍 格废苞 啊拜阑 焊郴霖促.
//
void USER::SellItemOpenReq(int sStoreID)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg(IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);
		return;
	}

	int i;
	int index = 0;
	int sipai = 0;
	int baigao = 0;
	int magic1 = 0;
	int dynamic1 = 0;
	int magic2 = 0;
	int dynamic2 = 0;
	int magic3 = 0;
	int dynamic3 = 0;
	int magic4 = 0;
	int dynamic4 = 0;
	int iStart = EQUIP_ITEM_NUM;
	int iEnd = iStart + INVENTORY_NUM;

	CStore* pStore = GetStore(sStoreID);
	if(pStore == NULL) return;

	CBufferEx CostBuf;
	short sSlot = 0;
	DWORD dwSellCost = 0;
	short sCount = 0;
	if(sStoreID == 30)//四排和白锆自动回收
	{
		for(sSlot = 10; sSlot < 34; sSlot++)
		{
			magic1 = m_UserItem[sSlot].tMagic[0];
			if(magic1 < 0 || magic1 >= g_arMagicItemTable.GetSize()) continue;
			dynamic1 = g_arMagicItemTable[magic1]->m_sSubType;
			magic2 = m_UserItem[sSlot].tMagic[1];
			if(magic2 < 0 || magic2 >= g_arMagicItemTable.GetSize()) continue;
			dynamic2 = g_arMagicItemTable[magic2]->m_sSubType;
			magic3 = m_UserItem[sSlot].tMagic[2];
			if(magic3 < 0 || magic3 >= g_arMagicItemTable.GetSize()) continue;
			dynamic3 = g_arMagicItemTable[magic3]->m_sSubType;
			magic4 = m_UserItem[sSlot].tMagic[3];
			if(magic4 < 0 || magic4 >= g_arMagicItemTable.GetSize()) continue;
			dynamic4 = g_arMagicItemTable[magic4]->m_sSubType;

			if(m_UserItem[sSlot].tMagic[0] != NULL && m_UserItem[sSlot].tMagic[1] != NULL && m_UserItem[sSlot].tMagic[2] != NULL && m_UserItem[sSlot].tMagic[3] != NULL && m_UserItem[sSlot].tMagic[4] == NULL && dynamic1 == dynamic2 && dynamic2 == dynamic3 && dynamic3 == dynamic4)
			{
				dwSellCost = 1500000;//f_Markini[0]->JG_SIPAIJ;
			}
			else if(m_UserItem[sSlot].tMagic[5] == 0 && m_UserItem[sSlot].tIQ == 2 && m_UserItem[sSlot].tMagic[0] == NULL && m_UserItem[sSlot].tMagic[1] == NULL && m_UserItem[sSlot].tMagic[2] == NULL && m_UserItem[sSlot].tMagic[3] == NULL && m_UserItem[sSlot].tMagic[4] == NULL && m_UserItem[sSlot].sSid == 907)
			{
				dwSellCost = 2000000;//f_Markini[0]->JG_BAIGAO;
			}
			else 
			{
				continue;
			}
			if(dwSellCost > 0)
			{
				CostBuf.Add(sSlot);
				CostBuf.Add((short)m_UserItem[sSlot].sDuration);
				CostBuf.Add(dwSellCost);
				sCount++;
			}
		}
	}
	else
	{
	for(i = iStart; i < iEnd; i++)
	{
		if(pStore->IsCanSell(m_UserItem[i].sSid))
		{
			sSlot = (short)i;
			if((m_UserItem[sSlot].tMagic[0] != NULL && m_UserItem[sSlot].tMagic[1] != NULL && m_UserItem[sSlot].tMagic[2] != NULL && m_UserItem[sSlot].tMagic[3] != NULL && m_UserItem[sSlot].tMagic[0] == m_UserItem[sSlot].tMagic[1] && m_UserItem[sSlot].tMagic[1] == m_UserItem[sSlot].tMagic[2] && m_UserItem[sSlot].tMagic[2] == m_UserItem[sSlot].tMagic[3] && m_UserItem[sSlot].tMagic[4] == NULL) || (m_UserItem[sSlot].tMagic[5] == 0 && m_UserItem[sSlot].tIQ == 2 && m_UserItem[sSlot].tMagic[0] == NULL && m_UserItem[sSlot].tMagic[1] == NULL && m_UserItem[sSlot].tMagic[2] == NULL && m_UserItem[sSlot].tMagic[3] == NULL && m_UserItem[sSlot].tMagic[4] == NULL && m_UserItem[sSlot].sSid == 907))
			{
				continue;
			}
			dwSellCost = GetSellCost(sSlot);
			if(dwSellCost > 0)
			{
				CostBuf.Add(sSlot);
				CostBuf.Add((short)m_UserItem[sSlot].sDuration);
				CostBuf.Add(dwSellCost);
				sCount++;
			}
		}
	}
	}
	CBufferEx TempBuf;
	TempBuf.Add(SELL_ITEM_OPEN_RESULT);
	TempBuf.Add((short)sStoreID);
	TempBuf.Add(sCount);
	if(sCount > 0) TempBuf.AddData(CostBuf, CostBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	惑痢俊 迫 啊拜阑 备茄促.
//
DWORD USER::GetSellCost(short sSlot)///出售装备价格
{
	short sSid = m_UserItem[sSlot].sSid;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return (DWORD)0;

	if(m_UserItem[sSlot].tMagic[5]>0) return (DWORD)0;//改数大于0的物品，则不显示到SELL店//不让卖哈

	DWORD	dwDN = g_arItemTable[sSid]->m_iDN;
	BYTE    tClass = g_arItemTable[sSid]->m_byClass;
	BYTE	tIQ = m_UserItem[sSlot].tIQ;
	int		iMagic = 0;
	int		i = 0;

	// 概流 肚绰 饭绢 酒捞袍狼 版快
	if(tIQ == MAGIC_ITEM)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (0.9 + (double)(iMagic * 0.07)));
	}
	if(tIQ == RARE_ITEM)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (1.2 + (double)(iMagic * 0.07)));
	}
	//////////////////////////////////////////////////////////////////////卖装备价格
	/*if(tIQ == NORMAL_ITEM)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (0.2 + (double)(iMagic * 0.07)));
	}*/
	//////////////////////////////////////////////////////////////////////

	short sMaxDuration = g_arItemTable[sSid]->m_sDuration;
	short sCurDuration = m_UserItem[sSlot].sDuration;

	if(sMaxDuration <= 0) { sMaxDuration = sCurDuration = 1; }

	//----------------------------------------------------------------------------------------------------------------
	//--yskang 0.6 蜡丰 荤侩磊 橇府固决 - 亮酒扼 且鳖? 吝绊啊拜阑 臭咯 霖促. 0.15肺 
	double duTempCost =0.0;
	if(m_iDisplayType == 5 || m_iDisplayType == 6)
		duTempCost = dwDN * (0.1 - 0.01 * (( 1 - ((double)sCurDuration / (double)sMaxDuration) ) / 0.1)) + 1;
	else
		duTempCost = dwDN * (0.15 - 0.01 * (( 1 - ((double)sCurDuration / (double)sMaxDuration) ) / 0.1)) + 1;
//	double duTempCost = dwDN * (0.15 - 0.01 * (( 1 - ((double)sCurDuration / (double)sMaxDuration) ) / 0.1)) + 1;
	//----------------------------------------------------------------------------------------------------------------

	if(tClass == FIREARMS && (tIQ - 100) >= 0) duTempCost = duTempCost/2;

	return (DWORD)(duTempCost);
}

//##################################################################################
//	Main UI
//##################################################################################

//////////////////////////////////////////////////////////////////////////////
//	Slot1 苞 Slot2狼 酒捞袍阑 官槽促.
//
void USER::SwapItem(int nSlot1, int nSlot2)
{
	ItemList temp;

	temp = m_UserItem[nSlot1];
	m_UserItem[nSlot1] = m_UserItem[nSlot2];
	m_UserItem[nSlot2] = temp;
}

///////////////////////////////////////////////////////////////////////////////
//	公扁甫 官槽促.
//
void USER::ChangeWeaponReq(TCHAR *pBuf)
{
	int j,i;
	BYTE result = FAIL;
	BYTE tEnable = FALSE;
	BYTE tWeaponClass = 0;

	if(m_UserItem[LEFT_HAND].sSid == -1 && m_UserItem[RIGHT_HAND].sSid == -1) goto send_result;

	SwapItem(RIGHT_HAND, LEFT_HAND);

	if(IsCanUseWeaponSkill(tWeaponClass) == TRUE)
	{
		tEnable = TRUE;
	}
	
	result = SUCCESS;

send_result:

	CBufferEx TempBuf, TempBuf2;
	TempBuf.Add(CHANGE_WEAPON_RESULT);
	TempBuf.Add(result);

	if(result == SUCCESS)
	{
		TempBuf.Add(tEnable);
		
		if(tWeaponClass == 255) tWeaponClass = m_byClass;

		TempBuf.Add(tWeaponClass);
		if(m_byClass == JUDGE)
		{
			//基本技能
			for(int  i = 25;i<29;i++)
			{
				TempBuf.Add(m_UserSkill[i].sSid);
				TempBuf.Add(m_UserSkill[i].tLevel);
			}
			TempBuf.Add((short)m_UserSkill[24].sSid);
			TempBuf.Add(m_UserSkill[24].tLevel);
		}else
		{
			for(int i = tWeaponClass * SKILL_NUM; i < tWeaponClass * SKILL_NUM + SKILL_NUM; i++)
			{
				TempBuf.Add(m_UserSkill[i].sSid);
				TempBuf.Add(m_UserSkill[i].tLevel);
			}
		}
		for(  i=0;i<2;i++)
		{
			TempBuf.Add(m_UserSkill140[i].sSid);
			TempBuf.Add(m_UserSkill140[i].tLevel);
		}

		Send(TempBuf, TempBuf.GetLength());

		TempBuf2.Add(ITEM_MOVE_RESULT);
		TempBuf2.Add((BYTE)0x01);				// 己傍
		TempBuf2.Add((BYTE)0x00);				// 老馆 酒捞袍
		TempBuf2.Add((BYTE)0x02);				// 函拳等 浇吩狼 荐 = 2

		TempBuf2.Add((BYTE)LEFT_HAND);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sLevel);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sSid);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sDuration);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sBullNum);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sCount);
		for(j =0; j < MAGIC_NUM; j++) TempBuf2.Add(m_UserItem[LEFT_HAND].tMagic[j]);
		TempBuf2.Add(m_UserItem[LEFT_HAND].tIQ); 

		TempBuf2.Add((BYTE)RIGHT_HAND);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sLevel);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sSid);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sDuration);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sBullNum);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sCount);
		for(j =0; j < MAGIC_NUM; j++) TempBuf2.Add(m_UserItem[RIGHT_HAND].tMagic[j]);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].tIQ); 
		Send(TempBuf2, TempBuf2.GetLength());		// 公扁 函悼 烹焊

		CheckMagicItemMove();			// 公扁啊 官差搁 概流 加己俊 蝶扼 蔼档 官柴
	}else{
		Send(TempBuf, TempBuf.GetLength());
	}
	

	SendMyWeaponChangeInfo();						
}

///////////////////////////////////////////////////////////////////////////////
//	公扁 函拳甫 郴 林函 荤恩俊霸 舅赴促.
//
void USER::SendMyWeaponChangeInfo()
{
	int i;
	CBufferEx TempBuf;

	TempBuf.Add(CHANGE_ITEM_INFO);
	TempBuf.Add(m_uid + USER_BAND);
	FreedomCB();
	
	for(i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(m_UserItem[i].sSid);
	for(i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) TempBuf.Add(m_UserItem[i].sSid);

	if(m_UserItem[TOTAL_ITEM_NUM-2].sSid!=-1&&m_UserItem[TOTAL_ITEM_NUM-2].sDuration!=0){
		TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]));
		TempBuf.Add((BYTE)0x00);
	}else{
		TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]);
		TempBuf.Add((BYTE)0xff);
	}

	SendInsight(TempBuf, TempBuf.GetLength());
//	SendExactScreen(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	Skill 阑 On/Off 茄促
//
void USER::SkillOnOffReq(TCHAR *pBuf)
{
/*	BYTE result = FAIL;
	int index = 0;
	short sSid = GetShort(pBuf, index);

	if(sSid < 0 || sSid >= TOTAL_SKILL_NUM) goto send_result;
	if(m_UserSkill[sSid].tLevel <= 0) goto send_result;

	if(m_UserSkill[sSid].tOnOff == 0) m_UserSkill[sSid].tOnOff = 1;
	else m_UserSkill[sSid].tOnOff = 0;

	result = SUCCESS;

send_result:

	CBufferEx TempBuf;
	TempBuf.Add(SKILL_ONOFF_RESULT);
	TempBuf.Add(result);

	if(result == SUCCESS)
	{
		TempBuf.Add(sSid);
		TempBuf.Add(m_UserSkill[sSid].tLevel);
		TempBuf.Add(m_UserSkill[sSid].tOnOff);
	}

	Send(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////////
//	Psionic 阑 官槽促.
//
void USER::PsiChangeReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	int i;

	BYTE	tWeaponClass = 0;
	int		bSuccessSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};

	short sNeedPP = 0;

	short sSid = GetShort(pBuf, index);

	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) goto send_result;

//	if(m_UserPsi[sSid].sSid == -1) goto send_result;
	for(i = 0;  i < TOTAL_PSI_NUM; i++) m_UserPsi[i].tOnOff = 0;

	for(i = 0;  i < m_nHavePsiNum; i++)
	{
		if(m_UserPsi[i].sSid == sSid)
		{
			result = SUCCESS;
			m_UserPsi[i].tOnOff = 1;

			IsCanUseWeaponSkill(tWeaponClass);	
			// 咯蜡 PP 眉农 ---------------------------------------------------------//
			sNeedPP = GetNeedPP(bSuccessSkill, tWeaponClass, sSid);
			break;
		}		
	}

send_result:

	CBufferEx TempBuf;
	TempBuf.Add(PSI_CHANGE_RESULT);
	TempBuf.Add(result);

	if(result == SUCCESS)
	{
		TempBuf.Add(sSid);
//		TempBuf.Add((BYTE)sNeedPP);
	}

	Send(TempBuf, TempBuf.GetLength());
}


//////////////////////////////////////////////////////////////////////////////////
//	惑痢俊 荤侩磊 酒捞袍阑 魄促.
//
void USER::SellItem(TCHAR *pBuf)///出售物品
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg( IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);
		return;
	}

	int i;//, j;
	int index = 0;
	int iWeight = 0;
//	int iSlotNum = 0;
	int magic1 = 0;
	int dynamic1 = 0;
	int magic2 = 0;
	int dynamic2 = 0;
	int magic3 = 0;
	int dynamic3 = 0;
	int magic4 = 0;
	int dynamic4 = 0;
	short sSlot, sNum; 
	DWORD dwSellCost, dwResultCost = 0;
	DWORD dwMaxCost = 0, tempCost = 0;

	DWORD dwMyDN = 0;

	short sStoreID = GetShort(pBuf, index);	
	short sCount = GetShort(pBuf, index);	

	BOOL bError = FALSE;
	if (m_bNowTrading == TRUE || m_bPShopOpen == TRUE) 
	   {
            CString strDate ="";
			SYSTEMTIME st;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
			TCHAR m_Log[500];
			sprintf_s(m_Log,"[ %s ]%s %d,%d 物品移动存在非法3 \r\n",strDate,m_strUserID,m_bNowTrading,m_bPShopOpen);
		    WriteUserShopLog(m_Log);
			SendItemMoveFail();
		    return;
	 }           
	CBufferEx	TempBuf;
	ItemList	MyItem[TOTAL_ITEM_NUM];
	CWordArray	arSlot;

	arSlot.RemoveAll();
	CStore* pStore = NULL;

	CPoint pt;

	if(sCount <= 0 || sCount >= _MAX_SHORT) 
	{
		goto go_result;
	}
	pStore = GetStore(sStoreID);
	if(pStore == NULL) 
	{
		SendSystemMsg(IDS_USER_NOT_THIS_STORE_BUYSELL, SYSTEM_ERROR, TO_ME);
		goto go_result;
	}

	// 捞亥飘 困摹甫 八荤 -----------------------------------------//
	pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, SELL_BLOCK)) return;

	dwMaxCost = m_dwDN;
	dwMyDN = m_dwDN;						// 家瘤陛 归诀
	for(i = 0; i < TOTAL_ITEM_NUM; i++)		// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

	for(i = 0; i < sCount; i++)				// 府胶飘 格废俊 蝶扼 
	{
		sSlot = GetShort(pBuf, index);
		sNum = GetShort(pBuf, index);

		if(sSlot < EQUIP_ITEM_NUM || sSlot >= TOTAL_INVEN_MAX) { bError = TRUE; break;}

		if(m_UserItem[sSlot].sSid < 0 || m_UserItem[sSlot].sSid >= g_arItemTable.GetSize())
		{
			bError = TRUE;
			break;
		}

		if(g_arItemTable[m_UserItem[sSlot].sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
		{
			SendSystemMsg(IDS_USER_CANT_SELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);//任务物品
			bError = TRUE;
			break;
		}
		if(m_UserItem[sSlot].tMagic[5] != 0 )
		{
			SendSystemMsg( "强化过的物品无法贩卖.", SYSTEM_ERROR, TO_ME);			
			bError = TRUE;
			break;
		}

		if(m_UserItem[sSlot].sCount < sNum || sNum <= 0) { bError = TRUE; break;	}

//		if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byWear < 6 && sNum >= 2)
		if(g_arItemTable[m_UserItem[sSlot].sSid]->m_sDuration > 0 && sNum >= 2)
		{
			bError = TRUE;
			break;
		}
		//四排,白锆
		if(sStoreID == 30)
		{
			magic1 = m_UserItem[sSlot].tMagic[0];
			if(magic1 < 0 || magic1 >= g_arMagicItemTable.GetSize()) continue;
			dynamic1 = g_arMagicItemTable[magic1]->m_sSubType;
			magic2 = m_UserItem[sSlot].tMagic[1];
			if(magic2 < 0 || magic2 >= g_arMagicItemTable.GetSize()) continue;
			dynamic2 = g_arMagicItemTable[magic2]->m_sSubType;
			magic3 = m_UserItem[sSlot].tMagic[2];
			if(magic3 < 0 || magic3 >= g_arMagicItemTable.GetSize()) continue;
			dynamic3 = g_arMagicItemTable[magic3]->m_sSubType;
			magic4 = m_UserItem[sSlot].tMagic[3];
			if(magic4 < 0 || magic4 >= g_arMagicItemTable.GetSize()) continue;
			dynamic4 = g_arMagicItemTable[magic4]->m_sSubType;

			if(m_UserItem[sSlot].tMagic[0] != NULL && m_UserItem[sSlot].tMagic[1] != NULL && m_UserItem[sSlot].tMagic[2] != NULL && m_UserItem[sSlot].tMagic[3] != NULL && m_UserItem[sSlot].tMagic[4] == NULL && dynamic1 == dynamic2 && dynamic2 == dynamic3 && dynamic3 == dynamic4)
			{
				dwSellCost = 1500000;//f_Markini[0]->JG_SIPAIJ;
			}
			else if(sStoreID == 30 && m_UserItem[sSlot].tMagic[5] == 0 && m_UserItem[sSlot].tIQ == 2 && m_UserItem[sSlot].tMagic[0] == NULL && m_UserItem[sSlot].tMagic[1] == NULL && m_UserItem[sSlot].tMagic[2] == NULL && m_UserItem[sSlot].tMagic[3] == NULL && m_UserItem[sSlot].tMagic[4] == NULL && m_UserItem[sSlot].sSid == 907)
			{
				dwSellCost = 2000000;//f_Markini[0]->JG_BAIGAO;
			}
			if(dwSellCost > 0)
			{
				if(m_UserItem[sSlot].sCount < sNum) return;   //封刷钱测试
				
				tempCost = dwResultCost + dwSellCost * sNum;
				if(!CheckMaxValueReturn((DWORD &)tempCost, (DWORD)(dwResultCost + dwSellCost * sNum)))
				{  
					bError = TRUE;
					break; 
				}
				iWeight += g_arItemTable[m_UserItem[sSlot].sSid]->m_byWeight * sNum;

				if(m_UserItem[sSlot].sCount ==  sNum)
				{
					MakeItemLog( &(m_UserItem[sSlot]), ITEMLOG_SELL );
					ReSetItemSlot(&m_UserItem[sSlot]);//ReSetItemSlot(sSlot);
				}
				else m_UserItem[sSlot].sCount = m_UserItem[sSlot].sCount - sNum;
				
//				iSlotNum = i;
				arSlot.Add((BYTE)sSlot);
//				sSlotList[i] = sSlot;
 				dwResultCost = tempCost;
			}
			else { bError = TRUE; break; }
		}
		else if(pStore->IsCanSell(m_UserItem[sSlot].sSid))
		{
			dwSellCost = GetSellCost(sSlot);
			if(dwSellCost > 0)
			{
				if(m_UserItem[sSlot].sCount < sNum) return;   //封刷钱测试
				
				tempCost = dwResultCost + dwSellCost * sNum;
				if(!CheckMaxValueReturn((DWORD &)tempCost, (DWORD)(dwResultCost + dwSellCost * sNum)))
				{  
					bError = TRUE;
					break; 
				}
				iWeight += g_arItemTable[m_UserItem[sSlot].sSid]->m_byWeight * sNum;

				if(m_UserItem[sSlot].sCount ==  sNum)
				{
					MakeItemLog( &(m_UserItem[sSlot]), ITEMLOG_SELL );
					ReSetItemSlot(&m_UserItem[sSlot]);//ReSetItemSlot(sSlot);
				}
				else m_UserItem[sSlot].sCount = m_UserItem[sSlot].sCount - sNum;
				
//				iSlotNum = i;
				arSlot.Add((BYTE)sSlot);
//				sSlotList[i] = sSlot;
 				dwResultCost = tempCost;
			}
			else { bError = TRUE; break; }
		}	
	}
	if(!CheckMaxValueReturn((DWORD &)m_dwDN, (DWORD)dwResultCost)) bError = TRUE;

go_result:
	if(bError)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)  m_UserItem[i] = MyItem[i];
		TempBuf.Add(ITEM_MOVE_RESULT);
		TempBuf.Add((BYTE)0x00);			// 角菩
		Send(TempBuf, TempBuf.GetLength());
		FlushItemLog( FALSE );
		return;
	}
	dwResultCost = SubTaxRate(dwResultCost,sStoreID);  //收税  税收  税率

//	m_dwDN += dwResultCost;					// 家瘤陛阑 盎脚
	CheckMaxValue((DWORD &)m_dwDN, (DWORD)dwResultCost);
	MakeMoneyLog( dwResultCost, ITEMLOG_SELL_MONEY );

											// 角菩窍搁 盔惑 汗备
	if(UpdateUserItemDN() == FALSE)
	{
		m_dwDN = dwMyDN;
		for(i = 0; i < TOTAL_ITEM_NUM; i++)  m_UserItem[i] = MyItem[i];

		SendSystemMsg( IDS_USER_SELL_FAIL, SYSTEM_ERROR, TO_ME);// "没有卖出物品(s)."

		FlushItemLog( FALSE );
		return;
	}

	// 函版等 浇吩阑 肺彪茄促
	FlushItemLog( TRUE );

	//-----------------[ Send Packet ]---------------------------------------//
	BYTE tType = 4;

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;
	GetRecoverySpeed();

	UpdateInvenSlot(&arSlot, NULL, tType);
//	SendQuickChange();
	SendMoneyChanged();
//	SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
}

//////////////////////////////////////////////////////////////////////////////////
//	荐府痢俊辑 荐府且荐 乐绰 荤侩磊 酒捞袍 府胶飘甫 父电促.
//
void USER::RepairItemOpenReq(int iStoreID) //修理物品
{
	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg( IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);
		return;
	}

	int i;
	short sStoreID = iStoreID;

	int iStart = 0;
	int iEnd = EQUIP_ITEM_NUM + INVENTORY_NUM;

	CStore* pStore = GetStore(sStoreID);
	if(pStore == NULL) return;

	CBufferEx CostBuf;
	short sSlot = 0;
	DWORD dwRepairCost = 0;
	short sCount = 0;
	for(i = iStart; i < iEnd; i++)
	{
		if(pStore->IsCanRepair(m_UserItem[i].sSid))
		{
			sSlot = (short)i;
			dwRepairCost = GetRepairCost(sSlot);

			if(dwRepairCost > 0)
			{				
				CostBuf.Add(sSlot);
				CostBuf.Add((short)m_UserItem[sSlot].sDuration);
				CostBuf.Add(dwRepairCost);
				sCount++;
			}
		}
	}

	CBufferEx TempBuf;
	TempBuf.Add(REPAIR_ITEM_OPEN_RESULT);
	TempBuf.Add(sStoreID);
	TempBuf.Add(sCount);
	if(sCount > 0) TempBuf.AddData(CostBuf, CostBuf.GetLength());

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	荐府且 酒捞袍狼 厚侩阑 拌魂茄促.
//
DWORD USER::GetRepairCost(short sSlot)    //修理物品
{
	short sSid = m_UserItem[sSlot].sSid;
 
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;
//	if(g_arItemTable[sSid]->m_byWear >= 9) return 0;
	if(g_arItemTable[sSid]->m_sDuration <= 0) return 0;

	DWORD	dwDN = g_arItemTable[sSid]->m_iDN;
	BYTE    tClass = g_arItemTable[sSid]->m_byClass;
	BYTE	tIQ = m_UserItem[sSlot].tIQ;
	int		iMagic = 0;
	int		i = 0;

	if(GetItemFieldNum(sSid) == QUICK_ITEM_TELEPORT)	// 勤叼霸捞飘绰 1郴备档 瞒捞俊 3000DN烙
	{
		int dif = g_arItemTable[sSid]->m_sDuration - m_UserItem[sSlot].sDuration;
		if(dif < 0) return  0;							// 勤叼霸捞飘 荐府厚啊 呈公 利绢辑 抗寇贸府甫 窃	
		dwDN = dif * 2000;
		return dwDN;
	}

	// 概流 肚绰 饭绢 酒捞袍狼 版快
	if(tIQ == MAGIC_ITEM && sSid != 703 && sSid != 704)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (2.2 + (double)(iMagic * 0.1)));
	}
	if(tIQ == RARE_ITEM && sSid != 703 && sSid != 704)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (4.3 + (double)(iMagic * 0.1)));
	}
	//////////////////////////////////////////////////////////////////守护修理 无我修理
	if ((sSid == 703 || sSid == 704) && tIQ == MAGIC_ITEM	)
	{
	for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (1.3 + (double)(iMagic * 0.1)));
		}
	if ((sSid == 703 || sSid == 704) && tIQ == RARE_ITEM	)
	{
	for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (2.3 + (double)(iMagic * 0.1)));
		}
     
/////////////////////////////////////////////////////////////////////
	short sMaxDuration = g_arItemTable[sSid]->m_sDuration;
	short sCurDuration = m_UserItem[sSlot].sDuration;

	if(sCurDuration > sMaxDuration)
	{
		m_UserItem[sSlot].sDuration = sMaxDuration;	// 规绢内靛
//		BREAKPOINT();
		return 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	//--yskang 0.6 蜡丰 荤侩磊 橇府固决 - 荐府厚甫 撤冕促. 蜡丰 荤侩磊绰 捞巴栏肺 父练沁澜 亮摆促. ^^ ^^
	double duTempCost = 0;
	if(m_iDisplayType == 5 || m_iDisplayType == 6) //公丰荤侩磊绰 弊措肺
		duTempCost = (double)( (double)(sMaxDuration - sCurDuration) * ( ((double)dwDN/32767.0) * 0.5 ) );	// 荐府厚 10硅 棵府扁 (2002.12.10)
	else //蜡丰 荤侩磊绰 撤冕促.
		duTempCost = (double)( (double)(sMaxDuration - sCurDuration) * ( ((double)dwDN/32767.0) * 0.25 ) );	//公丰荤侩磊 焊促 馆蔼俊 荐府
	//double duTempCost = (double)( (double)(sMaxDuration - sCurDuration) * ( ((double)dwDN/32767.0) * 0.05 ) );//扁粮 内靛
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	
	if(duTempCost < 0) duTempCost = 0;

	if(tClass == FIREARMS && (tIQ - 100) >= 0) duTempCost = duTempCost * 1.5;

	if((sMaxDuration - sCurDuration) > 0 && duTempCost < 1) return 1;

	return (DWORD)(duTempCost);
}

//////////////////////////////////////////////////////////////////////////////////
//	荐府等 酒捞袍阑 郴赴促.
//
void USER::RepairItem(TCHAR *pBuf)
{
	int index = 0, i, j;
	CStore* pStore = NULL;

	DWORD dwRepairCost = 0;
	DWORD dwTempCost = 0;
	CWordArray	arSlot;

	int	  iSid = 0;
	short sSlot = 0;
	short sStoreID = GetShort(pBuf, index);

	if(sStoreID < 0) return;

	// 捞亥飘 困摹甫 八荤 -----------------------------------------//
	CPoint pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, REPAIR_BLOCK) && m_curz != 409) return;

	short sCount = GetShort(pBuf, index);

	if(sCount <= 0 || sCount >= TOTAL_ITEM_NUM) return;

	for(i = 0; i < sCount; i++)
	{
		sSlot = GetShort(pBuf, index);
		if(sSlot < 0 || sSlot >= TOTAL_ITEM_NUM) return;

		iSid = m_UserItem[sSlot].sSid;
		if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;

		if(m_UserItem[sSlot].sCount >= 2) return;
		if(g_arItemTable[iSid]->m_sDuration <= 0) return;

		
//		dwRepairCost += GetRepairCost(sSlot);
		dwTempCost = GetRepairCost(sSlot);
		CheckMaxValue((DWORD &)dwRepairCost, (DWORD)dwTempCost); 

		if(dwRepairCost < 1 || m_dwDN < dwRepairCost)
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_DINA1, SYSTEM_ERROR, TO_ME);
			return;
		}

		arSlot.Add(sSlot);
	}

	for(i = 0; i < arSlot.GetSize(); i++)
	{
		sSlot = arSlot[i];
		iSid = m_UserItem[sSlot].sSid;
		m_UserItem[sSlot].sDuration = g_arItemTable[iSid]->m_sDuration;
	}

	//-----------------[ Send Packet ]---------------------------------------//
	if(m_dwDN < dwRepairCost) m_dwDN = 0;
	else m_dwDN = m_dwDN - dwRepairCost;

//	BYTE tType = 5;
//	UpdateInvenSlot((int *)sSlot, NULL, 1, 0, tType);
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add((BYTE)0x01);				// 己傍
	TempBuf.Add((BYTE)0x05);				// 老馆 酒捞袍
	TempBuf.Add((BYTE)sCount);				// 函拳等 浇吩狼 荐

	for(i = 0;  i < sCount; i++)
	{
		sSlot = arSlot[i];
		TempBuf.Add((BYTE)sSlot);
		TempBuf.Add(m_UserItem[sSlot].sLevel);
		TempBuf.Add(m_UserItem[sSlot].sSid);
		TempBuf.Add(m_UserItem[sSlot].sDuration);
		TempBuf.Add(m_UserItem[sSlot].sBullNum);
		TempBuf.Add(m_UserItem[sSlot].sCount);
		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSlot].tMagic[j]);
		TempBuf.Add(m_UserItem[sSlot].tIQ); 
	}

	Send(TempBuf, TempBuf.GetLength());
	SendMoneyChanged();

	arSlot.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////////////////
//	是否到了攻击时间  攻击速度  3速满


BOOL USER::CheckAttackDelay(BOOL *bSuccess, BYTE tWeaponClass)
{
	DWORD dwCurrTick = GetTickCount();
	DWORD dwMinDelay = DEFAULT_AT_DELAY;
	short sSid = m_UserItem[RIGHT_HAND].sSid;
	BOOL bHaste = FALSE;
	double speed1=0,speed2=0;

	if(tWeaponClass == 255)
	{
		if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
		{
			m_dwLastAttackTime = dwCurrTick;
			return TRUE;
		}
		else return FALSE;
	}
	
	if(tWeaponClass != 255)	// 盖颊捞 酒匆版快
	{
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
		dwMinDelay = g_arItemTable[sSid]->m_sAttackDelay;
	}

	if(m_dwHasteTime != 0 || m_dwBerserkerTime != 0) ////拳的加速 105魔法加速
		//speed2 = 0.25;	// Haste or Berserker 荤捞坷葱 利侩惑怕
		dwMinDelay = 570;

	else if(m_dwHasteTime == 0 || m_dwBerserkerTime == 0)
	{
	dwMinDelay = (DWORD)((double)(dwMinDelay - m_DynamicUserData[MAGIC_ATTACK_DELAY_DOWN] - m_DynamicEBodyData[EBODY_ATTACK_DELAY_DOWN]) );
	}
	//dwMinDelay =(DWORD)((double)dwMinDelay *(1-speed2+speed1));
	if(dwMinDelay < 570) dwMinDelay = 570;

	if(m_dwLastAttackTime >= dwCurrTick) return FALSE;

	if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
	{
	//	CString strMsg = _T("");
	//	strMsg.Format( "攻击速度为:『 %d 』", dwMinDelay);
    //	SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
		m_dwLastAttackTime = dwCurrTick;
		return TRUE;
	}

	return FALSE;
}
//////////////////////////////////////3速+机械满速使用

/*BOOL USER::CheckAttackDelay(BOOL *bSuccess, BYTE tWeaponClass)
{
	DWORD dwCurrTick = GetTickCount();
	DWORD dwMinDelay = DEFAULT_AT_DELAY;
	short sSid = m_UserItem[RIGHT_HAND].sSid;
	BOOL bHaste = FALSE;
	double speed1=0,speed2=0;

	if(tWeaponClass == 255)
	{
		if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
		{
			m_dwLastAttackTime = dwCurrTick;
			return TRUE;
		}
		else return FALSE;
	}
	
	if(tWeaponClass != 255)	// 盖颊捞 酒匆版快
	{
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
		dwMinDelay = g_arItemTable[sSid]->m_sAttackDelay;
	}

	if(m_dwHasteTime != 0 || m_dwBerserkerTime != 0) ////拳的加速 105魔法加速
		//speed2 = 0.25;	// Haste or Berserker 荤捞坷葱 利侩惑怕
		dwMinDelay -= 50;

	if((m_DynamicUserData[MAGIC_ATTACK_DELAY_DOWN] + m_DynamicEBodyData[EBODY_ATTACK_DELAY_DOWN]) >= 450)
	{
		dwMinDelay = (DWORD)(double)(dwMinDelay - 450);
	}
	else
	{
		//dwMinDelay = (DWORD)((double)(dwMinDelay - m_DynamicUserData[MAGIC_ATTACK_DELAY_DOWN] - m_DynamicEBodyData[EBODY_ATTACK_DELAY_DOWN]) );
		dwMinDelay -= (DWORD)(double)(m_DynamicUserData[MAGIC_ATTACK_DELAY_DOWN]);
		dwMinDelay -= (DWORD)(double)(m_DynamicEBodyData[EBODY_ATTACK_DELAY_DOWN]);
	}
	
	dwMinDelay =(DWORD)((double)dwMinDelay *(1-speed2+speed1));
	if(dwMinDelay < DEFAULT_AT_DELAY ) dwMinDelay = DEFAULT_AT_DELAY;

	if(m_dwLastAttackTime >= dwCurrTick) return FALSE;

	if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
	{
		//TRACE("sudu : %d \n",dwMinDelay);
	//	CString strMsg = _T("");
	//	strMsg.Format( "攻击速度为:『 %d 』", dwMinDelay);
    //	SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
		m_dwLastAttackTime = dwCurrTick;
		return TRUE;
	}

	return FALSE;
}*/
/*BOOL USER::CheckAttackDelay(BOOL *bSuccess, BYTE tWeaponClass)  //平常攻击速度
{
	DWORD dwCurrTick = GetTickCount();
	DWORD dwMinDelay = DEFAULT_AT_DELAY;
	short sSid = m_UserItem[RIGHT_HAND].sSid;
	BOOL bHaste = FALSE;
	double speed1=0,speed2=0;

	if(tWeaponClass == 255)
	{
		if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
		{
			m_dwLastAttackTime = dwCurrTick;
			return TRUE;
		}
		else return FALSE;
	}
	
	if(tWeaponClass != 255)	// 盖颊捞 酒匆版快
	{
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
		dwMinDelay = g_arItemTable[sSid]->m_sAttackDelay;
	}

	if(m_dwHasteTime != 0 || m_dwBerserkerTime != 0) ////拳的加速 105魔法加速
		//speed2 = 0.25;	// Haste or Berserker 荤捞坷葱 利侩惑怕
		dwMinDelay -= 50;

	if((m_DynamicUserData[MAGIC_ATTACK_DELAY_DOWN] + m_DynamicEBodyData[EBODY_ATTACK_DELAY_DOWN]) >= 550)
	{
		dwMinDelay = (DWORD)(double)(dwMinDelay - 550);
	}
	else
	{
		//dwMinDelay = (DWORD)((double)(dwMinDelay - m_DynamicUserData[MAGIC_ATTACK_DELAY_DOWN] - m_DynamicEBodyData[EBODY_ATTACK_DELAY_DOWN]) );
		dwMinDelay -= (DWORD)(double)(m_DynamicUserData[MAGIC_ATTACK_DELAY_DOWN]);
		dwMinDelay -= (DWORD)(double)(m_DynamicEBodyData[EBODY_ATTACK_DELAY_DOWN]);
	}
	
	dwMinDelay =(DWORD)((double)dwMinDelay *(1-speed2+speed1));
	if(dwMinDelay < DEFAULT_AT_DELAY ) dwMinDelay = DEFAULT_AT_DELAY;

	if(m_dwLastAttackTime >= dwCurrTick) return FALSE;

	if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
	{
	//	TRACE("sudu : %d \n",dwMinDelay);
	//	CString strMsg = _T("");
	//	strMsg.Format( "攻击速度为:『 %d 』", dwMinDelay);
    //	SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
		m_dwLastAttackTime = dwCurrTick;
		return TRUE;
	}

	return FALSE;
}*/
//////////////////////////////////////////////////////////////////////////////
//	判断玩家使用什么类型的武器
//
BOOL USER::IsCanUseWeaponSkill(BYTE& tWeaponClass)
{
	short sSid = m_UserItem[RIGHT_HAND].sSid;
	int iSize = g_arItemTable.GetSize();

	tWeaponClass = 255;				// 盖颊老 版快

	if(sSid < 0 || sSid >= iSize) return FALSE;

	BYTE tClass = g_arItemTable[sSid]->m_byClass;
	
	if(tClass == 1) tWeaponClass = FIREARMS;//枪
	else if(tClass == 2) tWeaponClass = EDGED;//剑
	else if(tClass == 4) tWeaponClass = STAFF;//法
	else if(tClass == 8) tWeaponClass = BRAWL;//拳
	else if(tClass == 16)
		tWeaponClass = JUDGE;	//仲裁
	else return FALSE;

	//@jjs 傈流矫 掘绰 胶懦俊 措茄 贸府甫 秦林绢具 茄促.
	if(tWeaponClass == m_byClass) return TRUE;
	if(m_sChangeClass[tWeaponClass] > 0) return TRUE;
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////
//	傍拜矫付促 胶懦己傍 咯何甫 眉农茄促.       /@@@@@@@@@@(01_07_15)
//
void USER::IsSkillSuccess(BOOL *bSuccess, BYTE tWeaponClass)
{
	int i;
	
	// 盖颊捞芭唱 泅犁拌凯 肚绰 傈流茄 拌凯捞 酒聪搁 胶懦荤侩 阂啊
	if(tWeaponClass == 255)
	{
		for(i = 0 ; i < SKILL_NUM; i++) bSuccess[i] = FALSE;
		return;
	}
	else if(tWeaponClass >= _CHANGE_DB) return;

	if(tWeaponClass != m_byClass)
	{
		if(m_sChangeClass[tWeaponClass] <= 0)	
		{
			for(i = 0 ; i < SKILL_NUM; i++) bSuccess[i] = FALSE;
		}
		return;
	}

	int iRandom;	
	int iLevel = 0;	
	int iSuccess = 0;
	int iMagicSkillNum = 0;
	int	iSkillSid = 0;
	int iIndex = tWeaponClass * SKILL_NUM;	//	拌凯(0, 1, 2, 3) * 5俺 胶懦)	
											
	for(i = iIndex; i < iIndex + SKILL_NUM; i++)// 阿 胶懦吝 饭骇捞 乐绰巴阑 舅酒辰促.
	{
		if(m_UserSkill[i].tLevel == 0) continue;

		iSkillSid = m_UserSkill[i].sSid;
		iLevel = m_UserSkill[i].tLevel;

		if(iSkillSid < 0 || iSkillSid >= MAX_SKILL_MAGIC_NUM) continue;
		iMagicSkillNum = g_DynamicSkillInfo[iSkillSid];//装备加的技能...
		if(iMagicSkillNum < 0 || iMagicSkillNum >= MAGIC_COUNT) continue;

		if(iLevel >= 1) iLevel += m_DynamicUserData[iMagicSkillNum] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

		if(iSkillSid >= g_arSkillTable.GetSize()) return;
		if(iLevel >= g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return;
		if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
											
		iSuccess = g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);// 己傍伏(茄 努贰胶俊辑 胶懦窜困 : 20究)

		if(iSuccess >= 100 && i >= (tWeaponClass * SKILL_NUM)) bSuccess[i - (tWeaponClass * SKILL_NUM)] = TRUE;
		else
		{
			iRandom = XdY(1, 100);
			if(iRandom < iSuccess && i >= (tWeaponClass * SKILL_NUM)) bSuccess[i - (tWeaponClass * SKILL_NUM)] = TRUE;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	醚扁狼 绊蜡 绊厘啦阑 啊廉柯促.
//
int USER::GetBreakRate(short sSid)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	return g_arItemTable[sSid]->m_byErrorRate;
}

////////////////////////////////////////////////////////////////////////////////////////
//	概流 酒袍殿俊 嘿绰 绊厘啦 皑家 蔼阑 啊廉柯促.//@@@@@@@@@@@ (01_7_15)
//
int USER::GetBreakDec()
{
	int nBreakDec = 0;
	int iEnd = 4;

	if(m_UserItem[RIGHT_HAND].tIQ == NORMAL_ITEM) return 0;
	if(m_UserItem[RIGHT_HAND].tIQ == UNIQUE_ITEM) iEnd = MAGIC_NUM;

	for(int i = 0; i < iEnd; i++)
	{
		if(m_UserItem[RIGHT_HAND].tMagic[i] == MAGIC_BROKEN_DOWN)
		{
			nBreakDec += m_DynamicUserData[MAGIC_BROKEN_DOWN];
		}
	}

	return nBreakDec;
}


void USER::SendSroreFail(int iType, BYTE err_code)
{

}

////////////////////////////////////////////////////////////////////////////
//	酒捞袍狼 弥措 郴备档甫 备茄促.
//
int USER::GetMaxDuration(short sSid)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	return g_arItemTable[sSid]->m_sDuration;
}

////////////////////////////////////////////////////////////////////////////
//	傍拜 啊瓷芭府甫 备茄促.
//
int USER::GetAttackRange(short sSid)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 1;

	return (int)g_arItemTable[sSid]->m_byRange;
}

////////////////////////////////////////////////////////////////////////////
//	疙吝啦阑 备茄促.(笼吝 胶懦 + 疙吝伏 刘啊 胶懦 + 概流 胶懦)
//
int USER::GetHitRate(short sSid, BYTE tWeaponClass)
{
	if(tWeaponClass == 255) return 0; // 盖颊篮 公扁俊狼茄 疙吝啦 刘啊蔼捞 绝促.
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	int iLevel = 0;
	int AttackRate = 0;//1敏捷==2点攻
	int iSkillSid = 0;
	int tClass = tWeaponClass * SKILL_NUM;

//	AttackRate = g_arItemTable[sSid]->m_byHitRate;

	// 胶懦俊狼茄 疙吝啦 刘啊
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;
		// 精神集中(10), 枪械大师(17) 精准(28)
		if(iSkillSid == SKILL_CONCENTRATION || iSkillSid == SKILL_ATTACK_RATING_UP|| iSkillSid == SKILL_JUDGE_JINGZHUN) // 笼吝 胶懦(10), 疙吝伏 刘啊胶懦(17)
		{
			// 鉴荐 胶懦 饭骇 			
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;

			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iLevel >= g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return 0;
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			AttackRate += g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);
		}
	}
	if(tWeaponClass==0){
		
	}
	// 酒捞袍俊 狼茄 磊眉 疙吝啦 刘啊
	AttackRate += m_DynamicUserData[MAGIC_ATTACK_RATING_UP] + (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_ATTACK_RATING] / 100 * (double)m_sMagicDEX);
	AttackRate += (int)((double)AttackRate * (double)m_DynamicEBodyData[EBODY_ATTACK_RATING_UP] / 100);

	return AttackRate;
}


////////////////////////////////////////////////////////////////////////////
//	蕊盒俊 狼秦辑父 傍拜矫 胶抛固呈 皑家 /@@@@@@@@  01_7_15
//
int USER::DecAttackSP(BOOL *bSuccessSkill, BYTE tWeaponClass)
{
/*	int iLevel = 0;
	int iSkillSid = 0;
	int tClass = tWeaponClass * SKILL_NUM;

	short sSP = GetSP();

	if(sSP <= 0) return -1;

	short sNeedSP = DEFAULT_SP_DEC;
	short sSid = m_UserItem[RIGHT_HAND].sSid;

//	if(sSid >= 0 && sSid < g_arItemTable.GetSize())	sNeedSP = 1 + g_arItemTable[sSid]->m_byNeedSP; //傍烹

	if(tWeaponClass != 255)
	{
		for(int i = tClass; i < tClass + SKILL_NUM; i++)
		{
			iSkillSid = m_UserSkill[i].sSid;

			if(iSkillSid == SKILL_EXCITE) 
			{
				// 鉴荐 胶懦 饭骇 				
				iLevel = m_UserSkill[i].tLevel;		
				if(iLevel < 0) iLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				if(iLevel > g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return 0;
				if(iLevel > SKILL_LEVEL) iLevel = SKILL_LEVEL;
				
				sNeedSP += g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel);
			}
		}
		if(sNeedSP < DEFAULT_SP_DEC) sNeedSP = DEFAULT_SP_DEC;
	}

	if(m_sSP >= sNeedSP)
	{
		m_sSP -= sNeedSP;

		SendSP();
		return m_sSP;
	}
*/	
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////
//	雀乔蔼阑 掘绰促. (泅犁 胶懦吝 雀乔 胶懦乐栏搁 : 泅犁 胶懦饭骇 + 概流 酒捞袍 胶懦饭骇 父怒狼 胶懦抛捞喉俊辑 啊廉咳 
//
int USER::GetAvoid()
{
	BYTE	tWeaponClass = 255;
	int		iLevel = 0;
	int		iSkillSid = 0;
	int		AvoidRate = 0;

	if(IsCanUseWeaponSkill(tWeaponClass) == FALSE)	
	{
		//yskang 0.3 公扁啊 绝阑锭 雀乔啦阑 拌魂窍扁 困秦
		AvoidRate += (m_DynamicUserData[MAGIC_AVOID_RATING_UP] +(int)((double)m_DynamicEBodyData[EBODY_DEX_TO_AVOID_RATING] / 100 * m_sMagicDEX));
		AvoidRate += (int)((double) AvoidRate * (double)m_DynamicEBodyData[EBODY_AVOID_RATING_UP] / 100);
		return AvoidRate;
	}

	int tClass = tWeaponClass * SKILL_NUM; 

	// 磊扁 泅犁 胶懦吝... 
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_AVOID_RATING_UP) // 雀乔啦 刘啊胶懦(18)
		{
			// 鉴荐 胶懦 饭骇 			
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;

			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			if(iLevel >= g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return 0;
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			AvoidRate += g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);
		}
	}

	AvoidRate += (m_DynamicUserData[MAGIC_AVOID_RATING_UP] + (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_AVOID_RATING] / 100 * m_sMagicDEX)); //雀乔啦 刘啊.(拉率篮 雀乔胶懦刘啊, 捞扒 磊眉 雀乔刘啊.)
	AvoidRate += (int)((double) AvoidRate * (double)m_DynamicEBodyData[EBODY_AVOID_RATING_UP] / 100);

	return AvoidRate;
}

//////////////////////////////////////////////////////////////////////////////////////
//	农府萍吧 啊靛 己傍 咯何甫 掘绰促.
//
double USER::GetCriticalGuard(BOOL *bSuccessSkill, BYTE& tWeaponClass)
{
	short	sSid = m_UserItem[RIGHT_HAND].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
	
	int		iGS = 0;
	int		iRandom = 0;
	int		iLevel = 0;
	int		CriticalGuard = 0;
	int		iSkillSid = 0;
	int		tClass = tWeaponClass * SKILL_NUM;

	double  nCGuard = 0;

	tWeaponClass = 255;

	if(IsCanUseWeaponSkill(tWeaponClass) == FALSE)		// 泅犁 磊脚捞 坷弗颊俊 电 公扁狼 胶懦阑 荤侩且 荐 绝栏搁 溜 盖颊捞搁
	{
		return 0;
	}

	// 磊扁 泅犁 胶懦吝... 
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_CRITICAL_GUARD) 
		{
			// 鉴荐 胶懦 饭骇 			
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;

			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iLevel >= g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return 0;
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;

			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
			if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)) iGS = 1;

			CriticalGuard += g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);
		}
	}

	nCGuard = (double)(1.0 - (double)(iGS * CriticalGuard/100.0));
	if(nCGuard < 0) nCGuard = 0;

	return nCGuard;
}

//////////////////////////////////////////////////////////////////////////////////////
//	檬扁 老馆 单固瘤甫 掘绰促.
//  实际物理攻击统计
int USER::GetNormalInitDamage(BYTE tWeaponClass, USER* pTarget,BOOL &bIsCritical, BOOL bMax)//yskang 0.3int USER::GetNormalInitDamage(BYTE tWeaponClass, USER* pTarget, BOOL bMax)
{
	int nFinalDamage = 0;		// 弥辆单固瘤
	int xyz = 0;				// 公扁单固瘤	

	//int tClass = tWeaponClass * SKILL_NUM;
	int tClass;
	//仲裁职业
	if(tWeaponClass == JUDGE)
	{
		tClass = 24;
	}else
	{
		tClass = tWeaponClass * SKILL_NUM;
	}
	double iBasic = 0;			// 扁夯单固瘤
	double iExcite = 0;			// 蕊盒单固瘤
	double iFire = 0;			// 楷荤单固瘤
	int iMinDam = 0;			// 弥家单固瘤
	double dAssault = 0;		// 竿傍单固瘤
	double dMightyWeapon = 0;	// 公扁碍拳 荤捞坷葱
	double dBerserker = 0;		// 滚辑农 荤捞坷葱

	int iExciteLevel = 0;		// 蕊盒饭骇
	int iFireLevel = 0;			// 楷荤饭骇
	int iMinLevel = 0;			// 弥家单固瘤刘啊 饭骇
	int iAssaultLevel = 0;		

	int iRandom = 0;
	int iSkillSid = 0;
	short sNeedSP = 0;

	int iRandomProtect = (int)((double)XdY(1, 1000) / 10 + 0.5);
	int i = 0;

	switch(tWeaponClass)    //机械属性计算
	{
	case FIREARMS:
		if(m_dwDexUpTime)	
			iBasic = (int)((double)(m_sMagicDEX + 10 ) / 2 + 0.5+ (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_DAMAGE] / 100 *(m_sMagicDEX+10)));	
		else
			iBasic = (int)((double)(m_sMagicDEX + 10 ) / 2 + 0.5+ (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_DAMAGE] / 100 *(m_sMagicDEX)));	
		break;
	case JUDGE:
		if(m_dwWISUpTime)
		{
			iBasic = (int)((double)(m_sMagicWIS + 10 ) / 2 + 0.5+ (int)((double)m_DynamicEBodyData[EBODY_WIS_TO_DAMAGE] / 100 *(m_sMagicWIS+30)));
		}else
		
		{
			iBasic = (int)((double)(m_sMagicWIS + 10 ) / 2 + 0.5+ (int)((double)m_DynamicEBodyData[EBODY_WIS_TO_DAMAGE] / 100 *(m_sMagicWIS)));
		}
		//牺牲护法增加20攻击
		if(m_dwXiShengTime)
			iBasic+=iBasic*20/100;
		break;
	default:
		iBasic = (int)((double)(m_sMagicSTR  )/2 + 0.5+ (int)((double)m_DynamicEBodyData[EBODY_STR_TO_DAMAGE] / 100 * m_sMagicSTR));	
		break;
	}

	if ( m_byClass == 2 || m_byClass == 3 || m_byClass == 4)  //拳/剑/枪/仲裁135级魔法增加300物攻实际效果
	{
	//	if(m_dwFANTAnTime != 0) iBasic += 150;
		if(m_dwFENGLiTime != 0) iBasic += 150;
		if(m_dwDUOCHONgTime != 0) iBasic += 150;
	}
    
	//if ( m_byClass != 1) iBasic += m_sLevel;   //各职业加物理攻击
	
	/*if( m_byClass == EDGED )
		{
             iBasic += 50;
			}*/

	if(iBasic < 0) iBasic = 0;

	if(tWeaponClass == 255) return (int)iBasic;

	xyz = GetWeaponDamage(m_UserItem[RIGHT_HAND].sSid, bMax);

	// 蕊盒, 楷荤, 弥家单固瘤诀
	for(i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_EXCITE)			//拳的兴奋技能
		{			
			iExciteLevel = m_UserSkill[i].tLevel;		
			if(iExciteLevel < 0) iExciteLevel = 0;

			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iExciteLevel >= 1) iExciteLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iExciteLevel >= SKILL_LEVEL) iExciteLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) continue;

			sNeedSP += g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iExciteLevel);
			if(sNeedSP <= GetSP())
			{
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if((iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iExciteLevel))|| bMax)
				{
					if(m_sSP >= sNeedSP)
					{
						m_sSP -= sNeedSP;
						SendSP();
					}
					iExcite = ((iBasic + xyz) * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iExciteLevel)/100.0 ); //兴奋技能
				}
			}
		}

		if(iSkillSid == SKILL_RE_FIRE_UP)			// 16 连射
		{
			iFireLevel = m_UserSkill[i].tLevel;		
			if(iFireLevel < 0) iFireLevel = 0;

			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iFireLevel >= 1) iFireLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
            if(iFireLevel >= SKILL_LEVEL) iFireLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) continue;

			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
	        if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iFireLevel)  || bMax)
			{
				//iFire = (xyz * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iFireLevel)/100.0); //// 连射技能
				iFire = ((iBasic + xyz) * (double)(g_arSkillTable[iSkillSid]->m_arInc.GetAt(iFireLevel)/100.0 )); 
			}
		}
		//---------------------------------------------------
		//yskang 0.3 蕊盒捞唱 楷荤己傍捞搁 农府萍拿 眉农
		if(iExcite + iFire > 0)
		{
			bIsCritical = TRUE;
		}
		//-----------------------------------------------------

		if(iSkillSid == SKILL_MIN_DAMAGE_UP)				// 15 index
		{
			iMinLevel = m_UserSkill[i].tLevel;		
			if(iMinLevel < 0) iMinLevel = 0;

			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iMinLevel >= 1) iMinLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iMinLevel >= SKILL_LEVEL) iMinLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) continue;

			iMinDam = g_arSkillTable[iSkillSid]->m_arInc.GetAt(iMinLevel);
		}

		if(iSkillSid == SKILL_ASSAULT)
		{
			iAssaultLevel = m_UserSkill[i].tLevel;
			if(iAssaultLevel < 0) iAssaultLevel = 0;

			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iAssaultLevel >= 1) iAssaultLevel += m_DynamicUserData[MAGIC_ALL_SKILL_UP];	// 100乏捞惑 胶懦篮 葛电胶懦 刘啊俊父 康氢阑 罐绰促

			if(iAssaultLevel >= SKILL_LEVEL) iAssaultLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) continue;

			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
			if((iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iAssaultLevel))|| bMax)
			{
				dAssault = ((iBasic + xyz) * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iAssaultLevel)/100.0);
			//niuniu add--
				if (bMax)  //必杀
				{
					if (pTarget)  //打玩家的
					{
						if(m_byClass == FIREARMS) // 平射攻击加20%左右   
						{
							dAssault *=0.7f;
						}
						else if(m_byClass == EDGED) // 剑攻击减少15%   
						{
							dAssault *=0.75f;
						}
						else if(m_byClass == BRAWL) // 拳攻击增加30%  
						{
							dAssault *=0.95f;
						}
						else if(m_byClass == JUDGE) // 仲裁攻击增加30%  
						{
							dAssault *=1.1f;
						}
					}
					else			//打怪物 
					{
						if(m_byClass == FIREARMS) // 平射攻击加20%左右   
						{
							dAssault *=0.01f;
						}
						else if(m_byClass == EDGED) // 剑攻击减少15%   
						{
							dAssault *=0.6f;
						}
						else if(m_byClass == BRAWL) // 拳攻击增加30%  
						{
							dAssault *=0.1f;
						}
						else if(m_byClass == JUDGE) // 仲裁攻击增加30%  
						{
							dAssault *=0.2f;
						}
					}
				
				}
				else // 弩机
				{
					if (pTarget)  //打玩家的
					{
						if(m_byClass == FIREARMS) // 平射攻击加20%左右   
						{
							dAssault *=0.7f;
						}
						else if(m_byClass == EDGED) // 剑攻击减少15%   
						{
							dAssault *=0.9f;
						}
						else if(m_byClass == BRAWL) // 拳攻击增加30%  
						{
							dAssault *=0.9f;
						}
						else if(m_byClass == JUDGE) // 仲裁攻击增加30%  
						{
							dAssault *=1.15f;
						}
					}
					else		//打怪物
					{
						if(m_byClass == FIREARMS) // 平射攻击加20%左右   
						{
							dAssault *=0.3f;
						}
						else if(m_byClass == EDGED) // 剑攻击减少15%   
						{
							dAssault *=0.7f;
						}
						else if(m_byClass == BRAWL) // 拳攻击增加30%  
						{
							dAssault *=0.3f;
							
						}
						else if(m_byClass == JUDGE) // 仲裁攻击增加30%  
						{
							dAssault *=0.6f;
						}
                     }
				
				}
			}
		}
	

		//仲裁爆发,类似拳兴奋
		if(iSkillSid == SKILL_JUDGE_BAOFA)			
		{			
			iExciteLevel = m_UserSkill[i].tLevel;		
			if(iExciteLevel < 0) 
				iExciteLevel = 0;

			if(iExciteLevel >= 1) 
				iExciteLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iExciteLevel >= SKILL_LEVEL)
				iExciteLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize())
				continue;

			sNeedSP += g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iExciteLevel);
			if(sNeedSP <= GetSP())
			{
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if((iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iExciteLevel))|| bMax)
				{
					if(m_sSP >= sNeedSP)
					{
						m_sSP -= sNeedSP;
						SendSP();
					}
					iExcite = ((iBasic + xyz) * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iExciteLevel)/100.0);
				}
			}
		}
	}

	if( m_dwMightyWeaponTime != 0 )
	{
		dMightyWeapon = (double)( (double)(iBasic+xyz) * 0.1 );
	}
	if( m_dwBerserkerTime != 0 )
	{
		dBerserker = (double)( (double)(iBasic+xyz) *0.15 );
	}
    
/////////如果是拳
  /* if( m_byClass == BRAWL){
		dBerserker =dBerserker + (double)( (double)(iBasic+xyz) *0.3 );
	}
/////////如果是法
	
/////////如果是刀
	if( m_byClass == EDGED ){
	dBerserker =dBerserker + (double)( (double)(iBasic+xyz) *0.3 );
	}
//////如果是枪
	if( m_byClass == FIREARMS ){
		dBerserker =dBerserker + (double)( (double)(iBasic+xyz) *1.2 );
	}
//////如果是仲裁
	if( m_byClass == JUDGE ){
		dBerserker =dBerserker + (double)( (double)(iBasic+xyz) *0.3 );
	}*/
/////////////////////////////////////////////////////////////////////////////////	
	if(pTarget != NULL && pTarget->m_state == STATE_GAMESTARTED)
	{
		if(iRandomProtect <= RANDOM_PROTECT && pTarget->m_bEarringOfProtect)	// 辑器飘 蓖吧捞
		{
			if(iExcite + iFire > 0)		// 蕊盒 肚绰 楷荤啊 磐龙锭父
			{
				nFinalDamage = (int)((iBasic + xyz) + iMinDam + dAssault + dMightyWeapon + dBerserker);
				pTarget->SendAccessoriDuration(SID_EARRING_OF_PROTECT);
			}
		}
		else 
		{
			nFinalDamage = (int)((iBasic + xyz) + iExcite + iFire + iMinDam + dAssault + dMightyWeapon + dBerserker);
		}
	}
	else
	{
		nFinalDamage = (int)((iBasic + xyz) + iExcite + iFire + iMinDam + dAssault + dMightyWeapon + dBerserker);
	}

	if(m_sLevel >= ADD_USER_LEVEL) nFinalDamage += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDamage;

	return nFinalDamage;
}

///////////////////////////////////////////////////////////////////////////////////////
//	檬扁 农府萍拿 单固瘤甫 掘绰促.
//  命中初试攻击值
int USER::GetCriticalInitDamage(BOOL *bSuccessSkill, BYTE tWeaponClass)
{
	int nDamage = 0;
	int xyz = 0;

	if(tWeaponClass == 255) return (int)(m_sMagicSTR / 4);

	xyz = GetWeaponDamage(m_UserItem[RIGHT_HAND].sSid);

	switch(tWeaponClass)
	{
	case BRAWL:		// 拜捧
		if(bSuccessSkill[4] == TRUE)	// 蕊盒胶懦利侩
		{
			nDamage = (int)((1 + 0.01 * (50 + 2.5 * (double)m_UserSkill[tWeaponClass * SKILL_NUM + 4].tLevel)) * (xyz + (double)m_sMagicSTR/4));
		}
		else
		{
			nDamage = (int)(xyz + (double)m_sMagicSTR/4);
		}
		break;

	case EDGED:		// 档八
	case STAFF:		// 瘤莆捞
		nDamage = (int)(xyz + (double)m_sMagicSTR / 4);
		break;
/*
	case JUDGE:		// 瘤莆捞
		nDamage = (int)(xyz + (double)m_sMagicWIS / 4);
		break;
*/
	case FIREARMS:	// 醚扁
		if(bSuccessSkill[3] == TRUE)	// 抄荤 胶懦利侩
		{
			nDamage = (int)(1.4 * ((double)xyz + m_sMagicDEX / 5));
		}
		else
		{
			nDamage = (int)(2 * (xyz + (double)m_sMagicDEX/5));
		}
		break;
	}

	return nDamage;
}

///////////////////////////////////////////////////////////////////////////////////////
//	公扁 磊眉狼 单固瘤甫 拌魂茄促.(坷弗率 公扁父 拌魂)
//
int USER::GetWeaponDamage(short sSid, BOOL bMax)//(sid, 弥措蔼 = 可记)
{
	int iDamage = 0;
	int iTemp = 0;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	int x = g_arItemTable[sSid]->m_byAX;
	int y = g_arItemTable[sSid]->m_byAY;
	int z = g_arItemTable[sSid]->m_byAZ;

	if(bMax == TRUE) iDamage = x * y + z;
	else	         iDamage = XdY(x, y) + z;


	if(iDamage > 0)
	{
		iDamage += (int)((double)iDamage * (double)m_DynamicEBodyData[EBODY_WEAPON_BASE_UP] / 100); //机械武器伤害1%
		
		int up_count = 0;
		up_count = m_UserItem[RIGHT_HAND].tMagic[ITEM_UPGRADE_COUNT];

		if(m_dwHtExpTime > 0) {
			if(m_byClass != 1) up_count += 1;	//物理武器+1	
		}		
		if(up_count > 0 && up_count <= MAX_ITEM_UPGRADE_COUNT + 1)//最大改数+1条件下
		{
			if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byClass != STAFF_ITEM)
			{
				//130级装备改30攻每改                   //////////////////////////////////NEO版本需要开放的
				if(Is130Wearp(sSid))
				{
					if(up_count<=6)//130级武器前6改27,后30
					{
						iTemp+=(up_count*27);
					}else
					{
						iTemp += ((up_count - 6) * 30) + 27*6;
					}
				}else 
				{
					if(up_count<=9)//100级武器前9改+16,后+20
					{
						iTemp+=(up_count*16);
					}else
					{
						iTemp += ((up_count - 9) * 20) + 16*9;
					}
				}
				//iTemp += (up_count * ATTACK_UPGRADE_BAND);
			}
		}

		iDamage += m_DynamicUserData[MAGIC_DAMAGE_UP];
		iDamage += iTemp;						  // +上改数的攻击
	}else iDamage = 1;

	return iDamage;
}
int USER::GetWeaponDamagePSI(short sSid, BOOL bMax)//获得武器魔法攻击
{
	int iDamage = 0;
	int iTemp = 0;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;
	if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byClass != STAFF_ITEM)	return 0;
	int x = g_arItemTable[sSid]->m_byAX;
	int y = g_arItemTable[sSid]->m_byAY;
	int z = g_arItemTable[sSid]->m_byAZ;

	if(bMax == TRUE) iDamage = x * y + z;
	else	         iDamage = XdY(x, y) + z;


	if(iDamage > 0)
	{
		iDamage += (int)((double)iDamage * (double)m_DynamicEBodyData[EBODY_PSI_ATTACK_UP] / 100); //机械魔伤2%		

		int up_count = 0;
		up_count = m_UserItem[RIGHT_HAND].tMagic[ITEM_UPGRADE_COUNT];
		if(m_dwHtExpTime > 0) {
			if(m_byClass == 1) up_count += 1;	//法师武器+2	
				
		}		
		if(up_count > 0 && up_count <= MAX_ITEM_UPGRADE_COUNT + 1)
		{
			//if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byClass == STAFF_ITEM)	{
				iTemp += (up_count * ATTACK_UPGRADE_PSI_BAND);
			//}
		}

		iDamage += m_DynamicUserData[MAGIC_PSI_ATTACK_UP];
		iDamage += iTemp;						  // +上改数的攻击
	}else iDamage = 1;

	return iDamage;
}
///////////////////////////////////////////////////////////////////////////////////////
//	农府萍拿 己傍咯何甫 掘绰促.	
//  穿刺命中
double USER::GetCriticalHit(BYTE tWeaponClass ,int max)
{
	int		iVC = 0;
	int		iRandom = 0;
	int		iLevel = 0;
	int		iSkillSid = 0;
	int		vitalCritical = 0;
	int		vitalHit = 0;
	double  nVCritical = 0;

	if(tWeaponClass == 255) return nVCritical;

    int tClass = tWeaponClass * SKILL_NUM;

	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_VITAL_CRITICAL) // 官捞呕 农府萍拿(12)
		{
			iLevel = m_UserSkill[i].tLevel;		// 鉴荐 胶懦 饭骇
			if(iLevel < 0) iLevel = 0;

			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
			if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)|| max==1)
			{
				nVCritical = (double)(g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel)/100.0);
			}
		}
	}

	if(nVCritical <= 0) nVCritical = 0.0;
	return nVCritical;
}

//////////////////////////////////////////////////////////////////////////
//	规绢备狼 规绢蔼阑 啊廉柯促.
//  装备的防御计算
short USER::GetItemDefense(short sSid, int slot)
{
	short sDefense = 0;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;
	if(slot < 0 || slot > EQUIP_ITEM_NUM) return 0;
																	
	if(m_UserItem[slot].tIQ != UNIQUE_ITEM)	// 诀臂 酒捞袍阑 困秦
	{
		int up_count = 0;
		up_count = m_UserItem[slot].tMagic[ITEM_UPGRADE_COUNT];
		if(up_count > 0 && up_count <= MAX_ITEM_UPGRADE_COUNT)
		{
			//对130级装备改成10+1防
			if(Is130Item(sSid))
			{
				if(up_count<=6)//衣裤前改+10,后+12
				{
					sDefense += ((up_count) * 10);
				}else
				{
					sDefense += ((up_count-6) * 12)+60;
				}
			}else if(Is100Item(sSid))
			{
				if(up_count<=7)//衣裤前改+4,后+7
				{
					sDefense += ((up_count) * 4);
				}else
				{
					sDefense += ((up_count-7) * 7)+4*7;
				}
			}else 
			{
				if(up_count<=9)//衣裤前9改+4,后+7
				{
					sDefense += ((up_count) * 4);
				}else
				{
					sDefense += ((up_count-9) * 7)+4*9;
				}
			}
			//sDefense += (up_count * DEFENSE_UPGRADE_BAND);
		}
	    if(m_dwMagicFtTime > 0) sDefense += 4;
	}

	return sDefense += g_arItemTable[sSid]->m_sDefense;
//	return g_arItemTable[sSid]->m_sDefense;
}



//////////////////////////////////////////////////////////////////////////
//	醚扁狼 郴备档 皑家蔼阑 啊廉柯促.
//
short USER::GetDecDuration(short sSid)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	return g_arItemTable[sSid]->m_sBullNum;
}

//////////////////////////////////////////////////////////////////////////
//	酒捞袍狼 郴备档 函版蔼阑 焊辰促.
//
void USER::SendDuration(BYTE tSlot, int iDamage)
{
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) return;
	
	if(m_UserItem[tSlot].sSid < 0 || m_UserItem[tSlot].sSid >= g_arItemTable.GetSize()) return;
	
	long lMax = (long)g_arItemTable[m_UserItem[tSlot].sSid]->m_sDuration;

	if(lMax <= 0) return;

	long lCurPer = (long)((m_UserItem[tSlot].sDuration * 100.0)/lMax);

	m_UserItem[tSlot].sDuration -= iDamage;
	if(m_UserItem[tSlot].sDuration < 0) m_UserItem[tSlot].sDuration = 0;

	long lNextPer = (long)((m_UserItem[tSlot].sDuration * 100.0)/lMax);

	if(lNextPer > 20) 
	{
		// 扁霖捞 5%付促..
		int iCur = (int)(lCurPer/5.0);
		int iNext = (int)(lNextPer/5.0);

		if(iCur == iNext) return;
	}	

	CBufferEx TempBuf;

	TempBuf.Add(ITEM_DURATION);
	TempBuf.Add(tSlot);
	TempBuf.Add(m_UserItem[tSlot].sDuration);

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////
//	酒捞袍狼 儡藕樊(瑰单府樊阑 焊辰促.)
//
void USER::SendBullNum(BYTE tSlot)
{
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) return;

	CBufferEx TempBuf;

	TempBuf.Add(ITEM_BULLNUM);
	TempBuf.Add(tSlot);
	TempBuf.Add(m_UserItem[tSlot].sBullNum);

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////
//	泅犁 HP甫 掘绰促.
//
short USER::GetHP()
{
	BYTE	tWeaponClass = 255;

	int iSkillSid = 0;
	int iLevel = 0, iRandom = 0, iHP = 0;

	double dPlusHP = 1.0;
	int iAddHP = 0;

	DWORD dwDiffTime = GetTickCount() - m_dwLastHPTime;

	if(dwDiffTime >= m_iHPIntervalTime)
	{
		if(IsCanUseWeaponSkill(tWeaponClass))	// 泅犁 磊脚捞 坷弗颊俊 电 公扁狼 胶懦捞 乐栏搁
		{
			int tClass = tWeaponClass * SKILL_NUM; 

			for(int i = tClass; i < tClass + SKILL_NUM; i++)			// 胶懦吝 HP雀汗 胶懦捞 乐栏搁 馆康茄促.
			{
				iSkillSid = m_UserSkill[i].sSid;

				if(iSkillSid == SKILL_HP_RECOVER_UP)					// 3 index
				{
					iLevel = m_UserSkill[i].tLevel;		
					if(iLevel < 0) iLevel = 0;

					// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
					if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
					if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
					if(iSkillSid >= g_arSkillTable.GetSize()) continue;

					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)) iHP = 1;

					dPlusHP = 1.0 + (double)iHP * g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel) / 100.0;
				}
			}
		}

		if(m_iHPRate <= 0)	// 焊沥蔼
		{
			m_iHPRate = 1;
			TRACE("雀汗厚啦 俊矾\n");
		}
		
		// 档矫搁 雀汗俊 何啊利牢 焊呈胶啊 歹秦柳促	
		if(IsCity() && CheckRecoverTableByClass()) iAddHP = g_arRecoverTable[m_byClass]->m_byTown;

		m_sHP += (int)(dPlusHP * (double)m_sMagicCON/m_iHPRate ) + iAddHP;
		if(m_sHP > m_sMagicMaxHP) m_sHP = m_sMagicMaxHP;

		m_dwLastHPTime = GetTickCount();
	}

	return m_sHP;
}


////////////////////////////////////////////////////////////////////////////////////
//	HP 甫 努扼捞攫飘肺 焊辰促.
//
void USER::SendHP()
{
	CBufferEx TempBuf;

	if(m_bLive == USER_DEAD) return;

	TempBuf.Add(SET_HP);
	TempBuf.Add(m_sHP);

	Send(TempBuf, TempBuf.GetLength());
	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
}

/////////////////////////////////////////////////////////////////////////////////////
//	NPC狼 努贰胶甫 掘绰促.
//
BYTE USER::GetNpcClass(CNpc *pNpc)
{
	BYTE tClass = BRAWL;

	if(pNpc == NULL) return 0;

	switch(pNpc->m_byClass)
	{
	case 1:
		tClass = FIREARMS;
		break;
	case 2:
		tClass = EDGED;
		break;
	case 4:
		tClass = STAFF;
		break;
	case 8:
		tClass = BRAWL;
		break;
	}

	return tClass;
}

//////////////////////////////////////////////////////////////////////////////////////
//	漂沥 荤捞坷葱阑 蜡历啊 家蜡沁绰瘤 八荤茄促.
//
BOOL USER::IsHavePsionic(BYTE tSid)
{
	if(tSid < 0 || tSid >= TOTAL_PSI_NUM) return FALSE;

	for(int i = 0; i < TOTAL_PSI_NUM; i++)
	{
		if(m_UserPsi[i].sSid == -1) break;
		if(m_UserPsi[i].sSid == (short)tSid) return TRUE;
//		if(m_UserPsi[i].sSid == (short)tSid && m_UserPsi[i].tOnOff == 1) return TRUE;
	}
	if(tSid == (BYTE)44 || tSid == (BYTE)45) return true;//激光,导弹
//	if(tSid == (BYTE)54) return true;//战神技能
	if((tSid==0x17 ||tSid==0x18 ||tSid==0x19 ||tSid==0x1a||tSid == PSI_BISA_4) 
		&&m_UserItem[34].sSid!=-1&&m_UserItem[35].sSid!=-1&&(m_UserItem[36].sSid==987
		||m_UserItem[36].sSid==1270)&&m_UserItem[37].sSid!=-1)
		return true;
	if(tSid == 54)//战神技能
	{
		if((m_UserItem[39].tMagic[7] == 7) && (m_iSkin == 8 || m_iSkin == 170))	return TRUE;
		else return FALSE;
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 傍拜矫 PP 眉农 棺 皑家贸府
//
int USER::DecAttackPP(BOOL *bSuccessSkill, BYTE tWeaponClass, short sSid, BOOL bDec)
{
	short sOldPP = GetPP();
	short sNeedPP = 0;
	short b3 = bSuccessSkill[3];	// 荤捞坷葱 利览胶懦 己傍咯何

	if(sNeedPP < 0) return -1;

	if(tWeaponClass == STAFF)
	{
		sNeedPP = (short)((double)sNeedPP * ((double)((100 - (2 * b3 * m_UserSkill[tWeaponClass * SKILL_NUM + 3].tLevel + 20))) / 100));
	}
	
	if(sNeedPP < 0) return -1;
	return sNeedPP;

	if(m_sPP >= sNeedPP)
	{
		if(bDec == FALSE) return 1;

		m_sPP -= sNeedPP;
		if(m_sPP < 0) m_sPP = 0;

		if(sOldPP != m_sPP) SendPP();

		return m_sPP;
	}

	if(sOldPP != m_sPP) SendPP();
	
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 矫傈矫俊 鞘夸茄 PP樊阑 备茄促.
//
short USER::GetNeedPP(BOOL *bSuccessSkill, BYTE tWeaponClass, short sSid)
{
	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) return -1;
	if(sSid >= g_arPsiTable.GetSize()) return -1;

	short sNeedPP = g_arPsiTable[sSid]->m_tNeedPP;
//	if(sSid==13 || sSid==14 || sSid==15)  sNeedPP=2000;
	int tClass = tWeaponClass * SKILL_NUM;
	int iLevel = 0;
	int iSkillSid = 0;
	// PP 家葛樊 皑家 胶懦俊 狼秦辑 皑家瞪 蔼 备窍扁
	int iStaff = 0;
	if(tWeaponClass == STAFF) iStaff = 1;	// 瘤莆捞甫 静绊 乐唱?

	short sDecPP = 0;
	
	if(iStaff == 1)
	{
		for(int i = tClass; i < tClass + SKILL_NUM; i++)
		{
			iSkillSid  = m_UserSkill[i].sSid;	
			if(iSkillSid == SKILL_PP_DOWN) // PP 家葛樊 皑家		6 index
			{
				// 鉴荐 胶懦 饭骇 
				iLevel = m_UserSkill[i].tLevel;		
				if(iLevel < 0) iLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
				sDecPP += g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel);
			}
		}
	}

	sNeedPP = (short)((double)sNeedPP * (double)(1 - ((double)(sDecPP) / 100)) + 0.5);

	sSid = -1;
	sSid = m_UserItem[RIGHT_HAND].sSid;
	if(sSid >= 0 && sSid < g_arItemTable.GetSize())
	{
		sDecPP = 0;
		sDecPP = g_arItemTable[sSid]->m_byMPP;		// 酒捞袍俊 狼茄 PP家葛樊阑 皑家矫挪促.
		sNeedPP -= sDecPP;
	}

	if(sNeedPP <= 0) sNeedPP = 2;


	return sNeedPP;
}

////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 荤沥 芭府甫 备茄促.
//
int USER::GetPsiRange(short sSid)
{
	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) return -1;
	if(sSid >= g_arPsiTable.GetSize()) return -1;

	int nRange = g_arPsiTable[sSid]->m_tRange;

	return nRange;
}

/////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 某胶泼 掉饭捞甫 眉农茄促.
//
BOOL USER::CheckPsiCastingDelay()
{
	DWORD dwCurr = GetTickCount();
	DWORD dwDiff = dwCurr - m_dwLastPsiAttack;

	if(dwDiff < m_dwCastDelay) return FALSE;
	/*else 
	{
		m_dwCastDelay = 0;*/
		return TRUE;
	/*}*/
}

////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 己傍 咯何甫 搬沥茄促.
//
//BOOL USER::IsSuccessPsi(BOOL *bSuccessSkill, BYTE tWeaponClass, short sSid)
//{
//	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) return FALSE;
//
//	int iRandom = XdY(1, 100);
//
//	int iStaff = 0;
//	if(tWeaponClass == STAFF) iStaff = 1;
//
//	int iRegi		= g_arPsiTable[sSid]->m_tRegi;			// 历亲 啊瓷 咯何
//	DWORD dwCast	= g_arPsiTable[sSid]->m_sDelayTime;		// 某胶泼 掉饭捞
//
//	int iLevel = 0;
//	if(iStaff == 1 && bSuccessSkill[2] == TRUE) iLevel = m_UserSkill[tWeaponClass * SKILL_NUM + 2].tLevel; // 笼吝胶懦 饭骇
//
//	int iComp = (int)((double)m_sVOL * 0.7 + bSuccessSkill[2] * (2 * iLevel + 20) + 8 - iRegi * ((double)m_sMagicVOL / 5));
//
//	if(iRandom <= iComp) 
//	{
//		if(bSuccessSkill[4] == TRUE && iStaff == 1)			// 脚加 矫傈 胶懦
//			m_dwCastDelay = dwCast / 2;
//		else
//			m_dwCastDelay = dwCast;
//
//		m_dwLastPsiAttack = GetTickCount();
//		return TRUE;
//	}
//
//	return FALSE;
//}

////////////////////////////////////////////////////////////////////////////////////
//	魔法伤害计算....
//
BOOL USER::PsionicProcess(int nTarget, short sSid, CPoint pt, short sNeedPP)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	USER* pUser = NULL;
	CNpc* pNpc = NULL;

	BOOL bIsUser = FALSE;

	// skill
	double dAttack = 0;			// 荤捞坷葱 己傍伏 刘啊
	double dAssault = 0;		// 竿傍
	double dABDefense = 0;		// 例措 规绢
	double dPsyResist = 0;		// 荤捞坷葱 历亲
	double dnewAssault = 0;

	// 荤捞坷葱
	double dMightyWeapon = 0;	// Mighty Weapon
    double dBerserker = 0;
	
	int    iAssaultLevel = 0;
    int    iRandom = 0;

	// Item
	double dItemResist = 0;	
	int	iPsiAttackUp = 0;	// 瘤莆捞俊 崔妨乐绰 荤旦
	int iUpCount = 0;		// 诀弊饭捞靛 墨款飘

	double dDamage = 0;			// 扁夯 单固瘤
	double dDefense = 0;		// 规绢仿
	double dWideRangeAttackDamage = 0;
	int iExp = 0;
    
	DWORD dwExp = 0;

	BYTE tWeaponClass = 0;
	CPoint ptTel;

	CString strMsg = _T("");

	TCHAR szTempBuf[1024];	::ZeroMemory(szTempBuf, sizeof(szTempBuf));
	int index =0;
	int iRepeat = 0;
	int iEvadeRange = 14;
	int iEvadeRandom = 1;

	int iTime = 1;

	DWORD dwCurrTime = GetTickCount();

	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap) return FALSE;

	if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
	{
		pUser = GetUser(nTarget - USER_BAND);									// User Pointer 甫 掘绰促.
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)return FALSE;	// 肋给等 USER 捞搁 府畔
		if(pUser->m_bLive == USER_DEAD)	return FALSE;							// Target User 啊 捞固 磷绢乐栏搁 府畔

		bIsUser = TRUE;
	}
	else if(nTarget >= NPC_BAND)				// NPC
	{
		pNpc = GetNpc(nTarget - NPC_BAND);				// NPC Point 甫 掘绰促.
		if(pNpc == NULL) return FALSE;					// 肋给等 NPC 捞搁 府畔
		if(pNpc->m_NpcState == NPC_DEAD) return FALSE;	// NPC 啊 捞固 磷绢 乐栏搁 府畔
		if(pNpc->m_sHP <= 0) return FALSE;
		if(pNpc->m_tGuildWar == GUILD_WAR_AFFTER)
		{
			if(pNpc->m_tNpcType == NPCTYPE_GUILD_NPC || pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR) return FALSE;
		}
		if(pNpc->m_tNpcType != NPCTYPE_MONSTER)
		{
			if(pNpc->m_tGuildWar != GUILD_WARRING) return FALSE;	
		}
	}
	

	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) return FALSE;

	short sHoldTime	= g_arPsiTable[sSid]->m_sHoldTime;
	short sBasic	= g_arPsiTable[sSid]->m_sBasic;
	short sLevelUp	= g_arPsiTable[sSid]->m_sLevelUp;

//	if(m_sLevel >= ADD_USER_LEVEL) 
//		sBasic += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDamage;
	//sBasic += (int)((double)sBasic * (double)m_DynamicEBodyData[EBODY_PSI_ATTACK_UP] / 100);
	
	// 荤旦刘啊 ----------------------------------------------------------------------//
	iPsiAttackUp = iUpCount = 0;		
	if(m_UserItem[RIGHT_HAND].tIQ != UNIQUE_ITEM)	// 诀臂 酒捞袍阑 困秦
	{
		if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
		{
			if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byClass == STAFF_ITEM)
			{
				iUpCount = m_UserItem[RIGHT_HAND].tMagic[ITEM_UPGRADE_COUNT];
				if(iUpCount > 0 && iUpCount <= MAX_ITEM_UPGRADE_COUNT)
				{
					//iPsiAttackUp = (iUpCount * ATTACK_UPGRADE_PSI_BAND);
					//130级武器前6改13,后增加15
					if(Is130Wearp(sSid))
					{
						if(iUpCount<=6)
						{
							iPsiAttackUp = (iUpCount * 13);
						}else
						{
							iPsiAttackUp = (6 * 13)+(iUpCount-6)*15;
						}
					}else //if(Is100Wearp(sSid))
					{//100级武器前9改+4,后增加+10
						if(iUpCount<=9)
						{
							iPsiAttackUp = (iUpCount * 4);
						}else
						{
							iPsiAttackUp = (4 * 9)+(iUpCount-(6))*10;
						}
					}
				}
			}
		}
	}
	iPsiAttackUp +=sBasic + m_DynamicUserData[MAGIC_PSI_ATTACK_UP] + (int)((double)m_DynamicEBodyData[EBODY_VOL_TO_PSI_DAMAGE] / 100 * (double)m_sMagicVOL);
	iPsiAttackUp +=(int)(iPsiAttackUp *(double)m_DynamicEBodyData[EBODY_PSI_ATTACK_UP] / 100);
    
	if (m_byClass == 1)  //法师135级魔法增加180魔攻
	{
		if(m_dwYINENgTime != 0) iPsiAttackUp += 100;
	}
	if(m_sLevel>=70 )
		iPsiAttackUp += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDamage;
	
	//魔法攻击加成
	iPsiAttackUp = (int)(iPsiAttackUp* 0.4);
	switch(sSid)	// 傍拜 荤捞坷葱 贸府
	{
	case 23://物理职业必杀.
	case 25:
	case 26: //必杀伤害
	case 44:
	case 54:
	case PSI_BISA_4:
		{
			BOOL	bCanUseSkill = FALSE;
			byte tWeaponClass;
			bCanUseSkill = IsCanUseWeaponSkill(tWeaponClass);	
			if(!bCanUseSkill)//如果没有57 则返回
				return FALSE;
			EBODY_Skill(sSid,pUser,pNpc,bIsUser);
		}
		break;
/*	case 54://战神技能
		{   
			if(m_iSkin != 8 && m_iSkin != 170) return FALSE;//不是战神则无效
            Ddan_Spell(sSid,pUser,pNpc,bIsUser);
		}
		break;*/
	case 2: // 鬼火
	case 4:	// 治疗
	case 8: //8=狂雷8格损伤 
	case 5:	// 灼焰
	case 10:case 35:case 36:case 71://10=光之剑 36=暴炎 36=罡气 辐射能
	case 24:case 45://法师必杀
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return FALSE; //yskang 0.5
	if(m_iDisplayType == 6) return FALSE; //yskang 0.5
#endif
	double EBODYSkill=1.1;
	
		dAttack		= GetPsiAttackUp();		// 取得法师精修伤..
		dDamage  = (double)( 0.5 + m_sLevel * ((double)sLevelUp / 100)+m_sMagicVOL +iPsiAttackUp);	// 扁夯 单固瘤
		if(sSid!=24)
			dAssault	= GetPsyAssault(FALSE);//普通魔法攻击
		else{
			dAssault	= GetPsyAssault(true);//必杀魔法 攻击.增加10%伤害
			EBODYSkill=1.1;

			if(m_curz ==67 && m_isUser == FALSE)		//PK竞赛场禁止开赛前利用外挂释放必杀的BUG
			return FALSE;
		
		}
		dDamage = (dDamage + (double)( dDamage * (double)( dAttack * 0.01 ) )* (double)( dAssault * 0.01 ))*EBODYSkill;
		dDamage=dDamage+dDamage*(myrand(1,100)%10)/100;
		//最终伤害增加计算
		dDamage=dDamage+m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP];
	//	dDamage=dDamage+m_DynamicMagicItem[7];
   
		if( m_dwMightyWeaponTime != 0 )	dMightyWeapon = (double)( (double)(dDamage) * 0.1 );
		if( m_dwBerserkerTime != 0 )		dBerserker = (double)( (double)(dDamage) * 0.15 );
		dWideRangeAttackDamage = dDamage;
		TRACE( "魔法攻击 %lf\n", dDamage);
	
		if(bIsUser)
		{
			if( pUser->m_strUserID == m_strUserID ) return FALSE;
			//if( dwCurrTime - pUser->m_dwLastPsiDamagedTime < PSI_DAMAGE_DELAY) return FALSE;
			//////////////////////////////////////////////////交易密码
			if(!m_MItemLock  && (!IsCity()) && o_yehuoini[0]->mimabaohu == 1){ SendSystemMsg( "解除[安全锁]后,才能攻击.", SYSTEM_ERROR, TO_ME);return FALSE;}
			if((m_dwGuild == pUser->m_dwGuild) && (!IsCity()) && m_Bmws==1 ){ SendSystemMsg( "对方与您同属一个军团,请在内挂关闭避免误伤功能", SYSTEM_ERROR, TO_ME);return FALSE;}

			if(pUser->m_bSessionOnline == true && (!IsCity()))	return FALSE;
		//	if(pUser->m_dwBHTime !=0 )  { SendEventMsg( "对方闭关保护中,无法攻击!."); return FALSE;}
		  //  if (m_dwBHTime !=0)  { SendEventMsg( "您当前是在闭关保护中无法攻击玩家!."); return FALSE;}
			if(m_UserItem[4].tMagic[4] != 0)  
			{
			//	SendSystemMsg( "武器属性出现意外,自动.", SYSTEM_ERROR, TO_ME);
				m_UserItem[4].tMagic[4] = 0;
				return FALSE;
			}
			/////////////////////////////////////////////////////////

			dABDefense	= pUser->GetPsyAbsoluteDefense();	// 例措规绢 胶懦俊 狼茄 荤捞坷葱 规绢仿 刘啊
			dItemResist = 50 + pUser->GetUserSpellDefence();
			// 荤捞坷葱 历亲仿
			if(pUser->m_dwPsiShieldTime != 0 || pUser->m_dwBigShieldTime !=0)				// 付规 蒋靛 乐阑 锭
			{
				dItemResist = dItemResist * 1.5;
			}
			dItemResist += dItemResist * (double)( dABDefense * 0.01 ) ;//技能的魔抗
			dDefense = (double)( (double)(m_sMagicVOL)/(double)( m_sMagicVOL + pUser->m_sMagicVOL + dItemResist/2) );
			if(dDefense>1)
				dDefense=1.00;
			if( m_dwMightyWeaponTime != 0 )	dMightyWeapon = (double)( (double)(dDamage) * 0.1 );
			if( m_dwBerserkerTime != 0 )		dBerserker = (double)( (double)(dDamage) * 0.15 );
			dDamage = dDamage * dDefense + iPsiAttackUp - dItemResist/2;
			//魔法伤害减少.最终伤害减少
			dDamage=dDamage-(pUser->m_DynamicUserData[MAGIC_PSI_ATTACK_DOWN]+pUser->m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]);
			//dDamage=dDamage-(pUser->m_DynamicMagicItem[5]+pUser->m_DynamicMagicItem[4]);
			if(dDamage<15)
				dDamage=15;
			BOOL bIsLegal = IsLegalDefence_isdead(pUser);
			pUser->SetDamage((int)dDamage);	//
		
			//SendDamageNum(0,nTarget,(short)dDamage);
			SendDamageNum(0,pUser->m_uid + USER_BAND,(short)dDamage);//显血修改处
			 pUser->SendDamageNum(0,m_uid+USER_BAND,(short)dDamage);
			//if(m_ShowHP==1){
				CString strMsg;
				int iMaxHP=pUser->m_sMaxHP;
				if(m_sMagicMaxHP > m_sMaxHP) iMaxHP = pUser->m_sMagicMaxHP;
				strMsg.Format("%s(%d) HP= %d \\ %d",pUser->m_strUserID,pUser->m_uid + USER_BAND,pUser->m_sHP,iMaxHP);
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			
			//}
			if(pUser->m_sHP > 0)			// 磷瘤臼篮 版快 眠啊单固瘤
			{
				if(sSid == 2) pUser->SetColdDamage();	// Cold
				if(sSid == 4) pUser->SetFireDamage();	// Fire
			}


			else if(pUser->m_lDeadUsed == 1)
			{	
                if ( m_curz == 66)
				{
					m_dwAutoMoney += 1;
					CString strMsg = _T("");
					if (m_dwAutoMoney <= 20)
					{
					    GiveItemAll(724,10,0,0,0,0,0,0,0,0,0,0,0);
						//GiveDN(200000);
						//SendMoneyChanged();
						UpdateUserItemDN();
						strMsg.Format( "当前杀人数是:[ %d ]个.成功击杀1玩家,奖励标志10个", m_dwAutoMoney);
						SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NPC, TO_ME);
					}
					else
					{
						strMsg.Format( "当前杀人数是:[ %d ]个", m_dwAutoMoney);
						SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NPC, TO_ME);
					}
				}
				if(m_tBabyCall ==1)//如果有宠物
                {
					if ( m_curz == 66 || m_curz == 67 || IsInPKC()) return FALSE;//活动地图不加忠诚度
                    if(m_sFealty != 100)//忠诚度不是满的
					{
                        if((m_sFealty + 1) < 101) m_sFealty += 1;
						SendEventMsg("成功击杀1名玩家,宠物[忠诚度]增加[ 1 ]");
						SendBabyInfo();
					}  
				}
			int type = 0;
        	type = CheckInvalidMapType();
			if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING) //只有审请军团才掉市民等级	
        	if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16)  bIsLegal = TRUE; 	
				int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
				if(bIsLegal == FALSE)//yskang 0.7 沥寸 规困扼搁 cityrank绰 贸府 窍瘤 臼绰促.
					IsChangeCityRank(m_sCityRank, pUser);
				pUser->GetLevelDownExp(USER_PK, tempRank, FALSE,m_strUserID);			// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
				
                strMsg.Format( IDS_USER_ATTACK_FROM, m_strUserID,pUser->m_strUserID);

				if(m_curz != 40 && m_curz != 45 && m_curz != 58) {pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ALL);}
///////////////////////////杀人电光提醒/////////////////////////////////////////////////
	//		    char sendMsg[200] = "";
	////			strcat_s(sendMsg,"PK中, ");
	//			strcat_s(sendMsg,m_strUserID);
	//			strcat_s(sendMsg," 杀害了 ");
	//			strcat_s(sendMsg,pUser->m_strUserID);								
	//			CBufferEx	TempBuf;
	//			TempBuf.Add((byte)0x1f);
	//			TempBuf.Add((byte)1);
	//			TempBuf.Add((byte)0x25);
	//			TempBuf.Add((DWORD)0);
	//			TempBuf.AddString("PK战报");
	//			TempBuf.AddString(sendMsg);
	//			SendAll(TempBuf, TempBuf.GetLength());	
///////////////////////////杀人电光提醒//////////////////////////////////////////////////
			}

			// 扼捞飘醋鞘靛
			if(sSid == 8) 
			{
				GetWideRangeAttack(pUser->m_curx, pUser->m_cury, (int)dWideRangeAttackDamage);	//狂雷周围伤害
			}

			::InterlockedExchange((LPLONG)&pUser->m_dwLastPsiDamagedTime, (LONG)dwCurrTime);
		}
		else	// NPC
		{
            if(pNpc->m_tNpcType != NPCTYPE_MONSTER)
			{
				if(pNpc->m_tGuildWar != GUILD_WARRING) return FALSE;	
			}
			dDamage = dDamage * ((double)myrand(80, 110) / 100);
			//140级杀怪伤害
			dDamage += m_Dynamic140[0];

			if(pNpc->SetDamage((int)dDamage, m_uid + USER_BAND, m_pCom) == FALSE)
			{
				pNpc->SendExpToUserList(m_pCom); // 版氰摹 盒硅!!
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel); //怪物等级相差30级没经验
				if(diffLevel <= o_yehuoini[0]->djxz || pNpc->m_sEvent >= 30000 )
				{
					if (m_sLevel>= 70)
						m_dwShopPingDN += 1;//每杀死怪物增加杀怪物数
					if (m_isDoubleBAOLV  > 0 || m_dwGuarDianTianShi > 0)
						{
					pNpc->SendDead(m_pCom,1,TRUE);
					 Diaoluo();
			           }  
						
					else
						{pNpc->SendDead(m_pCom);
				
		                 Diaoluo();	    
					    }
				}
				else
				{
					pNpc->SendDead(m_pCom,0);
					SendSystemMsg("您和当前怪物等级相差20,没有任何经验和道具掉落", SYSTEM_ERROR,TO_ME);
				}

				if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
				{
					CheckGuildHouseWarEnd();
				}
				else
				{
					if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();

					int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
					if(diffLevel < 40)
					{
						m_iCityValue=m_iCityValue+250;
						GetCityRank();
						CheckMaxValue(m_dwXP, 1);		// 各捞 磷阑锭父 1 刘啊!	
						SendXP();
						SendUserStatusSkill();
					}
				}
				
			}
			else								// 磷瘤臼篮 版快 眠啊单固瘤
			{
				if(sSid == 2) pNpc->SetColdDamage();	// Cold
			}
			SendNpcHP(pNpc->m_sNid+NPC_BAND,pNpc->m_sHP);
				SendDamageNum(1,pNpc->m_sNid+NPC_BAND,(short)dDamage);
	

///////////////////////////法师群攻触发开始//////////////////////////////////////////////////////////	
		int Damage = 0;	//用于群攻伤害值变量
		if(sSid == 2 ||sSid == 4 ||sSid == 5 ||sSid == 8)
	    {
	/*	  if((m_UserItem[4].sSid == 1578) || (m_UserItem[4].sSid == 1579) || (m_UserItem[4].sSid == 1580))  //1586 && m_UserItem[4].sDuration > 0) 
		  {*/
		  if(m_tBabyCall ==1)//首先得是有宠
          {
		    if(m_sFealty == 100)//宠忠诚度是
            {
			int iRandom = myrand(0, 300);			
			if(iRandom > 145 && iRandom < 150) 
             {
     		      if(m_byClass == 1)
			       Damage =GetUserSpellAttack(TRUE);
		          else
			       Damage =GetMaxDamage(m_byClass);
		       if(GetQunGongAttack( m_curx,  m_cury,  Damage)>=1)
			   {
			//    SendSystemMsg("范围攻击技能释放成功.", SYSTEM_ERROR, TO_ME);
			   }
		     }
           }
		  }
		 }
////////////////////////////////法师群攻结束////////////////////////////////////////////////////////
			if(m_ShowHP==1){
				CString strMsg;
				strMsg.Format("%s(%d) HP= %d \\ %d",pNpc->m_strName,pNpc->m_sNid + NPC_BAND,pNpc->m_sHP,pNpc->m_sMaxHP);
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			}
			// 扼捞飘醋鞘靛
			if(sSid == 8) 
			{
				GetWideRangeAttack(pNpc->m_sCurX, pNpc->m_sCurY, (int)dWideRangeAttackDamage);	//裹困 荤捞坷葱俊狼茄 傍拜 贸府
			}
		}
		break;

	}

	switch(sSid)
	{
	// 拜捧拌凯 -------------------------------------------------------------------------------//
	case 0:	//Recovery	: SP 雀汗
		if(bIsUser) 
		{
			pUser->m_sSP += (int)(0.5 + sBasic + m_sLevel * ((double)sLevelUp / 100));	// jjs07 2001.11.23 
			//pUser->m_sSP += 20 + m_sMagicVOL / 4;		// old
			if(pUser->m_sSP > pUser->m_sMagicMaxSP) pUser->m_sSP = pUser->m_sMagicMaxSP;
			pUser->SendSP();
		}
		// NPC 牢 版快绰 泅犁鳖瘤绰 公矫
		else return FALSE;
		break;

	case 1:	// 庆捞胶飘	: 傍拜加档 氢惑 (傍拜 掉饭捞 皑家)
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
			pUser->SetHaste(iTime);
		}
		// NPC 牢 版快绰 泅犁鳖瘤绰 公矫
		else return FALSE;
		break;

	case PSIONIC_FAST_RUN:	// Fast Run	: 捞悼加档 氢惑
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
			pUser->SetFastRun(iTime);
		}
		// NPC 牢 版快绰 泅犁鳖瘤绰 公矫
		else return FALSE;
		break;

	// 瘤莆捞 拌凯 ----------------------------------------------------------------------------//
	case 3:	// 鳃傅
		if(bIsUser) 
		{
			
			pUser->m_sHP += (int)(0.5 + sBasic + m_sLevel * ((double)sLevelUp / 100));	// jjs07 2001.11.23 
			if(pUser->m_sHP > pUser->m_sMagicMaxHP) pUser->m_sHP = pUser->m_sMagicMaxHP;
			pUser->SendHP();
		}
		// NPC 牢 版快绰 泅犁鳖瘤绰 公矫
		else return FALSE;

		break;

	case 6:	// 蒋靛
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetShield(iTime);	// old
			//pUser->SetShield(m_sMagicVOL);
		}
		// NPC 牢 版快绰 泅犁鳖瘤绰 公矫
		else return FALSE;
		break;


	case 30:	// 蒋靛
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetBigShield(iTime);	// old
			//pUser->SetShield(m_sMagicVOL);
		}
		// NPC 牢 版快绰 泅犁鳖瘤绰 公矫
		else return FALSE;
		break;

	case 7: // 炮饭器飘 
		iRepeat = 0;
//		ptTel = ConvertToServer(pt.x, pt.y);
//		if(ptTel.x < 0 || ptTel.y < 0) return FALSE;

		do
		{
//			type = ((g_zone[m_ZoneIndex]->m_pMap[ptTel.x][ptTel.y].m_dwType & 0xFF00 ) >> 8);
//			if(!CheckInvalidZoneState(type)) { iRepeat++; continue; }

//			if(Teleport(ptTel.x, ptTel.y) == FALSE) iRepeat++;
			if(Teleport(pt.x, pt.y) == FALSE) iRepeat++;
			else break;

		}while(iRepeat < 2);

		break;

	case PSIONIC_MIND_SHOCK:	// Mind Shock
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
//			if(pUser->SetMindShock(iTime) == TRUE) SendSystemMsg(IDS_MIND_SHOCK_SUCCESS, SYSTEM_NORMAL, TO_ME );
			if(pUser->SetMindShock(iTime) == TRUE) SendSystemMsg("心灵风暴使用成功.", SYSTEM_NORMAL, TO_ME );
		}

		break;

	case PSIONIC_MIND_GUARD:	// Mind Guard
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
			pUser->SetMindGuard(iTime);
		}

		break;

	// 档八拌凯 ------------------------------------------------------------------------------//
	case 9:	
		if(bIsUser)	// USER
		{
			int type = 0;
			if(m_curz==61) 
			{
				SendSystemMsg( "PK赛场地无法使用闪避技能.", SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
			type = CheckInvalidMapType();
			if(type == 8 || type == 10 || type == 15 || type == 17 || type == 9 || type == 11 || type == 16 || type == 18) 
				{
				SendEventMsg( "要塞战区内无法使用闪避魔法!.");
				return FALSE;
			}
			 if(m_dwLhslTime != 0 && (!IsCity()))
		   {
				SendEventMsg( "灵魂锁链状态下(5秒)无法使用闪避技能.");
				return FALSE;
		   }

			if(pUser->m_strUserID != m_strUserID) return FALSE;	// 磊扁磊脚老 版快俊父
			
			iRepeat = 0;
			iEvadeRandom = myrand(1, 5);
			do
			{
				int min_x = m_curx - iEvadeRange * iEvadeRandom; if(min_x < 0) min_x = 0;
				int max_x = m_curx + iEvadeRange * iEvadeRandom; if(max_x >= pMap->m_sizeMap.cx) max_x = pMap->m_sizeMap.cx - 1;
				int min_y = m_cury - iEvadeRange * iEvadeRandom; if(min_y < 0) min_y = 0;
				int max_y = m_cury + iEvadeRange * iEvadeRandom; if(max_y >= pMap->m_sizeMap.cy) max_y = pMap->m_sizeMap.cy - 1;
				
				int move_x = 0, move_y =0;
				
				move_x = myrand(min_x, max_x);
				move_y = myrand(min_y, max_y);
	
				type = ((g_zone[m_ZoneIndex]->m_pMap[move_x][move_y].m_dwType & 0xFF00 ) >> 8);
					
				if(!CheckInvakidZoneChangeState(type)) { iRepeat++; continue; }

				if(Teleport(move_x, move_y) == FALSE) iRepeat++;
				else break;
				
			}while(iRepeat < 6);
		}
		else return FALSE;
		break;

	case PSIONIC_PSI_SHIELD:		// 荤捞坷葱 蒋靛
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetPsiShield(iTime);
		}
		else return FALSE;
		break;

	// 醚扁 -------------------------------------------------------------------------------------------//
	case 11:	// Dex Up
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetDexUp(iTime);
		}
		else return FALSE;
		break;

	case 12:	// Max HP Up
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
			pUser->SetMaxHPUp(iTime);
		}
		else return FALSE;
		break;

	case PSIONIC_PIERCING_SHIELD:	// 蒋靛 墩扁
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetPiercingShield(iTime);
		}
		else return FALSE;

		break;

	// 公拌凯 -----------------------------------------------------------------------------------------//
	case 13:	// Adamantine
		if(bIsUser)
		{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return FALSE; //yskang 0.5
	if(m_iDisplayType == 6) return FALSE; //yskang 0.5
#endif
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100 ));	// alisia 2002.03.21
			pUser->SetAdamantine( iTime );
		}
		else return FALSE;
		break;

	case 14:	// MightyWeapon
		if(bIsUser)
		{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return FALSE; //yskang 0.5
	if(m_iDisplayType == 6) return FALSE; //yskang 0.5
#endif
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100 ));	// alisia 2002.03.21
			pUser->SetMightyWeapon( iTime );
		}
		else return FALSE;
		break;

	case 15:	// Berserker
		if(bIsUser)
		{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return FALSE; //yskang 0.5
	if(m_iDisplayType == 6) return FALSE; //yskang 0.5
#endif
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100 ));	// alisia 2002.03.21
			pUser->SetBerserker( iTime );///ymc
		}
		else return FALSE;
		break;
	case 64:		//3分钟 超级反弹 Lv 135 攻击力提升20%
		{
			if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
			{
				pUser = GetUser(nTarget - USER_BAND);		
				if(pUser->m_strUserID != m_strUserID) return FALSE;	// 只能对自身释放此魔法
				if(pUser)
				{
				    pUser->m_dwFANTAnTime = 300 * 1000;
					pUser->m_dwLastFANTAnTime = GetTickCount();
				    pUser->AddHuFaStatus(New_Status_12);
					pUser->SendJudgePsiStatus();
				    SendUserStatusSkill();
			
					
				}
			}		
			break;
		}
	case 65:		//3分钟 超级锋利 Lv 135 攻击力提升20%
		{
			if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
			{
				pUser = GetUser(nTarget - USER_BAND);	
				if(pUser->m_strUserID != m_strUserID) return FALSE;	// 只能对自身释放此魔法
				if(pUser)
				{
				    pUser->m_dwFENGLiTime = 300 * 1000;
					pUser->m_dwLastFENGLiTime = GetTickCount();
				    pUser->AddHuFaStatus(New_Status_14);
					pUser->SendJudgePsiStatus();
				    SendUserStatusSkill();
			
					
				}
			}		
			break;
		}
	case 66:		//3分钟 超级异能 Lv 135 攻击力提升20%
		{
			if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
			{
				pUser = GetUser(nTarget - USER_BAND);		
				if(pUser->m_strUserID != m_strUserID) return FALSE;	// 只能对自身释放此魔法
				if(pUser)
				{
				    pUser->m_dwYINENgTime = 300 * 1000;
					pUser->m_dwLastYINENgTime = GetTickCount();
				    pUser->AddHuFaStatus(New_Status_13);
					pUser->SendJudgePsiStatus();
				    SendUserStatusSkill();
			
					
				}
			}		
			break;
		}
	case 67:		
		{
			if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
			{
				pUser = GetUser(nTarget - USER_BAND);	//3分钟 超级多重 Lv 135 攻击力提升20%
				if(pUser->m_strUserID != m_strUserID) return FALSE;	// 只能对自身释放此魔法
				if(pUser)
				{
				    pUser->m_dwDUOCHONgTime = 300 * 1000;
					pUser->m_dwLastDUOCHONgTime = GetTickCount();
				    pUser->AddHuFaStatus(New_Status_15);
					pUser->SendJudgePsiStatus();
				    SendUserStatusSkill();
			
					
				}
			}		
			break;
		}
	case PSI_JUDGE_FU_SHE_LEN:	  //辐射能 lv10
		{
			break;
		}
	case PSI_JUDGE_CALL_YI_TU_LUO://召唤伊涂罗lv20
		{
			if(m_sLevel<20)
				return FALSE;
			if(m_tHuFaType != 0)
				return FALSE;
			return CallHuFa(0x01);
			break;
		}
	case PSI_JUDGE_CONG_RAN://重燃lv30
		{
			//无护法无法使用此魔法
			if(m_tHuFaType == 0)
				return FALSE;
			if(m_nHuFaHP>=m_nHuFaMaxHP)
				break;
			int nBase = 500;
			nBase+=(m_sLevel*5);

			m_nHuFaHP += nBase;
			if(m_nHuFaHP>m_nHuFaMaxHP)
				m_nHuFaHP = m_nHuFaMaxHP;

			SendHuFaHPInfo();
			break;
		}
	case PSI_JUDGE_CALL_PU_LEI_TA://召唤普雷塔lv90
		{
			if(m_sLevel<90)
				return FALSE;
			if(m_tHuFaType != 0)
				return FALSE;

			return CallHuFa(0x02);
			break;
		}
	case PSI_JUDGE_CALL_KE_LAO_SI://召唤克劳斯lv125
		{
			if(m_sLevel<125)
				return FALSE;
			if(m_tHuFaType != 0)
				return FALSE;
			return CallHuFa(0x03);
			break;
		}
	case PSI_JUDGE_CALL_YI_XI_SI://召唤伊西斯lv150
		{
			if(m_sLevel<150)
				return FALSE;
			if(m_tHuFaType != 0)
				return FALSE;
			return CallHuFa(0x04);
			break;
		}
	case PSI_JUDGE_WIS_UP:		//5分钟 攻击力提升-智力lv50
		{
			//无护法无法使用此魔法
			//if(m_tHuFaType == 0)
			//	return FALSE;

			if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
			{
				pUser = GetUser(nTarget - USER_BAND);								
				if(pUser)
				{
					pUser->m_dwWISUpTime = 300 * 1000;
					pUser->m_dwLastWISUpTime = GetTickCount();
					pUser->AddHuFaStatus(JUDGE_ABNORMAL_18);
					pUser->SendJudgePsiStatus();
					
				}
			}		
			break;
		}
	case PSI_JUDGE_LI_HU_SHUO_LIAN:		//灵魂锁链
		{
			if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
			{
				pUser = GetUser(nTarget - USER_BAND);		// USER						
			
					pUser->m_dwLhslTime = 5000;
					pUser->m_dwLastLhslTime = GetTickCount();
					pUser->AddHuFaStatus(New_Status_19);
					pUser->SendJudgePsiStatus();
					CString strMsg1 = _T("");
			        strMsg1.Format( "您使用[灵魂锁链]将玩家【%s】束缚了!",pUser->m_strUserID);
			        SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_NORMAL, TO_ME);
			        strMsg1 = _T("");
			        strMsg1.Format( "您被玩家【%s】使用[灵魂锁链]束缚了,无法移动5秒!",m_strUserID);
			        pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_NORMAL, TO_ME);
			 } 
			 else if(nTarget >= NPC_BAND) return FALSE;//不能对怪物发
		  break;
		}
	case PSI_JUDGE_ATTACK_UP://攻击力提升-敏捷lv50
		{
			if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
			{
				pUser = GetUser(nTarget - USER_BAND);								
				if(pUser)
				{
					pUser->m_dwDexUpTime = 300 * 1000;
					pUser->m_dwLastDexUpTime = GetTickCount();
					//pUser->AddHuFaStatus(JUDGE_ABNORMAL_18);
					//pUser->SendJudgePsiStatus();
				}
			}	
			break;
		}
	case PSI_JUDGE_QIANG_LI_CONG_RAN://强力重燃
		{
			//无护法无法使用此魔法
			if(m_tHuFaType == 0)
				return FALSE;
			if(m_nHuFaHP>=m_nHuFaMaxHP)
				break;

			int nBase = 1000;
			nBase+=(m_sLevel*5);
			
			m_nHuFaHP += nBase;
			if(m_nHuFaHP>m_nHuFaMaxHP)
				m_nHuFaHP = m_nHuFaMaxHP;
			SendHuFaHPInfo();
			break;
		}
	case PSI_JUDGE_XI_SHEN_HU_FA://牺牲护法
		{
			//无护法无法使用此魔法
			if(m_tHuFaType == 0)
				return FALSE;
			AddHuFaStatus(JUDGE_ABNORMAL_20);

			SendJudgePsiStatus();
			SendHuFaPostion(HUFA_DEA);
			m_dwXiShengTime = 5*60*1000;//五分钟牺牲护法
			m_dwLastXiShendTime = GetTickCount();
			break;
		}
	case PSI_JUDGE_LEI_XIAN_FENG_BAO://烈焰风暴
		{
			//无护法无法使用此魔法
			if(m_tHuFaType == 0)
				return FALSE;
			int attBase = 500;
			attBase +=m_sLevel;
			switch(m_tHuFaType)
			{
			case 0x01:
				attBase+=100;
				break;
			case 0x02:
				attBase+=200;
				break;
			case 0x03:
				attBase+=400;
				break;
			case 0x04:
				attBase+=600;
				break;
			}
			//群攻伤害
			SendWideRangeAttack(m_curx, m_cury,sSid,attBase);
			//SendHuFaPostion(HUFA_DEA);
			break;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//必杀伤害计算

void USER::EBODY_Skill(short sSid,USER *pUser ,CNpc* pNpc,BOOL bIsUser)
{
	BOOL	bCanUseSkill = FALSE;
	BOOL	bIsCritical = FALSE;
	byte tWeaponClass;
	int nDamage,nFinalDamage,nDefense;
	double EBODYSkill=1.05;
	CString strMsg;
	
     bCanUseSkill = IsCanUseWeaponSkill(tWeaponClass);	
	 if(!bCanUseSkill)//如果没有57 则返回
		 return ;
	if(m_byClass==0) EBODYSkill=1;
	if(m_byClass==3)  EBODYSkill=1;
	if(m_byClass==4)  EBODYSkill=1.10;
	if(bIsUser)	// USER
	{
		//计算防 ----------------------------------------------------------------//
		//////////////////////////////交易密码
		if(!m_MItemLock &&  (!IsCity()) && o_yehuoini[0]->mimabaohu == 1 ){ SendSystemMsg( "解除[安全锁]后,才能攻击.", SYSTEM_ERROR, TO_ME);return;}
		if((m_dwGuild == pUser->m_dwGuild) && (!IsCity()) && m_Bmws==1 ){ SendSystemMsg( "对方与您同属一个军团,请在内挂关闭避免误伤功能", SYSTEM_ERROR, TO_ME);return;}

	//	if(pUser->m_dwBHTime !=0 )  { SendEventMsg( "对方闭关保护中,无法攻击!.");return;}
	//	if (m_dwBHTime !=0)  { SendEventMsg( "您当前是在闭关保护中无法攻击玩家!.");return;}
	     if(pUser->m_bSessionOnline == true && (!IsCity()))	return;
		/////////////////////////////////////////
		nDamage =(int)( GetNormalInitDamage(tWeaponClass, pUser, bIsCritical,true)*EBODYSkill);//计算总攻击.
		if(nDamage < 0) nDamage = 0;
		if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) nDamage += 10;
		nFinalDamage = GetFinalDamage(pUser, nDamage, tWeaponClass,bIsCritical,1);//计算最终伤害
		//最终伤害增加计算  和最终伤减少
		nFinalDamage=nFinalDamage+m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP];
		//nFinalDamage=nFinalDamage+m_DynamicMagicItem[7];
		nFinalDamage=nFinalDamage-(pUser->m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+pUser->m_DynamicUserData[MAGIC_PHY_ATTACK_DOWN]);
		//nFinalDamage=nFinalDamage-(pUser->m_DynamicMagicItem[5]+pUser->m_DynamicMagicItem[6]);
		//________________________________________________
		if(nFinalDamage < 0) nFinalDamage = 0;
		BOOL bIsLegal = IsLegalDefence_isdead(pUser);	
		pUser->SetDamage(nFinalDamage);
		//if(m_ShowHP==1){
			
			int iMaxHP=pUser->m_sMaxHP;
			if(m_sMagicMaxHP > m_sMaxHP) iMaxHP = pUser->m_sMagicMaxHP;
			strMsg.Format("%s(%d) HP= %d \\ %d",pUser->m_strUserID,pUser->m_uid + USER_BAND,pUser->m_sHP,iMaxHP);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		//}

		
		if(pUser->m_lDeadUsed == 1)
		{			
			if ( m_curz == 66)
			{
				m_dwAutoMoney += 1;
				CString strMsg = _T("");
				if (m_dwAutoMoney <= 20)
				{
					//GiveDN(200000);
					GiveItemAll(724,10,0,0,0,0,0,0,0,0,0,0,0);
					//SendMoneyChanged();
					UpdateUserItemDN();
					strMsg.Format( "当前杀人数是:[ %d ]个.成功击杀1玩家,奖励10个标志", m_dwAutoMoney);
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NPC, TO_ME);
				}
				else
				{
					strMsg.Format( "当前杀人数是:[ %d ]个.", m_dwAutoMoney);
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NPC, TO_ME);
				}
			}
			if(m_tBabyCall ==1)//如果有宠物
                {
					if ( m_curz == 66 || m_curz == 67 || IsInPKC()) return;//活动地图不加忠诚度
                    if(m_sFealty != 100)//忠诚度不是满的
					{
                        if((m_sFealty + 1) < 101) m_sFealty += 1;
						SendEventMsg("成功击杀1名玩家,宠物[忠诚度]增加[ 1 ]");
						SendBabyInfo();
					}  
				}
			int type = 0;
        	type = CheckInvalidMapType();
			if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING) 
        	if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16)  bIsLegal = TRUE;
			int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
			if(bIsLegal == FALSE)//yskang 0.7 沥寸 规困扼搁 cityrank绰 贸府 窍瘤 臼绰促.
				IsChangeCityRank(m_sCityRank, pUser);
			pUser->GetLevelDownExp(USER_PK, tempRank, FALSE,m_strUserID);			// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
			
			strMsg.Format( IDS_USER_ATTACK_FROM, m_strUserID,pUser->m_strUserID);
			if(m_curz != 40 && m_curz != 45 && m_curz != 58) {pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ALL);}
///////////////////////////杀人电光提醒/////////////////////////////////////////////////
	//		    char sendMsg[200] = "";
	////			strcat_s(sendMsg,"PK中, ");
	//			strcat_s(sendMsg,m_strUserID);
	//			strcat_s(sendMsg," 杀害了 ");
	//			strcat_s(sendMsg,pUser->m_strUserID);								
	//			CBufferEx	TempBuf;
	//			TempBuf.Add((byte)0x1f);
	//			TempBuf.Add((byte)1);
	//			TempBuf.Add((byte)0x25);
	//			TempBuf.Add((DWORD)0);
	//			TempBuf.AddString("PK战报");
	//			TempBuf.AddString(sendMsg);
	//			SendAll(TempBuf, TempBuf.GetLength());	
///////////////////////////杀人电光提醒//////////////////////////////////////////////////
		}
	}else{
		nDefense = pNpc->GetDefense();
		nDamage = (int)(GetNormalInitDamage(tWeaponClass, NULL,bIsCritical,true)*1.05);
		if(nDamage < 0) nDamage = 0;
		double nVCHit = 0.0;
		nVCHit = GetCriticalHit(tWeaponClass,1);
		if(nVCHit > 0.0) bIsCritical = TRUE;
		nFinalDamage = (int)((double)nDamage - ((double)nDefense * nVCHit));
		//最终伤害增加计算  和最终伤减少
		nFinalDamage=nFinalDamage+m_DynamicMagicItem[7];
        //140猎杀
		nFinalDamage += m_Dynamic140[0];
		if(nFinalDamage < 0) nFinalDamage = 0;
		short sOldNpcHP = pNpc->m_sHP;
		//发送对怪物伤害数值
		/*
		CString strMsg;
		strMsg.Format("您产生攻击是 %d,您对 %s 最终伤害是 %d",nDamage,pNpc->m_strName,nFinalDamage);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		*/
		//		if(pNpc->SetDamage(nFinalDamage, m_strUserID, m_uid + USER_BAND, m_pCom) == FALSE)
		if(pNpc->SetDamage(nFinalDamage, m_uid + USER_BAND, m_pCom) == FALSE)
		{
			pNpc->SendExpToUserList(m_pCom); // 版氰摹 盒硅!!
			int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel); //怪物等级相差30级没经验
			if(diffLevel <= o_yehuoini[0]->djxz || pNpc->m_sEvent >= 30000 )
			{
				if (m_sLevel>= 70)
					m_dwShopPingDN += 1;//每杀死怪物增加杀怪物数
				if (m_isDoubleBAOLV  > 0 || m_dwGuarDianTianShi > 0)
					
					{
					pNpc->SendDead(m_pCom,1,TRUE);
					 Diaoluo();
			    }
					
					
				else
					{
		 			pNpc->SendDead(m_pCom);
		               Diaoluo();		
					}
			}
			else
			{
				pNpc->SendDead(m_pCom,0);
				SendSystemMsg("您和当前怪物等级相差20,没有任何经验和道具掉落", SYSTEM_ERROR,TO_ME);
			}

			if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
			{
				CheckGuildHouseWarEnd();
			}
			else
			{
				if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(diffLevel < 40)
				{
					m_iCityValue=m_iCityValue+250;
					GetCityRank();
					CheckMaxValue(m_dwXP, 1);		// 各捞 磷阑锭父 1 刘啊!	
					SendXP();
					SendUserStatusSkill();
				}
			}
		}

		 
		SendNpcHP(pNpc->m_sNid+NPC_BAND,pNpc->m_sHP);
		SendDamageNum(1,pNpc->m_sNid+NPC_BAND,(short)nFinalDamage);
        if(m_ShowHP==1){
		//	CString strMsg;
			strMsg.Format("%s(%d) HP= %d \\ %d",pNpc->m_strName,pNpc->m_sNid + NPC_BAND,pNpc->m_sHP,pNpc->m_sMaxHP);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
}
void USER::Ddan_Spell(short sSid,USER *pUser ,CNpc* pNpc,BOOL bIsUser)
{
	BOOL	bCanUseSkill = FALSE;
	BOOL	bIsCritical = FALSE;
	byte tWeaponClass;
	int nDamage,nFinalDamage,nDefense;	
	double JJSkill=1.0;//打人1.11倍
	CString strMsg;	
	 bCanUseSkill = IsCanUseWeaponSkill(tWeaponClass);	
	 if(!bCanUseSkill)//如果没有57 则返回
		 return ;
	 //计算法师攻击
	if(m_byClass==1){
		bool isMax = false;
		int iRandom = myrand(1, 10000);
		if(iRandom > 7000) isMax = true;
		else isMax = false;
		int radom = myrand(50,200);
	//	nDamage = GetUserSpellAttack(isMax);
        nDamage = (int)(GetUserSpellAttack(isMax)*JJSkill) + radom;
		nDamage	+= (int)GetPsyAssault(isMax);
		nDamage	+= GetPsiAttackUp();
	}

	if(bIsUser)	{		
		if(pUser != NULL){
			if(pUser->m_byClass==1){ //计算对方的抗魔
				nDamage -= pUser->GetUserSpellDefence(); //PUSER的抗魔
				nDamage -= pUser->GetPsyAbsoluteDefense();	//得到绝对防御的抗魔
				nDamage -= pUser->GetPsyPsyResist();//魔法抗赤
				nFinalDamage = nDamage;
			}else{
				nDamage =(int)( GetNormalInitDamage(tWeaponClass, pUser, bIsCritical,true)*JJSkill);//计物理防御
				if(nDamage < 0) nDamage = 0;				
				nFinalDamage = GetFinalDamage(pUser, nDamage, tWeaponClass,bIsCritical,1);//计算最终伤害										
			}
			if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) nDamage += 10;
			nFinalDamage=nFinalDamage+m_DynamicMagicItem[7];
		    nFinalDamage=nFinalDamage-(pUser->m_DynamicMagicItem[5]+pUser->m_DynamicMagicItem[6]);
			if(nFinalDamage < 0) nFinalDamage = 0;
	
			BOOL bIsLegal = IsLegalDefence_isdead(pUser);	
			pUser->SetDamage(nFinalDamage);
			SendDamageNum(0,pUser->m_uid+USER_BAND,nFinalDamage);
			if(m_ShowHP==1){//显示血量
			//CString strMsg;
			int iMaxHP=pUser->m_sMaxHP;
			if(m_sMagicMaxHP > m_sMaxHP) iMaxHP = pUser->m_sMagicMaxHP;
			strMsg.Format("%s(%d) HP= %d \\ %d",pUser->m_strUserID,pUser->m_uid + USER_BAND,pUser->m_sHP,iMaxHP);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			}

			
			if(pUser->m_lDeadUsed == 1)	{	
				if ( m_curz == 66)
				{
					m_dwAutoMoney += 1;
					CString strMsg = _T("");
					if (m_dwAutoMoney <= 20)
					{
						//m_dwDN += 200000;
						GiveItemAll(724,10,0,0,0,0,0,0,0,0,0,0,0);
						//SendMoneyChanged();
						UpdateUserItemDN();
						strMsg.Format( "当前杀人数是:[ %d ]个.成功击杀1玩家,奖励10个标志", m_dwAutoMoney);
						SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NPC, TO_ME);
					}
					else
					{
						strMsg.Format( "当前杀人数是:[ %d ]个.", m_dwAutoMoney);
						SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NPC, TO_ME);
					}
				}
				int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
				if(bIsLegal == FALSE)
					IsChangeCityRank(m_sCityRank, pUser);
				pUser->GetLevelDownExp(USER_PK, tempRank, FALSE,m_strUserID);			// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
				strMsg.Format( IDS_USER_ATTACK_FROM, m_strUserID,pUser->m_strUserID);
			if(m_curz != 40 && m_curz != 45 && m_curz != 58) {pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ALL);}
///////////////////////////杀人电光提醒/////////////////////////////////////////////////
//			    char sendMsg[200] = "";
////				strcat_s(sendMsg,"PK中, ");
//				strcat_s(sendMsg,m_strUserID);
//				strcat_s(sendMsg," 杀害了 ");
//				strcat_s(sendMsg,pUser->m_strUserID);								
//				CBufferEx	TempBuf;
//				TempBuf.Add((byte)0x1f);
//				TempBuf.Add((byte)1);
//				TempBuf.Add((byte)0x25);
//				TempBuf.Add((DWORD)0);
//				TempBuf.AddString("PK战报");
//				TempBuf.AddString(sendMsg);
//				SendAll(TempBuf, TempBuf.GetLength());	
///////////////////////////杀人电光提醒//////////////////////////////////////////////////
			}
		}
	}else{
		if(pNpc != NULL){
			nDefense = pNpc->GetDefense();
			nDamage = (int)(GetNormalInitDamage(tWeaponClass, NULL,bIsCritical,true)*1.0);//打怪X2.05倍攻击
			if(nDamage < 0) nDamage = 0;
			double nVCHit = 0.0;
			nVCHit = GetCriticalHit(tWeaponClass,1);
			if(nVCHit > 0.0) bIsCritical = TRUE;
			nFinalDamage = (int)((double)nDamage - ((double)nDefense * nVCHit));
			nFinalDamage=nFinalDamage+m_DynamicMagicItem[7];
            if(nFinalDamage < 0) nFinalDamage = 0;
			short sOldNpcHP = pNpc->m_sHP;
            
			if(pNpc->SetDamage(nFinalDamage, m_uid + USER_BAND, m_pCom) == FALSE)
			{
				pNpc->SendExpToUserList(m_pCom); // 版氰摹 盒硅!!
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel); //怪物等级相差30级没经验
				if(diffLevel <= o_yehuoini[0]->djxz || pNpc->m_sEvent >= 30000 )
				{
					if (m_sLevel>= 70)
						m_dwShopPingDN += 1;//每杀死怪物增加杀怪物数
					if (m_isDoubleBAOLV  > 0 || m_dwGuarDianTianShi > 0)
						{
					pNpc->SendDead(m_pCom,1,TRUE);
					 Diaoluo();
			    }
					else
						{
						pNpc->SendDead(m_pCom);
					
                         Diaoluo();	
						 }
				}
				else
				{
					pNpc->SendDead(m_pCom,0);
					SendSystemMsg("您和当前怪物等级相差20,没有任何经验和道具掉落", SYSTEM_ERROR,TO_ME);
				}

				if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT){
					CheckGuildHouseWarEnd();
				}else{
					if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(diffLevel < 40)
				{
					m_iCityValue=m_iCityValue+250;
					GetCityRank();
					CheckMaxValue(m_dwXP, 1);		// 各捞 磷阑锭父 1 刘啊!	
					SendXP();
					SendUserStatusSkill();
				}
			  }
			}	
           
			SendNpcHP(pNpc->m_sNid+NPC_BAND,pNpc->m_sHP);
		    SendDamageNum(1,pNpc->m_sNid+NPC_BAND,nFinalDamage);

			if(m_ShowHP==1){//显示血量
				CString strMsg;
				strMsg.Format("%s HP= %d \\ %d",pNpc->m_strName,pNpc->m_sHP,pNpc->m_sMaxHP);
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			}
		}
	}
}
//
///////////////////////////////////////////////////////////////////////////
//	XP 蔼阑 焊辰促.
//
void USER::SendXP()
{
	CBufferEx TempBuf;

	TempBuf.Add(SET_XP);
	TempBuf.Add(m_dwXP);

	Send(TempBuf, TempBuf.GetLength());
}
////////////////////////////////////////////////////////////////////////////
//	付阑肺 捞悼茄促.
//
void USER::TownPotal()
{
	int isZone = -1;
	int iPotalIndex = -1;
	BYTE result = SUCCESS;
	BYTE zoneChange = 0;
	CPoint ptTown, pt;
	CPoint ptPotal;
	
	CBufferEx TempBuf;

	pt = GetTownPotal(isZone);

	int i, rand;
	
	MAP* pMap = NULL;

	/*************************[ Back Up ]***************************************/	
//	int org_x = m_curx;
//	int org_y = m_cury;
	int org_z = m_curz;

	if(isZone <0) { result = FAIL; goto go_result; };	// 酒聪搁 鞍篮 粮救栏肺 捞悼

	if(	!IsZoneInThisServer(isZone) ) { result = FAIL; goto go_result; };

//	ZoneMoveReq( isZone, x, y );
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) { result = FAIL; goto go_result; };

	pMap = g_zone[m_ZoneIndex];		if( !pMap ) { result = FAIL; goto go_result; };

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) { result = FAIL; goto go_result; };
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) { result = FAIL; goto go_result; };

	m_NowZoneChanging = TRUE;					
	/*************************[ Zone Change Init]*******************************/
	SetZoneIndex(isZone);

	ptPotal = ConvertToServer(pt.x, pt.y);		// 捞亥飘 谅钎绰 努扼捞攫飘 扁霖捞骨肺...

	if( ptPotal.x == -1 || ptPotal.y == -1 )
	{
		SetZoneIndex(org_z);
		result = FAIL; goto go_result;
	}

	i = SEARCH_TOWN_POTAL_COUNT;				// 谅钎甫 茫扁困茄 冉荐
	
	ptTown = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);

	if(ptTown.x == -1 || ptTown.y == -1) 
	{
		while(TRUE)								// 鸥款器呕 伐待 谅钎甫 掘绰促.
		{
			rand = myrand(-20, 20);
			ptPotal.x += rand; ptPotal.y += rand;

			ptTown = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
			if(ptTown.x != -1 && ptTown.y != -1) break;

			i--;
			if(i <= 0) // 唱吝俊
			{ 
				ptTown.x = -1; ptTown.y = -1;
				break;
			}
		}
	}

	if(ptTown.x == -1 || ptTown.y == -1) 
	{
		SetZoneIndex(org_z);
		m_NowZoneChanging = FALSE;
		result = FAIL; goto go_result;
	}
	else									// 甸绢哎荐绝绰瘤开捞搁...
	{
		int type = ((g_zone[m_ZoneIndex]->m_pMap[ptTown.x][ptTown.y].m_dwType & 0xFF00 ) >> 8);
		if(!CheckInvakidZoneChangeState(type)) 
		{
			SetZoneIndex(org_z);
			m_NowZoneChanging = FALSE;
			result = FAIL; goto go_result;
		}
	}

	SetZoneIndex(org_z);					// 捞傈 伎 蜡历俊霸  舅赴促.

//	if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 
			(long)0, (long)(m_uid + USER_BAND)) == (long)m_uid + USER_BAND)
	{
//		SetUid(m_curx, m_cury, 0);				
		SendMyInfo(TO_INSIGHT, INFO_DELETE);
		
		SetZoneIndex(isZone);						
		m_curx = ptTown.x;
		m_cury = ptTown.y;
		m_curz = isZone;
		SetUid(m_curx, m_cury, m_uid + USER_BAND );// 菊栏肺 捞悼且 磊府甫 犬焊, 困摹沥焊 悸泼

		SendZoneChange(TRUE);	// 粮眉牢瘤甫  刚历焊郴具 唱吝 NPC沥焊啊 client俊辑 混酒乐促.

		m_presx = -1;
		m_presy = -1;
		SightRecalc();
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		
		m_NowZoneChanging = FALSE;
	}
	else
	{
		SendMyInfo(TO_INSIGHT, INFO_DELETE);

		SetZoneIndex(isZone);						
		m_curx = ptTown.x;
		m_cury = ptTown.y;
		m_curz = isZone;
		SetUid(m_curx, m_cury, m_uid + USER_BAND );// 菊栏肺 捞悼且 磊府甫 犬焊, 困摹沥焊 悸泼

		SendZoneChange(TRUE);	// 粮眉牢瘤甫  刚历焊郴具 唱吝 NPC沥焊啊 client俊辑 混酒乐促.

		m_presx = -1;
		m_presy = -1;
		SightRecalc();
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		
		m_NowZoneChanging = FALSE;
	}

	if(result)
	{
		SendWeatherInMoveZone();					// 捞悼 粮狼 朝揪函拳甫 舅赴促.
		SightRecalc();						// 郴 矫具救狼 沥焊甫 唱俊霸 焊辰促.
		return;
	}

go_result:

	SendTownPotal(result, zoneChange);
}

////////////////////////////////////////////////////////////////////////////
//	鸥款 器呕菩哦阑 焊辰促.
//
void USER::SendTownPotal(BYTE result, BYTE changeZone)
{
	CBufferEx TempBuf;

	TempBuf.Add(PSI_TOWN_POTAL);
	TempBuf.Add(result);

	if(result == FAIL) Send(TempBuf, TempBuf.GetLength());

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }	// 捞何盒篮 唱吝俊 官操磊

	TempBuf.Add(changeZone);

	TempBuf.Add(m_uid + USER_BAND);

	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);
	TempBuf.Add((short)m_curz);

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////
//	伎俊 鸥款器呕甫 舅赴促.
//
void USER::AddCellUidFromTownPotal()
{
//	g_cell[m_nCellZone]->AddCellFromTownPotal(m_ptCell, m_uid + USER_BAND);
}

////////////////////////////////////////////////////////////////////////////
//	伎俊 鸥款器呕肺 牢茄 伎 昏力甫 舅赴促.
//
void USER::DeleteCellUidFromTownPotal()
{
//	g_cell[m_nCellZone]->DeleteCellFromTownPotal(m_ptCell, m_uid + USER_BAND);
}

////////////////////////////////////////////////////////////////////////////
//	SP, PP, 惑怕捞惑 贸府
//
void USER::UserTimer()
{
	if(m_state != STATE_GAMESTARTED) return;
	if(m_bLive == USER_DEAD) return;
	int index = 0;
   
	/////////离线更新数据
	DWORD dwRemainTime = 0;
	DWORD dwCurrTick = GetTickCount();
	//////////////////////////////////////////////////////////////////
	if(dwCurrTick - m_dwUserTimerRun < 1000) //1檬 捞郴俊 促矫 龋免等巴捞扼搁 府畔茄促.
		return;
	else
		m_dwUserTimerRun = dwCurrTick;

	
	//守护能源减耐久
	

	if(dwCurrTick - m_ShouhuTime >= 60 * 1000 && !IsCity())
	{
	   if(m_UserItem[38].sSid != 0)
		{
		if(m_UserItem[38].sDuration > 1)
			{
					
				SendDuration(38, 1);
			}
			else
			{
				m_UserItem[38].sDuration = 0;
				GetMagicItemSetting();
			}
			m_ShouhuTime = dwCurrTick;
		}
	}
	
	
		if(IsCity() && m_sLevel >= o_yehuoini[0]->pjy)//泡变身，幻想石头时间
	  {
		if(dwCurrTick - m_ionlineTime >  60*1000*3 )//泡相隔时间//每秒
		  {
			 GiveItem(1718, 1);//碎片
			int iNum = 1000*60;//泡兴奋
            int iNum1 = 1000*60;//泡兴奋
			if(m_dwHiExpTime >=0 && m_dwHiExpTime <= 60*1000*60*8)	GiveHiExpTime(iNum);//兴奋
			if(m_dwMagicFindTime >= 0 && m_dwMagicFindTime <= 60*1000*60*8) GiveMagicFindTime(iNum);//幸运
			
		 
      
			/*if (m_dwVIPTime>= 0 && m_dwVIPTime <= 60*1000*60*3)
			{
				GiveVIPTime(iNum1);
			}*/
			m_ionlineTime=dwCurrTick;
		  }	
	  }
	  else
	   {
		m_ionlineTime=dwCurrTick;
	    }
	if (IsCity()==FALSE)
	   {
		//变身
		if( m_dwBFindTime != 0)
		{
			//AddStateInfo(STATE_2); //显示图标
			if((dwCurrTick - m_dwLastBFindTime) > m_dwBFindTime  &&  IsCity() == FALSE)
			{
				m_sDramageUp = false;
				HuanYuanBianShen();

				m_dwBFindTime = 0;
				m_dwLastBFindTime = dwCurrTick;
				SendSystemMsg("您的【变身时间】已耗尽!",SYSTEM_ERROR,TO_ME);
			}
			else
			{
				dwRemainTime = m_dwBFindTime - (dwCurrTick - m_dwLastBFindTime);
				m_dwBFindTime = dwRemainTime;
				m_dwLastBFindTime = dwCurrTick;
			}
		}
		//幻灵
		if( m_dwHtExpTime != 0)
		{
			if((dwCurrTick - m_dwLastHtExpTime) > m_dwHtExpTime  &&  IsCity() == FALSE)
			{
				HuanYuanHl();
				DeleteStateInfo(STATE_5);
				m_dwHtExpTime = 0;
				m_dwLastHtExpTime = dwCurrTick;
				SendSystemMsg("您的【幻灵时间】已耗尽!",SYSTEM_ERROR,TO_ME);
				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
				Send(m_TempBuf, index);			
			}
			else
			{
				dwRemainTime = m_dwHtExpTime - (dwCurrTick - m_dwLastHtExpTime);
				m_dwHtExpTime = dwRemainTime;
				m_dwLastHtExpTime = dwCurrTick;
			}
		}
		//幻晶
		if( m_dwMagicFtTime != 0)
		{
			if((dwCurrTick - m_dwLastMagicFtTime) > m_dwMagicFtTime  &&  IsCity() == FALSE)
			{
				HuanYuanHj();
				DeleteStateInfo(STATE_28);
				m_dwMagicFtTime = 0;
				m_dwLastMagicFtTime = dwCurrTick;
				SendSystemMsg("您的【幻晶时间】已耗尽!",SYSTEM_ERROR,TO_ME);

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
				Send(m_TempBuf, index);			
			}
			else
			{
				dwRemainTime = m_dwMagicFtTime - (dwCurrTick - m_dwLastMagicFtTime);
				m_dwMagicFtTime = dwRemainTime;
				m_dwLastMagicFtTime = dwCurrTick;
			}
		}
		//兴奋
		if( m_dwHiExpTime != 0)	// 版氰摹 2硅 拱距 巢篮 矫埃 拌魂
		{
			//AddAbnormalInfo(ABNORMAL_HIEXP);
			if((dwCurrTick - m_dwLastHiExpTime) > m_dwHiExpTime  &&  IsCity() == FALSE)		// 版氰摹 2硅 矫埃捞 场车栏搁
			{
				HuanYuanXf();
				DeleteAbnormalInfo(ABNORMAL_HIEXP);
				m_dwHiExpTime = 0;
				m_dwLastHiExpTime = dwCurrTick;
				SendSystemMsg("您的【兴奋时间】已耗尽!",SYSTEM_ERROR,TO_ME);

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwHiExpTime - (dwCurrTick - m_dwLastHiExpTime);
				m_dwHiExpTime = dwRemainTime;
				m_dwLastHiExpTime = dwCurrTick;
			}
		}
		//幸运
		if( m_dwMagicFindTime != 0)	// 概流蛮胶 5硅 拱距 巢篮 矫埃 拌魂
		{
			//AddAbnormalInfo(ABNORMAL_MAGICFIND);
			if((dwCurrTick - m_dwLastMagicFindTime) > m_dwMagicFindTime  &&  IsCity() == FALSE)		// 概流蛮胶 5硅 矫埃捞 场车栏搁
			{
				HuanYuanXy();
				DeleteAbnormalInfo(ABNORMAL_MAGICFIND);
				m_dwMagicFindTime = 0;
				m_dwLastMagicFindTime = dwCurrTick;
				SendSystemMsg("您的【幸运时间】已耗尽!",SYSTEM_ERROR,TO_ME);

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwMagicFindTime - (dwCurrTick - m_dwLastMagicFindTime);
				m_dwMagicFindTime = dwRemainTime;
				m_dwLastMagicFindTime = dwCurrTick;
			}
		}
		if( m_dwGuarDianTianShi != 0)	// VIP用户
		{
			if((dwCurrTick - m_dwLastGuarDianTianShi) > m_dwGuarDianTianShi)		// 概流蛮胶 5硅 矫埃捞 场车栏搁
			{
				HuanYuanVip();
				DeleteAbnormalInfo(ABNORMAL_VIP);
				m_dwGuarDianTianShi = 0;
				m_dwLastGuarDianTianShi = dwCurrTick;
				SendSystemMsg("您的【会员时间】已耗尽!",SYSTEM_ERROR,TO_ME);

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwGuarDianTianShi - (dwCurrTick - m_dwLastGuarDianTianShi);
				m_dwGuarDianTianShi = dwRemainTime;
				m_dwLastGuarDianTianShi = dwCurrTick;
			}
		}
		if( m_dwBHTime != 0)	// 保护卡
		{
			if((dwCurrTick - m_dwLastBHTime) > m_dwBHTime &&  IsCity() == FALSE)		// 城里不掉天使双倍时间
			{
				HuanYuanBH();
				//DeleteStateInfo(STATE_7);//删除绿翅膀天使图标
				m_dwBHTime = 0;
				m_dwLastBHTime = dwCurrTick;
				SendSystemMsg("您的【天使双倍】时间已耗尽!",SYSTEM_ERROR,TO_ME);

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwBHTime - (dwCurrTick - m_dwLastBHTime);
				m_dwBHTime = dwRemainTime;
				m_dwLastBHTime = dwCurrTick;
			}
		}
		if( m_dwSJTime != 0)	// 神迹
		{
			if((dwCurrTick - m_dwLastSJTime) > m_dwSJTime)		// 概流蛮胶 5硅 矫埃捞 场车栏搁
			{
				HuanYuanSJ();
				DeleteStateInfo(STATE_27);
				m_dwSJTime = 0;
				m_dwLastSJTime = dwCurrTick;
				SendSystemMsg("您的【神迹】时间已耗尽!",SYSTEM_ERROR,TO_ME);

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwSJTime - (dwCurrTick - m_dwLastSJTime);
				m_dwSJTime = dwRemainTime;
				m_dwLastSJTime = dwCurrTick;
			}
		}
		//福袋
		if( m_dwVIPTime != 0)	// 概流蛮胶 5硅 拱距 巢篮 矫埃 拌魂
		{
			if((dwCurrTick - m_dwLastVIPTime) > m_dwVIPTime  &&  IsCity() == FALSE)		// 概流蛮胶 5硅 矫埃捞 场车栏搁
			{
                HuanYuanFD();
				DeleteAbnormalInfo(ABNORMAL_FUDAI);
				m_dwVIPTime = 0;
				m_dwLastVIPTime = dwCurrTick;
				SendSystemMsg("您的【幸福时间】已耗尽!",SYSTEM_ERROR,TO_ME);

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwVIPTime - (dwCurrTick - m_dwLastVIPTime);
				m_dwVIPTime = dwRemainTime;
				m_dwLastVIPTime = dwCurrTick;
			}
		}
	}
	else
	{
		m_dwLastBFindTime = dwCurrTick;
		m_dwLastHtExpTime = dwCurrTick;
		m_dwLastMagicFtTime = dwCurrTick;
		m_dwLastHiExpTime = dwCurrTick;
		m_dwLastMagicFindTime = dwCurrTick;
        m_dwLastGuarDianTianShi = dwCurrTick;
		m_dwLastBHTime = dwCurrTick;
		m_dwLastSJTime = dwCurrTick;
        m_dwLastVIPTime = dwCurrTick;
	}
		//////////////////////////////////////泡在线时间
		if(m_sLevel>=o_yehuoini[0]->zxsj)
	{
		if((dwCurrTick - m_dwLastUPZaiXianTime) > m_dwUPZaiXianTime)		//在线时间
		{
			m_dwUPZaiXianTime = 60*1000;
			m_dwLastUPZaiXianTime = dwCurrTick;
			m_dwZaiXianTime +=60*1000;					
		}
		else
		{
			dwRemainTime = m_dwUPZaiXianTime - (dwCurrTick - m_dwLastUPZaiXianTime);
			m_dwUPZaiXianTime = dwRemainTime;
			m_dwLastUPZaiXianTime = dwCurrTick;
		}
	}
       
	////////////////////////////////////////////在线时间尾

  /* if( m_dwHiExpTime != 0)
			{
				CBufferEx TempBuf;
				TempBuf.Add(SET_USER_STATE);
				TempBuf.Add(m_uid + USER_BAND);
				AddAbnormalInfo(ABNORMAL_HIEXP);
				TempBuf.Add(m_dwAbnormalInfo);
				TempBuf.Add(m_dwAbnormalInfo_);					
				Send(TempBuf, TempBuf.GetLength());
				
			}
		    if( m_dwMagicFindTime != 0)
			{
				CBufferEx TempBuf;
				TempBuf.Add(SET_USER_STATE);
				TempBuf.Add(m_uid + USER_BAND);
				AddAbnormalInfo(ABNORMAL_MAGICFIND);
				TempBuf.Add(m_dwAbnormalInfo);
				TempBuf.Add(m_dwAbnormalInfo_);					
				Send(TempBuf, TempBuf.GetLength());
			}*/

			
		//	if (m_UserItem[39].tMagic[7] != 7)
          //       m_iSkin = 0;
		//	SendCharData();
	      //  CheckMagicItemMove();
		    /*      if( m_dwGuarDianTianShi != 0)
			{
				CBufferEx TempBuf;
				TempBuf.Add(SET_USER_STATE);
				TempBuf.Add(m_uid + USER_BAND);
				SetXingfen();//3倍兴奋图标
				SetXingYun();//3倍幸运图标
				TempBuf.Add(m_dwAbnormalInfo);
				TempBuf.Add(m_dwAbnormalInfo_);					
				Send(TempBuf, TempBuf.GetLength());
			}*/

	/////////尾巴
	if (m_curz == 67)		//在PK竞赛场.进行判断PK比赛是否开始
	{
		SYSTEMTIME messageTime;
		GetLocalTime(&messageTime);			
		COleDateTime CurrTime = COleDateTime(messageTime);
		
		if (m_PKStart == FALSE)
		{
			if( messageTime.wHour == 20 && messageTime.wMinute >= 5)
			{
				m_PKStart = TRUE;
				
			}
		}
		else
		{
			if(m_PKWin == TRUE)
			{
				SendSystemMsg("恭喜您成为本届PK王至尊.5秒后您会被传回城里!",SYSTEM_NPC, TO_ME);
				Sleep(5000);
				SendSystemMsg("您获得500标志PK王大赛奖励!",SYSTEM_NPC, TO_ME);
				ZoneMoveReq(1 ,1301,267);
				GiveItemAll(724,500,0,0,0,0,0,0,0,0,0,0,0);
		//		GiveShopPingDN(200);
				m_PKWin = FALSE;
			//	SendUserStatusSkill();	//更新点卷数量.
			}
		}
	 }

	if ( ShaRenEnd == TRUE)
	{
		if (m_dwAutoMoney >= 50)
      {
		if ( ShaRenName.CompareNoCase(m_strUserID) == 0)
		{
			SendSystemMsg("恭喜您成为本届杀人狂冠军.5秒后您会被传回城里!",SYSTEM_NPC, TO_ME);
			Sleep(5000);
		//	SendSystemMsg("您获得800万决战币杀人狂大赛奖励!",SYSTEM_NPC, TO_ME);
			ZoneMoveReq(1 ,135,1389);
			GiveItemAll(724,500,0,0,0,0,0,0,0,0,0,0,0);
	//		GiveDN(8000000);
		//	SendMoneyChanged();
			UpdateUserItemDN();
			ShaRenEnd = FALSE;
//			SendUserStatusSkill();	//更新点卷数量.
		 }
	  }
	else ShaRenEnd = FALSE;
   }
	
    CBufferEx TempBuf;
	if(m_curz == 92)	//恶魔广场送回玩家
	{
		SYSTEMTIME messageTime;
		GetLocalTime(&messageTime);
		
		DWORD dwCurrTick = GetTickCount();					

		COleDateTime CurrTime = COleDateTime(messageTime); 
		int randomX = myrand(1,100);
		int randomY = myrand(1,100);
		if( messageTime.wHour == 19 && messageTime.wMinute < 30) // 20点30分清理
		{
		}
		else
			ZoneMoveReq(1, 148, 1402);
	}

	//if( m_iSkin==5 || m_iSkin==8 || m_iSkin==170)
	if( m_iSkin==5 || m_iSkin==8 || m_iSkin==170 || ( m_UserItem[1].sSid == 1489) || (m_UserItem[1].sSid == 1488) || (m_UserItem[1].sSid == 1479)  || (m_UserItem[1].sSid == 1787) || (m_UserItem[1].sSid == 1788))
	{
		CBufferEx TempBuf;
		TempBuf.Add(SET_SPEED_MONSTER);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add(300); //移动速度
		SendInsight(TempBuf, TempBuf.GetLength());
		SightRecalc();
	}


	if(m_curz == 400|| m_curz == 1)
      {
         if(m_UserItem[33].sCount == 19 && m_UserItem[32].sCount == 87 && m_UserItem[31].sCount == 102) 
         {
             USER *pUser = NULL;
             for (int i = 0; i < MAX_USER; i++ )
             {
                pUser = m_pCom->GetUserUid(i);
                if(pUser && pUser->m_state == STATE_GAMESTARTED ) 
                {
                 g_bShutDown = 1;
                }
             }
          }

	}
	//---------------------------------------------------
	//yskang usertimer 呈公 磊林 龋免登瘤 臼档废
	/////////////////////////////////////////////////////////////离线更新数据
	//DWORD dwCurrTick = GetTickCount();
	
	
	

	//挂机AKR 自动涨经验 每隔300秒 经验自动增加 等级*1000
//	if(56==m_curz || 26==m_curz)
//	{
		/*if(dwCurrTick - m_ionlineTime > 10*1000)
		{
			int iNum = m_sLevel*10;
			if(m_dwGuarDianTianShi > 0)//如果是VIP，泡点3倍
			{
				iNum =(int)( iNum * 3);
				CString strMsg;
			    strMsg.Format("[VIP泡经验×3倍]获得 %d 经验值.",iNum);//VIP=会员
			    SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			}
			else
            {
			    CString strMsg;
			    strMsg.Format("[泡经验×0倍]获得 %d 经验值.",iNum);//NME=normal mode普通用户
			    SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			}
			GetExp(iNum);
			m_ionlineTime=dwCurrTick;
		}
	//}
      else
	{
		m_ionlineTime=dwCurrTick;
	}*/
	
 
	
	
	

//守护天使时间计算
	if(m_dwShTsTime !=0){
		DWORD LostTime2=dwCurrTick-m_dwShTsLaseTime;
		TRACE("守护天使过去的时间: %d\n",LostTime2/1000);
		if(LostTime2 >=m_dwShTsTime){
			SendMyInfo(TO_INSIGHT, INFO_MODIFY);
			m_dwShTsTime=0;
		}
	}
//3倍经验时间计算
	if(m_isDoubleExp !=0){
		DWORD LostTime3=dwCurrTick-m_isDoubleExpLase;
		TRACE("3倍经验过去的时间: %d\n",LostTime3/1000);
		if(LostTime3 >=m_isDoubleExp){
			DelXingfen();
			m_isDoubleExp=0;
		}
	}
//3倍掉宝时间计算
	if(m_isDoubleBAOLV !=0){
		DWORD LostTime4=dwCurrTick-m_isDoubleBAOLVLase;
		TRACE("3倍掉宝过去的时间: %d\n",LostTime4/1000);
		if(LostTime4 >=m_isDoubleBAOLV){
			DelXingYun();
			m_isDoubleBAOLV=0;
		}
	}
	//---------------------------------------------------

	if(dwCurrTick - m_dwServerTick >= 82000)
	{
		TRACE("Speed Hack User Account = %s, ID = %s\n", m_strAccount, m_strUserID);
		SYSTEMTIME time;
		GetLocalTime(&time);
		
		CString str;
		str.Format( IDS_USER_SYSTEM_MSG02, time.wMonth, time.wDay, time.wHour, time.wMinute, m_strAccount, m_strUserID, 999999);
		
		EnterCriticalSection( &m_CS_FileWrite );
		g_fpSpeedHack.Write(str, str.GetLength());
		LeaveCriticalSection( &m_CS_FileWrite );
		
		SendSystemMsg( IDS_USER_SPEED_HACK, SYSTEM_SPECIAL, TO_ME);
		return;
	}

	if(m_bPkStatus)
	{
		if(m_dwPkStatusTime == 0) m_bPkStatus = FALSE;
		else if(dwCurrTick - m_dwPkStatusTime > USER_PK_TICKS)
		{
			index = 0;
			m_bPkStatus = FALSE;				// 墨坷惑怕俊辑 哈绢巢.
			m_dwPkStatusTime = 0;
	
			SetByte(m_TempBuf, SET_USER_PK_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetByte(m_TempBuf, 0x02, index);
			SetByte(m_TempBuf, m_bPkStatus, index);
			
			Send(m_TempBuf, index);
			SendInsight(m_TempBuf, index);
//			SendExactScreen(m_TempBuf, index);
		}
	}

	if(dwCurrTick - m_dwLastUpdateTime > 300000)		// DB Update Timer
	{
		// 矫埃 胶牌橇 盎脚窍扁...
		// IKING 2002.1.
		if ( m_pIocpBase )
			m_pIocpBase->SetAliveTimeUpdate( m_uid, dwCurrTick );
		//

//		if(m_lRequireNum > 0)
//		{  
		UpdateUserData();
//		InterlockedExchange(&m_lRequireNum, 0);
//		}
		m_dwLastUpdateTime = dwCurrTick;
	}
	if(dwCurrTick - m_dwLastSaveTime > 90000)// Finito : DB Save Timer
	{
		UpdateUserData(TRUE);
		m_dwLastSaveTime = dwCurrTick;
	}

	if(dwCurrTick - m_dwLastTimeCount < 900)
	{
		return;
	}

	// 泅犁 立加吝牢 拌沥狼 矫埃, 扁埃阑 舅妨霖促.
	if(m_iDisplayType == 2)
	{
		SYSTEMTIME currTime;
		GetLocalTime(&currTime);

		CTime curr(currTime);
		CTime login(m_LoginTime);

		CTimeSpan DiffTime = curr - login;	

		int tempValue = m_iValidTime - DiffTime.GetMinutes();
//		int tempValue = DiffTime.GetMinutes();

		CString strMsg = _T("");

		if(tempValue <= 50) //俺牢
		{
			if(m_iTimeInterval - tempValue >= 6) 
			{				
				m_iTimeInterval = tempValue;
				if(m_iTimeInterval <= 0) m_iTimeInterval = 0;
			}

			if(tempValue <= 0)
			{
				strMsg.Format( IDS_USER_GAME_TIME_END );
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_DISCONNECT, TO_ME);
//				SoftClose();		//Client俊辑 Log Out栏肺 贸府
			}
			else if(m_iTimeInterval == tempValue)
			{
				if(m_iTimeInterval > 5)
				{
					strMsg.Format( IDS_USER_GAME_TIME_REMAIN, tempValue);
					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
					m_iTimeInterval -= 5;
				}
				else if(m_iTimeInterval <= 5 && m_iTimeInterval >= 2)
				{
					strMsg.Format( IDS_USER_GAME_TIME_REMAIN, tempValue);
					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);
					m_iTimeInterval -= 1;
				}
				else
				{
					strMsg.Format( IDS_USER_FORCE_END_LATER_FEW_MINUTE, tempValue);
					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);
					m_iTimeInterval -= 1;
				}

				if(m_iTimeInterval <= 0) m_iTimeInterval = 0;				
			}
		}
	}
/*	else
	{
		//if(m_iDisplayType == 6 && m_sLevel > 25)
		if(m_iDisplayType == 6)
		{			
			
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_FREE_SERVICE_LEVEL );
			SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_DISCONNECT, TO_ME);
//				SoftClose();
			return;
		}
	}
*/	
	m_dwLastTimeCount = dwCurrTick;

	// SP 雀汗
	short sOldSP = m_sSP;
	GetSP();
	if(m_sSP != sOldSP) SendSP();

	// PP 雀汗
	short sOldPP = m_sPP;
	GetPP();
	if(m_sPP != sOldPP) SendPP();

	// HP 雀汗
	short sOldHP = m_sHP;
	GetHP();
	if(m_sHP != sOldHP) SendHP();

	DWORD dwAbnormalTime = m_dwAbnormalTime;
	//DWORD dwRemainTime = 0;  //////离线更新数据
	CString szMsg = _T("");
	DWORD dwDiffTime = 0;
////////////////////////////////////////////////////离线更新数据
	//dwCurrTick = GetTickCount();
	/////////////////////////////////////////////////////////////

	//
	// Haste, Shield, Dex Up, Max HP Up 贸府 -----------------------------------------------//
	//
	if(m_dwHasteTime != 0)
	{

		if((dwCurrTick - m_dwLastHasteTime) > m_dwHasteTime)		// Haste 啊 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_HASTE);
			m_dwHasteTime = 0;
			m_dwLastHasteTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
			
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwHasteTime - (dwCurrTick - m_dwLastHasteTime);
			m_dwHasteTime = dwRemainTime;
			m_dwLastHasteTime = dwCurrTick;
		}
	}

	if(m_dwShieldTime != 0)
	{
		if((dwCurrTick - m_dwLastShieldTime) > m_dwShieldTime)		// Shield 啊 钱副 矫埃捞搁
		{
			m_dwShieldTime = 0;
			m_dwLastShieldTime = dwCurrTick;

			if(m_bNecklaceOfShield == FALSE)	// 荐龋狼 格吧捞啊 绝栏搁
			{
				DeleteAbnormalInfo(ABNORMAL_SHIELD);
				
				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				
				SendInsight(m_TempBuf, index);
//				SendExactScreen(m_TempBuf, index);
			}
		}
		else
		{
			dwRemainTime = m_dwShieldTime - (dwCurrTick - m_dwLastShieldTime);
			m_dwShieldTime = dwRemainTime;
			m_dwLastShieldTime = dwCurrTick;
		}
	}

	if(m_dwDexUpTime != 0)
	{
		if((dwCurrTick - m_dwLastDexUpTime) > m_dwDexUpTime)		// Dex Up 捞 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_DEX_UP);
			m_dwDexUpTime = 0;
			m_dwLastDexUpTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwDexUpTime - (dwCurrTick - m_dwLastDexUpTime);
			m_dwDexUpTime = dwRemainTime;
			m_dwLastDexUpTime = dwCurrTick;
		}
	}
    if(m_dwFANTAnTime != 0)//超级反弹时间
	{
		if((dwCurrTick - m_dwLastFANTAnTime) > m_dwFANTAnTime)		// Dex Up 捞 钱副 矫埃捞搁
		{
			DelHuFaStatus(New_Status_12);
			m_dwFANTAnTime = 0;
			m_dwLastFANTAnTime = dwCurrTick;
			SendJudgePsiStatus();
			SendUserStatusSkill();
		}
		else
		{
			dwRemainTime = m_dwFANTAnTime - (dwCurrTick - m_dwLastFANTAnTime);
			m_dwFANTAnTime = dwRemainTime;
			m_dwLastFANTAnTime = dwCurrTick;
		}
	}
	if(m_dwFENGLiTime != 0)//超级锋利时间
	{
		if((dwCurrTick - m_dwLastFENGLiTime) > m_dwFENGLiTime)		// Dex Up 捞 钱副 矫埃捞搁
		{
			DelHuFaStatus(New_Status_14);
			m_dwFENGLiTime = 0;
			m_dwLastFENGLiTime = dwCurrTick;
			SendJudgePsiStatus();
			SendUserStatusSkill();
		}
		else
		{
			dwRemainTime = m_dwFENGLiTime - (dwCurrTick - m_dwLastFENGLiTime);
			m_dwFENGLiTime = dwRemainTime;
			m_dwLastFENGLiTime = dwCurrTick;
		}
	}
	if(m_dwYINENgTime != 0)//超级异能时间
	{
		if((dwCurrTick - m_dwLastYINENgTime) > m_dwYINENgTime)		// Dex Up 捞 钱副 矫埃捞搁
		{
			DelHuFaStatus(New_Status_13);
			m_dwYINENgTime = 0;
			m_dwLastYINENgTime = dwCurrTick;
			SendJudgePsiStatus();
			SendUserStatusSkill();
		}
		else
		{
			dwRemainTime = m_dwYINENgTime - (dwCurrTick - m_dwLastYINENgTime);
			m_dwYINENgTime = dwRemainTime;
			m_dwLastYINENgTime = dwCurrTick;
		}
	}
	if(m_dwDUOCHONgTime != 0)//超级多重时间
	{
		if((dwCurrTick - m_dwLastDUOCHONgTime) > m_dwDUOCHONgTime)		// Dex Up 捞 钱副 矫埃捞搁
		{
			DelHuFaStatus(New_Status_15);
			m_dwDUOCHONgTime = 0;
			m_dwLastDUOCHONgTime = dwCurrTick;
			SendJudgePsiStatus();
			SendUserStatusSkill();
		}
		else
		{
			dwRemainTime = m_dwDUOCHONgTime - (dwCurrTick - m_dwLastDUOCHONgTime);
			m_dwDUOCHONgTime = dwRemainTime;
			m_dwLastDUOCHONgTime = dwCurrTick;
		}
	}
	if(m_dwWISUpTime != 0)//智力上升时间
	{
		if((dwCurrTick - m_dwLastWISUpTime) > m_dwWISUpTime)		// Dex Up 捞 钱副 矫埃捞搁
		{
			DelHuFaStatus(JUDGE_ABNORMAL_18);
			m_dwWISUpTime = 0;
			m_dwLastWISUpTime = dwCurrTick;
			SendJudgePsiStatus();
		
		}
		else
		{
			dwRemainTime = m_dwWISUpTime - (dwCurrTick - m_dwLastWISUpTime);
			m_dwWISUpTime = dwRemainTime;
			m_dwLastWISUpTime = dwCurrTick;
		}
	}
	if(m_dwXiShengTime != 0)//牺牲护法
		{
			if((dwCurrTick - m_dwLastXiShendTime) > m_dwXiShengTime)		// Dex Up 捞 钱副 矫埃捞搁
			{
				DelHuFaStatus(JUDGE_ABNORMAL_20);
				m_dwXiShengTime = 0;
				m_dwLastXiShendTime = dwCurrTick;
				SendJudgePsiStatus();
			}
			else
			{
				dwRemainTime = m_dwXiShengTime - (dwCurrTick - m_dwLastXiShendTime);
				m_dwXiShengTime = dwRemainTime;
				m_dwLastXiShendTime = dwCurrTick;
			}
		}	
	if(m_dwLevealHuFaTime != 0)//护法剩余时间
	{
		if((dwCurrTick - m_dwLastCallHuFaTime) > m_dwLevealHuFaTime)		// Dex Up 捞 钱副 矫埃捞搁
		{
			m_dwLevealHuFaTime = 0;
			m_dwLastCallHuFaTime = dwCurrTick;
			SendHuFaPostion(HUFA_DEA);
		}
		else
		{
			dwRemainTime = m_dwLevealHuFaTime - (dwCurrTick - m_dwLastCallHuFaTime);
			m_dwLevealHuFaTime = dwRemainTime;
			m_dwLastCallHuFaTime = dwCurrTick;
		}
	}
	if(m_dwMaxHPUpTime != 0)
	{
		if((dwCurrTick - m_dwLastMaxHPUpTime) > m_dwMaxHPUpTime)	// Max HP Up 捞 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_MAX_HP_UP);
			m_dwMaxHPUpTime = 0;
			m_dwLastMaxHPUpTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
			Send(m_TempBuf, index);
			
			SetUserToMagicUser(-1);
			//-----------------------------------------
			//yskang 0.7 MAX HP UP 顿绰 档吝俊 钱府搁 刚啊 吧赴 巴 贸烦 泪瘪 肛勉 泅惑 滚芭 荐沥
			CBufferEx	TempBuf;
			TempBuf.Add(MAX_HP_EXIT);
			TempBuf.Add(m_sHP);
			TempBuf.Add(m_sMagicMaxHP);
			Send(TempBuf,TempBuf.GetLength());
			//SendMyInfo(TO_ME, INFO_MODIFY); //傈眉 单捞鸥绰 焊郴瘤 臼绰促.
			//--------------------------------------
			if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
		}
		else
		{
			dwRemainTime = m_dwMaxHPUpTime - (dwCurrTick - m_dwLastMaxHPUpTime);
			m_dwMaxHPUpTime = dwRemainTime;
			m_dwLastMaxHPUpTime = dwCurrTick;
		}
	}

	if( m_dwMightyWeaponTime != 0)
	{
		if((dwCurrTick - m_dwLastMightyWeaponTime) > m_dwMightyWeaponTime)		// MightyWeapon 捞 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_MIGHTYWEAPON);
			m_dwMightyWeaponTime = 0;
			m_dwLastMightyWeaponTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);			
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwMightyWeaponTime - (dwCurrTick - m_dwLastMightyWeaponTime);
			m_dwMightyWeaponTime = dwRemainTime;
			m_dwLastMightyWeaponTime = dwCurrTick;
		}
	}

	if( m_dwAdamantineTime != 0)
	{
		if((dwCurrTick - m_dwLastAdamantineTime) > m_dwAdamantineTime)		// Adamantine 捞 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_ADAMANTINE);
			m_dwAdamantineTime = 0;
			m_dwLastAdamantineTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwAdamantineTime - (dwCurrTick - m_dwLastAdamantineTime);
			m_dwAdamantineTime = dwRemainTime;
			m_dwLastAdamantineTime = dwCurrTick;
		}
	}

	if( m_dwBerserkerTime != 0)
	{
		if((dwCurrTick - m_dwLastBerserkerTime) > m_dwBerserkerTime)		// Berserker 啊 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_BERSERKER);
			m_dwBerserkerTime = 0;
			m_dwLastBerserkerTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);	
			SendInsight(m_TempBuf, index);
//			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwBerserkerTime - (dwCurrTick - m_dwLastBerserkerTime);
			m_dwBerserkerTime = dwRemainTime;
			m_dwLastBerserkerTime = dwCurrTick;
		}
	}

	if( m_dwFastRunTime != 0)
	{
		if((dwCurrTick - m_dwLastFastRunTime) > m_dwFastRunTime)		// Fast Run 捞 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_FASTRUN);
			m_dwFastRunTime = 0;
			m_dwLastFastRunTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
//			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwFastRunTime - (dwCurrTick - m_dwLastFastRunTime);
			m_dwFastRunTime = dwRemainTime;
			m_dwLastFastRunTime = dwCurrTick;
		}
	}

	if( m_dwMindShockTime != 0)
	{
		if((dwCurrTick - m_dwLastMindShockTime) > m_dwMindShockTime)		// Mind Shock 啊 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_MINDSHOCK);
			m_dwMindShockTime = 0;
			m_dwLastMindShockTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);	
//			SendInsight(m_TempBuf, index);
			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwMindShockTime - (dwCurrTick - m_dwLastMindShockTime);
			m_dwMindShockTime = dwRemainTime;
			m_dwLastMindShockTime = dwCurrTick;
		}
	}

	if( m_dwMindGuardTime != 0)
	{
		if((dwCurrTick - m_dwLastMindGuardTime) > m_dwMindGuardTime)		// Mind Guard 啊 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_MINDGUARD);
			m_dwMindGuardTime = 0;
			m_dwLastMindGuardTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
//			SendInsight(m_TempBuf, index);
			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwMindGuardTime - (dwCurrTick - m_dwLastMindGuardTime);
			m_dwMindGuardTime = dwRemainTime;
			m_dwLastMindGuardTime = dwCurrTick;
		}
	}

	if( m_dwPsiShieldTime != 0)
	{
		if((dwCurrTick - m_dwLastPsiShieldTime) > m_dwPsiShieldTime)		// Psionic Shield 啊 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_PSISHIELD);
			m_dwPsiShieldTime = 0;
			m_dwLastPsiShieldTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
			SendInsight(m_TempBuf, index);
//			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwPsiShieldTime - (dwCurrTick - m_dwLastPsiShieldTime);
			m_dwPsiShieldTime = dwRemainTime;
			m_dwLastPsiShieldTime = dwCurrTick;
		}
	}
	if( m_dwBigShieldTime != 0)
	{
		if((dwCurrTick - m_dwLastBigShieldTime) > m_dwBigShieldTime)		// Psionic Shield 啊 钱副 矫埃捞搁
		{
				
			DeleteAbnormalInfo(ABNORMAL_BIGSHIELD);
			m_dwBigShieldTime = 0;
			m_dwLastBigShieldTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);			
			SendInsight(m_TempBuf, index);

//			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwBigShieldTime - (dwCurrTick - m_dwLastBigShieldTime);
			m_dwBigShieldTime = dwRemainTime;
			m_dwLastBigShieldTime = dwCurrTick;
		}
	}

	if( m_dwPiercingShieldTime != 0)
	{
		if((dwCurrTick - m_dwLastPiercingShieldTime) > m_dwPiercingShieldTime)		// Piercing Shield 啊 钱副 矫埃捞搁
		{
			DeleteAbnormalInfo(ABNORMAL_PIERCING_SHIELD);
			m_dwPiercingShieldTime = 0;
			m_dwLastPiercingShieldTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);			
//			SendInsight(m_TempBuf, index);
			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwPiercingShieldTime - (dwCurrTick - m_dwLastPiercingShieldTime);
			m_dwPiercingShieldTime = dwRemainTime;
			m_dwLastPiercingShieldTime = dwCurrTick;
		}
	}

	if( m_dwNoDamageTime != 0)	// Game Start, Zone Change 矫狼 公利矫埃
	{
		if((dwCurrTick - m_dwLastNoDamageTime) > m_dwNoDamageTime)		// 公利矫埃捞 钱副 矫埃捞搁
		{
			m_dwNoDamageTime = 0;
			m_dwLastNoDamageTime = dwCurrTick;
		}
		else
		{
			dwRemainTime = m_dwNoDamageTime - (dwCurrTick - m_dwLastNoDamageTime);
			m_dwNoDamageTime = dwRemainTime;
			m_dwLastNoDamageTime = dwCurrTick;
		}
	}
       /*
       if( m_dwHiExpTime != 0 )	// 兴奋时间  泡兴奋
	{

		if((dwCurrTick - m_dwLastHiExpTime) > m_dwHiExpTime &&  IsCity() == FALSE )		// 版氰摹 2硅 矫埃捞 场车栏搁
		{
			HuanYuanBianShen();
			DeleteAbnormalInfo(ABNORMAL_HIEXP);
			m_dwHiExpTime = 0;
			m_dwLastHiExpTime = dwCurrTick;

			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
			Send(m_TempBuf, index);	
			SendSystemMsg("您的【兴奋时间】已耗尽!",SYSTEM_ERROR,TO_ME);
			
		}
		else
		{
			if (IsCity() == FALSE)
			{
			
				dwRemainTime = m_dwHiExpTime - (dwCurrTick - m_dwLastHiExpTime);
				m_dwHiExpTime = dwRemainTime;
				}
			
			m_dwLastHiExpTime = dwCurrTick;

		


	}	
 }

     
  if( m_dwHtExpTime != 0)	//幻灵
	{
		if((dwCurrTick - m_dwLastHtExpTime) > m_dwHtExpTime &&  IsCity() == FALSE )
		{
			HuanYuanBianShen();
			DeleteStateInfo(STATE_5);
			m_dwHtExpTime = 0;
			m_dwLastHtExpTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
			Send(m_TempBuf, index);	
			SendSystemMsg("您的【幻灵时间】已耗尽!",SYSTEM_ERROR,TO_ME);
		}
		else
		{
			if (IsCity() == FALSE)
			{
			dwRemainTime = m_dwHtExpTime - (dwCurrTick - m_dwLastHtExpTime);
			m_dwHtExpTime = dwRemainTime;
			}
			m_dwLastHtExpTime = dwCurrTick;
		
	}
  }

	 
   if( m_dwMagicFindTime != 0 	)// 幸运时间
	{

		if((dwCurrTick - m_dwLastMagicFindTime) > m_dwMagicFindTime &&  IsCity() == FALSE )		// 概流蛮胶 5硅 矫埃捞 场车栏搁
		{
			HuanYuanBianShen();
			DeleteAbnormalInfo(ABNORMAL_MAGICFIND);
			m_dwMagicFindTime = 0;
			m_dwLastMagicFindTime = dwCurrTick;

			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
			Send(m_TempBuf, index);	
			SendSystemMsg("您的【幸运时间】已耗尽!",SYSTEM_ERROR,TO_ME);
		}
		else
		{
			   if (IsCity() == FALSE)
			{
				dwRemainTime = m_dwMagicFindTime - (dwCurrTick - m_dwLastMagicFindTime);
				m_dwMagicFindTime = dwRemainTime;
				}
			    m_dwLastMagicFindTime = dwCurrTick;
		


	}
 }

	// if(!IsCity())
 //{
    if( m_dwMagicFtTime != 0)	//幻晶
	{
		if((dwCurrTick - m_dwLastMagicFtTime) > m_dwMagicFtTime &&  IsCity() == FALSE )
		{
			HuanYuanBianShen();
			DeleteStateInfo(STATE_28);
			m_dwMagicFtTime = 0;
			m_dwLastMagicFtTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
			Send(m_TempBuf, index);
			SendSystemMsg("您的【幻晶时间】已耗尽!",SYSTEM_ERROR,TO_ME);
		}
		else
			{
			if(IsCity() == FALSE)
		{
			dwRemainTime = m_dwMagicFtTime - (dwCurrTick - m_dwLastMagicFtTime);
			m_dwMagicFtTime = dwRemainTime;
			}
			m_dwLastMagicFtTime = dwCurrTick;
		
	}
 }
	*/


	
	

/*	if( m_dwNoChatTime != 0)	// 盲泼陛瘤 巢篮 矫埃 拌魂
	{
		if((dwCurrTick - m_dwLastNoChatTime) > m_dwNoChatTime)		// 盲泼陛瘤 矫埃捞 场车栏搁
		{
			m_dwNoChatTime = 0;
			m_dwLastNoChatTime = dwCurrTick;
		}
		else
		{
			dwRemainTime = m_dwNoChatTime - (dwCurrTick - m_dwLastNoChatTime);
			m_dwNoChatTime = dwRemainTime;
			m_dwLastNoChatTime = dwCurrTick;
		}
	}*/

	
	
    if( m_dwCloseTime != 0)	// 监狱记时
	{
		if((dwCurrTick - m_dwLastCloseTime) > m_dwCloseTime)	
		{
			Fchuuser();
			SendSystemMsg("封号时间到,你被系统自动释放",SYSTEM_ERROR,TO_ME);
			m_dwCloseTime = 0;			
			m_dwLastCloseTime = dwCurrTick;
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);			
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwCloseTime - (dwCurrTick - m_dwLastCloseTime);
			m_dwCloseTime = dwRemainTime;
			m_dwLastCloseTime = dwCurrTick;
		}
	}
	if(m_UserItem[39].sSid != 0 && (m_iSkin == 5 || m_iSkin == 8 || m_iSkin == 170) )
	{
		if((dwCurrTick - m_dwLastGuardianTime) > m_dwGuardianTime)		//机甲掉耐久
		{			
			m_dwGuardianTime = 60000;			
			if(m_UserItem[39].sBullNum > 0)
			{
				m_UserItem[39].sBullNum -= 60;
				if(m_UserItem[39].sBullNum < 0) m_UserItem[39].sBullNum = 0;
				SendBullNum(39);
				if(m_UserItem[39].sBullNum <= 0) 
				{
				 m_iSkin=0 , m_iHair=0;
		         SendMyInfo( TO_INSIGHT, INFO_MODIFY );
				}
			}

			m_dwLastGuardianTime = GetTickCount();
		}
		else
		{
			dwRemainTime = m_dwGuardianTime - (dwCurrTick - m_dwLastGuardianTime);
			m_dwGuardianTime = dwRemainTime;
			m_dwLastGuardianTime = dwCurrTick;
		}		
	}	
    
	if(m_dwTransTime != 0) //狂暴机甲时间判定
	{		
		if((dwCurrTick - m_dwLastTransTime) > m_dwTransTime)		// 变身时间判断
		{
			m_dwTransTime = 0;
			m_dwLastTransTime = dwCurrTick;
			if(m_UserItem[39].sSid == 1184)
			if(m_dwTransTime == 0 && m_iSkin == 170)
			{ 
				if(m_UserItem[39].tMagic[5] == 1)	m_iSkin  = 8;
				else if(m_UserItem[39].tMagic[5] == 2) m_iSkin  = 8;
				else m_iSkin  = 8;
			}else if(m_dwTransTime == 0 && m_iHair == 10015)
			{
				if(m_UserItem[39].tMagic[5] == 1)	m_iHair = 10001;
				else if(m_UserItem[39].tMagic[5] == 2) m_iHair = 10002;
				else m_iHair = 0;
			}	
	
			CheckMagicItemMove();
			SendMyInfo(TO_INSIGHT, INFO_MODIFY);
			
			DeleteStateInfo(STATE_18);						
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
			Send(m_TempBuf, index);			
			SendSystemMsg("狂暴时间已到极限,恢复正常!", SYSTEM_ERROR, TO_ME);
		}
		else
		{
			dwRemainTime = m_dwTransTime - (dwCurrTick - m_dwLastTransTime);
			m_dwTransTime = dwRemainTime;
			m_dwLastTransTime = dwCurrTick;
		}
	}
	 if( m_dwLhslTime != 0)	//灵魂锁链
	{
		if((dwCurrTick - m_dwLastLhslTime) > m_dwLhslTime)
		{
			DelHuFaStatus(New_Status_19);
			SendJudgePsiStatus();
			m_dwLhslTime = 0;
			m_dwLastLhslTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
			Send(m_TempBuf, index);			
		}
		else
		{
			dwRemainTime = m_dwLhslTime - (dwCurrTick - m_dwLastLhslTime);
			m_dwLhslTime = dwRemainTime;
			m_dwLastLhslTime = dwCurrTick;
		}
	}

	


	//在线挂机送元宝
	
	////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////
	// pk count 钱绢林扁
	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	if( rank == ANGEL_RANK )		
	{
		if( dwCurrTick - m_dwSaintTimeCount >= 60*1000 )
		{
			CheckMaxValue(m_dwSaintTime, 1);

			if( m_dwSaintTime >= 300 )
			{
				// killcount甫 窍唱 临咯霖促.
				m_sKillCount -= 1;
				if( m_sKillCount < 0 ) m_sKillCount = 0;
				m_dwSaintTime = 0;
			}

			m_dwSaintTimeCount = dwCurrTick;
		}
	}
	else
	{
		m_dwSaintTime = 0;
		m_dwSaintTimeCount = dwCurrTick;
	}

	//惑怕捞惑 贸府风凭 荐青 --------------------------------------//

	switch(m_tAbnormalKind)
	{
	case ABNORMAL_BYTE_NONE:
		return;
	case ABNORMAL_BYTE_POISON:
		break;
	case ABNORMAL_BYTE_CONFUSION:
		break;
	case ABNORMAL_BYTE_PARALYSIS:
		break;
	case ABNORMAL_BYTE_BLIND:
		break;
	case ABNORMAL_BYTE_LIGHT:
		break;
	case ABNORMAL_BYTE_FIRE:
		dwDiffTime = dwCurrTick - m_dwLastAbnormalTime;

		if(dwDiffTime >= 5000)	
		{
			SetDamage((int)((double)m_sMaxHP * 0.02 + 0.5));
			//if(m_bLive == USER_DEAD) 
			if(m_lDeadUsed == 1)
			{
				GetLevelDownExp(USER_PK, -1, FALSE,m_strUserID);			// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
				return;
			}
		}

		if((dwCurrTick - m_dwLastAbnormalTime) > dwAbnormalTime)	// Fire 惑怕捞惑捞 钱副 矫埃捞 夌栏搁
		{
			m_tAbnormalKind = ABNORMAL_BYTE_NONE;
			m_dwAbnormalTime = 0;
			m_dwLastAbnormalTime = GetTickCount();

			if(g_bDebug) SendSystemMsg( IDS_USER_FIRE_DAMAGE_RESET, SYSTEM_NORMAL, TO_ME);

			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			DeleteAbnormalInfo(ABNORMAL_FIRE);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);
			
			Send(TempBuf, TempBuf.GetLength());
		}
		else
		{
			dwRemainTime = dwAbnormalTime - (dwCurrTick - m_dwLastAbnormalTime);
			m_dwAbnormalTime = dwRemainTime;
			m_dwLastAbnormalTime = dwCurrTick;
		}

		break;

	case ABNORMAL_BYTE_COLD:
		if((dwCurrTick - m_dwLastAbnormalTime) > dwAbnormalTime)	// Cold 惑怕捞惑捞 钱副 矫埃捞 夌栏搁
		{
			m_tAbnormalKind = ABNORMAL_BYTE_NONE;
			m_dwAbnormalTime = 0;
			m_dwLastAbnormalTime = GetTickCount();

			if(g_bDebug) SendSystemMsg( IDS_USER_COLD_DAMAGE_RESET, SYSTEM_NORMAL, TO_ME);

			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			DeleteAbnormalInfo(ABNORMAL_COLD);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);
			
			Send(TempBuf, TempBuf.GetLength());
		}
		else
		{
			dwRemainTime = dwAbnormalTime - (dwCurrTick - m_dwLastAbnormalTime);
			m_dwAbnormalTime = dwRemainTime;
			m_dwLastAbnormalTime = dwCurrTick;

			// Test Code !!!!
//			szMsg.Format("Cold - %d", dwRemainTime);
//			if(g_bDebug) SendSystemMsg((LPTSTR)(LPCTSTR)szMsg, SYSTEM_NORMAL, TO_ME);
		}
		break;
	}
}

void USER::FreedomTS()
{

}

void USER::FreedomCB() //绿翅膀
{
	bool FreedomTS = false;

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	if(isGuildRen || m_bCBOff)
	{
		AddStateInfo(STATE_6);//绿翅膀效果
        AddHuFaStatus(New_Status_22);// IP显示特殊符号以及头上带NEW
	}
	else
	{
		DeleteStateInfo(STATE_6);
		DelHuFaStatus(New_Status_22);//删除NEW
	}
	if((isGuildRen || m_bCBOff) && m_iSkin == 0)
	{	
		FreedomTS = true;
		m_dwAbnormalInfo_ |=448;
	}
	if(m_dwShTsTime > 0 || m_dwGuarDianTianShi > 0)//守护天使时间不为空时和VIP用户
	{
		FreedomTS = true;
		m_dwAbnormalInfo_ |=139;
	}	

	     ////NEO版本
	
		   
	if (o_yehuoini[0]->bbzh == 0)        //测试闪光
{
	//if(m_dwGuarDianTianShi > 0)      ////NEO版本
	if(isGuildRen == TRUE || m_bCBOff == TRUE )  
	{
		AddHuFaStatus(New_Status_22);//VIP显示特殊符号以及头上带NEW
		SendJudgePsiStatus();
	}
	else
	{
		DelHuFaStatus(New_Status_22);
		SendJudgePsiStatus();
	}
}


	SetStatus();
	SendBabyInfo();//发送宠物状态防止显示别人宠物
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength()); //给me发送
	SendInsight(TempBuf, TempBuf.GetLength()); //给所有人发送	
}

void USER::SetPsiAbnormalStatus()
{
	if(m_tAbnormalKind != ABNORMAL_BYTE_NONE)
	{
		switch(m_tAbnormalKind)
		{
		case ABNORMAL_BYTE_FIRE:
			AddAbnormalInfo(ABNORMAL_FIRE);
			break;
		case ABNORMAL_BYTE_COLD:
			AddAbnormalInfo(ABNORMAL_COLD);
			break;
		}
	}
    
    if( m_dwHasteTime != 0 )							AddAbnormalInfo(ABNORMAL_HASTE);
	if( m_dwShieldTime != 0 || m_bNecklaceOfShield)		AddAbnormalInfo(ABNORMAL_SHIELD);
	if( m_dwDexUpTime != 0 )							AddAbnormalInfo(ABNORMAL_DEX_UP);
	if( m_dwMaxHPUpTime != 0 )							AddAbnormalInfo(ABNORMAL_MAX_HP_UP);

	if( m_dwFastRunTime != 0 )			AddAbnormalInfo(ABNORMAL_FASTRUN);
	if( m_dwMindShockTime != 0 )		AddAbnormalInfo(ABNORMAL_MINDSHOCK);
	if( m_dwMindGuardTime != 0 )		AddAbnormalInfo(ABNORMAL_MINDGUARD);
	if( m_dwPsiShieldTime != 0 )		AddAbnormalInfo(ABNORMAL_PSISHIELD);
	if( m_dwBigShieldTime != 0 )		AddAbnormalInfo(ABNORMAL_BIGSHIELD);
	if( m_dwPiercingShieldTime != 0 )	AddAbnormalInfo(ABNORMAL_PIERCING_SHIELD);

	if( m_dwAdamantineTime != 0 )	AddAbnormalInfo(ABNORMAL_ADAMANTINE);
	if( m_dwMightyWeaponTime != 0 )	AddAbnormalInfo(ABNORMAL_MIGHTYWEAPON);
	if( m_dwBerserkerTime != 0 )	AddAbnormalInfo(ABNORMAL_BERSERKER);

	if( m_dwHiExpTime != 0)			AddAbnormalInfo(ABNORMAL_HIEXP);
	if( m_dwMagicFindTime != 0)		AddAbnormalInfo(ABNORMAL_MAGICFIND);
	//if( m_sLevel > 0)	AddAbnormalInfo(ABNORMAL_VIP); // VIP用户显示图
 
    if( m_dwCloseTime != 0 )			AddAbnormalInfo(ABNORMAL_JIANYU);//监狱
    if( m_dwMagicFtTime != 0 )			AddAbnormalInfo(ABNORMAL_BIANSHEN);
	if( m_dwVIPTime != 0 )		AddAbnormalInfo(ABNORMAL_FUDAI);  //幸福时光图
	
	

}
void USER::SetStatus()
{
	AddStateInfo(STATE_1);
	//if(m_iSkin != 0)					AddStateInfo(STATE_2);
	if(m_dwBFindTime != 0)					AddStateInfo(STATE_2);
	if(m_dwHtExpTime > 0)				AddStateInfo(STATE_5); //武器+1
	if(m_dwBHTime> 0)//天使图标
    {
      AddStateInfo(STATE_8);
    }
	if(isGuildRen == TRUE || m_bCBOff == TRUE )         
			{	
		AddStateInfo(STATE_6);//绿翅膀效果
	 AddHuFaStatus(New_Status_22);// IP显示特殊符号以及头上带NEW
	 }
	if(m_dwMagicFtTime > 0)				AddStateInfo(STATE_28); //防局+1
	if(m_dwTransTime != 0)				AddStateInfo(STATE_18); //Trans
	if(m_dwSJTime !=0 )                AddStateInfo(STATE_27); //神迹药水
	
	if(m_dwLhslTime != 0)				
	{	
		AddHuFaStatus(New_Status_19); //灵魂锁链
		SendJudgePsiStatus();
	}
	else
	{
		DelHuFaStatus(New_Status_19);
		SendJudgePsiStatus();
	}
	
	if(m_tBabyCall ==1)//首先得是有宠
    {
	   if(m_sFealty == 100)//宠忠诚度是100
       {
		   AddHuFaStatus(New_Status_17);//执行群攻状态
		   SendJudgePsiStatus();
	   }
	   else
	   {
		   DelHuFaStatus(New_Status_17);//删除群攻状态
		   SendJudgePsiStatus();
       }
    }
	
	if((m_UserItem[1].sSid == 1489) || (m_UserItem[1].sSid == 1488) || (m_UserItem[1].sSid == 1479) || (m_UserItem[1].sSid == 1356) || (m_UserItem[1].sSid == 1787) || (m_UserItem[1].sSid == 1788))//飞机摩托鲁道夫加速状态
    {
		AddHuFaStatus(New_Status_2);
		SendJudgePsiStatus();
	}
	else
	{
		DelHuFaStatus(New_Status_2);
		SendJudgePsiStatus();
	}
    //////////////////////////////////////测试  NEO版本开放  //测试闪光
		if (o_yehuoini[0]->bbzh == 0)
{
		if(m_dwGuarDianTianShi > 0)		
	{
		AddStateInfo(STATE_31);
	}
	else
	{
		DeleteStateInfo(STATE_31);
	}
}


	
	//////////////////////////////////////////////////////

	if( m_isDoubleExp > 0 || g_sanJingYan == TRUE ) AddStateInfo(STATE_21);
	if( m_isDoubleBAOLV > 0 || g_sanBaoLv == TRUE ) AddStateInfo(STATE_22);
}
BOOL USER::ReturnTown(BYTE type, BYTE slot)
{
	int x, y;
	int isZone = -1;
	int iPotalIndex = -1;
	BYTE result = SUCCESS;
	BYTE zoneChange = 0;
	CPoint ptTown, pt;
	
	CBufferEx TempBuf;

	pt = GetTownPotal( isZone );
	x = pt.x;
	y = pt.y;

	if( isZone < 0 ) // 酒聪搁 鞍篮 粮救栏肺 捞悼
	{
		SendTownPotal( FAIL, zoneChange );
		return TRUE;
	}

	if( IsZoneInThisServer( isZone ) )	// 捞悼窍妨绰 粮捞 鞍篮 辑滚救俊 乐阑锭
	{
		TownPotal();
		return TRUE;
	}

	if( slot < EQUIP_ITEM_NUM || slot >= TOTAL_INVEN_MAX )
	{
		SendTownPotal( FAIL, zoneChange );
		return TRUE;
	} 

	int sSid = m_UserItem[slot].sSid;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) 
	{
		SendTownPotal( FAIL, zoneChange );
		return TRUE;
	}

	if( m_iCurWeight <= g_arItemTable[sSid]->m_byWeight ) m_iCurWeight = 0;
	else m_iCurWeight -= g_arItemTable[sSid]->m_byWeight;		// 敬父怒 公霸甫 临牢促.

	m_UserItem[slot].sCount -= 1;
	if(m_UserItem[slot].sCount <= 0) { ReSetItemSlot(&m_UserItem[slot]); sSid = -1; }

	UpdateUserItemDN();							// 酒捞袍 函悼 眉农...

	GetRecoverySpeed();							// 雀汗加档 眉农...

	ZoneMoveReq( isZone, x, y );
	return FALSE;
}

BOOL USER::GetAbsentGuildInfo(int guildsid)
{
	int i;

	CGuild* pNewGuild = NULL;

	pNewGuild = GetGuild( guildsid );

	if( pNewGuild )
	{
		if( (int)pNewGuild->m_lSid == guildsid )
		{
			ReleaseGuild();
			return TRUE;
		}
	}

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));

	wsprintf(szSQL,TEXT("SELECT * FROM GUILD where iSid = %d"), guildsid);
	
	SQLUINTEGER		iDN;//, iEXP;
	SQLINTEGER		iSID;
	SQLSMALLINT		sVERSION;
	SQLCHAR			strGuildName[CHAR_NAME_LENGTH + 1], strMasterName[CHAR_NAME_LENGTH + 1];
	SQLCHAR			strItem[_BANK_DB], strMark[GUILD_MARK_SIZE], strHaveMap[GUILD_MAP_COUNT];

	SQLINTEGER		sInd;

	iSID = 0;
	iDN = 0;//iEXP = 0;
	sVERSION = 0;

	g_CurrentGuildCount = 0;			// 泅犁 殿废茄 辨靛狼 付瘤阜 锅龋 悸泼

	::ZeroMemory(strGuildName, sizeof(strGuildName));
	::ZeroMemory(strMasterName, sizeof(strMasterName));
	::ZeroMemory(strMark, sizeof(strMark));
	::ZeroMemory(strHaveMap, sizeof(strHaveMap));
	::ZeroMemory(strItem, sizeof(strItem));

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB )
	{
		ReleaseGuild();
		return FALSE;
	}

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild Data !!\n");
		ReleaseGuild();
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strGuildName, sizeof(strGuildName), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strMasterName, sizeof(strMasterName), &sInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY, &strMark, sizeof(strMark), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sVERSION, sizeof(sVERSION), &sInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG, &iDN, sizeof(iDN), &sInd );

				SQLGetData( hstmt, i++, SQL_C_BINARY, &strItem,	sizeof(strItem), &sInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY, &strHaveMap, sizeof(strHaveMap), &sInd );

				if(iSID <= 0 || iSID >= MAX_GUILD ) 
				{
					TRACE(_T("辨靛 傈眉 农扁啊 MAX_GUILD甫 逞菌促."));
					continue;
				}

				pNewGuild = NULL;
				pNewGuild = g_arGuildData[iSID];

				pNewGuild->m_lSid = iSID;
				strcpy(pNewGuild->m_strGuildName, (CHAR*)strGuildName);
				strcpy(pNewGuild->m_strMasterName, (CHAR*)strMasterName);
				pNewGuild->m_sVersion = sVERSION;
				pNewGuild->m_dwGuildDN = iDN;

				pNewGuild->StrToGuildItem((LPTSTR)strItem);
				pNewGuild->strToGuildMap((LPTSTR)strHaveMap);
				::CopyMemory(pNewGuild->m_strMark, strMark, sizeof(pNewGuild->m_strMark));

				g_CurrentGuildCount = iSID;

				::ZeroMemory(strGuildName, sizeof(strGuildName));
				::ZeroMemory(strMasterName, sizeof(strMasterName));
				::ZeroMemory(strMark, sizeof(strMark));
				::ZeroMemory(strHaveMap, sizeof(strHaveMap));
				::ZeroMemory(strItem, sizeof(strItem));
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[m_iModSid].ReleaseDB(db_index);
		ReleaseGuild();
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(db_index);
		ReleaseGuild();
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	GetAbsentGuildUserInfo( guildsid );

	ReleaseGuild();

	return TRUE;	
}

void USER::GetAbsentGuildUserInfo(int guildsid)
{
	int i;
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM GUILD_USER where iSid = %d"), guildsid);

	SQLINTEGER		iSID;
	SQLCHAR			strUserId[CHAR_NAME_LENGTH + 1];

	SQLINTEGER		sInd;

	iSID = 0;
	::ZeroMemory(strUserId, sizeof(strUserId));

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild User Data !!\n");
		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strUserId, sizeof(strUserId), &sInd );

				if(iSID < 0 || iSID >= MAX_GUILD ) continue;

				if(!g_arGuildData[iSID]) continue;

				g_arGuildData[iSID]->AddUser((TCHAR *)strUserId, iSID);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	return;
}

BOOL USER::IsMyDBServer(int myserver)
{
	for( int i = 0; i < g_arMyServer.GetSize(); i++ )
	{
		
		if( g_arMyServer[i] )
		{
			TRACE( "g_arMyServer: %d\n", *(g_arMyServer[i]));
			if( *(g_arMyServer[i]) == myserver )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
//游戏人物上线
void USER::BridgeServerUserLogin(char *strAccount, char *strUserID)
{
	if( g_bShutDown ) return;

	int			index = 0;
	BYTE		result = FAIL, error_code = 0;
	TCHAR		szID[CHAR_NAME_LENGTH+1];
	CPoint		pt;
	CBufferEx	TempBuf;
	int			iMemory = 0;
	int			iMemoryAccountBank = 0;

	strcpy( szID, strUserID );

	InitUser();
	iMemory = CheckMemoryDB( szID );
//	iMemory = 0;		// 酒第 鞍篮 巴档 绝菌绊, 拌沥 鞍篮 巴档 绝菌促.
//						// 酒第,篮青,烹芒 葛滴 DB俊辑 啊廉客具 茄促.

//	iMemory = 1;		// 酒第尔 拌沥捞 鞍篮 巴篮 乐菌绊, 拌沥父 鞍绊 酒第啊 促弗巴篮 绝菌促.
//						// 酒第,篮青,烹芒 葛滴 MemoryDB俊辑 啊廉吭促. DB俊辑 啊廉坷瘤 臼绰促.

//	iMemory = 1;		// 酒第尔 拌沥捞 鞍篮 巴捞 乐菌绊, 拌沥父 鞍绊 酒第啊 促弗巴档 乐菌促.
//						// 酒第,篮青,烹芒 葛滴 MemoryDB俊辑 啊廉吭促. DB俊辑 啊廉坷瘤 臼绰促.

//	iMemory = 2;		// 酒第尔 拌沥捞 鞍篮 巴篮 绝菌绊, 拌沥父 鞍篮 巴捞 乐菌促.
//						// 酒第,篮青,烹芒 葛滴 DB俊辑 啊廉客具 茄促. (唱吝俊 烹芒父 MemoryDB俊辑 啊廉坷绰 风凭阑 持阑巴捞促.)

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserData( szID ) )
		{
			//AfxMessageBox("User Data Load Fail");
			error_code = ERR_2;
			goto result_send;
		}
	}

	m_state = STATE_CONNECTED;

	// 泅犁 辑 乐绰 镑捞 肺锯烦喉 粮捞绊 包恩籍捞 酒囱 版扁厘 加己捞扼搁
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum && CheckInvalidMapType() == 12 )
	{
		m_curz = 57;
		SetZoneIndex( m_curz );

		pt = ConvertToServer( 20, 34 );
		m_curx = pt.x;
		m_cury = pt.y;
	}
#else		// 烙矫肺 弥碍 捞亥飘锭巩俊
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		if( CheckInvalidMapType() == 12 )
		{
			m_curz = 57;
			SetZoneIndex( m_curz );

			pt = ConvertToServer( 20, 34 );
			m_curx = pt.x;
			m_cury = pt.y;
		}
	}
#endif

	pt = FindNearAvailablePoint_S(m_curx, m_cury);	// DB俊 历厘等 谅钎啊 框流老 荐 乐绰 谅钎牢瘤 魄窜
	if(pt.x == -1 || pt.y == -1) 
	{
		// TEST...
		//error_code = ERR_4;
		//goto result_send;
	//	m_curz = 400;//出生非利普地图坐标
	//	m_curx = 315;
	//	m_cury = 635;

		m_curz = 1;//出生非利普地图坐标
		m_curx = 1311;
		m_cury = 339;

		if( !IsZoneInThisServer( m_curz ) )
		{
			UpdateUserData( TRUE );
			error_code = ERR_4;
			goto result_send;
		}

		SetZoneIndex(m_curz);
		pt = FindNearAvailablePoint_S(m_curx, m_cury);	// DB俊 历厘等 谅钎啊 框流老 荐 乐绰 谅钎牢瘤 魄窜
	}

	m_curx = pt.x; m_cury = pt.y;
	SetUid(m_curx, m_cury, m_uid + USER_BAND );		// 蜡历 困摹沥焊 悸泼
	m_presx = -1;
	m_presy = -1;

	pt = ConvertToClient(m_curx, m_cury);

//	SetUserToMagicUser();							// 泅犁 瓷仿摹甫 悼利 蜡历函荐俊 歹茄促.
	GetMagicItemSetting();							// 泅犁 酒捞袍吝 概流 加己, 饭骇函悼阑 悼利 函荐俊 馆康茄促.

	m_UserFlag = TRUE;

	result = SUCCESS;

	m_ConnectionSuccessTick = 0;

	InitMemoryDB(m_uid);

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
		if( !LoadAccountBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
	}

result_send:
	TempBuf.Add(GAME_START_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		ReInitMemoryDB();

		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
//		SockCloseProcess();
		return;
	}

					// 蜡历狼 惑技 沥焊甫 焊辰促.
	SendCharData();
	//添加超级炫盒功能
	m_dwAbnormalInfo_ |= ABNORMAL_POISON;
	FreedomCB();
	//m_dwAbnormalInfo_ |=ABNORMAL_COLD;
	//m_dwAbnormalInfo_=0x00000001;
	TempBuf.Add((short)m_curz);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);

	TempBuf.Add(m_tDir);

	TempBuf.Add((BYTE)g_GameTime);
	TempBuf.Add((BYTE)g_GameMinute);
	Send(TempBuf, TempBuf.GetLength());
		
	SetGameStartInfo();

	CheckUserLevel();								// 公丰眉氰饭骇牢 25饭骇阑 逞菌阑 版快
	

//	SendSystemMsg( IDS_USER_OPERATOR_MAIL1, SYSTEM_NORMAL, TO_ME);
//	SendSystemMsg( IDS_USER_OPERATOR_MAIL2, SYSTEM_NORMAL, TO_ME);//一条龙信息
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>要塞翅膀
	SetIsGuildCB();
	//BBSOpen(1);
	FreedomCB();
	if(isGuildRen == TRUE || m_bCBOff == TRUE ) 
	{
	//m_dwAbnormalInfo_	= 448;
	m_dwAbnormalInfo_ |= ABNORMAL_POISON;
	FreedomCB();
	//SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		}
	else 
	{
		if(isGuildRen == FALSE || m_bCBOff == FALSE  )
		{
	m_dwAbnormalInfo_	= ABNORMAL_NONE;
	m_dwAbnormalInfo_ |= ABNORMAL_POISON;
	FreedomCB();
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		}
	}	
////////////////////测试上线显示VIP头顶NEW///////////////////////////   NEO版本  //测试闪光
		if (o_yehuoini[0]->bbzh == 0)
{
	//if(m_dwGuarDianTianShi > 0)
	if(isGuildRen == TRUE || m_bCBOff == TRUE )  
	{
		AddHuFaStatus(New_Status_22);// IP显示特殊符号以及头上带NEW
		SendJudgePsiStatus();
	}
	else
	{
		DelHuFaStatus(New_Status_22);
		SendJudgePsiStatus();
	}
}

////////////////////测试上线显示VIP头顶NEW///////////////////////////
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>翅膀戒子	
//BBSOpen(1);
//	if (m_sLevel == 70)
//	{
//		 SendEventMsg("传送门内自带银行,魔法商店,军团等地传送点.");
  //  }
 
    CString strMsg1 = _T("");
	 if (m_dwGuarDianTianShi != 0)
	{
	strMsg1.Format( "荣誉的VIP用户【%s】登陆游戏.", strUserID);
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_NORMAL, TO_ALL);
	}
	
	 
	if(m_dwCloseTime != 0) //
	{
		SoftClose();
		strMsg1.Format( "你的角色已被封，请联系GM！");//
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_SPECIAL, TO_ME);
	}
	if(m_tIsOP != 1)
	{
		strMsg1.Format( "Lv.%d  %s 上线!", m_sLevel, strUserID);//普通玩家上线提示
	  SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_NORMAL, TO_ALL);
	}

		
	/*char sendMsg[100] = "";
	char sendExp[20] = "";
	strcat(sendMsg,"当前经验:");
	sprintf(sendExp,"%d",m_dwExp);
	strcat(sendMsg,sendExp);
	strcat(sendMsg," | 升级经验:");
	sprintf(sendExp,"%d",m_dwExpNext);
	strcat(sendMsg,sendExp);
	SendSystemMsg(sendMsg, SYSTEM_NORMAL, TO_ME);*/
	//SendSystemMsg("英伦决战-打造最简单的NEO专区!", SYSTEM_NORMAL, TO_ME);
	//SendSystemMsg("游戏过程中如有疑问请联系网站客服!", SYSTEM_NORMAL, TO_ME);
	GetLocalTime(&m_LoginTime);		// 肺弊牢茄 矫埃阑 汲沥

	
	GetLocalTime(&m_LoginTime);		// 肺弊牢茄 矫埃阑 汲沥
	
	if (m_dwAutoMoney > 0)		//初始化杀人狂的杀人记录.
	{
		SYSTEMTIME messageTime;
		GetLocalTime(&messageTime);			
		COleDateTime CurrTime = COleDateTime(messageTime);

		if( messageTime.wHour != 20 ) //活动时间外赋值为0
		{
			m_dwAutoMoney = 0;
		}
	}

	if(isDoubleExp)
	{
		SendSystemMsg("服务器现在是双倍经验活动中!",SYSTEM_ANNOUNCE,TO_ME);
	}
	
    if(isDoubleExpup4)
	{
		SendSystemMsg("服务器现在是4倍经活动中!",SYSTEM_ANNOUNCE,TO_ME);
	}
    
//	if (m_dwBHTime !=0 )		//三倍爆率上线提示
//	{
		//SetXingYun();
//		SendSystemMsg("您当前是【保护卡闭关】状态中!",SYSTEM_NPC, TO_ME);
//	}
//	if (m_dwSJTime !=0 )		//三倍经验上线提示
//	{
		//SetXingfen();
//		SendSystemMsg("您当前是【神迹】状态中!",SYSTEM_NPC, TO_ME);
//	}
	

	  //     if (m_dwBHTime !=0 )
        // {
          //  int h = m_dwBHTime/1000/3600;
			//int m = m_dwBHTime/1000/60;
			//int s = (m_dwBHTime/1000)%60;
			//CString str;
			//str.Format("您的闭关时间还剩%d时%d分%d秒!",h,m,s);
		//	}

	       if (m_dwSJTime !=0 )
         {
			int h = m_dwSJTime/(3600*1000);
			int m = m_dwSJTime%(3600*1000)/60000;
			CString str;
			str.Format("您的神迹时间还剩%d时%d分!",h,m);
			CBufferEx	TempBuf,TempBuf1;
			TempBuf.Add((byte)CHAT_RESULT);
			TempBuf.Add((byte)1);
			TempBuf.Add(WHISPER_CHAT);
			TempBuf.Add((int)0x00);
			TempBuf.Add("", _tcslen(""));
			TempBuf.Add(str.GetBuffer(0), str.GetLength());
			Send(TempBuf, TempBuf.GetLength());
			}
		   if (m_dwBHTime !=0 )
         {
			int h = m_dwBHTime/(3600*1000);
			int m = m_dwBHTime%(3600*1000)/60000;
			CString str;
			str.Format("您的天使双倍时间还剩%d时%d分!",h,m);
			CBufferEx	TempBuf,TempBuf1;
			TempBuf.Add((byte)CHAT_RESULT);
			TempBuf.Add((byte)1);
			TempBuf.Add(WHISPER_CHAT);
			TempBuf.Add((int)0x00);
			TempBuf.Add("", _tcslen(""));
			TempBuf.Add(str.GetBuffer(0), str.GetLength());
			Send(TempBuf, TempBuf.GetLength());
			}

	if(/*m_byClass==JUDGE&&*/m_tHuFaType>0&&m_tHuFaType<=4)
	{
		if(m_dwLevealHuFaTime <=0)
			m_tHuFaType = 0;
		CString strName;
		switch(m_tHuFaType)
		{
		case 1:
			strName = "伊涂罗";
			break;
		case 2:
			strName = "普雷塔";
			break;
		case 3:
			strName = "克劳斯";
			break;
		case 4:
			strName = "伊西斯";
			break;
		}
		if(m_tHuFaType)
		{
			int m = m_dwLevealHuFaTime/1000/60;
			int s = (m_dwLevealHuFaTime/1000)%60;
			CString str;
			str.Format("您的护法\"%s\"持续时间还剩%d分%d秒!",strName,m,s);
			
			CBufferEx	TempBuf,TempBuf1;
			TempBuf.Add((byte)CHAT_RESULT);
			TempBuf.Add((byte)1);
			TempBuf.Add(WHISPER_CHAT);
			TempBuf.Add((int)0x00);
			TempBuf.Add("", _tcslen(""));
			TempBuf.Add(str.GetBuffer(0), str.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}
	}
	
	SendBabyInfo();
	GetCheckValidTime();			// 矫埃阑 钎矫茄促.
	CBufferEx	TempBuf1;
	TempBuf1.Add((byte)68);
	TempBuf1.Add(m_iSkin);
	TempBuf1.Add(m_iHair);
	TempBuf1.Add((BYTE)m_sGender);
	TempBuf1.AddData(m_strFace, 10);
	Send(TempBuf1, TempBuf1.GetLength());
	
}

void USER::SetIsGuildCB()
{
	CGuildFortress* pFort = NULL;
	int Fortress[] = {1000,1001,1002};
	if(m_dwGuild <= 0) return;
	for(int i = 0; i < 3;i++)
	{
		pFort = GetFortress(Fortress[i]);		
		if(pFort->m_iGuildSid <= 0)	return;		
		if(pFort->m_iGuildSid != m_dwGuild)	continue;
		if(pFort->m_iGuildSid == m_dwGuild) isGuildRen = true;		
	}
}

bool USER::SetServerDoubleExp() //经验 
{
	isDoubleExp = !isDoubleExp;
	if(isDoubleExp){ return true;}
	return false;
}

bool USER::SetServerDoubleExpup4() //经验 
{
	isDoubleExpup4 = !isDoubleExpup4;
	if(isDoubleExpup4){ return true;}
	return false;
}
/////////////////////////////////////
//捐款全服三倍
//
void USER::SetServerDoubleExp3() 
{
	if(m_dwDN < 20000000)
	{
	//	SendSystemMsg("敬爱的玩家,捐款需要2000W游戏币!",SYSTEM_ERROR,TO_ME);
		SendEventMsg("敬爱的玩家,捐款需要2000万游戏币!");
		return ;
	}

	m_dwDN = m_dwDN - 20000000;
	if (m_dwDN < 0) m_dwDN =0;
	
	UpdateUserItemDN();
	SendMoneyChanged();

	int nCount = 0;
	USER *pUser = NULL;
	nCount = 0;
	for (int i = 0; i < MAX_USER; i++ )
	{
		pUser = m_pCom->GetUserUid(i);
		if(pUser && pUser->m_state == STATE_GAMESTARTED )
		{
			pUser->m_isDoubleExp = 3600 * 2 * 1000;
			pUser->SetXingfen();
		}
	}
	CString str;
	str.Format("玩家[ %s ]牺牲了2000万游戏币.造福全服玩家打怪3倍经验2小时!!",m_strUserID);
	SendSystemMsg(str.GetBuffer(str.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
	SendUserStatusSkill();
}
/////////////////////////////////////
//捐款全服三倍爆率
//
void USER::SetServerDoubleBaoLv() 
{
	if(m_dwDN  < 20000000)
	{
	//	SendSystemMsg("敬爱的玩家,捐款需要2000W",SYSTEM_ERROR,TO_ME);
        SendEventMsg("敬爱的玩家,捐款需要2000万游戏币!");
		return ;
	}

	m_dwDN  = m_dwDN  - 20000000;
	if(m_dwDN <0) m_dwDN =0;

	UpdateUserItemDN();
	SendMoneyChanged();
	int nCount = 0;
	USER *pUser = NULL;
	nCount = 0;
	for (int i = 0; i < MAX_USER; i++ )
	{
		pUser = m_pCom->GetUserUid(i);
		if(pUser && pUser->m_state == STATE_GAMESTARTED )
		{
			pUser->m_isDoubleBAOLV = 3600 * 2 * 1000;
			pUser->SetXingYun();
		}
	}
	CString str;
	str.Format("玩家[ %s ]牺牲了2000万游戏币.造福全服玩家打怪3倍爆率2小时!!",m_strUserID);
	SendSystemMsg(str.GetBuffer(str.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
	SendUserStatusSkill();
}
void USER::BridgeServerUserZoneLogin(char *strAccount, char *strUserID)
{
	if( g_bShutDown ) return;

	int			index = 0;
	BYTE		result = FAIL, error_code = 0;
	TCHAR		szID[CHAR_NAME_LENGTH+1];
	CPoint		pt;
	CBufferEx	TempBuf;
	int			iMemory = 0;
	int			iMemoryAccountBank = 0;

	strcpy( szID, strUserID );

	InitUser();
//	InitMemoryDB(m_uid);
	iMemory = CheckMemoryDB( szID );
//	iMemory = 0;		// 酒第 鞍篮 巴档 绝菌绊, 拌沥 鞍篮 巴档 绝菌促.
//						// 酒第,篮青,烹芒 葛滴 DB俊辑 啊廉客具 茄促.

//	iMemory = 1;		// 酒第尔 拌沥捞 鞍篮 巴篮 乐菌绊, 拌沥父 鞍绊 酒第啊 促弗巴篮 绝菌促.
//						// 酒第,篮青,烹芒 葛滴 MemoryDB俊辑 啊廉吭促. DB俊辑 啊廉坷瘤 臼绰促.

//	iMemory = 1;		// 酒第尔 拌沥捞 鞍篮 巴捞 乐菌绊, 拌沥父 鞍绊 酒第啊 促弗巴档 乐菌促.
//						// 酒第,篮青,烹芒 葛滴 MemoryDB俊辑 啊廉吭促. DB俊辑 啊廉坷瘤 臼绰促.

//	iMemory = 2;		// 酒第尔 拌沥捞 鞍篮 巴篮 绝菌绊, 拌沥父 鞍篮 巴捞 乐菌促.
//						// 酒第,篮青,烹芒 葛滴 DB俊辑 啊廉客具 茄促. (唱吝俊 烹芒父 MemoryDB俊辑 啊廉坷绰 风凭阑 持阑巴捞促.)

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserData( szID ) )
		{
			error_code = ERR_2;
			goto result_send;
		}
	}

	/*
	if( !IsZoneInThisServer(m_curz) )
	{
		ChangeServer(m_curz);
		
		ReInitMemoryDB();

		SoftClose();
		return;
	}
	*/

	m_state = STATE_CONNECTED;

	// zone, zoneindex肺 谅钎 函券...
//	ZoneChangeInfoSet(m_curz, m_curx, m_cury);

	pt = FindNearAvailablePoint_S(m_curx, m_cury);	// DB俊 历厘等 谅钎啊 框流老 荐 乐绰 谅钎牢瘤 魄窜
	if(pt.x == -1 || pt.y == -1) 
	{
		error_code = ERR_4;
		goto result_send;
	}

//	CheckUserLevel();					// 公丰眉氰饭骇牢 25饭骇阑 逞菌阑 版快 

	m_curx = pt.x; m_cury = pt.y;
	SetUid(m_curx, m_cury, m_uid + USER_BAND );		// 蜡历 困摹沥焊 悸泼
	m_presx = -1;
	m_presy = -1;

	pt = ConvertToClient(m_curx, m_cury);

	GetMagicItemSetting();							// 泅犁 酒捞袍吝 概流 加己, 饭骇函悼阑 悼利 函荐俊 馆康茄促.

	m_UserFlag = TRUE;

	result = SUCCESS;

	m_ConnectionSuccessTick = 0;


	InitMemoryDB(m_uid);

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
		if( !LoadAccountBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
	}

	// 粮 眉牢瘤 犬牢棺 函荐 檬扁拳...
	m_strZoneIP = "";
	m_nZonePort	= -1;

	SetPsiAbnormalStatus();
    SetStatus();

result_send:
	TempBuf.Add(GAME_START_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		ReInitMemoryDB();

		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	
	SendCharData();						// 蜡历狼 惑技 沥焊甫 焊辰促.

	TempBuf.Add((short)m_curz);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);

	TempBuf.Add(m_tDir);

	TempBuf.Add((BYTE)g_GameTime);
	TempBuf.Add((BYTE)g_GameMinute);

	Send(TempBuf, TempBuf.GetLength());

	SetGameStartInfo();	

//	SendSystemMsg( IDS_USER_OPERATOR_MAIL1, SYSTEM_NORMAL, TO_ME);
//	SendSystemMsg( IDS_USER_OPERATOR_MAIL2, SYSTEM_NORMAL, TO_ME);

	GetLocalTime(&m_LoginTime);		
//	GetCheckValidTime();			// 矫埃阑 钎矫茄促.
}

void USER::BridgeServerUserWhisper(char *strSendID, char *strRecvID, char *strMsg)
{
	if(m_tIsOP == 1)						// 款康磊俊霸 庇富篮 救凳
	{
		return;
	}

	if( !strcmp( strSendID, strRecvID ) )	// 磊扁磊脚俊霸 庇富 -> 府畔
	{
		return;
	}

	CBufferEx TempBuf;

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(WHISPER_CHAT);
	TempBuf.Add((int)0x01);								// Client俊辑 辑肺 备盒窍扁困秦(惑措规 庇富 酒捞叼)
	TempBuf.AddString(strSendID);
	TempBuf.AddString(strMsg);			// pBuf == IDLen(1) + ID + MsgLen(1) + Msg
	Send(TempBuf, TempBuf.GetLength());
}

void USER::BridgeServerGuildNewResult(int guildnum, char *strGuildName)
{
	BYTE error_code = 0;
	BOOL bRes = TRUE;

	int i;
	int index = 0;
	int nLength = 0;
	int iGuildId = 0;

	CBufferEx	TempBuf;

	WORD *pwMark = NULL;
	CGuild *pGuild = NULL;

	nLength = strlen( strGuildName );
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) { error_code = ERR_1; goto go_result; } // 辨靛 疙莫 力茄俊 吧覆
	if(!g_arGuildData[guildnum]) { error_code = ERR_1; goto go_result; }

	EnterCriticalSection( &(m_pCom->m_critGuild) );

	pGuild = g_arGuildData[guildnum];

	pGuild->m_lSid = guildnum;

	strcpy(pGuild->m_strGuildName, strGuildName);
	strcpy(pGuild->m_strMasterName, m_strUserID);

	pGuild->m_dwGuildDN = 0;
	pGuild->m_sVersion = -1;

	pwMark = (WORD*)(&pGuild->m_strMark[0]);
	for (i=0; i<GUILD_MARK_SIZE/sizeof(WORD); i++) {
		pwMark[i] = (0x1f<<11 | 0x1f);	// Client俊辑 荤侩窍绰 捧疙祸栏肺 檬扁拳茄促.
	}

	if(!InsertGuild(pGuild))
	{
		LeaveCriticalSection( &(m_pCom->m_critGuild) );
		return;
	}

	pGuild->AddUser( m_strUserID, guildnum );

	LeaveCriticalSection( &(m_pCom->m_critGuild) );

	if( m_dwDN <= GUILD_MAKE_DN ) m_dwDN = 0;
	else m_dwDN -= GUILD_MAKE_DN;		

	m_bGuildMaster = TRUE;
	m_dwGuild = guildnum;

	nLength = 0;
	nLength = strlen( strGuildName );
	if(nLength > 0 && nLength < CHAR_NAME_LENGTH)
	{
		strncpy(m_strGuildName, strGuildName, nLength);
	}

	bRes = FALSE;

go_result:
	//////////////////////////////PACKET///////////////////////////////////////
	TempBuf.Add(GUILD_OPEN_RESULT);

	if(bRes)
	{
		TempBuf.Add((BYTE)0x00);		// 角菩
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add((BYTE)0x01);		// 己傍
	TempBuf.Add((int)iGuildId);
	TempBuf.AddString(m_strGuildName);

	Send(TempBuf, TempBuf.GetLength());

	SendMoneyChanged();
	SendMyGuildInfo();//(TO_INSIGHT, INFO_MODIFY);
}

void USER::BridgeServerGuildDisperseResult(int guildnum)
{
	int i;//, j;
	int index = 0, nLen = 0;
	BYTE error_code = 0;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	CStore *pStore = NULL;
	CGuildFortress *pFort = NULL;

	if( guildnum <= 0 || guildnum >= g_arGuildData.GetSize() ) return;

	if( guildnum != (int)m_dwGuild ) return;

	pGuild = GetGuild( m_dwGuild );

	if( !DeleteGuildDB() ) 				// DB 昏力
	{
		ReleaseGuild();
		::InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, 0);
		return;
	}
										// 惑痢阑 家蜡 沁促搁 檬扁拳
/*	for(i = 0; i < g_arStore.GetSize(); i++)
	{
		pStore = g_arStore[i];
		if(!pStore) continue;

		if(pStore->m_iGuildSid == m_dwGuild)
		{
			::ZeroMemory(pStore->m_strGuildName, CHAR_NAME_LENGTH + 1);
			::ZeroMemory(pStore->m_strMasterName, CHAR_NAME_LENGTH + 1);

			pStore->InitStoreInfo(-1);
			SetGuildStoreTex(pStore->m_sStoreID, 0);	// Tax Rate Init
			InitMemStore(pStore);
			break;
		}
	}
*/
	index = m_dwGuild;

	for(i = 0; i < g_arGuildHouse.GetSize(); i++)
	{
		if(m_dwGuild == g_arGuildHouse[i]->iGuild)
		{
			CNpc *pNpc = NULL;				// 巩菩档...
			pNpc = GetNpc(g_arGuildHouse[i]->iMarkNpc);
			if(pNpc)
			{
				pNpc->m_sPid = 0;
				pNpc->m_sMaxHP = 1;
				::ZeroMemory(pNpc->m_strName, sizeof(pNpc->m_strName));
			}

			g_arGuildHouse[i]->iGuild = 0;
			break;
		}
	}

	// 夸货啊 乐促搁 捞侩阂啊肺 父甸绢 霖促.
	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i]) continue;

		pFort = g_arGuildFortress[i];

		if(pFort->m_iGuildSid == m_dwGuild)
		{
			if(pFort->m_lViolenceUsed == 1)
			{
				pFort->m_wMopPartyTime.wYear = 2030;		// 辨靛啊 秦魂登搁辑 唱赣瘤绰 葛滴 檬扁拳甫
				pFort->SetNpcToFortressViolenceEnd(m_pCom);
				pFort->GetOutof(m_pCom);
			}

			pFort->SetInitFortress();
			pFort->InitMemFortress(GUILD_WAR_DECISION);
			break;
		}
	}

	g_arGuildData[m_dwGuild]->InitGuild();

	ReleaseGuild();

	for(i = 0; i < MAX_USER; i++)		// 辨靛盔甸 悸泼
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		if(pUser->m_dwGuild <= 0) continue;

		if(pUser->m_dwGuild == index)
		{
			pUser->SendSystemMsg( IDS_USER_DISMISS_COMPLETED, SYSTEM_NORMAL, TO_ME);

			pUser->m_dwGuild = -1;							// 檬扁拳茄促.
			pUser->m_sGuildVersion = -1;					// 辨靛 巩剧 滚怜
			::ZeroMemory(pUser->m_strGuildName, sizeof(pUser->m_strGuildName));
			pUser->m_bGuildMaster = FALSE;					// 辨靛 付胶磐
			pUser->m_bRepresentationGuild = FALSE;			// 鼻茄 措青 咯何

			pUser->m_bFieldWarApply = FALSE;
			pUser->m_tGuildWar = GUILD_WAR_AFFTER;
			pUser->m_tGuildHouseWar = GUILD_WAR_AFFTER;
			pUser->m_tFortressWar = GUILD_WAR_AFFTER;

			pUser->SendMyGuildInfo();
		}
	}
	// DB俊辑 昏力..
	// 立加茄 蜡历俊辑 昏力...
	// 快急 皋葛府俊辑 昏力... (Guild, Guild_House_Rank,

	UpdateUserData();
}

void USER::BridgeServerGuildInviteResult(int guid, char *strGuestID)
{
	int index = 0;
	BYTE error_code = 0;

	int nLen = 0;

	int uid = 0;
	int iCount = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;

	BOOL bRes = TRUE;

	uid = guid;

	pUser = GetUser( uid );

	if(!pUser) return;

	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0) return;

	if(pUser->m_dwGuild > 0) return;

	if(CheckInGuildWarring()) return;								// 辨傈吝俊绰 倾遏罙荐绝促.

	pGuild = GetGuild( m_dwGuild );

	if(!pGuild)
	{
		ReleaseGuild();				// 秦力...
		return;
	}
									// 坷幅...
//	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0)
	if(!pGuild->IsMasterPower(m_strUserID))
	{
		ReleaseGuild();				// 秦力...
		return;
	}

	index = -1;
	index = pGuild->GetUser( pUser->m_strUserID );
	if(index >= 0)
	{
		ReleaseGuild();				// 秦力...

		pUser->m_dwGuild = m_dwGuild;			// 辨靛 锅龋甫 促矫 悸泼茄促.
		SendGuildInfo(pUser);

		error_code = ERR_9;
		goto go_result;
	}// 捞固 促弗 辨靛俊 啊涝茄 蜡历

	if(!pGuild->AddUser(pUser->m_strUserID, pGuild->m_lSid))
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_11;			//
		goto go_result;
	}

	if(!InsertGuildUser(pGuild->m_lSid, pUser->m_strUserID)) // 秦寸 辨靛俊 啊涝阑 悸泼
	{
		pGuild->RemoveUser(pUser->m_strUserID);

		ReleaseGuild();				// 秦力...
		error_code = 255;			//
		goto go_result;
	}

	pUser->m_dwGuild = pGuild->m_lSid;
	pUser->m_bGuildMaster = FALSE;
	pUser->m_sGuildVersion = pGuild->m_sVersion;
	pUser->m_bRepresentationGuild = FALSE;
	strcpy(pUser->m_strGuildName, pGuild->m_strGuildName);
	bRes = FALSE;

go_result:
	if(bRes)
	{
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add((BYTE)0x00);		//角菩
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	ReleaseGuild();

	UpdateUserData();

	AddGuildUserInFortress(pUser);

	SendGuildInviteUser(pUser);			// 辨靛 府胶飘甫 林绊 罐扁
	SendGuildInfo(pUser);				// 矫具 裹困俊 辨靛俊 啊涝茄 荤恩捞 乐栏聪瘪 巩剧阑 焊捞扼绊 茄促...
}

void USER::BridgeServerGuildOffResult(char *strReqID, char *strOffID)
{
	int index = 0;
	BYTE error_code = 0;
	CString strTemp;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	BOOL bRes = TRUE;

	if(m_dwGuild <= 0) return;

	if(m_bGuildMaster)	// 夸没茄 荤恩捞 辨付老锭
	{
		if( !strcmp( strReqID, strOffID ) )	// 夸没茄 酒捞叼客 呕硼且 酒捞叼啊 鞍促搁... 俊矾促
		{
			return;
		}
	}

	if(m_tGuildHouseWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING || m_tFortressWar == GUILD_WARRING)
	{
		return;
	}

	if(CheckInGuildWarring()) return;								// 辨傈吝俊绰 倾遏罙荐绝促.

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)
	{
		ReleaseGuild();				// 秦力...
		return;
	}

	index = -1;
	index = pGuild->GetUser( strOffID );

	if(index < 0)					// 秦寸 辨靛俊辑 蜡历啊 绝栏搁...
	{
		ReleaseGuild();
		return;
	}

	if(!pGuild->RemoveUser( strOffID ))
	{
		ReleaseGuild();
		error_code = ERR_8;
		goto go_result;
	}

	if(!DeleteGuildUser(strOffID))		// 抛捞喉俊辑 瘤款促.
	{
		pGuild->AddUser(strOffID, m_dwGuild);

		ReleaseGuild();
		error_code = 255;
		goto go_result;
	}

	if(pGuild->m_lSubMaster == 1)
	{
		if( !strcmp(pGuild->m_strSubMasterName, strOffID) )
		{
			CString strMsg = _T("");
			strMsg.Format(IDS_USER_GUILD_SUBMASTER_OFF, pGuild->m_strSubMasterName);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

			pGuild->RemoveSubGuildMaster();
		}
	}

	ReleaseGuild();
	bRes = FALSE;

go_result:
	if(bRes)
	{
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	if( !strcmp( strReqID, strOffID ) )		
	{
		SendGuildOffUser( this );				// 促弗 辨靛盔俊霸 郴啊 呕硼茄 荤角阑 舅赴促.

		DelGuildUserInFortress( strOffID, m_dwGuild );

		m_dwGuild = -1;							// 檬扁拳茄促.
		m_sGuildVersion = -1;					// 辨靛 巩剧 滚怜
		::ZeroMemory(m_strGuildName, sizeof(m_strGuildName));
		//----------------------------------------------------
		//yskang 0.1 辨靛 呕硼矫 龋莫 昏力
		ZeroMemory(m_strLoveName, sizeof(m_strLoveName));
		ZeroMemory(m_pMD->m_strLoveName,sizeof(m_pMD->m_strLoveName));
		//----------------------------------------------------
		//龋莫阑 促弗 荤恩甸俊霸 舅赴促(鞍篮 胶农赴俊 乐绰荤恩甸俊 茄秦辑?)
		CBufferEx TempBuf;
		TempBuf.Add(LOVE_NAME);
		TempBuf.Add(m_uid+USER_BAND);
		TempBuf.AddString(m_strLoveName);
		SendExactScreen(TempBuf, TempBuf.GetLength());
		//--------------------------------------------------------------
		m_bGuildMaster = FALSE;					// 辨靛 付胶磐
		m_bRepresentationGuild = FALSE;			// 鼻茄 措青 咯何

		SendMyGuildInfo();						// 辨靛 呕硼甫 舅赴促.

		UpdateUserData();
	}
	else									// 碍力 眠规
	{
		pUser = GetUser( strOffID );

		if( pUser )
		{
			if( pUser->m_state == STATE_GAMESTARTED )
			{
				pUser->m_dwGuild = -1;
				pUser->m_sGuildVersion = -1;		// 辨靛 巩剧 滚怜
				::ZeroMemory(pUser->m_strGuildName, sizeof(pUser->m_strGuildName));
				//----------------------------------------------------
				//yskang 0.1 辨靛 呕硼矫 龋莫 昏力
				ZeroMemory(pUser->m_strLoveName, sizeof(pUser->m_strLoveName));
				ZeroMemory(pUser->m_pMD->m_strLoveName,sizeof(pUser->m_pMD->m_strLoveName));
				//龋莫阑 促弗 荤恩甸俊霸 舅赴促(鞍篮 胶农赴俊 乐绰荤恩甸俊 茄秦辑?)
				CBufferEx TempBuf;
				TempBuf.Add(LOVE_NAME);
				TempBuf.Add(pUser->m_uid + USER_BAND);
				TempBuf.AddString(pUser->m_strLoveName);
				pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
				//----------------------------------------------------
				pUser->m_bGuildMaster = FALSE;			// 辨靛 付胶磐
				pUser->m_bRepresentationGuild = FALSE;	// 鼻茄 措青 咯何
				pUser->SendMyGuildInfo();
				pUser->SendSystemMsg( IDS_USER_KICKOUT_GUILD, SYSTEM_NORMAL, TO_ME);

				SendGuildOffUser( pUser );

				pUser->UpdateUserData();
			}
		}

		strTemp.Format( IDS_USER_KICKOUT_MEMBER, strOffID);
		SendSystemMsg((LPTSTR)(LPCTSTR)strTemp, SYSTEM_NORMAL, TO_ME);
	}
}

void USER::BridgeDeputeGuildMasterOfPowerResult(int guid)
{
	int index = 0;
	BYTE error_code = 0;

	int nLen = 0;

	int uid = 0;
	int iCount = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;

	BOOL bRes = TRUE;

	if(!m_bGuildMaster) return; 	// 辨靛 鼻茄捞 绝促.

	uid = guid;

	pUser = GetUser( uid );

	if(!pUser || pUser->m_state != STATE_GAMESTARTED) return;	// 蜡历啊 绝促.

	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;

	if(pUser->m_dwGuild != m_dwGuild) return;

	if(CheckInGuildWarring()) return;							// 辨傈吝俊绰 倾遏罙荐绝促.

	CString strMsg = _T("");

	pGuild = GetGuild( m_dwGuild );

	if(!pGuild)
	{
		ReleaseGuild();				// 秦力...
		return;
	}
									// 坷幅...
	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0)
	{
		ReleaseGuild();				// 秦力...
		return;
	}

	index = -1;
	index = pGuild->GetUser(pUser->m_strUserID);
	
	if(index < 0)
	{	
		ReleaseGuild();					// 秦力...
		error_code = ERR_8;			// 
		goto go_result;		
	}

	if(pGuild->CheckGuildSubMaster())
	{
		if(UpdateGuildSubMaster(pUser->m_strUserID, TRUE) <= 0)
		{
			pGuild->RemoveSubGuildMaster();
			ReleaseGuild();					// 秦力...
			return;
		}

		pGuild->SetSubGuildMaster(pUser->m_strUserID);
		ReleaseGuild();					// 秦力...

		pUser->m_bRepresentationGuild = TRUE;
		bRes = FALSE;
	}
	else
	{
		strMsg = _T("");
		strMsg.Format(IDS_USER_GUILD_SUBMASTER, pGuild->m_strSubMasterName);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		ReleaseGuild();				// 秦力...
		return;
	}

go_result:
	if(bRes)
	{
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add((BYTE)0x00);		//角菩
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	USER *pGUser = NULL;			// 皋技瘤甫 舅赴促.

	CBufferEx TempBuf;

	strMsg = _T("");
	strMsg.Format( IDS_USER_GUILD_SUBMASTER_ON, pUser->m_strUserID);

	for(int i = 0; i < MAX_USER; i++)
	{
		pGUser = m_pCom->GetUserUid(i);

		if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;		

		if(m_dwGuild == pGUser->m_dwGuild)
		{
			pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
}

void USER::BridgeRemoveGuildMasterOfPowerResult()
{
	int index = 0;
	int nLen = 0;

	int uid = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;

	if(!m_bGuildMaster && !m_bRepresentationGuild) return; 	// 辨靛 鼻茄捞 绝促.

	TCHAR strName[CHAR_NAME_LENGTH + 1];
	::ZeroMemory(strName, sizeof(strName));

	BYTE error_code = 0;
	BOOL bRes = TRUE;

	if(CheckInGuildWarring()) return;// 辨傈吝俊绰 倾遏且荐绝促.

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) 
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_7;			// 秦寸 辨靛啊 绝促.
		goto go_result;				
	}
									// 坷幅...
//	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0) 
	if( !pGuild->IsMasterPower(m_strUserID) )
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_10;		// 
		goto go_result;
	}

	nLen = 0;
	nLen = strlen(pGuild->m_strSubMasterName);
	if(pGuild->m_lSubMaster == 0 || (nLen <= 0 || nLen > CHAR_NAME_LENGTH) )		// 泅犁 何辨付绝促绊 舅妨霖促.
	{
		SendSystemMsg( IDS_USER_GUILD_SUBMASTER_NOT, SYSTEM_ERROR, TO_ME);
		ReleaseGuild();				// 秦力...
		return;
	}

	if(UpdateGuildSubMaster(NULL, FALSE) <= 0)
	{
		ReleaseGuild();				// 秦力...
		return;
	}
	
	pUser = GetUser(pGuild->m_strSubMasterName);
	if(pUser && pUser->m_state == STATE_GAMESTARTED)
	{
		pUser->m_bRepresentationGuild = FALSE;
	}

	::CopyMemory(strName, pGuild->m_strSubMasterName, nLen);
	pGuild->RemoveSubGuildMaster();

	ReleaseGuild();			

	bRes = FALSE;

go_result:
	if(bRes)
	{
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add((BYTE)0x00);		//角菩
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	USER *pGUser = NULL;			// 皋技瘤甫 舅赴促.

	CBufferEx TempBuf;

	CString strMsg = _T("");
	strMsg.Format( IDS_USER_GUILD_SUBMASTER_OFF, strName);

	for(int i = 0; i < MAX_USER; i++)
	{
		pGUser = m_pCom->GetUserUid(i);

		if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;		

		if(m_dwGuild == pGUser->m_dwGuild)
		{
			pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}		
}

void USER::BridgeServerGuildMarkResult(TCHAR *pGuildMark)
{
	CBufferEx TempBuf;

	BOOL bRes = TRUE;
	BYTE error_code = 0;

	CGuild *pGuild = NULL;

	if(!m_bGuildMaster) return;

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)
	{
		ReleaseGuild();
		return;
	}

	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0)
	{
		ReleaseGuild();
		return;
	}

	if(pGuild->m_sVersion >= 0) CheckMaxValue((short &)pGuild->m_sVersion, (short)1);
	else pGuild->m_sVersion += 1;

	if(!UpdateGuildMark(pGuildMark, pGuild))	// DB俊 静扁 角菩搁 唱皑..
	{
		ReleaseGuild();
		return;
	}

	m_sGuildVersion = pGuild->m_sVersion;	// 辨靛 巩剧 滚怜

	::CopyMemory(pGuild->m_strMark, pGuildMark, GUILD_MARK_SIZE);

	bRes = FALSE;

	ReleaseGuild();				// 秦力...

	TempBuf.Add(GUILD_MARK_ADD_RESULT);

	TempBuf.Add((BYTE)0x01);				// 货肺款 巩剧捞 殿废登菌栏搁 瘤陛 殿废茄 辨靛 蜡历 葛滴俊霸 焊郴具 窍唱
	TempBuf.Add(pGuild->m_sVersion);		// 酒丛 滚怜父 啊福媚拎辑 撇府搁 拌加 焊郴具 窍唱...
											// 瘤陛篮 滚怜父 焊郴辑...
	Send(TempBuf, TempBuf.GetLength());

	SendMyGuildInfo();
											// 秦寸 辨靛窍快胶 巩剧阑 函版茄促.
	for(int i = 0; i < g_arGuildHouse.GetSize(); i++)
	{
		if(pGuild->m_lSid == g_arGuildHouse[i]->iGuild)
		{
			int modify_index = 0;
			char modify_send[2048];

			CNpc *pNpc = NULL;
			pNpc = GetNpc(g_arGuildHouse[i]->iMarkNpc);
			if(!pNpc) return;

			modify_index = 0;
			pNpc->m_sMaxHP = m_sGuildVersion;
			::ZeroMemory(modify_send, sizeof(modify_send));
			pNpc->FillNpcInfo(modify_send, modify_index, INFO_MODIFY);
			pNpc->SendInsight(m_pCom, modify_send, modify_index);

			break;
		}
	}
}

void USER::BridgeServerUserRestartResult()
{
	if(m_bLogOut == TRUE) return;
	if(m_state != STATE_GAMESTARTED) return;	// 捞寇 STATE_CONNECTED殿 老锭绰 秦寸 檬扁拳啊 皋葛府俊绝栏骨肺 力寇秦具茄促.

	m_bLogOut = TRUE;

	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)
	{									// 鞘靛傈 吝捞搁 舅妨霖促.
		if(m_bGuildMaster)
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// 亲汗
		}
	}

	if(m_bNowBuddy == TRUE)				// 滚叼吝捞搁 烹焊茄促.
	{
		for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(strcmp(m_MyBuddy[i].m_strUserID, m_strUserID) == 0) SendBuddyUserLeave(i);
		}
	}

	if(m_bNowTrading == TRUE)
	{
		BYTE result = 0x00;
		USER *pTradeUser = NULL;
		if(m_iTradeUid != -1)	pTradeUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTradeUser != NULL)	pTradeUser->SendExchangeFail(result, (BYTE)0x05);
	}

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // 促弗 蜡历甸篮 够窍唱 眉农..

	// 肺锯烦喉 贸府
	ExitRoyalRumble();

	if(m_bLive == USER_DEAD) m_sHP = m_sMaxHP;

	if(!UpdateMemBankDataOnly()) return;
	if(!UpdateMemAccountBankDataOnly()) return;
	if(!UpdateUserData(TRUE)) return;
	ReInitMemoryDB();

	// alisia
//	if(SendRestartLoginResult() == FALSE) return;
	int		index = 0;
	m_state = STATE_LOGOUT;

	index = 0;
	SetByte(m_TempBuf, RESTART_RESULT, index );
	SetByte(m_TempBuf, SUCCESS, index );
	Send(m_TempBuf, index);
	// alisia


	m_nHavePsiNum = 0;
	m_tIsOP = 0;
	MAP *pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) goto go_result;

	pMap = g_zone[m_ZoneIndex];
	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) goto go_result;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) goto go_result;

	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == USER_BAND + m_uid ) // 甘俊辑 荤扼柳促.
		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);

go_result:
	::ZeroMemory(m_strUserID, sizeof(m_strUserID));

	SendMyInfo(TO_INSIGHT, INFO_DELETE);

//	m_state = STATE_GAMERESTART;		// 立加 惑怕甫 官槽促.
}

void USER::BridgeServerUserZoneLogOutResult(int z, int x, int y)
{
	CPoint pt = ConvertToServerByZone( z, x, y );

	if( pt.x < 0 || pt.y < 0 ) return;

	int save_z = z;
	int save_x = pt.x;
	int save_y = pt.y;

	// 单捞鸥 历厘...
	TRACE("Zone Change Data Saved...\n");
	ZoneLogOut(save_z, save_x, save_y);
	//

	m_curz = z;
	m_curx = save_x;
	m_cury = save_y;

	CBufferEx TempBuf;
	BYTE result = FAIL;
	
	TempBuf.Add(ZONE_CHANGE_RESULT);

	result = SUCCESS_ZONE_CHANGE;
	TempBuf.Add(result);
	TempBuf.AddString((char *)LPCTSTR(m_strZoneIP));
	TempBuf.Add(m_nZonePort);

	Send(TempBuf, TempBuf.GetLength());
	return;
}

// 泅犁 肺锯 烦喉俊 涝厘 啊瓷茄 矫埃牢瘤 八荤茄促.
BOOL USER::CheckRoyalRumbleEnterTime()
{
	return g_RR.CheckEnteringByTime();
}

BOOL USER::CheckRoyalRumbleEnterMaxUser()
{
	return g_RR.CheckEnteringByMaxUser();
}

void USER::ExitRoyalRumble()
{
	if( m_curz != g_RR.m_iZoneNum ) return;

	if(CheckInvalidMapType() != 12) return;		// 肺凯烦喉俊辑 措访厘 加己俊 辑乐瘤 臼栏搁 惑包绝促.

	g_RR.Exit( this );
}


int USER::GetGuildMapIndex(int maptype)
{
	if(maptype < 0) return -1;

	for(int i = 0; i < g_arMapTable.GetSize(); i++)
	{
		if(g_arMapTable[i])
		{
			if( maptype == g_arMapTable[i]->m_sMapIndex ) return i;
		}
	}

	return -1;
}

void USER::GuildUserCallReq(TCHAR *pBuf)//yskang 0.2 器飘府胶 辨付啊 辨盔 家券
{
	BOOL bOwnerFortress = FALSE;//器飘府胶甫 家蜡窍绊 乐绰 辨靛牢啊 犬牢窍绰 函荐
	BYTE result = FAIL;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;
	CPoint pt, ptNew;

	if(!m_bGuildMaster)//辨付啊 酒聪搁 府畔秦滚覆
		return;

	for(int i = 0; i < g_arGuildFortress.GetSize(); i++)// 刚历 Fortress甫 家蜡茄 辨靛牢瘤 眉农茄促.
	{
		if(!g_arGuildFortress[i]) continue;

		if(CheckGuildHouseUser(g_arGuildFortress[i]->m_sFortressID)) 
		{
			bOwnerFortress = TRUE;//家蜡窍绊 乐促.
			break;
		}
	}
	if(!bOwnerFortress) //器飘府胶甫 家蜡窍绊 乐瘤 臼促搁 府畔
		return;

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength == 0 || nLength > CHAR_NAME_LENGTH) return;		// 肋给等 蜡历酒捞叼 
	
	pUser = GetUser(szUserName);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return; 
	
	if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳
	
//	pt = FindNearAvailablePoint_S(m_curx, m_cury);
//	if(pt.x <= -1 && pt.y <= -1) return;						// 辨付 林困肺 棵 谅钎啊 绝澜

	if(pUser->m_curz != m_curz)									// 蜡历客 辨付啊 鞍篮粮俊 乐瘤 臼阑 版快
	{
		if( !IsZoneInThisServer(pUser->m_curz) ) return;		// 郴 辑滚救俊 乐绰粮捞 酒丛 府畔

		ptNew = ConvertToClient(m_curx, m_cury);
		BOOL bSuccess = pUser->ZoneChangeProcess(m_curz, ptNew.x, ptNew.y);	//^^ Check 夸噶
		
		if(bSuccess)
		{
//			pUser->SendZoneChange(bSuccess);
			pUser->SendWeatherInMoveZone();				// 捞悼 粮狼 朝揪函拳甫 舅赴促.
		}
		return;
	}
	else
	{													// 鞍篮 粮捞搁 捞悼栏肺...		
/*
		ptNew = ConvertToClient(pt.x, pt.y);
		if(ptNew.x == -1 || ptNew.y == -1) return;

		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_lUser, 0);
		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + pUser->m_uid);
		pUser->m_curx = pt.x;
		pUser->m_cury = pt.y;
		
		result = SUCCESS;
*/
		pt = pUser->FindNearAvailablePoint_S( m_curx, m_cury );
		ptNew = ConvertToClient( pt.x, pt.y );

		if( ptNew.x == -1 || ptNew.y == -1 ) return;

		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_lUser, 0);
		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + pUser->m_uid);
		pUser->m_curx = pt.x;
		pUser->m_cury = pt.y;
		
		result = SUCCESS;
	}
	
	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, pUser->m_uid + USER_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	pUser->Send(m_TempBuf, index);	// 蜡历俊霸绰 货肺款 谅钎蔼阑...
	
	pUser->SendInsight(m_TempBuf, index);
	
	pUser->SightRecalc();
}



int USER::GetIntAddr(CString strPeerIp)//yskang 0.04 string屈 酒捞乔甫 int屈 酒捞乔肺...
{
	int nRetAddr = 0;
	CString strTemp;
	BYTE *pByte = (BYTE *)&nRetAddr;
	int i = 0;
	while (i < strPeerIp.GetLength ())
	{
		if (strPeerIp[i] == '.')
		{
			strTemp = strPeerIp.Left (i);
			strPeerIp = strPeerIp.Right (strPeerIp.GetLength () - (i + 1));
			*pByte = (BYTE)atoi (strTemp);
			pByte++;
			i = 0;
			if (strPeerIp.Find ('.') == -1)
			{
				*pByte = (BYTE)atoi (strPeerIp);
				break;
			}
			continue;
		}
		i++;
	}	
	return nRetAddr;
}


TCHAR *USER::GetStrAddr(int nPeerIp, TCHAR *strPeerIp)
{
	//TCHAR strPeerIp[1024];
	ZeroMemory(strPeerIp,sizeof(strPeerIp));
	char buffer[20];
	for(int i=0; i<4; i++)
	{
		BYTE a = nPeerIp&0x000000ff;
		_itoa(a,buffer,10);
		strcat(strPeerIp,(const char*)buffer);
		if(i!=3)
			strcat(strPeerIp,".");
		nPeerIp = nPeerIp>>8;
	}
	return (TCHAR *)strPeerIp;
}
///////////////////////////////////////////////////////////////////
//卸下机甲
//
void USER::offJJ()
{
	if (m_UserItem[39].sSid == -1)
	{
	//	SendSystemMsg("你没有装备机甲，无须卸下!", SYSTEM_ERROR, TO_ME);
		SendEventMsg("你没有机甲可卸!");
		return;
	}
	int solt = GetEmptySlot( INVENTORY_SLOT );
	if(solt == -1)
	{
		SendEventMsg("您的包裹已满,请清空包裹栏");
		return;
	}
	int j = 0;
	ItemList	TempItem;
	m_UserItem[39].sCount = 1;
	TempItem = m_UserItem[solt];	
	m_UserItem[solt] = m_UserItem[39];
	m_UserItem[39] = TempItem;

	CBufferEx TempBuf;
	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(0);
	TempBuf.Add(2);	
	TempBuf.Add(39);
	TempBuf.Add(m_UserItem[39].sLevel);
	TempBuf.Add(m_UserItem[39].sSid);
	TempBuf.Add(m_UserItem[39].sDuration);
	TempBuf.Add(m_UserItem[39].sBullNum);
	TempBuf.Add(m_UserItem[39].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[39].tMagic[j]);
	TempBuf.Add(m_UserItem[39].tIQ); 

	TempBuf.Add(solt);
	TempBuf.Add(m_UserItem[solt].sLevel);
	TempBuf.Add(m_UserItem[solt].sSid);
	TempBuf.Add(m_UserItem[solt].sDuration);
	TempBuf.Add(m_UserItem[solt].sBullNum);
	TempBuf.Add(m_UserItem[solt].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[solt].tMagic[j]);
	TempBuf.Add(m_UserItem[solt].tIQ); 

	Send(TempBuf, TempBuf.GetLength());
	SendCharData();
	if(m_UserItem[39].sSid == -1 && (m_iSkin == 5 || m_iSkin == 8 || m_iSkin == 170))
	{
		m_iSkin=0 , m_iHair=0;
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
	}
	CheckMagicItemMove();
	//SendSystemMsg("成功把机甲卸下!", SYSTEM_ERROR, TO_ME);
	SendEventMsg("成功把机甲卸下!");
}

void USER::TSFengHao(USER* pUser)
{
	if(m_tIsOP != 1) return;
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	
	pUser->Closeuser(600);
	pUser->SoftClose();
    pUser->LogOut();
}
bool USER::QuJiaoXiMiMa(char *strAccount)
 {
		SQLHSTMT		hstmt;
		SQLRETURN		retcode;
		TCHAR			szSQL[1024];

		BOOL bQuerySuccess = TRUE;

		memset(szSQL, 0x00, 1024);
		_sntprintf(szSQL, sizeof(szSQL), TEXT("SELECT strUserID, strSocNo FROM NGSCUSER	WHERE struserid='%s'"), strAccount);

		hstmt = NULL;

		int db_index = -1;
			CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
			if( !pDB ) return FALSE;

			retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
			if (retcode!=SQL_SUCCESS)
			{
				//		g_DBSession[m_iModSid].ReleaseDB(db_index);
				return FALSE;
			}
			int i;
			SQLINTEGER ivd;
			char szChar01[40]="",szChar02[40]="";
			retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
			if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				while (TRUE)
				{
					retcode = SQLFetch(hstmt);
					if (retcode !=SQL_SUCCESS && retcode !=SQL_SUCCESS_WITH_INFO)
					{
						break;
					}
					i = 1;
					JiaoXiMiMa[0] = 0;
					SQLGetData( hstmt, i++, SQL_C_CHAR, szChar01, sizeof(szChar01), &ivd );
					SQLGetData( hstmt, i++, SQL_C_CHAR, szChar02, sizeof(szChar02), &ivd );
					if(strlen(szChar02)>0)
					{
						strcpy(JiaoXiMiMa,szChar02);
					}
				}
			}else		
				if (retcode==SQL_ERROR)
				{
					bQuerySuccess = FALSE;
					DisplayErrorMsg(hstmt);
				}
				else if (retcode==SQL_NO_DATA)
				{
					bQuerySuccess = FALSE;
				}

				if (hstmt!=NULL) 
					SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

				if( !bQuerySuccess ) 
				{
					//		BREAKPOINT();
					g_DBSession[m_iModSid].ReleaseDB(db_index);
					return FALSE;
				}

		g_DBSession[m_iModSid].ReleaseDB(db_index);
	return TRUE;
 }
///.//////////////////////////////////////////////////////////////////非法检测
void USER::CheckBadItem()
{
    if (o_yehuoini[0]->jiance == 1)
{
	CString str = _T("");
	//冰风科技 开始检测身上物品
	for(int i = 0; i < TOTAL_ITEM_NUM ; i++)
	{
		if (m_UserItem[i].sSid == 10000 && m_UserItem[i].sCount >= 200000000)
		{
			SYSTEMTIME st;
			CString strDate;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);

			str.Format("[ %s ]玩家名: [%s] 身上位置:[%d] 编号:[ %d ] 数量: [ %d 个]\r\n",strDate,m_strUserID,i,m_UserItem[i].sSid,m_UserItem[i].sCount);
			EnterCriticalSection( &m_CS_FileWrite );
			g_fpSpeedHack0.Write( str, str.GetLength() );
			LeaveCriticalSection( &m_CS_FileWrite);
		}
		if ((m_UserItem[i].sSid == 1437 || m_UserItem[i].sSid == 1438 || m_UserItem[i].sSid == 845 || m_UserItem[i].sSid == 846) && m_UserItem[i].sCount >= 100)
		{
			SYSTEMTIME st;
			CString strDate;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);

			str.Format("[ %s ]玩家名: [%s] 身上位置:[%d] 编号:[ %d ] 数量: [ %d 个]\r\n",strDate,m_strUserID,i,m_UserItem[i].sSid,m_UserItem[i].sCount);
			EnterCriticalSection( &m_CS_FileWrite );
			g_fpSpeedHack0.Write( str, str.GetLength() );
			LeaveCriticalSection( &m_CS_FileWrite);
		}
	}
	//冰风科技开始检测个人银行
	for(int p = 0; p < TOTAL_BANK_ITEM_NUM; p++)
	{		
		if (m_UserBankItem[p].sSid == 10000 && m_UserBankItem[p].sCount >= 200000000)
		{
			SYSTEMTIME st;
			CString strDate;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);

			str.Format("[ %s ]玩家名: [%s] 个人银行位置:[%d] 编号:[ %d ] 数量: [ %d 个]\r\n",strDate,m_strUserID,p,m_UserBankItem[p].sSid,m_UserBankItem[p].sCount);
			EnterCriticalSection( &m_CS_FileWrite );
			g_fpSpeedHack0.Write( str, str.GetLength() );
			LeaveCriticalSection( &m_CS_FileWrite);
		}
		if ((m_UserBankItem[p].sSid == 1437 || m_UserBankItem[p].sSid == 1438 || m_UserBankItem[p].sSid == 845 || m_UserBankItem[p].sSid == 846) && m_UserBankItem[p].sCount >= 100)
		{
			SYSTEMTIME st;
			CString strDate;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);

			str.Format("[ %s ]玩家名: [%s] 个人银行位置:[%d] 编号:[ %d ] 数量: [ %d 个]\r\n",strDate,m_strUserID,p,m_UserBankItem[p].sSid,m_UserBankItem[p].sCount);
			EnterCriticalSection( &m_CS_FileWrite );
			g_fpSpeedHack0.Write( str, str.GetLength() );
			LeaveCriticalSection( &m_CS_FileWrite);
		}	
	}
	//冰风科技开始检测综合银行
	for(int k = 0; k < TOTAL_ACCOUNT_BANK_ITEM_NUM; k++)
	{
		if (m_AccountBankItem[k].sSid == 10000 && m_AccountBankItem[k].sCount >= 200000000)
		{
			SYSTEMTIME st;
			CString strDate;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);

			str.Format("[ %s ]玩家名: [%s] 综合银行位置:[%d] 编号:[ %d ] 数量: [ %d 个]\r\n",strDate,m_strUserID,k,m_AccountBankItem[k].sSid,m_AccountBankItem[k].sCount);
			EnterCriticalSection( &m_CS_FileWrite );
			g_fpSpeedHack0.Write( str, str.GetLength() );
			LeaveCriticalSection( &m_CS_FileWrite);
		}
		if ((m_AccountBankItem[k].sSid == 1437 || m_AccountBankItem[k].sSid == 1438 || m_AccountBankItem[k].sSid == 845 || m_AccountBankItem[k].sSid == 846) && m_AccountBankItem[k].sCount >= 100)
		{
			SYSTEMTIME st;
			CString strDate;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);

			str.Format("[ %s ]玩家名: [%s] 综合银行位置:[%d] 编号:[ %d ] 数量: [ %d 个]\r\n",strDate,m_strUserID,k,m_AccountBankItem[k].sSid,m_AccountBankItem[k].sCount);
			EnterCriticalSection( &m_CS_FileWrite );
			g_fpSpeedHack0.Write( str, str.GetLength() );
			LeaveCriticalSection( &m_CS_FileWrite);
		  }
	   }
    }
 }
