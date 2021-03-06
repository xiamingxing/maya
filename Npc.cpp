// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "USER.h"
#include "Npc.h"

#include "Extern.h"
#include "MAP.h"
#include "BufferEx.h"

#include "Mcommon.h"
#include "scdefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CRITICAL_SECTION m_CS_EventItemLogFileWrite;

//////////////////////////////////////////////////////////////////////
// Fortress Paket Variable
extern CRITICAL_SECTION m_CS_FortressData;
extern CPtrList				RecvFortressData;
extern long nFortressDataCount;
extern struct drop_info g_DropItem[256][4];

//int surround_x[8] = {-2, -1, 0, 1, 2, 1, 0, -1};
//int surround_y[8] = {0, -1, -2, -1, 0, 1, 2, 1};

int surround_x[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int surround_y[8] = {0, -1, -1, -1, 0, 1, 1, 1};

int g_iMoonEvent = 1;

//#define STEP_DELAY			//440
/*TCHAR *g_MagicArray[]={   //属性列表
	"",
	"力值1",
	"体质1",
	"敏捷1",
	"智慧1",
	"智力1",
	"损伤5",
	"命中2%",
	"命中3%",
	"回避2%",
	"回避3%",
	"防御1",
	"防御2",
	"抗魔5",
	"魔伤5",
	"力值2",
	"体质2",
	"敏捷2",
	"智慧2",
	"智力2",
	"损伤10",
	"防止中毒状态异常",
	"防止麻痹状态异常",
	"防止混乱状态异常",
	"防止视觉丧失状态异常",
	"防止火焰状态异常",
	"防止冰冻状态异常",
	"防止电击状态异常",
	"射程1",
	"攻击速度1阶段提高",
	"命中4%",
	"命中5%",
	"回避4%",
	"回避5%",
	"故障率1%",
	"防御3",
	"防御4",
	"耐久100",
	"生命值10",
	"精神值10",
	"体力值10",
	"魔法抗斥力10",
	"魔法损伤10",
	"力值3",
	"体质3",
	"敏捷3",
	"智慧3",
	"智力3",
	"反击技术等级1",
	"兴奋技术等级1",
	"钢铁皮肤技术等级1",
	"自我医疗技术等级1",
	"魔法精修技术等级1",
	"法术大师技术等级1",
	"精神强化技术等级1",
	"冥想技术等级1",
	"穿刺技术等级1",
	"闪避技术等级1",
	"精神集中技术等级1",
	"回复技术等级1",
	"狙击技术等级1",
	"连射技术等级1",
	"灵动技术等级1",
	"枪械大师技术等级1",
	"损伤15",
	"诱发异常中毒状态",
	"诱发异常麻痹状态",
	"诱发异常混乱状态",
	"诱发异常视角丧失状态",
	"诱发异常火焰状态",
	"诱发异常冷气状态",
	"诱发异常电气状态",
	"攻击速度2阶段提高",
	"命中率6%",
	"命中率7%",
	"回避率6%",
	"回避率7%",
	"故障率2%",
	"防御力5",
	"防御力6",
	"耐久性200",
	"生命值20",
	"精神值20",
	"体力值20",
	"魔法抗斥15",
	"魔法损伤15",
	"力值4",
	"体质4",
	"敏捷4",
	"智慧4",
	"智力4",
	"损伤20",
	"射程2",
	"攻击速度3阶段提高",
	"命中率8%",
	"命中率9%",
	"回避率8%",
	"回避率9%",
	"故障率3%",
	"防御力7",
	"防御力8",
	"耐久性300",
	"生命值30",
	"精神值30",
	"体力值30",
	"魔法抗斥20",
	"魔法损伤20",
	"力值5",
	"体质5",
	"敏捷5",
	"智慧5",
	"智力5",
	"反击技术等级2",
	"兴奋技术等级2",
	"钢铁皮肤技术等级2",
	"自我医疗技术等级2",
	"魔法精修技术等级2",
	"法术大师技术等级2",
	"精神强化技术等级2",
	"冥想技术等级2",
	"穿刺技术等级2",
	"闪避技术等级2",
	"集中技术等级2",
	"回复技术等级2",
	"狙击技术等级2",
	"连射技术等级2",
	"灵动技术等级2",
	"枪械大师技术等级2",
	"损伤25",
	"防御力9",
	"防御力10",
	"耐久性500",
	"生命值40",
	"精神值40",
	"体力值40",
	"魔法抗斥25",
	"魔法损伤25",
	"所有的技术等级1",
	"命中率10%",
	"回避率10%",
	"耐久性700",
	"生命值50",
	"精神值50",
	"体力值50",
	"防止所有状态异常",
	"所有的技术等级2",
	"损伤30",
	"命中率15%",
	"所有的能力值6",
	"所有的技术3",
	"恢复速度1阶段",
	"恢复速度2阶段",
	"恢复速度3阶段",
	"恢复速度4阶段",
	"回避率15%+",
	"体质10",
	"体质15",
	"敏捷10",
	"敏捷15",
	"生命100",
	"精神100",
	"力值10",
	"力值15",
	"智慧10",
	"智慧15",
	"智力10",
	"智力15",
	"魔法抗斥1",
	"魔法抗斥2",
	"魔法抗斥3",
	"魔法抗斥4",
	"魔法损伤1",
	"魔法损伤2",
	"魔法损伤3",
	"魔法损伤4",
	"损伤1",
	"损伤2",
	"损伤3",
	"损伤4",
	"命中率1%",
	"回避率1%",
	"魔法损伤5",
	"生命值1",
	"生命值2",
	"生命值3",
	"生命值4",
	"生命值5",
	"保存经验值",
	"自动防护",
	"无视穿刺概率",
	"魔法抵抗力50",
	"所有能力值1",
	"每10秒消耗魔法值50",
	"生命值150",
	"防御力50",
	"获得经验值30%",
	"物品暴率30%",
	"所有能力值100",
	"强化护法技能等级1",
	"生命不息技能等级1",
	"爆发技能等级1",
	"精准技能等级1",
	"强化护法技能等级2",
	"生命不息技能等级2",
	"爆发技能等级2",
	"精准技能等级2",
	0
};*/

#define UPDATE_EVENT_INVEN_TIME		12

//////////////////////////////////////////////////////////////////////
//	Inline Function
//
inline int CNpc::GetUid(int x, int y )
{
	MAP* pMap = g_zone[m_ZoneIndex];
	return pMap->m_pMap[x][y].m_lUser;
}

inline BOOL CNpc::SetUid(int x, int y, int id)
{
	MAP* pMap = g_zone[m_ZoneIndex];

	if(pMap->m_pMap[x][y].m_bMove != 0) return FALSE;
	if(pMap->m_pMap[x][y].m_lUser != 0 && pMap->m_pMap[x][y].m_lUser != id ) return FALSE;

	pMap->m_pMap[x][y].m_lUser = id;

	return TRUE;
}

BOOL CNpc::SetUidNPC(int x, int y, int id)
{
	MAP* pMap = g_zone[m_ZoneIndex];

	if(pMap->m_pMap[x][y].m_bMove != 0) return FALSE;
	if(pMap->m_pMap[x][y].m_lUser != 0 && pMap->m_pMap[x][y].m_lUser != id ) return FALSE;

	pMap->m_pMap[x][y].m_lUser = id;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpc::CNpc()
{
	m_NpcVirtualState = NPC_STANDING;
	m_NpcState = NPC_LIVE;

	InitTarget();

	m_ItemUserLevel = 0;
//	m_Delay = 0;			// 促澜 惑怕肺 傈捞登扁 鳖瘤狼 矫埃( mili/sec )
//	m_dwLastThreadTime = 0;
	m_Delay = 0;
	m_dwLastThreadTime = GetTickCount();

	m_sClientSpeed = 0;		// 努扼捞攫飘 局聪甫 困茄 捞悼 厚啦
	m_dwStepDelay = 0;		// 促澜 浦哦鳖瘤狼 矫埃 掉饭捞
	m_tNpcAttType = 0;		// 傍拜 己氢
	m_tNpcLongType = 0;		// 盔芭府(1), 辟芭府(0)
	m_tNpcGroupType = 0;	// 档框阑 林绰衬(1), 救林绰衬?(0)
//	m_tNpcTraceType = 0;	// 场鳖瘤 蝶扼埃促(1), 矫具俊辑 绝绢瘤搁 弊父(0)

	m_pPath = NULL;
	m_pOrgMap = NULL;

//	m_pMap = NULL;
	m_lMapUsed = 0;			// 甘 皋葛府焊龋

	m_bFirstLive = TRUE;

	m_tWeaponClass = BRAWL;
	m_dwDelayCriticalDamage = 0;
	m_dwLastAbnormalTime	= GetTickCount();

	::ZeroMemory(m_pMap, sizeof(m_pMap));// 老瞒盔 甘栏肺 檬扁拳茄促.

	m_tAbnormalKind = 0;
	m_dwAbnormalTime = 0;

	m_presx = -1;
	m_presy = -1;

	m_lEventNpc = 0;

	m_pGuardStore = NULL;				// 辨靛傈阑困秦 秦寸 惑痢阑 啊瘤绊 柯促.
	m_pGuardFortress = NULL;

	m_tRepairDamaged = 0;
	m_tNCircle = NPC_NCIRCLE_DEF_STATE;
	m_lFortressState = 0;
	m_lDamage = 0;

	m_bSummon = FALSE;
	m_sSummonOrgZ = m_sOrgZ;
	m_sSummonOrgX = m_sOrgX;
	m_sSummonOrgY = m_sOrgY;
	m_SummonZoneIndex = m_ZoneIndex;

	m_bSummonDead = FALSE;
	m_lNowSummoning = 0;

	m_lKillUid = -1;
	m_sQuestSay = 0;

	InitSkill();
	InitUserList();
}

CNpc::~CNpc()
{
	ClearPathFindData();

	InitUserList();
}

//////////////////////////////////////////////////////////////////////
//	NPC 胶懦沥焊甫 檬扁拳 茄促.
//
void CNpc::InitSkill()
{
	for(int i = 0; i < SKILL_NUM; i++)
	{
		m_NpcSkill[i].sSid = 0;
		m_NpcSkill[i].tLevel = 0;
		m_NpcSkill[i].tOnOff = 0;
	}
}

///////////////////////////////////////////////////////////////////////
//	辨茫扁 单捞磐甫 瘤款促.
//
void CNpc::ClearPathFindData()
{
	::ZeroMemory(m_pMap, sizeof(m_pMap));	// 老瞒盔 甘阑 困秦
/*	int i;

	if(m_pMap)
	{
		int **tmp = m_pMap;
		
		m_pMap = NULL;

		for(i = 0; i < m_vMapSize.cx; i++)
		{
			delete[] tmp[i];
		}
		delete[] tmp;
	}
*/
}

///////////////////////////////////////////////////////////////////////
// NPC 啊 贸澜 积扁芭唱 磷菌促啊 混酒朝 锭狼 贸府
//
BOOL CNpc::SetLive(COM* pCom)
{
	NpcTrace(_T("SetLive()"));

	if(m_tRepairDamaged > 0) return FALSE;		 // 傍己傈吝 颊惑 罐疽促搁 荐府瞪锭鳖瘤 弊措肺 蜡瘤登绢具窃...
	if(m_pGuardFortress && m_tGuildWar == GUILD_WARRING)
	{
		if(!m_bFirstLive) return FALSE;
	}
	else m_tGuildWar = GUILD_WAR_AFFTER;		// 葛电 各阑 辨靛傈俊 措秦辑 檬扁拳茄促...

	if(m_bSummonDead)							// 家券夌菌带 各俊 措秦辑
	{
		m_ZoneIndex = m_TableZoneIndex;
		m_sCurZ		= m_sOrgZ	= m_sTableOrgZ;
		m_sOrgX		= m_sTableOrgX;
		m_sOrgY		= m_sTableOrgY;
		
		m_pOrgMap = g_zone[m_ZoneIndex]->m_pMap;	// MapInfo 沥焊 悸泼
		
		m_bSummonDead = FALSE;
	}

	// NPC狼 HP, PP 檬扁拳 ----------------------//	
	int i = 0, j = 0;
	m_sHP = m_sMaxHP;
	m_sPP = m_sMaxPP;
	NpcDrop=4;
	int iTryLiveCount = 0;

	InitTarget();

	InitUserList();					// 鸥百阑困茄 府胶飘甫 檬扁拳.	

	// NPC 檬扁困摹 搬沥 ------------------------//
	MAP* pMap = g_zone[m_ZoneIndex];
	
	m_nInitMinX = m_sOrgX - m_sMinX;		if(m_nInitMinX < 1) m_nInitMinX = 1; 
	m_nInitMinY = m_sOrgY - m_sMinY;		if(m_nInitMinY < 1) m_nInitMinY = 1; 
	m_nInitMaxX = m_sOrgX + m_sMaxX;		if(m_nInitMaxX >= pMap->m_sizeMap.cx) m_nInitMaxX = pMap->m_sizeMap.cx - 1;
	m_nInitMaxY = m_sOrgY + m_sMaxY;		if(m_nInitMaxY >= pMap->m_sizeMap.cy) m_nInitMaxY = pMap->m_sizeMap.cy - 1;

	CPoint pt;
	CPoint ptTemp;

	int modify_index = 0;
	char modify_send[2048];	

//	if(m_lEventNpc == 0 && m_sEvent == 1000) return TRUE;//@@@@@@@@@@@@@@@@@@@@@@ Test Code(烙矫肺 各阑 家券窍扁困秦)

	if(m_lEventNpc == 1 && !m_bFirstLive) 
	{
		// 蜡历俊霸 NPC 沥焊傈价... 
		// 捞蜡 : 磷篮 葛嚼阑 焊咯拎具窍扁锭巩俊 促澜 府哩瞪 瞒肥啊 棵锭 INFO_DELETE甫 焊辰促. 
		::ZeroMemory(modify_send, sizeof(modify_send));

		for(int i = 0; i < NPC_NUM; i++)
		{
			if(g_arEventNpcThread[0]->m_ThreadInfo.pNpc[i] != NULL)
			{
				if(g_arEventNpcThread[0]->m_ThreadInfo.pNpc[i]->m_sNid == m_sNid)
				{
					FillNpcInfo(modify_send, modify_index, INFO_DELETE);
					SendInsight(pCom, modify_send, modify_index);

					g_arEventNpcThread[0]->m_ThreadInfo.pNpc[i] = NULL;
					InterlockedExchange(&m_lEventNpc, (LONG)0);
					InterlockedExchange(&g_arEventNpcThread[0]->m_ThreadInfo.m_lNpcUsed[i], (LONG)0);								
					return TRUE;
				}
			}
		}
		return TRUE;
	}

	if(m_tNpcType != NPCTYPE_MONSTER && m_bFirstLive)//NPCTYPE_DOOR || m_tNpcType == NPCTYPE_GUARD)
	{
		m_nInitX = m_sCurX = m_sOrgX;
		m_nInitY = m_sCurY = m_sOrgY;

		pMap->m_pMap[m_sCurX][m_sCurY].m_lUser = m_sNid + NPC_BAND;

//		TRACE("NPC DOOR %s(nid = %d) - %d %d\n", m_strName, m_sNid, m_sCurX, m_sCurY);
		CPoint temp = ConvertToClient(m_sCurX, m_sCurY);
		TRACE("NPC DOOR %s(nid = %d) - %d %d\n", m_strName, m_sNid, temp.x, temp.y);
	}
	else
	{
		while(1)
		{
			i++;
																						
			if(m_lEventNpc == 1)			// 家券各老版快 登档废捞搁 贸澜 瘤沥茄 谅钎肺
			{
				if(pMap->m_pMap[m_sOrgX][m_sOrgY].m_bMove == 0)	
				{ 
					pt.x = m_sOrgX; 
					pt.y = m_sOrgY; 
					
					m_nInitX = m_sCurX = pt.x;
					m_nInitY = m_sCurY = pt.y;
					
					//ptTemp = ConvertToClient(m_sCurX, m_sCurY);
					break;
				}
				else
				{
					pt = FindNearRandomPoint(m_sOrgX, m_sOrgY);
					if(pt.x <= 0 || pt.y <= 0) 
					{
						pt.x = myrand(m_nInitMinX, m_nInitMaxX);
						pt.y = myrand(m_nInitMinY, m_nInitMaxY);
					}
				}
			}
			else
			{
				pt.x = myrand(m_nInitMinX, m_nInitMaxX);
				pt.y = myrand(m_nInitMinY, m_nInitMaxY);

				// Test Code By Youn Gyu 02-08-13 (泅犁绰 林函 25伎肺父 力茄)
				if( m_sCurZ != 1 && m_sCurZ != 1005 ) // 老窜 带怜父 茫绰促.
				{
					if(m_tNpcType == NPCTYPE_MONSTER)
					{
						if( !CheckUserForNpc_Live(pt.x, pt.y) )
						{
							iTryLiveCount += 1;
							if(iTryLiveCount >= 20) return FALSE;
							else continue;
						}
					}
				}
				//TRACE("MONSTER %s(nid = %d) - %d %d\n", m_strName, m_sNid, m_sCurX, m_sCurY);
			}
			
			if(pt.x < 0 || pt.x >= pMap->m_sizeMap.cx) continue;
			if(pt.y < 0 || pt.y >= pMap->m_sizeMap.cy) continue;

			if(pMap->m_pMap[pt.x][pt.y].m_bMove != 0 || pMap->m_pMap[pt.x][pt.y].m_lUser != 0)
			{
				if(i >= 100) 
				{
					m_nInitX = m_sCurX = m_sOrgX;
					m_nInitY = m_sCurY = m_sOrgY;
//					TRACE("sid = %d, loop = %d My standing point is invalid x = %d, y = %d\n", m_sSid, i, pt.x, pt.y);
					InterlockedIncrement(&g_CurrentNPCError);
					return FALSE;
//					break;
					
/*					DeleteNPC();// 昏力啊 酒丛...
					TRACE("sid = %d, loop = %d My standing point is invalid x = %d, y = %d\n", m_sSid, i, pt.x, pt.y);
					return FALSE;
*/					
				}
				continue;
			}

			m_nInitX = m_sCurX = pt.x;
			m_nInitY = m_sCurY = pt.y;

//			ptTemp = ConvertToClient(m_sCurX, m_sCurY);
			break;
		}
	}

	SetUid(m_sCurX, m_sCurY, m_sNid + NPC_BAND);

	if(m_sDimension > 0) SetMapTypeBeforeGuildWar(pCom);		// 甘阑 悸泼茄促.

	// 惑怕捞惑 沥焊 檬扁拳
	m_dwLastAbnormalTime	= GetTickCount();
	m_tAbnormalKind = 0;
	m_dwAbnormalTime = 0;

	// 傍己傈俊辑 NPC HP啊 荐府啊 救登绢 乐栏搁
//	if(m_pGuardFortress) SetFortressState();
	
	if(m_bFirstLive)	// NPC 啊 贸澜 混酒唱绰 版快
	{
		NpcTypeParser();
		m_tWeaponClass = GetWeaponClass();
		m_bFirstLive = FALSE;

		InterlockedIncrement(&g_CurrentNPC);
	}

	// 各阑 付瘤阜栏肺 磷牢 蜡历狼 uid 檬扁拳
	m_lKillUid = -1;

	// Test Code
//	CString strTemp = m_strName;
//	if(strTemp == "凝" || strTemp == "弊饭捞飘候遣")m_sHP = 1;

	// 蜡历俊霸 NPC 沥焊傈价...
	modify_index = 0;
	::ZeroMemory(modify_send, sizeof(modify_send));
	FillNpcInfo(modify_send, modify_index, INFO_MODIFY);

	SendInsight(pCom, modify_send, modify_index);

	m_presx = -1;
	m_presy = -1;

	SightRecalc(pCom);

	return TRUE;
}


///////////////////////////////////////////////////////////////////
//	NPC 扁夯利牢 己氢阑 盒幅, 悸泼茄促.
//
void CNpc::NpcTypeParser()
{
	MYSHORT sAI;

	BYTE upTemp = 0;			// 惑困 8厚飘
	BYTE dwTemp = 0;			// 窍困 8厚飘

	sAI.i = (short)m_sAI;

	upTemp = sAI.b[0];
	dwTemp = sAI.b[1];
//	temp = m_sAI;//m_byAI

	m_tNpcAttType = upTemp >> 7;

	upTemp = upTemp << 1;
	m_tNpcLongType = upTemp >> 7;

	upTemp = upTemp << 1;
	m_tNpcGroupType = upTemp >> 7;

	m_iNormalATRatio = m_byIronSkin;
	m_iSpecialATRatio = m_byReAttack;
	m_iMagicATRatio = m_bySubAttack;

	m_tSPATRange = m_byWildShot;

/*
	switch( (int)m_byVitalC )
	{
	case	0:	// 老馆父
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = FALSE;
		break;

	case	1:	// 概流父
		m_bCanNormalAT = FALSE;
		m_bCanMagicAT = TRUE;
		m_bCanSPAT = FALSE;
		break;

	case	2:	// 老馆, 漂荐
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = TRUE;

		m_tSPATRange = m_byWildShot;
		m_tSPATAI = m_byExcitedRate;
		break;

	case	3:	// 概流, 漂荐
		m_bCanNormalAT = FALSE;
		m_bCanMagicAT = TRUE;
		m_bCanSPAT = TRUE;

		m_tSPATRange = m_byWildShot;
		m_tSPATAI = m_byExcitedRate;
		break;

	case	4:	// 老馆, 概流
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = TRUE;
		break;

	case	5:	// 漂荐父
		m_bCanNormalAT = FALSE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = TRUE;

		m_tSPATRange = m_byWildShot;
		m_tSPATAI = m_byExcitedRate;
		break;

	case	6:	// 老馆, 概流, 漂荐
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = TRUE;
		m_bCanSPAT = TRUE;

		m_tSPATRange = m_byWildShot;
		m_tSPATAI = m_byExcitedRate;
		break;

	default:
		m_bCanNormalAT = TRUE;
		m_bCanMagicAT = FALSE;
		m_bCanSPAT = FALSE;
		break;

	}
*/
}

///////////////////////////////////////////////////////////////////
//	NPC 林函狼 利阑 茫绰促.
//
BOOL CNpc::FindEnemy(COM *pCom)
{
	BOOL bSearch = FALSE;

	if(m_tNpcType == NPCTYPE_NPC || m_tNpcType == NPCTYPE_DOOR || m_tNpcType == NPCTYPE_GUILD_DOOR) return FALSE;
	if(m_tNpcType == NPCTYPE_GUILD_NPC || m_tNpcType == NPCTYPE_GUILD_MARK) return FALSE;

	if(m_byAX == 0 && m_byAZ == 0 ) return FALSE;		// 郴啊 傍拜仿捞 绝栏搁 傍拜窍瘤 臼绰促
	if(m_bySearchRange == 0) return FALSE;
	if(m_tNpcType == NPCTYPE_GUARD) bSearch = TRUE;
	if(m_tNpcType == NPCTYPE_GUILD_GUARD)
	{
		if(m_pGuardFortress && m_tRepairDamaged == NPC_DEAD_REPAIR_STATE) return FALSE;
		// 傍己傈俊辑 秦寸 堡急器 HP啊 0捞搁 悼累 阂瓷烙 
		 bSearch = TRUE;
	}

	if(!bSearch && !m_tNpcAttType && m_Target.id < 0 )
	{
		return FALSE;
	}
	else
	{
//		if( (GetTickCount() - m_dLastFind) < (DWORD)1000 )
		if( (GetTickCount() - m_dLastFind) < (DWORD)2000 )
		{
			return FALSE;
		}
	}

	m_dLastFind = GetTickCount();

//	if(m_Target.id != -1) return TRUE;

	int min_x, min_y, max_x, max_y;

	min_x = m_sCurX - m_bySearchRange;		if( min_x < 0 ) min_x = 0;
	min_y = m_sCurY - m_bySearchRange;		if( min_y < 0 ) min_y = 0;
	max_x = m_sCurX + m_bySearchRange;
	max_y = m_sCurY + m_bySearchRange;

	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 2;
	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 2;

	int ix, iy;
	int target_uid;
	int uid;
	int rank = 0;

	USER *pUser = NULL;
	CNpc *pNpc = NULL;

	int tempLevel = 0, oldLevel = 1000;

	for(ix = min_x; ix <= max_x; ix++)
	{
		for(iy = min_y; iy <= max_y; iy++)
		{
			target_uid = m_pOrgMap[ix][iy].m_lUser;

			if( target_uid >= USER_BAND && target_uid < NPC_BAND )
			{
				uid = target_uid - USER_BAND;

				pUser = GetUser(pCom, uid);
				if( pUser != NULL && pUser->m_bLive == USER_LIVE)
				{
					if( ix != pUser->m_curx || iy != pUser->m_cury )
					{
						continue;
					}

					if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD)
					{
						//rank = DEATH_RANK - CITY_RANK_INTERVAL;
						if(pUser->m_sKillCount > 100 || pUser->m_bPkStatus)
						{
							m_Target.id	= target_uid;
							m_Target.failCount = 0;
							m_Target.x	= ix;
							m_Target.y	= iy;							
							return TRUE;
						}
					}
																	// 辨靛傈且锭绰 惑痢家蜡 辨靛盔篮 公矫
					if(m_tNpcType == NPCTYPE_GUILD_GUARD)	
					{
						if(m_tGuildWar == GUILD_WARRING)
						{
							//if(pUser->m_dwGuild == m_pGuardStore->m_iGuildSid) continue;
							if(m_pGuardStore) 
							{
								if(pUser->m_dwGuild == m_pGuardStore->m_iGuildSid) continue;
							}
							else if(m_pGuardFortress)
							{
								if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid) continue;
							}

							m_Target.id	= target_uid;
							m_Target.failCount = 0;
							m_Target.x	= ix;
							m_Target.y	= iy;
							return TRUE;
						}
					}

					if(pUser->m_tIsOP == 1) continue;				// 款康磊捞搁 公矫...^^
					if(pUser->m_bPShopOpen == TRUE) continue;		// User has personal shop
//					if(pUser->m_dwHideTime > 0) continue;			// 篮葱 惑怕搁 公矫等促.
					//饶傍各...
					if(!m_tNpcAttType)		// 朝 傍拜茄 仇阑 茫绰促.
					{
						if(IsDamagedUserList(pUser) || (m_tNpcGroupType && m_Target.id == target_uid))
						{
							m_Target.id	= target_uid;
							m_Target.failCount = 0;
							m_Target.x	= ix;
							m_Target.y	= iy;
							return TRUE;
						}
					}
					else	// 急傍各...
					{						
						if(IsSurround(ix, iy) == TRUE) continue;	//笛矾 阶咯 乐栏搁 公矫茄促.(盔芭府, 辟芭府 公矫)

						USER *pTUser;

						pTUser = pCom->GetUserUid(uid);
						if ( pTUser == NULL ) continue;

						tempLevel = pTUser->m_sLevel;

						if(tempLevel <= oldLevel) 
						{
							oldLevel = tempLevel;									
							m_Target.id	= target_uid;
							m_Target.failCount = 0;
							m_Target.x	= ix;
							m_Target.y	= iy;
							return TRUE;
						}
					}
				}
			}
		}
	}

	InitUserList();		// 酒公档 绝栏骨肺 府胶飘俊 包府窍绰 蜡历甫 檬扁拳茄促.
	InitTarget();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	林函俊 唱甫 傍拜茄 蜡历啊 乐绰瘤 舅酒夯促
//
BOOL CNpc::IsDamagedUserList(USER *pUser)
{
//	int count = m_arDamagedUserList.GetSize();
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;

	int sLen = strlen(pUser->m_strUserID);

	if(sLen < 0 || sLen > CHAR_NAME_LENGTH) return FALSE;

	for(int i = 0; i < NPC_HAVE_USER_LIST; i++)
	{
		if(strcmp(m_DamagedUserList[i].strUserID, pUser->m_strUserID) == 0) return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	Target 苞 NPC 埃 Path Finding阑 荐青茄促.
//
BOOL CNpc::GetTargetPath(COM* pCom)
{
	USER* pUser = GetUser(pCom, m_Target.id - USER_BAND);
	if(pUser == NULL)
	{
		InitTarget();
		return FALSE;
	}
	if(pUser->m_sHP <= 0 || pUser->m_state != STATE_GAMESTARTED || pUser->m_bLive == FALSE)
	{
		InitTarget();
		return FALSE;
	}
/*	if(strcmp(m_Target.szName, pUser->m_strUserID) != 0)
	{
		InitTarget();
		return FALSE;
	}
*/
	int iTempRange = m_bySearchRange;				// 老矫利栏肺 焊沥茄促.
//	if(m_arDamagedUserList.GetSize()) iTempRange *= 2;	// 傍拜罐篮 惑怕搁 茫阑 裹困 刘啊.
	if(IsDamagedUserList(pUser)) iTempRange *= 2;	// 傍拜罐篮 惑怕搁 茫阑 裹困 刘啊.
	else iTempRange += 4;
	
	int min_x = m_sCurX - iTempRange;	if(min_x < 0) min_x = 0;
	int min_y = m_sCurY - iTempRange;	if(min_y < 0) min_y = 0;
	int max_x = m_sCurX + iTempRange;	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
	int max_y = m_sCurY + iTempRange;	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

	// 格钎痢捞 Search Range甫 哈绢唱瘤 臼绰瘤 八荤
	CRect r = CRect(min_x, min_y, max_x+1, max_y+1);
	if(r.PtInRect(CPoint(pUser->m_curx, pUser->m_cury)) == FALSE) return FALSE;

	// Run Path Find ---------------------------------------------//
	CPoint start, end;
	start.x = m_sCurX - min_x;
	start.y = m_sCurY - min_y;
	end.x = pUser->m_curx - min_x;
	end.y = pUser->m_cury - min_y;

	m_ptDest.x = m_Target.x;
	m_ptDest.y = m_Target.y;

	m_min_x = min_x;
	m_min_y = min_y;
	m_max_x = max_x;
	m_max_y = max_y;

	return PathFind(start, end);
}

////////////////////////////////////////////////////////////////////////////////
//	NPC 啊 Path Find 窍妨绰 付瘤阜 痢篮 泅犁 舅绊府硫惑 亲惑 捞悼阂啊 谅钎捞骨肺
//	付瘤阜 谅钎甫 荐沥
//	
//	## 泅犁(2000-12-12) 菩胶颇牢靛 舅绊府硫捞 荐沥登绢 捞 窃荐绰 鞘夸绝澜 ##
BOOL CNpc::GetLastPoint(int sx, int sy, int& ex, int& ey)
{
	int i;
	int x = 0, y = 0;
	int nx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
	int ny[] = {-1, -1, -1, 0, 1, 1, 1, 0};
	
	BOOL bSearchDest = FALSE;
	MAP* pMap = g_zone[m_ZoneIndex];
	for(i = 0; i < sizeof(nx)/sizeof(nx[0]); i++)
	{
		x = ex + nx[i]; 
		if(x >= pMap->m_sizeMap.cx) x--;
		if(x < 0) x = 0;

		y = ey + ny[i];
		if(y >= pMap->m_sizeMap.cy) y--;
		if(y < 0) y = 0;
		
		if(m_pOrgMap[x][y].m_bMove == 0 && m_pOrgMap[x][y].m_lUser == 0) 
		{
			ex = x;
			ey = y;
			bSearchDest = TRUE;
			break;
		}
	}

	if (bSearchDest) return TRUE;

	int nSearchSize = max(abs(sx - ex), abs(sy - ey));
//	ASSERT(nSearchSize);
	
	for (i = nSearchSize; i > 0; i--) 
	{
		x = sx + (ex - sx) * i / nSearchSize;
		y = sy + (ey - sy) * i / nSearchSize;
		
		if ((x + y) % 2 != 0) y++;	//焊沥

		if(m_pOrgMap[x][y].m_bMove == 0 && m_pOrgMap[x][y].m_lUser == 0) 
		{
			ex = x;
			ey = y;
			bSearchDest = TRUE;
			break;
		}
	}

	if (!bSearchDest) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	NPC客 Target 苞狼 芭府啊 瘤沥 裹困焊促 累篮瘤 魄窜
//
BOOL CNpc::IsCloseTarget(COM* pCom, int nRange)
{
	// 趣矫唱 版厚捍捞 NPC甫 傍拜窍霸 登搁 鸥百捞 蜡历啊 酒聪扼 NPC老 荐 档 乐促.
	USER* pUser = GetUser(pCom, m_Target.id - USER_BAND);
	if(pUser == NULL) 
	{
		InitTarget();
		return FALSE;
	}
	if(pUser->m_sHP <= 0 || pUser->m_state != STATE_GAMESTARTED || pUser->m_bLive == FALSE)
	{
		InitTarget();
		return FALSE;
	}

	CPoint ptUser	= ConvertToClient(pUser->m_curx, pUser->m_cury);
	CPoint ptNpc	= ConvertToClient(m_sCurX, m_sCurY);

	//^^ 家付俊辑绰 芭府八祸捞 酒聪扼 甘 八祸栏肺 芭府拌魂 棺 肋给等 蜡历 谅钎甫 眉农窍绊 乐澜
	int dx = abs(ptUser.x - ptNpc.x);
	int dy = abs(ptUser.y - ptNpc.y);
	int max_dist = __max(dx, dy);
	
	if(max_dist > nRange * 2) return FALSE; // 努扼牢飘 谅钎绰 2瞒捞啊 茄伎捞骨肺 *2甫 茄促.

	m_Target.x = pUser->m_curx;
	m_Target.y = pUser->m_cury;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	NPC客 Target 苞狼 芭府啊 瘤沥 裹困焊促 累篮瘤 魄窜
//
BOOL CNpc::IsCloseTarget(USER *pUser, int nRange)
{
	if(pUser == NULL)
	{
		return FALSE;
	}
	if(pUser->m_sHP <= 0 || pUser->m_state != STATE_GAMESTARTED || pUser->m_bLive == FALSE)
	{
		return FALSE;
	}

	CPoint ptUser	= ConvertToClient(pUser->m_curx, pUser->m_cury);
	CPoint ptNpc	= ConvertToClient(m_sCurX, m_sCurY);

	//^^ 家付俊辑绰 芭府八祸捞 酒聪扼 甘 八祸栏肺 芭府拌魂 棺 肋给等 蜡历 谅钎甫 眉农窍绊 乐澜
	int dx = abs(pUser->m_curx - m_sCurX);
	int dy = abs(pUser->m_cury - m_sCurY);
	int max_dist = __max(dx, dy);
	
	if(max_dist > nRange * 2) return FALSE;

	InitTarget();
	m_Target.id = pUser->m_uid + USER_BAND;
	m_Target.x = pUser->m_curx;
	m_Target.y = pUser->m_cury;

/*	if(pUser->m_strUserID != NULL)
	{
		m_Target.nLen = strlen(pUser->m_strUserID);

		if(m_Target.nLen <= CHAR_NAME_LENGTH) strncpy(m_Target.szName, pUser->m_strUserID, m_Target.nLen);
		else								  ::ZeroMemory(m_Target.szName, sizeof(m_Target.szName));
	}
*/	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	Path Find 肺 茫篮辨阑 促 捞悼 沁绰瘤 魄窜
//
BOOL CNpc::IsMovingEnd()
{
	if( m_bRandMove )		// 8规氢 急琶 框流烙老锭
	{
		if( m_arRandMove.GetSize() ) return FALSE;

		return TRUE;
	}

	if(!m_pPath) return TRUE;

	int min_x = m_min_x;
	int min_y = m_min_y;

	if((m_sCurX - min_x) == m_vEndPoint.x && (m_sCurY - min_y) == m_vEndPoint.y) return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
//	秦寸 uid狼 USER* 甫 掘绰促.
//
USER* CNpc::GetUser(COM* pCom, int uid)
{
	if(!pCom) return NULL;
	//if(uid < 0 || uid >= MAX_USER) return NULL;

	return pCom->GetUserUid(uid);
}

/////////////////////////////////////////////////////////////////////////////////
//	Target 狼 困摹啊 促矫 辨茫扁甫 且 沥档肺 函沁绰瘤 魄窜
//	去鄂殿狼 惑怕捞惑 荤捞坷葱捞唱 胶懦殿捞 瘤盔登搁 捞 窃荐甫 捞侩窍搁 瞪 淀
//
BOOL CNpc::IsChangePath(COM* pCom, int nStep)
{
	if(!m_pPath) return TRUE;

	CPoint pt;
	GetTargetPos(pCom, pt);
	NODE* pTemp = m_pPath;

	CPoint ptPath[2];
	while(1)
	{
		if(pTemp == NULL) break;

		if(pTemp->Parent) 
		{
			ptPath[0].x = m_min_x + pTemp->x;
			ptPath[0].y = m_min_y + pTemp->y;

			pTemp = pTemp->Parent;			
		}
		else
		{			
			ptPath[1].x = m_min_x + pTemp->x;
			ptPath[1].y = m_min_y + pTemp->y;

			break;
		}
	}

	for(int i = 0; i < 2; i++)
	{
		if(abs(ptPath[i].x - pt.x) <= m_byRange && abs(ptPath[i].y - pt.y) <= m_byRange) return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//	Target 狼 泅犁 困摹甫 掘绰促.
//
BOOL CNpc::GetTargetPos(COM *pCom, CPoint &pt)
{
	USER* pUser = GetUser(pCom, m_Target.id - USER_BAND);

	if(!pUser) return FALSE;

	pt.x = pUser->m_curx;
	pt.y = pUser->m_cury;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//	Target 苞 NPC 埃俊 辨茫扁甫 促矫茄促.
//
BOOL CNpc::ResetPath(COM* pCom)
{
	CPoint pt;
	GetTargetPos(pCom, pt);

	m_Target.x = pt.x;
	m_Target.y = pt.y;

	return GetTargetPath(pCom);
}

/////////////////////////////////////////////////////////////////////////////////
//	Step 荐 父怒 鸥南阑 氢秦 捞悼茄促.
//
BOOL CNpc::StepMove(COM* pCom, int nStep)
{
//	if(m_tNpcType == NPCTYPE_GUILD_DOOR)	return FALSE;	// 捞悼救窍霸...

	if(!m_pPath && !m_bRandMove) return FALSE;
	if(m_NpcState != NPC_MOVING && m_NpcState != NPC_TRACING && m_NpcState != NPC_BACK) return FALSE;
	
	int min_x;
	int min_y;
	int will_x;
	int will_y;

	CPoint ptPre;

	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return FALSE;
	if( !pMap->m_pMap ) return FALSE;

	for(int i = 0; i < nStep; i++)
	{
		if( m_bRandMove )
		{
			if( !m_arRandMove.GetSize() ) return FALSE;

			min_x = m_min_x;
			min_y = m_min_y;

			will_x = min_x + m_arRandMove[0].x;
			will_y = min_y + m_arRandMove[0].y;

			m_arRandMove.RemoveAt( 0 );

			if( will_x >= pMap->m_sizeMap.cx || will_x < 0 || will_y >= pMap->m_sizeMap.cy || will_y < 0 )
			{
				m_vEndPoint.x = m_sCurX - min_x;
				m_vEndPoint.y = m_sCurY - min_y;

				return FALSE;
			}

			if( pMap->m_pMap[will_x][will_y].m_bMove != 0 || pMap->m_pMap[will_x][will_y].m_lUser != 0 )
			{
				m_vEndPoint.x = m_sCurX - min_x;
				m_vEndPoint.y = m_sCurY - min_y;

				return FALSE;
			}

			ptPre.x = m_sCurX;
			ptPre.y = m_sCurY;

			m_sCurX = will_x;
			m_sCurY = will_y;

			// 矫具 犁拌魂...
			SightRecalc( pCom );

			break;
		}
		else if(m_pPath->Parent)
		{
			m_pPath = m_pPath->Parent;

			min_x = m_min_x;
			min_y = m_min_y;

			will_x = min_x + m_pPath->x;
			will_y = min_y + m_pPath->y;

			if(will_x >= pMap->m_sizeMap.cx || will_x < 0 || will_y >= pMap->m_sizeMap.cy || will_y < 0) 
			{
				m_vEndPoint.x = m_sCurX - min_x;
				m_vEndPoint.y = m_sCurY - min_y;
				return FALSE;
			}
			
			if(pMap->m_pMap[will_x][will_y].m_bMove != 0 || pMap->m_pMap[will_x][will_y].m_lUser != 0)
			{
				m_vEndPoint.x = m_sCurX - min_x;
				m_vEndPoint.y = m_sCurY - min_y;
				return FALSE;
			}

			ptPre.x = m_sCurX;
			ptPre.y = m_sCurY;

			m_sCurX = will_x;
			m_sCurY = will_y;

			//矫具 犁拌魂...
			SightRecalc(pCom);

			break;
		}
		
		return FALSE;
	}

	if(SetUid(m_sCurX, m_sCurY, m_sNid + NPC_BAND))
	{
		pMap->m_pMap[ptPre.x][ptPre.y].m_lUser = 0;
		return TRUE;
	}
	else return FALSE;

//	return SetUid(m_sCurX, m_sCurY, m_sNid + NPC_BAND);
}

//////////////////////////////////////////////////////////////////////////////
//	Target 俊 措茄 傍拜 贸府
//
int CNpc::Attack(COM *pCom)
{
	if(!pCom) return 10000;

	int ret = 0;
	int nStandingTime = m_sStandTime;

	// 醚扁拌凯 老锭绰 鸥百苞狼 芭府拌魂阑 崔府秦具 茄促.
//	if(m_tNpcType != NPCTYPE_GUARD && m_tNpcType != NPCTYPE_GUILD_GUARD)// 版厚捍捞 酒聪搁 矫具 拌魂
//	{
	if(IsCloseTarget(pCom, m_byRange) == FALSE)// Check Code (窜瘤 规绢利牢 螟搁俊辑 持篮 内靛)
	{
		if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD)
		{
			m_NpcState = NPC_STANDING;
			return 0;
		}
		m_NpcState = NPC_TRACING;			// 傍拜窍绊 档噶啊绰 蜡历甫 蝶扼 棱扁困秦(馆览阑 粱歹 狐福霸) 
		return 0;							// IsCloseTarget()俊 蜡历 x, y蔼阑 盎脚窍绊 Delay = 0栏肺 淋
	}
//	}

	short	sTempHP		= 0;
	CNpc*	pNpc		= NULL;	
	USER*	pUser		= NULL;

	CByteArray	arSkillAction1, arSkillAction2;

	int		nHit = 0;
	int		nAvoid = 0;

	BOOL	bIsHit = FALSE;
	BOOL	bIsCritical = FALSE;

	int		nDamage		= 0;
	int		nDefense	= 0;

	int		iRandom = 0;
//	int		iDefenseDex = 0;
//	double	determine	= 0;
	int		determine = 0;
	int		iDexHitRate = 0, iLevelHitRate = 0;

	int nID = m_Target.id;					// Target 阑 备茄促.

	// 疙吝咯何 魄窜 函荐 檬扁拳
	bIsHit = FALSE;		

	// 雀乔蔼/疙吝魄沥/单固瘤 拌魂 -----------------------------------------//
	if(nID >= USER_BAND && nID < NPC_BAND)	// Target 捞 User 牢 版快
	{
		pUser = GetUser(pCom, nID - USER_BAND);
		
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)// User 啊 Invalid 茄 版快
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

		if(pUser->m_bLive == USER_DEAD)			// User 啊 捞固 磷篮版快
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

		if(pUser->m_dwNoDamageTime != 0)		// User 啊 公利鸥烙俊 乐阑版快
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}
		
		if(pUser->m_bPShopOpen == TRUE)			// User has personal shop 
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}
		if(pUser->m_bSessionOnline == true)			// 离线不被怪攻击
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

/*	困俊辑 捞固 眉农 窃
		if(pUser->m_state == STATE_DISCONNECTED)
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}
*/
		if(m_tNpcType == NPCTYPE_GUILD_GUARD)	
		{
			if(m_tGuildWar == GUILD_WARRING && pUser->m_dwGuild > 0)
			{
				if(m_pGuardStore) 
				{
					if(pUser->m_dwGuild == m_pGuardStore->m_iGuildSid) return nStandingTime;
				}
				else if(m_pGuardFortress)
				{
					if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid) return nStandingTime;
				}
			}
		}
												// 篮葱 惑怕捞搁 版厚捍阑 力寇茄 葛电 各俊 秦寸			
//		if(m_tNpcType != NPCTYPE_GUARD && pUser->m_dwHideTime > 0)
//		{
//			InitTarget();
//			m_NpcState = NPC_MOVING;
//			return nStandingTime;
//		}

		if(pUser->m_tIsOP == 1)
		{
			InitTarget();
			m_NpcState = NPC_MOVING;
			return nStandingTime;
		}

		// 雀乔蔼 拌魂 
		nAvoid = pUser->GetAvoid();
		
		// 疙吝咯何 魄窜
		iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5); 
		
		iDexHitRate = (int)( 30.0 * ( (double)m_sDEX/(m_sDEX + pUser->m_sMagicDEX) ) + 15.0 );
		iLevelHitRate = (int)( 70.0 * ( (double)m_byClassLevel/(pUser->m_sLevel + m_byClassLevel) ) + 15.0);

		determine = iDexHitRate + iLevelHitRate - (nAvoid+pUser->m_Avoid);
/*
		iDefenseDex = pUser->m_sMagicDEX;
		if(iDefenseDex < 0) iDefenseDex = 0; //规绢 内靛

		determine = 200 * ((double)m_sDEX / (m_sDEX + iDefenseDex)) * ((double)m_byClassLevel / (m_byClassLevel + pUser->m_sLevel));
		determine = determine - nAvoid;
*/
		if(determine < ATTACK_MIN) determine = ATTACK_MIN;			// 弥家 20
		else if(determine > ATTACK_MAX) determine = ATTACK_MAX;		// 弥措

		if(iRandom < determine)	bIsHit = TRUE;		// 疙吝

		// 傍拜 固胶
		if(bIsHit == FALSE)					
		{
			SendAttackMiss(pCom, nID);
			return m_sAttackDelay;;
		}

		// 疙吝捞搁 //Damage 贸府 ----------------------------------------------------------------//

		nDamage = GetFinalDamage(pUser);	// 弥辆 措固瘤
		//物理伤害减少与最终伤害减少
		//nDamage=nDamage-(pUser->m_DynamicMagicItem[5]+pUser->m_DynamicMagicItem[6]);
		nDamage=nDamage-(/*pUser->m_DynamicUserData[MAGIC_PHY_ATTACK_DOWN]+*/pUser->m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]);
		if(nDamage < 15) nDamage = 15;
		if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) nDamage += 10;		// 惑措祈捞 趁扁 捞惑捞搁 单固瘤 眠啊

		//如果存在护法，30%的几率先攻击护法
		if(pUser->m_tHuFaType &&pUser->m_nHuFaHP>0)
		{
			nID = nID+USER_HUFA_BAND;
			//伤害数据
			pUser->SendDamageNum(0,nID,(short)nDamage);

			SendAttackSuccess(pCom, nID, bIsCritical, pUser->m_nHuFaHP, pUser->m_nHuFaMaxHP);//yskang 0.3
			if(nDamage > 0)
				pUser->SetHuFaDamage(nDamage);
			if(pUser->m_nHuFaHP>0)
			{
				pUser->HuFaAttack(m_sNid+NPC_BAND);
			}
			return m_sAttackDelay;
		}

		if(nDamage > 0) pUser->SetDamage(nDamage);

		// 规绢螟 郴备档 皑家
		pUser->SendDamagedItem(nDamage);
		pUser->SendDamageNum(0,pUser->m_uid+USER_BAND,nDamage);
//		if(pUser->m_bLive == USER_DEAD || pUser->m_sHP <= 0)//@@@ 唱吝俊 绊魔
		if(pUser->m_lDeadUsed == 1)
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			m_Delay = m_sStandTime;

			if(m_NpcVirtualState == NPC_STANDING)
			{
				if(m_sPid == 179) pUser->GetLevelDownExp(FALSE, -1, TRUE,m_strName);//傍侩 阁胶磐老版快 版氰摹 1%皑家
				else				pUser->GetLevelDownExp(FALSE, -1, FALSE,m_strName);// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
			}
			if(m_tNpcType == NPCTYPE_GUARD) pUser->SendCityRank(1);				// 版厚捍俊霸 磷栏搁 PK 墨款飘 1 皑家
																				// 泅犁 版厚捍篮 畴富 傍拜父 窍骨肺 捞镑俊父 眠啊 
																				// Add by JJS 2002.05.24
		}

		//yskang 0.3 SendAttackSuccess(pCom, nID, arSkillAction1, arSkillAction2, pUser->m_sHP, pUser->m_sMagicMaxHP);
		SendAttackSuccess(pCom, nID, bIsCritical, pUser->m_sHP, pUser->m_sMagicMaxHP);//yskang 0.3
	}

	return m_sAttackDelay;
}

CNpc* CNpc::GetNpc(int nid)
{
	CNpc* pNpc = NULL;

	int nSize = g_arNpc.GetSize();

	if(nid < 0 || nid >= nSize) return NULL;

	for( int i = 0; i < g_arNpc.GetSize(); i++)
	{
		pNpc = g_arNpc[i];
		if( !pNpc ) continue;

		if( pNpc->m_sNid == nid )
		{
			return pNpc;
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////
//	NPC 狼 傍拜仿阑 掘绢柯促.
//
int CNpc::GetAttack()
{
	int X = m_byAX;
	int Y = m_byAZ;

	return XdY(X, Y);
}

////////////////////////////////////////////////////////////////////////////
//	NPC 狼 规绢仿阑 掘绢柯促.
//
int CNpc::GetDefense()
{
	return m_iDefense;
}

/////////////////////////////////////////////////////////////////////////////
//	Damage 拌魂, 父距 m_sHP 啊 0 捞窍捞搁 荤噶贸府
//
BOOL CNpc::SetDamage(int nDamage)
{
	if(m_NpcState == NPC_DEAD) return TRUE;
	if(m_sHP <= 0) return TRUE;
	if(nDamage <= 0) return TRUE;

	m_sHP -= nDamage;

	if( m_sHP <= 0 )
	{
		m_sHP = 0;
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//	NPC 荤噶贸府
//
void CNpc::Dead()
{
	long lNpcUid = m_sNid + NPC_BAND;
	if(m_pOrgMap[m_sCurX][m_sCurY].m_lUser == lNpcUid)
	{
		::InterlockedExchange(&m_pOrgMap[m_sCurX][m_sCurY].m_lUser, (LONG)0);
	}
	
	m_sHP = 0;
	m_NpcState = NPC_DEAD;
	
	if(m_bSummon)
	{
		m_bSummonDead = TRUE;
		m_bSummon = FALSE;
	}
	
	if(m_NpcVirtualState == NPC_MOVING)	m_NpcVirtualState = NPC_WAIT;
	
	m_Delay = m_sRegenTime;
	m_bFirstLive = FALSE;
}

//////////////////////////////////////////////////////////////////////////////
//	NPC 荤噶贸府矫 版氰摹 盒硅甫 拌魂茄促.(老馆 蜡历客 滚叼 荤侩磊备盒)
//
void CNpc::SendExpToUserList(COM *pCom)
{
	int i;
	int exp = 0;//, eventExp = 0;
	int totalDamage = 0;
	int firstDamage = 0;
	DWORD plusExp = 0;
	int MaxDamage=0;
	USER *KeypUser=NULL;

	if(m_NpcVirtualState == NPC_WAIT) return;		// 啊惑傍埃俊辑 版氰摹 绝促.
	if(m_tNpcType >= NPCTYPE_GUILD_NPC) return;


	if(InterlockedCompareExchange((long*)&m_lDamage, (long)1, (long)0) == (long)0){
		if(NpcDrop<=0) {
			InterlockedExchange(&m_lDamage, (LONG)0); 
			return;
		}else{
			NpcDrop=NpcDrop-1;
		}

		InterlockedExchange(&m_lDamage, (LONG)0); 
	}
//	if(m_tNpcType == NPCTYPE_GUILD_NPC || m_tNpcType == NPCTYPE_GUILD_DOOR) return;
//	if(m_tNpcType == NPCTYPE_GUILD_GUARD) return;	// 辨靛傈俊辑 磷篮 版厚捍篮 版氰摹甫 救霖促.
//	SYSTEMTIME gTime;
//	GetLocalTime(&gTime);	

	USER *pUser = NULL;

	IsUserInSight(pCom);					// 泅犁 伎裹困救俊 乐蠢衬?(各阑 扁霖栏肺 茄 拳搁 : 敲贰弊 悸泼)

	if(m_DamagedUserList[0].iUid >= 0 && m_DamagedUserList[0].nDamage > 0)	// 霉锅掳俊 蜡历啊 乐促搁 2硅 
	{
		MaxDamage=firstDamage = m_DamagedUserList[0].nDamage;
		m_DamagedUserList[0].nDamage = m_DamagedUserList[0].nDamage * 2;
		
	}
														
	for(i = 0; i < NPC_HAVE_USER_LIST; i++)				// 老窜 府胶飘甫 八祸茄促.
	{												
		if(m_DamagedUserList[i].iUid < 0 || m_DamagedUserList[i].nDamage<= 0) continue;		// 救傈内靛
		if(m_DamagedUserList[i].bIs == TRUE) pUser = GetUser(pCom, m_DamagedUserList[i].iUid);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		KeypUser=pUser;
		if(abs(pUser->m_sLevel-m_byClassLevel) >o_yehuoini[0]->djxz)  //人物与怪差距40级无法获得经验限制处
			 continue;
		//if(pUser->m_dwShopPingDN > 3000 )  //105级后不能得到经验，杀死怪物几个后不能得到经验
		//	 continue;

		totalDamage = m_DamagedUserList[i].nDamage;
		if(MaxDamage<totalDamage){
			MaxDamage=totalDamage;
			KeypUser=pUser;
		}

		if ( (m_sExp / 5) <= 0 ) continue;
	    if ( totalDamage == 0 ) continue;
		if(((m_TotalDamage + firstDamage) /5) <= 0) continue;
		long long t=(long long)(m_sExp / 5) * totalDamage;
		exp =(int)(t/((m_TotalDamage + firstDamage) / 5));

		pUser->m_iCityValue += m_sInclination;
		if(pUser->m_iCityValue > 2000000000) pUser->m_iCityValue = 2000000000;	// 弥措蔼捞 绝绢辑 烙狼肺 沥沁促.
		
		if(pUser->m_iDisplayType != 5 && pUser->m_iDisplayType != 6)
			pUser->GetExpCommon((int)(exp * 1.1));
		else
			pUser->GetExpCommon((int)(exp * 0.8)); //公丰 荤侩磊绰 版氰摹甫 撤冕促.
		//---------------------------------------------------------------------------------------------
		

	}
	Dead_User_level=0;
	if(KeypUser!=NULL)
		Dead_User_level=KeypUser->m_sLevel;
	
}

//////////////////////////////////////////////////////////////////////////////
//	泅犁 各阑 扁霖栏肺 茄 拳搁 裹困救俊 乐绰瘤 魄窜
//
void CNpc::IsUserInSight(COM *pCom)
{
	int j;

	USER* pUser = NULL;

	int iSearchRange = m_bySearchRange;						// 烙矫肺 棱疽促.
	int min_x, min_y, max_x, max_y;

	min_x = m_sCurX - 12;		if( min_x < 0 ) min_x = 0;
	min_y = m_sCurY - 13;		if( min_y < 0 ) min_y = 0;
	max_x = m_sCurX + 12;
	max_y = m_sCurY + 13;

	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

	int ix, iy;
	int target_uid;
	int uid;
	int iLen = 0;

	for(j = 0; j < NPC_HAVE_USER_LIST; j++)
	{
		m_DamagedUserList[j].bIs = FALSE;
	}

	for(ix = min_x; ix <= max_x; ix++)
	{
		for(iy = min_y; iy <= max_y; iy++)
		{
			target_uid = m_pOrgMap[ix][iy].m_lUser;

			if( target_uid >= USER_BAND && target_uid < NPC_BAND )
			{
				uid = target_uid - USER_BAND;
				for(j = 0; j < NPC_HAVE_USER_LIST; j++)
				{												// 爱绊乐绰 府胶飘惑狼 蜡历客 鞍促搁		
					if(m_DamagedUserList[j].iUid == uid)		// 弥辆 ID甫 厚背秦辑 悼老窍搁	
					{
						pUser = pCom->GetUserUid(uid);		
						if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_sCurZ) continue;
																
						iLen = strlen(pUser->m_strUserID);
						if(iLen <= 0 || iLen > CHAR_NAME_LENGTH) continue;

						if(strcmp(pUser->m_strUserID, m_DamagedUserList[j].strUserID) == 0) 
						{										// 捞锭辑具 粮犁茄促绰 钎矫甫 茄促.
							m_DamagedUserList[j].bIs = TRUE;
						}
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//	NPC DEAD Packet 阑 牢辟 蜡历俊霸 焊辰促.
//
int CNpc::SendDead(COM *pCom, int type,BOOL TreeBaoLv)
{
//	ASSERT(pCom);
	if(!pCom) return 0;
	if(m_NpcState != NPC_DEAD || m_sHP > 0) return 0;

	CBufferEx TempBuf;

	CPoint pt = ConvertToClient(m_sCurX, m_sCurY);

	TempBuf.Add(DEAD);
	TempBuf.Add((short)(m_sNid + NPC_BAND));
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);
		
	SendInsight(pCom, TempBuf, TempBuf.GetLength());
	if(type) GiveNpcHaveItem(pCom ,TreeBaoLv);	//掉东西
//	if(( (type)  &&  (abs(Dead_User_level-m_byClassLevel) <=50) )|| m_sEvent!=0)
//		GiveNpcHaveItem(pCom);	// 酒捞袍 冻备扁(版厚捍捞搁 救冻绢飘覆)
	
	return m_sRegenTime;
}

////////////////////////////////////////////////////////////////////////////////
//	林函俊 利捞 绝芭唱 荐悼各狼 版快 烙狼狼 痢栏肺 辨茫扁甫 茄 饶 框流牢促.
//
BOOL CNpc::RandomMove(COM *pCom)
{
	if(m_bySearchRange == 0) return FALSE;
	if(pCom == NULL) return FALSE;

	if(m_tNpcType == NPCTYPE_GUILD_DOOR)	return FALSE;	// 捞悼救窍霸...
	if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD)	return FALSE;	// 捞悼救窍霸...
	// NPC 啊 檬扁 困摹甫 哈绢车绰瘤 魄窜茄促.
	BOOL bIsIn = IsInRange();

	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return FALSE;
	if( !pMap->m_pMap ) return FALSE;

	CPoint pt;
	int nLoop = 0;
	int nDestX = -1, nDestY = -1;
	int min_x, min_y, max_x, max_y;
	int temp_minx = 0, temp_miny = 0, temp_maxx = 0, temp_maxy = 0;

	CRect rectIn;

	if(bIsIn)	// NPC 啊 檬扁 困摹甫 哈绢唱瘤 臼疽栏搁
	{
/*alisia
		int temp_range = m_bySearchRange / 2;

		min_x = m_sCurX - temp_range;	if(min_x < 0) min_x = 0;
		min_y = m_sCurY - temp_range;	if(min_y < 0) min_y = 0;
		max_x = m_sCurX + temp_range;	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
		max_y = m_sCurY + temp_range;	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

		rectIn.IntersectRect(CRect(m_nInitMinX, m_nInitMinY, m_nInitMaxX, m_nInitMaxY), CRect(min_x, min_y, max_x, max_y));

		nLoop = 0;
		while(1)
		{
			nDestX = myrand(rectIn.left, rectIn.right);
			nDestY = myrand(rectIn.top, rectIn.bottom);

			if(pMap->m_pMap[nDestX][nDestY].m_bMove != 0 || pMap->m_pMap[nDestX][nDestY].m_lUser != 0)
			{
				if(nLoop++ >= 10) 
				{
					TRACE("NOT FIND~~\n");
					return FALSE;
				}
				continue;
			}
			
			break;
		}
alisia*/

		m_bRandMove = TRUE;		// 泅犁 框流烙捞 8规氢 急琶 框流烙牢瘤甫 唱鸥辰促 - PathFind() 窃荐 救俊辑 府悸茄促

		m_arRandMove.RemoveAll();

		int axis_x[3];	axis_x[0] = -1;	axis_x[1] = 0;	axis_x[2] = 1;
		int axis_y[3];	axis_y[0] = -1;	axis_y[1] = 0;	axis_y[2] = 1;
		int rand_x, rand_y, rand_d;

		rand_x = myrand( 0, 2 );
		rand_y = myrand( 0, 2 );
		rand_d = myrand( 1, 5 );

		for( int i = 1; i <= rand_d; i++ )
		{
			m_arRandMove.Add( CPoint( axis_x[rand_x] * i, axis_y[rand_y] * i ) );
		}

		m_min_x = m_sCurX;
		m_min_y = m_sCurY;

		return TRUE;
	}
	else		// NPC 啊 檬扁 困摹甫 哈绢车栏搁
	{
		int x = 0, y = 0;
		
		min_x = m_sCurX;
		min_y = m_sCurY;
		max_x = m_sCurX;
		max_y = m_sCurY;

		if(m_nInitMinX < m_sCurX)	{min_x -= m_bySearchRange;	x += 1;} if(min_x < 0) min_x = 0;
		if(m_nInitMinY < m_sCurY)	{min_y -= m_bySearchRange;	y += 1;} if(min_y < 0) min_y = 0;
		if(m_nInitMaxX > m_sCurX)	{max_x += m_bySearchRange;	x += 1;} if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
		if(m_nInitMaxY > m_sCurY)	{max_y += m_bySearchRange;	y += 1;} if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;
				
		nLoop = 0;
		while(1)
		{
			nDestX = min_x + (rand() % (m_bySearchRange * x + 1)); 
			if(nDestX > max_x) nDestX = max_x;

			nDestY = min_y + (rand() % (m_bySearchRange * y + 1));
			if(nDestY > max_y) nDestY = max_y;

			if(pMap->m_pMap[nDestX][nDestY].m_bMove != 0 || pMap->m_pMap[nDestX][nDestY].m_lUser != 0)
			{
				if(nLoop++ >= 10) return FALSE;
				continue;
			}
			
			break;
		}
	}

	if(nDestX < 0 || nDestY < 0)
	{
		return FALSE;
	}

	// Run Path Find ---------------------------------------------//
	CPoint start, end;
	start.x = m_sCurX - min_x;
	start.y = m_sCurY - min_y;
	end.x = nDestX - min_x;
	end.y = nDestY - min_y;

	if(start.x < 0 || start.y < 0 || end.x < 0 || end.y < 0)
	{
		return FALSE;
	}

	m_ptDest.x = nDestX;
	m_ptDest.y = nDestY;

	m_min_x = min_x;
	m_min_y = min_y;
	m_max_x = max_x;
	m_max_y = max_y;

	return PathFind(start, end);
}

/////////////////////////////////////////////////////////////////////////////////////
//	NPC 啊 檬扁 积己困摹 救俊 乐绰瘤 八荤
//
BOOL CNpc::IsInRange()
{
	// NPC 啊 檬扁 困摹甫 哈绢车绰瘤 魄窜茄促.
//	CRect rect(m_nInitMinX, m_nInitMinY, m_nInitMaxX, m_nInitMaxY);
	
//	return rect.PtInRect(CPoint(m_sCurX, m_sCurY));

	if( m_nInitMinX > m_sCurX || m_nInitMaxX < m_sCurX ) return FALSE;
	if( m_nInitMinY > m_sCurY || m_nInitMaxY < m_sCurY ) return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
//	矫具啊 函版灯绰瘤 魄窜窍绊 函版灯促搁 函版郴侩阑 努扼捞攫飘肺 傈价茄促.
//
void CNpc::SightRecalc(COM* pCom)
{
	int sx, sy;

	sx = m_sCurX / SIGHT_SIZE_X;
	sy = m_sCurY / SIGHT_SIZE_Y;

	int dir_x = 0;
	int dir_y = 0;

	if( sx == m_presx && sy == m_presy ) return;
	
	if( m_presx == -1 || m_presy == -1 )
	{
		dir_x = 0;
		dir_y = 0;
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

	int prex = m_presx;
	int prey = m_presy;
	m_presx = sx;
	m_presy = sy;

	SendUserInfoBySightChange(dir_x, dir_y, prex, prey, pCom);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	矫具函版栏肺 牢茄 蜡历沥焊 傈价
//
void CNpc::SendUserInfoBySightChange(int dir_x, int dir_y, int prex, int prey, COM *pCom)
{
	int min_x = 0, min_y = 0;
	int max_x = 0, max_y = 0;

	int sx = m_presx;
	int sy = m_presy;

	int modify_index = 0;
	char modify_send[1024];		::ZeroMemory(modify_send, sizeof(modify_send));
	FillNpcInfo(modify_send, modify_index, INFO_MODIFY);

	int delete_index = 0;
	char delete_send[1024];		::ZeroMemory(delete_send, sizeof(delete_send));
	FillNpcInfo(delete_send, delete_index, INFO_DELETE);

	if( prex == -1 || prey == -1 )
	{
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
		return;
	}
	if( dir_x == DIR_OUTSIDE || dir_y == DIR_OUTSIDE )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
		return;
	}
	if( dir_x > 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx+1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
	}
	if( dir_y > 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy+1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
	}
	if( dir_x < 0 )
	{
		min_x = (prex+1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
	}
	if( dir_y < 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey+1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy)*SIGHT_SIZE_Y;
		SendToRange(pCom, modify_send, modify_index, min_x, min_y, max_x, max_y);
	}
}

////////////////////////////////////////////////////////////////////////////////
//	茄拳搁郴狼 蜡历俊霸父 沥焊傈价
//
void CNpc::SendExactScreen(COM* pCom, TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;
	
	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_SCREEN;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = m_sCurX;
	pNewData->y = m_sCurY;
	pNewData->z = m_sCurZ;
	pNewData->zone_index = m_ZoneIndex;

	pCom->Send(pNewData);
	if(pNewData) delete pNewData;
}

///////////////////////////////////////////////////////////////////////////////
//	矫具救俊 乐绰 蜡历俊霸 单捞磐 傈价
//
void CNpc::SendInsight(COM* pCom, TCHAR *pBuf, int nLength)
{
/*
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;
	
	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_INSIGHT;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = m_sCurX;
	pNewData->y = m_sCurY;
	pNewData->z = m_sCurZ;
	pNewData->zone_index = m_ZoneIndex;

	EnterCriticalSection( &(pCom->m_critSendData) );
	pCom->m_arSendData.Add( pNewData );
	LeaveCriticalSection( &(pCom->m_critSendData) );

	PostQueuedCompletionStatus( pCom->m_hSendIOCP, 0, 0, NULL );
*/
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;

	int sx = m_sCurX / SIGHT_SIZE_X;
	int sy = m_sCurY / SIGHT_SIZE_Y;
	
	int min_x = (sx-1)*SIGHT_SIZE_X; if( min_x < 0 ) min_x = 0;
	int max_x = (sx+2)*SIGHT_SIZE_X;
	int min_y = (sy-1)*SIGHT_SIZE_Y; if( min_y < 0 ) min_y = 0;
	int max_y = (sy+2)*SIGHT_SIZE_Y;
	
	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return;
	
	if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
	if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;
	
	int temp_uid;
	USER* pUser = NULL;

	for( int i = min_x; i < max_x; i++ )
	{
		for( int j = min_y; j < max_y; j++ )
		{				
			temp_uid = pMap->m_pMap[i][j].m_lUser;

			if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
			else temp_uid -= USER_BAND;
			
			if( temp_uid >= 0 && temp_uid < MAX_USER )
			{
				pUser = pCom->GetUserUid(temp_uid);
				if ( pUser == NULL ) continue;
				
				if( pUser->m_state == STATE_GAMESTARTED )
				{
					if( pUser->m_curx == i && pUser->m_cury == j && pUser->m_curz == m_sCurZ )
					{
						Send( pUser, pBuf, nLength );
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	泅犁 NPC 狼 困摹客 (xpos, ypos) 荤捞狼 芭府甫 拌魂
//
BOOL CNpc::GetDistance(int xpos, int ypos, int dist)
{
	if(xpos >= g_zone[m_ZoneIndex]->m_sizeMap.cx || xpos < 0 || ypos >= g_zone[m_ZoneIndex]->m_sizeMap.cy || ypos < 0) return FALSE;

	int dx = abs(xpos - m_sCurX);
	int dy = abs(ypos - m_sCurY);

	if(dx + dy > dist * 2) return FALSE;
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////
//	NPC 啊 啊柳 酒捞袍阑 冻焙促.
//怪物死后掉的物品计算
void CNpc::GiveNpcHaveItem(COM *pCom,BOOL TreeBaoLv)
{
	int temp = 0;
	int iPer = 0, iVal = 0;
	int iRandom;
	int nCount = 1;
	int nDnHap = 0;
    USER *pUser = GetUser(pCom, m_DamagedUserList[0].iUid);

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)	// 肋给等 蜡历酒捞叼捞搁 俊矾
	{
		return;
	}
//	SYSTEMTIME gTime;
//	GetLocalTime(&gTime);	

	if(m_NpcVirtualState == NPC_WAIT) return;

/*	if(gTime.wDay > 7 && gTime.wDay < 14)					// 8老何磐 13老鳖瘤	
	{
		if(m_byClassLevel <= 20)		nCount = 1;
		else if(m_byClassLevel <= 50)	nCount = 2;
		else if(m_byClassLevel <= 70)	nCount = 3;
		else							nCount = 5;
	}
	else
	{
*/
	if(TreeBaoLv ==TRUE || g_sanBaoLv ==TRUE)		nCount = 3;		//根据活动是否爆多少件装备
	else							nCount = 2;

	if(m_sEvent== 32000 || m_sEvent== 30007 )                    nCount = 8;
	if( m_sEvent == NPC_EVENT_MOP )				nCount = 16;		// 31000时掉16件物品
	if( m_sEvent == NPC_EVENT_GREATE_MOP )		nCount = 10;	// 竣胶府柯 殿 漂荐 阁胶磐老 版快 10俺甫 冻绢哆赴促.
//	}
/*
	for(int i = 0; i < nCount; i++)
	{
		iRandom = myrand(1, 30);

		if(iRandom < m_tItemPer)
		{

			iRandom = myrand(1, 10000);
			for(int i = 2; i < g_NpcItem.m_nField; i += 2)
			{			
//				iPer = g_NpcItem.m_ppItem[i][m_byClassLevel];
				iPer = g_NpcItem.m_ppItem[i][m_sHaveItem];
				if(iPer == 0) return;
				if(iRandom < iPer)
				{											// 快急 扁夯抛捞喉甫 曼炼窍扁困秦	
//					temp = g_NpcItem.m_ppItem[i-1][m_byClassLevel];
					temp = g_NpcItem.m_ppItem[i-1][m_sHaveItem];
					iVal = temp;
					if(temp >= g_arItemTable.GetSize()) return;

					if(g_arItemTable[temp]->m_byWear <= 5 || g_arItemTable[temp]->m_byWear == 117 || g_arItemTable[temp]->m_byWear == 20)	// 函屈抛捞喉捞促.
					{
						iVal = IsTransformedItem(g_arItemTable[temp]->m_sSid);
						if(iVal == -1) iVal = temp;//return;
					}
					if (iVal == 845 || iVal == 909 || iVal == 846 || iVal == 907 || iVal == 908) iVal = 847;
					GiveItemToMap(pCom, iVal, TRUE);
					break;
				}
			}

		}

		else if(iRandom < m_tDnPer)
		{
//			SYSTEMTIME gTime;										//@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Event Code 扁埃悼救 老矫利栏肺 疵妨霖促.
//			GetLocalTime(&gTime);

			iPer = g_arDNTable[m_byClassLevel]->m_sMinDn;
			iVal = g_arDNTable[m_byClassLevel]->m_sMaxDn;
			iRandom = myrand(iPer, iVal);
																	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Event Code 扁埃悼救 老矫利栏肺 疵妨霖促.	

			nDnHap += iRandom;
			//GiveItemToMap(pCom, iRandom, FALSE);
		}
	}
	if(nDnHap > 0 )
	{
      if (nDnHap == 845 || nDnHap == 909 || nDnHap == 846 || nDnHap == 907 || nDnHap == 908) nDnHap = 847;
		GiveItemToMap(pCom, nDnHap, FALSE);
	}
	*/
	int tItemHavePer;
	if(m_sPid>255)
		return;
	int tItemPer=g_DropItem[m_sPid][m_byColor].DropNovelity;                      //掉物品几率
	int tLeechdomPer = g_DropItem[m_sPid][m_byColor].DropLeechdom+tItemPer;      //掉蓝几率
	int tItemN=g_DropItem[m_sPid][m_byColor].n;                                 //物品种类数量
	int i;
	//TRACE("数量 %d\n",nCount);
	for(i = 0; i < nCount; i++)
	{
		//TRACE( "i=%d\n",i);
		iRandom = myrand(1, 100);
		//TRACE("随机数 %d\n",iRandom);
		if(iRandom < tItemPer)
		{
			//TRACE( "物品--\n");
			iRandom = myrand(1, 10000)%tItemN;
			tItemHavePer = g_DropItem[m_sPid][m_byColor].novelity[iRandom].per;
			temp = iVal = g_DropItem[m_sPid][m_byColor].novelity[iRandom].code1+g_DropItem[m_sPid][m_byColor].novelity[iRandom].code2*256;
			if(iVal >= g_arItemTable.GetSize()){
				return;
			}
			iRandom = myrand(1, 1000);
			if(iRandom < tItemHavePer){	
				if(g_arItemTable[temp]->m_byWear <= 5 || g_arItemTable[temp]->m_byWear == 117 || g_arItemTable[temp]->m_byWear == 20)	// 函屈抛捞喉捞促.
				{
					iVal = IsTransformedItem(g_arItemTable[temp]->m_sSid);
					if(iVal == -1) iVal = temp;
				}
				/*if (iVal == 987)
				{
					CString strMsg;
					strMsg.Format("玩家『 %s 』 打怪获得『 死亡使者 』!", pUser->m_strUserID);//公告提示
					pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
				}
				if (iVal == 1093)
				{
					CString strMsg;
					strMsg.Format("玩家『 %s 』 打怪获得『 冰魂 』!", pUser->m_strUserID);//公告提示
					pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
				}*/
			//	TRACE( "物品代码 %s,%d\n",g_arItemTable[iVal]->m_strName,iVal);
				GiveItemToMap(pCom, iVal, TRUE);
				
				//break;
			}
		}
		else if(iRandom < tLeechdomPer)//掉蓝药
		{
			//TRACE( "蓝药--\n");
			if(m_byClassLevel <30)
				iVal=31;
			else if(m_byClassLevel <70)
				iVal=32;
			else if(m_byClassLevel <120)
				iVal=33;
			else
				//iVal=227+256*3;
				iVal=33;
			GiveItemToMap(pCom, iVal, TRUE);
		}
		else{
			//TRACE( "钱--\n");
			//int money= g_DropItem[m_sPid][m_byColor].money;
			//iRandom =money+(money *myrand(0,15)%15);//15%浮动
			//nDnHap += iRandom;
			int money= g_DropItem[m_sPid][m_byColor].money;
			if (m_byClassLevel < 50)
			{
				iRandom  = money * 120 /100;
			}
			if ( m_byClassLevel >= 50 && m_byClassLevel < 70)
			{
				iRandom  = money;
			}
			else if ( m_byClassLevel >= 70 && m_byClassLevel < 90)
			{
				iRandom  = money * 80 / 100;
			}
			else
				iRandom  = money * 50 / 100;
			
			nDnHap += iRandom;

		}
		//TRACE( "i=%d\n",i);
	}
	//TRACE( "i=%d----------------------------------------\n",i);
	if(nDnHap > 0 )
	{
		if (nDnHap == 845 || nDnHap == 909 || nDnHap == 846 || nDnHap == 907 || nDnHap == 908) nDnHap = 847;
		GiveItemToMap(pCom, nDnHap, FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	酒捞袍吝 函屈酒捞袍捞 乐绰瘤 魄窜茄促.
//
int CNpc::IsTransformedItem(int sid)
{
	int iVal = 0;
	int i, j, iRandom;

	for(i = 0; i < g_ValItem.m_nRow; i++)
	{
		if(g_ValItem.m_ppItem[g_ValItem.m_nField-2][i] == sid)		// 盖 付瘤阜 鞘靛俊辑 酒捞袍 亲格(6俺 辆幅)
		{
			iRandom = myrand(1, 100);
			for(j = 2; j < g_ValItem.m_nField; j+= 2)
			{
				iVal = g_ValItem.m_ppItem[j][i];
				if(iRandom < iVal)
				{
					if(sid >= g_arItemTable.GetSize()) return -1;
					else return g_ValItem.m_ppItem[j-1][i];
				}
			}
		}
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////////
//	酒捞袍吝 概流 酒捞袍阑 冻绢靛副荐乐绰瘤 魄窜
//怪物掉落物品蓝黄率
int CNpc::IsMagicItem(COM* pCom, ItemList *pItem, int iTable)
{
	/*int i = 0, j;
	int iMagicTemp = 1 , iRareTemp = 1;
	int iRandom = myrand( 1, 10000 );

	int iMagicCount = 0, iCount = 0;
	int nLoop = 0, iType = 0;
	int nEventMoon = 0;
	int nEventSongpeon = 0;
	int nEventBox = 0;
	int iMagicUp=0;
//	int pingfen=0;  //评分

    USER* pUser = NULL;

	/*SYSTEMTIME gTime;										//@@@@@@@@@@@@@@@ Event Code 扁埃悼救 老矫利栏肺 疵妨霖促.
	GetLocalTime(&gTime);
	if(gTime.wMonth < 7) { iMagicTemp = 10; iRareTemp = 2; }
	else if(gTime.wMonth == 7) 
	{ 
		if(gTime.wDay <= 7) { iMagicTemp = 10; iRareTemp = 2; } 
	}
	*/
/*	if(m_sEvent== 32000){
		iMagicTemp=0;
		iRareTemp = 3; 
	}else*/
	/*if(m_sEvent == NPC_EVENT_MOP )				// 泅犁 候遣,凝,胶妮乔坷
	{
		iMagicTemp = 0; 
		iRareTemp = 4; 
	}
	else if(m_sEvent == NPC_EVENT_GREATE_MOP)	// 竣胶府柯
	{
		iMagicTemp = 0;  //70
		iRareTemp = 8;    //75
	}
	
	else if(m_sEvent == 31000)	// 竣胶府柯
	{
		iMagicTemp = 0;  //70
		iRareTemp = 10000;    //75
	}


	/*if(m_lKillUid >= 0)
	{
		pUser = GetUser(pCom, m_lKillUid);
		if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
		{
			if(pUser->m_dwMagicFindTime != 0)	// 概流 颇牢歹甫 冈篮 惑怕捞搁
			{
					iMagicTemp=0;
					iMagicUp=10000;
			}
		}
	}*/
	/*if(m_lKillUid >= 0)   
	{
		pUser = GetUser(pCom, m_lKillUid);
		if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
		{
			if(pUser->m_dwMagicFindTime != 0 || pUser->m_isDoubleBAOLV != 0 || g_sanBaoLv == TRUE)
			{
				iMagicTemp *= 7; //o_yehuoini[0]->xyl;  //幸运时掉蓝装备
				iRareTemp *= 15; //o_yehuoini[0]->xyh;    //幸运时掉金装备
				    
			}
			else if(pUser->m_dwMagicFindTime == 0 || pUser->m_isDoubleBAOLV == 0)
			{
				iMagicTemp *= 4; //o_yehuoini[0]->lan;  //没有幸运时掉蓝装备
				iRareTemp *= 1; //o_yehuoini[0]->huang;    //没有幸运时掉金装备
			}
            
		}
	}
			/*if(pUser->m_dwMagicFindTime != 0)	// 概流 颇牢歹甫 冈篮 惑怕捞搁
			{
					iMagicTemp=0;
					iMagicUp=10000;
			}*/
		
	

	/*nEventMoon		= NPC_RARE_ITEM * iRareTemp + (NPC_EVENT_MOON - NPC_RARE_ITEM);
	nEventSongpeon	= nEventMoon + (NPC_EVENT_SONGPEON - NPC_EVENT_MOON);
	nEventBox		= nEventSongpeon + (NPC_EVENT_BOX - NPC_EVENT_SONGPEON);

    if(iRandom <= NPC_MAGIC_ITEM * iMagicTemp)	// 概流
	{
		nLoop = 2;
		iType = MAGIC_ITEM;
	}
	else if((iRandom > NPC_MAGIC_ITEM * iMagicTemp) && (iRandom <=( NPC_RARE_ITEM * iRareTemp+iMagicUp)))	
	{ 
		nLoop = 4;
		iType = RARE_ITEM; 
	}	
	else if(0 && iRandom > NPC_RARE_ITEM * iRareTemp && iRandom <= nEventMoon)
	{
		return EVENT_ITEM_MOON;
	}
	else if(0 && iRandom > nEventMoon && iRandom <= nEventSongpeon)
	{
		return EVENT_ITEM_SONGPEON;
	}
	else if(0 && iRandom > nEventSongpeon && iRandom <= nEventBox)
	{
		return EVENT_ITEM_BOX;
	}
	else return NORMAL_ITEM;								// 老馆酒捞袍

	int iTemp = 0;

	if(m_ItemUserLevel <= 20)       iMagicCount = 205;	//42;	// 掉的物品属性控制    //neo版本
	else if(m_ItemUserLevel <= 40)  iMagicCount = 205;  //106
	else if(m_ItemUserLevel <= 60)  iMagicCount = 205;  //143
	else							iMagicCount = 205;    //数值越大,掉的属性越好

	

	if(iMagicCount >= g_arMagicItemTable.GetSize()) iMagicCount = g_arMagicItemTable.GetSize() - 1;

	while(nLoop > i)										// 公炼扒 4俺加己蔼阑 备茄促.
	{
		
		iRandom = myrand(0, iMagicCount);
		//------------------------------------------------------------------------------------------------
		//yskang 0.6 蜡丰荤侩磊俊霸 橇府固决 亮篮 可记狼 酒捞袍捞 冻绢柳促.
		if(pUser != NULL)
		{
			if(pUser->m_iDisplayType !=5 && pUser->m_iDisplayType !=6)//蜡丰 荤侩磊捞促.
			{
				if(m_ItemUserLevel <= 20)
					iRandom = myrand(0, iMagicCount); //亮篮 酒捞袍捞 冻绢龙 犬伏阑 臭牢促.
				else
					iRandom = myrand(41, iMagicCount); 
			}
		}
		//------------------------------------------------------------------------------------------------

		if(!g_arMagicItemTable[iRandom]->m_tUse) continue;

		if(CheckClassItem(iTable, iRandom) == FALSE) 
		{
			if(i == 0) continue;							// 概流篮 扁夯捞 1俺
//			else if(iType == RARE_ITEM && i <= 2) continue;	// 限制黄色物品属性必须为2条以上
			else if(iType == RARE_ITEM && i <= 3) continue;	// 限制黄色物品属性必须为3条以上
			else { i++; continue; }
		}*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int i = 0, j;
	int iMagicTemp = 1 , iRareTemp = 1;
	int iRandom = myrand( 1, 10000 );

	if(pItem == NULL) return 0;

	int pai = 2;

	int iMagicCount = 0, iCount = 0;
	int nLoop = 0, iType = 0;
	int nEventMoon = 0;
	int nEventSongpeon = 0;
	int nEventBox = 0;
	int iMagicUp=0;

	USER* pUser = NULL;

	SYSTEMTIME gTime;								
	GetLocalTime(&gTime);


	if(m_lKillUid >= 0)
	{
		pUser = GetUser(pCom, m_lKillUid);
		int random = myrand(0, 10000);

		int random2 = myrand(0,10000);
		if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
		{
			if(pUser->m_dwMagicFindTime > 0 || pUser->m_isDoubleBAOLV == TRUE || g_sanBaoLv == TRUE)	//幸运 捐款 或者服务器三倍都只爆黄
			{	
				if(random <= 6500)		//5% 黄金
				{
					if(random2 <= 1)		//3排
					{
						nLoop = 3;
						iType = RARE_ITEM;
					}
					else
					{
						nLoop = 4;
						iType = RARE_ITEM;

					}
				}
				else if( random > 3500 && random <= 6500) //25% 蓝色
				{
					if(random2 <= 5000)		//1排
					{
						nLoop = 1;
						iType = MAGIC_ITEM;
					}
					else
					{
						nLoop = 2;
						iType = MAGIC_ITEM;
					}
				}
				else
					iType = NORMAL_ITEM;
			}
			else									//不是幸运时
			{
				if(random <= 400)		//1% 黄金
				{
					if(random2 <= 3000)		//3排
					{
						nLoop = 3;
						iType = RARE_ITEM;
					}
					else
					{
						nLoop = 4;
						iType = RARE_ITEM;
					}

				}
				else if( random > 400 && random <= 600) //25% 蓝色
				{
					if(random2 <= 5000)		//1排
					{
						nLoop = 1;
						iType = MAGIC_ITEM;
					}
					else
					{
						nLoop = 2;
						iType = MAGIC_ITEM;
					}

				}
				else
					iType = NORMAL_ITEM;

			}
		}
		else
		{
			iType = NORMAL_ITEM;
		}
	}
	else 
	{
		iType = NORMAL_ITEM;
	}

	if(m_sEvent >= 30000)
	{
		nLoop = 4;
		iType = RARE_ITEM;
			
	}


	int iTemp = 0;

	if(m_ItemUserLevel <= 20)       iMagicCount = 205;		// 阳光删除垃圾属性主意的地方
	else if(m_ItemUserLevel <= 40)  iMagicCount = 205;
	else if(m_ItemUserLevel <= 60)  iMagicCount = 205;
	else							iMagicCount = 205;



	if(iMagicCount >= g_arMagicItemTable.GetSize()) 
		iMagicCount = g_arMagicItemTable.GetSize() - 1;

	while(nLoop > i)										// 公炼扒 4俺加己蔼阑 备茄促.
	{

		iRandom = myrand(0, iMagicCount);

		if(pUser != NULL && pUser->m_state == STATE_GAMESTARTED)
		{
			
			if(m_ItemUserLevel <= 20)
				iRandom = myrand(0, iMagicCount); 
			else
				iRandom = myrand(0, iMagicCount); 
		}

 		if(iRandom == 107 || iRandom == 109 || iRandom == 110 || iRandom == 128 ||iRandom == 141 || iRandom == 135 || iRandom == 136 || iRandom == 138 || iRandom == 139)		
 		{
				int random = myrand(0, 10000);
				if (random > 3000)
					continue; 		
		}
		if(!g_arMagicItemTable[iRandom]->m_tUse) continue;

	
		if(CheckClassItem(iTable, iRandom) == FALSE) 
		{
			if(i == 0) continue;							
			else if(iType == RARE_ITEM && i <= 3) continue;	// 如果是金装备.属性必须4条 4排 <=3 是4排
			else { i++; continue; }
		}
		for(j = 0; j < 4; j++)
		{
			if(o_yehuoini[0]->chongdie == 0)
		  {
			  if (pItem->tMagic[j] < 0 || pItem->tMagic[j] >= iMagicCount)
				continue;
			iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType;//限制物品不出重复属性代码
		  }
			if(iCount != 0 && iCount == g_arMagicItemTable[iRandom]->m_sSubType)	// 加己捞 般磨荐 乐栏骨肺 捞吝 奴蔼父 急琶	
			{
				iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sChangeValue;
				if(iCount < g_arMagicItemTable[iRandom]->m_sChangeValue)
				{
					iTemp = g_arMagicItemTable[pItem->tMagic[j]]->m_tLevel;
					if(pItem->sLevel - iTemp > 0) pItem->sLevel -= iTemp;
					pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
					pItem->tMagic[j] = iRandom; 

					if(g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType == MAGIC_DURATION_UP)
					{
						iTemp = g_arMagicItemTable[pItem->tMagic[j]]->m_sChangeValue;
						if(pItem->sDuration - iTemp > 0) pItem->sDuration -= iTemp;
						pItem->sDuration += g_arMagicItemTable[iRandom]->m_sChangeValue; // 郴备仿 刘啊 加己篮 悸泼瞪锭 
					}
					break;
				}
				else if(iCount == g_arMagicItemTable[iRandom]->m_sChangeValue) break;
			 }

			if(pItem->tMagic[j] > 0) continue;	// 捞固 浇吩俊 蔼捞 乐栏搁 逞绢皑
			else
			{ 
				pItem->tMagic[j] = iRandom; i++; 
				if(g_arMagicItemTable[iRandom]->m_tLevel > 0) pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
				if(g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType == MAGIC_DURATION_UP)
				{
					pItem->sDuration += g_arMagicItemTable[iRandom]->m_sChangeValue; // 郴备仿 刘啊 加己篮 悸泼瞪锭 
				}
				
			/*	int bianhao = pItem->sSid;
				int shuxing = pItem->tMagic[j];
				int tClass = g_arItemTable[bianhao]->m_byWear;
				if ( tClass == 1 )//检测类型为武器给评分
				{
					//属性 128 损伤25     138命中10    93攻击速度3    145所有技术2   113兴奋2   117法术2   120穿刺2    125连射2
					if (shuxing == 138 || shuxing == 128 || shuxing == 93 || shuxing == 145|| shuxing == 113 || shuxing == 117 || shuxing == 120 || shuxing == 125)
					{
						pingfen += 50;
					}
					//属性 95命中9%  
					if (shuxing == 95 )
					{
						pingfen += 45;
					}
					//属性  91损伤20   94命中8   72攻击速度2  
					if (shuxing == 91 || shuxing == 94 || shuxing == 72 )
					{
						pingfen += 40;
					}
					//属性 75命中 7
					if (shuxing == 74 )
					{
						pingfen += 35;
					}
					//属性 64损伤15  73命中6  137所有技术等级1   49兴奋技术等级1    53法术大师等级1    56穿刺技术等级1   61连射技术等级1
					if (shuxing == 64 || shuxing == 73 || shuxing == 137 || shuxing == 49 || shuxing == 53 || shuxing == 56 || shuxing == 61)
					{
						pingfen += 30;
					}
					//属性   31命中5
					if (shuxing == 31 )
					{
						pingfen += 25;
					}
				}
				else if ( tClass != 1 )//检测类型装备给评分
				{
					//属性 145所有2  128损伤25    141HP50  135抗魔25  139回避10   107力量5  110智慧5   109敏捷5   130防御10   
					if (shuxing == 145 || shuxing == 128|| shuxing == 141 || shuxing == 135 || shuxing == 139 || shuxing == 107 || shuxing == 110 || shuxing == 109 || shuxing == 130)
					{
						pingfen += 50;
					}
					//属性  97回避9    129防御9增加  
					if (shuxing == 97 || shuxing == 129 )
					{
						pingfen += 45;
					}
					//属性 100防御8、132HP40  105抗魔20  96回避8   86力量4  88敏捷4   89智慧4
					if (shuxing == 100 || shuxing == 132 || shuxing == 105 || shuxing == 96 || shuxing == 86 || shuxing == 88 || shuxing == 89 )
					{
						pingfen += 40;
					}
					//属性 76回避7    99防御7
					if (shuxing == 76 || shuxing == 99 )
					{
						pingfen += 35;
					}
					//属性  75回避6  102HP30   84抗魔15  75回避6   43力量3 46智慧3  45敏捷3
					if (shuxing == 75 || shuxing == 102 || shuxing == 84|| shuxing == 75 || shuxing == 43 || shuxing == 46 || shuxing == 45 )
					{
						pingfen += 30;
					}
					//属性   33回避5   78防御5
					if (shuxing == 33|| shuxing == 78 )
					{
						pingfen += 25;
					}
					
				}

				break; 
			}
		}

	}

	int bianhao2 = pItem->sSid;
	int tClass = g_arItemTable[bianhao2]->m_byWear;
	if ( tClass == 1 && pingfen >10 )//物品为武器 评分大于0 数值自己改
	{
		CString strMsg;
		CString GoodsMagic;
		for(int i = 0; i < 4; i++)
		{
			if(pItem->tMagic[i] != 0)
			{
				GoodsMagic+=g_MagicArray[pItem->tMagic[i]];
				GoodsMagic+=",";
			}
		}
                                 /*评分[ %d ]*/                    /* ,pingfen*/
      /* strMsg.Format("玩家:『 %s 』 打到『 %s 』属性:", pUser->m_strUserID,g_arItemTable[bianhao2]->m_strName );//公告提示
		strMsg+=GoodsMagic;
		pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
	}
	else if ( tClass != 1 && pingfen > 10 )//物品为装备 评分大于0 数值自己改
	{
		CString strMsg;
		CString GoodsMagic;
		//ZeroMemory(GoodsMagic,0);
		for(int i = 0; i < 4; i++)
		{
			if(pItem->tMagic[i] != 0)
			{
				GoodsMagic+=g_MagicArray[pItem->tMagic[i]];
				GoodsMagic+=",";
			}
		}
		 strMsg.Format("玩家:『 %s 』 打到『 %s 』属性:", pUser->m_strUserID,g_arItemTable[bianhao2]->m_strName );//公告提示
		strMsg+=GoodsMagic;
		pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
	}*/


	
				break; 
			}
		}

	}

	return iType;
}
//随机守护属性
void CNpc::shouhu_rand(	ItemList *pMapItem)
{
	int a;

	a=myrand(0,15)%100;
	pMapItem->tIQ=0x09;//守护颜色
	switch (a){
		case 0:pMapItem->tMagic[0]=1; return;//损伤2 3 5
		case 1:pMapItem->tMagic[0]=4; return;//武器等级增加1 2 3
		case 2:pMapItem->tMagic[0]=7; return;//将对方生命2 5 10转化成自己生命
		case 3:pMapItem->tMagic[0]=10; return;//对周围造成2% 5 10伤害
		case 4:pMapItem->tMagic[0]=13; return;//13 防御增加3 6 10
		case 5:pMapItem->tMagic[0]=16; return;//魔法防御增加3 6 10
		case 6:pMapItem->tMagic[0]=19; return;//将敌人对自己攻击伤害2% 3 5 反射到敌人身上
		case 7:pMapItem->tMagic[0]=22; return;// 打怪经验值提高2% 3% 5%
		case 8:pMapItem->tMagic[0]=25; return;//金币提高10% 金币提高20% 金币提高50%
		case 9:pMapItem->tMagic[0]=28; return;// 所有技能增加1  所有技能增加2  所有技能增加3
		case 10:pMapItem->tMagic[0]=31; return;//掉落物品提高2% 掉落物品提高3% 掉落物品提高5%
		case 11:pMapItem->tMagic[0]=34; return;//攻击力增加10 攻击力增加20 攻击力增加30
		case 12:pMapItem->tMagic[0]=37; return;//吸取对方生命10 吸取对方生命20 吸取对方生命30
		case 13:pMapItem->tMagic[0]=40; return;//生命增加10 生命增加20 生命增加30
		case 14:pMapItem->tMagic[0]=1; return;//损伤2 3 5
		case 15:pMapItem->tMagic[0]=13; return;//13 防御增加3 6 10
		case 16:pMapItem->tMagic[0]=1; return;//损伤2 3 5
		case 17:pMapItem->tMagic[0]=34; return;//攻击力增加10 攻击力增加20 攻击力增加30
		case 18:pMapItem->tMagic[0]=13; return;//13 防御增加3 6 10
		case 19:pMapItem->tMagic[0]=1; return;//损伤2 3 5
		case 20:pMapItem->tMagic[0]=40; return;//生命增加10 生命增加20 生命增加30
		case 21:pMapItem->tMagic[0]=1; return;//损伤2 3 5
		case 22:pMapItem->tMagic[0]=13; return;//13 防御增加3 6 10
		case 23:pMapItem->tMagic[0]=40; return;//生命增加10 生命增加20 生命增加30
		case 24:pMapItem->tMagic[0]=40; return;//生命增加10 生命增加20 生命增加30
		case 25:pMapItem->tMagic[0]=1; return;//损伤2 3 5
		case 26:pMapItem->tMagic[0]=13; return;//13 防御增加3 6 10
		case 27:pMapItem->tMagic[0]=1; return;//损伤2 3 5
		case 28:pMapItem->tMagic[0]=40; return;//生命增加10 生命增加20 生命增加30
		case 29:pMapItem->tMagic[0]=34; return;//攻击力增加10 攻击力增加20 攻击力增加30
		case 30:pMapItem->tMagic[0]=1; return;//损伤2 3 5
		default:
			pMapItem->tMagic[0]=0;return;
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	NPC Item阑 甘俊 冻焙促.
//
void CNpc::GiveItemToMap(COM *pCom, int iItemNum, BOOL bItem, int iEventNum)
{
	int i, iRandom = 0;
	int iType = 0;
	BYTE tEBodySid = 0;

	CPoint pt;
	pt = FindNearRandomPointForItem(m_sCurX, m_sCurY);							// 泅犁 磊扁谅钎甫 力寇茄 24沫
	if(pt.x <= -1 || pt.y <= -1) return;
	if(pt.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pt.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return;

	ItemList *pMapItem = NULL;
	if(InterlockedCompareExchange((long*)&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, (long)1, (long)0) == (long)0)
	{
		pMapItem = new ItemList;

		if(!bItem)					// 捣老 版快
		{
			pMapItem->tType = TYPE_MONEY;
			pMapItem->dwMoney = iItemNum;

			pMapItem->uid[0] = m_iHaveItemUid[0].uid;
			pMapItem->uid[1] = m_iHaveItemUid[1].uid;
			pMapItem->uid[2] = m_iHaveItemUid[2].uid;

			pMapItem->SuccessRate[0] = (BYTE)m_iHaveItemUid[0].nDamage;
			pMapItem->SuccessRate[1] = (BYTE)m_iHaveItemUid[1].nDamage;
			pMapItem->SuccessRate[2] = (BYTE)m_iHaveItemUid[2].nDamage;
			pMapItem->dwTime = GetItemThrowTime();
		}
		else							// 酒捞袍 格废老版快
		{
			if(iItemNum >= g_arItemTable.GetSize())
			{
				if(pMapItem) delete pMapItem;
				return; 
			}
			else
			{
				pMapItem->tType = TYPE_ITEM;
				pMapItem->sLevel = g_arItemTable[iItemNum]->m_byRLevel;
				pMapItem->sSid = g_arItemTable[iItemNum]->m_sSid;
				pMapItem->sDuration = g_arItemTable[iItemNum]->m_sDuration;
				pMapItem->sCount = 1;
				pMapItem->sBullNum = g_arItemTable[iItemNum]->m_sBullNum;
				for(i = 0; i < MAGIC_NUM; i++) pMapItem->tMagic[i] = 0;	// 唱吝俊 Magic Item 眠啊且 巴
				pMapItem->tIQ = NORMAL_ITEM;
				pMapItem->iItemSerial = 0;

				pMapItem->uid[0] = m_iHaveItemUid[0].uid;				// 快急 鉴困
				pMapItem->uid[1] = m_iHaveItemUid[1].uid;
				pMapItem->uid[2] = m_iHaveItemUid[2].uid;

				pMapItem->SuccessRate[0] = (BYTE)m_iHaveItemUid[0].nDamage;	// 快急 鉴困 厚啦
				pMapItem->SuccessRate[1] = (BYTE)m_iHaveItemUid[1].nDamage;
				pMapItem->SuccessRate[2] = (BYTE)m_iHaveItemUid[2].nDamage;

				pMapItem->dwTime = GetItemThrowTime();
				//GetLocalTime(&pMapItem->ThrowTime);
			
				int iWear = g_arItemTable[iItemNum]->m_byWear;

				if(iWear >= 1 && iWear <= 5) 
				{
					iType = IsMagicItem(pCom, pMapItem, iItemNum);
					if(iType == MAGIC_ITEM)
					{
						pMapItem->tIQ = MAGIC_ITEM;	// 概流父 贸府...
					}
					else if(iType == RARE_ITEM)
					{
						pMapItem->tIQ = RARE_ITEM;	// 贰绢父 贸府...

// 						int n = pMapItem->tMagic[0] + pMapItem->tMagic[1] + pMapItem->tMagic[2] + pMapItem->tMagic[3];
// 						if(n > 500) {
// 							int iRandom = myrand(1, 100);
// 							//极品掉落控制
// 							if(iRandom < 30){
// 								pMapItem->tMagic[0] = 0;
// 							}
// 						}
						
					}
					else if(iType == EVENT_ITEM_MOON)	// 焊抚崔
					{
						iItemNum = EVENTITEM_SID_MOON;
						pMapItem->sLevel = g_arItemTable[iItemNum]->m_byRLevel;
						pMapItem->sSid = g_arItemTable[iItemNum]->m_sSid;
						pMapItem->sDuration = g_arItemTable[iItemNum]->m_sDuration;
						pMapItem->sCount = 1;
						pMapItem->sBullNum = g_arItemTable[iItemNum]->m_sBullNum;
						iWear = g_arItemTable[iItemNum]->m_byWear;
					}
					else if(iType == EVENT_ITEM_SONGPEON)	// 价祈 肚绰 岿捍
					{
						/*
						if(m_byClassLevel < 11) iItemNum = EVENTITEM_SID_SONGPEON_01;
						else if(m_byClassLevel >= 11 && m_byClassLevel < 31) iItemNum = EVENTITEM_SID_SONGPEON_11;
						else if(m_byClassLevel >= 31 && m_byClassLevel < 51) iItemNum = EVENTITEM_SID_SONGPEON_31;
						else if(m_byClassLevel >= 51 && m_byClassLevel < 71) iItemNum = EVENTITEM_SID_SONGPEON_51;
						else if(m_byClassLevel >= 71) iItemNum = EVENTITEM_SID_SONGPEON_71;
						*/
						iItemNum = EVENTITEM_SID_SONGPEON_01;
						
						pMapItem->sLevel = g_arItemTable[iItemNum]->m_byRLevel;
						pMapItem->sSid = g_arItemTable[iItemNum]->m_sSid;
						pMapItem->sDuration = g_arItemTable[iItemNum]->m_sDuration;
						pMapItem->sCount = 1;
						pMapItem->sBullNum = g_arItemTable[iItemNum]->m_sBullNum;
						iWear = g_arItemTable[iItemNum]->m_byWear;
					}
					else if(iType == EVENT_ITEM_BOX)	// 急拱惑磊
					{
						iItemNum = EVENTITEM_SID_BOX;
						pMapItem->sLevel = g_arItemTable[iItemNum]->m_byRLevel;
						pMapItem->sSid = g_arItemTable[iItemNum]->m_sSid;
						pMapItem->sDuration = g_arItemTable[iItemNum]->m_sDuration;
						pMapItem->sCount = 1;
						pMapItem->sBullNum = g_arItemTable[iItemNum]->m_sBullNum;
						iWear = g_arItemTable[iItemNum]->m_byWear;
					}
				}
				////////////////////////////////////////////////////////////////////////月神套
				else if(iItemNum==818 || iItemNum==733 || iItemNum==735 ){
					byte tMagic=0;
					iRandom = myrand(1, 11);
					switch(iRandom){
						case 1: tMagic=78;break;//防御5
						case 2: tMagic=107;break;//力量5
						case 3: tMagic=108;break;//体质5
						case 4: tMagic=109;break;//敏捷5
						case 5: tMagic=110;break;//智慧5
                        case 6: tMagic=111;break;//智力5
						case 7: tMagic=33;break;//回避5
						case 8: tMagic=14;break;//魔法损伤5
						case 9: tMagic=6;break;//损伤5
						case 10: tMagic=31;break;//命中5
					    case 11: tMagic=137;break;//所有1
					    default: tMagic=0;break;
					}
					pMapItem->tIQ = SET_ITEM;
					pMapItem->tMagic[0] =tMagic;
				}
				else if(iItemNum==991 ){
					//byte tMagic=0;
					
					pMapItem->tIQ = UNIQUE_ITEM;
					//pMapItem->tMagic[0] =tMagic;
				}
				 //////////////////////////////////////////////////////////////////////////////
				// 厩技荤府 贸府
				else if(iWear >= 6 && iWear <= 8)	// 首饰掉落
				{
					pMapItem->tIQ = MAGIC_ITEM;	// 厩技荤府绰 公炼扒 概流
					pMapItem->tMagic[0] = g_arItemTable[iItemNum]->m_bySpecial;
				}
				
				// 俊捞歹 贸府
				else if(iWear == 126) //浩石掉落....和属性随机.
				{
					iRandom = myrand(1, 1040);
					for(i = 0; i < g_arEBodyTable.GetSize(); i++)
					{
						if(iRandom <= g_arEBodyTable[i]->m_sRandom) 
						{
							tEBodySid = g_arEBodyTable[i]->m_tSid;
							break;
						}	
					}
					pMapItem->tIQ = MAGIC_ITEM;	//
					pMapItem->tMagic[0] = tEBodySid;
				}else if(iWear==130)
				{
					shouhu_rand(pMapItem);

				}else if(iWear==143){//幻石
					byte tMagic=0;
					iRandom = myrand(1, 6);
					switch(iRandom){
						case 1: tMagic=3;break;
						case 2: tMagic=13;break;
						case 3: tMagic=23;break;
						case 4: tMagic=43;break;
						case 5: tMagic=53;break;
						case 6: tMagic=63;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
					pMapItem->tMagic[5]= 1;

				}
				else if(iItemNum == 1051)//芯片
 				{
					byte tMagic = 0;
 					iRandom = myrand(1,3);
					switch(iRandom)
					{
						case 1: tMagic = 5;break;
						case 2: tMagic = 6;break;
						case 3: tMagic = 12;break;
					}
 					pMapItem->tIQ = MAGIC_ITEM;
 					pMapItem->tMagic[0] = tMagic;
 				}
				else if(iItemNum==1287){//炼狱球
					byte tMagic=0;
					iRandom = myrand(1, 8);
					switch(iRandom){
						case 1: tMagic=162;break;//15力
						case 2: tMagic=156;break;//15体质
						case 3: tMagic=158;break;//15敏捷
						case 4: tMagic=164;break;//15智慧
						case 5: tMagic=166;break;//15智力
						case 6: tMagic=190;break;//
						case 7: tMagic=193;break;//150/50抗血
						case 8: tMagic=194;break;//50防
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;
				
				}else if(iWear==166){//程序卡掉落属性随机
					byte tMagic=0;
					iRandom = myrand(1, 44);
					switch(iRandom){
						case 1: tMagic=1;break;
						case 2: tMagic=2;break;
						case 3: tMagic=3;break;
						case 4: tMagic=4;break;
						case 5: tMagic=5;break;
						case 6: tMagic=6;break;
						case 7: tMagic=7;break;
						case 8: tMagic=8;break;
						case 9: tMagic=9;break;
						case 10: tMagic=10;break;
						case 11: tMagic=11;break;
						case 12: tMagic=12;break;
						case 13: tMagic=13;break;
						case 14: tMagic=14;break;
						case 15: tMagic=15;break;
						case 16: tMagic=16;break;
						case 17: tMagic=17;break;
						case 18: tMagic=18;break;
						case 19: tMagic=19;break;
						case 20: tMagic=20;break;
						case 21: tMagic=21;break;
						case 22: tMagic=22;break;
						case 23: tMagic=23;break;
						case 24: tMagic=24;break;
						case 25: tMagic=25;break;
						case 26: tMagic=26;break;
						case 27: tMagic=27;break;
						case 28: tMagic=38;break;
						case 29: tMagic=29;break;
						case 30: tMagic=30;break;
						case 31: tMagic=31;break;
						case 32: tMagic=32;break;
						case 33: tMagic=33;break;
						case 34: tMagic=34;break;
						case 35: tMagic=35;break;
						case 36: tMagic=36;break;
						case 37: tMagic=37;break;
						case 38: tMagic=38;break;
						case 39: tMagic=39;break;
						case 40: tMagic=40;break;
						case 41: tMagic=41;break;
						case 42: tMagic=42;break;
						case 43: tMagic=43;break;
						case 44: tMagic=44;break;
						default: tMagic=0;break;
					}
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =tMagic;}
				                             
				else if(iItemNum== 1175) //晶片
 				{
 					pMapItem->tIQ = MAGIC_ITEM;
 					pMapItem->tMagic[0] = 2;
 				}
 				else if(iItemNum == 1176) //库丁
 				{
 					pMapItem->tIQ = MAGIC_ITEM;
 					pMapItem->tMagic[0] = 3;
 				}
				else if(iItemNum == 1174) //仙瑟
 				{
 					pMapItem->tIQ = MAGIC_ITEM;
 					pMapItem->tMagic[0] = 1;
 				}
				else if(iItemNum == 1177) //V3附件
 				{
 					pMapItem->tIQ = MAGIC_ITEM;
 					pMapItem->tMagic[0] = 7;
 				}
				  
				else if(iItemNum==1178){   //导弹固定属性
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =4;
				
				}else if(iItemNum==1179){  //激光固定属性
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =5;
				
				}else if(iItemNum==987){
					pMapItem->tIQ = MAGIC_ITEM;	
					pMapItem->tMagic[0] =59;

				}
			}
		}		

		// 秦寸 蜡历俊霸 舅赴促.			
	//	pCom->DelThrowItem();
		pCom->SetThrowItem( pMapItem, pt.x, pt.y, m_ZoneIndex );

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, 0);
	}
}

//////////////////////////////////////////////////////////////////////////////////
//	泅犁 困摹甫 吝缴栏肺 25 伎吝 酒捞袍甫 冻绢龙荐乐绰 谅钎甫 伐待栏肺 积己
//
CPoint CNpc::FindNearRandomPoint(int x, int y)
{
	CPoint t;
	int i;
	int iX, iY;
	int rand_x = 1, rand_y = 1;

	MAP *pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return CPoint(-1, -1);
	if( !pMap->m_pMap ) return CPoint(-1, -1);

	int dir[25][2];

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

	rand_x = myrand(1, 8, TRUE);
	rand_y = myrand(0, 1, TRUE);

	iX = dir[rand_x][rand_y] + x;
	iY = dir[rand_x][rand_y] + y;

	rand_x = iX; rand_y = iY;
		
	if(rand_x >= pMap->m_sizeMap.cx || rand_x < 0 || rand_y >= pMap->m_sizeMap.cy || rand_y < 0) return CPoint(-1, -1);

	if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].m_lUser == 0)
	{
		if( IsMovable( rand_x, rand_y ) )
		{
//			if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].iIndex == -1) return CPoint( rand_x, rand_y );
			return CPoint( rand_x, rand_y );
		}
	}

	rand_x = x, rand_y = y;

	for( i = 1; i < 25; i++)
	{
		iX = rand_x + dir[i][0];
		iY = rand_y + dir[i][1];

		if( iX >= pMap->m_sizeMap.cx || iX < 0 || iY >= pMap->m_sizeMap.cy || iY < 0) continue;

		if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].m_lUser != 0) continue;	// 后 甘牢瘤 犬牢茄促.

		if( IsMovable( iX, iY ) )
		{
//			if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].iIndex == -1) return CPoint( iX, iY );
			return CPoint( iX, iY );
		}
	}

	return CPoint(-1, -1);
}

CPoint CNpc::FindNearRandomPointForItem(int x, int y)
{
	CPoint t;
	int i;
	int iX, iY;
	int rand_x = 1, rand_y = 1;

	MAP *pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return CPoint(-1, -1);
	if( !pMap->m_pMap ) return CPoint(-1, -1);

	int dir[25][2];

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

	rand_x = myrand(1, 8, TRUE);
	rand_y = myrand(0, 1, TRUE);

	iX = dir[rand_x][rand_y] + x;
	iY = dir[rand_x][rand_y] + y;

	rand_x = iX; rand_y = iY;
		
	if(rand_x >= pMap->m_sizeMap.cx || rand_x < 0 || rand_y >= pMap->m_sizeMap.cy || rand_y < 0) return CPoint(-1, -1);

//	if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].m_lUser == 0)
//	{
//		if( IsMovable( rand_x, rand_y ) )
//		{
//			if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].iIndex == -1) return CPoint( rand_x, rand_y );
//		}
//	}

	if( g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].m_FieldUse == 0 )
	{
		if( g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].iIndex == -1 ) return CPoint( rand_x, rand_y );
	}

	rand_x = x, rand_y = y;

	for( i = 1; i < 25; i++)
	{
		iX = rand_x + dir[i][0];
		iY = rand_y + dir[i][1];

		if( iX >= pMap->m_sizeMap.cx || iX < 0 || iY >= pMap->m_sizeMap.cy || iY < 0) continue;

//		if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].m_lUser != 0) continue;	// 后 甘牢瘤 犬牢茄促.
		if( g_zone[m_ZoneIndex]->m_pMap[iX][iY].m_FieldUse != 0 ) continue;	// 荤侩吝牢瘤 眉农.

//		if( IsMovable( iX, iY ) )
//		{
//			if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].iIndex == -1) return CPoint( iX, iY );
//		}
		if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].iIndex == -1) return CPoint( iX, iY );
	}

	return CPoint(-1, -1);
}

///////////////////////////////////////////////////////////////////////////////////
//	x, y 啊 框流老 荐 乐绰 谅钎牢瘤 魄窜
//
BOOL CNpc::IsMovable(int x, int y)
{
	if(x < 0 || y < 0 ) return FALSE;

	if(!g_zone[m_ZoneIndex] ) return FALSE;
	if(!g_zone[m_ZoneIndex]->m_pMap) return FALSE;
	if(x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return FALSE;
	if(g_zone[m_ZoneIndex]->m_pMap[x][y].m_bMove || g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser) return FALSE;

	return TRUE;
}
			
//////////////////////////////////////////////////////////////////////////////////////////////
//	NPC 傍拜 己傍
//
void CNpc::SendAttackSuccess(COM *pCom, int tuid, BOOL bIsCritical, short sHP, short sMaxHP)
{
	if(pCom == NULL) return;

	CBufferEx TempBuf;
//	CByteArray arAction1;
//	CByteArray arAction2;
	
	TempBuf.Add(ATTACK_RESULT);
	//--------------------------------------------------
	//yskang 0.3 NPC绰 农府萍拿 傍拜捞 绝促.
	//--------------------------------------------------
	TempBuf.Add(ATTACK_SUCCESS);
	TempBuf.Add((int)(m_sNid + NPC_BAND));
	TempBuf.Add(tuid);
	//----------------------------------------------------

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

//	SendInsight(pCom, TempBuf, TempBuf.GetLength());
	SendExactScreen(pCom, TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////
//	NPC 傍拜 固胶
//
void CNpc::SendAttackMiss(COM *pCom, int tuid)
{
	CBufferEx TempBuf;
	
	TempBuf.Add(ATTACK_RESULT);
	TempBuf.Add(ATTACK_MISS);
	TempBuf.Add((int)(m_sNid + NPC_BAND));
	TempBuf.Add(tuid);

//	SendInsight(pCom, TempBuf, TempBuf.GetLength());
	SendExactScreen(pCom, TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////
//	NPC 狼 公扁 拌凯阑 备茄促.
//
BYTE CNpc::GetWeaponClass()
{
	BYTE tClass = BRAWL;

	switch (m_byClass)
	{
	case 1:
		tClass = BRAWL;
		break;
		
	case 2:
		tClass = STAFF;
		break;
		
	case 4:
		tClass = EDGED;
		break;
		
	case 8:
		tClass = FIREARMS;
		break;
	}

	return tClass;
}

/////////////////////////////////////////////////////////////////////////////////////
//	傍拜矫付促 胶懦己傍 咯何甫 眉农茄促.
//
void CNpc::IsSkillSuccess(BOOL *bSuccess)
{
	int iOnCount = 0;
	int i = 0;
	
	for(i = 0; i < SKILL_NUM; i++) bSuccess[i] = FALSE;

	for(i = 0; i < SKILL_NUM; i++) 
	{
		if(m_NpcSkill[i].tOnOff == 1) iOnCount++;
	}

	int iRandom = XdY(1, 100);
	int iRate = 0;

	for(i = 0; i < SKILL_NUM; i++)
	{
		iRate = (int)((double)m_sWIS * 0.5 + m_NpcSkill[i].tLevel * 2 - iOnCount * 25 + 50 /* +Magic Bonus*/);		//!Magic

		if(iRandom <= iRate) 
		{
			bSuccess[i] = TRUE;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////
//	檬扁 老馆 单固瘤甫 掘绰促.
//
int CNpc::GetNormalInitDamage()
{
	int nHit = 0;
	int nDamage = 0;
	int xyz = 0;

	xyz = XdY(m_byAX, m_byAY) + m_byAZ;

//  2002-10-17 by Youn Gyu
//	if(m_byClass == FIREARMS) nHit = (int)((double)m_sDEX/3 + 0.5);
//	else					  nHit = (int)((double)m_sSTR/2 + 0.5);

	if(nHit < 0) nHit = 0;

	nDamage = nHit + xyz;
	return nDamage;
}

///////////////////////////////////////////////////////////////////////////////////////
//	檬扁 农府萍拿 单固瘤甫 掘绰促.
//
int CNpc::GetCriticalInitDamage(BOOL *bSuccessSkill)
{
	int nDamage = 0;
	int xyz = 0;

	xyz = XdY(m_byAX, m_byAY) + m_byAZ;
/*
	// 焊胶各老锭 扁裙俊辑 舅妨临抗沥...
*/
	return nDamage;

}
///////////////////////////////////////////////////////////////////////////
//	趁扁 单固瘤甫 悸泼茄促.
//
void CNpc::SetColdDamage()
{
	if(m_tAbnormalKind != ABNORMAL_BYTE_NONE) return;		// 捞固 惑怕捞惑捞 吧妨乐绰 惑怕捞搁 府畔

	m_tAbnormalKind = ABNORMAL_BYTE_COLD;
	m_dwAbnormalTime = COLD_TIME;
	m_dwLastAbnormalTime = GetTickCount();
}

///////////////////////////////////////////////////////////////////////////
//	拳堪单固瘤甫 悸泼茄促.
//
void CNpc::SetFireDamage()
{
	if(m_tAbnormalKind != ABNORMAL_BYTE_NONE) return;		// 捞固 惑怕捞惑捞 吧妨乐绰 惑怕捞搁 府畔

	m_tAbnormalKind = ABNORMAL_BYTE_FIRE;
	m_dwAbnormalTime = FIRE_TIME;
	m_dwLastAbnormalTime = GetTickCount();
}

/////////////////////////////////////////////////////////////////////////////
//	Damage 拌魂, 父距 m_sHP 啊 0 捞窍捞搁 荤噶贸府
//
BOOL CNpc::SetDamage(int nDamage, int uuid, COM *pCom)
{
	if(m_NpcState == NPC_DEAD) return TRUE;
	if(m_sHP <= 0) return TRUE;
	if(nDamage <= 0) return TRUE;

	if(m_tNpcType == NPCTYPE_GUARD) return TRUE;

	if(m_tGuildWar == GUILD_WAR_AFFTER)
	{
		if(m_tNpcType >= NPCTYPE_GUILD_NPC) return TRUE;
//		if(m_tNpcType == NPCTYPE_GUILD_NPC) return TRUE;
//		if(m_tNpcType == NPCTYPE_GUILD_GUARD) return TRUE;
//		if(m_tNpcType == NPCTYPE_GUILD_DOOR) return TRUE;
	}

	if(InterlockedCompareExchange((long*)&m_lDamage, (long)1, (long)0) == (long)0)
	{
		int i;
		int iLen = 0;
		int userDamage = 0;
		ExpUserList *tempUser = NULL;

		int uid = uuid - USER_BAND;

		USER* pUser = GetUser(pCom, uid);
														// 秦寸 荤侩磊牢瘤 牢刘
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) 
		{
			InterlockedExchange(&m_lDamage, (LONG)0);
			return TRUE;
		}

		iLen = strlen(pUser->m_strUserID);
		if(iLen <= 0 || iLen > CHAR_NAME_LENGTH) 
		{
			InterlockedExchange(&m_lDamage, (LONG)0);
			return TRUE;
		}

		if(m_tGuildWar == GUILD_WARRING)// 辨靛傈捞 国绢瘤绰 惑痢狼 惑老 版快
		{
			if(m_tNpcType == NPCTYPE_GUILD_NPC)		// 惑痢苞 傍己傈俊辑 NPC绰 辑肺 促弗 搬苞甫 父甸扁 锭巩
			{
//				if(m_pGuardStore) { SetDamagedInGuildWar(nDamage, pUser); InterlockedExchange(&m_lDamage, (LONG)0); return TRUE; }
				if(m_pGuardFortress) { SetDamagedInFortressWar(nDamage, pUser); InterlockedExchange(&m_lDamage, (LONG)0); return TRUE; }
			}
			else if(m_tNpcType == NPCTYPE_GUILD_DOOR)	// 傍己傈阑 困秦 漂喊洒	  
			{
				if(m_pGuardFortress) { SetDoorDamagedInFortressWar(nDamage, pUser); InterlockedExchange(&m_lDamage, (LONG)0); return TRUE; }
			}
			else if(m_tNpcType == NPCTYPE_GUILD_GUARD)	// 阿 辨靛俊 加茄 版厚捍篮 磊扁辨靛盔栏肺何磐 焊龋(?)罐扁困秦
			{
				if(pUser->m_dwGuild > 0)
				{
/*					if(m_pGuardStore) 
					{ 
						if(m_pGuardStore->m_iGuildSid == pUser->m_dwGuild)
						{
							InterlockedExchange(&m_lDamage, (LONG)0);
							return TRUE; 
						}
*/					if(m_pGuardFortress) 
					{ 
						if(m_pGuardFortress->m_iGuildSid == pUser->m_dwGuild)
						{
							InterlockedExchange(&m_lDamage, (LONG)0); 
							return TRUE; 
						}
					}
				}
			}
		}

		if( m_sEvent == NPC_EVENT_GREATE_MOP || m_sEvent== NPC_EVENT_MOP )		 //如果打BOSS攻击只有平时的50%.
		{
			nDamage = (int)( (double)nDamage * 0.5 );
		}

		userDamage = nDamage;
																
		if( (m_sHP - nDamage) < 0 ) userDamage = m_sHP;

		for(i = 0; i < NPC_HAVE_USER_LIST; i++)
		{
			if(m_DamagedUserList[i].iUid == uid)
			{
				if(strcmp(m_DamagedUserList[i].strUserID, pUser->m_strUserID) == 0) 
				{ 
					m_DamagedUserList[i].nDamage += userDamage; 
					goto go_result;
				}
			}
		}

		for(i = 0; i < NPC_HAVE_USER_LIST; i++)				// 牢盔 力茄捞 弥辆 措固瘤俊 康氢阑 固摹唱?
		{
			if(m_DamagedUserList[i].iUid == -1)
			{
				if(m_DamagedUserList[i].nDamage <= 0)
				{
					strncpy(m_DamagedUserList[i].strUserID, pUser->m_strUserID, iLen);
					m_DamagedUserList[i].iUid = uid;
					m_DamagedUserList[i].nDamage = userDamage;
					m_DamagedUserList[i].bIs = FALSE;
					break;
				}
			}
		}

go_result:
		m_TotalDamage += userDamage;
		m_sHP -= nDamage;
	/*	////////////////////////////////////////////////////////////////////////////////////////////////////////////// 杀BOSS提醒
		if( m_sEvent == 31000 )
		{
			CString strMsg = "";
		    int g_pid = m_sPid;
		    int g_sid = m_sSid;			
			CString Monstername = g_arNpcTable[g_sid]->m_strName;

			if(m_sHP <= 0 )
			{
				Dead();
			    strMsg.Format( "玩家『 %s 』战胜BOSS『 %s 』 获得大量的宝贝!", pUser->m_strUserID,m_strName);
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
		}*/
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( m_sHP <= 0 )
		{
			UserListSort();							// 酒捞袍狼 林牢捞 穿焙啊

			m_ItemUserLevel = pUser->m_sLevel;
			m_sHP = 0;

			InterlockedExchange(&m_lKillUid, (LONG)uid);
			if(m_sPid==190)
				m_sEvent=2;
			// 磷篮 各捞 涅胶飘俊 加茄 捞亥飘 各牢版快 秦寸 捞亥飘甫 角青
			if(m_sEvent > 0 && m_sEvent <= NPC_QUEST_MOP)	
				pUser->RunQuestEvent(this, m_sCurZ, m_sEvent);
			Dead();
			InterlockedExchange(&m_lDamage, (LONG)0);
			return FALSE;
		}

		ChangeTarget(pUser, pCom);

		InterlockedExchange(&m_lDamage, (LONG)0);
	}
	return TRUE;
}

BOOL CNpc::CheckNpcRegenCount()
{
/*	if(m_NpcState != NPC_DEAD) return FALSE;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_RegenLastCount);

	if((m_RegenLastCount - m_RegenStartCount) >= g_Online_Update_Min_ticks)
	{
		m_RegenStartCount += g_Online_Update_Min_ticks;
		m_RegenCount += 10000;
	}

	if(m_RegenCount >= (DWORD)m_sRegenTime) return TRUE;
*/
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
//	鸥百捞 笛矾 阶咯 乐栏搁 促澜 鸥百阑 茫绰促.
//
BOOL CNpc::IsSurround(int targetx, int targety)
{
	if(m_tNpcLongType) return FALSE;		//盔芭府绰 烹苞

	for(int i = 0; i < (sizeof(surround_x) / sizeof(surround_x[0])); i++)		// 林函 8规氢
	{
		if(IsMovable(targetx + surround_x[i], targety + surround_y[i])) return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	唱甫 傍拜茄 蜡历甫 鸥百栏肺 伙绰促.(扁霖 : 肪苞 HP甫 扁霖栏肺 急沥)
//
void CNpc::ChangeTarget(USER *pUser, COM* pCom)
{
	int preDamage, lastDamage;
	int dist;

	if(m_byAX == 0 && m_byAZ == 0 ) return;		// 郴啊 傍拜仿捞 绝栏搁 傍拜窍瘤 臼绰促
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_bLive == USER_DEAD) return;
	if(pUser->m_tIsOP == 1) return;		// 款康磊绰 公矫...^^
	if(pUser->m_bPShopOpen == TRUE) return;
	USER *preUser = NULL;
	preUser = GetUser(pCom, m_Target.id - USER_BAND);

	if(pUser == preUser) return;

	if(preUser != NULL && preUser->m_state == STATE_GAMESTARTED)
	{
		if(strcmp(pUser->m_strUserID, preUser->m_strUserID) == 0) return;
		
		preDamage = 0; lastDamage = 0;
		preDamage = GetFinalDamage(preUser, 0);
		lastDamage = GetFinalDamage(pUser, 0); 

		dist = abs(preUser->m_curx - m_sCurX) + abs(preUser->m_cury - m_sCurY);
		if(dist == 0) return;
		preDamage = (int)((double)preDamage/dist + 0.5);
		dist = abs(pUser->m_curx - m_sCurX) + abs(pUser->m_cury - m_sCurY);
		if(dist == 0) return;
		lastDamage = (int)((double)lastDamage/dist + 0.5);

		if(preDamage > lastDamage) return;
	}
		
	m_Target.id	= pUser->m_uid + USER_BAND;
	m_Target.x	= pUser->m_curx;
	m_Target.y	= pUser->m_cury;

/*	if(pUser->m_strUserID != NULL)
	{
		m_Target.nLen = strlen(pUser->m_strUserID);

		if(m_Target.nLen <= CHAR_NAME_LENGTH) strncpy(m_Target.szName, pUser->m_strUserID, m_Target.nLen);
		else								  ::ZeroMemory(m_Target.szName, sizeof(m_Target.szName));
	}
*/										// 绢浇贩 芭府绰单 傍拜窍搁 官肺 馆拜
	if(m_NpcState == NPC_STANDING || m_NpcState == NPC_MOVING)
	{									// 啊鳖捞 乐栏搁 馆拜栏肺 捞绢瘤备
		if(IsCloseTarget(pUser, m_byRange) == TRUE)
		{
			m_NpcState = NPC_FIGHTING;
			NpcFighting(pCom);
		}
		else							// 官肺 档噶啊搁 谅钎甫 盎脚窍绊 眠利	
		{
			if(GetTargetPath(pCom) == TRUE)	// 馆拜 悼累饶 距埃狼 掉饭捞 矫埃捞 乐澜	
			{
				m_NpcState = NPC_TRACING;
				NpcTracing(pCom);
			}
			else
			{
				ToTargetMove(pCom, pUser);
			}
		}
	}
//	else m_NpcState = NPC_ATTACKING;	// 茄曼 傍拜窍绰单 穿啊 规秦窍搁 格钎甫 官厕

	if(m_tNpcGroupType)					// 啊练鸥涝捞搁 矫具救俊 鞍篮 鸥涝俊霸 格钎 瘤沥
	{
		m_Target.failCount = 0;
		FindFriend();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	NPC 惑怕喊肺 盒拳茄促.
//
void CNpc::NpcLive(COM *pCom)
{
	if(SetLive(pCom))
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
        if(m_sEvent == 31000)
		{
			if(m_sCurZ == 19)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在 [ 污染洞3层 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 49)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在 [ 矿洞4层 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 400)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在  [ 菲利普 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 93)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在  [ 忍者洞1层 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 94)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在  [ 忍者洞2层 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 36)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在  [ 电厂3层 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 416)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在  [ 死神洞穴 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 311)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在  [ 火神洞2层 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
			if(m_sCurZ == 315)
			{
				CString strMsg;
				strMsg.Format("领主BOSS『 %s 』在  [ 机甲工厂 ] 刷新了!", m_strName);//公告提示
				pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
			}
		}
	}
	else
	{
		m_NpcState = NPC_LIVE;
		m_Delay = m_sStandTime * 10;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	NPC啊 辑乐绰版快.
//
void CNpc::NpcStanding(COM *pCom)
{
	NpcTrace(_T("NpcStanding()"));

	if(RandomMove(pCom) == TRUE)
	{
		m_NpcState = NPC_MOVING;

		if( m_sStandTime > 2500 )
		{
			m_Delay = m_sStandTime - 2000;
		}
		else
		{
			m_Delay = m_sStandTime;
		}

//		m_Delay = m_sStandTime;
//		m_Delay = m_sSpeed;		 // 2001-09-01, jjs07 
		return;
	}

	m_NpcState = NPC_STANDING;

	m_Delay = m_sStandTime;
}

/////////////////////////////////////////////////////////////////////////////
//	NPC啊 捞悼窍绰 版快.
//
void CNpc::NpcMoving(COM *pCom)
{
	NpcTrace(_T("NpcMoving()"));

	if(m_sHP <= 0) 
	{
		Dead();
		return;
	}

	if(FindEnemy(pCom) == TRUE)		// 利阑 茫绰促. 
	{
		if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD) 
		{ 
			m_NpcState = NPC_FIGHTING; 
			m_Delay = 0; 
		}
		else 
		{ 
			m_NpcState = NPC_ATTACKING;
			m_Delay = m_sSpeed;
		}
		return;
	}

	if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD) // 捞悼救窍霸...
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
		return;
	}

	if(IsMovingEnd())				// 捞悼捞 场车栏搁
	{
		m_NpcState = NPC_STANDING;

		//康开 观俊 乐栏搁 辑乐绰 矫埃阑 陋霸...
		if(IsInRange())	m_Delay = m_sStandTime;
		else m_Delay = m_sStandTime - 1000;

		if(m_Delay < 0) m_Delay = 0;

		return;
	}

	if(StepMove(pCom, 1) == FALSE)	// 茄沫 框流烙(叭绰悼累, 崔副锭绰 2沫)
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
		return;
	}

	CBufferEx TempBuf;

	CPoint t = ConvertToClient( m_sCurX, m_sCurY );		// 框流捞妨绰 辑滚谅钎啊 努扼捞攫飘俊辑 给框流捞绰 谅钎搁 府畔

	if(IsStepEnd())	TempBuf.Add(MOVE_END_RESULT);
	else			TempBuf.Add(MOVE_RESULT);

	TempBuf.Add(SUCCESS);
	TempBuf.Add((int)(NPC_BAND + m_sNid));
	TempBuf.Add((short)t.x);
	TempBuf.Add((short)t.y);

	SendInsight(pCom, TempBuf, TempBuf.GetLength());

	m_Delay = m_sSpeed;
}

/////////////////////////////////////////////////////////////////////////////
//	NPC啊 傍拜窍绰版快.
//
void CNpc::NpcAttacking(COM *pCom)
{
	NpcTrace(_T("NpcAttacking()"));
	
	int ret = 0;

	if( m_byPsi > 0 && m_byPsi < g_arMonsterPsi.GetSize() )	// 付过捞 乐绰 仇捞扼搁...
	{
		CMonsterPsi* pMagic = g_arMonsterPsi[(int)m_byPsi];

		if( pMagic )
		{
			if( IsCloseTarget( pCom, pMagic->m_byRange ) )
			{
				m_NpcState = NPC_FIGHTING;
				m_Delay = 0;
				return;
			}
		}
	}

	if(IsCloseTarget(pCom, m_byRange))	// 傍拜且 荐 乐绰父怒 啊鳖款 芭府牢啊?
	{
		m_NpcState = NPC_FIGHTING;
		m_Delay = 0;
		return;
	}

	if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD)// 辑乐绰 版厚捍老锭 傍拜捞 角菩窍搁 泪瘪 悼救 浆档废茄促. 
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime/2;
		return;
	}

	if(GetTargetPath(pCom) == FALSE)
	{
		if(RandomMove(pCom) == FALSE)
		{
			m_NpcState = NPC_STANDING;
			m_Delay = m_sStandTime;
			return;
		}

		m_NpcState = NPC_MOVING;
		m_Delay = m_sSpeed;
		return;
	}

	m_NpcState = NPC_TRACING;
	m_Delay = 0;
}

/////////////////////////////////////////////////////////////////////////////
//	NPC啊 蜡历甫 眠利窍绰 版快.
//
void CNpc::NpcTracing(COM *pCom)
{
	NpcTrace(_T("NpcTracing()"));

	if(m_tNpcType == NPCTYPE_GUARD || m_tNpcType == NPCTYPE_GUILD_GUARD) return;

	if(GetUser(pCom, (m_Target.id - USER_BAND)) == NULL)	// Target User 啊 粮犁窍绰瘤 八荤
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
		return;
	}

	if( m_byPsi > 0 && m_byPsi < g_arMonsterPsi.GetSize() )	// 付过捞 乐绰 仇捞扼搁...
	{
		CMonsterPsi* pMagic = g_arMonsterPsi[(int)m_byPsi];

		if( pMagic )
		{
			if( IsCloseTarget( pCom, pMagic->m_byRange ) )
			{
				m_NpcState = NPC_FIGHTING;
				m_Delay = 0;
				return;
			}
		}
	}

	if(IsCloseTarget(pCom, m_byRange))						// 辟立傈阑 国老父怒 啊鳖款 芭府牢啊?
	{
		m_NpcState = NPC_FIGHTING;
		m_Delay = 0;
		return;
	}

	if(IsSurround(m_Target.x, m_Target.y))					// 格钎 鸥百捞 笛矾阶咯 乐栏搁 促澜 鸥百阑 茫绰促.
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;
		return;
	}

	if(IsChangePath(pCom))									// 辨茫扁甫 促矫 且父怒 Target 狼 困摹啊 函沁绰啊?
	{
		if(ResetPath(pCom) == FALSE)// && !m_tNpcTraceType)
		{
			m_NpcState = NPC_STANDING;
			m_Delay = m_sStandTime;
			return;
		}
	}
	
	if(StepMove(pCom, 1) == FALSE)							// 茄沫 框流烙(叭绰悼累, 崔副锭绰 2沫)
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sStandTime;	
		return;
	}

	CBufferEx TempBuf;

	CPoint t = ConvertToClient( m_sCurX, m_sCurY );		// 框流捞妨绰 辑滚谅钎啊 努扼捞攫飘俊辑 给框流捞绰 谅钎搁 府畔

	if(IsStepEnd())	TempBuf.Add(MOVE_END_RESULT);
	else			TempBuf.Add(MOVE_RESULT);

	TempBuf.Add(SUCCESS);
	TempBuf.Add((int)(NPC_BAND + m_sNid));
	TempBuf.Add((short)t.x);
	TempBuf.Add((short)t.y);

	SendInsight(pCom, TempBuf, TempBuf.GetLength());

	m_Delay = m_sSpeed;
}

/////////////////////////////////////////////////////////////////////////////
//	各狼 加档 函拳甫 舅妨 霖促.
//
void CNpc::ChangeSpeed(COM *pCom, int delayTime)
{
/*	CBufferEx TempBuf;

	int tempTime = delayTime * NPC_TRACING_STEP;
	
	if(m_Delay > m_sSpeed) m_Delay = m_sSpeed;// 胶拍爹 矫埃捞 1000老版快 

	m_Delay = m_Delay + tempTime;			// 10, 50, 100窜困肺 刘.皑
	
	if(m_Delay <= 500) m_Delay = 500;		// 弥家绰 亲惑 500烙	
											// 500阑 100%捞搁 600篮 80栏肺 皑家	
	short step = 100 - (m_Delay - 500) * 10/50;

	TempBuf.Add(SET_SPEED_MONSTER);

	TempBuf.Add(m_sNid + NPC_BAND);
	TempBuf.Add(step);

		// NPC 林困狼 蜡历俊霸 沥焊傈价
	CPoint ptOld;
	if(SightRecalc(ptOld))
	{
		SendRemainSight(pCom, TempBuf, TempBuf.GetLength(), ptOld);
	}
	else SendInsight(pCom, TempBuf, TempBuf.GetLength());
*/
}

/////////////////////////////////////////////////////////////////////////////
//	NPC啊 傍拜窍绰 版快.
//  有魔法的怪用
void CNpc::NpcFighting(COM *pCom)
{
	NpcTrace(_T("NpcFighting()"));

	if(m_sHP <= 0) 
	{
		Dead();
		return;
	}

	m_dwDelayCriticalDamage = 0;

	int at_type_total3 = m_iNormalATRatio + m_iSpecialATRatio + m_iMagicATRatio;	
	int at_type[300], i;
	for( i = 0; i < m_iNormalATRatio; i++ ) at_type[i] = 1;
	int rand_index = m_iNormalATRatio;
	for( i = rand_index; i < rand_index+m_iSpecialATRatio; i++ )	at_type[i] = 2;
	rand_index += m_iSpecialATRatio;
	for( i = rand_index; i < rand_index+m_iMagicATRatio; i++ )	at_type[i] = 3;
	int at_type_rand = myrand( 0, at_type_total3);	

	if( at_type[at_type_rand] == 3 && m_byPsi > 0)
	{		
		m_Delay = PsiAttack( pCom );				
		if( m_Delay == -1 )
		{
			m_NpcState = NPC_ATTACKING;
			m_Delay = m_sSpeed;				
		}											
		return;
	}
	else if( at_type[at_type_rand] == 2 )
	{
			m_Delay = AreaAttack( pCom );		
			if( m_Delay == -1 )
			{
				m_NpcState = NPC_ATTACKING;
				m_Delay = m_sSpeed;
			}
			return;
	}else	m_Delay = Attack(pCom);		 	
}


/////////////////////////////////////////////////////////////////////////////
//	鸥百苞狼 芭府甫 荤沥芭府 裹困肺 蜡瘤茄促.(伎窜困)
//
void CNpc::NpcBack(COM *pCom)
{
	NpcTrace(_T("NpcBack()"));

	if(GetUser(pCom, (m_Target.id - USER_BAND)) == NULL)	// Target User 啊 粮犁窍绰瘤 八荤
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sSpeed;//STEP_DELAY;
		return;
	}

	if(IsMovingEnd())								// 捞悼捞 场车栏搁
	{
		m_Delay = m_sSpeed;
		NpcFighting(pCom);
		return;
	}
	
	if(StepMove(pCom, 1) == FALSE)					// 茄沫 框流烙(叭绰悼累, 崔副锭绰 2沫)
	{
		m_NpcState = NPC_STANDING;
		m_Delay = m_sSpeed;//STEP_DELAY;
		return;
	}

	m_Delay = m_sSpeed;//STEP_DELAY;
}

/////////////////////////////////////////////////////////////////////////////
//	促弗 各苞狼 楷拌甫 困秦辑...
//
void CNpc::NpcStrategy(BYTE type)
{
	switch(type)
	{
	case NPC_ATTACK_SHOUT:
		m_NpcState = NPC_TRACING;
		m_Delay = m_sSpeed;//STEP_DELAY;
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	矫具 裹困郴狼 郴悼丰甫 茫绰促.
//
void CNpc::FindFriend()
{
	CNpc* pNpc = NULL;

	if(m_bySearchRange == 0) return;

	int min_x, min_y, max_x, max_y;

	min_x = m_sCurX - m_bySearchRange;		if( min_x < 0 ) min_x = 0;
	min_y = m_sCurY - m_bySearchRange;		if( min_y < 0 ) min_y = 0;
	max_x = m_sCurX + m_bySearchRange;
	max_y = m_sCurY + m_bySearchRange;

	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

	int ix, iy;
	int target_uid;
	int uid;

	int tempLevel = 0, oldLevel = 1000;

	if(m_Target.id == -1) return;

	for(ix = min_x; ix <= max_x; ix++)
	{
		for(iy = min_y; iy <= max_y; iy++)
		{
			target_uid = m_pOrgMap[ix][iy].m_lUser;

			if( target_uid >= NPC_BAND && target_uid < INVALID_BAND)
			{
				uid = target_uid - NPC_BAND;				
				pNpc = g_arNpc[uid];
				if(pNpc == NULL) continue;
									
				if(pNpc->m_tNpcGroupType && m_sNid != uid && pNpc->m_sFamilyType == m_sFamilyType)
				{
//					pNpc->m_Target.nLen = strlen(pNpc->m_Target.szName);	// 捞固 格钎啊 乐绢辑 捞固 傍拜窍绊 乐栏搁...
//					if(pNpc->m_Target.nLen > 0 && pNpc->m_NpcState == NPC_FIGHTING) continue;
					if(pNpc->m_Target.id >= 0 && pNpc->m_NpcState == NPC_FIGHTING) continue;

					pNpc->m_Target.id = m_Target.id;		// 鞍篮 鸥涝狼 悼丰俊霸 档框阑 夸没茄促.
					pNpc->m_Target.x = m_Target.x;			// 鞍篮 格钎甫 傍拜窍磊绊...
					pNpc->m_Target.y = m_Target.y;

/*					if(m_Target.szName != NULL)
					{
						pNpc->m_Target.nLen = strlen(m_Target.szName);

						if(pNpc->m_Target.nLen <= CHAR_NAME_LENGTH) strncpy(pNpc->m_Target.szName, m_Target.szName, pNpc->m_Target.nLen);
						else								  ::ZeroMemory(pNpc->m_Target.szName, sizeof(pNpc->m_Target.szName));
					}
*/					pNpc->m_Target.failCount = 0;
					pNpc->NpcStrategy(NPC_ATTACK_SHOUT);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	鸥百痢栏肺何磐 弥措茄 钢府 冻绢柳 痢阑 茫绰促.(林狼 : 盔芭府绰 加档啊 弧扼具 茄促...救弊矾搁 拌加 芭府甫 蜡瘤窍促 磷绰荐啊 乐促.)
//
BOOL CNpc::GetBackPoint(int &x, int &y)
{
	int ex = m_sCurX;
	int ey = m_sCurY;

	int dx = m_Target.x - m_sCurX;
	int dy = m_Target.y - m_sCurY;

	int min = ( abs(dx) + abs(dy) )/2;
	int max = m_byRange - min;
	int count = myrand(min, max);

	if(count <= 0) return FALSE;							// 0捞搁 盔贰 荤沥芭府客 鞍促.
	if(count >= m_byRange && count > 2) count -= 1;			// 趣矫唱 隔扼辑 弥措茄 撅力茄促.

	if(dy > 0)
	{
		if(dx > 0)		{ ex -= count; ey -= count; }
		else if(dx < 0)	{ ex += count; ey -= count; }
		else			{ ey -= (count*2); }				// 圈荐, 娄荐甫 嘎冕促.
	}
	else if(dy < 0)
	{
		if(dx > 0)		{ ex -= count; ey += count; }
		else if(dx < 0)	{ ex += count; ey += count; }
		else			{ ey += (count*2); }
	}
	else
	{
		if(dx > 0)		{ ex -= (count*2); }
		else			{ ex += (count*2); }
	}

	if(IsMovable(ex, ey) == FALSE)							// 茫篮 痢捞 框龙荐 绝绰 镑捞扼搁 8规氢栏肺 沤祸
	{
		for(int i = 0; i < (sizeof(surround_x) / sizeof(surround_x[0])); i++)		// 林函 8规氢
		{
			if(IsMovable(ex + surround_x[i], ey + surround_y[i])) 
			{
				x = ex; y = ey;
				return TRUE;
			}
		}
	}
	else
	{
		x = ex; y = ey;
		return TRUE;
	}

	return FALSE;									
}

/////////////////////////////////////////////////////////////////////////////
//	蜡历府胶飘甫 檬扁拳茄促.
//
void CNpc::InitUserList()
{
	m_TotalDamage = 0;
	for(int i = 0; i < NPC_HAVE_USER_LIST; i++)
	{
		m_DamagedUserList[i].bIs = FALSE;
		m_DamagedUserList[i].iUid = -1;
		m_DamagedUserList[i].nDamage = 0;
		::ZeroMemory(m_DamagedUserList[i].strUserID, sizeof(m_DamagedUserList[i].strUserID));
	}
/*	int i;
									// 版氰摹 盒硅甫 困秦 包府窍带 府胶飘
	for(i = 0; i < m_arDamagedUserList.GetSize(); i++)
	{
		if(m_arDamagedUserList[i])
		{
			delete m_arDamagedUserList[i];
			m_arDamagedUserList[i] = NULL;
		}
	}
	m_arDamagedUserList.RemoveAll();
*/
}

/////////////////////////////////////////////////////////////////////////////
//	秦寸 概流 加己捞 酒捞袍 拌凯 棺 傀绢沥焊啊 嘎绰瘤 眉农茄促.
//
BOOL CNpc::CheckClassItem(int artable, int armagic)
{
	if(artable < 0 || artable >= g_arItemTable.GetSize()) return FALSE;
	if(armagic < 0 || armagic >= g_arMagicItemTable.GetSize()) return FALSE;
	if(armagic==148||armagic==149||armagic==150||armagic==151||armagic==152||armagic==153||armagic==156|| armagic==158 || armagic==162||armagic==164 || armagic==166){
		return FALSE;
	}
	int iWear;

	BYTE armWear = g_arItemTable[artable]->m_byWear;			// 酒捞袍 拌凯 1: 公扁 2~8 : 馒侩酒捞袍
	BYTE tNeedClass = g_arItemTable[artable]->m_byClass;
	BYTE armMagic = g_arMagicItemTable[armagic]->m_tNeedClass;	// 概流加己 拌凯

	if(armMagic != 31/*15*/)
	{
		BYTE tTemp = 1;	
		BYTE tFire = 0;
		BYTE tEdge = 0;
		BYTE tStaff = 0;
		BYTE tBrawl = 0;
		BYTE tJudge = 0;

		tFire	 = tTemp & tNeedClass; tTemp = 2; 
		tEdge	 = tTemp & tNeedClass; tTemp = 4;
		tStaff	 = tTemp & tNeedClass; tTemp = 8;
		tJudge   = tTemp & tNeedClass; tTemp = 16;
		tBrawl	 = tTemp & tNeedClass;

		tFire = tFire & armMagic;
		tEdge = tEdge & armMagic;
		tStaff = tStaff & armMagic;
		tBrawl = tBrawl & armMagic;
		tJudge = tJudge & armMagic;

		tTemp = tFire^tEdge^tStaff^tBrawl^tJudge;
		if(!tTemp) return FALSE;
//		if(tNeedClass != armMagic) return FALSE;
	}

	iWear = g_arMagicItemTable[armagic]->m_tWearInfo;		// 傀绢 沥焊肺 肋给等 加己捞 嘿绰巴阑 规瘤

	if(iWear == 0) return TRUE;
	else if(iWear == 1)											
	{														// 1锅捞搁 公扁幅俊 嘿绰促.
		if(armWear != 1) return FALSE;
		else return TRUE;
	}
	else if(iWear == 2)										// 2锅捞搁 公扁甫 力寇茄 馒侩酒捞袍俊 嘿绰促.
	{
		if(armWear <= 1 || armWear >= 9) return FALSE;
		else return TRUE;
	}
	else return FALSE;
}

void CNpc::DeleteNPC()
{
	// 瘤陛 阁胶磐甫 瘤快瘤 臼绰促. 窜瘤, 静饭靛 惑俊辑 倒瘤 给窍档废 阜扁父 茄促.
	m_bFirstLive = FALSE;
	m_tNpcType = 2;

	// 唱吝俊 扁废窍磊.
}

//////////////////////////////////////////////////////////////////////////////////
//	弥辆 单固瘤甫 备茄促.
//
int CNpc::GetFinalDamage(USER *pUser, int type)
{
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return 0;

	int iInitDamage = GetNormalInitDamage();
	int iFinalDamage = 0, iFinalTemp = 0;
	
//	int iBasic = (int)((double)(pUser->m_sCON + pUser->m_DynamicUserData[MAGIC_CON_UP])/3 + 0.5);					// 扁夯镐笼
	int iBasic = (int)((double)(pUser->m_sMagicCON + pUser->m_DynamicUserData[MAGIC_CON_UP] + (int)((double)pUser->m_DynamicEBodyData[EBODY_CON_TO_DEFENSE] / 100 * (double)pUser->m_sMagicCON) )/3 + 0.5);				// 扁夯镐笼
	if(iBasic < 0) iBasic = 0;

	BYTE tWeaponClass = 255;
	BOOL bCanUseSkill = pUser->IsCanUseWeaponSkill(tWeaponClass);

	int		iDefense = 1;
	double	dIron = 0;
	double	dShield = 0;
	double	dGuard = 0;
	int		iCAttack = 0;
	double	dAdamantine = 0;
	double	dDefenseUP = 0;
	double	dABDefense = 0;

	int		iIronLevel = 0;
	int		iGuardLevel = 0;
	int		iVitalLevel = 0;
	int		iCounterAttackLevel = 0;
	int		iDefenseUPLevel = 0;
	int		iABDefenseLevel = 0;

	int		iIS = 0;
	int		iCA = 0;

	int i = 0;
	int iRandom = 0;
	int iSkillSid = 0;
	int tClass = tWeaponClass * SKILL_NUM;

	iDefense = pUser->GetDefense();							// 规绢备

	if(tWeaponClass != 255)
	{
		for(i = tClass; i < tClass + SKILL_NUM; i++)	// IronSkill
		{
			iSkillSid = pUser->m_UserSkill[i].sSid;

			if(iSkillSid == SKILL_IRON)					// 1 index
			{
				iIronLevel = pUser->m_UserSkill[i].tLevel;
				if(iIronLevel < 0) iIronLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iIronLevel >= 1) iIronLevel += pUser->m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iIronLevel >= SKILL_LEVEL) iIronLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;

				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iIronLevel)) iIS = 1;
				
				// 酒捞攫胶挪俊 狼茄 扁夯 镐笼狼 刘啊
				iBasic = (int)((double)iBasic * (1 + (double)(iIS * g_arSkillTable[iSkillSid]->m_arInc.GetAt(iIronLevel) / 100)) );
			}

			if(iSkillSid == SKILL_CRITICAL_GUARD)					// Critical Guard 11 index
			{
				iGuardLevel = pUser->m_UserSkill[i].tLevel;		
				if(iGuardLevel < 0) iGuardLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iGuardLevel >= 1) iGuardLevel += pUser->m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iGuardLevel >= SKILL_LEVEL) iGuardLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iGuardLevel))
				{				
					dGuard = (double)(iInitDamage *g_arSkillTable[iSkillSid]->m_arInc.GetAt(iGuardLevel))/100.0;
				}
			}

			if(iSkillSid == SKILL_BACK_ATTACK)					// 馆拜 2 index
			{
				iCounterAttackLevel = pUser->m_UserSkill[i].tLevel;		
				if(iCounterAttackLevel < 0) iCounterAttackLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iCounterAttackLevel >= 1) iCounterAttackLevel += pUser->m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iCounterAttackLevel >= SKILL_LEVEL) iCounterAttackLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				if(pUser->GetDistance(m_sCurX, m_sCurY, 1) == FALSE && pUser->m_dwFANTAnTime == 0 ) iCA = 0;//超级反弹

				else
				{
					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iCounterAttackLevel)) iCA = 1;
				}
				
			
				 iCAttack = (int)(iInitDamage * iCA * (double)((g_arSkillTable[iSkillSid]->m_arInc.GetAt(iCounterAttackLevel)) / 100.0) ); 
			   }

			if(iSkillSid == SKILL_ABSOLUTE_DEFENSE)					// 绝对 Defense
			{
				iABDefenseLevel = pUser->m_UserSkill[i].tLevel;		
				if(iABDefenseLevel < 0) iABDefenseLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iABDefenseLevel >= 1) iABDefenseLevel += pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iABDefenseLevel >= SKILL_LEVEL) iABDefenseLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iABDefenseLevel))
				{
					dABDefense = (double)(iDefense * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iABDefenseLevel)/100.0);
				}
			}
			if(iSkillSid == SKILL_DEFENSE_UP)					// 超级 Defense
			{
				iDefenseUPLevel = pUser->m_UserSkill[i].tLevel;		
				if(iDefenseUPLevel < 0) iDefenseUPLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iDefenseUPLevel >= 1) iDefenseUPLevel += pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iDefenseUPLevel >= SKILL_LEVEL) iDefenseUPLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iDefenseUPLevel))
				{
					dDefenseUP = (double)(iDefense * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iDefenseUPLevel)/100.0);
				}
			}
		}
	}

//	if(pUser->m_dwShieldTime != 0)	dShield = (double)(iInitDamage * 0.2);
	if(pUser->m_bNecklaceOfShield && pUser->m_dwShieldTime != 0)		dShield = (double)(iInitDamage * 0.3);
	else if(pUser->m_bNecklaceOfShield && pUser->m_dwBigShieldTime != 0)	dShield = (double)(iInitDamage * 0.35);
	else if(pUser->m_bNecklaceOfShield || pUser->m_dwShieldTime != 0 )	dShield = (double)(iInitDamage * 0.2);
    else if(pUser->m_dwBigShieldTime !=0 && !pUser->m_bNecklaceOfShield ) dShield = (double)(iInitDamage * 0.25);
	if(pUser->m_bNecklaceOfShield) pUser->SendAccessoriDuration(SID_NECKLACE_OF_SHIELD);

	if( pUser->m_dwAdamantineTime != 0 )
	{
		dAdamantine = (double)( (double)iDefense * 0.1 );
	}

	iDefense = (int)( iDefense + dABDefense + dDefenseUP + dAdamantine );

	iFinalDamage = (int)(iInitDamage - (iDefense + iBasic + dShield + dGuard)); 

	if(iFinalDamage < 0) iFinalDamage = 0;
	if(iFinalDamage <= 15)
	{
		iFinalTemp = iFinalDamage;
		iFinalDamage += (int)((double)iInitDamage * 0.2 + 1.5);	// 弥家措固瘤甫 弥措 15栏肺 茄促.
		if(iFinalDamage > 15) iFinalDamage = 15;
		iFinalDamage = max(iFinalDamage, iFinalTemp);
	}

	if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) iFinalDamage += 10;

	if(iCAttack > 0 && type) 
	{
		iCA = iCAttack;		// 弥辆 馆拜 单固瘤

		if(iCA > 0)			// 馆拜捞 0焊促 努锭 蜡历 府胶飘俊 歹窍绊 盒硅...
		{
//			pUser->SetCounterAttack(m_sNid, iCA);
			
			// alisia
			int iDamage = iCA;

			if( pUser->GetDistance(m_sCurX, m_sCurY, 2)  || pUser->m_dwFANTAnTime != 0 ) 

			{
				if( SetDamage(iDamage, (pUser->m_uid) + USER_BAND, pUser->m_pCom) == FALSE )
				{
					SendExpToUserList( pUser->m_pCom ); // 版氰摹 盒硅!!
					SendDead( pUser->m_pCom );

					if( m_NpcVirtualState == NPC_STANDING )
					{
						CheckMaxValue( pUser->m_dwXP, 1);	// 各捞 磷阑锭父 1 刘啊!	
						pUser->SendXP();
					}
				}
			     pUser->SendDamageNum(1,m_sNid + NPC_BAND,(short)iDamage);//拳反击显数字
			     pUser->SendNpcHP(m_sNid + NPC_BAND,m_sHP);	
			}

		}
	}

	return iFinalDamage;
}
//
int CNpc::SetHuFaFinalDamage(USER *pUser, int iDamage)
{
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) 
		return 0;

	if( SetDamage(iDamage, (pUser->m_uid) + USER_BAND, pUser->m_pCom) == FALSE )
	{
		SendExpToUserList( pUser->m_pCom ); 
		SendDead( pUser->m_pCom );

		if( m_NpcVirtualState == NPC_STANDING )
		{
			CheckMaxValue( pUser->m_dwXP, 1);	// 各捞 磷阑锭父 1 刘啊!	
			pUser->SendXP();
		}
	}


	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
//	NPC 沥焊甫 滚欺俊 历厘茄促.
//  NPC模型
void CNpc::FillNpcInfo(char *temp_send, int &index, BYTE flag)
{
	CPoint t;

	SetByte(temp_send, NPC_INFO, index );
	SetByte(temp_send, flag, index );
	SetShort(temp_send, m_sNid+NPC_BAND, index );

	if(flag != INFO_MODIFY)	return;

	SetShort(temp_send, m_sPid, index);
	SetVarString(temp_send, m_strName, _tcslen(m_strName), index);

	t = ConvertToClient(m_sCurX, m_sCurY);
	
	SetShort(temp_send, t.x, index);
	SetShort(temp_send, t.y, index);

	if(m_sHP <= 0) SetByte(temp_send, 0x00, index);
	else SetByte(temp_send, 0x01, index);

	SetByte(temp_send, m_tNpcType, index);
	SetInt(temp_send, m_sMaxHP, index);
	SetInt(temp_send, m_sHP, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetByte(temp_send , 0x00, index);
	SetShort(temp_send, m_sClientSpeed, index);

	SetByte(temp_send, m_byColor, index);

	if(m_tNpcType == NPCTYPE_GUILD_DOOR) 
	{
		SetShort(temp_send, m_sDimension, index);
	}
	SetShort(temp_send, m_sQuestSay, index);
}

///////////////////////////////////////////////////////////////////////////////////////
//	辑滚谅钎甫 努扼捞攫飘 谅钎肺 官槽促.
//
CPoint CNpc::ConvertToClient(int x, int y)
{
	if(!g_zone[m_ZoneIndex]) return CPoint(-1,-1);

	int tempx, tempy;
	int temph = g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cy / 2 - 1;

	if( y >= g_zone[m_ZoneIndex]->m_sizeMap.cy || x >= g_zone[m_ZoneIndex]->m_sizeMap.cx ) return CPoint(-1,-1);

	tempx = x - temph + y;
	tempy = y - x + temph;

	return CPoint( tempx, tempy );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	老沥 康开狼 蜡历俊霸 单捞磐甫 焊辰促.
//
void CNpc::SendToRange(COM *pCom, char *temp_send, int index, int min_x, int min_y, int max_x, int max_y)
{
/*
	if( index <= 0 || index >= SEND_BUF_SIZE ) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;

	if( !pNewData ) return;

	pNewData->flag = SEND_RANGE;
	pNewData->len = index;

	::CopyMemory( pNewData->pBuf, temp_send, index );

	pNewData->uid = 0;
	pNewData->z = m_sCurZ;
	pNewData->rect.left		= min_x;
	pNewData->rect.right	= max_x;
	pNewData->rect.top		= min_y;
	pNewData->rect.bottom	= max_y;
	pNewData->zone_index = m_ZoneIndex;

	EnterCriticalSection( &(pCom->m_critSendData) );

	pCom->m_arSendData.Add( pNewData );

	LeaveCriticalSection( &(pCom->m_critSendData) );

	PostQueuedCompletionStatus( pCom->m_hSendIOCP, 0, 0, NULL );
*/

	if( index <= 0 || index >= SEND_BUF_SIZE ) return;

	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return;
	
	int tmin_x = min_x;		if(tmin_x < 0 ) tmin_x = 0;
	int tmax_x = max_x;		if(tmax_x >= pMap->m_sizeMap.cx ) tmax_x = pMap->m_sizeMap.cx - 1;
	int tmin_y = min_y;		if(tmin_y < 0 ) tmin_y = 0;
	int tmax_y = max_y;		if(tmax_y >= pMap->m_sizeMap.cy ) tmax_y = pMap->m_sizeMap.cy - 1;

	int temp_uid;
	USER* pUser = NULL;

	for( int i = tmin_x; i < tmax_x; i++ )
	{
		for( int j = tmin_y; j < tmax_y; j++ )
		{
			temp_uid = pMap->m_pMap[i][j].m_lUser;

			if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
			else temp_uid -= USER_BAND;

			if( temp_uid >= 0 && temp_uid < MAX_USER )
			{
				pUser = pCom->GetUserUid(temp_uid);
				if ( pUser == NULL ) continue;
				
				if( pUser->m_state == STATE_GAMESTARTED )
				{
					if( pUser->m_curx == i && pUser->m_cury == j && pUser->m_curz == m_sCurZ )
					{
						Send( pUser, temp_send, index);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//	傍拜措惑(Target)阑 檬扁拳 茄促.
//
inline void CNpc::InitTarget()
{
	m_Target.id = -1;
	m_Target.x = 0;
	m_Target.y = 0;
	m_Target.failCount = 0;
//	m_Target.nLen = 0; 
//	::ZeroMemory(m_Target.szName, sizeof(m_Target.szName));
}

/////////////////////////////////////////////////////////////////////////////////////////
//	PathFind 甫 荐青茄促.
//
BOOL CNpc::PathFind(CPoint start, CPoint end)
{
	m_bRandMove = FALSE;

	if(start.x < 0 || start.y < 0 || end.x < 0 || end.y < 0)
	{
		return FALSE;
	}

	int i, j;

	int min_x, max_x;
	int min_y, max_y;

	min_x = m_min_x;
	min_y = m_min_y;
	max_x = m_max_x;
	max_y = m_max_y;

	if(InterlockedCompareExchange((LONG*)&m_lMapUsed, (long)1, (long)0) == (long)0)
	{
		ClearPathFindData();

		m_vMapSize.cx = max_x - min_x + 1;		
		m_vMapSize.cy = max_y - min_y + 1;

	
/*		m_pMap = new int*[m_vMapSize.cx];

		for(i = 0; i < m_vMapSize.cx; i++)
		{
			m_pMap[i] = new int[m_vMapSize.cy];
		}
*/
		for(i = 0; i < m_vMapSize.cy; i++)
		{
			for(j = 0; j < m_vMapSize.cx; j++)
			{
				if( min_x+j == m_sCurX && min_y+i == m_sCurY )
				{
					m_pMap[j*m_vMapSize.cy + i] = 0;
//					m_pMap[j][i] = 0;
				}
				else
				{
					if(m_pOrgMap[min_x + j][min_y + i].m_bMove || m_pOrgMap[min_x + j][min_y + i].m_lUser != 0 )
					{
//						m_pMap[j][i] = 1;
						m_pMap[j*m_vMapSize.cy + i] = 1;
					}
					else
					{
//						m_pMap[j][i] = 0;
						m_pMap[j*m_vMapSize.cy + i] = 0;
					}
				}
			}
		}

		m_vStartPoint  = start;		m_vEndPoint = end;
		m_pPath = NULL;
		m_vPathFind.SetMap(m_vMapSize.cx, m_vMapSize.cy, m_pMap);

		m_pPath = m_vPathFind.FindPath(end.x, end.y, start.x, start.y);

		::InterlockedExchange(&m_lMapUsed, 0);

		if(m_pPath)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else return FALSE; 

}

/////////////////////////////////////////////////////////////////////////////
//	菩胶 颇牢靛俊辑 茫篮 版肺甫 促 捞悼 沁绰瘤 眉农
//
BOOL CNpc::IsStepEnd()
{
	if( !m_pPath )	return FALSE;

	if( m_NpcState != NPC_MOVING && m_NpcState != NPC_TRACING ) return FALSE;

	if( !m_pPath->Parent )	return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	NPC 扁夯沥焊 檬扁拳
//
void CNpc::Init()
{		
	int i, j;
	m_dLastFind = GetTickCount();
	m_Delay = 0;
	m_dwLastThreadTime = GetTickCount();

	if((m_sOrgX + m_sOrgY) % 2 != 0) m_sOrgX++;
	CPoint pt = ConvertToServer(m_sOrgX, m_sOrgY);

	if(pt.x == -1 || pt.y == -1)
	{
		CString szTemp;
		szTemp.Format(_T("Invalid NPC AXIS : Name = %s, x = %d, y = %d"), m_strName, m_sOrgX, m_sOrgY);
		AfxMessageBox(szTemp);
		InterlockedIncrement(&g_CurrentNPCError);
	}
	else
	{
		m_sTableOrgX = m_sOrgX = pt.x;
		m_sTableOrgY = m_sOrgY = pt.y;

		m_NpcVirtualState = NPC_STANDING;
		
		if(m_sGuild >= NPC_GUILDHOUSE_BAND)			// 档矫甫 扁霖栏肺 0锅 档矫, 1, 2 捞繁侥栏肺 柳青 m_sGuild = 10000(贸澜)
		{											// 0锅档矫 = 荤唱靛 1锅档矫 = 器飘矫... 捞扒 10000锅措烙
			int index = 0;
			index = GetCityNumForVirtualRoom(m_sCurZ);
			
			if(index >= 0) g_arGuildHouseWar[index]->m_CurrentGuild.arNpcList.Add(m_sNid);

			m_NpcVirtualState = NPC_WAIT;
		}
		else if(m_tNpcType == NPCTYPE_MONSTER && m_sGuild >= FORTRESS_BAND && m_sGuild < NPC_GUILDHOUSE_BAND)	// 捞扒 1000锅措烙
		{
			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(!g_arGuildFortress[i]) continue;

				if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
				{
					g_arGuildFortress[i]->m_arViolenceNpcList.Add(m_sNid);

					m_NpcVirtualState = NPC_WAIT;

//					m_pGuardFortress = g_arGuildFortress[i];
					break;
				}
			}			
		}

		switch(m_tNpcType)
		{
		case NPCTYPE_GUILD_GUARD: case NPCTYPE_GUILD_NPC:
			{
				if(m_sGuild < FORTRESS_BAND)				// 惑痢俊 加茄 版厚捍捞搁
				{
					CStore *pStore = NULL;
					for(i = 0; i < g_arStore.GetSize(); i++)
					{
						if(g_arStore[i] == NULL) continue;

						pStore = g_arStore[i];					// 惑痢 器牢磐甫 掘绢柯促.(辨靛傈俊辑 静扁困秦)
						if(pStore->m_sStoreID == (short)m_sGuild) 
						{	
							pStore->m_arNpcList.Add(m_sNid);	// 郴 酒捞叼甫 惑痢俊 殿废茄促.
							m_pGuardStore = pStore;	
							break; 
						}
					}
				}
				else											// 夸货俊 加茄 NPC捞搁...
				{
					for(i = 0; i < g_arGuildFortress.GetSize(); i++)
					{
						if(!g_arGuildFortress[i]) continue;

						if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
						{
							if(m_sEZone >= GUILD_FORTRESS_NPC_BAND)
							{
								for(j = 0; j < FORTRESS_TARGET_MAX_NUM; j++)
								{
									if(InterlockedCompareExchange((long*)&g_arGuildFortress[i]->m_arFortressTarget[j].lUsed, (long)1, (long)0) == (long)0)
									{
										g_arGuildFortress[i]->m_arFortressTarget[j].sTargertID = m_sNid;
										break;
									}
								}
							}
							else if(m_sEZone < GUILD_FORTRESS_NPC_BAND && m_sEZone >= GUILD_REPAIR_NPC_BAND)	// 荐府瞪荐 乐绰 巩, 堡急器 
							{
								g_arGuildFortress[i]->m_arRepairNpcList.Add(m_sNid);
							}

							g_arGuildFortress[i]->m_arNpcList.Add(m_sNid);

							m_pGuardFortress = g_arGuildFortress[i];
							break;
						}
					}
				}
			}
			break;

		case NPCTYPE_GUILD_MARK:
			{
				if(m_sGuild >= 0 && m_sGuild < g_arGuildData.GetSize())
				{
					m_sPid = g_arGuildHouse[m_sGuild]->iGuild;		
					g_arGuildHouse[m_sGuild]->iMarkNpc = m_sNid;
					::ZeroMemory(m_strName, sizeof(m_strName));
				
					if( m_sPid >= 0 && m_sPid < g_arGuildData.GetSize())
					{										// 辨靛俊 悼扁拳甫 且鞘夸啊 绝促.
						if(g_arGuildData[m_sPid])
						{
							int nLen = 0;
							
							m_sMaxHP = g_arGuildData[m_sPid]->m_sVersion;
							nLen = strlen(g_arGuildData[m_sPid]->m_strGuildName);
							if(nLen > 0)
							{							
								strncpy(m_strName, g_arGuildData[m_sPid]->m_strGuildName, nLen);
							}
						}
					}				
				}
			}
			break;

		case NPCTYPE_GUILD_DOOR:
			{
				for(i = 0; i < g_arGuildFortress.GetSize(); i++)
				{
					if(!g_arGuildFortress[i]) continue;

					if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
					{
						if(m_sEZone < GUILD_FORTRESS_NPC_BAND && m_sEZone >= GUILD_REPAIR_NPC_BAND)	// 荐府瞪荐 乐绰 巩, 堡急器 
						{
							g_arGuildFortress[i]->m_arRepairNpcList.Add(m_sNid);
						}

						g_arGuildFortress[i]->m_arNpcList.Add(m_sNid);

						m_pGuardFortress = g_arGuildFortress[i];
						break;
					}
				}
			}
/*
		case NPCTYPE_FORTRESS:
			{
				for(i = 0; i < g_arGuildFortress.GetSize(); i++)
				{
					if(!g_arGuildFortress[i]) continue;

					if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
					{
						for(j = 0; j < FORTRESS_TARGET_MAX_NUM; j++)
						{
							if(InterlockedCompareExchange(&g_arGuildFortress[i]->m_arFortressTarget[j].lUsed, (LONG)1, (LONG)0) == 0)
							{
								g_arGuildFortress[i]->m_arFortressTarget[j].bChange = FALSE;
								g_arGuildFortress[i]->m_arFortressTarget[j].sTargertID = m_sNid;
								m_pGuardFortress = g_arGuildFortress[i];
								break;
							}
						}
					}
				}
			}
			break;
/*
		case NPCTYPE_REPAIR_GUARD:
			{
				for(i = 0; i < g_arGuildFortress.GetSize(); i++)
				{
					if(!g_arGuildFortress[i]) continue;

					if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
					{
						g_arGuildFortress[i]->m_arRepairNpcList.Add(m_sNid);
						m_pGuardFortress = g_arGuildFortress[i];
						break;
					}
				}
			}
			break;
*/		}
/*
	else
	{
		m_sOrgX = pt.x;
		m_sOrgY = pt.y;

		m_NpcVirtualState = NPC_STANDING;

		if(m_sGuild >= NPC_GUILDHOUSE_BAND)			// 档矫甫 扁霖栏肺 0锅 档矫, 1, 2 捞繁侥栏肺 柳青 m_sGuild = 10000(贸澜)
		{											// 0锅档矫 = 荤唱靛 1锅档矫 = 器飘矫...
			int index = 0;
			index = GetCityNumForVirtualRoom(m_sCurZ);
			
			g_arGuildHouseWar[index]->m_CurrentGuild.arNpcList.Add(m_sNid);

			m_NpcVirtualState = NPC_WAIT;
		}		
		else if(m_tNpcType == NPCTYPE_GUILD_GUARD || m_tNpcType == NPCTYPE_GUILD_NPC)
		{
			if(m_sGuild < FORTRESS_BAND)				// 惑痢俊 加茄 版厚捍捞搁
			{
				CStore *pStore = NULL;
				for(i = 0; i < g_arStore.GetSize(); i++)
				{
					if(g_arStore[i] == NULL) continue;

					pStore = g_arStore[i];					// 惑痢 器牢磐甫 掘绢柯促.(辨靛傈俊辑 静扁困秦)
					if(pStore->m_sStoreID == (short)m_sGuild) 
					{	
						pStore->m_arNpcList.Add(m_sNid);	// 郴 酒捞叼甫 惑痢俊 殿废茄促.
						m_pGuardStore = pStore;	
						break; 
					}
				}
			}
			else											// 夸货俊 加茄 NPC捞搁...
			{
				for(i = 0; i < g_arGuildFortress.GetSize(); i++)
				{
					if(!g_arGuildFortress[i]) continue;

					if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
					{
						g_arGuildFortress[i]->m_arNpcList.Add(m_sNid);
						m_pGuardFortress = g_arGuildFortress[i];
						break;
					}
				}
			}
		}
		else if(m_tNpcType == NPCTYPE_GUILD_MARK)
		{
			if(m_sGuild >= 0 && m_sGuild < g_arGuildData.GetSize())
			{
				m_sPid = g_arGuildHouse[m_sGuild]->iGuild;		
				g_arGuildHouse[m_sGuild]->iMarkNpc = m_sNid;
				::ZeroMemory(m_strName, sizeof(m_strName));
			
				if( m_sPid >= 0 && m_sPid < g_arGuildData.GetSize())
				{										// 辨靛俊 悼扁拳甫 且鞘夸啊 绝促.
					if(g_arGuildData[m_sPid])
					{
						int nLen = 0;
						
						m_sMaxHP = g_arGuildData[m_sPid]->m_sVersion;
						nLen = strlen(g_arGuildData[m_sPid]->m_strGuildName);
						if(nLen > 0)
						{							
							strncpy(m_strName, g_arGuildData[m_sPid]->m_strGuildName, nLen);
						}
					}
				}				
			}
		}
		else if(m_tNpcType == NPCTYPE_FORTRESS)
		{
			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(!g_arGuildFortress[i]) continue;

				if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
				{
					for(j = 0; j < FORTRESS_TARGET_MAX_NUM; j++)
					{
						if(InterlockedCompareExchange(&g_arGuildFortress[i]->m_arFortressTarget[j].lUsed, (LONG)1, (LONG)0) == 0)
						{
							g_arGuildFortress[i]->m_arFortressTarget[j].bChange = FALSE;
							g_arGuildFortress[i]->m_arFortressTarget[j].sTargertID = m_sNid;
							m_pGuardFortress = g_arGuildFortress[i];
							break;
						}
					}
				}
			}
		}
		else if(m_tNpcType == NPCTYPE_REPAIR_GUARD)
		{
			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(!g_arGuildFortress[i]) continue;

				if(g_arGuildFortress[i]->m_sFortressID == (short)(m_sGuild))
				{
					g_arGuildFortress[i]->m_arRepairNpcList.Add(m_sNid);
					m_pGuardFortress = g_arGuildFortress[i];
					break;
				}
			}
		}
*/	}

	m_pOrgMap = g_zone[m_ZoneIndex]->m_pMap;	// MapInfo 沥焊 悸泼
}

int CNpc::GetCityNumForVirtualRoom(int zone)		// 瘤陛篮 档矫锅龋瘤父 唱吝俊 VirtualRoom捞 拌加 眠啊登搁..
{													// 官操绢具 等促. (int zone, int &curGuild)
	int nRet = -1;

	switch(zone)									// 眠啊且 抗沥烙...
	{
	case 1005:										// 1004粮 捞搁..
		nRet = SANAD;								// 档矫绰 荤唱靛, m_CurrentGuild = 0锅掳
		break;

	default:
		break;
	}

	return nRet;
}

////////////////////////////////////////////////////////////////////////////////////
//	Client 谅钎甫 辑滚谅钎肺 函券茄促
//
CPoint CNpc::ConvertToServer(int x, int y)
{
	if(!g_zone[m_ZoneIndex]) return CPoint(-1,-1); 

	int tempx, tempy;
	int temph = g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cy / 2 - 1;

	if( y >= g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cy || x >= g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cx ) return CPoint(-1,-1);

	if( (x+y)%2 == 0 )
	{
		tempx = temph - ( y / 2 ) + ( x / 2 );

		if( x % 2 ) tempy = ( y / 2 ) + ( ( x / 2 ) + 1 );
		else        tempy = ( y / 2 ) + ( x / 2 );

		return CPoint( tempx, tempy );
	}
	else return CPoint(-1,-1);
}

////////////////////////////////////////////////////////////////////////////////////
//	叼滚彪阑 困秦 NPC 沥焊甫 TRACE 茄促.
//
void CNpc::NpcTrace(TCHAR *pMsg)
{
	if(g_bDebug == FALSE) return;

	CString szMsg = _T("");
	CPoint pt = ConvertToClient(m_sCurX, m_sCurY);
	szMsg.Format(_T("%s : uid = %d, name = %s, xpos = %d, ypos = %d\n"), pMsg, m_sNid, m_strName, pt.x, pt.y);
	TRACE(szMsg);
}

///////////////////////////////////////////////////////////////////////////////////
//	傍拜阑 寸沁阑锭 菩胶颇牢爹俊 角菩窍搁 蜡历俊霸 啊鳖款率栏肺 框流牢促.
//
void CNpc::ToTargetMove(COM *pCom, USER *pUser)
{
	if(!pCom) return;
	if(!pUser) return;
	if(!g_zone[m_ZoneIndex]) return;

	int xx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int yy[] = {0, -1, -1, -1, 0, 1, 1, 1};

	CPoint ptUser = ConvertToClient(pUser->m_curx, pUser->m_cury);

	struct _min
	{
		int x, y;
		int value;
	}min;

	int minindex;
	int i, j;

	int dx, dy;
	CPoint ptNew;
	int max_dist;
/*
	int test1[8], test2[8];

	for(i = 0; i < 8; i++)
	{
		ptNew = ConvertToClient(m_sCurX + xx[i], m_sCurY + yy[i]);
		dx = abs(ptUser.x - ptNew.x);
		dy = abs(ptUser.y - ptNew.y);
		test1[i] = dx + dy;
	}
*/	
	for(i = 0; i < sizeof(xx)/sizeof(xx[0]) - 1; i++)
	{
		minindex = i;
		
		ptNew = ConvertToClient(m_sCurX + xx[i], m_sCurY + yy[i]);

		if(ptNew.x <= -1 || ptNew.y <= -1) continue;
		if(ptNew.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptNew.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) continue;

		dx = abs(ptUser.x - ptNew.x);
		dy = abs(ptUser.y - ptNew.y);
		max_dist = dx + dy;

		min.value = max_dist;
		min.x = xx[i];
		min.y = yy[i];

		for(j = i + 1; j < sizeof(xx)/sizeof(xx[0]); j++)
		{
			ptNew = ConvertToClient(m_sCurX + xx[j], m_sCurY + yy[j]);

			if(ptNew.x <= -1 || ptNew.y <= -1) continue;
			if(ptNew.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptNew.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) continue;

			dx = abs(ptUser.x - ptNew.x);
			dy = abs(ptUser.y - ptNew.y);
			max_dist = dx + dy;

			if(min.value > max_dist)
			{
				min.value = max_dist;
				min.x = xx[j];
				min.y = yy[j];
				minindex = j;
			}
		}

		xx[minindex] = xx[i];
		yy[minindex] = yy[i];

		xx[i] = min.x;
		yy[i] = min.y;
	}
/*
	for(i = 0; i < 8; i++)
	{
		ptNew = ConvertToClient(m_sCurX + xx[i], m_sCurY + yy[i]);
		dx = abs(ptUser.x - ptNew.x);
		dy = abs(ptUser.y - ptNew.y);
		test2[i] = dx + dy;
	}
*/
	MAP* pMap = g_zone[m_ZoneIndex];
	CPoint ptPre(m_sCurX, m_sCurY);
	int will_x, will_y;
	BOOL bMove = FALSE;
	int new_dist = 0, cur_dist = 0;

	CPoint ptCurr = ConvertToClient(m_sCurX, m_sCurY);
	cur_dist = abs(ptUser.x - ptCurr.x) + abs(ptUser.y - ptCurr.y);

	for(i = 0; i < sizeof(xx)/sizeof(xx[0]); i++)
	{
		will_x = m_sCurX + xx[i];
		will_y = m_sCurY + yy[i];

		ptNew = ConvertToClient(m_sCurX + xx[i], m_sCurY + yy[i]);
		new_dist = abs(ptUser.x - ptNew.x) + abs(ptUser.y - ptNew.y);

		if(new_dist > cur_dist) continue;

		if(will_x <= -1 || will_y <= -1) continue;
		if(will_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || will_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) continue;

		if(pMap->m_pMap[will_x][will_y].m_bMove != 0 || pMap->m_pMap[will_x][will_y].m_lUser != 0)
		{
			continue;
		}
		else
		{
			if(InterlockedCompareExchange((LONG*)&m_pOrgMap[will_x][will_y].m_lUser,
				(long)m_pOrgMap[m_sCurX][m_sCurY].m_lUser, (long)0) == (long)0)
			{
				::InterlockedExchange(&m_pOrgMap[m_sCurX][m_sCurY].m_lUser, 0);
				m_sCurX = will_x;
				m_sCurY = will_y;
				SightRecalc(pCom);
				bMove = TRUE;
				break;
			}
			else continue;
		}
	}

	if(!bMove) return;

	CBufferEx TempBuf;
	CPoint t = ConvertToClient(m_sCurX, m_sCurY);		// 框流捞妨绰 辑滚谅钎啊 努扼捞攫飘俊辑 给框流捞绰 谅钎搁 府畔
	if(t.x <= -1 || t.y <= -1) return;

	TempBuf.Add(MOVE_RESULT);

	TempBuf.Add(SUCCESS);
	TempBuf.Add((int)(NPC_BAND + m_sNid));
	TempBuf.Add((short)t.x);
	TempBuf.Add((short)t.y);

	SendInsight(pCom, TempBuf, TempBuf.GetLength());

	m_Delay = m_sSpeed;
}



void CNpc::EventNpcInit(int x, int y)
{
	m_dwLastThreadTime = GetTickCount();

	m_sOrgX = x;
	m_sOrgY = y;

	m_pOrgMap = g_zone[m_ZoneIndex]->m_pMap;	// MapInfo 沥焊 悸泼

	m_Delay = 0;
}

///////////////////////////////////////////////////////////////////////////////////
//	辨靛傈阑 困秦 鸥涝阑 官槽促. 
//
void CNpc::SetGuildType(COM *pCom)
{
/*	int modify_index = 0;
	char modify_send[2048];	

	::ZeroMemory(modify_send, sizeof(modify_send));

	if(m_tGuildWar == GUILD_WARRING)
	{
		m_tNpcAttType = 1;
		if(m_tNpcType == NPCTYPE_GUARD) m_tNpcType = NPCTYPE_GUILD_GUARD;
		else if(m_tNpcType == NPCTYPE_NPC) m_tNpcType = NPCTYPE_GUILD_NPC;

		// 蜡历俊霸 NPC 沥焊傈价...
		FillNpcInfo(modify_send, modify_index, INFO_MODIFY);
		SendInsight(pCom, modify_send, modify_index);
	}
	else if(m_tGuildWar == GUILD_WAR_AFFTER)
	{
		m_tNpcAttType = 1;
		if(m_tNpcType == NPCTYPE_GUILD_GUARD) m_tNpcType = NPCTYPE_GUARD;
		else if(m_tNpcType == NPCTYPE_GUILD_NPC) m_tNpcType = NPCTYPE_NPC;

		FillNpcInfo(modify_send, modify_index, INFO_MODIFY);
		SendInsight(pCom, modify_send, modify_index);
	}
*/
}

void CNpc::SetDamagedInGuildWar(int nDamage, USER *pUser)// COM *pCom)
{
	int i, j;
	BOOL flag = FALSE;

	CNpc *pNpc = NULL;
//	int uid = uuid - USER_BAND;
//	USER* pUser = GetUser(pCom, uid);
														// 辨靛傈阑 脚没茄 辨靛牢瘤 魄窜.
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_dwGuild <= 0 || pUser->m_tGuildWar == GUILD_WAR_AFFTER) return;
	if(pUser->m_dwGuild == m_pGuardStore->m_iGuildSid) return;	// 规绢螟捞 绊狼肺 傍拜窍绰巴阑 规瘤茄促.

	if(m_pGuardStore->m_lUsed == 0) return;				// 辨靛傈捞 场车促.

	for(j = 0; j < GUILD_ATTACK_MAX_NUM; j++)
	{
		if(pUser->m_dwGuild != m_pGuardStore->m_arAttackGuild[j]) continue;

		m_sHP -= nDamage;
		if( m_sHP <= 0 )								// 咯扁俊辑 辨靛傈捞 场抄促.
		{
			m_sHP = m_sMaxHP;
			if(InterlockedCompareExchange((LONG*)&m_pGuardStore->m_lUsed, (long)0, (long)1) == (long)1)
			{											// 1俊辑 0栏肺 父甸绊 秦寸 惑痢傈里篮 场车澜阑 舅赴促.
				if(pUser->StoppingTheGuildWar(m_pGuardStore))
				{										// 秦寸 NPC俊霸 舅妨霖促.
					for(i =0; i < m_pGuardStore->m_arNpcList.GetSize(); i++)
					{
						pNpc = GetNpc(m_pGuardStore->m_arNpcList[i]);
						if(pNpc) 
						{
							pNpc->m_tGuildWar = GUILD_WAR_AFFTER;
							pNpc->m_tNpcAttType = 0;
						}
					}
					m_tGuildWar = GUILD_WAR_AFFTER;
					flag = TRUE;
					break;
				}
			}
		}
	}

	if(flag)
	{
		for(j = 0; j < GUILD_ATTACK_MAX_NUM; j++)
		{
			m_pGuardStore->m_arAttackGuild[j] = 0;
		}
	}

	return;
}

void CNpc::Send(USER *pUser, TCHAR *pBuf, int nLength)
{
	if ( !pUser ) return;

	pUser->Send( pBuf, nLength );
}

///////////////////////////////////////////////////////////////////////////////////
//	烙矫 捞亥飘 内靛烙 (扁埃 : 2001斥 12岿 29老 ~~ 2002斥 1岿 2老)
//
//@@@@@@@@@@@@@@@@@@@@@@@@
void CNpc::GiveEventItemToUser(USER *pUser)
{
	return;

	BOOL bFlag = FALSE;

	int iEventItemSid = 0;
	int iEventNum = -1;
	int iEvent = 0;
	int iSlot = 0;
	
	int j;

	SYSTEMTIME gTime;
	GetLocalTime(&gTime);
//	if(gTime.wYear == 2002 && gTime.wDay >= 2) return;

//	if(gTime.wMonth != 2) return;					// 2岿 汲朝侩 捞亥飘
	if(gTime.wDay < 7 || gTime.wDay > 13) return;// 8老何磐 13老鳖瘤	

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	
	if(abs(m_byClassLevel - pUser->m_sLevel) > 25)
	{
		if(m_byClassLevel < pUser->m_sLevel) return;
	}

	iEventNum = GetEventItemNum(pUser->m_pCom);

	if(iEventNum < 0) return;
	
	int type = (int)g_arAddEventItemTable[iEventNum]->m_tType;
	if(type < 100 || type > 255) return;

	if(!UpdateEventItem(iEventNum)) 
	{
		g_arAddEventItemTable[iEventNum]->m_tEnd = 0;
		return;
	}

	CString strMsg = _T("");

	iSlot = pUser->GetEmptySlot(INVENTORY_SLOT);

	if(iSlot != -1)
	{
		if(NPC_EVENT_ITEM >= g_arItemTable.GetSize())
		{
			int ttt = 0;
		}
		if(pUser->m_iMaxWeight >= pUser->m_iCurWeight + g_arItemTable[NPC_EVENT_ITEM]->m_byWeight) bFlag = TRUE;
	}

	switch(type)
	{
/*	case 1:
		if(bFlag) { iEvent = EVENT_SP1_ITEM; strMsg.Format("泅犁 %s丛膊辑 归拳痢 惑前鼻甫 掘栏继嚼聪促.", pUser->m_strUserID); }
		else      iEvent = 1001;
		break;
	case 2:
		if(bFlag) { iEvent = EVENT_SP2_ITEM; strMsg.Format("泅犁 %s丛膊辑 巩拳 惑前鼻甫 掘栏继嚼聪促.", pUser->m_strUserID); }
		else      iEvent = 1002;
		break;
	case 3:
		if(bFlag) { iEvent = EVENT_DEF_ITEM; strMsg.Format("泅犁 %s丛膊辑 规绢备 背券鼻甫 掘栏继嚼聪促.", pUser->m_strUserID); }
		else      iEvent = 1003;
		break;
	case 4:
		if(bFlag) { iEvent = EVENT_ATT_ITEM; strMsg.Format("泅犁 %s丛膊辑 公扁 背券鼻甫 掘栏继嚼聪促.", pUser->m_strUserID); }
		else      iEvent = 1004;
		break;
	case 5:
		if(bFlag) { iEvent = EVENT_POT_ITEM; strMsg.Format("泅犁 %s丛膊辑 拱距 背券鼻甫 掘栏继嚼聪促.", pUser->m_strUserID); }
		else      iEvent = 1005;
		break;
*/
	case EVENT_ATT7_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT7_ITEM; strMsg.Format(IDS_EVENT_ATT7_ITEM, pUser->m_strUserID); }
		else      iEvent = 1001;
		break;
	case EVENT_DEF7_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF7_ITEM; strMsg.Format(IDS_EVENT_DEF7_ITEM, pUser->m_strUserID); }
		else      iEvent = 1002;
		break;
	case EVENT_ATT6_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT6_ITEM; strMsg.Format(IDS_EVENT_ATT6_ITEM, pUser->m_strUserID); }
		else      iEvent = 1003;
		break;
	case EVENT_DEF6_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF6_ITEM; strMsg.Format(IDS_EVENT_DEF6_ITEM, pUser->m_strUserID); }
		else      iEvent = 1004;
		break;
	case EVENT_ATT_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT_ITEM; strMsg.Format(IDS_EVENT_ATT5_ITEM, pUser->m_strUserID); }
		else      iEvent = 1005;
		break;
	case EVENT_DEF_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF_ITEM; strMsg.Format(IDS_EVENT_DEF5_ITEM, pUser->m_strUserID); }
		else      iEvent = 1006;
		break;
	case EVENT_ATT4_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT4_ITEM; strMsg.Format(IDS_EVENT_ATT4_ITEM, pUser->m_strUserID); }
		else      iEvent = 1007;
		break;
	case EVENT_DEF4_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF4_ITEM; strMsg.Format(IDS_EVENT_DEF4_ITEM, pUser->m_strUserID); }
		else      iEvent = 1008;
		break;
	case EVENT_ATT3_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_ATT3_ITEM; strMsg.Format(IDS_EVENT_ATT3_ITEM, pUser->m_strUserID); }
		else      iEvent = 1009;
		break;
	case EVENT_DEF3_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_DEF3_ITEM; strMsg.Format(IDS_EVENT_DEF3_ITEM, pUser->m_strUserID); }
		else      iEvent = 1010;
		break;

	case EVENT_INIT_STAT_ITEM:
		if(bFlag) { iEventItemSid = NPC_EVENT_INIT_STAT; iEvent = EVENT_INIT_STAT_ITEM; strMsg.Format(IDS_EVENT_RESET_STAT, pUser->m_strUserID); }
		else      iEvent = 1011;
		break;

	case EVENT_USER_GAME_TIME:
		if(bFlag) { iEventItemSid = NPC_EVENT_ITEM; iEvent = EVENT_USER_GAME_TIME; strMsg.Format(IDS_EVENT_PERSONAL, pUser->m_strUserID); }
		else      iEvent = 1012;
		break;
		break;
	default:
		return;
		break;
	}

	if(bFlag)								// 磊悼栏肺 牢亥俊 茫酒辑 甸绢埃促.
	{
		if(iEventItemSid == NPC_EVENT_ITEM || iEventItemSid == NPC_EVENT_INIT_STAT)
		{
			pUser->m_UserItem[iSlot].tType = TYPE_ITEM;
			pUser->m_UserItem[iSlot].sLevel = g_arItemTable[iEventItemSid]->m_byRLevel;
			pUser->m_UserItem[iSlot].sSid = g_arItemTable[iEventItemSid]->m_sSid;
			pUser->m_UserItem[iSlot].sCount = 1;
			pUser->m_UserItem[iSlot].sDuration = g_arItemTable[iEventItemSid]->m_sDuration;
			pUser->m_UserItem[iSlot].sBullNum = g_arItemTable[iEventItemSid]->m_sBullNum;
			pUser->m_UserItem[iSlot].tIQ = (BYTE)iEvent;
			pUser->m_UserItem[iSlot].iItemSerial = 0;

			SetISerialToItem(&pUser->m_UserItem[iSlot], iEventNum);
/*			for(j = 0; j < MAGIC_NUM; j++)
			{
				pUser->m_UserItem[iSlot].tMagic[j] = 0;
				pUser->m_UserItem[iSlot].tMagic[j] = tSerial[j];//g_arAddEventItemTable[iEventNum]->m_strSerialNum[j];
			}
*/
			CBufferEx TempBuf;

			TempBuf.Add(ITEM_LOAD_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add((BYTE)0x01);

			TempBuf.Add((BYTE)iSlot);
			TempBuf.Add(pUser->m_UserItem[iSlot].sLevel);
			TempBuf.Add(pUser->m_UserItem[iSlot].sSid);
			TempBuf.Add(pUser->m_UserItem[iSlot].sDuration);
			TempBuf.Add(pUser->m_UserItem[iSlot].sBullNum);
			TempBuf.Add(pUser->m_UserItem[iSlot].sCount);
			for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)pUser->m_UserItem[iSlot].tMagic[j]);

			TempBuf.Add((BYTE)pUser->m_UserItem[iSlot].tIQ);

			pUser->Send(TempBuf, TempBuf.GetLength());

			pUser->m_iCurWeight += g_arItemTable[iEventItemSid]->m_byWeight;
			pUser->GetRecoverySpeed();			// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券

			pUser->m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL);
//			pUser->m_pCom->Announce(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL);
		}
		return;
	}
											// 牢亥持扁俊 角菩窍搁 甘俊 悸泼
	GiveItemToMap(pUser->m_pCom, iEvent, TRUE, iEventNum);		// 捞亥飘 酒捞袍
}

///////////////////////////////////////////////////////////////////////////////////
//	捞亥飘 内靛烙 (扁埃 : 2002斥 4岿 8老 ~~ 2002斥 4岿 13老)
//
//@@@@@@@@@@@@@@@@@@@@@@@@
void CNpc::SetISerialToItem(ItemList *pItem, int iEventSid)
{
	int i, j = 0;
	TCHAR strTemp[3];

	if(!pItem) return;
	if(iEventSid < 0 || iEventSid >= g_arAddEventItemTable.GetSize()) return;

	for(i = 0; i < MAGIC_NUM; i++)
	{	
		::ZeroMemory(strTemp, sizeof(strTemp));
		strncpy(strTemp,g_arAddEventItemTable[iEventSid]->m_strSerialNum+j, 3);

		pItem->tMagic[i] = 0;
		pItem->tMagic[i] = atoi(strTemp);
		j = j + 4;
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	捞亥飘 内靛烙 (扁埃 : 2002斥 4岿 8老 ~~ 2002斥 4岿 13老)
//
//@@@@@@@@@@@@@@@@@@@@@@@@
int CNpc::GetEventItemNum(COM *pCom)
{
	int i, iRet = -1;
	DWORD dwCurTick = 0;
	DWORD dwPreTick = 0;

	EnterCriticalSection( &(pCom->m_critEvent) );

	dwCurTick = GetTickCount();

	for(i = 0; i < g_arAddEventItemTable.GetSize(); i++)
	{
		int tt = g_arAddEventItemTable[i]->m_tEnd;
		if(!g_arAddEventItemTable[i]->m_tEnd)				// 酒流 冻绢瘤瘤 臼疽促搁
		{
			if(i == 0) dwPreTick = 0;
			else dwPreTick = g_arAddEventItemTable[i - 1]->m_dwTick;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			if(dwCurTick - dwPreTick >= 60000 * 20)		// 24盒捞 逞菌促搁
//			if(dwCurTick - dwPreTick >= 1000)				// 1盒捞 逞菌促搁	
			{
				g_arAddEventItemTable[i]->m_dwTick = dwCurTick;
				g_arAddEventItemTable[i]->m_tEnd = 1;
														// 角力荐父 锅龋甫 悸泼秦辑 霖促.	
				if(g_arAddEventItemTable[i]->m_tGiveFlag)	iRet = g_arAddEventItemTable[i]->m_sSid;
			}

			LeaveCriticalSection( &(pCom->m_critEvent) );
			return iRet;
		}
	}

	LeaveCriticalSection( &(pCom->m_critEvent) );
	return iRet;
}

///////////////////////////////////////////////////////////////////////////////////
//	烙矫 捞亥飘 内靛烙 (扁埃 : 2001斥 12岿 29老 ~~ 2002斥 1岿 2老)
//
//@@@@@@@@@@@@@@@@@@@@@@@@
BOOL CNpc::UpdateEventItem(int sid)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	SQLINTEGER		iRetInd = SQL_NTS;

	SQLSMALLINT		sRet = 0;

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_EVENT_ITEM(%d, ?)}"), sid);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}

	retcode = SQLBindParameter( hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT,0,0, &sRet,0, &iRetInd);
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;
	if(sRet = 0) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	荐府啊 救登绢 乐栏搁 泅犁 HP甫 蜡瘤茄促.	
//
void CNpc::SetFortressState()
{
	for(int i = 0; i < GUILD_REPAIR_MAX_NUM; i++)
	{
		if(m_pGuardFortress->m_arRepairDBList[i].sUid == m_sEZone)
		{
			if(m_pGuardFortress->m_arRepairDBList[i].sHP < m_sMaxHP)
			{
				m_sHP = m_pGuardFortress->m_arRepairDBList[i].sHP;

				if(m_sHP == 0)  m_tRepairDamaged = NPC_DEAD_REPAIR_STATE; 
				else			m_tRepairDamaged = NPC_NEED_REPAIR_STATE;

				break;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	N_Circle俊 措茄 贸府甫 拌魂茄促.
//
//void CNpc::SetDamagedInFortressWar(int nDamage, TCHAR *id, int uuid, COM *pCom)
void CNpc::SetDamagedInFortressWar(int nDamage, USER *pUser)
{
	int i;
	int iCount = 0;
	int index = 0;

	BOOL bSuccess = FALSE;
	
	CBufferEx TempBuf;
	CNpc *pNpc = NULL;
														// 辨靛傈阑 脚没茄 辨靛牢瘤 魄窜.
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_dwGuild <= 0 || !m_pGuardFortress) return;

	if(pUser->m_tFortressWar == GUILD_WAR_AFFTER) return;

	if(m_pGuardFortress->m_lUsed == 0) return;			// 辨靛傈捞 场车促.

	if(m_pGuardFortress->m_lChangeUsed == 1) return;

	for(i = 0; i < GUILDFORTRESS_ATTACK_MAX_NUM; i++)
	{
		if(pUser->m_dwGuild == m_pGuardFortress->m_arAttackGuild[i].lGuild)
		{
			bSuccess = TRUE;
			break;
		}
	}

	if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid) bSuccess = TRUE;
	
	if(!bSuccess) return;								// 脚没茄 辨靛啊 酒丛

	if(InterlockedCompareExchange((LONG*)&m_lFortressState, (long)1, (long)0) == (long)0)
	{
		if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid)	// 规绢螟篮 
		{
			if(m_tNCircle != NPC_NCIRCLE_ATT_STATE)
			{
				InterlockedExchange(&m_lFortressState, (LONG)0); 
				return;
			}

			m_sHP -= nDamage;									// 傍拜螟篮 - 蔼
			if(m_sHP <= 0)
			{
				m_sHP = m_sMaxHP;
				m_byColor = 0;
				m_tNCircle = NPC_NCIRCLE_DEF_STATE;
				SendFortressNCircleColor(pUser->m_pCom);
			}
		}
		else
		{
			if(m_tNCircle != NPC_NCIRCLE_DEF_STATE) 
			{
				InterlockedExchange(&m_lFortressState, (LONG)0); 
				return;
			}

			m_sHP -= nDamage;									// 傍拜螟篮 - 蔼
			if(m_sHP <= 0)
			{
				m_sHP = m_sMaxHP;
				m_byColor = 1;
				m_tNCircle = NPC_NCIRCLE_ATT_STATE;
				SendFortressNCircleColor(pUser->m_pCom);
			}
		}

		iCount = 0;
		for(i = 0; i < FORTRESS_TARGET_MAX_NUM; i++)
		{
			pNpc = NULL;
			pNpc = GetNpc(m_pGuardFortress->m_arFortressTarget[i].sTargertID);	
			if(pNpc)
			{
				if(pNpc->m_tNCircle == NPC_NCIRCLE_ATT_STATE) iCount++; 
			}
		}

		if(iCount == FORTRESS_TARGET_MAX_NUM)
		{
			if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid)
			{
				InterlockedExchange(&m_lFortressState, (LONG)0); 
				return;
			}

			for(i = 0; i < g_arGuildFortress.GetSize(); i++)
			{
				if(!g_arGuildFortress[i]) continue;

				if(g_arGuildFortress[i]->m_sFortressID == m_pGuardFortress->m_sFortressID)
				{
					if(g_arGuildFortress[i]->m_lUsed == 1)
					{
						if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_lChangeUsed, (long)1, (long)0) == (long)0)
						{
							FORTRESSDATAPACKET *pFDP = NULL;
							pFDP = new FORTRESSDATAPACKET;

							pFDP->sFortressIndex = i;
							
							memset(pFDP->FORTRESS, NULL, CHAR_NAME_LENGTH+sizeof(int)+1);
							index = strlen(pUser->m_strGuildName);
							if(index > 0 && index <= CHAR_NAME_LENGTH) memcpy(pFDP->FORTRESS, pUser->m_strGuildName, index );

							EnterCriticalSection( &m_CS_FortressData );
							RecvFortressData.AddTail(pFDP);
 							nFortressDataCount = RecvFortressData.GetCount();
							LeaveCriticalSection( &m_CS_FortressData );	

							pUser->StoppingTheFortressWar(g_arGuildFortress[i]);		// 沥秦柳 矫埃傈捞骨肺 霸烙篮 拌加登绢具 茄促. 
							InterlockedExchange(&g_arGuildFortress[i]->m_lChangeUsed, (LONG)0); 
						}
					}
					break;
				}
			}
		}

		InterlockedExchange(&m_lFortressState, (LONG)0); 
	}

	return;
}

//void CNpc::SetDoorDamagedInFortressWar(int nDamage, TCHAR *id, int uuid, COM *pCom)
void CNpc::SetDoorDamagedInFortressWar(int nDamage, USER *pUser)
{													// 傍己傈 扁埃捞扼搁 穿备唱 傍拜窍档废.. 窜 规绢螟篮 救凳
	if(!pUser || !m_pGuardFortress) return;
	if(pUser->m_dwGuild == m_pGuardFortress->m_iGuildSid) return;	// 规绢螟捞 绊狼肺 傍拜窍绰巴阑 规瘤茄促.

	if(m_pGuardFortress->m_lUsed == 0) return;				// 辨靛傈捞 场车促.

	m_sHP -= nDamage;

	if( m_sHP <= 0 )
	{
		m_sHP = 0;

		MAP* pMap = g_zone[m_ZoneIndex];
		pMap->m_pMap[m_sCurX][m_sCurY].m_lUser = 0;

		m_NpcState = NPC_DEAD;

		m_Delay = m_sRegenTime;
		m_bFirstLive = FALSE;

		SetMapAfterGuildWar();

		SendDead(pUser->m_pCom);
	}
}

void CNpc::SendFortressNCircleColor(COM *pCom)
{
	int modify_index = 0;
	char modify_send[2048];	

	CBufferEx TempBuf;

	TempBuf.Add(GUILD_FORTRESS_NCIRCLE);
	TempBuf.Add((BYTE)0x00);					// 秦寸 N_Circle狼 祸捞 函拳
	TempBuf.Add((int)(m_sNid + NPC_BAND));
	TempBuf.Add(m_tNCircle);

	SendFortressInsight(pCom, TempBuf, TempBuf.GetLength());

	::ZeroMemory(modify_send, sizeof(modify_send));
	FillNpcInfo(modify_send, modify_index, INFO_MODIFY);
	SendFortressInsight(pCom, modify_send, modify_index);

}


void CNpc::SetMapTypeBeforeGuildWar(COM *pCom)
{
	int i;
	int uid = 0;

	USER *pUser = NULL;
	CNpc *pNpc = NULL;

	int x, y;

	long lNpcUid = 0;

//	POINT temp1Map[12] = {{-2,-2}, {-2,-1}, {-2,0}, {-1,-1}, {-1,0}, {0,-1}, {0,0},{1,-1},{1,0}, {2,-1}, {2,0}, {2,1}};
	POINT temp1Map[16] = {{-3,-2}, {-4,-1}, {-3,-1}, {-2,-1}, {-4,0}, {-3,0}, {-2,0},{-1,0},{0,0}, {1,0}, {2,0}, {1,1}, {2,1}, {2,2},{3,2},{3,3}};
	POINT temp2Map[17] = {{-2,-3}, {-1,-3}, {0,-3}, {-1,-2}, {0,-2}, {0,-1}, {0,0},{0,1},{1,1}, {0,2}, {1,2}, {2,2}, {0,3}, {1,3},{2,3},{0,4},{1,4}};
	POINT temp3Map[6] = {{-2,0},{-1,0},{0,0},{1,0},{2,0},{2,1}};
	
//	POINT temp2Map[] = {{1,-2}, {1,-1}, {1,0}, {1,1}, {1,2}, {0,-2},{0,-1},{0,0},{0,1},{0,2}};

	MAP* pMap = g_zone[m_ZoneIndex];

	switch(m_sDimension)
	{
	case 1:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < sizeof(temp1Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp1Map[i].x;
			y = m_sCurY + temp1Map[i].y;
	
			uid = pMap->m_pMap[x][y].m_lUser;

			if(uid >= USER_BAND && uid < NPC_BAND)	// Target 捞 User 牢 版快
			{
				pUser = GetUser(pCom, uid - USER_BAND);
				if( pUser->m_tIsOP != 1 ) pUser->TownPotal();
			}

			::InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, lNpcUid);
		}
		break;
	case 2:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < sizeof(temp2Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp2Map[i].x;
			y = m_sCurY + temp2Map[i].y;
	
			uid = pMap->m_pMap[x][y].m_lUser;

			if(uid >= USER_BAND && uid < NPC_BAND)	// Target 捞 User 牢 版快
			{
				pUser = GetUser(pCom, uid - USER_BAND);
				if( pUser->m_tIsOP != 1 ) pUser->TownPotal();
			}

			::InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, lNpcUid);
		}
		break;
	case 3:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < sizeof(temp3Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp3Map[i].x;
			y = m_sCurY + temp3Map[i].y;
	
			uid = pMap->m_pMap[x][y].m_lUser;

			if(uid >= USER_BAND && uid < NPC_BAND)	// Target 捞 User 牢 版快
			{
				pUser = GetUser(pCom, uid - USER_BAND);
				if( pUser->m_tIsOP != 1 ) pUser->TownPotal();
			}

			::InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, lNpcUid);
		}
		break;

	}
}


void CNpc::SetMapAfterGuildWar()
{
	int i;
	int uid = 0;

	USER *pUser = NULL;

	int x, y;

//	POINT temp1Map[12] = {{-2,-2}, {-2,-1}, {-2,0}, {-1,-1}, {-1,0}, {0,-1}, {0,0},{1,-1},{1,0}, {2,-1}, {2,0}, {2,1}};
//	POINT temp2Map[] = {{1,-2}, {1,-1}, {1,0}, {1,1}, {1,2}, {0,-2},{0,-1},{0,0},{0,1},{0,2}};
	POINT temp1Map[16] = {{-3,-2}, {-4,-1}, {-3,-1}, {-2,-1}, {-4,0}, {-3,0}, {-2,0},{-1,0},{0,0}, {1,0}, {2,0}, {1,1}, {2,1}, {2,2},{3,2},{3,3}};
	POINT temp2Map[17] = {{-2,-3}, {-1,-3}, {0,-3}, {-1,-2}, {0,-2}, {0,-1}, {0,0},{0,1},{1,1}, {0,2}, {1,2}, {2,2}, {0,3}, {1,3},{2,3},{0,4},{1,4}};
	POINT temp3Map[6] = {{-2,0},{-1,0},{0,0},{1,0},{2,0},{2,1}};

	MAP* pMap = g_zone[m_ZoneIndex];

	switch(m_sDimension)
	{
	case 1:
		for(i =0; i < sizeof(temp1Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp1Map[i].x;
			y = m_sCurY + temp1Map[i].y;

			InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, 0);
		}
		break;
	case 2:
		for(i =0; i < sizeof(temp2Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp2Map[i].x;
			y = m_sCurY + temp2Map[i].y;

			InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, 0);
		}
		break;
	case 3:
		for(i =0; i < sizeof(temp3Map)/sizeof(POINT); i++)
		{
			x = m_sCurX + temp3Map[i].x;
			y = m_sCurY + temp3Map[i].y;

			InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, 0);
		}
		break;
	}
}

void CNpc::SendFortressInsight(COM *pCom, TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;

	int insight_range = 10;

	int sx = m_sCurX / SIGHT_SIZE_X;
	int sy = m_sCurY / SIGHT_SIZE_Y;
	
	int min_x = (sx-8)*(SIGHT_SIZE_X); if( min_x < 0 ) min_x = 0;
	int max_x = (sx+9)*(SIGHT_SIZE_X);
	int min_y = (sy-8)*(SIGHT_SIZE_Y); if( min_y < 0 ) min_y = 0;
	int max_y = (sy+9)*(SIGHT_SIZE_Y);
	
	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return;
	
	if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
	if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;
	
	int temp_uid;
	USER* pUser = NULL;

	for( int i = min_x; i < max_x; i++ )
	{
		for( int j = min_y; j < max_y; j++ )
		{				
			temp_uid = pMap->m_pMap[i][j].m_lUser;

			if(temp_uid < USER_BAND || temp_uid >= NPC_BAND) continue;
			else temp_uid -= USER_BAND;
			
			if( temp_uid >= 0 && temp_uid < MAX_USER )
			{
				pUser = pCom->GetUserUid(temp_uid);
				if(pUser == NULL) continue;
				
				if( pUser->m_state == STATE_GAMESTARTED )
				{
					if( pUser->m_curx == i && pUser->m_cury == j && pUser->m_curz == m_sCurZ )
					{
						Send( pUser, pBuf, nLength );
					}
				}
			}
		}
	}
}

void CNpc::TestCode(COM *pCom, USER *pUser)
{
	int i;
	int uid = 0;

	CNpc *pNpc = NULL;

	int x, y;

	long lNpcUid = 0;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	POINT temp1Map[12] = {{-2,-2}, {-2,-1}, {-2,0}, {-1,-1}, {-1,0}, {0,-1}, {0,0},{1,-1},{1,0}, {2,-1}, {2,0}, {2,1}};// server
//	POINT temp2Map[8] = {{0,0}, {0,1}, {0,2}, {0,3}, {-1,0}, {-1,1}, {-1,2},{-1,3}};
//	POINT temp1Map[12] = {{147,1183}, {148,1184}, {149,1185}, {149,1183}, {150,1184}, {150,1182}, {151,1183},{151,1181},{152,1182}, {152,1180}, {153,1181}, {154,1182}};
	POINT temp2Map[] = {{-1,-3}, {0,-2}, {1,-1}, {2,0}, {3,1}, {-2,-2}, {-1,-1}, {0,0}, {1,1}, {2,2}};	// client

	MAP* pMap = g_zone[m_ZoneIndex];

	CPoint temp = ConvertToClient(m_sCurX, m_sCurY);
	switch(m_sDimension)
	{
	case 1:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < 12; i++)
		{
			x = m_sCurX + temp1Map[i].x;
			y = m_sCurY + temp1Map[i].y;
	
			CString strMsg = _T("");
			CPoint pt = ConvertToClient(x, y);
//			CPoint pt = ConvertToServer(temp1Map[i].x, temp1Map[i].y);
			strMsg.Format("1Luinet locked door x = %d, y = %d", pt.x - temp.x, pt.y - temp.y);

//			pUser->NormalChat(strMsg.GetBuffer(strMsg.GetLength()));

//			::InterlockedExchange(&pMap->m_pMap[x][y].m_lUser, lNpcUid);
		}
		break;
	case 2:
		lNpcUid = m_sNid + NPC_BAND;

		for(i =0; i < 10; i++)
		{
			x = temp.x + temp2Map[i].x;
			y = temp.y + temp2Map[i].y;

			CPoint pt = ConvertToServer(x, y);

			CString strMsg = _T("");			
//			CPoint pt = ConvertToServer(temp1Map[i].x, temp1Map[i].y);
			strMsg.Format("1Sanad locked door x = %d, y = %d", pt.x - m_sCurX, pt.y - m_sCurY);

//			pUser->NormalChat(strMsg.GetBuffer(strMsg.GetLength()));

		}
		break;
	}
}

int CNpc::PsiAttack(COM *pCom) //NPC魔法攻击
{
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

	BOOL	bPsiSuccess = FALSE;

	int index = 0;

	int delay = -1;
	int nTargetID	= m_Target.id;			// Target ID 甫 掘绰促.
	BYTE byPsi		= m_byPsi;				// Psionic sid 甫 掘绰促.

	int nPsiX = -1;							// Teleport且 困摹
	int nPsiY = -1;
	CPoint ptPsi(-1, -1);

	if( byPsi < 0 || byPsi >= g_arMonsterPsi.GetSize() ) return -1;

	if( nTargetID < USER_BAND || nTargetID >= INVALID_BAND ) return-1;	// 肋给等 Target 捞搁 return

	pUser = GetUser( pCom, nTargetID - USER_BAND );
	if( !pUser ) return -1;

	CMonsterPsi* pMagic = g_arMonsterPsi[(int)byPsi];
	if( !pMagic ) return -1;
	
	// 荤沥芭府 拌魂 ------------------------------------------------------------------------//
	if( !IsCloseTarget( pCom, (int)pMagic->m_byRange ) ) return -1;

	short damage, result;

	if(pMagic->m_sSid != 0)	
	{	
		damage = myrand( pMagic->m_sMinDmg, pMagic->m_sMaxDmg );
	
		result = damage * m_sVOL - pUser->GetUserSpellDefence();

/*		//如果存在护法，则攻击护法
		if(pUser->m_tHuFaType &&pUser->m_nHuFaHP>0)
		{
			int nID = pUser->m_uid+USER_BAND+USER_HUFA_BAND;
			//伤害数据
			pUser->SendDamageNum(0,nID,(short)result);

			SendAttackSuccess(pCom, nID, 0, pUser->m_nHuFaHP, pUser->m_nHuFaMaxHP);//yskang 0.3
			if(nDamage > 0)
				pUser->SetHuFaDamage(result);
			if(pUser->m_nHuFaHP>0)
			{
				pUser->HuFaAttack(m_sNid+NPC_BAND);
			}
			CBufferEx TempBuf;
			TempBuf.Add(PSI_ATTACK_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add( (BYTE)33 );				// Psionic sid
			TempBuf.Add( m_sNid + NPC_BAND );
			TempBuf.Add( nID );
			SendExactScreen(pCom, TempBuf, TempBuf.GetLength());
			return 1200;
		}*/
		pUser->SetDamage((int)result);	
//		pUser->SetFaNu(result);
		pUser->SendDamageNum(0,pUser->m_uid+USER_BAND,result);
		if(pUser->m_lDeadUsed == 1)
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			delay = m_sStandTime;

			if(m_NpcVirtualState == NPC_STANDING) 
			{
				if(m_sPid == 179) pUser->GetLevelDownExp(FALSE, -1, TRUE,m_strName); // 魔傍侩 阁胶磐老版快 版氰摹 1%皑家
				else			pUser->GetLevelDownExp(FALSE, -1, FALSE,m_strName);		// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
			}
		}
		else
		{
			
			if(m_sPid == 204) //黑暗死神 变魔法招怪
			{
				int iRandom = myrand(1, 10000);
				if(iRandom <= 3000)	
				{					
					CBufferEx TempBuf;
					TempBuf.Add(PSI_ATTACK_RESULT);
					TempBuf.Add(SUCCESS);
					TempBuf.Add( (BYTE)33 );				// Psionic sid
					TempBuf.Add( m_sNid + NPC_BAND );
					TempBuf.Add( nTargetID );
					SendExactScreen(pCom, TempBuf, TempBuf.GetLength());
					delay = (int)1200;					
					CPoint pt(-1, -1);
					pt = pUser->FindNearAvailablePoint_S(m_sCurX, m_sCurY);
					for(int i = 0; i< 2;i++){
						pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
				        pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					    pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					    pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					    pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					    pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					    pUser->SummonQuestMonster(174,m_sCurZ,pt.x,pt.y);
					}
					return (int)delay;
				}									
				
			}
			switch(pMagic->m_sSid)
			{
				case	8:
				case	27:
				case	31:
				case	32:
					GetWideRangeAttack( pCom, pUser->m_curx, pUser->m_cury, (int)damage, nTargetID - USER_BAND );				
					break;
				case	2:		// 筋绰 付过
				case	5:
				case	28:
				case	37:
				case	38:
				case	40:
				case	41:
				case	42:
				case	43:
					pUser->SetFireDamage();					
					break;				
				case	4:
				case	10:
				case	21:				
					pUser->SetColdDamage();					
					break;				
				case	24:
//					pUser->SetConFusion();					
					GetWideRangeAttack( pCom, pUser->m_curx, pUser->m_cury, (int)damage, nTargetID - USER_BAND );				
					break;				
			}
			
		}

	}

	CBufferEx TempBuf;

	TempBuf.Add(PSI_ATTACK_RESULT);
	TempBuf.Add(SUCCESS);

	TempBuf.Add( (BYTE)pMagic->m_sPid );				// Psionic sid
	TempBuf.Add( m_sNid + NPC_BAND );
	TempBuf.Add( nTargetID );

//	SendInsight( pCom, TempBuf, TempBuf.GetLength());
	SendExactScreen(pCom, TempBuf, TempBuf.GetLength());

	delay = (int)pMagic->m_sCasting;

	return (int)delay;
}

void CNpc::GetWideRangeAttack(COM* pCom, int x, int y, int damage, int except_uid)	// 瘤陛篮 傍拜父 贸府...
{
	int dir[9][2];
	int ix, iy;
	int nTarget = 0;
	int nDamage = 0;
	double result = 0;

	USER* pUser = NULL;
	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap) return;

	dir[0][0]  =  0;		dir[0][1] =  0;		// 
	dir[1][0]  = -1;		dir[1][1] =  0;		// 
	dir[2][0]  = -1;		dir[2][1] =  1;		// 
	dir[3][0]  =  0;		dir[3][1] =  1;		// 
	dir[4][0]  =  1;		dir[4][1] =  1;		// 

	dir[5][0]  =  1;		dir[5][1] =  0;		// 
	dir[6][0]  =  1;		dir[6][1] = -1;		// 
	dir[7][0]  =  0;		dir[7][1] = -1;		// 
	dir[8][0]  = -1;		dir[8][1] = -1;		// 

	for(int i = 1; i < 9; i++)
	{
		ix = x + dir[i][0];
		iy = y + dir[i][1];

		if(ix < 0) ix = 0;
		if(iy < 0) iy = 0;
		if(ix >= pMap->m_sizeMap.cx) ix = pMap->m_sizeMap.cx - 1;
		if(iy >= pMap->m_sizeMap.cy) iy = pMap->m_sizeMap.cy - 1;

		nTarget = pMap->m_pMap[ix][iy].m_lUser;

		if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
		{
			pUser = GetUser( pCom, nTarget - USER_BAND);			// User Pointer 甫 掘绰促.

			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;						// 肋给等 USER 捞搁 府畔
			if(pUser->m_bLive == USER_DEAD)	continue;		// Target User 啊 捞固 磷绢乐栏搁 府畔
			if(pUser->m_uid == except_uid ) continue;	// 吝缴俊 乐绰 蜡历绰 拌魂窍瘤 臼绰促

			result = (double)damage * (double)( m_sVOL * 20 ) / (double)( pUser->m_sMagicVOL * 15 + pUser->m_DynamicUserData[MAGIC_PSI_RESIST_UP] + m_sVOL * 20 );
			pUser->SetDamage((int)result);

			if(pUser->m_sHP > 0)		// 混篮 版快 傈扁单固瘤 眠啊
			{
//				pUser->SetColdDamage();
			}
			else
			{
//				IsChangeCityRank(pUser);
				if(m_sPid == 179) pUser->GetLevelDownExp(FALSE, -1, TRUE,m_strName); // 魔傍侩 阁胶磐老版快 版氰摹 1%皑家
				else			pUser->GetLevelDownExp(USER_PK, -1, FALSE,m_strName);			// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
			}
		}
/*
		else if(nTarget >= NPC_BAND)				// NPC
		{
			pNpc = GetNpc(nTarget - NPC_BAND);				// NPC Point 甫 掘绰促.
			if(pNpc == NULL) continue;					// 肋给等 NPC 捞搁 府畔
			if(pNpc->m_NpcState == NPC_DEAD || pNpc->m_tNpcType != NPCTYPE_MONSTER) continue;	// NPC 啊 捞固 磷绢 乐栏搁 府畔
			if(pNpc->m_sHP <= 0) continue;

			nDamage = (int)(damage *  ((double)m_sMagicVOL / (m_sMagicVOL + pNpc->m_sVOL)));
			nDamage = (int)((double)nDamage/2 + 0.5);	// 单固瘤狼 50%父 甸绢埃促.

			if(pNpc->SetDamage(nDamage, m_strUserID, m_uid + USER_BAND, m_pCom) == FALSE)
			{
				if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
				{
					CheckGuildHouseWarEnd();
				}

				pNpc->SendExpToUserList(m_pCom); // 版氰摹 盒硅!!
				pNpc->SendDead(m_pCom);
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(difflevel < 30)
				{
					CheckMaxValue(m_dwXP, 1);		// 各捞 磷阑锭父 1 刘啊!	
					SendXP();
				}
			}
			else									// 混篮 版快 傈扁单固瘤 眠啊
			{
//				pNpc->SetColdDamage();
			}
		}
*/
	}
}

int CNpc::AreaAttack(COM *pCom)
{
	if(!pCom) return 10000;
	if(m_tNpcType == NPCTYPE_GUARD) return -1;
	if(m_tNpcType == NPCTYPE_GUILD_GUARD) return -1;

	int nStandingTime = m_sStandTime;

	// 醚扁拌凯 老锭绰 鸥百苞狼 芭府拌魂阑 崔府秦具 茄促.
	if(IsCloseTarget(pCom, m_byRange) == FALSE)// Check Code (窜瘤 规绢利牢 螟搁俊辑 持篮 内靛)
	{
		m_NpcState = NPC_TRACING;			// 傍拜窍绊 档噶啊绰 蜡历甫 蝶扼 棱扁困秦(馆览阑 粱歹 狐福霸) 
		TRACE("AreaAttack - 芭府 钢绢辑 角菩\n");
		return -1;							// IsCloseTarget()俊 蜡历 x, y蔼阑 盎脚窍绊 Delay = 0栏肺 淋
	}

	USER* pUser = NULL;
	int nRange = 1;				// 傍拜 裹困 : 1 - 林困 8沫, 2 - 林困 24沫...
	int nTargetCount = 0;
	int target_uid = -1;

	int center_x = m_sCurX;		// 傍拜 裹困狼 吝缴痢 : 吝缴捞 傍拜窍绰 NPC老 荐档 乐绊 绢恫 痢阑 备且 荐档 乐促.
	int center_y = m_sCurY;		// 泅犁绰 磊扁 林困 傍拜栏肺 茄沥

	switch( (int)m_tSPATRange )
	{
	case	0:
	case	1:
		nRange = 2;
		center_x = m_sCurX;
		center_y = m_sCurY;
		break;

	case	2:
		nRange = 2;
		center_x = m_sCurX;
		center_y = m_sCurY;
		break;

	case	3:
		nRange = 1;
		center_x = m_Target.x;
		center_y = m_Target.y;
		break;

	case	4:
		nRange = 2;
		center_x = m_Target.x;
		center_y = m_Target.y;
		break;

	default:
		nRange = 1;
		center_x = m_sCurX;
		center_y = m_sCurY;
		break;
	}

	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap)
	{
		TRACE("AreaAttack - 甘绝绢辑 角菩\n");
		return -1;
	}

	int min_x = center_x - nRange;		if( min_x < 0 ) min_x = 0;
	int min_y = center_y - nRange;		if( min_y < 0 ) min_y = 0;
	int max_x = center_x + nRange;
	int max_y = center_y + nRange;

	if(max_x >= pMap->m_sizeMap.cx) max_x = pMap->m_sizeMap.cx - 1;
	if(max_y >= pMap->m_sizeMap.cy) max_y = pMap->m_sizeMap.cy - 1;

	TargetUser tuser[25];


	int		nAvoid = 0;
	int		iRandom = 0;
	int		determine = 0;
	int		iDexHitRate = 0, iLevelHitRate = 0;
	short	sTempHP		= 0;

	int		nHit = 0;

	BOOL	bIsHit = FALSE;
	BOOL	bIsCritical = FALSE;

	int		nDamage		= 0;
	int		nDefense	= 0;

	int nID = m_Target.id;					// Target 阑 备茄促.

	// 疙吝咯何 魄窜 函荐 檬扁拳
	bIsHit = FALSE;		


	for( int ix = min_x; ix <= max_x; ix++ )
	{
		for( int iy = min_y; iy <= max_y; iy++ )
		{
			target_uid = pMap->m_pMap[ix][iy].m_lUser;

			if( target_uid < USER_BAND || target_uid >= NPC_BAND )
			{
				continue;
			}

			pUser = GetUser(pCom, target_uid - USER_BAND);

			if( !pUser ) continue;
			if( pUser->m_bLive != USER_LIVE ) continue;

			if( ix != pUser->m_curx || iy != pUser->m_cury ) continue;

			if(pUser->m_state == STATE_DISCONNECTED) continue;
			if(pUser->m_tIsOP == 1 ) continue;
			if(pUser->m_bPShopOpen == TRUE) continue;

			// 雀乔蔼 拌魂 
			nAvoid = pUser->GetAvoid();

			// 疙吝咯何 魄窜
			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5); 
			
			iDexHitRate = (int)( 30.0 * ( (double)m_sDEX/(m_sDEX + pUser->m_sMagicDEX) ) + 15.0 );
			iLevelHitRate = (int)( 70.0 * ( (double)m_byClassLevel/(pUser->m_sLevel + m_byClassLevel) ) + 15.0);

			determine = iDexHitRate + iLevelHitRate - (nAvoid+pUser->m_Avoid);

			if(determine < ATTACK_MIN) determine = ATTACK_MIN;			// 弥家 20
			else if(determine > ATTACK_MAX) determine = ATTACK_MAX;		// 弥措

			if(iRandom < determine)	bIsHit = TRUE;		// 疙吝

			// 傍拜 固胶
			if(bIsHit == FALSE)
			{
				TRACE("AreaAttack - 傍拜 固胶\n");
				continue;
			}

			// 疙吝捞搁 //Damage 贸府 ----------------------------------------------------------------//

			nDamage = GetFinalDamage(pUser);	// 弥辆 措固瘤

			if(nDamage > 0) 
			{
		        pUser->SetDamage(nDamage);
			    pUser->SendDamageNum(0,pUser->m_uid+USER_BAND,nDamage);
			}
			// 规绢螟 郴备档 皑家
			pUser->SendDamagedItem(nDamage);

//			if(pUser->m_bLive == USER_DEAD || pUser->m_sHP <= 0)//@@@ 唱吝俊 绊魔
			if(pUser->m_lDeadUsed == 1)
			{
				if(m_NpcVirtualState == NPC_STANDING) 
				{
					if(m_sPid == 179) pUser->GetLevelDownExp(FALSE, -1, TRUE,m_strName); // 魔傍侩 阁胶磐老版快 版氰摹 1%皑家
					else			pUser->GetLevelDownExp(FALSE, -1, FALSE,m_strName);		// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
				}
			}

			tuser[nTargetCount].iUid = target_uid;
			tuser[nTargetCount].sHP = pUser->m_sHP;
			tuser[nTargetCount].sMaxHP = pUser->m_sMagicMaxHP;
			pUser->SendHP();

			nTargetCount++;

			if( nTargetCount >= 25 ) break;
		}

		if( nTargetCount >= 25 ) break;
	}

	if( !nTargetCount )
	{
		TRACE("AreaAttack - 裹困 救俊 蜡历 绝绢辑 角菩\n");
//		return -1;
	}

	CBufferEx TempBuf;
//	2a 0 0 0 ca 1 1 9f 5b 0 0 6a 2c 0 0 0 0
	TempBuf.Add(AREA_ATTACK_RESULT);
	TempBuf.Add(ATTACK_SUCCESS);
	TempBuf.Add( (byte)1 );
	TempBuf.Add((int)(m_sNid + NPC_BAND));
	
	for(int i = 0; i < 1; i++ )
	{
		TempBuf.Add( (int)tuser[i].iUid );
		TempBuf.Add( (short)0 );
		TempBuf.Add( (short)0 );
	}

	SendInsight(pCom, TempBuf, TempBuf.GetLength());
//	SendExactScreen(pCom, TempBuf, TempBuf.GetLength());

	TRACE("AreaAttack - 己傍\n");
	
	return m_sAttackDelay;
}



void CNpc::GiveEventItemNewToUser(USER *pUser)
{
	if( !pUser ) return;
	if( pUser->m_state != STATE_GAMESTARTED ) return;
//	if( pUser->m_iDisplayType == 6 && pUser->m_sLevel > 25) return; //yskang 0.5
	if( pUser->m_iDisplayType == 6) return; //yskang 0.5

	int i;
	CEventItemNew* pNewItem = NULL;

	BOOL bFlag = FALSE;

	int sItemSid = -1;
	BYTE tItemQuality = 0;
	BYTE tItemWear = 0;

	int j;
	int iSlot = -1;

	SYSTEMTIME time;
	GetLocalTime( &time );
	CString strMsg = _T("");

	MYSHORT upper;	upper.i = 0;
	MYINT lower;	lower.i = 0;

	for( i = 0; i < g_arEventItemNew.GetSize(); i++ )
	{
		if( g_arEventItemNew[i] )
		{
			pNewItem = g_arEventItemNew[i];

			if( ::InterlockedCompareExchange( (long*)&(pNewItem->m_lGive), (long)0, (long)1 ) == (long)0 ) continue;

			if( pNewItem->m_sSid != NPC_EVENT_LOTTO )
			{
				// 汗鼻捞 酒囱 版快 蜡历乏苞 各狼 乏狼 瞒捞啊 25乏阑 檬苞窍搁 林瘤 臼绰促.
				if(abs(m_byClassLevel - pUser->m_sLevel) > 25)
				{
					if(m_byClassLevel < pUser->m_sLevel) return;
				}
			}
			else
			{
				// 汗鼻篮 蜡历乏苞 各狼 乏狼 瞒捞啊 40乏阑 檬苞窍搁 林瘤 臼绰促.
				if(abs(m_byClassLevel - pUser->m_sLevel) > 40)
				{
					if(m_byClassLevel < pUser->m_sLevel) return;
				}
			}

			//////////////////////////////////////////////////////////////////////
			// 眠啊登绰 捞亥飘 酒捞袍捞 乐栏搁 酒贰俊 眠啊茄促.
			//////////////////////////////////////////////////////////////////////

			// 采官备聪 捞亥飘
			if( pNewItem->m_sSid == NPC_EVENT_FLOWER )
			{
				if( time.wYear == 2002 && time.wMonth == 5 && ( time.wDay >= 1 || time.wDay <= 5 ) )
				{
					sItemSid = pNewItem->m_sSid;
					tItemQuality = 0;
				}
				else
				{
					return;
				}
			}

			if( pNewItem->m_sSid == NPC_EVENT_LOTTO )
			{
				if( time.wYear == 2002 && ( ( time.wMonth == 5 && time.wDay >= 16 ) || ( time.wMonth == 6 && time.wDay <= 22 ) ) )
				{
					sItemSid = pNewItem->m_sSid;
					tItemQuality = pNewItem->m_tQuality;
				}
				else
				{
					return;
				}

				if( pUser->m_sLevel < 25 )		// 捞亥飘 汗鼻篮 25饭骇 固父篮 林瘤 臼绰促.
				{
					return;
				}
			}

			/////////////////////////////////////////////////////////////////////

			if( sItemSid < 0 || sItemSid >= g_arItemTable.GetSize() ) return;
			CItemTable* pItemTable = g_arItemTable[sItemSid];

			iSlot = pUser->GetEmptySlot(INVENTORY_SLOT);

			if( iSlot != -1 )
			{
				if(pUser->m_iMaxWeight >= pUser->m_iCurWeight + pItemTable->m_byWeight) bFlag = TRUE;
			}

			switch( sItemSid )
			{
				case NPC_EVENT_FLOWER:
					if(bFlag) { strMsg.Format(IDS_EVENT_FLOWER, pUser->m_strUserID); }
					break;
				case NPC_EVENT_LOTTO:
					if(bFlag) { strMsg.Format(IDS_EVENT_LOTTO); }
					break;
				default:
					return;
			}

			ItemList newItem;
			pUser->ReSetItemSlot( &newItem );

			newItem.tType = TYPE_ITEM;
			newItem.sLevel = pItemTable->m_byRLevel;
			newItem.sSid = sItemSid;
			newItem.sCount = 1;
			newItem.sDuration = pItemTable->m_sDuration;
			newItem.sBullNum = pItemTable->m_sBullNum;
			newItem.tIQ = tItemQuality;
			newItem.iItemSerial = 0;

			for( j = 0; j < MAGIC_NUM; j++ ) newItem.tMagic[j] = 0;

			// 捞亥飘 酒捞袍 林扁 抛捞喉俊辑 巢篮 俺荐甫 茄俺 临咯霖促.
			pNewItem->m_sRemain--;

			if( pNewItem->m_sRemain < 0 )
			{
				pNewItem->m_sRemain = 0;
			}
			if( !UpdateEventItemNewRemain( pNewItem ) )
			{
				pNewItem->m_sRemain++;
				::InterlockedExchange( &(pNewItem->m_lGive), 1 );
				return;
			}

			if( pNewItem->m_tSerialExist != 255 )		// 矫府倔 锅龋甫 何咯秦具 窍绰 惑炔捞扼搁
			{
				upper.i = pNewItem->m_tSerialExist;		// 10000 锅措 锅龋
				lower.i = pNewItem->m_sRemain;

				newItem.tMagic[0] = upper.b[0];
				newItem.tMagic[1] = upper.b[1];

				newItem.tMagic[2] = lower.b[0];
				newItem.tMagic[3] = lower.b[1];
				newItem.tMagic[4] = lower.b[2];
				newItem.tMagic[5] = lower.b[3];
			}

			// bFlag - 牢亥俊 后浇吩捞 乐绊, 公霸力茄俊 吧府瘤 臼疽阑锭 TRUE捞促.
			if(bFlag)
			{
				pUser->m_UserItem[iSlot].tType = newItem.tType;
				pUser->m_UserItem[iSlot].sLevel = newItem.sLevel;
				pUser->m_UserItem[iSlot].sSid = newItem.sSid;
				pUser->m_UserItem[iSlot].sCount = newItem.sCount;
				pUser->m_UserItem[iSlot].sDuration = newItem.sDuration;
				pUser->m_UserItem[iSlot].sBullNum = newItem.sBullNum;
				pUser->m_UserItem[iSlot].tIQ = newItem.tIQ;
				pUser->m_UserItem[iSlot].iItemSerial = newItem.iItemSerial;

				for( j = 0; j < MAGIC_NUM; j++ ) pUser->m_UserItem[iSlot].tMagic[j] = newItem.tMagic[j];

				CBufferEx TempBuf;

				TempBuf.Add(ITEM_LOAD_RESULT);
				TempBuf.Add(SUCCESS);
				TempBuf.Add((BYTE)0x01);

				TempBuf.Add((BYTE)iSlot);
				TempBuf.Add(pUser->m_UserItem[iSlot].sLevel);
				TempBuf.Add(pUser->m_UserItem[iSlot].sSid);
				TempBuf.Add(pUser->m_UserItem[iSlot].sDuration);
				TempBuf.Add(pUser->m_UserItem[iSlot].sBullNum);
				TempBuf.Add(pUser->m_UserItem[iSlot].sCount);
				for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)pUser->m_UserItem[iSlot].tMagic[j]);

				TempBuf.Add((BYTE)pUser->m_UserItem[iSlot].tIQ);

				pUser->Send(TempBuf, TempBuf.GetLength());

				pUser->m_iCurWeight += pItemTable->m_byWeight;
				pUser->GetRecoverySpeed();			// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券

				switch( sItemSid )
				{
					case NPC_EVENT_FLOWER:
						pUser->m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL);
//						pUser->m_pCom->Announce(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL);
						break;
					case NPC_EVENT_LOTTO:
						pUser->SendSystemMsg( IDS_EVENT_LOTTO, SYSTEM_NORMAL, TO_ME);
						break;
					default:
						return;
				}

				strMsg.Format("(%04d-%02d-%02d %02d:%02d:%02d) %s - Get %d Item(%d)\r\n",
					time.wYear, 
					time.wMonth, 
					time.wDay, 
					time.wHour, 
					time.wMinute, 
					time.wSecond,	
					pUser->m_strUserID, 
					newItem.sSid,
					upper.i * 10000 + lower.i );

				EnterCriticalSection( &m_CS_EventItemLogFileWrite );
				g_fpEventItem.Write( strMsg, strMsg.GetLength() );
				LeaveCriticalSection( &m_CS_EventItemLogFileWrite );
			}
			else
			{
				// 牢亥持扁俊 角菩窍搁 甘俊 悸泼
				GiveItemToMap( pUser->m_pCom, &newItem );

				strMsg.Format("(%04d-%02d-%02d %02d:%02d:%02d) %s - Map %d Item(%d)\r\n",
					time.wYear, 
					time.wMonth, 
					time.wDay, 
					time.wHour, 
					time.wMinute, 
					time.wSecond,	
					pUser->m_strUserID, 
					newItem.sSid,
					upper.i * 10000 + lower.i );

				EnterCriticalSection( &m_CS_EventItemLogFileWrite );
				g_fpEventItem.Write( strMsg, strMsg.GetLength() );
				LeaveCriticalSection( &m_CS_EventItemLogFileWrite );
			}
		}
	}
}

void CNpc::GiveItemToMap(COM *pCom, ItemList *pItem)
{
	CPoint pt = FindNearRandomPointForItem(m_sCurX, m_sCurY);							// 泅犁 磊扁谅钎甫 力寇茄 24沫
	if(pt.x <= -1 || pt.y <= -1) return;
	if(pt.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pt.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return;

	if( InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, (long)1, (long)0) == (long)0 )
	{
		ItemList* pNewItem = new ItemList;

		memcpy( pNewItem, pItem, sizeof( ItemList ) );

		// 秦寸 蜡历俊霸 舅赴促.			
		//pCom->DelThrowItem();
		pCom->SetThrowItem( pNewItem, pt.x, pt.y, m_ZoneIndex );

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, 0);
	}
}

BOOL CNpc::UpdateEventItemNewRemain(CEventItemNew *pEventItem)
{
	SQLSMALLINT	sRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call update_event_item_new_remain ( %d, %d, ? )}"), 
		pEventItem->m_sIndex, 
		pEventItem->m_sRemain );

	int db_index = 0;
	CDatabase* pDB = g_DBNew[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	int i = 1;
	SQLBindParameter( hstmt, i++ ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &iRetInd);

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[AUTOMATA_THREAD].ReleaseDB(db_index);

	if( sRet == -1 ) return FALSE;

	return TRUE;
}

void CNpc::UserListSort()
{
	int i, j;
	int total = 0;

	ItemUserRightlist temp;

	for(i = 0; i < NPC_HAVE_USER_LIST; i++)
	{
		m_iHaveItemUid[i].uid = -1;
		m_iHaveItemUid[i].nDamage = 0;

		if( m_DamagedUserList[i].nDamage > 0 )
		{
			m_iHaveItemUid[i].uid = m_DamagedUserList[i].iUid;
			m_iHaveItemUid[i].nDamage = m_DamagedUserList[i].nDamage;
		}
	}

	for(i = 2; i < NPC_HAVE_USER_LIST; i++)
	{
		temp.uid = m_iHaveItemUid[i].uid;
		temp.nDamage = m_iHaveItemUid[i].nDamage;
		
		j = i;

		while(m_iHaveItemUid[j-1].nDamage < temp.nDamage)
		{
			m_iHaveItemUid[j].uid = m_iHaveItemUid[j-1].uid;
			m_iHaveItemUid[j].nDamage = m_iHaveItemUid[j-1].nDamage;
			j--;

			if(j <= 0) break;
		}

		m_iHaveItemUid[j].uid = temp.uid;
		m_iHaveItemUid[j].nDamage = temp.nDamage;
	}

	for(i = 0; i < ITEM_USER_RIGHT_NUM; i++)
	{
		if(m_iHaveItemUid[i].nDamage > 0) total += m_iHaveItemUid[i].nDamage;
	}

	if(total <= 0) total = 1;

	for(i = 0; i < ITEM_USER_RIGHT_NUM; i++)
	{
		j = 0;
		j = (int)( (m_iHaveItemUid[i].nDamage * 100)/total );

		if(j > 100) j = 100;
		else if(j <= 0) j = 1;

		m_iHaveItemUid[i].nDamage = (BYTE)j;	
	}
}

DWORD CNpc::GetItemThrowTime()
{
	DWORD dwCurTime = 0;

	SYSTEMTIME SaveTime;
	GetLocalTime(&SaveTime);
	
	WORD wTemp = 0;
	DWORD dwYear = 0;
	DWORD dwMon = 0;
	DWORD dwDay = 0;
	DWORD dwHour = 0;
	DWORD dwMin = 0;
	DWORD dwSecond = 0;
										// 2 Byte 滚府绊
	wTemp = SaveTime.wYear << 12;		// 惑困 4 Byte
	wTemp = wTemp >> 12;
	dwYear = (DWORD)wTemp; 
	dwYear = dwYear << 26;

	wTemp = SaveTime.wMonth << 12;		// 4 Byte
	wTemp = wTemp >> 12;
	dwMon = (DWORD)wTemp; 
	dwMon = dwMon << 22;

	wTemp = SaveTime.wDay << 11;		// 5 Byte
	wTemp = wTemp >> 11;
	dwDay = (DWORD)wTemp;
	dwDay = dwDay << 17;

	wTemp = SaveTime.wHour << 11;		// 5 Byte
	wTemp = wTemp >> 11;
	dwHour = (DWORD)wTemp;
	dwHour = dwHour << 12;

	wTemp = SaveTime.wMinute << 10;		// 6 Byte
	wTemp = wTemp >> 10;
	dwMin = (DWORD)wTemp;
	dwMin = dwMin << 6;

	wTemp = SaveTime.wSecond << 10;		// 6 Byte
	wTemp = wTemp >> 10;
	dwSecond = (DWORD)wTemp;

	dwCurTime = dwYear^dwMon^dwDay^dwHour^dwMin^dwSecond;

	return dwCurTime;
}

BOOL CNpc::CheckUserForNpc_Live(int x, int y)
{
	return TRUE;

	int min_x, min_y, max_x, max_y;

	min_x = m_sCurX - m_bySearchRange;		if( min_x < 0 ) min_x = 0;
	min_y = m_sCurY - m_bySearchRange;		if( min_y < 0 ) min_y = 0;
	max_x = m_sCurX + m_bySearchRange;
	max_y = m_sCurY + m_bySearchRange;

	if(max_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) max_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 2;
	if(max_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) max_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 2;

	int ix, iy;
	int target_uid;

	int tempLevel = 0, oldLevel = 1000;

	for(ix = min_x; ix <= max_x; ix++)
	{
		for(iy = min_y; iy <= max_y; iy++)
		{
			target_uid = m_pOrgMap[ix][iy].m_lUser;

			if( target_uid >= USER_BAND && target_uid < NPC_BAND ) return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	Summon 栏肺 牢秦 函版等 困摹沥焊甫 林函俊 焊辰促.
//
void CNpc::SendNpcInfoBySummon(COM *pCom)
{
	int min_x = 0, min_y = 0;
	int max_x = 0, max_y = 0;

	int sx = m_sCurX / SIGHT_SIZE_X;
	int sy = m_sCurY / SIGHT_SIZE_Y;

	int delete_index = 0;
	char delete_send[1024];		::ZeroMemory(delete_send, sizeof(delete_send));
	FillNpcInfo(delete_send, delete_index, INFO_DELETE);

	min_x = (sx-1)*SIGHT_SIZE_X;
	max_x = (sx+2)*SIGHT_SIZE_X;
	min_y = (sy-1)*SIGHT_SIZE_Y;
	max_y = (sy+2)*SIGHT_SIZE_Y;

	SendToRange(pCom, delete_send, delete_index, min_x, min_y, max_x, max_y);
}
