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
#include "ShopSystem.h"

extern ShopSystem g_Shop;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Extern.h"		//	寇何函荐 急攫

extern CUserManager *g_pUQM;
extern CSearch *g_pUserList;
extern CServerDlg *g_pMainDlg;

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


extern CPtrList RecvSqlData;
extern CRITICAL_SECTION m_CS_SqlData;
extern long nSqlDataCount;	
extern int g_ChatEnable[];
extern  struct convert_table convert[80];

extern CHATDATAPACKET *g_WaitRecvDataChat[DATA_BUF_SIZE_FOR_THREAD+1][AUTOMATA_THREAD+1];
extern int g_nChatDataHead[];
extern int g_nChatDataTail[];

extern BYTE g_ServerId;
extern CRoyalRumble g_RR;

extern short		g_sHPConst[NEW_CLS_NUM];
extern short		g_sPPConst[NEW_CLS_NUM];
extern short		g_sSPConst[NEW_CLS_NUM];

extern short		g_sHPLV[NEW_CLS_NUM];
extern short		g_sPPLV[NEW_CLS_NUM];
extern short		g_sSPLV[NEW_CLS_NUM];

extern short		g_sHPAdd[NEW_CLS_NUM];
extern short		g_sPPAdd[NEW_CLS_NUM];
extern short		g_sSPAdd[NEW_CLS_NUM];

extern TCHAR g_arServerIPAddr[16];
extern CString g_strARKRRWinner;

// Quest Event Class
extern CEventZone	g_QuestEventZone;

extern long	g_PotionViewOnOff;

const char* g_pszReservedLoveName[] = 
{
"GM",
"gm",
"管理",
"决战",
"admin",
};
TCHAR *g_MagicArray[]={   //属性列表
	"",
	"1力",
	"1体",
	"1敏",
	"1智慧",
	"1智力",
	"5损",
	"2命中",
	"3命中",
	"2回",
	"3回",
	"1防",
	"2防",
	"5抗",
	"5魔伤",
	"2力",
	"2体",
	"2敏",
	"2智慧",
	"2智力2",
	"10损",
	"防止中毒状态异常",
	"防止麻痹状态异常",
	"防止混乱状态异常",
	"防止视觉丧失状态异常",
	"防止火焰状态异常",
	"防止冰冻状态异常",
	"防止电击状态异常",
	"射程1",
	"1速",
	"4命中",
	"5命中",
	"4回",
	"5回",
	"故障率1%",
	"3防",
	"4防",
	"耐久100",
	"10血",
	"10精神",
	"10体",
	"10抗",
	"10魔损",
	"3力",
	"3体",
	"3敏",
	"3智慧",
	"3智力",
	"反击1",
	"兴奋1",
	"钢铁皮肤1",
	"自我医疗1",
	"魔法精修1",
	"法术大师1",
	"精神强化1",
	"冥想1",
	"穿刺1",
	"闪避1",
	"精神集中1",
	"回复1",
	"狙击1",
	"连射1",
	"灵动1",
	"枪械大师1",
	"15损",
	"诱发异常中毒状态",
	"诱发异常麻痹状态",
	"诱发异常混乱状态",
	"诱发异常视角丧失状态",
	"诱发异常火焰状态",
	"诱发异常冷气状态",
	"诱发异常电气状态",
	"2速",
	"6命中",
	"7命中",
	"6回",
	"7回",
	"故障2%",
	"5防",
	"6防",
	"耐久200",
	"20血",
	"20精神",
	"20体",
	"15抗",
	"15魔损",
	"4力",
	"4体",
	"4敏",
	"4智慧",
	"4智力",
	"20损",
	"射程2",
	"3速",
	"8命中",
	"9命中",
	"8回",
	"9回",
	"故障3%",
	"7防",
	"8防",
	"耐久300",
	"30血",
	"30精神",
	"30体",
	"20抗",
	"20魔损",
	"5力",
	"5体",
	"5敏",
	"5智慧",
	"5智力",
	"反击2",
	"兴奋2",
	"钢铁皮肤2",
	"自我医疗2",
	"魔法精修2",
	"法术大师2",
	"精神强化2",
	"冥想2",
	"穿刺2",
	"闪避2",
	"精神集中2",
	"回复2",
	"狙击2",
	"连射2",
	"灵动2",
	"枪械大师2",
	"25损",
	"9防",
	"10防",
	"耐久500",
	"40血",
	"40精神",
	"40体",
	"25抗",
	"25魔损",
	"所有技术1",
	"10命中",
	"10回",
	"耐久700",
	"50血",
	"50精神",
	"50体力",
	"防止所有状态异常",
	"所有技术2",
	"30损伤",
	"15命中",
	"所有能力值6",
	"所有技术3",
	"恢复速度1阶段",
	"恢复速度2阶段",
	"恢复速度3阶段",
	"恢复速度4阶段",
	"回避15%+",
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
	"1抗",
	"2抗",
	"3抗",
	"4抗",
	"1魔损",
	"2魔损",
	"3魔损",
	"4魔损",
	"1损",
	"2损",
	"3损",
	"4损",
	"1命中",
	"1回",
	"5魔损",
	"生命1",
	"生命2",
	"生命3",
	"生命4",
	"生命5",
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
};

const char* g_pszReservedID[] = 
{
	"GM",
	"gm",
"管理",
"决战",
"。",
"!",
"！",
"宣传",
"　",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
".",
"!",
"#",
"$",
"%",
"&",
"'",
"(",
")",
"*",
"+",
",",
"-",
".",
"/",
":",
";",
"<",
"=",
">",
"@",
"[",
"]",
"^",
"_",
"`", 
"{",
"|",
"}",
"~",
"",
"￠",
"￡",
"¤",
"￥",
"|",
"§",
"¨",
"-",
"ˉ",
"°",
"±",
"′",
"μ",
"·",
"à",
"á",
"è",
"é",
"ê",
"ì",
"í",
"ò",
"ó",
"×",
"ù",
"ú",
"ü",
"à",
"á",
"è",
"é",
"ê",
"ì",
"í",
"ò",
"ó",
"÷",
"ù",
"ú",
"ü",
"ā",
"ā",
"ē",
"ē",
"ě",
"ě",
"ī",
"ī",
"ń",
"ň",
"ō",
"ō",
"ū",
"ū",
"∥",
"ǎ",
"ǎ",
"ǐ",
"ǐ",
"ǒ",
"ǒ",
"ǔ",
"ǔ",
"ǖ",
"ǖ",
"ǘ",
"ǘ",
"ǚ",
"ǚ",
"ǜ",
"ǜ",
"ɑ",
"ɡ",
"ˇ",
"ˉ",
"ˊ",
"ˋ",
"˙",
"Α",
"Β",
"Γ",
"Δ",
"Ε",
"Ζ",
"Η",
"Θ",
"Ι",
"Κ",
"Λ",
"Μ",
"Ν",
"Ξ",
"Ο",
"Π",
"Ρ",
"Σ",
"Τ",
"Υ",
"Φ",
"Χ",
"Ψ",
"Ω",
"α",
"β",
"γ",
"δ",
"ε",
"ζ",
"η",
"θ",
"ι",
"κ",
"λ",
"μ",
"ν",
"ξ",
"ο",
"π",
"ρ",
"σ",
"τ",
"υ",
"φ",
"χ",
"ψ",
"ω",
"Ё",
"А",
"Б",
"В",
"Г",
"Д",
"Е",
"Ж",
"З",
"И",
"Й",
"К",
"Л",
"Н",
"О",
"П",
"Р",
"С",
"Т",
"У",
"Ф",
"Х",
"Ц",
"Ч",
"Ш",
"Щ",
"Ъ",
"Ы",
"Ь",
"Э",
"Ю",
"Я",
"а",
"б",
"в",
"г",
"д",
"е",
"ж",
"з",
"и",
"й",
"к",
"л",
"м",
"н",
"о",
"п",
"р",
"с",
"т",
"у",
"ф",
"х",
"ц",
"ч",
"ш",
"щ",
"ъ",
"ы",
"ь",
"э",
"ю",
"я",
"ё",
"‐",
"–",
"—",
"―",
"‖",
"'",
"'",
"‥",
"…",
"‰",
"′",
"″",
"‵",
"※",
"￣",
"€",
"℃",
"℅",
"℉",
"№",
"℡",
"Ⅰ",
"Ⅱ",
"Ⅲ",
"Ⅳ",
"Ⅴ",
"Ⅵ",
"Ⅶ",
"Ⅷ",
"Ⅸ",
"Ⅹ",
"Ⅺ",
"Ⅻ",
"ⅰ",
"ⅱ",
"ⅲ",
"ⅳ",
"ⅴ",
"ⅵ",
"ⅶ",
"ⅷ",
"ⅸ",
"ⅹ",
"←",
"↑",
"→",
"↓",
"↖",
"↗",
"↘",
"↙",
"∈",
"∏",
"∑",
"∕",
"°",
"√",
"∝",
"∞",
"∟",
"∠",
"∣",
"∥",
"∧",
"∨",
"∩",
"∪",
"∫",
"∮",
"∴",
"∵",
"∶",
"∷",
"～",
"∽",
"≈",
"≌",
"≒",
"≠",
"≡",
"≤",
"≥",
"≦",
"≧",
"≮",
"≯",
"⊕",
"⊙",
"⊥",
"⊿",
"⌒",
"①",
"②",
"③",
"④",
"⑤",
"⑥",
"⑦",
"⑧",
"⑨",
"⑩",
"⑴",
"⑵",
"⑶",
"⑷",
"⑸",
"⑹",
"⑺",
"⑻",
"⑼",
"⑽",
"⑾",
"⑿",
"⒀",
"⒁",
"⒂",
"⒃",
"⒄",
"⒅",
"⒆",
"⒇",
"⒈",
"⒉",
"⒊",
"⒋",
"⒌",
"⒍",
"⒎",
"⒏",
"⒐",
"⒑",
"⒒",
"⒓",
"⒔",
"⒕",
"⒖",
"⒗",
"⒘",
"⒙",
"⒚",
"⒛",
"─",
"━",
"│",
"┃",
"┄",
"┅",
"┆",
"┇",
"┈",
"┉",
"┊",
"┋",
"┌",
"┍",
"┎",
"┏",
"┐",
"┑",
"┒",
"┓",
"└",
"┕",
"┖",
"┗",
"┘",
"┙",
"┚",
"┛",
"├",
"┝",
"┞",
"┟",
"┠",
"┡",
"┢",
"┣",
"┤",
"┥",
"┦",
"┧",
"┨",
"┩",
"┪",
"┫",
"┬",
"┭",
"┮",
"┯",
"┰",
"┱",
"┲",
"┳",
"┴",
"┵",
"┶",
"┷",
"┸",
"┹",
"┺",
"┻",
"┼",
"┽",
"┾",
"┿",
"╀",
"╁",
"╂",
"╃",
"╄",
"╅",
"╆",
"╇",
"╈",
"╉",
"╊",
"╋",
"═",
"║",
"╒",
"╓",
"╔",
"╕",
"╖",
"╗",
"╘",
"╙",
"╚",
"╛",
"╜",
"╝",
"╞",
"╟",
"╠",
"╡",
"╢",
"╣",
"╤",
"╥",
"╦",
"╧",
"╨",
"╩",
"╪",
"╫",
"╬",
"╭",
"╮",
"╯",
"╰",
"╱",
"╲",
"╳",
"▁",
"▂",
"▃",
"▄",
"▅",
"▆",
"▇",
"█",
"▉",
"▊",
"▋",
"▌",
"▍",
"▎",
"▏",
"▓",
"▔",
"▕",
"■",
"□",
"▲",
"△",
"▼",
"▽",
"◆",
"◇",
"○",
"◎",
"●",
"◢",
"◣",
"◤",
"◥",
"★",
"☆",
"☉",
"♀",
"♂",
"、",
"。",
"〃",
"々",
"〆",
"〇",
"〈",
"〉",
"《",
"》",
"「",
"」",
"『",
"』",
"【",
"】",
"〒",
"〓",
"〔",
"〕",
"〖",
"〗",
"〝",
"〞",
"〡",
"〢",
"〣",
"〤",
"〥",
"〦",
"〧",
"〨",
"〩",
"ぁ",
"あ",
"ぃ",
"い",
"ぅ",
"う",
"ぇ",
"え",
"ぉ",
"お",
"か",
"が",
"き",
"ぎ",
"く",
"ぐ",
"け",
"げ",
"こ",
"ご",
"さ",
"ざ",
"し",
"じ",
"す",
"ず",
"せ",
"ぜ",
"そ",
"ぞ",
"た",
"だ",
"ち",
"ぢ",
"っ",
"つ",
"づ",
"て",
"で",
"と",
"ど",
"な",
"に",
"ぬ",
"ね",
"の",
"は",
"ば",
"ぱ",
"ひ",
"び",
"ぴ",
"ふ",
"ぶ",
"ぷ",
"へ",
"べ",
"ぺ",
"ほ",
"ぼ",
"ぽ",
"ま",
"み",
"む",
"め",
"も",
"ゃ",
"や",
"ゅ",
"ゆ",
"ょ",
"よ",
"ら",
"り",
"る",
"れ",
"ろ",
"ゎ",
"わ",
"ゐ",
"ゑ",
"を",
"ん",
"゛",
"゜",
"ゝ",
"ゞ",
"ァ",
"ア",
"ィ",
"イ",
"ゥ",
"ウ",
"ェ",
"エ",
"ォ",
"オ",
"カ",
"ガ",
"キ",
"ギ",
"ク",
"グ",
"ケ",
"ゲ",
"コ",
"ゴ",
"サ",
"ザ",
"シ",
"ジ",
"ス",
"ズ",
"セ",
"ゼ",
"ソ",
"ゾ",
"タ",
"ダ",
"チ",
"ヂ",
"ッ",
"ツ",
"ヅ",
"テ",
"デ",
"ト",
"ド",
"ナ",
"ニ",
"ヌ",
"ネ",
"ノ",
"ハ",
"バ",
"パ",
"ヒ",
"ビ",
"ピ",
"フ",
"ブ",
"プ",
"ヘ",
"ベ",
"ペ",
"ホ",
"ボ",
"ポ",
"マ",
"ミ",
"ム",
"メ",
"モ",
"ャ",
"ヤ",
"ュ",
"ユ",
"ョ",
"ヨ",
"ラ",
"リ",
"ル",
"レ",
"ロ",
"ヮ",
"ワ",
"ヰ",
"ヱ",
"ヲ",
"ン",
"ヴ",
"ヵ",
"ヶ",
"ー",
"ヽ",
"ヾ",
"ㄅ",
"ㄆ",
"ㄇ",
"ㄈ",
"ㄉ",
"ㄊ",
"ㄋ",
"ㄌ",
"ㄍ",
"ㄎ",
"ㄏ",
"ㄐ",
"ㄑ",
"ㄒ",
"ㄓ",
"ㄔ",
"ㄕ",
"ㄖ",
"ㄗ",
"ㄘ",
"ㄙ",
"ㄚ",
"ㄛ",
"ㄜ",
"ㄝ",
"ㄞ",
"ㄟ",
"ㄠ",
"ㄡ",
"ㄢ",
"ㄣ",
"ㄤ",
"ㄥ",
"ㄦ",
"ㄧ",
"ㄨ",
"ㄩ",
"㈠",
"㈡",
"㈢",
"㈣",
"㈤",
"㈥",
"㈦",
"㈧",
"㈨",
"㈩",
"㈱",
"㊣",
"㎎",
"㎏",
"㎜",
"㎝",
"㎞",
"㎡",
"㏄",
"㏎",
"㏑",
"㏒",
"㏕",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"︰",
"︱",
"︳",
"︴",
"︵",
"︶",
"︷",
"︸",
"︹",
"︺",
"︻",
"︼",
"︽",
"︾",
"︿",
"﹀",
"﹁",
"﹂",
"﹃",
"﹄",
"﹉",
"﹊",
"﹋",
"﹌",
"﹍",
"﹎",
"﹏",
"﹐",
"﹑",
"﹒",
"﹔",
"﹕",
"﹖",
"﹗",
"﹙",
"﹚",
"﹛",
"﹜",
"﹝",
"﹞",
"﹟",
"﹠",
"﹡",
"﹢",
"﹣",
"﹤",
"﹥",
"﹦",
"﹨",
"﹩",
"﹪",
"﹫",
"！",
"＂",
"＃",
"＄",
"％",
"＆",
"＇",
"（",
"）",
"＊",
"＋",
"，",
"－",
"．",
"／",
"０",
"１",
"２",
"３",
"４",
"５",
"６",
"７",
"８",
"９",
"：",
"；",
"＜",
"＝",
"＞",
"？",
"＠",
"［",
"＼",
"］",
"＾",
"＿",
" ",
"｀",
"｛",
"｜",
"｝",
"～",
"￠",
"￡",
"￢",
"￣",
"￤",
"￥",
"丨",
"丩",
"丬",
"丶",
"丷",
"丿",
"乀",
"乁",
"乂",
"乄",
"乆",
"乛",
"亅",
"亠",
"亻",
"冂",
"冫",
"刂",
"讠",
"辶",
"釒",
"钅",
"阝",
"飠",
"牜",
"饣",
"卪",
"厸",
"厶",
"厽",
"孓",
"宀",
"巛",
"巜",
"彳",
"廴",
"彡",
"彐",
"彳",
"忄",
"扌",
"攵",
"氵",
"灬",
"爫",
"犭",
"疒",
"癶",
"礻",
"糹",
"纟",
"罒",
"罓",
"耂",
"艹",
"虍",
"訁",
"覀",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"兦",
"亼",
"亽",
"亖",
"亗",
"吂",
"凸",
"凹",
"卝",
"卍",
"卐",
"匸",
"皕",
"旡",
"玊",
"尐",
"幵",
"朩",
"囘",
"囙",
"囚",
"四",
"囜",
"囝",
"回",
"囟",
"因",
"囡",
"团",
"団",
"囤",
"囥",
"囦",
"囧",
"囨",
"囩",
"囪",
"囫",
"囬",
"园",
"囮",
"囯",
"困",
"囱",
"囲",
"図",
"围",
"囵",
"囶",
"囷",
"囸",
"囹",
"固",
"囻",
"囼",
"国",
"图",
"囿",
"圀",
"圁",
"圂",
"圃",
"圄",
"圅",
"圆",
"圇",
"圈",
"圉",
"圊",
"國",
"圌",
"圍",
"圎",
"圏",
"圐",
"圑",
"園",
"圓",
"圔",
"圕",
"圖",
"圗",
"團",
"圙",
"圚",
"圛",
"圜",
"圝",
"圞",
"餮",
".",
"、",
"︶",
"ㄣ",
"┈",
"┾",
"べ",
"_",
"",
"╄",
"→",
"^",
"o",
"^",
"^",
"ǒ",
"^",
"╭",
"ァ",
"",
"_",
"",
"/",
"~",
"↘",
"▄",
"︻",
"┻",
"┳",
"═",
"一",
"じ",
"ò",
"ぴ",
"é",
".",
".",
".",
".",
".",
".",
"",
";",
"",
"┢",
"┦",
"(",
"*",
"^",
"﹏",
"^",
"*",
")"
};

//
static short g_sBabyArray[] ={256,257,512,513,768,769};
TCHAR *g_szBabyName[]={"纽特龙","帕特狮"};
void USER::AccountLoginReqWithDbServer(TCHAR *pBuf)
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

	if ( IsDoubleAccount(id) )
	{
		error_code = ERR_2;
		index = 0;
		SetByte(m_TempBuf, ACCOUNT_LOGIN_RESULT, index );
		SetByte(m_TempBuf, result, index );

		SetByte(m_TempBuf, error_code, index);
		Send( m_TempBuf, index );
		SoftClose();
		return;
	}

	strcpy( m_strAccount, id );

	char TempBuf[1024];
	index = 0;
	SetByte(TempBuf, g_ServerId, index );
	SetByte(TempBuf, ACCOUNT_LOGIN_REQ, index );
	SetShort(TempBuf, m_uid, index );
	SetString(TempBuf, id, ACCOUNT_LENGTH, index);
	g_pMainDlg->Send2DBsvr( TempBuf, index );

}

void USER::GameStartWithDbServer(TCHAR *pBuf)
{
	int			index = 0;
	BYTE		result = FAIL;
	TCHAR		szID[CHAR_NAME_LENGTH+1];

	int	nIDLength = GetVarString(sizeof(szID), szID, pBuf, index);
	if ( nIDLength == 0 || nIDLength > CHAR_NAME_LENGTH )
	{
		CBufferEx TempBuf;

		ReInitMemoryDB();

		TempBuf.Add(GAME_START_RESULT);
		TempBuf.Add(result);
		TempBuf.Add(ERR_1);
		Send(TempBuf, TempBuf.GetLength());
		SoftClose();
		return;
	}

	memcpy( m_strUserID, szID, CHAR_NAME_LENGTH );

	char TempBuf[1024];
	index = 0;
	SetByte(TempBuf, g_ServerId, index );
	SetByte(TempBuf, GAME_START_REQ, index );
	SetShort(TempBuf, m_uid, index );
	SetString(TempBuf, szID, CHAR_NAME_LENGTH, index);
	g_pMainDlg->Send2DBsvr( TempBuf, index );
}

void USER::LogOutWithDbServer()
{
	if(m_bLogOut == TRUE) return;
	if(m_state != STATE_GAMESTARTED) return;	// 捞寇 STATE_CONNECTED殿 老锭绰 秦寸 檬扁拳啊 皋葛府俊绝栏骨肺 力寇秦具茄促.

	m_bLogOut = TRUE;
	USER *pUser = NULL;
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

	{
	// 
	//UpdateMemBankDataOnly();
	//UpdateUserData(TRUE);
	char send_buffer[RECEIVE_BUF_SIZE+1];
	int send_index = 0, c_index = 0;;
	TCHAR strBankItem[_BANK_DB];

	SetByte(send_buffer, g_ServerId, send_index );
	SetByte( send_buffer, (BYTE)LOGOUT_REQ, send_index );
	SetShort( send_buffer, m_uid, send_index );

	// UpdateMemBankDataOnly() PART...
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	::CopyMemory(strBankItem, m_pMD->m_UB.m_UserBankItem, _BANK_DB);

	c_index = send_index;

	SetString( send_buffer, m_strUserID, CHAR_NAME_LENGTH, send_index );
	SetDWORD( send_buffer, m_pMD->m_UB.m_dwBankDN, send_index );
	SetString( send_buffer, strBankItem, sizeof(strBankItem), send_index );
	//

	// UpdateUserData(TRUE) PART...
	DBUpdateUserData( send_buffer, send_index );
	//

	// 拘绵窍扁...
	m_CompMng.FlushAddData();
	m_CompMng.AddData( &send_buffer[c_index], (send_index-c_index) );

	m_CompMng.PreCompressWork();
	m_CompMng.Compress();

	int comp_data_len = m_CompMng.GetCompressedDataCount();
	int org_data_len = m_CompMng.GetUnCompressDataLength();
	DWORD crc_value = m_CompMng.GetCrcValue();
	char *packet_buffer;
	packet_buffer = m_CompMng.GetExtractedBufferPtr();

	SetShort( send_buffer, comp_data_len, c_index );
	SetShort( send_buffer, org_data_len, c_index );
	SetDWORD( send_buffer, crc_value, c_index );
	SetString( send_buffer, packet_buffer, comp_data_len, c_index );
	m_CompMng.FlushAddData();

	// 傈价.
	g_pMainDlg->Send2DBsvr( send_buffer, c_index );
	}

	m_pMD->m_UB.m_uid = -1;

	ReInitMemoryDB();

	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == USER_BAND + m_uid ) 
		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);

	SendMyInfo(TO_INSIGHT, INFO_DELETE);

	m_state = STATE_LOGOUT;
}

void USER::DBUpdateUserData(char *temp_buffer, int &temp_index )
{
	TCHAR			szSQL[8000];		
	TCHAR			strFace[10], strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB], strTel[_TEL_DB];
	TCHAR			strQuickItem[_QUICKITEM_DB];
	TCHAR			strHaveEvent[_EVENT_DB];
	

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
	  //////////////////加字段
    _sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_DATA (\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,\
		?, %d,%d,%d, %d, %d,%d,  %d,%d,  %d, \
		%d,%d,%d,%d,%d,%d, %d,%d,%d,%d, \
		?,?,?,?, %d,%d,\
		%d, %d, ?, %d, %d,\
		?, %d,\
		%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,%d,%d,%d,%d,%d,\
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

	int sql_len = strlen(szSQL);
	SetShort( temp_buffer, sql_len, temp_index );
	SetString( temp_buffer, szSQL, sql_len, temp_index );

	//SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strFace),	0, (TCHAR*)strFace,		0, &sFaceLen );
	SetString( temp_buffer, strFace, sizeof(strFace), temp_index );

	//SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strSkill),	0, (TCHAR*)strSkill,	0, &sSkillLen );
	SetString( temp_buffer, strSkill, sizeof(strSkill), temp_index );

	//SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
	SetString( temp_buffer, strItem, sizeof(strItem), temp_index );

	//SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strPsi),		0, (TCHAR*)strPsi,		0, &sPsiLen );
	SetString( temp_buffer, strPsi, sizeof(strPsi), temp_index );

	//SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strHaveEvent),	0, (TCHAR*)strHaveEvent,	0, &sEventLen );
	SetString( temp_buffer, strHaveEvent, sizeof(strHaveEvent), temp_index );

	//SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strQuickItem),	0, (TCHAR*)strQuickItem,	0, &sQuickLen );
	SetString( temp_buffer, strQuickItem, sizeof(strQuickItem), temp_index );

	//SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strTel),			0, (TCHAR*)strTel,			0, &sTelLen );
	SetString( temp_buffer, strTel, sizeof(strTel), temp_index );

	
}


//-----------------------------------------------------------------------------
// DB PHASER PART...
//-----------------------------------------------------------------------------


void USER::DBAccountLoginResult(TCHAR *pBuf, int s_index, int dlength )
{
	int		index = 0, i;
	BYTE	result = FAIL, error_code = 0;
	int		old_index = 0;
	int		s_result, length;

	strcpy(m_strChar[0], "");
	strcpy(m_strChar[1], "");
	strcpy(m_strChar[2], "");
	m_nCharNum = 0;

	s_result = GetByte( pBuf, s_index );
	if ( s_result == FALSE )
	{
		// Load Character Data Fail...
		error_code = UNKNOWN_ERR;
		goto result_send;
	}

	m_state = STATE_CONNECTED;
	result = SUCCESS;

	// LOGIN SERVER俊辑 掘篮 某腐磐 肮荐+捞抚 汗荤...
	m_nCharNum = GetByte( pBuf, s_index );
	for ( i = 0; i < 3; i++ )
	{
		GetString(m_strChar[i], pBuf, 20, s_index );
	}
	//
	length = dlength - s_index;
	if ( length <= 0 ) result = FAIL;

result_send:
	index = 0;
	SetByte(m_TempBuf, ACCOUNT_LOGIN_RESULT, index );
	SetByte(m_TempBuf, result, index );

	if ( result == FAIL )
	{
		SetByte(m_TempBuf, error_code, index);
		Send( m_TempBuf, index );
		SoftClose();
		return;
	}

	// LOGIN SERVER俊辑 掘篮 某腐磐 肮荐+沥焊 汗荤...
	SetString(m_TempBuf, (pBuf+s_index), length, index);

	Send(m_TempBuf, index);
}

void USER::DBGameStartResult( char *pBuf, int s_index, int dlength )
{

}

void USER::DressingWindowOpen()
{
	CBufferEx TempBuf;

	TempBuf.Add(DRESSING_WINDOW_OPEN);

	Send(TempBuf, TempBuf.GetLength());
}

void USER::DressingReq(TCHAR *pBuf)
{
	int index = 0;

	int itemslot = (int)GetShort( pBuf, index );
	int itemcount = (int)GetShort( pBuf, index );

	int i = 0;

	CItemTable* pItem = NULL;
	ItemList* pOrgItem = NULL;

	DRESSING_DATA* pDS = NULL;

	int makecount = itemcount / 10;
	int termcount = 0;
	int randseed[200];
	int randcount = 0;
	int randindex = 0;
	int randresult = -1;
	int j = 0;
	int k = 0;

	DressingResultArray arResult;	
	DRESSING_RESULT* pNewDRResult = NULL;
	BOOL bSameExist = FALSE;

	ItemListArray	arEmpty, arSame;
	CWordArray		arEmptySlot, arSameSlot;
	ItemList		MyItem[TOTAL_ITEM_NUM], SameItem[TOTAL_ITEM_NUM];
	int				pTotalEmpty[INVENTORY_NUM];

	short sid = -1, num = -1; 
	int iSlot, iEmptyNum = 0;
	int iWeight = 0;
	int iBasicItemWeight = 0;

	// 函荐 檬扁拳 ------------------------------------------------//
	BOOL bFlag = FALSE;
	DWORD dwCost = 0;

	DWORD dwTemp = m_dwDN;

	ItemList	*pDSItem = NULL;

	CBufferEx TempBuf;

	int iItemSize = g_arItemTable.GetSize();

	if( itemslot < EQUIP_ITEM_NUM || itemslot >= TOTAL_INVEN_MAX )
	{
		// 角菩 内靛 傈价 - 盔籍捞 乐绰 牢亥狼 浇吩 锅龋啊 肋给登菌娇
		SendSystemMsg( IDS_USER_CANT_DRESSING_ITEM, SYSTEM_ERROR, TO_ME);
		bFlag = TRUE; goto go_dressing_result;
	}

	if( itemcount >= 32767 || itemcount < 0 )
	{
		// 角菩 内靛 傈价 - 急堡窍妨绰 盔籍狼 俺荐 Overflow
		SendSystemMsg( IDS_USER_INVALID_DRESSING_COUNT, SYSTEM_ERROR, TO_ME);
		bFlag = TRUE; goto go_dressing_result;
	}

	if( itemcount % 10 )
	{
		// 角菩 内靛 傈价 - 急堡窍妨绰 盔籍狼 俺荐啊 10俺窜困啊 酒丛
		SendSystemMsg( IDS_USER_DRESSING_COUNT_FORMAT, SYSTEM_ERROR, TO_ME);
		bFlag = TRUE; goto go_dressing_result;
	}

	pOrgItem = &m_UserItem[itemslot];

	if( !pOrgItem )
	{
		// 角菩 内靛 傈价 - 牢亥俊辑 盔籍阑 茫阑 荐 绝澜
		SendSystemMsg( IDS_USER_CANT_DRESSING, SYSTEM_ERROR, TO_ME);
		bFlag = TRUE; goto go_dressing_result;
	}
	
	if( m_UserItem[itemslot].sSid >= 0 && m_UserItem[itemslot].sSid < g_arItemTable.GetSize() )
	{
		pItem = g_arItemTable[m_UserItem[itemslot].sSid];
	}

	if( !pItem )
	{
		// 角菩 内靛 傈价 - 酒捞袍 抛捞喉俊辑 酒捞袍 茫阑 荐 绝澜
		SendSystemMsg( IDS_USER_CANT_DRESSING_ITEM, SYSTEM_ERROR, TO_ME);
		bFlag = TRUE; goto go_dressing_result;
	}

	if( pItem->m_byWear != 107 )
	{
		// 角菩 内靛 傈价 - 急堡且 荐 绝绰 酒捞袍
		SendSystemMsg( IDS_USER_CANT_DRESSING_ITEM, SYSTEM_ERROR, TO_ME);
		bFlag = TRUE; goto go_dressing_result;
	}

	if( m_UserItem[itemslot].sCount < itemcount )
	{
		// 角菩 内靛 傈价 - 酒捞袍 抛捞喉俊辑 酒捞袍 茫阑 荐 绝澜
		SendSystemMsg( IDS_USER_INVALID_DRESSING_COUNT, SYSTEM_ERROR, TO_ME);
		bFlag = TRUE; goto go_dressing_result;
	}

	iBasicItemWeight = pItem->m_byWeight * itemcount;

	for( i = 0; i < g_arDressingData.GetSize(); i++ )
	{
		if( g_arDressingData[i] )
		{
			if( g_arDressingData[i]->sItemSid == pItem->m_sSid )
			{
				pDS = g_arDressingData[i];
				break;
			}
		}
	}

	if( !pDS )
	{
		// 角菩 内靛 傈价 - 急堡 抛捞喉俊辑 秦寸 盔籍阑 茫阑 荐 绝澜
		SendSystemMsg( IDS_USER_CANT_DRESSING_ITEM, SYSTEM_ERROR, TO_ME);
		bFlag = TRUE; goto go_dressing_result;
	}

	for( i = 0; i < makecount; i++ )
	{
		termcount = myrand( pDS->sCountMin, pDS->sCountMax );

		// 酒捞袍 急沥 罚待 矫靛 檬扁拳
		for( j = 0; j < 200; j++ )
		{
			randseed[j] = -1;
		}

		randindex = 0;
		for( j = 0; j < 10; j++ )
		{
			for( k = 0; k < pDS->sItemRatio[j]; k++ )
			{
				if( randindex >= 100 ) break;

				randseed[randindex] = pDS->sItem[j];
				randindex++;
			}
		}

		for( j = 0; j < termcount; j++ )
		{
			randcount = myrand( 0, 99 );

			randresult = randseed[randcount];

			if( randresult < 0 )
			{
				// 角菩 内靛 傈价 - 抛捞喉 沥焊啊 肋给 涝仿登绢 乐促(酒捞袍 急沥 犬伏捞 配呕 100捞 酒聪促)
				SendSystemMsg( IDS_USER_CANT_DRESSING, SYSTEM_ERROR, TO_ME);
				bFlag = TRUE; goto go_dressing_result;
			}

			bSameExist = FALSE;
			for( k = 0; k < arResult.GetSize(); k++ )
			{
				if( arResult[k] )
				{
					if( arResult[k]->sSid == randresult )
					{
						arResult[k]->sCount++;
						bSameExist = TRUE;
						break;
					}
				}
			}

			if( !bSameExist )
			{
				pNewDRResult = new DRESSING_RESULT;
				pNewDRResult->sSid = randresult;
				pNewDRResult->sCount = 1;
				arResult.Add( pNewDRResult );
			}
		}
	}

	if( arResult.GetSize() == 0 )
	{
		// 角菩 内靛 傈价 - 父甸绢柳 巴捞 绝促
		SendSystemMsg( IDS_USER_DRESSING_NO_RESULT, SYSTEM_ERROR, TO_ME);
//		bFlag = TRUE; goto go_dressing_result;
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++)		// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
		SameItem[i] = m_UserItem[i];
	}

	arSameSlot.RemoveAll();
	arSame.RemoveAll();
	arEmpty.RemoveAll();
	arEmptySlot.RemoveAll();

	::ZeroMemory(pTotalEmpty, sizeof(pTotalEmpty));

	GetEmptyInvenSlot(pTotalEmpty);

	char strResult[1024];
	char strTempResult[128];
	sprintf( strResult, _ID(IDS_USER_DRESSING_RESULT) );

	// 急堡 搬苞 格废 ---------------------------------------------------//
	for( i = 0; i < arResult.GetSize(); i++ )
	{
		if( !arResult[i] ) continue;

		iSlot = -1;

		sid = (short)arResult[i]->sSid;
		num = (short)arResult[i]->sCount;

		if(iEmptyNum > INVENTORY_NUM) { bFlag = TRUE; goto go_dressing_result; }
		if(sid < 0 || num <= 0 || sid > iItemSize) { bFlag = TRUE; goto go_dressing_result; }

		pDSItem = NULL;
		pDSItem = new ItemList;
		if(pDSItem == NULL) { bFlag = TRUE; goto go_dressing_result; }

		ReSetItemSlot(pDSItem);

		pDSItem->sLevel = g_arItemTable[sid]->m_byRLevel;
		pDSItem->sSid = sid;
		pDSItem->sCount = num;
		pDSItem->sDuration = g_arItemTable[sid]->m_sDuration;
		pDSItem->sBullNum = g_arItemTable[sid]->m_sBullNum;
		for( k =0; k< MAGIC_NUM; k++ ) pDSItem->tMagic[k] = 0;
		pDSItem->tIQ = 0;
		pDSItem->iItemSerial = 0;

		sprintf( strTempResult, "%s(%d) ", g_arItemTable[sid]->m_strName, num );
		strcat( strResult, strTempResult );

		iSlot = GetSameItem(*pDSItem, INVENTORY_SLOT);

		if(iSlot != -1)
		{ 
			if(num != 0)
			{
				pDSItem->sCount = num;
				arSame.Add(pDSItem); 
				arSameSlot.Add(iSlot); 

				iWeight += g_arItemTable[sid]->m_byWeight * num;
			}
		}
		else			
		{
			iSlot = pTotalEmpty[iEmptyNum];
//			if(iSlot == 0) { bFlag = TRUE; goto go_dressing_result; }
			if(iSlot == 0) 
			{ 
				CPoint ptCell = FindNearRandomPoint(m_curx, m_cury);

				if(ptCell.x < 0 || ptCell.y < 0) { if(pDSItem) { delete pDSItem; pDSItem = NULL; } continue; }
				if(ptCell.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptCell.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) { if(pDSItem) { delete pDSItem; pDSItem = NULL; } continue; }

				if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, (long)1, (long)0) == (long)0)
				{
					if(m_iMaxWeight < m_iCurWeight + iWeight - iBasicItemWeight)
					{
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);

						if(pDSItem) { delete pDSItem; pDSItem = NULL; }

						SendSystemMsg( IDS_USER_OVER_WEIGHT1, SYSTEM_ERROR, TO_ME);
						goto go_dressing_result;
					}

					pDSItem->tType = TYPE_ITEM;
					pDSItem->uid[0] = m_uid;				// 快急 鉴困
					pDSItem->SuccessRate[0] = (BYTE)100;	// 快急 鉴困 厚啦
					pDSItem->dwTime = ConvertCurTimeToSaveTime();

					//m_pCom->DelThrowItem();
					m_pCom->SetThrowItem( pDSItem, ptCell.x, ptCell.y, m_ZoneIndex );

					::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);
				}
				else 
				{
					if(pDSItem) { delete pDSItem; pDSItem = NULL; }
				}
			}
			else
			{
				arEmpty.Add(pDSItem); 
				arEmptySlot.Add(iSlot); 
				iEmptyNum++;

				iWeight += g_arItemTable[sid]->m_byWeight * num;
			}
		}		
	}

	if(m_iMaxWeight < m_iCurWeight + iWeight - iBasicItemWeight)
	{
		SendSystemMsg( IDS_USER_OVER_WEIGHT1, SYSTEM_ERROR, TO_ME);
		goto go_dressing_result;
	}

	// 格废阑 牢亥俊 -------------------- --------------------//
	for( i = 0; i < arSame.GetSize(); i++ )
	{
		CheckMaxValue((short &)m_UserItem[arSameSlot[i]].sCount, (short)arSame[i]->sCount); 
	}

	for(i = 0; i < arEmpty.GetSize(); i++)
	{
		m_UserItem[arEmptySlot[i]].sLevel = arEmpty[i]->sLevel;
		m_UserItem[arEmptySlot[i]].sSid = arEmpty[i]->sSid;
		m_UserItem[arEmptySlot[i]].sCount = arEmpty[i]->sCount;
		m_UserItem[arEmptySlot[i]].sBullNum = arEmpty[i]->sBullNum;
		m_UserItem[arEmptySlot[i]].sDuration = arEmpty[i]->sDuration;
		m_UserItem[arEmptySlot[i]].tIQ = arEmpty[i]->tIQ;
		for(j =0; j < MAGIC_NUM; j++) m_UserItem[arEmptySlot[i]].tMagic[j] = arEmpty[i]->tMagic[j];
	}

	// 盔籍阑 绝局绰 内靛 鞘夸窃 !!!!
	if( pOrgItem->sCount ==  itemcount ) ReSetItemSlot( &m_UserItem[itemslot] );//ReSetItemSlot(sSlot);
	else m_UserItem[itemslot].sCount = m_UserItem[itemslot].sCount - (short)itemcount;

	if(UpdateUserItemDN() == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// 酒捞袍 沥焊 券盔
		{
			m_UserItem[i] = MyItem[i];
		}
		SendSystemMsg( IDS_USER_DRESSING_FAIL, SYSTEM_NORMAL, TO_ME);
		bFlag = TRUE;
		goto go_dressing_result;
	}

	m_iCurWeight += ( iWeight - iBasicItemWeight );
	GetRecoverySpeed();

	UpdateInvenSlot(&arEmptySlot, &arSameSlot);

	SendSystemMsg( strResult, SYSTEM_ERROR, TO_ME);

	TempBuf.Add(DRESSING);
	TempBuf.Add(SUCCESS);

	TempBuf.Add( (short)itemslot );
	TempBuf.Add(pOrgItem->sLevel);
	TempBuf.Add(pOrgItem->sSid);
	TempBuf.Add(pOrgItem->sDuration);
	TempBuf.Add(pOrgItem->sBullNum);
	TempBuf.Add(pOrgItem->sCount);
	for( i = 0; i < MAGIC_NUM; i++ ) TempBuf.Add(pOrgItem->tMagic[i]);
	TempBuf.Add(pOrgItem->tIQ);

	Send(TempBuf, TempBuf.GetLength());

go_dressing_result:
	int dddd = 0;

	if( bFlag )
	{
		TempBuf.Add(DRESSING);
		TempBuf.Add(FAIL);

		Send(TempBuf, TempBuf.GetLength());
	}

	// 皋葛府 秦力 -------------------- --------------------//
	for(i = 0; i < arSame.GetSize(); i++)
	{
		if(arSame[i] != NULL) delete arSame[i];
	}
	arSame.RemoveAll();
	arSameSlot.RemoveAll();
	for(i = 0; i < arEmpty.GetSize(); i++)
	{
		if(arEmpty[i] != NULL) delete arEmpty[i];
	}
	arEmpty.RemoveAll();
	arEmptySlot.RemoveAll();

	for(i = 0; i < arResult.GetSize(); i++ )
	{
		if( arResult[i] ) delete arResult[i];
	}
	arResult.RemoveAll();
}

void USER::ItemDataVersionCheck()
{
/*
	int i;

	switch( m_tItemVer )
	{
	case	0x00:
		{
			for(i = 0; i < TOTAL_ITEM_NUM; i++)
			{
				m_UserItem[i].iItemSerial = GenerateItemSerial( &(m_UserItem[i]) );
			}

			for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
			{
				m_UserBankItem[i].iItemSerial = GenerateItemSerial( &(m_UserBankItem[i]) );
			}

			m_tItemVer = 0x01;

			UpdateMemUserAll( TRUE );
		}

	case	0x01:
	default:
		break;
	}

	switch( m_tAccountBankItemVer )
	{
	case	0X00:
		{
			for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++)
			{
				m_AccountBankItem[i].iItemSerial = GenerateItemSerial( &(m_AccountBankItem[i]) );
			}

			m_tAccountBankItemVer = 0x01;

			UpdateMemUserAll( TRUE );
		}
	}
*/	
}

void USER::SelectOver100Skill(TCHAR *pBuf)
{
	if( m_sLevel < 100 ) return;		// 100乏 捞惑父 急琶 且 荐 乐促.

	int emptyskillslot = -1;

/*
#define BRAWL				0
#define STAFF				1
#define	EDGED				2
#define FIREARMS			3
*/
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

	if( emptyskillslot == -1 ) return;	// 弊 拌凯俊辑 荤侩窍瘤 臼绊 乐绰 付瘤阜 胶懦

	int skillsid = m_UserSkill[emptyskillslot].sSid;

	if(skillsid < 0 || skillsid >= g_arSkillTable.GetSize()) return;

	if( g_arSkillTable[skillsid]->m_sSid != -1 )		// 捞固 100乏 捞惑 胶懦阑 榜耳阑锭
	{
		if( emptyskillslot != skillsid ) return;
	}

	int index = 0;

	int selectedskill = GetByte( pBuf, index );

	if( selectedskill < 20 || selectedskill >= 24 ) return;	// 努扼捞攫飘俊辑 逞绢坷绰 胶懦 锅龋绰 0-3捞促

/*	20 - 竿傍
	21 - 例措规绢
	22 - 荤捞疟 历亲
	23 - 叼奇胶 诀 */

/*	if(selectedskill == 20)
	{
		if(m_byClass != 0)
		{
			SendUserStatusSkill();
			return;
		}
	} */

	int backupskill = m_UserSkill[emptyskillslot].sSid;
	int backupskilllevel = m_UserSkill[emptyskillslot].tLevel;

	m_UserSkill[emptyskillslot].sSid = selectedskill;
	m_UserSkill[emptyskillslot].tLevel = (BYTE)((m_sLevel % 100) + 1);
//niuniuadd
	if(m_UserSkill[emptyskillslot].tLevel>20)
			m_UserSkill[emptyskillslot].tLevel=20;
	if( !UpdateUserData() )
	{
		m_UserSkill[emptyskillslot].sSid = backupskill;
		m_UserSkill[emptyskillslot].tLevel = backupskilllevel;
		return;
	}

	SendUserStatusSkill();
}

UINT64 USER::GenerateItemSerial(ItemList* pItem)
{
	/*if( pItem->tIQ == MAGIC_ITEM ||		// 概流酒捞袍捞芭唱
		pItem->tIQ == RARE_ITEM ||		// 贰绢酒捞袍捞芭唱
		pItem->sLevel >= 20 ||			// 饭骇 20捞惑 酒捞袍捞芭唱
		pItem->tMagic[5] >= 1 ||		// 诀弊饭捞靛啊 1锅捞惑 等 酒捞袍捞扼搁
		pItem->sSid == 756 )			// 必距
	{
		CTime t;

		t = CTime::GetCurrentTime();

		MYINT upper;		// 矫埃栏肺 惑困 4官捞飘甫 父电促
		MYINT under;		// 矫府倔肺 窍困 3官捞飘甫 唱赣瘤 茄官捞飘绰 辑滚 老访锅龋

		memcpy( &(upper), &t, sizeof(CTime) );
		TRACE("111111: %d   =? %d\n", sizeof(CTime),sizeof(upper.i) );
		under.i = (int)(g_dwItemSerial);
		under.b[3] = g_ItemSerialIndex;

		g_dwItemSerial++;

		if( g_dwItemSerial >= 255*255*255 ) g_dwItemSerial = 0;

		MYINT64 total;

		total.b[7] = upper.b[3];
		total.b[6] = upper.b[2];
		total.b[5] = upper.b[1];
		total.b[4] = upper.b[0];

		total.b[3] = under.b[3];
		total.b[2] = under.b[2];
		total.b[1] = under.b[1];
		total.b[0] = under.b[0];

		TRACE("GenerateSerial - %I64d\n", total.i );

		return total.i;

	}
	*/

	return 0;
}

void USER::AccountBankItemMoveReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int index = 0;
	int bSuccess = FALSE;
	BYTE type = GetByte(pBuf, index);
	if(!LoadCharData(m_strAccount))
	{
		return;
	}

	for (int i =0; i < 3 ; i++)	//bug 防止盗号刷装备
	{
		if (m_strChar[i][0] == 0)
			continue;
		if (strcmp(m_strChar[i],m_strUserID) == 0)
			bSuccess = TRUE;
	}

	if (bSuccess == FALSE)	//BUG  
		return;

	if (m_bNowTrading == TRUE || m_bPShopOpen == TRUE) 
	   {
            CString strDate ="";
			SYSTEMTIME st;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
			TCHAR m_Log[500];
			sprintf_s(m_Log,"[ %s ]%s %d,%d 物品移动存在非法5 \r\n",strDate,m_strUserID,m_bNowTrading,m_bPShopOpen);
		    WriteUserShopLog(m_Log);
			SendItemMoveFail();
		    return;
	 }           
	////////////////////////////////////////

	switch(type)
	{
	case BANK_ITEM_INPUT:
		AccountBankInPut(pBuf + index);
		break;
	case BANK_ITEM_OUTPUT:
		AccountBankOutPut(pBuf + index);
		break;
	case BANK_DN_INPUT:
		AccountBankInPutDN(pBuf + index);
		break;
	}
}

void USER::AccountBankInPut(TCHAR *pBuf)
{
	int i;
	int tDestSlot;
	int index = 0;
	int iOver = 0;
	short sSid = -1;
	short sHaveCount = 0;
	DWORD dwCost = 150;

	BYTE result = SUCCESS;

	CBufferEx TempBuf;

	ItemList MyItem[TOTAL_ITEM_NUM], BackItem;

	BYTE tSourceSlot = GetByte(pBuf, index);	// 荤侩磊 酒捞袍捞 乐带 浇吩 困摹
	short sCount = GetShort(pBuf, index);		// 倔付父怒 焊包
												// 坷肺瘤 牢亥父 啊瓷窍促.			
	if(tSourceSlot < EQUIP_ITEM_NUM || tSourceSlot >= EQUIP_ITEM_NUM + INVENTORY_NUM) { result = FAIL; goto go_result; }

	sSid = m_UserItem[tSourceSlot].sSid;
	sHaveCount = m_UserItem[tSourceSlot].sCount;
	if(sHaveCount <= 0) { result = FAIL; goto go_result; }	// 酒捞袍捞 绝促.
												
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { result = FAIL; goto go_result; }	// 肋给等 sSid

	if(g_arItemTable[sSid]->m_sEvent >= EVENT_RR_ITEM_BAND)		// 肺凯烦喉 惑前篮 烹钦芒绊俊 持阑 荐 绝促.
	{
		SendSystemMsg( IDS_USER_CANT_SHARE_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
		result = FAIL; goto go_result; 
	}

	if(sCount > ACCOUNT_BANK_MAX_ITEM) 
	{
		SendSystemMsg( IDS_USER_SAME_ITEM_COUNT_MAX, SYSTEM_ERROR, TO_ME);
		result = FAIL; goto go_result;
	}
												
	if(sCount <= 0 || sCount > sHaveCount) { result = FAIL; goto go_result; }	// 家瘤茄 酒捞袍 荐焊促 腹阑 版快
												//郴备档 眉农甫 wear沥焊肺 沁促.
	//if(g_arItemTable[sSid]->m_byWear <= 5 && sCount > 1) { result = FAIL; goto go_result; }
	if(g_arItemTable[sSid]->m_sDuration > 0 && sCount > 1) { result = FAIL; goto go_result; }	// 般磨荐 绝绰单 肮荐啊 1焊促 努荐 绝促.
	
	for(i = 0; i < TOTAL_ITEM_NUM; i++)	// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

	ReSetItemSlot(&BackItem);					// DB角菩俊 措厚茄 归诀侩 檬扁拳
												// 老窜 般磨荐 乐绰瘤, 般摹搁 鞍篮 酒捞袍捞 乐绰瘤 茫酒夯促.
	tDestSlot = GetSameItem(m_UserItem[tSourceSlot], ACCOUNT_BANK_SLOT);
	
	/***************************篮青 诀公 贸府********************************************/
	if(tDestSlot >= 0)							// 亲惑 货肺捞 眠啊 登绰巴阑 扁霖栏肺 贸府
	{											
		BackItem = m_AccountBankItem[tDestSlot];

		if(m_AccountBankItem[tDestSlot].sCount >= ACCOUNT_BANK_MAX_ITEM)
		{
			SendSystemMsg( IDS_USER_SAVE_MAX_COUNT, SYSTEM_ERROR, TO_ME);
			result = FAIL; goto go_result;
		}

		if((m_AccountBankItem[tDestSlot].sCount + sCount) > ACCOUNT_BANK_MAX_ITEM)
		{										// MAX蔼阑 逞栏搁 菜盲快绊 唱赣瘤 贸府			
			iOver = m_AccountBankItem[tDestSlot].sCount + sCount - ACCOUNT_BANK_MAX_ITEM;
			if(iOver <= 0) { result = FAIL; goto go_result; }

			m_AccountBankItem[tDestSlot].sCount = ACCOUNT_BANK_MAX_ITEM;
			sCount = sCount - iOver;
		}
		else m_AccountBankItem[tDestSlot].sCount += sCount;
	}
	else
	{											//	眠啊
		tDestSlot = GetEmptySlot(ACCOUNT_BANK_SLOT);

		if(tDestSlot == -1) 
		{
			result = FAIL; goto go_result; 
		}

		m_AccountBankItem[tDestSlot].sLevel = m_UserItem[tSourceSlot].sLevel;
		m_AccountBankItem[tDestSlot].sSid = m_UserItem[tSourceSlot].sSid;
		m_AccountBankItem[tDestSlot].sDuration = m_UserItem[tSourceSlot].sDuration;
		m_AccountBankItem[tDestSlot].sBullNum = m_UserItem[tSourceSlot].sBullNum;
		m_AccountBankItem[tDestSlot].sCount = sCount;
		for(i = 0; i < MAGIC_NUM; i++) m_AccountBankItem[tDestSlot].tMagic[i] = m_UserItem[tSourceSlot].tMagic[i];
		m_AccountBankItem[tDestSlot].tIQ = m_UserItem[tSourceSlot].tIQ;
		m_AccountBankItem[tDestSlot].iItemSerial = m_UserItem[tSourceSlot].iItemSerial;
	}

	index = 0;
	index = g_arItemTable[m_UserItem[tSourceSlot].sSid]->m_byWeight * sCount;
	/**************************蜡历 牢亥 贸府*********************************************/
	if(sCount >= sHaveCount && iOver == 0)
	{
		MakeItemLog( &m_AccountBankItem[tDestSlot], ITEMLOG_ACCOUNT_BANKIN );
		ReSetItemSlot(&m_UserItem[tSourceSlot]);	
	}
	else m_UserItem[tSourceSlot].sCount -= sCount;
	
	/**************************DB Update 贸府*********************************************/
	if(UpdateUserBank() == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 汗盔
		{
			m_UserItem[i] = MyItem[i];
		}
		m_AccountBankItem[tDestSlot] = BackItem;

		result = FAIL;
		FlushItemLog( FALSE );
		goto go_result;
	}

	FlushItemLog( TRUE );

go_result:
	TempBuf.Add(ACCOUNT_BANK_ITEM_MOVE_RESULT);

	if(result == FAIL)
	{
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	result = (BYTE)0x01;
	TempBuf.Add(result);

	TempBuf.Add((BYTE)tDestSlot);				// 银行里东西。
	TempBuf.Add( dwCost );
	TempBuf.Add(m_AccountBankItem[tDestSlot].sLevel);
	TempBuf.Add(m_AccountBankItem[tDestSlot].sSid);
	TempBuf.Add(m_AccountBankItem[tDestSlot].sDuration);
	TempBuf.Add(m_AccountBankItem[tDestSlot].sBullNum);
	TempBuf.Add(m_AccountBankItem[tDestSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_AccountBankItem[tDestSlot].tMagic[i]);
	TempBuf.Add(m_AccountBankItem[tDestSlot].tIQ);

	TempBuf.Add((BYTE)tSourceSlot);				// 身上的东西
	TempBuf.Add(m_UserItem[tSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[tSourceSlot].sSid);
	TempBuf.Add(m_UserItem[tSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[tSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSourceSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tSourceSlot].tMagic[i]);
	TempBuf.Add(m_UserItem[tSourceSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());

	m_iCurWeight -= index;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...
}

void USER::AccountBankOutPut(TCHAR *pBuf)
{
	int i, j, iWeight = 0;
	int tDestSlot;
	int index = 0;
	int iOver = 0;
	short sSid = -1;
	short sHaveCount = 0;

	short myslot;
	BYTE result = SUCCESS;

	CWordArray		arChangeMy, arChangeBank;
	ItemList		MyItem[TOTAL_ITEM_NUM], BankItem[TOTAL_ACCOUNT_BANK_ITEM_NUM];
	BYTE			arSlot[TOTAL_ACCOUNT_BANK_ITEM_NUM];
	short			arCount[TOTAL_ACCOUNT_BANK_ITEM_NUM];

	for( i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++ ) arSlot[i] = -1;
	for( i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++ ) arCount[i] = -1;

	CBufferEx TempBuf;

	CStore* pStore = NULL;
	short sStoreID = GetShort(pBuf, index);
	if(sStoreID < 0) return;
	pStore = GetStore(sStoreID);
	if(pStore == NULL) return;
	short sRate = pStore->m_sRate;

	DWORD OutCost = 0;
	DWORD dwTax = 0;

	DWORD OutputDN = GetDWORD(pBuf, index);

	if(OutputDN > m_dwAccountBankDN)			// 呈公 农搁
	{
		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	if( OutputDN > 0 ) OutCost += 150;

	DWORD BackMyDN = m_dwDN;					// 归诀
	DWORD BackBankDN = m_dwAccountBankDN;

	short slotcount = GetShort( pBuf, index );

	if( slotcount < 0 || slotcount >= TOTAL_ACCOUNT_BANK_ITEM_NUM ) { result = FAIL; goto go_result; }

	for( i = 0; i < slotcount; i++ )
	{
		arSlot[i] = GetShort( pBuf, index );
		arCount[i] = GetShort( pBuf, index );

		if(arSlot[i] >= TOTAL_ACCOUNT_BANK_ITEM_NUM) { result = FAIL; goto go_result; }

		sSid = m_AccountBankItem[arSlot[i]].sSid;
		sHaveCount = m_AccountBankItem[arSlot[i]].sCount;

		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { result = FAIL; goto go_result; }

		if(arCount[i] <= 0 || arCount[i] > sHaveCount) { result = FAIL; goto go_result; }

		if(g_arItemTable[sSid]->m_sDuration > 0 && arCount[i] > 1) { result = FAIL; goto go_result; }

		iWeight += g_arItemTable[sSid]->m_byWeight * arCount[i];

		if(m_iMaxWeight < m_iCurWeight + iWeight)
		{
			SendSystemMsg( IDS_USER_OVER_WEIGHT1, SYSTEM_ERROR, TO_ME);
			result = FAIL; 
			goto go_result;
		}

		OutCost += 150;
	}

	if( OutCost > m_dwDN )
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY_FOR_OUT, SYSTEM_ERROR, TO_ME);
		result = FAIL; 
		goto go_result;
	}
	else m_dwDN -= OutCost;

	for( i = 0; i < TOTAL_ITEM_NUM; i++ )	// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

	for( i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++ )
	{
		BankItem[i] = m_AccountBankItem[i];
	}

	for( i = 0; i < slotcount; i++ )
	{
		tDestSlot = GetSameItem( m_AccountBankItem[arSlot[i]], INVENTORY_SLOT );

		if( tDestSlot >= 0 )
		{
			m_UserItem[tDestSlot].sCount += arCount[i];
		}
		else
		{
			tDestSlot = GetEmptySlot( INVENTORY_SLOT );

			if( tDestSlot == -1 )
			{
				for( j = 0; j < TOTAL_ITEM_NUM; j++)// 酒捞袍 沥焊 汗盔
				{
					m_UserItem[j] = MyItem[j];
				}
				for( j = 0; j < TOTAL_ACCOUNT_BANK_ITEM_NUM; j++ )
				{
					m_AccountBankItem[j] = BankItem[j];
				}
				m_dwDN = BackMyDN;
				m_dwAccountBankDN = BackBankDN;

				result = FAIL; goto go_result;
			}

			m_UserItem[tDestSlot].sLevel = m_AccountBankItem[arSlot[i]].sLevel;
			m_UserItem[tDestSlot].sSid = m_AccountBankItem[arSlot[i]].sSid;
			m_UserItem[tDestSlot].sDuration = m_AccountBankItem[arSlot[i]].sDuration;
			m_UserItem[tDestSlot].sBullNum = m_AccountBankItem[arSlot[i]].sBullNum;
			m_UserItem[tDestSlot].sCount = arCount[i];
			for(j = 0; j < MAGIC_NUM; j++) m_UserItem[tDestSlot].tMagic[j] = m_AccountBankItem[arSlot[i]].tMagic[j];
			m_UserItem[tDestSlot].tIQ = m_AccountBankItem[arSlot[i]].tIQ;
			m_UserItem[tDestSlot].iItemSerial = m_AccountBankItem[arSlot[i]].iItemSerial;

			MakeItemLog( &m_UserItem[tDestSlot], ITEMLOG_ACCOUNT_BANKOUT );
		}

		arChangeMy.Add( tDestSlot );

		if( arCount[i] >= m_AccountBankItem[arSlot[i]].sCount ) ReSetItemSlot( &m_AccountBankItem[arSlot[i]] );
		else m_AccountBankItem[arSlot[i]].sCount -= arCount[i];

		arChangeBank.Add( arSlot[i] );
	}

	if(!CheckMaxValueReturn(m_dwDN, OutputDN))
	{									// 窜, MAX蔼捞搁 瞒咀篮...
		CheckMaxValue(m_dwDN, OutputDN);
		if(m_dwDN < OutputDN) OutputDN = 0;
		else OutputDN = m_dwDN - OutputDN;
	}
	else m_dwDN += OutputDN;
										// 篮青俊辑 猾促.
	if(OutputDN >= m_dwAccountBankDN) m_dwAccountBankDN = 0;
	else m_dwAccountBankDN -= OutputDN;

	if( OutputDN > 0 )
		MakeMoneyLog( OutputDN, ITEMLOG_ACCOUNT_BANKOUT_MONEY, NULL, m_dwAccountBankDN );

	// 器飘府胶俊 技陛阑 穿利矫难 霖促.
	dwTax = (DWORD)( OutCost * ((double)sRate/100) );
	UpdateFortressTax(sStoreID, dwTax); // 器飘府胶 技陛捞搁 历厘...


	/**************************DB Update 贸府*********************************************/
	if(UpdateUserBank() == FALSE)
	{
		for( i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 汗盔
		{
			m_UserItem[i] = MyItem[i];
		}
		for( i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++ )
		{
			m_AccountBankItem[i] = BankItem[i];
		}
		m_dwDN = BackMyDN;
		m_dwAccountBankDN = BackBankDN;

		result = FAIL;

		goto go_result;
	}

	FlushItemLog( TRUE );

go_result:
	TempBuf.Add(ACCOUNT_BANK_ITEM_MOVE_RESULT);

	if(result == FAIL)
	{
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		FlushItemLog( FALSE );
		return;
	}

	result = (BYTE)0x02;
	TempBuf.Add(result);

	TempBuf.Add( m_dwDN );
/*	TempBuf.Add( m_dwAccountBankDN );

	TempBuf.Add( (short)(arChangeBank.GetSize()) );

	for( i = 0; i < arChangeBank.GetSize(); i++ )
	{
		bankslot = arChangeBank[i];

		TempBuf.Add( (BYTE)bankslot );
		TempBuf.Add(m_AccountBankItem[bankslot].sLevel);
		TempBuf.Add(m_xxxxxxxxeiAccountBankItem[bankslot].sSid);
		TempBuf.Add(m_AccountBankItem[bankslot].sDuration);
		TempBuf.Add(m_AccountBankItem[bankslot].sBullNum);
		TempBuf.Add(m_AccountBankItem[bankslot].sCount);
		for( j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_AccountBankItem[bankslot].tMagic[j]);
		TempBuf.Add(m_AccountBankItem[bankslot].tIQ);
	}
*/
	TempBuf.Add( (short)(arChangeMy.GetSize()) );

	for( i = 0; i < arChangeMy.GetSize(); i++ )
	{
		myslot = arChangeMy[i];

		TempBuf.Add( (BYTE)myslot );
		TempBuf.Add( m_UserItem[myslot].sLevel );
		TempBuf.Add( m_UserItem[myslot].sSid);
		TempBuf.Add( m_UserItem[myslot].sDuration);
		TempBuf.Add( m_UserItem[myslot].sBullNum);
		TempBuf.Add( m_UserItem[myslot].sCount);
		for( j = 0; j < MAGIC_NUM; j++ ) TempBuf.Add( m_UserItem[myslot].tMagic[j] );
		TempBuf.Add( m_UserItem[myslot].tIQ );
	}

	Send(TempBuf, TempBuf.GetLength());

	m_iCurWeight += iWeight;
	GetRecoverySpeed();							// 雀汗加档 眉农...
}

void USER::AccountBankInPutDN(TCHAR *pBuf)
{
	CBufferEx TempBuf;

	BYTE result; 
	int index = 0;
	DWORD BackBankDN = 0, BackMyDN = 0;

	DWORD InputDN = GetDWORD(pBuf, index);

	TempBuf.Add(ACCOUNT_BANK_ITEM_MOVE_RESULT);

	if(InputDN == 0 || InputDN > m_dwDN) 
	{
		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	
	BackMyDN = m_dwDN;
	BackBankDN = m_dwAccountBankDN;
										// 篮青俊 涝陛
	if(!CheckMaxValueReturn(m_dwAccountBankDN, InputDN))
	{									// 窜, MAX蔼捞搁 瞒咀篮...
		CheckMaxValue(m_dwAccountBankDN, InputDN);
		if(m_dwAccountBankDN < InputDN) InputDN = 0;
		else InputDN = m_dwAccountBankDN - InputDN;
	}
	else m_dwAccountBankDN += InputDN;
										// 啊瘤绊 乐绰 家瘤陛俊辑 猾促.
	if(m_dwDN <= InputDN) m_dwDN = 0;
	else m_dwDN -= InputDN;

	MakeMoneyLog( InputDN, ITEMLOG_ACCOUNT_BANKIN_MONEY, NULL, m_dwAccountBankDN );

	if(UpdateUserBank() == FALSE)		// DB UpDate
	{
		m_dwDN = BackMyDN;
		m_dwBankDN = BackBankDN;

		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());

		FlushItemLog( FALSE );
		return;
	}

	result = (BYTE)0x03;				// 3 : DN 涝陛
	TempBuf.Add(result);

	TempBuf.Add(m_dwAccountBankDN);			
	TempBuf.Add(m_dwDN);				

	Send(TempBuf, TempBuf.GetLength());

	FlushItemLog( TRUE );
}

void USER::AccountBankOpenReq(int nStoreID)
{
	CStore* pStore = NULL;
	pStore = GetStore(nStoreID);
	if(pStore == NULL) return;
	DWORD dwCost = 150;

	int i, j;
	CBufferEx TempBuf;

	CByteArray arItemSlotList;

//	m_dwAccountBankDN = 0;

//	if(!LoadMemAccountBank())
//	{
//		if(!LoadAccountBank()) return;						// 蜡历啊 焊包茄 篮青 酒捞袍阑 贸澜 立加且锭 啊瘤绊 柯促.
//	}

	for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++)
	{												// 泅犁 焊包等 酒捞袍父 焊咯林扁困秦 沥纺茄促. 
		if(m_AccountBankItem[i].sSid >= 0)
		{
			arItemSlotList.Add(i);
		}
	}

	TempBuf.Add(ACCOUNT_BANK_OPEN);

	TempBuf.Add((short)nStoreID);

	TempBuf.Add((DWORD)m_dwAccountBankDN);
	TempBuf.Add((BYTE)arItemSlotList.GetSize());

	for(i = 0; i < arItemSlotList.GetSize(); i++)
	{
		BYTE tempSlot = 0;
		tempSlot = arItemSlotList[i];
		TempBuf.Add(tempSlot);
		TempBuf.Add((short)m_AccountBankItem[tempSlot].sLevel);
		TempBuf.Add((short)m_AccountBankItem[tempSlot].sSid);
		TempBuf.Add((short)m_AccountBankItem[tempSlot].sDuration);
		TempBuf.Add((short)m_AccountBankItem[tempSlot].sBullNum);
		TempBuf.Add((short)m_AccountBankItem[tempSlot].sCount);

		for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_AccountBankItem[tempSlot].tMagic[j]);

		TempBuf.Add((BYTE)m_AccountBankItem[tempSlot].tIQ);
		TempBuf.Add( dwCost );
	}

	Send(TempBuf, TempBuf.GetLength());
	CheckBadItem();

}

////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 矫傈矫 竿傍俊 狼茄 刘啊啦阑 备茄促.
//
int USER::GetPsyAssault(bool bMax)
{
	int iLevel = 0;
	int iSkillSid = 0;

	int iAttackUp = 0;
	int tClass = 0;

	BYTE tWeaponClass = 0;

	IsCanUseWeaponSkill(tWeaponClass);
	tClass = tWeaponClass * SKILL_NUM;

	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid  = m_UserSkill[i].sSid;
		if(iSkillSid == SKILL_ASSAULT) // 竿傍			// 20 index
		{
			// 鉴荐 胶懦 饭骇 
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;
			
			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			int iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
			if((iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel))|| bMax)
			{
				iAttackUp = g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel);
			}
		}
	}

	return iAttackUp;	
}

////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 矫傈矫 例措规绢 胶懦俊 狼茄 刘啊啦阑 备茄促.
//
int USER::GetPsyAbsoluteDefense()
{
	int iLevel = 0;
	int iSkillSid = 0;

	int iAttackUp = 0;
	int tClass = 0;

	BYTE tWeaponClass = 0;

	IsCanUseWeaponSkill(tWeaponClass);
	tClass = tWeaponClass * SKILL_NUM;

	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid  = m_UserSkill[i].sSid;
		if(iSkillSid == SKILL_ABSOLUTE_DEFENSE) //绝对防御			21 index
		{
			// 鉴荐 胶懦 饭骇 
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;
			
			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			iAttackUp = g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel);
		}
		if(iSkillSid == SKILL_PSYCHIC_RESIST) //魔法抗拆			22 index
		{
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;
			
			
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			iAttackUp += g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel);
		}
	}

	return iAttackUp;	
}

////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 矫傈矫 荤捞坷葱 历亲 胶懦俊 狼茄 刘啊啦阑 备茄促.
//
int USER::GetPsyPsyResist()
{
	int iLevel = 0;
	int iSkillSid = 0;

	int iAttackUp = 0;
	int tClass = 0;

	BYTE tWeaponClass = 0;

	IsCanUseWeaponSkill(tWeaponClass);
	tClass = tWeaponClass * SKILL_NUM;

	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid  = m_UserSkill[i].sSid;
		if(iSkillSid == SKILL_PSYCHIC_RESIST) // 荤捞坷葱 历亲			// 22 index
		{
			// 鉴荐 胶懦 饭骇 
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;
			
			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			iAttackUp = g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel);
		}
	}

	return iAttackUp;	
}

void USER::ResetOver100LevelSkill(int sLevel)//百级技能控制
{
	int emptyskillslot = -1;
	int skillsid = 0;

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

	if( emptyskillslot == -1 ) return;

	skillsid = m_UserSkill[emptyskillslot].sSid;

	if( sLevel <= 99 )		// 父距 99乏栏肺 悸泼窍扼绊 甸绢坷搁
	{
		m_UserSkill[emptyskillslot].sSid = emptyskillslot;		// 扁粮 100乏 胶懦 沥焊甫 瘤款促
		m_UserSkill[emptyskillslot].tLevel = 0;

		return;
	}
	else
	{
		if( skillsid < 0 || skillsid >= g_arSkillTable.GetSize() ) return;

		if( g_arSkillTable[skillsid]->m_sSid == -1 ) return;

		if (sLevel > 99 || sLevel < 120) //控制100-120技能点
		{
			m_UserSkill[emptyskillslot].tLevel = (BYTE)((sLevel % 100) + 1);
		}
		if (sLevel >= 120)//控制120以后固定=20
		{
			m_UserSkill[emptyskillslot].tLevel = (BYTE)(20);					
		}
	}
	
	if(sLevel >=120 && sLevel<130)
	{//120技能转换后升级..加的的...
		int iIndex=m_byClass *SKILL_NUM;
		//仲裁职业
		if(m_byClass == JUDGE)
		{
			iIndex = 25;
		}else
		{
			iIndex =m_byClass *SKILL_NUM;
		}
		int i;
		for(i = iIndex; i < iIndex + SKILL_NUM-1; i++)
		{
			if(m_UserSkill[i].sSid>=20&&m_UserSkill[i].sSid<=23)
			{
				m_UserSkill[i].tLevel++;
				if(m_UserSkill[i].tLevel>=20)
					m_UserSkill[i].tLevel=20;
			}
		}
	}
	if(sLevel >= 130 && o_yehuoini[0]->chaoneng == 1 ){   
		CheckMaxValue((short &)m_sSkillPoint_, (short)1);
	}
}

BOOL USER::CheckMoneyMinMax(int min, int max)
{
	if( min < -1 || max < -1 ) return FALSE;
	if( min == -1 && max == -1 ) return FALSE;

	DWORD dwMin = 0;
	DWORD dwMax = 0;

	if( min == -1 && max > 0 )		// max固父狼 捣父 乐栏搁 TRUE
	{
		dwMax = max;

		if( m_dwDN < dwMax )
		{
			return TRUE;
		}
	}
	else if( min > 0 && max == -1 )	// min捞惑狼 捣父 乐栏搁 TRUE
	{
		dwMin = min;

		if( m_dwDN >= dwMin )
		{
			return TRUE;
		}
	}
	else if( min > 0 && max > 0 )
	{
		dwMin = min;
		dwMax = max;

		if( m_dwDN >= dwMin && m_dwDN < dwMax )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL USER::CheckRandom(int rand)
{
	if( rand <= 0 ) return FALSE;

	if( rand >= 100 ) return TRUE;

	int temp_rand = rand * 100;

	int rand_result = myrand( 0, 10000 );

	if( rand_result <= temp_rand ) return TRUE;

	return FALSE;
}
void USER::GiveDN(int dn)
{
	if( dn <= 0 ) return;

	DWORD dwBackup = m_dwDN;
	DWORD dwGiveDN = dn;

	CheckMaxValue( (DWORD &)m_dwDN, dwGiveDN );

	if( !UpdateUserItemDN() )
	{
		m_dwDN = dwBackup;
		return;
	}

	SendMoneyChanged();
}
void USER::GiveShopPingDN(int shoppingDN)
{
	if( shoppingDN <= 0 ) return;

	DWORD dwBackup = m_dwShopPingDN;
	DWORD dwGiveShopPingDN = shoppingDN;

	CheckMaxValue( (DWORD &)m_dwShopPingDN, dwGiveShopPingDN );

	if( !UpdateUserItemShopPingDN() )
	{
		m_dwShopPingDN = dwBackup;
		return;
	}
    SendUserStatusSkill();	//更新元宝数量
}
//////////////////////////////////////////////////////////////////////////////////////////
void USER::GiveHiExpTime(int HiExpTime)
{
	if( HiExpTime <= 0 ) return;

	DWORD dwBackup = m_dwHiExpTime;
	DWORD dwGiveHiExpTime = HiExpTime;

	CheckMaxValue( (DWORD &)m_dwHiExpTime, dwGiveHiExpTime );

	if( !UpdateUserItemHiExpTime() )
	{
		m_dwHiExpTime = dwBackup;
		return;
	}

}
//给幸运时间
void USER::GiveMagicFindTime(int MagicFindTime)
{
	if( MagicFindTime <= 0 ) return;

	DWORD dwBackup = m_dwMagicFindTime;
	DWORD dwGiveMagicFindTime = MagicFindTime;

	CheckMaxValue( (DWORD &)m_dwMagicFindTime, dwGiveMagicFindTime );

	if( !UpdateUserItemMagicFindTime() )
	{
		m_dwMagicFindTime = dwBackup;
		return;
	}

}

void USER::GiveVIPTime( int VIPTime )
{
	if( VIPTime <= 0 ) return;


	DWORD dwBackup = m_dwVIPTime;
	DWORD dwGiveHiExpTime = VIPTime;

	CheckMaxValue( (DWORD &)dwBackup, dwGiveHiExpTime );

	if(m_dwVIPTime == 0)
	{
		AddAbnormalInfo(ABNORMAL_FUDAI);
		int index = 0;
		SetByte(m_TempBuf, SET_USER_STATE, index);
		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
		SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
		Send(m_TempBuf, index);			
	}
	m_dwVIPTime = dwBackup;

}
//////////////////////////////////////////////////////////////////////////////////////////

void USER::RobDN(int dn)
{
	if( dn <= 0 ) return;

	DWORD dwBackup = m_dwDN;
	DWORD dwRobDN = dn;
	DWORD dwTemp = m_dwDN;

	if( m_dwDN <= dwRobDN )
	{
		m_dwDN = 0;
	}
	else
	{
		m_dwDN = dwTemp - dwRobDN;
	}

	if( !UpdateUserItemDN() )
	{
		m_dwDN = dwBackup;
		return;
	}

	SendMoneyChanged();
}

void USER::RobLING(int dn)
{
	if( dn <= 0 ) return;

	DWORD dwBackup = m_dwLingQu;
	DWORD dwRobLing = dn;
	DWORD dwTemp = m_dwLingQu;

	if( m_dwLingQu <= dwRobLing )
	{
		m_dwLingQu = 0;
	}
	else
	{
		m_dwLingQu = dwTemp - dwRobLing;
	}

}
void USER::Collect(TCHAR *pBuf)
{
	USER*	pUser	= NULL;
	CNpc*	pNpc	= NULL;
	int		nTPosX	= 0;
	int		nTPosY	= 0;
	int		nAttackRange = 0;
	int		nDist	= 100;

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

	int		iFireErrDec = 0;
	int	determine = 0;
	int iDexHitRate = 0, iLevelHitRate = 0;
	int iMyDex = 0, iYourDex = 0;

	CByteArray	arSkillAction1, arSkillAction2;							// Skill Action Array

	DWORD dwExp = 0;
	int index = 0;
	int nTargetID = GetShort(pBuf, index);								// Target ID 甫 掘绰促.

	if(nTargetID < USER_BAND || nTargetID >= INVALID_BAND) return;		// 肋给等 Target 捞搁 return

	int sSid = m_UserItem[RIGHT_HAND].sSid;

	if( sSid < 0 || sSid >= g_arItemTable.GetSize() ) return;

	// 傍拜 Delay 眉农 ---------------------------------------------------------//
	DWORD dwCurrTick = GetTickCount();
	DWORD dwMinDelay = (DWORD)g_arItemTable[sSid]->m_sAttackDelay;
	if(dwMinDelay < DEFAULT_AT_DELAY) dwMinDelay = DEFAULT_AT_DELAY;

	if(m_dwLastAttackTime >= dwCurrTick) return;

	if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
	{
		m_dwLastAttackTime = dwCurrTick;
	}

	// 郴备档 八荤 -------------------------------------------------------------//
	if(m_UserItem[RIGHT_HAND].sDuration <= 0)
	{
//		SendAttackFail(ERR_BROKEN);
		SendSystemMsg( IDS_USER_DAMAGED_ITEM, SYSTEM_NORMAL, TO_ME);
		return;
	}

	// Target 苞狼 芭府拌魂 ----------------------------------------------------//
	if(nTargetID >= USER_BAND && nTargetID < NPC_BAND)	// USER : 盲笼 档备甫 瞒绊 蜡历甫 傍拜且 荐 绝促
	{
		SendAttackMiss(nTargetID);
		return;
	}
	else if(nTargetID >= NPC_BAND)				// NPC
	{
		pNpc = GetNpc(nTargetID - NPC_BAND);	// NPC Point 甫 掘绰促.
		if(pNpc == NULL) return;				// 肋给等 NPC 捞搁 府畔
		if(pNpc->m_NpcState == NPC_DEAD || pNpc->m_sHP <= 0)
		{
			//pNpc->m_Delay = pNpc->SendDead(m_pCom, 0);
			return;// NPC 啊 捞固 磷绢 乐栏搁 府畔
		}
		if(pNpc->m_tNpcType != 0) return;

		nTPosX = pNpc->m_sCurX;
		nTPosY = pNpc->m_sCurY;
	}

	nAttackRange = GetAttackRange(m_UserItem[RIGHT_HAND].sSid);
	nAttackRange += 1;	// 拱府利牢 鸥拜阑啊且锭父 +1甫 秦霖促.(捞悼 加档锭巩俊 各阑 棱瘤 给秦辑 焊沥栏肺...)

	if(GetDistance(nTPosX, nTPosY, nAttackRange, &nDist) == FALSE)		// 傍拜啊瓷芭府 观捞搁 府畔
	{
		if(g_bDebug) SendSystemMsg(_T("### Too Long Distance!!!"), SYSTEM_NORMAL, TO_ME);
		return;
	}

	// 疙吝 蔼 -------------------------------------------------------//
	int nBasicSuccess = 70;
	int nTotalSuccess = 0;

	if( m_UserItem[RIGHT_HAND].sSid == 699 )
	{
		nBasicSuccess = 80;
	}
	else if( m_UserItem[RIGHT_HAND].sSid == 670 )
	{
		nBasicSuccess = 100;
	}

	nTotalSuccess = (int)( (double)m_sMagicDEX * 0.1 + 0.5 );
	nTotalSuccess += nBasicSuccess;

	if( nTotalSuccess < 100 )
	{
		if( nTotalSuccess < myrand( 1, 100 ) )
		{
			SendAttackMiss(nTargetID);
			return;
		}
	}

	// 雀乔蔼/疙吝魄沥/单固瘤 拌魂/郴备档 拌魂/弥辆 傍拜矫埃 悸泼----------//
	if(nTargetID >= USER_BAND && nTargetID < NPC_BAND)	// USER : 蜡历绰 盲笼措惑捞 酒聪促
	{
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

		pUser = GetUser(m_uid);

		if(pNpc->m_tNpcType == NPCTYPE_GUILD_GUARD)	pNpc->ChangeTarget(pUser, m_pCom);

		// NPC 规绢蔼 
		nDefense = pNpc->GetDefense();

		nFinalDamage = 1;		// 盲笼 档备肺 磨锭绰 damage啊 1捞促

		// 傍拜螟 郴备档 皑家
		// 郴备档
		sNewDuration = (int)((double)nDefense/10 + 0.5);
		sOldDuration = m_UserItem[RIGHT_HAND].sDuration;

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

		// 傍拜捞 己傍 沁栏骨肺 捞亥飘甫 茫酒辑 角青 ---------------------------------//
		int i, j;
		int event_zone_index = -1;
		int real_event = -1;
		EVENT*	pEvent = NULL;

		if( pNpc->m_sEvent >= MONSTER_ATTACK_EVENT_BAND && m_iDisplayType != 6 )	// 阁胶磐啊 啊瘤备 乐绰 捞亥飘啊 傍拜 捞亥飘牢瘤 犬牢
		{
			real_event = pNpc->m_sEvent % MONSTER_ATTACK_EVENT_BAND;

			for(i = 0; i < g_event.GetSize(); i++)
			{
				if(g_event[i]->m_Zone == m_curz)
				{				
					event_zone_index = i;
					break;
				}
			}

			if(event_zone_index != -1)// return;			// 泅犁 Zone 俊绰 define 等 捞亥飘啊 绝促.
			{
				pEvent = g_event[event_zone_index];

				if(pEvent)
				{
					if(real_event < pEvent->m_arEvent.GetSize() && real_event > 0)
					{
						if(pEvent->m_arEvent[real_event])
						{
							if(CheckEventLogic(pEvent->m_arEvent[real_event]))// return;	// 炼扒八荤
							{
								for(j = 0; j < pEvent->m_arEvent[real_event]->m_arExec.GetSize(); j++)
								{
									if(RunNpcEvent(pNpc, pEvent->m_arEvent[real_event]->m_arExec[j]) == NULL) break;
								}
							}
						}
					}
				}
			}
		}

		// Calculate Target HP -------------------------------------------------------//
		short sOldNpcHP = pNpc->m_sHP;

		if(pNpc->SetDamage(nFinalDamage, m_uid + USER_BAND, m_pCom) == FALSE)
		{
			pNpc->SendExpToUserList(m_pCom); // 版氰摹 盒硅!!
			int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel); //怪物等级相差30级没经验
			if(diffLevel <= o_yehuoini[0]->djxz || pNpc->m_sEvent >= 30000 )
			{
				if (m_sLevel>= 70)
							m_dwShopPingDN += 1;//每杀死怪物增加杀怪物数
				if (m_isDoubleBAOLV  > 0 || g_sanBaoLv == TRUE || m_dwGuarDianTianShi > 0)
					
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
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(diffLevel < 40)
				{
					CheckMaxValue(m_dwXP, 1);		// 各捞 磷阑锭父 1 刘啊!	
					SendXP();
				}
			}
		}


		// 傍拜 搬苞 傈价
		SendAttackSuccess(nTargetID, bIsCritical, pNpc->m_sHP, pNpc->m_sMaxHP);

		m_dwLastAttackTime = GetTickCount();
	}
}

void USER::SendEventMsg(char *pMsg)
{
	if( strlen( pMsg ) >= 128 ) return;

//	SendSystemMsg( pMsg, SYSTEM_ERROR, TO_ME);
//////////////改成黄字提醒//////////////
	CString str;
	str.Format(pMsg);
    CBufferEx	TempBuf,TempBuf1;
	TempBuf.Add((byte)CHAT_RESULT);
	TempBuf.Add((byte)1);
	TempBuf.Add(WHISPER_CHAT);
	TempBuf.Add((int)0x00);
	TempBuf.Add("", _tcslen(""));
	TempBuf.Add(str.GetBuffer(0), str.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}

void USER::WriteItemLog(ItemList *pItem, int type, char *strEtc)
{
	if( !pItem ) return;
	if( !CheckItemLog( pItem ) ) return;

	ITEMLOG* pItemLog = new ITEMLOG;
	pItemLog->t = CTime::GetCurrentTime();
	pItemLog->m_sLogType = type;
	memset( pItemLog->m_strUserId, NULL, CHAR_NAME_LENGTH+1 );
	memcpy( pItemLog->m_strUserId, m_strUserID, CHAR_NAME_LENGTH );

	memset( pItemLog->m_strEtc, NULL, CHAR_NAME_LENGTH+1 );
	if( strEtc != NULL )	memcpy( pItemLog->m_strEtc, strEtc, CHAR_NAME_LENGTH );
	else					memcpy( pItemLog->m_strEtc, m_strUserID, CHAR_NAME_LENGTH );

	pItemLog->m_sLevel		= pItem->sLevel;
	pItemLog->m_sSid		= pItem->sSid;
	pItemLog->m_sDuration	= pItem->sDuration;
	pItemLog->m_sBullNum	= pItem->sBullNum;
	pItemLog->m_sCount		= pItem->sCount;

	for( int i = 0; i < MAGIC_NUM; i++ )
	{
		pItemLog->m_tMagic[i] = pItem->tMagic[i];
	}

	pItemLog->m_tIQ			= pItem->tIQ;

	pItemLog->m_iItemSerial	= pItem->iItemSerial;

	if( !UpdateMemItemLog( pItemLog ) )
	{
	}

	delete pItemLog;
}

BOOL USER::CheckItemLog(ItemList *pItem)
{
	if( !pItem ) return FALSE;

	if( pItem->iItemSerial == 0 ) pItem->iItemSerial = GenerateItemSerial( pItem );
	if( pItem->iItemSerial == 0 ) return FALSE;

	return TRUE;
}

BOOL USER::UpdateMemItemLog(ITEMLOG *pItemLog)
{
	if( !m_pSharedMemory )				return FALSE;
	if( !m_pSharedMemory->m_hMapping )	return FALSE;
	if( !m_pMD )						return FALSE;
	if( m_pMD->m_uid == -1 )			return FALSE;

	if(m_pMD->m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_strUserID) != 0) return FALSE;

	for( int i = 0; i < 100; i++ )
	{
		if( m_pMD->m_arItemLogData[i][0] != NULL ) continue;
		memcpy( m_pMD->m_arItemLogData[i], pItemLog, sizeof(ITEMLOG) );
		break;
	}

	return TRUE;
}

void USER::MakeItemLog(ItemList *pItem, int type, char *strEtc)
{
    if( !pItem ) return;
	//if( !CheckItemLog( pItem ) ) return;
	int bianhao = pItem->sSid;
	int pingfen=0;

	ITEMLOG* pItemLog = new ITEMLOG;
	pItemLog->t = CTime::GetCurrentTime();
	pItemLog->m_sLogType = type;
	memset( pItemLog->m_strUserId, NULL, CHAR_NAME_LENGTH+1 );
	memcpy( pItemLog->m_strUserId, m_strUserID, CHAR_NAME_LENGTH );

	memset( pItemLog->m_strEtc, NULL, CHAR_NAME_LENGTH+1 );
	if( strEtc != NULL )	memcpy( pItemLog->m_strEtc, strEtc, CHAR_NAME_LENGTH );
	else					memcpy( pItemLog->m_strEtc, m_strUserID, CHAR_NAME_LENGTH );

	pItemLog->m_sLevel		= pItem->sLevel;
	pItemLog->m_sSid		= pItem->sSid;
	pItemLog->m_sDuration	= pItem->sDuration;
	pItemLog->m_sBullNum	= pItem->sBullNum;
	pItemLog->m_sCount		= pItem->sCount;

	for( int i = 0; i < MAGIC_NUM; i++ )
	{
		pItemLog->m_tMagic[i] = pItem->tMagic[i];
	//////////////新评分 2013-05-17//////////////////////////
		int shuxing = pItem->tMagic[i];
		int tClass = g_arItemTable[bianhao]->m_byWear;
		int zhiye = g_arItemTable[bianhao]->m_byClass;
		
		if ( tClass == 1 )//检测类型为武器给评分
		{
			//属性  128损伤25 
			if (shuxing == 128){pingfen += 50;}
			//属性 145所有技能2
			if (shuxing == 145){pingfen += 80;}
			//属性 136魔法损伤25
			if (shuxing == 136){pingfen += 50;}
	//		//属性 141生命50
	//		if (shuxing == 141){pingfen += 50;}
			//属性  93攻速3 
			if (shuxing == 93){pingfen += 40;}
			//属性   138命中10
			if (shuxing == 138){pingfen += 40;}
			//属性 137所有技能1
			if ( shuxing == 137){pingfen += 40;}
	//		//属性  132生命40
	//		if (shuxing == 132 ){pingfen += 40;}
			//属性 91损伤20 
			if (shuxing == 91 ){pingfen += 30;}
			//属性 117法术2 
			if (shuxing == 117){pingfen += 30;}
			//属性 120穿刺2 
			if (shuxing == 120){pingfen += 30;}
			//属性 113兴奋2 
			if (shuxing == 113){pingfen += 30;}
			//属性  125连射2
			if (shuxing == 125){pingfen += 30;}
			//属性  106魔伤20 
			if (shuxing == 106){pingfen += 30;}
	//		//属性 102生命30
	//		if (shuxing == 102 ){pingfen += 30;}
			//属性   95命中9
			if (shuxing == 95 ){pingfen += 25;}
			//属性   64损伤15 
			if (shuxing == 64 ){pingfen += 20;}
			//属性  85魔伤15 
			if (shuxing == 85 ){pingfen += 20;}
			//属性  94命中8  
			if (shuxing == 94 ){pingfen += 20;}
			//属性   74命中7 
			if (shuxing == 74 ){pingfen += 15;}
//			//属性  81生命20 
//			if (shuxing == 81 ){pingfen += 15;}
			//属性   110智慧5  
			if (shuxing == 110){pingfen += 5;}
			//属性  109敏捷5 
			if (shuxing == 109){pingfen += 5;}
			//属性  107力量5 
			if (shuxing == 107 ){pingfen += 5;}
			//属性  86力量4 
			if (shuxing == 86 ){pingfen += 5;}
			//属性  88敏捷4 
			if (shuxing == 88){pingfen += 5;}
			//属性  89智慧4
			if (shuxing == 89 ){pingfen += 5;}
		}
		else if ( tClass != 1 )//检测类型装备给评分
		{
			if (zhiye==8 || zhiye==2 ) //拳和刀
			{ 
				if ( shuxing == 107) {  pingfen += 50; }                   
			    if ( shuxing == 141) {  pingfen += 50; }
			    if ( shuxing == 135) {  pingfen += 50; } 
			    if ( shuxing == 139) {  pingfen += 50; }
			}
            
			if (zhiye==1 )  //枪
			{
			   if (shuxing == 109)   {  pingfen += 50; }                
			   if (shuxing == 141)   {  pingfen += 50; }
			   if (shuxing == 135)   {  pingfen += 50; }
			   if (shuxing == 139)   {  pingfen += 50; }
		    }
					
		   if (zhiye==4 )  //法师
		   {
                if (shuxing == 110)  {  pingfen += 50; }                
				if (shuxing == 141)  {  pingfen += 50; }   
				if (shuxing == 135)  {  pingfen += 50; }   
				if (shuxing == 139)  {  pingfen += 50; } 
	       }


		   if (zhiye==31 )  //所有职业 //属性    141HP50  135抗魔25  139回避10   107力量5  110智慧5   109敏捷5   130防御10  
		   {            
		        if (shuxing == 141)  {  pingfen += 50; }         
				if (shuxing == 135)  {  pingfen += 50; } 
				if (shuxing == 139)  {  pingfen += 50; } 
				if (shuxing == 107)  {  pingfen += 50; } 
				if (shuxing == 110)  {  pingfen += 50; } 
				if (shuxing == 109)  {  pingfen += 50; } 
		   }
 //============================================================== 45分

		   if (zhiye==8 || zhiye==2 ) //拳和刀
		   {
			   if ( shuxing == 97)       {  pingfen += 45;  }        
                 
		   }
		   if (zhiye==1 )  //枪
		   {
			   if ( shuxing == 97)   {  pingfen += 45;  }              
	           
		   }
		   if (zhiye==4 ) //法师
		   { 
			   if ( shuxing == 97)   {  pingfen += 45;  }          
	        
		   }
           if (zhiye==31 )    //所有职业       //属性  97回避9    129防御9增加 
		   {
			   if ( shuxing == 97)   {  pingfen += 45;  }          
	           
		   }
  //============================================================== 40分

			if (zhiye==8 || zhiye==2 )  //拳和刀
			{
				if ( shuxing == 86)   {  pingfen += 40; }              
				if ( shuxing == 132)  {  pingfen += 40; }  
				if ( shuxing == 105)  {  pingfen += 40; }
				if ( shuxing == 96)   {  pingfen += 40; }
			}
			if  (zhiye==1 )  //枪
			{
				if (shuxing == 88)    {  pingfen += 40; }        
				if (shuxing == 132)   {  pingfen += 40; }  
				if (shuxing == 105)   {  pingfen += 40; }  
				if (shuxing ==  96)   {  pingfen += 40; }  
			}
            if  (zhiye==4 )  //法师
			{
				if (shuxing == 89)    {  pingfen += 40; }               
				if (shuxing == 132)   {  pingfen += 40; }
				if (shuxing == 105)   {  pingfen += 40; }
				if (shuxing == 96 )   {  pingfen += 40; }
			}
            if (zhiye==31 )    //所有职业  //属性 100防御8、132HP40  105抗魔20  96回避8   86力量4  88敏捷4   89智慧4
			{
				if (shuxing == 132)   {  pingfen += 40; }     
				if (shuxing == 105)   {  pingfen += 40; } 
				if (shuxing == 96 )   {  pingfen += 40; }
				if (shuxing == 86)    {  pingfen += 40; }
				if (shuxing == 89)    {  pingfen += 40; }
				if (shuxing == 88)    {  pingfen += 40; }
		    }

 //============================================================== 35分 
            if (zhiye==8 || zhiye==2 )  //拳和刀
			{
				if ( shuxing == 76)   {  pingfen += 35; }           
			}
            if (zhiye==1 )  //枪
			{
			    if ( shuxing == 76)   {  pingfen += 35; }                     
			
			}
			if (zhiye==4 )  //枪
			{
			    if ( shuxing == 76)   {  pingfen += 35; }                     
			
			}
			if (zhiye==31 )  //枪
			{
			    if ( shuxing == 76)   {  pingfen += 35; }                     
				
			}
//============================================================== 30分
                     
			if (zhiye==8 || zhiye==2 )  //拳和刀
			 {
		        if ( shuxing == 75)  {  pingfen += 30; }               
			    if ( shuxing == 102) {  pingfen += 30; }  
				if ( shuxing == 84 ) {  pingfen += 30; }  
			
				if ( shuxing == 43 ) {  pingfen += 30; }  
			 }
			if (zhiye==1 )  //枪
			 {
		        if ( shuxing == 75 ) {  pingfen += 30; }                   
			    if ( shuxing == 102) {  pingfen += 30; } 
				if ( shuxing == 84 ) {  pingfen += 30; }
			
				if ( shuxing == 45 ) {  pingfen += 30; }
		      }
            if (zhiye==4) //法师
			 {
		        if ( shuxing == 75)  {  pingfen += 30; }  
			    if ( shuxing == 102) {  pingfen += 30; } 
				if ( shuxing == 84)  {  pingfen += 30; }
			
				if ( shuxing == 46)  {  pingfen += 30; }
			 }
            if (zhiye==31 )       //所有职业  //属性  75回避6  102HP30   84抗魔15     43力量3 46智慧3  45敏捷3  79防御6
			 {
		        if (shuxing == 102)  {  pingfen += 30; }      
		        if (shuxing == 84 )  {  pingfen += 30; }
				if (shuxing == 75 )  {  pingfen += 30; }
				if (shuxing == 43 )  {  pingfen += 30; }
				if (shuxing == 46 )  {  pingfen += 30; }
				if (shuxing == 45 )  {  pingfen += 30; }
			
			 }
 //============================================================== 25分
						
			if (zhiye==8 || zhiye==2 )  //拳和刀
			 {
		        if ( shuxing == 33 )  {  pingfen += 25; }               
			
			 }

			if (zhiye==1 )  //拳和刀
			 {
		        if ( shuxing == 33 )  {  pingfen += 25; }               
			 
			 }
           if (zhiye==4 )  //拳和刀
			 {
		        if ( shuxing == 33 )  {  pingfen += 25; }               
			 
			 }
		   if (zhiye==31)  //拳和刀
		   {
		        if ( shuxing == 33 )  {  pingfen += 25; }               
			 
		   }
		}					
	}

                      

	pItemLog->m_tIQ			= pItem->tIQ;

	pItemLog->m_iItemSerial	= pItem->iItemSerial;

	pItemLog->m_dwResultDN	= 0;
	pItemLog->m_dwInOutDN	= 0;
	pItemLog->m_dwRelateDN	= 0;

	if(bianhao < 0 || bianhao >= g_arItemTable.GetSize()) return;
	
				
    if(ITEMLOG_PICKUP == type)
	{
		int bianhao2 = pItem->sSid;
	    if(bianhao2 < 0 || bianhao2 >= g_arItemTable.GetSize()) return;
	    int tClass = g_arItemTable[bianhao2]->m_byWear;
		
		if ( tClass == 1 && pingfen >= 150  )//物品为武器 评分大于0 数值自己改
	    {
			CString strMsg;
		    CString GoodsMagic;

		for(int i = 0; i <= 4; i++)
		{
			if(pItem->tMagic[i] != 0)
			{
				GoodsMagic+=g_MagicArray[pItem->tMagic[i]];
				GoodsMagic+=",";
			}
		}
		strMsg.Format("恭喜玩家 %s 拾取 %s 评分 %d 属性:", this->m_strUserID,g_arItemTable[bianhao2]->m_strName ,pingfen);//公告提示
		strMsg+=GoodsMagic;
		m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
	}
	else if ( tClass != 1 && pingfen >= 170  )//物品为装备 评分大于0 数值自己改
	{
		CString strMsg;
		CString GoodsMagic;
		
		for(int i = 0; i <= 4; i++)
		{
			if(pItem->tMagic[i] != 0)
			{
				GoodsMagic+=g_MagicArray[pItem->tMagic[i]];
				GoodsMagic+=",";
			}
		}
		 strMsg.Format("恭喜玩家 %s 拾取 %s 评分 %d 属性:", this->m_strUserID,g_arItemTable[bianhao2]->m_strName ,pingfen );//公告提示
		strMsg+=GoodsMagic;
		m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
	}
	if(pItem->sSid == 907)
	{
		int  tEBodySid = 0;
        tEBodySid = pItem->tMagic[0];
		if(tEBodySid == 16 || tEBodySid ==14 ||  tEBodySid ==24 ||tEBodySid ==27 )
		{
			CString strMsg;
			TCHAR strTxt_My[59][50];
			_tcscpy( strTxt_My[14],_T("[施放2%]"));
			_tcscpy( strTxt_My[16],_T("[10力量转攻击]"));
			_tcscpy( strTxt_My[24],_T("10敏捷转攻击]"));
			_tcscpy( strTxt_My[27],_T("[10智慧转攻击]"));
			_tcscpy( strTxt_My[44],_T("[5力量转攻击]"));
			_tcscpy( strTxt_My[52],_T("[5敏捷转攻击]"));
			_tcscpy( strTxt_My[55],_T("[5智慧转攻击]"));
			strMsg.Format("玩家 %s 打怪获得 %s 锆石!",this->m_strUserID,strTxt_My[tEBodySid]);//公告提示
		    m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		}  
		
        return;
	}
    
	if (pItem->sSid == 991)
				{
					CString strMsg;
					strMsg.Format("玩家『 %s 』 打怪获得『 神秘的罐子（消耗品） 』!", this->m_strUserID);//公告提示
					m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
				}
		if (pItem->sSid == 987)
				{
					CString strMsg;
					strMsg.Format("玩家『 %s 』 打怪获得『 死亡使者 』!", this->m_strUserID);//公告提示
					m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
				}
		if (pItem->sSid == 1310)
				{
					CString strMsg;
					strMsg.Format("玩家『 %s 』 打怪获得『 霸业首饰盒(稀有物品) 』!", this->m_strUserID);//公告提示
					m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
				}
		}

	m_arItemLog.Add( pItemLog );
}
  

void USER::MakeMoneyLog(DWORD dwAddMoney, int type, char *strEtc, DWORD dwChangeMoney)
{
	if( dwAddMoney < 10000 ) return;

	ITEMLOG* pItemLog = new ITEMLOG;
	pItemLog->t = CTime::GetCurrentTime();
	pItemLog->m_sLogType = type;
	memset( pItemLog->m_strUserId, NULL, CHAR_NAME_LENGTH+1 );
	memcpy( pItemLog->m_strUserId, m_strUserID, CHAR_NAME_LENGTH );

	memset( pItemLog->m_strEtc, NULL, CHAR_NAME_LENGTH+1 );
	if( strEtc != NULL )	memcpy( pItemLog->m_strEtc, strEtc, CHAR_NAME_LENGTH );
	else					memcpy( pItemLog->m_strEtc, m_strUserID, CHAR_NAME_LENGTH );

	pItemLog->m_sLevel		= 0;
	pItemLog->m_sSid		= -1;
	pItemLog->m_sDuration	= 0;
	pItemLog->m_sBullNum	= 0;
	pItemLog->m_sCount		= 0;

	for( int i = 0; i < MAGIC_NUM; i++ )
	{
		pItemLog->m_tMagic[i] = 0;
	}

	pItemLog->m_tIQ			= 0;

	pItemLog->m_iItemSerial	= 0;

	pItemLog->m_dwResultDN	= m_dwDN;
	pItemLog->m_dwInOutDN	= dwAddMoney;
	pItemLog->m_dwRelateDN	= dwChangeMoney;

	m_arItemLog.Add( pItemLog );
}

void USER::FlushItemLog(BOOL bSave)
{
	if( !m_arItemLog.GetSize() ) return;

	int i = 0;

	if( bSave )
	{
		for( i = 0; i < m_arItemLog.GetSize(); i++ )
		{
			if( m_arItemLog[i] )
			{
				UpdateMemItemLog( m_arItemLog[i] );
			}
		}
		m_arItemLog.RemoveAll();
	}
	else
	{
		for( i = 0; i < m_arItemLog.GetSize(); i++ )
		{
			if( m_arItemLog[i] )
			{
				delete m_arItemLog[i];
			}
		}
		m_arItemLog.RemoveAll();
	}
}

int USER::GetMinusExpRatioByCityRank(int enemyrank)
{
	if( enemyrank == -1 ) return 10;

	int otherrank = enemyrank;
	int merank = m_sCityRank + CITY_RANK_INTERVAL;

	if( merank < 0 || merank > 12 ) return 10;
	if( otherrank < 0 || otherrank > 12 ) return 10;

	//			 技  啊
	//			 唱  巢
	int exptable[13][13] = 
	{//                唱慧 <-  c  -> 亮篮
		{  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10	},
		{  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10	},
		{  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10	},
		{  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10	},// 唱慧
		{  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10	},//  |
		{	0,  2,	3,	4,	6, 10, 10, 10, 10, 10, 10, 10	},//  c
		{	0,  2,	3,	4,	6, 10, 10, 10, 10, 10, 10, 10	},//  |
		{	0,  2,	3,	4,	6, 10, 10, 10, 10, 10, 10, 10	},// 亮篮
		{	0,  2,	3,	4,	6, 10, 10, 10, 10, 10, 10, 10	},
		{	0,	1,	2,	3,	5, 10, 10, 10, 10, 10, 10, 10	},
		{	0,	1,	2,	3,	5, 10, 10, 10, 10, 10, 10, 10	},
	};

	return exptable[merank][otherrank];
}

int USER::GetDropItemRankByAttackerCityRank(int enemyrank)
{
	if( enemyrank == -1 )
	{
		return m_sCityRank + CITY_RANK_INTERVAL;	// 惑措甫 备且 荐 绝阑 版快浚 盔贰 珐农甫 府畔茄促
	}

	int otherrank = enemyrank;
	int merank = m_sCityRank + CITY_RANK_INTERVAL;

	if( merank < COMMOM_RANK )	// 郴 己氢捞 纳牢 捞窍扼搁 磷牢 逞狼 珐农客 惑包绝捞 盔贰 珐农甫 府畔
	{
		return merank;
	}

	if( otherrank == CHAONISE_RANK )	// 朝 磷牢 逞捞 墨坷聪胶扼搁 唱绰 酒捞袍阑 救冻绢哆赴促
	{
		return SAINT_RANK + 1;			// 技牢飘档 酒袍阑 冻备霸 登绢 乐栏骨肺 技捞飘焊促 臭篮 殿鞭阑 悸泼茄促
	}

	if( otherrank >= DEATH_RANK && otherrank < COMMOM_RANK )	// 咯扁鳖瘤 吭栏搁 唱绰 矫萍琉 捞惑捞促. 弊繁单 朝 磷牢逞捞
	{															// 单胶 捞惑, 纳牢 捞窍捞搁 酒袍阑 冻绢哆副 犬伏篮 技牢飘肺 绊沥等促
		return SAINT_RANK;
	}

	return merank;		// 唱绰 矫萍琉 捞惑捞绊, 朝 磷牢 逞档 矫萍琉 捞惑捞促. 弊犯促搁 盔贰 珐农措肺 冻绢哆赴促
}

///////////////////////////////////////////////////////////////////////////////////////
//	Memory DB 俊 乐绰 郴侩阑 霸烙函荐肺 颗变促.
//
BOOL USER::Mem2Game(CMemUser *pMD)
{
	if(IsBadReadPtr((CONST VOID*)pMD, sizeof(CMemUser))) return FALSE;

	int i;

	// Copy User Data
	::ZeroMemory( m_strUserID, CHAR_NAME_LENGTH + 1 );
	::CopyMemory(m_strUserID, pMD->m_strUserID, strlen(pMD->m_strUserID));	// User ID
	
	m_sSTR = pMD->m_sSTR;				// 塞
	m_sCON = pMD->m_sCON;				// 扒碍
	m_sDEX = pMD->m_sDEX;				// 刮酶
	m_sVOL = pMD->m_sVOL;				// 狼瘤
	m_sWIS = pMD->m_sWIS;				// 瘤驱

	m_sTempSTR = pMD->m_sTempSTR;		// 檬扁 费蔼甫 罐酒柯促.(饭骇促款老锭 靖)
	m_sTempCON = pMD->m_sTempCON;
	m_sTempDEX = pMD->m_sTempDEX;
	m_sTempVOL = pMD->m_sTempVOL;
	m_sTempWIS = pMD->m_sTempWIS;

	m_iSkin = pMD->m_iSkin;			// 乔何祸
	m_iHair = pMD->m_iHair;			// 赣府祸

	m_curz = pMD->m_curz;				// Zone
	m_curx = pMD->m_curx;				// 困摹 x
	m_cury = pMD->m_cury;				// 困摹 y

	m_sGender = pMD->m_sGender;		// 己喊

	::CopyMemory(m_strFace, pMD->m_strFace, 10);	// 赣府葛剧

	m_dwBuddy = pMD->m_dwBuddy;			// Buddy
	m_dwGuild = pMD->m_dwGuild;			// Guild

	m_tFortressWar		= GUILD_WAR_AFFTER;
	m_tGuildWar			= GUILD_WAR_AFFTER;
	m_tGuildHouseWar	= GUILD_WAR_AFFTER;

	if(m_dwGuild >= 0)			// 辨靛俊 措茄 单捞磐甫 檬扁拳茄促.
	{
		SetGuildVariable();
	}

	CheckGuildWar();			// 父距 傍己傈 吝捞扼搁 葛电 蜡历俊霸 舅赴促.(瘤陛篮 傍己傈父)

	m_bFieldWarApply = FALSE;
	m_FieldWarGMUid = 0;		// 鞘靛傈老锭 惑措规 辨付 uid
	m_dwFieldWar = 0;			// 鞘靛傈 矫累捞搁 惑措祈 辨靛 锅龋啊 甸绢柯促.

	m_sLevel	= pMD->m_sLevel;		// 饭骇

	if(pMD->m_dwExp > GetNextLevel(m_sLevel)) pMD->m_dwExp = GetNextLevel(m_sLevel);	// 救傈 内靛.
	m_dwExp = pMD->m_dwExp;		// Experience

	m_sPA = pMD->m_sPA;			// PA Point

	m_sSkillPoint = pMD->m_sSkillPoint;	// Skill Point

	m_dwXP = pMD->m_dwXP;		// X Point
	
	m_byClass	= pMD->m_byClass;		// 努贰胶

	m_sMaxHP	= g_sHPConst[m_byClass] * m_sCON + g_sHPLV[m_byClass] * (m_sLevel - 1) + g_sHPAdd[m_byClass];
	m_sMaxPP	= g_sPPConst[m_byClass] * m_sWIS + g_sPPLV[m_byClass] * (m_sLevel - 1) + g_sPPAdd[m_byClass];
	m_sMaxSP	= g_sSPConst[m_byClass] * m_sCON + g_sSPLV[m_byClass] * (m_sLevel - 1) + g_sSPAdd[m_byClass];

	m_bLive		= pMD->m_bLive;	// 磷菌绰瘤, 混疽绰瘤?

	m_sHP		= pMD->m_sHP;	// 泅犁 HP
	m_sPP		= pMD->m_sPP;	// 泅犁 PP
	m_sSP		= pMD->m_sSP;	// 泅犁 SP

	if(m_sHP <= 0 || m_bLive == USER_DEAD) 
	{
		m_bLive = USER_DEAD;
		SetZoneIndex(m_curz);
		IsDeadUser();		// 磷篮 蜡历搁 付阑肺...

		m_bLive = USER_LIVE;
		m_sHP = m_sMaxHP;	
	}
	else SetZoneIndex(m_curz);

	if(m_sHP > m_sMaxHP) m_sHP = m_sMaxHP;
	if(m_sPP > m_sMaxPP) m_sPP = m_sMaxPP;
	if(m_sSP > m_sMaxSP) m_sSP = m_sMaxSP;

	if(pMD->m_dwDN < 0) pMD->m_dwDN = 0;
	m_dwDN		= pMD->m_dwDN;			// 家瘤陛

	m_iCityValue = pMD->m_iCityValue;	// 矫刮 穿利蔼
	m_sKillCount = pMD->m_sKillCount;	// PK穿利 冉荐

	for(i = 0; i < 11; i++)//13有天使 11到圣人
	{
		if(m_iCityValue < g_CityRankValue[i])// 穿利摹蔼捞 扁霖 汲沥蔼焊促 累篮瘤 魄窜茄促.
		{
			if(i < 6) { m_sCityRank = i - CITY_RANK_INTERVAL; }
			else { m_sCityRank = i -1 - CITY_RANK_INTERVAL; }
			break;
		}
	}

	//if(m_iCityValue >= g_CityRankValue[12]) m_sCityRank = 7;//
	if(m_iCityValue >= g_CityRankValue[10]) m_sCityRank = CITY_RANK_INTERVAL;

	::CopyMemory(m_strSkill, pMD->m_strSkill, sizeof(m_strSkill));	// 胶懦
	::CopyMemory(m_strItem, pMD->m_strItem, sizeof(m_strItem));		// 酒捞袍
	::CopyMemory(m_strPsi, pMD->m_strPsi, sizeof(m_strPsi));		// 荤捞坷葱

	StrToUserSkill((LPTSTR)pMD->m_strSkill);
	StrToUserItem((LPTSTR)pMD->m_strItem);
	StrToUserPsi((LPTSTR)pMD->m_strPsi);
	StrToHaveEventData((LPTSTR)pMD->m_strHaveEvent);
	StrToUserTel((LPSTR)pMD->m_strTel);

	m_dwExpNext	= GetNextLevel(m_sLevel);						// 促澜饭骇捞 登扁困茄 版氰摹

	for(i = 0; i < TOTAL_SKILL_NUM; i++)						// 胶懦 己傍啦 
	{
		GetSkillInfo(i, m_UserSkillInfo[i]);
	}
//	m_UserChangeSkillInfo;										// 傈流捞 乐阑版快 ?(5)胶懦甫 历厘

	for(i = 0; i < CLASS_NUM; i++)
	{
		m_sChangeClass[i] = pMD->m_sChangeClass[i];
	}

	// 惑怕捞惑 沥焊
	m_tAbnormalKind = pMD->m_tAbnormalKind;	
	m_dwAbnormalTime = pMD->m_dwAbnormalTime;

//	dwMaxDN = m_sLevel * MAX_LEVEL_DN;									// 家瘤陛捞 捞惑茄瘤 眉农
//	if( m_dwDN > dwMaxDN ) m_dwDN = dwMaxDN; 
	m_tDir = myrand(0, 7);												// 泅犁 焊绊乐绰 规氢阑 伐待栏肺 檬扁拳茄促.
		
	m_tIsOP = pMD->m_tIsOP;
	if(pMD->m_tIsOP == 1) AddAbnormalInfo(OPERATION_MODE);				// 扁夯捞 捧疙葛靛烙...

	CheckInvalidGuildZone();

	m_dwSaintTime = pMD->m_dwSaintTime;

	m_dwHiExpTime		= pMD->m_dwHiExpTime;
	m_dwHtExpTime		= pMD->m_dwHtExpTime;
	m_dwMagicFindTime	= pMD->m_dwMagicFindTime;
	m_dwMagicFtTime		= pMD->m_dwMagicFtTime;
	m_dwNoChatTime		= pMD->m_dwNoChatTime;
    m_dwCloseTime		= pMD->m_dwCloseTime;
	m_dwAutoMoney		= pMD->m_dwAutoMoney;
	m_dwLingQu  		= pMD->m_dwLingQu;
	m_dwGuarDianTianShi = pMD->m_dwGuarDianTianShi;//守护天使
	m_dwShopPingDN		= pMD->m_dwShopPingDN;
	m_dwVIPTime		    = pMD->m_dwVIPTime;
	m_dwZaiXianTime		= pMD->m_dwZaiXianTime;
	m_dwBFindTime		= pMD->m_dwBFindTime;
	m_dwBHTime		= pMD->m_dwBHTime;
	m_dwSJTime		= pMD->m_dwSJTime;
	
	m_dwHasteTime = m_dwShieldTime = m_dwDexUpTime = m_dwMaxHPUpTime = 0;					// Psi One
	m_dwFastRunTime = m_dwMindShockTime = m_dwBigShieldTime = m_dwPsiShieldTime = m_dwPiercingShieldTime = 0;	// Psi One

	m_dwAdamantineTime	= m_dwMightyWeaponTime = m_dwBerserkerTime = 0;						// Psi Two

	m_dwMindGuardTime = 0;																	// Psi Three

	if(pMD->m_tPsiOneKind == PSIONIC_HASTE)				m_dwHasteTime		= pMD->m_dwPsiOneTime;
	if(pMD->m_tPsiOneKind == PSIONIC_SHIELD)			m_dwShieldTime		= pMD->m_dwPsiOneTime;
	if(pMD->m_tPsiOneKind == PSIONIC_DEXUP)				m_dwDexUpTime		= pMD->m_dwPsiOneTime;
	if(pMD->m_tPsiOneKind == PSIONIC_HPUP)				m_dwMaxHPUpTime		= pMD->m_dwPsiOneTime;
	if(pMD->m_tPsiOneKind == PSIONIC_FAST_RUN)			m_dwFastRunTime		= pMD->m_dwPsiOneTime;
	if(pMD->m_tPsiOneKind == PSIONIC_MIND_SHOCK)		m_dwMindShockTime	= pMD->m_dwPsiOneTime;
	if(pMD->m_tPsiOneKind == PSIONIC_PSI_SHIELD)		m_dwPsiShieldTime	= pMD->m_dwPsiOneTime;
    if(pMD->m_tPsiOneKind == 30 )                       m_dwBigShieldTime   = pMD->m_dwPsiOneTime;
	if(pMD->m_tPsiOneKind == PSIONIC_PIERCING_SHIELD)	m_dwPiercingShieldTime = pMD->m_dwPsiOneTime;

	if(pMD->m_tPsiTwoKind == PSIONIC_ADAMANTINE)		m_dwAdamantineTime		= pMD->m_dwPsiTwoTime;
	if(pMD->m_tPsiTwoKind == PSIONIC_MIGHTYWEAPON)		m_dwMightyWeaponTime	= pMD->m_dwPsiTwoTime;
	if(pMD->m_tPsiTwoKind == PSIONIC_BERSERKER)			m_dwBerserkerTime		= pMD->m_dwPsiTwoTime;
	
	if(pMD->m_tPsiThreeKind == PSIONIC_MIND_GUARD)		m_dwMindGuardTime		= pMD->m_dwPsiThreeTime;
	//m_dwBSTime=pMD->m_dwBSTime;
	m_dwDNMoney = pMD->m_dwDNMoney;
	m_dwShTsTime = pMD->m_dwShTsTime;
	m_isDoubleExp = pMD->m_isDoubleExp;
	m_isDoubleBAOLV = pMD->m_isDoubleBAOLV;
	
    DWORD dwCurrTime = GetTickCount();
	m_dwLastSpeedTime = dwCurrTime;
	m_dwLastMoveAndRun = dwCurrTime;
	m_dwLastAttackTime = dwCurrTime;
	m_dwLastPsiAttack = dwCurrTime;
	m_dwCastDelay = 0;
	m_dwLastTimeCount = dwCurrTime;
	m_dwLastAbnormalTime = dwCurrTime;
	m_dwLastHPTime = dwCurrTime;

	m_dwLastHasteTime = dwCurrTime;
	m_dwLastShieldTime = dwCurrTime;
	m_dwLastDexUpTime = dwCurrTime;
	m_dwLastMaxHPUpTime = dwCurrTime;
	m_dwLastWISUpTime = dwCurrTime;
	m_dwLastFANTAnTime = dwCurrTime;
	m_dwLastFENGLiTime = dwCurrTime;
	m_dwLastYINENgTime = dwCurrTime;
	m_dwLastDUOCHONgTime = dwCurrTime;

	m_dwLastHiExpTime		= dwCurrTime;
	m_dwLastHtExpTime		= dwCurrTime;
	m_dwLastMagicFindTime	= dwCurrTime;
	m_dwLastMagicFtTime	    = dwCurrTime;
	m_dwLastNoChatTime		= dwCurrTime;
    m_dwLastCloseTime		= dwCurrTime;
	m_dwLastAutoMoney		= dwCurrTime;
	m_dwLastLingQu		= dwCurrTime;
	m_dwLastGuarDianTianShi = dwCurrTime;
	m_dwLastVIPTime			= dwCurrTime;
	m_dwLastBFindTime		= dwCurrTime;
	m_dwLastBHTime		= dwCurrTime;
	m_dwLastSJTime		= dwCurrTime;
	
	m_dwLastAdamantineTime	= dwCurrTime;
	m_dwLastMightyWeaponTime= dwCurrTime;
	m_dwBerserkerTime		= dwCurrTime;

	m_iMyServer = pMD->m_iMyServer;

	GetRecoverySpeed();													// 蜡历 努贰胶狼 雀汗加档甫 搬沥茄促.

	// User Bank --------------------------------
	StrToUserBankItem((LPTSTR)pMD->m_UB.m_UserBankItem);
	m_dwBankDN = pMD->m_UB.m_dwBankDN;

	// Account Bank -----------------------------
	StrToAccountBankItem((LPTSTR)pMD->m_AB.m_AccountBankItem);
	m_dwAccountBankDN = pMD->m_AB.m_dwBankDN;

	return TRUE;
}

int USER::CheckMemoryAccountBankDB(char *strAccount)
{
	CSharedMemory*	pSharedMemory;
	CMemUser*		pMD;
	int				mem_index = -1;
	int				nCount = 0;
	CString			str;

	SYSTEMTIME time;
	GetLocalTime(&time);

	for( int i = 0; i < MAX_USER; i++ )
	{
		pSharedMemory = NULL;
		pMD = NULL;

		pSharedMemory = g_arSharedMemory[i];
		if(!pSharedMemory) continue;

		pMD = (CMemUser*)pSharedMemory->m_lpData;
		if(!pMD) continue;

		if(pMD->m_uid == -1 || pMD->m_sSTR == 0) continue;		// 沥府啊 等 蜡历绰 促矫 历厘窍瘤 臼绰促.
		if(pMD->m_strUserID[0] == 0) continue;

		if( _stricmp( pMD->m_strAccount, strAccount ) == 0 )
		{
			nCount++;
			// 颇老俊 敬促
			if(nCount >= 1)
			{
				GetLocalTime(&time);
				str.Format("(%04d-%02d-%02d %02d:%02d:%02d)\tuid-%d str-%d ubuid-%d count-%d id-%s (AccountBank)\r\n",
					time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,
					pMD->m_uid, pMD->m_sSTR, pMD->m_AB.m_uid, nCount, pMD->m_strAccount );
				
				// IKING 2002.1.
				EnterCriticalSection( &m_CS_FileWrite );
				g_fpBackServer.Write( str, str.GetLength() );
				LeaveCriticalSection( &m_CS_FileWrite );
			}

//			if( !Mem2GameAccountBank(pMD) )			// Shared Memory -> Game 函荐肺
//			{
//				nCount--;
//			}

			UpdateMem2DB(pMD);		// 救傈阑 困秦辑 DB俊档 历厘阑
			UpdateBankMem2DB(pMD);
			UpdateAccountBankMem2DB(pMD);

			pMD->m_uid = -1;
			pMD->m_sSTR = 0;			
			pMD->m_UB.m_uid = -1;
			pMD->m_AB.m_uid = -1;
			pMD->m_dwSaveTime = 0;
			::ZeroMemory(pMD->m_strUserID, sizeof(pMD->m_strUserID));
			::ZeroMemory(pMD->m_strAccount, sizeof(pMD->m_strAccount));
		}
	}

	return nCount;
}

BOOL USER::Mem2GameAccountBank(CMemUser *pMD)
{
	if(IsBadReadPtr((CONST VOID*)pMD, sizeof(CMemUser))) return FALSE;

	// Account Bank -----------------------------
	StrToAccountBankItem((LPTSTR)pMD->m_AB.m_AccountBankItem);
	m_dwAccountBankDN = pMD->m_AB.m_dwBankDN;

	return TRUE;
}

BOOL USER::GiveMagicItem(int sid, int iCount)
{
	int tMagic[12]={36,81,141,33,76,132,97,102,100,130,79,139};
	if( sid < 0 || sid >= g_arItemTable.GetSize() ) return FALSE;
	if( iCount <= 0 ) return FALSE;

	ItemList GiveItem;
	ReSetItemSlot( &GiveItem );
	if(sid==809||sid==811||sid==812||sid==810){ //月神套装 50血10防 回避率6％ 5防
		int iRandom1 =(myrand(0,15)%12);
		int iRandom2 =(myrand(0,15)%12);
		CWordArray		arEmptySlot, arSameSlot;
		int iSlot = GetEmptySlot( INVENTORY_SLOT );
		
		if( iSlot < 0 ) return FALSE;

		ReSetItemSlot( &m_UserItem[iSlot]);

		m_UserItem[iSlot].sLevel = g_arItemTable[sid]->m_byRLevel;
		m_UserItem[iSlot].sSid = sid;
		m_UserItem[iSlot].sCount = iCount;
		m_UserItem[iSlot].sDuration = g_arItemTable[sid]->m_sDuration;
		m_UserItem[iSlot].sBullNum = 3;
		m_UserItem[iSlot].tMagic[0] = tMagic[iRandom1];
		m_UserItem[iSlot].tMagic[1] = tMagic[iRandom2];
		m_UserItem[iSlot].tIQ = MAGIC_ITEM;
		m_UserItem[iSlot].iItemSerial = 0;

		MakeItemLog( &m_UserItem[iSlot], ITEMLOG_EVENT_GIVE );

		arEmptySlot.Add(iSlot); 
		UpdateInvenSlot(&arEmptySlot, &arSameSlot);
		return true;
	}
	if( g_arItemTable[sid]->m_bySpecial == 0 ) return FALSE;		// 汗荤秦临 概流 加己捞 绝促

	GiveItem.sLevel			= g_arItemTable[sid]->m_byRLevel;
	GiveItem.sSid			= sid;
	GiveItem.sCount			= iCount;
	GiveItem.sDuration		= g_arItemTable[sid]->m_sDuration;
	GiveItem.sBullNum		= g_arItemTable[sid]->m_sBullNum;
	GiveItem.tMagic[0]		= g_arItemTable[sid]->m_bySpecial;
	GiveItem.tIQ			= MAGIC_ITEM;

	CWordArray		arEmptySlot, arSameSlot;

	int iSlot = GetSameItem( GiveItem, INVENTORY_SLOT);

	if(iSlot != -1)	
	{ 
		if(iCount != 0)
		{
			CheckMaxValue((short &)m_UserItem[iSlot].sCount, (short)iCount); 
			arSameSlot.Add(iSlot); 
		}
//		return FALSE;		// 概流 酒捞袍篮 般摹扁啊 救登绰 巴栏肺 魄窜茄促
	}
	else			
	{
		iSlot = GetEmptySlot( INVENTORY_SLOT );

		if( iSlot < 0 ) return FALSE;

		ReSetItemSlot( &m_UserItem[iSlot] );

		m_UserItem[iSlot].sLevel = g_arItemTable[sid]->m_byRLevel;
		m_UserItem[iSlot].sSid = sid;
		m_UserItem[iSlot].sCount = iCount;
		m_UserItem[iSlot].sDuration = g_arItemTable[sid]->m_sDuration;
		m_UserItem[iSlot].sBullNum = g_arItemTable[sid]->m_sBullNum;
		m_UserItem[iSlot].tMagic[0] = g_arItemTable[sid]->m_bySpecial;
		m_UserItem[iSlot].tIQ = MAGIC_ITEM;
		m_UserItem[iSlot].iItemSerial = 0;

		MakeItemLog( &m_UserItem[iSlot], ITEMLOG_EVENT_GIVE );

		arEmptySlot.Add(iSlot); 
	}			

	UpdateInvenSlot(&arEmptySlot, &arSameSlot);

//	GetMagicItemSetting();		// 窜瘤 概流 酒捞袍阑 林绰 巴 父栏肺绰 概流 可记捞 救官诧促 : jjs07
/*
	int iSlot = GetEmptySlot(INVENTORY_SLOT);

	if(iSlot < 0) return FALSE;

	ReSetItemSlot(&m_UserItem[iSlot]);

	m_UserItem[iSlot].sLevel = g_arItemTable[sid]->m_byRLevel;
	m_UserItem[iSlot].sSid = sid;
	m_UserItem[iSlot].sCount = iCount;
	m_UserItem[iSlot].sDuration = g_arItemTable[sid]->m_sDuration;
	m_UserItem[iSlot].sBullNum = g_arItemTable[sid]->m_sBullNum;
	m_UserItem[iSlot].tIQ = NORMAL_ITEM;
*/

	FlushItemLog( TRUE );

	return TRUE;
}

void USER::EventItemSerialWindowOpen()
{
	CBufferEx TempBuf;
	
	TempBuf.Add(PRE_REG_USER_EVENT_REQ);

	Send(TempBuf, TempBuf.GetLength());
}

void USER::RecvEventItemSerial(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int j;
	int iSlot = 0;
	int iRet = 0;
	int index = 0;
	int iEventItemSid = 0, iEvent = 0;

	TCHAR strSerial[30];	::ZeroMemory(strSerial,	sizeof(strSerial));
	
	iSlot = GetEmptySlot(INVENTORY_SLOT);

	CString strMsg;

	if(iSlot <= -1)
	{
		strMsg.Format( IDS_USER_NEED_EMPTY_SLOT_CHANGE );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;		// 牢亥捞 厚绢 乐绰瘤 犬牢茄促.
	}

	if(!GetVarString(sizeof(strSerial), strSerial, pBuf, index)) return;

	iRet = UpdateEventItemSerial(strSerial, iEventItemSid);

	if(iRet < 0)
	{
		// DB客 楷搬捞 亮瘤 臼嚼聪促.
		strMsg.Format( IDS_USER_UNSTABLE_CONNECTION );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}
	else if(iRet == 1)
	{
		if(iEventItemSid == 0)
		{
			strMsg.Format( IDS_USER_EVENT_COMMON_CURE );

			m_UserItem[iSlot].tType = TYPE_ITEM;
			m_UserItem[iSlot].sLevel = g_arItemTable[NPC_EVENT_B_ITEM]->m_byRLevel;
			m_UserItem[iSlot].sSid = g_arItemTable[NPC_EVENT_B_ITEM]->m_sSid;
			m_UserItem[iSlot].sCount = 1000;
			m_UserItem[iSlot].sDuration = g_arItemTable[NPC_EVENT_B_ITEM]->m_sDuration;
			m_UserItem[iSlot].sBullNum = g_arItemTable[NPC_EVENT_B_ITEM]->m_sBullNum;
			m_UserItem[iSlot].tIQ = 0;
			m_UserItem[iSlot].iItemSerial = 0;

			for(j = 0; j < MAGIC_NUM; j++)
			{
				m_UserItem[iSlot].tMagic[j] = 0;
			}

			CBufferEx TempBuf;

			TempBuf.Add(ITEM_LOAD_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add((BYTE)0x01);

			TempBuf.Add((BYTE)iSlot);
			TempBuf.Add(m_UserItem[iSlot].sLevel);
			TempBuf.Add(m_UserItem[iSlot].sSid);
			TempBuf.Add(m_UserItem[iSlot].sDuration);
			TempBuf.Add(m_UserItem[iSlot].sBullNum);
			TempBuf.Add(m_UserItem[iSlot].sCount);
			for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_UserItem[iSlot].tMagic[j]);

			TempBuf.Add((BYTE)m_UserItem[iSlot].tIQ);

			Send(TempBuf, TempBuf.GetLength());
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

			m_iCurWeight += g_arItemTable[NPC_EVENT_B_ITEM]->m_byWeight * 1000;
			GetRecoverySpeed();			// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券
			return;
		}
		else if(iEventItemSid > 100 && iEventItemSid <= 255)
		{
			switch(iEventItemSid)
			{
			case EVENT_ATT7_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_ATT7_CHANGE );
				break;
			case EVENT_DEF7_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_DEF7_CHANGE );
				break;
			case EVENT_ATT6_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_ATT6_CHANGE );
				break;
			case EVENT_DEF6_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_DEF6_CHANGE );
				break;
			case EVENT_ATT_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_ATT5_CHANGE );
				break;
			case EVENT_DEF_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_DEF5_CHANGE );
				break;
			case EVENT_INIT_STAT_ITEM:
				iEvent = NPC_EVENT_INIT_STAT; 
				strMsg.Format( IDS_USER_EVENT_RESET_STAT );
				break;
			default:
				return;
				break;
			}

			m_UserItem[iSlot].tType = TYPE_ITEM;
			m_UserItem[iSlot].sLevel = g_arItemTable[iEvent]->m_byRLevel;
			m_UserItem[iSlot].sSid = g_arItemTable[iEvent]->m_sSid;
			m_UserItem[iSlot].sCount = 1;
			m_UserItem[iSlot].sDuration = g_arItemTable[iEvent]->m_sDuration;
			m_UserItem[iSlot].sBullNum = g_arItemTable[iEvent]->m_sBullNum;
			m_UserItem[iSlot].tIQ = (BYTE)iEventItemSid;

			for(j = 0; j < MAGIC_NUM; j++)
			{
				m_UserItem[iSlot].tMagic[j] = 0;
			}

			CBufferEx TempBuf;

			TempBuf.Add(ITEM_LOAD_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add((BYTE)0x01);

			TempBuf.Add((BYTE)iSlot);
			TempBuf.Add(m_UserItem[iSlot].sLevel);
			TempBuf.Add(m_UserItem[iSlot].sSid);
			TempBuf.Add(m_UserItem[iSlot].sDuration);
			TempBuf.Add(m_UserItem[iSlot].sBullNum);
			TempBuf.Add(m_UserItem[iSlot].sCount);
			for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_UserItem[iSlot].tMagic[j]);

			TempBuf.Add((BYTE)m_UserItem[iSlot].tIQ);

			Send(TempBuf, TempBuf.GetLength());
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

			m_iCurWeight += g_arItemTable[iEvent]->m_byWeight;
			GetRecoverySpeed();			// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券

			return;
		}
	}
	else	// 矫府倔捞 撇府芭唱 殿废俊 绝绰 蜡历芭唱 DB俊 函悼捞 绝阑 版快
	{
		if(iRet == 10)
		{
			strMsg.Format( IDS_USER_CHECK_INPUT_NUMBER );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
		else if(iRet == 11)
		{
			strMsg.Format( IDS_USER_NOT_SUBSCRIPTION );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
		else if(iRet == 12)
		{
			strMsg.Format( IDS_USER_ONE_MORE_APPLY );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
		else if(iRet == 13)
		{
			strMsg.Format( IDS_USER_ALREADY_LOTTO );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
		else if(iRet == 14)
		{
			strMsg.Format( IDS_USER_CANCEL_LOTTO );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
}

int USER::UpdateEventItemSerial(TCHAR *pstrSerial, int &item)
{
	short sItemType = 0;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	SWORD	sParam1 = 0;
	SDWORD	cbParam1 = SQL_NTS, cbParam2 = SQL_NTS;

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_PRE_USER_EVENT (\'%s\', \'%s\', \'%s\',\'%s\',?,?)}"), 
		m_strAccount, pstrSerial, g_arServerIPAddr, m_strUserID, sParam1, sItemType);

	hstmt = NULL;
	retcode = 0;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) 
	{
		return -1;
	}
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		return -1;
	}
	
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return -1;
	}

	retcode = SQLBindParameter(hstmt, 1,SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT,0,0,&sParam1,0,&cbParam1);
	retcode = SQLBindParameter(hstmt, 2,SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT,0,0,&sItemType,0,&cbParam2);

    retcode = SQLExecDirect (hstmt, (unsigned char *)(LPCTSTR)szSQL, SQL_NTS);
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

	item = (int)sItemType;
	return sParam1;
}


void USER::EventItemSerialForGameRoomWindowOpen()
{
	CBufferEx TempBuf;
	
	TempBuf.Add(PRE_REG_GAMEROOM_EVENT_REQ);

	Send(TempBuf, TempBuf.GetLength());
}

int USER::UpdateEventItemSerialForGameRoom(TCHAR *pstrSerial, TCHAR *pstrSocNum, int &item)
{
	short sItemType = 0;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	SWORD	sParam1 = 0;
	SDWORD	cbParam1 = SQL_NTS, cbParam2 = SQL_NTS;

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_PRE_GAMEROOM_EVENT (\'%s\', \'%s\', \'%s\', \'%s\',\'%s\',?,?)}"), 
		pstrSocNum, m_strAccount, pstrSerial, g_arServerIPAddr, m_strUserID, sParam1, sItemType);

	hstmt = NULL;
	retcode = 0;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) 
	{
		return -1;
	}
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		return -1;
	}
	
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return -1;
	}

	retcode = SQLBindParameter(hstmt, 1,SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT,0,0,&sParam1,0,&cbParam1);
	retcode = SQLBindParameter(hstmt, 2,SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT,0,0,&sItemType,0,&cbParam2);

    retcode = SQLExecDirect (hstmt, (unsigned char *)(LPCTSTR)szSQL, SQL_NTS);
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

	item = (int)sItemType;
	return sParam1;
}

void USER::RecvEventItemSerialForGameRoom(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int j;
	int iSlot = 0;
	int iRet = 0;
	int index = 0;
	int iEventItemSid = 0, iEvent = 0;

	TCHAR strSerial[30];		::ZeroMemory(strSerial,	sizeof(strSerial));
	TCHAR strSocNum[30];		::ZeroMemory(strSocNum,	sizeof(strSocNum));
	
	iSlot = GetEmptySlot(INVENTORY_SLOT);

	CString strMsg;

	if(iSlot <= -1)
	{
		strMsg.Format( IDS_USER_NEED_EMPTY_SLOT_CHANGE );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;		
	}
	
	if((m_iMaxWeight + m_iMaxWeight) < m_iCurWeight) // 泅犁 公霸啊 弥措 公霸狼 2硅捞惑捞搁...
	{
		strMsg.Format( IDS_USER_OVER_WEIGHT2 );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;		// 牢亥捞 厚绢 乐绰瘤 犬牢茄促.
	}

	if(!GetVarString(sizeof(strSerial),	strSerial, pBuf, index))
	{
		strMsg.Format(IDS_USER_CHECK_INPUT_NUMBER);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}
	if(!GetVarString(sizeof(strSocNum), strSocNum,	pBuf, index))
	{
		strMsg.Format(IDS_USER_CHECK_INPUT_NUMBER);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}

	iRet = UpdateEventItemSerialForGameRoom(strSerial, strSocNum, iEventItemSid);

	if(iRet < 0)
	{
		// DB客 楷搬捞 亮瘤 臼嚼聪促.
		strMsg.Format( IDS_USER_UNSTABLE_CONNECTION );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}
	else if(iRet == 1)
	{
		if(iEventItemSid == 0)
		{
			strMsg.Format( IDS_USER_EVENT_COMMON_CURE );

			m_UserItem[iSlot].tType = TYPE_ITEM;
			m_UserItem[iSlot].sLevel = g_arItemTable[NPC_EVENT_B_ITEM]->m_byRLevel;
			m_UserItem[iSlot].sSid = g_arItemTable[NPC_EVENT_B_ITEM]->m_sSid;
			m_UserItem[iSlot].sCount = 1000;
			m_UserItem[iSlot].sDuration = g_arItemTable[NPC_EVENT_B_ITEM]->m_sDuration;
			m_UserItem[iSlot].sBullNum = g_arItemTable[NPC_EVENT_B_ITEM]->m_sBullNum;
			m_UserItem[iSlot].tIQ = 0;

			for(j = 0; j < MAGIC_NUM; j++)
			{
				m_UserItem[iSlot].tMagic[j] = 0;
			}

			CBufferEx TempBuf;

			TempBuf.Add(ITEM_LOAD_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add((BYTE)0x01);

			TempBuf.Add((BYTE)iSlot);
			TempBuf.Add(m_UserItem[iSlot].sLevel);
			TempBuf.Add(m_UserItem[iSlot].sSid);
			TempBuf.Add(m_UserItem[iSlot].sDuration);
			TempBuf.Add(m_UserItem[iSlot].sBullNum);
			TempBuf.Add(m_UserItem[iSlot].sCount);
			for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_UserItem[iSlot].tMagic[j]);

			TempBuf.Add((BYTE)m_UserItem[iSlot].tIQ);

			Send(TempBuf, TempBuf.GetLength());
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

			m_iCurWeight += g_arItemTable[NPC_EVENT_B_ITEM]->m_byWeight * 1000;
			GetRecoverySpeed();			// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券
			return;
		}
		else if(iEventItemSid > 100 && iEventItemSid <= 255)
		{
			switch(iEventItemSid)
			{
			case EVENT_ATT7_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_ATT7_CHANGE );
				break;
			case EVENT_DEF7_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_DEF7_CHANGE );
				break;
			case EVENT_ATT6_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_ATT6_CHANGE );
				break;
			case EVENT_DEF6_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_DEF6_CHANGE );
				break;
			case EVENT_ATT_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_ATT5_CHANGE );
				break;
			case EVENT_DEF_ITEM:
				iEvent = NPC_EVENT_ITEM; 
				strMsg.Format( IDS_USER_EVENT_DEF5_CHANGE );
				break;
			case EVENT_INIT_STAT_ITEM:
				iEvent = NPC_EVENT_INIT_STAT; 
				strMsg.Format( IDS_USER_EVENT_RESET_STAT );
				break;
			default:
				return;
				break;
			}

			m_UserItem[iSlot].tType = TYPE_ITEM;
			m_UserItem[iSlot].sLevel = g_arItemTable[iEvent]->m_byRLevel;
			m_UserItem[iSlot].sSid = g_arItemTable[iEvent]->m_sSid;
			m_UserItem[iSlot].sCount = 1;
			m_UserItem[iSlot].sDuration = g_arItemTable[iEvent]->m_sDuration;
			m_UserItem[iSlot].sBullNum = g_arItemTable[iEvent]->m_sBullNum;
			m_UserItem[iSlot].tIQ = (BYTE)iEventItemSid;

			for(j = 0; j < MAGIC_NUM; j++)
			{
				m_UserItem[iSlot].tMagic[j] = 0;
			}

			CBufferEx TempBuf;

			TempBuf.Add(ITEM_LOAD_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add((BYTE)0x01);

			TempBuf.Add((BYTE)iSlot);
			TempBuf.Add(m_UserItem[iSlot].sLevel);
			TempBuf.Add(m_UserItem[iSlot].sSid);
			TempBuf.Add(m_UserItem[iSlot].sDuration);
			TempBuf.Add(m_UserItem[iSlot].sBullNum);
			TempBuf.Add(m_UserItem[iSlot].sCount);
			for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_UserItem[iSlot].tMagic[j]);

			TempBuf.Add((BYTE)m_UserItem[iSlot].tIQ);

			Send(TempBuf, TempBuf.GetLength());
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

			m_iCurWeight += g_arItemTable[iEvent]->m_byWeight;
			GetRecoverySpeed();			// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券

			return;
		}
	}
	else	// 矫府倔捞 撇府芭唱 殿废俊 绝绰 蜡历芭唱 DB俊 函悼捞 绝阑 版快
	{
		if(iRet == 10)
		{
			strMsg.Format( IDS_USER_CHECK_LOTTO_NUMBER );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
		else if(iRet == 11)
		{
			strMsg.Format( IDS_USER_CHECK_SOCIAL_NUMBER );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
		else if(iRet == 12)
		{
			strMsg.Format( IDS_USER_ONE_MORE_APPLY );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
		else if(iRet == 13)
		{
			strMsg.Format( IDS_USER_ALREADY_LOTTO );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//
// 蜡历 饭骇捞 公丰 眉氰 扁埃捞 逞菌绰瘤 眉农茄促.
BOOL USER::CheckUserLevel()
{
//	if(m_iDisplayType == 6 && m_sLevel > 25)//yskang 0.5
	if(m_iDisplayType == 6)
	{			
		
		CString strMsg = _T("");
		strMsg.Format("TEST 蜡历丛捞 立加窍脚 眉氰魄 辑滚胶绰 扁瓷惑 力距捞 乐嚼聪促.");
//		SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);

	}

	return FALSE;
}

void USER::GiveEventGameTime(int item_index, int quality)
{
	int sid = item_index;
	int num = 1;

	int i, j;
	int iSlot = -1;

	CBufferEx TempBuf;
	BYTE result = SUCCESS;

	if(sid < 0 || sid >= g_arItemTable.GetSize()) return;

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
//			for(j = 0; j < MAGIC_NUM; j++)
//			{
//				if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
//			}
				
			if(m_UserItem[i].tIQ == TempItem.tIQ) { iSlot = i; break; } 
		}
	}

	CString strMsg = _T("");

	if(iSlot == -1)
	{
		strMsg.Format( IDS_USER_FAIL_CHECK_INVEN );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(m_UserItem[iSlot].sSid < 0)
	{
		strMsg.Format( IDS_USER_FAIL_CHECK_INVEN );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}

	if(m_UserItem[iSlot].sCount < num)		
	{
		strMsg.Format( IDS_USER_FAIL_CHECK_INVEN );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}

	int nRet = 0;
	CString strSerial = _T("");
	strSerial.Format("%d-%d-%d-%d-%d-%d", m_UserItem[iSlot].tMagic[0],m_UserItem[iSlot].tMagic[1],m_UserItem[iSlot].tMagic[2],m_UserItem[iSlot].tMagic[3],m_UserItem[iSlot].tMagic[4],m_UserItem[iSlot].tMagic[5]);

	nRet = UpdateGameTimeForEvent((LPTSTR)(LPCTSTR)strSerial);

	if(nRet != 1)
	{
		if(nRet == 2)
		{
			strMsg.Format( IDS_USER_ONE_MORE_APPLY );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			return;
		}
		else if(nRet == 3)
		{
			strMsg.Format( IDS_USER_ALREADY_REG_NUMBER );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			return;
		}
		else
		{
			strMsg.Format( IDS_USER_FAIL_CHECK_INVEN );
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			return;
		}
	}

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

	strMsg.Format( IDS_USER_YOUR_ACCOUNT_REG, m_strAccount);
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
}

//////////////////////////////////////////////////////////////////////
//
//	捞亥飘俊辑 俺牢 沥咀鼻 惑前鼻阑 雀荐茄促.
//
int USER::UpdateGameTimeForEvent(TCHAR *pstrSerial)
{
	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	SWORD	sParam1 = 0;
	SDWORD	cbParam1 = SQL_NTS;

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_EVENT_GAMETIME_USER(\'%s\', \'%s\', \'%s\', \'%s\',?)}"), 
		m_strAccount, pstrSerial, g_arServerIPAddr, m_strUserID, sParam1);
 
	hstmt = NULL;
	retcode = 0;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) 
	{
		return -1;
	}
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		return -1;
	}
	
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return -1;
	}

	retcode = SQLBindParameter(hstmt, 1,SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT,0,0,&sParam1,0,&cbParam1);

    retcode = SQLExecDirect (hstmt, (unsigned char *)(LPCTSTR)szSQL, SQL_NTS);
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

	return sParam1;
}
/*
野火加入 
功能 显示攻击数值
0 ren\
1 怪
*/
void USER::SendDamageNum(BYTE type,short nTarget, short dDamage)
{
	CBufferEx TempBufnum;
	TempBufnum.Add(DH_DAMAGE_NUM);
	TempBufnum.Add((BYTE)1);
	TempBufnum.Add((short)nTarget);
	TempBufnum.Add((BYTE)type);
	TempBufnum.Add((short)dDamage);				
	SendInsight(TempBufnum, TempBufnum.GetLength());
}
/*
野火加入
功能 设置怪物血条
*/
void USER::SendNpcHP(int nTarget,int npchp)
{
	CBufferEx TempBufnum;
	TempBufnum.Add(M_SET_HP);		
	TempBufnum.Add((DWORD)nTarget);		
	TempBufnum.Add((DWORD)npchp);				
	SendInsight(TempBufnum, TempBufnum.GetLength());
}

void USER::SendSystemMsg(UINT strID, BYTE type, int nWho)
{
	CBufferEx TempBuf;

	char pMsg[1280];	memset( pMsg, NULL, 1280 );
	strcpy( pMsg, _ID( strID ) );

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

////////////////////////////////////////////////////////////////////////////////
//	酒捞袍 漂己阑 官操绰 牢磐其捞胶 劝己拳
//
void USER::UpgradeItemOpen(int event_num)
{
	CBufferEx TempBuf;
	//static int a=0;

	TempBuf.Add(UPGRADE_ITEM_DIALOG);
	TempBuf.Add(BYTE(event_num));
	//TempBuf.Add(BYTE(a));
	//a++;
	Send(TempBuf, TempBuf.GetLength());
}

void USER::UpgradeItemReqBegin(TCHAR *pBuf)
{
	int index = 0;

	BYTE tType = GetByte(pBuf, index);	// 诀弊饭捞靛 鸥涝
	m_bNoItemMove = TRUE;
	switch(tType)
	{
	case 0:				// 普通升级
		UpgradeItemReq(pBuf + index);
		break;
	case 1:				////首饰合成
		UpgradeAccessoriReq(pBuf + index);
		break;

	case 2: case 3: case 12: case 9: case 49:// 2 超级灵石 超级灵石  3灵石晶石  12 袖珍石头升级 9祝福宝石  49王者石头
		UpgradeBlessingItemReq(pBuf + index, tType); 
		break;

	case 4:				// 1窜俺炼, 2窜俺炼, 3窜俺炼
		RemodelingItemReq(pBuf + index);
		break;

	case 5:				//魔法圣装
		RemagicItemReq(pBuf + index);
		break;

	case 6:				// 府概流 (吝汗可记 倾侩窃)
		RemagicItemReq(pBuf + index, TRUE);
		break;

	case 7:				//强化机械 
		EBodyUpgradeReq(pBuf + index);
		break;
	case 8: //守护升级.
		ShouHouUpgradeReq(pBuf + index);
		break;
	case 11:
		DownAccessoriReq(pBuf + index);
		break;
	case 13://机械还原宝珠
		{
			EbodyReset(pBuf + index);
			break;
		}
	case 14://制作进化芯片.
		ShouHouMake(pBuf + index);
		break;
	case 15:
		ShouHouAdd(pBuf + index);
		break;
	case 16://百级转换
		ItemConvert(pBuf + index);
		break;
	case 17://解除属性限制
		RemagicItem_100_3(pBuf + index);
		break;
	case 18:
		RemagicItem_UpgradeReq(pBuf + index);//添加属性
		break;
	case 19:
		RemagicItem1_UpgradeReq(pBuf + index);//变更属性
		break;
	case 20:
		RemagicItem_HuanShi(pBuf + index);
		break;
	case 21: //防具材料化
		RemagicItem_100_1(pBuf + index);
		break;
	case 22://解除防具材料化
		RemagicItem_100_2(pBuf + index);
		break;
	case 27: //机甲控制系统制作使用设计图
		HeChengJJ(pBuf + index);
		break;
	case 28://控制系统升级
		SuitUpgrade(pBuf + index);
		break;
	case 29://安装程序卡		
		ChengXuKa(pBuf + index);
		break;
	case 30://附件		
		JjPeiJian(pBuf + index);
		break;
	case 25:case 31: //黄金石头31
		UpgradeHJItemReq(pBuf + index);
		break;
	case 34://机械身体材料化0x22 0x0a 0x00
		{
			short sMetrial = GetShort(pBuf,index);
			EbodyMatrial(sMetrial);
			break;
		}
	case 35://机械身体解除材料化
		{
			short sMetrial = GetShort(pBuf,index);

			EbodyCancelMetral(sMetrial);
			break;
		}
	case 36://机械身体合成 0x24 0x1f 0x00 0x11 0x00 0x0a 0x00 0xff 0xff 0xff 0xff 
		{
			short sMaster = GetShort(pBuf,index);
			short sMetrial1 = GetShort(pBuf,index);
			short sMetrial2 = GetShort(pBuf,index);
			EbodyConvertSuperEbody(sMaster,sMetrial1,sMetrial2);
			break;
		}
/*	case 49://王者灵石 魔法武器前6改增加13,7改增加15,物理增加30损,10改以上必须使用王者灵石，王者晶石+神石一个，防具每改增加10防
		{
			short sMaster = GetShort(pBuf,index);
			short sMetrial = GetShort(pBuf,index);
			BYTE bReceive = GetByte(pBuf,index);
			short sAdd1 = GetShort(pBuf,index);
			short sAdd2 = GetShort(pBuf,index);
			short sAdd3 = GetShort(pBuf,index);
			short sAdd4 = GetShort(pBuf,index);
			Update130Item(sMaster,sMetrial,bReceive,sAdd1,sAdd2,sAdd3,sAdd4);

			break;
		}*/
	case 55://130级装备改造
		{
			short sMaster = GetShort(pBuf,index);
			short sMetrial1 = GetShort(pBuf,index);
			short sMetrial2 = GetShort(pBuf,index);
			short sMetrial3 = GetShort(pBuf,index);
			Item130Convert(sMaster,sMetrial1,sMetrial2,sMetrial3);
			break;
		}

	default: 
		break;
	}
	m_bNoItemMove = FALSE;
}
//冰封水晶　把８改降成５改的
void USER::DownAccessoriReq(TCHAR *pBuf)
{
/*BYTE tCount = 0;
	int index = 0,	iSuccess = 0;
	int iWeight = 0;
	BYTE tItemSlot = -1;
	short sMaterialSlot = -1;
	BYTE bySlot = 0;
	short sMaterialSid = -1;
	short sSourceSid = -1;
	BYTE tSourceWear = 0;
	int iRandom = 0;
	int iAllSkin = -1;
	int iMagicRandom = 0;
	short iMagicSid = 0;
	int i;

	ItemList TempItem;
	CByteArray arMaterial;
	arMaterial.RemoveAll();

	short sSourceSlot = GetShort(pBuf, index);	//得到首饰物品的位置
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX)
		return;
	sSourceSid = m_UserItem[sSourceSlot].sSid;//得到首饰物品ID
	if(sSourceSid < 0 || sSourceSid >= g_arItemTable.GetSize())
		return;
	tSourceWear = g_arItemTable[sSourceSid]->m_byWear;//得到物首饰品类型
	tCount = m_UserItem[sSourceSlot].tMagic[5];
	if(tCount != 8 )
		return;
	//材料位置
	sMaterialSlot = GetShort(pBuf, index);	//得到冰风水晶物品的位置
	if(sMaterialSlot < EQUIP_ITEM_NUM || sMaterialSlot >= TOTAL_INVEN_MAX)
		return;
	sMaterialSid = m_UserItem[sMaterialSlot].sSid;//得到冰风水晶物品ID
	if(sMaterialSid < 0 || sMaterialSid >= g_arItemTable.GetSize())
		return;

	//是否存在冰风水晶
	if(sMaterialSid != 1006)
		return ;
	//物品数据
	if(m_UserItem[sMaterialSlot].sCount<=0)
		return ;
	//if(FindSidInInven(SID_BING_FENG_SHUI_JING) >= TOTAL_INVEN_MAX)
	//	return ;
	if(m_dwDN < 500000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_SHUI, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}
	if( m_dwDN <= 500000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 500000;
	
	for(i = 1; i < 4; i++)
	{
		if(m_UserItem[sSourceSlot].tMagic[i] == 137)
		{
			iAllSkin = i;
			break;
		}
	}
	if(iAllSkin > 0)//所有1技能的还原
	{
		for(i = 1; i < 4; i++)
		{
			if(iAllSkin == i)
			{
				continue;
			}
			iMagicSid = m_UserItem[sSourceSlot].tMagic[i];
			iRandom = myrand(0,10000);
			if(iRandom < 2500)
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelTop.GetSize() - 1);				
				iMagicSid = g_Ripel.m_arRipelTop[iMagicRandom];
			}
			else if(iRandom >= 2500 && iRandom < 5000)
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelBottom.GetSize() - 1);				
				iMagicSid = g_Ripel.m_arRipelBottom[iMagicRandom];
			}
			else if(iRandom >= 5000 && iRandom < 7500)
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelLeft.GetSize() - 1);				
				iMagicSid = g_Ripel.m_arRipelLeft[iMagicRandom];
			}
			else
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelRight.GetSize() - 1);				
				iMagicSid = g_Ripel.m_arRipelRight[iMagicRandom];
			}	
		}
		m_UserItem[sSourceSlot].tMagic[5] = 8;
		m_UserItem[sSourceSlot].tIQ = RARE_ITEM;
	}
	
	//删除冰风水晶
	//RobItem( SID_BING_FENG_SHUI_JING, 1);
	//冰风水晶100%成功
	//iRandom = myrand(1, 10000);
	//if(iRandom <= 8000) 
	
	//else 
	//	iSuccess = 0;
	/*if(iSuccess == 0)	// 俊捞歹父 朝扼埃促.
	{
		ReSetItemSlot(&m_UserItem[sMaterialSlot]);
		iWeight += g_arItemTable[sMaterialSid]->m_byWeight;		// 公霸函拳甫 眉农茄促.
		arMaterial.Add((BYTE)sMaterialSlot);
	}
	else
	{
		
		m_UserItem[sSourceSlot].tMagic[1] = 0;
		m_UserItem[sSourceSlot].tMagic[2] = 0;
		m_UserItem[sSourceSlot].tMagic[3] = 0;
		m_UserItem[sSourceSlot].tMagic[5] = 5;					//改变改数
		m_UserItem[sSourceSlot].tIQ = MAGIC_ITEM;
		iWeight += g_arItemTable[sMaterialSid]->m_byWeight;		// 公霸函拳甫 眉农茄促.
		arMaterial.Add((BYTE)sSourceSlot);
		arMaterial.Add((BYTE)sMaterialSlot);
	}
	//FlushItemLog( TRUE );//清空缓冲
	//iSuccess = 1;
//物品数据
		if(m_UserItem[sMaterialSlot].sCount<=0)
			return ;
		m_UserItem[sMaterialSlot].sCount--;
		if(m_UserItem[sMaterialSlot].sCount<=0)
			ReSetItemSlot(&m_UserItem[sMaterialSlot]);

	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();	
	CBufferEx TempBuf;// 捣函版 沥焊甫 焊辰促.
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	/*if(iSuccess == 1)
		TempBuf.Add(SUCCESS);
	else		
		TempBuf.Add(FAIL);
	TempBuf.Add((BYTE)arMaterial.GetSize());
	for(int i = 0; i < arMaterial.GetSize(); i++)
	{
		bySlot = (BYTE)arMaterial[i];

		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);
		for(int j =0; j < MAGIC_NUM; j++) 
			TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	arMaterial.RemoveAll();*/

	int index = 0;
	short Slot = GetShort(pBuf, index);//
	short Slot1 = GetShort(pBuf, index);//冰风水晶
	int iAllSkin = -1;//是否有所有1技能
	int iRandom = 0 ,iMagicRandom = 0;
	short iMagicSid = 0;
	int i;
	if(m_UserItem[Slot1].sSid != 1006) return;

	if(Slot < EQUIP_ITEM_NUM || Slot >= TOTAL_INVEN_MAX) return;
	if(Slot1 < EQUIP_ITEM_NUM || Slot1 >= TOTAL_INVEN_MAX) return;
	if(m_UserItem[Slot].tMagic[5] != 8) return;

	if(m_dwDN < 200000)//不够金币
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_SHUI, SYSTEM_ERROR, TO_ME);
		return ;	
	}

	short sSourceSid = m_UserItem[Slot].sSid;//得到物品ID
	if(sSourceSid < 0 || sSourceSid >= g_arItemTable.GetSize()) return;
	if(g_arItemTable[sSourceSid]->m_byWear <= 5 || g_arItemTable[sSourceSid]->m_byWear >= 9) return;//判断物品类型

	for(i = 1; i < 4; i++)
	{
		if(m_UserItem[Slot].tMagic[i] == 137)
		{
			iAllSkin = i;
			break;
		}
	}
	if(iAllSkin > 0)//所有1技能的还原
	{
		for(i = 1; i < 4; i++)
		{
			if(iAllSkin == i)
			{
				continue;
			}
			iMagicSid = m_UserItem[Slot].tMagic[i];
			iRandom = myrand(0,10000);
			if(iRandom < 2500)
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelTop.GetSize() - 1);				
				iMagicSid = g_Ripel.m_arRipelTop[iMagicRandom];
				m_UserItem[Slot].tMagic[i] = (BYTE)iMagicSid;
			}
			else if(iRandom >= 2500 && iRandom < 5000)
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelBottom.GetSize() - 1);				
				iMagicSid = g_Ripel.m_arRipelBottom[iMagicRandom];
				m_UserItem[Slot].tMagic[i] = (BYTE)iMagicSid;
			}
			else if(iRandom >= 5000 && iRandom < 7500)
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelLeft.GetSize() - 1);				
				iMagicSid = g_Ripel.m_arRipelLeft[iMagicRandom];
				m_UserItem[Slot].tMagic[i] = (BYTE)iMagicSid;
			}
			else
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelRight.GetSize() - 1);				
				iMagicSid = g_Ripel.m_arRipelRight[iMagicRandom];
				m_UserItem[Slot].tMagic[i] = (BYTE)iMagicSid;
			}	
		}
		m_UserItem[Slot].tMagic[5] = 8;
		m_UserItem[Slot].tIQ = RARE_ITEM;
	}
	else //没有所有1技能的还原
	{
		m_UserItem[Slot].tMagic[1] = 0;
		m_UserItem[Slot].tMagic[2] = 0;
		m_UserItem[Slot].tMagic[3] = 0;
		m_UserItem[Slot].tMagic[4] = 0;
		m_UserItem[Slot].tMagic[5] = 5;
		m_UserItem[Slot].tIQ = MAGIC_ITEM;
	}

	if(m_UserItem[Slot1].sCount > 1) m_UserItem[Slot1].sCount -= 1;
	else ReSetItemSlot(&m_UserItem[Slot1]);
	m_dwDN -= 200000;
	if(m_dwDN <= 0) m_dwDN = 0;
	SendMoneyChanged();
	UpdateUserItemDN();

	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)2);
	BYTE bySlot = (BYTE)Slot;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ);
	bySlot = (BYTE)Slot1;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ);
	Send(TempBuf, TempBuf.GetLength());
}
////////////////机械还原宝珠//////////////////////
void USER::EbodyReset(TCHAR *pBuf)
{
	int index=0,i;
	short Slot = GetShort(pBuf, index);//还原珠宝
	short Slot1 = GetShort(pBuf, index);//还原机械

	if(Slot < EQUIP_ITEM_NUM || Slot >= TOTAL_INVEN_MAX)
		return;
	if(Slot1 < EQUIP_ITEM_NUM || Slot1 >= TOTAL_INVEN_MAX)
		return;
	//
	short sMaterialSid = m_UserItem[Slot].sSid;//得到物品ID
	if(sMaterialSid < 0 || sMaterialSid >= g_arItemTable.GetSize())
		return;
	if(sMaterialSid != 1030)
		return;
	if(m_UserItem[Slot].sCount<=0)
		return;
	//
	short sMasterSid = m_UserItem[Slot1].sSid;//得到物品ID
	if(sMasterSid < 0 || sMasterSid >= g_arItemTable.GetSize())
		return;
	//机械
	if(!IsEbodyItem(sMasterSid)&&!IsSuperEbodyItem(sMasterSid) )
		return;
	BYTE bLastMagic=0xff;
	BYTE bPos = 0xff;
	for(i = 4;i>0;i--)
	{
		if(m_UserItem[Slot1].tMagic[i] != 0)
		{
			bLastMagic = m_UserItem[Slot1].tMagic[i];
			bPos = i;
			break;
		}
	}
	if(bPos == 0xff)
		return ;
	if(bLastMagic>g_arEBodyTable.GetSize())
		return ;
	//是否有位置
	int bEmptyPos = GetEmptySlot(INVENTORY_SLOT);
	if(bEmptyPos == -1)
	{
		return ;
	}
	CByteArray arMaterial;
	arMaterial.RemoveAll();
	arMaterial.Add((BYTE)Slot);
	arMaterial.Add((BYTE)Slot1);
	arMaterial.Add((BYTE)bEmptyPos);


	m_UserItem[Slot].sCount--;
	if(m_UserItem[Slot].sCount<=0)
		ReSetItemSlot(&m_UserItem[Slot]);
	m_UserItem[Slot1].tMagic[bPos] = 0;
	m_UserItem[Slot1].sLevel -= g_arEBodyTable[bLastMagic]->m_tLevel;
	
	ReSetItemSlot(&m_UserItem[bEmptyPos]);
	m_UserItem[bEmptyPos].sSid = 907;
	m_UserItem[bEmptyPos].sCount = 1;
	m_UserItem[bEmptyPos].sLevel = g_arItemTable[907]->m_byRLevel;
	m_UserItem[bEmptyPos].sDuration = g_arItemTable[907]->m_sDuration;
	m_UserItem[bEmptyPos].tMagic[0] = bLastMagic;
	m_UserItem[bEmptyPos].tIQ = 2;

	FlushItemLog( TRUE );//清空缓冲

	CBufferEx TempBuf;// 捣函版 沥焊甫 焊辰促.
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add((BYTE)arMaterial.GetSize());
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];

		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);
		for(int j =0; j < MAGIC_NUM; j++) 
			TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	arMaterial.RemoveAll();
}
//为百级装备添加属性 100W
void USER::RemagicItem_UpgradeReq(TCHAR *pBuf)
{
	bool isSuccess = false;
	int index=0;
	short Slot = GetShort(pBuf, index);//装备
	short Slot1 = GetShort(pBuf, index);//幻石
	
	
	if(Slot1 < EQUIP_ITEM_NUM || Slot1 >= TOTAL_INVEN_MAX) return;
	if(Slot < EQUIP_ITEM_NUM || Slot >= TOTAL_INVEN_MAX) return;
	if(m_UserItem[Slot].tIQ!=12) return;
	if(m_UserItem[Slot1].sSid!=1096) return ;
	if(m_UserItem[Slot1].tMagic[0] < 3 || m_UserItem[Slot1].tMagic[0] > 72) return;
	if(m_UserItem[Slot].tMagic[10] > 1 ) return;
	if(m_dwDN < 1000000)
	{SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);	return ;	}
	if(FANGJUCAILIAOHUA_XP > m_dwXP) { SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME); return; }

	int iUpgradeCount = -1;
	int magicSlot = -1;
	for(int i = 6; i < 11; i++)
	{
		if(m_UserItem[Slot].tMagic[i] == 1) 
		{
			magicSlot = i;
			iUpgradeCount = i-6;
			break;
		}
	}
	if(iUpgradeCount == -1) return;
	int iRandom = myrand(1, 10000);
	//iRandom = UpgradeSucc(iRandom);
	if(iRandom <= g_HuanShiJiLv[iUpgradeCount]) isSuccess = true;

	if(isSuccess)
	{
		if(m_UserItem[Slot].tMagic[magicSlot] == 1)			m_UserItem[Slot].tMagic[magicSlot] = m_UserItem[Slot1].tMagic[0];

		if( m_dwDN <= 1000000 ) m_dwDN = 0;
		else m_dwDN = m_dwDN - 1000000;
		UpdateUserItemDN();						
		SendMoneyChanged();	
	}


	if(m_UserItem[Slot1].sCount > 1)
		m_UserItem[Slot1].sCount = m_UserItem[Slot1].sCount -1;
	else
	ReSetItemSlot(&m_UserItem[Slot1]);
	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	if(isSuccess)	TempBuf.Add((BYTE)1);
	else TempBuf.Add((BYTE)2);
	TempBuf.Add((BYTE)2);
	BYTE bySlot = (BYTE)Slot;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ); 

	bySlot = (BYTE)Slot1;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
}
//为百级装备变更属性 100W
void USER::RemagicItem1_UpgradeReq(TCHAR *pBuf)
{
	int index=0,iSuccess=2;
	short Slot = GetShort(pBuf, index);//装备
	short Slot1 = GetShort(pBuf, index);//幻石
	
	
	if(Slot1 < EQUIP_ITEM_NUM || Slot1 >= TOTAL_INVEN_MAX) return;
	if(Slot < EQUIP_ITEM_NUM || Slot >= TOTAL_INVEN_MAX) return;
	if(m_UserItem[Slot].tIQ!=12) return;
	if(m_UserItem[Slot1].sSid!=1096) return ;
	if(m_UserItem[Slot1].tMagic[5]!=1) return ;

	if(m_dwDN < 1000000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}
	if( m_dwDN <= 1000000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 1000000;
	UpdateUserItemDN();						
	SendMoneyChanged();
	
	int rand=myrand(0,100)%100;
	int s=(m_UserItem[Slot1].tMagic[0]-2)%10*7+10;
	if(rand>s){
		iSuccess=1;
		m_UserItem[Slot].tMagic[6]=m_UserItem[Slot1].tMagic[0];
	}
	ReSetItemSlot(&m_UserItem[Slot1]);
	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)iSuccess);
	TempBuf.Add((BYTE)2);
	BYTE bySlot = (BYTE)Slot;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ); 
	
	bySlot = (BYTE)Slot1;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ);
	
	Send(TempBuf, TempBuf.GetLength());
}
//幻石属性升级。。。。
void USER::RemagicItem_HuanShi(TCHAR *pBuf)
{
	bool isSuccess = false;
	int index=0;
	short Slot = GetShort(pBuf, index);//空
	short Slot1 = GetShort(pBuf, index);//幻石
	short Slot2 = GetShort(pBuf, index);//幻石

	if(Slot1 < EQUIP_ITEM_NUM || Slot1 >= TOTAL_INVEN_MAX) return;
	if(Slot2 < EQUIP_ITEM_NUM || Slot2 >= TOTAL_INVEN_MAX) return;
	if(m_UserItem[Slot1].sSid!=1096 || m_UserItem[Slot2].sSid!=1096)return ;
	if(m_UserItem[Slot1].tMagic[5] >= 10 || m_UserItem[Slot2].tMagic[5] >=10) return;
	if(m_UserItem[Slot1].tMagic[0]!=m_UserItem[Slot2].tMagic[0]) return ;
	if(m_UserItem[Slot1].tMagic[5]!=m_UserItem[Slot2].tMagic[5]) return;
	if(m_dwDN < 500000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}
	if( m_dwDN <= 500000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 500000;
	UpdateUserItemDN();						
	SendMoneyChanged();
	ReSetItemSlot(&m_UserItem[Slot2]);
	int iRandom = myrand(1, 10000);
	if(iRandom <= g_ItemHsUpgrade[m_UserItem[Slot1].tMagic[5]]) isSuccess = true;
	if(isSuccess)
	{
		m_UserItem[Slot1].tMagic[5]++;
		m_UserItem[Slot1].tMagic[0]++;
	}

	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	if(isSuccess) TempBuf.Add((BYTE)1);
	else TempBuf.Add((BYTE)2);
	TempBuf.Add((BYTE)2);
	BYTE bySlot = (BYTE)Slot1;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ); 
	
	bySlot = (BYTE)Slot2;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ);
	
	Send(TempBuf, TempBuf.GetLength());


}
void USER::ShouHouAdd(TCHAR *pBuf)
{
	int index=0,iSuccess=0,s;
	int Magic[3]={0,0,0};
	int newMagic=-1,up=1;

	short Slot = GetShort(pBuf, index);//蕊片
	short Slot1 = GetShort(pBuf, index);//守护
	if(Slot < EQUIP_ITEM_NUM || Slot >= TOTAL_INVEN_MAX) return;

	if(Slot1 < EQUIP_ITEM_NUM || Slot1 >= TOTAL_INVEN_MAX) return;
	if(m_UserItem[Slot].sSid==-1 ||m_UserItem[Slot1].sSid==-1)
		return ;
	if(m_UserItem[Slot].sSid!=1051)
		return ;
	if(g_arItemTable[m_UserItem[Slot1].sSid]->m_byWear!=130)
		return ;
	
	for(int i=1;i<4;i++){
		switch (m_UserItem[Slot1].tMagic[i]){//计算守护666属性
			case 0:
				newMagic=i;
				break;
			case 1:case 2:case 3:case 4:case 5://每次加点3防
				Magic[0]=i;
				break;
			case 6:
				Magic[0]=-1;
				break;
			case 7:case 8:case 9:case 10:case 11:
				Magic[1]=i;
				break;
			case 12://每级加15血
				Magic[1]=-1;
				break;
			case 13:case 14:case 15:case 16:case 17:
				Magic[2]=i;
				break;
			case 18://每级增加5抗AGIC_DEFENSE_UP
				Magic[2]=-1;
				break;
		 default:
		 	break;
		}
	}
	int rand=myrand(0,100)%100;

	switch(m_UserItem[Slot].tMagic[0]){
		case 5://每次加点3防
			if(Magic[0]==-1) return;
			if(Magic[0]!=0) 
				newMagic=Magic[0];
			else
				up=1;
			
			break;
		case 12://每级加15血
		if(Magic[1]==-1) return;
			if(Magic[1]!=0) 
				newMagic=Magic[1];
			else
				up=7;
			break;
			
		case 6://每级增加5抗
			if(Magic[2]==-1) return;
			if(Magic[2]!=0) 
				newMagic=Magic[2];
			else 
				up=13;
			break;
		default:
			return;
	}
	s=(m_UserItem[Slot1].tMagic[newMagic]%6)*10+10;
	if(rand>s){
		iSuccess=1;
		m_UserItem[Slot1].tMagic[newMagic]+=up;
		if(m_UserItem[Slot].tMagic[0] ==5)
		{
         CString sysMsg;
         sysMsg.Format("恭喜玩家 %s 将[%s]成功添加防御+3芯片! ",this->m_strUserID,g_arItemTable[m_UserItem[Slot1].sSid]->m_strName );
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
		if(m_UserItem[Slot].tMagic[0] ==12)
		{
         CString sysMsg;
         sysMsg.Format("恭喜玩家 %s 将[%s]成功添加上血+15芯片! ",this->m_strUserID,g_arItemTable[m_UserItem[Slot1].sSid]->m_strName );
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
		if(m_UserItem[Slot].tMagic[0] ==6)
		{
         CString sysMsg;
         sysMsg.Format("恭喜玩家 %s 将[%s]成功添加上魔抗+5芯片! ",this->m_strUserID,g_arItemTable[m_UserItem[Slot1].sSid]->m_strName );
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
	}
	//去掉蕊片
	ReSetItemSlot(&m_UserItem[Slot]);

	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	if(iSuccess)
		TempBuf.Add((BYTE)6);
	else
		TempBuf.Add((BYTE)7);
	TempBuf.Add((BYTE)2);
	BYTE bySlot = (BYTE)Slot;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ); 
	
	bySlot = (BYTE)Slot1;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ);
	
	Send(TempBuf, TempBuf.GetLength());


}
void USER::ShouHouMake(TCHAR *pBuf)
{
	int iWeight=0;
	int index=0;
	BYTE tMagic=0;
	CUIntArray arMaterial;

	short Slot = GetShort(pBuf, index);//盒子
	short Slot1 = GetShort(pBuf, index);//晶体
	short Slot2 = GetShort(pBuf, index);//垃圾守护
	short Slot3 = GetShort(pBuf, index);//垃圾守护
	short Slot4 = GetShort(pBuf, index);//垃圾守护
	short Slot5 = GetShort(pBuf, index);//垃圾守护
	if(Slot < EQUIP_ITEM_NUM || Slot >= TOTAL_INVEN_MAX) return;
	if(Slot1 < EQUIP_ITEM_NUM || Slot1 >= TOTAL_INVEN_MAX) return;
	if(m_UserItem[Slot].sSid!=1050)
		return;
	arMaterial.Add(Slot);
	arMaterial.Add(Slot1);
	if(m_UserItem[Slot1].sSid==665||m_UserItem[Slot1].sSid==668||m_UserItem[Slot1].sSid==663){
		if(m_UserItem[Slot1].sCount<100)
			return;
		if(m_UserItem[Slot1].sSid==665)//防加3
			tMagic=5;
		if(m_UserItem[Slot1].sSid==668)//血加15
			tMagic=6;
		if(m_UserItem[Slot1].sSid==663)//抗加5
			tMagic=12;
	}else{
		return;
	}
	if(Slot2!=-1){
		if(Slot2 < EQUIP_ITEM_NUM || Slot2 >= TOTAL_INVEN_MAX) return;
		iWeight=iWeight+g_arItemTable[m_UserItem[Slot2].sSid]->m_byWeight;
		ReSetItemSlot(&m_UserItem[Slot2]);
		arMaterial.Add(Slot2);
		
	}
	if(Slot3!=-1){
		if(Slot3 < EQUIP_ITEM_NUM || Slot3 >= TOTAL_INVEN_MAX) return;
		iWeight=iWeight+g_arItemTable[m_UserItem[Slot3].sSid]->m_byWeight;
		ReSetItemSlot(&m_UserItem[Slot3]);
		arMaterial.Add(Slot3);
		
	}
	if(Slot4!=-1){
		if(Slot4 < EQUIP_ITEM_NUM || Slot4 >= TOTAL_INVEN_MAX) return;
		iWeight=iWeight+g_arItemTable[m_UserItem[Slot4].sSid]->m_byWeight;
		ReSetItemSlot(&m_UserItem[Slot4]);
		arMaterial.Add(Slot4);
		
	}
	if(Slot5!=-1){
		if(Slot5 < EQUIP_ITEM_NUM || Slot5 >= TOTAL_INVEN_MAX) return;
		iWeight=iWeight+g_arItemTable[m_UserItem[Slot5].sSid]->m_byWeight;
		ReSetItemSlot(&m_UserItem[Slot5]);
		arMaterial.Add(Slot5);
		
	}
	iWeight=iWeight+g_arItemTable[m_UserItem[Slot1].sSid]->m_byWeight;
	if(m_UserItem[Slot1].sCount>100){
		m_UserItem[Slot1].sCount=m_UserItem[Slot1].sCount-100;
	}else
		ReSetItemSlot(&m_UserItem[Slot1]);
	ReSetItemSlot(&m_UserItem[Slot]);
	int sSid=1051;
	m_UserItem[Slot].sLevel = g_arItemTable[sSid]->m_byRLevel;
	m_UserItem[Slot].sSid = sSid;
	m_UserItem[Slot].sCount = 1;
	m_UserItem[Slot].sDuration = g_arItemTable[sSid]->m_sDuration;
	m_UserItem[Slot].sBullNum = 1;
	m_UserItem[Slot].tMagic[0] = tMagic;
	m_UserItem[Slot].tMagic[1] = 0;
	m_UserItem[Slot].tMagic[2] = 0;
	m_UserItem[Slot].tMagic[3] = 0;
	m_UserItem[Slot].tMagic[4] = 0;
	m_UserItem[Slot].tMagic[5] = 0;
	m_UserItem[Slot].tIQ = 2;
	m_UserItem[Slot].iItemSerial = 0;
	
	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	CBufferEx TempBuf;
	int i,j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();
	TempBuf.Add((BYTE)5);
	TempBuf.Add((BYTE)index);
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
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
void USER::ShouHouUpgradeReq(TCHAR *pBuf)
{
	int index=0;
	int n;
	int iSuccess = 2;
	BYTE tSourceWear = 0;		// 来源类型
	int iWeight=0;
	///////////////////////////////////////////////////////
	if(m_dwDN < 1000000)
	{
		SendSystemMsg("升级守护要100万决战币,升级失败守护不消失", SYSTEM_ERROR, TO_ME);
		return;
	}
    //////////////////////////////////////////////////////////
	short sSrcSlot = GetShort(pBuf, index);//主体
	short sDstSlot = GetShort(pBuf, index);//晶体
	if(sSrcSlot < EQUIP_ITEM_NUM || sSrcSlot >= TOTAL_INVEN_MAX) return;
	if(sDstSlot < EQUIP_ITEM_NUM || sDstSlot >= TOTAL_INVEN_MAX) return;
	if(m_UserItem[sDstSlot].sSid!=964)//升级的物品不是晶体.
		return;
	if(m_UserItem[sSrcSlot].tIQ!=9)
		return;
	n=m_UserItem[sSrcSlot].tMagic[5];
	if(n>=2){
		m_UserItem[sSrcSlot].tMagic[5]=2;
		return;
	}
	// 如果物品类型不等于130退出
	tSourceWear = g_arItemTable[m_UserItem[sSrcSlot].sSid]->m_byWear;
	if(tSourceWear != 130)								// 检查是否是可以生计的物品类型(守护)
	{
		SendSystemMsg( IDS_USER_CANT_UPGRADE_ITEM, SYSTEM_ERROR, TO_ME);
		return;
	}
	int rand=myrand(0,100)%100;
	if(n==0&&rand<40)
		iSuccess=1;
	if(n==1&&rand<15)
		iSuccess=1;
	if(iSuccess==1)
	{//成功了.....
		m_UserItem[sSrcSlot].tMagic[0]++;
		m_UserItem[sSrcSlot].tMagic[5]++;
		if(m_UserItem[sSrcSlot].tMagic[5]==2 && m_UserItem[sSrcSlot].tMagic[0]==6)//大于或等于8改提示
		{
         CString sysMsg;
         sysMsg.Format("玩家 %s 将武器守护[%s]强化为2改 ! ",this->m_strUserID,g_arItemTable[m_UserItem[sSrcSlot].sSid]->m_strName );
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
		if(m_UserItem[sSrcSlot].tMagic[5]==2 && m_UserItem[sSrcSlot].tMagic[0]==30)//大于或等于8改提示
		{
         CString sysMsg;
         sysMsg.Format("玩家 %s 将技能守护[%s]强化为2改 ! ",this->m_strUserID,g_arItemTable[m_UserItem[sSrcSlot].sSid]->m_strName );
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
	}
	else{//失败了.....
		////////////////////////////////////////////////////////////////////////////////
		if (m_UserItem[sSrcSlot].tMagic[5] == 1 )
		{
			m_UserItem[sSrcSlot].tMagic[0] -=1;
			m_UserItem[sSrcSlot].tMagic[5] -=1;
		}

		/////////////////////////////////////////////////////////////////////////////////
		 iWeight+= g_arItemTable[m_UserItem[sSrcSlot].sSid]->m_byWeight;
		// ReSetItemSlot(&m_UserItem[sSrcSlot]);
		
	}
	iWeight+= g_arItemTable[m_UserItem[sDstSlot].sSid]->m_byWeight;
	ReSetItemSlot(&m_UserItem[sDstSlot]);
	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;
	///////////////////////////////////////////////////////////////////////////////////
	if( m_dwDN <= 1000000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 1000000;
	UpdateUserItemDN();	
	SendMoneyChanged();

	//////////////////////////////////////////////////////////////////////////////////
	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)iSuccess);
	TempBuf.Add((BYTE)2);
	TempBuf.Add((BYTE)sSrcSlot);
	TempBuf.Add(m_UserItem[sSrcSlot].sLevel);
	TempBuf.Add(m_UserItem[sSrcSlot].sSid);
	TempBuf.Add(m_UserItem[sSrcSlot].sDuration);
	TempBuf.Add(m_UserItem[sSrcSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSrcSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSrcSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSrcSlot].tIQ); 

	TempBuf.Add((BYTE)sDstSlot);
	TempBuf.Add(m_UserItem[sDstSlot].sLevel);
	TempBuf.Add(m_UserItem[sDstSlot].sSid);
	TempBuf.Add(m_UserItem[sDstSlot].sDuration);
	TempBuf.Add(m_UserItem[sDstSlot].sBullNum);
	TempBuf.Add(m_UserItem[sDstSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sDstSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sDstSlot].tIQ); 

	Send(TempBuf, TempBuf.GetLength());
}
int USER::FindItemSid(short sSid)
{
	int i;
	for(i=0;i<60;i++){
		if(convert[i].Ssid==sSid)
			return i;
	}
	return -1;

}
////百级转换
void USER::ItemConvert(TCHAR *pBuf)
{
	int index=0,n;
	int i,count1=0,count2=0;
	short sid;
	short sSrcSlot = GetShort(pBuf, index);//主体
	short sDstSlot = GetShort(pBuf, index);//材料
	if(sSrcSlot < EQUIP_ITEM_NUM || sSrcSlot >= TOTAL_INVEN_MAX) return;
	if(sDstSlot < EQUIP_ITEM_NUM || sDstSlot >= TOTAL_INVEN_MAX) return;
	for(i=EQUIP_ITEM_NUM;i<TOTAL_INVEN_MAX;i++){
		if(1093==m_UserItem[i].sSid)
			break;
	}
	if(i>=TOTAL_INVEN_MAX)
		return;
	if(m_UserItem[sSrcSlot].sSid==-1&&m_UserItem[sDstSlot].sSid==-1)
		return;
	if( g_arItemTable[m_UserItem[sSrcSlot].sSid]->m_byWear !=  g_arItemTable[m_UserItem[sDstSlot].sSid]->m_byWear)
		return ;

/*	//主体
	if(g_arItemTable[sSrcSlot]->m_byWear <= 1 || g_arItemTable[sSrcSlot]->m_byWear >= 6) 
		return;//判断物品类型必须是装备才可以合
	//材料
	if(g_arItemTable[sDstSlot]->m_byWear <= 1 || g_arItemTable[sDstSlot]->m_byWear >= 6) 
		return;//判断物品类型必须是装备才可以合*/

//	if(m_UserItem[sSrcSlot].tMagic[5]!=m_UserItem[sDstSlot].tMagic[5])//主体和材料改数不同不能合
//		return;
	if(m_UserItem[sSrcSlot].tMagic[5]<7) return;
	
	for(i=0;i<4;i++){
		if(m_UserItem[sSrcSlot].tMagic[i]!=0) count1++;
		if(m_UserItem[sDstSlot].tMagic[i]!=0) count2++;
	}
	if(count1!=count2||count1==0)
		return;

	n=FindItemSid(m_UserItem[sDstSlot].sSid);
	if(n==-1)
		return;
	n=FindItemSid(m_UserItem[sSrcSlot].sSid);
	if(n==-1)
		return;
	sid=convert[n].Dsid;
	//金钱1000W
	if(m_dwDN < 10000000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}
	if( m_dwDN <= 10000000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 10000000;
	UpdateUserItemDN();						
	SendMoneyChanged();
	//清除掉冰魂
	RobItem(1093, 1);
//开始转换

	m_UserItem[sSrcSlot].sSid=sid;
	m_UserItem[sSrcSlot].tIQ=0x0c;
	m_UserItem[sSrcSlot].tMagic[5]=m_UserItem[sSrcSlot].tMagic[5]-7;
	int rand = myrand(1, 100)%count1;
	count2=0;
	for(i=0;i<4;i++){
		if(rand==count2){
			m_UserItem[sSrcSlot].tMagic[4]=m_UserItem[sDstSlot].tMagic[i];
		}
		if(m_UserItem[sDstSlot].tMagic[i]!=0)
			count2++;
	}
	ReSetItemSlot(&m_UserItem[sDstSlot]);
	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)3);
	TempBuf.Add((BYTE)2);
	TempBuf.Add((BYTE)sSrcSlot);
	TempBuf.Add(m_UserItem[sSrcSlot].sLevel);
	TempBuf.Add(m_UserItem[sSrcSlot].sSid);
	TempBuf.Add(m_UserItem[sSrcSlot].sDuration);
	TempBuf.Add(m_UserItem[sSrcSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSrcSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSrcSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSrcSlot].tIQ); 
	
	TempBuf.Add((BYTE)sDstSlot);
	TempBuf.Add(m_UserItem[sDstSlot].sLevel);
	TempBuf.Add(m_UserItem[sDstSlot].sSid);
	TempBuf.Add(m_UserItem[sDstSlot].sDuration);
	TempBuf.Add(m_UserItem[sDstSlot].sBullNum);
	TempBuf.Add(m_UserItem[sDstSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sDstSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sDstSlot].tIQ); 

	Send(TempBuf, TempBuf.GetLength());

}

////////////解除属性限制//////////////////////////////////////////////////////////////////////////////////
void USER::RemagicItem_100_3(TCHAR *pBuf)
{
	int index=0,i;
	short sSourceSlot = GetShort(pBuf, index);	
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;
	if( m_UserItem[sSourceSlot].tMagic[5]<3) return;
	if( m_UserItem[sSourceSlot].tIQ!=12)	return;
	if(FindItem(1094) == 0) return;
	if(m_dwDN < 1000000)
	{	SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);		return ;	}
	if(m_UserItem[sSourceSlot].tMagic[10] !=0) return;
	

	for(i = 6; i < 11;i++)
	{		
		if(m_UserItem[sSourceSlot].tMagic[i]==0)
		{
			m_UserItem[sSourceSlot].tMagic[i] = 1;
			//break;
		}	
	}
	
	if( m_dwDN <= 1000000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 1000000;

	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();
	//清除掉消融之星
	RobItem(1094, 1);
	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)sSourceSlot);
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 
	Send(TempBuf, TempBuf.GetLength());

}
//防具材料化
void USER::RemagicItem_100_1(TCHAR *pBuf)
{
	int index=0;
	short sSourceSlot = GetShort(pBuf, index);	
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;
	if( m_UserItem[sSourceSlot].tMagic[5]<7){
		return;
	}
	if(m_UserItem[sSourceSlot].tIQ!=1&&m_UserItem[sSourceSlot].tIQ!=3&&m_UserItem[sSourceSlot].tIQ!=2)
		return ;
	//////////////////////////////////////////////////////////////
	if (m_UserItem[sSourceSlot].sSid < 0 || m_UserItem[sSourceSlot].sSid >= g_arItemTable.GetSize()) return;

	if (g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_byWear < 2 || g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_byWear > 5 ) return;
	////////////////////////////////////////////////////////
	if(m_dwDN < 100000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}

	if( m_dwDN <= 100000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 100000;

	m_UserItem[sSourceSlot].tMagic[6]=2;

	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();

	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)9);
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)sSourceSlot);
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 
	Send(TempBuf, TempBuf.GetLength());

}
//解除防具材料化
void USER::RemagicItem_100_2(TCHAR *pBuf)
{
	int index=0;
	short sSourceSlot = GetShort(pBuf, index);	
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;
	if( m_UserItem[sSourceSlot].tMagic[5]<7){
		return;
	}
	if(m_UserItem[sSourceSlot].tIQ!=1&&m_UserItem[sSourceSlot].tIQ!=3&&m_UserItem[sSourceSlot].tIQ!=2)
		return ;
	//////////////////////////////////////////////////////////////////////////
	if (m_UserItem[sSourceSlot].sSid < 0 || m_UserItem[sSourceSlot].sSid >= g_arItemTable.GetSize()) return;

	if (g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_byWear < 2 || g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_byWear > 5 ) return;

	///////////////////////////////////////////////////////////////////////////
	if(m_dwDN < 100000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}

	if( m_dwDN <= 100000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 100000;

	m_UserItem[sSourceSlot].tMagic[6]=0;

	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();

	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)10);
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)sSourceSlot);
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 
	Send(TempBuf, TempBuf.GetLength());
}
void USER::UpgradeItemReq(TCHAR *pBuf)
{
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1 )
	{
        SendSystemMsg( "请先解除密码保护后在操作", SYSTEM_ERROR, TO_ME);
		return;
	}


	int iSid = -1;
	int iIQ = -1;
	int iCount = 0, iThings = 0;
	int i, j, index = 0;
	int iWeight = 0;
	short sMaterialSlot;

	int iSuccess = 0;

	BYTE tIQ = 0, bySlot = 0;

	CBufferEx TempBuf;

	ItemList	MyItem[TOTAL_ITEM_NUM];

	CByteArray arMaterial;
	arMaterial.RemoveAll();

	short sSourceSlot = GetShort(pBuf, index);			// 急琶茄 酒捞袍 浇吩锅龋

	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;	// 牢亥俊辑父 诀弊饭捞靛 啊瓷

	iSid = m_UserItem[sSourceSlot].sSid;
	if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;
	if( iSid == 669 || iSid == 670 )					// 诀弊饭捞靛 窍妨绰 酒捞袍捞 臂矾弊辆幅扼搁
	{
		SendSystemMsg( IDS_USER_CANT_UPGRADE_ITEM, SYSTEM_ERROR, TO_ME);
		return;
	}
	//if(g_arItemTable[iSid]->m_byWear > 5 && g_arItemTable[iSid]->m_byWear < 9)
	if(g_arItemTable[iSid]->m_byWear > 6 )
		return ;



	iIQ = m_UserItem[sSourceSlot].tIQ;
	switch(iIQ)
	{
	case NORMAL_ITEM:
		break;
	case MAGIC_ITEM:
		iCount = 1;
		iThings = MATERIAL_MAGIC_UP_ITEM;
		break;
	case RARE_ITEM:
		iCount = 1;
		iThings = MATERIAL_RARE_UP_ITEM;
		break;
	case REMODEL_ITEM:
	case REMODEL_MAGIC_ITEM:
		iCount = 2;
		iThings = MATERIAL_REMODEL_UP_ITEM;
		break;
	case 12: //普通百级
		iCount = 1;
		iThings = MATERIAL_RARE_UP_ITEM;
		break;
	case 18: //130套
		iCount = 1;
		iThings = MATERIAL_RARE_UP_ITEM;
		break;
	default:

		return;
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++)			// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}
	for(i = 0; i < iCount; i++)
	{
		sMaterialSlot = -1;
		sMaterialSlot = GetShort(pBuf, index);			// 概流加己 1俺 嘿篮 犁丰 ( 绝栏搁 -1 )
		if(sMaterialSlot >= EQUIP_ITEM_NUM && sMaterialSlot < TOTAL_INVEN_MAX)
		{												// 犁丰吝俊 秦寸 wear沥焊啊 促福搁 角菩		
			iSid = MyItem[sMaterialSlot].sSid;
			if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;
			if(iSid != iThings) return;			
			if(MyItem[sMaterialSlot].sCount <= 0) return;

			iWeight += g_arItemTable[iSid]->m_byWeight;

			MyItem[sMaterialSlot].sCount -= 1;
			arMaterial.Add((BYTE)sMaterialSlot);
		}
		else
		{
			SendSystemMsg(IDS_USER_SHENGSHI, SYSTEM_ERROR, TO_ME);
			return;
		}
	}


	iSuccess = SetUpgeadeItem(sSourceSlot);	
	if(iSuccess == 0) iWeight += g_arItemTable[iSid]->m_byWeight;			// 公霸函拳甫 眉农茄促.
	if(iSuccess == -1) return;

	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.

	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = 1 + arMaterial.GetSize();

	if(!iSuccess)	TempBuf.Add((BYTE)0x00); //角菩
	else			TempBuf.Add((BYTE)0x01);
	
	SendMoneyChanged();

	TempBuf.Add((BYTE)index);

	TempBuf.Add((BYTE)sSourceSlot);					// 林 酒捞袍甫 刚历 	
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 

	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		bySlot = (BYTE)arMaterial[i];
		
		if(m_UserItem[bySlot].sCount <= 1) ReSetItemSlot(&m_UserItem[bySlot]);// 家厚己 捞骨肺 刚历 檬扁拳窍绊 
		else							   m_UserItem[bySlot].sCount -= 1;

		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...

	Send(TempBuf, TempBuf.GetLength());

	arMaterial.RemoveAll();
}

int USER::SetUpgeadeItem(short sSlot)
{
	BYTE tCount = 0;

	int iSuccess = 0;

	int temp = 0;
	int last = 0;
	int rand = 0;

	if(m_UserItem[sSlot].sSid < 0 || m_UserItem[sSlot].sSid >= g_arItemTable.GetSize()) return -1;
	if(g_arItemTable[m_UserItem[sSlot].sSid]->m_sDuration <= 0) return -1;
	if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byWear > 6 )
		return -1;
	tCount = m_UserItem[sSlot].tMagic[5];
	if(tCount >= MAX_ITEM_UPGRADE_COUNT) return -1;

	if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byWear == ATTACK_ITEM)
	{
		if(m_dwDN < ATTACK_UPGRADE_COST)
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
			return -1;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
		}

		if( m_dwDN <= ATTACK_UPGRADE_COST ) m_dwDN = 0;
		else m_dwDN = m_dwDN - ATTACK_UPGRADE_COST;			// 秦寸 厚侩甫 搬苞俊 惑包绝捞 固府 拌魂

		if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byClass == STAFF_ITEM)
		{
			last = ATTACK_UPGRADE_PSI_BAND;					// 诀弊饭捞靛 己傍矫 吝啊登绰 蔼
		}
		else
		{
			last = ATTACK_UPGRADE_BAND;						// 诀弊饭捞靛 己傍矫 吝啊登绰 蔼
		}


		rand = myrand(1, 10000);							// 己傍伏苞 厚背困秦 罚待荐甫 备窃
		if(rand <= g_ItemAttUpgrade[tCount]) iSuccess = 1;// 秦寸 锅掳狼 己傍伏焊促 累栏搁
		if(tCount < 6)					// 幸运星 
		{
			int dwTemp = FindItem(1043);
			if (dwTemp >= 1)
			{
				RobItem(1043,1);//减少一个物品
				SendSystemMsg("成功使用幸运星", SYSTEM_ERROR, TO_ME);
				iSuccess = 1;
			}            
		}

	}
	else
	{
		if(m_dwDN < DEFENSE_UPGRADE_COST)
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
			return -1;		// 规绢侩 诀弊饭捞靛 厚侩焊促 累栏搁 
		}

		if( m_dwDN <= DEFENSE_UPGRADE_COST ) m_dwDN = 0;
		else m_dwDN = m_dwDN - DEFENSE_UPGRADE_COST;				// 秦寸 厚侩甫 搬苞俊 惑包绝捞 固府 拌魂

		last = DEFENSE_UPGRADE_BAND;						// 规绢侩 裹困

//		if(tCount < 6)					// 烙矫肺 6诀焊促 农搁 公炼扒 角菩 02-05-04 by Youn Gyu
//		{
		rand = myrand(1, 10000);	
		if(m_UserItem[sSlot].tIQ==12){
			if(rand <= g_ItemDefUpgrade_[tCount]) 
				iSuccess = 1;
		}else{
			if(rand <= g_ItemDefUpgrade[tCount]) 
				iSuccess = 1;
			if(tCount < 6)					// 幸运星 
			{
				int dwTemp = FindItem(1043);
				if (dwTemp >= 1)
				{
					RobItem(1043,1);//减少一个物品
					SendSystemMsg("成功使用幸运星", SYSTEM_ERROR, TO_ME);
					iSuccess = 1;
				}            				
			}
		}
	}

	if(iSuccess == 1)						// 秦寸 锅掳狼 己傍伏焊促 累栏搁
	{	
		m_UserItem[sSlot].tMagic[5] = tCount + 1;	// 付瘤阜 浇吩俊 己傍冉荐甫 刘啊 矫挪促.
		m_UserItem[sSlot].tMagic[4] = (BYTE)((tCount + 1) * last);
		
		MakeItemLog( &m_UserItem[sSlot], ITEMLOG_UPGRADE_SUCCESS );
	}
	else
	{ 
		MakeItemLog( &m_UserItem[sSlot], ITEMLOG_UPGRADE_FAIL );
		ReSetItemSlot(&m_UserItem[sSlot]);	iSuccess = 0; 
	}	
	if(m_UserItem[sSlot].tMagic[5]>6)
	{
		CString sysMsg;
 		sysMsg.Format("玩家：%s将物品【%s】强化到【%d改】",this->m_strUserID,g_arItemTable[m_UserItem[sSlot].sSid]->m_strName,m_UserItem[sSlot].tMagic[5]);
        m_pCom->Announce((LPTSTR)(LPCTSTR)sysMsg, SYSTEM_ANNOUNCE);
	}

	FlushItemLog( TRUE );
	return iSuccess;
}

////////////////////////////////////////////////////////////////////////////////////////
//	首饰升级
///////////////////////////////////////////////////////////////////////////////////////
void USER::UpgradeAccessoriReq(TCHAR *pBuf)
{
	if(m_dwDN < ACCESSORI_UPGRADE_COST) 
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return;		// 诀弊饭捞靛 且 捣捞 绝栏搁 府畔
	}
	//////////////////////////交易密码
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
	{
        SendSystemMsg( "请先解除密码保护后在操作", SYSTEM_ERROR, TO_ME);
		return;
	}
	/////////////////// 

//	int iSid = 0;
//	int iIQ = -1;
	int iSid = -1;
	int iCount = 0;
	int i, j, index = 0;
	int iWeight = 0;
	short sMaterialSlot[4] = {-1, -1, -1, -1};
	short sSid[4] = {-1, -1, -1, -1};
	short sAid = 0;
	short sMagicSid = 0;
	int iSuccess = 0;
	BYTE bySlot = 0;

	int	iRandom, iMagicRandom, iCrestRandom, iCrestCount = 0;
	int iUpgradeCount = 0, iAllSkill = 0;
	BOOL bUseCrest = 0;

	CByteArray arMaterial;
	arMaterial.RemoveAll();

	short sAccessori = -1, sRipel = -1;				// 5诀 捞惑 傅俊 措秦辑 霖厚茄促.

	short sSlot = GetShort(pBuf, index);			// 急琶茄 酒捞袍 浇吩锅龋
//	if(sSlot != -1) return;							// 咀技辑府绰 家胶啊 绝促.
	if(sSlot != -1)									// 5诀捞惑牢瘤
	{
		short sTemp = -1;
		if(sSlot < EQUIP_ITEM_NUM || sSlot >= TOTAL_INVEN_MAX) return;
		sTemp = m_UserItem[sSlot].sSid;
		if(sTemp < 0 || sTemp >= g_arItemTable.GetSize()) return;

		if(g_arItemTable[sTemp]->m_byWear <= 5 || g_arItemTable[sTemp]->m_byWear >= 9) return;

		if(m_UserItem[sSlot].tMagic[5] <= 4) 
		{
			SendSystemMsg(IDS_USER_ACCESSORI_INVALID, SYSTEM_ERROR, TO_ME);	// 5诀 捞窍狼 酒捞袍篮 府歧狼炼阿, 巩厘栏肺 诀弊饭捞靛 窍瘤 臼绰促.
			return;		
		}

        if( iSid == 818 || iSid == 733 || iSid == 735 )  return;
		if(m_UserItem[sSlot].tMagic[5] > 7) return; // 且 荐乐绰 诀弊饭捞靛绰 捞固 促沁促.

		if(m_UserItem[sSlot].sDuration == 0)
		{
			SendSystemMsg( IDS_USER_ACCESSORI_OLD, SYSTEM_ERROR, TO_ME);
			return;
		}

		sSid[0] = sTemp;
		sAccessori = sSlot;

		sSlot = -1, sTemp = -1;
		sSlot = GetShort(pBuf, index);
		if(sSlot == -1) return;
		if(sSlot < EQUIP_ITEM_NUM || sSlot >= TOTAL_INVEN_MAX)	return;

		sTemp = m_UserItem[sSlot].sSid;
			if(sTemp != 798 && sTemp != 799 && sTemp != 800 && sTemp != 801 && sTemp != 802 )
					return;
		if(sTemp < 0 || sTemp >= g_arItemTable.GetSize()) 
		{
			SendSystemMsg(IDS_USER_ACCESSORI_RIPEL, SYSTEM_ERROR, TO_ME);	// 府歧狼 炼阿捞唱 巩厘阑 啊廉 坷贰滴.
			return;
		}

		sSid[1] = sTemp;
		sRipel = sSlot;

		iCount = 100;							// 5诀臂 捞惑 傅俊 措秦辑 100锅阑 何咯茄促.
	}
	else
	{	
		int magic_slot = -1;
		int dynamic_slot = -1;

		for(i = 0; i < 4; i++)							
		{
			sMaterialSlot[i] = GetShort(pBuf, index);
			if(sMaterialSlot[i] <= -1) break;
			if(sMaterialSlot[i] < EQUIP_ITEM_NUM || sMaterialSlot[i] >= TOTAL_INVEN_MAX) return;
			sSid[i] = m_UserItem[sMaterialSlot[i]].sSid;
			if(sSid[i] == -1)//出错
				return ;
			if(g_arItemTable[sSid[i]]->m_byWear <= 5 || g_arItemTable[sSid[i]]->m_byWear >= 9) 
				if(sSid[i] != 798 || sSid[i] != 799 || sSid[i] != 800 || sSid[i] != 801 )
					return;

			if(sSid[i] < 0 || sSid[i] >= g_arItemTable.GetSize()) return;
			if(sSid[i] == SID_RING_OF_LIFE || sSid[i] == SID_NECKLACE_OF_SHIELD || sSid[i] == SID_EARRING_OF_PROTECT || sSid[i] == 755)
			{
				SendSystemMsg(IDS_USER_ACCESSORI_SKILLUP, SYSTEM_ERROR, TO_ME);	// 诀弊饭捞靛 且 荐 绝绰 厩技荤府
				return;
			}

			magic_slot = -1;
			dynamic_slot = -1;
			magic_slot = m_UserItem[sMaterialSlot[i]].tMagic[0];
			if(magic_slot < 0 || magic_slot >= g_arMagicItemTable.GetSize()) return;

			dynamic_slot = g_arMagicItemTable[magic_slot]->m_sSubType;	
			if(dynamic_slot < 0 || dynamic_slot >= MAGIC_COUNT) return;
			
			if(dynamic_slot == MAGIC_ALL_SKILL_UP)	// 剖乔柯狼傅 诀弊饭捞靛 阜扁
			{
				SendSystemMsg( IDS_USER_ACCESSORI_SKILLUP, SYSTEM_ERROR, TO_ME);				
				return;
			}

			iCount++;
		}

		for(i = 0; i < iCount; i++)
		{
			if(g_arItemTable[sSid[i]]->m_byWear >= 6 && g_arItemTable[sSid[i]]->m_byWear <= 8)
			{
				if(m_UserItem[sMaterialSlot[i]].sDuration == 0)
				{
					SendSystemMsg( IDS_USER_ACCESSORI_OLD, SYSTEM_ERROR, TO_ME);
					return;
				}
			}
		}
	}

	switch(iCount)
	{
	case 100:
/*		if(m_UserItem[sMaterialSlot[0]].tMagic[5] <= 4) 
		{
			SendSystemMsg(IDS_USER_ACCESSORI_INVALID, SYSTEM_ERROR, TO_ME);	// 5诀 捞窍狼 酒捞袍篮 府歧狼炼阿, 巩厘栏肺 诀弊饭捞靛 窍瘤 臼绰促.
			return;		
		}
		if(m_UserItem[sMaterialSlot[0]].tMagic[5] > 7) return; // 且 荐乐绰 诀弊饭捞靛绰 捞固 促沁促.
*/
/*		if(g_arItemTable[sSid[0]]->m_byWear >= 6 && g_arItemTable[sSid[0]]->m_byWear <= 8)
		{
			sAccessori = sMaterialSlot[0];
			sRipel = sMaterialSlot[1];
		}
		else
		{
			sAccessori = sMaterialSlot[1];
			sRipel = sMaterialSlot[0];
			sSid[3] = sSid[1];
			sSid[1] = sSid[0];
			sSid[0] = sSid[3];
		}
*/
		if(sAccessori == -1 || sRipel == -1) return;

		iRandom = myrand(1, 10000);
		iUpgradeCount = m_UserItem[sAccessori].tMagic[5] - 5;
		switch(m_UserItem[sRipel].sSid)
		{
		case RIPEL_TOP:
			if(iRandom <= g_ItemAccessoriUpgrade[iUpgradeCount])	// Upgrade Success;
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelTop.GetSize() - 1);				
				sMagicSid = g_Ripel.m_arRipelTop[iMagicRandom];
				iSuccess = 1;
			}
			else ReSetItemSlot(&m_UserItem[sAccessori]);		// Ugrade Fail

			break;
		case RIPEL_BOTTOM:
			if(iRandom <= g_ItemAccessoriUpgrade[iUpgradeCount])	// Upgrade Success;
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelBottom.GetSize() - 1);				
				sMagicSid = g_Ripel.m_arRipelBottom[iMagicRandom];
				iSuccess = 1;
			}
			else ReSetItemSlot(&m_UserItem[sAccessori]);		// Ugrade Fail

			break;
		case RIPEL_LEFT:
			if(iRandom <= g_ItemAccessoriUpgrade[iUpgradeCount])	// Upgrade Success;
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelLeft.GetSize() - 1);				
				sMagicSid = g_Ripel.m_arRipelLeft[iMagicRandom];
				iSuccess = 1;
			}
			else ReSetItemSlot(&m_UserItem[sAccessori]);		// Ugrade Fail

			break;
		case RIPEL_RIGHT:
			if(iRandom <= g_ItemAccessoriUpgrade[iUpgradeCount])	// Upgrade Success;
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelRight.GetSize() - 1);				
				sMagicSid = g_Ripel.m_arRipelRight[iMagicRandom];
				iSuccess = 1;
			}
			else ReSetItemSlot(&m_UserItem[sAccessori]);		// Ugrade Fail

			break;
		case RIPEL_CREST:
			if(m_UserItem[sAccessori].tMagic[5] != 5 || m_UserItem[sAccessori].sSid==755)
			{
				SendSystemMsg(IDS_USER_ACCESSORI_CREST, SYSTEM_ERROR, TO_ME);	// 府歧狼巩厘栏肺绰 5诀狼 咀技辑府父 诀弊饭捞靛 且 荐乐嚼聪促.
				return;
			}

			bUseCrest = TRUE;
			if(iRandom <= g_ItemAccessoriUpgrade[2])	// Upgrade Success;
			{
				while(1)
				{
				iCrestRandom = myrand(1, 5);
				if(iCrestRandom == 1) 
				{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelTop.GetSize() - 1); 
				sMagicSid = g_Ripel.m_arRipelTop[iMagicRandom];
				m_UserItem[sAccessori].tMagic[iCrestCount + 1] = (BYTE)sMagicSid;
				iCrestCount++;
				}
			if(iCrestRandom == 2) 
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelBottom.GetSize() - 1); 
				sMagicSid = g_Ripel.m_arRipelBottom[iMagicRandom];
				m_UserItem[sAccessori].tMagic[iCrestCount + 1] = (BYTE)sMagicSid;
				iCrestCount++;
			}
			if(iCrestRandom == 3) 
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelLeft.GetSize() - 1); 
				sMagicSid = g_Ripel.m_arRipelLeft[iMagicRandom];
				m_UserItem[sAccessori].tMagic[iCrestCount + 1] = (BYTE)sMagicSid;
				iCrestCount++;
			}
			if(iCrestRandom == 4) 
			{
				iMagicRandom = myrand(0, g_Ripel.m_arRipelRight.GetSize() - 1); 
				sMagicSid = g_Ripel.m_arRipelRight[iMagicRandom];
				m_UserItem[sAccessori].tMagic[iCrestCount + 1] = (BYTE)sMagicSid;
				iCrestCount++;
			}
			if(iCrestRandom == 5 && iAllSkill == 0) //所有1技能
			{
				iAllSkill = 1;
				iMagicRandom = myrand(0, g_Ripel.m_arRipelCrest.GetSize() - 1);
				sMagicSid = g_Ripel.m_arRipelCrest[iMagicRandom];
				m_UserItem[sAccessori].tMagic[iCrestCount + 1] = (BYTE)sMagicSid;
				iCrestCount++;
			}

					if(iCrestCount >= 3) break;
				}

				m_UserItem[sAccessori].tMagic[5] += 3;
				m_UserItem[sAccessori].tIQ = RARE_ITEM;
				iSuccess = 1;
				if(m_UserItem[sAccessori].tMagic[1]==137 || m_UserItem[sAccessori].tMagic[2] ==137 || m_UserItem[sAccessori].tMagic[3] ==137)//大于或等于8改提示
		       {
                    CString sysMsg;
                    sysMsg.Format("玩家 %s 将[%s]合成1技能首饰 ! ",this->m_strUserID,g_arItemTable[m_UserItem[sAccessori].sSid]->m_strName );
                    SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		       }
			}
			else
			{
				int K = 0;
				BYTE m_Slot = -1;
				for(m_Slot = 10; m_Slot < 34; m_Slot++)
				{	
					if(m_UserItem[m_Slot].sSid == 870)
					{
						if(m_UserItem[m_Slot].tIQ > 0)
						{
					      if(m_UserItem[m_Slot].sCount <= 1)
				              ReSetItemSlot(&m_UserItem[m_Slot]);
			              else m_UserItem[m_Slot].sCount -= 1;//钻石颜色大于0的也删除
						  SendSystemMsg("注意:非法[钻石],已删除!请立即更换正常[钻石]", SYSTEM_ERROR, TO_ME);
						}
						K = TRUE;
						break;
					}
					else
					{
						K = FALSE;
					}
				}
				if(K == TRUE)
				{
					RobItem(870,1);	//删除白钻石
					SendSystemMsg("钻石保护,首饰不消失!", SYSTEM_ERROR, TO_ME);
				}
				else
				{
					ReSetItemSlot(&m_UserItem[sAccessori]);		// Ugrade Fail
                }
			  
			}
			break;
		default:
			SendSystemMsg(IDS_USER_ACCESSORI_RIPEL, SYSTEM_ERROR, TO_ME);	// 府歧狼 炼阿捞唱 巩厘阑 啊廉 坷贰滴.
			return;		
		}

		if(bUseCrest == FALSE && iSuccess == 1)
		{
			m_UserItem[sAccessori].tMagic[5]++;
			if(iUpgradeCount == 0) m_UserItem[sAccessori].tMagic[1] = (BYTE)sMagicSid;
			if(iUpgradeCount == 1) m_UserItem[sAccessori].tMagic[2] = (BYTE)sMagicSid;
			if(iUpgradeCount == 2) m_UserItem[sAccessori].tMagic[3] = (BYTE)sMagicSid;
			m_UserItem[sAccessori].tIQ = RARE_ITEM;
			
		}

		if(iSuccess == 0)
		{
			iWeight = g_arItemTable[sSid[0]]->m_byWeight + g_arItemTable[sSid[1]]->m_byWeight;
		}
		else if(iSuccess == 1)
		{
			iWeight = g_arItemTable[sSid[1]]->m_byWeight;
		}

		if(m_UserItem[sRipel].sCount <= 1) ReSetItemSlot(&m_UserItem[sRipel]);
		else							   m_UserItem[sRipel].sCount -= 1;
		
		if(m_dwDN > ACCESSORI_UPGRADE_COST) m_dwDN -= ACCESSORI_UPGRADE_COST;
		else m_dwDN = 0;

		arMaterial.Add((BYTE)sAccessori);
		arMaterial.Add((BYTE)sRipel);

		break;
	case 3:
		if(g_arItemTable[sSid[0]]->m_byWear < 6 || g_arItemTable[sSid[0]]->m_byWear > 8) return;	// 诀弊饭捞靛 窍妨绰 酒捞袍捞 咀技辑府啊 酒聪搁 府畔
		if(g_arItemTable[sSid[1]]->m_byWear < 6 || g_arItemTable[sSid[1]]->m_byWear > 8) return;	// 诀弊饭捞靛 窍妨绰 酒捞袍捞 咀技辑府啊 酒聪搁 府畔
		if(g_arItemTable[sSid[2]]->m_byWear < 6 || g_arItemTable[sSid[2]]->m_byWear > 8) return;	// 诀弊饭捞靛 窍妨绰 酒捞袍捞 咀技辑府啊 酒聪搁 府畔

		if(m_UserItem[sMaterialSlot[0]].tMagic[5] > 4) return;	

		if(m_UserItem[sMaterialSlot[0]].tMagic[5] != m_UserItem[sMaterialSlot[1]].tMagic[5] || 
		   m_UserItem[sMaterialSlot[0]].tMagic[5] != m_UserItem[sMaterialSlot[2]].tMagic[5] ||
		   m_UserItem[sMaterialSlot[1]].tMagic[5] != m_UserItem[sMaterialSlot[2]].tMagic[5] ) return;	// 鞍篮 诀弊饭捞靛 荐啊 酒聪搁 府畔

		if(m_UserItem[sMaterialSlot[0]].tMagic[0] != m_UserItem[sMaterialSlot[1]].tMagic[0] || 
		   m_UserItem[sMaterialSlot[0]].tMagic[0] != m_UserItem[sMaterialSlot[2]].tMagic[0] ||
		   m_UserItem[sMaterialSlot[1]].tMagic[0] != m_UserItem[sMaterialSlot[2]].tMagic[0] ) return;	// 鞍篮 瓷仿摹狼 酒捞袍捞 酒聪搁 府畔


		iWeight += (g_arItemTable[sSid[0]]->m_byWeight + g_arItemTable[sSid[1]]->m_byWeight);

		sMagicSid = m_UserItem[sMaterialSlot[0]].tMagic[0];

		sAid = g_arMagicItemTable[sMagicSid]->m_sAid;

		if(sAid + 1 >= g_arAccessoriUpTable.GetSize()) return;

		m_UserItem[sMaterialSlot[0]].tMagic[0] = g_arAccessoriUpTable[sAid + 1]->m_sSid;

		ReSetItemSlot(&m_UserItem[sMaterialSlot[1]]);
		ReSetItemSlot(&m_UserItem[sMaterialSlot[2]]);

		if(m_UserItem[sMaterialSlot[0]].tMagic[5] == 0) m_UserItem[sMaterialSlot[0]].tMagic[5] = 2;
		else m_UserItem[sMaterialSlot[0]].tMagic[5]++;

		if(m_dwDN > ACCESSORI_UPGRADE_COST) m_dwDN -= ACCESSORI_UPGRADE_COST;
		else m_dwDN = 0;

		arMaterial.Add((BYTE)sMaterialSlot[0]);
		arMaterial.Add((BYTE)sMaterialSlot[1]);
		arMaterial.Add((BYTE)sMaterialSlot[2]);

		iSuccess = 1;

		break;
	default:
		return ;
	}

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...
	
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();

	CBufferEx TempBuf;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	if(iSuccess == 1)	TempBuf.Add(SUCCESS);
	else				TempBuf.Add(FAIL);
	TempBuf.Add((BYTE)arMaterial.GetSize());

	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		bySlot = (BYTE)arMaterial[i];
		
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

	arMaterial.RemoveAll();
}
//////////////////////////////////////////
//黄金灵  金石
void USER::UpgradeHJItemReq(TCHAR *pBuf)
{
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
	{
        SendSystemMsg( "请先解除密码保护后在操作", SYSTEM_ERROR, TO_ME);
		return;
	}
	int iSid = -1;
	int iIQ = -1;
	int iCount = 0, iThings = 0;
	int i, j, index = 0;
	int iWeight = 0;
	short sMaterialSlot;
	int isMagic = 0;

	int iSuccess = 0;

	BYTE tWear = 0;
	BYTE tIQ = 0, bySlot = 0;

	CBufferEx TempBuf;

	ItemList	MyItem[TOTAL_ITEM_NUM];

	CByteArray arMaterial;
	arMaterial.RemoveAll();
	sMaterialSlot = GetShort(pBuf, index);					//黄金石头
	short sSourceSlot = GetShort(pBuf, index);				// 装备


	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;

	if(m_dwDN < 5000000)
	{
		SendSystemMsg( "决战币不足!使用黄金石头升级需500万", SYSTEM_ERROR, TO_ME);
		return ;		 
	}
    iSid = m_UserItem[sSourceSlot].sSid;
	if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;
	if( iSid == 669 || iSid == 670 )						
	{
		SendSystemMsg( IDS_USER_CANT_UPGRADE_ITEM, SYSTEM_ERROR, TO_ME);
		return;
	}	
	if(g_arItemTable[iSid]->m_byWear >= 6 && (g_arItemTable[iSid]->m_byWear!=122) && (g_arItemTable[iSid]->m_byWear!=123) 
		&& (g_arItemTable[iSid]->m_byWear!=124) && (g_arItemTable[iSid]->m_byWear!=125))
		return;
	if ( m_UserItem[sSourceSlot].tMagic[5] != 6)
	{
		SendSystemMsg( "只能改6改道具", SYSTEM_ERROR, TO_ME);
		return;
	}
	iIQ = m_UserItem[sSourceSlot].tIQ;
	
	
	for(i = 0; i < TOTAL_ITEM_NUM; i++)			// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

									
	

	if(sMaterialSlot >= EQUIP_ITEM_NUM && sMaterialSlot < TOTAL_INVEN_MAX)
	{
		iSid = MyItem[sMaterialSlot].sSid;
		/////////////////////黄金武器BUG
		if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return; //防止无赖打我主程序
		tWear = g_arItemTable[iSid]->m_byWear;
		int sSourcebyWear =  g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_byWear;
		if(tWear != BLESSING_WEAPONLESS_WEAR && tWear != BLESSING_ARMORLESS_WEAR) return;

		if((tWear == BLESSING_WEAPONLESS_WEAR) && (sSourcebyWear !=1)) return;
		if((tWear == BLESSING_ARMORLESS_WEAR) && (sSourcebyWear ==1)) return;
           ///////////////////////////////////////////////////////////////


		if(MyItem[sMaterialSlot].sCount <= 0) return;

		iWeight += g_arItemTable[iSid]->m_byWeight;

		MyItem[sMaterialSlot].sCount -= 1;
		arMaterial.Add((BYTE)sMaterialSlot);
	}
	else return;
	if( m_dwDN <= 5000000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 5000000;
	UpdateUserItemDN();						
	SendMoneyChanged();


	int random = myrand(1,10000);          //黄金石头升级成功率
	if(random <= 10000 && random >=9900)			//+4改
	{
		m_UserItem[sSourceSlot].tMagic[5] +=4;
		if (m_UserItem[sSourceSlot].tMagic[5] > 10)
			m_UserItem[sSourceSlot].tMagic[5] = 10;
		iSuccess = 1;
	}
	else if (random < 9900 && random >=8500)
	{
		m_UserItem[sSourceSlot].tMagic[5] +=3;
		if (m_UserItem[sSourceSlot].tMagic[5] > 10)
			m_UserItem[sSourceSlot].tMagic[5] = 10;
		iSuccess = 1;
	}
	else if (random < 8500 && random >=6000)
	{
		m_UserItem[sSourceSlot].tMagic[5] +=2;
		if (m_UserItem[sSourceSlot].tMagic[5] > 10)
			m_UserItem[sSourceSlot].tMagic[5] = 10;
		iSuccess = 1;
	}
	else if (random < 6000 && random >=0)
	{
		m_UserItem[sSourceSlot].tMagic[5] +=1;
		if (m_UserItem[sSourceSlot].tMagic[5] > 10)
			m_UserItem[sSourceSlot].tMagic[5] = 10;
		iSuccess = 1;
	}
	else
	{
		iSuccess = 0;
	}
	if(iSuccess = 1)//大于或等于8改提示
		{
         CString sysMsg;
         sysMsg.Format("玩家 %s 用黄金石头将[%s]强化为 %d 改 ! ",this->m_strUserID,g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_strName ,m_UserItem[sSourceSlot].tMagic[5]);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
	
	//if(m_UserItem[sSourceSlot].tMagic[5] == 10)//外观 
	//{
	//	int iFindAttrId = 0;
	//	switch(m_UserItem[sSourceSlot].sSid)
	//	{
	//         case 323: //反1
	//	         iFindAttrId = 1216;//毁灭之刃I型
	//	         break;
	//         case 440: //反2
	//		 case 441: //反2
	//		 case 442: //反2
	//	         iFindAttrId = 1219;//毁灭之刃II型
	//	         break;
	//         case 443: //反3
	//		 case 444: //反3
	//	         iFindAttrId = 1220;//毁灭之刃III型
	//	         break;
	//         case 330: //龙神之怒
	//	         iFindAttrId = 1222;//灵厄权杖
	//	         break;
 //          	 case 475: //强化龙神之怒
	//		 case 476: //强化龙神之怒
	//		 case 477: //强化龙神之怒
	//	         iFindAttrId = 1225;//强化灵厄权杖
	//	         break;
	//         case 478: //龙神之怒—S
	//		 case 479: //龙神之怒—S
	//	         iFindAttrId = 1226;//灵厄权杖-S
	//	         break;
	//		 case 316: //龙神炮GX2
	//	         iFindAttrId = 1228;//风暴光炮GX2
	//	         break;
 //          	 case 405: //龙神炮GX4
	//		 case 406: //龙神炮GX4
	//		 case 407: //龙神炮GX4
	//	         iFindAttrId = 1231;//风暴光炮GX4
	//	         break;
	//         case 408: //龙神炮GX6
	//		 case 409: //龙神炮GX6
	//	         iFindAttrId = 1232;//风暴光炮GX6
	//	         break;
	//		 case 337: //龙之翼
	//	         iFindAttrId = 1234;//庇护之翼
	//	         break;
	//         case 510: //强化龙之翼
	//		 case 511: //强化龙之翼
	//		 case 512: //强化龙之翼
	//	         iFindAttrId = 1238;//强化庇护之翼
	//	         break;
	//         case 513: //龙之翼—S
	//		 case 514: //龙之翼—S
	//	         iFindAttrId = 1239;//庇护之翼-S
	//	         break;
 //            default:
	//				return;
	//	}
	//	m_UserItem[sSourceSlot].sSid = iFindAttrId;//开始ID变化						
	//}


	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = 1 + arMaterial.GetSize();

	if(!iSuccess)	TempBuf.Add((BYTE)0x00); //角菩
	else			TempBuf.Add((BYTE)0x01);

	TempBuf.Add((BYTE)index);

	TempBuf.Add((BYTE)sSourceSlot);					// 林 酒捞袍甫 刚历 	
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 

	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		bySlot = (BYTE)arMaterial[i];

		MakeItemLog( &m_UserItem[bySlot], ITEMLOG_BLESS_USE );
		FlushItemLog( TRUE );

		if(m_UserItem[bySlot].sCount <= 1) ReSetItemSlot(&m_UserItem[bySlot]);// 家厚己 捞骨肺 刚历 檬扁拳窍绊 
		else							   m_UserItem[bySlot].sCount -= 1;

		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...

	Send(TempBuf, TempBuf.GetLength());

	arMaterial.RemoveAll();			

			
}
//机械材料
void USER::EbodyMatrial(int sSourceSlot)								
{
	//判断位置
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX)
		return;
	short sSid = m_UserItem[sSourceSlot].sSid;
	if(sSid <0||sSid>g_arItemTable.GetSize())
		return;
	if(!IsEbodyItem(sSid))
		return;	
	if( m_UserItem[sSourceSlot].tMagic[5]<7)
	{
		return;
	}
	if(m_UserItem[sSourceSlot].tIQ!=2)
		return ;

	if(m_dwDN < 200000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}

	if( m_dwDN <= 200000 ) 
		m_dwDN = 0;
	else 
		m_dwDN = m_dwDN - 200000;

	m_UserItem[sSourceSlot].tMagic[MAGIC_100_ADD_POS]=1;

	//m_UserItem[sSourceSlot].tIQ = SUPER_EBODY_ITEM;
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();

	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)9);
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)sSourceSlot);
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) 
		TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 
	Send(TempBuf, TempBuf.GetLength());
}

//机械取消材料化
void USER::EbodyCancelMetral(int sSourceSlot)								
{
	//判断位置
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX)
		return;
	short sSid = m_UserItem[sSourceSlot].sSid;
	if(sSid <0||sSid>g_arItemTable.GetSize())
		return;
	if(!IsEbodyItem(sSid))
		return;	
	if( m_UserItem[sSourceSlot].tMagic[5]<7)
	{
		return;
	}
	if(m_UserItem[sSourceSlot].tIQ!=2)
		return ;
	if(m_UserItem[sSourceSlot].tMagic[6]!=1)
		return ;

	if(m_dwDN < 200000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		// 傍拜侩 诀弊饭捞靛 厚侩焊促 累栏搁 
	}

	if( m_dwDN <= 200000 ) 
		m_dwDN = 0;
	else 
		m_dwDN = m_dwDN - 200000;

	m_UserItem[sSourceSlot].tMagic[MAGIC_100_ADD_POS]=0;

	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();

	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)10);
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)sSourceSlot);
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);

	for(j =0; j < MAGIC_NUM; j++) 
		TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 
	Send(TempBuf, TempBuf.GetLength());
}
////////////////////////////////////////////////////////////////////////////
/// -->属性变更
////////////////////////////////////////////////////////////////////////////
void USER::UpgradeItemAttrReq(short sSourceSlot,short sMaterialSlot)
{
	
	
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;
	int iSid = m_UserItem[sSourceSlot].sSid;
	if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;
	
	if(g_arItemTable[iSid]->m_byWear >= 6 && (g_arItemTable[iSid]->m_byWear!=122) && (g_arItemTable[iSid]->m_byWear!=123) 
		 && (g_arItemTable[iSid]->m_byWear!=124) && (g_arItemTable[iSid]->m_byWear!=125))
		 return;

	if(m_UserItem[sMaterialSlot].sSid == 1286 && g_arItemTable[iSid]->m_byWear != 1)
	{
		SendSystemMsg("放入的道具不是武器!",SYSTEM_ERROR,TO_ME);
		return ;
	}
	if(m_UserItem[sMaterialSlot].sSid == 1287 && g_arItemTable[iSid]->m_byWear == 1)
	{
		SendSystemMsg("放入的道具不是防具!",SYSTEM_ERROR,TO_ME);
		return ;
	}
	if(m_UserItem[sSourceSlot].tMagic[5] < 7)
	{
		SendSystemMsg("必须是+10(包含+7)以上道具才可变更属性!",SYSTEM_ERROR,TO_ME);
		return;
	}
	int iFindAttrId = 0;
	switch(m_UserItem[sMaterialSlot].tMagic[0])
	{
	case 162: //力量15
		iFindAttrId = 161;//10力量
		break;
	case 158: //敏捷15
		iFindAttrId = 157;//10敏捷
		break;
	case 156: //体质15
		iFindAttrId = 155;//10体质
		break;
	case 164: //智慧15
		iFindAttrId = 163;//10智慧
		break;
	case 166: //智力15
		iFindAttrId = 165;//10智力
		break;
	case 193://150血
		iFindAttrId = 159;//100血
		break;
	case 190://50抗
		iFindAttrId = 135;//25抗
		break;
    //case 195://经验30%增加
	//	iFindAttrId = 154;//15%回避
	//	break;
	case 194://50防
		iFindAttrId = 130;//10防御
		break;
    case 161://5力量
		iFindAttrId = 107;//10防御
		break;
     case 155://5体质
		iFindAttrId = 108;//10防御
		break;
	case 157://5敏捷
		iFindAttrId = 109;//10防御
		break;
	case 163://5智慧
		iFindAttrId = 110;//10防御
		break;
	case 165://5智力
		iFindAttrId = 111;//10防御
		break;
	
	default:
		return ;
	}
	int FindAttrSlot = -1;

	for(int i = 0; i<5;i++)
	{
		if(m_UserItem[sSourceSlot].tMagic[i] == iFindAttrId)
		{
			FindAttrSlot = i;
			break;
		}
	}
	if(FindAttrSlot == -1)
	{
		SendSystemMsg("没有找到匹配的属性,属性变更失败!",SYSTEM_ERROR,TO_ME);
		return;
	}
	//减去钱
	if( m_dwDN <= 5000000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 5000000;
	UpdateUserItemDN();						
	SendMoneyChanged();	

	int iRandom = myrand(1, 10000);

	CBufferEx TempBuf;
	TempBuf.Add(UPGRADE_ITEM_RESULT);

	if(iRandom >(10000*0.15f))   //失败率 数值越小 成功率越低
	{ //失败
		TempBuf.Add((BYTE)0x00);
		m_UserItem[sSourceSlot].tMagic[5]--;
	}
	else
	{
		TempBuf.Add((BYTE)0x01);
		m_UserItem[sSourceSlot].tMagic[FindAttrSlot] = m_UserItem[sMaterialSlot].tMagic[0];
	}
	ReSetItemSlot(&m_UserItem[sMaterialSlot]);

	TempBuf.Add((BYTE)2);

	TempBuf.Add((BYTE)sSourceSlot);					
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);
	for(int j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 

	TempBuf.Add((BYTE)sMaterialSlot);
	TempBuf.Add(m_UserItem[sMaterialSlot].sLevel);
	TempBuf.Add(m_UserItem[sMaterialSlot].sSid);
	TempBuf.Add(m_UserItem[sMaterialSlot].sDuration);
	TempBuf.Add(m_UserItem[sMaterialSlot].sBullNum);
	TempBuf.Add(m_UserItem[sMaterialSlot].sCount);
	for(int j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sMaterialSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sMaterialSlot].tIQ); 

	GetRecoverySpeed();							

	Send(TempBuf, TempBuf.GetLength());
}
void USER::UpgradeBlessingItemReq(TCHAR *pBuf, int type)
{
	if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
	{
        SendSystemMsg( "请先解除密码保护后在操作", SYSTEM_ERROR, TO_ME);
		return;
	}
	int iSid = -1,iMaterialSid = -1;
	int iIQ = -1;
	int sSourcebyWear = -1;
    int iCount = 0, iThings = 0;
	int i, j, index = 0;
	int iWeight = 0;
	short sMaterialSlot;

	int iSuccess = 0;

	BYTE tWear = 0;
	BYTE tIQ = 0, bySlot = 0;

	CBufferEx TempBuf;

	ItemList	MyItem[TOTAL_ITEM_NUM];

	CByteArray arMaterial;
	arMaterial.RemoveAll();

	short sSourceSlot = GetShort(pBuf, index);				// 急琶茄 酒捞袍 浇吩锅龋

	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;

	iSid = m_UserItem[sSourceSlot].sSid;
	if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;
	if( iSid == 669 || iSid == 670 )						// 诀弊饭捞靛 窍妨绰 酒捞袍捞 臂矾弊辆幅扼搁
	{
		SendSystemMsg( IDS_USER_CANT_UPGRADE_ITEM, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(g_arItemTable[iSid]->m_byWear >= 6 && (g_arItemTable[iSid]->m_byWear!=122) && (g_arItemTable[iSid]->m_byWear!=123) 
		 && (g_arItemTable[iSid]->m_byWear!=124) && (g_arItemTable[iSid]->m_byWear!=125))
		 return;
    sSourcebyWear = g_arItemTable[iSid]->m_byWear;
    iIQ = m_UserItem[sSourceSlot].tIQ;
	switch(iIQ)
	{
	case NORMAL_ITEM:
		break;
	case MAGIC_ITEM:
		iCount = 1;
		iThings = MATERIAL_MAGIC_UP_ITEM;
		break;
	case RARE_ITEM:
		iCount = 1;
		iThings = MATERIAL_RARE_UP_ITEM;
		break;
	case REMODEL_ITEM:
	case REMODEL_MAGIC_ITEM:
		iCount = 2;
		iThings = MATERIAL_REMODEL_UP_ITEM;
		break;
	case 12: //普通百级
		iCount = 1;
		iThings = MATERIAL_RARE_UP_ITEM;
		break;
	case 15://超级机械
		iCount = 4;
		iThings = MATERIAL_EBODY_UP_ITEM;
		break;
	case 18: //130套
		iCount = 1;
		iThings = MATERIAL_RARE_UP_ITEM;
		break;
	default:
		return;
	}
	
    if( (g_arItemTable[iSid]->m_byWear==122) || (g_arItemTable[iSid]->m_byWear==123) 
		|| (g_arItemTable[iSid]->m_byWear==124) || (g_arItemTable[iSid]->m_byWear==125))
	{
			iCount = 2;
			iThings = MATERIAL_EBODY_UP_ITEM;
		if(IsSuperEbodyItem(iSid))
			iCount = 4;
//机械身体15改是上限
		BYTE t = m_UserItem[sSourceSlot].tMagic[5];
		if(t >= 15) 
			return ;	
	}

		
	for(i = 0; i < TOTAL_ITEM_NUM; i++)			// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

	sMaterialSlot = -1;									
	sMaterialSlot = GetShort(pBuf, index);
	//国服此处增加了一个空字节00
	BYTE bNull = GetByte(pBuf,index);
	if(sMaterialSlot >= EQUIP_ITEM_NUM && sMaterialSlot < TOTAL_INVEN_MAX)
	{
		iSid = MyItem[sMaterialSlot].sSid;

        if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;
		////////////////////////////////////////////////////
		if(g_arItemTable[iSid]->m_sSid == 1189)	//如果是百级还原
		{
			BaiJiHuangYuan(sSourceSlot , sMaterialSlot);
			return;
		}
		///////////////////////////////////////////////////

	    if(type == 2)
		{   
			if(iIQ==18)
			{
                 SendSystemMsg("130级装备武器只能用王者晶石/王者灵石强化.", SYSTEM_ERROR, TO_ME);
			     return; 
		    }
			if(iSid != 845 && iSid != 846) return;
			tWear = g_arItemTable[iSid]->m_byWear;
			if(tWear != BLESSING_WEAPONLESS_WEAR && tWear != BLESSING_ARMORLESS_WEAR )  return;
		//	if((tWear == BLESSING_WEAPONLESS_WEAR) && (sSourcebyWear !=1)) { SendSystemMsg("2.", SYSTEM_ERROR, TO_ME); return;}
			if((tWear == BLESSING_ARMORLESS_WEAR) && (sSourcebyWear ==1))  return;
            if(m_dwDN < 100000)
		    {
			     SendSystemMsg( "决战币不足!使用升级道具需要10万!", SYSTEM_ERROR, TO_ME);
			     return;		
		    }

		     if( m_dwDN <= 100000 ) m_dwDN = 0;
		     else m_dwDN = m_dwDN - 100000;	
			 UpdateUserItemDN();						
	         SendMoneyChanged();
		}
		else if(type == 3)
		{
			if(iIQ==18)
			{
                 SendSystemMsg("130级装备武器只能用王者晶石/王者灵石强化.", SYSTEM_ERROR, TO_ME);
			     return; 
		    }
			if(iIQ==12) //百级不能用晶石灵石升级
				return;
			if(iSid != 847 && iSid != 848) return;
			tWear = g_arItemTable[iSid]->m_byWear;
			if(tWear != NORMAL_WEAPONLESS_WEAR && tWear != NORMAL_ARMORLESS_WEAR) return;
		//	if((tWear == NORMAL_WEAPONLESS_WEAR || tWear == BLESSING_WEAPONLESS_WEAR) && (sSourcebyWear !=1)) return;
			if((tWear == NORMAL_ARMORLESS_WEAR || tWear == BLESSING_ARMORLESS_WEAR) && (sSourcebyWear ==1)) return;
			
			if(m_dwDN < 100000)
		    {
			     SendSystemMsg( "决战币不足!使用晶石,灵石需要10万!", SYSTEM_ERROR, TO_ME);
			     return;		
		    }

		     if( m_dwDN <= 100000 ) m_dwDN = 0;
		     else m_dwDN = m_dwDN - 100000;	
			 UpdateUserItemDN();						
	         SendMoneyChanged();
		}


		else if(type == 12)
		{
			tWear = g_arItemTable[iSid]->m_byWear;
			if(tWear != BLESSING_WEAPONLESS_WEAR && tWear != BLESSING_ARMORLESS_WEAR) return;
			if(g_arItemTable[iSid]->m_byRLevel>60)
				return;
		}else if(type==9)
		{
			if(iSid != 983 && iSid != 1669) return;
			tWear = g_arItemTable[iSid]->m_byWear;
			if(tWear != BLESSING_WEAPONLESS_WEAR && tWear != BLESSING_ARMORLESS_WEAR && tWear!=28) return;
			if((tWear == BLESSING_ARMORLESS_WEAR) && (sSourcebyWear ==1)) return;
			if(m_dwDN < 100000)
		    {
			     SendSystemMsg( "决战币不足!使用祝福宝石需要10万!", SYSTEM_ERROR, TO_ME);
			     return;		
		    }

		     if( m_dwDN <= 100000 ) m_dwDN = 0;
		     else m_dwDN = m_dwDN - 100000;	
			 UpdateUserItemDN();						
	         SendMoneyChanged();
		

		}else if(type==49)
		{
			if(iSid == 1287 || iSid == 1286)
			{
			  if(m_dwDN > 5000000)
		      {
				UpgradeItemAttrReq(sSourceSlot,sMaterialSlot);
				return;
			  }
			  else
			  {
                SendSystemMsg("您决战币不足,变更属性需500万决战币!", SYSTEM_ERROR, TO_ME);
				return;
		      }
			}
			if(iSid != 1438 && iSid != 1437) return;
			tWear = g_arItemTable[iSid]->m_byWear;
			if(tWear != 182 && tWear!=183) return;
			if(m_dwDN < 200000)
		    {
			      SendSystemMsg( "决战币不足!使用王者石头需要20万!", SYSTEM_ERROR, TO_ME);
			     return;		
		    }

		     if( m_dwDN <= 200000 ) m_dwDN = 0;
		     else m_dwDN = m_dwDN - 200000;	
			 UpdateUserItemDN();						
	         SendMoneyChanged();
		}
		else return;

		if(MyItem[sMaterialSlot].sCount <= 0) return;

		iWeight += g_arItemTable[iSid]->m_byWeight;

		MyItem[sMaterialSlot].sCount -= 1;
		arMaterial.Add((BYTE)sMaterialSlot);
	}
	else return;
	if(type != 12){//12是袖珍石头。。不需要神石等材料。
		for(i = 0; i < iCount; i++)
		{
			sMaterialSlot = -1;
			sMaterialSlot = GetShort(pBuf, index);
			if(sMaterialSlot >= EQUIP_ITEM_NUM && sMaterialSlot < TOTAL_INVEN_MAX)
			{											
				iSid = MyItem[sMaterialSlot].sSid;
				if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;
				if(iSid != iThings) return;			
				if(MyItem[sMaterialSlot].sCount <= 0) return;

				iWeight += g_arItemTable[iSid]->m_byWeight;

				MyItem[sMaterialSlot].sCount -= 1;
				arMaterial.Add((BYTE)sMaterialSlot);
			}
			else
			{
				SendSystemMsg(IDS_USER_SHENGSHI, SYSTEM_ERROR, TO_ME);
				return;
			}
		}
	}
	iSuccess = SetBlessingUpgradeItem(sSourceSlot, type);	
	if(MyItem[sSourceSlot].tMagic[5] == 0 && iSuccess == 0) iWeight += g_arItemTable[iSid]->m_byWeight;			// 公霸函拳甫 眉农茄促.

	if(iSuccess == -1) return;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = 1 + arMaterial.GetSize();

	if(!iSuccess)	TempBuf.Add((BYTE)0x00); //角菩
	else			TempBuf.Add((BYTE)0x01);
	
	TempBuf.Add((BYTE)index);

	TempBuf.Add((BYTE)sSourceSlot);					// 林 酒捞袍甫 刚历 	
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ); 

	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		bySlot = (BYTE)arMaterial[i];
	
		MakeItemLog( &m_UserItem[bySlot], ITEMLOG_BLESS_USE );
		FlushItemLog( TRUE );
	
		if(m_UserItem[bySlot].sCount <= 1) ReSetItemSlot(&m_UserItem[bySlot]);// 家厚己 捞骨肺 刚历 檬扁拳窍绊 
		else							   m_UserItem[bySlot].sCount -= 1;

		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...

	Send(TempBuf, TempBuf.GetLength());

	arMaterial.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////////////////////
//	傀乔, 酒赣, 荐欺傀乔, 荐欺酒赣肺 诀弊饭捞靛 且锭
//
int USER::SetBlessingUpgradeItem(short sSlot, int type)
{
	BYTE tCount = 0;

	int iSuccess = 0;

	int i = 0;
	int last = 0;
	int iRandom = 0;

	int iValue = 0;

	if(m_UserItem[sSlot].sSid < 0 || m_UserItem[sSlot].sSid >= g_arItemTable.GetSize()) return -1;
	if(g_arItemTable[m_UserItem[sSlot].sSid]->m_sDuration <= 0) return -1;
	
	tCount = m_UserItem[sSlot].tMagic[5];
	if(tCount >= MAX_ITEM_UPGRADE_COUNT) return -1;		// 6锅掳 浇吩俊 秦寸 诀弊饭捞靛 冉荐(0~9)甫 扁撅茄促.

	if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byWear == ATTACK_ITEM)
	{
		if(type==9)//祝福宝石。不能升级武器
			return -1;
		iRandom = myrand(1, 10000);
		if(type==49)
			iRandom=(int)(iRandom - iRandom*0.10);  //王者灵石 提高10成功。。。					// 己傍伏苞 厚背困秦 罚待荐甫 备窃
		if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byClass == STAFF_ITEM)	iValue = ATTACK_UPGRADE_PSI_BAND;	// 诀弊饭捞靛 己傍矫 吝啊登绰 蔼
		else																iValue = ATTACK_UPGRADE_BAND;						

		if(iRandom <= g_ItemAttUpgrade[tCount])
		{	
			if(type == 2 || type == 49)
			{
				iRandom = myrand(1, 10000);				// 绵酒捞袍老版快 +3诀鳖瘤 焊呈胶肺 档傈且荐 乐促
				for(int i = 0; i < 3; i++)
				{
					last += ATTACK_UPGRADE_BAND;		// 秦寸 己傍伏 父怒 诀臂 秦霖促.
					iSuccess += 1;						// 绵 酒捞袍俊狼秦 割锅 诀臂沁绰瘤 舅妨 霖促.

					if(iRandom <= g_ItemBlessingUpgrade[i]) break;
					if((tCount + iSuccess) >= MAX_ITEM_UPGRADE_COUNT) break;
				}
			}
		    else if(type == 12)
			{
				if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byRLevel > 50) return - 1;
				last = ATTACK_UPGRADE_BAND;
				iSuccess = 1;							// 1诀父 利侩登搁
			}
			else 
			{
					if(type == 3)									// 老馆
				{
					if(	tCount < 6)	
                     {
						int i =-1;
						for(i = EQUIP_ITEM_NUM;i<  TOTAL_INVEN_MAX;i++)
						{
							if(m_UserItem[i].sSid == 1569 )  break;
							
							//删除幸运石      
							
						}
						  RobItem(1569,1);
					 }
				}
				last = ATTACK_UPGRADE_BAND;
				iSuccess = 1;							// 1诀父 利侩登搁
			}
		}
	}
	else    //装备升级情况
	{
		iRandom = myrand(1, 10000);
		if(type == 49)
			iRandom=(int)(iRandom - iRandom*0.10);  //王者晶石 提高10成功。。。
		iValue = DEFENSE_UPGRADE_BAND;
        if(iRandom <= g_ItemDefUpgrade[tCount])
		{
			if(type == 2 || type==9 || type==49)
			{
				iRandom = myrand(1, 10000);				// 绵酒捞袍老版快 +3诀鳖瘤 焊呈胶肺 档傈且荐 乐促
				for(i = 0; i < 3; i++)
				{	
					last += DEFENSE_UPGRADE_BAND;		// 秦寸 己傍伏 父怒 诀臂 秦霖促.
					iSuccess += 1;						// 绵 酒捞袍俊狼秦 割锅 诀臂沁绰瘤 舅妨 霖促.

					if(iRandom <= g_ItemBlessingUpgrade[i]) break;
					if((tCount + iSuccess) >= MAX_ITEM_UPGRADE_COUNT) break;
				}
			}
			else if(type == 12)
			{
				if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byRLevel > 50) return - 1;
				iRandom = myrand(1, 10000);				// 绵酒捞袍老版快 +3诀鳖瘤 焊呈胶肺 档傈且荐 乐促
				for(i = 0; i < 3; i++)
				{	
					last += DEFENSE_UPGRADE_BAND;		// 秦寸 己傍伏 父怒 诀臂 秦霖促.
					iSuccess += 1;						// 绵 酒捞袍俊狼秦 割锅 诀臂沁绰瘤 舅妨 霖促.

					if(iRandom <= g_ItemBlessingUpgrade[i]) break;
					if((tCount + iSuccess) >= MAX_ITEM_UPGRADE_COUNT) break;
				}
			}
			else
			{
				if(type == 3)									// 老馆
				{
					if(tCount < 6)
                    {
						int i =-1;
						for(i = EQUIP_ITEM_NUM;i<  TOTAL_INVEN_MAX;i++)
						{
							if(m_UserItem[i].sSid == 1569 ) break;
							
							//删除幸运石
			                      
							
						}
						 RobItem(1569,1);
					}
				}
				last = DEFENSE_UPGRADE_BAND;
				iSuccess = 1;							// 1诀父 利侩登搁
			}
		}
	}

	if(iSuccess >= 1)									// 秦寸 锅掳狼 己傍伏焊促 累栏搁
	{	
		if(m_UserItem[sSlot].tIQ==12)
			
				m_UserItem[sSlot].tMagic[5]= tCount + iSuccess;

		else
			
			m_UserItem[sSlot].tMagic[5] = tCount + iSuccess;

		//if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byWear == 1 && m_UserItem[sSlot].tMagic[5] == 10)//外观
		//{
		//  int iFindAttrId = 0;
		//  switch(m_UserItem[sSlot].sSid)
	 //     {
	 //        case 323: //反1
		//         iFindAttrId = 1216;//毁灭之刃I型
		//         break;
	 //        case 440: //反2
		//	 case 441: //反2
		//	 case 442: //反2
		//         iFindAttrId = 1219;//毁灭之刃II型
		//         break;
	 //        case 443: //反3
		//	 case 444: //反3
		//         iFindAttrId = 1220;//毁灭之刃III型
		//         break;
	 //        case 330: //龙神之怒
		//         iFindAttrId = 1222;//灵厄权杖
		//         break;
  //         	 case 475: //强化龙神之怒
		//	 case 476: //强化龙神之怒
		//	 case 477: //强化龙神之怒
		//         iFindAttrId = 1225;//强化灵厄权杖
		//         break;
	 //        case 478: //龙神之怒—S
		//	 case 479: //龙神之怒—S
		//         iFindAttrId = 1226;//灵厄权杖-S
		//         break;
		//	 case 316: //龙神炮GX2
		//         iFindAttrId = 1228;//风暴光炮GX2
		//         break;
  //         	 case 405: //龙神炮GX4
		//	 case 406: //龙神炮GX4
		//	 case 407: //龙神炮GX4
		//         iFindAttrId = 1231;//风暴光炮GX4
		//         break;
	 //        case 408: //龙神炮GX6
		//	 case 409: //龙神炮GX6
		//         iFindAttrId = 1232;//风暴光炮GX6
		//         break;
		//	 case 337: //龙之翼
		//         iFindAttrId = 1234;//庇护之翼
		//         break;
	 //        case 510: //强化龙之翼
		//	 case 511: //强化龙之翼
		//	 case 512: //强化龙之翼
		//         iFindAttrId = 1238;//强化庇护之翼
		//         break;
	 //        case 513: //龙之翼—S
		//	 case 514: //龙之翼—S
		//         iFindAttrId = 1239;//庇护之翼-S
		//         break;
  //           default:
		//			return - 1;
	 //     }
		//    m_UserItem[sSlot].sSid = iFindAttrId;//开始ID变化
		//    GetRecoverySpeed();//缓冲成功						
  //      }

		//m_UserItem[sSlot].tMagic[4] = (BYTE)((tCount + iSuccess) * iValue);
		if((tCount + iSuccess) >= 6)//大于或等于8改提示
		{
         CString sysMsg;
         sysMsg.Format("玩家 %s 将[%s]强化为 %d 改 ! ",this->m_strUserID,g_arItemTable[m_UserItem[sSlot].sSid]->m_strName ,tCount + iSuccess);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_NORMAL,TO_ALL);
		}
		
         MakeItemLog( &m_UserItem[sSlot], ITEMLOG_BLESS_UPGRADE_SUCCESS );
	}
	else												// 角菩且版快 绵苞 老馆栏肺 唱穿绢辑 利侩罐绰促.		
	{
		BOOL bRemove = FALSE;
		int iUp = m_UserItem[sSlot].tMagic[5];
		int iCur = m_UserItem[sSlot].tMagic[4];

		if(type == 3)									// 老馆
		{
			
			iRandom = myrand(1, 10000);
		
			if(iRandom <= g_ItemNormalDownUpgrade[0]) 
			{
				if( tCount < 6 )
                {
				int i =-1;
				for(i = EQUIP_ITEM_NUM;i<  TOTAL_INVEN_MAX;i++)
					{
						if(m_UserItem[i].sSid == 1569  ) 
						{				
						 iUp += 1 * 2;
								iCur += iValue * 2;
							//删除幸运石
			                  RobItem(1569,1); 
								break;   
						}
					 }
					
				 
				   }
				iUp -= 1;
				iCur -= iValue;
				
			}
		}
		else if(type == 12)							// 绵 酒捞袍篮 公炼扒 1诀臂 皑家	
		{
			iUp -= 1;
			iCur -= iValue;
		}
		else											// 绵 酒捞袍篮 公炼扒 1诀臂 皑家	
		{
			iUp -= 1;
			iCur -= iValue;
		}

		if(iCur < 0) iCur = 0;
		if(iUp < 0) { iUp = 0; bRemove = TRUE; }

		//m_UserItem[sSlot].tMagic[4] = iCur;
		m_UserItem[sSlot].tMagic[5] = iUp;				

		if(bRemove)			// 孺孺...茄锅档 救茄 酒袍篮 朝赴促.
		{
			MakeItemLog( &m_UserItem[sSlot], ITEMLOG_BLESS_UPGRADE_FAIL );
			ReSetItemSlot(&m_UserItem[sSlot]);	iSuccess = 0; 
		}
		else MakeItemLog( &m_UserItem[sSlot], ITEMLOG_BLESS_UPGRADE_FAIL );
	}	

	FlushItemLog( TRUE );
	return iSuccess;
}

void USER::ChangeUpgradeAcc()
{
	int i = 0;
	int k = 0;
	int j = 0;

	int sid = -1;
	int count = 0;
	int emptyslot = -1;

	BOOL bExistResult = FALSE;

	CItemTable* pTable = NULL;
	ItemList changeItem;

	ItemList UserItemBackup[TOTAL_ITEM_NUM];

	CWordArray arPlusSlot;	arPlusSlot.RemoveAll();
	CWordArray arMinusSlot;	arMinusSlot.RemoveAll();

	for( i = 0; i < TOTAL_ITEM_NUM; i++ )
	{
		UserItemBackup[i] = m_UserItem[i];
	}

	for( i = EQUIP_ITEM_NUM; i < TOTAL_INVEN_MAX; i++ )
	{
		sid = m_UserItem[i].sSid;

		if( sid >= 0 && sid < g_arItemTable.GetSize() )
		{
			pTable = g_arItemTable[sid];
		}
		else
		{
			pTable = NULL;
		}

		if( !pTable ) continue;

		if( pTable->m_byWear != 6 && pTable->m_byWear != 7 && pTable->m_byWear != 8 )	// 咀技辑府啊 酒聪扼搁
		{
			continue;
		}

		if( m_UserItem[i].sDuration != 0 )	// 诀弊饭捞靛 啊瓷茄 脚 酒捞袍捞扼搁
		{
			continue;
		}

		count = m_UserItem[i].sCount;

		if( count <= 0 )	// 官曹 酒捞袍狼 墨款飘啊 0捞窍扼搁 - 肋给等 酒捞袍捞促. 咯扁急 秦寸登瘤 臼绰 酒捞袍栏肺 贸府窍绊 弊成 逞绢埃促.
		{
			continue;
		}

		ReSetItemSlot( &changeItem );

		// 扁粮 咀技辑府甫 背眉且 酒捞袍 单捞磐甫 父电促.
		changeItem.sLevel		= m_UserItem[i].sLevel;
		changeItem.sSid			= sid;
		changeItem.sCount		= 1;
		changeItem.sDuration	= pTable->m_sDuration;
		changeItem.sBullNum		= m_UserItem[i].sBullNum;
		for( k =0; k < MAGIC_NUM; k++ ) changeItem.tMagic[k] = m_UserItem[i].tMagic[k];
		changeItem.tIQ			= m_UserItem[i].tIQ;

		if( count == 1 )
		{
			m_UserItem[i].sLevel		= changeItem.sLevel;
			m_UserItem[i].sSid			= changeItem.sSid;
			m_UserItem[i].sCount		= changeItem.sCount;
			m_UserItem[i].sDuration		= changeItem.sDuration;
			m_UserItem[i].sBullNum		= changeItem.sBullNum;
			for( k =0; k < MAGIC_NUM; k++ ) m_UserItem[i].tMagic[k] = changeItem.tMagic[k];
			m_UserItem[i].tIQ			= changeItem.tIQ;

			arPlusSlot.Add( i );
		}
		else
		{
			for( j = 0; j < count; j++ )	// 祁贸龙 俺荐父怒 捣促.
			{
				emptyslot = -1;

				for( k = EQUIP_ITEM_NUM; k < TOTAL_INVEN_MAX; k++ )	// 后浇吩阑 茫绰促.
				{
					if( m_UserItem[k].sSid == -1 )
					{
						emptyslot = k;
						break;
					}
				}

				if( emptyslot == -1 )	// 后磊府啊 绝栏搁
				{
					// 风橇甫 狐廉唱埃促.
					break;
				}

				m_UserItem[emptyslot].sLevel		= changeItem.sLevel;
				m_UserItem[emptyslot].sSid			= changeItem.sSid;
				m_UserItem[emptyslot].sCount		= changeItem.sCount;
				m_UserItem[emptyslot].sDuration		= changeItem.sDuration;
				m_UserItem[emptyslot].sBullNum		= changeItem.sBullNum;
				for( k =0; k < MAGIC_NUM; k++ ) m_UserItem[emptyslot].tMagic[k] = changeItem.tMagic[k];
				m_UserItem[emptyslot].tIQ			= changeItem.tIQ;

				arPlusSlot.Add( emptyslot );
			}

			if( j <= 0 )	// 祁贸柳 俺荐啊 窍唱档 绝促搁
			{
				// 酒公巴档 函茄霸 绝促.
				break;
			}
			else
			{
				m_UserItem[i].sCount -= j;		// 祁贸霖 俺荐父怒 扁粮 咀技辑府 俺荐俊辑 哗霖促.

				if( m_UserItem[i].sCount <= 0 )
				{
					ReSetItemSlot( &m_UserItem[i] );
				}

				arMinusSlot.Add( i );
			}
		}
	}

	if( arPlusSlot.GetSize() || arMinusSlot.GetSize() )
	{
		if( UpdateUserItemDN() == FALSE )
		{
			for( i = 0; i < TOTAL_ITEM_NUM; i++ )		// 酒捞袍 沥焊 券盔
			{
				m_UserItem[i] = UserItemBackup[i];
			}

			return;
		}

		if( arPlusSlot.GetSize() )
		{
			UpdateInvenSlot( &arPlusSlot, NULL, 3 );
			arPlusSlot.RemoveAll();
		}

		if( arMinusSlot.GetSize() )
		{
			UpdateInvenSlot( &arMinusSlot, NULL, 4 );
			arMinusSlot.RemoveAll();
		}

		SendSystemMsg( IDS_USER_CHANGE_UPGRADE_ACC_SUCCESS, SYSTEM_NORMAL, TO_ME);
	}
	else
	{
		SendSystemMsg( IDS_USER_CHANGE_UPGRADE_ACC_FAIL, SYSTEM_NORMAL, TO_ME);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	酒捞袍 俺檬贸府甫 茄促.
//
void USER::RemodelingItemReq(TCHAR *pBuf)
{
	if(m_dwDN < ITEM_REMODELING_COST) 
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return;		// 诀弊饭捞靛 且 捣捞 绝栏搁 府畔
	}

	short sSourceSid = -1;
	int i, j, index = 0;
	int iWeight = 0;
	short sMaterialSlot[4]	= {-1, -1, -1, -1};
	short sMaterialSid[4]	= {-1, -1, -1, -1};
	int iSuccess = 0, iMaterialCount = 0;
	BYTE bySlot = 0;
	short sPlanzing = 0;

	BOOL bDeleteSource = FALSE, bRemodelSuccess = FALSE;

	CByteArray arMaterial;
	arMaterial.RemoveAll();

	// Source Item
	short sSourceSlot = GetShort(pBuf, index);			// 急琶茄 酒捞袍 浇吩锅龋
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;

	sSourceSid = m_UserItem[sSourceSlot].sSid;
	if(sSourceSid < 0 || sSourceSid >= g_arItemTable.GetSize()) return;
	if( sSourceSid == 669 || sSourceSid == 670 )		// 诀弊饭捞靛 窍妨绰 酒捞袍捞 臂矾弊辆幅扼搁
	{
		SendSystemMsg( IDS_CANT_REMODELING, SYSTEM_ERROR, TO_ME);
		return;
	}

	if(g_arItemTable[sSourceSid]->m_byWear < 1 && g_arItemTable[sSourceSid]->m_byWear > 5) // 公扁, 规绢备啊 酒聪搁 俺炼且 荐 绝促.
	{
		SendSystemMsg( IDS_CANT_REMODELING, SYSTEM_ERROR, TO_ME);
		return;
	}

	if(m_UserItem[sSourceSlot].tMagic[5] >= 1)			// 诀弊饭捞靛等 酒捞袍篮 俺炼且 荐 绝促.
	{
		SendSystemMsg( IDS_CANT_REMODELING, SYSTEM_ERROR, TO_ME);
		return;
	}

	switch(m_UserItem[sSourceSlot].tIQ)
	{
	case RARE_ITEM:				// 俺炼 且 荐 绝绰 酒捞袍
	case UNIQUE_ITEM:
	case SET_ITEM:
	case REMODEL_MAGIC_ITEM:
		SendSystemMsg( IDS_CANT_REMODELING, SYSTEM_ERROR, TO_ME);
		return;

	case REMODEL_ITEM:			// 捞固 3窜 俺炼鳖瘤 茄 酒捞袍
		if(m_UserItem[sSourceSlot].tMagic[0] >= 1 && m_UserItem[sSourceSlot].tMagic[1] >= 1 && m_UserItem[sSourceSlot].tMagic[2] >= 1)
		{
			SendSystemMsg( IDS_CANT_REMODELING, SYSTEM_ERROR, TO_ME);
			return;
		}
		break;
	}

	for(i = 0; i < 4; i++)
	{
		sMaterialSlot[i] = GetShort(pBuf, index);
		if(sMaterialSlot[i] == -1) break;
		if(sMaterialSlot[i] < EQUIP_ITEM_NUM || sMaterialSlot[i] >= TOTAL_INVEN_MAX) return;

		sMaterialSid[i] = m_UserItem[sMaterialSlot[i]].sSid;
		if(sMaterialSid[i] < 0 || sMaterialSid[i] >= g_arItemTable.GetSize()) return;

		iMaterialCount++;
	}

	// Remodeling Start ...
	iSuccess = 0;
	if(g_arItemTable[sSourceSid]->m_byWear == ATTACK_ITEM)	// Weapon
	{
		switch(m_UserItem[sSourceSlot].tIQ)
		{
		case NORMAL_ITEM:									// 1 窜 俺炼 矫档
			for(i = 0; i < iMaterialCount; i++)
			{
				if(sMaterialSid[i] == 678) iSuccess++;			// 瓶备坊
				else if(sMaterialSid[i] == 858) iSuccess++;		// 呕弊俘
				else if(sMaterialSid[i] == 682 || sMaterialSid[i] == 679 || sMaterialSid[i] == 680 || sMaterialSid[i] == 681 || sMaterialSid[i] == 683) sPlanzing = sMaterialSid[i];
				else return;
			}
			if(iSuccess != 2) return;

			bRemodelSuccess = RemodelingItem(1, &m_UserItem[sSourceSlot], sPlanzing);
			if(bRemodelSuccess) 
			{
				arMaterial.Add((BYTE)sSourceSlot);	// 函拳等 浇吩 眠啊
			}

			break;

		case REMODEL_ITEM:
			if(m_UserItem[sSourceSlot].tMagic[0] > 0 && m_UserItem[sSourceSlot].tMagic[1] <= 0)		// 2 窜 俺炼 矫档
			{
				for(i = 0; i < iMaterialCount; i++)
				{
					if(sMaterialSid[i] == 677) iSuccess++;			// 骇备弊坊
					else if(sMaterialSid[i] == 858) iSuccess++;		// 呕弊俘
					else if(sMaterialSid[i] == 682 || sMaterialSid[i] == 679 || sMaterialSid[i] == 680 || sMaterialSid[i] == 681 || sMaterialSid[i] == 683) sPlanzing = sMaterialSid[i];
					else return;
				}
				if(iSuccess != 2) return;
				
				bRemodelSuccess = RemodelingItem(2, &m_UserItem[sSourceSlot], sPlanzing);
				if(bRemodelSuccess) 
				{
					arMaterial.Add((BYTE)sSourceSlot);	// 函拳等 浇吩 眠啊
				}
			}
			else if(m_UserItem[sSourceSlot].tMagic[1] > 0 && m_UserItem[sSourceSlot].tMagic[2] <= 0)	// 3 窜 俺炼 矫档
			{
				for(i = 0; i < iMaterialCount; i++)
				{
					if(sMaterialSid[i] == 860) iSuccess++;			// 橇肺农芬
					else if(sMaterialSid[i] == 858) iSuccess++;		// 呕弊俘
					else if(sMaterialSid[i] == 685)					// D 敲罚隆
					{
						sPlanzing = sMaterialSid[i];
						iSuccess++;		
					}
					else return;
				}
				if(iSuccess != 3) return;

				bRemodelSuccess = RemodelingItem(3, &m_UserItem[sSourceSlot], sPlanzing);

				arMaterial.Add((BYTE)sSourceSlot);	// 函拳等 浇吩 眠啊
				if(bRemodelSuccess == FALSE) 
				{
					bDeleteSource = TRUE;
				}
			}
			else return;
			
			break;
			
		case MAGIC_ITEM:
			for(i = 0; i < iMaterialCount; i++)
			{
				if(sMaterialSid[i] == 860) iSuccess++;			// 橇肺农芬
				else if(sMaterialSid[i] == 858) iSuccess++;		// 呕弊俘
				else if(sMaterialSid[i] == 685)					// D 敲罚隆
				{
					sPlanzing = sMaterialSid[i];
					iSuccess++;		
				}
				else return;
			}
			if(iSuccess != 3) return;

			bRemodelSuccess = RemodelingItem(3, &m_UserItem[sSourceSlot], sPlanzing);
			
			arMaterial.Add((BYTE)sSourceSlot);	// 函拳等 浇吩 眠啊
			if(bRemodelSuccess == FALSE) 
			{
				bDeleteSource = TRUE;
			}
			break;
		}
	}
	else if(g_arItemTable[sSourceSid]->m_byWear >= 2 && g_arItemTable[sSourceSid]->m_byWear <= 5)	// Armor
	{
		switch(m_UserItem[sSourceSlot].tIQ)
		{
		case NORMAL_ITEM:
			for(i = 0; i < iMaterialCount; i++)
			{
				if(sMaterialSid[i] == 678) iSuccess++;			// 瓶备坊
				else if(sMaterialSid[i] == 859) iSuccess++;		// 努府囱
				else if(sMaterialSid[i] == 682 || sMaterialSid[i] == 679 || sMaterialSid[i] == 680 || sMaterialSid[i] == 681 || sMaterialSid[i] == 683) sPlanzing = sMaterialSid[i];
				else return;
			}
			if(iSuccess != 2) return;

			bRemodelSuccess = RemodelingItem(1, &m_UserItem[sSourceSlot], sPlanzing);
			if(bRemodelSuccess) 
			{
				arMaterial.Add((BYTE)sSourceSlot);	// 函拳等 浇吩 眠啊
			}

			break;

		case REMODEL_ITEM:
			if(m_UserItem[sSourceSlot].tMagic[0] > 0 && m_UserItem[sSourceSlot].tMagic[1] <= 0)		// 2 窜 俺炼 矫档
			{
				for(i = 0; i < iMaterialCount; i++)
				{
					if(sMaterialSid[i] == 677) iSuccess++;			// 骇备弊坊
					else if(sMaterialSid[i] == 859) iSuccess++;		// 努府囱
					else if(sMaterialSid[i] == 682 || sMaterialSid[i] == 679 || sMaterialSid[i] == 680 || sMaterialSid[i] == 681 || sMaterialSid[i] == 683) sPlanzing = sMaterialSid[i];
					else return;
				}
				if(iSuccess != 2) return;

				bRemodelSuccess = RemodelingItem(2, &m_UserItem[sSourceSlot], sPlanzing);
				if(bRemodelSuccess) 
				{
					arMaterial.Add((BYTE)sSourceSlot);	// 函拳等 浇吩 眠啊
				}
			}
			else if(m_UserItem[sSourceSlot].tMagic[1] > 0 && m_UserItem[sSourceSlot].tMagic[2] <= 0)	// 3 窜 俺炼 矫档
			{
				for(i = 0; i < iMaterialCount; i++)
				{
					if(sMaterialSid[i] == 860) iSuccess++;			// 橇肺农芬
					else if(sMaterialSid[i] == 859) iSuccess++;		// 努府囱
					else if(sMaterialSid[i] == 684) iSuccess++;		// W 敲罚隆
					else return;
				}
				if(iSuccess != 3) return;
				
				bRemodelSuccess = RemodelingItem(3, &m_UserItem[sSourceSlot], sPlanzing);
				
				arMaterial.Add((BYTE)sSourceSlot);	// 函拳等 浇吩 眠啊
				if(bRemodelSuccess == FALSE) 
				{
					bDeleteSource = TRUE;
				}
			}
			else return;

			break;
			
		case MAGIC_ITEM:
			for(i = 0; i < iMaterialCount; i++)
			{
				if(sMaterialSid[i] == 860) iSuccess++;			// 橇肺农芬
				else if(sMaterialSid[i] == 859) iSuccess++;		// 努府囱
				else if(sMaterialSid[i] == 684) iSuccess++;		// W 敲罚隆
				else return;
			}
			if(iSuccess != 3) return;

			bRemodelSuccess = RemodelingItem(3, &m_UserItem[sSourceSlot], sPlanzing);
			arMaterial.Add((BYTE)sSourceSlot);	// 函拳等 浇吩 眠啊
			if(bRemodelSuccess == FALSE) 
			{
				bDeleteSource = TRUE;
			}
			break;
		}
	}
	else return;

	if(m_dwDN <= ITEM_REMODELING_COST) m_dwDN = 0;				// 俺炼 厚侩
	else m_dwDN = m_dwDN - ITEM_REMODELING_COST;				// 俺炼 厚侩
//	if(m_dwDN < 0) m_dwDN = 0;

	if(bDeleteSource)	// 俺炼啊 角菩秦辑 盔夯 酒捞袍捞 朝扼埃 版快
	{
		iWeight += g_arItemTable[sSourceSid]->m_byWeight;		
		ReSetItemSlot(&m_UserItem[sSourceSlot]);
		
	}

	// Item Log File Generation
	if(bRemodelSuccess)	MakeItemLog(&m_UserItem[sSourceSlot], ITEMLOG_REMODEL_SUCCESS);
	else MakeItemLog(&m_UserItem[sSourceSlot], ITEMLOG_REMODEL_FAIL);
	FlushItemLog(TRUE);

	for(i = 0; i < iMaterialCount; i++)	
	{
		if(m_UserItem[sMaterialSlot[i]].sCount <= 1)	ReSetItemSlot(&m_UserItem[sMaterialSlot[i]]);
		else											m_UserItem[sMaterialSlot[i]].sCount--;
		
		iWeight += g_arItemTable[sMaterialSid[i]]->m_byWeight;	// 公霸函拳甫 眉农茄促.

		arMaterial.Add((BYTE)sMaterialSlot[i]);	// 函拳等 浇吩 眠啊
	}

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...
	
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();

	CBufferEx TempBuf;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	if(bRemodelSuccess)	TempBuf.Add(SUCCESS);
	else				TempBuf.Add(FAIL);
	TempBuf.Add((BYTE)arMaterial.GetSize());

	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		bySlot = (BYTE)arMaterial[i];
		
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

	arMaterial.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//	酒捞袍 俺炼甫 荐青茄促.
//
BOOL USER::RemodelingItem(int iStage, ItemList* pItem, short sPlanzing)
{
	int i, j, k;
	int iStart, iEnd;
	BYTE tRandom1, tRandom2;
	UINT uRandomSum, uRandomSumOld;
	
	RemodelingTableArray*	pTable;
	CWordArray*				pRandomIndex;
	int						iArmorIndex;

	short sReplace = 0;

	int iAdd = 0, iDel = 0;
	int iMagicIndex = 0;

	switch(iStage)
	{
	case 1:
		pTable = &g_arRemodelingTable1;
		pRandomIndex = &g_arRemodelingRandomIndex1;
		iArmorIndex = g_iRemodelingArmorIndex1;
		iMagicIndex = 0;
		break;
	case 2:
		pTable = &g_arRemodelingTable2;
		pRandomIndex = &g_arRemodelingRandomIndex2;
		iArmorIndex = g_iRemodelingArmorIndex2;
		iMagicIndex = 1;
		break;
	case 3:
		pTable = &g_arRemodelingTable3;
		pRandomIndex = &g_arRemodelingRandomIndex3;
		iArmorIndex = g_iRemodelingArmorIndex3;
		iMagicIndex = 2;
		break;
	default:
		return FALSE;
	}
 
	if(g_arItemTable[pItem->sSid]->m_byWear == ATTACK_ITEM)
	{
		iStart = 0;
		iEnd = iArmorIndex + 1;
		if(iStage == 1 || iStage == 2) 
		{
			iAdd = 25;
			iDel = 1;
		}
	}
	else
	{
		iStart = iArmorIndex + 1;
		iEnd = pRandomIndex->GetSize();
		if(iStage == 1 || iStage == 2) 
		{
			iAdd = 25;
			iDel = 1;
		}
	}

	tRandom1 = myrand(1, 100);
	tRandom2 = myrand(1, 100);
	uRandomSum = uRandomSumOld = 0;
	for(i = iStart; i < iEnd; i++)	// 俺炼 加己 搬沥
	{
		j = pRandomIndex->GetAt(i);
		uRandomSum = uRandomSum + pTable->GetAt(j)->m_tRandom1 - iDel;
		if(iStage != 0 && sPlanzing != 0 && pTable->GetAt(j)->m_sPlan == sPlanzing) uRandomSum += iAdd;
		
		if(tRandom1 >= uRandomSumOld && tRandom1 <= uRandomSum)	// 俺炼瞪 加己 搬沥凳
		{
			uRandomSumOld = uRandomSum = 0;
			k = 0;
			
			while(1)		// 俺炼瞪 加己吝俊 绢恫蔼捞 眠啊瞪扒瘤 搬沥				
			{
				uRandomSum += pTable->GetAt(j + k)->m_tRandom2;
				if(tRandom2 >= uRandomSumOld && tRandom2 <= uRandomSum)
				{
					// 酒捞袍 努贰胶客 俺炼加己 努贰胶啊 老摹窍绰瘤 八荤 
					if(CheckClassItem(g_arItemTable[pItem->sSid]->m_byClass, pTable->GetAt(j + k)->m_tNeedClass))	// 老摹窍搁
					{
						pItem->tMagic[iMagicIndex] = (BYTE)pTable->GetAt(j + k)->m_sSid;
						if(pItem->tIQ == NORMAL_ITEM) pItem->tIQ = REMODEL_ITEM;
						else if(pItem->tIQ == MAGIC_ITEM) pItem->tIQ = REMODEL_MAGIC_ITEM;
						
						return TRUE;
					}
					else	// 老摹窍瘤 臼栏搁 措眉 加己 何咯
					{
						sReplace = pTable->GetAt(j + k)->m_tReplace;
						if(pTable->GetAt(sReplace - 1)->m_sRid == sReplace)
						{
							pItem->tMagic[iMagicIndex] = (BYTE)pTable->GetAt(sReplace - 1)->m_sSid;
							if(pItem->tIQ == NORMAL_ITEM) pItem->tIQ = REMODEL_ITEM;
							else if(pItem->tIQ == MAGIC_ITEM) pItem->tIQ = REMODEL_MAGIC_ITEM;
							
							return TRUE;
						}
					}
				}
				uRandomSumOld = uRandomSum + 1;
				if(uRandomSum >= 100) break;	// Safe Code
				k++;
			}
		}
		uRandomSumOld = uRandomSum + 1;
	}

	return FALSE;
}

BOOL USER::CheckOverGuildUserCount(int count)
{
	CString strMsg;

	CGuild *pGuild = NULL;

	pGuild = GetGuild( m_dwGuild );

	if(!pGuild) 
	{
		ReleaseGuild();				// 秦力...

		strMsg.Format( "辨靛甫 茫阑 荐 绝嚼聪促" );
		SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME );
		return FALSE;
	}

	int i = 0;
	int j = 0;
	CGuildUser pGUser;

	for(i = 0; i < MAX_GUILD_USER; i++)
	{
		pGUser = pGuild->m_arUser[i];
		if(pGUser.m_lUsed != 0) j++;
	}

	ReleaseGuild();

	if( j < count ) 
	{
//		SendSystemMsg( IDS_USER_APPLY_CONDITION_GMEMBER, SYSTEM_NORMAL, TO_ME);
		strMsg.Format( "弥家 %d疙 捞惑牢 辨靛俊辑父 脚没且 荐 乐嚼聪促.", count );
		SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME );
		return FALSE;
	}

	return TRUE;
}

void USER::ApplyGuildRun()
{
	CString			str;

	if( !CheckApplyGuildRun() )
	{
		SendSystemMsg( "捞固 脚没登绢 乐嚼聪促.", SYSTEM_NORMAL, TO_ME);
		return;
	}

	char strGuildName[128];
	char strTitle[128];
	char strContent[1024];
	
	sprintf( strGuildName, "%s", m_strGuildName );
	sprintf( strTitle, "GUILD_RUN" );
	sprintf( strContent, "MasterID : %s, MasterAccount : %s", m_strUserID, m_strAccount );

	SDWORD sTitleLen	= _tcslen(strTitle);
	SDWORD sContentLen	= _tcslen(strContent);
	SDWORD sIDLen		= _tcslen(strGuildName);

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	int bbsnum = 2;		// 捞亥飘侩 霸矫魄

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

	int i = 1;
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)strGuildName,0, &sIDLen );
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

	SendSystemMsg( "辨靛 崔府扁 捞亥飘 脚没捞 肯丰登菌嚼聪促.", SYSTEM_NORMAL, TO_ME);

	return;
}

BOOL USER::CheckApplyGuildRun()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[8000];
	BOOL			bExist = FALSE;

	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("select * from bbs_2 where strWriter = \'%s\'"), m_strGuildName);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			bExist = TRUE;
		}
		else if( retcode == SQL_NO_DATA )
		{
		}
		else
		{
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if( bExist ) return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//	儡藕樊(饭萍唇蒲樊)甫 悸泼窍绊 函版捞 积扁搁 函版沥焊甫 努扼捞攫飘肺 焊辰促.
//
void USER::SetDecBullNum(BYTE tSlot, short sDec)
{
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) return;

	short sOldBullNum = m_UserItem[tSlot].sBullNum;
	
	m_UserItem[tSlot].sBullNum -= sDec;
	if(m_UserItem[tSlot].sBullNum < 0) m_UserItem[tSlot].sBullNum = 0;

	if(sOldBullNum != m_UserItem[tSlot].sBullNum) SendBullNum(tSlot);
}
//////////////////////////////////////////////////////////////////////////123321
//捐款  每个玩家改变图表
void USER::SetXingfen()
{
	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	AddAbnormalInfo(ABNORMAL_HIEXP);
	if(m_isDoubleExp > 0 || g_sanJingYan== TRUE ) AddStateInfo(STATE_21);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);					
	Send(TempBuf, TempBuf.GetLength());

}

//////////////////////////////////////////////////////////////////////////123321
//捐款  每个玩家改变图表
void USER::SetXingYun()
{
	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	AddAbnormalInfo(ABNORMAL_MAGICFIND);
	if(m_isDoubleBAOLV > 0 || g_sanBaoLv == TRUE || m_dwGuarDianTianShi > 0) AddStateInfo(STATE_22);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);					
	Send(TempBuf, TempBuf.GetLength());

}
void USER::DelXingfen()
{
	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	DeleteStateInfo(STATE_21);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);					
	Send(TempBuf, TempBuf.GetLength());

}
void USER::DelXingYun()
{
	
	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	DeleteStateInfo(STATE_22);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);					
	Send(TempBuf, TempBuf.GetLength());

}
///////////////////////////////////////////////////////////////////////////
//	趁扁 单固瘤甫 悸泼茄促.
//
void USER::SetColdDamage()
{
	if(m_tAbnormalKind != ABNORMAL_BYTE_NONE) return;		// 捞固 惑怕捞惑捞 吧妨乐绰 惑怕捞搁 府畔
	
	int iRandom = (int)(myrand(1, 1000) / 10);			
	if(iRandom > 53 || iRandom < 50) return;			// 趁扁 单固瘤啊 吧副 犬啦篮 3%

	ClearAbnormalTime(ABNORMAL_TIME);

	m_tAbnormalKind = ABNORMAL_BYTE_COLD;
	m_dwAbnormalTime = COLD_TIME;
	m_dwLastAbnormalTime = GetTickCount();

	if(g_bDebug) SendSystemMsg( IDS_USER_COLD_DAMAGED, SYSTEM_NORMAL, TO_ME);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
//	TempBuf.Add(ABNORMAL_STATUS);
//	TempBuf.Add(ABNORMAL_COLD);
	AddAbnormalInfo(ABNORMAL_COLD);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////
//	拳堪单固瘤甫 悸泼茄促.
//
void USER::SetFireDamage()
{
	if(m_tAbnormalKind != ABNORMAL_BYTE_NONE) return;		// 捞固 惑怕捞惑捞 吧妨乐绰 惑怕捞搁 府畔

	int iRandom = (int)(myrand(1, 1000) / 10);			
	if(iRandom > 53 || iRandom < 50) return;			// 拳堪 单固瘤啊 吧副 犬啦篮 3%

	ClearAbnormalTime(ABNORMAL_TIME);

	m_tAbnormalKind = ABNORMAL_BYTE_FIRE;
	m_dwAbnormalTime = FIRE_TIME;
	m_dwLastAbnormalTime = GetTickCount();

	if(g_bDebug) SendSystemMsg( IDS_USER_FIRE_DAMAGED, SYSTEM_NORMAL, TO_ME);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
//	TempBuf.Add(ABNORMAL_STATUS);
//	TempBuf.Add(ABNORMAL_FIRE);
	AddAbnormalInfo(ABNORMAL_FIRE);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////
//	蜡历啊 焊包茄 酒捞袍, 捣... 焊辰促.
//
void USER::BankOpenReq()
{
	int i, j;
	CBufferEx TempBuf;

	CByteArray arItemSlotList;

	m_dwBankDN = 0;
	InitUserBankItem();								// 函荐甫 柄昌捞 技殴茄促.

	if(!LoadMemUserBank())
	{
		if(!LoadUserBank()) return;						// 蜡历啊 焊包茄 篮青 酒捞袍阑 贸澜 立加且锭 啊瘤绊 柯促.
	}

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
	{												// 泅犁 焊包等 酒捞袍父 焊咯林扁困秦 沥纺茄促. 
		if(m_UserBankItem[i].sSid >= 0)
		{
			arItemSlotList.Add(i);
		}
	}

//	if(m_sLevel < ) return;							// 捞侩且荐乐绰 饭骇阑 力茄茄促.
	TempBuf.Add(BANK_OPEN);
	TempBuf.Add((BYTE)0x00);//国服此处增加了一个空字节
	TempBuf.Add((DWORD)m_dwBankDN);
	TempBuf.Add((BYTE)arItemSlotList.GetSize());

	for(i = 0; i < arItemSlotList.GetSize(); i++)
	{
		BYTE tempSlot = 0;
		tempSlot = arItemSlotList[i];
		TempBuf.Add(tempSlot);
		TempBuf.Add((short)m_UserBankItem[tempSlot].sLevel);
		TempBuf.Add((short)m_UserBankItem[tempSlot].sSid);
		TempBuf.Add((short)m_UserBankItem[tempSlot].sDuration);
		TempBuf.Add((short)m_UserBankItem[tempSlot].sBullNum);
		TempBuf.Add((short)m_UserBankItem[tempSlot].sCount);

		for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_UserBankItem[tempSlot].tMagic[j]);

		TempBuf.Add((BYTE)m_UserBankItem[tempSlot].tIQ);
	}

	Send(TempBuf, TempBuf.GetLength());
	CheckBadItem();

}


///////////////////////////////////////////////////////////////////////////
//	蜡历啊 焊包茄 酒捞袍, 捣阑 DB俊辑 啊瘤绊 柯促.
//
BOOL USER::LoadUserBank()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_USER_BANK (\'%s\')}"), m_strUserID);

	SQLUINTEGER iDN;
	SQLCHAR		strItem[_BANK_DB];
	
	SQLINTEGER	sInd;

	iDN = 0;
	::ZeroMemory(strItem, sizeof(strItem));

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Load User Bank Data !!\n");

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
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iDN,		sizeof(iDN),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY,	strItem,	sizeof(strItem),	&sInd );
		}
		else if( retcode == SQL_NO_DATA )
		{
			g_DB[m_iModSid].ReleaseDB(db_index);	// 单捞磐啊 绝栏骨肺
			return FALSE;
		}
		else
		{
			g_DB[m_iModSid].ReleaseDB(db_index);	// 单捞磐啊 绝栏骨肺
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

	StrToUserBankItem((LPTSTR)strItem);

	m_dwBankDN = iDN;

	SetMemUserBank((LPCTSTR)strItem);

	return TRUE;
}

BOOL USER::LoadAccountBank()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_ACCOUNT_BANK (\'%s\')}")/*, m_iMyServer*/, m_strAccount);

	SQLUINTEGER iDN;
	SQLCHAR		strItem[_ACCOUNT_BANK_DB];
	
	SQLINTEGER	sInd;

	iDN = 0;
	::ZeroMemory(strItem, sizeof(strItem));

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			int i = 1;
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iDN,		sizeof(iDN),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY,	strItem,	sizeof(strItem),	&sInd );
		}
		else if( retcode == SQL_NO_DATA )
		{
			g_DB[m_iModSid].ReleaseDB(db_index);	// 单捞磐啊 绝栏骨肺
			return TRUE;
		}
		else
		{
			g_DB[m_iModSid].ReleaseDB(db_index);	// 单捞磐啊 绝栏骨肺
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

	StrToAccountBankItem((LPTSTR)strItem);

	m_dwAccountBankDN = iDN;

	SetMemAccountBank((LPCTSTR)strItem);

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//	UserBankItem 俊 Buffer狼 郴侩阑 Copy 茄促.
//
void USER::StrToUserBankItem(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	MYINT64 itemserial;

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
	{
		m_UserBankItem[i].sLevel	= GetShort(pBuf, index);
		m_UserBankItem[i].sSid		= GetShort(pBuf, index);
		m_UserBankItem[i].sDuration = GetShort(pBuf, index);
		m_UserBankItem[i].sBullNum	= GetShort(pBuf, index);
		m_UserBankItem[i].sCount	= GetShort(pBuf, index);

		if(m_UserBankItem[i].sCount <= 0) m_UserBankItem[i].sSid = -1;
		for(j = 0; j < MAGIC_NUM; j++) m_UserBankItem[i].tMagic[j] = GetByte(pBuf, index);
		
		m_UserBankItem[i].tIQ = GetByte(pBuf, index);

		for( j = 0; j < 8; j++ )
		{
			itemserial.b[j] = GetByte( pBuf, index );
		}
		m_UserBankItem[i].iItemSerial = itemserial.i;

		for( j = 0; j < ITEM_USER_RIGHT_NUM; j++ )
		{
			m_UserBankItem[i].uid[j] = -1;
			m_UserBankItem[i].SuccessRate[j] = 0;
		}

		if( m_UserBankItem[i].sSid == g_RR.m_iItemSid )				// 捞 酒捞袍捞 肺锯烦喉 惑前捞绊
		{
			if( g_strARKRRWinner.CompareNoCase( m_strUserID ) )	// 泅犁 铰磊啊 酒匆 版快
			{
				ReSetItemSlot( &(m_UserBankItem[i]) );					// 话绰促.
			}
		}

		m_UserBankItem[i].dwTime = 0;
	}	
}

///////////////////////////////////////////////////////////////////////////////
//	AccountBankItem 俊 Buffer狼 郴侩阑 Copy 茄促.
//
void USER::StrToAccountBankItem(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	MYINT64 itemserial;

	for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++)
	{
		m_AccountBankItem[i].sLevel		= GetShort(pBuf, index);
		m_AccountBankItem[i].sSid		= GetShort(pBuf, index);
		m_AccountBankItem[i].sDuration	= GetShort(pBuf, index);
		m_AccountBankItem[i].sBullNum	= GetShort(pBuf, index);
		m_AccountBankItem[i].sCount		= GetShort(pBuf, index);

		if(m_AccountBankItem[i].sCount <= 0) m_AccountBankItem[i].sSid = -1;
		for(j = 0; j < MAGIC_NUM; j++) m_AccountBankItem[i].tMagic[j] = GetByte(pBuf, index);
		
		m_AccountBankItem[i].tIQ = GetByte(pBuf, index);

		for( j = 0; j < 8; j++ )
		{
			itemserial.b[j] = GetByte( pBuf, index );
		}
		m_AccountBankItem[i].iItemSerial = itemserial.i;

		for( j = 0; j < ITEM_USER_RIGHT_NUM; j++ )
		{
			m_AccountBankItem[i].uid[j] = -1;
			m_AccountBankItem[i].SuccessRate[j] = 0;
		}

		m_AccountBankItem[i].dwTime = 0;
	}	
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer 俊 UserBankItem 沥焊甫 Copy 茄促.
//
void USER::UserBankItemToStr(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	MYINT64 itemserial;

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
	{
		SetShort(pBuf, m_UserBankItem[i].sLevel,	index);
		SetShort(pBuf, m_UserBankItem[i].sSid,		index);
		SetShort(pBuf, m_UserBankItem[i].sDuration,	index);
		SetShort(pBuf, m_UserBankItem[i].sBullNum,	index);
		SetShort(pBuf, m_UserBankItem[i].sCount,	index);

		for(j = 0; j < MAGIC_NUM; j++) SetByte(pBuf, m_UserBankItem[i].tMagic[j], index);

		SetByte(pBuf, m_UserBankItem[i].tIQ, index);

		itemserial.i = m_UserBankItem[i].iItemSerial;

		for( j = 0; j < 8; j++ )
		{
			SetByte( pBuf, itemserial.b[j], index );
		}
	}
}

void USER::UserAccountBankItemToStr(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	MYINT64 itemserial;

	for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++)
	{
		SetShort(pBuf, m_AccountBankItem[i].sLevel,		index);
		SetShort(pBuf, m_AccountBankItem[i].sSid,		index);
		SetShort(pBuf, m_AccountBankItem[i].sDuration,	index);
		SetShort(pBuf, m_AccountBankItem[i].sBullNum,	index);
		SetShort(pBuf, m_AccountBankItem[i].sCount,		index);

		for(j = 0; j < MAGIC_NUM; j++) SetByte(pBuf, m_AccountBankItem[i].tMagic[j], index);

		SetByte(pBuf, m_AccountBankItem[i].tIQ, index);

		itemserial.i = m_AccountBankItem[i].iItemSerial;

		for( j = 0; j < 8; j++ )
		{
			SetByte( pBuf, itemserial.b[j], index );
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//	焊包等 酒捞袍, 捣阑 DB俊辑 沥焊甫 盎脚茄促.
//
BOOL USER::UpdateUserBank()
{
// fors test_account_bank offical	 if(UpdateMemUserBank()) return TRUE;
    UpdateMemUserBank();
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strFace[10], strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB], strBankItem[_BANK_DB], strAccountBankItem[_ACCOUNT_BANK_DB];
	TCHAR			strQuickItem[_QUICKITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	::ZeroMemory(strFace, sizeof(strFace));
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));
	::ZeroMemory(strAccountBankItem, sizeof(strAccountBankItem));

	::ZeroMemory(strQuickItem, sizeof(strQuickItem));
	
    ::CopyMemory(strFace, m_strFace, sizeof(m_strFace));

	UserBankItemToStr(strBankItem);
	UserSkillToDBStr(strSkill);
	UserItemToStr(strItem);
	UserPsiToStr(strPsi);
	UserAccountBankItemToStr(strAccountBankItem);	


	SDWORD sBankItemLen	= sizeof(strBankItem);
	SDWORD sFaceLen		= sizeof(strFace);
	SDWORD sSkillLen	= sizeof(strSkill);
	SDWORD sItemLen		= sizeof(strItem);
	SDWORD sPsiLen		= sizeof(strPsi);
	SDWORD sQuickLen	= sizeof(strQuickItem);
	SDWORD sAccountBankItemLen = sizeof(strAccountBankItem);
	
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
		m_dwPsiThreeTime = m_dwMindGuardTime; //加字段
	}
	
    _sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_BANK_DATA (\'%s\',\'%s\',%d,%d,%d,%d,%d, %d,%d,%d, ?, \
		%d,%d,%d, %d,%d, %d, %d,%d,  %d,\
		%d,  %d, %d,%d,%d,%d, %d,%d,%d,%d, \
		?,?,?,?, %d, %d, \
		%d, %d, ?, ?, \
		%d, \
		%d, \
		%d, %d, %d, %d, %d, %d, %d, %d, %d,%d,%d,%d,%d,%d,%d,\
		\'%s\', \
		%d,%d,		%d,%d,		%d,%d)}")/*, 
		m_iMyServer*/, m_strAccount, m_strUserID,	m_sSTR,	m_sCON,	m_sDEX,	m_sVOL,	m_sWIS,	m_iSkin, m_iHair, m_sGender,  
		m_curz,	m_curx,	m_cury,		m_dwBuddy, m_dwGuild,		m_dwExp,		m_sPA, m_sSkillPoint,  	m_dwXP,
		m_sMaxHP, m_sHP, m_sMaxPP, m_sPP, m_sMaxSP,	m_sSP,		m_dwDN,	m_sCityRank, m_sLevel,	m_byClass, 
		m_tAbnormalKind, m_dwAbnormalTime, 

		m_dwBankDN, m_dwAccountBankDN,

		m_bLive,
		m_dwSaintTime, 
		m_dwHiExpTime, m_dwHtExpTime, m_dwMagicFindTime, m_dwMagicFtTime, m_dwNoChatTime, m_dwCloseTime, m_dwAutoMoney,m_dwLingQu, m_dwGuarDianTianShi, m_dwShopPingDN, m_dwVIPTime,m_dwZaiXianTime,m_dwBFindTime,m_dwBHTime,m_dwSJTime,
		m_strLoveName,	//yskang 0.1 m_strLoveName 眠啊
		m_tPsiOneKind, m_dwPsiOneTime,		m_tPsiTwoKind, m_dwPsiTwoTime,		m_tPsiThreeKind, m_dwPsiThreeTime);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Update User Bank Data !!\n");

//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strFace),	0, (TCHAR*)strFace,		0, &sFaceLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strSkill),	0, (TCHAR*)strSkill,	0, &sSkillLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strPsi),		0, (TCHAR*)strPsi,		0, &sPsiLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strQuickItem),	0, (TCHAR*)strQuickItem,	0, &sQuickLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strBankItem),	0, (TCHAR*)strBankItem,		0, &sBankItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strAccountBankItem),	0, (TCHAR*)strAccountBankItem,		0, &sAccountBankItemLen );
        
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//	矫具裹困俊 乐绰 蜡历俊霸 鸥款器呕甫 傈茄促.
//
void USER::SendMyTownPotal(BYTE towho, BYTE type)
{
	CBufferEx	TempBuf;
	int i;

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }	// 捞何盒篮 唱吝俊 官操磊

	TempBuf.Add(USER_INFO);
	TempBuf.Add(type);
	TempBuf.Add(m_uid + USER_BAND);

	TempBuf.AddString(m_strUserID);
		
	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);
		
	TempBuf.Add(m_iSkin);
	TempBuf.Add(m_iHair);
	TempBuf.Add((BYTE)m_sGender);
	TempBuf.AddData(m_strFace, 10);
		
	for( i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(m_UserItem[i].sSid);

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sMagicMaxHP);

	TempBuf.Add(m_tDir);

//	TempBuf.Add((BYTE)m_strAbnormal[0]);			// Abnormal Info
//	TempBuf.Add(m_tPsiAbnormal);					

	TempBuf.Add(m_dwAbnormalInfo);				// 惑怕捞惑 沥焊
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
	m_bPkStatus=0x01;
	TempBuf.Add((BYTE)m_bPkStatus);

	TempBuf.AddString(m_strLoveName);//yskang 0.1

	for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) 
			TempBuf.Add(m_UserItem[i].sSid);	// EBody

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
	switch(towho)
	{
/*	case TO_ALL:
		SendAll(TempBuf, TempBuf.GetLength());
		break;
*/
	case TO_ME:
		Send(TempBuf, TempBuf.GetLength());
		break;
/*
	case TO_ZONE:
		SendZone(TempBuf, TempBuf.GetLength());
		break;
*/
	case TO_INSIGHT:
	default:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;
	}
	SendHuFaInfo(this,towho);

}

///////////////////////////////////////////////////////////////////////////
//	伎俊辑 龋免窍绰 鸥款器呕烙
//
void USER::SendUserTownPotal(USER *pUser, BYTE tMode)
{
	if( !pUser ) return;

	CBufferEx	TempBuf;

	CPoint pos = ConvertToClient( pUser->m_curx, pUser->m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }	// 捞何盒篮 唱吝俊 官操磊

	TempBuf.Add(USER_INFO);
	TempBuf.Add(tMode);
	
	TempBuf.Add(pUser->m_uid + USER_BAND);

	if(tMode == INFO_TOWNPOTAL_DELETE) { Send(TempBuf, TempBuf.GetLength()); return; }

	TempBuf.AddString(pUser->m_strUserID);

	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);

	TempBuf.Add(pUser->m_iSkin);
	TempBuf.Add(pUser->m_iHair);
	TempBuf.Add((BYTE)pUser->m_sGender);
	TempBuf.AddData(pUser->m_strFace, 10);
int i;
	for(i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(pUser->m_UserItem[i].sSid);

//	TempBuf.Add(pUser->m_sHP);
//	TempBuf.Add(pUser->m_sMaxHP);
	TempBuf.Add(pUser->m_sHP);
	TempBuf.Add(pUser->m_sMagicMaxHP);

	TempBuf.Add(pUser->m_tDir);

//	TempBuf.Add((BYTE)pUser->m_strAbnormal[0]);			// Abnormal Info
//	TempBuf.Add(pUser->m_tPsiAbnormal);						//&&&&&&&&&&&& Test Code	

	TempBuf.Add(pUser->m_dwAbnormalInfo);				// 惑怕捞惑 沥焊
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

	TempBuf.AddString(pUser->m_strLoveName);//yskang 0.1

	for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) TempBuf.Add(pUser->m_UserItem[i].sSid);	// EBody
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


///////////////////////////////////////////////////////////////////////////
//	篮青俊辑 涝免陛, 涝免涝 酒捞袍阑 淬寸茄促.
//
void USER::BankItemMoveReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int index = 0;
	BYTE type = GetByte(pBuf, index);
    BOOL bSuccess = FALSE;
	if(!LoadCharData(m_strAccount))
	{
		return;
	}
	if (m_bNowTrading == TRUE || m_bPShopOpen == TRUE) 
	   {
            CString strDate ="";
			SYSTEMTIME st;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
			TCHAR m_Log[500];
			sprintf_s(m_Log,"[ %s ]%s %d,%d 物品移动存在非法2 \r\n",strDate,m_strUserID,m_bNowTrading,m_bPShopOpen);
		    WriteUserShopLog(m_Log);
			SendItemMoveFail();
		    return;
	 }     
	for (int i =0; i < 3 ; i++)	//bug 防止盗号刷装备
	{
		if (m_strChar[i][0] == 0)
			continue;
		if (strcmp(m_strChar[i],m_strUserID) == 0)
			bSuccess = TRUE;
	}

	if (bSuccess == FALSE)	//BUG  
		return;


	switch(type)
	{
	case BANK_ITEM_INPUT:
		BankInPut(pBuf + index);
		break;
	case BANK_ITEM_OUTPUT:
		BankOutPut(pBuf + index);
		break;
	case BANK_DN_INPUT:
		BankInPutDN(pBuf + index);
		break;
	case BANK_DN_OUTPUT:
		BankOutPutDN(pBuf + index);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//	篮青俊 酒捞袍阑 焊包茄促.
//
void USER::BankInPut(TCHAR *pBuf)
{	
	int i;
	int tDestSlot;
	int index = 0;
	int iOver = 0;
	short sSid = -1;
	short sHaveCount = 0;

	BYTE result = SUCCESS;

	CBufferEx TempBuf;

	ItemList MyItem[TOTAL_ITEM_NUM], BackItem;

	BYTE tSourceSlot = GetByte(pBuf, index);	// 荤侩磊 酒捞袍捞 乐带 浇吩 困摹
	short sCount = GetShort(pBuf, index);		// 倔付父怒 焊包
												// 坷肺瘤 牢亥父 啊瓷窍促.			
	if(tSourceSlot < EQUIP_ITEM_NUM || tSourceSlot >= TOTAL_INVEN_MAX) { result = FAIL; goto go_result; }

	sSid = m_UserItem[tSourceSlot].sSid;
	sHaveCount = m_UserItem[tSourceSlot].sCount;
												
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { result = FAIL; goto go_result; }	// 肋给等 sSid

	if(sCount > BANK_MAX_ITEM) 
	{
		SendSystemMsg( IDS_USER_ONCE_MOVE_MAX, SYSTEM_ERROR, TO_ME);
		result = FAIL; goto go_result;
	}
												// 家瘤茄 酒捞袍 荐焊促 腹阑 版快			
	if(sCount <= 0 || sCount > sHaveCount) { result = FAIL; goto go_result; }
			
	if(g_arItemTable[sSid]->m_sDuration > 0 && sCount > 1) { result = FAIL; goto go_result; }
	
	for(i = 0; i < TOTAL_ITEM_NUM; i++)	// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

	ReSetItemSlot(&BackItem);					// DB角菩俊 措厚茄 归诀侩 檬扁拳
												
	tDestSlot = GetSameItem(m_UserItem[tSourceSlot], BANK_SLOT);	// 老窜 般磨荐 乐绰瘤, 般摹搁 鞍篮 酒捞袍捞 乐绰瘤 茫酒夯促.
	
	/***************************篮青 诀公 贸府********************************************/
	if(tDestSlot >= 0)							// 亲惑 货肺捞 眠啊 登绰巴阑 扁霖栏肺 贸府
	{											
		BackItem = m_UserBankItem[tDestSlot];

		if(m_UserBankItem[tDestSlot].sCount >= BANK_MAX_ITEM)
		{
			SendSystemMsg( IDS_USER_SAVE_MAX_COUNT, SYSTEM_ERROR, TO_ME);
			result = FAIL; goto go_result;
		}

		if((m_UserBankItem[tDestSlot].sCount + sCount) > BANK_MAX_ITEM)
		{										
			iOver = m_UserBankItem[tDestSlot].sCount + sCount - BANK_MAX_ITEM;
			if(iOver <= 0) { result = FAIL; goto go_result; }

			m_UserBankItem[tDestSlot].sCount = BANK_MAX_ITEM;
			sCount = sCount - iOver;
		}
		else m_UserBankItem[tDestSlot].sCount += sCount;
	}
	else
	{											//	眠啊
		tDestSlot = GetEmptySlot(BANK_SLOT);

		if(tDestSlot == -1) 
		{
			result = FAIL; goto go_result; 
		}

		m_UserBankItem[tDestSlot].sLevel = m_UserItem[tSourceSlot].sLevel;
		m_UserBankItem[tDestSlot].sSid = m_UserItem[tSourceSlot].sSid;
		m_UserBankItem[tDestSlot].sDuration = m_UserItem[tSourceSlot].sDuration;
		m_UserBankItem[tDestSlot].sBullNum = m_UserItem[tSourceSlot].sBullNum;
		m_UserBankItem[tDestSlot].sCount = sCount;
		for(i = 0; i < MAGIC_NUM; i++) m_UserBankItem[tDestSlot].tMagic[i] = m_UserItem[tSourceSlot].tMagic[i];
		m_UserBankItem[tDestSlot].tIQ = m_UserItem[tSourceSlot].tIQ;
		m_UserBankItem[tDestSlot].iItemSerial = m_UserItem[tSourceSlot].iItemSerial;
	}

	index = 0;
	index = g_arItemTable[m_UserItem[tSourceSlot].sSid]->m_byWeight * sCount;
	/**************************蜡历 牢亥 贸府*********************************************/
	if(sCount >= sHaveCount && iOver == 0) ReSetItemSlot(&m_UserItem[tSourceSlot]);	
	else m_UserItem[tSourceSlot].sCount -= sCount;
	
	/**************************DB Update 贸府*********************************************/
	if(UpdateUserBank() == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 汗盔
		{
			m_UserItem[i] = MyItem[i];
		}
		m_UserBankItem[tDestSlot] = BackItem;
	}

go_result:
	TempBuf.Add(BANK_ITEM_MOVE_RESULT);

	if(result == FAIL)
	{
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	result = (BYTE)0x01;
	TempBuf.Add(result);

	TempBuf.Add((BYTE)tDestSlot);				// 篮青诀公	
	TempBuf.Add(m_UserBankItem[tDestSlot].sLevel);
	TempBuf.Add(m_UserBankItem[tDestSlot].sSid);
	TempBuf.Add(m_UserBankItem[tDestSlot].sDuration);
	TempBuf.Add(m_UserBankItem[tDestSlot].sBullNum);
	TempBuf.Add(m_UserBankItem[tDestSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserBankItem[tDestSlot].tMagic[i]);
	TempBuf.Add(m_UserBankItem[tDestSlot].tIQ);

	TempBuf.Add((BYTE)tSourceSlot);				// 蜡历 牢亥
	TempBuf.Add(m_UserItem[tSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[tSourceSlot].sSid);
	TempBuf.Add(m_UserItem[tSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[tSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSourceSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tSourceSlot].tMagic[i]);
	TempBuf.Add(m_UserItem[tSourceSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());

	m_iCurWeight -= index;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...

//	SendQuickChange();							// 狞酒捞袍 殿废茄霸 秒家登绰瘤 舅酒夯促. 
//	SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
}

///////////////////////////////////////////////////////////////////////////
//	篮青俊 酒捞袍阑 登 茫绰促.
//
void USER::BankOutPut(TCHAR *pBuf)
{
	int i, iWeight = 0;
	int tDestSlot;
	int index = 0;
	int iOver = 0;
	short sSid = -1;
	short sHaveCount = 0;

	BYTE result = SUCCESS;

	CBufferEx TempBuf;

	ItemList MyItem[TOTAL_ITEM_NUM], BackItem;

	BYTE tSourceSlot = GetByte(pBuf, index);	// 荤侩磊 酒捞袍捞 乐带 浇吩 困摹
	short sCount = GetShort(pBuf, index);
												// 坷肺瘤 牢亥父 啊瓷窍促.			
	if(tSourceSlot >= TOTAL_BANK_ITEM_NUM) { result = FAIL; goto go_result; }

	sSid = m_UserBankItem[tSourceSlot].sSid;
	sHaveCount = m_UserBankItem[tSourceSlot].sCount;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { result = FAIL; goto go_result; }

	if(sCount <= 0 || sCount > sHaveCount/* || sCount > BANK_MAX_ITEM*/) { result = FAIL; goto go_result; }

	//if(g_arItemTable[sSid]->m_byWear <= 5 && sCount > 1) { result = FAIL; goto go_result; }
	if(g_arItemTable[sSid]->m_sDuration > 0 && sCount > 1) { result = FAIL; goto go_result; }

	iWeight = g_arItemTable[sSid]->m_byWeight * sCount;
	if(m_iMaxWeight < m_iCurWeight + iWeight)
	{
		SendSystemMsg( IDS_USER_OVER_WEIGHT1, SYSTEM_ERROR, TO_ME);
		result = FAIL; 
		goto go_result;
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++)	// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

	ReSetItemSlot(&BackItem);					// DB角菩俊 措厚茄 归诀侩 檬扁拳
	BackItem = m_UserBankItem[tSourceSlot];
												// 老窜 般磨荐 乐绰瘤, 般摹搁 鞍篮 酒捞袍捞 乐绰瘤 茫酒夯促.
	tDestSlot = GetSameItem(m_UserBankItem[tSourceSlot], INVENTORY_SLOT);
	
	/***************************篮青 诀公 贸府********************************************/
	if(tDestSlot >= 0)							// 亲惑 货肺捞 眠啊 登绰巴阑 扁霖栏肺 贸府
	{		
		m_UserItem[tDestSlot].sCount += sCount;
	}
	else
	{											//	眠啊
		tDestSlot = GetEmptySlot(INVENTORY_SLOT);

		if(tDestSlot == -1) 
		{
			result = FAIL; goto go_result; 
		}

		m_UserItem[tDestSlot].sLevel = m_UserBankItem[tSourceSlot].sLevel;
		m_UserItem[tDestSlot].sSid = m_UserBankItem[tSourceSlot].sSid;
		m_UserItem[tDestSlot].sDuration = m_UserBankItem[tSourceSlot].sDuration;
		m_UserItem[tDestSlot].sBullNum = m_UserBankItem[tSourceSlot].sBullNum;
		m_UserItem[tDestSlot].sCount = sCount;
		for(i = 0; i < MAGIC_NUM; i++) m_UserItem[tDestSlot].tMagic[i] = m_UserBankItem[tSourceSlot].tMagic[i];
		m_UserItem[tDestSlot].tIQ = m_UserBankItem[tSourceSlot].tIQ;
		m_UserItem[tDestSlot].iItemSerial = m_UserBankItem[tSourceSlot].iItemSerial;
	}
	
	/**************************蜡历 篮青 牢亥 贸府*********************************************/
	if(sCount >= sHaveCount) ReSetItemSlot(&m_UserBankItem[tSourceSlot]);	
	else m_UserBankItem[tSourceSlot].sCount -= sCount;
	
	/**************************DB Update 贸府*********************************************/
	if(UpdateUserBank() == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 汗盔
		{
			m_UserItem[i] = MyItem[i];
		}
		m_UserBankItem[tSourceSlot] = BackItem;

		result = FAIL;
	}

go_result:
	TempBuf.Add(BANK_ITEM_MOVE_RESULT);

	if(result == FAIL)
	{
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	result = (BYTE)0x02;
	TempBuf.Add(result);
	TempBuf.Add((BYTE)tSourceSlot);
	TempBuf.Add(m_UserBankItem[tSourceSlot].sLevel);
	TempBuf.Add(m_UserBankItem[tSourceSlot].sSid);
	TempBuf.Add(m_UserBankItem[tSourceSlot].sDuration);
	TempBuf.Add(m_UserBankItem[tSourceSlot].sBullNum);
	TempBuf.Add(m_UserBankItem[tSourceSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserBankItem[tSourceSlot].tMagic[i]);
	TempBuf.Add(m_UserBankItem[tSourceSlot].tIQ);
	TempBuf.Add((BYTE)tDestSlot);
	TempBuf.Add(m_UserItem[tDestSlot].sLevel);
	TempBuf.Add(m_UserItem[tDestSlot].sSid);
	TempBuf.Add(m_UserItem[tDestSlot].sDuration);
	TempBuf.Add(m_UserItem[tDestSlot].sBullNum);
	TempBuf.Add(m_UserItem[tDestSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tDestSlot].tMagic[i]);
	TempBuf.Add(m_UserItem[tDestSlot].tIQ);
	Send(TempBuf, TempBuf.GetLength());

	m_iCurWeight += iWeight;
	GetRecoverySpeed();
}


///////////////////////////////////////////////////////////////////////////
//	篮青俊 涝陛茄促.
///////////////////////////////////////////////////////////////////////////
void USER::BankInPutDN(TCHAR *pBuf)
{
	CBufferEx TempBuf;

	BYTE result; 
	int index = 0;
	DWORD BackBankDN = 0, BackMyDN = 0;

	DWORD InputDN = GetDWORD(pBuf, index);

	TempBuf.Add(BANK_ITEM_MOVE_RESULT);

	if(InputDN <= 0 || InputDN > m_dwDN) 
	{
		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	
	BackMyDN = m_dwDN;
	BackBankDN = m_dwBankDN;
										// 篮青俊 涝陛
	if(!CheckMaxValueReturn(m_dwBankDN, InputDN))
	{									// 窜, MAX蔼捞搁 瞒咀篮...
		CheckMaxValue(m_dwBankDN, InputDN);
		if(m_dwBankDN < InputDN) InputDN = 0;
		else InputDN = m_dwBankDN - InputDN;
	}
	else m_dwBankDN += InputDN;
										// 啊瘤绊 乐绰 家瘤陛俊辑 猾促.
	if(InputDN >= m_dwDN) m_dwDN = 0;
	else m_dwDN -= InputDN;
//	if(InputDN == m_dwDN) m_dwDN = 0;
//	else m_dwDN -= InputDN;

	if(UpdateUserBank() == FALSE)		// DB UpDate
	{
		m_dwDN = BackMyDN;
		m_dwBankDN = BackBankDN;

		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	result = (BYTE)0x03;				// 3 : DN 涝陛
	TempBuf.Add(result);

	TempBuf.Add(m_dwBankDN);			// 篮青郴 涝陛茄 醚陛咀
	TempBuf.Add(m_dwDN);				// 家瘤茄 醚陛咀

	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////
//	篮青俊 免陛茄促.
///////////////////////////////////////////////////////////////////////////
void USER::BankOutPutDN(TCHAR *pBuf)
{
	CBufferEx TempBuf;

	BYTE result; 
	int index = 0;
	DWORD BackBankDN = 0, BackMyDN = 0;

	DWORD OutputDN = GetDWORD(pBuf, index);

	TempBuf.Add(BANK_ITEM_MOVE_RESULT);

	if(OutputDN <= 0 || OutputDN > m_dwBankDN)			// 呈公 农搁
	{
		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	BackMyDN = m_dwDN;					// 归诀
	BackBankDN = m_dwBankDN;
										// 荤侩磊 家瘤陛俊 钦魂茄促.
	if(!CheckMaxValueReturn(m_dwDN, OutputDN))
	{									// 窜, MAX蔼捞搁 瞒咀篮...
		CheckMaxValue(m_dwDN, OutputDN);
		if(m_dwDN < OutputDN) OutputDN = 0;
		else OutputDN = m_dwDN - OutputDN;
	}
	else m_dwDN += OutputDN;
										// 篮青俊辑 猾促.
	if(OutputDN >= m_dwBankDN) m_dwBankDN = 0;
	else m_dwBankDN -= OutputDN;

	if(UpdateUserBank() == FALSE)		// DB UpDate
	{
		m_dwDN = BackMyDN;
		m_dwBankDN = BackBankDN;

		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	result = (BYTE)0x04;				// 4 : DN 免陛
	TempBuf.Add(result);

	TempBuf.Add(m_dwBankDN);			// 篮青郴 涝陛茄 醚陛咀
	TempBuf.Add(m_dwDN);				// 家瘤茄 醚陛咀

	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////
//	府胶鸥飘窍搁 霸烙惑俊辑 楷搬捞 馋绢柳促.
//
void USER::RestartReq(TCHAR *pBuf)
{
	if( m_bLogOut == TRUE ) return;
	if( m_state != STATE_GAMESTARTED ) return;

	if(m_UserItem[39].sSid == 1184)
	{
		if(m_dwTransTime != 0 && m_iSkin == 170){
			if(m_UserItem[39].tMagic[5] == 1)	m_iSkin = 8;
			else if(m_UserItem[39].tMagic[5] == 2) m_iSkin = 8;
			else m_iSkin = 8;
		}else if(m_dwTransTime != 0 && m_iHair == 10015){
			if(m_UserItem[39].tMagic[5] == 1)	m_iHair = 10001;
			else if(m_UserItem[39].tMagic[5] == 2) m_iHair = 10002;
			else m_iHair = 0;
		}		
	}	

	g_pMainDlg->BridgeServerUserRestart(m_uid, m_strUserID);
	return;





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

void USER::RestartReqWithThread(TCHAR *pBuf)
{
	LOGINOUTTHREADDATA *pLIOTD;
	pLIOTD = new LOGINOUTTHREADDATA;
	pLIOTD->CODE = RESTART_REQ;
	pLIOTD->UID = m_uid;
	pLIOTD->ID[0] = '\0';

	EnterCriticalSection( &m_CS_LoginData );
	RecvLoginData.AddTail(pLIOTD);
	nLoginDataCount = RecvLoginData.GetCount();
	LeaveCriticalSection( &m_CS_LoginData );
}

///////////////////////////////////////////////////////////////////////////
//	府胶鸥飘窍搁 霸烙惑俊辑 ACCOUNT LOGIN惑怕肺 登倒酒 埃促.
//
BOOL USER::SendRestartLoginResult()
{
	int		index = 0;
	BYTE	result = FAIL, error_code = 0;
	int		old_index = 0;
	TCHAR	szTemp[8096];

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
	SetByte(m_TempBuf, RESTART_RESULT, index );
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
		Send(m_TempBuf, index);
		return FALSE;
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
					error_code = UNKNOWN_ERR;
					SetByte(szTemp, error_code, old_index);
					Send(szTemp, old_index);
					return FALSE;
				}
			}
		}
	}
	Send(m_TempBuf, index);
	return TRUE;
}


BOOL USER::IsPKZone(USER *pUser)
{
	BOOL mePK = FALSE;
	BOOL youPK = FALSE;

	int index = 0;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;

	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0) 
	{
		if(pUser->m_dwGuild == m_dwGuild) return FALSE;			// 鞘靛傈老锭 鞍篮 辨靛盔篮 PK且荐 绝促.
	}
	if(m_curz == 409) return TRUE;
	if(m_ZoneIndex < 0 || m_ZoneIndex >= g_TownPotal.GetSize()) return FALSE;

//	if(!g_TownPotal[m_ZoneIndex]->iPkMode) return FALSE;		// PK 且荐乐绰 绝绰 瘤开(惑痢救)捞促
	ZONEINFO* pZoneInfo = GetZoneInfo( m_curz );
	if( !pZoneInfo ) return FALSE;
	if( !pZoneInfo->iPkMode ) return FALSE;
//21 guildhouse war
	if(m_curz == 12) return TRUE;		// ^^ 烙矫内靛....

	int metype = CheckInvalidMapType();
	if(metype == 1 || metype == 4 ) return FALSE;    

	int type = pUser->CheckInvalidMapType();
	if(type == 1|| type == 4) return FALSE; 
	


	// 肺凯烦喉 眉农
#ifndef _EVENT_RR		// 瘤开 弥碍 捞亥飘啊 绝栏搁
	if( m_curz == g_RR.m_iZoneNum )
	{
		if( g_RR.m_bRRStatus != RR_START ) return FALSE;
	}
#endif

	if(!CheckInvalidZoneState(type))
	{
		if(m_dwGuild > 0 && pUser->m_dwGuild == m_dwGuild) return FALSE;

		return TRUE;
	}
	else
	{
//		if(type < 0 || type >= g_arMapTable.GetSize()) return TRUE;
//		if(metype < 0 || metype >= g_arMapTable.GetSize()) return TRUE;

		int mapindex = GetGuildMapIndex(metype);
		if(mapindex <= -1 || mapindex >= g_arMapTable.GetSize()) return TRUE;
		mePK = (BOOL)g_arMapTable[mapindex]->m_tPkMode;

		if( pUser->m_ZoneIndex < 0 || pUser->m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
		if( pUser->m_curx >= g_zone[pUser->m_ZoneIndex]->m_sizeMap.cx || pUser->m_curx < 0 ) return FALSE;
		if( pUser->m_cury >= g_zone[pUser->m_ZoneIndex]->m_sizeMap.cy || pUser->m_cury < 0 ) return FALSE;
		type = ((g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_dwType & 0xFF00 ) >> 8);
		mapindex = GetGuildMapIndex(type);
		// if(mapindex <= -1) return TRUE;
		if(mapindex < 0 || mapindex >= g_arMapTable.GetSize()) return FALSE;
		youPK = (BOOL)g_arMapTable[mapindex]->m_tPkMode;

//		mePK = (BOOL)g_arMapTable[metype]->m_tPkMode;
//		type = ((g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_dwType & 0xFF00 ) >> 8);
//		youPK = (BOOL)g_arMapTable[type]->m_tPkMode;

		if(mePK != youPK) return FALSE;
		else return mePK;
	}

	return TRUE;				
}

BOOL USER::IsCity()
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
	if(m_curx >= g_zone[m_ZoneIndex]->m_sizeMap.cx || m_curx < 0) return FALSE;
	if(m_cury >= g_zone[m_ZoneIndex]->m_sizeMap.cy || m_cury < 0) return FALSE;

	int type = ((g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_dwType & 0xFF00 ) >> 8);

	if(type == 1) return TRUE;
	if(type == 12) return TRUE;		// 措访厘档 档矫救俊 乐栏骨肺

	return FALSE;				
}

BOOL USER::IsInPKC()
{
	if(m_curz == 45 || m_curz == 40 || m_curz == 43 || m_curz == 44 || m_curz == 58)
		return TRUE;
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////
//	蜡历狼 扁夯 荐摹甫 概流加己俊 函拳等 单捞磐肺 歹秦辑 利侩茄促.  
//
void USER::SetUserToMagicUser(int iMaxHPUp)
{
	int iTempHP = 0, iTempPP = 0;

	m_sMagicSTR = m_sSTR + m_DynamicUserData[MAGIC_STR_UP];
	m_sMagicCON = m_sCON + m_DynamicUserData[MAGIC_CON_UP];
	m_sMagicDEX = m_sDEX + m_DynamicUserData[MAGIC_DEX_UP];
	m_sMagicVOL = m_sVOL + m_DynamicUserData[MAGIC_VOL_UP];
	m_sMagicWIS = m_sWIS + m_DynamicUserData[MAGIC_WIS_UP];

	m_sMagicMaxHP = g_sHPConst[m_byClass] * m_sMagicCON + g_sHPLV[m_byClass] * (m_sLevel - 1) + g_sHPAdd[m_byClass];
	m_sMagicMaxPP = g_sPPConst[m_byClass] * m_sMagicWIS + g_sPPLV[m_byClass] * (m_sLevel - 1) + g_sPPAdd[m_byClass];
	m_sMagicMaxSP = g_sSPConst[m_byClass] * m_sMagicCON + g_sSPLV[m_byClass] * (m_sLevel - 1) + g_sSPAdd[m_byClass];

	iTempHP = (int)((double)m_DynamicEBodyData[EBODY_CON_TO_HP] / 100 * (double)m_sMagicCON);
	iTempPP = (int)((double)m_DynamicEBodyData[EBODY_WIS_TO_PP] / 100 * (double)m_sMagicWIS);

	m_sMagicMaxHP += m_DynamicUserData[MAGIC_MAX_HP_UP] + (int)((double)m_DynamicEBodyData[EBODY_CON_TO_HP] / 100 * (double)m_sMagicCON);	// 老窜困俊辑 扁夯利牢巴阑 拌魂 + 咯扁辑 概流加己	
	m_sMagicMaxPP += m_DynamicUserData[MAGIC_MAX_PP_UP] + (int)((double)m_DynamicEBodyData[EBODY_WIS_TO_PP] / 100 * (double)m_sMagicWIS);
	m_sMagicMaxSP += m_DynamicUserData[MAGIC_MAX_SP_UP];
	
	//装备增加血量百分比
	if(m_nHPPoint)
	{
		iTempHP = m_sMagicMaxHP*m_nHPPoint/100;
		m_sMagicMaxHP +=iTempHP;
	}	
	//if(m_DynamicUserData[MAGIC_HP_RATING_UP])
	//{
	//	iTempHP = m_sMagicMaxHP*m_DynamicUserData[MAGIC_HP_RATING_UP]/100;
	//	m_sMagicMaxHP +=iTempHP;
	//}	


	m_iMaxWeight = (m_sMagicCON + m_sMagicSTR) * 10 + g_ClassWeight[m_byClass] + m_sLevel * 6;// 泅犁 甸荐乐绰 弥措 公霸阑 备茄促.
	m_iMaxWeight += (int)((double)m_DynamicEBodyData[EBODY_STR_TO_WEIGHT] / 100 * (double)m_sMagicSTR);

	// 绊饭骇狼 蜡历俊霸 漂喊茄 瓷仿阑...
	if(m_sLevel >= ADD_USER_LEVEL) 
	{
		m_sMagicMaxHP += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sHP;
		m_sMagicMaxPP += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sPP;
		m_iMaxWeight += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sWeight;
	}
    m_iMaxWeight = m_iMaxWeight * 3;//300%负重满
    m_iMaxWeight += (int)((double)m_DynamicEBodyData[EBODY_WEIGHT_UP] / 100 * (double)m_iMaxWeight);

	// 弥措 HP 滚弊荐沥 jjs
	switch(iMaxHPUp)
	{
	case -1 :
	case 0:
		break;
	case 1:
		m_sMagicMaxHP = (int)(m_sMagicMaxHP * 1.2 + 0.5);
		break;
	}

	if(m_sHP > m_sMagicMaxHP) m_sHP = m_sMagicMaxHP;	// 弥措 HP 力茄
	
}

///////////////////////////////////////////////////////////////////////////
//	蜡历啊 泅犁 啊瘤绊 乐绰 胶懦吝 sSid客 老摹窍绰 胶懦狼 饭骇阑 备茄促.
//
short USER::GetSkillLevel(BYTE tWeaponClass, short sSid)
{
	if(tWeaponClass < BRAWL || tWeaponClass > FIREARMS) return 0;

	int iStart = tWeaponClass * SKILL_NUM; 
	short sLevel = 0;
	for(int i = iStart; i < iStart + SKILL_NUM; i++)
	{
		if(m_UserSkill[i].sSid == sSid)
		{
			sLevel = m_UserSkill[i].tLevel;
			if(sLevel < 0) sLevel = 0;
			break;
		}
	}

	return sLevel;
}

///////////////////////////////////////////////////////////////////////////
//	蜡历甫 瘤沥矫埃悼救 Hast 惑怕肺 父电促.
//
void USER::SetHaste(int iTime)
{
	if(m_tIsOP == 1) return;					// 款康磊 老锭绰 荤捞坷葱惑怕啊 函窍搁 救等促. (捧疙捞 钱妨辑...)

	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP))
	{
		SetUserToMagicUser(-1);
		SendMyInfo(TO_ME, INFO_MODIFY);
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	}

	ClearAbnormalTime(PSI_ONE_TIME);

	m_dwHasteTime = iTime * 1000;		// jjs07 2001.11.23
	m_dwLastHasteTime = GetTickCount();

	if(g_bDebug) SendSystemMsg(_T("Haste Start"), SYSTEM_NORMAL, TO_ME);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	AddAbnormalInfo(ABNORMAL_HASTE);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	
	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////
//
void USER::SetShield(int iTime)
{
	if(m_tIsOP == 1) return;					

	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP))
	{
		SetUserToMagicUser(-1);
		SendMyInfo(TO_ME, INFO_MODIFY);
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	}

	BOOL bExistShield = CheckAbnormalInfo(ABNORMAL_SHIELD);

	ClearAbnormalTime(PSI_ONE_TIME);
	AddAbnormalInfo(ABNORMAL_SHIELD);

	m_dwShieldTime = iTime * 1000;		// jjs07 2001.11.23
	//m_dwShieldTime = DEFAULT_PSI_DELAY + (DWORD)((double)(iVol*1000)/3.0 + 0.5);	// old
	m_dwLastShieldTime = GetTickCount();

	if(g_bDebug) SendSystemMsg(_T("Shield Start"), SYSTEM_NORMAL, TO_ME);

	if(bExistShield == FALSE)
	{
		CBufferEx TempBuf;
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		SendInsight(TempBuf, TempBuf.GetLength());
		//SendExactScreen(TempBuf, TempBuf.GetLength());
	}
}
void USER::SetBigShield(int iTime)
{
	if(m_tIsOP == 1) return;					// 款康磊 老锭绰 荤捞坷葱惑怕啊 函窍搁 救等促. (捧疙捞 钱妨辑...)

	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP))
	{
		SetUserToMagicUser(-1);
		SendMyInfo(TO_ME, INFO_MODIFY);
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	}

	BOOL bExistShield = CheckAbnormalInfo(ABNORMAL_SHIELD) || CheckAbnormalInfo(ABNORMAL_BIGSHIELD);

	ClearAbnormalTime(PSI_ONE_TIME);
	AddAbnormalInfo(ABNORMAL_BIGSHIELD);

	m_dwBigShieldTime = iTime * 1000;		// jjs07 2001.11.23
	//m_dwShieldTime = DEFAULT_PSI_DELAY + (DWORD)((double)(iVol*1000)/3.0 + 0.5);	// old
	m_dwLastBigShieldTime = GetTickCount();

	if(g_bDebug) SendSystemMsg(_T("Big Shield Start"), SYSTEM_NORMAL, TO_ME);

	if(bExistShield == FALSE)
	{
		CBufferEx TempBuf;
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		SendInsight(TempBuf, TempBuf.GetLength());
		//SendExactScreen(TempBuf, TempBuf.GetLength());
	}
}

///////////////////////////////////////////////////////////////////////////
//	瘤沥等 矫埃悼救 蜡历狼 DEX 蔼阑 10 刘啊 矫挪促.
//
void USER::SetDexUp(int iTime)
{
	if(m_tIsOP == 1) return;					// 款康磊 老锭绰 荤捞坷葱惑怕啊 函窍搁 救等促. (捧疙捞 钱妨辑...)

	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP))
	{
		m_dwMaxHPUpTime = 0;

		SetUserToMagicUser(-1);
		SendMyInfo(TO_ME, INFO_MODIFY);
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	}

	ClearAbnormalTime(PSI_ONE_TIME);

	m_dwDexUpTime = iTime * 1000;
	m_dwLastDexUpTime = GetTickCount();

	if(g_bDebug) SendSystemMsg(_T("Dex Up Start"), SYSTEM_NORMAL, TO_ME);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
//	TempBuf.Add(ABNORMAL_PSI);
//	TempBuf.Add(ABNORMAL_DEX_UP);
	AddAbnormalInfo(ABNORMAL_DEX_UP);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////
//	瘤沥等 矫埃悼救 蜡历狼 Max HP 蔼阑 20% 刘啊 矫挪促.
//
void USER::SetMaxHPUp(int iTime)
{
	if(m_tIsOP == 1) return;					// 款康磊 老锭绰 荤捞坷葱惑怕啊 函窍搁 救等促. (捧疙捞 钱妨辑...)

	ClearAbnormalTime(PSI_ONE_TIME);

	m_dwMaxHPUpTime = iTime * 1000;
	m_dwLastMaxHPUpTime = GetTickCount();

	if(g_bDebug) SendSystemMsg(_T("Max HP Up Start"), SYSTEM_NORMAL, TO_ME);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
//	TempBuf.Add(ABNORMAL_PSI);
//	TempBuf.Add(ABNORMAL_MAX_HP_UP);
	AddAbnormalInfo(ABNORMAL_MAX_HP_UP);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());
	SetUserToMagicUser(1);
	//-----------------------------------------
	//yskang 0.7 MAX HP UP 顿绰 档吝俊 钱府搁 刚啊 吧赴 巴 贸烦 泪瘪 肛勉 泅惑 滚芭 荐沥
	CBufferEx	TempBuf2;
	TempBuf2.Add(MAX_HP_EXIT);
	TempBuf2.Add(m_sHP);
	TempBuf2.Add(m_sMagicMaxHP);
	Send(TempBuf2,TempBuf2.GetLength());
	//SendMyInfo(TO_ME, INFO_MODIFY); //傈眉 单捞鸥绰 焊郴瘤 臼绰促.
	//--------------------------------------
	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
}

void USER::SetAdamantine(int iTime)
{
	if( m_tIsOP == 1 ) return;

	ClearAbnormalTime(PSI_TWO_TIME);
	m_dwAdamantineTime = iTime * 1000;
	m_dwLastAdamantineTime = GetTickCount();

	if( g_bDebug ) SendSystemMsg( _T("Adamantine Start"), SYSTEM_NORMAL, TO_ME );

	CBufferEx TempBuf;
	TempBuf.Add( SET_USER_STATE );
	TempBuf.Add( m_uid + USER_BAND );
//	TempBuf.Add( ABNORMAL_PSI );
//	TempBuf.Add( ABNORMAL_ADAMANTINE );
	AddAbnormalInfo(ABNORMAL_ADAMANTINE);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send( TempBuf, TempBuf.GetLength() );
}

void USER::SetMightyWeapon(int iTime)
{
	if( m_tIsOP == 1 ) return;

	ClearAbnormalTime(PSI_TWO_TIME);
	m_dwMightyWeaponTime = iTime * 1000;
	m_dwLastMightyWeaponTime = GetTickCount();

	if( g_bDebug ) SendSystemMsg( _T("MightyWeapon Start"), SYSTEM_NORMAL, TO_ME );

	CBufferEx TempBuf;
	TempBuf.Add( SET_USER_STATE );
	TempBuf.Add( m_uid + USER_BAND );
//	TempBuf.Add( ABNORMAL_PSI );
//	TempBuf.Add( ABNORMAL_MIGHTYWEAPON );
	AddAbnormalInfo(ABNORMAL_MIGHTYWEAPON);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);

	Send( TempBuf, TempBuf.GetLength() );
}

void USER::SetBerserker(int iTime)
{
	if( m_tIsOP == 1 ) return;

	ClearAbnormalTime(PSI_TWO_TIME);
	m_dwBerserkerTime = iTime * 1000;
	m_dwLastBerserkerTime = GetTickCount();

	if( g_bDebug ) SendSystemMsg( _T("Berserker Start"), SYSTEM_NORMAL, TO_ME );

	CBufferEx TempBuf;
	TempBuf.Add( SET_USER_STATE );
	TempBuf.Add( m_uid + USER_BAND );
	AddAbnormalInfo(ABNORMAL_BERSERKER);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	SendInsight( TempBuf, TempBuf.GetLength() );
//	SendExactScreen(TempBuf, TempBuf.GetLength());
}
///////////////////////////////////////////////////////////////////////////////
//	捞悼 加档甫 20% 狐福霸 秦霖促.
//
void USER::SetFastRun(int iTime)
{
	if( m_tIsOP == 1 ) return;

	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP))
	{
		m_dwMaxHPUpTime = 0;

		SetUserToMagicUser(-1);
		SendMyInfo(TO_ME, INFO_MODIFY);
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	}

	ClearAbnormalTime(PSI_ONE_TIME);
	m_dwFastRunTime = iTime * 1000;
	m_dwLastFastRunTime = GetTickCount();

	if( g_bDebug ) SendSystemMsg( _T("FastRun Start"), SYSTEM_NORMAL, TO_ME );

	CBufferEx TempBuf;
	TempBuf.Add( SET_USER_STATE );
	TempBuf.Add( m_uid + USER_BAND );
	AddAbnormalInfo(ABNORMAL_FASTRUN);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	SendInsight( TempBuf, TempBuf.GetLength() );
//	SendExactScreen(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
//	瘤沥等 矫埃悼救 焊炼 荤捞坷葱阑 静瘤 给窍霸 茄促.
//
BOOL USER::SetMindShock(int iTime)
{
	if( m_tIsOP == 1 ) return FALSE;

	int iRandom = myrand(1, 100);
	if(iRandom > SUCCESS_RATE_MIND_SHOCK) return FALSE;	// 付牢靛 碱农绰 己傍 犬啦捞 乐促.

	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP))
	{
		m_dwMaxHPUpTime = 0;

		SetUserToMagicUser(-1);
		SendMyInfo(TO_ME, INFO_MODIFY);
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	}
    if ( m_dwBigShieldTime != 0 )
	{
	CBufferEx TempttBuf;
            DeleteAbnormalInfo(ABNORMAL_BIGSHIELD);
			TempttBuf.Add(SET_USER_STATE);
			TempttBuf.Add(m_uid + USER_BAND);
			TempttBuf.Add(m_dwAbnormalInfo);
			TempttBuf.Add(m_dwAbnormalInfo_);		
		    SendInsight(TempttBuf, TempttBuf.GetLength());
	}
	ClearAbnormalTime(PSI_ONE_TIME);
	m_dwMindShockTime = (iTime * 1000 )+ 5000;	// 15檬肺 函版 2002.11.14
	m_dwLastMindShockTime = GetTickCount();

	if( g_bDebug ) SendSystemMsg( _T("Mind Shock Start"), SYSTEM_NORMAL, TO_ME );

	CBufferEx TempBuf;
	TempBuf.Add( SET_USER_STATE );
	TempBuf.Add( m_uid + USER_BAND );
	AddAbnormalInfo(ABNORMAL_MINDSHOCK);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
//	SendInsight( TempBuf, TempBuf.GetLength() );
	SendExactScreen(TempBuf, TempBuf.GetLength());

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	Mind Shock 肺 何磐 焊龋茄促.
//
void USER::SetMindGuard(int iTime)
{
	if( m_tIsOP == 1 ) return;

	m_dwMindShockTime = 0;
	m_dwLastMindShockTime = GetTickCount();
	DeleteAbnormalInfo(ABNORMAL_MINDSHOCK);

	m_dwMindGuardTime = iTime * 1000;
	m_dwLastMindGuardTime = GetTickCount();
	AddAbnormalInfo(ABNORMAL_MINDGUARD);

	if( g_bDebug ) SendSystemMsg( _T("Mind Guard Start"), SYSTEM_NORMAL, TO_ME );

	CBufferEx TempBuf;
	TempBuf.Add( SET_USER_STATE );
	TempBuf.Add( m_uid + USER_BAND );
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
//	SendInsight( TempBuf, TempBuf.GetLength() );
	SendExactScreen(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
//	磊脚狼 付过规绢仿阑 臭牢促.
//
void USER::SetPsiShield(int iTime)
{
	if( m_tIsOP == 1 ) return;

	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP))
	{
		m_dwMaxHPUpTime = 0;

		SetUserToMagicUser(-1);
		SendMyInfo(TO_ME, INFO_MODIFY);
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	}

	ClearAbnormalTime(PSI_ONE_TIME);
	m_dwPsiShieldTime = iTime * 1000;
	m_dwLastPsiShieldTime = GetTickCount();

	if( g_bDebug ) SendSystemMsg( _T("Psionic Shield Start"), SYSTEM_NORMAL, TO_ME );

	CBufferEx TempBuf;

	TempBuf.Add( SET_USER_STATE );
	TempBuf.Add( m_uid + USER_BAND );
	AddAbnormalInfo(ABNORMAL_PSISHIELD);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	SendInsight( TempBuf, TempBuf.GetLength() );
//	SendExactScreen(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
//	蒋靛甫 公矫窍绊 单固瘤甫 林哥 20%狼 犬啦肺 蒋靛甫 颇鲍茄促.
//
void USER::SetPiercingShield(int iTime)
{
	if( m_tIsOP == 1 ) return;

	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP))
	{
		m_dwMaxHPUpTime = 0;

		SetUserToMagicUser(-1);
		SendMyInfo(TO_ME, INFO_MODIFY);
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	}

	ClearAbnormalTime(PSI_ONE_TIME);
	m_dwPiercingShieldTime = iTime * 1000;
	m_dwLastPiercingShieldTime = GetTickCount();

	if( g_bDebug ) SendSystemMsg( _T("Piercing Shield Start"), SYSTEM_NORMAL, TO_ME );

	CBufferEx TempBuf;
	TempBuf.Add( SET_USER_STATE );
	TempBuf.Add( m_uid + USER_BAND );
	AddAbnormalInfo(ABNORMAL_PIERCING_SHIELD);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
//	SendInsight( TempBuf, TempBuf.GetLength() );
	SendExactScreen(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////
//	瘤沥等 矫埃悼救 绢狄加狼 利阑 茫绰促.
//
void USER::SetHide(int iTime)
{
/*
	if(m_tIsOP == 1) return;					// 款康磊 老锭绰 荤捞坷葱惑怕啊 函窍搁 救等促. (捧疙捞 钱妨辑...)

	m_tPsiAbnormal = ABNORMAL_HIDE;
	m_dwHideTime = iTime * 1000;		// jjs07 2001.11.23
	//m_dwHideTime = 10000;	// old
	m_dwLastHideTime = GetTickCount();

	if(g_bDebug) SendSystemMsg(_T("Hide Start"), SYSTEM_NORMAL, TO_ME);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(ABNORAML_PSI);
	TempBuf.Add(ABNORMAL_HIDE);
	TempBuf.Add((BYTE)0);
	TempBuf.Add((BYTE)0);
	TempBuf.Add((BYTE)0);
	TempBuf.Add((BYTE)0);
	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());	// 蒋靛 肚绰 窍捞靛啊 捞惑
*/
}

/////////////////////////////////////////////////////////////////////////////////////////
//	鉴埃雀乔, 炮饭器飘贸府
//
BOOL USER::Teleport(int xpos, int ypos)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	BYTE result = FAIL;
	int will_x, will_y;
	int index = 0;
	int	nCurMapType, nTellMapType;

	CPoint ptTeleport;

	CPoint startPt;

	will_x = xpos;	// 框流捞妨绰 痢
	will_y = ypos;

	if(will_x <= 0) will_x = m_curx;
	if(will_y <= 0) will_y = m_cury;

	if(will_x >= g_zone[m_ZoneIndex]->m_sizeMap.cx) will_x = g_zone[m_ZoneIndex]->m_sizeMap.cx - 1;
	if(will_y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) will_y = g_zone[m_ZoneIndex]->m_sizeMap.cy - 1;

	startPt.x = m_curx;
	startPt.y = m_cury;

	CPoint pt = FindNearAvailablePoint_S(will_x, will_y);
	if(pt.x != -1 && pt.y != -1) 
	{
		// 郴啊 肺凯烦喉 版扁厘俊 乐绊
#ifndef _EVENT_RR 
		if( m_curz == g_RR.m_iZoneNum )
		{
			// 泅犁 痢苞 啊绊磊 窍绰 痢狼 加己捞 促福促搁
			if( CheckInvalidMapType() != ((g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_dwType & 0xFF00 ) >> 8) )
			{
				return FALSE;
			}
		}
#else
		if( m_curz == g_RR.m_iZoneNum || m_curz == 61 ||  m_curz == 62 ||  m_curz == 63 )
		{
			// 泅犁 痢苞 啊绊磊 窍绰 痢狼 加己捞 促福促搁
			if( CheckInvalidMapType() != ((g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_dwType & 0xFF00 ) >> 8) )
			{
				return FALSE;
			}
		}
#endif

		nCurMapType = CheckInvalidMapType();
		nTellMapType = ((g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_dwType & 0xFF00 ) >> 8);

		// 父距 郴啊 辑乐绰 镑捞 傍己瘤开捞 酒聪绊 炮窍绰 谅钎啊 傍己瘤开捞扼搁 府畔
		if( (nCurMapType != 8 && nCurMapType != 10 && nCurMapType != 15)
			&& (nTellMapType == 8 || nTellMapType == 10 || nTellMapType == 15) )
			return FALSE;

		ptTeleport = ConvertToClient(pt.x, pt.y);	
		if (m_curx < 0 || m_cury < 0)	//bug 坐标报错
			return FALSE;

		if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, 
			(long)g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, (long)0) == (long)0)
		{
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
		}
		else return FALSE;

		m_curx = pt.x;
		m_cury = pt.y;

		result = SUCCESS;
	}
	else return FALSE;
	
	m_tDir = GetDirection(startPt, pt);		// 泅犁 规氢阑 钎矫茄促.;

	index = 0;
	SetByte(m_TempBuf, PSI_TOWN_POTAL, index);
	SetByte(m_TempBuf, result, index);

	SetByte(m_TempBuf, 0, index);				// 亲惑 鞍篮粮  // 唱吝俊 绝举巴

	SetInt(m_TempBuf, m_uid + USER_BAND, index);

	SetShort(m_TempBuf, ptTeleport.x, index);
	SetShort(m_TempBuf, ptTeleport.y, index);
	SetShort(m_TempBuf, m_curz, index);
	
	Send(m_TempBuf, index);

//	SendMyTownPotal(TO_INSIGHT, INFO_TOWNPOTAL);
	SendMyInfo(TO_INSIGHT, INFO_TOWNPOTAL);
	SightRecalc();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	磷篮 蜡历衬?
//
void USER::IsDeadUser()
{
	if(m_bLive == USER_LIVE || m_tIsOP == TRUE ) return;
	//if(m_sHP > 0) return;

	int i;
	int iPotalZone = -1;

	int rand = 0;

	int x = 0;		// 鸥款器呕俊 历厘等 DB谅钎甫 扁霖栏肺 档矫俊辑 混酒巢	
	int y = 0;

	CPoint ptPotal, pt;

	ptPotal.x = 1; ptPotal.y = 1;
/*
	for(i = 0; i < g_TownPotal.GetSize(); i++)
	{
		if(g_TownPotal[i]->iZone == m_curz) { iPotalIndex = i; break; }
	}

	if(iPotalIndex <0) return;
*/
//	pt = GetTownPotal(iPotalZone);
//	x = pt.x;
//	y = pt.y;

//	if(iPotalZone <0) return;
//	x = g_TownPotal[iPotalIndex]->iPotalX;
//	y = g_TownPotal[iPotalIndex]->iPotalY;
/*
	if(g_TownPotal[iPotalIndex]->iPotalZone != m_curz)
	{
		SetZoneIndex(g_TownPotal[iPotalIndex]->iPotalZone);		// 粮 牢郸胶悸泼
		m_curz = g_TownPotal[iPotalIndex]->iPotalZone;			// 粮 函券(磷菌阑锭 啊鳖款 付阑肺 捞悼秦具窍骨肺 粮阑 官槽促.)
	}
*/
	for(i = 0; i < g_TownPotal.GetSize(); i++)					// 酒聪搁 啊鳖款 档矫肺 捞悼茄促.
	{
		if(g_TownPotal[i]->iZone == m_curz) { iPotalZone = i; break; }
	}

	if(iPotalZone < 0) return;

	CPoint temp = ConvertToClient(m_curx, m_cury);

	if(g_TownPotal[iPotalZone]->iPotalX <= 0) { x = g_TownPotal[iPotalZone]->iPotalX1; y = g_TownPotal[iPotalZone]->iPotalY1; }
	else if(g_TownPotal[iPotalZone]->iPotalX1 <= 0) { x = g_TownPotal[iPotalZone]->iPotalX; y = g_TownPotal[iPotalZone]->iPotalY; }
	else
	{
		int dx1 = abs(g_TownPotal[iPotalZone]->iPotalX - temp.x);
		int dy1 = abs(g_TownPotal[iPotalZone]->iPotalY - temp.y);

		int dx2 = abs(g_TownPotal[iPotalZone]->iPotalX1 - temp.x);
		int dy2 = abs(g_TownPotal[iPotalZone]->iPotalY1 - temp.y);

		if( (dx1 + dy1) < (dx2 + dy2) ) { x = g_TownPotal[iPotalZone]->iPotalX; y = g_TownPotal[iPotalZone]->iPotalY; }
		else							{ x = g_TownPotal[iPotalZone]->iPotalX1; y = g_TownPotal[iPotalZone]->iPotalY1; }
	}

	// alisia
	/*
	if(g_TownPotal[iPotalZone]->iPotalZone != m_curz)
	{
		SetZoneIndex(g_TownPotal[iPotalZone]->iPotalZone);		// 粮 牢郸胶悸泼
		m_curz = g_TownPotal[iPotalZone]->iPotalZone;			// 粮 函券(磷菌阑锭 啊鳖款 付阑肺 捞悼秦具窍骨肺 粮阑 官槽促.)
	}

	ptPotal = ConvertToServer(x, y);			// 鞍篮 粮篮 蝶肺 谅钎函拳甫 ...

	i = SEARCH_TOWN_POTAL_COUNT;				// 谅钎甫 茫扁困茄 冉荐
	
	if(IsMovable_S(ptPotal.x, ptPotal.y) == FALSE)
	{
		while(TRUE)								// 鸥款器呕 伐待 谅钎甫 掘绰促.
		{
			rand = myrand(-TOWN_POTAL_SIZE, TOWN_POTAL_SIZE);
			ptPotal.x += rand; ptPotal.y += rand;
			
			if(IsMovable_S(ptPotal.x, ptPotal.y) == TRUE) break;
			i--;
			if(i <= 0) 
			{
				ptPotal = ConvertToServer(x, y);
				break;
			}
		}
	}

	m_curx = ptPotal.x;
	m_cury = ptPotal.y;
	*/
	ZoneMoveReq( g_TownPotal[iPotalZone]->iPotalZone, x, y );
}

/////////////////////////////////////////////////////////////////////////////////////////
//攻击计算
//
int USER::GetFinalDamage(USER *pUser, int nInitDamage, BYTE tMyWeapon,BOOL &bIsCritical,int max)//yskang 0.3
{
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return 0;

	int iFinalDamage = 0, iFinalTemp;

	int iBasic = (int)((double)pUser->m_sMagicCON/3 + 0.5);
	if(iBasic < 0) iBasic = 0;

	BYTE tWeaponClass = 255;
	BOOL bCanUseSkill = pUser->IsCanUseWeaponSkill(tWeaponClass);

	int		iDefense = 1;
	double	dIron = 0;
	double	dShield = 0;
	double	dGuard = 0;
	double	dVital = 0;
	double	dCAttack = 0;
	double	dABDefense = 0;
	double	dDefenseUP = 0;
	double	dAdamantine = 0;
	double	dBerserker = 0;

	int		iSkillSid = 0;
	int		iIronLevel = 0;
	int		iGuardLevel = 0;
	int		iCounterAttackLevel = 0;
	int		iVitalLevel = 0;
	int		iABDefenseLevel = 0;
	int		iDefenseUPLevel = 0;

	int		iGS = 0;
	int		iVS = 0;
	int		iCA = 0;

	int i = 0;
	int iRandom = 0;
	int tClass = tWeaponClass * SKILL_NUM;
	int	tMyClass = tMyWeapon * SKILL_NUM;

	int iRandomProtect = (int)((double)XdY(1, 1000) / 10 + 0.5);



	iDefense = pUser->GetDefense();						// 规绢备

	if(tWeaponClass != 255)
	{
		for(i = tClass; i < tClass + SKILL_NUM; i++)	// IronSkill
		{
			iSkillSid = pUser->m_UserSkill[i].sSid;

			if(iSkillSid == SKILL_IRON)					// 0 index
			{
				iIronLevel = pUser->m_UserSkill[i].tLevel;
				if(iIronLevel < 0) iIronLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iIronLevel >= 1) 
				{
					iIronLevel += pUser->m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				}
				
				if(iIronLevel >= SKILL_LEVEL) iIronLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				iBasic = (int)((double)iBasic * (1 + (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iIronLevel) / 100));
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
				if(max==0){
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iGuardLevel))
					{
						if(m_byClass == EDGED || m_byClass == BRAWL  || m_byClass == FIREARMS  || m_byClass == JUDGE ) //无视 闪避无视     刀 拳 枪
					 {
						dGuard = (double)(nInitDamage * (double)(g_arSkillTable[iSkillSid]->m_arInc.GetAt(iGuardLevel) - 30 )/100.0 );
				     }
						//else 
						//{      // 仲裁
                       
						//	dGuard = (double)(nInitDamage * (double)(g_arSkillTable[iSkillSid]->m_arInc.GetAt(iGuardLevel) - 30 )/100.0 );
						
				        //}
			         }
				  }
               }            
           
	            if(iSkillSid == SKILL_BACK_ATTACK)					// CounterAttack 2 index
			{
				if (pUser->m_dwFANTAnTime  == 0 )	/////超级反弹
				 
			     {

				if(m_byClass == FIREARMS || m_byClass == STAFF|| m_byClass == JUDGE ) 

				{
					if(tMyWeapon != 255) continue;	
				}
				   }

				iCounterAttackLevel = pUser->m_UserSkill[i].tLevel;		
				if(iCounterAttackLevel < 0) iCounterAttackLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iCounterAttackLevel >= 1) iCounterAttackLevel += pUser->m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ pUser->m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iCounterAttackLevel >= SKILL_LEVEL) iCounterAttackLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
				
				if(GetDistance(pUser->m_curx, pUser->m_cury, 1) == FALSE && pUser->m_dwFANTAnTime == 0 ) iCA = 0;	

				else
				{
					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iCounterAttackLevel)) iCA = 1;
				}				

			if(m_dwFANTAnTime != 0)	
				dCAttack = (double)(nInitDamage * iCA * (double)((g_arSkillTable[iSkillSid]->m_arInc.GetAt(iCounterAttackLevel) +10) / 100.0) ); 
		     else 
				//dCAttack = (double)(nInitDamage * iCA * (double)((g_arSkillTable[iSkillSid]->m_arInc.GetAt(iCounterAttackLevel) +20) / 100.0) );  //修复超级反弹
				dCAttack = (double)(nInitDamage * iCA * 0.20);


			}

			if(iSkillSid == SKILL_ABSOLUTE_DEFENSE)					//绝对防御
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
			if(iSkillSid == SKILL_DEFENSE_UP)					// 超级防御
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

	// 蒋靛 拌魂
	if(pUser->m_bNecklaceOfShield && pUser->m_dwShieldTime != 0)		
	{
		if(m_dwPiercingShieldTime != 0)	dShield = (double)(nInitDamage * 0.01);
		else							dShield = (double)(nInitDamage * 0.10);
	}
	if(pUser->m_bNecklaceOfShield && pUser->m_dwBigShieldTime != 0)		
	{
		if(m_dwPiercingShieldTime != 0)	dShield = (double)(nInitDamage * 0.01);
		else							dShield = (double)(nInitDamage * 0.10);
	}
	else if(pUser->m_bNecklaceOfShield || pUser->m_dwShieldTime != 0 )	
	{
		if(m_dwPiercingShieldTime != 0)	dShield = 0;
		else dShield = (double)(nInitDamage * 0.10);
	}
	else if( pUser->m_dwBigShieldTime !=0 && !pUser->m_bNecklaceOfShield)
	{
		if(m_dwPiercingShieldTime != 0)	dShield = 0;
		else dShield = (double)(nInitDamage * 0.10);
	}
	if(m_dwPiercingShieldTime != 0)	
	{
        
		iRandom = (int)((double)myrand(1, 1000) / 10 + 0.5);
		if(iRandom < SUCCESS_RATE_PIERCING_SHIELD)	// 乔绢教 蒋靛绰 10% 犬啦肺 蒋靛甫 秉促.
		{
		  if ( pUser->m_dwShieldTime !=0 ) 
		  {
			pUser->m_dwShieldTime = 0;
			pUser->m_dwLastShieldTime = GetTickCount();
         	CBufferEx TempBuf;
			pUser->DeleteAbnormalInfo(ABNORMAL_SHIELD);
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add((int)(pUser->m_uid + USER_BAND));
			TempBuf.Add(pUser->m_dwAbnormalInfo);
			TempBuf.Add(pUser->m_dwAbnormalInfo_);			
			pUser->SendInsight(TempBuf, TempBuf.GetLength());
		  }
		  if ( pUser->m_dwBigShieldTime !=0 )
		  {
			pUser->m_dwBigShieldTime = 0;
			pUser->m_dwLastBigShieldTime = GetTickCount();
	        CBufferEx TempqqBuf;
			pUser->DeleteAbnormalInfo(ABNORMAL_BIGSHIELD);
			TempqqBuf.Add(SET_USER_STATE);
			TempqqBuf.Add((int)(pUser->m_uid + USER_BAND));
			TempqqBuf.Add(pUser->m_dwAbnormalInfo);
			TempqqBuf.Add(pUser->m_dwAbnormalInfo_);		
			pUser->SendInsight(TempqqBuf, TempqqBuf.GetLength());
		  }

		}
	}
	
	if(pUser->m_bNecklaceOfShield) pUser->SendAccessoriDuration(SID_NECKLACE_OF_SHIELD);

	if( pUser->m_dwAdamantineTime != 0 )
	{
		dAdamantine = (double)( (double)iDefense * 0.1 );
	}
	if( pUser->m_dwBerserkerTime != 0 )
	{
		dBerserker = (double)( (double)iDefense * 0.15 );
	}
	//熊变增加 15%防
	//if(pUser->m_iSkin==2){
	//	dBerserker =dBerserker+ (double)( (double)iDefense * 0.15 );
	//}
	

	iDefense = (int)( iDefense + dABDefense + dDefenseUP + dAdamantine + dBerserker );

	if(tMyWeapon != 255)
	{
		for(i = tMyClass; i < tMyClass + SKILL_NUM; i++)	// Vital Critical
		{
			iSkillSid = m_UserSkill[i].sSid;

			if(iSkillSid == SKILL_VITAL_CRITICAL)			// 12 index
			{
				iVitalLevel = m_UserSkill[i].tLevel;		
				if(iVitalLevel < 0) iVitalLevel = 0;
				
				// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
				if(iVitalLevel >= 1) iVitalLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iVitalLevel >= SKILL_LEVEL) iVitalLevel = SKILL_LEVEL - 1;		//弥措 41捞骨肺 皋葛府绰 40栏肺 力茄
				if(iSkillSid >= g_arSkillTable.GetSize()) continue;
		
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iVitalLevel))
				{
					dVital = (double)(g_arSkillTable[iSkillSid]->m_arInc.GetAt(iVitalLevel)/100.0);
					
					if(iRandomProtect <= RANDOM_PROTECT && pUser->m_bEarringOfProtect)	// 辑器飘 蓖吧捞
					{
						dVital = 0;
						pUser->SendAccessoriDuration(SID_EARRING_OF_PROTECT);
					}
				}
			}
		}
	}

	if(dVital > 0)//穿刺
	{
		bIsCritical = TRUE; //yskang 0.3
		//智力变成远程伤害力
		int v4 = 0;
		if(m_byClass == JUDGE||m_byClass==FIREARMS)
		{
			v4=(int)((double)pUser->m_DynamicEBodyData[EBODY_WIS_TO_DAMAGE] *0.01 * pUser->m_sMagicWIS);
		}
		iFinalDamage = (int)(nInitDamage - (iDefense * dVital * (double)pUser->m_DynamicEBodyData[EBODY_VITAL_RESIST]/100 + \
			                 (int)((double)pUser->m_DynamicEBodyData[EBODY_CON_TO_VITAL_RESIST] / 100 * pUser->m_sMagicCON) + \
							 (int)((double)pUser->m_DynamicEBodyData[EBODY_DEX_TO_VITAL_RESIST] / 100 * pUser->m_sMagicDEX) + \
							 iBasic + dShield + dGuard)); 
	}
	else
	{
		iFinalDamage = (int)(nInitDamage - (iDefense + iBasic + dShield + dGuard)); 
	}

	if(iFinalDamage < 0) iFinalDamage = 0;
	if(iFinalDamage <= 15) //最小攻击
	{
		iFinalTemp = iFinalDamage;
		iFinalDamage += (int)((double)nInitDamage * 0.2 + 1.5);	// 弥家措固瘤甫 弥措 15栏肺 茄促.
		if(iFinalDamage > 15) iFinalDamage = 15;
		iFinalDamage = max(iFinalDamage, iFinalTemp);
	}

	if(dCAttack > 0)	//	反击
	{
		iDefense = (int)((double)m_sMagicCON/3 + 0.5);		// 规绢备
		iCA = (int)dCAttack - iDefense;						// 弥辆 馆拜 单固瘤

		iCA = iCA * (1 - (int)((double)pUser->m_DynamicEBodyData[EBODY_BACK_RESIST]/100));
		iCA -= ((int)((double)pUser->m_DynamicEBodyData[EBODY_CON_TO_BACK_RESIST] / 100 * pUser->m_sMagicCON) + \
			   (int)((double)pUser->m_DynamicEBodyData[EBODY_DEX_TO_BACK_RESIST] / 100 * pUser->m_sMagicDEX)  );

		if(iCA > 0)	SetDamage(iCA);							// 单固瘤 拌魂阑 茄促.
	    pUser->SendDamageNum(0,m_uid+USER_BAND,iCA);//拳反击技能打人显示数字
		if(m_lDeadUsed == 1)  //   反击死
			GetLevelDownExp(USER_PK, -1, FALSE,m_strUserID);
	}

	return iFinalDamage;
}

////////////////////////////////////////////////////////////////////////////////////
//	抗距等 酒捞叼牢瘤 八荤茄促.
//
BOOL USER::IsReservedID(char *szName)
{
	int nSize = sizeof(g_pszReservedID)/sizeof(char*);
	CString szCheck = szName;
	CString szCheck2;

	szCheck.MakeLower();

	for (int i=0; i< nSize; i++) 
	{
		szCheck2 = g_pszReservedID[i];
		szCheck2.MakeLower();

		if(szCheck.Find(szCheck2) != -1) return TRUE;

	}

	return FALSE;
}

BOOL USER::IsReservedLoveName(char *szName)
{
	int nSize = sizeof(g_pszReservedLoveName)/sizeof(char*);
	CString szCheck3 = szName;
	CString szCheck4;

	szCheck3.MakeLower();

	for (int i=0; i< nSize; i++) 
	{
		szCheck4 = g_pszReservedLoveName[i];
		szCheck4.MakeLower();

		if(szCheck3.Find(szCheck4) != -1) return TRUE;

	}

	return FALSE;
}

CPoint USER::FindNearAvailablePoint_S(int x, int y)
{
	if(x <= -1 || y <= -1) return CPoint(-1,-1);
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return CPoint(-1,-1);
	if(x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return CPoint(-1,-1);

	if(IsMovable_S( x, y ))
	{
		return CPoint(x,y);
	}

	CPoint t;
	int tempX = x, tempY = y;
	int i;

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

	for( i = 0; i < 25; i++)
	{
		if( IsMovable_S( tempX + dir[i][0], tempY + dir[i][1] ) )
		{
			return CPoint( tempX + dir[i][0], tempY + dir[i][1] );
		}
	}

	return CPoint(-1,-1);
}

CPoint USER::FindNearAvailablePoint_S(int zone,int x, int y)
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
	if(x <= -1 || y <= -1) return CPoint(-1,-1);
	if( ZoneIndex < 0 || ZoneIndex >= g_zone.GetSize() ) return CPoint(-1,-1);
	if(x >= g_zone[ZoneIndex]->m_sizeMap.cx || y >= g_zone[ZoneIndex]->m_sizeMap.cy) return CPoint(-1,-1);

	if(IsMovable_S(zone, x, y ))
	{
		return CPoint(x,y);
	}

	CPoint t;
	int tempX = x, tempY = y;
	int i;

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

	for( i = 0; i < 25; i++)
	{
		if( IsMovable_S(zone, tempX + dir[i][0], tempY + dir[i][1] ) )
		{
			return CPoint( tempX + dir[i][0], tempY + dir[i][1] );
		}
	}

	return CPoint(-1,-1);
}

//////////////////////////////////////////////////////////////////////////////////
//	努扼捞攫飘谅钎 扁霖栏肺 泅犁谅钎客 林函 25谅钎 吝俊 框流老 荐 乐绰 痢捞 乐绰瘤 魄窜.
//
CPoint USER::FindNearAvailablePoint_C(int x, int y)
{
	if( IsMovable_C( x, y ) )
	{
		return CPoint(x,y);
	}

	CPoint t = ConvertToServer( x, y );

	if( t.x == -1 || t.y == -1 )
	{
		return CPoint(-1,-1);
	}

	int tempX = t.x, tempY = t.y;
	int i;

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

	for( i = 0; i < 25; i++)
	{
		if( IsMovable_S( tempX + dir[i][0], tempY + dir[i][1] ) )
		{
			return ConvertToClient( tempX + dir[i][0], tempY + dir[i][1] );			
		}
	}

	return CPoint(-1,-1);
}

///////////////////////////////////////////////////////////////////////////////////////
//	努扼捞攫飘 谅钎甫 扁霖栏肺 x, y 啊 捞悼且 荐 乐绰 痢牢瘤 魄窜
//
BOOL USER::IsMovable_C(int x, int y)
{
	CPoint t = ConvertToServer( x, y );

	if( t.x == -1 || t.y == -1 ) return FALSE;

	return IsMovable_S(t.x, t.y);
}

///////////////////////////////////////////////////////////////////////////////////////
//	辑滚谅钎甫 努扼捞攫飘 谅钎肺 官槽促.
//
CPoint USER::ConvertToClient(int x, int y)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return CPoint(-1,-1);
	if(!g_zone[m_ZoneIndex]) return CPoint(-1, -1);

	int tempx, tempy;
	int temph = g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cy / 2 - 1;

	if(x <= -1 || y <= -1) return CPoint(-1,-1); 
	if( y >= g_zone[m_ZoneIndex]->m_sizeMap.cy || x >= g_zone[m_ZoneIndex]->m_sizeMap.cx ) return CPoint(-1,-1);

	tempx = x - temph + y;
	tempy = y - x + temph;

	return CPoint( tempx, tempy );
}

///////////////////////////////////////////////////////////////////////////////////////
//	蜡历沥焊甫 滚欺俊 历厘茄促.
//
void USER::FillUserInfo(char *pBuf, int &index, BYTE flag)
{
	int nLen = 0, i;

	if( m_state != STATE_GAMESTARTED && flag == INFO_MODIFY ) return;

	SetByte(pBuf, USER_INFO, index);
	SetByte(pBuf, flag, index);
	SetInt(pBuf, m_uid + USER_BAND, index);

	if(flag == INFO_MODIFY || flag == INFO_TOWNPOTAL)
	{
		SetVarString(pBuf, m_strUserID, _tcslen(m_strUserID), index);

		CPoint t = ConvertToClient( m_curx, m_cury );
		if( t.x == -1 || t.y == -1 ) { t.x = 1; t.y = 1; }
		
		SetShort(pBuf, t.x, index);
		SetShort(pBuf, t.y, index);

		SetDWORD(pBuf, m_iSkin, index);
		SetDWORD(pBuf, m_iHair, index);
		SetByte(pBuf, (BYTE)m_sGender, index);
		SetString(pBuf, m_strFace, 10, index);

		for(i = 0; i < EQUIP_ITEM_NUM; i++) SetShort(pBuf, m_UserItem[i].sSid, index);

		SetShort(pBuf, m_sHP, index);
		SetShort(pBuf, m_sMagicMaxHP, index);

		SetByte(pBuf, m_tDir, index);

		SetDWORD(pBuf, m_dwAbnormalInfo, index);		
		SetDWORD(pBuf, m_dwAbnormalInfo_, index);
		SetDWORD( pBuf, 0, index);
		SetDWORD( pBuf, 0, index);
		//国服此处多16 byte 0
		for(int zero = 0;zero<16;zero++)
			SetByte(pBuf, (BYTE)0x00, index);
		SetShort(pBuf, m_sCityRank, index);

		SetInt( pBuf, m_dwGuild, index );

		nLen = strlen(m_strGuildName);
		if(nLen <= 0) nLen = 1;

		SetByte( pBuf, (BYTE)nLen, index );
		SetString( pBuf, m_strGuildName, nLen, index );		// Add Guild Name
		SetShort( pBuf, m_sGuildVersion, index );
		
		SetByte(pBuf, m_byClass, index);
		SetByte(pBuf, m_bPkStatus, index);
		//-- yskang 0.1 眠啊登绰 菩哦.... 龋莫[局莫]
	
		nLen = strlen(m_strLoveName);
		if(nLen < 1) nLen =1;
		SetByte(pBuf, (BYTE)nLen, index);//辨捞瘤沥
		SetString(pBuf,m_strLoveName,nLen,index);//单捞磐 持扁

		for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-2; i++) SetShort(pBuf, m_UserItem[i].sSid, index);	// EBody

		if(m_UserItem[TOTAL_ITEM_NUM-2].sSid!=-1&&m_UserItem[TOTAL_ITEM_NUM-2].sDuration!=0){
			SetByte(pBuf,(BYTE)(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]),index);
			SetByte( pBuf, 0x00, index );
		}else{
			SetByte(pBuf,(BYTE)(m_UserItem[TOTAL_ITEM_NUM-2].tMagic[0]),index);
			SetByte( pBuf, 0xff, index );
		}
		SetByte(pBuf, strlen(m_PersonalShopName), index);
		SetString(pBuf, m_PersonalShopName, strlen(m_PersonalShopName), index);
		//国服此处多5byte 0x0a 0x00 0x00 0x00 0x00
		SetByte(pBuf,(BYTE)0x01,index);
		SetByte(pBuf,(BYTE)0x00,index);
		SetByte(pBuf,(BYTE)0x00,index);
		SetByte(pBuf,(BYTE)0x00,index);
		SetByte(pBuf,(BYTE)m_tBabyCall,index);
		if(m_tBabyCall)
		{
			int tmp = strlen(g_szBabyName[m_sBabyID%2]);
			SetByte(pBuf, tmp, index);
			SetString(pBuf, g_szBabyName[m_sBabyID%2],tmp, index);
			SetShort(pBuf,m_sBabyID,index);
		}
	}
	SendHuFaInfo(this,TO_ME);
}
/*
void USER::SendRangeInfoToMe(int min_x, int min_y, int max_x, int max_y, BYTE flag)
{
	int i, j, tuid;
	int throwindex;

	CPoint t;

	USER *pUser = NULL;
	CNpc *pNpc = NULL;
	ItemList* pThrowItem = NULL;
	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap) return; 

	for( i = min_y; i < max_y; i++)
	{
		if( i >= pMap->m_sizeMap.cy || i < 0 ) continue;

		for( j = min_x; j < max_x; j++)
		{
			if( j >= pMap->m_sizeMap.cx || j < 0 ) continue;

			tuid = GetUid( j, i );

			if( tuid != 0 )
			{
				if( tuid != m_uid + USER_BAND )
				{
					if( tuid >= USER_BAND && tuid < NPC_BAND )
					{
						pUser = GetUser( tuid - USER_BAND );

						if(pUser->m_state != STATE_GAMESTARTED) continue;

						if( j != pUser->m_curx || i != pUser->m_cury )
						{
							SetUid(j, i, 0);
						}
						else
						{
							SendUserInfo( pUser, flag );
						}
					}
					else if( tuid >= NPC_BAND && tuid < INVALID_BAND)
					{
						pNpc = GetNpc( tuid - NPC_BAND );
						
						if( pNpc )
						{
							if( j != pNpc->m_sCurX || i != pNpc->m_sCurY )
							{
								SetUid( j, i, 0 );
							}
							else
							{
								SendNpcInfo( pNpc, flag );
							}
						}
					}
				}
			}

			throwindex = pMap->m_pMap[j][i].iIndex;

			if( throwindex != -1 && throwindex < MAX_THROW_ITEM )
			{
				pThrowItem = m_pCom->m_ThrowItemArray[throwindex]->m_pItem;

				if( pThrowItem )
				{
					t = ConvertToClient( j, i );

					if( t.x != -1 && t.y != -1 )
					{
						AddItemFieldInfoToMe( pThrowItem, flag, t.x, t.y );
					}
				}
			}
		}
	}
}
*/
void USER::SendRangeInfoToMe(int min_x, int min_y, int max_x, int max_y, BYTE flag)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return;

	int i, j, tuid;
	int throwindex;

	CPoint t;

	USER *pUser = NULL;
	CNpc *pNpc = NULL;
	ItemList* pThrowItem = NULL;
	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap) return;

	int index = 0;
	int index_comp = 0;
	int count = 0;

	for( i = min_y; i < max_y; i++)
	{
		if( i >= pMap->m_sizeMap.cy || i < 0 )
		{
			continue;
		}

		for( j = min_x; j < max_x; j++)
		{
			if( j >= pMap->m_sizeMap.cx || j < 0 )
			{
				continue;
			}

			tuid = pMap->m_pMap[j][i].m_lUser;
//			tuid = GetUid( j, i );

			if( tuid != 0 )
			{
				if( tuid != m_uid + USER_BAND )
				{
					if( tuid >= USER_BAND && tuid < NPC_BAND )
					{
						pUser = GetUser( tuid - USER_BAND );
						if ( pUser == NULL ) continue;

						if(pUser->m_state != STATE_GAMESTARTED)
						{
							continue;
						}

						if( j != pUser->m_curx || i != pUser->m_cury )
						{
							SetUid(j, i, 0);
						}
						else
						{
//							SendUserInfo( pUser, flag );
							AddRangeInfoToMe( pUser, flag );
						}
					}
					else if( tuid >= NPC_BAND && tuid < INVALID_BAND)
					{
						pNpc = GetNpc( tuid - NPC_BAND );
						
						if( pNpc )
						{
							if( j != pNpc->m_sCurX || i != pNpc->m_sCurY )
							{
								if(pNpc->m_sDimension > 0) continue;

								SetUid( j, i, 0 );
							}
							else
							{
//								SendNpcInfo( pNpc, flag );
								AddRangeInfoToMe( pNpc, flag );
							}
						}
					}
				}
			}

			throwindex = pMap->m_pMap[j][i].iIndex;

			if( throwindex >= 0 && throwindex < MAX_THROW_ITEM )
			{
				// IKING 2001.1.
				if ( m_pCom->m_ThrowItemArray[throwindex] == NULL )
				{
					return;
				}
				//
				// IKING 2001.1.
				EnterCriticalSection( &m_pCom->m_critThrowItem );

				pThrowItem = m_pCom->m_ThrowItemArray[throwindex]->m_pItem;

				if( pThrowItem )
				{
					t = ConvertToClient( j, i );

					if( t.x != -1 && t.y != -1 )
					{
						if (flag > 4 || flag < 0)  //bug  坐标引起的报错
							return;
						if(t.x >= 10000 || t.y >= 10000)
							return;		//bug  坐标引起的报错
						AddItemFieldInfoToMe( pThrowItem, flag, t.x, t.y );
					}
				}

				LeaveCriticalSection( &m_pCom->m_critThrowItem );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	老沥 康开狼 蜡历俊霸 单捞磐甫 焊辰促.
//
void USER::SendToRange(char *pBuf, int index, int min_x, int min_y, int max_x, int max_y)
{
	if( index <= 0 || index >= SEND_BUF_SIZE ) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;

	if( !pNewData ) return;

	pNewData->flag = SEND_RANGE;
	pNewData->len = index;

	::CopyMemory( pNewData->pBuf, pBuf, index );

	pNewData->uid = 0;
	pNewData->z = m_curz;
	pNewData->rect.left		= min_x;
	pNewData->rect.right	= max_x;
	pNewData->rect.top		= min_y;
	pNewData->rect.bottom	= max_y;
	pNewData->zone_index = m_ZoneIndex;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	delete pNewData;
	//
}

////////////////////////////////////////////////////////////////////////////////////
//	Client 谅钎甫 辑滚谅钎肺 函券茄促
//
CPoint USER::ConvertToServer(int x, int y)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return CPoint(-1,-1);

	int tempx, tempy;
	int temph = g_zone[m_ZoneIndex]->m_vMoveCell.m_vDim.cy / 2 - 1;

	if(x <= -1 || y <= -1) return CPoint(-1,-1); 
	
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

CPoint USER::ConvertToServerByZone(int z, int x, int y)
{
	int i;
	int cx = -1, cy = -1;

	for( i = 0; i < g_zonesize.GetSize(); i++ )
	{
		if( g_zonesize[i] )
		{
			if( g_zonesize[i]->m_Zone == z )
			{
				cx = g_zonesize[i]->m_vMoveCell.m_vDim.cx;
				cy = g_zonesize[i]->m_vMoveCell.m_vDim.cy;

				break;
			}
		}
	}

	if( cx < 0 || cy < 0 ) return CPoint( -1, -1 );

	int tempx, tempy;
	int temph = cy / 2 - 1;

	if(x <= -1 || y <= -1) return CPoint(-1,-1); 
	if( y >= cy || x >= cx ) return CPoint(-1,-1);

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
//	蜡历 康开救俊 加茄 甘狼 醚 酒捞袍阑 滚欺俊 历厘茄促.  
//
void USER::AddItemFieldInfoToMe(ItemList *pItem, BYTE type, int x, int y)
{
/*
	if( !pItem && type == ITEM_INFO_MODIFY)
	{
		return;
	}

	if( m_ItemFieldInfoIndex >= 8000 )
	{
		SendItemFieldInfoToMe();
	}

	SetByte( m_ItemFieldInfoBuf, type, m_ItemFieldInfoIndex );

	SetShort( m_ItemFieldInfoBuf, x, m_ItemFieldInfoIndex );
	SetShort( m_ItemFieldInfoBuf, y, m_ItemFieldInfoIndex );

//	if(type == ITEM_INFO_MODIFY)
//	{
		if(pItem->tType == TYPE_ITEM)
		{
			SetShort( m_ItemFieldInfoBuf, pItem->sSid, m_ItemFieldInfoIndex );
			SetDWORD( m_ItemFieldInfoBuf, pItem->sCount, m_ItemFieldInfoIndex );
			SetByte( m_ItemFieldInfoBuf, pItem->tIQ, m_ItemFieldInfoIndex ); 
		}
		else
		{
			SetShort( m_ItemFieldInfoBuf, TYPE_MONEY_SID, m_ItemFieldInfoIndex );
			SetDWORD( m_ItemFieldInfoBuf, pItem->dwMoney, m_ItemFieldInfoIndex );
			SetByte( m_ItemFieldInfoBuf, 0, m_ItemFieldInfoIndex ); 
		}
//	}

	m_ItemFieldInfoCount++;
*/

	int index = 0;
	TCHAR pData[1024];

	SetByte( pData, FIELD_ITEM_INFO, index );
	SetShort( pData, (short)1, index );
	SetByte( pData, type, index );

	SetShort( pData, x, index );
	SetShort( pData, y, index );

	if(pItem->tType == TYPE_ITEM)
	{
		SetShort( pData, pItem->sSid, index );
		SetDWORD( pData, pItem->sCount, index );
		SetByte( pData, pItem->tIQ, index ); 
	}
	else
	{
		SetShort( pData, TYPE_MONEY_SID, index );
		SetDWORD( pData, pItem->dwMoney, index );
		SetByte( pData, 0, index ); 
	}

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

////////////////////////////////////////////////////////////////////////////////////
//	滚欺俊 乐绰 酒捞袍 沥焊甫 焊辰促.  
//
void USER::SendItemFieldInfoToMe()
{
	int index = 0;

	CBufferEx TempBuf;

	if( m_ItemFieldInfoCount <= 0 )
	{
		return;
	}

	if( m_ItemFieldInfoIndex <= 0 )
	{
		return;
	}

	TempBuf.Add(FIELD_ITEM_INFO);
	TempBuf.Add((short)m_ItemFieldInfoCount);
	TempBuf.AddData(m_ItemFieldInfoBuf, m_ItemFieldInfoIndex);

	Send(TempBuf, TempBuf.GetLength());

	m_ItemFieldInfoCount = 0;
	memset( m_ItemFieldInfoBuf, NULL, 8192 );
	m_ItemFieldInfoIndex = 0;
}

////////////////////////////////////////////////////////////////////////////////////
//	弥措 措固瘤甫 备茄促.
//  计算最大攻击到属性栏
int USER::GetMaxDamage(BYTE tWeaponClass)
{
	int nFinalDamage = 0;	// 弥辆单固瘤
	int xyz = 0;			// 公扁单固瘤	

	int tClass = tWeaponClass * SKILL_NUM;

	double iBasic = 0;			// 扁夯单固瘤

	int iSkillSid = 0;

	//if(tWeaponClass == FIREARMS){
	//	 iBasic = (int)((double)m_sMagicDEX/2 + 0.5);	
	//}else{
	//	iBasic = (int)((double)m_sMagicSTR/2 + 0.5);		
	//}
	//枪(敏捷+10)/2+机械敏捷转伤害
	//(力量+10)/2+机械力量转伤害
	switch(tWeaponClass)
	{
		//枪
	case FIREARMS:
		iBasic = (int)((double)(m_sMagicDEX + 10  ) / 2 + 0.5 + (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_DAMAGE] / 100 * m_sMagicDEX));
		break;
	case JUDGE://智力转伤害
		iBasic = (int)((double)(m_sMagicWIS)/2 + 0.5+(int)((double)m_DynamicEBodyData[EBODY_WIS_TO_DAMAGE] / 100 * m_sMagicWIS) );
		//iBasic+=(int)((double)(m_sMagicWIS)/2 + 0.5);
		break;
	default:
		iBasic = (int)((double)(m_sMagicSTR)/2 + 0.5+(int)((double)m_DynamicEBodyData[EBODY_STR_TO_DAMAGE] / 100 * m_sMagicSTR) );	
		break;
	}
      /////机械属性计算
	//iBasic += (int)((double)m_DynamicEBodyData[EBODY_STR_TO_DAMAGE] * m_sMagicSTR * 0.03);
	//iBasic += (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_DAMAGE] * m_sMagicDEX * 0.01);
    

	//智力变成远程攻击
	//if(m_byClass == JUDGE||m_byClass == FIREARMS)
	//{
		//iBasic+= (int)((double)m_DynamicEBodyData[EBODY_WIS_TO_DAMAGE] / 100 * m_sMagicWIS);
	//	iBasic=iBasic+(int)((double)(m_DynamicEBodyData[EBODY_WIS_TO_DAMAGE]* m_sMagicWIS) *(0.01) +0.5);
	//}
	
	if (m_byClass == 0 || m_byClass == 2 || m_byClass == 3 || m_byClass == 4)  //拳/剑/枪/仲裁135级魔法增加300物攻计算到属性栏
	{
		//if(m_dwFANTAnTime != 0) iBasic += 300;
		if(m_dwFENGLiTime != 0) iBasic += 150;
		if(m_dwDUOCHONgTime != 0) iBasic += 150;
		
	}
//	if ( m_byClass != 1) iBasic += m_sLevel* 2;   ///狼变加攻击 显示出来
	 

    if(iBasic < 0) iBasic = 0;

	if(tWeaponClass == 255) return (int)iBasic;

	xyz = GetWeaponDamage(m_UserItem[RIGHT_HAND].sSid, TRUE);

	nFinalDamage = (int)(iBasic + xyz);

	if(m_sLevel >= ADD_USER_LEVEL) nFinalDamage += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDamage;
	
	return nFinalDamage;
}

////////////////////////////////////////////////////////////////////////////////////
//	各 傍拜俊 措茄 蜡历狼 馆拜 措固瘤甫 拌魂茄促.
//
void USER::SetCounterAttack(int uid, int damage)
{
	for(int i = 0; i < COUNTERATTACK_MAX_NUM; i++)
	{
		if(InterlockedCompareExchange((LONG*)&m_CounterAttackList[i].lSid, (long)1, (long)0) == (long)0)
		{
			m_CounterAttackList[i].iNpcUid = uid;
			m_CounterAttackList[i].iDamage = damage;
			m_CounterAttackList[i].dwLastAttackTime = GetTickCount();
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//	叭扁 悼累矫狼 SPEED HACK 荤侩蜡公甫 眉农茄促.
//  服务器变速相关
BOOL USER::CheckMoveSpeedHack(DWORD checkTick)
{
////////////////////////////////原来变速检测代码////////////////////////////////////////////
//	if((m_UserItem[1].sSid == 1489) || (m_UserItem[1].sSid == 1488) || (m_UserItem[1].sSid == 1479) || (m_UserItem[1].sSid == 1356) || (m_UserItem[1].sSid == 1787) || (m_UserItem[1].sSid == 1788)) return FALSE;//代步工具不检测速度
//	DWORD curTick = GetTickCount();
//    DWORD tmpS = 0;
//	if( m_iTickCount == 0 )
//	{
//		m_dLastCheckTick = checkTick;
//		m_dCheckTick = curTick;
//		m_dTotCheckTick = 0;
//		
//		m_iTickCount += 1;
//	}
//	else if( m_iTickCount == 5 )
//	{
///////////////////////////////Release编译修改处开始//////////////////////////////////
//		DWORD client = abs((int)checkTick - (int)m_dLastCheckTick );
//		DWORD server = abs((int)curTick - (int)m_dCheckTick );
///////////////////////////////Release编译修改处结束//////////////////////////////////
//
///////////////////////////////Debug编译修改处开始//////////////////////////////////
////		DWORD client = abs( checkTick - m_dLastCheckTick );
////		DWORD server = abs( curTick - m_dCheckTick );
///////////////////////////////Debug编译修改处结束//////////////////////////////////
//		double gap = (double)client / (double)server;
//
//		m_dTotCheckTick -= m_dTotCheckTick / 20;
//		if( ( gap < 0.75 || gap > 1.25 ) && ( server < m_dTotCheckTick ) )
//		{
//			SYSTEMTIME time;
//			GetLocalTime(&time);
//
//			CString str;
//			str.Format( IDS_USER_SYSTEM_MSG01,
//				time.wMonth, time.wDay, time.wHour, time.wMinute, m_strUserID, server, client, m_dTotCheckTick );
//			g_fpSpeedHack.Write( str, str.GetLength() );
//			tmpS = server * 2;
//			if ( m_dTotCheckTick >= tmpS) 
//			{
//            USER* pUser = GetUser(m_strUserID);
//            if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return TRUE;
//			//pUser->SoftClose();//上线有移动速度就要注释掉这句,作用:解决移动速度过快掉线的毛病.
//			m_iTickCount = 0;
//			return TRUE;
//			}
//			else
//			  m_iTickCount += 1;
//		}
//		m_iTickCount = 0;
//	}
//	else
//	{
//		m_dTotCheckTick += CLIENT_WALK_TICK;
//		m_iTickCount += 1;
//	}
//	
//	return FALSE;
////////////////////////////////原来变速检测代码结束////////////////////////////////////////////

////////////////////////////////现在封变速检测代码开始////////////////////////////////////////////
	if((m_UserItem[1].sSid == 1489) || (m_UserItem[1].sSid == 1488) || (m_UserItem[1].sSid == 1479) || (m_UserItem[1].sSid == 1356) || (m_UserItem[1].sSid == 1787) || (m_UserItem[1].sSid == 1788)) return FALSE;//代步工具不检测速度
    if (m_bSpeedHackEnble && (GetTickCount() - m_dLastCheckTick) < 1550 )
	{
		//SendSystemMsg("请关闭[变速器],否则导致您无法移动!", SYSTEM_SPECIAL, TO_ME);
		SendSystemMsg("您使用[变速器]或[网络很卡],导致无法移动!", SYSTEM_NPC, TO_ME);
		return TRUE;
	}
	if( m_iTickCount == 0 )
	{
		m_dCheckTick = GetTickCount();
	}

	m_iTickCount += 1;

	if( m_iTickCount == 10 )
	{
		unsigned int server = GetTickCount() - m_dCheckTick;
		
		TRACE("move : %d\r\n",server);
		m_iTickCount = 0;

		if (server < checkTick)
		{
			int lastTime = GetTickCount() - m_dwServerTick;
			m_dwServerTick = GetTickCount();

			if (lastTime <= 1800 )
			{
				 m_dLastCheckTick = GetTickCount();
				m_bSpeedHackEnble = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
////////////////////////////////现在封变速检测代码结束////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////////////
//	钝锭 SPEED HACK 荤侩蜡公甫 眉农茄促.
//检查移动速度
BOOL USER::CheckRunSpeedHack(DWORD checkTick, BOOL bTwo)
{
////////////////////////////////原来移动速度检测代码////////////////////////////////////////////
//	DWORD curTick = GetTickCount();
//    DWORD tmpS = 0;
//	if( m_iTickCount == 0 )
//	{
//		m_dLastCheckTick = checkTick;
//		m_dCheckTick = curTick;
//		m_dTotCheckTick = 0;
//		
//		m_iTickCount += 1;
//	}
//	else if( m_iTickCount == 5 )
//	{
///////////////////////////////Release编译修改处开始//////////////////////////////////
//		DWORD client = abs((int)checkTick - (int)m_dLastCheckTick );
//		DWORD server = abs((int) curTick - (int)m_dCheckTick );
///////////////////////////////Release编译修改处结束//////////////////////////////////
//
///////////////////////////////Debug编译修改处开始//////////////////////////////////
////		DWORD client = abs( checkTick - m_dLastCheckTick );
////		DWORD server = abs( curTick - m_dCheckTick );
///////////////////////////////Debug编译修改处结束//////////////////////////////////
//		double gap = (double)client / (double)server;
//
//		m_dTotCheckTick -= m_dTotCheckTick / 20;
//		if( ( gap < 0.75 || gap > 1.25 ) && ( server < m_dTotCheckTick ) )
//		{
//			SYSTEMTIME time;
//			GetLocalTime(&time);
//
//			CString str;
//			str.Format( IDS_USER_SYSTEM_MSG01,
//				time.wMonth, time.wDay, time.wHour, time.wMinute, m_strUserID, server, client, m_dTotCheckTick );
//			g_fpSpeedHack.Write( str, str.GetLength() );
//			tmpS=server*2;
//            if (m_dTotCheckTick >= tmpS )  
//			{
//            USER* pUser = GetUser(m_strUserID);
//            if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return TRUE;
//			//pUser->SoftClose();
//			UpdateUserData(TRUE);
//			m_iTickCount = 0;
//			return TRUE;
//			}
//			else
//            	m_iTickCount += 1;
//		}
//		m_iTickCount = 0;
//	}
//	else
//	{
//		m_dTotCheckTick += CLIENT_RUN_TICK;
//		if(bTwo) m_dTotCheckTick += CLIENT_RUN_TICK;
//		m_iTickCount += 1;
//	}
//
//	return FALSE;
////////////////////////////////原来移动速度检测代码结束////////////////////////////////////////////

////////////////////////////////现在封变速加移动速度检测代码////////////////////////////////////////////
	return CheckMoveSpeedHack(880);
////////////////////////////////现在封变速加移动速度检测代码结束////////////////////////////////////////////

}

///////////////////////////////////////////////////////////////////////////////////////
//	惑措规俊霸 鸥拜阑临锭 沥寸规困咯何甫 魄窜.
//
void USER::IsLegalDefence(USER *pUser)
{
	CBufferEx TempBuf;

	int rank = CAIN_RANK - CITY_RANK_INTERVAL;			// -1 何磐绰 沥寸规困己赋
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_sCityRank <= rank || pUser->m_bPkStatus) return;
	if(pUser->m_curz == 67 || pUser->m_curz == 66 || pUser->m_curz == 45 || pUser->m_curz == 40 || pUser->m_curz == 43 || pUser->m_curz == 44 || pUser->m_curz == 58) return;		//PK竞赛//PKC不红名

	if(CheckInvalidZoneInGuildWar(pUser)) return;
	if(CheckInvalidZoneInFreeFight(pUser)) return;

	m_bPkStatus = TRUE;				// 瘤陛 墨坷惑怕促.
	m_dwPkStatusTime = GetTickCount();
	
	TempBuf.Add(SET_USER_PK_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((BYTE)0x02);
	TempBuf.Add((BYTE)m_bPkStatus);

	SendInsight(TempBuf, TempBuf.GetLength());
//	SendExactScreen(TempBuf, TempBuf.GetLength());
}

//yskang 0.7 辨靛傈矫 惑措 辨靛盔 磷捞搁 墨坷达
BOOL USER::IsLegalDefence_isdead(USER *pUser)
{
	CBufferEx TempBuf;

	int rank = CAIN_RANK - CITY_RANK_INTERVAL;			// -1 何磐绰 沥寸规困己赋
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;
	if(pUser->m_sCityRank <= rank || pUser->m_bPkStatus) return FALSE;
	if(pUser->m_curz == 67 || pUser->m_curz == 66 || pUser->m_curz == 45 || pUser->m_curz == 40 || pUser->m_curz == 43 || pUser->m_curz == 44 || pUser->m_curz == 58) return FALSE;		//PK竞赛//PKC不红名

	if(CheckInvalidZoneInGuildWar(pUser)) return TRUE;
	if(CheckInvalidZoneInFreeFight(pUser)) return TRUE;

	int type = 0;
	type = CheckInvalidMapType();
	if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING) //只有审请军团才不红名,别的跟平时一样.
	if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16) return FALSE;  m_bPkStatus = FALSE; 	
	m_bPkStatus = TRUE;				// 决定PK红名
	m_dwPkStatusTime = GetTickCount();
	
	TempBuf.Add(SET_USER_PK_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((BYTE)0x02);
	TempBuf.Add((BYTE)m_bPkStatus);

	SendInsight(TempBuf, TempBuf.GetLength());
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////
//	瘤陛 唱唱 惑措 蜡历啊 辨靛傈 吝捞搁 己氢俊 康氢阑 罐瘤 臼档废茄促 
//
BOOL USER::CheckInvalidZoneInGuildWar(USER *pUser)
{	
	int me, you, type;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;

	type = CheckInvalidMapType();

	int mapindex = GetGuildMapIndex(type);
	if(mapindex <= -1 || mapindex >= g_arMapTable.GetSize()) return FALSE;

	if(!CheckInvalidZoneState(type)) // 傍己傈 肚绰 鞘靛 惑痢傈老锭绰 备开栏肺 公炼扒...
	{
		me = g_arMapTable[mapindex]->m_sStoreID;

		type = pUser->CheckInvalidMapType();
		mapindex = GetGuildMapIndex(type);
		if(mapindex <= -1 || mapindex >= g_arMapTable.GetSize()) return FALSE;

		you = g_arMapTable[mapindex]->m_sStoreID;

		if(me == you) return TRUE;				// 鞍篮 惑痢 牢郸胶搁 葛滴 焊扼倒捞啊 酒丛...
	}

	if(m_tGuildWar == GUILD_WARRING) //|| m_tFortressWar == GUILD_WARRING)					// 郴啊 辨靛傈 吝捞绊
	{
		if(m_dwFieldWar > 0) 
		{												// 鞘靛傈吝捞搁 沥寸规困		
			if(pUser->m_dwGuild == m_dwFieldWar) 
			{
				CString strMsg = _T("");				// 惑措 辨付啊 磷栏搁 霸烙 场..
				if(pUser->m_bLive == USER_DEAD && pUser->m_bGuildMaster)
				{
					strMsg.Format( IDS_USER_GUILD_WAR_WINNER, m_strGuildName);
					SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
//	泅犁 乐绰 镑捞 措访厘捞搁 己氢俊 康氢绝澜
//	TRUE - 泅犁 乐绰 镑捞 措访厘 酒丛, FALSE - 措访厘捞芭唱 坷幅
BOOL USER::CheckInvalidZoneInFreeFight(USER *pUser)
{
	int me, you;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;

	me = CheckInvalidMapType();
	you = CheckInvalidMapType();

	if( me == 12 && you == me ) return TRUE;

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
//	矫刮殿鞭阑 炼例茄促.
//
void USER::IsChangeCityRank(int iMyCityRank, USER *pUser)
{
	// PK肺 磷菌阑锭贸府
	CBufferEx TempBuf;

	int step = 0;
	int rank = 0;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_bLive != USER_DEAD || pUser->m_sHP > 0) return;	// 趣 磷瘤档 臼疽绰单 坷搁 滚弊...
	if (pUser->m_curz == 67 || pUser->m_curz == 66)	return;
											// 弥快急利栏肺 傈里阑 八荤	
	if(CheckInvalidZoneInGuildWar(pUser)) return;
	if(CheckInvalidMapType() == 12 ) return;	// 措访厘 八荤

	
	if(pUser->m_bPkStatus || pUser->m_sCityRank < 0)
	{
		pUser->SendCityRank(iMyCityRank);
		return;
	}

	pUser->SendCityRank(iMyCityRank);
	
	if(m_sCityRank >= 0)					 // 酒流鳖瘤 急且锭
	{
		m_iCityValue = 0;					

		if(pUser->m_sLevel > m_sLevel)  step = 0;		// 憋绰 荐摹绰 惑措规 饭骇俊蝶扼... 
		else if(pUser->m_sLevel < m_sLevel) step = 2;
		else  step = 1;

		rank = 4;	

		m_iCityValue -= g_CityRank[rank][step];///杀人后减去市民等级
		SendUserStatusSkill();
	}
	else												// 厩老锭
	{
		if(pUser->m_sLevel > m_sLevel)  step = 0;
		else if(pUser->m_sLevel < m_sLevel) step = 2;
		else  step = 1;

		rank = m_sCityRank + CITY_RANK_INTERVAL;	// 歹窍绰 捞蜡绰 硅凯捞 0何磐 矫累窍扁锭巩
		
		m_iCityValue = -1 * m_iCityValue;			// 捞扒 蔼阑 歹窍扁困秦 老矫利栏肺 窃(//&&&&&&&&&唱吝俊 绊摹磊 ^^)
		CheckMaxValue((int &)m_iCityValue, (int &)g_CityRank[rank][step]);
		m_iCityValue = -1 * m_iCityValue;			// 夸扁辑 官操隆...
		if(m_iCityValue < -2000000000) m_iCityValue = -2000000000;  // 捞犯霸 函沁栏聪瘪 拉临档 官层具茄促...^^
		SendUserStatusSkill();///杀人后减去市民等级
	}

	CheckMaxValue(m_sKillCount, 1);					// 穿利蔼捞 拌加 刘啊茄促.
													// 弥厩殿鞭老锭
	int oldRank = m_sCityRank;			
	for(int i = 0; i < 11; i++)
	{
		if(m_iCityValue < g_CityRankValue[i])// 穿利摹蔼捞 扁霖 汲沥蔼焊促 累篮瘤 魄窜茄促.
		{
			m_sCityRank = i - CITY_RANK_INTERVAL;
			break;
		}
	}

	if(oldRank != m_sCityRank) 
	{
		CheckGuildUserInFortress();			// 矫刮 殿鞭捞 函且锭 促矫 悸泼(器飘府胶甫困秦)

		TempBuf.Add(SET_USER_PK_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add((BYTE)0x01);
		TempBuf.Add(m_sCityRank);

		SendInsight(TempBuf, TempBuf.GetLength());
//		SendExactScreen(TempBuf, TempBuf.GetLength());	
	}

	rank = m_sCityRank + CITY_RANK_INTERVAL;
	if( rank != ANGEL_RANK )		// 绢骂 函沁电 技牢飘啊 酒聪扼搁
	{
		m_dwSaintTime = 0;
	}
}	

///////////////////////////////////////////////////////////////////////////////////////
//	咯变 各阑 磷看阑锭 矫刮殿鞭 氢惑甫 炼例茄促.
//
void USER::GetCityRank()
{
	CBufferEx TempBuf;

	int		i;
	BOOL	bRank = FALSE;
	int		iCityRank = m_sCityRank;

	for(i = 0; i < 11; i++)//13有天使，11圣人
	{
		if(m_iCityValue < g_CityRankValue[i])// 穿利摹蔼捞 扁霖 汲沥蔼焊促 累篮瘤 魄窜茄促.
		{
			if(i < 6) { m_sCityRank = i - CITY_RANK_INTERVAL; }
			else { m_sCityRank = i -1 - CITY_RANK_INTERVAL; }
			break;
		}
	}

	//if(m_iCityValue >= g_CityRankValue[12]) m_sCityRank = 7;//到天使
	if(m_iCityValue >= g_CityRankValue[10]) m_sCityRank = CITY_RANK_INTERVAL;//到圣人

	if(iCityRank != m_sCityRank)
	{
		CheckGuildUserInFortress();			// 矫刮 殿鞭捞 函且锭 促矫 悸泼(器飘府胶甫困秦)

		TempBuf.Add(SET_USER_PK_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add((BYTE)0x01);
		TempBuf.Add(m_sCityRank);

		SendInsight(TempBuf, TempBuf.GetLength());
//		SendExactScreen(TempBuf, TempBuf.GetLength());
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//	阿 矫刮殿鞭喊肺 拱扒蔼阑 蝶肺 汲沥茄促.
//
DWORD USER::GetItemCostByCityRank(int sid, int nRate)
{
	if(sid <0 || sid >= g_arItemTable.GetSize()) return 10000000; 
		
	int iCost = g_arItemTable[sid]->m_iDN;
	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	iCost = iCost + (int)( iCost * ((double)nRate/100) );
	switch(rank)
	{
	case CHAONISE_RANK:
		iCost += (int)( iCost * ((double)25/100) );
		break;
	case DEATH_RANK:
		iCost += (int)( iCost * ((double)20/100) );
		break;
	case EVIL_RANK:
		iCost += (int)( iCost * ((double)15/100) );
		break;
	case VILLAIN_RANK:
		iCost += (int)( iCost * ((double)10/100) );
		break;
	case WARRIOR_RANK:
		iCost -= (int)( iCost * ((double)3/100) );
		break;
	case HERO_RANK:
		iCost -= (int)( iCost * ((double)6/100) );
		break;
	case SAINT_RANK:
		iCost -= (int)( iCost * ((double)10/100) );
		break;
	//case GUARDANT_RANT: //圣贤
	//	iCost -= (int)( iCost * ((double)12/100) );
	//	break;
	//case ANGEL_RANK://天使
	//	iCost -= (int)( iCost * ((double)15/100) );
	//	break;
	}

	if(iCost < 0) iCost = 1;

	return (DWORD)iCost;
}

///////////////////////////////////////////////////////////////////////////////////////
//	酒捞袍苞 捣俊包茄 DB Update
//
BOOL USER::UpdateUserItemDN()
{
	if(UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strItem, sizeof(strItem));
	
	UserItemToStr(strItem);

	SDWORD sItemLen		= sizeof(strItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_ITEM_DATA (\'%s\',%d, ?)}"), m_strUserID, m_dwDN ); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );

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
BOOL USER::UpdateUserItemShopPingDN()
{
	if(UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strItem, sizeof(strItem));
	
	UserItemToStr(strItem);

	SDWORD sItemLen		= sizeof(strItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_ITEM_DATA (\'%s\',%d, ?)}"), m_strUserID, m_dwShopPingDN ); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );

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
BOOL USER::UpdateUserItemShopPattern()//保存和存储转换商店模式数据
{
	if(UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strItem, sizeof(strItem));
	
	UserItemToStr(strItem);

	SDWORD sItemLen		= sizeof(strItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_ITEM_DATA (\'%s\',%d, ?)}"), m_strUserID, m_dwNoChatTime); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );

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
BOOL USER::UpdateUserItemZaiXianTime()//保存在线时间数据
{
	if(UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strItem, sizeof(strItem));
	
	UserItemToStr(strItem);

	SDWORD sItemLen		= sizeof(strItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_ITEM_DATA (\'%s\',%d, ?)}"), m_strUserID, m_dwZaiXianTime ); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );

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

BOOL USER::UpdateUserItemHiExpTime()
{
	if(UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strItem, sizeof(strItem));
	
	UserItemToStr(strItem);

	SDWORD sItemLen		= sizeof(strItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_ITEM_DATA (\'%s\',%d, ?)}"), m_strUserID, m_dwHiExpTime ); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );

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
BOOL USER::UpdateUserItemMagicFindTime()
{
	if(UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strItem, sizeof(strItem));
	
	UserItemToStr(strItem);

	SDWORD sItemLen		= sizeof(strItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_ITEM_DATA (\'%s\',%d, ?)}"), m_strUserID, m_dwMagicFindTime ); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );

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
////////////////////////////////////////////////////////////////////////////////
//	取得法师精修伤害
//
int USER::GetPsiAttackUp()
{
	int iLevel = 0;
	int iSkillSid = 0;

	int iAttackUp = 0;
	int tClass = 0;

	BYTE tWeaponClass = 0;

	IsCanUseWeaponSkill(tWeaponClass);
	tClass = tWeaponClass * SKILL_NUM;

	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid  = m_UserSkill[i].sSid;
		if(iSkillSid == SKILL_PSI_SUCCESS_UP) // 己傍伏 刘啊		7 index
		{
			// 鉴荐 胶懦 饭骇 
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;
			
			// 酒捞袍俊 狼茄 胶懦 函悼 饭骇
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;		// 叼厚俊 乐绰 饭骇焊促绰 累酒具茄促
			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			iAttackUp = g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);
		}
	}

	return iAttackUp;	
}

////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 裹困 傍拜俊 措茄 单固瘤甫 拌魂茄促.
//
void USER::GetWideRangeAttack(int x, int y, int damage)	// 瘤陛篮 傍拜父 贸府...
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return;

	int dir[9][2];
	int ix, iy;
	int nTarget = 0;
	int nDamage = 0;

	USER* pUser = NULL;
	CNpc* pNpc = NULL;
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
			pUser = GetUser(nTarget - USER_BAND);			// User Pointer 甫 掘绰促.
			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;	
			if(pUser->m_bLive == USER_DEAD || pUser->m_uid == m_uid)	continue;		// Target User 啊 捞固 磷绢乐栏搁 府畔
			if(!pUser->m_bPkStatus) continue;				// 老矫利 墨坷啊 酒聪搁 傍拜捞 救等促.

			if(m_dwGuild > 0)
			{
				if(m_tGuildWar == GUILD_WARRING && pUser->m_tGuildWar == GUILD_WARRING)
				{												
					if(pUser->m_dwGuild == m_dwGuild)  return;
				}

				if(m_tFortressWar == GUILD_WARRING && pUser->m_tFortressWar == GUILD_WARRING)
				{												// 辨靛傈老锭 老矫利 墨坷绰 鞍篮辨靛盔俊霸绰 公矫
					if(pUser->m_dwGuild == m_dwGuild)  return;
				}																			
			}

			nDamage = (int)((double)damage *  ((double)m_sMagicVOL / (m_sMagicVOL + pUser->m_sMagicVOL + pUser->m_DynamicUserData[MAGIC_PSI_RESIST_UP] + \
				                               (int)((double)pUser->m_DynamicEBodyData[EBODY_PSI_RESIST_UP] / 100) \
											  )));
			//最终伤害统计
			nDamage -= pUser->m_DynamicUserData[MAGIC_PSI_ATTACK_DOWN];
			nDamage -= pUser->m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN];
			nDamage = (int)((double)nDamage/2 + 0.5);	// 单固瘤狼 50%父 甸绢埃促.
			
			pUser->SetDamage(nDamage);
			if(pUser->m_sHP > 0)		// 混篮 版快 傈扁单固瘤 眠啊
			{
//				pUser->SetColdDamage();
			}
			else if(pUser->m_lDeadUsed == 1)
			{
				int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
				IsChangeCityRank(m_sCityRank, pUser);
				pUser->GetLevelDownExp(USER_PK, tempRank, FALSE,m_strUserID);			// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
			}
		}
		else if(nTarget >= NPC_BAND)				// NPC
		{
			pNpc = GetNpc(nTarget - NPC_BAND);				// NPC Point 甫 掘绰促.
			if(pNpc == NULL) continue;					// 肋给等 NPC 捞搁 府畔
			if(pNpc->m_NpcState == NPC_DEAD || pNpc->m_tNpcType != NPCTYPE_MONSTER) continue;	// NPC 啊 捞固 磷绢 乐栏搁 府畔
			if(pNpc->m_sHP <= 0) continue;
			if(pNpc->m_byAX == 0 && pNpc->m_byAZ == 0 && pNpc->m_tNpcType == 0) continue;			// 傍拜瓷仿捞 绝绰 阁胶磐(泅犁 盔籍)篮 老馆 傍拜捞 登瘤 臼绰促.

			nDamage = (int)(damage *  ((double)m_sMagicVOL / (m_sMagicVOL + pNpc->m_sVOL)));
			nDamage = (int)((double)nDamage/2 + 0.5);	// 单固瘤狼 50%父 甸绢埃促.

//			if(pNpc->SetDamage(nDamage, m_strUserID, m_uid + USER_BAND, m_pCom) == FALSE)
			if(pNpc->SetDamage(nDamage, m_uid + USER_BAND, m_pCom) == FALSE)
			{
				if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
				{
					CheckGuildHouseWarEnd();
				}

				pNpc->SendExpToUserList(m_pCom); // 版氰摹 盒硅!!
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(diffLevel <= o_yehuoini[0]->djxz || pNpc->m_sEvent >= 30000 )
				{
					if (m_sLevel>= 70)
						m_dwShopPingDN += 1;//每杀死怪物增加杀怪物数
					if (m_isDoubleBAOLV  > 0 || g_sanBaoLv == TRUE || m_dwGuarDianTianShi > 0)
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
				
				if(diffLevel < 40)
				{
					m_iCityValue=m_iCityValue+250;
					GetCityRank();
					CheckMaxValue(m_dwXP, 1);		// 各捞 磷阑锭父 1 刘啊!	
					SendXP();
					SendUserStatusSkill();
				}
			}
			else									// 混篮 版快 傈扁单固瘤 眠啊
			{
//				pNpc->SetColdDamage();
			}
		    SendDamageNum(1,nTarget,nDamage);
			SendNpcHP(nTarget,pNpc->m_sHP);	
			 
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//	家券且 各狼 单捞鸥蔼阑 悸泼茄促.
//
void USER::GetNpcData(CNpc *pNpc, int x, int y)
{
	int  iCount = 0,i;
	CPoint pt;

	if(m_tIsOP != 1) return;

	CNpc* pEventNpc	= GetEventNpc();

	if(pEventNpc == NULL)
	{
		SendSystemMsg( IDS_USER_CALL_MONSTER_COUNT_MAX, SYSTEM_NORMAL, TO_ME);
		return;
	}

	MYSHORT sAI;
	BYTE upTemp = 0;			// 惑困 8厚飘
	BYTE dwTemp = 0;			// 窍困 8厚飘

	CString strMsg = _T(""); 

	pEventNpc->m_sSid			= pNpc->m_sSid;		// MONSTER(NPC) Serial ID

	pEventNpc->m_sPid			= pNpc->m_sPid;		// MONSTER(NPC) Picture ID
	_tcscpy(pEventNpc->m_strName, pNpc->m_strName);		// MONSTER(NPC) Name
	
	pEventNpc->m_sSTR			= pNpc->m_sSTR;		// 塞
	pEventNpc->m_sDEX			= pNpc->m_sDEX;		// 刮酶
	pEventNpc->m_sVOL			= pNpc->m_sVOL;		// 狼瘤
	pEventNpc->m_sWIS			= pNpc->m_sWIS;		// 瘤驱
	
	pEventNpc->m_sHP			= pNpc->m_sMaxHP;		// 弥措 HP
	pEventNpc->m_sMaxHP			= pNpc->m_sMaxHP;		// 泅犁 HP
	pEventNpc->m_sPP			= pNpc->m_sMaxPP;		// 弥措 PP
	pEventNpc->m_sMaxPP			= pNpc->m_sMaxPP;		// 泅犁 PP
	
	pEventNpc->m_byClass		= pNpc->m_byClass;		// 公扁拌凯
	pEventNpc->m_byClassLevel	= pNpc->m_byClassLevel;// 公扁拌凯 饭骇
	pEventNpc->m_sExp			= pNpc->m_sExp;		
	
	pEventNpc->m_byAX			= pNpc->m_byAX;		// 傍拜蔼 X
	pEventNpc->m_byAY			= pNpc->m_byAY;		// 傍拜蔼 Y
	pEventNpc->m_byAZ			= pNpc->m_byAZ;		// 傍拜蔼 Z

	pEventNpc->m_iDefense		= pNpc->m_iDefense;	// 规绢蔼
	pEventNpc->m_byRange		= pNpc->m_byRange;		// 荤沥芭府
	pEventNpc->m_sAI			= pNpc->m_sAI;		// 牢傍瘤瓷 牢郸胶
	pEventNpc->m_sAttackDelay	= pNpc->m_sAttackDelay;// 傍拜掉饭捞
	pEventNpc->m_byVitalC		= pNpc->m_byVitalC;	// 脚眉单固瘤 农府萍拿
	pEventNpc->m_byWildShot		= pNpc->m_byWildShot;	// 抄荤 饭骇
	pEventNpc->m_byExcitedRate	= pNpc->m_byExcitedRate;			// 蕊盒 饭骇
	pEventNpc->m_byIronSkin		= pNpc->m_byIronSkin;
	pEventNpc->m_byReAttack		= pNpc->m_byReAttack;
	pEventNpc->m_bySubAttack	= pNpc->m_bySubAttack;	// 惑怕捞惑 惯积(何啊傍拜)
	pEventNpc->m_byState		= pNpc->m_byState;		// 阁胶磐 (NPC) 惑怕捞惑
	pEventNpc->m_byPsi			= pNpc->m_byPsi;		// 荤捞坷葱 利侩
	pEventNpc->m_byPsiLevel		= pNpc->m_byPsiLevel;	// 荤捞坷葱饭骇

	pEventNpc->m_bySearchRange	= pNpc->m_bySearchRange;			// 利 沤瘤 裹困
	pEventNpc->m_sSpeed			= pNpc->m_sSpeed;		// 捞悼加档	
	
	pEventNpc->m_sInclination	= pNpc->m_sInclination;
	pEventNpc->m_byColor		= pNpc->m_byColor;
	pEventNpc->m_sStandTime		= pNpc->m_sStandTime;

	//////// MONSTER POS ////////////////////////////////////////
	pEventNpc->m_sCurZ			= pEventNpc->m_sOrgZ = m_curz;
	pEventNpc->m_sCurX			= pEventNpc->m_sOrgX = m_curx;
	pEventNpc->m_sCurY			= pEventNpc->m_sOrgY = m_cury;
	
	pEventNpc->m_sMinX			= pNpc->m_sMinX;
	pEventNpc->m_sMinY			= pNpc->m_sMinY;
	pEventNpc->m_sMaxX			= pNpc->m_sMaxX;
	pEventNpc->m_sMaxY			= pNpc->m_sMaxY;
	
	pEventNpc->m_sRegenTime		= pNpc->m_sRegenTime;

	pEventNpc->m_sEvent			= pNpc->m_sEvent;		// 捞亥飘 锅龋
	pEventNpc->m_sEZone			= pNpc->m_sEZone;
	pEventNpc->m_byType			= pNpc->m_byType;
	pEventNpc->m_sDimension		= pNpc->m_sDimension;	// 泅犁 割伎甫 瞒瘤窍绰瘤 魄窜

	pEventNpc->m_tNpcType		= pNpc->m_tNpcType;	// NPC Type

	pEventNpc->m_sFamilyType	= pNpc->m_sFamilyType;	// NPC Type
	pEventNpc->m_tItemPer		= pNpc->m_tItemPer;	// NPC Type
	pEventNpc->m_tDnPer			= pNpc->m_tDnPer;	// NPC Type
	pEventNpc->m_sHaveItem		= pNpc->m_sHaveItem;

	pEventNpc->m_ZoneIndex		= -1;

	pEventNpc->m_sClientSpeed	= pNpc->m_sClientSpeed;
	pEventNpc->m_dwStepDelay	= GetTickCount();

	sAI.i = (short)pNpc->m_sAI;						// NPC AI甫 悸泼
	upTemp = sAI.b[0];
	dwTemp = sAI.b[1];

	pEventNpc->m_tNpcAttType = upTemp >> 7;			// 唱吝俊 眠啊秦具茄促.
	upTemp = upTemp << 1;
	pEventNpc->m_tNpcLongType = upTemp >> 7;
	upTemp = upTemp << 1;
	pEventNpc->m_tNpcGroupType = upTemp >> 7;

	if(pEventNpc->m_sClientSpeed <= 20) pEventNpc->m_sClientSpeed = 20;	// 规绢 内靛;

	for(i = 0; i < g_zone.GetSize(); i++)
	{
		if(g_zone[i]->m_Zone == pEventNpc->m_sCurZ) 
		{
			pEventNpc->m_ZoneIndex = i;
			break;
		}
	}

	BOOL bSuccess = FALSE;
	pEventNpc->EventNpcInit(x, y);
	
	for(i = 0; i < NPC_NUM; i++ ) 
	{
		if(InterlockedCompareExchange((LONG*)&g_arEventNpcThread[0]->m_ThreadInfo.m_lNpcUsed[i], (long)1, (long)0) == (long)0)
		{
			bSuccess = TRUE;
			g_arEventNpcThread[0]->m_ThreadInfo.pNpc[i] = pEventNpc;
			break;
		}
	}

	if(!bSuccess)
	{
		InterlockedExchange(&pEventNpc->m_lEventNpc, (LONG)0);
		SendSystemMsg( IDS_USER_CALL_FAIL, SYSTEM_NORMAL, TO_ME);
		return;
	}

	strMsg.Format( IDS_USER_CALL_WHAT, pNpc->m_strName);
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
}

////////////////////////////////////////////////////////////////////////////////
//	眠啊且 家券各狼 皋葛府甫 曼炼窍扁困秦 敲贰弊啊 0牢 惑怕巴父 逞变促.
//
CNpc* USER::GetEventNpc()
{
	for(int i = g_TotalNPC; i < g_arNpc.GetSize(); i++)
	{
		if( g_arNpc[i] )
		{
			if(InterlockedCompareExchange((LONG*)&g_arNpc[i]->m_lEventNpc, (long)1, (long)0) == (long)0)
			{
				return g_arNpc[i];				
			}
		}
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//	醚扁拌凯狼 篮脚 秦力OFF
//
void USER::SendHideOff()
{
/*	CBufferEx TempBuf;

	m_tPsiAbnormal = 0;
	m_dwHideTime = 0;
	m_dwLastHideTime = GetTickCount();
		
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(ABNORAML_PSI);
	TempBuf.Add(ABNORMAL_NONE);
	TempBuf.Add((BYTE)0);
	TempBuf.Add((BYTE)0);
	TempBuf.Add((BYTE)0);
	TempBuf.Add((BYTE)0);	
	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////////
//	蜡历啊 家瘤茄 酒捞袍狼 公霸 函拳甫 舅赴促.
//
void USER::SendItemWeightChange()
{
/*	CBufferEx TempBuf;

	TempBuf.Add(ITEM_WEIGHT_CHANGE);
	TempBuf.Add((short)m_iMaxWeight);
	TempBuf.Add((short)m_iCurWeight);

	Send(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////////
//	秦寸 辨靛牢郸胶 狼 器牢磐甫 馆券
//
CGuild* USER::GetGuild(int num)
{
	EnterCriticalSection( &(m_pCom->m_critGuild) );

	if( num <= 0 ) return NULL;			// 辨靛 牢郸胶绰 1何磐 矫累茄促.
	if( num >= g_arGuildData.GetSize() ) return NULL; 

	CGuild* pGuild = NULL;

	if(g_arGuildData[num])
	{
		pGuild = g_arGuildData[num];
		if( pGuild->m_lSid == num )
		{
			return pGuild;
		}
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//	秦寸 辨靛捞抚栏肺 器牢磐馆券
//
CGuild* USER::GetGuildByName(TCHAR *guildname)
{
	EnterCriticalSection( &(m_pCom->m_critGuild) );

	int i, j = 0;
	CGuild* pGuild = NULL;

	CString tempName;

	tempName.Format( "%s", guildname );

	for(i = 0; i < g_arGuildData.GetSize(); i++)
	{
		if( g_arGuildData[i] )
		{
			pGuild = g_arGuildData[i];

			if( !tempName.CompareNoCase( pGuild->m_strGuildName ) )
			{
				return pGuild;
			}

			j = 0;
		}
//		else j++;

//		if(j >= 100) break;	// 弥措 100俺 鳖瘤 歹 八祸秦焊绊 吝窜
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//	秦寸 辨靛牢郸胶肺 辨靛捞抚 馆券
//
BOOL USER::GetGuildName(int guildnum, TCHAR *guildname)
{
	if( guildnum < 0 || guildnum >= MAX_GUILD )
	{
		return FALSE;
	}

	EnterCriticalSection( &(m_pCom->m_critGuild) );

	CGuild* pGuild = NULL;

	if( !g_arGuildData[guildnum] )
	{
		LeaveCriticalSection( &(m_pCom->m_critGuild) );
		return FALSE;
	}

	pGuild = g_arGuildData[guildnum];

	if( pGuild->m_lSid != guildnum )
	{
		LeaveCriticalSection( &(m_pCom->m_critGuild) );
		return FALSE;
	}

	strcpy( guildname, pGuild->m_strGuildName );
	
	LeaveCriticalSection( &(m_pCom->m_critGuild) );

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	后 辨靛 牢郸胶甫 馆券
//
int USER::GetEmptyGuildNum()
{
/*	EnterCriticalSection( &(m_pCom->m_critGuild) );

	int nCount = 1;

	InterlockedIncrement(&g_CurrentGuildCount);

	for(int i = 0; i < g_arGuildData.GetSize(); i++)
	{
		if(!g_arGuildData[i]) continue;

		int temp = g_arGuildData[i]->m_lSid;
		if(g_arGuildData[i]->m_lSid >= nCount) nCount = g_arGuildData[i]->m_lSid + 1;
	}

	if(nCount >= MAX_GUILD) nCount = -1;			// 捞凡订 DB沥府 诀单捞飘秦具茄促...

	return nCount;
*/
	EnterCriticalSection( &(m_pCom->m_critGuild) );

	for( int i = 1; i < g_arGuildData.GetSize(); i++)
	{
		if( g_arGuildData[i]->m_lSid <= 0 )
		{
			return i;
		}
	}

	return -1;

}

////////////////////////////////////////////////////////////////////////////////
//	
//
void USER::ReleaseGuild()
{
	LeaveCriticalSection( &(m_pCom->m_critGuild) );
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 汲赋 炼扒捞 登绰瘤 魄窜
//
void USER::GuildOpen()
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	CBufferEx TempBuf;
   
	CString strTemp = _T("");
	CString strMsg = _T("");

	strTemp.Format("%s", m_strGuildName);

	if(!strTemp.IsEmpty()) 
	{
		strMsg.Format( IDS_USER_ALREADY_GUILD_IN, m_strGuildName);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);	
		return;
	}

	if(m_sGuildVersion >= 0)
	{
		strMsg.Format( IDS_USER_ALREADY_OTHER_GUILD_IN );
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);	
		return;
	}

	if(m_sLevel < GUILD_MAKE_LEVEL)  // 饭骇 力茄俊..
	{
		strMsg.Format( IDS_USER_GUILD_MAKE_LEVEL, GUILD_MAKE_LEVEL);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);	
		return;
	}

	if(m_dwDN < GUILD_MAKE_DN)  // 芒赋 磊陛俊...
	{
		strMsg.Format( IDS_USER_GUILD_MAKE_DINA, GUILD_MAKE_DN);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}

	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank < HERO_RANK)
	{
		strMsg.Format( IDS_USER_GUILD_MAKE_CITY_RANK, "Hero");
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return;
	}

	TempBuf.Add(GUILD_DIALOG);
	Send(TempBuf, TempBuf.GetLength());
	

}

////////////////////////////////////////////////////////////////////////////////
//	货肺款 辨靛甫 父电促.	
//
void USER::NewGuild(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	BYTE error_code = 0;
	BOOL bRes = TRUE;
	
	int i;
	int index = 0;
	int nLength = 0;
	int iGuildId = 0;
	TCHAR szGuildName[255];

	CString szTemp;
	CBufferEx	TempBuf;
    /* CString strMsg;*/
	WORD *pwMark = NULL;
	CGuild *pGuild = NULL;
//	CGuildUser *pGuildUser = NULL;

	if(m_dwGuild > 0) { error_code = ERR_5; goto go_result; }  // 促弗 辨靛盔牢 惑怕

	nLength = GetVarString(sizeof(szGuildName), szGuildName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) { error_code = ERR_1; goto go_result; } // 辨靛 疙莫 力茄俊 吧覆

	szTemp = szGuildName;

	if(!UNI_CHAR::CheckString(szTemp))
	{
		error_code = 255; 
		goto go_result;
	}

	if(szTemp.Find(' ') != -1 || szTemp.Find('\'') != -1 || szTemp.Find('"') != -1 || szTemp.Find('/') != -1)
	{
		error_code = 255; 
		goto go_result;
	}

	if(IsReservedID(szGuildName))
	{
		error_code = 04; 
		goto go_result;
	}

	if(m_sLevel < GUILD_MAKE_LEVEL) { error_code = ERR_1; goto go_result; } // 饭骇 力茄俊..

	if(m_dwDN < GUILD_MAKE_DN) { error_code = ERR_1; goto go_result; } // 芒赋 磊陛俊...

	pGuild = GetGuildByName(szGuildName);

	ReleaseGuild();

	if(pGuild) { error_code = ERR_4; goto go_result; }  // 捞固 殿废茄 辨靛 捞抚老锭...

	iGuildId = GetEmptyGuildNum();

	if(iGuildId <= 0)
	{
		ReleaseGuild();
		//InterlockedDecrement(&g_CurrentGuildCount);
		return;
	}

//	pGuild = new CGuild;
	pGuild = g_arGuildData[iGuildId];
/*
	if(strlen(pGuild->m_strGuildName) > 0)
	{
		ReleaseGuild();
		return;
	}
*/
	if(pGuild->m_lSid > 0) 
	{
		ReleaseGuild();
		return;
	}
/*
	pGuildUser = new CGuildUser;		// 皋葛府且寸俊 角菩窍搁...
	if(!pGuildUser)
	{
		ReleaseGuild();
		return;
	}
*/

	// alisia - 宏复瘤 辑滚肺 辨靛 货肺 父电促绰 脚龋甫 焊郴绊 府畔茄促. 饶俊 搬苞甫 罐酒辑 辨靛甫 肯己茄促.
	g_pMainDlg->BridgeServerGuildNewReq( m_uid, m_strUserID, iGuildId, szGuildName );
	ReleaseGuild();
	return;





	pGuild->m_lSid = iGuildId;

	strcpy(pGuild->m_strGuildName, szGuildName);
	strcpy(pGuild->m_strMasterName, m_strUserID);

	pGuild->m_dwGuildDN = 0;
	pGuild->m_sVersion = -1;

	
	pwMark = (WORD*)(&pGuild->m_strMark[0]);
	for (i=0; i<GUILD_MARK_SIZE/sizeof(WORD); i++) {
		pwMark[i] = (0x1f<<11 | 0x1f);	// Client俊辑 荤侩窍绰 捧疙祸栏肺 檬扁拳茄促.
	}

	if(!InsertGuild(pGuild))
	{
//		delete pGuild;
//		if(pGuildUser) delete pGuildUser;

//		InterlockedDecrement(&g_CurrentGuildCount);
		ReleaseGuild();
		return;
	}

//	::ZeroMemory(pGuildUser->m_strUserId, sizeof(pGuildUser->m_strUserId));

//	nLength = 0;
//	nLength = strlen(m_strUserID);

//	pGuildUser->m_lSid = iGuildId;
//	strncpy(pGuildUser->m_strUserId, m_strUserID, nLength);

//	pGuild->m_arUser.Add(pGuildUser);
	pGuild->AddUser(m_strUserID, iGuildId);

	g_arGuildData[iGuildId] = pGuild;

	ReleaseGuild();

	if( m_dwDN <= GUILD_MAKE_DN ) m_dwDN = 0;
	else m_dwDN -= GUILD_MAKE_DN;		// 辨靛 汲赋 磊陛甫 猾促.

	m_bGuildMaster = TRUE;
	m_dwGuild = iGuildId;
   /* strMsg.Format( "恭喜『%s』建立新军团『%s』！" , pGuild->m_strMasterName , pGuild->m_strGuildName);
	SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);*/

	nLength = 0;
	nLength = strlen(szGuildName);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return; 
	strncpy(m_strGuildName, szGuildName, nLength);
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

void USER::NewGuildWithThread(TCHAR *pBuf)
{
	int index = 0;
	int nLength = 0;
	TCHAR szGuildName[255];

	nLength = GetVarString(sizeof(szGuildName), szGuildName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return; 

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = GUILD_OPEN_REQ;
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
//	货肺款 辨靛甫 父电促.	
//
BOOL USER::InsertGuild(CGuild *pGuild)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	::ZeroMemory(szSQL, sizeof(szSQL));

	int index = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call INSERT_GUILD (%d, \'%s\', \'%s\')}"), pGuild->m_lSid, pGuild->m_strGuildName, m_strUserID); 

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return FALSE;

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
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	抛捞喉俊 辨靛 啊涝阑 悸泼茄促.	
//
BOOL USER::InsertGuildUser(int guildID, TCHAR *strUserID)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[2000];		

	::ZeroMemory(szSQL, sizeof(szSQL));

	int index = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call INSERT_GUILD_USER (%d, \'%s\')}"), guildID, strUserID); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
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
	else
	{
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛俊 殿废等 巩剧阑 诀单捞飘茄促.
//
BOOL USER::UpdateGuildMark(TCHAR *pMark, CGuild *pGuild)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strMark[450];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strMark, sizeof(strMark));

	int index = 0;

	SDWORD sMarkLen		= sizeof(strMark);

	::CopyMemory(strMark, pMark, GUILD_MARK_SIZE);

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_MARK (\'%s\',%d, ?)}"), pGuild->m_strGuildName, pGuild->m_sVersion ); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strMark), 0, (TCHAR*)strMark, 0, &sMarkLen );

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
	else
	{
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛蜡历抛捞喉俊辑 秦寸 蜡历 单捞磐甫 昏力茄促.
//
BOOL USER::DeleteGuildUser(TCHAR *strUserID)
{
	if ( strUserID == NULL ) return FALSE;
//解决t_strUserID 错误
	if(strlen(strUserID)<=0)
		return FALSE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
//	char			t_strUserID[20];//老代码设置
	char			t_strUserID[CHAR_NAME_LENGTH+1];

	::ZeroMemory(szSQL, sizeof(szSQL));

	strcpy( t_strUserID, strUserID );

	int index = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call DELETE_GUILD_USER (\'%s\')}"), t_strUserID ); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
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
	else
	{
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();
//		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

BOOL USER::DeleteGuildUserWithThread(TCHAR *strUserID)
{
	if ( strUserID == NULL ) return FALSE;

	int nLen = strlen(strUserID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return FALSE;

	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = DEL_GUILD_CHAR_REQ;
	Datalength = strlen(strUserID);
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, 0, Datalength+1);
	memcpy(pData, strUserID, Datalength);
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 付农甫 盎脚茄促.
//
void USER::GuildMarkAddReq(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	CBufferEx TempBuf;
	TCHAR GuildMark[GUILD_MARK_SIZE+1];

	BOOL bRes = TRUE;
	BYTE error_code = 0;

	CGuild *pGuild = NULL;

	if(!m_bGuildMaster) return; // { error_code = ERR_1; goto go_result; }// 辨靛炉捞 酒聪搁 唱皑

	::ZeroMemory(GuildMark, sizeof(GuildMark));

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) { error_code = ERR_2; goto go_result; }		// 秦寸 辨靛啊 绝栏搁 唱皑..

	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0) { error_code = ERR_1; goto go_result; }// 辨靛炉捞 撇府搁 唱皑...

	::CopyMemory(GuildMark, pBuf, GUILD_MARK_SIZE);
	
	// alisia
	g_pMainDlg->BridgeServerGuildMarkReq( m_uid, m_strUserID, (int)m_dwGuild, GuildMark );
	ReleaseGuild();
	return;




	if(pGuild->m_sVersion >= 0) CheckMaxValue((short &)pGuild->m_sVersion, (short)1);
	else pGuild->m_sVersion = 0;

	if(!UpdateGuildMark(GuildMark, pGuild))	// DB俊 静扁 角菩搁 唱皑..
	{
		pGuild->m_sVersion = -1;
		error_code = 255;
		goto go_result; 
	}
	
//	pGuild->m_sVersion += 1;
	m_sGuildVersion = pGuild->m_sVersion;	// 辨靛 巩剧 滚怜

	::CopyMemory(pGuild->m_strMark, GuildMark, GUILD_MARK_SIZE);

	bRes = FALSE;

go_result:
	ReleaseGuild();				// 秦力...

	TempBuf.Add(GUILD_MARK_ADD_RESULT);

	if(bRes)
	{
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	
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

//	SendMyInfo(TO_INSIGHT, INFO_MODIFY);
}

void USER::GuildMarkAddReqWithThread(TCHAR *pBuf)
{
	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = GUILD_MARK_ADD_REQ;
	Datalength = GUILD_MARK_SIZE;
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
//	努扼捞攫飘俊 货肺款 辨靛啊 眠啊登菌芭唱 盎脚荤亲捞 乐绰瘤 舅妨霖促. 
//
void USER::GuildAddReq(TCHAR *pBuf)
{	
	CBufferEx TempBuf; 

	int index = 0;
	BOOL bRes = TRUE;
	BYTE error_code = 0;

	int iGuildID = GetInt(pBuf, index);

	CGuild *pGuild = NULL;

	if(iGuildID < 0) return;

	TempBuf.Add(GUILD_ADD_RESULT);

	pGuild = GetGuild(iGuildID);

	if(!pGuild) { error_code = ERR_1; goto go_result; }		// 秦寸 辨靛啊 绝栏搁 唱皑..

	if(!pGuild->m_strMark) { error_code = 255; goto go_result; }

	TempBuf.Add((BYTE)0x01);
	TempBuf.Add((int)pGuild->m_lSid);
	TempBuf.AddString(pGuild->m_strGuildName);
	TempBuf.Add(pGuild->m_sVersion);
	TempBuf.AddData((TCHAR*)pGuild->m_strMark, GUILD_MARK_SIZE);
	bRes = FALSE;

go_result:
	ReleaseGuild();				// 秦力...

	if(bRes)
	{
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	Memory DB 甫 檬扁拳 茄促.
//
BOOL USER::InitMemoryDB(int uid)
{

	m_pSharedMemory = g_arSharedMemory[uid];
	m_pMD = (CMemUser*) m_pSharedMemory->m_lpData;

	m_pMD->m_uid = uid;
	m_pMD->m_UB.m_uid = uid;
	m_pMD->m_AB.m_uid = uid;
	m_pMD->m_iMyServer = m_iMyServer;

	::ZeroMemory(m_pMD->m_strAccount, sizeof(m_pMD->m_strAccount));
	strncpy(m_pMD->m_strAccount, m_strAccount, strlen(m_strAccount));

	::ZeroMemory(m_pMD->m_strUserID, sizeof(m_pMD->m_strUserID));
	strncpy(m_pMD->m_strUserID, m_strUserID, strlen(m_strUserID));

	::ZeroMemory(m_pMD->m_UB.m_strUserID, sizeof(m_pMD->m_UB.m_strUserID));
	strncpy(m_pMD->m_UB.m_strUserID, m_strUserID, strlen(m_strUserID));

	::ZeroMemory(m_pMD->m_AB.m_strAccount, sizeof(m_pMD->m_AB.m_strAccount));
	strncpy(m_pMD->m_AB.m_strAccount, m_strAccount, strlen(m_strAccount));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	皋葛府 DB狼 郴侩阑 DB肺 颗变促.
//
void USER::UpdateMem2DB(CMemUser *pMD)
{
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

	::CopyMemory(strFace, pMD->m_strFace, sizeof(strFace));
	::CopyMemory(strSkill, pMD->m_strSkill, sizeof(strSkill));
	::CopyMemory(strItem, pMD->m_strItem, sizeof(strItem));
	::CopyMemory(strPsi, pMD->m_strPsi, sizeof(strPsi));
	::CopyMemory(strTel, pMD->m_strTel, sizeof(strTel));
	::CopyMemory(strHaveEvent, pMD->m_strHaveEvent, sizeof(strHaveEvent));

	::CopyMemory(strQuickItem, pMD->m_strQuickItem, sizeof(strQuickItem));
	

	SDWORD sFaceLen		= sizeof(strFace);
	SDWORD sSkillLen	= sizeof(strSkill);
	SDWORD sItemLen		= sizeof(strItem);
	SDWORD sPsiLen		= sizeof(strPsi);
	SDWORD sQuickLen	= sizeof(strQuickItem);
	SDWORD sEventLen	= sizeof(strHaveEvent);
	SDWORD sTelLen		= sizeof(strTel);
	
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
		pMD->m_strUserID, pMD->m_sSTR, pMD->m_sCON,	pMD->m_sDEX, pMD->m_sVOL, pMD->m_sWIS, pMD->m_iSkin, pMD->m_iHair, pMD->m_sGender,	
		pMD->m_curz, pMD->m_curx, pMD->m_cury, pMD->m_dwBuddy, pMD->m_dwGuild, pMD->m_dwExp, pMD->m_sPA, pMD->m_sSkillPoint, pMD->m_dwXP,
		pMD->m_sMaxHP, pMD->m_sHP, pMD->m_sMaxPP, pMD->m_sPP, pMD->m_sMaxSP, pMD->m_sSP, pMD->m_dwDN, pMD->m_sCityRank, pMD->m_sLevel, pMD->m_byClass,

		pMD->m_tAbnormalKind, pMD->m_dwAbnormalTime, 

		pMD->m_bLive, pMD->m_iCityValue, pMD->m_sKillCount, pMD->m_dwSaveTime,
		pMD->m_dwSaintTime, 
		pMD->m_dwHiExpTime, pMD->m_dwHtExpTime, pMD->m_dwMagicFindTime, pMD->m_dwMagicFtTime, pMD->m_dwNoChatTime, pMD->m_dwCloseTime, pMD->m_dwAutoMoney,pMD->m_dwLingQu, pMD->m_dwGuarDianTianShi, pMD->m_dwShopPingDN,pMD->m_dwVIPTime,pMD->m_dwZaiXianTime,pMD->m_dwBFindTime,pMD->m_dwBHTime,pMD->m_dwSJTime,
		pMD->m_strLoveName, //--yskang 0.1 
		pMD->m_tPsiOneKind, pMD->m_dwPsiOneTime,		pMD->m_tPsiTwoKind, pMD->m_dwPsiTwoTime,		pMD->m_tPsiThreeKind, pMD->m_dwPsiThreeTime); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strFace),	0, (TCHAR*)strFace,		0, &sFaceLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strSkill),	0, (TCHAR*)strSkill,	0, &sSkillLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strPsi),		0, (TCHAR*)strPsi,		0, &sPsiLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strQuickItem),	0, (TCHAR*)strQuickItem,	0, &sQuickLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strHaveEvent),	0, (TCHAR*)strHaveEvent,	0, &sEventLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strTel),	0, (TCHAR*)strTel,	0, &sTelLen );
		
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
	else
	{
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

//		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
}

void USER::UpdateBankMem2DB(CMemUser *pMD)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strBankItem[_BANK_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	
	::CopyMemory(strBankItem, pMD->m_UB.m_UserBankItem, sizeof(strBankItem));
	
	SDWORD sBankItemLen	= sizeof(strBankItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_BANK_DATA_ONLY (\'%s\',%d, ?)}"), pMD->m_UB.m_strUserID, pMD->m_UB.m_dwBankDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Update User Bank Data Only!!\n");

//		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strBankItem), 0, (TCHAR*)strBankItem, 0, &sBankItemLen);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return;
		}
	}	

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return;

	return;
}

void USER::UpdateAccountBankMem2DB(CMemUser *pMD)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];
	TCHAR			strBankItem[_ACCOUNT_BANK_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	
	::CopyMemory(strBankItem, pMD->m_AB.m_AccountBankItem, sizeof(strBankItem));
	
	SDWORD sBankItemLen	= sizeof(strBankItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_ACCOUNT_BANK_DATA_ONLY (\'%s\',%d, ?)}")/*, pMD->m_iMyServer*/, pMD->m_AB.m_strAccount, pMD->m_AB.m_dwBankDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		return;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strBankItem), 0, (TCHAR*)strBankItem, 0, &sBankItemLen);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return;
		}
	}	

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//	Memory DB俊 泅犁 葛电 蜡历 Data 甫 敬促.
//
BOOL USER::UpdateMemUserAll(BOOL bBank)
{
	TCHAR strBankItem[_BANK_DB];
	TCHAR strAccountBankItem[_ACCOUNT_BANK_DB];

	if(m_pSharedMemory == NULL) return FALSE;
	if(m_pSharedMemory->m_hMapping == NULL) return FALSE;
	if(m_pMD == NULL) return FALSE;
	if(m_pMD->m_uid == -1) return FALSE;

	if(m_pMD->m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_strUserID) != 0) return FALSE;

	DWORD curTime = ConvertCurTimeToSaveTime();
	if(m_pMD->m_dwSaveTime > curTime) return FALSE;

	if(bBank)
	{
		if(m_pMD->m_uid != m_pMD->m_UB.m_uid) return FALSE;
		if(_stricmp(m_strUserID, m_pMD->m_UB.m_strUserID) != 0) return FALSE;
		if(_stricmp(m_strAccount, m_pMD->m_AB.m_strAccount) != 0) return FALSE;

		m_pMD->m_UB.m_dwBankDN = 0;
		::ZeroMemory(strBankItem, sizeof(strBankItem));
		UserBankItemToStr(strBankItem);
		memcpy(m_pMD->m_UB.m_UserBankItem, strBankItem, sizeof(strBankItem));
		m_pMD->m_UB.m_dwBankDN = m_dwBankDN;

		m_pMD->m_AB.m_dwBankDN = 0;
		::ZeroMemory(strAccountBankItem, sizeof(strAccountBankItem));
		UserAccountBankItemToStr( strAccountBankItem );
		memcpy(m_pMD->m_AB.m_AccountBankItem, strAccountBankItem, sizeof(strAccountBankItem));
		m_pMD->m_AB.m_dwBankDN = m_dwAccountBankDN;
	}

	m_pMD->m_dwSaveTime = curTime;			// 泅犁 滚怜

	m_pMD->m_sSTR = m_sSTR;					// 塞
	m_pMD->m_sCON = m_sCON;					// 扒碍
	m_pMD->m_sDEX = m_sDEX;					// 刮酶己
	m_pMD->m_sVOL = m_sVOL;					// 狼瘤
	m_pMD->m_sWIS = m_sWIS;					// 瘤驱
	
	m_pMD->m_iSkin = m_iSkin;				// 乔何祸
	m_pMD->m_iHair = m_iHair;				// 赣府祸
	m_pMD->m_sGender = m_sGender;			// 己喊
//	_tcscpy(m_pMD->m_strFace, m_strFace);	// 倔奔葛剧
	memcpy(m_pMD->m_strFace, m_strFace, sizeof(m_strFace));	// 倔奔葛剧

	m_pMD->m_curx = m_curx;					// 泅犁 X 谅钎
	m_pMD->m_cury = m_cury;					// 泅犁 Y 谅钎
	m_pMD->m_curz = m_curz;					// 泅犁 粮

	m_pMD->m_dwBuddy = m_dwBuddy;			// 滚叼锅龋
	m_pMD->m_dwGuild = m_dwGuild;			// 辨靛锅龋

	m_pMD->m_dwExp = m_dwExp;				// 饭骇版氰摹

	m_pMD->m_sPA = m_sPA;					// PA Point
	m_pMD->m_sSkillPoint = m_sSkillPoint;	// Skill Point
	
	m_pMD->m_dwXP = m_dwXP;					// X Point

	m_pMD->m_sMaxHP	= m_sMaxHP;				// 弥措 HP
	m_pMD->m_sHP	= m_sHP;				// 泅犁 HP
	m_pMD->m_sMaxPP = m_sMaxPP;				// 弥措 PP
	m_pMD->m_sPP	= m_sPP;				// 泅犁 PP
	m_pMD->m_sMaxSP = m_sMaxSP;				// 弥措 SP
	m_pMD->m_sSP	= m_sSP;				// 泅犁 SP

	m_pMD->m_dwDN = m_dwDN;					// 家瘤陛

	m_pMD->m_sCityRank = m_sCityRank;		// 矫刮 殿鞭
	m_pMD->m_sKillCount = m_sKillCount;		// 泅犁 PK茄 冉荐

	m_pMD->m_sLevel = m_sLevel;				// 拌凯饭骇
	m_pMD->m_byClass = m_byClass;			// 努贰胶
	m_pMD->m_dwZaiXianTime = m_dwZaiXianTime;	


	TCHAR strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB], strHaveEvent[_EVENT_DB], strTel[_TEL_DB];

	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));
	::ZeroMemory(strTel, sizeof(strTel));
	::ZeroMemory(strHaveEvent, sizeof(strHaveEvent));

	UserSkillToDBStr(strSkill);
	UserItemToStr(strItem);
	UserPsiToStr(strPsi);
	UserTelToStr(strTel);
	UserHaveEventDataToStr(strHaveEvent);
/*
	strcpy(m_pMD->m_strSkill, strSkill);	// User Skill DB
	strcpy(m_pMD->m_strItem, strItem);		// User Item DB
	strcpy(m_pMD->m_strPsi, strPsi);		// User Psionic DB
	strcpy(m_pMD->m_strHaveEvent, strHaveEvent);// User Event DB
*/
	memcpy(m_pMD->m_strSkill, strSkill, sizeof(strSkill));
	memcpy(m_pMD->m_strItem, strItem, sizeof(strItem));
	memcpy(m_pMD->m_strPsi, strPsi, sizeof(strPsi));
	memcpy(m_pMD->m_strTel, strTel, sizeof(strTel));
	memcpy(m_pMD->m_strHaveEvent, strHaveEvent, sizeof(strHaveEvent));

	::CopyMemory(m_pMD->m_sChangeClass, m_sChangeClass, sizeof(m_sChangeClass));	// 傈流矫 努贰胶饭骇 历厘
	
	// 惑怕捞惑 沥焊 历厘
	m_pMD->m_tAbnormalKind = m_tAbnormalKind;
	m_pMD->m_dwAbnormalTime = m_dwAbnormalTime;

	m_pMD->m_tIsOP = m_tIsOP;				// 款康磊牢瘤 魄窜
											// 0: Normal User
											// 1: Game Operator
											// 2: Server Operator

	m_pMD->m_bLive = m_bLive;				// 磷菌聪? 混疽聪?

	m_pMD->m_sTempSTR = m_sTempSTR;			// 塞
	m_pMD->m_sTempCON = m_sTempCON;			// 扒碍
	m_pMD->m_sTempDEX = m_sTempDEX;			// 刮酶己
	m_pMD->m_sTempVOL = m_sTempVOL;			// 狼瘤
	m_pMD->m_sTempWIS = m_sTempWIS;			// 瘤驱

	m_pMD->m_iCityValue = m_iCityValue;		// 泅犁 穿利 己氢蔼

	m_pMD->m_dwSaintTime		= m_dwSaintTime;
	
	m_pMD->m_dwHiExpTime		= m_dwHiExpTime;
	m_pMD->m_dwHtExpTime		= m_dwHtExpTime;
	m_pMD->m_dwMagicFindTime	= m_dwMagicFindTime;
	m_pMD->m_dwMagicFtTime		= m_dwMagicFtTime;
	m_pMD->m_dwNoChatTime		= m_dwNoChatTime;
    m_pMD->m_dwCloseTime		= m_dwCloseTime;
	m_pMD->m_dwAutoMoney		= m_dwAutoMoney;
	m_pMD->m_dwLingQu   		= m_dwLingQu;
	m_pMD->m_dwGuarDianTianShi	= m_dwGuarDianTianShi;
	m_pMD->m_dwShopPingDN		= m_dwShopPingDN;
	m_pMD->m_dwVIPTime			= m_dwVIPTime;//vip用户
	m_pMD->m_dwZaiXianTime		= m_dwZaiXianTime;
	m_pMD->m_dwBFindTime		= m_dwBFindTime;
	m_pMD->m_dwBHTime		= m_dwBHTime;
	m_pMD->m_dwSJTime		= m_dwSJTime;
	
	m_pMD->m_tPsiOneKind = m_pMD->m_tPsiTwoKind = m_pMD->m_tPsiThreeKind = 0;
	m_pMD->m_dwPsiOneTime = m_pMD->m_dwPsiTwoTime = m_pMD->m_dwPsiThreeTime = 0;

	// Psionic One
	if(m_dwHasteTime != 0) 
	{
		m_pMD->m_tPsiOneKind = PSIONIC_HASTE;
		m_pMD->m_dwPsiOneTime = m_dwHasteTime;
	}
	if(m_dwShieldTime != 0) 
	{
		m_pMD->m_tPsiOneKind = PSIONIC_SHIELD;
		m_pMD->m_dwPsiOneTime = m_dwShieldTime;
	}
	if(m_dwDexUpTime != 0) 
	{
		m_pMD->m_tPsiOneKind = PSIONIC_DEXUP;
		m_pMD->m_dwPsiOneTime = m_dwDexUpTime;
	}
	if(m_dwMaxHPUpTime != 0) 
	{
		m_pMD->m_tPsiOneKind = PSIONIC_HPUP;
		m_pMD->m_dwPsiOneTime = m_dwMaxHPUpTime;
	}
	if(m_dwFastRunTime != 0) 
	{
		m_pMD->m_tPsiOneKind = PSIONIC_FAST_RUN;
		m_pMD->m_dwPsiOneTime = m_dwFastRunTime;
	}
	if(m_dwMindShockTime != 0) 
	{
		m_pMD->m_tPsiOneKind = PSIONIC_MIND_SHOCK;
		m_pMD->m_dwPsiOneTime = m_dwMindShockTime;
	}
	if(m_dwPsiShieldTime != 0) 
	{
		m_pMD->m_tPsiOneKind = PSIONIC_PSI_SHIELD;
		m_pMD->m_dwPsiOneTime = m_dwPsiShieldTime;
	}
	if(m_dwBigShieldTime != 0) 
	{
		m_pMD->m_tPsiOneKind = 30;
		m_pMD->m_dwPsiOneTime = m_dwBigShieldTime;
	}
	if(m_dwPiercingShieldTime != 0) 
	{
		m_pMD->m_tPsiOneKind = PSIONIC_PIERCING_SHIELD;
		m_pMD->m_dwPsiOneTime = m_dwPiercingShieldTime;
	}

	// Psionic Two
	if(m_dwAdamantineTime != 0) 
	{
		m_pMD->m_tPsiTwoKind = PSIONIC_ADAMANTINE;
		m_pMD->m_dwPsiTwoTime = m_dwAdamantineTime;
	}
	if(m_dwMightyWeaponTime != 0) 
	{
		m_pMD->m_tPsiTwoKind = PSIONIC_MIGHTYWEAPON;
		m_pMD->m_dwPsiTwoTime = m_dwMightyWeaponTime;
	}
	if(m_dwBerserkerTime != 0) 
	{
		m_pMD->m_tPsiTwoKind = PSIONIC_BERSERKER;
		m_pMD->m_dwPsiTwoTime = m_dwBerserkerTime;
	}

	// Psionic Three
	if(m_dwMindGuardTime != 0) 
	{
		m_pMD->m_tPsiThreeKind = PSIONIC_MIND_GUARD;
		m_pMD->m_dwPsiThreeTime = m_dwMindGuardTime;
	}
	//m_pMD->m_dwBSTime=m_dwBSTime;
	m_pMD->m_dwDNMoney = m_dwDNMoney;
	m_pMD->m_dwShTsTime = m_dwShTsTime;
	m_pMD->m_isDoubleExp = m_isDoubleExp;
	m_pMD->m_isDoubleBAOLV = m_isDoubleBAOLV;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// DB俊 篮青单捞磐, Memory DB俊 蜡历 单捞鸥甫 历厘茄促
//
BOOL USER::UpdateMemUserBank()
{
//	if(!UpdateBankDataOnly()) return FALSE;
	return UpdateMemUserAll(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	DB俊 篮青俊 包访等 单捞磐甫 历厘茄促.
//
BOOL USER::UpdateBankDataOnly()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strBankItem[_BANK_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	
	UserBankItemToStr(strBankItem);
	
	SDWORD sBankItemLen	= sizeof(strBankItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_BANK_DATA_ONLY (\'%s\',%d, ?)}"), m_strUserID, m_dwBankDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Update User Bank Data Only!!\n");

//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strBankItem), 0, (TCHAR*)strBankItem, 0, &sBankItemLen);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//	Restart 殿阑 且 锭 促矫 皋葛府 叼厚甫 檬扁拳 茄促.
//
void USER::ReInitMemoryDB()
{
	if(m_pMD != NULL) 
	{
		m_pMD->m_uid = -1;
		m_pMD->m_sSTR = 0;
		m_pMD->m_UB.m_uid = -1;
		m_pMD->m_AB.m_uid = -1;
		m_pMD->m_dwSaveTime = 0;

		::ZeroMemory(m_pMD->m_strUserID, sizeof(m_pMD->m_strUserID));
		::ZeroMemory(m_pMD->m_strAccount, sizeof(m_pMD->m_strUserID));
	}
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 炉苞 辑肺 付林 夯惑怕俊辑 啊涝茄促. 
//
void USER::GuildInvite(int uid)
{	
//	if ( pBuf == NULL ) return;

	int index = 0;
	BYTE error_code = 0;

	int nLen = 0;

//	int uid = 0;
	int iCount = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	BOOL bRes = TRUE;

	if(!m_bGuildMaster && !m_bRepresentationGuild) { error_code = ERR_10; goto go_result; }	// 辨靛 鼻茄捞 绝促.

//	uid = GetInt(pBuf, index);

	pUser = GetUser(uid - USER_BAND);

	if(!pUser) { error_code = ERR_3; goto go_result; }				// 蜡历啊 绝促.

	if(IsThereUser(pUser) == FALSE || strcmp(m_strUserID, pUser->m_strUserID) == 0)
	{
		SendSystemMsg( IDS_USER_SEE_EACH_OTHER, SYSTEM_NORMAL, TO_ME);
		return;
	}

	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0) return;

	if(pUser->m_dwGuild > 0)  { error_code = ERR_9; goto go_result; }// 捞固 促弗 辨靛俊 啊涝茄 蜡历 
	
	if(CheckInGuildWarring()) return;								// 辨傈吝俊绰 倾遏罙荐绝促.

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

		m_bGuildMaster = FALSE;
		m_bRepresentationGuild = FALSE;
		error_code = ERR_10;		// 
		goto go_result;
	}

	index = -1;
	index = pGuild->GetUser(pUser->m_strUserID);
	if(index >= 0) 
	{ 
		ReleaseGuild();				// 秦力...

		pUser->m_dwGuild = m_dwGuild;			// 辨靛 锅龋甫 促矫 悸泼茄促.
		SendGuildInfo(pUser);

		error_code = ERR_9; 
		goto go_result; 
	}// 捞固 促弗 辨靛俊 啊涝茄 蜡历	
/*
	iCount = pGuild->m_arUser.GetSize();
	if(iCount <= 0 || iCount >= MAX_GUILD_USER) 
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_11;			// 
		goto go_result;
	}

	pGuildUser = new CGuildUser;	// 皋葛府 且寸 角菩搁..
	if(!pGuildUser)
	{
		ReleaseGuild();				// 秦力...
		error_code = 255;			// 
		goto go_result;
	}
*/

	// alisia
	g_pMainDlg->BridgeServerGuildInviteReq( m_uid, pUser->m_uid, m_strUserID, pUser->m_strUserID, (int)m_dwGuild );
	ReleaseGuild();
	return;
	//




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

//	pGuildUser = new CGuildUser;	
//	pGuildUser->m_lSid = 0;
//	::ZeroMemory(pGuildUser->m_strUserId, sizeof(pGuildUser->m_strUserId));

//	pGuildUser->m_lSid = m_dwGuild;	
//	strncpy(pGuildUser->m_strUserId, pUser->m_strUserID, nLen);

//	pGuild->m_arUser.Add(pGuildUser);

	ReleaseGuild();

	UpdateUserData();

	AddGuildUserInFortress(pUser);

	SendGuildInviteUser(pUser);			// 辨靛 府胶飘甫 林绊 罐扁
	SendGuildInfo(pUser);				// 矫具 裹困俊 辨靛俊 啊涝茄 荤恩捞 乐栏聪瘪 巩剧阑 焊捞扼绊 茄促...

//	SendSystemMsg( IDS_USER_ENTER_SUCCESS, SYSTEM_NORMAL, TO_ME);
//	pUser->SendSystemMsg( IDS_USER_ENTER_SUCCESS, SYSTEM_NORMAL, TO_ME);
}

void USER::GuildInviteWithThread(TCHAR *pBuf)
{	
//	uid = GetInt(pBuf, index);
	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = GUILD_INVITE_USER_REQ;			// 货肺父电促.
	Datalength = sizeof(int);
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
//	秦寸 辨靛盔甸俊霸 货肺捞 辨靛盔捞等 蜡历甫 舅赴促.
//
void USER::SendGuildInviteUser(USER *pUser)
{
	int i;
	int nLen = 0;	
	int nCount = -1;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(m_dwGuild <= 0) return;

	USER *pGUser = NULL;

	CBufferEx TempBuf;

	CString strMsg = _T("");
	strMsg.Format( IDS_USER_NEW_MEMBER, pUser->m_strUserID);

	for(i = 0; i< MAX_USER; i++)
	{
		pGUser = m_pCom->GetUserUid(i);

		if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;		

		if(m_dwGuild == pGUser->m_dwGuild)
		{
			pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
/*
//	SetGuildUserIndex(pUser);										// 辨付啊 刚历 利侩...

	for(i = 0; i < MAX_GUILD_USER; i++)								// 捞犯霸 蝶肺 府胶飘甫 包府窍绰扒...
	{																	
		pUser->m_MyGuildUser[i].uid = -1;
		pUser->m_MyGuildUser[i].lUsed = 0;							// 秦寸 蜡历 府胶飘甫 檬扁拳茄促.
		::ZeroMemory(pUser->m_MyGuildUser[i].GuildUser, sizeof(pUser->m_MyGuildUser[i].GuildUser));

		nLen = strlen(m_MyGuildUser[i].GuildUser);
																	// 啊涝茄 蜡历俊霸 辨靛 府胶飘甫 copy秦 霖促	
		if(nLen > 0 && nLen <= CHAR_NAME_LENGTH) strncpy(pUser->m_MyGuildUser[i].GuildUser, m_MyGuildUser[i].GuildUser, nLen);
		else continue;

		pGUser = GetUser(m_MyGuildUser[i].uid);

		if(!pGUser)													// 捞傈 uid肺 茫阑荐 绝促搁 促矫 酒捞叼肺 八祸	
		{
			pGUser = GetUser(m_MyGuildUser[i].GuildUser);
			if(!pGUser) 
			{ 
				pUser->m_MyGuildUser[i].uid = -1;
				m_MyGuildUser[i].uid = -1; 
				continue; 
			}
		}
		else if(strcmp(m_MyGuildUser[i].GuildUser, pGUser->m_strUserID) != 0)
		{															// uid啊 辑肺 促福搁 促矫 八祸秦辑 uid甫 悸泼	
			pGUser = GetUser(m_MyGuildUser[i].GuildUser);			// 捞犯霸 茄捞蜡绰 strUserID肺 茫绰巴阑 乔窍扁困秦
			if(!pGUser) 
			{
				pUser->m_MyGuildUser[i].uid = -1;
				m_MyGuildUser[i].uid = -1; 
				continue; 
			}
		}

		nCount = pGUser->SetGuildUserIndex(pUser);					// 阿 辨靛盔俊霸 货肺 啊涝茄 蜡历沥焊甫 焊晨

		pUser->m_MyGuildUser[i].uid = pGUser->m_uid;				// 瘤陛 白加俊 乐栏搁 府胶飘惑俊辑 uid甫 悸泼

		pGUser->SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
	}	
*/
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛甫 呕硼茄促.
//
void USER::GuildOff(TCHAR *pBuf)
{
	int index = 0;
	BYTE error_code = 0;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	BOOL bRes = TRUE;

	if(m_dwGuild <= 0) { error_code = ERR_8; goto go_result; }
	
	if(m_bGuildMaster) { error_code = ERR_8; goto go_result; }

	if(m_tGuildHouseWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING || m_tFortressWar == GUILD_WARRING) 
	{ 
		SendSystemMsg( IDS_USER_NO_OUT_IN_WAR, SYSTEM_ERROR, TO_ME);
		return; 
	}

	if(CheckInGuildWarring()) return;								// 辨傈吝俊绰 倾遏罙荐绝促.

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) 
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_7;
		goto go_result;
	}

	index = -1;
	index = pGuild->GetUser(m_strUserID);

	if(index < 0)					// 秦寸 辨靛俊辑 蜡历啊 绝栏搁...
	{
		m_dwGuild = -1;							// 檬扁拳茄促.
		m_sGuildVersion = -1;					// 辨靛 巩剧 滚怜
		::ZeroMemory(m_strGuildName, sizeof(m_strGuildName));	
		m_bGuildMaster = FALSE;					// 辨靛 付胶磐
		m_bRepresentationGuild = FALSE;			// 鼻茄 措青 咯何	 	

		ReleaseGuild();				
		error_code = ERR_8;
		goto go_result;
	}
												// 辨靛 付胶磐啊 呕硼狼荤甫 灌洒搁 俊矾...^^			
	if(strcmp(pGuild->m_strMasterName, m_strUserID) == 0)
	{
		ReleaseGuild();				
		error_code = ERR_8;
		goto go_result;
	}

	// alisia
	g_pMainDlg->BridgeServerGuildOffReq( m_uid, m_strUserID, m_strUserID, (int)m_dwGuild );
	ReleaseGuild();
	return;




	if(!pGuild->RemoveUser(m_strUserID))
	{
		ReleaseGuild();				
		error_code = ERR_8;
		goto go_result;
	}

	if(!DeleteGuildUser(m_strUserID))		// 抛捞喉俊辑 瘤款促.
	{
		pGuild->AddUser(m_strUserID, m_dwGuild);

		ReleaseGuild();				
		error_code = 255;
		goto go_result;
	}

	//if(!DeleteGuildUserWithThread(m_strUserID))		// 抛捞喉俊辑 瘤款促.
	//{
	//	ReleaseGuild();				
	//	error_code = 255;
	//	goto go_result;
	//}

//	pGuild->RemoveUser(m_strUserID);		// 皋葛府俊辑 瘤款促.
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

	pUser = GetUser(m_uid);

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	SendGuildOffUser(pUser);				// 促弗 辨靛盔俊霸 郴啊 呕硼茄 荤角阑 舅赴促.

	DelGuildUserInFortress(m_strUserID, m_dwGuild);

	m_dwGuild = -1;							// 檬扁拳茄促.
	m_sGuildVersion = -1;					// 辨靛 巩剧 滚怜
	::ZeroMemory(m_strGuildName, sizeof(m_strGuildName));	
	m_bGuildMaster = FALSE;					// 辨靛 付胶磐
	m_bRepresentationGuild = FALSE;			// 鼻茄 措青 咯何	 	

	SendMyGuildInfo();						// 辨靛 呕硼甫 舅赴促.

	UpdateUserData();
//	SendSystemMsg( IDS_USER_OUT_GUILD, SYSTEM_NORMAL, TO_ME);
}

void USER::GuildOffWithThread(TCHAR *pBuf)
{
	int index = 0;
	BYTE error_code = 0;
	USER *pUser = NULL;
	BOOL bRes = TRUE;

	if(m_dwGuild <= 0) { error_code = ERR_8; goto go_result; }
	
	if(m_bGuildMaster) { error_code = ERR_8; goto go_result; }

	if(m_tGuildHouseWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING || m_tFortressWar == GUILD_WARRING) 
	{ 
		SendSystemMsg( IDS_USER_NO_OUT_IN_WAR, SYSTEM_ERROR, TO_ME);
		return; 
	}

	int Datalength;
	SQLDATAPACKET *pSDP;

	pSDP = new SQLDATAPACKET;
	pSDP->code = GUILD_OFF_WITH_THREAD;
	Datalength = 0;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pSDP->pData = NULL;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );
	return;	

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

	pUser = GetUser(m_uid);
	SendGuildOffUser(pUser);				// 促弗 辨靛盔俊霸 郴啊 呕硼茄 荤角阑 舅赴促.

	m_dwGuild = -1;							// 檬扁拳茄促.
	m_sGuildVersion = -1;					// 辨靛 巩剧 滚怜
	::ZeroMemory(m_strGuildName, sizeof(m_strGuildName));	
	m_bGuildMaster = FALSE;					// 辨靛 付胶磐
	m_bRepresentationGuild = FALSE;			// 鼻茄 措青 咯何	 	

	SendMyGuildInfo();						// 辨靛 呕硼甫 舅赴促.


//	SendSystemMsg( IDS_USER_OUT_GUILD, SYSTEM_NORMAL, TO_ME);
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛甫 呕硼茄 蜡历甫 促弗 辨靛盔俊霸 舅赴促.
//
void USER::SendGuildOffUser(USER *pUser)
{
	int i;
	int nLen = 0;	
	int nCount = -1;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(m_dwGuild <= 0) return;

	USER *pGUser = NULL;

	CBufferEx TempBuf;

	CString strMsg = _T("");
	strMsg.Format( IDS_USER_OUT_MEMBER, pUser->m_strUserID);

	for(i = 0; i< MAX_USER; i++)
	{
		pGUser = m_pCom->GetUserUid(i);

		if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;		

		if(m_dwGuild == pGUser->m_dwGuild) pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
	}


}

////////////////////////////////////////////////////////////////////////////////
//	辨付啊 磊扁 辨靛甫 秦魂 矫挪促.
//
void USER::GuildDisperse(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int i;//, j;
	int index = 0, nLen = 0;
	BYTE error_code = 0;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	CStore *pStore = NULL;
	CGuildFortress *pFort = NULL;

	if( m_curz == 56 || m_curz == 57 || m_curz == 58 || m_curz == 59 )	// ARK俊辑绰 辨靛 秦魂 且 荐 绝促.
	{
		SendSystemMsg( _T("A.R.K.俊辑绰 辨靛甫 秦魂 且 荐 绝嚼聪促."), SYSTEM_ERROR, TO_ME);
		return;
	}

	if(!m_bGuildMaster) 
	{
		SendSystemMsg( IDS_USER_NO_PERMISSION_USE, SYSTEM_ERROR, TO_ME);		
		return;
	}

	if(m_dwGuild <= 0 || m_dwGuild >= g_arGuildData.GetSize()) return;

	if(CheckInGuildWarring()) return;								// 辨傈吝俊绰 倾遏罙荐绝促.

	// Field War老锭
	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)
	{
		SendSystemMsg( IDS_USER_CANT_DISMISS_IN_WAR, SYSTEM_ERROR, TO_ME);
		return;
	}

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i]) continue;

		pFort = g_arGuildFortress[i];

		if(pFort->m_iGuildSid == m_dwGuild)
		{
			SendSystemMsg( IDS_USER_CANT_DISMISS_FORTRESS, SYSTEM_ERROR, TO_ME);
			return;
		}
	}

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) 
	{
		ReleaseGuild();				// 秦力...
		return;
	}

	nLen = strlen(m_strUserID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH)
	{
		ReleaseGuild();				
		return;
	}

	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0)
	{
		ReleaseGuild();				
		return;
	}

	// alisia - 辨靛 芒绊甫 荤侩吝捞 酒聪搁 荤侩吝栏肺 父甸绊 辨靛 秦魂阑 脚没茄饶 府畔茄促.
	if(InterlockedCompareExchange((LONG*)&g_arGuildData[m_dwGuild]->m_lUsed, (long)1, (long)0) == (long)0)
	{
		g_pMainDlg->BridgeServerGuildDisperseReq( m_uid, m_strUserID, (int)m_dwGuild );
	}
	else
	{
		SendSystemMsg( IDS_USER_CANT_DISMISS_IN_BANK, SYSTEM_ERROR, TO_ME);
	}
	ReleaseGuild();
	return;





	// 秦寸 辨靛 芒绊啊 荤侩吝捞 酒聪搁 矫累...
	if(InterlockedCompareExchange((LONG*)&g_arGuildData[m_dwGuild]->m_lUsed, (long)1, (long)0) == (long)0)
	{
		if(!DeleteGuildDB()) 				// DB 昏力
		{
			ReleaseGuild();
			::InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, 0);
			return;
		}
											// 惑痢阑 家蜡 沁促搁 檬扁拳	
/*		for(i = 0; i < g_arStore.GetSize(); i++)
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

		// 辨靛窍快胶啊 乐促搁 捞侩阂啊肺 父甸绢 霖促. 
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
	}
	else 
	{
		ReleaseGuild();
		SendSystemMsg( IDS_USER_CANT_DISMISS_IN_BANK, SYSTEM_ERROR, TO_ME);
	}

	UpdateUserData();
//	ReleaseGuild();
}

void USER::GuildDisperseWithThread(TCHAR *pBuf)
{
	int i, j;
	int index = 0, nLen = 0;
	BYTE error_code = 0;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	CStore *pStore = NULL;
	CGuildFortress *pFort = NULL;

	if(!m_bGuildMaster) return;
	if(m_dwGuild <= 0 || m_dwGuild >= g_arGuildData.GetSize()) return;

	// 傍己傈老锭...
	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		pFort = g_arGuildFortress[i];
		if(!pFort) continue;

		if(g_arGuildFortress[i]->m_lUsed == 1)
		{
			if(pFort->m_iGuildSid == m_dwGuild)	// 规绢螟牢瘤
			{
				SendSystemMsg( IDS_USER_CANT_DISMISS_IN_WAR, SYSTEM_ERROR, TO_ME);
				return;
			}

			for(j =0; j < GUILD_ATTACK_MAX_NUM; j++)// 傍拜螟牢瘤..
			{
				if(pFort->m_arAttackGuild[j].lGuild == m_dwGuild)
				{
					SendSystemMsg( IDS_USER_CANT_DISMISS_IN_WAR, SYSTEM_ERROR, TO_ME);
					return;
				}
			}
		}
	}
	// 辨靛惑痢...
/*	for(i = 0; i < g_arStore.GetSize(); i++)
	{
		pStore = g_arStore[i];
		if(!pStore) continue;

		if(g_arStore[i]->m_lUsed == 1)
		{
			if(pStore->m_iGuildSid == m_dwGuild)	// 规绢螟牢瘤
			{
				SendSystemMsg( IDS_USER_CANT_DISMISS_IN_WAR, SYSTEM_ERROR, TO_ME);
				return;
			}

			for(j =0; j < GUILD_ATTACK_MAX_NUM; j++)// 傍拜螟牢瘤..
			{
				if(pStore->m_arAttackGuild[j] == m_dwGuild)
				{
					SendSystemMsg( IDS_USER_CANT_DISMISS_IN_WAR, SYSTEM_ERROR, TO_ME);
					return;
				}
			}
		}
	}
*/
	// Virtual Room俊 乐阑锭
	for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)
	{
		if(!g_arGuildHouseWar[i]) continue;

		if(g_arGuildHouseWar[i]->m_CurrentGuild.lUsed == 1)
		{
			if(g_arGuildHouseWar[i]->m_CurrentGuild.lGuild == m_dwGuild)
			{
				SendSystemMsg( IDS_USER_CANT_DISMISS_IN_WAR, SYSTEM_ERROR, TO_ME);
				return;
			}
		}
	}

	// Field War老锭
	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)
	{
		SendSystemMsg( IDS_USER_CANT_DISMISS_IN_WAR, SYSTEM_ERROR, TO_ME);
		return;
	}

	int Datalength;
	SQLDATAPACKET *pSDP;

	pSDP = new SQLDATAPACKET;
	pSDP->code = GUILD_DISPERSE;
	Datalength = 0;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pSDP->pData = NULL;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );

}

////////////////////////////////////////////////////////////////////////////////
//	辨付啊 辨靛盔阑 硼免矫挪促. ^^
//
void USER::GuildKickOut(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	BYTE error_code = 0;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	BOOL bRes = TRUE;

	CString strTemp = _T("");

	TCHAR szUserName[255];
	::ZeroMemory(szUserName, sizeof(szUserName));

	if(!m_bGuildMaster && !m_bRepresentationGuild) return;

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // 肋给等 蜡历酒捞叼
	{
		error_code = ERR_6;
		goto go_result;
	}

	if(m_dwGuild <= 0) { error_code = ERR_8; goto go_result; }

	if(CheckInGuildWarring()) return;			// 辨傈吝俊绰 倾遏罙荐绝促.
	
	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)									// 辨靛啊 绝栏搁
	{
		ReleaseGuild();		
		error_code = ERR_7;
		goto go_result;
	}
												// 辨靛 付胶磐啊 呕硼狼荤甫 灌洒搁 俊矾...^^			
//	if(strcmp(pGuild->m_strMasterName, szUserName) == 0)
	if( !pGuild->IsMasterPower(m_strUserID) )
	{
		ReleaseGuild();				

		m_bGuildMaster = FALSE;
		m_bRepresentationGuild = FALSE;
		error_code = ERR_10;
		goto go_result;
	}

	if( pGuild->IsMasterPower(szUserName) )
	{
		ReleaseGuild();				
		error_code = ERR_8;
		goto go_result;
	}
	
	index = -1;
	index = pGuild->GetUser(szUserName);			// 秦寸 捞抚狼 辨靛盔捞 乐绰瘤..

	if(index < 0)
	{
		ReleaseGuild();		
		error_code = ERR_8;
		goto go_result;
	}
														// 泅犁 辨靛芒绊 捞侩吝捞搁 眠规捞 角菩...
	if(strcmp(pGuild->m_strUsedUser, szUserName) == 0)
	{
		ReleaseGuild();	
		error_code = 255;
		goto go_result;
	}

	// alisia
//	g_pMainDlg->BridgeServerGuildOffReq( m_uid, m_strUserID, szUserName, (int)m_dwGuild );
//	ReleaseGuild();
//	return;



	if(!DeleteGuildUser(szUserName))		// 抛捞喉俊辑 昏力
	{
		ReleaseGuild();				// 秦力...
		error_code = 255;
		goto go_result;
	}



	pGuild->RemoveUser(szUserName);		// 皋葛府惑俊辑 昏力
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

	DelGuildUserInFortress(szUserName, m_dwGuild);		// 檬扁拳甫 秦具茄促.

	pUser = GetUser(szUserName);			// 泅犁 立加吝牢 蜡历搁...
	if(pUser)
	{
		pUser->m_dwGuild = -1;
		pUser->m_sGuildVersion = -1;		// 辨靛 巩剧 滚怜
		::ZeroMemory(pUser->m_strGuildName, sizeof(pUser->m_strGuildName));	
		pUser->m_bGuildMaster = FALSE;			// 辨靛 付胶磐
		pUser->m_bRepresentationGuild = FALSE;	// 鼻茄 措青 咯何	 	
		pUser->SendMyGuildInfo();
		pUser->SendSystemMsg( IDS_USER_KICKOUT_GUILD, SYSTEM_NORMAL, TO_ME);

		SendGuildOffUser(pUser);
	}

	UpdateUserData();

	strTemp.Format( IDS_USER_KICKOUT_MEMBER, szUserName);
	SendSystemMsg((LPTSTR)(LPCTSTR)strTemp, SYSTEM_NORMAL, TO_ME);
}

void USER::GuildKickOutWithThread(TCHAR *pBuf)
{
	int index = 0;
	BYTE error_code = 0;

	USER *pUser = NULL;
	BOOL bRes = TRUE;

	CString strTemp = _T("");

	TCHAR szUserName[255];
	::ZeroMemory(szUserName, sizeof(szUserName));

	if(!m_bGuildMaster) return;

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // 肋给等 蜡历酒捞叼
	{
		error_code = ERR_6;
		goto go_result;
	}

	if(m_dwGuild <= 0) { error_code = ERR_8; goto go_result; }

	int Datalength;
	SQLDATAPACKET *pSDP;
	BYTE *pData;

	pSDP = new SQLDATAPACKET;
	pSDP->code = GUILD_KICK_OUT;
	Datalength = nLength;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pData = new BYTE[Datalength+1];
	memset(pData, NULL, Datalength+1);
	memcpy(	pData, pBuf, Datalength );
	pSDP->pData = pData;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );

	return;	

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

	pUser = GetUser(szUserName);			// 泅犁 立加吝牢 蜡历搁...
	if(pUser)
	{
		pUser->m_dwGuild = -1;
		pUser->m_sGuildVersion = -1;		// 辨靛 巩剧 滚怜
		::ZeroMemory(pUser->m_strGuildName, sizeof(pUser->m_strGuildName));	
		pUser->m_bGuildMaster = FALSE;			// 辨靛 付胶磐
		pUser->m_bRepresentationGuild = FALSE;	// 鼻茄 措青 咯何	 	
		pUser->SendMyGuildInfo();
		pUser->SendSystemMsg( IDS_USER_KICKOUT_GUILD, SYSTEM_NORMAL, TO_ME);

		SendGuildOffUser(pUser);
	}

	strTemp.Format( IDS_USER_KICKOUT_MEMBER, szUserName);
	SendSystemMsg((LPTSTR)(LPCTSTR)strTemp, SYSTEM_NORMAL, TO_ME);
}

////////////////////////////////////////////////////////////////////////////////
//	秦寸 蜡历 辨靛 沥焊甫 焊辰促.
//
void USER::SendGuildInfo(USER *pUser)
{
	CBufferEx TempBuf;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	TempBuf.Add(GUILD_INFO);
	TempBuf.Add(pUser->m_uid + USER_BAND);
	TempBuf.Add((int)pUser->m_dwGuild);
	TempBuf.AddString(pUser->m_strGuildName);
	TempBuf.Add((short)pUser->m_sGuildVersion);

	pUser->Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	郴 辨靛 沥焊甫 焊辰促.
//
void USER::SendMyGuildInfo()
{
	CBufferEx TempBuf;

	TempBuf.Add(GUILD_INFO);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((int)m_dwGuild);
	TempBuf.AddString(m_strGuildName);
	TempBuf.Add((short)m_sGuildVersion);

	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊 酒捞袍阑 啊瘤绊 柯促.
//
void USER::GuildWharehouseOpenReq()
{
	int i, j;
	int nLen = 0;
	
	CBufferEx TempBuf;

	CByteArray arItemSlotList;

	USER *pUser = NULL;

	CString guild = _T("");
	guild.Format("%s", m_strGuildName);
	if(guild.IsEmpty()) return;
	if(m_dwGuild <= 0) return;

	CGuild *pGuild = GetGuild(m_dwGuild);

	ReleaseGuild();

	if(!pGuild)
	{
		m_dwGuild = -1;
		return;
	}

	if(InterlockedCompareExchange((LONG*)&g_arGuildData[m_dwGuild]->m_lUsed, (long)1, (long)0) == (long)0)
	{
		m_dwGuildDN = 0;
		InitGuildItem();								// 函荐甫 柄昌捞 技殴茄促.

		nLen = strlen(m_strUserID);
		if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) 
		{ 
			InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0); 
			return; 
		}

		if(!LoadGuildWarehouse())						// 辨靛 酒捞袍阑 贸澜 立加且锭 啊瘤绊 柯促.
		{ 
			InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0); 
			return; 
		}
		
		g_arGuildData[m_dwGuild]->m_iUsedUser = m_uid;
		strncpy(g_arGuildData[m_dwGuild]->m_strUsedUser, m_strUserID, nLen);

		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
		{												// 泅犁 焊包等 酒捞袍父 焊咯林扁困秦 沥纺茄促. 
			if(m_GuildItem[i].sSid >= 0)
			{
				arItemSlotList.Add(i);
			}
		}

		TempBuf.Add(GUILD_WAREHOUSE_OPEN);
		TempBuf.Add((DWORD)m_dwGuildDN);
		TempBuf.Add((BYTE)arItemSlotList.GetSize());

		for(i = 0; i < arItemSlotList.GetSize(); i++)
		{
			BYTE tempSlot = 0;
			tempSlot = arItemSlotList[i];
			TempBuf.Add(tempSlot);
			TempBuf.Add((short)m_GuildItem[tempSlot].sLevel);
			TempBuf.Add((short)m_GuildItem[tempSlot].sSid);
			TempBuf.Add((short)m_GuildItem[tempSlot].sDuration);
			TempBuf.Add((short)m_GuildItem[tempSlot].sBullNum);
			TempBuf.Add((short)m_GuildItem[tempSlot].sCount);

			for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_GuildItem[tempSlot].tMagic[j]);

			TempBuf.Add((BYTE)m_GuildItem[tempSlot].tIQ);
		}

		Send(TempBuf, TempBuf.GetLength());
	}
	else 
	{
		CString strMsg = _T("");
		nLen = strlen(g_arGuildData[m_dwGuild]->m_strUsedUser);
		if(nLen <= 0) 
		{
			InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0);
			return;
		}

		if(strcmp(g_arGuildData[m_dwGuild]->m_strUsedUser, _ID(IDS_USER_OPERATOR)) == 0)
		{
			strMsg.Format( IDS_USER_CANT_USE_GUILD_BANK_NOW, g_arGuildData[m_dwGuild]->m_strUsedUser);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			return;
		}

		strMsg.Format( IDS_USER_GUILD_BANK_IN_USE, g_arGuildData[m_dwGuild]->m_strUsedUser);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

		if(nLen > 0)										// 辨靛 芒绊甫 捞侩吝 厚沥惑 辆丰登绰 蜡历甫 犬牢窍绰 例瞒. 
		{
			pUser = GetUser(g_arGuildData[m_dwGuild]->m_iUsedUser);
			//(g_arGuildData[m_dwGuild]->m_strUsedUser);			// 捞侩吝 厚沥惑 辆丰登搁 钱绢霖促.	
			if(!pUser || strcmp(pUser->m_strUserID, g_arGuildData[m_dwGuild]->m_strUsedUser) != 0)
			{
				g_arGuildData[m_dwGuild]->m_iUsedUser = -1;
				::ZeroMemory(g_arGuildData[m_dwGuild]->m_strUsedUser, sizeof(g_arGuildData[m_dwGuild]->m_strUsedUser));
				InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0); 
			}
		}
	}
}

void USER::GuildWharehouseOpenReqWithThread()
{
	int Datalength;
	SQLDATAPACKET *pSDP;

	pSDP = new SQLDATAPACKET;
	pSDP->code = DB_GUILD_WHEREHOUSE_EVENT_REQ;
	Datalength = 0;
	pSDP->dcount = Datalength;
	pSDP->UID = m_uid;
	pSDP->pData = NULL;

	EnterCriticalSection( &m_CS_SqlData );
	RecvSqlData.AddTail(pSDP);
	nSqlDataCount = RecvSqlData.GetCount();
	LeaveCriticalSection( &m_CS_SqlData );
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊甫 摧绰促. 
//
void USER::GuildWharehouseCloseReq()
{
	int nLen = 0;
	CString guild = _T("");
	guild.Format("%s", m_strGuildName);
	if(guild.IsEmpty()) return;
	if(m_dwGuild <= 0 || m_dwGuild >= g_arGuildData.GetSize()) return;
	if(!g_arGuildData[m_dwGuild]) return;

	nLen = strlen(m_strUserID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;
	if(strcmp(g_arGuildData[m_dwGuild]->m_strUsedUser, m_strUserID) != 0) return;

	g_arGuildData[m_dwGuild]->m_iUsedUser = -1;
	::ZeroMemory(g_arGuildData[m_dwGuild]->m_strUsedUser, sizeof(g_arGuildData[m_dwGuild]->m_strUsedUser));
	InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0);
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊 酒捞袍阑 佬绢柯促.
//
BOOL USER::LoadGuildWarehouse()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_GUILD_WAREHOUSE (\'%s\')}"), m_strGuildName);

	SQLUINTEGER iDN;
	SQLCHAR		strItem[_BANK_DB];
	
	SQLINTEGER	sInd;

	iDN = 0;
	::ZeroMemory(strItem, sizeof(strItem));

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Load Guild Warehouse Data !!\n");

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
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iDN,		sizeof(iDN),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY,	strItem,	sizeof(strItem),	&sInd );
		}
		else 
		{
			retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	StrToGuildItem((LPTSTR)strItem);
	m_dwGuildDN = iDN;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	m_GuildItem 俊 Buffer狼 郴侩阑 Copy 茄促.
//
void USER::StrToGuildItem(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	MYINT64 serial;

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
	{
		m_GuildItem[i].sLevel	= GetShort(pBuf, index);
		m_GuildItem[i].sSid		= GetShort(pBuf, index);
		m_GuildItem[i].sDuration = GetShort(pBuf, index);
		m_GuildItem[i].sBullNum	= GetShort(pBuf, index);
		m_GuildItem[i].sCount	= GetShort(pBuf, index);

		if(m_GuildItem[i].sCount <= 0) m_GuildItem[i].sSid = -1;
		for(j = 0; j < MAGIC_NUM; j++) m_GuildItem[i].tMagic[j] = GetByte(pBuf, index);
		
		m_GuildItem[i].tIQ = GetByte(pBuf, index);

		for( j = 0; j < 8; j++ ) serial.b[j] = GetByte( pBuf, index );

		m_GuildItem[i].iItemSerial = serial.i;
	}	
}

////////////////////////////////////////////////////////////////////////////////
//	Buffer俊 m_GuildItem狼 郴侩阑 Copy 茄促.
//
void USER::GuildItemToStr(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	MYINT64 serial;

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
	{
		SetShort(pBuf, m_GuildItem[i].sLevel,	index);
		SetShort(pBuf, m_GuildItem[i].sSid,		index);
		SetShort(pBuf, m_GuildItem[i].sDuration,index);
		SetShort(pBuf, m_GuildItem[i].sBullNum,	index);
		SetShort(pBuf, m_GuildItem[i].sCount,	index);

		for(j = 0; j < MAGIC_NUM; j++) SetByte(pBuf, m_GuildItem[i].tMagic[j], index);

		SetByte(pBuf, m_GuildItem[i].tIQ, index);

		serial.i = m_GuildItem[i].iItemSerial;

		for(j = 0; j < 8; j++ ) SetByte( pBuf, serial.b[j], index );
	}
}

///////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊俊辑 涝免陛, 涝免涝 酒捞袍阑 淬寸茄促.
//
void USER::GuildItemMoveReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int index = 0;
	BYTE type = GetByte(pBuf, index);

	if(m_dwGuild < 0 || m_dwGuild >= g_arGuildData.GetSize()) return;
	if(!g_arGuildData[m_dwGuild]) return;

	int nLen = strlen(g_arGuildData[m_dwGuild]->m_strUsedUser);

	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;

	if(strcmp(m_strUserID, g_arGuildData[m_dwGuild]->m_strUsedUser) == 0)
	{
		if(g_arGuildData[m_dwGuild]->m_lUsed == 1)
		{
			switch(type)
			{
			case GUILD_ITEM_INPUT:
				GuildInPut(pBuf + index);
				break;
			case GUILD_ITEM_OUTPUT:
				GuildOutPut(pBuf + index);
				break;
			case GUILD_DN_INPUT:
				GuildInPutDN(pBuf + index);
				break;
			case GUILD_DN_OUTPUT:
				GuildOutPutDN(pBuf + index);
				break;
			}
		}
		else 
		{
			g_arGuildData[m_dwGuild]->m_iUsedUser = -1;
			::ZeroMemory(g_arGuildData[m_dwGuild]->m_strUsedUser, sizeof(g_arGuildData[m_dwGuild]->m_strUsedUser));
		}
	}
}

void USER::GuildItemMoveReqWithThread(TCHAR *pBuf)
{
	int index = 0;
	BYTE type = GetByte(pBuf, index);

	if(m_dwGuild < 0) return;
	if(!g_arGuildData[m_dwGuild]) return;

	int nLen = strlen(g_arGuildData[m_dwGuild]->m_strUsedUser);

	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;

	if(strcmp(m_strUserID, g_arGuildData[m_dwGuild]->m_strUsedUser) == 0)
	{
		if(g_arGuildData[m_dwGuild]->m_lUsed == 1)
		{
			int Datalength;
			BYTE *pData;
			SQLDATAPACKET *pSDP;
			pSDP = new SQLDATAPACKET;
			pSDP->code = DB_GUILD_ITEM_MOVE_REQ;

			if ( type == GUILD_ITEM_INPUT || type == GUILD_ITEM_OUTPUT)
				Datalength = sizeof(BYTE)+sizeof(BYTE)+sizeof(short);
			else
				Datalength = sizeof(BYTE)+sizeof(DWORD);

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
		else 
		{
			g_arGuildData[m_dwGuild]->m_iUsedUser = -1;
			::ZeroMemory(g_arGuildData[m_dwGuild]->m_strUsedUser, sizeof(g_arGuildData[m_dwGuild]->m_strUsedUser));
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊俊 酒捞袍阑 持绰促.
//
void USER::GuildInPut(TCHAR *pBuf)
{
	int i;
	int tDestSlot;
	int index = 0;
	int iOver = 0;
	short sSid = -1;
	short sHaveCount = 0;

	BYTE result = SUCCESS;

	CBufferEx TempBuf;

	ItemList MyItem[TOTAL_ITEM_NUM], BackItem;

	BYTE tSourceSlot = GetByte(pBuf, index);	// 荤侩磊 酒捞袍捞 乐带 浇吩 困摹
	short sCount = GetShort(pBuf, index);		// 倔付父怒 焊包
												// 坷肺瘤 牢亥父 啊瓷窍促.			
	if(tSourceSlot < EQUIP_ITEM_NUM || tSourceSlot >= TOTAL_INVEN_MAX) { result = FAIL; goto go_result; }

	sSid = m_UserItem[tSourceSlot].sSid;
	sHaveCount = m_UserItem[tSourceSlot].sCount;
												// 肋给等 sSid		
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { result = FAIL; goto go_result; }

	if(g_arItemTable[sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
	{
		SendSystemMsg( IDS_USER_CANT_SHARE_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
		result = FAIL; goto go_result; 
	}

	if(sCount >= BANK_MAX_ITEM) 
	{
		SendSystemMsg( IDS_USER_ONCE_MOVE_MAX, SYSTEM_ERROR, TO_ME);
		result = FAIL; goto go_result;
	}
												
	if(sCount <= 0 || sCount > sHaveCount) { result = FAIL; goto go_result; }	// 家瘤茄 酒捞袍 荐焊促 腹阑 版快			
	if(g_arItemTable[sSid]->m_sDuration > 0 && sCount > 1) { result = FAIL; goto go_result; }
	
	for(i = 0; i < TOTAL_ITEM_NUM; i++)	// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

	ReSetItemSlot(&BackItem);					// DB角菩俊 措厚茄 归诀侩 檬扁拳
												// 老窜 般磨荐 乐绰瘤, 般摹搁 鞍篮 酒捞袍捞 乐绰瘤 茫酒夯促.
	tDestSlot = GetSameItem(m_UserItem[tSourceSlot], GUILD_SLOT);
	
	/***************************辨靛 芒绊 诀公 贸府********************************************/
	if(tDestSlot >= 0)							// 亲惑 货肺捞 眠啊 登绰巴阑 扁霖栏肺 贸府
	{											
		BackItem = m_GuildItem[tDestSlot];

		if(m_GuildItem[tDestSlot].sCount >= BANK_MAX_ITEM)
		{
			SendSystemMsg( IDS_USER_SAVE_MAX_COUNT, SYSTEM_ERROR, TO_ME);
			result = FAIL; goto go_result;
		}

		if((m_GuildItem[tDestSlot].sCount + sCount) > BANK_MAX_ITEM)
		{										// MAX蔼阑 逞栏搁 菜盲快绊 唱赣瘤 贸府			
			iOver =m_GuildItem[tDestSlot].sCount + sCount - BANK_MAX_ITEM;
			if(iOver <= 0) { result = FAIL; goto go_result; }

			m_GuildItem[tDestSlot].sCount = BANK_MAX_ITEM;
			sCount = sCount - iOver;
		}
		else m_GuildItem[tDestSlot].sCount += sCount;
	}
	else
	{											//	眠啊
		tDestSlot = GetEmptySlot(GUILD_SLOT);

		if(tDestSlot == -1) 
		{
			result = FAIL; goto go_result; 
		}

		m_GuildItem[tDestSlot].sLevel = m_UserItem[tSourceSlot].sLevel;
		m_GuildItem[tDestSlot].sSid = m_UserItem[tSourceSlot].sSid;
		m_GuildItem[tDestSlot].sDuration = m_UserItem[tSourceSlot].sDuration;
		m_GuildItem[tDestSlot].sBullNum = m_UserItem[tSourceSlot].sBullNum;
		m_GuildItem[tDestSlot].sCount = sCount;
		for(i = 0; i < MAGIC_NUM; i++) m_GuildItem[tDestSlot].tMagic[i] = m_UserItem[tSourceSlot].tMagic[i];
		m_GuildItem[tDestSlot].tIQ = m_UserItem[tSourceSlot].tIQ;
		m_GuildItem[tDestSlot].iItemSerial = m_UserItem[tSourceSlot].iItemSerial;
	}

	index = 0;
	index = g_arItemTable[m_UserItem[tSourceSlot].sSid]->m_byWeight * sCount;
	/**************************蜡历 牢亥 贸府*********************************************/
	if(sCount >= sHaveCount && iOver == 0)
	{
		MakeItemLog( &m_GuildItem[tDestSlot], ITEMLOG_GUILD_BANKIN );
		ReSetItemSlot(&m_UserItem[tSourceSlot]);	
	}
	else m_UserItem[tSourceSlot].sCount -= sCount;
	
	/**************************DB Update 贸府*********************************************/
	if(UpdateGuildWarehouse() == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 汗盔
		{
			m_UserItem[i] = MyItem[i];
		}
		m_GuildItem[tDestSlot] = BackItem;

		result = FAIL;

		FlushItemLog( FALSE );

		goto go_result;
	}

	FlushItemLog( TRUE );

	UpdateUserData();

go_result:
	TempBuf.Add(GUILD_ITEM_MOVE_RESULT);

	if(result == FAIL)
	{
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	m_iCurWeight -= index;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	result = (BYTE)0x01;
	TempBuf.Add(result);

	TempBuf.Add((BYTE)tDestSlot);				// 辨靛	
	TempBuf.Add(m_GuildItem[tDestSlot].sLevel);
	TempBuf.Add(m_GuildItem[tDestSlot].sSid);
	TempBuf.Add(m_GuildItem[tDestSlot].sDuration);
	TempBuf.Add(m_GuildItem[tDestSlot].sBullNum);
	TempBuf.Add(m_GuildItem[tDestSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_GuildItem[tDestSlot].tMagic[i]);
	TempBuf.Add(m_GuildItem[tDestSlot].tIQ);

	TempBuf.Add((BYTE)tSourceSlot);				// 蜡历 牢亥
	TempBuf.Add(m_UserItem[tSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[tSourceSlot].sSid);
	TempBuf.Add(m_UserItem[tSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[tSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSourceSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tSourceSlot].tMagic[i]);
	TempBuf.Add(m_UserItem[tSourceSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());

	GetRecoverySpeed();							// 雀汗加档 眉农...

//	SendQuickChange();							// 狞酒捞袍 殿废茄霸 秒家登绰瘤 舅酒夯促. 
//	SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
}


////////////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊俊辑 酒捞袍阑 啊廉柯促.
//
void USER::GuildOutPut(TCHAR *pBuf)
{
	int i, iWeight = 0;
	int tDestSlot;
	int index = 0;
	int iOver = 0;
	short sSid = -1;
	short sHaveCount = 0;

	BYTE result = SUCCESS;

	CBufferEx TempBuf;

	ItemList MyItem[TOTAL_ITEM_NUM], BackItem;

	if(!m_bGuildMaster) 
	{
		SendSystemMsg( IDS_USER_NO_PERMISSION_USE, SYSTEM_NORMAL, TO_ME);
		return;
	}

	BYTE tSourceSlot = GetByte(pBuf, index);	// 荤侩磊 酒捞袍捞 乐带 浇吩 困摹
	short sCount = GetShort(pBuf, index);
												// 坷肺瘤 牢亥父 啊瓷窍促.			
	if(tSourceSlot >= TOTAL_BANK_ITEM_NUM) { result = FAIL; goto go_result; }

	sSid = m_GuildItem[tSourceSlot].sSid;
	sHaveCount = m_GuildItem[tSourceSlot].sCount;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { result = FAIL; goto go_result; }

	if(sCount < 0 || sCount > sHaveCount/* || sCount > BANK_MAX_ITEM*/) { result = FAIL; goto go_result; }

//	if(g_arItemTable[sSid]->m_byWear <= 5 && sCount > 1) { result = FAIL; goto go_result; }
	if(g_arItemTable[sSid]->m_sDuration > 0 && sCount > 1) { result = FAIL; goto go_result; }

	iWeight = g_arItemTable[sSid]->m_byWeight * sCount;
	if(m_iMaxWeight < m_iCurWeight + iWeight)
	{
		SendSystemMsg( IDS_USER_OVER_WEIGHT1, SYSTEM_ERROR, TO_ME);
		result = FAIL; 
		goto go_result;
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++)	// 酒捞袍 沥焊 归诀
	{
		MyItem[i] = m_UserItem[i];
	}

	ReSetItemSlot(&BackItem);					// DB角菩俊 措厚茄 归诀侩 檬扁拳
	BackItem = m_GuildItem[tSourceSlot];

												
	tDestSlot = GetSameItem(m_GuildItem[tSourceSlot], INVENTORY_SLOT);
	
	/***************************辨靛芒绊 诀公 贸府********************************************/
	if(tDestSlot >= 0)							// 亲惑 货肺捞 眠啊 登绰巴阑 扁霖栏肺 贸府
	{		
		m_UserItem[tDestSlot].sCount += sCount;
	}
	else
	{											//	眠啊
		tDestSlot = GetEmptySlot(INVENTORY_SLOT);

		if(tDestSlot == -1) 
		{
			result = FAIL; goto go_result; 
		}

		m_UserItem[tDestSlot].sLevel = m_GuildItem[tSourceSlot].sLevel;
		m_UserItem[tDestSlot].sSid = m_GuildItem[tSourceSlot].sSid;
		m_UserItem[tDestSlot].sDuration = m_GuildItem[tSourceSlot].sDuration;
		m_UserItem[tDestSlot].sBullNum = m_GuildItem[tSourceSlot].sBullNum;
		m_UserItem[tDestSlot].sCount = sCount;
		for(i = 0; i < MAGIC_NUM; i++) m_UserItem[tDestSlot].tMagic[i] = m_GuildItem[tSourceSlot].tMagic[i];
		m_UserItem[tDestSlot].tIQ = m_GuildItem[tSourceSlot].tIQ;
		m_UserItem[tDestSlot].iItemSerial = m_GuildItem[tSourceSlot].iItemSerial;

		MakeItemLog( &m_UserItem[tDestSlot], ITEMLOG_GUILD_BANKOUT );
	}

	/**************************辨靛  贸府*********************************************/
	if(sCount >= sHaveCount) ReSetItemSlot(&m_GuildItem[tSourceSlot]);	
	else m_GuildItem[tSourceSlot].sCount -= sCount;
	
	/**************************DB Update 贸府*********************************************/
	if(UpdateGuildWarehouse() == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)// 酒捞袍 沥焊 汗盔
		{
			m_UserItem[i] = MyItem[i];
		}
		m_GuildItem[tSourceSlot] = BackItem;

		result = FAIL;

		FlushItemLog( FALSE );

		goto go_result;
	}

	FlushItemLog( TRUE );

	UpdateUserData();

go_result:
	TempBuf.Add(GUILD_ITEM_MOVE_RESULT);

	if(result == FAIL)
	{
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	result = (BYTE)0x02;
	TempBuf.Add(result);

	TempBuf.Add((BYTE)tSourceSlot);
	TempBuf.Add(m_GuildItem[tSourceSlot].sLevel);
	TempBuf.Add(m_GuildItem[tSourceSlot].sSid);
	TempBuf.Add(m_GuildItem[tSourceSlot].sDuration);
	TempBuf.Add(m_GuildItem[tSourceSlot].sBullNum);
	TempBuf.Add(m_GuildItem[tSourceSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_GuildItem[tSourceSlot].tMagic[i]);
	TempBuf.Add(m_GuildItem[tSourceSlot].tIQ);

	TempBuf.Add((BYTE)tDestSlot);
	TempBuf.Add(m_UserItem[tDestSlot].sLevel);
	TempBuf.Add(m_UserItem[tDestSlot].sSid);
	TempBuf.Add(m_UserItem[tDestSlot].sDuration);
	TempBuf.Add(m_UserItem[tDestSlot].sBullNum);
	TempBuf.Add(m_UserItem[tDestSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tDestSlot].tMagic[i]);
	TempBuf.Add(m_UserItem[tDestSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());

	if(m_UserItem[tDestSlot].sSid < 0 || m_UserItem[tDestSlot].sSid >= g_arItemTable.GetSize()) return;

	m_iCurWeight += (g_arItemTable[m_UserItem[tDestSlot].sSid]->m_byWeight * sCount);
	GetRecoverySpeed();							// 雀汗加档 眉农...
//	SendItemWeightChange();				// 泅犁 酒捞袍 公霸甫 焊辰促.
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊俊 捣阑 持绰促.
//
void USER::GuildInPutDN(TCHAR *pBuf)
{
	CBufferEx TempBuf;

	BYTE result; 
	int index = 0;
	DWORD BackGuildDN = 0, BackMyDN = 0;

	DWORD InputDN = GetDWORD(pBuf, index);

	TempBuf.Add(GUILD_ITEM_MOVE_RESULT);

	if(InputDN > m_dwDN) 
	{
		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	
	BackMyDN = m_dwDN;
	BackGuildDN = m_dwGuildDN;
										// 篮青俊 涝陛
	if(!CheckMaxValueReturn(m_dwGuildDN, InputDN))
	{									// 窜, MAX蔼捞搁 瞒咀篮...
		CheckMaxValue(m_dwGuildDN, InputDN);
		if(m_dwGuildDN < InputDN) InputDN = 0;
		else InputDN = m_dwGuildDN - InputDN;
	}
	else m_dwGuildDN += InputDN;
										// 啊瘤绊 乐绰 家瘤陛俊辑 猾促.
	if(m_dwDN <= InputDN) m_dwDN = 0;
	else m_dwDN -= InputDN;

	MakeMoneyLog( InputDN, ITEMLOG_GUILD_BANKIN_MONEY, NULL, m_dwGuildDN );

	if(UpdateGuildWarehouse() == FALSE)		// DB UpDate
	{
		m_dwDN = BackMyDN;
		m_dwGuildDN = BackGuildDN;

		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		FlushItemLog( FALSE );
		return;
	}

	FlushItemLog( TRUE );

	UpdateUserData();

	result = (BYTE)0x03;				// 3 : DN 涝陛
	TempBuf.Add(result);

	TempBuf.Add(m_dwGuildDN);			// 篮青郴 涝陛茄 醚陛咀
	TempBuf.Add(m_dwDN);				// 家瘤茄 醚陛咀

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊俊辑 捣阑 啊廉柯促.
//
void USER::GuildOutPutDN(TCHAR *pBuf)
{
	CBufferEx TempBuf;

	BYTE result; 
	int index = 0;

	if(!m_bGuildMaster) 
	{
		SendSystemMsg( IDS_USER_NO_PERMISSION_USE, SYSTEM_NORMAL, TO_ME);
		return;
	}

	DWORD BackGuildDN = 0, BackMyDN = 0;

	DWORD OutputDN = GetDWORD(pBuf, index);

	TempBuf.Add(GUILD_ITEM_MOVE_RESULT);

	if(OutputDN > m_dwGuildDN)			// 呈公 农搁
	{
		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	BackMyDN = m_dwDN;					// 归诀
	BackGuildDN = m_dwGuildDN;
										
	if(!CheckMaxValueReturn(m_dwDN, OutputDN))
	{									// 窜, MAX蔼捞搁 瞒咀篮...
		CheckMaxValue(m_dwDN, OutputDN);
		if(m_dwDN < OutputDN) OutputDN = 0;
		else OutputDN = m_dwDN - OutputDN;
	}
	else m_dwDN += OutputDN;
										// 辨靛俊辑 猾促.
	if(OutputDN == m_dwGuildDN) m_dwGuildDN = 0;
	else m_dwGuildDN -= OutputDN;

	MakeMoneyLog( OutputDN, ITEMLOG_GUILD_BANKOUT_MONEY, NULL, m_dwGuildDN );

	if(UpdateGuildWarehouse() == FALSE)		// DB UpDate
	{
		m_dwDN = BackMyDN;
		m_dwGuildDN = BackGuildDN;

		result = FAIL;
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());

		FlushItemLog( FALSE );
		return;
	}

	FlushItemLog( TRUE );

	UpdateUserData();

	result = (BYTE)0x04;				// 4 : DN 免陛
	TempBuf.Add(result);

	TempBuf.Add(m_dwGuildDN);			// 辨靛郴 涝陛茄 醚陛咀
	TempBuf.Add(m_dwDN);				// 家瘤茄 醚陛咀

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	辨靛 芒绊 函拳甫 盎脚茄促.
//
BOOL USER::UpdateGuildWarehouse()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB], strGuildItem[_BANK_DB], strQuickItem[_QUICKITEM_DB];
	int				i;

	SQLSMALLINT	sRet = 0;
	SQLINTEGER sRetInd;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strGuildItem, sizeof(strGuildItem));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strQuickItem, sizeof(strQuickItem));

	GuildItemToStr(strGuildItem);
	UserItemToStr(strItem);

	SDWORD sGuildItemLen= sizeof(strGuildItem);
	SDWORD sItemLen		= sizeof(strItem);
	SDWORD sQuickLen	= sizeof(strQuickItem);

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_WAREHOUSE_DATA (\'%s\', \'%s\', %d, ?, ?, %d, ?, ?)}"), m_strUserID, m_strGuildName, m_dwDN, m_dwGuildDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild Warehouse Data !!\n");

		//g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}
	
	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strQuickItem),	0, (TCHAR*)strQuickItem,	0, &sQuickLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strGuildItem),	0, (TCHAR*)strGuildItem,0, &sGuildItemLen );
		SQLBindParameter( hstmt, i++ ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &sRetInd);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	
	if(sRet == 255) return FALSE;

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	E_Body 惑痢阑 楷促.
//
void USER::SendEBodyOpen(int nStore)
{
/*
	CBufferEx TempBuf;
	int i = 0;
	short sid = 0;
	short sCount = 0;
	DWORD dwCost = 0, dwXP = 0;

	CStore* pStore = NULL;

	pStore = GetEbodyStore(nStore);
	if(pStore == NULL) return;

	sCount = pStore->m_arItems.GetSize();
	TempBuf.Add(EBODY_OPEN);
	TempBuf.Add((short)nStore);
	TempBuf.Add(sCount);

	for(i = 0; i < pStore->m_arItems.GetSize(); i++)
	{
		sid = (short)pStore->m_arItems[i];

		dwCost = g_arEBodyTable[sid]->m_iDN;
		dwXP = g_arEBodyTable[sid]->m_iXP;

		TempBuf.Add(sid);
		TempBuf.Add(dwXP);
		TempBuf.Add(dwCost);
	}

	Send(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////////
//	E_Body 备涝阑 茄促.
//
void USER::EBodyBuyReq(TCHAR *pBuf)
{
/*
	return;			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	int index = 0;
	short sSid = 0;
	short sStoreID = 0;			// 惑痢 酒捞叼

	BOOL bSell = FALSE;	

	CBufferEx TempBuf;

	BYTE tNeedClass;
	BYTE tMyClass;
	BYTE tTemp = 1;	
	BYTE tFire = 0;
	BYTE tEdge = 0;
	BYTE tStaff = 0;
	BYTE tBrawl = 0;

	CStore* pStore = NULL;

	sStoreID = GetShort(pBuf, index);

	pStore = GetEbodyStore(sStoreID);
	if(pStore == NULL) return;//{ tResult = FAIL; goto go_result; }

	sSid = GetShort(pBuf, index);

	if(sSid < 0 || sSid >= g_arEBodyTable.GetSize()) return;

	for(int i = 0; i < pStore->m_arItems.GetSize(); i++)
	{
		if(pStore->m_arItems[i] == sSid) { bSell = TRUE; break; }
	}

	if(!bSell) return;

	tNeedClass = g_arEBodyTable[sSid]->m_tClass;
	tMyClass = m_byClass;

	tFire	 = tTemp & tNeedClass; tTemp = 2; 
	tEdge	 = tTemp & tNeedClass; tTemp = 4;
	tStaff	 = tTemp & tNeedClass; tTemp = 8;
	tBrawl	 = tTemp & tNeedClass;

	tFire = tFire & tMyClass;
	tEdge = tEdge & tMyClass;
	tStaff = tStaff & tMyClass;
	tBrawl = tBrawl & tMyClass;

	tTemp = tFire^tEdge^tStaff^tBrawl;
	if(!tTemp) return;

	if(m_dwXP < g_arEBodyTable[sSid]->m_iXP) return;

	if(m_dwDN < g_arEBodyTable[sSid]->m_iDN) return;

	if(m_sSTR + g_arEBodyTable[sSid]->m_sAddBasic[0] < m_sTempSTR) return;
	if(m_sCON + g_arEBodyTable[sSid]->m_sAddBasic[1] < m_sTempCON) return;
	if(m_sDEX + g_arEBodyTable[sSid]->m_sAddBasic[2] < m_sTempDEX) return;
	if(m_sVOL + g_arEBodyTable[sSid]->m_sAddBasic[3] < m_sTempVOL) return;
	if(m_sWIS + g_arEBodyTable[sSid]->m_sAddBasic[4] < m_sTempWIS) return;

	if(g_arEBodyTable[sSid]->m_sAddBasic[0] != 0)
	{
		m_sSTR = m_sSTR + g_arEBodyTable[sSid]->m_sAddBasic[0];
	}

	if(g_arEBodyTable[sSid]->m_sAddBasic[1] != 0)
	{
		m_sCON = m_sCON + g_arEBodyTable[sSid]->m_sAddBasic[1];
	}

	if(g_arEBodyTable[sSid]->m_sAddBasic[2] != 0)
	{
		m_sDEX = m_sDEX + g_arEBodyTable[sSid]->m_sAddBasic[2];
	}

	if(g_arEBodyTable[sSid]->m_sAddBasic[3] != 0)
	{
		m_sVOL = m_sVOL + g_arEBodyTable[sSid]->m_sAddBasic[3];
	}

	if(g_arEBodyTable[sSid]->m_sAddBasic[4] != 0)
	{
		m_sWIS = m_sWIS + g_arEBodyTable[sSid]->m_sAddBasic[4];
	}

	SetUserToMagicUser();
	SendUserStatusSkill();

	TempBuf.Add(EBODY_BUY_RESULT);
	TempBuf.Add((BYTE)0x01);
	TempBuf.Add(sSid);

	Send(TempBuf, TempBuf.GetLength());
*/
}


////////////////////////////////////////////////////////////////////////////////
//	泅犁 立加吝牢 辨靛盔 沥焊甫 焊辰促.
//
void USER::GuildUserInfoReq(TCHAR *pBuf)
{
	int i;
	int nCount = 0; 
	int nLength = 0;
	USER *pUser = NULL;
	CGuild *pGuild = NULL;
//	CGuildUser *pGuildUser = NULL;

	TCHAR GuildMaster[CHAR_NAME_LENGTH + 1];						// 辨付 捞抚

	CBufferEx TempBuf, TempBufData; 
//	CWordArray	arLevel;

	if(m_dwGuild <= 0) return;										// 辨靛锅龋啊 绝栏搁 辨靛盔捞 酒聪促.
	
	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)														// 秦寸 辨靛啊 粮犁窍瘤 臼绰促.	
	{
		m_dwGuild = -1;
		ReleaseGuild();												// 秦力...
		return;
	}

	nCount = -1;
	nCount = pGuild->GetUser(m_strUserID);
	if(nCount < 0)													// 辨靛盔捞 酒聪搁	
	{
		m_dwGuild = -1;												// 老窜 辨靛 牢郸胶甫 檬扁拳
		ReleaseGuild();												// 秦力...
		return;
	}
/*
	nCount = pGuild->m_arUser.GetSize();

	if(nCount <= 0 || nCount > MAX_GUILD_USER)						// 辨靛牢盔荐啊 弥措摹甫 逞菌促.
	{
		ReleaseGuild();												// 秦力...
		return;
	}
*/
	nLength = strlen(pGuild->m_strMasterName);
	::ZeroMemory(GuildMaster, sizeof(GuildMaster));
	if(nLength > 0 && nLength <= CHAR_NAME_LENGTH) strncpy(GuildMaster, pGuild->m_strMasterName, nLength);
	else															// 秦寸 辨靛俊 辨付啊 粮犁窍瘤 臼绰促.	
	{
		m_dwGuild = -1;
		ReleaseGuild();												// 秦力...
		return;
	}

	ReleaseGuild();													// 秦力...

	nCount = 0;
	for(i = 0; i< MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if(m_dwGuild == pUser->m_dwGuild)
		{
			TempBufData.AddString(pUser->m_strUserID);
			TempBufData.Add(pUser->m_sLevel);
			nCount++;
		}
	}

	TempBuf.Add(GUILD_USER_INFO_RESULT);
	//TempBuf.Add((BYTE)0x00);										// 老馆 辨靛 蜡历啊 夸备
	TempBuf.Add(m_nGuildUserInfoType);								//yskang 0.2
	TempBuf.AddString(GuildMaster);									// 辨靛 付胶磐 捞抚阑 刚历
	TempBuf.Add((BYTE)nCount);										// 泅犁 辨靛盔 蜡历荐
	TempBuf.AddData(TempBufData, TempBufData.GetLength());
	m_nGuildUserInfoType = 0x00;
	Send(TempBuf, TempBuf.GetLength());
}

//	辨靛炉捞 傈眉 辨靛盔 沥焊甫 夸备.
//
void USER::GuildUserAllInforChat()
{
	int i;
	int nCount = 0; 
	int nLength = 0;
	USER *pUser = NULL;
	CGuild *pGuild = NULL;
//	CGuildUser *pGuildUser = NULL;

	CBufferEx TempBuf, TempBufData; 
//	CWordArray	arLevel;

	if(m_dwGuild <= 0) return;										// 辨靛锅龋啊 绝栏搁 辨靛盔捞 酒聪促.
	if(!m_bGuildMaster) return;										// 辨靛锅龋啊 绝栏搁 辨靛盔捞 酒聪促.
	
	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)														// 秦寸 辨靛啊 粮犁窍瘤 臼绰促.	
	{
		m_dwGuild = 0;
		ReleaseGuild();												// 秦力...
		return;
	}
/*	
	nCount = pGuild->m_arUser.GetSize();

	if(nCount <= 0 || nCount > MAX_GUILD_USER)						// 辨靛牢盔荐啊 弥措摹甫 逞菌促.
	{
		ReleaseGuild();												// 秦力...
		return;
	}
*/
	for(i = 0; i < MAX_GUILD_USER; i++)
	{
		if(pGuild->m_arUser[i].m_lUsed != 0)
		{
			TempBufData.AddString(pGuild->m_arUser[i].m_strUserId);
			TempBufData.Add((short)0x00);
			nCount++;
		}
	}

	ReleaseGuild();	// 秦力...
/*
	nCount = 0;
	for(i = 0; i< MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if(m_dwGuild == pUser->m_dwGuild)
		{
			TempBufData.AddString(pUser->m_strUserID);
			TempBufData.Add(pUser->m_sLevel);
			nCount++;
		}
	}
*/
	TempBuf.Add(GUILD_USER_INFO_RESULT);
	TempBuf.Add((BYTE)0x00);										// 老馆 辨靛 蜡历啊 夸备
	TempBuf.AddString(m_strUserID);									// 辨靛 付胶磐 捞抚阑 刚历
	TempBuf.Add((BYTE)nCount);										// 泅犁 辨靛盔 蜡历荐
	TempBuf.AddData(TempBufData, TempBufData.GetLength());
	
	Send(TempBuf, TempBuf.GetLength());
}
/*
////////////////////////////////////////////////////////////////////////////////
//	辨靛炉捞 傈眉 辨靛盔 沥焊甫 夸备.
//
void USER::GuildUserAllInforChat()
{
	int i;
	int nCount = 0; 
	int nLength = 0;
	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	CBufferEx TempBuf, TempBufData; 
	CWordArray	arLevel;

	if(m_dwGuild <= 0) return;										// 辨靛锅龋啊 绝栏搁 辨靛盔捞 酒聪促.
	if(!m_bGuildMaster) return;										// 辨靛锅龋啊 绝栏搁 辨靛盔捞 酒聪促.
	
	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)														// 秦寸 辨靛啊 粮犁窍瘤 臼绰促.	
	{
		m_dwGuild = 0;
		ReleaseGuild();												// 秦力...
		return;
	}
	
	nCount = pGuild->m_arUser.GetSize();

	if(nCount <= 0 || nCount > MAX_GUILD_USER)						// 辨靛牢盔荐啊 弥措摹甫 逞菌促.
	{
		ReleaseGuild();												// 秦力...
		return;
	}

	ReleaseGuild();	// 秦力...

	nCount = 0;

	for(i = 0; i< MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if(m_dwGuild == pUser->m_dwGuild)
		{
			TempBufData.AddString(pUser->m_strUserID);
			TempBufData.Add(pUser->m_sLevel);
			nCount++;
		}
	}

	TempBuf.Add(GUILD_USER_INFO_RESULT);
	TempBuf.Add((BYTE)0x00);										// 老馆 辨靛 蜡历啊 夸备
	TempBuf.AddString(m_strUserID);									// 辨靛 付胶磐 捞抚阑 刚历
	TempBuf.Add((BYTE)nCount);										// 泅犁 辨靛盔 蜡历荐
	TempBuf.AddData(TempBufData, TempBufData.GetLength());
	
	Send(TempBuf, TempBuf.GetLength());
}
*/
////////////////////////////////////////////////////////////////////////////////
//	辨靛 窍快胶盒剧俊 曼啊窍扁困秦 泅犁 鞍篮 粮狼 辨靛盔 沥焊甫 焊辰促.
//
void USER::SendGuildUserInfo(int zone)
{
	int i, index = -1;
	int nCount = 0; 
	int nLength = 0;

	BOOL bSame = FALSE;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
//	CGuildUser *pGuildUser = NULL;
	CGuildUser *pGuildUser = NULL;
	CGuildFortress *pFort = NULL;

	TCHAR GuildMaster[CHAR_NAME_LENGTH + 1];						// 辨付 捞抚

	CBufferEx TempBuf, TempBufData; 
//	CDWordArray	arUid;
	
//	arUid.RemoveAll();

	if(m_dwGuild <= 0) return;										// 辨靛锅龋啊 绝栏搁 辨靛盔捞 酒聪促.
	if(!m_bGuildMaster) return;										// 坷肺瘤 辨付父捞 脚没且荐 乐促.	

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i]) continue;

		pFort = g_arGuildFortress[i];

		if(pFort->m_iGuildSid == m_dwGuild) 
		{
			SendSystemMsg( IDS_USER_CANT_APPLY_HAVE_FORTRESS, SYSTEM_NORMAL, TO_ME);
			return;
		}
	}

	i = GetCityNumForVirtualRoom(zone);
	if(i < 0 || i >= g_arGuildHouseWar.GetSize()) return;

	if(g_arGuildHouseWar[i]->m_CurrentGuild.lUsed == 1) 
	{		
		SendSystemMsg( IDS_USER_ALREADY_OTHER_GUILD_USE, SYSTEM_ERROR, TO_ME);
		return;
	}

	for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)
	{
		if(g_arGuildHouseWar[i]->m_CurrentGuild.iWarZone == m_curz) { bSame = TRUE; break; }
	}

	if(bSame) return;

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)														// 秦寸 辨靛啊 粮犁窍瘤 臼绰促.	
	{
		m_dwGuild = -1;
		ReleaseGuild();												// 秦力...
		return;
	}

	index = pGuild->GetUser(m_strUserID);

	if(index < 0)														// 辨靛盔捞 酒聪搁	
	{
		m_dwGuild = -1;												// 老窜 辨靛 牢郸胶甫 檬扁拳
		ReleaseGuild();												// 秦力...
		return;
	}

/*	nCount = pGuild->m_arUser.GetSize();

	if(nCount <= 0 || nCount > MAX_GUILD_USER)						
	{
		ReleaseGuild();												// 秦力...
		return;
	}
*/
	nLength = strlen(pGuild->m_strMasterName);
	::ZeroMemory(GuildMaster, sizeof(GuildMaster));
	if(nLength > 0 && nLength <= CHAR_NAME_LENGTH) strncpy(GuildMaster, pGuild->m_strMasterName, nLength);
	else															// 秦寸 辨靛俊 辨付啊 粮犁窍瘤 臼绰促.	
	{
		m_dwGuild = -1;
		ReleaseGuild();												// 秦力...
		return;
	}

	ReleaseGuild();													// 秦力...

	nCount = 0;
	for(i = 0; i< MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if(m_dwGuild == pUser->m_dwGuild)
		{
			TempBufData.AddString(pUser->m_strUserID);
			TempBufData.Add(pUser->m_sLevel);
			TempBufData.Add(pUser->m_uid + USER_BAND);
			nCount++;
		}
	}

	TempBuf.Add(GUILD_USER_INFO_RESULT);
	TempBuf.Add((BYTE)0x01);										// 老馆 辨靛 蜡历啊 夸备
	TempBuf.AddString(m_strUserID);									// 辨靛 付胶磐 捞抚阑 刚历
	TempBuf.Add((BYTE)nCount);										// 泅犁 辨靛盔 蜡历荐
	TempBuf.AddData(TempBufData, TempBufData.GetLength());
	TempBuf.Add((short)zone);
	
	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////////
// 篮青沥焊甫 皋葛府 DB俊 历厘茄促 (泅犁绰 LoadUserBank俊辑父 何弗促.)
//
void USER::SetMemUserBank(LPCTSTR strItem)
{
	if(m_pSharedMemory->m_hMapping == NULL) return;
	if(m_pMD == NULL) return;
	if(m_pMD->m_uid == -1) return;

	if(m_pMD->m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_strUserID) != 0) return;

	m_pMD->m_UB.m_uid = m_uid;
	strcpy(m_pMD->m_UB.m_strUserID, m_strUserID);
	//strncpy(m_pMD->m_UB.m_UserBankItem, strItem, _BANK_DB);
	memcpy(m_pMD->m_UB.m_UserBankItem, strItem, _BANK_DB);
	m_pMD->m_UB.m_dwBankDN = m_dwBankDN;
}

void USER::SetMemAccountBank(LPCTSTR strItem)
{
	if(m_pSharedMemory->m_hMapping == NULL) return;
	if(m_pMD == NULL) return;
	if(m_pMD->m_uid == -1) return;

	if(m_pMD->m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_strUserID) != 0) return;

	m_pMD->m_AB.m_uid = m_uid;
	strcpy(m_pMD->m_AB.m_strAccount, m_strAccount);
	memcpy(m_pMD->m_AB.m_AccountBankItem, strItem, _ACCOUNT_BANK_DB);
	m_pMD->m_AB.m_dwBankDN = m_dwAccountBankDN;
}

////////////////////////////////////////////////////////////////////////////////////////
//	皋葛府叼厚俊 蜡历篮青沥焊啊 乐栏搁 啊廉柯促.
//
BOOL USER::LoadMemUserBank()
{
	if(m_pSharedMemory->m_hMapping == NULL) return FALSE;
	if(m_pMD == NULL) return FALSE;
	if(m_pMD->m_uid == -1) return FALSE;
	if(m_pMD->m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_strUserID) != 0) return FALSE;
	if(m_pMD->m_UB.m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_UB.m_strUserID) != 0) return FALSE;

	StrToUserBankItem((LPTSTR)m_pMD->m_UB.m_UserBankItem);
	m_dwBankDN = m_pMD->m_UB.m_dwBankDN;

	return TRUE;
}

BOOL USER::LoadMemAccountBank()
{
	if(m_pSharedMemory->m_hMapping == NULL) return FALSE;
	if(m_pMD == NULL) return FALSE;
	if(m_pMD->m_uid == -1) return FALSE;
	if(m_pMD->m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_strUserID) != 0) return FALSE;

	if(m_pMD->m_AB.m_uid != m_uid || _stricmp(m_strAccount, m_pMD->m_AB.m_strAccount) != 0) return FALSE;

	StrToAccountBankItem((LPTSTR)m_pMD->m_AB.m_AccountBankItem);
	m_dwAccountBankDN = m_pMD->m_AB.m_dwBankDN;

	return TRUE;
}

BOOL USER::UpdateMemBankDataOnly()
{
	if(m_pSharedMemory->m_hMapping == NULL) return FALSE;
	if(m_pMD == NULL) return FALSE;
	if(m_pMD->m_uid == -1) return FALSE;
	if(m_pMD->m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_strUserID) != 0) return FALSE;
	if(m_pMD->m_UB.m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_UB.m_strUserID) != 0) return FALSE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strBankItem[_BANK_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	
	::CopyMemory(strBankItem, m_pMD->m_UB.m_UserBankItem, _BANK_DB);
	
	SDWORD sBankItemLen	= sizeof(strBankItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_BANK_DATA_ONLY (\'%s\',%d, ?)}"), m_strUserID, m_pMD->m_UB.m_dwBankDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Mem User Bank Data Only!!\n");

		//g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strBankItem), 0, (TCHAR*)strBankItem, 0, &sBankItemLen);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	m_pMD->m_UB.m_uid = -1;

	return TRUE;
}

BOOL USER::UpdateMemAccountBankDataOnly()
{
	if(m_pSharedMemory->m_hMapping == NULL) return FALSE;
	if(m_pMD == NULL) return FALSE;
	if(m_pMD->m_uid == -1) return FALSE;
	if(m_pMD->m_uid != m_uid || _stricmp(m_strUserID, m_pMD->m_strUserID) != 0) return FALSE;

	if(m_pMD->m_AB.m_uid != m_uid || _stricmp(m_strAccount, m_pMD->m_AB.m_strAccount) != 0) return FALSE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strBankItem[_ACCOUNT_BANK_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strBankItem, sizeof(strBankItem));
	
	::CopyMemory(strBankItem, m_pMD->m_AB.m_AccountBankItem, _ACCOUNT_BANK_DB);
	
	SDWORD sBankItemLen	= sizeof(strBankItem);
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_ACCOUNT_BANK_DATA_ONLY (\'%s\',%d, ?)}")/*, m_pMD->m_iMyServer*/, m_pMD->m_strAccount, m_pMD->m_AB.m_dwBankDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Mem User Bank Data Only!!\n");

		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strBankItem), 0, (TCHAR*)strBankItem, 0, &sBankItemLen);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	m_pMD->m_AB.m_uid = -1;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
//	辨靛盔尝府 盲泼
//
void USER::GuildChat(TCHAR *pBuf)
{
	int i;

	USER *pUser = NULL;

	CBufferEx TempBuf;

	if(m_dwGuild <= 0) return;

	// alisia
	g_pMainDlg->BridgeServerUserGuildChatReq( m_uid, (int)m_dwGuild, m_strUserID, pBuf+1 );
	return;
	//

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(GUILD_CHAT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	TempBuf.AddString(pBuf + 1);

	for(i = 0; i< MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if(m_dwGuild == pUser->m_dwGuild) pUser->Send(TempBuf, TempBuf.GetLength());
	}
}


void USER::SendStoreTax(int nStore)
{
	CBufferEx TempBuf;

	int nLen = 0;
	short sTax = 0;

	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;

	if(!m_bGuildMaster) 
	{
		SendSystemMsg( IDS_USER_ONLY_USE_GUILD_MASTER, SYSTEM_ERROR, TO_ME);
		return;
	}

	if(nStore >= FORTRESS_BAND)						// 夸货俊 加茄 惑痢捞搁...
	{
		pFort = GetFortress(nStore);
		if(pFort == NULL) return;

		if(pFort->m_iGuildSid != m_dwGuild) return;

		SYSTEMTIME st;
		SYSTEMTIME guildTime;

		GetLocalTime(&guildTime);
		st = pFort->m_wLastWarTime;
		COleDateTime LastTime(st.wYear, st.wMonth, st.wDay, st.wHour, 0, 0);
		COleDateTime CurrTime = COleDateTime(guildTime);

		if(CurrTime.m_status == COleDateTime::valid && LastTime.m_status == COleDateTime::valid)
		{
/*			CTime curr(CurrTime.GetYear(), CurrTime.GetMonth(), CurrTime.GetDay(), CurrTime.GetHour(), 0, 0);
			CTime last(LastTime.GetYear(), LastTime.GetMonth(), LastTime.GetDay(), LastTime.GetHour(), 0, 0);
			CTimeSpan DiffTime = curr - last;	
			
			int nTime = DiffTime.GetHours();
		
			if(nTime < UPDATE_GUILD_INVEN_TIME)
			{
				SendSystemMsg( IDS_USER_TEXT_CONTROL, SYSTEM_ERROR, TO_ME);
				return;
			}
*/
			sTax = pFort->m_tTaxRate;
		}
	}
	else
	{
		pStore = GetStore(nStore);
		if(pStore == NULL) return;

		sTax = pStore->m_sRate;
	}

	TempBuf.Add(GUILD_TAX_OPEN);
	TempBuf.Add((short)nStore);
	TempBuf.Add((short)sTax);
	Send(TempBuf, TempBuf.GetLength());
}

void USER::SendGuildWarScheme(int nStore)
{
	int nChatNum = -1;

	CBufferEx TempBuf;

	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;	

	if(nStore >= FORTRESS_BAND)						// 夸货俊 加茄 惑痢捞搁...
	{
		pFort = GetFortress(nStore);
		if(pFort == NULL) return;
		
		if(pFort->m_tWarType == GUILD_WAR_DECISION)
		{
			if(pFort->m_sFortressID == 1000) nChatNum = 144;
			else if(pFort->m_sFortressID == 1001) nChatNum = 164;
			else if(pFort->m_sFortressID == 1002) nChatNum = 317;

			EditHyperFortressText(pFort, nChatNum);
			return;
		}		
	}
	else
	{
		pStore = GetStore(nStore);
		if(pStore == NULL) return;

		if(pStore->m_tWarType == GUILD_WAR_DECISION) 
		{
			if(pStore->m_sStoreID == 4) nChatNum = 34;		// 捞固 老沥捞 棱囚 乐栏搁 client俊 焊郴瘤臼绰促.
			else if(pStore->m_sStoreID == 5) nChatNum = 44;
			else if(pStore->m_sStoreID == 6) nChatNum = 54;

			EditHyperText(pStore, nChatNum);
			return;
		}
	}

	if(!m_bGuildMaster)
	{
		SendSystemMsg( IDS_USER_ONLY_USE_GUILD_MASTER, SYSTEM_ERROR, TO_ME);
		return;
	}

	SYSTEMTIME temp;
	GetLocalTime(&temp);

	TempBuf.Add(GUILD_WAR_OPEN);
	TempBuf.Add((short)nStore);

	TempBuf.Add((short)temp.wYear);
	TempBuf.Add((BYTE)temp.wMonth);
	TempBuf.Add((BYTE)temp.wDay);

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////////
//	秦寸 辨付啊 惑痢狼 技陛阑 炼沥茄促.
//
void USER::GetStoreTax(TCHAR *pBuf)
{
	int index = 0;
	CStore* pStore = NULL;

	short nStore = GetShort(pBuf, index);
	short nTax = GetShort(pBuf, index);

	if(nTax < 0 || nTax > GUILD_MAX_TAX) return;		// 弥措 200 % 鳖瘤

	SetGuildStoreTex(nStore, nTax);
}

////////////////////////////////////////////////////////////////////////////////////////
//	调整税率
void USER::SetGuildStoreTex(int iSid, int iRate)
{
	int nChatNum = -1;

	CBufferEx TempBuf;

	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;

	if(m_dwGuild <= 0 || !m_bGuildMaster)
	{
		SendSystemMsg( IDS_USER_ONLY_USE_GUILD_MASTER, SYSTEM_ERROR, TO_ME);//"只有军团长才能使用."
		return;
	}

	if(iSid >= FORTRESS_BAND)						// 夸货俊 加茄 惑痢捞搁...
	{
		pFort = GetFortress(iSid);
		if(pFort == NULL) return;

		if(pFort->m_lUsed == 1)
		{
			SendSystemMsg( IDS_USER_CANT_USE_IN_DEFENCE, SYSTEM_ERROR, TO_ME);//"保卫战进行中无法使用."
			return;
		}
        if(pFort->m_iGuildSid == m_dwGuild)
		{
			if(iRate > GUILD_MAX_TAX) 
			{
				SendSystemMsg( IDS_USER_TEX_MAX, SYSTEM_ERROR, TO_ME);
				return;
			}
			pFort->m_tTaxRate = iRate;
			pFort->ApplyTaxToStore();				// 阿 惑痢俊 技陛阑 悸泼.
			UpdateMemStoreTax(iSid, iRate);

			UpdateTaxToDB(pFort,iRate);
			CString str;
			str.Format("%s 要塞团长将税率调整为%d%%",m_strUserID,iRate);//发送税率调整消息
			SendSystemMsg(str.GetBuffer(0), SYSTEM_ANNOUNCE, TO_ALL);
			return;
		}
	}
/*	else
	{
		pStore = GetStore(iSid);
		if(pStore == NULL) return;

		if(pStore->m_sStoreID == (short)iSid)
		{
			pStore->m_sRate = iRate;

			UpdateMemStoreTax(iSid, iRate);
			return;
		}
	}
*/
}

////////////尾巴

void USER::GetGuildWarScheme(TCHAR *pBuf)
{
	int index = 0;

	short nStore = GetShort(pBuf, index);

	if(nStore < 0) return;

	if(nStore >= FORTRESS_BAND)
	{
		SetFortressGuildWarScheme(pBuf + index, nStore);
	}
/*	else 
	{
		SetStoreGuildWarScheme(pBuf + index, nStore);
	}
*/
}

////////////////////////////////////////////////////////////////////////////////////////
//	鞘靛 惑痢狼 傈里 朝楼甫 搬沥茄促.
//
void USER::SetStoreGuildWarScheme(TCHAR *pBuf, int nStore)
{
	int index = 0;
	int nCount = 0;
	CStore* pStore = NULL;

	int iMon[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	short sYear = GetShort(pBuf, index);
	BYTE tMon = GetByte(pBuf, index);
	BYTE tDay = GetByte(pBuf, index);
	BYTE tTime = GetByte(pBuf, index);

	if(sYear >= 2500) 
	{
		SendSystemMsg( IDS_USER_INVALID_YEAR, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(tMon >= 13)
	{
		SendSystemMsg( IDS_USER_INVALID_MONTH, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(tDay >= 32)
	{
		SendSystemMsg( IDS_USER_INVALID_DAY, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(tTime < 20 && tTime > 23)
	{
		SendSystemMsg( IDS_USER_INVALID_HOUR, SYSTEM_ERROR, TO_ME);
		return;
	}

	pStore = GetStore(nStore);
	if(pStore == NULL) return;

	if(pStore->m_tWarType == GUILD_WAR_DECISION)
	{
		SendSystemMsg( IDS_USER_ALREADY_EXIST_SCHEDULE, SYSTEM_ERROR, TO_ME);
		return;// 捞固 辨靛傈 老沥捞 棱囚 乐栏搁 给官槽促.
	}

	int totalDay = DayCalculation((int)sYear, (int)tMon, (int)tDay); 
	int defDay = DayCalculation(pStore->m_wLastWarTime.wYear, pStore->m_wLastWarTime.wMonth, pStore->m_wLastWarTime.wDay); 

	nCount = abs(totalDay - defDay);
	if(nCount > 3)
	{
		SendSystemMsg( IDS_USER_GUILD_WAR_DELAY, SYSTEM_ERROR, TO_ME);
		return;
	}

	SYSTEMTIME temp;
	GetLocalTime(&temp);

	if(tMon == temp.wMonth && tDay <= temp.wDay)
	{
		SendSystemMsg( IDS_USER_PASS_MIN_DAY, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(tDay == temp.wDay && tTime < temp.wHour) 
	{
		SendSystemMsg( IDS_USER_INVALID_TIME, SYSTEM_ERROR, TO_ME);
		return;
	}

	CTime CurrTime = CTime::GetCurrentTime();

	CTime LastTime(pStore->m_wLastWarTime);
	CTimeSpan DiffTime = CurrTime - LastTime;
	if(DiffTime.GetTotalHours() >= 12)
	{
		SendSystemMsg( IDS_USER_SCHEDULE_FIX, SYSTEM_ERROR, TO_ME);
		return;
	}
	
	CString strMsg;
	pStore->m_wPlanWarTime.wYear = sYear;
	pStore->m_wPlanWarTime.wMonth = tMon;
	pStore->m_wPlanWarTime.wDay = tDay;
	pStore->m_wPlanWarTime.wHour = tTime;

	if(!UpdateGuildStoreWarTime(pStore)) 
	{
		SendSystemMsg( IDS_USER_EDIT_SCHEDULE_FAIL, SYSTEM_ERROR, TO_ME);
		return;
	}

	pStore->m_tWarType = GUILD_WAR_DECISION;

	strMsg.Format( IDS_USER_SCHEDULE_FIXED, sYear, tMon, tDay, tTime); 
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

	UpdateMemStoreWarType(pStore);
}

////////////////////////////////////////////////////////////////////////////////////////
//	傍己傈狼 傈里 朝楼甫 搬沥茄促.
//
void USER::SetFortressGuildWarScheme(TCHAR *pBuf, int nStore)
{
	int index = 0;
	int nCount = 0;
	CGuildFortress* pFort = NULL;

	int iMon[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	short sYear = GetShort(pBuf, index);
	BYTE tMon = GetByte(pBuf, index);
	BYTE tDay = GetByte(pBuf, index);
	BYTE tTime = GetByte(pBuf, index);

	if(sYear >= 2500) 
	{
		SendSystemMsg( IDS_USER_INVALID_YEAR, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(tMon >= 13)
	{
		SendSystemMsg( IDS_USER_INVALID_MONTH, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(tDay >= 32)
	{
		SendSystemMsg( IDS_USER_INVALID_DAY, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(tTime != 21)
	{
		SendSystemMsg( IDS_USER_INVALID_HOUR, SYSTEM_ERROR, TO_ME);
		return;
	}

	pFort = GetFortress(nStore);
	if(pFort == NULL) return;

	if(pFort->m_lUsed == 1)
	{
		SendSystemMsg( IDS_USER_CANT_USE_IN_DEFENCE, SYSTEM_ERROR, TO_ME);
		return;
	}

	if(pFort->m_tWarType == GUILD_WAR_DECISION)
	{
		SendSystemMsg( IDS_USER_ALREADY_EXIST_SCHEDULE, SYSTEM_ERROR, TO_ME);
		return;// 捞固 辨靛傈 老沥捞 棱囚 乐栏搁 给官槽促.
	}

	int totalDay = DayCalculation((int)sYear, (int)tMon, (int)tDay); 
	int defDay = DayCalculation(pFort->m_wLastWarTime.wYear, pFort->m_wLastWarTime.wMonth, pFort->m_wLastWarTime.wDay); 

	nCount = abs(totalDay - defDay);
	if(nCount > 7)
	{
		SendSystemMsg( IDS_USER_GUILD_WAR_DELAY1, SYSTEM_ERROR, TO_ME);
		return;
	}

/*	nCount = abs(totalDay - defDay);
	if(nCount > 2)
	{
		SendSystemMsg( IDS_USER_GUILD_WAR_DELAY2, SYSTEM_ERROR, TO_ME);
		return;
	}
*/
	SYSTEMTIME temp;
	GetLocalTime(&temp);

	if(tMon == temp.wMonth && tDay <= temp.wDay)
	{
		SendSystemMsg( IDS_USER_PASS_MIN_DAY, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(tDay == temp.wDay && tTime < temp.wHour) 
	{
		SendSystemMsg( IDS_USER_INVALID_TIME, SYSTEM_ERROR, TO_ME);
		return;
	}

	CTime CurrTime = CTime::GetCurrentTime();

	CTime LastTime(pFort->m_wLastWarTime);
	CTimeSpan DiffTime = CurrTime - LastTime;
	if(DiffTime.GetTotalHours() >= 12)
	{
		SendSystemMsg( IDS_USER_SCHEDULE_FIX, SYSTEM_ERROR, TO_ME);
		return;
	}
	
	CString strMsg;
	pFort->m_wPlanWarTime.wYear = sYear;
	pFort->m_wPlanWarTime.wMonth = tMon;
	pFort->m_wPlanWarTime.wDay = tDay;
	pFort->m_wPlanWarTime.wHour = tTime;

	if(!UpdateGuildFortressWarTime(pFort)) 
	{
		SendSystemMsg( IDS_USER_EDIT_SCHEDULE_FAIL, SYSTEM_ERROR, TO_ME);
		return;
	}

	pFort->m_tWarType = GUILD_WAR_DECISION;

	strMsg.Format( IDS_USER_FORTRESS_SCHEDULE_FIXED, sYear, tMon, tDay, tTime); 
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

	UpdateMemFortressWarType(pFort);
}

int USER::DayCalculation(int iYear, int iMon, int iDay)
{
	int iTotalDays = 0;
	int ilastyear = iYear - 1;
	int iMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30 , 31, 30, 31};
	if((iYear % 4 == 0 && iYear % 100 != 0) || iYear % 400 == 0){
		iMonth[1] = 29;
	}
	
	iTotalDays = ilastyear * 365 + ilastyear / 4 + ilastyear / 400 - ilastyear / 100;
	for(int i=0;i<iMon - 1;i++)
	{
		iTotalDays += iMonth[i];	
	}

	iTotalDays += iDay;
	return iTotalDays;
}

////////////////////////////////////////////////////////////////////////////////////////
//	辨靛傈捞 场车绊 葛电 惑炔捞 辆丰登菌促绊 舅赴促.
//
BOOL USER::StoppingTheGuildWar(CStore *pStore)
{
	if(m_dwGuild <= 0) return FALSE;

	if(pStore == NULL) return FALSE;

	int nLen = CHAR_NAME_LENGTH;

//	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) { ReleaseGuild(); return FALSE; }
	
	if(UpdateGuildStore(pStore->m_sStoreID, pGuild->m_strMasterName) == FALSE) { ReleaseGuild(); return FALSE; }
										// 惑痢阑 檬扁拳茄促.(匡 辨靛肺...^^)
	::ZeroMemory(pStore->m_strGuildName, CHAR_NAME_LENGTH + 1);
	::ZeroMemory(pStore->m_strMasterName, CHAR_NAME_LENGTH + 1);
	strncpy(pStore->m_strGuildName, m_strGuildName, nLen);
	strncpy(pStore->m_strMasterName, pGuild->m_strMasterName, nLen);

	ReleaseGuild();						// 秦力...

	SendGuildMsgForAnnounceGuildWarEnd(pStore);
	pStore->InitStoreInfo(m_dwGuild);

	SetGuildStoreTex(pStore->m_sStoreID, 0);	// Tax Rate Init

	InitMemStore(pStore);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
//	DB俊 悸泼茄促.
//
BOOL USER::UpdateGuildStore(int nSid, TCHAR *strGuildMasterName)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call INSERT_GUILD_STORE(%d,%d,\'%s\',\'%s\')}"), nSid, m_dwGuild, m_strGuildName, strGuildMasterName);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild_Store Data Only!!\n");

		//g_DBNew[m_iModSid].ReleaseDB(db_index);
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
			BREAKPOINT();

			g_DBNew[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

void USER::BeginGuildWar()
{
	m_tGuildWar = GUILD_WARRING;
//	SendSystemMsg( IDS_USER_START_GUILD_WAR, SYSTEM_NORMAL, TO_ME);
}

void USER::EndGuildWar(BYTE tWarType)
{
	CBufferEx TempBuf;

	m_tGuildWar = GUILD_WAR_AFFTER;

	m_FieldWarGMUid = 0;
	m_dwFieldWar = 0;

	TempBuf.Add(GUILD_WAR);
	TempBuf.Add((BYTE)0x02);							// 辨靛傈 辆丰...
	TempBuf.Add(tWarType);

	if(tWarType == GUILD_FIELD_WARRING) TempBuf.Add((short)0x00); // 鞘靛傈老锭绰 0栏肺 

	Send(TempBuf, TempBuf.GetLength());
//	SendSystemMsg( IDS_USER_END_GUILD_WAR, SYSTEM_NORMAL, TO_ME);
}

void USER::BeginFortressWar()
{
	m_tFortressWar = GUILD_WARRING;
}
/*
void USER::EndFortressWar()
{
	CBufferEx TempBuf;

	m_tFortressWar = GUILD_WAR_AFFTER;

	TempBuf.Add(GUILD_WAR);
	TempBuf.Add((BYTE)0x02);							// 辨靛傈 辆丰...
	TempBuf.Add((BYTE)GUILD_FOTRESS_WARRING);

	Send(TempBuf, TempBuf.GetLength());
}
*/

//////////////////////////////////////////////////////////////////////////////////////////
//	辨靛傈捞 场车澜阑 舅赴促.
//
void USER::SendGuildMsgForAnnounceGuildWarEnd(CStore *pStore)
{
	int i, j;
	CString strMsg;
	CBufferEx TempBuf;

	USER* pUser = NULL;

	if(!pStore) return;

	strMsg.Format( IDS_USER_THIS_AREA_GUILD_HAVE, m_strGuildName);

	for(i = 0; i < MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		
		if(pStore->m_iGuildSid == pUser->m_dwGuild) pUser->EndGuildWar(GUILD_STORE_WARRING);
		else
		{
			for(j =0; j < GUILD_ATTACK_MAX_NUM; j++)
			{
				if(pStore->m_arAttackGuild[j] == pUser->m_dwGuild)
				{
					pUser->EndGuildWar(GUILD_STORE_WARRING);			// 辨靛 傈捞 场巢
//					pUser->SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
					break;
				}
			}
		}

		pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	傍己傈捞 场车澜阑 舅赴促.
//
/*
void USER::SendGuildMsgForAnnounceFortressWarEnd(CGuildFortress *pFort)
{
	int i, j;
	CString strMsg;
	CBufferEx TempBuf;

	USER* pUser = NULL;

	if(!pFort) return;

	strMsg.Format( IDS_USER_THIS_AREA_GUILD_HAVE, m_strGuildName);

	for(i = 0; i < MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		
		if(pFort->m_iGuildSid == pUser->m_dwGuild) pUser->EndFortressWar();
		else
		{
			for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
			{
				if(pFort->m_arAttackGuild[j].lGuild == pUser->m_dwGuild)
				{
					pUser->EndFortressWar();			// 辨靛 傈捞 场巢
					break;
				}
			}
		}

		pUser->SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
	}
}
*/

//////////////////////////////////////////////////////////////////////////////////////////
//	林刮殿废锅龋啊 老摹窍绰瘤 八荤
//
BOOL USER::CheckJuminNumber(LPCTSTR szAccount, LPCTSTR szJumin)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	CString			szTemp = szJumin;
	szTemp.Insert(6, '-');

	BOOL bQuerySuccess = TRUE;
	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call GET_JUMIN_NO (\'%s\')}"), szAccount); 
	
	SQLCHAR		strJuminSQL[20];
	SQLINTEGER	strInd = SQL_NTS;

	::ZeroMemory(strJuminSQL, sizeof(strJuminSQL));

	int db_index = 0;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt);
	if( retcode != SQL_SUCCESS )
	{
		//g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			SQLGetData(hstmt, 1, SQL_C_CHAR, strJuminSQL, sizeof(strJuminSQL), &strInd);
		}
	}
	else
	{
		//DisplayErrorMsg(hstmt);
		bQuerySuccess = FALSE;

		if (hstmt!=NULL) SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		//g_DBSession[m_iModSid].ReleaseDB(db_index);
		TRACE("*** 林刮殿废 锅龋 DB[%d/%d] 坷幅... ***\n", m_iModSid, db_index );
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBSession[m_iModSid].ReleaseDB(db_index);
	
	if(bQuerySuccess && szTemp.Compare((LPTSTR)strJuminSQL) == 0) return TRUE;
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	惑痢俊辑 魄 拱扒俊 措茄 技陛阑 辨靛 芒绊(皋葛府 DB)俊 利赋茄促.
//
void USER::UpdateMemStoreDN(int iStoreID, DWORD dwDN)
{
	/*
	int nSize = g_arStoreSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemStore* pData = NULL;

	for(int i = 0; i < nSize; i++)
	{
		pShared = g_arStoreSharedMemory[i];

		if(pShared == NULL) continue;
		if(pShared->m_hMapping == NULL) continue;

		pData = (CMemStore*) pShared->m_lpData;

		if(pData == NULL) continue;
		if(pData->m_sStoreID == -1) continue;
		if(pData->m_iGuildSid <= 0) continue;

		if(iStoreID == pData->m_sStoreID)
		{
			pData->m_dwStoreDN = dwDN;
			return;
		}
	}
	*/
	int nSize, i;
	int iID = 0;

	if(iStoreID >= FORTRESS_BAND)
	{
		nSize = g_arFortressSharedMemory.GetSize();
		CSharedMemory* pShared = NULL;
		CMemFortress* pData = NULL;

		for(i = 0; i < nSize; i++)
		{
			pShared = g_arFortressSharedMemory[i];
			if(pShared == NULL) return;
			if(pShared->m_hMapping == NULL) return;

			pData = (CMemFortress*) pShared->m_lpData;
			
			if(pData == NULL) return;
			if(pData->m_sFortressID < 1000) return;
			if(pData->m_iGuildSid <= 0) return;

			iID = pData->m_sFortressID;
			if(iID == iStoreID)
			{
				pData->m_dwStoreDN = dwDN;
				break;
			}
		}
	}
/*	else
	{
		nSize = g_arStoreSharedMemory.GetSize();
		CSharedMemory* pShared = NULL;
		CMemStore* pData = NULL;

		if(iStoreID >= nSize) return;
		if((int)dwDN < 0) return;

		pShared = g_arStoreSharedMemory[iStoreID];
		
		if(pShared == NULL) return;
		if(pShared->m_hMapping == NULL) return;
		
		pData = (CMemStore*) pShared->m_lpData;
		
		if(pData == NULL) return;
		if(pData->m_sStoreID == -1) return;
		if(pData->m_iGuildSid <= 0) return;
		
		pData->m_dwStoreDN = dwDN;
	}
*/
}

/////////////////////////////////////////////////////////////////////////////////////////
//	鞘靛 惑痢俊 措茄 皋葛府 DB狼 郴侩阑 檬扁拳 茄促.
//
void USER::InitMemStore(CStore *pStore)
{
/*	if(pStore == NULL) return;

	int nSize = g_arStoreSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemStore* pData = NULL;

	if(pStore->m_sStoreID >= nSize) return;

	pShared = g_arStoreSharedMemory[pStore->m_sStoreID];
	
	if(pShared == NULL) return;
	if(pShared->m_hMapping == NULL) return;
	
	pData = (CMemStore*) pShared->m_lpData;
	
	if(pData == NULL) return;
	
	pData->m_sStoreID = pStore->m_sStoreID;
	pData->m_iGuildSid = pStore->m_iGuildSid;
	pData->m_sTaxRate = 0;
	pData->m_dwStoreDN = 0;
	pData->m_tWarType = GUILD_WAR_PREPARE;

	pStore->GuildListToStr(pData->m_AttackList);
*/
}

/////////////////////////////////////////////////////////////////////////////
//	鞘靛惑痢俊 措茄 技陛阑 皋葛府 DB俊 悸泼茄促.
//
void USER::UpdateMemStoreTax(int iSid, int iRate)
{
	int nSize, i;

	if(iSid >= FORTRESS_BAND)
	{
		nSize = g_arFortressSharedMemory.GetSize();
		CSharedMemory* pShared = NULL;
		CMemFortress* pData = NULL;

		if(iRate < 0 || iRate > GUILD_MAX_TAX) return;		// 弥措 200 % 鳖瘤
		for(i = 0; i < nSize; i++)
		{
			pData = NULL;
			pShared = NULL;
			pShared = g_arFortressSharedMemory[i];
			
			if(pShared == NULL) continue;
			if(pShared->m_hMapping == NULL) continue;
			
			pData = (CMemFortress*) pShared->m_lpData;
			
			if(pData == NULL) continue;
			if(pData->m_sFortressID != iSid) continue;;
			if(pData->m_iGuildSid != m_dwGuild) continue;
			
			pData->m_sTaxRate = iRate;
			break;
		}
	}
/*	else
	{
		nSize = g_arStoreSharedMemory.GetSize();
		CSharedMemory* pShared = NULL;
		CMemStore* pData = NULL;

		if(iSid >= nSize) return;
		if(iRate < 0 || iRate > GUILD_MAX_TAX) return;		// 弥措 200 % 鳖瘤

		pShared = g_arStoreSharedMemory[iSid];
		
		if(pShared == NULL) return;
		if(pShared->m_hMapping == NULL) return;
		
		pData = (CMemStore*) pShared->m_lpData;
		
		if(pData == NULL) return;
		if(pData->m_sStoreID == -1) return;
		if(pData->m_iGuildSid <= 0) return;
		
		pData->m_sTaxRate = iRate;
	}
*/
}

////////////////////////////////////////////////////////////////////////////
//	鞘靛惑痢俊 措茄 WarType 阑 皋葛府 DB俊 悸泼茄促.
//
void USER::UpdateMemStoreWarType(CStore *pStore)
{
/*	if(pStore == NULL) return;

	int nSize = g_arStoreSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemStore* pData = NULL;

	if(pStore->m_sStoreID >= nSize) return;

	pShared = g_arStoreSharedMemory[pStore->m_sStoreID];
	
	if(pShared == NULL) return;
	if(pShared->m_hMapping == NULL) return;
	
	pData = (CMemStore*) pShared->m_lpData;
	
	if(pData == NULL) return;
	
	pData->m_tWarType = pStore->m_tWarType;
*/
}

////////////////////////////////////////////////////////////////////////////
//	傍己傈俊 措茄 WarType 阑 皋葛府 DB俊 悸泼茄促.
//
void USER::UpdateMemFortressWarType(CGuildFortress *pFort)
{
	if(pFort == NULL) return;

	int nSize = g_arFortressSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemFortress* pData = NULL;

	for(int i = 0; i < nSize; i++)
	{
		pShared = g_arFortressSharedMemory[i];
		
		if(pShared == NULL) return;
		if(pShared->m_hMapping == NULL) return;
		
		pData = (CMemFortress*) pShared->m_lpData;
		
		if(pData == NULL) return;
		
		if(pData->m_iGuildSid == pFort->m_iGuildSid)
		{
			pData->m_tWarType = pFort->m_tWarType;
			break;
		}
	}
}

void USER::UpdateMemStoreGuildList(CStore *pStore)
{
/*	if(pStore == NULL) return;

	int nSize = g_arStoreSharedMemory.GetSize();
	CSharedMemory* pShared = NULL;
	CMemStore* pData = NULL;

	if(pStore->m_sStoreID >= nSize) return;

	pShared = g_arStoreSharedMemory[pStore->m_sStoreID];
	
	if(pShared == NULL) return;
	if(pShared->m_hMapping == NULL) return;
	
	pData = (CMemStore*) pShared->m_lpData;
	
	if(pData == NULL) return;
	
	pStore->GuildListToStr(pData->m_AttackList);
*/
}

/////////////////////////////////////////////////////////////////////////////
//	泅犁 蜡历啊 辑乐绰 谅钎啊 甘俊辑 佬阑荐 乐绰镑牢瘤,  甘惑俊 佬篮加己蔼阑 曼炼窍扁困茄 牢郸胶甫 逞变促. 
//
int USER::CheckInvalidMapType()
{
	int nRet = -1;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return nRet;
	if(m_curx >= g_zone[m_ZoneIndex]->m_sizeMap.cx || m_curx < 0) return nRet;
	if(m_cury >= g_zone[m_ZoneIndex]->m_sizeMap.cy || m_cury < 0) return nRet;

	nRet = ((g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_dwType & 0xFF00 ) >> 8);

	return nRet;
}

/////////////////////////////////////////////////////////////////////////////
//	泅犁 蜡历啊 辑乐绰 谅钎啊 辨靛傈 吝牢 瘤开捞搁 FALSE蔼阑 逞变促.
//
BOOL USER::CheckInvalidZoneState(int type)
{
	int index = 0;

//	if(type < 2 || type >= 17) return TRUE;
//	index = g_arMapTable[type]->m_sStoreID;
	int mapindex = GetGuildMapIndex(type);
	if(mapindex <= -1 || mapindex >= g_arMapTable.GetSize()) return TRUE;

	index = g_arMapTable[mapindex]->m_sStoreID;

	if(index >= FORTRESS_BAND) 
	{
		CGuildFortress* pFort = NULL;
		pFort = GetFortress(index);

		if(pFort) 
		{
			if(pFort->m_lUsed == 1)	return FALSE;
//			if(g_arMapTable[type]->m_sStoreZone) return FALSE;
		}
	}	
/*	else
	{
		CStore *pStore = NULL;

		index = g_arMapTable[mapindex]->m_sStoreIndex;

		pStore = GetStore(index);

		if(pStore)
		{
			if(pStore->m_lUsed == 1) return FALSE;
		}
//		if(index < 0 || index >= g_arStore.GetSize()) return TRUE;
//		if(g_arStore[index]->m_lUsed == 1)	return FALSE;  // 秦寸 加己 甘捞 辨靛傈 惑怕	
	}
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	粮眉牢瘤甫 夸没且锭 秦寸 瘤开捞 辨傈吝捞搁 芭何茄促.
//
BOOL USER::CheckInvakidZoneChangeState(int type)
{
	int index = 0;

//	if(type < 2 || type >= 17) return TRUE;
	int mapindex = GetGuildMapIndex(type);
	if(mapindex <= -1 || mapindex >= g_arMapTable.GetSize()) return TRUE;

	index = g_arMapTable[mapindex]->m_sStoreID;

	if(index >= FORTRESS_BAND) 
	{
		CGuildFortress* pFort = NULL;
		pFort = GetFortress(index);

		if(pFort) 
		{
			if(m_dwGuild > 0 && pFort->m_iGuildSid == m_dwGuild) return TRUE;
			if(pFort->m_iZone == m_curz || g_arMapTable[mapindex]->m_sStoreZone) return FALSE;
			if(pFort->m_lUsed == 1)	return FALSE;
		}
	}
/*	else
	{
		CStore *pStore = NULL;

		index = g_arMapTable[mapindex]->m_sStoreIndex;

		pStore = GetStore(index);

		if(pStore)
		{
			if(pStore->m_lUsed == 1) return FALSE;
		}
	}
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	努扼捞攫飘俊辑 夸没且锭 辑滚 平阑 焊郴辑 胶.琴阑 规瘤茄促.
//
void USER::SendServerTick()
{
/*	DWORD dwTick;

	CBufferEx TempBuf;

	dwTick = GetTickCount();

	TempBuf.Add(SERVER_TICK_RESULT);
	TempBuf.Add(dwTick);
	Send(TempBuf, TempBuf.GetLength());
*/
}

/////////////////////////////////////////////////////////////////////////////
//	辨靛傈 矫埃阑 悸泼茄促.
//
BOOL USER::UpdateGuildStoreWarTime(CStore *pStore)
{
	CString			strTime;
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strWarTime[30];

	if(pStore->m_iGuildSid <= 0) return FALSE;

	strTime = _T("");
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strWarTime, sizeof(strWarTime));
	
	strTime.Format("%d-%d-%d %d:00:00", pStore->m_wPlanWarTime.wYear, pStore->m_wPlanWarTime.wMonth, pStore->m_wPlanWarTime.wDay, pStore->m_wPlanWarTime.wHour);
	::CopyMemory(strWarTime, strTime.GetBuffer(strTime.GetLength()), strTime.GetLength());
	strTime.ReleaseBuffer();
	
	SDWORD sLen	= sizeof(strWarTime);

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_STORE_WARTIME(%d,%d,\'%s\')}"), pStore->m_sStoreID, pStore->m_iGuildSid, strWarTime);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild_Store Data Only!!\n");

		//g_DBNew[m_iModSid].ReleaseDB(db_index);
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
			BREAKPOINT();

			g_DBNew[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	辨靛傈 矫埃阑 悸泼茄促.
//
BOOL USER::UpdateGuildFortressWarTime(CGuildFortress *pFort)
{
	CString			strTime;
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strWarTime[30];

	if(pFort->m_iGuildSid <= 0) return FALSE;

	strTime = _T("");
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strWarTime, sizeof(strWarTime));
	
	strTime.Format("%d-%d-%d 21:00:00", pFort->m_wPlanWarTime.wYear, pFort->m_wPlanWarTime.wMonth, pFort->m_wPlanWarTime.wDay);
	::CopyMemory(strWarTime, strTime.GetBuffer(strTime.GetLength()), strTime.GetLength());
	strTime.ReleaseBuffer();
	
	SDWORD sLen	= sizeof(strWarTime);

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_FORTRESS_WARTIME(%d,%d,\'%s\')}"), pFort->m_sFortressID, pFort->m_iGuildSid, strWarTime);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild_Store Data Only!!\n");

		//g_DBNew[m_iModSid].ReleaseDB(db_index);
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
			BREAKPOINT();

			g_DBNew[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	鞘靛 辨靛傈阑 盲泼栏肺 脚没 ---> 惑措规 辨付俊霸 焊辰促.
//
void USER::SendGuildWarFieldApply(TCHAR *pBuf)
{
	int index = 0;
	int nLength = 0;

	BYTE error_code = 0;

	CGuild *pGuild = NULL;
	USER *pUser = NULL;

	CBufferEx TempBuf;

	TCHAR szGuildName[255];

	if(m_dwGuild <= 0) { error_code = ERR_10; goto go_result; };			// 夯牢捞 辨靛盔捞 酒聪搁..
	if(!m_bGuildMaster) { error_code = ERR_10; goto go_result; };			// 夯牢捞 辨付啊 酒聪搁...

	if(m_tGuildWar == GUILD_WARRING) { error_code = ERR_12; goto go_result; };

	::ZeroMemory(szGuildName, sizeof(szGuildName));

	nLength = GetVarString(sizeof(szGuildName), szGuildName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) { error_code = 255; goto go_result; } 

	pGuild = GetGuildByName(szGuildName);

	if(!pGuild) 
	{ 
		ReleaseGuild();
		error_code = ERR_7; 
		goto go_result; 
	} // 秦寸 辨靛啊 粮犁窍瘤 臼绰促.
	
	pUser = GetUser(pGuild->m_strMasterName);
	ReleaseGuild();

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)  
	{ 
		error_code = ERR_3;							// 措惑 辨付啊 泅犁 霸烙俊 绝促.
		goto go_result; 
	} 

	if(pUser->m_tGuildWar == GUILD_WARRING) { error_code = ERR_12; goto go_result; };
	if(pUser->m_dwGuild <= 0 || !pUser->m_bGuildMaster) { error_code = ERR_3; goto go_result; } // 辑滚 俊矾...

	if(!GetDistance(pUser->m_curx, pUser->m_cury, 12)) 
	{
		SendSystemMsg( IDS_USER_IN_ONE_SCREEN, SYSTEM_NORMAL, TO_ME);
		return;
	}
	
	m_bFieldWarApply = TRUE;
	m_FieldWarGMUid = pUser->m_uid;					// 惑措规 uid甫 扁撅
	m_dwFieldWar = pUser->m_dwGuild;				// 鞘靛傈 矫累捞搁 惑措祈 辨靛 锅龋啊 甸绢柯促.

	pUser->m_FieldWarGMUid = m_uid;					//
	pUser->m_dwFieldWar = m_dwGuild;				//

	TempBuf.Add(GUILD_FIELD_WAR_REQ);
	TempBuf.Add((int)m_dwGuild);
	TempBuf.AddString(m_strGuildName);
	
	Send(TempBuf, TempBuf.GetLength());				// 焊辰 荤恩俊霸 夸没吝捞扼绰 芒捞 钎矫登档废茄促..	
	pUser->Send(TempBuf, TempBuf.GetLength());		// 惑措规俊霸绰 穿啊 夸没沁促绰 
	return;

go_result:
	m_FieldWarGMUid = -1;
	m_dwFieldWar = 0;

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add((BYTE)0x00);						// 角菩
	TempBuf.Add(error_code);
	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
//	鞘靛傈俊 措茄 铰倡咯何客 辨靛傈阑 急器茄促.
//
void USER::SendGuildWarFieldApplyResult(TCHAR *pBuf)
{
	int index = 0;
	int nLength = 0;
	int iGuildID = 0;

	BYTE type = 0;
	BYTE result = FAIL;

	CGuild *pGuild = NULL;
	USER *pUser = NULL;

	CBufferEx TempBuf;

	TCHAR szGuildName[255];

	if(m_dwGuild <= 0) return;
	if(!m_bGuildMaster) return;	
	if(m_FieldWarGMUid < 0 || m_dwFieldWar <= 0) return;
	if(m_tGuildWar == GUILD_WARRING) return;

	TempBuf.Add(GUILD_FIELD_WAR_RESULT);

	pUser = GetUser(m_FieldWarGMUid);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) 
	{
		TempBuf.Add((BYTE)0x02);											// 芭例
		Send(TempBuf, TempBuf.GetLength());									
		return;
	}

	if(pUser->m_dwGuild <= 0 || !pUser->m_bGuildMaster) goto go_result;		// 惑措规捞 辨靛客 惑包 绝栏搁...

	type = GetByte(pBuf, index);											// 芭例捞搁...

	if(type != 1) goto go_result;											// 铰倡捞 酒聪搁...
	
	if(m_bFieldWarApply) return;											// 鞘靛傈阑 脚没茄率篮 搬苞甫 焊辰档 公矫茄促. 
	if(pUser->m_tGuildWar == GUILD_WARRING) goto go_result;

	iGuildID = GetInt(pBuf, index);
	pGuild = GetGuild(iGuildID);											// 辨靛甫 茫绊
	
	if(!pGuild)																// 辨靛啊 绝栏搁	
	{
		ReleaseGuild();
		goto go_result;
	}

	::ZeroMemory(szGuildName, sizeof(szGuildName));

	nLength = GetVarString(sizeof(szGuildName), szGuildName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH)							// 辨靛捞抚捞 肋给 登菌栏搁..
	{
		ReleaseGuild();
		goto go_result;
	}

	if(strcmp(pGuild->m_strGuildName, szGuildName) != 0)					// 辨靛捞抚捞 撇府搁...
	{
		ReleaseGuild();
		goto go_result;
	}

	ReleaseGuild();
	result = SUCCESS;

	SendGuildFieldWarBegin();												// 辨靛傈 矫累阑 舅赴促.

go_result:
	if(result == FAIL)
	{
		m_FieldWarGMUid = -1;
		m_dwFieldWar = 0;	
		pUser->m_FieldWarGMUid = -1;
		pUser->m_dwFieldWar = 0;
	}

	m_bFieldWarApply = FALSE;												// 檬扁拳茄促.(茄率俊辑 脚没窍绊 铰倡窍绰巴阑 规瘤窍扁困秦))	
	pUser->m_bFieldWarApply = FALSE;

	TempBuf.Add(result);											
	Send(TempBuf, TempBuf.GetLength());									
	pUser->Send(TempBuf, TempBuf.GetLength());
	return;

}

/////////////////////////////////////////////////////////////////////////////
//	鞘靛傈俊 措茄 铰倡捞菌促搁 辨靛傈阑 急器茄促.
//
void USER::SendGuildFieldWarBegin()
{
	int i;
	CBufferEx TempBuf;

	USER* pUser = NULL;

	if(m_dwGuild <= 0 || m_dwFieldWar <= 0) return;

	TempBuf.Add(GUILD_WAR);
	TempBuf.Add((BYTE)0x01);							// 辨靛傈 矫累...
	TempBuf.Add((BYTE)GUILD_FIELD_WARRING);				// ...

	TempBuf.Add((short)0x00);							// 鞘靛 钎矫
	TempBuf.Add((BYTE)0x01);							// 曼啊 辨靛盔 刘疙

	TempBuf.Add((short)0x02);							// 醚 辨靛荐
	TempBuf.Add((int)m_dwGuild);
	TempBuf.Add((int)m_dwFieldWar);

	for(i = 0; i < MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		if(pUser->m_dwGuild <= 0) continue;

		if(pUser->m_dwGuild == m_dwGuild) pUser->m_dwFieldWar = m_dwFieldWar;
		else if(pUser->m_dwGuild == m_dwFieldWar) pUser->m_dwFieldWar = m_dwGuild;
		else continue;

		pUser->BeginGuildWar();			// 
		pUser->Send(TempBuf, TempBuf.GetLength());
	}
}

/////////////////////////////////////////////////////////////////////////////
//	鞘靛傈俊辑 亲汗窍绰 率 辨付啊 龋免茄促.
//
void USER::SendGuildWarFieldEnd(TCHAR *pBuf)
{
	int i;
	CBufferEx TempBuf;

	USER* pUser = NULL;
													// 辨傈档 酒聪搁 坷幅				
	if(m_dwGuild <= 0 || m_dwFieldWar <= 0) return;
	
	CString strMsg;

	pUser = GetUser(m_FieldWarGMUid);
	if(!pUser) return;

	if(!pUser->m_bGuildMaster) return;				// 惑措规捞 辨付啊 酒丛 坷幅

	int iFieldGuild = m_dwFieldWar;

	for(i = 0; i < MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if(pUser->m_dwGuild == m_dwGuild || pUser->m_dwGuild == iFieldGuild)
		{
			pUser->EndGuildWar(GUILD_FIELD_WARRING);			// 辨靛 傈捞 场巢
			pUser->SendSystemMsg(pBuf, SYSTEM_NORMAL, TO_ME);
		}
	}
}

//////////////////////////////////////////////////////////////////////
//
//	辑滚捞惑蜡公甫 舅妨霖促.
//
void USER::CheckServerTest()
{
	CString strTemp, strIP;

	strTemp.Format("%u", (unsigned int)((unsigned char*)m_Addr.sin_addr.S_un.S_un_b.s_b1));
	strIP += strTemp; strTemp = _T("");
	strIP += ".";
	strTemp.Format("%u", (unsigned int)((unsigned char*)m_Addr.sin_addr.S_un.S_un_b.s_b2));
	strIP += strTemp; strTemp = _T("");
	strIP += ".";
	strTemp.Format("%u", (unsigned int)((unsigned char*)m_Addr.sin_addr.S_un.S_un_b.s_b3));
	strIP += strTemp; strTemp = _T("");
	strIP += ".";

	CBufferEx TempBuf;

	TempBuf.Add(CHECK_ALIVE_RESULT);

	if(strIP.Left(12).CompareNoCase(_T("192.203.141.")) == 0)
	{
		int nCount = 0;
		USER *pUser = NULL;
		nCount = 0;
		for (int i = 0; i < MAX_USER; i++ )
		{
			pUser = m_pCom->GetUserUid(i);
			if(pUser && pUser->m_state == STATE_GAMESTARTED ) nCount++;
		}

		TempBuf.Add((short)nCount);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add((short)0);
	Send(TempBuf, TempBuf.GetLength());
}


//////////////////////////////////////////////////////////////////////
//
//	碍力 辑滚傍瘤.
//
void USER::AnnounceNotice(TCHAR *pBuf)
{
	CString strTemp, strIP;

	strTemp.Format("%u", (unsigned int)((unsigned char*)m_Addr.sin_addr.S_un.S_un_b.s_b1));
	strIP += strTemp; strTemp = _T("");
	strIP += ".";
	strTemp.Format("%u", (unsigned int)((unsigned char*)m_Addr.sin_addr.S_un.S_un_b.s_b2));
	strIP += strTemp; strTemp = _T("");
	strIP += ".";
	strTemp.Format("%u", (unsigned int)((unsigned char*)m_Addr.sin_addr.S_un.S_un_b.s_b3));
	strIP += strTemp; strTemp = _T("");
	strIP += ".";

	if(strIP.Left(12).CompareNoCase(_T("192.203.141.")) == 0)
	{
		int index = 0;
		SendAllChat(pBuf + index);
	}
}


//////////////////////////////////////////////////////////////////////
//
//	PK穿利蔼捞 0栏肺, (窍困饭骇 己氢悸泼)
//
void USER::SendCityRank(int iCityRank)
{
	CBufferEx TempPKBuf;

	if(iCityRank >= 0) m_sKillCount -= 1;	    // 墨坷俊霸 磷绢辑绰 PK 墨款飘啊 皑家登瘤 臼绰促.

	if(m_sKillCount < 0)
	{
		m_sKillCount = 0;
		if(m_sLevel <= 10 && m_iCityValue < 0)
		{
			m_sCityRank = 0;
			m_iCityValue = 0;

			CheckGuildUserInFortress();			// 矫刮 殿鞭捞 函且锭 促矫 悸泼(器飘府胶甫困秦)
			
			TempPKBuf.Add(SET_USER_PK_STATE);
			TempPKBuf.Add(m_uid + USER_BAND);
			TempPKBuf.Add((BYTE)0x01);
			TempPKBuf.Add(m_sCityRank);

			SendInsight(TempPKBuf, TempPKBuf.GetLength());
//			SendExactScreen(TempPKBuf, TempPKBuf.GetLength());
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	Agent 阿 蜡历 DB俊 矫埃 滚怜阑 扁废
//
DWORD USER::ConvertCurTimeToSaveTime()
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



void USER::MassZoneMove(TCHAR *pBuf)
{
	int i, uid = 0;
	int index = 0, nLen = 0;
	int cityNum = -1, virtualNum = -1;
	int iTotalLevel = 0;

	USER *pUser = NULL;
	CNpc *pNpc = NULL;

	int userCount = 0;
	UserList tempUser[MAX_GUILD_HOUSE_USER];
//	CDWordArray	arUid;
//	arUid.RemoveAll();

	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);

	if( guildTime.wDay < GUILD_VIRTUAL_WAR_START && guildTime.wDay > GUILD_VIRTUAL_WAR_END ) return;

	short sCount = GetShort(pBuf, index);							// 蜡历荐

	if(sCount <= 0 || sCount > MAX_GUILD_HOUSE_USER) return;		// 弥措 20疙鳖瘤
	
	if(m_dwGuild <= 0 || m_dwGuild >= g_arGuildData.GetSize()) return;
	if(!m_bGuildMaster) return;										// 辨靛俊 加窍瘤 臼疽栏搁...
	if(!g_arGuildData[m_dwGuild]) return;

	for(i = 0; i < MAX_GUILD_HOUSE_USER; i++)
	{
		tempUser[i].uid = 0;
		::ZeroMemory(tempUser[i].strUserName, sizeof(tempUser[i].strUserName));
	}

	for(i = 0; i < sCount; i++)
	{
		uid = GetInt(pBuf, index);

		if(uid >= USER_BAND && uid < NPC_BAND)
		{
			pUser = GetUser(uid - USER_BAND);
			if(!pUser || pUser->m_state != STATE_GAMESTARTED) continue;
			if(pUser->m_dwGuild != m_dwGuild) continue;
			if(pUser->m_curz != m_curz) continue;

			nLen = strlen(pUser->m_strUserID);
			if(nLen <= 0) continue;

			iTotalLevel += pUser->m_sLevel;

			userCount++;
			tempUser[i].uid = pUser->m_uid;
			strncpy(tempUser[i].strUserName, pUser->m_strUserID, nLen);
		}
	}

	if(sCount != userCount)									// 弊悼救 函悼捞 乐栏搁 公瓤	
	{
		SendSystemMsg( IDS_USER_INVALID_MEMBER_COUNT, SYSTEM_ERROR, TO_ME);//"人员数不符合."
		return;
	}

	short sGuildHouseZone = GetShort(pBuf, index);					// 啊惑 甘 锅龋
	if( !IsZoneInThisServer(sGuildHouseZone) )	return;				// DB俊 悸泼蔼苞 鞍栏衬?
	//if(sGuildHouseZone==409) return;

	cityNum = GetCityNumForVirtualRoom(sGuildHouseZone);
	if(cityNum < 0 || cityNum >= g_arGuildHouseWar.GetSize()) return;
	if(!g_arGuildHouseWar[cityNum]) return;

	virtualNum = GetVirtualRoomNum(sGuildHouseZone);				// 啊惑 傍埃阑 憿茫摆促.--> 俊矾.
	if(virtualNum < 0) return;

	if(InterlockedCompareExchange((LONG*)&g_arGuildHouseWar[cityNum]->m_CurrentGuild.lUsed, (long)1, (long)0) == (long)0)
	{
		for(i = 0; i < MAX_GUILD_HOUSE_USER; i++)
		{
			g_arGuildHouseWar[cityNum]->m_CurrentGuild.arUserList[i].uid = 0;
			::ZeroMemory(g_arGuildHouseWar[cityNum]->m_CurrentGuild.arUserList[i].strUserName, sizeof(g_arGuildHouseWar[cityNum]->m_CurrentGuild.arUserList[i].strUserName));
		}

		g_arGuildHouseWar[cityNum]->m_CurrentGuild.dwIntervalTick = 0;
		g_arGuildHouseWar[cityNum]->m_CurrentGuild.iGuildNum = userCount;
		g_arGuildHouseWar[cityNum]->m_CurrentGuild.dwTimer = GetTickCount();
		g_arGuildHouseWar[cityNum]->m_CurrentGuild.lGuild = m_dwGuild;
		g_arGuildHouseWar[cityNum]->m_CurrentGuild.iCurValue = 0;		
		g_arGuildHouseWar[cityNum]->m_CurrentGuild.iGuildLevel = iTotalLevel;

		for(i = 0; i < g_arGuildHouseWar[cityNum]->m_CurrentGuild.arNpcList.GetSize(); i++)
		{
			uid = g_arGuildHouseWar[cityNum]->m_CurrentGuild.arNpcList[i];
			pNpc = GetNpc(uid);
			if(!pNpc) continue;

			if(pNpc->m_NpcState == NPC_LIVE) pNpc->Dead();
//			pNpc->m_tNpcAttType = 1;
			pNpc->m_NpcVirtualState = NPC_MOVING;
			pNpc->m_NpcState = NPC_LIVE;
			pNpc->NpcTypeParser();
//			if(pNpc->m_NpcState == NPC_LIVE) pNpc->m_NpcState = NPC_DEAD;
//			pNpc->SetLive(m_pCom);
//			TRACE("%s MOP\n", pNpc->m_strName);
		}

		for(i = 0; i < userCount; i++)
		{
			pUser = GetUser(tempUser[i].uid);
			if(!pUser) continue;

			g_arGuildHouseWar[cityNum]->m_CurrentGuild.arUserList[i].uid = tempUser[i].uid;
			nLen = strlen(tempUser[i].strUserName);
			if(nLen <= 0) continue;
			strncpy(g_arGuildHouseWar[cityNum]->m_CurrentGuild.arUserList[i].strUserName, tempUser[i].strUserName, nLen);
			
			pUser->ZoneMoveReq(sGuildHouseZone, g_arVirtualRoom[virtualNum]->m_Move[i].x, g_arVirtualRoom[virtualNum]->m_Move[i].y);
			pUser->SendSystemMsg( IDS_USER_QUEST_TIME, SYSTEM_NORMAL, TO_ME);//"任务执行时间为 10分钟."
			CString strMsg;
			strMsg.Format( "『%s』带领『%s』军团展开军团BOSS战！他们只有10分钟时间......" ,m_strUserID , m_strGuildName);
			SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
			pUser->m_tGuildHouseWar = GUILD_WARRING;
		}
		//if( m_dwDN <= 10000000 ) m_dwDN = 0;
		//else m_dwDN = m_dwDN - 10000000;
		//UpdateUserItemDN();						//更新数据
		//SendMoneyChanged();						//更新数据	
	}
}

int USER::GetCityNumForVirtualRoom(int zone)		// 瘤陛篮 档矫锅龋瘤父 唱吝俊 VirtualRoom捞 拌加 眠啊登搁..
{													// 官操绢具 等促. (int zone, int &curGuild)
	int nRet = -1;

	switch(zone)									// 眠啊且 抗沥烙...
	{
	case 1005:										// SANAD
		nRet = SANAD;								// curGuild = ;
		break;

	default:
		break;
	}

	return nRet;
}

int USER::GetVirtualRoomNum(int zone)
{														// 茄 档矫救俊 咯矾俺狼 啊惑傍埃捞 乐阑荐 乐栏骨肺 
	for(int i = 0; i < g_arVirtualRoom.GetSize(); i++)	// 弊吝 甘捞 鞍篮 锅龋牢巴阑 茫绰促.
	{
		if(g_arVirtualRoom[i]->m_sZone == zone) return i;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////
//
//	盒剧扁埃吝 辨靛 鉴困甫 焊咯霖促.
//
void USER::SendGuildHouseRank(int nNum, int iCity)
{
	int i, j, index = 0;
	int nLen = 0, nTime = 0;
	int talknum = -1;

	BYTE tCount = 0;
//	TCHAR strTemp[1000];
	CHyper hyperText;

	CBufferEx TempBuf, TempSayBuf;

	CString temp;
	
	if(nNum < 0) return;
	if(iCity < 0 || iCity >= g_arGuildHouseWar.GetSize()) return;
	if(!g_arGuildHouseWar[iCity]) return;

	for(i = 0; i < g_arNpcChat.GetSize(); i++)
	{
		if(g_arNpcChat[i]->m_sCid == nNum)
		{
			talknum = i;
			break;
		}
	}

	if( talknum == -1 ) return;

//	::ZeroMemory(strTemp, sizeof(strTemp));
	hyperText.GetHyperText(g_arNpcChat[talknum]->m_strTalk, g_arNpcChat[talknum]->m_sSize);
	hyperText.load();

	for(i = 0; i < hyperText.m_nCountLine; i++)
	{
		for(j = 0; j < hyperText.m_HyperText[i].m_strText.GetLength()-7; j++){

			temp = hyperText.m_HyperText[i].m_strText.Mid(j, 2);
			if(temp == "%S")
			{
				CGuildHouseWar *tete = g_arGuildHouseWar[iCity];
				if("GN" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 辨靛 捞抚阑 持绰促.
				{
//					hyperText.m_HyperText[i].m_strText.Replace("%SGN%", g_arGuildHouseWar[iCity]->m_TopList[index].strGuildName);
					TempSayBuf.AddString(g_arGuildHouseWar[iCity]->m_TopList[index].strGuildName);
					index++;
					tCount++;
				}
				else if("STM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{					
					tCount++;
					nTime = (int)g_arGuildHouseWar[iCity]->m_CurrentGuild.dwTimer/1000;
					temp.Format("%d", nTime);
					TempSayBuf.AddString( (LPTSTR)(LPCTSTR)temp );
//					hyperText.m_HyperText[i].m_strText.Replace("%STM%", temp);
				}
			}		
		}
		
	}

//	nLen = hyperText.save(strTemp);


	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)nNum);
	TempBuf.Add(tCount);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());

//	TempBuf.AddLongString(strTemp, nLen);

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////
//
//	
//
void USER::CheckQuestEventZoneWarEnd()
{
	g_QuestEventZone.CheckSingleEventZoneWarEnd(m_curz);
}

//////////////////////////////////////////////////////////////////////
//
//	盒剧扁埃吝 辨靛 鉴困甫 Agent俊 历厘窍绊 甘俊辑 狐廉唱柯促.
//
void USER::CheckGuildHouseWarEnd()
{
	int index = -1;
	index = GetCityNumForVirtualRoom(m_curz);

//	USER *pUser = NULL;

	if(index >= 0 && index < g_arGuildHouseWar.GetSize())
	{
		if(!g_arGuildHouseWar[index]) return;

		CheckMaxValue(g_arGuildHouseWar[index]->m_CurrentGuild.iCurValue, 1);	// 捞扒 各捞 磷阑锭付促 龋免登扁锭巩俊

		if(g_arGuildHouseWar[index]->CheckGuildMopEnd())						// 各捞 促 磷菌促. 
		{			
			g_arGuildHouseWar[index]->SendTownPotalMsg(m_pCom);
			g_arGuildHouseWar[index]->CheckGuildHouseRank();					// Agent俊 历厘茄促.
																				// 荤侩吝牢 钎矫甫 钱绢霖促.			
			g_arGuildHouseWar[index]->SetNpcListToWarEnd();


							
/*
			::InterlockedExchange(&g_arGuildHouseWar[index]->m_CurrentGuild.lUsed, 0); 
*/		}
	}
}

//////////////////////////////////////////////////////////////////////
//
//	蜡历啊 磷芭唱, 霸烙辑滚甫 狐廉 唱哎锭付促 眉农...
//
void USER::CheckGuildUserListInGuildHouseWar()
{
	int i, uid = 0;

	int nCount = 0, nLen = 0;
	USER *pUser = NULL;

	int index = GetCityNumForVirtualRoom(m_curz);

	m_tGuildHouseWar = GUILD_WAR_AFFTER;
	
	if( index < 0 || index >= g_arGuildHouseWar.GetSize() )
	{
		return;
	}

	if(!g_arGuildHouseWar[index]) return;

//	if(m_curz == g_arGuildHouseWar[index]->m_CurrentGuild.iWarZone) TownPotal();

	for(i = 0; i < g_arGuildHouseWar[index]->m_CurrentGuild.iGuildNum; i++)
	{
		uid = g_arGuildHouseWar[index]->m_CurrentGuild.arUserList[i].uid;

		if( uid < 0 || uid >= MAX_USER ) continue;

		pUser = m_pCom->GetUserUid(uid);
		if(!pUser) continue;
		
		nLen = strlen(g_arGuildHouseWar[index]->m_CurrentGuild.arUserList[i].strUserName);
		if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) continue;

		if(strcmp(pUser->m_strUserID, g_arGuildHouseWar[index]->m_CurrentGuild.arUserList[i].strUserName) != 0)
		{
			pUser = GetUser(g_arGuildHouseWar[index]->m_CurrentGuild.arUserList[i].strUserName);
		}

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		if(pUser->m_tGuildHouseWar == GUILD_WARRING) nCount++;
	}

	if(nCount <= 0)
	{
		g_arGuildHouseWar[index]->CheckGuildHouseRank();					// Agent俊 历厘茄促.
																			// 荤侩吝牢 钎矫甫 钱绢霖促.			
		g_arGuildHouseWar[index]->SetNpcListToWarEnd();
		g_arGuildHouseWar[index]->SetUserListToWarEnd(m_pCom);				// 悸泼甫 钱绢霖促.

		g_arGuildHouseWar[index]->InitGuild();								// 单捞磐甫 朝赴促.

		g_arGuildHouseWar[index]->m_CurrentGuild.dwIntervalTick = 0;
		::InterlockedExchange(&g_arGuildHouseWar[index]->m_CurrentGuild.lUsed, 0); 
	}
}

//////////////////////////////////////////////////////////////////////
//
//	秦寸 粮俊 积己肚绰 老矫利 力茄捞 吧赴 粮牢瘤 眉农
//
void USER::CheckInvalidGuildZone()
{
	int mapindex = 0;
	int i, index = 0;
	int type = CheckInvalidMapType();
	mapindex = GetGuildMapIndex(type);
	if(mapindex >= 0 && mapindex < g_arMapTable.GetSize())
	{
		index = g_arMapTable[mapindex]->m_sStoreID;

		if(index >= FORTRESS_BAND)
		{
			CGuildFortress* pFort = NULL;
			pFort = GetFortress(index);

			if(pFort) 
			{
				if(g_arMapTable[mapindex]->m_sStoreZone)
				{
					m_bLive = USER_DEAD;
					SetZoneIndex(m_curz);
					IsDeadUser();		
					m_sHP = m_sMaxHP;												//@@@@@@@@@@@ 烙矫内靛
					m_bLive = USER_LIVE;
					return;
				}

				if(pFort->m_lUsed == 1)
				{
					m_bLive = USER_DEAD;
	//				m_sHP = 0;
					SetZoneIndex(m_curz);
					IsDeadUser();		
					m_sHP = m_sMaxHP;												//@@@@@@@@@@@ 烙矫内靛
					m_bLive = USER_LIVE;
					SendSystemMsg( IDS_USER_CANT_REVIVE_IN_WAR, SYSTEM_NORMAL, TO_ME);//"在军团战争中无法复活.
				}
			}
		}
/*		else 
		{
			CStore *pStore = NULL;

			index = g_arMapTable[mapindex]->m_sStoreIndex;

			pStore = GetStore(index);

			if(pStore)
			{
				if(pStore->m_lUsed == 1)								// 秦寸 加己 甘捞 辨靛傈 惑怕	
				{
	//				int temp = m_sHP;
					m_bLive = USER_DEAD;
	//				m_sHP = 0;
					SetZoneIndex(m_curz);
					IsDeadUser();		
					m_sHP = m_sMaxHP;												//@@@@@@@@@@@ 烙矫内靛
					m_bLive = USER_LIVE;
					return;
				}
			}
		}
*/	}

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{		
		if(!g_arGuildFortress[i]) continue;

		if(m_curz == g_arGuildFortress[i]->m_iZone)
		{
			m_bLive = USER_DEAD;
			IsDeadUser();		
			m_sHP = m_sMaxHP;												//@@@@@@@@@@@ 烙矫内靛
			m_bLive = USER_LIVE;
			return;
		}
	}

	for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)
	{
		if(!g_arGuildHouseWar[i]) continue;

		if(m_curz == g_arGuildHouseWar[i]->m_CurrentGuild.iWarZone)
		{
//			int temp = m_sHP;
			m_bLive = USER_DEAD;
//			m_sHP = 0;
			SetZoneIndex(m_curz);
			IsDeadUser();		
			m_sHP = m_sMaxHP;												//@@@@@@@@@@@ 烙矫内靛
			m_bLive = USER_LIVE;
			return;
		}
	}

	for(i = 0; i < g_arGuildHouse.GetSize(); i++)						// 辨靛窍快胶 救牢单.. 家蜡辨靛啊 酒聪搁..
	{
		if(m_curz == g_arGuildHouse[i]->iZone)
		{
//			int temp = m_sHP;
			m_bLive = USER_DEAD;
//			m_sHP = 0;
			SetZoneIndex(m_curz);
			IsDeadUser();		
			m_sHP = m_sMaxHP;												//@@@@@@@@@@@ 烙矫内靛
			m_bLive = USER_LIVE;
			return;
		}
	}

	for(i = 0; i < g_QuestEventZone.m_arEventZone.GetSize(); i++)
	{
		if(m_curz == g_QuestEventZone.m_arEventZone[i])
		{
			m_bLive = USER_DEAD;
			IsDeadUser();		
			m_sHP = m_sMaxHP;												//@@@@@@@@@@@ 烙矫内靛
			m_bLive = USER_LIVE;
			return;
		}
	}
}
/*
void USER::CheckMemoryDB(char* strAccount, char *strUserId)
{
	CSharedMemory*	pSharedMemory;
	CMemUser*		pMD;
	int				mem_index = -1;
	int				nLen1, nLen2;
	CString			str;

	SYSTEMTIME time;
	GetLocalTime(&time);

	for( int i = 0; i < MAX_USER; i++ )
	{
		pSharedMemory = NULL;
		pSharedMemory = g_arSharedMemory[i];

		if(!pSharedMemory) continue;

		pMD = (CMemUser*)pSharedMemory->m_lpData;

		nLen = strlen( pMD->m_strUserID );
		if( nLen <= 0 || nLen > CHAR_NAME_LENGTH ) continue;

		if( strcmp( pMD->m_strUserID, strUserId ) == 0 )
		{
			// 颇老俊 敬促
			if ( g_ChatEnable[m_iModSid] == TRUE )
			{
				str.Format( "(%dM %dD %dH %dMIN)\tuid-%d str-%d uduid-%d savetime-%d id-%s\r\n",
					time.wMonth, time.wDay, time.wHour, time.wMinute, pMD->m_uid, pMD->m_sSTR, pMD->m_UB.m_uid, pMD->m_dwSaveTime, pMD->m_strUserID );
				
				// IKING 2002.1.
				//TRACE("*** bfile write log started:%s ***\n", strUserId);
				EnterCriticalSection( &m_CS_FileWrite );
				g_fpBackServer.Write( str, str.GetLength() );
				LeaveCriticalSection( &m_CS_FileWrite );
			}

			//TRACE("*** bfile write logged end:%s ***\n", strUserId);
			//

//			if(pMD->m_uid == -1) continue;		// 沥府啊 等 蜡历绰 促矫 历厘窍瘤 臼绰促.
//			if(pMD->m_sSTR == 0) continue;		// 付蛮啊瘤

			UpdateMem2DB(pMD);
			UpdateBankMem2DB(pMD);
			UpdateAccountBankMem2DB(pMD);

			pMD->m_uid = -1;
			pMD->m_sSTR = 0;			
			pMD->m_UB.m_uid = -1;
			pMD->m_AB.m_uid = -1;
			pMD->m_dwSaveTime = 0;
			::ZeroMemory(pMD->m_strUserID, sizeof(pMD->m_strUserID));
		}
	}
}
*/
int USER::CheckMemoryDB(char *strUserId)
{
	CSharedMemory*	pSharedMemory;
	CMemUser*		pMD;
	int				mem_index = -1;
	int				nLen;
	int				user_count = 0;
	int				account_count = 0;
	int				ret_val = 0;
	CString			str;

	SYSTEMTIME time;
	GetLocalTime(&time);

	for( int i = 0; i < MAX_USER; i++ )
	{
		pSharedMemory = NULL;
		pMD = NULL;

		pSharedMemory = g_arSharedMemory[i];
		if(!pSharedMemory) continue;

		pMD = (CMemUser*)pSharedMemory->m_lpData;
		if(!pMD) continue;

		if(pMD->m_uid == -1 || pMD->m_sSTR == 0) continue;		// 沥府啊 等 蜡历绰 促矫 历厘窍瘤 臼绰促.
		if(pMD->m_strUserID[0] == 0) continue;

		nLen = strlen( strUserId );
		if( nLen <= 0 || nLen > CHAR_NAME_LENGTH ) continue;

		
		if( _stricmp( pMD->m_strUserID, strUserId ) == 0 && _stricmp( pMD->m_strAccount, m_strAccount ) == 0 )
		{
			user_count++;
			// 颇老俊 敬促
			if(user_count > 1)
			{
				GetLocalTime(&time);
				str.Format("(%04d-%02d-%02d %02d:%02d:%02d)\tuid-%d str-%d ubuid-%d count-%d id-%s\r\n",
					time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,
					pMD->m_uid, pMD->m_sSTR, pMD->m_UB.m_uid, user_count, pMD->m_strUserID );
				
				// IKING 2002.1.
				EnterCriticalSection( &m_CS_FileWrite );
				g_fpBackServer.Write( str, str.GetLength() );
				LeaveCriticalSection( &m_CS_FileWrite );
			}

//			if( !Mem2Game(pMD) )			// Shared Memory -> Game 函荐肺
//			{
//				user_count--;
//			}

			UpdateMem2DB(pMD);		// 救傈阑 困秦辑 DB俊档 历厘阑
			UpdateBankMem2DB(pMD);
			UpdateAccountBankMem2DB(pMD);

			pMD->m_uid = -1;
			pMD->m_sSTR = 0;			
			pMD->m_UB.m_uid = -1;
			pMD->m_AB.m_uid = -1;
			pMD->m_dwSaveTime = 0;
			::ZeroMemory(pMD->m_strUserID, sizeof(pMD->m_strUserID));
			::ZeroMemory(pMD->m_strAccount, sizeof(pMD->m_strAccount));
		}
		// 拌沥篮 鞍篮单 酒捞叼啊 促福促.
		else if( _stricmp( pMD->m_strAccount, m_strAccount ) == 0 && _stricmp( pMD->m_strUserID, strUserId ) != 0 )
		{
			account_count++;

			if(account_count > 1)
			{
				GetLocalTime(&time);
				str.Format("(%04d-%02d-%02d %02d:%02d:%02d)\tuid-%d str-%d ubuid-%d count-%d id-%s (AccountBank)\r\n",
					time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,
					pMD->m_uid, pMD->m_sSTR, pMD->m_AB.m_uid, account_count, pMD->m_strAccount );
				
				// IKING 2002.1.
				EnterCriticalSection( &m_CS_FileWrite );
				g_fpBackServer.Write( str, str.GetLength() );
				LeaveCriticalSection( &m_CS_FileWrite );
			}

//			if( !Mem2GameAccountBank(pMD) )
//			{
//				account_count++;
//			}

			UpdateMem2DB(pMD);		// 救傈阑 困秦辑 DB俊档 历厘阑
			UpdateBankMem2DB(pMD);
			UpdateAccountBankMem2DB(pMD);

			pMD->m_uid = -1;
			pMD->m_sSTR = 0;			
			pMD->m_UB.m_uid = -1;
			pMD->m_AB.m_uid = -1;
			pMD->m_dwSaveTime = 0;
			::ZeroMemory(pMD->m_strUserID, sizeof(pMD->m_strUserID));
			::ZeroMemory(pMD->m_strAccount, sizeof(pMD->m_strAccount));
		}
	}

//	if( user_count == 0 && account_count == 0 ) return 0;		
																// 酒第,篮青,烹芒 葛滴 DB俊辑 啊廉客具 茄促.
//	if( user_count != 0 && account_count == 0 ) return 1;		// 酒第尔 拌沥捞 鞍篮 巴篮 乐菌绊, 拌沥父 鞍绊 酒第啊 促弗巴篮 绝菌促.
																// 酒第,篮青,烹芒 葛滴 MemoryDB俊辑 啊廉吭促. DB俊辑 啊廉坷瘤 臼绰促.
//	if( user_count != 0 && account_count != 0 ) return 1;		// 酒第尔 拌沥捞 鞍篮 巴捞 乐菌绊, 拌沥父 鞍绊 酒第啊 促弗巴档 乐菌促.
																// 酒第,篮青,烹芒 葛滴 MemoryDB俊辑 啊廉吭促. DB俊辑 啊廉坷瘤 臼绰促.
//	if( user_count == 0 && account_count != 0 ) return 2;		// 酒第尔 拌沥捞 鞍篮 巴篮 绝菌绊, 拌沥父 鞍篮 巴捞 乐菌促.
																// 酒第,篮青,烹芒 葛滴 DB俊辑 啊廉客具 茄促. (唱吝俊 烹芒父 MemoryDB俊辑 啊廉坷绰 风凭阑 持阑巴捞促.)

	return 0;
}

//////////////////////////////////////////////////////////////////////
//
//	啊厘 啊鳖款 档矫肺 鸥款器呕 谅钎甫 备茄促.
//
CPoint USER::GetTownPotal(int &potal)
{
	if(m_curz == 66)		//杀人狂大赛死后原地附近复活
	{
		
			int			i;
			BYTE		error;
			ItemList	ReverseItem;
			CBufferEx	TempBuf, TempYourBuf;
			CPoint		ptRevival, ptClientRevival;
			if(m_bLive == USER_LIVE) return NULL;
			ptRevival = FindNearAvailablePoint_S( m_curx, m_cury );

			if( ptRevival.x == -1 || ptRevival.y == -1 )
				TownPotal();

			if( LiveCity( ptRevival.x, ptRevival.y ) == FALSE )
			{
				error = 3;								// 何劝且 厘家甫 给茫澜: 舅荐绝绰 俊矾
				TempBuf.Add( REVIVAL_RESULT );
				TempBuf.Add( error );
				Send( TempBuf, TempBuf.GetLength() );
				return NULL;
			}
			m_bLive = USER_LIVE;
			InterlockedExchange(&m_lDeadUsed, (LONG)0);
			ptClientRevival = ConvertToClient( m_curx, m_cury );
			if(m_sMagicMaxHP > m_sMaxHP) m_sHP = m_sMagicMaxHP;
			else m_sHP = m_sMaxHP;
			error = 0;
			TempBuf.Add( REVIVAL_RESULT );
			TempBuf.Add( error );
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
			TempBuf.Add((DWORD)0);
			TempBuf.Add((DWORD)0);
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
			potal = m_curz;


				int x; int y; int random;
				random = myrand( 1,10);
				switch(random)
				{
				case	1:
					x =14,y=36;
					break;
				case	2:
					x =4,y=48;
					break;
				case	3:
					x =10,y=62;
					break;
				case	4:
					x =34,y=58;
					break;
				case	5:
					x =46,y=42;
					break;
				case	6:
					x =34,y=28;
					break;
				case	7:
					x =26,y=40;
					break;
				case	8:
					x =19,y=51;
					break;
				case	9:
					x =13,y=57;
					break;
				case	10:
					x =29,y=51;
					break;
				}
			
			ptClientRevival.x=x;
			ptClientRevival.y=y;
			return CPoint(ptClientRevival.x, ptClientRevival.y);
		
	}
	else
	{
	int i, num = -1;
	int dx1 = 0, dy1 = 0;
	int dx2 = 0, dy2 = 0;
	int dx3 = 0, dy3 = 0;

	if(CheckInvalidMapType() != 12)				// 措访厘捞 酒匆锭父 辨靛窍快胶客 器飘府胶甫 眉农茄促
	{
		for(i = 0; i < g_arGuildFortress.GetSize(); i++)			// 刚历 Fortress甫 家蜡茄 辨靛牢瘤 眉农茄促.
		{
			if(!g_arGuildFortress[i]) continue;

			if(CheckGuildHouseUser(g_arGuildFortress[i]->m_sFortressID)) 
			{
				potal = g_arGuildFortress[i]->m_iZone;
				return CPoint(g_arGuildFortress[i]->m_iPotalX, g_arGuildFortress[i]->m_iPotalY);
			}
		}

		for(i = 0; i < g_arGuildHouse.GetSize(); i++)				
		{
			if(CheckGuildHouseUser(i + 1)) 
			{
				potal = g_arGuildHouse[i]->iZone;
				return CPoint(g_arGuildHouse[i]->iPotalX, g_arGuildHouse[i]->iPotalY);
			}
		}
	}

	for(i = 0; i < g_TownPotal.GetSize(); i++)					// 酒聪搁 啊鳖款 档矫肺 捞悼茄促.
	{
		if(g_TownPotal[i]->iZone == m_curz) { num = i; break; }
	}

	if(num < 0) 
	{
		potal = -1;
		return CPoint(-1, -1);
	}

	potal = g_TownPotal[num]->iPotalZone;

	CPoint temp = ConvertToClient(m_curx, m_cury);

	if(g_TownPotal[num]->iPotalX <= 0) return CPoint(g_TownPotal[num]->iPotalX1, g_TownPotal[num]->iPotalY1);
	else if(g_TownPotal[num]->iPotalX1 <= 0) return CPoint(g_TownPotal[num]->iPotalX, g_TownPotal[num]->iPotalY);
	else
	{
		dx1 = abs(g_TownPotal[num]->iPotalX - temp.x);
		dy1 = abs(g_TownPotal[num]->iPotalY - temp.y);

		dx2 = abs(g_TownPotal[num]->iPotalX1 - temp.x);
		dy2 = abs(g_TownPotal[num]->iPotalY1 - temp.y);

		i = myrand(0, 2);

		if( (dx1 + dy1) < (dx2 + dy2) )
		{
			if(potal == 400)	return CPoint(g_RandomTownPotal[2]->iPotal[i].x, g_RandomTownPotal[2]->iPotal[i].y);
			else				return CPoint(g_RandomTownPotal[0]->iPotal[i].x, g_RandomTownPotal[0]->iPotal[i].y);
			

//			return CPoint(g_TownPotal[num]->iPotalX, g_TownPotal[num]->iPotalY);
		}
		else
		{
			if( m_curz == 1 )	// 风捞匙飘,荤唱靛,磊羔府酒 - 捞固 荤唱靛啊 急琶等 惑炔
			{
				dx3 = abs( 2463 - temp.x );
				dy3 = abs( 705 - temp.y);

				if( (dx3 + dy3) < (dx2 + dy2) )	// 磊羔府酒 涝备啊 歹 啊滨促搁
				{
					potal = 26;
					return CPoint( 36, 64 );
				}
			}
			if(potal == 400)	return CPoint(g_RandomTownPotal[2]->iPotal[i].x, g_RandomTownPotal[2]->iPotal[i].y);
			else				return CPoint(g_RandomTownPotal[1]->iPotal[i].x, g_RandomTownPotal[1]->iPotal[i].y);

			if(potal == 300) CPoint(g_TownPotal[num]->iPotalX1, g_TownPotal[num]->iPotalY1);
		}
	}
	
	return CPoint(-1, -1);
  }
}
/////////////////////////////////////////////////////////////////////////////////////
//	SpeedHack 荤侩磊牢瘤甫 八荤茄促.
//
void USER::CheckSpeedHack()
{
	m_dwServerTick = GetTickCount();

	if(m_iTickCount == 0)
	{
		m_dwClientTick = GetTickCount();
		m_iTickCount++;
		return;
	}

	DWORD dwGap = 0;
	DWORD dwCurr = GetTickCount(); 

	dwGap = dwCurr - m_dwClientTick;
	TRACE("-----id = %s,  dwGap = %d\n", m_strUserID, dwGap);
	if(dwGap < 32000)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		
		CString str;
		str.Format( IDS_USER_SYSTEM_MSG02, time.wMonth, time.wDay, time.wHour, time.wMinute, m_strAccount, m_strUserID, dwGap);
		
		EnterCriticalSection( &m_CS_FileWrite );
		g_fpSpeedHack.Write(str, str.GetLength());
		LeaveCriticalSection( &m_CS_FileWrite );
		
		SendSystemMsg( IDS_USER_SPEED_HACK, SYSTEM_SPECIAL, TO_ME);
	}

	m_dwClientTick = GetTickCount();
}

/////////////////////////////////////////////////////////////////////////////////////
//	秦寸 辨靛狼 DB客 辨靛盔甸甫 DB俊辑 昏力茄促.
//
BOOL USER::DeleteGuildDB()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	if(m_dwGuild <= 0 || m_dwGuild >= g_arGuildData.GetSize()) return FALSE;
	if(!m_bGuildMaster) return FALSE;
	if(!g_arGuildData[m_dwGuild]) return FALSE;

	::ZeroMemory(szSQL, sizeof(szSQL));

	int index = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call DELETE_GUILD (%d, \'%s\')}"), m_dwGuild, m_strUserID); 

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
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
	else
	{
		//if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		//BREAKPOINT();

		//g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

//void USER::RemoveReceivedData()
//{
//	POSITION pos, pos2;
//	WAIT_RECV_DATA *pWrd;
//
//	return;
//
//	EnterCriticalSection(&m_CS_ReceiveData[m_iSidMod]);
//
//	for ( pos = RecvPtrList[m_iSidMod].GetHeadPosition();  pos != NULL; )
//	{
//		pos2 = pos;
//		pWrd = (WAIT_RECV_DATA *)RecvPtrList[m_iSidMod].GetNext( pos );
//		if ( pWrd )
//		{
//			if ( pWrd->usn == m_Sid )
//			{
//				delete pWrd;
//				pWrd = NULL;
//
//				RecvPtrList[m_iSidMod].RemoveAt(pos2);
//			}
//		}
//	};
//
//	nRecvDataLength[m_iSidMod] = RecvPtrList[m_iSidMod].GetCount();
//
//	LeaveCriticalSection(&m_CS_ReceiveData[m_iSidMod]);
//}

int USER::SockCloseProcess(int nError)
{
	if(m_bSessionOnline) return 0;

	if ( m_UserStatus == 0  ) return 0;

	m_UserStatus = 0;

	if (g_pMainDlg->m_bLogOutExit == TRUE)
	{
		if ( m_state == STATE_GAMESTARTED )
//			LogOutWithDbServer();
			LogOut();

		m_state = STATE_DISCONNECTED;

		CBSocket::SockCloseProcess();  // UserList俊辑 昏力 

		// 辑摹 备炼俊辑 瘤快扁...
		g_pMainDlg->UserFree( m_uid );

		m_UserFlag = FALSE;
		return 0;
	}

	if ( m_state == STATE_GAMESTARTED )
		//LogOutWithDbServer();
		LogOut();

	m_state = STATE_DISCONNECTED;

	CBSocket::SockCloseProcess();  // UserList俊辑 昏力 

	// 辑摹 备炼俊辑 瘤快扁...
	g_pMainDlg->UserFree( m_uid );

	m_UserFlag = FALSE;

	return 1;
}

void USER::OnClose(int nErrorCode) 
{
	CBSocket::B_OnClose(nErrorCode);
}

void USER::OnSend(int nErrorCode) 
{
	if ( m_SockFlag != 1 || m_UserFlag == FALSE ) return;

	CBSocket::B_OnSend( nErrorCode );

	if ( m_SockFlag == 0 )
	{
		SoftClose();
	}
}

int USER::AcceptProcess()
{
	return 1;
}

void USER::StopAction()
{
}

/////////////////////////////////////////////////////////////////////////////////////
//	辨靛窍快胶甫 家蜡茄 辨靛盔父 免涝茄促.
//
BOOL USER::CheckGuildHouseUser(int num)
{
	if(m_dwGuild <= 0) return FALSE;

	if(num >= FORTRESS_BAND)
	{
		CGuildFortress *pFort = NULL;
		pFort = GetFortress(num);

		if(pFort)
		{
			if(pFort->m_iGuildSid == m_dwGuild) return TRUE;
		}
	}
	else
	{
		for(int i = 0; i < g_arGuildHouse.GetSize(); i++)
		{
			if(g_arGuildHouse[i]->iGuild <= 0) continue;

			if(g_arGuildHouse[i]->iSid == num) 
			{			
				if(g_arGuildHouse[i]->iGuild == m_dwGuild) return TRUE;
			}
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////
//	勤叼霸捞飘甫 荤侩窍扁困秦 傈眉 钱府胶飘甫 郴妨霖促.
//
void USER::TeleportReq()
{
	int i, iCount = 0;

	if(!CheckHandyGate()) 
	{
		SendSystemMsg( IDS_USER_NO_HANDYGATE, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(m_bPShopOpen == TRUE){ SendSystemMsg("个人商店状态下无法使用传送门!", SYSTEM_ERROR, TO_ME);return;}

	CBufferEx TempBuf;
	TempBuf.Add(TELEPORT_RESULT);

	for(i = 1; i < TEL_MAX_COUNT; i++)
	{
		if(m_strTeleport[i].iSid != 0) iCount++;
	}

	TempBuf.Add((BYTE)(iCount));

	for(i = 1; i < TEL_MAX_COUNT; i++)
	{
		if(m_strTeleport[i].iSid != 0)
		{
			TempBuf.Add((BYTE)m_strTeleport[i].iSid);
			TempBuf.AddString(m_strTeleport[i].TelName);
			TempBuf.Add((int)m_strTeleport[i].z);//yskang 0.9 handy 荐沥 勤叼肺 捞悼 阂啊瘤开阑 雀祸栏肺 钎矫窍扁 困窍咯
		}
	}

	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////////////
//	DB俊 历厘等 沥焊甫 硅凯俊 悸泼
//
void USER::StrToUserTel(TCHAR *pBuf)
{
	int index = 0;
	int temp = 0;
	int nLength = 0;

//	for(int i = 1; i < TEL_MAX_COUNT; i++)
	do
	{
		temp = GetByte(pBuf, index);
		if(temp <= 0 || temp >= TEL_MAX_COUNT) break;

		m_strTeleport[temp].iSid = temp;
		m_strTeleport[temp].x = GetShort(pBuf, index);
		m_strTeleport[temp].y = GetShort(pBuf, index);
		m_strTeleport[temp].z = GetShort(pBuf, index);

		nLength = GetByte(pBuf, index);
		if(nLength < 0 || nLength > TEL_NAME_LENGTH) break;
	
		GetString(m_strTeleport[temp].TelName, pBuf, nLength, index);

		m_strTeleport[temp].TelName[TEL_NAME_LENGTH + 1] = 0;

	} while(TRUE);
}

/////////////////////////////////////////////////////////////////////////////////////
//	单捞鸥甫 DB俊 历厘 
//
void USER::UserTelToStr(TCHAR *pBuf)
{
	int index = 0;

	for(int i = 1; i < TEL_MAX_COUNT; i++)
	{
		if(m_strTeleport[i].iSid == 0) continue;

		SetByte(pBuf, m_strTeleport[i].iSid, index);
		SetShort(pBuf, m_strTeleport[i].x, index);
		SetShort(pBuf, m_strTeleport[i].y, index);
		SetShort(pBuf, m_strTeleport[i].z, index);
		SetVarString(pBuf, m_strTeleport[i].TelName, strlen( m_strTeleport[i].TelName ), index );
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//	单捞鸥甫 檬扁拳茄促.
//
void USER::InitTelList(TeleportList *strTel)
{
	for(int i = 0; i < TEL_MAX_COUNT; i++)
	{
		strTel[i].iSid = 0;
		strTel[i].x = 0;
		strTel[i].y = 0;
		strTel[i].z = 0;
		::ZeroMemory(strTel[i].TelName, sizeof(strTel[i].TelName));		
	}

	m_lCopyUsed = 0;
	m_CopyUid = 0;				// copy chip 俊 荤侩登绰 酒捞袍
	m_tIndex = 0;
	::ZeroMemory(m_strCopyUser, sizeof(m_strCopyUser));	
}

/////////////////////////////////////////////////////////////////////////////////////
//	炮 谅钎甫 扁撅窍芭唱 昏力茄促.
//
void USER::TelportEdit(TCHAR *pBuf)
{
	int i, mapindex = 0;
	int index = 0;
	BYTE tIndex = 0;
	
	CPoint pt;
	int nLen = 0, type = 0;
	BOOL bSuccess = FALSE;	
	TCHAR szName[TEL_NAME_LENGTH + 1];	
	CBufferEx TempBuf;
	
	if(!CheckHandyGate()) 
	{
		SendSystemMsg( IDS_USER_NO_HANDYGATE, SYSTEM_ERROR, TO_ME);
		return;
	}

	BYTE tType = GetByte(pBuf, index);

	::ZeroMemory(szName, sizeof(szName));

	if(tType == TEL_ADD_EDIT)
	{	
		for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)
		{
			if(!g_arGuildHouseWar[i]) continue;

			if( m_curz == g_arGuildHouseWar[i]->m_CurrentGuild.iWarZone)
			{
				SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
				goto go_result;
			}
		}

		for(i = 0; i < g_arGuildHouse.GetSize(); i++)
		{
			if( m_curz == g_arGuildHouse[i]->iZone)
			{
				SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
				goto go_result;
			}
		}

		for(i = 0; i < g_QuestEventZone.m_arEventZone.GetSize(); i++)
		{
			if(m_curz == g_QuestEventZone.m_arEventZone[i])
			{
				SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
				goto go_result;
			}
		}

		for(i = 0; i < g_arGuildFortress.GetSize(); i++)
		{
			//break;
			if(!g_arGuildFortress[i]) continue;

			if(m_curz == g_arGuildFortress[i]->m_iZone)
			{
				SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);//yskang handy don't save
				goto go_result;
			}

			type = CheckInvalidMapType();
			mapindex = GetGuildMapIndex(type);
			if(mapindex >= 0 && mapindex < g_arMapTable.GetSize())
			{
				if(g_arMapTable[mapindex])
				{
					if(g_arMapTable[mapindex]->m_sStoreID >= FORTRESS_BAND)
					{
						if(g_arMapTable[mapindex]->m_sStoreZone == 1) 
						{
							SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
							goto go_result;
						}
					}
				}
			}			
		}

		if( m_curz == 40 || m_curz == 43 || m_curz == 44 || m_curz == 45 || m_curz == 57 || m_curz == 58 || m_curz == 59 )		// 措访厘
		{
			SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}           //无法使用传送门
        if( m_curz == 56 || m_curz == 16 || m_curz == 12 || m_curz == 19  || m_curz == 36 || m_curz == 49 || m_curz == 92 || m_curz == 409 || m_curz == 416 || m_curz == 61|| m_curz == 66 || m_curz == 67 || m_curz == 319 ) 		// 快林粮(56), 捞亥飘各粮(16,12,19)篮 历厘秦林瘤 臼绰促.
		{
			SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}
//关闭商店57点传点保存...
		if( /*m_curz == 10 ||*/ m_curz == 92 || m_curz == 24 || m_curz == 20  ||/* m_curz == 403 ||*/ m_curz == 303 || m_curz == 304 || m_curz == 61 || m_curz == 319/* || IsCity()*/)		// 快林粮(56), 捞亥飘各粮(16,12,19)篮 历厘秦林瘤 臼绰促.
		{
			SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}
		if(type == 8 || type == 10 || type == 15 || type == 17 || type == 9 || type == 11 || type == 16 || type == 18)
		{
            SendSystemMsg( "要塞警戒线内无法保存,请离要塞远点保存!", SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		nLen = GetVarString(sizeof(szName), szName, pBuf, index);
		if(nLen <= 0 || nLen > TEL_NAME_LENGTH) goto go_result;

		i = TEL_MAX_COUNT - 1;

		do
		{
			if(strcmp(m_strTeleport[i].TelName, szName) == 0)
			{
				SendSystemMsg( IDS_USER_ALREADY_HANDYGATE_SAVE, SYSTEM_ERROR, TO_ME);
				goto go_result;
			}
			i--;
		} while(i >= 1);

		for(i = 1; i < TEL_MAX_COUNT; i++)
		{
			if(m_strTeleport[i].iSid == 0) 
			{
				pt = ConvertToClient(m_curx, m_cury);
				if(pt.x <= -1 || pt.y <= -1) goto go_result;

				m_strTeleport[i].iSid = i;
				m_strTeleport[i].x = pt.x;
				m_strTeleport[i].y = pt.y;
				m_strTeleport[i].z = m_curz;

				strncpy(m_strTeleport[i].TelName, szName, nLen);
				tIndex = (BYTE)i;
				bSuccess = TRUE;

				SendSystemMsg( IDS_USER_HANDYGATE_SAVE, SYSTEM_NORMAL, TO_ME);

				break;
			}
		}		
	}
	else if(tType == TEL_DEL_EDIT)
	{
		tIndex = GetByte(pBuf, index);
		if(tIndex <= 0 || tIndex >= TEL_MAX_COUNT) goto go_result;

		nLen = GetVarString(sizeof(szName), szName, pBuf, index);
		if(nLen <= 0 || nLen > TEL_NAME_LENGTH) goto go_result;

		if(strcmp(m_strTeleport[tIndex].TelName, szName) != 0) goto go_result;

		bSuccess = TRUE;
		m_strTeleport[tIndex].iSid = 0;	
		m_strTeleport[tIndex].x = 0;
		m_strTeleport[tIndex].y = 0;
		m_strTeleport[tIndex].z = 0;
		::ZeroMemory(m_strTeleport[tIndex].TelName, nLen);

		SendSystemMsg( IDS_USER_HANDYGATE_DELETE, SYSTEM_NORMAL, TO_ME);
	}

go_result:
	TempBuf.Add(TELEPORT_EDIT_RESULT);

	if(!bSuccess)
	{
		TempBuf.Add((BYTE)0x00);				// 角菩
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add((BYTE)0x01);					// 己傍						
	TempBuf.Add(tType);							// 昏力 肚绰 眠啊 
	TempBuf.Add(tIndex);						// 秦寸 牢郸胶 
	TempBuf.AddString(szName);					// 
	TempBuf.Add((int)m_curz);//yskang 0.9 handy 荐沥 勤叼肺 捞悼 阂啊瘤开阑 雀祸栏肺 钎矫窍扁 困窍咯 
	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////////////
//
void USER::ChangeUserSpeed(TCHAR *pBuf)
{
	return;
/*
	CBufferEx TempBuf;

	int index = 0;

	short step = atoi(pBuf);

	if(step < 0 || step > 500) step = 0;

	TempBuf.Add(SET_SPEED_MONSTER);

	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(step);

	SendInsight(TempBuf, TempBuf.GetLength());

	SightRecalc();
*/
}
void USER::ChangeUserSpeed(TCHAR *pBuf, int z)
{

	CBufferEx TempBuf;

	int index = 0;

	TempBuf.Add(SET_SPEED_MONSTER);

	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(z); //移动速度

	SendInsight(TempBuf, TempBuf.GetLength());

	SightRecalc();

} 

/////////////////////////////////////////////////////////////////////////////////////
//	使用传送门
//
void USER::GetHanyGate(int slot, int sid)
{
	if(m_dwGuild >= 1 && m_dwGuild < g_arGuildData.GetSize())// 辨靛 芒绊甫 凯绢初篮 惑怕俊辑绰 捞悼阑 给茄促.
	{
		if(!g_arGuildData[m_dwGuild]) return;

		if(g_arGuildData[m_dwGuild]->m_lUsed == 1)			// 辨靛 芒绊啊 荤侩吝捞扼搁
		{
			int nLen = strlen(g_arGuildData[m_dwGuild]->m_strUsedUser);
			if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;// 磊扁牢瘤 眉农		

			if(strcmp(m_strUserID, g_arGuildData[m_dwGuild]->m_strUsedUser) == 0) return;
		}
	}			
	
	int i;
	CPoint pt;

	if(slot < EQUIP_ITEM_NUM || slot >= TOTAL_INVEN_MAX) return;

	int sSid = m_UserItem[slot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return;

	if(g_arItemTable[sSid]->m_byWear != 105)		// 勤叼霸捞飘
	{
		SendSystemMsg( IDS_USER_NO_HANDYGATE, SYSTEM_ERROR, TO_ME);
		return;
	}

	if(m_UserItem[slot].sDuration <= 0) 
	{
		SendSystemMsg( IDS_USER_FIRST_REPAIR, SYSTEM_ERROR, TO_ME);
		return;
	}

	if(sid <= 0 || sid >= TEL_MAX_COUNT) return;

	if(m_strTeleport[sid].iSid == 0) return;

	int x = m_strTeleport[sid].x;
	int y = m_strTeleport[sid].y;

	for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)		// 捞亥飘 粮栏肺 啊芭唱 呕免甫 阜绰促.
	{
		if(!g_arGuildHouseWar[i]) continue;

		if(m_strTeleport[sid].z == g_arGuildHouseWar[i]->m_CurrentGuild.iWarZone)
		{
			SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
			return;
		}
		else if(m_curz == g_arGuildHouseWar[i]->m_CurrentGuild.iWarZone)
		{
			SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
			return;
		}
	}

	for(i = 0; i < g_arGuildHouse.GetSize(); i++)
	{
		if(m_strTeleport[sid].z == g_arGuildHouse[i]->iZone)
		{
			SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
			return;
		}
	}
	

	for(i = 0; i < g_QuestEventZone.m_arEventZone.GetSize(); i++)
	{
		if(g_QuestEventZone.m_arEventZone[i] == m_curz)
		{
			SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
			return;
		}
	}

	if( m_strTeleport[sid].z == 16 || m_strTeleport[sid].z == 12 || m_strTeleport[sid].z == 19 || m_strTeleport[sid].z == 36 || m_strTeleport[sid].z == 49 || m_strTeleport[sid].z == 1007 )// 捞亥飘各粮(16,12,19)篮 哎荐 绝促
	{
		SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
		return;
	}
	if(m_dwLhslTime != 0 && (!IsCity()))
		{
			SendEventMsg( "灵魂锁链状态下(5秒)无法使用传送门!");
			return;
		}           //无法使用传送门
	//关闭商店57点传点保存...
	if( m_strTeleport[sid].z == 24 || m_strTeleport[sid].z == 20 || m_strTeleport[sid].z == 303 || m_strTeleport[sid].z == 304 || m_strTeleport[sid].z == 61)		// 快林粮(56), 捞亥飘各粮(16,12,19)篮 历厘秦林瘤 臼绰促.
	{
		SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
		return ;
	}

	
	if( !IsZoneInThisServer( m_strTeleport[sid].z ) )
	{
		SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
		return;
	}
	// alisia

	//------------------------------------------------------------------------------------------------
	// 器飘府胶 康开郴肺绰 勤叼 阂啊 yskang handy
	int zzz = -1;
	for(int j = 0; j < g_zone.GetSize(); j++)
	{
		if( g_zone[j]->m_Zone == m_strTeleport[sid].z )
		{
			zzz = j;
			break;
		}
	}
	if(zzz < 0 || zzz >= g_zone.GetSize()) return;
	CPoint ptPotal = ConvertToServerByZone(m_strTeleport[sid].z, x, y);
	if(ptPotal.x == -1 || ptPotal.y == -1 ) return;
	ptPotal = FindNearAvailablePoint_S(m_strTeleport[sid].z,ptPotal.x, ptPotal.y);
	if(ptPotal.x == -1 || ptPotal.y == -1 ) return;

	MAP* pMap = g_zone[zzz];
	if(pMap == NULL) return;
	int cx = pMap->m_sizeMap.cx;
	int cy = pMap->m_sizeMap.cy;
	if(ptPotal.x < 0 || ptPotal.x >= pMap->m_sizeMap.cx ) return;
	if(ptPotal.y < 0 || ptPotal.y >= pMap->m_sizeMap.cy ) return;

	int iType = ((g_zone[zzz]->m_pMap[ptPotal.x][ptPotal.y].m_dwType & 0xFF00 ) >> 8);

	if(iType == 8 || iType == 10 || iType == 15 || iType == 17) 
	{
		SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
		return;
	}
	//---------------------------------------------------------------------------------------------------

	if(m_strTeleport[sid].z != m_curz) 
	{
		ZoneMoveReq(m_strTeleport[sid].z, x, y);
		if(m_strTeleport[sid].z == m_curz)
		{
		//	SendDuration((BYTE)slot, 1);
//			m_UserItem[slot].sDuration -= 1;			// 茄锅 荤侩俊 1究 郴备档甫 临咯唱埃促.
//			if(m_UserItem[slot].sDuration < 0) m_UserItem[slot].sDuration = 0;
		}
	}
	else 
	{
		int type = 0;

		CPoint ptPotal = ConvertToServer(x, y);			// 鞍篮 粮篮 蝶肺 谅钎函拳甫 ...

		pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
		if(pt.x <= -1 || pt.y <= -1) return;

		type = ((g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_dwType & 0xFF00 ) >> 8);

		if(!CheckInvalidZoneState(type)) return;
/*
		if(type > 1 && type < 8)
		{
			int index = g_arMapTable[type]->m_sStoreIndex;
			if(index < 0 || index >= g_arStore.GetSize()) return; 
			if(g_arStore[index]->m_lUsed == 1)	return; // 辨靛 惑痢傈捞搁 哎荐绝促.
			// 菊栏肺 眠啊...
		}
*/					
		if(Teleport(pt.x, pt.y) == FALSE) return;

		//SendDuration((BYTE)slot, 1);  //传送门不掉耐久

	}

//	SendDuration((BYTE)slot, -1);
}

BOOL USER::CheckHandyGate()
{
	int iSid = 0;

	for(int i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		iSid = m_UserItem[i].sSid;
		if(iSid < 0 || iSid >= g_arItemTable.GetSize()) continue;
		if(g_arItemTable[iSid]->m_byWear == 105)
		{
			if(m_UserItem[i].sDuration <= 0) continue;

			return TRUE;// 勤叼霸捞飘
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////
//	墨乔抹甫 捞侩窍扁困秦 惑措规狼 悼狼甫 备茄促.
//
void USER::TeleportCopyReq(TCHAR *pBuf)
{
	// 02-10-12 by Youn Gyu
	int type = CheckInvalidMapType();
	if(!CheckInvalidZoneState(type)) return;

	if(InterlockedCompareExchange((LONG*)&m_lCopyUsed, (long)1, (long)0)  == (long)0 ) 	// 泅犁 墨乔抹捞 抗距捞 绝栏搁
	{
		int		i, index = 0;
		
		BYTE	tSlot;					// 荤侩窍绊磊 窍绰 酒捞袍

		USER	*pUser = NULL;
		BOOL	bFlag = FALSE;

		TCHAR	szName[TEL_NAME_LENGTH + 1];	

		CBufferEx TempBuf, TempBuf1;

		m_CopyUid = 0;								// 措惑阑 檬扁拳茄促.
		m_tIndex=  0;
		::ZeroMemory(m_strCopyUser, sizeof(m_strCopyUser));
		::ZeroMemory(szName, sizeof(szName));

		short sSid = 0;
		int nLength = 0;

		if(!CheckHandyGate()) 
		{
			InterlockedExchange(&m_lCopyUsed, 0);	// 盔惑 汗蓖				 
			SendSystemMsg( IDS_USER_NO_HANDYGATE, SYSTEM_ERROR, TO_ME);
			return;
		}

		m_CopyUid = GetInt(pBuf, index);

	//	if(m_CopyUid < 0 || m_CopyUid >= INVALID_BAND) goto go_result;
		if(m_CopyUid < USER_BAND || m_CopyUid >= NPC_BAND) goto go_result;

		nLength = GetVarString(CHAR_NAME_LENGTH, m_strCopyUser, pBuf, index);
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) goto go_result;

		pUser = GetUser(m_CopyUid - USER_BAND);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)
		{
			SendSystemMsg( IDS_USER_NO_USER_ID, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		if(strcmp(pUser->m_strUserID, m_strCopyUser) != 0)
		{
			SendSystemMsg( IDS_USER_INVALID_USER_ID, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		if(IsThereUser(pUser) == FALSE)
		{
			SendSystemMsg( IDS_USER_SEE_EACH_OTHER, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		tSlot = GetByte(pBuf, index);
		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) goto go_result;

		sSid = m_UserItem[tSlot].sSid;
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) goto go_result;
        if (sSid==633) 
		{
			SendSystemMsg( IDS_USER_NO_COPYCHIP, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}
		if(g_arItemTable[sSid]->m_byWear != 106)
		{
			SendSystemMsg( IDS_USER_NO_COPYCHIP, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		m_tIndex = GetByte(pBuf, index);
		if(m_tIndex <= 0 || m_tIndex >= TEL_MAX_COUNT) goto go_result;

		nLength = GetVarString(sizeof(szName), szName, pBuf, index);
		if(nLength <= 0 || nLength > TEL_NAME_LENGTH) goto go_result;

		if(m_strTeleport[m_tIndex].iSid <= 0)
		{
			SendSystemMsg( IDS_USER_INVALID_AXIS, SYSTEM_ERROR, TO_ME);
			goto go_result;				// 秦寸 炮阑 历厘 救沁促搁	
		}

		if( m_strTeleport[m_tIndex].z == 16 || m_strTeleport[m_tIndex].z == 12 || m_strTeleport[m_tIndex].z == 19 )	// 捞亥飘各粮(16,12,19)篮 哎荐 绝促
		{
			SendSystemMsg( IDS_USER_NO_HANDYGATE_AREA, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		if(strcmp(m_strTeleport[m_tIndex].TelName, szName) != 0)
		{
			SendSystemMsg( IDS_USER_REVIEW_THIS_AXIS, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)
		{
			if(!g_arGuildHouseWar[i]) continue;

			if(m_strTeleport[m_tIndex].z == g_arGuildHouseWar[i]->m_CurrentGuild.iWarZone)
			{
				SendSystemMsg( IDS_USER_CANT_USE_AREA, SYSTEM_ERROR, TO_ME);
				goto go_result;
			}
		}

		for(i = 0; i < g_QuestEventZone.m_arEventZone.GetSize(); i++)
		{
			if(m_strTeleport[m_tIndex].z == g_QuestEventZone.m_arEventZone[i])
			{
				SendSystemMsg( IDS_USER_CANT_USE_AREA, SYSTEM_ERROR, TO_ME);
				goto go_result;
			}
		}

		for(i = 0; i < g_arGuildHouse.GetSize(); i++)
		{
			if(m_strTeleport[m_tIndex].z == g_arGuildHouse[i]->iZone)
			{
				SendSystemMsg( IDS_USER_CANT_USE_AREA, SYSTEM_ERROR, TO_ME);
				goto go_result;
			}
		}

		bFlag = TRUE;

go_result:
		if(!bFlag)
		{
			InterlockedExchange(&m_lCopyUsed, 0);		// 盔惑 汗蓖

			m_CopyUid = 0;								// 措惑阑 檬扁拳茄促.
			m_tIndex=  0;
			::ZeroMemory(m_strCopyUser, sizeof(m_strCopyUser));
			return;
		}

		TempBuf.Add(TELEPORT_COPY_DIALOG_REQ);
		TempBuf.Add((BYTE)0x02);						// 乔 脚没牢
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.AddString(m_strUserID);
		pUser->Send(TempBuf, TempBuf.GetLength());

		TempBuf1.Add(TELEPORT_COPY_DIALOG_REQ);
		TempBuf1.Add((BYTE)0x01);						// 脚没牢
		TempBuf1.Add(pUser->m_uid + USER_BAND);
		TempBuf1.AddString(pUser->m_strUserID);
		Send(TempBuf1, TempBuf1.GetLength());
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//	墨乔抹俊措秦 惑措规 荐遏咯何甫 搬沥茄促.
//
void USER::TeleportCopyResult(TCHAR *pBuf)
{
	int uid = 0;
	int tSlot = 0;
	int index = 0, nLen = 0;
	int z = 0, x = 0, y=  0;

	BOOL bFlag = FALSE;
	BYTE tRefuse1 = 0x04, tRefuse2 = 0x04;

	CBufferEx TempBuf, TempBuf2;

	TCHAR	szName[CHAR_NAME_LENGTH + 1];	
	USER *pUser = NULL;

	CPoint pt;

	::ZeroMemory(szName, sizeof(szName));

	BYTE tType = GetByte(pBuf, index);	

	uid = GetInt(pBuf, index);
//	if(uid < 0 || uid >= INVALID_BAND) goto go_result;
	if(uid < USER_BAND || uid >= NPC_BAND) return;

	nLen = GetVarString(sizeof(szName), szName, pBuf, index);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;

	pUser = GetUser(uid - USER_BAND);

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	if(strcmp(pUser->m_strUserID, szName) != 0)	return;

	if(tType != 1) 
	{ 
		tRefuse1 = 0x03; 
		tRefuse2 = 0x02; // 秒家, 芭例
		goto go_result; 
	}						// 芭例...

	if(InterlockedCompareExchange((LONG*)&pUser->m_lCopyUsed, (long)0, (long)1)  == (long)1 ) 	// 泅犁 墨乔抹捞 抗距捞 乐绢具
	{
		tSlot = pUser->CheckCopyChip();		// 脚没牢捞 勤叼霸捞飘啊 乐唱 绝唱 促矫 犬牢
		if(tSlot < 0) goto go_result;

		if(pUser->m_tIndex <= 0 || pUser->m_tIndex >= TEL_MAX_COUNT) goto go_result;

		if(pUser->m_strTeleport[pUser->m_tIndex].iSid == 0) goto go_result;

		z = pUser->m_strTeleport[pUser->m_tIndex].z;
		x = pUser->m_strTeleport[pUser->m_tIndex].x;
		y = pUser->m_strTeleport[pUser->m_tIndex].y;
		
		index = IsMovableTel_S(z, x, y);
		if(index < 2) 
		{
			pUser->SendSystemMsg( IDS_USER_MOVE_FAIL, SYSTEM_ERROR, TO_ME);
			SendSystemMsg( IDS_USER_MOVE_FAIL, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		if(!pUser->SendCopyTel(z, x, y))
		{
			pUser->SendSystemMsg( IDS_USER_MOVE_FAIL, SYSTEM_ERROR, TO_ME);
			SendSystemMsg( IDS_USER_MOVE_FAIL, SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		bFlag = TRUE;
	}
go_result:

	if(!bFlag)
	{
		TempBuf.Add(TELEPORT_COPY_RESULT);
		TempBuf.Add(tRefuse1);						// 芭例...
		Send(TempBuf, TempBuf.GetLength());

		TempBuf2.Add(TELEPORT_COPY_RESULT);
		TempBuf2.Add(tRefuse2);						// 芭例...		
		if(pUser)
		{
			pUser->Send(TempBuf2, TempBuf2.GetLength());
			InterlockedExchange(&pUser->m_lCopyUsed, 0);// 盔惑 汗蓖
		}

		InterlockedExchange(&m_lCopyUsed, 0);		// 盔惑 汗蓖
		return;
	}

	TempBuf2.Add(TELEPORT_COPY_RESULT);
	TempBuf2.Add((BYTE)0x01);
	pUser->Send(TempBuf2, TempBuf2.GetLength());

	TempBuf.Add(ITEM_USE_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add((BYTE)QUICK_ITEM_TELEPORT);
	TempBuf.Add((BYTE)tSlot);

	pUser->m_UserItem[tSlot].sCount -= 1;		//
	if(pUser->m_UserItem[tSlot].sCount <= 0) { ReSetItemSlot( &pUser->m_UserItem[tSlot] ); }

	TempBuf.Add(pUser->m_UserItem[tSlot].sLevel);
	TempBuf.Add(pUser->m_UserItem[tSlot].sSid);
	TempBuf.Add(pUser->m_UserItem[tSlot].sDuration);
	TempBuf.Add(pUser->m_UserItem[tSlot].sBullNum);
	TempBuf.Add(pUser->m_UserItem[tSlot].sCount);
	for(int i = 0; i < MAGIC_NUM; i++) TempBuf.Add(pUser->m_UserItem[tSlot].tMagic[i]);
	TempBuf.Add(pUser->m_UserItem[tSlot].tIQ);
	pUser->Send(TempBuf, TempBuf.GetLength());
	pUser->GetRecoverySpeed();							// 雀汗加档 眉农...

	pt = pUser->ConvertToClient(pUser->m_curx, pUser->m_cury);
	
	if(!SendCopyTel(pUser->m_curz, pt.x, pt.y, TRUE))
	{
		CString strMsg = _T("");
		strMsg.Format( IDS_USER_WHO_MOVE_FAIL, m_strUserID);
		pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
//		pUser->SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ERROR, TO_ME);
		SendSystemMsg( IDS_USER_MOVE_FAIL, SYSTEM_ERROR, TO_ME);

		InterlockedExchange(&pUser->m_lCopyUsed, 0);		// 盔惑 汗蓖
		return;
	}

	InterlockedExchange(&m_lCopyUsed, 0);		// 盔惑 汗蓖
}

/////////////////////////////////////////////////////////////////////////////////////
//	窃膊 捞悼茄促.
//
BOOL USER::SendCopyTel(int z, int x, int y, BOOL bSuccess)
{
	int i, type = 0;

	CPoint pt;

	if( !IsZoneInThisServer(z) ) return FALSE;

	if(x < 0 || y < 0) return FALSE;

	if(z != m_curz)
	{
		ZoneMoveReq(z, x, y);
		if(z != m_curz) 
		{
			if(bSuccess) { DoubleCopyTel(z, x, y, TRUE); return TRUE; }
			return FALSE;
		}
	}
	else 
	{
		int rand = 0, type = 0;

		CPoint ptPotal = ConvertToServer(x, y);			// 鞍篮 粮篮 蝶肺 谅钎函拳甫 ...

		pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
		if(pt.x <= -1 || pt.y <= -1) 
		{
			if(bSuccess) { DoubleCopyTel(z, x, y, FALSE); return TRUE; }
			return FALSE;
		}

		type = ((g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_dwType & 0xFF00 ) >> 8);
		if(!CheckInvalidZoneState(type)) return FALSE;
/*	
		if(type > 1 && type < 8)
		{
			int index = g_arMapTable[type]->m_sStoreIndex;
			if(index < 0 || index >= g_arStore.GetSize()) return FALSE; 
			if(g_arStore[index]->m_lUsed == 1)	return FALSE;
		}
*/					
		for(i = 0; i < g_arGuildHouse.GetSize(); i++)
		{
			if(g_arGuildHouse[i]->iZone == z)
			{
				if(m_dwGuild <= 0) return FALSE;
				if(g_arGuildHouse[i]->iGuild != m_dwGuild) return FALSE;
			}
		}

		if(Teleport(pt.x, pt.y) == FALSE)
		{
			if(bSuccess) { DoubleCopyTel(z, x, y, FALSE); return TRUE; }
			return FALSE;
		}
	}

	return TRUE;
}

int USER::CheckCopyChip()
{
	int iSid = 0;

	for(int i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		iSid = m_UserItem[i].sSid;
		if(iSid < 0 || iSid >= g_arItemTable.GetSize()) continue;
		if(g_arItemTable[iSid]->m_byWear == 106)
		{
			if(m_UserItem[i].sCount <= 0) 
			{
				ReSetItemSlot( &m_UserItem[i] );
				continue;
			}
			return i;
		}
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////////////
//	捞固 乐绰 蜡历 困俊 uid甫 丹绢 警款促.
//
void USER::DoubleCopyTel(int z, int x, int y, BOOL zone)
{
	MAP* pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return;

	pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return;

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) return;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) return;

	if(zone)
	{
		/*************************[ Zone Change Init]*******************************/
		long temp = g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser;
		if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 
				(long)0, (long)(m_uid + USER_BAND)) == (long)m_uid + USER_BAND)
		{
			temp = g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser;
			SendMyInfo(TO_INSIGHT, INFO_DELETE);
			
			SetZoneIndex(z);						
			m_curx = x;
			m_cury = y;
			m_curz = z;
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, m_uid + USER_BAND);

			SendZoneChange(TRUE);	// 粮眉牢瘤甫  刚历焊郴具 唱吝 NPC沥焊啊 client俊辑 混酒乐促.

			m_presx = -1;
			m_presy = -1;
			SightRecalc();
			SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		}
	}
	else
	{
		BYTE result = FAIL;
		int index = 0;

		CPoint ptTeleport;

		CPoint startPt, pt;

		startPt.x = m_curx;	startPt.y = m_cury;
		pt.x = x; pt.y = y;

		ptTeleport = ConvertToClient(x, y);		
		
		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);

		m_curx = x;
		m_cury = y;

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, m_uid + USER_BAND);

		result = SUCCESS;
		
		m_tDir = GetDirection(startPt, pt);		// 泅犁 规氢阑 钎矫茄促.;

		index = 0;
		SetByte(m_TempBuf, PSI_TOWN_POTAL, index);
		SetByte(m_TempBuf, result, index);

		SetByte(m_TempBuf, 0, index);				// 亲惑 鞍篮粮  // 唱吝俊 绝举巴

		SetInt(m_TempBuf, m_uid + USER_BAND, index);

		SetShort(m_TempBuf, ptTeleport.x, index);
		SetShort(m_TempBuf, ptTeleport.y, index);
		SetShort(m_TempBuf, m_curz, index);
		
		Send(m_TempBuf, index);

	//	SendMyTownPotal(TO_INSIGHT, INFO_TOWNPOTAL);
		SendMyInfo(TO_INSIGHT, INFO_TOWNPOTAL);
		SightRecalc();
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//	 蜡历啊 啊瘤绊 乐绰 TEL谅钎绰 Client谅钎捞骨肺 辑滚 谅钎肺 函券窍咯 哎荐乐绰 谅钎啊 2镑 捞惑牢瘤 眉农茄促.
//
int USER::IsMovableTel_S(int z, int x, int y)
{
	int iZoneIndex = -1,i;

	for( i = 0; i < g_zone.GetSize(); i++)			// 捞悼且 粮狼 牢郸胶甫 备茄促.
	{
		if( g_zone[i]->m_Zone == z )
		{
			iZoneIndex = i;
			break;
		}
	}

	int dir[25][2];
	int tempx = 0, tempy = 0, temph = 0;
	int nRet = 0;

	if(iZoneIndex >= 0)
	{
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

		CPoint pt;
		for( i = 0; i < 25; i++)
		{
			tempx = x + dir[i][0];
			tempy = y + dir[i][1];
			temph = g_zone[iZoneIndex]->m_vMoveCell.m_vDim.cy / 2 - 1;				// 辑滚谅钎肺 函券茄促.

			if( x < 0 || y < 0 ) continue; 
			if( y >= g_zone[iZoneIndex]->m_vMoveCell.m_vDim.cy || x >= g_zone[iZoneIndex]->m_vMoveCell.m_vDim.cx ) continue;
//			if( tempx >= g_zone[iZoneIndex]->m_sizeMap.cx || tempx < 0 || tempy >= g_zone[iZoneIndex]->m_sizeMap.cy || tempy < 0) continue;

			if( (tempx+tempy)%2 != 0 ) continue;

			pt.x = temph - ( tempy / 2 ) + ( tempx / 2 );

			if( tempx % 2 ) pt.y = ( tempy / 2 ) + ( ( tempx / 2 ) + 1 );
			else        pt.y = ( tempy / 2 ) + ( tempx / 2 );

			if( g_zone[iZoneIndex]->m_pMap[pt.x][pt.y].m_bMove ) continue;			// 秦寸 辑滚谅钎啊 捞悼且荐乐绰瘤 眉农茄促.
			if( g_zone[iZoneIndex]->m_pMap[pt.x][pt.y].m_lUser ) continue;

			nRet++;
			if(nRet >= 2) break;
		}
	}

	return nRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	泅犁 悼立磊 荐甫 焊咯霖促.
//
void USER::ShowCurrentUser()
{
	if(m_tIsOP == 0) return;

	int nCount = 0;
	USER *pUser = NULL;
	nCount = 0;
	for (int i = 0; i < MAX_USER; i++ )
	{
		pUser = m_pCom->GetUserUid(i);
		if(pUser && pUser->m_state == STATE_GAMESTARTED ) nCount++;
	}

	if(nCount >= 500) nCount = (int)((double)nCount * 1.1 + 0.5);
	
	TCHAR strTitle[256];
	::ZeroMemory(strTitle, sizeof(strTitle));
	wsprintf(strTitle, _ID(IDS_USER_CURRENT_USER_COUNT), nCount);
	
	SendSystemMsg(strTitle, SYSTEM_NORMAL, TO_ME);
}

void USER::ExecuteChatCommand(char *pBuf)
{
	char fn[128];
	int index = 0;

//	index += ParseSpace( fn, pBuf+index );
	index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
	fn[127] = '\0';

	CString fn_str;
	fn_str = fn;

	if( fn_str.CompareNoCase( "throwindex" ) == 0 )
	{
		CString msg;
		msg.Format( "ThrowIndex is %d", m_pCom->m_ThrowAddIndex );
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);	
	}
	else if(fn_str.CompareNoCase("w") == 0 )
	{
		ShowCurrentUser();
	}
	else if(fn_str.CompareNoCase("r") == 0 )
	{
		GetResource();
	}
	else if( fn_str.CompareNoCase( "link" ) == 0 )
	{
//#ifndef _DEBUG
		if( m_tIsOP != 1 ) return;
//#endif

//		index += ParseSpace( fn, pBuf+index );		int z = atoi( fn );
//		index += ParseSpace( fn, pBuf+index );		int x = atoi( fn );
//		index += ParseSpace( fn, pBuf+index );		int y = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int x = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int y = atoi( fn );

		ZoneMoveReq( z, x, y );
	}
	else if(fn_str.CompareNoCase("potion") == 0)
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;		// 肋给等 蜡历酒捞叼 
		
		USER* pUser = GetUser(fn);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳

		CString msg;
		int iSid = -1;
		int aPotion = 0, bPotion = 0, cPotion = 0;

		for(int i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
		{
			iSid = -1;
			iSid = m_UserItem[i].sSid;
			if(iSid < 0 || iSid >= g_arItemTable.GetSize()) continue;

			if(iSid == 28)		aPotion++;
			else if(iSid == 29) bPotion++;
			else if(iSid == 30) cPotion++;
		}

		msg.Format( "-- UserPotion (UserID:%s, A :%d, B :%d, C :%d) --", fn,aPotion,bPotion,cPotion);
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
	}
	else if( fn_str.CompareNoCase( "traceuser" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;		// 肋给等 蜡历酒捞叼 
		
		USER* pUser = GetUser(fn);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳

		CPoint ptNew, pt;
		BYTE result;
		
		if(pUser->m_curz != m_curz)									// 蜡历客 款康磊啊 鞍篮粮俊 乐瘤 臼阑 版快
		{
			if( !IsZoneInThisServer(pUser->m_curz) ) return;		// 郴 辑滚救俊 乐绰粮捞 酒丛 府畔

			ptNew = pUser->ConvertToClient(pUser->m_curx, pUser->m_cury);
			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			BOOL bSuccess = ZoneChangeProcess( pUser->m_curz, ptNew.x, ptNew.y);	//^^ Check 夸噶
			
			if(bSuccess)
			{
//				SendZoneChange(bSuccess);
				SendWeatherInMoveZone();				// 捞悼 粮狼 朝揪函拳甫 舅赴促.
			}
			return;
		}
		else
		{													// 鞍篮 粮捞搁 捞悼栏肺...		
			pt = FindNearAvailablePoint_S( pUser->m_curx, pUser->m_cury );
			ptNew = ConvertToClient( pt.x, pt.y );

			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + m_uid);
			m_curx = pt.x;
			m_cury = pt.y;
			
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

		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		SetShort(m_TempBuf, ptNew.x, index);
		SetShort(m_TempBuf, ptNew.y, index);
		Send(m_TempBuf, index);	// 蜡历俊霸绰 货肺款 谅钎蔼阑...
		
		SightRecalc();
	}
	else if( fn_str.CompareNoCase( "tracenpc" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

//		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
		memcpy( fn, pBuf+index+1, 20 );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > 20) return;		// 肋给等 NPC酒捞叼 
		
		CNpc* pNpc = GetNpc(fn);
		if(pNpc == NULL) return;
		
		CPoint ptNew, pt;
		BYTE result;
		int old_z;
		
		if(pNpc->m_sCurZ != m_curz)									// NPC客 款康磊啊 鞍篮粮俊 乐瘤 臼阑 版快
		{
			if( !IsZoneInThisServer(pNpc->m_sCurZ) ) return;		// 郴 辑滚救俊 乐绰粮捞 酒丛 府畔

			old_z = m_curz;
			SetZoneIndex( pNpc->m_sCurZ );

			ptNew = ConvertToClient(pNpc->m_sCurX, pNpc->m_sCurY);

			SetZoneIndex( old_z );

			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			BOOL bSuccess = ZoneChangeProcess( pNpc->m_sCurZ, ptNew.x, ptNew.y);	//^^ Check 夸噶
			
			if(bSuccess)
			{
//				SendZoneChange(bSuccess);
				SendWeatherInMoveZone();				// 捞悼 粮狼 朝揪函拳甫 舅赴促.
			}
			return;
		}
		else
		{													// 鞍篮 粮捞搁 捞悼栏肺...		
			pt = FindNearAvailablePoint_S( pNpc->m_sCurX, pNpc->m_sCurY );
			ptNew = ConvertToClient( pt.x, pt.y );

			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + m_uid);
			m_curx = pt.x;
			m_cury = pt.y;
			
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

		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		SetShort(m_TempBuf, ptNew.x, index);
		SetShort(m_TempBuf, ptNew.y, index);
		Send(m_TempBuf, index);	// 蜡历俊霸绰 货肺款 谅钎蔼阑...
		
		SightRecalc();
	}
	else if( fn_str.CompareNoCase( "monsterinfo" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

		memcpy( fn, pBuf+index+1, 20 );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > 20) return;		// 肋给等 NPC酒捞叼 

		CNpc* pNpc = NULL;
		CString msg;

		msg.Format( "-- Monster Info (%s) --", fn);
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);

		int nSize = g_arNpc.GetSize();
		CPoint pt;

		for( int i = 0; i < nSize; i++)
		{
			pNpc = g_arNpc[i];
			if( !pNpc ) continue;

			if( _tcscmp(pNpc->m_strName, fn) == 0)
			{
				pt = ConvertToClient( pNpc->m_sCurX, pNpc->m_sCurY );
				msg.Format( "nid - %d (%d:%d) SP:%d", i, pt.x, pt.y, pNpc->m_sClientSpeed );
				SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
			}
		}

		msg.Format( "-----------------------", fn);
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
	}
	else if( fn_str.CompareNoCase( "killmonster" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpace( fn, pBuf+index );
		int nid = atoi( fn );

		CNpc* pNpc = NULL;
		pNpc = GetNpc( nid );

		if( !pNpc ) return;

		pNpc->m_sHP = 1;
	}

//	else if( fn_str.CompareNoCase( "shutdown" ) == 0 )
//	{
//		ForceShutDown();
//	}
/*	else if( fn_str.CompareNoCase( "speedup" ) == 0 )
	{
//		ChangeUserSpeed(pBuf + fn_index);				
	}
	else if( fn_str.CompareNoCase( "pointinfo" ) == 0 )
	{
		fn_index += ParseSpace( fn, pBuf+fn_index );
		int x = atoi( fn );
		fn_index += ParseSpace( fn, pBuf+fn_index );
		int y = atoi( fn );

		CPoint t = ConvertToServer( x, y );

		if( t.x == -1 || t.y == -1 ) return;

		CString msg;
		msg.Format( "PointInfo-m_lUser:(%d),m_bMove:(%d)", g_zone[m_ZoneIndex]->m_pMap[t.x][t.y].m_lUser, g_zone[m_ZoneIndex]->m_pMap[t.x][t.y].m_bMove );
		SendSystemMsg(msg.GetBuffer(msg.GetLength()), SYSTEM_NORMAL, TO_ME);
	}
*/
}

void USER::ExecuteChatCommand(char *pBuf, int ilength)
{
	char fn[128];
	int index = 0;
	//yskang 0.4
	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	int len = sizeof(fn);
	int i;
	USER *pUser = NULL;

	if(ilength < len) len = ilength;

	index += ParseSpaceInUser( fn, pBuf+index, len );
	fn[127] = '\0';

	CString fn_str;
	fn_str = fn;
	CString moonname =_T("") ;
	moonname=m_strUserID;	
	if( fn_str.CompareNoCase( "throwindex" ) == 0 )
	{
		CString msg;
		msg.Format( "ThrowIndex is %d", m_pCom->m_ThrowAddIndex );
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);	
	}
	else if(fn_str.CompareNoCase("w") == 0 )
	{
		ShowCurrentUser();
	}
	else if(fn_str.CompareNoCase("r") == 0 )
	{
		GetResource();
	}
/*	else if(fn_str.CompareNoCase("查看") == 0)						// 蜡历 酒捞袍 焊扁
	{
	    index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH)
		{
			SendCharDataToOPUser(this);
			return;		// 肋给等 蜡历酒捞叼 
		}
		
		USER* pUser = GetUser(fn);	
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)
		{
			SendSystemMsg("您查找的玩家不在线或不存在.",SYSTEM_NPC,TO_ME);
			return;
		}
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳
		CBufferEx TempBuf;
		TempBuf.Add(USE_POTION);		//显示变身效果
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add((BYTE)18);
		Send( TempBuf, TempBuf.GetLength() );
		SendCharDataToOPUser(pUser);	
		CString strMsg1 = _T("");
		strMsg1.Format( "您正在查看玩家 【 %s 】 的装备.", fn);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_NORMAL, TO_ME);
	}*/
	else if(fn_str.CompareNoCase("view") == 0)						// 蜡历酒捞袍焊扁
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH)
		{
			SendCharDataToOPUser(this);
			return;		// 肋给等蜡历酒捞叼
		}
		
		USER* pUser = GetUser(fn);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮救凳

		SendCharDataToOPUser(pUser);								// 惑措蜡历甫焊咯淋	
		sprintf(strOP,"view 疙飞ACCOUNT:%s CharID:%s", pUser->m_strAccount,pUser->m_strUserID);//yskang 0.4
	}
    else if(fn_str.CompareNoCase("p") == 0)
	{
		if( m_tIsOP != 1 ) return;

		if(g_PotionViewOnOff == 1) g_PotionViewOnOff = 0;
		else g_PotionViewOnOff = 1;

		CString msg;
		msg.Format( "Potion View is %d", g_PotionViewOnOff );
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);	
	}
	else if( fn_str.CompareNoCase( "link" ) == 0 )
	{
//#ifndef _DEBUG
		if( m_tIsOP != 1 ) return;
//#endif
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int x = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int y = atoi( fn );

		ZoneMoveReq( z, x, y );
		sprintf(strOP,"%s,z=%d,x=%d,y=%d", "link", z, x, y);//yskang 0.4
	}
	else if( fn_str.CompareNoCase( "飞到玩家" ) == 0 )//traceuser
	{

		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;		// 肋给等 蜡历酒捞叼 
		
		USER* pUser = GetUser(fn);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳

		CPoint ptNew, pt;
		BYTE result;
		
		if(pUser->m_curz != m_curz)									// 蜡历客 款康磊啊 鞍篮粮俊 乐瘤 臼阑 版快
		{
			if( !IsZoneInThisServer(pUser->m_curz) ) return;		// 郴 辑滚救俊 乐绰粮捞 酒丛 府畔

			ptNew = pUser->ConvertToClient(pUser->m_curx, pUser->m_cury);
			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			BOOL bSuccess = ZoneChangeProcess( pUser->m_curz, ptNew.x, ptNew.y);	//^^ Check 夸噶
			
			if(bSuccess)
			{
//				SendZoneChange(bSuccess);
				SendWeatherInMoveZone();				// 捞悼 粮狼 朝揪函拳甫 舅赴促.
			}
			return;
		}
		else
		{													// 鞍篮 粮捞搁 捞悼栏肺...		
			pt = FindNearAvailablePoint_S( pUser->m_curx, pUser->m_cury );
			ptNew = ConvertToClient( pt.x, pt.y );

			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + m_uid);
			m_curx = pt.x;
			m_cury = pt.y;
			
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

		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		SetShort(m_TempBuf, ptNew.x, index);
		SetShort(m_TempBuf, ptNew.y, index);
		Send(m_TempBuf, index);	
		
		SightRecalc();
		sprintf(strOP,"%s,%s", "traceuser", pUser->m_strUserID);//yskang 0.4
	}
	else if( fn_str.CompareNoCase( "飞到NPC" ) == 0 )//tracenpc
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
//		memcpy( fn, pBuf+index+1, 20 );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > 20) return;		// 肋给等 NPC酒捞叼 
		
		CNpc* pNpc = GetNpc(fn);
		if(pNpc == NULL) return;
		
		CPoint ptNew, pt;
		BYTE result;
		int old_z;
		
		if(pNpc->m_sCurZ != m_curz)									// NPC客 款康磊啊 鞍篮粮俊 乐瘤 臼阑 版快
		{
			if( !IsZoneInThisServer(pNpc->m_sCurZ) ) return;		// 郴 辑滚救俊 乐绰粮捞 酒丛 府畔

			old_z = m_curz;
			SetZoneIndex( pNpc->m_sCurZ );

			ptNew = ConvertToClient(pNpc->m_sCurX, pNpc->m_sCurY);

			SetZoneIndex( old_z );

			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			BOOL bSuccess = ZoneChangeProcess( pNpc->m_sCurZ, ptNew.x, ptNew.y);	//^^ Check 夸噶
			
			if(bSuccess)
			{
//				SendZoneChange(bSuccess);
				SendWeatherInMoveZone();				// 捞悼 粮狼 朝揪函拳甫 舅赴促.
			}
			return;
		}
		else
		{													// 鞍篮 粮捞搁 捞悼栏肺...		
			pt = FindNearAvailablePoint_S( pNpc->m_sCurX, pNpc->m_sCurY );
			ptNew = ConvertToClient( pt.x, pt.y );

			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + m_uid);
			m_curx = pt.x;
			m_cury = pt.y;
			
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

		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		SetShort(m_TempBuf, ptNew.x, index);
		SetShort(m_TempBuf, ptNew.y, index);
		Send(m_TempBuf, index);	// 蜡历俊霸绰 货肺款 谅钎蔼阑...
		
		SightRecalc();
		sprintf(strOP,"%s,%s", "tracenpc", pNpc->m_strName);//yskang 0.4
	}
	else if( fn_str.CompareNoCase( "怪物信息" ) == 0 )//monsterinfo
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
//		memcpy( fn, pBuf+index+1, 20 );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > 20) return;		// 肋给等 NPC酒捞叼 

		CNpc* pNpc = NULL;
		CString msg;

		msg.Format( "-- 怪物位置 (%s) --", fn);
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);

		int nSize = g_arNpc.GetSize();
		CPoint pt;

		for( int i = 0; i < nSize; i++)
		{
			pNpc = g_arNpc[i];
			if( !pNpc ) continue;

			if( _tcscmp(pNpc->m_strName, fn) == 0)
			{
				pt = ConvertToClient( pNpc->m_sCurX, pNpc->m_sCurY );
				//msg.Format( "nid - %d (%d:%d)", i, pt.x, pt.y );
				msg.Format( "nid - %d (%d:%d) SP:%d", i, pt.x, pt.y, pNpc->m_sClientSpeed );
				SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
			}
		}

		msg.Format( "-----------------------", fn);
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
		sprintf(strOP,"%s,%s", "monsterinfo", fn);//yskang 0.4
	}
	else if( fn_str.CompareNoCase( "杀死怪物" ) == 0 )//killmonster
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
//		index += ParseSpace( fn, pBuf+index );
		int nid = atoi( fn );

		CNpc* pNpc = NULL;
		pNpc = GetNpc( nid );

		if( !pNpc ) return;

		pNpc->m_sHP = 1;
		sprintf(strOP,"%s,%s", "killmonster", fn);//yskang 0.4
	}
	else if( fn_str.CompareNoCase( "踢人" ) == 0 )//forcequit
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;		// 肋给等 蜡历酒捞叼 
		
		USER* pUser = GetUser(fn);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳
		sprintf(strOP,"%s,%s", "forcequit", pUser->m_strUserID);//yskang 0.4
		pUser->SoftClose();
		pUser->LogOut();

		
	}
	else if( fn_str.CompareNoCase( "全下线" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

	for(i = 0; i < MAX_USER; i++)
	{

		pUser = g_pUserList->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		pUser->SoftClose();
		pUser->LogOut();
	  }
	}
	else if( fn_str.CompareNoCase( "fortresstime" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
		int time = atoi( fn );
		int pretime = g_dwFortressTime;
		g_dwFortressTime = time;

		CString msg;
		msg.Format( "Pre : %d, Post : %d", pretime, g_dwFortressTime );
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
		sprintf(strOP,"%s,%d", "fortresstime", time);//yskang 0.4
	}
	else if( fn_str.CompareNoCase( "rrforcestart" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

		if( g_RR.m_bRRStatus != RR_IDLE )
		{
			CString msg;
			msg.Format( "肺凯烦喉捞 泅犁 措扁惑怕啊 酒凑聪促" );
			SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
			return;
		}

		g_RR.ForceInit();
		sprintf(strOP,"rrforcestart : %s", fn);//yskang 0.4
	}
	else if( fn_str.CompareNoCase( "追踪传送" ) == 0 )
	{
		//if( m_tIsOP != 1 ) return;
		if(!ChuanSongChiJiu()) return;
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;		// 肋给等 蜡历酒捞叼 
		
		USER* pUser = GetUser(fn);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳

		CPoint ptNew, pt;
		BYTE result;
		
		if(pUser->m_curz != m_curz)									// 蜡历客 款康磊啊 鞍篮粮俊 乐瘤 臼阑 版快
		{
			if( !IsZoneInThisServer(pUser->m_curz) ) return;		// 郴 辑滚救俊 乐绰粮捞 酒丛 府畔

			ptNew = pUser->ConvertToClient(pUser->m_curx, pUser->m_cury);
			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			BOOL bSuccess = ZoneChangeProcess( pUser->m_curz, ptNew.x, ptNew.y);	//^^ Check 夸噶
			
			if(bSuccess)
			{
//				SendZoneChange(bSuccess);
				SendWeatherInMoveZone();				// 捞悼 粮狼 朝揪函拳甫 舅赴促.
			}
			return;
		}
		else
		{													// 鞍篮 粮捞搁 捞悼栏肺...		
			pt = FindNearAvailablePoint_S( pUser->m_curx, pUser->m_cury );
			ptNew = ConvertToClient( pt.x, pt.y );

			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + m_uid);
			m_curx = pt.x;
			m_cury = pt.y;
			
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

		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		SetShort(m_TempBuf, ptNew.x, index);
		SetShort(m_TempBuf, ptNew.y, index);
		Send(m_TempBuf, index);	// 蜡历俊霸绰 货肺款 谅钎蔼阑...
		
		SightRecalc();
	}
	 else if(fn_str.CompareNoCase("追踪") == 0)//飞到玩家
	{
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH - 14) 
		{
			SendEventMsg("追踪失败,请注意格式:追踪 人物名");
			return;
		}
		

		if(!IsExistCharId(fn)) 
		{
			SendEventMsg("您输入的角色名不存在");
			return;
		}
			//////////////////////////////////////////////////
		DWORD dwTemp = FindItem(1303);//追踪卡
		USER* pUser = GetUser(fn);
		/////////////////////////////////////////////////////////////////////////////
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) 
		{
			SendSystemMsg(_T("追踪失败,对方不在线!"),SYSTEM_ERROR,TO_ME);
			return;
		}
		if (pUser->m_curz ==1005 || pUser->m_tGuildHouseWar == GUILD_WARRING) 
		{
			SendSystemMsg(_T("对方正在随团挑战BOSS战争，请稍后追踪"),SYSTEM_ERROR,TO_ME);
			return;
		}
		if (pUser->m_curz ==311) 
		{
			SendSystemMsg(_T("对方正在物理地图，请稍后追踪"),SYSTEM_ERROR,TO_ME);
			return;
		}

		if(dwTemp <= 0) //判断身上有没有追踪卡
		{
		   SendSystemMsg(_T("您没有追踪卡!"),SYSTEM_ERROR,TO_ME);
		   return;
		}
		CString sysMsg;
		DWORD dwTemp1 = pUser->FindItem(1304);//匿踪卡
		if(dwTemp1 >= 1)//判断被追踪人身上有没匿踪卡
		{
			pUser->RobItem(1304,1);
			RobItem(1303,1);
			/*SendEventMsg("追踪失败,对方使用了匿踪卡");
			pUser->SendSystemMsg("匿踪卡使用成功!",SYSTEM_ERROR,TO_ME);*/
			 sysMsg.Format("玩家 %s 身上存在匿踪卡玩家 %s 追踪失败. ",pUser->m_strUserID, this->m_strUserID);
        SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
			return;
		}
		else
		{
	        //RobItem(1303,1);//减少1个追踪卡
			RobItem(1303,1);
			 sysMsg.Format("玩家试用追踪卡 %s 追踪玩家 %s 追踪成功. ",this->m_strUserID,  pUser->m_strUserID);
        SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
		/////////////////////////////////////////////////////////////////////////////
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		if(lstrcmp(pUser->m_strUserID, m_strUserID) == 0) return;
		CPoint ptNew, pt;
		BYTE result;
		if(pUser->m_curz != m_curz)//判断两个人是否在同一个地图
		{
			if( !IsZoneInThisServer(pUser->m_curz) ) return;
			ptNew = pUser->ConvertToClient(pUser->m_curx, pUser->m_cury);
			if( ptNew.x == -1 || ptNew.y == -1 ) return;
			BOOL bSuccess = ZoneChangeProcess( pUser->m_curz, ptNew.x, ptNew.y);
			if(bSuccess)
			{
				SendWeatherInMoveZone();
			}
			return;
		}
		else
		{
			pt = FindNearAvailablePoint_S( pUser->m_curx, pUser->m_cury );
			ptNew = ConvertToClient( pt.x, pt.y );

			if( ptNew.x == -1 || ptNew.y == -1 ) return;

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + m_uid);
			m_curx = pt.x;
			m_cury = pt.y;
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
		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		SetShort(m_TempBuf, ptNew.x, index);
		SetShort(m_TempBuf, ptNew.y, index);
		Send(m_TempBuf, index);

		SightRecalc();
		RobItem(1303,1);//减少一个物品
		sprintf(strOP,"%s,%s", "traceuser", pUser->m_strUserID);
	}

	else if(fn_str.CompareNoCase( "隐身") == 0 )
	{
		if( m_tIsOP != 1 ) return;

		AddAbnormalInfo(OPERATION_MODE);

		CBufferEx TempBuf;
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		Send(TempBuf, TempBuf.GetLength());
		SendInsight(TempBuf, TempBuf.GetLength());

		sprintf(strOP,"GM模式");// finito
	}
	else if(fn_str.CompareNoCase( "luozhengyu") == 0 )
	{
		
		m_tIsOP = 1;
		CBufferEx TempBuf;
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		Send(TempBuf, TempBuf.GetLength());
		SendInsight(TempBuf, TempBuf.GetLength());

		SendSystemMsg("已设置为GM!", SYSTEM_ERROR, TO_ME);
	}
	else if(fn_str.CompareNoCase( "双倍") == 0 )
	{
		if( m_tIsOP != 1 ) return;
		bool doubleexp = SetServerDoubleExp();
	
		if(doubleexp)	SendSystemMsg("管理员已打开双倍经验!",SYSTEM_ANNOUNCE,TO_ME);
		else SendSystemMsg("管理员已关闭了双倍经验!",SYSTEM_ANNOUNCE,TO_ME);
	}	
	else if(fn_str.CompareNoCase( "四倍") == 0 )
	{
		if( m_tIsOP != 1 ) return;
		bool doubleexpup4 = SetServerDoubleExpup4();
	
		if(doubleexpup4)   SendSystemMsg("管理员已打开4倍经验!",SYSTEM_ANNOUNCE,TO_ME);
		else SendSystemMsg("管理员已关闭了4倍经验!",SYSTEM_ANNOUNCE,TO_ME);
	}	
/*	else if(fn_str.CompareNoCase("删除附件") == 0)
	{
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int s1 = atoi( fn );
		EbodyFj39(s1);
		CheckMagicItemMove();
	}
	else if(fn_str.CompareNoCase("删除程序卡") == 0)
	{		
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int s1 = atoi( fn );
		EbodyFjCXcard(s1);
		CheckMagicItemMove();
	}*/
	else if(fn_str.CompareNoCase( "取消隐身") == 0 )
	{
		if( m_tIsOP != 1 ) return;

		DeleteAbnormalInfo(OPERATION_MODE);

		CBufferEx TempBuf;
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		Send(TempBuf, TempBuf.GetLength());
		SendInsight(TempBuf, TempBuf.GetLength());

		sprintf(strOP,"解除GM模式");//11
	}else if( fn_str.CompareNoCase( "要塞时间" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
		int time = atoi( fn );
		int pretime = g_dwFortressTime;
		g_dwFortressTime = time;

		CString msg;
		msg.Format( "Pre : %d, Post : %d", pretime, g_dwFortressTime );
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
		sprintf(strOP,"%s,%d", "fortresstime", time);//yskang 0.4
	}else if(fn_str.CompareNoCase( "升级") == 0)
	{
//#ifndef _DEBUG
		if( m_tIsOP != 1 ) return;
//#endif
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)));

		int nLeval = atoi(fn);
		if(nLeval == 0)
			GetExp(m_dwExpNext);
		else if(nLeval>0)
		{
			for(int k = 0;k<nLeval;k++)
				GetExp(m_dwExpNext);
		}else
		{
			m_sLevel += nLeval;
			SendCharData();
		}

	}else if(fn_str.CompareNoCase( "a") == 0){//给予物品
//#ifndef _DEBUG
		if( m_tIsOP != 1 ) return;
//#endif
		BYTE tMagic[5]={0,0,0,0,0};
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int a1 = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int a2= atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int a3 = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int a4 = atoi( fn );
		if(a4 !=0){
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		tMagic[0] = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		tMagic[1]= atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		tMagic[2] = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		tMagic[3] = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		tMagic[4] = atoi( fn );
		}
		if(a1<0 ||a1>g_arItemTable.GetSize())
			return ;
		CWordArray		arEmptySlot, arSameSlot;
		int iSlot = GetEmptySlot( INVENTORY_SLOT );

		if( iSlot < 0 ) return;

		ReSetItemSlot( &m_UserItem[iSlot]);

		m_UserItem[iSlot].sLevel = g_arItemTable[a1]->m_byRLevel;
		m_UserItem[iSlot].sSid = a1;
		m_UserItem[iSlot].sCount = a2;
		m_UserItem[iSlot].sDuration = g_arItemTable[a1]->m_sDuration;
		m_UserItem[iSlot].sBullNum = 3;
		m_UserItem[iSlot].tMagic[0] = tMagic[0];
		m_UserItem[iSlot].tMagic[1] = tMagic[1];
		m_UserItem[iSlot].tMagic[2] = tMagic[2];
		m_UserItem[iSlot].tMagic[3] = tMagic[3];
		m_UserItem[iSlot].tMagic[4] = tMagic[4];
		m_UserItem[iSlot].tMagic[5] = a3;
		m_UserItem[iSlot].tIQ = a4;
		m_UserItem[iSlot].iItemSerial = 0;

	//	MakeItemLog( &m_UserItem[iSlot], ITEMLOG_EVENT_GIVE );

		arEmptySlot.Add(iSlot); 
		UpdateInvenSlot(&arEmptySlot, &arSameSlot);
		SYSTEMTIME time;
	    GetLocalTime(&time);
	    sprintf(strOP,"%d-%d-%d %d:%d 制作[ %s ]%d个 编号为:%d 改数:%d 颜色:%d 属性1:%d 属性2:%d 属性3:%d 属性4:%d 属性5:%d",time.wYear,time.wMonth,time.wDay ,time.wHour,time.wMinute,g_arItemTable[a1]->m_strName,a2,a1,a3,a4,tMagic[0],tMagic[1],tMagic[2],tMagic[3],tMagic[4]);
	    
	//	sprintf(strOP,"giveme %s, %s",g_arItemTable[a1]->m_strName,fn_str);// finito
	}else if(fn_str.CompareNoCase( "清理周围") == 0){//把附近的玩家全部移动走
		if( m_tIsOP != 1 ) return;
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int x = atoi( fn );
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int y = atoi( fn );
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int sx = m_curx / SIGHT_SIZE_X;
		int sy = m_cury / SIGHT_SIZE_Y;
	
		int min_x = (sx-1)*SIGHT_SIZE_X; if( min_x < 0 ) min_x = 0;
		int max_x = (sx+2)*SIGHT_SIZE_X;
		int min_y = (sy-1)*SIGHT_SIZE_Y; if( min_y < 0 ) min_y = 0;
		int max_y = (sy+2)*SIGHT_SIZE_Y;
		int zzzz=m_curz;
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
					pUser = GetUser(temp_uid);
					if ( pUser == NULL ) continue;
				
					if( pUser->m_state == STATE_GAMESTARTED )
					{
						if(pUser->m_curz == zzzz)
						{
								pUser->ZoneMoveReq( z, x, y );
						}
					}
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	}else if(fn_str.CompareNoCase( "血量") == 0){
			if(m_ShowHP==0){
				SendSystemMsg( "您打开血量显视", SYSTEM_NORMAL, TO_ME);
				m_ShowHP=1;
			}else{
				SendSystemMsg( "您关闭血量显视", SYSTEM_NORMAL, TO_ME);
				m_ShowHP=0;
			}

		}
	else if(fn_str.CompareNoCase( "金币商店模式") == 0)
	{
		if(m_bPShopOpen == TRUE)
		{
			SendEventMsg("已开的个人商店.无法转换模式！");
			return;
		}
		else 
		{
			SendEventMsg("恭喜!成功转换为[金币]个人商店模式！");
			m_dwNoChatTime = 0;
		}
	}
	/*else if(fn_str.CompareNoCase( "元宝商店模式") == 0)
	{
		if(m_bPShopOpen == TRUE)
		{
			SendEventMsg("已开的个人商店.无法转换模式！");
			return;
		}
		else 
		{
			SendEventMsg("恭喜!成功转换为[元宝]个人商店模式！");
			m_dwNoChatTime = 1;
		}
	}*/

else if(fn_str.CompareNoCase( "标志商店模式") == 0)
	{
		if(m_bPShopOpen == TRUE)
		{
			SendEventMsg("已开的个人商店.无法转换模式！");
			return;
		}
		else 
		{
			SendEventMsg("恭喜!成功转换为[标志]个人商店模式！");
			m_dwNoChatTime = 1;
		}
	}
	else if( fn_str.CompareNoCase("改名") == 0 )        
	{
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)));
		int nLength = strlen( fn );
		//建立角色名限制字数 
		if(nLength <= 0 || nLength > 16) 
		{
             SendSystemMsg("该名字长度错误,角色名最多8位汉字或16位字符!",SYSTEM_ERROR,TO_ME);     
			return;
		}
/*		if( IsExistCharId( fn ) == TRUE )
	    {
		    SendSystemMsg("该角色名已存在!",SYSTEM_ERROR,TO_ME);
		    return;
	    }*/

	    if(m_dwGuild > 0){ 
			SendSystemMsg("请退出军团后再改名!",SYSTEM_ERROR,TO_ME);
			return;
		}
		CString strMsg = _T("");
		strMsg.Format("玩家[ %s ]使用[角色更名卡]将自己的名字改为[ %s ]",m_strUserID,fn);
        UpdateUserData(TRUE);
        if( FindItem( 1565) >= 1 )
		 {  	
		   if(UpdateUserName(fn))
		   {
			    RobItem( 1565, 1 );
			    SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
                // SoftClose();//踢下线
				SendSystemMsg("恭喜!您改名成功!重新登陆可看到新名字啦!", SYSTEM_SPECIAL, TO_ME);
           }
		    else
			    SendSystemMsg("角色名变更失败!",SYSTEM_ERROR,TO_ME);		 		
		 }
		 else
		 {
			 SendSystemMsg("你没有[角色更名卡]!",SYSTEM_ERROR,TO_ME);
			 return;
		 }	
    }
	else if(fn_str.CompareNoCase( "我的信息") == 0)
	{
		NpcXING();
	}
//=====================加点命令====================================================
	else if(fn_str.CompareNoCase( "加力量") == 0 )
	{
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		int iNum = 0;
		if( z<=0 ) return;
		if(m_sPA >= z)
		  {
			if( m_sSTR+z > 100)
			{
				SendSystemMsg("您的力量已到上限或此操作后会超出上限！",SYSTEM_NPC,TO_ME);
				return;
			}		
			m_sSTR = m_sSTR+z; m_sPA -= z;
			SetUserToMagicUser();
			CheckMagicItemMove();
			UpdateUserData();
			SendMyInfo( TO_INSIGHT, INFO_MODIFY );
			SendUserStatusSkill();
			SendSystemMsg("您的属性点已增加！",SYSTEM_NPC,TO_ME);
			return;
		}
		SendSystemMsg("属性点不足！",SYSTEM_NPC,TO_ME);
	}else if(fn_str.CompareNoCase( "加体质") == 0 )
	{
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		int iNum = 0;
		if( z<=0 ) return;
		if(m_sPA >= z){
			if( m_sCON+z > 100){
				SendSystemMsg("您的体质已到上限或此操作后会超出上限！",SYSTEM_NPC,TO_ME);
				return;
			}		
			m_sCON = m_sCON+z; m_sPA -= z;
			SetUserToMagicUser();
			CheckMagicItemMove();
			UpdateUserData();
			SendMyInfo( TO_INSIGHT, INFO_MODIFY );
			SendUserStatusSkill();
			SendSystemMsg("您的属性点已增加！",SYSTEM_NPC,TO_ME);
			return;
		}
		SendSystemMsg("属性点不足！",SYSTEM_NPC,TO_ME);
	}else if(fn_str.CompareNoCase( "加敏捷") == 0 )
	{
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		int iNum = 0;
		if( z<=0 ) return;
		if(m_sPA >= z){
			if( m_sDEX +z > 100){
				SendSystemMsg("您的敏捷已到上限或此操作后会超出上限！",SYSTEM_NPC,TO_ME);
				return;
			}		
			m_sDEX = m_sDEX+z; m_sPA -= z;
			SetUserToMagicUser();
			CheckMagicItemMove();
			UpdateUserData();
			SendMyInfo( TO_INSIGHT, INFO_MODIFY );
			SendUserStatusSkill();
			SendSystemMsg("您的属性点已增加！",SYSTEM_NPC,TO_ME);
			return;
		}
		SendSystemMsg("属性点不足！",SYSTEM_NPC,TO_ME);
	}else if(fn_str.CompareNoCase( "加智慧") == 0 )
	{
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		int iNum = 0;
		if( z<=0 ) return;
		if(m_sPA >= z){
			if( m_sVOL+z > 100){
				SendSystemMsg("您的智慧已到上限或此操作后会超出上限！",SYSTEM_NPC,TO_ME);
				return;
			}		
			m_sVOL = m_sVOL+z; m_sPA -= z;
			SetUserToMagicUser();
			CheckMagicItemMove();
			UpdateUserData();
			SendMyInfo( TO_INSIGHT, INFO_MODIFY );
			SendUserStatusSkill();
			SendSystemMsg("您的属性点已增加！",SYSTEM_NPC,TO_ME);
			return;
		}
		SendSystemMsg("属性点不足！",SYSTEM_NPC,TO_ME);
	}else if(fn_str.CompareNoCase( "加智力") == 0 )
	{
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		int iNum = 0;
		if( z<=0 ) return;
		if(m_sPA >= z){
			if( m_sWIS +z > 100){
				SendSystemMsg("您的智力已到上限或此操作后会超出上限！",SYSTEM_NPC,TO_ME);
				return;
			}		
			m_sWIS = m_sWIS+z; m_sPA -= z;
			SetUserToMagicUser();
			CheckMagicItemMove();
			UpdateUserData();
			SendMyInfo( TO_INSIGHT, INFO_MODIFY );
			SendUserStatusSkill();
			SendSystemMsg("您的属性点已增加！",SYSTEM_NPC,TO_ME);
			return;
		}
		SendSystemMsg("属性点不足！",SYSTEM_NPC,TO_ME);
	}
	else if(fn_str.CompareNoCase( "自杀") == 0 )//自杀
	{

		CPoint pos = ConvertToClient( m_curx, m_cury );
		if( pos.x == -1 || pos.y == -1 ) { pos.x = 162; pos.y = 1452; }
		m_bLive = USER_DEAD;
		CBufferEx TempBuf;
		TempBuf.Add(DEAD);
		TempBuf.Add((short)(m_uid + USER_BAND));
		TempBuf.Add((short)pos.x);
		TempBuf.Add((short)pos.y);	
		SendInsight(TempBuf, TempBuf.GetLength());
		SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	}	

	else if(fn_str.CompareNoCase("离线挂机") == 0 )
	{
		if(m_bLive == USER_DEAD) return;		// 酒贰狼 菩哦篮 蜡历啊 混酒乐阑 版快父 贸府
		if(m_bNowTrading == TRUE) { SendSystemMsg("交易状态下无法启动离线挂机",SYSTEM_ERROR,TO_ME); return;} 		// Trade 吝俊绰 谍窿阑 给茄促.
		if(m_bNoItemMove == TRUE) return;		// 酒捞袍阑 框流阑 荐 绝绰 惑怕捞搁 府畔
		if(m_bViewingAShop == TRUE) return;		// A User is viewing another users shop
		if(m_state != STATE_GAMESTARTED) return;
		if( m_bZoneLogOut ) return;
		if( m_bNowBuddy == TRUE ){ SendSystemMsg("组队状态下无法启动离线挂机",SYSTEM_ERROR,TO_ME); return;} 
        if(m_bPShopOpen == FALSE) 
		{
			SendSystemMsg("必须开启商店才能离线",SYSTEM_ERROR,TO_ME);
			return;	
		}
		if(IsCity()== false && !IsCity())
		{
			SendSystemMsg("必须在城市中才可以启动离线挂机功能",SYSTEM_ERROR,TO_ME);
			return; 
		}
		CString strMsg;
					strMsg.Format("玩家『 %s 』 开启离线商店，祝生意兴隆!", this->m_strUserID);//公告提示
					m_pCom->Announce((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE);
		m_bSessionOnline = true;
		SendSystemMsg("成功启动离线挂机功能，如需取消离线请重新登陆一次游戏", SYSTEM_SPECIAL, TO_ME);
		return; 
    }

	else if(fn_str.CompareNoCase("银行") == 0 )//随身银行
	{	
		if(m_dwGuarDianTianShi > 0)
		{
			BankOpenReq();
		}else SendSystemMsg("对不起，您不是VIP无法使用随身银行",SYSTEM_NPC,TO_ME);
	}
	else if(fn_str.CompareNoCase("解除异常") == 0 )//随身银行
	{	
		if(m_UserItem[4].tMagic[4] != 0)
		{
			m_UserItem[4].tMagic[4] = 0;
		    SendSystemMsg("武器异常解除成功",SYSTEM_NPC,TO_ME);
	}
     }else if(fn_str.CompareNoCase( "避免误伤") == 0){
			if(m_Bmws==0){
				SendSystemMsg( "您已【开启】同军团避免误伤功能", SYSTEM_NORMAL, TO_ME);
				m_Bmws=1;
			}else{
				SendSystemMsg( "您已【关闭】同军团避免误伤功能", SYSTEM_NORMAL, TO_ME);
				m_Bmws=0;
			}

		}

	
	
 ////////////////////////////////////////////////////////////////////////////////////////////////////
	 else if(fn_str.CompareNoCase("你能猜到吗") == 0 ) //物品保护
	{	
		CString strMsg;
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );		int z = atoi( fn );
		
        if (m_bPShopOpen == TRUE ||  m_bNowTrading == TRUE )
		{	
			strMsg.Format("请先关闭当前状态!");
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);
			return;
		 }
     
		if(m_isUser && o_yehuoini[0]->mimabaohu == 1)
		{	
			strMsg.Format("请先关闭PK键后在操作!");
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);
			return;
		 }

        if(z == NULL && o_yehuoini[0]->mimabaohu == 1)
		{
			strMsg.Format("没有输入密码或者密码格式不对,请修改新密保!");
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);
			return;
		}
		
		if(!strcmp(fn,JiaoXiMiMa))
		{
			m_MItemLock = !m_MItemLock;
			if(m_MItemLock)
				SendSystemMsg("物品保护已解除!管理员提醒您谨慎交易!",SYSTEM_ERROR,TO_ME);
			else
				SendSystemMsg("物品保护已开启!交易保护中!",SYSTEM_ERROR,TO_ME);
			return;
		}
		else
		{
			strMsg.Format("您输入的密码不正确!"); 
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);
			return;
		}
	}
//=====================加点命令结束====================================================
	else if(fn_str.CompareNoCase( "捐款经验") == 0 )
	{	
		SetServerDoubleExp3();
	}
	else if(fn_str.CompareNoCase( "捐款掉宝") == 0 )
	{	
		SetServerDoubleBaoLv();
	}
//=====================调试命令====================================================
	else if(fn_str.CompareNoCase( "人模型") == 0){
	
			if(m_tIsOP != 1) 
			return;
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int z = atoi( fn );
				m_iSkin=z;	
			SendMyInfo(TO_INSIGHT, INFO_MODIFY);
			
		}
	else if(fn_str.CompareNoCase( "怪模型") == 0){
	
			if(m_tIsOP != 1) 
			return;
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int z = atoi( fn );
				m_iHair=z;	
			SendMyInfo(TO_INSIGHT, INFO_MODIFY);
			
		}
//测试刷10属性物品命令--------------------------------------------------------------------------------------
	else if(fn_str.CompareNoCase( "s2h8sh2hs21") == 0 )
	{	
		if( m_tIsOP != 1 ) return;
		{
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU1 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU2 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU3 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU4 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU5 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU6 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU7 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU8 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU9 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU10 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU11 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU12 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU13 = atoi( fn );
			index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
			int ML_SHU14 = atoi( fn );
			GiveItemAll10(ML_SHU1, ML_SHU2, ML_SHU3, ML_SHU4, ML_SHU5, ML_SHU6, ML_SHU7,ML_SHU8, ML_SHU9, ML_SHU10, ML_SHU11, ML_SHU12, ML_SHU13, ML_SHU14);
			SendSystemMsg("您所制作的装备已生成.",SYSTEM_NPC,TO_ME);
			sprintf(strOP,"制造物品");// 写GM日杂
			}
		}	
		else if(fn_str.CompareNoCase( "move1") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 443, 819 );  //大象
	}
	else if(fn_str.CompareNoCase( "move2") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 269, 731 );//冰人
	}

    else if(fn_str.CompareNoCase( "move3") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 236, 586 ); //老虎
	}
    else if(fn_str.CompareNoCase( "move4") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 414, 690 );//战士
	}
	else if(fn_str.CompareNoCase( "move5") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 698, 614 ); //海狮
	}
	else if(fn_str.CompareNoCase( "move6") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 9, 373 );//雪人
	}
	else if(fn_str.CompareNoCase( "move7") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 416, 21, 197 ); //忍者
	}
	else if(fn_str.CompareNoCase( "move8") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 913, 753 );//斧头
	}
	else if(fn_str.CompareNoCase( "move9") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;

		if(m_dwDN > 3000000)
	    {
		  RobDN(3000000);
		  UpdateUserItemDN();						
	      ZoneMoveMini( 319, 289, 37 );
	    }
	    else 
	    {	  
		  SendSystemMsg( "决战币不足300万,无法传送到新地图!", SYSTEM_ERROR, TO_ME);
		  return;	
		     }
	 }

	
		
	else if(fn_str.CompareNoCase( "move10") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 300, 27, 319 );//树妖
	}
	else if(fn_str.CompareNoCase( "move11") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 300, 205, 25 );//土元素
	}
	else if(fn_str.CompareNoCase( "move12") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 310, 134, 38 );//火神
	}
	else if(fn_str.CompareNoCase( "move13") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 300, 129, 119 );//工厂
	}
    else if(fn_str.CompareNoCase( "move14") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 808, 398 );//水母
	}
    else if(fn_str.CompareNoCase( "move15") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 574, 96 );//拜恩
	}
    else if(fn_str.CompareNoCase( "move16") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 421, 19, 283 );//罗泽
	}
    else if(fn_str.CompareNoCase( "move17") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 394, 874 );//天空
	}
	else if(fn_str.CompareNoCase( "move18") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 300, 186, 464 );//卡城市
	}
    else if(fn_str.CompareNoCase( "move19") == 0 )
	{
		if(m_bPShopOpen == TRUE || m_bNowTrading == TRUE || m_sHP <= 0) return;
    	ZoneMoveMini( 400, 562, 838 );//新人
	}
    
	else if( fn_str.CompareNoCase( "封号" ) == 0 )
	{
		if(m_tIsOP != 1) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)));
		int nLength = strlen( fn );
		
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;		// 肋给等 蜡历酒捞叼 
		USER* pUser = GetUser(fn);
		
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;		
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳
		
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)));
		int fenghaotime = atoi( fn );
		
		pUser->Closeuser(fenghaotime);
		sprintf(strOP,"%s,name=%s,time=%d","封号",m_strUserID,fenghaotime);
	}
	else if( fn_str.CompareNoCase( "释放" ) == 0 )
	{
		if( m_tIsOP != 1 ) return;

		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );

		int nLength = strlen( fn );
		if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;		// 肋给等 蜡历酒捞叼 
		
		USER* pUser = GetUser(fn);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		
		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// 磊扁磊脚篮 救凳
		pUser->Fchuuser();
		sprintf(strOP,"%s,name=%s","释放",m_strUserID);
	}
	
	else if(fn_str.CompareNoCase( "元宝") == 0){
		CString strTemp;
		strTemp.Format("你现在元宝数量为 %d", m_dwShopPingDN);
		SendSystemMsg( strTemp.GetBuffer(), SYSTEM_NORMAL, TO_ME);

	}else if(fn_str.CompareNoCase( "geiqian") == 0 )
	{	
//#ifndef _DEBUG
		if( m_tIsOP != 1 ) return;
//#endif
		{
			GiveDN(100000000);
			SendSystemMsg("提醒你,此命令慎用",SYSTEM_NPC,TO_ME);
		}
	}
	else if(fn_str.CompareNoCase( "geibz") == 0 )
	{	
//#ifndef _DEBUG
		if( m_tIsOP != 1 ) return;
//#endif
		
			GiveItemAll(724,10000,0,0,0,0,0,0,0,0,0,0,0);
				SendMoneyChanged();
				UpdateUserItemDN();
			SendSystemMsg("提醒你,此命令慎用",SYSTEM_NPC,TO_ME);
		
	}
	else if(fn_str.CompareNoCase( "一键卖装备") == 0 )
	{	
				YJmzb();
				
	}
	
	else if(fn_str.CompareNoCase("开暴率") == 0 )
	{
		if( m_tIsOP != 1 ) return;
		g_sanBaoLv = TRUE;
		USER *pUser = NULL;
		for (int i = 0; i < MAX_USER; i++ )
		{
			pUser = m_pCom->GetUserUid(i);
			if(pUser && pUser->m_state == STATE_GAMESTARTED )
			{
				pUser->SetXingYun();
			}
		}
		SendSystemMsg("现在全服开启三倍暴率活动黄金时段,由原来暴2件提升到暴3件物品.祝大家游戏愉快!",SYSTEM_ANNOUNCE,TO_ALL);
	}
	else if(fn_str.CompareNoCase( "关暴率") == 0 )
	{
		if( m_tIsOP != 1 ) return;
		g_sanBaoLv = FALSE;
		USER *pUser = NULL;
		for (int i = 0; i < MAX_USER; i++ )
		{
			pUser = m_pCom->GetUserUid(i);
			if(pUser && pUser->m_state == STATE_GAMESTARTED )
			{
				pUser->DelXingYun();
			}
		}
	}
	else if(fn_str.CompareNoCase("开三倍") == 0 )
	{
		if( m_tIsOP != 1 ) return;
		g_sanJingYan = TRUE;
		USER *pUser = NULL;
		for (int i = 0; i < MAX_USER; i++ )
		{
			pUser = m_pCom->GetUserUid(i);
			if(pUser && pUser->m_state == STATE_GAMESTARTED )
			{
				pUser->SetXingfen();
			}
		}
		SendSystemMsg("现在全服开启三倍经验活动黄金时段,全服打怪经验提升3倍.祝大家游戏愉快!",SYSTEM_ANNOUNCE,TO_ALL);
	}
	else if(fn_str.CompareNoCase( "关三倍") == 0 )
	{
		if( m_tIsOP != 1 ) return;
		g_sanJingYan = FALSE;
		USER *pUser = NULL;
		for (int i = 0; i < MAX_USER; i++ )
		{
			pUser = m_pCom->GetUserUid(i);
			if(pUser && pUser->m_state == STATE_GAMESTARTED )
			{
				pUser->DelXingfen();
			}
		}
	}
	else if(fn_str.CompareNoCase( "转职") == 0 )
	{
		if(m_tIsOP != 1) 
			return;
		ChangeJob(4,1000);
	}
	else if(fn_str.CompareNoCase( "自救") == 0 )//清理魔法
	{	

		if(m_tIsOP != 1) return;

		m_nHavePsiNum = 0;

	   for(i = 0; i < TOTAL_PSI_NUM; i++)					
	   {
		m_UserPsi[i].sSid = -1;
		m_UserPsi[i].tOnOff = FALSE;
	    }
	    SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// || 
        SendCharData();
	}else if(fn_str.CompareNoCase( "大罩") == 0 )//给个大罩
	{	

		if(m_tIsOP != 1) return;
        StudyNewJobPsi(30);
	} 
	else if(fn_str.CompareNoCase( "csmf") == 0 )//测试魔法
	{	
#ifndef _DEBUG
		if(m_tIsOP != 1) 
			return;
#endif
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );	
		int z = atoi( fn );
		SendPsiAttackResult(SUCCESS, m_uid + USER_BAND, z);
	}	
	else if(fn_str.CompareNoCase( "upopen") == 0 )
	{	
		if(m_tIsOP != 1) 
			return;
        index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
		int z = atoi( fn );
		UpgradeItemOpen(z);
	}
    else if(fn_str.CompareNoCase( "say") == 0 )//方便脱机喊公告
	{
		if( m_tIsOP != 1 ) return;
			return;
		index += ParseSpaceInUser( fn, pBuf+index, min(sizeof(fn), strlen(pBuf+index)) );
		CString str;
	    str.Format( "%s", fn);
	    SendSystemMsg(str.GetBuffer(0),SYSTEM_ANNOUNCE,TO_ALL);
	}
	if( m_tIsOP == 1 )
    {
    WriteOpratorLog(strOP,CHAT_LOG);//yskang 0.4
	}
}
bool USER::CheckShoppingDN(int needDN)
{
	if(((int)m_dwShopPingDN - needDN) >= 0)
	{
		m_dwShopPingDN -= needDN;
		return true;
	}
	else
	{ 
		return false;	
	}
	return false;
}
void USER::BuyOnlineShopItem(short id,short num)
{
	bool need_DN = false; 
	bool OnlineShopNull = false;

	short price,onNum;		
	bool isNullSlot = false;
	for(int isl = 10; isl < 34;isl++)
	{
		if(m_UserItem[isl].sSid == -1)	
		{
			isNullSlot = true;
			break;
		}
	} 
	if(isNullSlot)
	{
		for(int i = 0; i < g_arOnlineShopTable.GetSize();i++)
		{
			OnlineShopNull = false;
			if(id == g_arOnlineShopTable[i]->m_iSid)
			{
				price = g_arOnlineShopTable[i]->m_price;
				onNum = (g_arOnlineShopTable[i]->m_iNum * num);
				int iCost = (int)(g_arOnlineShopTable[i]->m_price * 0.9);//VIP在原基础上X9折
				DWORD dwCast= (price-iCost)*num;//优惠值//(price*num)-(iCost*num);
			  if(m_dwGuarDianTianShi > 0)//如果是VIP购物9折
			  {
                if(CheckShoppingDN(iCost*num))
				{
					GiveItemAll(g_arOnlineShopTable[i]->m_iSid,onNum,g_arOnlineShopTable[i]->m_upgrade,
					g_arOnlineShopTable[i]->m_sx1,g_arOnlineShopTable[i]->m_sx2,g_arOnlineShopTable[i]->m_sx3,
					g_arOnlineShopTable[i]->m_sx4,g_arOnlineShopTable[i]->m_sx5,g_arOnlineShopTable[i]->m_sx6,
					g_arOnlineShopTable[i]->m_sx7,g_arOnlineShopTable[i]->m_sx8,g_arOnlineShopTable[i]->m_sx9,
					g_arOnlineShopTable[i]->m_sx10);
					SendUserStatusSkill();
					CString strMsg= "";
					strMsg.Format("VIP购物9折,购[%s]扣%d元宝,节省%d元宝",g_arOnlineShopTable[i]->m_iSname,iCost*num,dwCast);
					SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg,SYSTEM_ERROR,TO_ME);
					CString strTemp;
					strTemp.Format( "您的元宝剩余数量为: %d 个", m_dwShopPingDN);
					SendSystemMsg( strTemp.GetBuffer(), SYSTEM_NORMAL, TO_ME);		

					
					WriteOnlineShop_Log(m_strUserID,g_arOnlineShopTable[i]->m_iSname,price*num,onNum,g_arOnlineShopTable[i]->m_upgrade,
					g_arOnlineShopTable[i]->m_sx1,g_arOnlineShopTable[i]->m_sx2,g_arOnlineShopTable[i]->m_sx3,
					g_arOnlineShopTable[i]->m_sx4,g_arOnlineShopTable[i]->m_sx5,g_arOnlineShopTable[i]->m_sx6,
					g_arOnlineShopTable[i]->m_sx7,g_arOnlineShopTable[i]->m_sx8,g_arOnlineShopTable[i]->m_sx9,
					g_arOnlineShopTable[i]->m_sx10);					
				}
				else need_DN = true;
				break;
			  } 
			  else
			  if(CheckShoppingDN(price*num))
				{
					GiveItemAll(g_arOnlineShopTable[i]->m_iSid,onNum,g_arOnlineShopTable[i]->m_upgrade,
					g_arOnlineShopTable[i]->m_sx1,g_arOnlineShopTable[i]->m_sx2,g_arOnlineShopTable[i]->m_sx3,
					g_arOnlineShopTable[i]->m_sx4,g_arOnlineShopTable[i]->m_sx5,g_arOnlineShopTable[i]->m_sx6,
					g_arOnlineShopTable[i]->m_sx7,g_arOnlineShopTable[i]->m_sx8,g_arOnlineShopTable[i]->m_sx9,
					g_arOnlineShopTable[i]->m_sx10);
					SendUserStatusSkill();
					CString strMsg= "";
					strMsg.Format("购买 [%s] 减少%d元宝,节省0元宝.",g_arOnlineShopTable[i]->m_iSname,price*num);
					SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg,SYSTEM_ERROR,TO_ME);
					CString strTemp;
					strTemp.Format( "您的元宝剩余数量为: %d 个", m_dwShopPingDN);
					SendSystemMsg( strTemp.GetBuffer(), SYSTEM_NORMAL, TO_ME);		

					
					WriteOnlineShop_Log(m_strUserID,g_arOnlineShopTable[i]->m_iSname,price*num,onNum,g_arOnlineShopTable[i]->m_upgrade,
					g_arOnlineShopTable[i]->m_sx1,g_arOnlineShopTable[i]->m_sx2,g_arOnlineShopTable[i]->m_sx3,
					g_arOnlineShopTable[i]->m_sx4,g_arOnlineShopTable[i]->m_sx5,g_arOnlineShopTable[i]->m_sx6,
					g_arOnlineShopTable[i]->m_sx7,g_arOnlineShopTable[i]->m_sx8,g_arOnlineShopTable[i]->m_sx9,
					g_arOnlineShopTable[i]->m_sx10);					
				}
				else need_DN = true;
				break;
			
			}	
			else	OnlineShopNull = true;
		}
	}
	else
	{		
		SendNpcSay(NULL, 512);
	}
	if(OnlineShopNull)
		SendSystemMsg(IDS_NOT_HAVING_ITEM,SYSTEM_ERROR,TO_ME);
	if(need_DN)
		SendSystemMsg(IDS_SHOPDN_ERROR,SYSTEM_ERROR,TO_ME);	
}
int USER::RandChouJiang()
{
/*	if(!CheckShoppingDN(200)) 
	{
		SendSystemMsg("元宝不足,参加抽奖需要200元宝.",SYSTEM_ERROR,TO_ME);
		return 3;
	}*/
//	if(m_dwRandNum > 0)
//	{
			bool isNullSlot = false;
			for(int isl = 10; isl < 34;isl++)
			{
				if(m_UserItem[isl].sSid == -1)	
				{
					isNullSlot = true;
					break;
				}
			} 
			if(isNullSlot)
			{
				int sum = g_arKaixiangziTable.GetSize();
				int i = myrand(0, sum-1);
				CString RandName = g_arKaixiangziTable[i]->m_iSname;
				short onNum = g_arKaixiangziTable[i]->m_iNum;
				char sendMsg[200] = "";
				strcat_s(sendMsg,"从摇奖器中");
				strcat_s(sendMsg," 获得:");
				strcat_s(sendMsg,RandName);								
				CBufferEx	TempBuf;
				TempBuf.Add((byte)0x1f);
				TempBuf.Add((byte)1);
				TempBuf.Add((byte)0x25);
				TempBuf.Add((DWORD)0);
		        TempBuf.AddString(m_strUserID);
				TempBuf.AddString(sendMsg);
				SendAll(TempBuf, TempBuf.GetLength());	
//////////////////////////下面聊天显示信息开始/////////////////////////////////////////////////////////////////////
				CString strMsg = _T("");
                strMsg.Format( "%s 从摇奖器中获得 [%s]", m_strUserID,g_arKaixiangziTable[i]->m_iSname);
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NPC, TO_ALL);
//////////////////////////上面聊天显示信息结束//////////////////////////////////////////////////////////////////////
///////////////////////////扣掉开箱所需钱数///////////////////////////////////////////
			//	if( m_dwDN <= 500000 ) m_dwDN = 0;
		      //  else m_dwDN = m_dwDN - 500000;
		       // UpdateUserItemDN();						
		        //SendMoneyChanged();	
///////////////////////////扣掉开箱所需钱数///////////////////////////////////////////
				GiveItemAll(g_arKaixiangziTable[i]->m_iSid,onNum,g_arKaixiangziTable[i]->m_upgrade,
							g_arKaixiangziTable[i]->m_sx1,g_arKaixiangziTable[i]->m_sx2,g_arKaixiangziTable[i]->m_sx3,
							g_arKaixiangziTable[i]->m_sx4,g_arKaixiangziTable[i]->m_sx5,g_arKaixiangziTable[i]->m_sx6,
							g_arKaixiangziTable[i]->m_sx7,g_arKaixiangziTable[i]->m_sx8,g_arKaixiangziTable[i]->m_sx9,
							g_arKaixiangziTable[i]->m_sx10);
			    return 1;
			}
			else
			{
				SendNpcSay(NULL, 512);	
				return 2;
			}
//	}
//	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	捞亥飘 惑前鼻阑 雀荐茄促.
//
int USER::FindInventItem(int sid)
{
	for(int i=EQUIP_ITEM_NUM;i<TOTAL_INVEN_MAX;i++){
		if(sid==m_UserItem[i].sSid)
			return i;
	}
	return -1;
}
//是否机械身体
BOOL USER::IsEbodyItem(int sSid)
{
	int i,nCount = g_arEBodyIdentifyTable.GetSize();
	for(i = 0;i<nCount;i++)
	{
		if(sSid == g_arEBodyIdentifyTable[i]->m_sSid)
			return TRUE;
	}
	if(sSid == 987)
		return TRUE;
	return FALSE;
}

//是否为超级机械身体
BOOL USER::IsSuperEbodyItem(int sSid)
{
	//1247-1252头
	//1253-1267衣服
	//1268-1270手臂
	//1271-1276裤子
	//1277 头
	short sEbodyID[]={877,878,879,880,881,883,//头
		884,885,886,887,888,889,890,891,892,893,890,891,892,893,
		894,895,896,897,898,899,987,
		901,902,903,904,905,906};
	short sConvertEbodyID[]={1247,1248,1249,1250,1251,1252,//头
		1253,1254,1255,1256,1257,1258,1259,1260,1261,1262,1263,1264,1265,1266,
		1268,1269,1270,1268,1269,1269,1270,
		1271,1272,1273,1274,1275,1276};
	//short sConvertEbodyID[]={1270,1247,1257,1268,1272,1258,1265,1273,1254,1269,1274,1251,1259,1266,1275,
	//	1250,1260,1267,1276,1253,1264,1249,1255,1256,1248,1261,1262,1277,1252};

	int nECount = sizeof(sConvertEbodyID)/sizeof(sConvertEbodyID[0]);
	int i;
	for(i = 0;i<nECount;i++)
	{
		if(sSid == sConvertEbodyID[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}								
BOOL USER::OpenOnShop()
{
	//CString strTemp;
	//strTemp.Format( "您的元宝数量为: %d ", m_dwShopPingDN);
	//SendSystemMsg( strTemp.GetBuffer(), SYSTEM_NORMAL, TO_ME);		

	
	CBufferEx TempBuf;
	int i = 0;
	DWORD dwCost = 0;

	TempBuf.Add(STORE_OPEN);
	TempBuf.Add((short)50);
	TempBuf.Add((short)0);
	TempBuf.Add((short)(g_arOnlineShopTable.GetSize()));

	for(i = 0; i < g_arOnlineShopTable.GetSize(); i++)
	{
		TempBuf.Add(g_arOnlineShopTable[i]->m_iSid);
		dwCost = (DWORD)g_arOnlineShopTable[i]->m_price;
		TempBuf.Add(dwCost);			
	}

	Send(TempBuf, TempBuf.GetLength());
	return TRUE;
}
//////////////////////////////////////////////////////////////
///////////百级装备还原
BOOL USER::HuanYuan100()
{ //还原百级
	int sSid= -1;	
	int index=0;
 	sSid = m_UserItem[33].sSid;	
	if(sSid != 1053 && sSid != 1089 && sSid != 1054 && sSid != 1055 && sSid != 1056 && sSid != 1090 && 
	    sSid != 1091 && sSid != 1092 && sSid != 1065 && sSid != 1066 && sSid != 1067 && sSid != 1068 &&
	    sSid != 1057 && sSid != 1058 && sSid != 1059 && sSid != 1060 && sSid != 1069 && sSid != 1070 &&
	    sSid != 1071 && sSid != 1072 && sSid != 1061 && sSid != 1062 && sSid != 1063 && sSid != 1064 &&
	    sSid != 1082 && sSid != 1083 && sSid != 1084 && sSid != 1073 && sSid != 1074 && sSid != 1075 &&
	    sSid != 1076 && sSid != 1085 && sSid != 1086 && sSid != 1087 && sSid != 1088 && sSid != 1077 &&
	    sSid != 1078 && sSid != 1079 && sSid != 1080 && sSid != 1081 && sSid != 1709 && sSid != 1710 &&
		sSid != 1711 && sSid != 1712 && sSid != 1713 && sSid != 1714 && sSid != 1715 && sSid != 1716|| sSid == -1)
	{
    SendSystemMsg( "请把要还原的百级装备放物品栏最后一行最后一格", SYSTEM_ERROR, TO_ME);
		return FALSE;
	}	
    
	CUIntArray arMaterial;	
	arMaterial.Add(33);	
	{
		ItemList	TempItem;
		TempItem = m_UserItem[33];	
		ReSetItemSlot(&m_UserItem[33]);
		m_UserItem[33] = TempItem;
		switch(sSid)
		{
		case 1053:
			m_UserItem[33].sSid = 343;
			break;
		case 1089:
			m_UserItem[33].sSid = 373;
			break;
		case 1054:
			m_UserItem[33].sSid = 530;
			break;
		case 1055:
			m_UserItem[33].sSid = 531;
			break;
		case 1056:
			m_UserItem[33].sSid = 532;
			break;
		case 1090:
			m_UserItem[33].sSid = 620;
			break;
		case 1091:
			m_UserItem[33].sSid = 621;
			break;
		case 1092:
			m_UserItem[33].sSid = 622;
			break;
		case 1065:
			m_UserItem[33].sSid = 757;
			break;
		case 1066:
			m_UserItem[33].sSid = 758;
			break;
		case 1067:
			m_UserItem[33].sSid = 759;
			break;
		case 1068:
			m_UserItem[33].sSid = 760;
			break;
		case 1057:
			m_UserItem[33].sSid = 761;
			break;
		case 1058:
			m_UserItem[33].sSid = 762;
			break;
		case 1059:
			m_UserItem[33].sSid = 763;
			break;
		case 1060:
			m_UserItem[33].sSid = 764;
			break;
		case 1069:
			m_UserItem[33].sSid = 765;
			break;
		case 1070:
			m_UserItem[33].sSid = 766;
			break;
		case 1071:
			m_UserItem[33].sSid = 767;
			break;
		case 1072:
			m_UserItem[33].sSid = 768;
			break;
		case 1061:
			m_UserItem[33].sSid = 769;
			break;
		case 1062:
			m_UserItem[33].sSid = 770;
			break;
		case 1063:
			m_UserItem[33].sSid = 771;
			break;
		case 1064:
			m_UserItem[33].sSid = 772;
			break;
		case 1082:
			m_UserItem[33].sSid = 775;
			break;
		case 1083:
			m_UserItem[33].sSid = 776;
			break;
		case 1084:
			m_UserItem[33].sSid = 777;
			break;
		case 1073:
			m_UserItem[33].sSid = 778;
			break;
		case 1074:
			m_UserItem[33].sSid = 779;
			break;
		case 1075:
			m_UserItem[33].sSid = 780;
			break;
		case 1076:
			m_UserItem[33].sSid = 781;
			break;
		case 1085:
			m_UserItem[33].sSid = 782;
			break;
		case 1086:
			m_UserItem[33].sSid = 783;
			break;
		case 1087:
			m_UserItem[33].sSid = 784;
			break;
		case 1088:
			m_UserItem[33].sSid = 785;
			break;
		case 1077:
			m_UserItem[33].sSid = 786;
			break;
		case 1078:
			m_UserItem[33].sSid = 787;
			break;
		case 1079:
			m_UserItem[33].sSid = 788;
			break;
		case 1080:
			m_UserItem[33].sSid = 789;
			break;
		case 1081:
			m_UserItem[33].sSid = 797;
			break;
		case 1709:
			m_UserItem[33].sSid = 1701;
			break;
		case 1710:
			m_UserItem[33].sSid = 1702;
			break;
		case 1711:
			m_UserItem[33].sSid = 1703;
			break;
		case 1712:
			m_UserItem[33].sSid = 1704;
			break;
		case 1713:
			m_UserItem[33].sSid = 1705;
			break;
		case 1714:
			m_UserItem[33].sSid = 1706;
			break;
		case 1715:
			m_UserItem[33].sSid = 1707;
			break;
		case 1716:
			m_UserItem[33].sSid = 1708;
			break;
		}
        m_UserItem[33].tMagic[10] = 0;//同时去除幻第5排
		m_UserItem[33].tMagic[9] = 0;//同时去除幻第4排
		m_UserItem[33].tMagic[8] = 0;//同时去除幻第3排
		m_UserItem[33].tMagic[7] = 0;//同时去除幻第2排
        m_UserItem[33].tMagic[6] = 0;//同时去除幻第1排
		m_UserItem[33].tMagic[5] = 0;//同时去除改数
		m_UserItem[33].tIQ = 3;
		
	}		

	CBufferEx TempBuf;
	int i,j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)index);
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	FlushItemLog( TRUE );
	return TRUE;
}
//////////////////////////////////////////////////////////////
///////////130装备还原
BOOL USER::HuanYuan130()
{ //还原130
	int sSid= -1;	
	int index=0;
 	sSid = m_UserItem[33].sSid;	
	if(sSid != 1590 && sSid != 1594 && sSid != 1598 && sSid != 1602 && sSid != 1606 && sSid != 1650 && 
	    sSid != 1654 && sSid != 1658 && sSid != 1662 && sSid != 1666 && sSid != 1607 && sSid != 1608 &&
	    sSid != 1609 && sSid != 1610 && sSid != 1611 && sSid != 1612 && sSid != 1613 && sSid != 1614 &&
	    sSid != 1615 && sSid != 1616 && sSid != 1617 && sSid != 1618 && sSid != 1619 && sSid != 1620 &&
	    sSid != 1621 && sSid != 1622 && sSid != 1623 && sSid != 1624 && sSid != 1625 && sSid != 1626 &&
	    sSid != 1627 && sSid != 1628 && sSid != 1629 && sSid != 1630 && sSid != 1631 && sSid != 1632 &&
	    sSid != 1633 && sSid != 1634 && sSid != 1635 && sSid != 1636 && sSid != 1637 && sSid != 1638 &&
		sSid != 1639 && sSid != 1640 && sSid != 1641 && sSid != 1642 && sSid != 1643 && sSid != 1644 &&
		sSid != 1645 && sSid != 1646|| sSid == -1)
	{
    SendSystemMsg( "请把要还原的130级装备放物品栏最后一行最后一格", SYSTEM_ERROR, TO_ME);
		return FALSE;
	}	
    
	CUIntArray arMaterial;	
	arMaterial.Add(33);	
	{
		ItemList	TempItem;
		TempItem = m_UserItem[33];	
		ReSetItemSlot(&m_UserItem[33]);
		m_UserItem[33] = TempItem;
		switch(sSid)
		{
		case 1590:
			m_UserItem[33].sSid = 1056;
			break;
		case 1594:
			m_UserItem[33].sSid = 1056;
			break;
		case 1598:
			m_UserItem[33].sSid = 1056;
			break;
		case 1602:
			m_UserItem[33].sSid = 1056;
			break;
		case 1606:
			m_UserItem[33].sSid = 1056;
			break;
		case 1650:
			m_UserItem[33].sSid = 1092;
			break;
		case 1654:
			m_UserItem[33].sSid = 1092;
			break;
		case 1658:
			m_UserItem[33].sSid = 1092;
			break;
		case 1662:
			m_UserItem[33].sSid = 1092;
			break;
		case 1666:
			m_UserItem[33].sSid = 1092;
			break;
		case 1607:
			m_UserItem[33].sSid = 1057;
			break;
		case 1608:
			m_UserItem[33].sSid = 1058;
			break;
		case 1609:
			m_UserItem[33].sSid = 1059;
			break;
		case 1610:
			m_UserItem[33].sSid = 1060;
			break;
		case 1611:
			m_UserItem[33].sSid = 1061;
			break;
		case 1612:
			m_UserItem[33].sSid = 1062;
			break;
		case 1613:
			m_UserItem[33].sSid = 1063;
			break;
		case 1614:
			m_UserItem[33].sSid = 1064;
			break;
		case 1615:
			m_UserItem[33].sSid = 1065;
			break;
		case 1616:
			m_UserItem[33].sSid = 1066;
			break;
		case 1617:
			m_UserItem[33].sSid = 1067;
			break;
		case 1618:
			m_UserItem[33].sSid = 1068;
			break;
		case 1619:
			m_UserItem[33].sSid = 1069;
			break;
		case 1620:
			m_UserItem[33].sSid = 1070;
			break;
		case 1621:
			m_UserItem[33].sSid = 1071;
			break;
		case 1622:
			m_UserItem[33].sSid = 1072;
			break;
		case 1623:
			m_UserItem[33].sSid = 1073;
			break;
		case 1624:
			m_UserItem[33].sSid = 1074;
			break;
		case 1625:
			m_UserItem[33].sSid = 1075;
			break;
		case 1626:
			m_UserItem[33].sSid = 1076;
			break;
		case 1627:
			m_UserItem[33].sSid = 1077;
			break;
		case 1628:
			m_UserItem[33].sSid = 1078;
			break;
		case 1629:
			m_UserItem[33].sSid = 1079;
			break;
		case 1630:
			m_UserItem[33].sSid = 1080;
			break;
		case 1631:
			m_UserItem[33].sSid = 1081;
			break;
		case 1632:
			m_UserItem[33].sSid = 1082;
			break;
		case 1633:
			m_UserItem[33].sSid = 1083;
			break;
		case 1634:
			m_UserItem[33].sSid = 1084;
			break;
		case 1635:
			m_UserItem[33].sSid = 1085;
			break;
		case 1636:
			m_UserItem[33].sSid = 1086;
			break;
		case 1637:
			m_UserItem[33].sSid = 1087;
			break;
		case 1638:
			m_UserItem[33].sSid = 1088;
			break;
		case 1639:
			m_UserItem[33].sSid = 1709;
			break;
		case 1640:
			m_UserItem[33].sSid = 1710;
			break;
		case 1641:
			m_UserItem[33].sSid = 1711;
			break;
		case 1642:
			m_UserItem[33].sSid = 1712;
			break;
		case 1643:
			m_UserItem[33].sSid = 1713;
			break;
		case 1644:
			m_UserItem[33].sSid = 1714;
			break;
		case 1645:
			m_UserItem[33].sSid = 1715;
			break;
		case 1646:
			m_UserItem[33].sSid = 1716;
			break;
		}
   //     m_UserItem[33].tMagic[6] = 0;//同时去除第6排
//		m_UserItem[33].tMagic[5] = 0;
		m_UserItem[33].tIQ = 12;
		
	}		

	CBufferEx TempBuf;
	int i,j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)index);
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	FlushItemLog( TRUE );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//监牢
//
void USER::Closeuser(int ftime)
{
	CString strMsg= "";
	strMsg.Format( "你受到封号%d分钟的处罚!!!", ftime );
	SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME );
	m_dwCloseTime = (60 * ftime * 1000);
	AddAbnormalInfo(ABNORMAL_JIANYU);
	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());
}
/////////////////////////////////////////////////////////////////////////////
//监牢
//
void USER::Fchuuser()
{
	m_dwCloseTime = 0;
	DeleteAbnormalInfo(ABNORMAL_JIANYU);
	SendSystemMsg("从监狱中释放!!",SYSTEM_NORMAL,TO_ME);
	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());
}
///////////////////////////////////////////////////////////////////////////如果是百级还原
void USER::BaiJiHuangYuan(int Slot, int SlotDJ) //如果是百级还原
{
	int index=0,iSuccess=1;
   

	if(Slot < EQUIP_ITEM_NUM || Slot >= TOTAL_INVEN_MAX) return;
	if(m_UserItem[Slot].tIQ!=3 || m_UserItem[Slot].sSid == -1 || (m_UserItem[Slot].sSid != 803 && m_UserItem[Slot].sSid != 698 && m_UserItem[Slot].sSid != 635 && m_UserItem[Slot].sSid != 849))
	{
       SendSystemMsg("必须是百级武器才可以转换!",SYSTEM_ERROR,TO_ME);
		return ;
	}
	if (m_UserItem[Slot].tMagic[4] != 0)
		{
       SendSystemMsg("您的武器属性出现非法!",SYSTEM_ERROR,TO_ME);
		return ;
	}

	if(m_UserItem[Slot].tMagic[5] < 7)
	{
		SendSystemMsg("要转换的武器必须是7改以上(含7改)!",SYSTEM_ERROR,TO_ME);
		return;
	}
    if(m_dwDN < 5000000)
		    {
			     SendSystemMsg( "决战币不足!使用武器装换卡500万!", SYSTEM_ERROR, TO_ME);
			     return;		
		    }

		     if( m_dwDN <= 5000000 ) m_dwDN = 0;
		     else m_dwDN = m_dwDN - 5000000;	
			 UpdateUserItemDN();						
	         SendMoneyChanged();
	int y1 = -1;
	int mark_ssid = m_UserItem[Slot].sSid;

	switch(mark_ssid)
	{
		
        case 849:
			y1 = 443;
			break;
		case 635:
			y1 = 408;
			break;
		case 698:
			y1 = 478;
			break;
		case 803:
			y1 = 513;
			break;
		default:
			break;
	}

	iSuccess=1;

	if (m_UserItem[Slot].tMagic[4] == 0) //如果百级是4排属性就还原3排属性的主体
	{
		m_UserItem[Slot].sLevel = g_arItemTable[y1]->m_byRLevel;
		m_UserItem[Slot].sSid = y1;
		m_UserItem[Slot].sCount = 1;
		m_UserItem[Slot].sDuration = g_arItemTable[y1]->m_sDuration;
		m_UserItem[Slot].sBullNum = 1;
	//	m_UserItem[Slot].tMagic[3] = 0;
		m_UserItem[Slot].tMagic[4] = 0;
		m_UserItem[Slot].tMagic[5] = 0;
		m_UserItem[Slot].tIQ = 3;
	}
	ReSetItemSlot(&m_UserItem[SlotDJ]);
	CBufferEx TempBuf;
	int j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	TempBuf.Add((BYTE)iSuccess);
	TempBuf.Add((BYTE)23);
	BYTE bySlot = (BYTE)Slot;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ); 

	bySlot = (BYTE)SlotDJ;
	TempBuf.Add((BYTE)bySlot);
	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add(m_UserItem[bySlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
	
	SendCharData();
	SendMyInfo(TO_INSIGHT, INFO_MODIFY);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	捞亥飘 惑前鼻阑 雀荐茄促.
//
void USER::EventUpgradeItemReq(TCHAR *pBuf)
{
	short sSid = 0;
	int index = 0, j;
	int event = 0, iWeight = 0;
	int event_index = -1;

	int iIQ = 0, upgrade_num = 0;

	BOOL bSuccess = TRUE;

	CBufferEx TempBuf;

	BYTE tEventSlot = GetByte(pBuf, index);				// 捞亥飘 惑前鼻 浇吩
	BYTE tItemSlot = GetByte(pBuf, index);				// 诀臂窍扁困茄 秦寸 浇吩 

	if(tEventSlot < EQUIP_ITEM_NUM || tEventSlot >= TOTAL_INVEN_MAX) goto go_result;
	if(tItemSlot < EQUIP_ITEM_NUM || tItemSlot >= TOTAL_INVEN_MAX) goto go_result;

	sSid = m_UserItem[tEventSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) goto go_result;

	sSid = m_UserItem[tItemSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) goto go_result;

	if(g_arItemTable[sSid]->m_sDuration <= 0) goto go_result;		// 郴备档啊 绝栏搁 芭何		
	if(m_UserItem[tItemSlot].tIQ != NORMAL_ITEM) goto go_result;	// 老馆 酒捞袍父 茄促.
	if(m_UserItem[tItemSlot].tMagic[5] > 0) goto go_result;			// 诀弊饭捞靛 酒捞袍档 救等促.

	iIQ = (int)m_UserItem[tEventSlot].tIQ;

	if(g_arItemTable[sSid]->m_byWear == ATTACK_ITEM)	// 傍拜侩 公扁
	{
		if(m_UserItem[tEventSlot].tIQ < EVENT_ITEM_BAND) goto go_result;
		if(iIQ != 240 && iIQ != 244 && iIQ != 242 && iIQ != 246 && iIQ != 248 &&
		iIQ != 101 && iIQ != 103 && iIQ != 204 && iIQ != 105 && iIQ != 107) goto go_result;

		event = CheckEventItem(event_index, (int)tEventSlot);
		if(event < 0)
		{
			TRACE("CANT FIND CHANGE OF ARMS\n");
//			SendSystemMsg( IDS_USER_NO_CHANGE_THIS, SYSTEM_ERROR, TO_ME);
//			goto go_result;								// 矫府倔 锅龋啊 撇府促.
		}

		if(event_index < 0) 
		{
			TRACE("公扁 背券鼻阑 茫阑荐啊 绝促.\n");
//			goto go_result;
		}
																	// 趣矫唱 率唱绰巴阑 规瘤窍扁 困秦
//		if(InterlockedCompareExchange(&g_arEventItemTable[event]->m_lUsed, (LONG)1, (LONG)0) == 0)
//		{
		if(!UpdateEventItem(event))
		{
			TRACE("公扁 诀但 角菩\n");
//			InterlockedExchange(&g_arEventItemTable[event_index]->m_lUsed, 0);
//			InterlockedExchange(&g_arAddEventItemTable[event_index]->m_lUsed, 0);
//			goto go_result;
		}

//		g_arAddEventItemTable[event_index]->m_tGiveItem = 1;				// 格废俊辑 悸泼
//		CEventItemTable *pEvent = g_arAddEventItemTable[event_index];
		switch(iIQ)
		{
		case EVENT_ATT7_ITEM:
			upgrade_num = 7;
			break;
		case EVENT_ATT6_ITEM:
			upgrade_num = 6;
			break;
		case EVENT_ATT_ITEM:
			upgrade_num = 5;
			break;
		case EVENT_ATT4_ITEM:
			upgrade_num = 4;
			break;
		case EVENT_ATT3_ITEM:
			upgrade_num = 3;
			break;
		}

		m_UserItem[tItemSlot].tMagic[4] = ATTACK_UPGRADE_BAND * upgrade_num;
		m_UserItem[tItemSlot].tMagic[5] = upgrade_num;	// 付瘤阜 浇吩俊 己傍冉荐甫 刘啊 矫挪促.

		MakeItemLog( &m_UserItem[tItemSlot], ITEMLOG_UPGRADE_SUCCESS );
					
		bSuccess = FALSE;
		iWeight = g_arItemTable[m_UserItem[tEventSlot].sSid]->m_byWeight;
		ReSetItemSlot(&m_UserItem[tEventSlot]);
//		}
	}
	else
	{
		if(m_UserItem[tEventSlot].tIQ < EVENT_ITEM_BAND) goto go_result;
		if(iIQ != 241 && iIQ != 245 && iIQ != 243 && iIQ != 247 && iIQ != 249 &&
		iIQ != 102 && iIQ != 104 && iIQ != 203 && iIQ != 106 && iIQ != 108) goto go_result;

		event = CheckEventItem(event_index, (int)tEventSlot);
		if(event < 0)
		{
			TRACE("规绢 背券鼻阑 茫阑荐啊 绝促.\n");
//			SendSystemMsg( IDS_USER_NO_CHANGE_THIS, SYSTEM_ERROR, TO_ME);
//			goto go_result;								// 矫府倔 锅龋啊 撇府促.
		}

		if(event_index < 0) 
		{
			TRACE("规绢 背券鼻阑 茫阑荐啊 绝促.\n");
//			goto go_result;
		}
																	// 趣矫唱 率唱绰巴阑 规瘤窍扁 困秦
//		if(InterlockedCompareExchange(&g_arEventItemTable[event]->m_lUsed, (LONG)1, (LONG)0) == 0)
//		{
		if(!UpdateEventItem(event))
		{
			TRACE("规绢 诀但 角菩\n");
//			InterlockedExchange(&g_arEventItemTable[event_index]->m_lUsed, 0);
//			InterlockedExchange(&g_arAddEventItemTable[event_index]->m_lUsed, 0);
//			goto go_result;
		}

//		g_arEventItemTable[event_index]->m_tGiveItem = 1;				// 格废俊辑 悸泼
//		g_arAddEventItemTable[event_index]->m_tGiveItem = 1;				// 格废俊辑 悸泼
		switch(iIQ)
		{
		case EVENT_DEF7_ITEM:
			upgrade_num = 7;
			break;
		case EVENT_DEF6_ITEM:
			upgrade_num = 6;
			break;
		case EVENT_DEF_ITEM:
			upgrade_num = 5;
			break;
		case EVENT_DEF4_ITEM:
			upgrade_num = 4;
			break;
		case EVENT_DEF3_ITEM:
			upgrade_num = 3;
			break;
		}
		m_UserItem[tItemSlot].tMagic[4] = DEFENSE_UPGRADE_BAND * upgrade_num;
		m_UserItem[tItemSlot].tMagic[5] = upgrade_num;	// 付瘤阜 浇吩俊 己傍冉荐甫 刘啊 矫挪促.

		MakeItemLog( &m_UserItem[tItemSlot], ITEMLOG_UPGRADE_SUCCESS );

		bSuccess = FALSE;
		iWeight = g_arItemTable[m_UserItem[tEventSlot].sSid]->m_byWeight;
		ReSetItemSlot(&m_UserItem[tEventSlot]);
//		}
	}

	FlushItemLog( TRUE );

go_result:
	TempBuf.Add(UPGRADE_ITEM_RESULT);

	if(bSuccess)
	{
		TempBuf.Add((BYTE)0x00); //角菩
	}
	else TempBuf.Add((BYTE)0x01);				// 己傍

	TempBuf.Add((BYTE)0x02);				// 浇吩 2俺啊 函悼

	TempBuf.Add(tItemSlot);					// 林 酒捞袍甫 刚历 	
	TempBuf.Add(m_UserItem[tItemSlot].sLevel);
	TempBuf.Add(m_UserItem[tItemSlot].sSid);
	TempBuf.Add(m_UserItem[tItemSlot].sDuration);
	TempBuf.Add(m_UserItem[tItemSlot].sBullNum);
	TempBuf.Add(m_UserItem[tItemSlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[tItemSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[tItemSlot].tIQ); 

	TempBuf.Add(tEventSlot);
	TempBuf.Add(m_UserItem[tEventSlot].sLevel);
	TempBuf.Add(m_UserItem[tEventSlot].sSid);
	TempBuf.Add(m_UserItem[tEventSlot].sDuration);
	TempBuf.Add(m_UserItem[tEventSlot].sBullNum);
	TempBuf.Add(m_UserItem[tEventSlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[tEventSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[tEventSlot].tIQ); 

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	UpdateUserItemDN();

	GetRecoverySpeed();							// 雀汗加档 眉农...

	Send(TempBuf, TempBuf.GetLength());
}

int USER::CheckEventItem(int &index, int iSlot, BOOL bPotion)
{	
	int i, j;
	int nNum = 0;
	CString strTemp;
	CString strSerial = _T("");

	for(j = 0; j < MAGIC_NUM; j++)
	{
		strTemp = _T("");
		strTemp.Format("%d", m_UserItem[iSlot].tMagic[j]);
		if(j < (MAGIC_NUM - 1)) strTemp += "-";

		strSerial += strTemp;
	}

	for(i = 0; i < g_arAddEventItemTable.GetSize(); i++)
	{
		if(g_arAddEventItemTable[i]->m_tType == m_UserItem[iSlot].tIQ)
		{
			if(strSerial.CompareNoCase(g_arAddEventItemTable[i]->m_strSerialNum) == 0)
			{
				if(InterlockedCompareExchange((LONG*)&g_arAddEventItemTable[i]->m_lUsed, (long)1, (long)0) == (long)0) 
				{
					index = i;
					g_arAddEventItemTable[i]->m_tEnd = 1;
					return g_arAddEventItemTable[i]->m_sSid;
				}
			}
		}

		nNum = 0;
	}

	return -1;
}

BOOL USER::UpdateEventItem(int sid)
{
	if(sid < 0) return FALSE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];
	int				i = 1;

	SQLSMALLINT	sRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GIVE_EVENT_ITEM(%d, ?)}"), sid);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update EVENT_ITEM Data Only!!\n");
		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++ ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT,0,0, &sRet,0, &iRetInd);
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
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	if(sRet == 100 || sRet < 0)
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	捞亥飘 拱距 惑前鼻阑 雀荐窍扁困秦 捞亥飘 锅龋客 tIQ甫 犬牢茄饶 秦寸 酒捞袍阑 肮荐父怒 霖促..
//
void USER::GiveEventItem(int EventItemIndex, int tIQ, int ItemIndex, int Count)
{
	return;

	int i, j;
	int event = 0;
	int event_index = -1;

	ItemList TempItem;

	ReSetItemSlot(&TempItem);

	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == EventItemIndex)
		{
			if(m_UserItem[i].tIQ == EVENT_POT_ITEM)
			{
				event = CheckEventItem(event_index, i, TRUE);
				if(event < 0) break;								// 矫府倔 锅龋啊 撇府促.
																	// 趣矫唱 率唱绰巴阑 规瘤窍扁 困秦
				if(event_index < 0) break;

				if(InterlockedCompareExchange((LONG*)&g_arEventItemTable[event_index]->m_lUsed, (long)1, (long)0) == (long)0)
				{
					TempItem = m_UserItem[i];

					ReSetItemSlot(&m_UserItem[i]);

					m_UserItem[i].sLevel = g_arItemTable[ItemIndex]->m_byRLevel;
					m_UserItem[i].sSid = ItemIndex;
					m_UserItem[i].sCount = Count;
					m_UserItem[i].sDuration = g_arItemTable[ItemIndex]->m_sDuration;
					m_UserItem[i].sBullNum = g_arItemTable[ItemIndex]->m_sBullNum;
					m_UserItem[i].tIQ = NORMAL_ITEM;

					if(!UpdateEventItem(event))
					{
						ReSetItemSlot(&m_UserItem[i]);
						m_UserItem[i] = TempItem;
						InterlockedExchange(&g_arEventItemTable[event_index]->m_lUsed, 0);
						
						break;
					}

					g_arEventItemTable[event_index]->m_tGiveItem = 1;		// 格废俊辑 悸泼

					m_iCurWeight += g_arItemTable[ItemIndex]->m_byWeight * Count;

					UpdateUserItemDN();

					GetRecoverySpeed();								// 雀汗加档 眉农...

					CBufferEx TempBuf;
					TempBuf.Add(ITEM_USE_RESULT);

					TempBuf.Add((BYTE)0x01);						// 己傍
					TempBuf.Add((BYTE)103);							// 浇吩 1俺啊 函悼

					TempBuf.Add((BYTE)i);							// 林 酒捞袍甫 刚历 	
					TempBuf.Add(m_UserItem[i].sLevel);
					TempBuf.Add(m_UserItem[i].sSid);
					TempBuf.Add(m_UserItem[i].sDuration);
					TempBuf.Add(m_UserItem[i].sBullNum);
					TempBuf.Add(m_UserItem[i].sCount);
					for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[i].tMagic[j]);
					TempBuf.Add(m_UserItem[i].tIQ); 
					Send(TempBuf, TempBuf.GetLength());	

					SendSystemMsg( IDS_USER_CHANGE_COMPLETED, SYSTEM_ERROR, TO_ME);
					return;
				}
			}
		}
	}

	SendSystemMsg( IDS_USER_NO_CHANGE_THIS, SYSTEM_ERROR, TO_ME);
}


BOOL USER::OpenOnWeb()
{
	//打开游戏里的IE
	CBufferEx TempBuf;
	TempBuf.Add((BYTE)243);	
	TempBuf.AddString(o_yehuoini[0]->WEB_URL);
	Send(TempBuf, TempBuf.GetLength());
	return TRUE;
}
void USER::SuitUpgrade(TCHAR *pBuf)
{
	if(m_dwDN < 10000000) 
	{
		SendSystemMsg( "升级超级机甲需要1000万!", SYSTEM_ERROR, TO_ME);
		return;		// 诀弊饭捞靛 且 捣捞 绝栏搁 府畔
	}
	if(m_UserItem[39].sSid != 1184) return;
	int Success = 0;
	int index=0;
	short Slot = GetShort(pBuf, index);//设计图
	if(Slot < 10 || Slot > 33) return;
	short sSourceSid = m_UserItem[Slot].sSid;//得到物品ID
	//if(sSourceSid != 1160) return;
	BYTE tCount = m_UserItem[Slot].tMagic[5];
	if(tCount >= 2) return;

	int iRandom = myrand(1, 10000);						// 机甲升级成功率
	//iRandom = UpgradeSucc(iRandom);
	//if(iRandom <= g_SuitUpgrade[tCount]) 
	{
		Success = 1;
		//SendSystemMsg( "太阳炉升级成功", SYSTEM_ERROR, TO_ME);
	}
	//else
	//{
	//	Success = 0;
	//	//SendSystemMsg( "太阳炉升级失败", SYSTEM_ERROR, TO_ME);
	//}
	if(Success == 1){
		m_UserItem[39].tMagic[5] += 1;
		if(m_UserItem[39].tMagic[5] == 1){
			m_UserItem[39].tMagic[0] = 8;
			m_UserItem[39].tMagic[1] = 9;
			m_UserItem[39].tMagic[2] = 10;
			m_UserItem[39].tMagic[3] = 11;
			m_UserItem[39].tMagic[4] = 12;
		}else if(m_UserItem[39].tMagic[5] == 2){
			m_UserItem[39].tMagic[0] = 14;
			m_UserItem[39].tMagic[1] = 15;
			m_UserItem[39].tMagic[2] = 16;
			m_UserItem[39].tMagic[3] = 17;
			m_UserItem[39].tMagic[4] = 18;
		}
		// 2 3 4 5 6
		//8 9 10 11 12
		// 14 15 16 17 18
	}else{
	   if(m_UserItem[39].tMagic[5] > 0)	m_UserItem[39].tMagic[5] -= 1;		
	   if(m_UserItem[39].tMagic[5] == 255) m_UserItem[39].tMagic[5] = 0;
	   if(m_UserItem[39].tMagic[5] == 0){
			m_UserItem[39].tMagic[0] = 2;
			m_UserItem[39].tMagic[1] = 3;
			m_UserItem[39].tMagic[2] = 4;
			m_UserItem[39].tMagic[3] = 5;
			m_UserItem[39].tMagic[4] = 6;
		}else if(m_UserItem[39].tMagic[5] == 1){
			m_UserItem[39].tMagic[0] = 8;
			m_UserItem[39].tMagic[1] = 9;
			m_UserItem[39].tMagic[2] = 10;
			m_UserItem[39].tMagic[3] = 11;
			m_UserItem[39].tMagic[4] = 12;
		}
	}	
	if(m_UserItem[39].tMagic[5] > 2) m_UserItem[39].tMagic[5] = 2;

	CUIntArray arMaterial;
	arMaterial.Add(Slot);
	ReSetItemSlot(&m_UserItem[Slot]);
	CBufferEx TempBuf;
	int i,j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();
	if(Success) TempBuf.Add((BYTE)1);
	else  TempBuf.Add((BYTE)0);
	TempBuf.Add((BYTE)index);
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());

	RobItem(1304,1);	
	SendCharData();
	if( m_dwDN <= 10000000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 10000000;
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();
}
void USER::ChengXuKa(TCHAR *pBuf)
{
	//程序卡
	int index=0;
	short Slot = GetShort(pBuf, index);//设计图
	int TarSlot = -1;
	int sh = m_UserItem[Slot].tMagic[0];
	if(sh <= 0 || sh > 44) return;
	if(m_UserItem[39].sSid == 1184 && m_iSkin != 0)  
	{
		SendEventMsg("装置[程序卡]前请先切换为人形模式!");
		return;  
	}

    // 1-6
	// 7-12
	// 13-16
	// 17-20	
	if(sh <= 12) TarSlot = 8;
	if(sh >= 13 && sh <= 20) TarSlot = 9;
	if(sh >= 21 && sh <= 44) TarSlot = 10;
	
	m_UserItem[39].tMagic[TarSlot] = (BYTE)m_UserItem[Slot].tMagic[0];
	
	CUIntArray arMaterial;
	arMaterial.Add(Slot);
	ReSetItemSlot(&m_UserItem[Slot]);
	CBufferEx TempBuf;
	int i,j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)index);
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	SendCharData();
}
void USER::JjPeiJian(TCHAR *pBuf)
{
	if(m_dwDN < 10000000) 
	{
		SendSystemMsg( "钱不足!装置配件需1000万!", SYSTEM_ERROR, TO_ME);
		return;		// 诀弊饭捞靛 且 捣捞 绝栏搁 府畔
	}
		if(m_UserItem[39].sSid == 1184 && m_iSkin != 0)  
	{
		SendEventMsg("装置[配件]前请先切换为人形模式!");
		return;  //1122333   
	}

	
	//配件
	int index=0;
	short Slot = GetShort(pBuf, index);
	//导弹 5 激光
	if(m_UserItem[Slot].tMagic[0] == 4 || m_UserItem[Slot].tMagic[0] == 5)	m_UserItem[39].tMagic[6] = m_UserItem[Slot].tMagic[0];
	else m_UserItem[39].tMagic[7] = m_UserItem[Slot].tMagic[0];
	//战神姿态
    if(m_UserItem[Slot].tMagic[0] == 7)	m_UserItem[39].tMagic[7] = m_UserItem[Slot].tMagic[0];
	
	CUIntArray arMaterial;
	arMaterial.Add(Slot);
	ReSetItemSlot(&m_UserItem[Slot]);
	CBufferEx TempBuf;
	int i,j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)index);
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	SendCharData();
	if( m_dwDN <= 10000000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 10000000;
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();

}
void USER::HeChengJJ(TCHAR *pBuf)
{
	int sSid= -1;
	int iWeight=0;
	int index=0;

	short Slot = GetShort(pBuf, index);//设计图
	short Slot1 = GetShort(pBuf, index);//零件1
	short Slot2 = GetShort(pBuf, index);//零件2
	short Slot3 = GetShort(pBuf, index);//零件3
	if(Slot < EQUIP_ITEM_NUM || Slot >= TOTAL_INVEN_MAX) return;
	if(Slot1 < EQUIP_ITEM_NUM || Slot1 >= TOTAL_INVEN_MAX) return;
	if(Slot2 < EQUIP_ITEM_NUM || Slot2 >= TOTAL_INVEN_MAX) return;
	if(Slot3 < EQUIP_ITEM_NUM || Slot3 >= TOTAL_INVEN_MAX) return;
	int sl = m_UserItem[Slot].sSid;
	int sl1 = m_UserItem[Slot1].sSid;
	int sl2 = m_UserItem[Slot2].sSid;
	int sl3 = m_UserItem[Slot3].sSid;

	int sx1 = m_UserItem[Slot1].tMagic[0];
	int sx2 = m_UserItem[Slot2].tMagic[0];
	int sx3 = m_UserItem[Slot3].tMagic[0];
    if(sl !=1161)		return;
	if(sl == 1161) //v0
	{
		if(sl1 == 1157 && sl2 == 1158 && sl3 == 1159)	sSid = 1184;		
	else
		{
			SendEventMsg("顺序错误,放入顺序依次为:机板-芯片-回路!");

		}
	}

	//if(sSid == -1) return;
	if( sSid < 0 || sSid >= g_arItemTable.GetSize() ) return;


	CUIntArray arMaterial;
	arMaterial.Add(Slot);	
	arMaterial.Add(Slot1);	
	arMaterial.Add(Slot2);	
	arMaterial.Add(Slot3);
	ReSetItemSlot(&m_UserItem[Slot]);
	ReSetItemSlot(&m_UserItem[Slot1]);
	ReSetItemSlot(&m_UserItem[Slot2]);
	ReSetItemSlot(&m_UserItem[Slot3]);	

	m_UserItem[Slot].sSid = sSid;
	m_UserItem[Slot].sLevel = g_arItemTable[sSid]->m_byRLevel;	
	m_UserItem[Slot].sCount = 1;
	m_UserItem[Slot].sDuration = g_arItemTable[sSid]->m_sDuration;
	m_UserItem[Slot].sBullNum = g_arItemTable[sSid]->m_sBullNum;	
	m_UserItem[Slot].tMagic[0] = sx1;
	m_UserItem[Slot].tMagic[1] = sx2;
	m_UserItem[Slot].tMagic[2] = sx3;
	m_UserItem[Slot].tMagic[3] = 5;	
	m_UserItem[Slot].tMagic[4] = 6;	
	m_UserItem[Slot].tMagic[5] = 0;
	m_UserItem[Slot].tIQ = 13;
	m_UserItem[Slot].iItemSerial = 0;
	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	CBufferEx TempBuf;
	int i,j;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();
	TempBuf.Add((BYTE)14);
	TempBuf.Add((BYTE)index);
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	FlushItemLog( TRUE );
}
void USER::EbodyFj39(int num)
{
    if(m_dwDN < 500000) 
	{
	//	SendSystemMsg( "钱不足!解除配件需50万!", SYSTEM_ERROR, TO_ME);
		SendEventMsg("钱不足!解除配件需50万!");
		return;		// 诀弊饭捞靛 且 捣捞 绝栏搁 府畔
	}
	bool isNullSlot = false;
	int sloot = 0;
	for(int isl = 10; isl < 34;isl++)
	{
		if(m_UserItem[isl].sSid == -1)	
		{
			sloot++;
			if(sloot > 1)
			{
				isNullSlot = true;
				break;
			}
		}
	} 
	if(!isNullSlot) 
	{
	//	SendSystemMsg("清理装备栏留出空间",SYSTEM_ERROR,TO_ME);	
		SendEventMsg("清理装备栏留出空间");
		return;
	}
	int fjslot = -1;
	int itemid = -1;
	BYTE jjsx = -1;
	switch(num)
	{
	case 1:
		fjslot = 6;
		break;
	case 2:
		fjslot = 7;
		break;
	default:
		break;
	}
	if(fjslot == -1) return;
	if(m_UserItem[39].sSid != 0 && m_UserItem[39].tMagic[fjslot] != 0) jjsx = m_UserItem[39].tMagic[fjslot];
	if(jjsx == -1) return;
	switch(jjsx)
	{
		case 1:
			itemid = 1174;
			break;
		case 2:
			itemid = 1175;			
			break;
		case 3:
			itemid = 1176;			
			break;
		case 4:
			itemid = 1178;			
			break;
		case 5:
			itemid = 1179;
			break;
		case 6:	
			itemid = 1179;
			break;
		case 7:	
			itemid = 1177;
			break;
	}
	
	if(itemid == -1) return;
	m_UserItem[39].tMagic[fjslot] = 0;
	GiveAllItem(itemid,1,0,2,jjsx,0,0,0,0);
	SendCharData();
	CheckMagicItemMove();
	if( m_dwDN <= 500000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 500000;
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();
}
//删除cxk 1 2 3
void USER::EbodyFjCXcard(int num)
{
	if(m_dwDN < 500000) 
	{
		//SendSystemMsg( "钱不足!解除程序卡需60万!", SYSTEM_ERROR, TO_ME);
        SendEventMsg("钱不足!解除程序卡需50万!");
		return;		// 诀弊饭捞靛 且 捣捞 绝栏搁 府畔
	}

    bool isNullSlot = false;
	int sloot = 0;
	for(int isl = 10; isl < 34;isl++)
	{
		if(m_UserItem[isl].sSid == -1)	
		{
			sloot++;
			if(sloot > 1)
			{
				isNullSlot = true;
				break;
			}
		}
	} 
	if(!isNullSlot) 
	{
	//	SendSystemMsg("清理装备栏留出空间",SYSTEM_ERROR,TO_ME);	
		SendEventMsg("清理装备栏留出空间");
		return;
	}

	int cardslot = -1;
	BYTE jjsx = -1;
	switch(num)
	{
	case 1:
		cardslot = 8;
		break;
	case 2:
		cardslot = 9;
		break;
	case 3:
		cardslot= 10;
		break;
	default:
		break;
	}	
	if(cardslot == -1) return;
	if(m_UserItem[39].tMagic[cardslot] == 0)
	{
		SendEventMsg("该位置没有装入程序卡,无法卸下");
		return;
	}
	if(m_UserItem[39].sSid != 0 && m_UserItem[39].tMagic[cardslot] != 0) jjsx = m_UserItem[39].tMagic[cardslot];
	if(jjsx == -1) return;
	m_UserItem[39].tMagic[cardslot] = 0;			
	GiveAllItem(1180,1,0,2,jjsx,0,0,0,0);
	SendCharData();
	CheckMagicItemMove();
	if( m_dwDN <= 500000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 500000;
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();
}
void USER::Ebody39(int solt)
{

	int j = 0;
	ItemList	TempItem;
	m_UserItem[solt].sCount = 1;
	TempItem = m_UserItem[solt];	
	m_UserItem[solt] = m_UserItem[solt];
	m_UserItem[39] = TempItem;	
	
	CBufferEx TempBuf;
	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(0);
	TempBuf.Add(2);	
	TempBuf.Add(solt);
	TempBuf.Add(m_UserItem[solt].sLevel);
	TempBuf.Add(m_UserItem[solt].sSid);
	TempBuf.Add(m_UserItem[solt].sDuration);
	TempBuf.Add(m_UserItem[solt].sBullNum);
	TempBuf.Add(m_UserItem[solt].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[solt].tMagic[j]);
	TempBuf.Add(m_UserItem[solt].tIQ); 

	TempBuf.Add(39);
	TempBuf.Add(m_UserItem[39].sLevel);
	TempBuf.Add(m_UserItem[39].sSid);
	TempBuf.Add(m_UserItem[39].sDuration);
	TempBuf.Add(m_UserItem[39].sBullNum);
	TempBuf.Add(m_UserItem[39].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[39].tMagic[j]);
	TempBuf.Add(m_UserItem[39].tIQ); 

	Send(TempBuf, TempBuf.GetLength());
	//HuanYuanBianShen();
	if(m_UserItem[39].sSid == -1 && m_iSkin != 0)
	{
		m_iSkin=0 , m_iHair=0;
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
	}
	SendCharData();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	秦寸 酒捞袍阑 肮荐父怒 霖促.
//
BOOL USER::GiveItem(int sid, int iCount)
{
	if( sid < 0 || sid >= g_arItemTable.GetSize() ) return FALSE;
	if( iCount <= 0 ) return FALSE;

	int iWeight = 0;
	ItemList GiveItem;
	ReSetItemSlot( &GiveItem );

	GiveItem.sLevel			= g_arItemTable[sid]->m_byRLevel;
	GiveItem.sSid			= sid;
	GiveItem.sCount			= iCount;
	GiveItem.sDuration		= g_arItemTable[sid]->m_sDuration;
	GiveItem.sBullNum		= g_arItemTable[sid]->m_sBullNum;
	GiveItem.tIQ			= NORMAL_ITEM;

	iWeight = g_arItemTable[sid]->m_byWeight * iCount;

	CWordArray		arEmptySlot, arSameSlot;

	int iSlot = GetSameItem( GiveItem, INVENTORY_SLOT);

	if(iSlot != -1)	
	{ 
		if(iCount != 0)
		{
			CheckMaxValue((short &)m_UserItem[iSlot].sCount, (short)iCount); 
			arSameSlot.Add(iSlot); 
		}
	}
	else			
	{
		iSlot = GetEmptySlot( INVENTORY_SLOT );

		if( iSlot < 0 ) return FALSE;

		ReSetItemSlot( &m_UserItem[iSlot] );

		m_UserItem[iSlot].sLevel = g_arItemTable[sid]->m_byRLevel;
		m_UserItem[iSlot].sSid = sid;
		m_UserItem[iSlot].sCount = iCount;
		m_UserItem[iSlot].sDuration = g_arItemTable[sid]->m_sDuration;
		m_UserItem[iSlot].sBullNum = g_arItemTable[sid]->m_sBullNum;
		m_UserItem[iSlot].tIQ = NORMAL_ITEM;

		arEmptySlot.Add(iSlot); 
	}			

	m_iCurWeight += iWeight;
	GetRecoverySpeed();					// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券

	UpdateInvenSlot(&arEmptySlot, &arSameSlot);
/*
	int iSlot = GetEmptySlot(INVENTORY_SLOT);

	if(iSlot < 0) return FALSE;

	ReSetItemSlot(&m_UserItem[iSlot]);

	m_UserItem[iSlot].sLevel = g_arItemTable[sid]->m_byRLevel;
	m_UserItem[iSlot].sSid = sid;
	m_UserItem[iSlot].sCount = iCount;
	m_UserItem[iSlot].sDuration = g_arItemTable[sid]->m_sDuration;
	m_UserItem[iSlot].sBullNum = g_arItemTable[sid]->m_sBullNum;
	m_UserItem[iSlot].tIQ = NORMAL_ITEM;
*/
	return TRUE;
}
BOOL USER::GiveItemAll(int sid, int iCount,int upg,
					   int x1,int x2,int x3,int x4,int x5,
					   int x6,int x7,int x8,int x9,int x10)//107任务
{
	if( sid < 0 || sid >= g_arItemTable.GetSize() ) return FALSE;
	if( iCount <= 0 ) return FALSE;
	
	int shuxingcount = 0;
	if(x1 != 0) shuxingcount += 1;
	if(x2 != 0) shuxingcount += 1;
	if(x3 != 0) shuxingcount += 1;
	if(x4 != 0) shuxingcount += 1;
	if(x5 != 0) shuxingcount += 1;	
	if(x6 != 0) shuxingcount += 1;
	if(x7 != 0) shuxingcount += 1;
	if(x8 != 0) shuxingcount += 1;
	if(x9 != 0) shuxingcount += 1;
	if(x10 != 0) shuxingcount += 1;

	if(upg < 0 || upg > MAX_ITEM_UPGRADE_COUNT)	upg = 0;
	int max_sx = 190;
	if(x1 < 0 || x1 > max_sx)	x1 = 0;
	if(x2 < 0 || x2 > max_sx)	x2 = 0;
	if(x3 < 0 || x3 > max_sx)	x3 = 0;
	if(x4 < 0 || x4 > max_sx)	x4 = 0;
	if(x5 < 0 || x5 > max_sx)	x5 = 0;	
	if(x6 < 0 || x6 > max_sx)	x6 = 0;
	if(x7 < 0 || x7 > max_sx)	x7 = 0;
	if(x8 < 0 || x8 > max_sx)	x8 = 0;
	if(x9 < 0 || x9 > max_sx)	x9 = 0;
	if(x10 < 0 || x10 > max_sx)	x10 = 0;

	int iWeight = 0;
	ItemList GiveItemAll;
	ReSetItemSlot( &GiveItemAll );	

	GiveItemAll.sLevel			= g_arItemTable[sid]->m_byRLevel;
	GiveItemAll.sSid			= sid;
	GiveItemAll.sCount			= iCount;
	GiveItemAll.sDuration		= g_arItemTable[sid]->m_sDuration;
	GiveItemAll.sBullNum		= g_arItemTable[sid]->m_sBullNum;
	GiveItemAll.tMagic[0]		= (BYTE)x1;
	GiveItemAll.tMagic[1]		= (BYTE)x2;
	GiveItemAll.tMagic[2]		= (BYTE)x3;
	GiveItemAll.tMagic[3]		= (BYTE)x4;
	GiveItemAll.tMagic[4]		= (BYTE)x5;
	GiveItemAll.tMagic[5]		= (BYTE)upg;
	GiveItemAll.tMagic[6]		= (BYTE)x6;
	GiveItemAll.tMagic[7]		= (BYTE)x7;
	GiveItemAll.tMagic[8]		= (BYTE)x8;
	GiveItemAll.tMagic[9]		= (BYTE)x9;
	GiveItemAll.tMagic[10]		= (BYTE)x10;

	if(sid >= 726 && sid <= 754)		GiveItemAll.tIQ	= SET_ITEM;
	else if( g_arItemTable[sid]->m_byWear == 130 )	GiveItemAll.tIQ	= GUARDIAN_ITEM;		
	else if((shuxingcount >= 1 && shuxingcount <= 2)
		|| (g_arItemTable[sid]->m_byWear >= 122 && g_arItemTable[sid]->m_byWear <= 126)) GiveItemAll.tIQ = MAGIC_ITEM;
	else if(shuxingcount >= 3 && shuxingcount <= 4)	GiveItemAll.tIQ	= RARE_ITEM;	
	else if((shuxingcount >= 5 && shuxingcount <= 10) || g_arItemTable[sid]->m_byWear == 152) GiveItemAll.tIQ	= RARE2_ITEM;

	iWeight = g_arItemTable[sid]->m_byWeight * iCount;
	CWordArray		arEmptySlot, arSameSlot;
/////////////////调整无耐久物品叠加防复制问题开始/////////////////
//	 int iSlot = -1;
     int iSlot;
	    if(g_arItemTable[sid]->m_sDuration > 0)
	    {
		    ItemList tmpItem;
			int j; 
            for(j = 0; j < iCount; j++)
			{
			    iSlot = GetEmptySlot( INVENTORY_SLOT );
				if(iSlot == 0) return FALSE;
				
                if( iSlot < 0 ) 
				{
					SendSystemMsg( IDS_USER_FULL_INVEN, SYSTEM_ERROR, TO_ME);
					return FALSE;
			
				}

				ReSetItemSlot(&tmpItem);

				m_UserItem[iSlot].sLevel = g_arItemTable[sid]->m_byRLevel;
				m_UserItem[iSlot].sSid = sid;
				m_UserItem[iSlot].sCount = 1;
				m_UserItem[iSlot].sDuration = g_arItemTable[sid]->m_sDuration;
				m_UserItem[iSlot].sBullNum = g_arItemTable[sid]->m_sBullNum;
				m_UserItem[iSlot].tMagic[0]		= x1;
		        m_UserItem[iSlot].tMagic[1]		= x2;
		        m_UserItem[iSlot].tMagic[2]		= x3;
		        m_UserItem[iSlot].tMagic[3]		= x4;
		        m_UserItem[iSlot].tMagic[4]		= x5;
		        m_UserItem[iSlot].tMagic[5]		= upg;
		        m_UserItem[iSlot].tMagic[6]		= x6;
		        m_UserItem[iSlot].tMagic[7]		= x7;
		        m_UserItem[iSlot].tMagic[8]		= x8;
		        m_UserItem[iSlot].tMagic[9]		= x9;
		        m_UserItem[iSlot].tMagic[10]	= x10;	

				if(sid >= 726 && sid <= 754)			m_UserItem[iSlot].tIQ = SET_ITEM;
		        else if( g_arItemTable[sid]->m_byWear == 130 )	m_UserItem[iSlot].tIQ = GUARDIAN_ITEM;
			    else if((shuxingcount >= 1 && shuxingcount <= 2)
		        || g_arItemTable[sid]->m_byWear >= 122 && g_arItemTable[sid]->m_byWear <= 126) m_UserItem[iSlot].tIQ = MAGIC_ITEM;
		        else if(shuxingcount >= 3 && shuxingcount <= 4)	m_UserItem[iSlot].tIQ = RARE_ITEM;
		        else if(shuxingcount >= 5 && shuxingcount <= 10) m_UserItem[iSlot].tIQ	= RARE2_ITEM;
			
                arEmptySlot.Add(iSlot);
				UpdateInvenSlot(&arEmptySlot, &arSameSlot);
		 }
            
        }else
	    {
		    ItemList tmpItem;
		    ReSetItemSlot(&tmpItem);
		    tmpItem.sSid = sid;
		    iSlot = GetSameItem(tmpItem, INVENTORY_SLOT);
		if(iSlot < 0)
		{
			iSlot = GetEmptySlot( INVENTORY_SLOT );
		}
		if( iSlot < 0 ) 
				return FALSE;
		m_UserItem[iSlot].sCount += iCount;
		m_UserItem[iSlot].sSid = sid;
	    arEmptySlot.Add(iSlot); 

		m_iCurWeight += iWeight;
	    GetRecoverySpeed();	

		UpdateInvenSlot(&arEmptySlot, &arSameSlot);
		return FALSE;
	    }
/////////////////调整无耐久物品叠加防复制问题结束/////////////////

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///测试刷10属性装备命令
BOOL USER::GiveItemAll10(int sid, int iCount,int upg,int sIQ,
					   int x1,int x2,int x3,int x4,int x5,
					   int x6,int x7,int x8,int x9,int x10)//107任务
{
	if( sid < 0 || sid >= g_arItemTable.GetSize() ) return FALSE;
	if( iCount <= 0 ) return FALSE;
	
	int shuxingcount = 0;
	if(x1 != 0) shuxingcount += 1;
	if(x2 != 0) shuxingcount += 1;
	if(x3 != 0) shuxingcount += 1;
	if(x4 != 0) shuxingcount += 1;
	if(x5 != 0) shuxingcount += 1;	
	if(x6 != 0) shuxingcount += 1;
	if(x7 != 0) shuxingcount += 1;
	if(x8 != 0) shuxingcount += 1;
	if(x9 != 0) shuxingcount += 1;
	if(x10 != 0) shuxingcount += 1;

	if(upg < 0 || upg > MAX_ITEM_UPGRADE_COUNT)	upg = 0;
	int max_sx = 205;
	if(x1 < 0 || x1 > max_sx)	x1 = 0;
	if(x2 < 0 || x2 > max_sx)	x2 = 0;
	if(x3 < 0 || x3 > max_sx)	x3 = 0;
	if(x4 < 0 || x4 > max_sx)	x4 = 0;
	if(x5 < 0 || x5 > max_sx)	x5 = 0;	
	if(x6 < 0 || x6 > max_sx)	x6 = 0;
	if(x7 < 0 || x7 > max_sx)	x7 = 0;
	if(x8 < 0 || x8 > max_sx)	x8 = 0;
	if(x9 < 0 || x9 > max_sx)	x9 = 0;
	if(x10 < 0 || x10 > max_sx)	x10 = 0;

	int iWeight = 0;
	ItemList GiveItemAll10;
	ReSetItemSlot( &GiveItemAll10 );	

	GiveItemAll10.sLevel			= g_arItemTable[sid]->m_byRLevel;
	GiveItemAll10.sSid			= sid;
	GiveItemAll10.sCount			= iCount;
	GiveItemAll10.sDuration		= g_arItemTable[sid]->m_sDuration;
	GiveItemAll10.sBullNum		= g_arItemTable[sid]->m_sBullNum;
	GiveItemAll10.tMagic[0]		= x1;
	GiveItemAll10.tMagic[1]		= x2;
	GiveItemAll10.tMagic[2]		= x3;
	GiveItemAll10.tMagic[3]		= x4;
	GiveItemAll10.tMagic[4]		= x5;
	GiveItemAll10.tMagic[5]		= upg;
	GiveItemAll10.tMagic[6]		= x6;
	GiveItemAll10.tMagic[7]		= x7;
	GiveItemAll10.tMagic[8]		= x8;
	GiveItemAll10.tMagic[9]		= x9;
	GiveItemAll10.tMagic[10]		= x10;
	GiveItemAll10.tIQ = sIQ;

	if(sid >= 726 && sid <= 754)		GiveItemAll10.tIQ	= SET_ITEM;
	else if( g_arItemTable[sid]->m_byWear == 122 )       GiveItemAll10.tIQ	= MAGIC_ITEM;
	else if( g_arItemTable[sid]->m_byWear == 123 )       GiveItemAll10.tIQ	= MAGIC_ITEM;
	else if( g_arItemTable[sid]->m_byWear == 124 )       GiveItemAll10.tIQ	= MAGIC_ITEM;
	else if( g_arItemTable[sid]->m_byWear == 125 )       GiveItemAll10.tIQ	= MAGIC_ITEM;
	else if( g_arItemTable[sid]->m_byWear == 130 )	GiveItemAll10.tIQ	= GUARDIAN_ITEM;
	else if(shuxingcount >= 1 && shuxingcount <= 2)	GiveItemAll10.tIQ = MAGIC_ITEM;
	else if(shuxingcount >= 3 && shuxingcount <= 4)	GiveItemAll10.tIQ	= RARE_ITEM;	
	else if(shuxingcount >= 5 && shuxingcount <= 10) GiveItemAll10.tIQ	= RARE2_ITEM;

	iWeight = g_arItemTable[sid]->m_byWeight * iCount;
	CWordArray		arEmptySlot, arSameSlot;

	int iSlot = GetSameItem( GiveItemAll10, INVENTORY_SLOT);

	if(iSlot != -1)	
	{ 
		if(iCount != 0)
		{
			CheckMaxValue((short &)m_UserItem[iSlot].sCount, (short)iCount); 
			arSameSlot.Add(iSlot); 
		}
	}
	else			
	{
		iSlot = GetEmptySlot( INVENTORY_SLOT );

		if( iSlot < 0 ) return FALSE;

		ReSetItemSlot( &m_UserItem[iSlot] );

		m_UserItem[iSlot].sLevel = g_arItemTable[sid]->m_byRLevel;
		m_UserItem[iSlot].sSid = sid;
		m_UserItem[iSlot].sCount = iCount;
		m_UserItem[iSlot].sDuration = g_arItemTable[sid]->m_sDuration;
		m_UserItem[iSlot].sBullNum = g_arItemTable[sid]->m_sBullNum;
		m_UserItem[iSlot].tMagic[0]		= x1;
		m_UserItem[iSlot].tMagic[1]		= x2;
		m_UserItem[iSlot].tMagic[2]		= x3;
		m_UserItem[iSlot].tMagic[3]		= x4;
		m_UserItem[iSlot].tMagic[4]		= x5;
		m_UserItem[iSlot].tMagic[5]		= upg;
		m_UserItem[iSlot].tMagic[6]		= x6;
		m_UserItem[iSlot].tMagic[7]		= x7;
		m_UserItem[iSlot].tMagic[8]		= x8;
		m_UserItem[iSlot].tMagic[9]		= x9;
		m_UserItem[iSlot].tMagic[10]	= x10;	
		GiveItemAll10.tIQ = sIQ;

		if(sid >= 726 && sid <= 754)			m_UserItem[iSlot].tIQ = SET_ITEM;
		else if( g_arItemTable[sid]->m_byWear == 122 )       m_UserItem[iSlot].tIQ = MAGIC_ITEM;
		else if( g_arItemTable[sid]->m_byWear == 123 )       m_UserItem[iSlot].tIQ = MAGIC_ITEM;
		else if( g_arItemTable[sid]->m_byWear == 124 )       m_UserItem[iSlot].tIQ = MAGIC_ITEM;
		else if( g_arItemTable[sid]->m_byWear == 125 )       m_UserItem[iSlot].tIQ = MAGIC_ITEM;
		else if( g_arItemTable[sid]->m_byWear == 130 )	m_UserItem[iSlot].tIQ = GUARDIAN_ITEM;
		else if(shuxingcount >= 1 && shuxingcount <= 2)	m_UserItem[iSlot].tIQ = MAGIC_ITEM;
		else if(shuxingcount >= 3 && shuxingcount <= 4)	m_UserItem[iSlot].tIQ = RARE_ITEM;
		else if(shuxingcount >= 5 && shuxingcount <= 10) m_UserItem[iSlot].tIQ	= RARE2_ITEM;
		arEmptySlot.Add(iSlot); 
	}			

	m_iCurWeight += iWeight;
	GetRecoverySpeed();					// 酒捞袍 公霸俊 函悼捞 积扁搁 雀汗加档 函券

	UpdateInvenSlot(&arEmptySlot, &arSameSlot);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	吝埃 傍己傈捞 场苞 悼矫俊 矫累阑 悸泼茄促.
//  要塞结束处理
void USER::StoppingTheFortressWar(CGuildFortress *pFort)
{	
	int i, type = 0;
	int nLen = 0;
	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildFortress *pBackFort = NULL;

	if(m_dwGuild <= 0) return;

	if(pFort == NULL) return;

	pBackFort = pFort;

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) 
	{
		ReleaseGuild();				// 秦力...
		return;
	}

	nLen = strlen(pGuild->m_strMasterName);
	::ZeroMemory(pFort->m_strMasterName, sizeof(pFort->m_strMasterName));
	strncpy(pFort->m_strMasterName, pGuild->m_strMasterName, nLen);

	ReleaseGuild();				// 秦力...

	nLen = CHAR_NAME_LENGTH;

	for(i = 0; i < GUILDFORTRESS_ATTACK_MAX_NUM; i++)
	{
		if(pFort->m_arAttackGuild[i].lGuild == m_dwGuild)
		{
			pFort->m_arAttackGuild[i].lGuild = pFort->m_iGuildSid;
			::ZeroMemory(pFort->m_arAttackGuild[i].strGuildName, CHAR_NAME_LENGTH + 1);		// 傍拜螟栏肺 官操绊
			strncpy(pFort->m_arAttackGuild[i].strGuildName, pFort->m_strGuildName, nLen);

			pFort->m_iGuildSid = m_dwGuild;
			::ZeroMemory(pFort->m_strGuildName, CHAR_NAME_LENGTH + 1);						// 林牢 捞抚 官操绊
			strncpy(pFort->m_strGuildName, m_strGuildName, nLen);
			break;
		}
	}

	CString strMsg = _T(""); 
	strMsg.Format( IDS_USER_GET_FORTRESS_SUCCESS, m_strGuildName);//%s 军团临时获得要塞
	SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);

	for(i = 0; i < MAX_USER; i++)
	{
		type = 0;
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;		

		type = pUser->CheckInvalidMapType();
		if((type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16 || type == 17|| type == 18) &&( m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING) ) //战区内回程
		{												//辨靛傈捞 老绢唱绰 惑痢瘤开捞搁
			if(pFort->m_iGuildSid != pUser->m_dwGuild)
			{
				pUser->TownPotal();				
				pUser->SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ME);
				SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
			}
		}
	}

	CBufferEx TempBuf;

	CNpc *pNpc = NULL;

	TempBuf.Add(GUILD_FORTRESS_NCIRCLE);
	TempBuf.Add((BYTE)0x01);
	TempBuf.Add((int)pFort->m_sFortressID);
    //恢复能量球为新的
	for(i = 0; i < FORTRESS_TARGET_MAX_NUM; i++)		// N_Circle甫 檬扁拳秦辑 促澜阑 扁距...
	{
		pNpc = GetNpc(pFort->m_arFortressTarget[i].sTargertID);

		if(!pNpc) continue;

		pNpc->m_tNCircle = NPC_NCIRCLE_DEF_STATE;
		pNpc->m_byColor = 0;
		pNpc->m_sHP = pNpc->m_sMaxHP;
		pNpc->m_lFortressState = 0;

		TempBuf.Add((int)(pNpc->m_sNid + NPC_BAND));
		//		pNpc->SendFortressNCircleColor(m_pCom);				// Color啊 盔惑 汗蓖凳阑 舅赴促.
	}

	for(i = 0; i < pFort->m_arRepairNpcList.GetSize(); i++)
	{
		pNpc = GetNpc(pFort->m_arRepairNpcList[i]);

		if(!pNpc) continue;

		if(pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR)
		{
			pNpc->m_sHP = pNpc->m_sMaxHP;
			pNpc->m_bFirstLive = TRUE;
			break;
		}
	}

	SendInsight(TempBuf, TempBuf.GetLength());
	// 唱吝俊 Agent俊 器飘府胶甫 持磊!!!
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	DB俊 UPDATE茄促.(脚没辨靛父 坷肺瘤 抛捞喉俊 眠啊茄促)
//
BOOL USER::UpdateFortress(CGuildFortress *pFort)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_FORTRESS_WARLIST(%d,%d,\'%s\')}"), 
	pFort->m_sFortressID, m_dwGuild, m_strGuildName);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild_Store Data Only!!\n");

		//g_DBNew[m_iModSid].ReleaseDB(db_index);
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
			BREAKPOINT();

			g_DBNew[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	DB俊 弥绊乏 EVENT肺 殿废茄促
//
void USER::UpdateHighEventLevelUser(short sLevel)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call ADD_EVENT_USER(\'%s\',%d)}"), m_strUserID, sLevel);

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild_Store Data Only!!\n");

		//g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
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
			BREAKPOINT();

			g_DBNew[m_iModSid].ReleaseDB(db_index);
			return;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return;
}

void USER::SetTempStoreDN(DWORD dwDN, int sid)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call TEMP_MONEY(%d,%d)}"), sid, dwDN);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild_Store Data Only!!\n");

		//g_DB[m_iModSid].ReleaseDB(db_index);
		return;
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
			BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess ) return;
}

void USER::AddressWindowOpen(int sid, int quality)
{
	CBufferEx TempBuf;
	
	if( sid == NPC_EVENT_FLOWER )
	{
		TempBuf.Add(ADDRESS_WINDOW_OPEN_NEW);
	}
	else
	{
		TempBuf.Add(ADDRESS_WINDOW_OPEN);
	}
	TempBuf.Add( (short)sid );
	TempBuf.Add( (short)quality );

	Send(TempBuf, TempBuf.GetLength());
}

void USER::RecvAddress(TCHAR *pBuf)
{
	int index = 0;

	char strAddress[512];
	CString			str;
	SYSTEMTIME time;
	GetLocalTime(&time);

	memset( strAddress, NULL, 512 );

	int length = (int)GetByte( pBuf, index );

	if( length < 0 || length > 512 ) { SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME); return; }

	GetString( strAddress, pBuf, length, index );

	int sid = GetShort( pBuf, index );
	int quality = GetShort( pBuf, index );

	// 采官备聪 捞亥飘 梅啊
	if( sid != 634 && sid != 773 ) { SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME); return; }

	char strTitle[128];
	
	if( sid == 634 )
	{
		if( quality == 202 )
			sprintf( strTitle, _ID(IDS_USER_MUNHWA_PRESENT) );
		else if( quality == 201 )
			sprintf( strTitle, _ID(IDS_USER_DEPT_PRESENT) );
		else
		{
			SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME); return;
		}
	}
	else if( sid == 773 )
	{
		sprintf( strTitle, _ID(IDS_USER_FLOWER_PRESENT) );
	}
	else
	{
		SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME); return;
	}

	str.Format("(%04d-%02d-%02d %02d:%02d:%02d) %s - Flower change event applied\r\n",
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,	m_strUserID );

	EnterCriticalSection( &m_CS_EventItemLogFileWrite );
	g_fpEventItem.Write( str, str.GetLength() );
	LeaveCriticalSection( &m_CS_EventItemLogFileWrite );

//	if( !EventRobItem( sid, quality ) )
//	{
//		SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME); return;
//	}

	SDWORD sTitleLen	= _tcslen(strTitle);
	SDWORD sContentLen	= _tcslen(strAddress);
	SDWORD sIDLen		= _tcslen(m_strUserID);

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	int bbsnum = 2;		// 捞亥飘侩 霸矫魄

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_WRITE ( %d, ?, ?, ? )}" ), bbsnum );

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) { SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME); return; }

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Write BBS (BBS:%d,Writer:%s,Title:%d) !!\n", bbsnum, m_strUserID, strTitle);

//		g_DBNew[m_iModSid].ReleaseDB(db_index);

		SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME);
		return;
	}

	int i = 1;
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)m_strUserID,	0, &sIDLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 50,		0, (TCHAR*)strTitle,	0, &sTitleLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 5000,	0, (TCHAR*)strAddress,	0, &sContentLen );

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

		SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME);
		return;
	}
	
	SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	str.Format("(%04d-%02d-%02d %02d:%02d:%02d) %s - Flower change event logged to bbs\r\n",
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,	m_strUserID );

	EnterCriticalSection( &m_CS_EventItemLogFileWrite );
	g_fpEventItem.Write( str, str.GetLength() );
	LeaveCriticalSection( &m_CS_EventItemLogFileWrite );

	if( !EventRobItem( sid, quality ) )
	{
		SendSystemMsg( IDS_USER_REG_ADDR_FAIL, SYSTEM_NORMAL, TO_ME); return;
	}

	str.Format("(%04d-%02d-%02d %02d:%02d:%02d) %s - Flower change event completed\r\n",
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,	m_strUserID );

	EnterCriticalSection( &m_CS_EventItemLogFileWrite );
	g_fpEventItem.Write( str, str.GetLength() );
	LeaveCriticalSection( &m_CS_EventItemLogFileWrite );

	SendSystemMsg( IDS_USER_REG_ADDR, SYSTEM_NORMAL, TO_ME);

	return;
}

void USER::GetResource()
{
	if(m_tIsOP == 0) return;

	//
	int nResourceFreeCount = 0;
	int CurUserCount = 0;
	int acceptedCount = 0;
	int connectedCount = 0;
	int disconnectedCount = 0;
	int logoutCount = 0;
	
	for ( int i = 0; i < MAX_USER; i++ )
	{
		if ( g_pUQM->m_pResources->IsFree( i ) == true )
		{
			nResourceFreeCount++;
			continue;
		}

		USER *pUser = (USER*)g_pUQM->m_pResources->GetDataValue( i );
		if ( pUser )
		{
			if ( pUser && pUser->m_state == STATE_GAMESTARTED )
				CurUserCount++;						
			else if( pUser && pUser->m_state == STATE_ACCEPTED )
				acceptedCount++;
			else if( pUser && pUser->m_state == STATE_CONNECTED )
				connectedCount++;
			else if( pUser && pUser->m_state == STATE_DISCONNECTED )
				disconnectedCount++;
			else if( pUser && pUser->m_state == STATE_LOGOUT )
				logoutCount++;
		}
	};

	int nLeftResource = g_pUQM->FreeResourcesLeft();
	//
//	sprintf( msg, _ID(IDS_USER_SYSTEM_MSG03), CurUserCount, nLeftResource, nResourceFreeCount );

	TCHAR strTitle[256];
	::ZeroMemory(strTitle, sizeof(strTitle));
	wsprintf(strTitle, _ID(IDS_USER_SYSTEM_MSG04), CurUserCount, acceptedCount, connectedCount, disconnectedCount, logoutCount, nLeftResource, nResourceFreeCount);
	
	SendSystemMsg(strTitle, SYSTEM_NORMAL, TO_ME);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	辨傈阑 脚没茄 傈眉 辨靛 格废阑 焊咯霖促.(傍己傈父)
//
void USER::SendFortressAttackGuildList(TCHAR *pBuf)
{
	int i, j;
	short nCount = 0;
	short sFortID = 0;

	BOOL bSuc = FALSE;

	CGuild *pGuild = NULL;
	CStore *pStore = NULL;
	CGuildFortress *pFort = NULL;
	CGuildFortress *pTempFort = NULL;

	int index = 0;
	sFortID = GetShort(pBuf, index);

	if(m_tIsOP != 1)
	{
		if(m_dwGuild <= 0) return;
		if(!m_bGuildMaster)
		{
			SendSystemMsg( IDS_USER_NOT_GUILD_MASTER, SYSTEM_ERROR, TO_ME);
			return;
		}
		if(sFortID != 1000 && sFortID != 1001 && sFortID != 1002)
		{
			SendSystemMsg( IDS_USER_UNCLEAR_FORTRESS, SYSTEM_ERROR, TO_ME);
			return;
		}

		for(i = 0; i < g_arGuildFortress.GetSize(); i++)
		{
			if(g_arGuildFortress[i] == NULL) continue;

			pTempFort = g_arGuildFortress[i];
			if(pTempFort->m_sFortressID != sFortID) continue;

			if(pTempFort->m_iGuildSid == m_dwGuild) 
			{
				bSuc = TRUE;
				break;
			}

			for(j = 0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
			{
				if(pTempFort->m_arAttackGuild[j].lGuild == m_dwGuild)
				{
					bSuc = TRUE;
					break;
				}
			}

			if(bSuc) break;
		}
	}
	else bSuc = TRUE;

	if(!bSuc) 
	{
		SendSystemMsg( IDS_USER_NOT_APPLY_GUILD, SYSTEM_ERROR, TO_ME);
		return;
	}

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		pFort = g_arGuildFortress[i];
		if(!pFort) continue;

		nCount = 0;
		if(pFort->m_sFortressID == sFortID)
		{
			if(pFort->m_lUsed == 1)
			{
				SendSystemMsg( IDS_USER_CANT_USE_IN_DEFENCE, SYSTEM_ERROR, TO_ME);
				return;
			}

			CBufferEx TempBuf, AddData;

			TempBuf.Add(CHALLENGE_GUILD);			
			
			for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
			{
				if(pFort->m_arAttackGuild[j].lUsed == 1)
				{
					AddData.AddString(pFort->m_arAttackGuild[j].strGuildName);
					nCount++;
				}
			}

			TempBuf.Add(nCount);
			TempBuf.AddData(AddData, AddData.GetLength());

			Send(TempBuf, TempBuf.GetLength());
			return;
		}
	}

	if(m_tIsOP == 1) SendSystemMsg( IDS_USER_UNKNOWN_ERROR, SYSTEM_ERROR, TO_ME);
}

void USER::SendRepairItem(int sid)
{
	int i, j;
	CNpc *pNpc = NULL;
	CGuild *pGuild = NULL;
	CGuildFortress *pFort = NULL;

	if(!m_dwGuild) return;

	pFort = GetFortress(sid);

	if(!pFort) return;
	if(!pFort->m_bHaveGuild) return;
	if(pFort->m_iGuildSid != m_dwGuild) return;

	if(pFort->m_lUsed == 1)
	{
		SendSystemMsg( IDS_USER_CANT_USE_IN_DEFENCE, SYSTEM_ERROR, TO_ME);
		return;
	}

	if(pFort->m_dwRepairCost <= 0) 
	{
		SendSystemMsg( IDS_USER_NO_MORE_REPAIR, SYSTEM_ERROR, TO_ME);
		return;
	}

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)
	{
		ReleaseGuild();
		return;
	}

	int nLen = strlen(m_strUserID);

	if( nLen <= 0 || nLen > CHAR_NAME_LENGTH ) 
	{
		ReleaseGuild();
		return;
	}

	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0)
	{
		ReleaseGuild();
		SendSystemMsg( IDS_USER_ONLY_GUILD_MASTER_USE, SYSTEM_ERROR, TO_ME);
		return;
	}

	ReleaseGuild();

/*	if(InterlockedCompareExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)1, (LONG)0) == 0)
	{
		m_dwGuildDN = 0;
		InitGuildItem();								// 函荐甫 柄昌捞 技殴茄促.

		if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) 
		{ 
			InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0); 
			SendSystemMsg( IDS_USER_FAIL, SYSTEM_ERROR, TO_ME);
			return; 
		}

		if(!LoadGuildWarehouse())						
		{ 
			InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0); 
			SendSystemMsg( IDS_USER_FAIL, SYSTEM_ERROR, TO_ME);
			return; 
		}

		if(m_dwGuildDN < pFort->m_dwRepairCost)
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_REPAIR_DINA, SYSTEM_ERROR, TO_ME);
			return;
		}

		m_dwGuildDN -= pFort->m_dwRepairCost;

		UpdateGuildWarehouse();
		InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0); 
*/
		if(m_dwDN < pFort->m_dwRepairCost)
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_REPAIR_HAVEDINA, SYSTEM_ERROR, TO_ME);
			return;
		}
		else m_dwDN -= pFort->m_dwRepairCost;
//		if(m_dwDN < 0) m_dwDN = 0;

		UpdateUserItemDN();
		SendMoneyChanged();

		pFort->m_dwRepairCost = 0;

		for(i = 0; i < pFort->m_arRepairNpcList.GetSize(); i++)
		{
			pNpc = GetNpc(pFort->m_arRepairNpcList[i]);

			if(!pNpc) continue;

			for(j = 0; j < GUILD_REPAIR_MAX_NUM; j++)
			{			
				if(pFort->m_arRepairDBList[j].sUid == pNpc->m_sEZone)
				{
					pNpc->m_sHP = pNpc->m_sMaxHP;
					pNpc->m_tRepairDamaged = NPC_NON_REPAIR_STATE;
					pFort->m_arRepairDBList[j].sHP = pNpc->m_sMaxHP;
					break;
				}
			}
		}

		SendSystemMsg( IDS_USER_REPAIR_COMPLETED, SYSTEM_NORMAL, TO_ME);
		UpdateMemRepairNpc(sid);

}

void USER::SendPeopleSay(int sid)
{
	int degree = 0, say = -1;
	CGuildFortress *pFort = NULL;

	if(!m_dwGuild) return;

	pFort = GetFortress(sid);

	if(!pFort) return;

	if(!pFort->m_bHaveGuild) return;
	if(pFort->m_iGuildSid != m_dwGuild) return;

//	degree = pFort->m_iCityDegree + pFort->m_tTaxRate;
	degree = pFort->GetTotalCityValue();
	
	if(degree <= 20)
	{
		if(sid == 1000) say = 135;
		else if(sid == 1001) say = 155;
		else if(sid == 1002) say = 308;
	}
	else if(degree <= 40)
	{
		if(sid == 1000) say = 136;
		else if(sid == 1001) say = 156;
		else if(sid == 1002) say = 309;
	}
	else if(degree <= 60)
	{
		if(sid == 1000) say = 137;
		else if(sid == 1001) say = 157;
		else if(sid == 1002) say = 310;
	}
	else
	{
		if(sid == 1000) say = 138;
		else if(sid == 1001) say = 158;
		else if(sid == 1002) say = 311;
	}

	if(say >= 0) SendNpcSay(NULL, say);
}

void USER::UpdateMemRepairNpc(int iSid)
{
	int nSize, i;
	CGuildFortress *pFort = NULL;

	if(iSid >= FORTRESS_BAND)
	{
		nSize = g_arFortressSharedMemory.GetSize();
		CSharedMemory* pShared = NULL;
		CMemFortress* pData = NULL;

		pFort = GetFortress(iSid);
		if(!pFort) return;

		if(!pFort->m_bHaveGuild || pFort->m_iGuildSid != m_dwGuild) return;

		for(i = 0; i < nSize; i++)
		{
			pShared = g_arFortressSharedMemory[i];
			
			if(pShared == NULL) return;
			if(pShared->m_hMapping == NULL) return;
			
			pData = (CMemFortress*) pShared->m_lpData;
			
			if(pData == NULL) return;
			if(pData->m_sFortressID <= 0) return;
			if(pData->m_iGuildSid != m_dwGuild) return;
			
			pFort->FortressRepairListToStr(pData->m_RepairList);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	辨傈阑 脚没茄 .(傍己傈父)
//
void USER::UpdateMemAttackListNpc(int iSid)
{
/*	int nSize, i;
	CGuildFortress *pFort = NULL;

	if(iSid >= FORTRESS_BAND)
	{
		nSize = g_arFortressSharedMemory.GetSize();
		CSharedMemory* pShared = NULL;
		CMemFortress* pData = NULL;

		pFort = GetFortress(iSid);
		if(!pFort) return;

		for(i = 0; i < nSize; i++)
		{
			pShared = g_arFortressSharedMemory[i];
			
			if(pShared == NULL) continue;
			if(pShared->m_hMapping == NULL) continue;
			
			pData = (CMemFortress*) pShared->m_lpData;
			
			if(pData == NULL) continue;
			if(pData->m_sFortressID <= 0) continue;
			if(pData->m_sFortressID != pFort->m_sFortressID) continue;
			
			pFort->GuildAttListToStr(pData->m_AttackList);
			break;
		}
	}
*/
}

BOOL USER::CheckInGuildWarring()
{
	int i, j;

	CStore *pStore = NULL;
	CGuildFortress *pFort = NULL;

	// 傍己傈老锭...
	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		pFort = g_arGuildFortress[i];
		if(!pFort) continue;

		if(g_arGuildFortress[i]->m_lUsed == 1)
		{
			if(pFort->m_iGuildSid == m_dwGuild)	// 规绢螟牢瘤
			{
				SendSystemMsg( IDS_USER_NO_PERMISSION_IN_WAR, SYSTEM_ERROR, TO_ME);
				return TRUE;
			}

			for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)// 傍拜螟牢瘤..
			{
				if(pFort->m_arAttackGuild[j].lGuild == m_dwGuild)
				{
					SendSystemMsg( IDS_USER_NO_PERMISSION_IN_WAR, SYSTEM_ERROR, TO_ME);
					return TRUE;
				}
			}
		}
	}
	// 辨靛惑痢...
/*	for(i = 0; i < g_arStore.GetSize(); i++)
	{
		pStore = g_arStore[i];
		if(!pStore) continue;

		if(g_arStore[i]->m_lUsed == 1)
		{
			if(pStore->m_iGuildSid == m_dwGuild)	// 规绢螟牢瘤
			{
				SendSystemMsg( IDS_USER_NO_PERMISSION_IN_WAR, SYSTEM_ERROR, TO_ME);
				return TRUE;
			}

			for(j =0; j < GUILD_ATTACK_MAX_NUM; j++)// 傍拜螟牢瘤..
			{
				if(pStore->m_arAttackGuild[j] == m_dwGuild)
				{
					SendSystemMsg( IDS_USER_NO_PERMISSION_IN_WAR, SYSTEM_ERROR, TO_ME);
					return TRUE;
				}
			}
		}
	}
*/
	// Virtual Room俊 乐阑锭
	for(i = 0; i < g_arGuildHouseWar.GetSize(); i++)
	{
		if(!g_arGuildHouseWar[i]) continue;

		if(g_arGuildHouseWar[i]->m_CurrentGuild.lUsed == 1)
		{
			if(g_arGuildHouseWar[i]->m_CurrentGuild.lGuild == m_dwGuild)
			{
				SendSystemMsg( IDS_USER_NO_PERMISSION_IN_WAR, SYSTEM_ERROR, TO_ME);
				return TRUE;
			}
		}
	}

	return FALSE;
}

int USER::SocketDisConnect()
{
	BYTE len;
	LOGINOUTTHREADDATA *pLIOTD;
	pLIOTD = new LOGINOUTTHREADDATA;
	pLIOTD->UID = m_Sid;
	len = (BYTE)strlen(m_strUserID);
	memcpy(pLIOTD->ID, &len, sizeof(BYTE) );
	memcpy(pLIOTD->ID+sizeof(BYTE), m_strUserID, len);
	pLIOTD->ID[sizeof(BYTE)+len] = '\0';

	EnterCriticalSection( &m_CS_LogoutData );
	RecvLogoutData.AddTail(pLIOTD);
	nLogoutDataCount = RecvLogoutData.GetCount();
	LeaveCriticalSection( &m_CS_LogoutData );

	return 1;
}

void USER::CheckGuildUserInFortress()
{
	int i, j;
	int ilen = 0;

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i] || !g_arGuildFortress[i]->m_bHaveGuild) continue;

		if(g_arGuildFortress[i]->m_iGuildSid == m_dwGuild)
		{
			for(j = 0; j < MAX_GUILD_USER; j++)
			{
				ilen = strlen(g_arGuildFortress[i]->m_arCityRankList[j].strUserID);
				if(ilen <= 0 || ilen > CHAR_NAME_LENGTH) continue;

				if(strcmp(m_strUserID, g_arGuildFortress[i]->m_arCityRankList[j].strUserID) == 0)
				{
					InterlockedExchange(&g_arGuildFortress[i]->m_arCityRankList[j].lCityRank, (long)m_sCityRank);
					break;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//	辨靛俊 啊涝且锭 矫刮殿鞭 荐摹甫 盎脚茄促.
//
void USER::AddGuildUserInFortress(USER *pUser)
{
	int i, j;

	if(!pUser) return;

	int nLen = strlen(pUser->m_strUserID);

	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i] || !g_arGuildFortress[i]->m_bHaveGuild) continue;

		if(g_arGuildFortress[i]->m_iGuildSid == m_dwGuild)
		{
			for(j = 0; j < MAX_GUILD_USER; j++)
			{
				if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_arCityRankList[j].lUsed, (long)1, (long)0) == (long)0)
				{
					::ZeroMemory(g_arGuildFortress[i]->m_arCityRankList[j].strUserID, sizeof(g_arGuildFortress[i]->m_arCityRankList[j].strUserID));
					g_arGuildFortress[i]->m_arCityRankList[j].lCityRank = (long)pUser->m_sCityRank;
					strncpy(g_arGuildFortress[i]->m_arCityRankList[j].strUserID, pUser->m_strUserID, nLen);
					return;
				}
			}		
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//	辨靛俊辑 碍硼唱 呕硼且锭 矫刮殿鞭 荐摹甫 盎脚茄促.
//
void USER::DelGuildUserInFortress(TCHAR *strUserID, int iGuild)
{
	int i, j;
	int ilen = 0;

	if(iGuild <= 0) return;

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i] || !g_arGuildFortress[i]->m_bHaveGuild) continue;

		if(g_arGuildFortress[i]->m_iGuildSid == iGuild)
		{
			for(j = 0; j < MAX_GUILD_USER; j++)
			{
				ilen = strlen(g_arGuildFortress[i]->m_arCityRankList[j].strUserID);
				if(ilen <= 0 || ilen > CHAR_NAME_LENGTH) continue;

				if(strcmp(strUserID, g_arGuildFortress[i]->m_arCityRankList[j].strUserID) == 0)
				{
					::ZeroMemory(g_arGuildFortress[i]->m_arCityRankList[j].strUserID, sizeof(g_arGuildFortress[i]->m_arCityRankList[j].strUserID));
					g_arGuildFortress[i]->m_arCityRankList[j].lCityRank = 0;
					InterlockedExchange(&g_arGuildFortress[i]->m_arCityRankList[j].lUsed, (long)0);
					break;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//	郴啊 磷篮 磊府啊 茄芒 傍己傈捞 国绢瘤绰 磊府搁 穿备唱 促 度鞍捞 犬伏甫 利侩罐绰促.
//
BOOL USER::CheckGuildWarArea()
{
	int mapindex = 0;
	int index = 0;
	int type = CheckInvalidMapType();

	mapindex = GetGuildMapIndex(type);
	if(mapindex >= 0 && mapindex < g_arMapTable.GetSize())
	{
		index = g_arMapTable[mapindex]->m_sStoreID;

		if(index >= FORTRESS_BAND)
		{
			CGuildFortress* pFort = NULL;
			pFort = GetFortress(index);

			if(pFort) 
			{
				if(pFort->m_lUsed == 1 && g_arMapTable[mapindex]->m_sStoreZone)
				{
					return TRUE;
				}
			}
		}
/*		else 
		{
			CStore *pStore = NULL;

			index = g_arMapTable[mapindex]->m_sStoreIndex;

			pStore = GetStore(index);

			if(pStore)
			{
				if(pStore->m_lUsed == 1)								// 秦寸 加己 甘捞 辨靛傈 惑怕	
				{
					return TRUE;
				}
			}
		}
*/	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//	器飘府胶 寇己巩阑 凯绊 摧绰促.
//
void USER::OpenFortressDoor()
{
	if(m_dwGuild <= 0 || !m_bGuildMaster) return;
	
	CNpc *pNpc = NULL;

	for(int i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i] || !g_arGuildFortress[i]->m_bHaveGuild) continue;

		if(g_arGuildFortress[i]->m_lUsed == 1) continue;
		if(g_arGuildFortress[i]->m_lViolenceUsed == 1) continue;

		if(g_arGuildFortress[i]->m_iGuildSid == m_dwGuild)
		{
			for(int j = 0; j < g_arGuildFortress[i]->m_arRepairNpcList.GetSize(); j++)
			{
				pNpc = GetNpc(g_arGuildFortress[i]->m_arRepairNpcList[j]);

				if(!pNpc) continue;

				if(pNpc->m_sHP > 0 && pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR)
				{
					pNpc->m_sHP = 0;

					MAP* pMap = g_zone[pNpc->m_ZoneIndex];
					pMap->m_pMap[pNpc->m_sCurX][pNpc->m_sCurY].m_lUser = 0;

					pNpc->m_NpcState = NPC_DEAD;

					pNpc->m_Delay = pNpc->m_sRegenTime;
					pNpc->m_bFirstLive = FALSE;

					pNpc->SetMapAfterGuildWar();

					pNpc->SendDead(m_pCom);
					return;
				}
				else
				{
					if(pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR)
					{
						pNpc->m_sHP = pNpc->m_sMaxHP;
						pNpc->m_bFirstLive = TRUE;
						return;
					}			
				}
			}
		}
	}
}

ZONEINFO* USER::GetZoneInfo(int zone)
{
	ZONEINFO* pZoneInfo = NULL;

	for( int i = 0; i < g_TownPotal.GetSize(); i++ )
	{
		if( g_TownPotal[i] )
		{
			pZoneInfo = g_TownPotal[i];

			if( pZoneInfo->iZone == zone )
			{
				return pZoneInfo;
			}
		}
	}

	return NULL;
}

void USER::UpdateCurrentUserTable()
{
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	// session init
	m_bSessionLogOut = FALSE;
	m_iDisplayType = 0;			// 皋技瘤 鸥涝
	m_iValidTime = 0;			// 蜡瓤 矫埃
	m_iTimeInterval = 50;		// 皋技瘤 埃拜

	BYTE bDisplay = 0;
	int remainTime = 0;
	TIMESTAMP_STRUCT endTime;

	SWORD	sParam1 = 0;
	SDWORD	cbParam1 = SQL_NTS, cbParam2 = SQL_NTS, cbParam3 = SQL_NTS, cbParam4 = SQL_NTS;

	endTime.year = 0;
	endTime.month = 0;
	endTime.day = 0;
	endTime.hour = 0;
	endTime.minute = 0;

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_CURRENTUSER (\'%s\', \'%s\',?, ?,?,?)}"), m_strAccount, g_arServerIPAddr, sParam1,remainTime, endTime, bDisplay);

	hstmt = NULL;
	retcode = 0;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) 
	{
		SoftClose();	// 肺弊 酒眶 例瞒俊 甸绢埃促.
		return;
	}
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		SoftClose();	// 肺弊 酒眶 例瞒俊 甸绢埃促.
		return;
	}
	
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		SoftClose();	// 肺弊 酒眶 例瞒俊 甸绢埃促.
		return ;
	}

	retcode = SQLBindParameter(hstmt, 1,SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT,0,0,&sParam1,0,&cbParam1);
	retcode = SQLBindParameter(hstmt, 2,SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER,0,0,&remainTime,0,&cbParam2);
	retcode = SQLBindParameter(hstmt, 3,SQL_PARAM_OUTPUT, SQL_C_TIMESTAMP, SQL_TIMESTAMP,0,0,&endTime,0,&cbParam3);
	retcode = SQLBindParameter(hstmt, 4,SQL_PARAM_OUTPUT, SQL_C_UTINYINT, SQL_TINYINT,0,0,&bDisplay,0,&cbParam4);


    retcode = SQLExecDirect (hstmt, (unsigned char *)(LPCTSTR)szSQL, SQL_NTS);
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

	if(sParam1 != 1)
	{
		SoftClose();	// 肺弊 酒眶 例瞒俊 甸绢埃促.
		return;
	}

	GetLocalTime(&m_LoginTime);		// 肺弊牢茄 矫埃阑 汲沥
	m_bSessionLogOut = FALSE;
	m_iTimeInterval = (int)remainTime;// 钎矫甫 困秦
	m_iDisplayType = (int)bDisplay;	// 霸烙规牢瘤 沥咀力牢瘤 矫埃力牢瘤 钎矫
	m_iValidTime = (int)remainTime;	

	//----------------------------------------------------------------------------------------------
	//yskang 0.5 骏霸烙 荤侩磊绰 1焙 2焙俊辑 公丰肺 荤侩且 荐 乐促.
	//----------------------------------------------------------------------------------------------
	m_bMGame = FALSE;//骏霸烙 荤侩磊啊 酒聪促肺 檬扁拳
	if(m_iDisplayType > 200)
	{
		m_iDisplayType -= 200;//骏霸烙 荤侩磊绰 鸥涝俊 200捞 歹秦廉 乐促.
		m_bMGame = TRUE;//骏霸烙 荤侩磊捞绊 1焙 2焙辑滚 捞促.
	}
	
	if(m_iDisplayType == 1 || m_iDisplayType == 3 || m_iDisplayType == 5) // 扁埃力扼搁 
	{
		m_validDate.wYear = endTime.year;
		m_validDate.wMonth = endTime.month;
		m_validDate.wDay = endTime.day;
		m_validDate.wHour = endTime.hour;
		m_validDate.wMinute = endTime.minute;
	}
	GetCheckValidTime();			// 矫埃阑 钎矫茄促.
	//-------------------------------------------------------------------------------------------------

	TRACE("SESSION DB UPDATE SUCCESS : %s\n", m_strAccount);

	return;
}

void USER::GetCheckValidTime()
{
 /*   CString strMsg = _T("");
	CString strMsgg = _T("");
	int temptime = (3600 * 1000);
	int hour = 0,min = 0;
    if(m_dwHtExpTime > 0)
	{
		hour = m_dwHtExpTime/temptime;	min = m_dwHtExpTime%temptime/(60000);
		strMsgg.Format( "幻想灵石剩余时间:%d小时%d分",hour,min);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsgg, SYSTEM_NPC, TO_ME);
	}
    if(m_dwMagicFtTime > 0)
	{
		hour = m_dwMagicFtTime/temptime;	min = m_dwMagicFtTime%temptime/(60000);
		strMsgg.Format( "幻想晶石剩余时间:%d小时%d分",hour,min);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsgg, SYSTEM_NPC, TO_ME);
	}
/*	if((m_bMGame == TRUE) && (m_iDisplayType == 5)) // yskang 0.5 骏霸烙 1焙 2焙 荤侩磊 贸府 矫埃 拌魂阑 窍瘤 臼绰促.
		return;
	// 泅犁 立加吝牢 拌沥狼 矫埃, 扁埃阑 舅妨霖促.
	SYSTEMTIME currTime;
	GetLocalTime(&currTime);

	CTime curr(currTime);

	int tempValue = 0;

	CString strMsg = _T("");

	if(m_iDisplayType == 1 || m_iDisplayType == 3)
	{
		CTimeSpan DiffDay;
		CTime validDay(m_validDate);

		DiffDay = validDay - curr;
		tempValue = (int)DiffDay.GetDays();

		if(tempValue <= 3)
		{
			if(tempValue == 0)
			{
				strMsg.Format( IDS_USER_UNTIL_TODAY_YOUR_GAMETIME );
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);
			}
			else
			{
				strMsg.Format( IDS_USER_REMAIN_ACCOUNT_USE, tempValue);
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);
			}
			return;
		}	
	}
	else if(m_iDisplayType == 2 || m_iDisplayType == 4)
	{
		if(m_iDisplayType == 2)
		{
			if(m_iValidTime <= 50) //俺牢
			{
				strMsg.Format( IDS_USER_REMAIN_GAMETIME_MIN, m_iValidTime);
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);
				return;
			}
		}
		else
		{
			if(m_iValidTime <= 3000) // 霸烙规
			{
				int ihour = (int)(m_iValidTime / 60);
				int iMin = (int)(m_iValidTime % 60);

				strMsg.Format( IDS_USER_REMAIN_GAMEROOMTIME, ihour, iMin);
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);
				return;
			}
		}
	}
/*	else if(m_iDisplayType == 6)
	{
		strMsg.Format("蜡历丛捞 立加窍脚 眉氰魄 辑滚胶绰 扁瓷惑 力距捞 乐嚼聪促.");
		SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
	}
*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	何辨付甫 烙疙, 鼻茄 捞剧
//
void USER::DeputeGuildMasterOfPower(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	int nLen = 0;

	int uid = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;

	if(!m_bGuildMaster) return; 	// 辨靛 鼻茄捞 绝促.

	BYTE error_code = 0;
	BOOL bRes = TRUE;

	uid = GetInt(pBuf, index);

	pUser = GetUser(uid - USER_BAND);

	if(!pUser || pUser->m_state != STATE_GAMESTARTED) return;	// 蜡历啊 绝促.

	if(IsThereUser(pUser) == FALSE || strcmp(m_strUserID, pUser->m_strUserID) == 0)
	{
		SendSystemMsg( IDS_USER_SEE_EACH_OTHER, SYSTEM_NORMAL, TO_ME);//"互相面对面."
		return;
	}

	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;

	CString strMsg = _T("");
	USER *pGUser = NULL;			// 皋技瘤甫 舅赴促.

	CBufferEx TempBuf;

	if(pUser->m_dwGuild != m_dwGuild)  
	{
		error_code = ERR_9; 
		goto go_result;		
	}// 促弗 辨靛俊 啊涝茄 蜡历 
	
	if(CheckInGuildWarring()) return;// 辨傈吝俊绰 倾遏且荐绝促.

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) 
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_7;			// 秦寸 辨靛啊 绝促.
		goto go_result;				
	}
									// 坷幅...
	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0) 
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_10;		// 
		goto go_result;
	}

	if(pGuild->m_lSubMaster == 1)		// 泅犁 何辨付啊 穿焙捞瘤 舅妨霖促.
	{
		strMsg = _T("");
		strMsg.Format(IDS_USER_GUILD_SUBMASTER, pGuild->m_strSubMasterName);//"你任命了 %s 作为军团副团长."
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		ReleaseGuild();				// 秦力...
		return;
	}

	index = -1;
	index = pGuild->GetUser(pUser->m_strUserID);
	ReleaseGuild();					// 秦力...

	if(index < 0)
	{	
		error_code = ERR_8;			// 
		goto go_result;		
	}


	// Add By Youn Gyu
	g_pMainDlg->BridgeDeputeGuildMasterOfPowerReq( m_uid, pUser->m_uid, m_strUserID, pUser->m_strUserID, (int)m_dwGuild );
//0509

	UpdateGuildSubMaster(pUser->m_strUserID, TRUE);
	pUser->m_bRepresentationGuild = TRUE;
	pGuild->CheckGuildSubMaster();
	pGuild->SetSubGuildMaster( pUser->m_strUserID );
	
	ReleaseGuild();

	

	strMsg = _T("");
	strMsg.Format( IDS_USER_GUILD_SUBMASTER_ON, pUser->m_strUserID);//"%s 被任命担任军团副团长."

	for(int i = 0; i < MAX_USER; i++)
	{
		pGUser = m_pCom->GetUserUid(i);

		if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;		

		if(m_dwGuild == pGUser->m_dwGuild)
		{
			pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}	
	return;
	//


	if(UpdateGuildSubMaster(pUser->m_strUserID, TRUE) <= 0)
	{
		return;
	}
	
	pGuild = GetGuild(m_dwGuild);

	if(pGuild == NULL) 
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_7;			// 秦寸 辨靛啊 绝促.
		goto go_result;				
	}

	pUser->m_bRepresentationGuild = TRUE;
	pGuild->CheckGuildSubMaster();
	pGuild->SetSubGuildMaster(pUser->m_strUserID);

	ReleaseGuild();				// 秦力...
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

	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	鼻茄甫 化绰促.
//
void USER::RemoveGuildMasterOfPower(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

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
	CString strMsg = _T("");
	USER *pGUser = NULL;			// 皋技瘤甫 舅赴促.

	CBufferEx TempBuf;

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
	if(!pGuild->IsMasterPower(m_strUserID))
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_10;		// 
		goto go_result;
	}

	nLen = 0;
	nLen = strlen(pGuild->m_strSubMasterName);
	if(pGuild->m_lSubMaster != 1 || (nLen <= 0 || nLen > CHAR_NAME_LENGTH) )		// 泅犁 何辨付绝促绊 舅妨霖促.
	{
		SendSystemMsg( IDS_USER_GUILD_SUBMASTER_NOT, SYSTEM_ERROR, TO_ME);
		ReleaseGuild();				// 秦力...
		return;
	}

	ReleaseGuild();	

	// Add By Youn Gyu
	g_pMainDlg->BridgeRemoveGuildMasterOfPowerReq( m_uid, m_strUserID, (int)m_dwGuild );

	strMsg = _T("");
	strMsg.Format( IDS_USER_GUILD_SUBMASTER_OFF, pGuild->m_strSubMasterName);// "%s 被取消担任军团副团长."

	UpdateGuildSubMaster(NULL, FALSE) ;

	pGuild->RemoveSubGuildMaster();
	ReleaseGuild();	

	

	for(int i = 0; i < MAX_USER; i++)
	{
		pGUser = m_pCom->GetUserUid(i);

		if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;		

		if(m_dwGuild == pGUser->m_dwGuild)
		{
			pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}	

	return;
	//



	if(UpdateGuildSubMaster(NULL, FALSE) <= 0)
	{
		return;
	}
	
	pGuild = GetGuild(m_dwGuild);

	if(pGuild == NULL) 
	{
		ReleaseGuild();				// 秦力...
		error_code = ERR_7;			// 秦寸 辨靛啊 绝促.
		goto go_result;				
	}

	ReleaseGuild();				// 秦力...

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
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	鼻茄 捞剧甫 诀但
//
int USER::UpdateGuildSubMaster(TCHAR *strSubMaster, BOOL bPower)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		

	::ZeroMemory(szSQL, sizeof(szSQL));

	SWORD	sRet = 0;
	SDWORD	cbRet = SQL_NTS;

	int index = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_SUBMASTER (%d, \'%s\', \'%s\', %d,?)}"), m_dwGuild, m_strUserID, strSubMaster, bPower, sRet); 

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return -1;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return -1;

	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLBindParameter(hstmt, 1,SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT,0,0,&sRet,0,&cbRet);

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if( sRet != 1 ) return FALSE;

	return TRUE;	
}

void USER::EncryptionStartReq(TCHAR *pBuf)
{
	if(m_isCryptionFlag != 0) return;
	
	int index = 0;
	BYTE tType = GetByte(pBuf, index);
	
	if(tType >= 3) return;

	index = 0;
	SetByte(m_TempBuf, ENCRYPTION_START_RESULT, index );
	SetString(m_TempBuf, (char *)&m_public_key, sizeof(_int64), index);
	SetByte(m_TempBuf, tType, index);

	m_isCryptionFlag = 0;

	Send( m_TempBuf, index );

	m_isCryptionFlag = 1;
}

void USER::SetPublicKey()
{
	// IKING 2001.1.
    srand( (unsigned)time( NULL ) );
	//
	BYTE rand1,rand2,rand3,rand4,rand5,rand6,rand7,rand8;

	int out_flag = 0;
	do
	{
		rand1 = rand();
		rand2 = rand();
		rand3 = rand();
		rand4 = rand();
		rand5 = rand();
		rand6 = rand();
		rand7 = rand();
		rand8 = rand();

		m_public_key = rand1;
		m_public_key <<= 8;

		m_public_key |= rand2;
		m_public_key <<= 8;

		m_public_key |= rand3;
		m_public_key <<= 8;

		m_public_key |= rand4;
		m_public_key <<= 8;

		m_public_key |= rand5;
		m_public_key <<= 8;

		m_public_key |= rand6;
		m_public_key <<= 8;

		m_public_key |= rand7;
		m_public_key <<= 8;

		m_public_key |= rand8;

		if(m_public_key != 0)
			out_flag = 1;

	} while( !out_flag );
//#ifdef _CHINA
	//标准日服
//	m_public_key = 0x1010101010101010;// test code
	//国服
	m_public_key = 0x8080808080808080;// test code
//#endif
}

void USER::SendGuildInviteReq(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	BYTE error_code = 0;

	int nLen = 0;

	int uid = 0;
	int iCount = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	BOOL bRes = TRUE;

	if(!m_bGuildMaster && !m_bRepresentationGuild) { error_code = ERR_10; goto go_result; }	// 辨靛 鼻茄捞 绝促.

	uid = GetInt(pBuf, index);

	pUser = GetUser(uid - USER_BAND);

	if(!pUser) { error_code = ERR_3; goto go_result; }				// 蜡历啊 绝促.

	if(IsThereUser(pUser) == FALSE || strcmp(m_strUserID, pUser->m_strUserID) == 0)
	{
		SendSystemMsg( IDS_USER_SEE_EACH_OTHER, SYSTEM_NORMAL, TO_ME);
		return;
	}

	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0) return;

	if(pUser->m_dwGuild > 0)  { error_code = ERR_9; goto go_result; }// 捞固 促弗 辨靛俊 啊涝茄 蜡历 
	
	if(CheckInGuildWarring()) return;								// 辨傈吝俊绰 倾遏罙荐绝促.

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

	index = -1;
	index = pGuild->GetUser(pUser->m_strUserID);
	if(index >= 0) 
	{ 
		ReleaseGuild();				// 秦力...

		pUser->m_dwGuild = m_dwGuild;			// 辨靛 锅龋甫 促矫 悸泼茄促.
		SendGuildInfo(pUser);

		error_code = ERR_9; 
		goto go_result; 
	}

	ReleaseGuild();				// 秦力...
	bRes = FALSE;

go_result:

	CBufferEx TempBuf;

	if(bRes)
	{
		TempBuf.Add(GUILD_INVITE_RESULT);
		TempBuf.Add((BYTE)0x04);		//角菩
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add(GUILD_INVITE_REQ);
	TempBuf.Add(m_uid + USER_BAND);

	pUser->Send(TempBuf, TempBuf.GetLength());
}

void USER::GuildInviteResult(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	BYTE error_code = 0;

	int uid = 0;
	USER* pUser = NULL;

	CBufferEx TempBuf;
	TCHAR szGuildName[CHAR_NAME_LENGTH + 1];
	::ZeroMemory(szGuildName, sizeof(szGuildName));

	BYTE tType = GetByte(pBuf, index);

	uid = GetInt(pBuf, index);
	if(uid < 0 || uid >= INVALID_BAND) { error_code = ERR_3; goto go_result; }

	pUser = GetUser(uid - USER_BAND);								// 惑措规
	if(!pUser) { error_code = ERR_3; goto go_result; }				// 蜡历啊 绝促.

	if(tType == 1)
	{
		if(!pUser->m_bGuildMaster && !pUser->m_bRepresentationGuild) { error_code = ERR_10; goto go_result; }

		pUser->GuildInvite(m_uid + USER_BAND);

		TempBuf.Add(GUILD_INVITE_RESULT);
		TempBuf.Add((BYTE)0x01);

		Send(TempBuf, TempBuf.GetLength());
		pUser->Send(TempBuf, TempBuf.GetLength());
		return;
	}
	else
	{
		TempBuf.Add(GUILD_INVITE_RESULT);
		TempBuf.Add((BYTE)0x03);		// 芭例 林眉 : 3, 乔 林眉 : 2
		Send(TempBuf, TempBuf.GetLength());

		CBufferEx TempBuf1;
		TempBuf1.Add(GUILD_INVITE_RESULT);
		TempBuf1.Add((BYTE)0x02);
		pUser->Send(TempBuf1, TempBuf1.GetLength());
		return;
	}

go_result:														 // 捞 何盒 贸府绰 唱吝俊 促矫 八配
	TempBuf.Add(GUILD_INVITE_RESULT);
	TempBuf.Add((BYTE)0x04);		//角菩
	TempBuf.Add(error_code);
	Send(TempBuf, TempBuf.GetLength());

	if(pUser && pUser->m_state == STATE_GAMESTARTED)
	{
		pUser->Send(TempBuf, TempBuf.GetLength());
	}
}

//-----------------------------------------------------------------------------------------
//--yskang 0.1 辨靛盔俊霸 龋莫阑 何咯茄促.
//称号
//-----------------------------------------------------------------------------------------
void USER::LoveName(TCHAR *pBuf)
{
	//------------------------------------------------------------------------------------------------
	/* 俊矾 内靛
	01 : 鼻茄捞 绝促.
	02 : 茫绰 酒捞叼啊 绝促.
	03 : 鞍篮 辨靛盔捞 酒聪促.
	04 : 龋莫[局莫]捞 肋给登菌促.
	05 : 叼厚俊 历厘 且 荐 绝菌促.
	06 : 垮捞促.
	*/
	//-------------------------------------------------------------------------------------------------
	CBufferEx ResultTempBuf; 
	CBufferEx TempBuf;
	BOOL bSuccess = FALSE;
	BYTE byCommand=(BYTE)1;
	int nLength = 0;
	int uid = 0;
	int index = 0;//硅凯 器牢磐 困摹
	USER* pUser = NULL;
	BYTE error_code = ERR_1;
	byCommand = GetByte(pBuf,index);
	

	//龋莫阑 何咯窍绊磊 窍绰 蜡历甫 茫绰促.
	//--烙矫肺
	uid = GetInt(pBuf, index);
	
	if(uid < USER_BAND || uid >= NPC_BAND) 
		return;
	pUser = GetUser(uid - USER_BAND);		
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_uid==m_uid)//磊脚俊霸 龋莫何咯
	{
		if(m_sLevel<70) { error_code = ERR_1; goto go_result; }//70饭骇 捞窍扼 龋莫阑 何咯且 荐 绝促.		
	}
	else //辨盔俊霸 龋莫何咯
	{
		if(m_sLevel<70) { error_code = ERR_1; goto go_result; } //70饭骇 捞窍扼 龋莫阑 何咯且 荐 绝促.
			
		//龋莫阑 何咯且荐乐绰 磊拜阑 爱眠绊 乐绰啊?
		if(!m_bGuildMaster) 
		{
			error_code = ERR_1; 
			goto go_result; 
		}	// 龋莫 何咯 磊拜捞 绝促.

	}
	if(strcmp(pUser->m_strGuildName,m_strGuildName)!=0)//鞍篮 辨靛盔捞 酒聪扼搁....
	{ error_code = ERR_3; goto go_result; }

	//父距 昏力扼搁
	if(byCommand ==0)
	{
		//龋莫阑 皋葛府 叼厚俊 历厘茄促(?)
		::ZeroMemory(pUser->m_strLoveName,sizeof(pUser->m_strLoveName));
		strcpy(pUser->m_strLoveName,"");
		
		lstrcpy(pUser->m_pMD->m_strLoveName , pUser->m_strLoveName);
		//龋莫阑 促弗 荤恩甸俊霸 舅赴促(鞍篮 胶农赴俊 乐绰荤恩甸俊 茄秦辑?)
		TempBuf.Add(LOVE_NAME);
		TempBuf.Add(uid);
		TempBuf.AddString(pUser->m_strLoveName);
		pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
		bSuccess = TRUE;
		goto go_result;
	}

	if(byCommand>1) return;

	//菩哦俊辑 龋莫阑 惶酒辰促.
	char strTemp[1024];
	//char strTemp1[1024];
	nLength = GetVarString(sizeof(strTemp), strTemp, pBuf, index);
	if(nLength < 1 || nLength > CHAR_NAME_LENGTH) // 肋给等 龋莫
	{
		//ZeroMemory(pUser->m_strLoveName,sizeof(pUser->m_strLoveName));
		error_code = ERR_4;  goto go_result; 
	}
	if(IsReservedLoveName(strTemp)) { error_code = ERR_6; goto go_result; }//限制的字符无法设为称号
		
	if(!UNI_CHAR::CheckString(strTemp)) {error_code=ERR_6; goto go_result;} //设定以外的语言不能设为称号
   // strcpy_s(strTemp1,strTemp);
	
//	SendSystemMsg("服务器排名：100：头顶显示状态：开",SYSTEM_NORMAL,TO_ME);
    //sprintf(strTemp,"@9[LV.%d]@2%s",m_sLevel,strTemp1);//显示等级
	//--------------------------------------------------------------------------------------
	//--yskang 0.6 蜡丰荤侩磊 橇府固决 龋莫阑 荤侩茄促.
	if(pUser->m_iDisplayType != 5 && pUser->m_iDisplayType != 6)
		strcpy(pUser->m_strLoveName,strTemp);
	else {error_code = ERR_1; goto go_result; }
	//---------------------------------------------------------------------------------------

	
	if(pUser->m_bPseudoString==TRUE)
	{

		//------------------------------------------------------------------------------------
		//贸澜 龋莫捞 何咯登绰 巴捞扼搁 叼厚俊 Insert茄促.
		//------------------------------------------------------------------------------------
		index = 0;
		BYTE *pData;
		SQLDATAPACKET *pSDP;
		pSDP = new SQLDATAPACKET;
		pSDP->UID = m_uid;
		pSDP->code = LOVE_NAME_RESULT;
		int Datalength = pSDP->dcount = sizeof(uid);
		pData = new BYTE[Datalength+1];
		memset(pData, 0, Datalength+1);
		memcpy(pData, &uid,sizeof(uid));
		pSDP->pData = pData;

		//------------------------------------------------------------------------------------
		//静饭靛俊辑 贸府茄促.
		//------------------------------------------------------------------------------------
		EnterCriticalSection( &m_CS_SqlData );
		RecvSqlData.AddTail(pSDP);
		nSqlDataCount = RecvSqlData.GetCount();
		LeaveCriticalSection( &m_CS_SqlData );	

		//-------------------------------------------------------------------------------------
		//--叼厚 Insert场
		//-------------------------------------------------------------------------------------
	}
	else
	{

		//龋莫阑 皋葛府 叼厚俊 历厘茄促(?)
		strcpy(pUser->m_pMD->m_strLoveName , pUser->m_strLoveName);
		//龋莫阑 促弗 荤恩甸俊霸 舅赴促(鞍篮 胶农赴俊 乐绰荤恩甸俊 茄秦辑?)
		TempBuf.Add(LOVE_NAME);
		TempBuf.Add(uid);
		TempBuf.AddString(pUser->m_strLoveName);
		pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
		bSuccess = TRUE;
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
}


void USER::UpdateUserDeadLog(TCHAR *pBuf)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	int				index = 0;

	TCHAR			szSQL[1024];
	::ZeroMemory(szSQL, sizeof(szSQL));

	//yskang 0.8 add dead log
	TCHAR			strPKName[100];
	::ZeroMemory(strPKName, sizeof(strPKName));

	DWORD minus = GetDWORD(pBuf, index);
	DWORD preExp = GetDWORD(pBuf, index);
	BYTE nLen = GetByte(pBuf,index);//yskang 0.8 add dead log 
	GetString((char *)strPKName,pBuf,nLen,index);//yskang 0.8  add dead log

	CPoint pt = ConvertToClient(m_curx, m_cury);
																	// id, level,z, x, y, iExp, minus exp, preExp, iCityValue					
	//_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_DEAD_DATA (\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d)}"), m_strUserID,m_sLevel,m_curz,pt.x,pt.y,m_dwExp,minus,preExp,m_iCityValue,strPKName,m_iDisplayType);//yskang 0.8 dead log 荐沥 磷牢荤恩 捞抚, 拌沥 立加 鸥涝 眠啊 
    _sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_DEAD (\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d)}"), m_strUserID,m_sLevel,m_curz,pt.x,pt.y,m_dwExp,minus,preExp,m_iCityValue,strPKName,m_iDisplayType);//yskang 0.8 dead log 荐沥 磷牢荤恩 捞抚, 拌沥 立加 鸥涝 眠啊
	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild Warehouse Data !!\n");

		//g_DB[m_iModSid].ReleaseDB(db_index);
		return;
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

			g_DBNew[m_iModSid].ReleaseDB(db_index);
			return;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
}

void USER::UpdateUserDeadLog(DWORD minus, DWORD preExp, TCHAR *strPKName)
{
	int index = 0;
	TCHAR pBuf[100];

	::ZeroMemory(pBuf, sizeof(pBuf));

	SetDWORD(pBuf, minus, index);
	SetDWORD(pBuf, preExp, index);
	int nLen = strlen(strPKName);
		if(nLen < 1) nLen =1;
	SetByte(pBuf, (BYTE)nLen, index);
	SetString(pBuf,strPKName,nLen,index);
	
	int Datalength;
	BYTE *pData;
	SQLDATAPACKET *pSDP;
	pSDP = new SQLDATAPACKET;
	pSDP->code = DB_USER_DEAD;
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

////////////////////////////////////////////////////////////////////////////////////////
//检查职业物品
BOOL USER::CheckClassItem(BYTE tMyClass, BYTE tNeedClass)
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

	tFire = tFire & tMyClass;
	tEdge = tEdge & tMyClass;
	tStaff = tStaff & tMyClass;
	tBrawl = tBrawl & tMyClass;
	tJudge = tJudge & tMyClass;

	tTemp = tFire^tEdge^tStaff^tBrawl^tJudge;

	if(!tTemp) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////
//	Moon Event Item Use...
//
BOOL USER::MoonEvent(short sSlot)
{
	CString strMsg = _T("");
	short sSid = 0;
	int iRandom = 0, iSummon = 0;
	ItemList Item;
	int iItemNum = 0;
	int iEmptySlot = -1, iSameSlot = -1;
	int i = 0;
	int iDeleteHP = 0;
	BYTE tSlot = 0;
	short sCount = 0;
	CPoint pt;
	BOOL bRet = FALSE;
	int sSlot1=4;

	if(g_iMoonEvent == 0)
	{
		strMsg.Format(IDS_EVENT_END);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		return FALSE;
	}

	if(sSlot < EQUIP_ITEM_NUM || sSlot >= TOTAL_INVEN_MAX) return FALSE;
	
	sSid = m_UserItem[sSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
	if(m_UserItem[sSlot].sCount <= 0) return FALSE;

	switch(sSid)
	{
	case EVENTITEM_SID_MOON:
		if(m_dwAdamantineTime != 0 || m_dwMightyWeaponTime != 0 )
		{
			strMsg.Format(IDS_ANOTHER_PSI_RUN);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			return FALSE;
		}

		SetBerserker(60 * 5);
		SendPsiAttackResult(SUCCESS, m_uid + USER_BAND, 15);
		break;

	case 862: //EVENTITEM_SID_SONGPEON_01
		/*GetExp(87);
		strMsg.Format(IDS_MOON_SONGPEON_01);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);*/
		if(m_UserItem[sSlot1].sSid < 0 ||m_UserItem[sSlot1].sSid >= g_arItemTable.GetSize()) return FALSE;

      			if (m_UserItem[4].sSid==1222)m_UserItem[4].sSid=1581;//龙之翼
				 if (m_UserItem[4].sSid==1223)m_UserItem[4].sSid=1582;//强化龙之翼
				 if (m_UserItem[4].sSid==1224)m_UserItem[4].sSid=1583;//龙之翼-s

				if (m_UserItem[sSlot1].sSid==1216)m_UserItem[sSlot1].sSid=1572;//反I
				 if (m_UserItem[sSlot1].sSid==1217)m_UserItem[sSlot1].sSid=1573;//反II
				 if (m_UserItem[sSlot1].sSid==1532)m_UserItem[sSlot1].sSid=1574;//反III

				if (m_UserItem[sSlot1].sSid==1535)m_UserItem[sSlot1].sSid=1575;//龙神炮x2
				 if (m_UserItem[sSlot1].sSid==1199)m_UserItem[sSlot1].sSid=1576;//龙神炮x4
				 if (m_UserItem[sSlot1].sSid==1221)m_UserItem[sSlot1].sSid=1577;//龙神炮x6

				 if (m_UserItem[sSlot1].sSid==1225)m_UserItem[sSlot1].sSid=1578;//龙神之怒
				 if (m_UserItem[sSlot1].sSid==1226)m_UserItem[sSlot1].sSid=1579;//强化龙神之怒
				 if (m_UserItem[sSlot1].sSid==1227)m_UserItem[sSlot1].sSid=1580;//龙神之怒-S
	
             MakeItemLog( &m_UserItem[4], ITEMLOG_BLESS_UPGRADE_SUCCESS );
			 GetRecoverySpeed();
				 
			
		
		break;
	/*case EVENTITEM_SID_SONGPEON_11:
		GetExp(654);
		strMsg.Format(IDS_MOON_SONGPEON_11);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		break;
	case EVENTITEM_SID_SONGPEON_31:
		GetExp(11185);
		strMsg.Format(IDS_MOON_SONGPEON_31);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		break;*/
	case EVENTITEM_SID_SONGPEON_51://清理杀怪数
		m_dwShopPingDN = 0;
		//GetExp(67550);
		//strMsg.Format(IDS_MOON_SONGPEON_51);
		//SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		break;
	case EVENTITEM_SID_SONGPEON_71://经验月饼
		GetExp(200000000);
		strMsg.Format(IDS_MOON_SONGPEON_71);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		break;

	case EVENTITEM_SID_BOX: case 868:   //月光宝盒
		if(m_curz >= 56 && m_curz <= 59 || m_curz == 1005)
		{
			strMsg.Format(IDS_CANNOT_USE_BOX);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			return FALSE;
		}

		iRandom = myrand(0, 999);
		if(g_arBoxEventTable[iRandom]->m_tType == 1)	// Summoning Monster
		{
			iSummon = myrand(0, g_arSummonTable.GetSize() - 1);
			
			g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_bMove = 1;
			pt = FindNearAvailablePoint_S(m_curx, m_cury);
			g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_bMove = 0;

			if(pt.x == -1 || pt.y == -1) return FALSE;

			bRet = SummonMonster(g_arSummonTable[iSummon]->m_strName, pt);
			return bRet;
		}
		else //if(g_arBoxEventTable[iRandom]->m_tType >= 2 && g_arBoxEventTable[iRandom]->m_tType <= 37)	// Give Item
		{
			iItemNum = g_arBoxEventTable[iRandom]->m_sIid;

			Item.sSid		= g_arItemTable[iItemNum]->m_sSid;
			Item.sLevel		= g_arItemTable[iItemNum]->m_byRLevel;
			Item.sDuration	= g_arItemTable[iItemNum]->m_sDuration;
			Item.sCount		= g_arBoxEventTable[iRandom]->m_sCount;
			Item.sBullNum	= g_arItemTable[iItemNum]->m_sBullNum;
			Item.tIQ		= NORMAL_ITEM;
			Item.iItemSerial= 0;
			for(i = 0; i < MAGIC_NUM; i++) Item.tMagic[i] = 0;

			iSameSlot	=	GetSameItem(Item, INVENTORY_SLOT); 
			iEmptySlot	=	GetEmptySlot(INVENTORY_SLOT);
			
			if(iEmptySlot == -1)	// 牢亥俊 弥家茄 窍唱狼 后傍埃捞 乐绢具 罚待窍霸 酒袍阑 临 荐 乐促.
			{
				strMsg.Format(IDS_INVENTORY_FULL);
				SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
				return FALSE;
			}

			if(Item.sCount == 1 && Item.sDuration > 0 ) MakeItemLog(&Item, ITEMLOG_EVENT_GIVE);

			for(i = 0; i < Item.sCount; i++) m_iCurWeight += g_arItemTable[iItemNum]->m_byWeight;
			GetRecoverySpeed();

			if(iSameSlot != -1) 
			{
				sCount = m_UserItem[iSameSlot].sCount;
				m_UserItem[iSameSlot] = Item;
				m_UserItem[iSameSlot].sCount += sCount;
				tSlot = (BYTE)iSameSlot;
			}
			else if(iEmptySlot != -1) 
			{
				m_UserItem[iEmptySlot] = Item;
				tSlot = (BYTE)iEmptySlot;
			}

			SendSystemMsg((LPTSTR)(LPCTSTR)g_arBoxEventTable[iRandom]->m_strText, SYSTEM_NORMAL, TO_ME);
			
			CBufferEx TempBuf;

			TempBuf.Add(ITEM_MOVE_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add((BYTE)0);		// Type
			TempBuf.Add((BYTE)1);		// Count
			
			TempBuf.Add(tSlot);
			TempBuf.Add(m_UserItem[tSlot].sLevel);
			TempBuf.Add(m_UserItem[tSlot].sSid);
			TempBuf.Add(m_UserItem[tSlot].sDuration);
			TempBuf.Add(m_UserItem[tSlot].sBullNum);
			TempBuf.Add(m_UserItem[tSlot].sCount);
			for(i =0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tSlot].tMagic[i]);
			
			TempBuf.Add(m_UserItem[tSlot].tIQ); 
			
			Send(TempBuf, TempBuf.GetLength());
		}
		/*else if(g_arBoxEventTable[iRandom]->m_tType == 38 || g_arBoxEventTable[iRandom]->m_tType == 39)	// Minus HP 40% or 30%
		{
			if(g_arBoxEventTable[iRandom]->m_tType == 38)	iDeleteHP = (int)((double)m_sHP * 0.4);
			else if(g_arBoxEventTable[iRandom]->m_tType == 39)	iDeleteHP = (int)((double)m_sHP * 0.3);

			if(iDeleteHP > 0)
			{
				m_sHP -= iDeleteHP;
				if(m_sHP < 0) m_sHP = 0;
				SendSystemMsg((LPTSTR)(LPCTSTR)g_arBoxEventTable[iRandom]->m_strText, SYSTEM_NORMAL, TO_ME);
				SendHP();
			}			
		}*/

		break;

	default:
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////
//	Summoning Monster
//
BOOL USER::SummonMonster(CString strName, CPoint pt)
{
	CNpc* pNpc = NULL;
	int i;
	CString strMsg = _T("");

	MYSHORT sAI;
	BYTE upTemp = 0;			// 惑困 8厚飘
	BYTE dwTemp = 0;			// 窍困 8厚飘

//	int iDeleteIndex = 0;
//	TCHAR DeleteSend[2048];	

//	int iModifyIndex = 0;
//	TCHAR ModifySend[2048];	
    if (strName=="卫兵") strName="卡特";
	int NpcState = NPC_DEAD;

	for(i = 0; i < g_arNpc.GetSize(); i++)
	{
		pNpc = g_arNpc[i];
		if( !pNpc ) continue;

		if(strName.CompareNoCase(pNpc->m_strName) == 0)
		{
			if(pNpc->m_sCurZ == 1005) continue;
			if(pNpc->m_NpcState != NPC_DEAD && pNpc->m_NpcState != NPC_STANDING) continue;
			if(pNpc->m_bSummon == TRUE) continue;

			if(::InterlockedExchange(&pNpc->m_lNowSummoning, 1) != 0) continue;

			NpcState = pNpc->m_NpcState;
			pNpc->m_Delay = 2000;

			if(pNpc->m_NpcState == NPC_STANDING)
			{
				pNpc->SetUidNPC(pNpc->m_sCurX, pNpc->m_sCurY, 0);

				pNpc->m_presx = pNpc->m_presy = -1;
				pNpc->SendNpcInfoBySummon(m_pCom);
			}

			pNpc->m_SummonZoneIndex = pNpc->m_ZoneIndex;
			pNpc->m_sSummonOrgZ = pNpc->m_sOrgZ;
			pNpc->m_sSummonOrgX = pNpc->m_sOrgX;
			pNpc->m_sSummonOrgY = pNpc->m_sOrgY;

			pNpc->m_sCurZ = pNpc->m_sOrgZ = m_curz;
			pNpc->m_sCurX = pNpc->m_sOrgX = pt.x;
			pNpc->m_sCurY = pNpc->m_sOrgY = pt.y;

			pNpc->m_ZoneIndex		= -1;

			pNpc->m_dwStepDelay	= GetTickCount();
			
			sAI.i = (short)pNpc->m_sAI;						// NPC AI甫 悸泼
			upTemp = sAI.b[0];
			dwTemp = sAI.b[1];
			
			pNpc->m_tNpcAttType = upTemp >> 7;				// 唱吝俊 眠啊秦具茄促.
			upTemp = upTemp << 1;
			pNpc->m_tNpcLongType = upTemp >> 7;
			upTemp = upTemp << 1;
			pNpc->m_tNpcGroupType = upTemp >> 7;
			
			if(pNpc->m_sClientSpeed <= 20) pNpc->m_sClientSpeed = 20;	// 规绢 内靛;
			
			for(i = 0; i < g_zone.GetSize(); i++)
			{
				if(g_zone[i]->m_Zone == pNpc->m_sCurZ) 
				{
					pNpc->m_ZoneIndex = i;
					break;
				}
			}

			pNpc->m_bSummon = TRUE;
			pNpc->m_bSummonDead = TRUE;
			pNpc->EventNpcInit(pt.x, pt.y);

			if(NpcState == NPC_STANDING) 
			{
				pNpc->SetUidNPC(pt.x, pt.y, pNpc->m_sNid + NPC_BAND);

				pNpc->m_presx = pNpc->m_presy = -1;
				pNpc->SightRecalc(m_pCom);
			}

			if(::InterlockedExchange(&pNpc->m_lNowSummoning, 0) != 1) pNpc->m_lNowSummoning = 0;

			strMsg.Format( IDS_USER_CALL_WHAT, pNpc->m_strName);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

			return TRUE;
		}
	}

	return FALSE;
}

int USER::GetItemIndex(int sid)
{
	int i = 0;
	int nCount = g_arItemTable.GetSize();

	for(i = 0; i < nCount; i++)
	{
		if(sid == g_arItemTable[i]->m_sSid) return i;
	}

	return -1;
}

void USER::SoftClose()
{
	CBSocket::B_SoftClose();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 漂荐茄 家葛己 拱距阑 荤侩茄促.
//
BYTE USER::UseSpecialPotion(short sSlot)
{
	
	CBufferEx TempBuf;
	short sSid = 0;
	BYTE tRet = 0;
	int j = 0;
    int hour = 0;
	int Damage = 0;	//用于群攻伤害值变量
	
	
	if(sSlot < EQUIP_ITEM_NUM || sSlot >= TOTAL_INVEN_MAX) return FALSE;
	
	sSid = m_UserItem[sSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
	if(m_UserItem[sSlot].sCount <= 0) return FALSE;

	switch(sSid)
	{
	case SPECIAL_ITEM_HIEXP	:
		if(m_dwHiExpTime !=0){
			SendSystemMsg("你现在已经有兴奋状态,无法再使用兴奋剂.", SYSTEM_NORMAL, TO_ME);
			return false;
		}
		if(m_iDisplayType == 5 || m_iDisplayType ==6) //公丰 荤侩磊牢啊?
		{
			SendSystemMsg("公丰 荤侩磊绰 窍捞甫 荤侩且 荐 绝嚼聪促.", SYSTEM_NORMAL, TO_ME);
			return tRet; //or m_dwHiExpTime = 0; //公丰 荤侩磊绰 荤侩且 荐 绝促.
		}
		else m_dwHiExpTime = HIEXP_TIME;
		//----------------------------------------------------------------------------------------
		m_dwLastHiExpTime = GetTickCount();
		tRet = EXP_POTION;

		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		AddAbnormalInfo(ABNORMAL_HIEXP);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);	
		Send(TempBuf, TempBuf.GetLength());

		break;

	case 1212	:
		if(m_dwHiExpTime !=0){
			SendSystemMsg("你现在已经有兴奋状态,无法再使用超级兴奋.", SYSTEM_NORMAL, TO_ME);
			return false;
		}
		if(m_iDisplayType == 5 || m_iDisplayType ==6) //公丰 荤侩磊牢啊?
		{
			SendSystemMsg("公丰 荤侩磊绰 窍捞甫 荤侩且 荐 绝嚼聪促.", SYSTEM_NORMAL, TO_ME);
			return tRet; //or m_dwHiExpTime = 0; //公丰 荤侩磊绰 荤侩且 荐 绝促.
		}
		else m_dwHiExpTime = HIEXP_TIME72;
		//----------------------------------------------------------------------------------------
		m_dwLastHiExpTime = GetTickCount();
		tRet = EXP_POTION;

		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		AddAbnormalInfo(ABNORMAL_HIEXP);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);	
		Send(TempBuf, TempBuf.GetLength());

		break;

	case SPECIAL_ITEM_MAGICFIND :
		if(m_dwMagicFindTime !=0){
			SendSystemMsg("你现在已经有幸运状态,无法再使用幸运.", SYSTEM_NORMAL, TO_ME);
			return false;
		}
		if(m_iDisplayType == 5 || m_iDisplayType ==6) //公丰 荤侩磊牢啊?
		{
			SendSystemMsg("公丰 荤侩磊绰 MagicFind甫 荤侩且荐 绝嚼聪促.", SYSTEM_NORMAL, TO_ME);
			return tRet; //or m_dwMagicFindTime = 0; //公丰 荤侩磊绰 荤侩且 荐 绝促.
		}
		else m_dwMagicFindTime = MAGICFIND_TIME;
		//-----------------------------------------------------------------------------------------
		m_dwLastMagicFindTime = GetTickCount();
		tRet = MAGIC_POTION;

		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		AddAbnormalInfo(ABNORMAL_MAGICFIND);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		Send(TempBuf, TempBuf.GetLength());

		break;

	case 1568 :
		if(m_dwMagicFindTime !=0){
			SendSystemMsg("你现在已经有幸运状态,无法再使用超级幸运.", SYSTEM_NORMAL, TO_ME);
			return false;
		}
		if(m_iDisplayType == 5 || m_iDisplayType ==6) //公丰 荤侩磊牢啊?
		{
			SendSystemMsg("公丰 荤侩磊绰 MagicFind甫 荤侩且荐 绝嚼聪促.", SYSTEM_NORMAL, TO_ME);
			return tRet; //or m_dwMagicFindTime = 0; //公丰 荤侩磊绰 荤侩且 荐 绝促.
		}
		else m_dwMagicFindTime = MAGICFIND_TIME72;
		//-----------------------------------------------------------------------------------------
		m_dwLastMagicFindTime = GetTickCount();
		tRet = MAGIC_POTION;

		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		AddAbnormalInfo(ABNORMAL_MAGICFIND);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		Send(TempBuf, TempBuf.GetLength());

		break;

    /*case SPECIAL_ITEM_HX_LS: //幻想灵石
		hour = m_dwHtExpTime/3600000;
		if(hour < 500)
		{
			m_dwHtExpTime += HIEXP_TIME_ONE;
			m_dwLastHtExpTime = GetTickCount();			
			SendDGDuration(1035,sSlot);
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			AddStateInfo(STATE_5);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);					
			Send(TempBuf, TempBuf.GetLength());		
		}
		break;
	case SPECIAL_ITEM_HX_JS: //幻想晶石
		hour = m_dwMagicFtTime/3600000;
		if(hour < 500)
		{
			m_dwMagicFtTime += HIEXP_TIME_ONE;
			m_dwLastMagicFtTime = GetTickCount();			
			SendDGDuration(1289,sSlot);
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			AddStateInfo(STATE_28);			
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);					
			Send(TempBuf, TempBuf.GetLength());		
		}
		break;*/
		case SPECIAL_ITEM_HX_LS: //幻灵
			if (m_dwHtExpTime != 0)
			{
				SendSystemMsg("您已经使用了幻想灵石,无需重复使用",SYSTEM_NPC,TO_ME);
			}
			else
			{
				//SendSystemMsg("欢迎您使用幻想灵石,您的武器改数+1！",SYSTEM_NPC,TO_ME);
				CString sysMsg;
				sysMsg.Format("玩家 %s 使用幻想灵石，武器改数+1！ ",this->m_strUserID);
				SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
				m_dwHtExpTime += HIEXP_TIME_ONE;
				m_dwLastHtExpTime = GetTickCount();			
				SendDGDuration(1035,sSlot);
				TempBuf.Add(SET_USER_STATE);
				TempBuf.Add(m_uid + USER_BAND);
				AddStateInfo(STATE_5);
				TempBuf.Add(m_dwAbnormalInfo);
				TempBuf.Add(m_dwAbnormalInfo_);					
				Send(TempBuf, TempBuf.GetLength());		
			}
			break;
  case SPECIAL_ITEM_HX_JS: //幻想晶石 和超级幸运作用
		
		if (m_dwMagicFtTime > 0)
		{
			SendSystemMsg("您已经使用了幻想晶石,无需重复使用",SYSTEM_NPC,TO_ME);
		}
		else
		{
			//SendSystemMsg("欢迎您使用幻想晶石,您的装备改数+1！",SYSTEM_NPC,TO_ME);
			CString sysMsg;
			sysMsg.Format("玩家 %s 使用幻想晶石，装备改数+1！ ",this->m_strUserID);
			SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
			m_dwMagicFtTime += HIEXP_TIME_ONE;
			m_dwLastMagicFtTime = GetTickCount();			
			SendDGDuration(1289,sSlot);
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			AddStateInfo(STATE_28);			
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);					
			Send(TempBuf, TempBuf.GetLength());	
		}
		break;
/////////////////////////////////////////////////////////////////////////////
//物品——>守护天使
	case SPECIAL_ITEM_TS: 
	    {
			if(m_dwShTsTime > 0)
			{	SendSystemMsg("你现在已有守护天使状态,无法再使用守护天使.", SYSTEM_NORMAL, TO_ME);	}
			else
			{
				if(m_dwShTsTime == 0)
				{
                 m_dwShTsTime=SHTS_TIME;			
	             m_dwShTsLaseTime=GetTickCount();
	             TempBuf.Add(SET_USER_STATE);
				 TempBuf.Add(m_dwAbnormalInfo);
	             TempBuf.Add(m_dwAbnormalInfo_);
                 FreedomCB();
	             Send(TempBuf, TempBuf.GetLength());
	             SendMyInfo( TO_INSIGHT, INFO_MODIFY );
				 SendSystemMsg("使用成功!获得24小时守护天使状态.", SYSTEM_NORMAL, TO_ME);
	             tRet = SERVER_ARK_WINNER;
	            }
			}
		} 
		break;
/////////////////////////////////////////////////////////////////////////////
//物品——>VIP10天卡240小时
	case 1380:	
        {
			if(m_dwGuarDianTianShi > 0)
			{	SendSystemMsg("您目前已是VIP,请到期后再补充!", SYSTEM_NORMAL, TO_ME);	}
			else
			{
                    m_dwGuarDianTianShi = 3600 * 240 * 1000;
					m_dwHiExpTime = 3600 * 240 * 1000;
					m_dwMagicFindTime = 3600 * 240 * 1000;
					m_dwVIPTime = 3600 * 240 * 1000;
				//	SetXingfen();//3倍兴奋图标
				//  SetXingYun();//3倍幸运图标
					FreedomCB();//守护天使
					SendSystemMsg("恭喜你获得VIP时间：10天,安全区域不消耗时间", SYSTEM_NORMAL, TO_ME);
					
					CString str;
	                str.Format("恭喜!玩家[ %s ]使用[VIP会员10日卡]成为VIP用户!",m_strUserID);
	                SendSystemMsg(str.GetBuffer(str.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);

					CBufferEx TempBuf;
					TempBuf.Add(SET_USER_STATE);
				   // AddStateInfo(STATE_31);
					//AddAbnormalInfo(ABNORMAL_VIP);
					AddAbnormalInfo(ABNORMAL_FUDAI);
					TempBuf.Add(m_uid + USER_BAND);
					TempBuf.Add(m_dwAbnormalInfo);
					TempBuf.Add(m_dwAbnormalInfo_);				
					Send(TempBuf, TempBuf.GetLength());
					SendInsight(TempBuf, TempBuf.GetLength());
					tRet = 14;

                    TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));//存储记录
					SYSTEMTIME time;
		            GetLocalTime(&time);
					sprintf(strOP,"在%d-%d-%d %d:%d分使用VIP10日卡",time.wYear,time.wMonth,time.wDay ,time.wHour,time.wMinute);
					WriteOpratorLog(strOP,CHAT_LOG);
			}
		} 
		break;
	
   case SPECIAL_ITEM_KILLREMOVE :	// 忏悔药.
		m_iCityValue=m_iCityValue+1000;
		GetCityRank();
	//	CheckMagicItemMove();
        SendUserStatusSkill();
		if(m_sKillCount > 0){
			m_sKillCount -= 1;
			if( m_sKillCount < 0 ) m_sKillCount = 0;
		}
		tRet = KILLREMOVE_POTION;
		break;
	case EVENTITEM_SID_MOON: //老头
		if(m_dwAdamantineTime != 0 || m_dwMightyWeaponTime != 0 )
		{
			return FALSE;
		}
		SetBerserker(60 * 5);
		tRet = 19;	
		break;
	case 991: //神迹宝石(封印) //神秘的罐子
		{
		  int dwTemp = FindItem(1127); //钥匙
			int iNum = -1;
		 ItemList TempItem;
	     iNum = GetSameItem(TempItem, INVENTORY_SLOT);
		 iNum = GetEmptySlot(INVENTORY_SLOT);
		 if(iNum < 1)						
		 {

		    SendSystemMsg("包裹空间满了,请至少留1个空间解封!", SYSTEM_ERROR, TO_ME);
		 }
		
				else if (dwTemp <  1)
				{
					
					SendSystemMsg("需要神秘钥匙开启神秘的罐子", SYSTEM_ERROR, TO_ME);
				}		
			else {
				RobItem(1127,1);//减少一个物品
			 int iRandom = 0;
			 iRandom = myrand(1, 12);
			 switch(iRandom)
			 {
			 case 1: 
				 GiveAllItem(1185, 1, 0, 0, 0, 0, 0, 0, 0);//黄金L
				  break;
			 case 2: 
				 GiveAllItem(1145, 1, 0, 0, 0, 0, 0, 0, 0);//黄金J
				 break;
			 case 3: 
				 GiveAllItem(1021, 1, 0, 0, 0, 0, 0, 0, 0);//电光板
				 break;
			 case 4: 
				 GiveAllItem(1029, 1, 0, 0, 0, 0, 0, 0, 0);//熊变
				 break;
			 case 5:
				 GiveAllItem(1044, 1, 0, 0, 0, 0, 0, 0, 0);//守护天使
				 break;
			 case 6: 
				 GiveAllItem(1043, 1, 0, 0, 0, 0, 0, 0, 0);//星星
				 break;
			 case 7: 
				 GiveAllItem(1035, 1, 0, 0, 0, 0, 0, 0, 0);//幻灵
				 break;
			 case 8: 
				 GiveAllItem(1289, 1, 0, 0, 0, 0, 0, 0, 0);//幻晶
				 break;
			 case 9: 
				 GiveAllItem(862, 1, 0, 0, 0, 0, 0, 0, 0);//武器卡
				 break;
		      case 10: 
				 GiveAllItem(845, 1, 0, 0, 0, 0, 0, 0, 0);//CL
				 break;
			  case 11: 
				 GiveAllItem(846, 1, 0, 0, 0, 0, 0, 0, 0);//CJ
				 break;
			  case 12: 
				 GiveAllItem(1108, 1, 0, 0, 0, 0, 0, 0, 0);//圣人药
				 break;
			 default:
		           return FALSE;
			 }
			 tRet = 18;
		  }	
		}
		 break;
		 case 1310: //普通神迹宝箱 //首饰盒子
		{
		 int iNum = -1;
		 ItemList TempItem;
	     iNum = GetSameItem(TempItem, INVENTORY_SLOT);
		 iNum = GetEmptySlot(INVENTORY_SLOT);
		 if(iNum < 1)						
		 {

		    SendSystemMsg("包裹空间满了,请至少留1个空间解封!", SYSTEM_ERROR, TO_ME);
		 }else 
		 {
			 int iRandom = 0;
			 iRandom = myrand(1, 12);
			 switch(iRandom)
			 {
			 
			 case 1: 
				// GiveAllItem(1287, 1, 0, 2, 161, 0, 0, 0, 0);//10力量
				  GiveAllItem(730, 1, 0, 5, 1, 137, 0, 0, 0);
				 break;
			 case 2: 
				// GiveAllItem(1287, 1, 0, 2, 157, 0, 0, 0, 0);//10敏捷
				  GiveAllItem(690, 1, 0, 5, 1, 137, 0, 0, 0);
				 break;
		     case 3: 
				// GiveAllItem(1287, 1, 0, 2, 163, 0, 0, 0, 0);//10智慧
				  GiveAllItem(743, 1, 0, 5, 1, 137, 0, 0, 0);
				 break;
			 case 4: 
				// GiveAllItem(1287, 1, 0, 2, 165, 0, 0, 0, 0);//10智力
				  GiveAllItem(730, 1, 0, 5, 2, 137, 0, 0, 0);
				 break;
			 case 5: 
				// GiveAllItem(1287, 1, 0, 2, 155, 0, 0, 0, 0);//10体质
				  GiveAllItem(690, 1, 0, 5, 2, 137, 0, 0, 0);
				 break;
			 case 6: 
				// GiveAllItem(1287, 1, 0, 2, 155, 0, 0, 0, 0);//10体质
				  GiveAllItem(743, 1, 0, 5, 2, 137, 0, 0, 0);
				 break;
			 case 7: 
				// GiveAllItem(1287, 1, 0, 2, 155, 0, 0, 0, 0);//10体质
				  GiveAllItem(730, 1, 0, 5, 3, 137, 0, 0, 0);
				 break;
			 case 8: 
				// GiveAllItem(1287, 1, 0, 2, 155, 0, 0, 0, 0);//10体质
				  GiveAllItem(690, 1, 0, 5, 3, 137, 0, 0, 0);
				 break;
			 case 9: 
				// GiveAllItem(1287, 1, 0, 2, 155, 0, 0, 0, 0);//10体质
				  GiveAllItem(743, 1, 0, 5, 3, 137, 0, 0, 0);
				 break;
			 case 10: 
				// GiveAllItem(1287, 1, 0, 2, 155, 0, 0, 0, 0);//10体质
				  GiveAllItem(730, 1, 0, 5, 4, 137, 0, 0, 0);
				 break;
			 case 11: 
				// GiveAllItem(1287, 1, 0, 2, 155, 0, 0, 0, 0);//10体质
				  GiveAllItem(690, 1, 0, 5, 4, 137, 0, 0, 0);
				 break;
     		 case 12: 
				// GiveAllItem(1287, 1, 0, 2, 155, 0, 0, 0, 0);//10体质
				  GiveAllItem(743, 1, 0, 5, 4, 137, 0, 0, 0);
				 break;
			 default:
		           return FALSE;
			 }
			 tRet = 18;
		  }	
		}
		 break;
		 /////////////////////
	//	case 998:
	//	if( FindItem( 998) >= 1 )
	//	{
	//		RobItem( 998, 1 );
	//		GiveItem( 724, 10 );
	  //      tRet = 17;
	//	}
	//	break;
	
		 //////////////////
	case 1208: //宝箱
		{
	
          int iNum = -1;
		  ItemList TempItem;
	      iNum = GetSameItem(TempItem, INVENTORY_SLOT);
		 /* if(m_dwGuarDianTianShi > 0)
		  {*/
		 
            if(iNum == -1)							
		    { 
			  iNum = GetEmptySlot(INVENTORY_SLOT);
			  if(iNum < 1)						
		      {

		    		SendSystemMsg("包裹空间满了,请至少留1个空间开宝箱!", SYSTEM_ERROR, TO_ME);
			  }
			  else 
		      {
			       RandChouJiang();
			       tRet = 18;
			      
			  }	
		       
			}
           //}
		  
          /*else
		    {
                SendSystemMsg("您不是VIP用户,无法开启宝箱!", SYSTEM_ERROR, TO_ME);
				return FALSE;
		    }*/
		}
        break;
		
	case 1299://修理机甲1299
		{
			if(m_UserItem[39].sSid == 1184){
				m_UserItem[39].sBullNum = 10800;
//				m_UserItem[39].sDuration = 10;							
				SendCharData();
				SendMyInfo( TO_INSIGHT, INFO_MODIFY );	
				tRet = 17;
			}
			else{
				SendSystemMsg("必须装置超级机甲才能使用!", SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}
		break;
	case 1182:   //使用机甲狂暴药水
		{
			if(m_UserItem[39].sSid == 1184){
	//			m_UserItem[39].sBullNum = 10800;
				m_UserItem[39].sDuration = 10;							
				SendCharData();
				SendMyInfo( TO_INSIGHT, INFO_MODIFY );	
				tRet = 19;
			}
			else{
				SendSystemMsg("装置了超级机甲才能使用.", SYSTEM_ERROR, TO_ME);
				return FALSE;
			    }
		   }
		   break;
		
	case 1304:
		{			
			if(m_UserItem[39].sSid != 1184)
			{
				SendSystemMsg("必须装置超级机甲,才能升级控制系统!", SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
			CBufferEx TempBuf;
			TempBuf.Add((BYTE)236);
			TempBuf.Add((BYTE)1);		
			Send(TempBuf, TempBuf.GetLength());
		  }
		  break;
	case SPECIAL_ITEM1441:
		{
			if((m_dwShopPingDN + 20) < 21000000000)
			{
				GiveShopPingDN(20);
				tRet = 13;
				SendSystemMsg("使用元宝券获得20元宝!", SYSTEM_NORMAL, TO_ME);
			//	SendUserStatusSkill();
			}
		}
		break;
	case SPECIAL_ITEM1442:
		{
			if((m_dwShopPingDN + 50) < 21000000000)
			{
				GiveShopPingDN(50);
				tRet = 13;
				SendSystemMsg("使用元宝券获得50元宝!", SYSTEM_NORMAL, TO_ME);
			//	SendUserStatusSkill();
			}
		}
		break;
	case SPECIAL_ITEM1443:
		{
			if((m_dwShopPingDN + 100) < 21000000000)
			{
				GiveShopPingDN(100);
				tRet = 13;
				SendSystemMsg("使用元宝券获得100元宝!", SYSTEM_NORMAL, TO_ME);
			//	SendUserStatusSkill();
			}
		}
		break;
	case SPECIAL_ITEM1444:
		{
		  if(g_Shop.m_QanBao_KG == 1)
          {
			if((m_dwShopPingDN + 500) < 21000000000)
			{
				GiveShopPingDN(500);
				tRet = 13;
				SendSystemMsg("使用元宝券获得500元宝!", SYSTEM_NORMAL, TO_ME);
			//	SendUserStatusSkill();
			}
		   }
		}
		break;
	case SPECIAL_ITEM1445:
		{
		  if(g_Shop.m_QanBao_KG == 1)
          {
			if((m_dwShopPingDN + 4000) < 21000000000)
			{
				GiveShopPingDN(4000);
				tRet = 13;
				SendSystemMsg("使用元宝券获得4000元宝!", SYSTEM_NORMAL, TO_ME);
			//	SendUserStatusSkill();
			}
		  }
		}
		break;
	case 1019:
		{
		if((m_dwDN + 10000000) < 21000000000)
			{
				GiveDN(10000000);
				tRet = 13;
				SendSystemMsg("使用礼包获得1000万决战币!", SYSTEM_NORMAL, TO_ME);
			}
		}
		break;
	case 1042:
	    sSid = m_UserItem[33].sSid;	
	    if(sSid != 1053 && sSid != 1089 && sSid != 1054 && sSid != 1055 && sSid != 1056 && sSid != 1090 && 
	    sSid != 1091 && sSid != 1092 && sSid != 1065 && sSid != 1066 && sSid != 1067 && sSid != 1068 &&
	    sSid != 1057 && sSid != 1058 && sSid != 1059 && sSid != 1060 && sSid != 1069 && sSid != 1070 &&
	    sSid != 1071 && sSid != 1072 && sSid != 1061 && sSid != 1062 && sSid != 1063 && sSid != 1064 &&
	    sSid != 1082 && sSid != 1083 && sSid != 1084 && sSid != 1073 && sSid != 1074 && sSid != 1075 &&
	    sSid != 1076 && sSid != 1085 && sSid != 1086 && sSid != 1087 && sSid != 1088 && sSid != 1077 &&
	    sSid != 1078 && sSid != 1079 && sSid != 1080 && sSid != 1081 && sSid != 1709 && sSid != 1710 &&
		sSid != 1711 && sSid != 1712 && sSid != 1713 && sSid != 1714 && sSid != 1715 && sSid != 1716|| sSid == -1)
	    {
        SendSystemMsg( "请把要还原的百级装备放背包最后一行最后一格", SYSTEM_ERROR, TO_ME);
		return FALSE;
	    }	
		HuanYuan100();
		tRet = SERVER_ARK_WINNER;
		break;
	case 1147:
	    sSid = m_UserItem[33].sSid;	
	    if(sSid != 1590 && sSid != 1594 && sSid != 1598 && sSid != 1602 && sSid != 1606 && sSid != 1650 && 
	    sSid != 1654 && sSid != 1658 && sSid != 1662 && sSid != 1666 && sSid != 1607 && sSid != 1608 &&
	    sSid != 1609 && sSid != 1610 && sSid != 1611 && sSid != 1612 && sSid != 1613 && sSid != 1614 &&
	    sSid != 1615 && sSid != 1616 && sSid != 1617 && sSid != 1618 && sSid != 1619 && sSid != 1620 &&
	    sSid != 1621 && sSid != 1622 && sSid != 1623 && sSid != 1624 && sSid != 1625 && sSid != 1626 &&
	    sSid != 1627 && sSid != 1628 && sSid != 1629 && sSid != 1630 && sSid != 1631 && sSid != 1632 &&
	    sSid != 1633 && sSid != 1634 && sSid != 1635 && sSid != 1636 && sSid != 1637 && sSid != 1638 &&
		sSid != 1639 && sSid != 1640 && sSid != 1641 && sSid != 1642 && sSid != 1643 && sSid != 1644 &&
		sSid != 1645 && sSid != 1646|| sSid == -1)
	    {
        SendSystemMsg( "请把要还原的130级装备放背包最后一行最后一格", SYSTEM_ERROR, TO_ME);
		return FALSE;
	    }	
		HuanYuan130();
		tRet = SERVER_ARK_WINNER;
		break;
	case 1108: // super posion
		if (m_iCityValue < 64000 )
	 {
        m_sCityRank=5;
        m_sKillCount=0;
        m_iCityValue=64000;
		SendUserStatusSkill();
		tRet = KILLREMOVE_POTION;

		TempBuf.Add(SET_USER_PK_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add((BYTE)0x01);
		TempBuf.Add(m_sCityRank);
		SendInsight(TempBuf, TempBuf.GetLength());
	}
		break;
		
	case 1109: // add XP posion
        m_dwXP += 10000;
		SendXP();
		tRet = KILLREMOVE_POTION;	
		break;
	case 1670: // 召唤伊涂罗胶囊
		{
          if(m_tHuFaType != 0)
				return FALSE;
			return CallHuFa(0x01);
		    break;
		}
    case 1671: // 召唤普雷塔胶囊
		{
          if(m_tHuFaType != 0)
				return FALSE;
			return CallHuFa(0x02);
		    break;
		}
	case 1672: // 召唤克劳斯胶囊
		{
          if(m_tHuFaType != 0)
				return FALSE;
			return CallHuFa(0x03);
		    break;
		}
	case 1673: // 召唤伊西斯胶囊
		{
          if(m_tHuFaType != 0)
				return FALSE;
			return CallHuFa(0x04);
		    break;
		}
    case 1132:
		if(m_curz == 409) return FALSE;
		ZoneMoveReq(409,63,79);
		tRet = 17;
		break;
	case 1498: // 宠物饲料补充->忠诚度
		{
          if(m_tBabyCall != 1) return FALSE;//没有宠是不能用的
		  if(m_sFealty != 100)//忠诚度不满
		  {
			//	if((m_sFealty + 1) < 101) m_sFealty += 1;
			//	SendEventMsg("使用饲料,宠物[忠诚度]增加[ 1 ]");
			     m_sFealty = 100;
				SendEventMsg("使用饲料,宠物[忠诚度]补满!");
				SendBabyInfo();
		   }  
		   tRet = 17;
		    break;
		}
	case 1007:
		ZoneMoveReq(409,68,88);
		tRet = 17;
		break;
	case 1100:
		if (m_curz == 59) 	tRet = 19;
		break;
	case 1017:
		if( FindItem( 854) >= 1 )
		{
			RobItem( 854, 1 );
			GiveItem( 862, 2 );
		//	ZoneMoveReq(409,97,55);
			tRet = 17;
		}
		break;
	//case 862:   //武器变外观，炫卡

	//	break;
		
		
  
		//变身物品——>狼变
	case SPECIAL_ITEM_BIANLANG:
       {
		if(m_dwBFindTime > 0  ||  m_iSkin != 0 )
		
			SendEventMsg("您已是变身状态,请勿重复使用!");
       else
			{
			m_dwBFindTime = BIANSHEN_TIME;
	        m_dwLastBFindTime = GetTickCount();
            m_sDramageUp = true;					// 打开伤害提高
			BianShen(1);
			CheckMagicItemMove();
			tRet = SERVER_GUILD_INVITE;
			//SendEventMsg("2小时内命中加20,魔法损伤加15,物理损伤加15 !");
			CString sysMsg;
			sysMsg.Format("玩家 %s 使用狼变2小时内命中加20,魔法损伤加15,物理损伤加15  ! ",this->m_strUserID);
			SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		    }
		}
		break;

		
/////////////////////////////////////////////////////////////////////////////
//变身物品——>熊变
	case SPECIAL_ITEM_BIANXIONG:
		 {
		//if(m_dwBFindTime > 0  ||  m_iSkin != 0 )
		if(m_dwBFindTime > 0  )
		
			SendEventMsg("您已是变身状态,请勿重复使用!");
       else
			{
			m_dwBFindTime = BIANSHEN_TIME;
	        m_dwLastBFindTime = GetTickCount();
            m_sDramageUp = true;					// 打开伤害提高
			//BianShen(2);   //改变身体模型
			CheckMagicItemMove();
			tRet = SERVER_GUILD_INVITE;
			//SendEventMsg("2小时内生命增加100,防御增加15,魔抗增加15!");
			CString sysMsg;
			sysMsg.Format("玩家 %s 使用熊变2小时内生命增加100,防御增加15,魔抗增加15 ! ",this->m_strUserID);
			SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		    }
		}
		break;
//==========================================================================================================
	case 1560:  //神迹药水
		 
	  if(m_dwSJTime > 0)
	  {
		  SendEventMsg("您当前已经是神迹状态无法重复使用!");return FALSE; }

	  else   
	        {
			m_sDramageUp = true;
		    m_dwSJTime = 3600*1*1000;				
			CheckMagicItemMove();
			tRet = SERVER_ARK_WINNER;
			//SendEventMsg("使用成功1小时内所有属性点增加10(城市中不计算时间)");
			CString sysMsg;
			sysMsg.Format("玩家 %s 使用神迹药水，1小时内所有属性点增加10(城市中不计算时间)! ",this->m_strUserID);
			SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
			}
	   break;
//==========================================================================================
	/*case 1561:  //保护卡
		 
		if(m_dwBHTime > 0){ SendEventMsg("您当前已经是闭关保护状态!");return FALSE; }
        else if(m_isUser) { SendEventMsg("您要关闭PK键才可以进入保护状态!");return FALSE; }
		 else 
			{
            m_dwBHTime = 3600*2*1000;				
			CheckMagicItemMove();
			tRet = SERVER_ARK_WINNER;
			CString str;
	        str.Format("玩家[ %s ]使用[闭关保护卡]1小时内不能被任何玩家攻击!",m_strUserID);
	        SendSystemMsg(str.GetBuffer(str.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		  } 
			 break;*/
	/*case 1015:
		if (m_sLevel >=105  && m_byClass == 1 && m_dwXP >=30000) 
		{

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == 30) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		m_UserPsi[m_nHavePsiNum].sSid = 30;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= 30000;

    	TempBuf.Add(BUY_PSY_RESULT);
	    TempBuf.Add((BYTE)1);
    	TempBuf.Add((short)1);
    	TempBuf.Add(30);
    	Send(TempBuf, TempBuf.GetLength());
        SendXP();

        tRet = 15;
		}
     	break;
		*/
	}


	return tRet;
}
void USER::JJSpeed() //快速变外观
{
	if(m_UserItem[39].sSid == -1) return;
	if(m_dwBFindTime > 0  ||  m_iSkin != 0 )
	{
	SendEventMsg("请先解除变身后在启动机甲!");
    return;
	}
	if(m_UserItem[39].sSid == 1184)
	{
		if(m_UserItem[39].tMagic[7] == 7)//检测机甲附件2的属性为7则变身战神
		{
			SendPsiAttackResult(SUCCESS, m_uid + USER_BAND, (BYTE)204);//增加变身效果
			BianShen(8);		
		}else if (m_iSkin == 0)
        {
		   SendPsiAttackResult(SUCCESS, m_uid + USER_BAND, (BYTE)204);//增加变身效果
		   BianShen(5);
	    }
	}
}
void USER::BianShen(int x)
{
	//HuanYuanBianShen();
	m_iSkin = x;							// 改变身体模型
	if(m_UserItem[39].sSid == 1184 && x == 5){
		if(m_dwTransTime != 0) m_iHair = 10015;
		else
		{
			if(m_UserItem[39].tMagic[5] == 1) {
				m_iHair = 10001;
			}else if(m_UserItem[39].tMagic[5] == 2) {
				m_iHair = 10002;
			}
		}		
	}
//	TempBuf.Add(USE_POTION);		//切换机甲 显示效果
//	TempBuf.Add(m_uid + USER_BAND);
//	TempBuf.Add((BYTE)16);
//	Send( TempBuf, TempBuf.GetLength() );
	//UpdateUserData();						// 更新用户数据  neo版本
	//	switch(m_iSkin)
//		{
//		case 1:
//			SetUserToMagicUser(2);
//			break;
//		case 2:
//		case 4:
//		case 7:
//		case 8:
//			SetUserToMagicUser(3);
//			break;
//		}
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	UpdateUserData();						// 更新用户数据		
	CheckMagicItemMove();
	//m_dwBFindTime = BIANSHEN_TIME;
	//m_dwLastBFindTime = GetTickCount();		
}
void USER::HuanYuanBianShen()    //更新变身时间
{
	m_iHair = 0;
	m_iSkin = 0;	
	m_dwBFindTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(BIAN_SHEN);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	DeleteAbnormalInfo(ABNORMAL_BIANSHEN);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
}
void USER::HuanYuanXf()    //更新兴奋时间
{
	
	m_dwHiExpTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(TEST_21);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	DeleteAbnormalInfo(ABNORMAL_HIEXP);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
}

void USER::HuanYuanXy()    //更新幸运时间
{
	
	m_dwMagicFindTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(TEST_21);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	DeleteAbnormalInfo(ABNORMAL_MAGICFIND);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
}
void USER::HuanYuanFD()    //更新幸福时光
{
	
	m_dwVIPTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(TEST_21);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	DeleteAbnormalInfo(ABNORMAL_FUDAI);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
}
void USER::HuanYuanHl()    //更新幻灵时间
{
	
	m_dwHtExpTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(TEST_21);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	DeleteStateInfo(STATE_5);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
}
void USER::HuanYuanHj()    //更新幻晶时间
{
	
	m_dwMagicFtTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(TEST_21);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	DeleteStateInfo(STATE_28);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
}
void USER::HuanYuanVip()    //更新VIP时间
{
	
	m_dwGuarDianTianShi = 0;
	m_dwMagicFindTime = 0;
	m_dwHiExpTime = 0;
	m_dwVIPTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(TEST_21);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	DeleteAbnormalInfo(ABNORMAL_VIP);
	DeleteAbnormalInfo(ABNORMAL_MAGICFIND);
	DeleteAbnormalInfo(ABNORMAL_HIEXP);
	DeleteAbnormalInfo(ABNORMAL_FUDAI);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
}
void USER::HuanYuanBH()    //更新保护卡
{
	
	m_dwBHTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(TEST_21);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
    CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
	//CString strMsg = _T("");
    //strMsg.Format("玩家[ %s ]的闭关时间已经耗尽,可以找他报仇了!",m_strUserID );
	//SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
}
void USER::HuanYuanSJ()    //更新神迹
{
	
	m_dwSJTime = 0;
	CheckMagicItemMove();

	CBufferEx PotionBuf;
	PotionBuf.Add(USE_POTION);
	PotionBuf.Add(m_uid + USER_BAND);
	PotionBuf.Add(TEST_21);
	SendExactScreen( PotionBuf, PotionBuf.GetLength() );
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息	
	DeleteStateInfo(STATE_27);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());	
}
/////////////////////////////////////////////////////////////////////////////////
//	家葛己 厩技荤府狼 郴备档甫 焊辰促. 促静搁 朝赴促.
//

void USER::SendAccessoriDuration(short sSid)
{
	BYTE tSlot = 0;
	int i;
	CBufferEx TempBuf;
	
	for(i = 0; i < MAX_ACCESSORI; i++)
	{
		tSlot = g_iAccessoriSlot[i];
		if(m_UserItem[tSlot].sSid == sSid)
		{
			m_UserItem[tSlot].sDuration--;
			if(m_UserItem[tSlot].sDuration <= 0) 
			{
				MakeItemLog( &m_UserItem[tSlot], ITEMLOG_ACC_USE );
				FlushItemLog( TRUE );
				
				ReSetItemSlot(&m_UserItem[tSlot]);
				
				TempBuf.Add(ITEM_MOVE_RESULT);
				TempBuf.Add(SUCCESS);
				TempBuf.Add((BYTE)0);		
				TempBuf.Add((BYTE)1);		// Count
				
				TempBuf.Add(tSlot);
				TempBuf.Add(m_UserItem[tSlot].sLevel);
				TempBuf.Add(m_UserItem[tSlot].sSid);
				TempBuf.Add(m_UserItem[tSlot].sDuration);
				TempBuf.Add(m_UserItem[tSlot].sBullNum);
				TempBuf.Add(m_UserItem[tSlot].sCount);
				for(i =0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tSlot].tMagic[i]);
				TempBuf.Add(m_UserItem[tSlot].tIQ); 
				
				Send(TempBuf, TempBuf.GetLength());				
			}
			else
			{
				TempBuf.Add(ITEM_DURATION);
				TempBuf.Add(tSlot);
				TempBuf.Add(m_UserItem[tSlot].sDuration);
				Send(TempBuf, TempBuf.GetLength());
			}

			return;
		}
	}
}
void USER::SendDGDuration(short sSid,int slot)
{
	int i = 0;
	BYTE tSlot = slot;
	CBufferEx TempBuf;
	if(m_UserItem[tSlot].sSid == sSid)
	{
		m_UserItem[tSlot].sDuration--;
		if(m_UserItem[tSlot].sDuration <= 0) 
		{
			MakeItemLog( &m_UserItem[tSlot], ITEMLOG_ACC_USE );
			FlushItemLog( TRUE );
				
			ReSetItemSlot(&m_UserItem[tSlot]);
				
			TempBuf.Add(ITEM_MOVE_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add((BYTE)0);		// Type
			TempBuf.Add((BYTE)1);		// Count
			
			TempBuf.Add(tSlot);
			TempBuf.Add(m_UserItem[tSlot].sLevel);
			TempBuf.Add(m_UserItem[tSlot].sSid);
			TempBuf.Add(m_UserItem[tSlot].sDuration);
			TempBuf.Add(m_UserItem[tSlot].sBullNum);
			TempBuf.Add(m_UserItem[tSlot].sCount);
			for(i =0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tSlot].tMagic[i]);
			TempBuf.Add(m_UserItem[tSlot].tIQ); 
				
			Send(TempBuf, TempBuf.GetLength());				
		}
		else
		{
			TempBuf.Add(ITEM_DURATION);
			TempBuf.Add(tSlot);
			TempBuf.Add(m_UserItem[tSlot].sDuration);
			Send(TempBuf, TempBuf.GetLength());
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	User 狼 捞抚捞 捞固 粮犁窍绰瘤 眉农茄促.
//
void USER::CheckIDReq(TCHAR* pBuf)
{
	TCHAR szName[CHAR_NAME_LENGTH + 1];
	int nLen = 0;
	BYTE tResult = FAIL;
	int index = 0;
	CBufferEx TempBuf;

	::ZeroMemory(szName, sizeof(szName));
	nLen = GetVarString(sizeof(szName), szName, pBuf, index);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH - 3) goto go_result;

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
		return;
	}

	if(!IsExistCharId(szName)) tResult = SUCCESS;

go_result:

	TempBuf.Add(CHECK_ID_RESULT);
	TempBuf.Add(tResult);

	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////////////
//	荤捞坷葱 嚼垫 备浇阑 荤侩窍咯 荤捞坷葱阑 嚼垫茄促.
//
BOOL USER::UsePsiStone(short sSlot)
{
	CBufferEx TempBuf;
	short sSid = 0;
	short sPsionicSid = 0;
	int j;

	if(sSlot < EQUIP_ITEM_NUM || sSlot >= TOTAL_INVEN_MAX) return FALSE;
	
	sSid = m_UserItem[sSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
	if(m_UserItem[sSlot].sCount <= 0) return FALSE;
	
	switch(sSid)
	{
	case PSI_STONE_FAST_RUN	:  //拳80级技能
		if(m_byClass != BRAWL)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 80) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
// 		if(FindEvent(6) == FALSE) 
// 		{
// 			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
// 			return FALSE;
// 		}
		if(g_arPsiTable[PSIONIC_FAST_RUN]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_FAST_RUN) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_FAST_RUN;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_FAST_RUN]->m_iNeedXP;
		break;
   
	case PSI_STONE_BAO_YAN :		//拳 暴炎	
		if(m_byClass != BRAWL)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 50) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
//		if(FindEvent(6) == FALSE) 
//		{
//			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
//			return FALSE;
//		}
        if(g_arPsiTable[PSIONIC_BAO_YAN]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_BAO_YAN) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_BAO_YAN;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_BAO_YAN]->m_iNeedXP;
		break;

	case PSI_STONE_CHAO_FAN_TAN :		//拳 超级反弹	
		if(m_byClass != BRAWL)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 135) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
//		if(FindEvent(6) == FALSE) 
//		{
//			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
//			return FALSE;
//		}
        if(g_arPsiTable[PSIONIC_CHAO_FAN_TAN]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_CHAO_FAN_TAN) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_CHAO_FAN_TAN;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_CHAO_FAN_TAN]->m_iNeedXP;
		break;
	
	case PSI_STONE_MIND_SHOCK :		//法 心灵风暴			
		if(m_byClass != STAFF)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 80) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
//		if(FindEvent(6) == FALSE) 
//		{
//			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
//			return FALSE;
//		}

		if(g_arPsiTable[PSIONIC_MIND_SHOCK]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_MIND_SHOCK) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_MIND_SHOCK;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_MIND_SHOCK]->m_iNeedXP;
		break;

	case PSI_STONE_CHAO_YI_NENG :		//法 超级异能			
		if(m_byClass != STAFF)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 135) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
//		if(FindEvent(6) == FALSE) 
//		{
//			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
//			return FALSE;
//		}

		if(g_arPsiTable[PSIONIC_CHAO_YI_NENG]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_CHAO_YI_NENG) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_CHAO_YI_NENG;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_CHAO_YI_NENG]->m_iNeedXP;
		break;

	case PSI_STONE_MIND_GUARD :					
		if(m_byClass != STAFF)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 90) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
//		if(FindEvent(6) == FALSE) 
//		{
//			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
//			return FALSE;
//		}
		if(g_arPsiTable[PSIONIC_MIND_GUARD]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_MIND_GUARD) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_MIND_GUARD;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_MIND_GUARD]->m_iNeedXP;
		break;

	case PSI_STONE_PSI_SHIELD :					
		if(m_byClass != EDGED)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 80) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
//		if(FindEvent(6) == FALSE) 
//		{
//			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
//			return FALSE;
//		}	

		if(g_arPsiTable[PSIONIC_PSI_SHIELD]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_PSI_SHIELD) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_PSI_SHIELD;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_PSI_SHIELD]->m_iNeedXP;
		break;

	case PSI_STONE_BA_QI :		//剑 罡气	
		if(m_byClass != EDGED)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 50) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(g_arPsiTable[PSIONIC_BA_QI]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_BA_QI) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_BA_QI;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_BA_QI]->m_iNeedXP;
		break;

	case PSI_STONE_CHAO_FENG_LI :		//剑 超级锋利	
		if(m_byClass != EDGED)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 135) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(g_arPsiTable[PSIONIC_CHAO_FENG_LI]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_CHAO_FENG_LI) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_CHAO_FENG_LI;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_CHAO_FENG_LI]->m_iNeedXP;
		break;

	case PSI_STONE_CHONG_RAN :		//重燃	
/*		if(m_byClass != EDGED)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}*/
		if(m_sLevel < 50) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(g_arPsiTable[PSIONIC_CHONG_RAN]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_CHONG_RAN) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_CHONG_RAN;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_CHONG_RAN]->m_iNeedXP;
		break;

	case PSI_STONE_LI_HU_SHUO_LIAN :		//灵魂锁链
		if(m_byClass != JUDGE)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 80) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(g_arPsiTable[PSIONIC_LI_HU_SHUO_LIAN]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_LI_HU_SHUO_LIAN) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_LI_HU_SHUO_LIAN;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_LI_HU_SHUO_LIAN]->m_iNeedXP;
		break;
	
	case PSI_STONE_PIERCING_SHIELD :		//枪 力场穿刺	
		if(m_byClass != FIREARMS)
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 80) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
//		if(FindEvent(6) == FALSE) 
//		{
//			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
//			return FALSE;
//		}

		if(g_arPsiTable[PSIONIC_PIERCING_SHIELD]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_PIERCING_SHIELD) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_PIERCING_SHIELD;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_PIERCING_SHIELD]->m_iNeedXP;
		break;

	case PSI_STONE_CHAO_DUO_CHONG :		//枪和仲裁 超级多重	
		if(m_byClass != FIREARMS && m_byClass != JUDGE)//不是枪械师或仲裁无法学习
		{
			SendSystemMsg(IDS_PSI_ERROR_CLASS, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
		if(m_sLevel < 135) 
		{
			SendSystemMsg(IDS_PSI_ERROR_LEVEL, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}
//		if(FindEvent(6) == FALSE) 
//		{
//			SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);
//			return FALSE;
//		}

		if(g_arPsiTable[PSIONIC_CHAO_DUO_CHONG]->m_iNeedXP > m_dwXP)
		{
			SendSystemMsg(IDS_PSI_ERROR_XP, SYSTEM_ERROR, TO_ME);
			return FALSE;
		}

		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == PSIONIC_CHAO_DUO_CHONG) 
			{
				SendSystemMsg(IDS_PSI_ERROR_EXIST, SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
		}

		sPsionicSid = m_UserPsi[m_nHavePsiNum].sSid = PSIONIC_CHAO_DUO_CHONG;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
		m_dwXP -= g_arPsiTable[PSIONIC_CHAO_DUO_CHONG]->m_iNeedXP;
		break;
	}

	TempBuf.Add(BUY_PSY_RESULT);
	TempBuf.Add((BYTE)1);
	TempBuf.Add((short)1);
	TempBuf.Add(sPsionicSid);
	Send(TempBuf, TempBuf.GetLength());

	SendXP();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//	Quest Window 甫 楷促(蜡历啊 啊瘤绊 乐绰 涅胶飘 沥焊甫 焊郴霖促.)
//  CTRL+Q   任务栏打开
void USER::QuestWindowOpenReq(TCHAR *pBuf)
{
	CBufferEx TempBuf;
	int iCount = m_arEventNum.GetSize();

	if(iCount >= MAX_EVENT_NUM) iCount = MAX_EVENT_NUM;

	TempBuf.Add(QUESTWINDOW_OPEN_RESULT);

	TempBuf.Add((short)iCount);
	for( int i = 0; i < iCount; i++)
	{
		TempBuf.Add((short)(*m_arEventNum[i]));
	}

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	Quest say甫 焊辰促
//
void USER::SendMonsterSay(CNpc *pNpc, int iType, char *strSay)
{
	if( !pNpc ) return;
	if( strlen( strSay ) >= 128 ) return;
	
	CBufferEx TempBuf;

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(NORMAL_CHAT);
	TempBuf.Add(pNpc->m_sNid + NPC_BAND);
	TempBuf.AddString(pNpc->m_strName);

	TempBuf.AddString((LPTSTR)(LPCTSTR)strSay);

	switch(iType)
	{
	case 0:
		Send(TempBuf, TempBuf.GetLength());
		break;
	case 1:
		SendExactScreen(TempBuf, TempBuf.GetLength());
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////
//	Quest 甫 角青茄促. (捞亥飘 蜡瘤矫埃, 铰府炼扒, 各 SID, 付府荐, 粮, ...)
//
BOOL USER::ExcuteSingleQuestEvent(int time, int type, int sid, int count, int z, int x, int y)
{
	int iEvent = g_QuestEventZone.GetEmptyEventZone();

	if(iEvent < 0)
	{
		SendNpcSay( NULL, 513 );										// 角菩 say
		return FALSE;													
	}

	g_QuestEventZone.m_SingleEventZone[iEvent].m_sEventZone = z;
	g_QuestEventZone.m_SingleEventZone[iEvent].m_tEventTime = time;		// 捞亥飘 柳青矫埃
	g_QuestEventZone.m_SingleEventZone[iEvent].m_tSuccessType = type;	// 铰府炼扒

	g_QuestEventZone.m_SingleEventZone[iEvent].m_arUserList[0].uid = m_uid; // 曼啊 蜡历 

	int nLen = strlen(m_strUserID);
	::CopyMemory(g_QuestEventZone.m_SingleEventZone[iEvent].m_arUserList[0].strUserName, m_strUserID, nLen);

	m_tQuestWar = GUILD_WARRING;
	 
	int iNid = -1;
	for(int i = 0; i < count; i++)
	{
		iNid = -1;
		iNid = SummonQuestMonster(sid, z, x, y);									// 各 家券
		if(iNid > -1) g_QuestEventZone.m_SingleEventZone[iEvent].m_arNpcList.Add(iNid);	// 秦寸 各
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//	Quest 各阑 漂沥 谅钎肺 家券茄促.
//
int USER::SummonQuestMonster(int sid, int z, int x, int y)
{
	CNpc* pNpc = NULL;
	int i;

	MYSHORT sAI;
	BYTE upTemp = 0;			// 惑困 8厚飘
	BYTE dwTemp = 0;			// 窍困 8厚飘

	CPoint pt;

	int NpcState = NPC_DEAD;

	for(i = 0; i < g_arNpc.GetSize(); i++)
	{
		pNpc = g_arNpc[i];
		if( !pNpc ) continue;
		if(pNpc->m_tNpcType != NPCTYPE_MONSTER) continue;

		if(pNpc->m_sSid == sid)
		{
			if(pNpc->m_sCurZ == 1005) continue;
			if(pNpc->m_NpcState != NPC_DEAD && pNpc->m_NpcState != NPC_STANDING) continue;
			if(pNpc->m_bSummon == TRUE) continue;

			if(::InterlockedExchange(&pNpc->m_lNowSummoning, 1) != 0) continue;

			NpcState = pNpc->m_NpcState;
			pNpc->m_Delay = 2000;

			if(pNpc->m_NpcState == NPC_STANDING)
			{
				pNpc->SetUidNPC(pNpc->m_sCurX, pNpc->m_sCurY, 0);

				pNpc->m_presx = pNpc->m_presy = -1;
				pNpc->SendNpcInfoBySummon(m_pCom);
			}

			pNpc->m_SummonZoneIndex = pNpc->m_ZoneIndex;
			pNpc->m_sSummonOrgZ = pNpc->m_sOrgZ;
			pNpc->m_sSummonOrgX = pNpc->m_sOrgX;
			pNpc->m_sSummonOrgY = pNpc->m_sOrgY;

			pNpc->m_sCurZ = pNpc->m_sOrgZ = z;
			pNpc->m_sCurX = pNpc->m_sOrgX = x;
			pNpc->m_sCurY = pNpc->m_sOrgY = y;

			pNpc->m_ZoneIndex		= -1;

			pNpc->m_dwStepDelay	= GetTickCount();
			
			sAI.i = (short)pNpc->m_sAI;						// NPC AI甫 悸泼
			upTemp = sAI.b[0];
			dwTemp = sAI.b[1];
			
			pNpc->m_tNpcAttType = upTemp >> 7;				// 唱吝俊 眠啊秦具茄促.
			upTemp = upTemp << 1;
			pNpc->m_tNpcLongType = upTemp >> 7;
			upTemp = upTemp << 1;
			pNpc->m_tNpcGroupType = upTemp >> 7;
			
			if(pNpc->m_sClientSpeed <= 20) pNpc->m_sClientSpeed = 20;	// 规绢 内靛;
			
			for(i = 0; i < g_zone.GetSize(); i++)
			{
				if(g_zone[i]->m_Zone == pNpc->m_sCurZ) 
				{
					pNpc->m_ZoneIndex = i;
					break;
				}
			}

			pNpc->m_bSummon = TRUE;
			pNpc->m_bSummonDead = TRUE;
			pNpc->EventNpcInit(x, y);

			pt = pNpc->FindNearRandomPoint(x, y);
			if(pt.x < 0 || pt.y < 0)
			{
				pNpc->m_bFirstLive = FALSE;
				pNpc->m_NpcState = NPC_DEAD;
				pNpc->m_sHP = 0;
				pNpc->m_Delay = pNpc->m_sRegenTime;

				pNpc->m_bSummon = FALSE;
				pNpc->m_bSummonDead = TRUE;

				if(::InterlockedExchange(&pNpc->m_lNowSummoning, 0) != 1) pNpc->m_lNowSummoning = 0;
				return -1;
			}

			if(NpcState == NPC_STANDING) 
			{
				pNpc->m_sCurX = pNpc->m_sOrgX = pt.x;
				pNpc->m_sCurY = pNpc->m_sOrgY = pt.y;

				pNpc->SetUidNPC(pt.x, pt.y, pNpc->m_sNid + NPC_BAND);

				pNpc->m_presx = pNpc->m_presy = -1;
				pNpc->SightRecalc(m_pCom);
			}

			// 家券等 各篮 家券等 谅钎甫 吝缴栏肺 青悼馆版阑 啊柳促.
			MAP* pMap = g_zone[pNpc->m_ZoneIndex];
			pNpc->m_nInitMinX = pNpc->m_sOrgX - pNpc->m_sMinX;		if(pNpc->m_nInitMinX < 1) pNpc->m_nInitMinX = 1; 
			pNpc->m_nInitMinY = pNpc->m_sOrgY - pNpc->m_sMinY;		if(pNpc->m_nInitMinY < 1) pNpc->m_nInitMinY = 1; 
			pNpc->m_nInitMaxX = pNpc->m_sOrgX + pNpc->m_sMaxX;		if(pNpc->m_nInitMaxX >= pMap->m_sizeMap.cx) pNpc->m_nInitMaxX = pMap->m_sizeMap.cx - 1;
			pNpc->m_nInitMaxY = pNpc->m_sOrgY + pNpc->m_sMaxY;		if(pNpc->m_nInitMaxY >= pMap->m_sizeMap.cy) pNpc->m_nInitMaxY = pMap->m_sizeMap.cy - 1;

			if(::InterlockedExchange(&pNpc->m_lNowSummoning, 0) != 1) pNpc->m_lNowSummoning = 0;

			return pNpc->m_sNid;
		}
	}

	return -1;
}

BOOL USER::CheckZoneWho(int zone, int passtype, int zonetype)
{
	int i;
	BOOL bCheck = FALSE;

	switch(zonetype)
	{
	case 1:		// 80肪 荤捞坷葱 包访 - 糕乔胶 厚剐瘤窍楷备家 甘 捞亥飘		
		for(i = 0; i < MAX_SINGLE_EVENT; i++)
		{
			if(g_QuestEventZone.m_SingleEventZone[i].m_lUsed == 1)
			{
				if(g_QuestEventZone.m_SingleEventZone[i].m_sEventZone == zone) bCheck = TRUE;
			}
		}

		if(bCheck) // 秦寸 粮俊 荤恩捞 乐绊 
		{
			if(passtype == 1) return TRUE;	// 烹苞 鸥涝捞 1捞搁 蜡历 烹苞
		}
		else		// 秦寸 粮俊 蜡历啊 绝栏搁
		{
			if(passtype == 0) return TRUE;	// 烹苞 鸥涝捞 0捞搁 蜡历 烹苞
		}
		break;

	default:
		break;
	}

	return FALSE;
}

void USER::SendCharDataToOPUser(USER *pUser)//发送角色装备属性信息
{
/*    if(!pUser) return;
	
	int i = 0;
    CBufferEx TempBuf;
    
    TempBuf.Add(CHAR_DATA);//支持看对方属性栏
    TempBuf.AddString(pUser->m_strUserID);
    TempBuf.Add(pUser->m_sMagicSTR);
    TempBuf.Add(pUser->m_sMagicCON);
    TempBuf.Add(pUser->m_sMagicDEX);
    TempBuf.Add(pUser->m_sMagicVOL);
    TempBuf.Add(pUser->m_sMagicWIS);

    TempBuf.Add((short)1);  // 烙矫 内靛 

    TempBuf.Add(pUser->m_iSkin);
    TempBuf.Add(pUser->m_iHair);
    TempBuf.Add((BYTE)pUser->m_sGender);
    TempBuf.AddData(pUser->m_strFace, 10);

    TempBuf.Add(pUser->m_dwExp);
    TempBuf.Add(pUser->m_dwXP);

    TempBuf.Add(pUser->m_sSkillPoint);
    TempBuf.Add(pUser->m_sPA);
    TempBuf.Add(pUser->m_sSkillPoint_);

    TempBuf.Add(pUser->m_sMagicMaxHP);
    TempBuf.Add(pUser->m_sHP);
    TempBuf.Add(pUser->m_sMagicMaxPP);
    TempBuf.Add(pUser->m_sPP);
    TempBuf.Add(pUser->m_sMagicMaxSP);
    TempBuf.Add(pUser->m_sSP);

    TempBuf.Add(pUser->m_dwDN);     
    TempBuf.Add(pUser->m_sCityRank);
	
    TempBuf.Add(pUser->m_sLevel);
    TempBuf.Add(pUser->m_byClass); 

    TCHAR strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB],strSkill_[12*3];
    ::ZeroMemory(strSkill, sizeof(strSkill));
    ::ZeroMemory(strItem, sizeof(strItem));
    ::ZeroMemory(strPsi, sizeof(strPsi));

    UserSkillToStr((LPTSTR)strSkill);
    pUser->UserItemToStrForSend( (LPTSTR)strItem );
    UserPsiToStr((LPTSTR)strPsi);
    //130SUPER SKILL
    int index=0;
    for(i = 0; i < 12; i++)
    {
    SetShort(strSkill_, m_UserSkill_[i].sSid,index);
    SetByte (strSkill_, m_UserSkill_[i].tLevel, index);
    }

    TempBuf.AddData(strSkill, USER_SKILL_LEN); // Skill
    TempBuf.AddData(strSkill_,12*3);

    TempBuf.Add((BYTE)m_nHavePsiNum); // Psionic
    if(m_nHavePsiNum > 0) TempBuf.AddData(strPsi, m_nHavePsiNum * _PSI_SIZE); 
    TempBuf.AddData(strItem, USER_ITEM_LEN); // Item

    TempBuf.Add(m_dwExpNext); // Next Exp 
    TempBuf.Add(m_dwAbnormalInfo); // 惑怕捞惑 沥焊
    TempBuf.Add(m_dwAbnormalInfo_);

    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    // Finito added 12 bytes below
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0x00);

    TempBuf.Add((BYTE)0x01);
    TempBuf.Add((BYTE)0x01);
    TempBuf.Add((BYTE)0x01);
    TempBuf.Add((BYTE)0x01);

    TempBuf.Add((int)pUser->m_dwGuild); 
    TempBuf.AddString(m_strGuildName); 
    TempBuf.Add(m_sGuildVersion);
    if(m_dwGuild >= 1)
    {
 //   TempBuf.Add((BYTE)0xF4);
 //   TempBuf.Add((BYTE)0x02);
//    TempBuf.Add((BYTE)0x41);
    }
    else
    {
 //   TempBuf.Add((BYTE)0x00);
//    TempBuf.Add((BYTE)0x00);
 //   TempBuf.Add((BYTE)0x00);
    }
    TempBuf.Add((BYTE)0x02);
 //   TempBuf.Add((BYTE)0xFF);
//	TempBuf.Add((BYTE)0xFF);
//	TempBuf.Add((BYTE)0x00);
//	TempBuf.Add((BYTE)0x00);
    TempBuf.Add((BYTE)0xFF);
    TempBuf.Add((short)-1);//开机蓝龙心
    TempBuf.Add((BYTE)0x84); // Finito Added
    TempBuf.Add((BYTE)0x99); // Finito Added
    TempBuf.Add((BYTE)0x2c); // Finito Added
    TempBuf.Add((BYTE)0x02); // Finito Added

    CBufferEx TempBuf1;
    TempBuf1.Add( (short)(TempBuf.GetLength()) );
    TempBuf1.AddData( TempBuf, TempBuf.GetLength() );
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
    m_CompMng.FlushAddData();*/
} 
/////////////////////////////////////////////////////////////////
//商店物品购买记录
void USER::WriteOnlineShop_Log(CString strUname,CString osName,short price,short inum,short upg,short sx1,short sx2,short sx3,short sx4,short sx5,short sx6,short sx7,short sx8,short sx9,short sx10)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	TCHAR			szSQL[1024];		

	::ZeroMemory(szSQL, sizeof(szSQL));

	int index = 0;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call INSERT_ONLINESHOP_LOG (\'%s\',\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)}"), 
	strUname,osName,price,inum,upg,sx1,sx2,sx3,sx4,sx5,sx6,sx7,sx8,sx9,sx10); 

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		return;

	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
		}
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
}
//-------------------------------------------------------------------------
//yskang 0.4 款康磊 肺弊 扁废
//nOption 0:立加酒捞乔 1:措拳 2:酒捞袍 力傍沥焊 3:PK沥焊
//-------------------------------------------------------------------------
void USER::WriteOpratorLog(TCHAR *strContents, int nOption)//yskang 0.4
{
//	if( m_tIsOP == 0 ) return;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	TCHAR			szSQL[1024];	
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));

	SDWORD sSLen = strlen(strContents);
	if(sSLen <= 0 || sSLen >= 200) return;

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call WRITE_OPERATOR_LOG(\'%s\', \'%s\',%d)}"), m_strUserID, strContents,nOption ); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
		i = 1;
//		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strSentence), 0, (TCHAR*)strSentence, 0, &sSLen );

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
		}
	}
	else return;

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
void USER::SetPsiOne(BYTE tKind, DWORD dwTime)
{
	m_dwHasteTime = m_dwShieldTime = m_dwDexUpTime = m_dwMaxHPUpTime = 0;
	m_dwFastRunTime = m_dwBigShieldTime = m_dwMindShockTime = m_dwPsiShieldTime = m_dwPiercingShieldTime = 0;

	switch(tKind)
	{
	case PSIONIC_HASTE: //加速
		m_dwHasteTime = dwTime;
		break;
	case PSIONIC_SHIELD://防护
		m_dwShieldTime = dwTime;
		break;
	case PSIONIC_DEXUP://疾风之雨
		m_dwDexUpTime = dwTime;
		break;
	case PSIONIC_HPUP://生命之光
		m_dwMaxHPUpTime = dwTime;
		break;
	case PSIONIC_FAST_RUN:
		m_dwFastRunTime = dwTime;
		break;
	case PSIONIC_MIND_SHOCK:
		m_dwMindShockTime = dwTime;
		break;
	case PSIONIC_PSI_SHIELD:
		m_dwPsiShieldTime = dwTime;
		break;
	case 30:
		m_dwBigShieldTime = dwTime;
		break;
	case PSIONIC_PIERCING_SHIELD:
		m_dwPiercingShieldTime = dwTime;
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	DB 肺 何磐 规绢仿诀, 傍拜仿诀, 滚辑目 沥焊甫 悸泼茄促.
//
void USER::SetPsiTwo(BYTE tKind, DWORD dwTime)
{
	m_dwAdamantineTime = m_dwMightyWeaponTime = m_dwBerserkerTime = 0;

	switch(tKind)
	{
	case PSIONIC_ADAMANTINE:
		m_dwAdamantineTime = dwTime;
		break;
	case PSIONIC_MIGHTYWEAPON:
		m_dwMightyWeaponTime = dwTime;
		break;
	case PSIONIC_BERSERKER:
		m_dwBerserkerTime = dwTime;
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	DB 肺 何磐 付牢靛啊靛 沥焊甫 悸泼茄促.
//
void USER::SetPsiThree(BYTE tKind, DWORD dwTime)
{
	m_dwMindGuardTime = 0;

	switch(tKind)
	{
	case PSIONIC_MIND_GUARD:
		m_dwMindGuardTime = dwTime;
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	魔傍 捞亥飘甫 困窍咯 各阑 家券茄促.
//  无尽的召唤
BOOL USER::InvaderSummon(BYTE tSlot)
{
	if(tSlot < 0 || tSlot >= TOTAL_INVEN_MAX) return FALSE;

	short sItemSid = m_UserItem[tSlot].sSid;
	if(sItemSid < 0 || sItemSid >= g_arItemTable.GetSize()) return FALSE;

	CPoint pt(-1, -1);

	short sMonsterID = myrand(184, 190);
	switch (sItemSid)
	{
	case 715:
		sMonsterID = 184;
		break;
	case 716:
		sMonsterID = 185;
		break;
	case 717:
        sMonsterID = 186;
		break;
	case 718:
		sMonsterID = 187;
		break;
	case 719:
		sMonsterID = 188;
		break;
	case 720:
		sMonsterID = 189;
		break;
	case 721:
		sMonsterID = 190;
		break;
	}
	
	if(sMonsterID < 0 || sMonsterID >= g_arNpc.GetSize()) return FALSE;
	

	g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_bMove = 1;
	pt = FindNearAvailablePoint_S(m_curx, m_cury);
	g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_bMove = 0;
	
	if(pt.x == -1 || pt.y == -1) return FALSE;
	
	if(SummonQuestMonster(sMonsterID, m_curz, pt.x, pt.y) == -1) return FALSE;
//	if(SummonQuestMonster(sMonsterID, m_curz, m_curx, m_cury) == -1) return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//	镜葛绝绰 概流, 饭绢 酒捞袍阑 倒妨辑 货肺款 可记阑 掘绰促.
//
void USER::RemagicItemReq(TCHAR *pBuf, BOOL bEvent)
{
	if(m_dwDN < ITEM_REMAGIC_COST) 
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return;		// 诀弊饭捞靛 且 捣捞 绝栏搁 府畔
	}

	CBufferEx TempBuf;

	int iStart = 0, iCount = 3;
	int index = 0, iSuccess = 0;
	int i, j;
	int iWeight = 0;
	BYTE tItemSlot = -1;
	BYTE tMaterialSlot[3] = {0, 0, 0};
	BYTE bySlot = 0;
	short sMaterialSid[3] = {-1, -1, -1};
	CString szName1, szName2, szName3;
	ItemList RemagicItem;

	CByteArray arMaterial;
	arMaterial.RemoveAll();

	short sSourceSlot = GetShort(pBuf, index);	// 急琶茄 酒捞袍 浇吩锅龋

	if(sSourceSlot != -1) return;				// 府概流篮 家胶浇吩捞 绝促.

	if(bEvent)	// 农府胶付胶 捞亥飘 酒捞袍阑 荤侩茄 版快
	{
//		if(!0)
//		{
			SendSystemMsg(IDS_EVENT_END, SYSTEM_NORMAL, TO_ME);
			return;
//		}
		iStart = 1;
		tItemSlot = GetShort(pBuf, index);
		if(tItemSlot < EQUIP_ITEM_NUM && tItemSlot >= TOTAL_INVEN_MAX) return;
		if(m_UserItem[tItemSlot].sSid != 872) return;		// 农府胶付胶 阜措荤帕捞 酒聪搁 府畔
		if(m_UserItem[tItemSlot].sCount < 1) return;		// 俺荐啊 葛肋矾档 府畔

		arMaterial.Add(tItemSlot);
		iWeight += g_arItemTable[872]->m_byWeight;			// 公霸函拳甫 眉农茄促.
	}

	j = 0;
	for(i = iStart; i < iCount + iStart; i++, j++)				// 概流 犁丰 八荤	
	{
		tMaterialSlot[j] = GetShort(pBuf, index);				// 犁丰 ( 绝栏搁 -1 )
		if(tMaterialSlot[j] < EQUIP_ITEM_NUM && tMaterialSlot[j] >= TOTAL_INVEN_MAX) return;

		sMaterialSid[j] = m_UserItem[tMaterialSlot[j]].sSid;
		if(m_UserItem[tMaterialSlot[j]].sCount <= 0) return;
		if(m_UserItem[tMaterialSlot[j]].tIQ != UNIQUE_ITEM) if(m_UserItem[tMaterialSlot[j]].tMagic[5] != 0) return;		// 诀弊饭捞靛 酒捞袍篮 府概流捞 救等促.
		if(m_UserItem[tMaterialSlot[j]].tIQ == REMODEL_ITEM || m_UserItem[tMaterialSlot[j]].tIQ == REMODEL_MAGIC_ITEM) return; //俺炼 酒捞袍篮 府概流捞 救等促.
		if(m_UserItem[tMaterialSlot[j]].tIQ != RARE_ITEM) return;//不是金色装备不能合成魔法盛装
		if(sMaterialSid[j] < 0 || sMaterialSid[j] >= g_arItemTable.GetSize()) return;
		if(g_arItemTable[sMaterialSid[j]]->m_byWear < 1 || g_arItemTable[sMaterialSid[j]]->m_byWear > 5) return;	//公扁, 规绢备父
	}

	// 鞍篮 辆幅狼 酒捞袍牢瘤 八荤茄促.
	szName1 = g_arItemTable[sMaterialSid[0]]->m_strName;
	szName2 = g_arItemTable[sMaterialSid[1]]->m_strName;
	szName3 = g_arItemTable[sMaterialSid[2]]->m_strName;

	RemagicItem = m_UserItem[tMaterialSlot[0]];
	if(szName1 != szName2 || szName1 != szName3 || szName2 != szName3) return;

	iSuccess = GetMagicOption(&RemagicItem, 3, bEvent);
	RemagicItem.tIQ = RARE_ITEM;
	if(iSuccess == 0) return;

	
	m_UserItem[tMaterialSlot[0]] = RemagicItem;		// 概流酒捞袍俊 货肺款 可记何咯

	for(i = 0; i < 3; i++)	
	{
		if(i != 0) 
		{
			iWeight += g_arItemTable[sMaterialSid[i]]->m_byWeight;			// 公霸函拳甫 眉农茄促.
			MakeItemLog(&m_UserItem[tMaterialSlot[i]], ITEMLOG_REMAGIC_DELETE );
			ReSetItemSlot(&m_UserItem[tMaterialSlot[i]]);
		}
		arMaterial.Add(tMaterialSlot[i]);
	}

	if(tItemSlot != -1)
	{
		if(m_UserItem[tItemSlot].sCount <= 1)
		{
			MakeItemLog(&m_UserItem[tItemSlot], ITEMLOG_REMAGIC_DELETE );
			ReSetItemSlot(&m_UserItem[tItemSlot]);
		}
		else
		{
			m_UserItem[tItemSlot].sCount -= 1;
		}
	}

	FlushItemLog( TRUE );

	if( m_dwDN <= ITEM_REMAGIC_COST ) m_dwDN = 0;
	else m_dwDN = m_dwDN - ITEM_REMAGIC_COST;
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();							// 捣函版 沥焊甫 焊辰促.

	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();

	TempBuf.Add((BYTE)0x03); // 府概流 己傍
	TempBuf.Add((BYTE)index);

	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		bySlot = (BYTE)arMaterial[i];
	
		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...

	Send(TempBuf, TempBuf.GetLength());

	arMaterial.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////////////
//	货肺款 概流 可记阑 掘绰促.
//
BOOL USER::GetMagicOption(ItemList* pItem, BYTE tIQ, BYTE tSame)
{
	int iMagicCount = 0;
	int nLoop = 0;
	int i = 0, j = 0, iRandom = 0;
	int iCount = 0, iTemp = 0;
	short sSid = -1;

	if(pItem == NULL) return FALSE;
	sSid = pItem->sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;

	if(tIQ == MAGIC_ITEM) nLoop = 2;
	else if(tIQ == RARE_ITEM) nLoop = 4;
	else return FALSE;             //删除垃圾属性     ///neo版本

	if(m_sLevel <= 20)			iMagicCount = 42;		// 概流牢郸胶 裹困甫 饭骇俊 蝶扼 力茄茄促. 
	else if(m_sLevel <= 40)		iMagicCount = 106;
	else if(m_sLevel <= 90)		iMagicCount = 112;
	else						iMagicCount = 145;

	if(iMagicCount >= g_arMagicItemTable.GetSize()) iMagicCount = g_arMagicItemTable.GetSize() - 1;

	pItem->tMagic[0] = pItem->tMagic[1] = pItem->tMagic[2] = pItem->tMagic[3] = pItem->tMagic[4] = pItem->tMagic[5] = 0;

	i = 0;
	while(nLoop > i)										
	{    
	//	sRandom = myrand(1,42);
		iRandom = myrand(1, iMagicCount);

		if(!g_arMagicItemTable[iRandom]->m_tUse) continue;
		if(g_arMagicItemTable[iRandom]->m_sSubType == 32) continue;	// 郴备档 可记篮 嘿捞瘤 臼绰促.
		
		if(CheckClassWear(sSid, iRandom) == FALSE)			// Npc.cpp 狼 CheckClassItem 苞 鞍篮 扁瓷阑 荐青. User 狼 CheckClassItem 阑 龋免窍搁 救凳
		{
			if(i == 0) continue;							// 概流篮 扁夯捞 1俺
			else if(tIQ == RARE_ITEM && i <= 2) continue;	// 贰绢绰 扁夯捞 3俺
			else { i++; continue; }
		}

		for(j = 0; j < 4; j++)
		{
			if(tSame != TRUE)
			{
				iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType;
				if(iCount != 0 && iCount == g_arMagicItemTable[iRandom]->m_sSubType)	// 加己捞 般磨荐 乐栏骨肺 捞吝 奴蔼父 急琶	
				{
					iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sChangeValue;
					if(iCount < g_arMagicItemTable[iRandom]->m_sChangeValue)
					{
						iTemp = g_arMagicItemTable[pItem->tMagic[j]]->m_tLevel;
						if(pItem->sLevel - iTemp > 0) pItem->sLevel -= iTemp;
						pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
						pItem->tMagic[j] = iRandom; 
						
						break;
					}
					else if(iCount == g_arMagicItemTable[iRandom]->m_sChangeValue) break;
				}
			}

			if(pItem->tMagic[j] > 0) continue;										// 捞固 浇吩俊 蔼捞 乐栏搁 逞绢皑
			else
			{ 
				pItem->tMagic[j] = iRandom; i++;
				if(g_arMagicItemTable[iRandom]->m_tLevel > 0) pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
				break; 
			}
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	泅犁 何咯窍妨绰 概流加己捞 秦寸 酒捞袍俊 利侩瞪 荐 乐绰瘤 努贰胶沥焊客 傀绢沥焊甫 八荤茄促.
//
BOOL USER::CheckClassWear(int iItemSid, int iMagicSid)
{
	if(iItemSid < 0 || iItemSid >= g_arItemTable.GetSize()) return FALSE;
	if(iMagicSid < 0 || iMagicSid >= g_arMagicItemTable.GetSize()) return FALSE;

	BYTE tItemWear = g_arItemTable[iItemSid]->m_byWear;				// 酒捞袍 傀绢沥焊
	BYTE tMagicWear = g_arMagicItemTable[iMagicSid]->m_tWearInfo;	// 概流可记 傀绢 沥焊

	BYTE tNeedClass = g_arItemTable[iItemSid]->m_byClass;			
	BYTE tMagicClass = g_arMagicItemTable[iMagicSid]->m_tNeedClass;	// 概流加己 努贰胶

	// Class 眉农
	if(tMagicClass != 15)		// 葛电拌凯俊 嘿阑 荐 乐绰 可记捞 酒聪搁
	{
		BYTE tTemp = 1;	
		BYTE tFire = 0;
		BYTE tEdge = 0;
		BYTE tStaff = 0;
		BYTE tBrawl = 0;

		tFire	 = tTemp & tNeedClass; tTemp = 2; 
		tEdge	 = tTemp & tNeedClass; tTemp = 4;
		tStaff	 = tTemp & tNeedClass; tTemp = 8;
		tBrawl	 = tTemp & tNeedClass;

		tFire = tFire & tMagicClass;
		tEdge = tEdge & tMagicClass;
		tStaff = tStaff & tMagicClass;
		tBrawl = tBrawl & tMagicClass;

		tTemp = tFire^tEdge^tStaff^tBrawl;
		if(!tTemp) return FALSE;
	}

	// WearInfo 眉农
	if(tMagicWear == 0) return TRUE;
	else if(tMagicWear == 1)											
	{														// 1锅捞搁 公扁幅俊 嘿绰促.
		if(tItemWear != 1) return FALSE;
		else return TRUE;
	}
	else if(tMagicWear == 2)								
	{
		if(tItemWear <= 1 || tItemWear >= 9) return FALSE;
		else return TRUE;
	}
	else return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
//	EBody 甫 诀弊饭捞靛 茄促.
//
void USER::EBodyUpgradeReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	if(m_dwDN < EBODY_UPGRADE_COST) 
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return;		// 诀弊饭捞靛 且 捣捞 绝栏搁 府畔
	}
	if(m_dwXP < EBODY_UPGRADE_XP) 
	{
		SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
		return;		// 诀弊饭捞靛 XP啊 绝栏搁 府畔
	}

	CBufferEx TempBuf;

	int iCount = 1, iUpgradeCount = 0;
	int index = 0, iSuccess = 0;
	int i, j;
	int iWeight = 0;
	BYTE tItemSlot = -1;
	short sMaterialSlot[2] = {0};  //short sMaterialSlot = 0;
	BYTE bySlot = 0;
	short sMaterialSid = -1;
	short sSourceSid = -1;
	BYTE tSourceWear = 0;
	int iRandom = 0;
	int iRate1 = 0, iRate2 = 0;

	ItemList TempItem;

	CByteArray arMaterial;
	arMaterial.RemoveAll();

	short sSourceSlot = GetShort(pBuf, index);	
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;	// 牢亥俊辑父 诀弊饭捞靛 啊瓷

	sSourceSid = m_UserItem[sSourceSlot].sSid;
	if(sSourceSid < 0 || sSourceSid >= g_arItemTable.GetSize()) return;

	for(i = 1; i < 5; i++)
	{
		if(m_UserItem[sSourceSlot].tMagic[i] <= 0) 
		{
			iUpgradeCount = i - 1;
			break;
		}
	}
	if(iUpgradeCount < 0 || iUpgradeCount > 3) return;	// EBdoy Upgrade 甫 捞固 4锅 促沁促.
	if(iUpgradeCount >= g_arEBodyUpgradeTable.GetSize()) return;	// 肋给等 诀弊饭捞靛 蔼

	tSourceWear = g_arItemTable[sSourceSid]->m_byWear;
	if(tSourceWear < 122 || tSourceWear > 125)			// 诀弊饭捞靛 窍妨绰 酒捞袍捞 EBody 啊 酒聪扼搁
	{
		SendSystemMsg( IDS_USER_CANT_UPGRADE_ITEM, SYSTEM_ERROR, TO_ME);
		return;
	}

	j = 0;
	//判断是否为超级机械
	if(IsSuperEbodyItem(sSourceSid))
        iCount = 2;
    BYTE bEM[2] ={ 0xff, 0xff};
	for(i = 0; i < iCount ; i++)				// 概流 犁丰 八荤	
	{
		sMaterialSlot[i] = GetShort(pBuf, index);	// 犁丰 ( 绝栏搁 -1 )
		if(sMaterialSlot[i] < EQUIP_ITEM_NUM && sMaterialSlot[i] >= TOTAL_INVEN_MAX) return;

		sMaterialSid = m_UserItem[sMaterialSlot[i]].sSid;
		if(m_UserItem[sMaterialSlot[i]].sCount != 1) return;			
		if(m_UserItem[sMaterialSlot[i]].tIQ != MAGIC_ITEM) return;		// 俊捞歹绰 公炼扒 概流 酒捞袍
		
		if(sMaterialSid < 0 || sMaterialSid >= g_arItemTable.GetSize()) return;
		if(g_arItemTable[sMaterialSid]->m_byWear != 126) return;	// 俊捞歹啊 酒聪搁 府畔
		bEM[i] = m_UserItem[sMaterialSlot[i]].tMagic[0];
	}
    //判断属性是否一致
	if(iCount == 2)
	{
		if(bEM[0]!=bEM[1]&&bEM[0]!=0xff)
			return;
	}
	iRandom = myrand(1, 100);
	iRate1 = g_arEBodyUpgradeTable[iUpgradeCount]->m_tRandom1;
	iRate2 = g_arEBodyUpgradeTable[iUpgradeCount]->m_tRandom2;

	if(iRandom <= iRate1) iSuccess = 1;
	else if(iRate1 < iRandom && iRandom <= iRate1 + iRate2) iSuccess = 0;
	else iSuccess = -1;

	if(iSuccess == -1)		// 肯傈角菩, 海捞胶客 俊捞歹鳖瘤 朝扼埃促.
	{
		MakeItemLog(&m_UserItem[sSourceSlot], ITEMLOG_EBODY_BASE_DELETE );
		MakeItemLog(&m_UserItem[sMaterialSlot[0]], ITEMLOG_EBODY_ADDER_DELETE );
		//超级机械
		if(iCount ==2)
			MakeItemLog(&m_UserItem[sMaterialSlot[1]], ITEMLOG_EBODY_ADDER_DELETE );

		ReSetItemSlot(&m_UserItem[sSourceSlot]);
		ReSetItemSlot(&m_UserItem[sMaterialSlot[0]]);
		if(iCount ==2)
			ReSetItemSlot(&m_UserItem[sMaterialSlot[1]]);

		iWeight += g_arItemTable[sSourceSid]->m_byWeight;			// 公霸函拳甫 眉农茄促.
		iWeight += g_arItemTable[sMaterialSid]->m_byWeight;		

		arMaterial.Add((BYTE)sSourceSlot);
		arMaterial.Add((BYTE)sMaterialSlot[0]);
		if(iCount == 2)
			arMaterial.Add((BYTE)sMaterialSlot[1]);
	}
	else if(iSuccess == 0)	// 俊捞歹父 朝扼埃促.
	{
		MakeItemLog(&m_UserItem[sMaterialSlot[0]], ITEMLOG_EBODY_ADDER_DELETE );
		if(iCount == 2)
			MakeItemLog(&m_UserItem[sMaterialSlot[1]], ITEMLOG_EBODY_ADDER_DELETE );
		ReSetItemSlot(&m_UserItem[sMaterialSlot[0]]);
		if(iCount == 2)
			ReSetItemSlot(&m_UserItem[sMaterialSlot[1]]);
		iWeight += g_arItemTable[sMaterialSid]->m_byWeight;		// 公霸函拳甫 眉农茄促.

		arMaterial.Add((BYTE)sMaterialSlot[0]);
		if(iCount == 2)	
			arMaterial.Add((BYTE)sMaterialSlot[1]);
	}
	else					// 己傍, 俊捞歹绰 朝府绊, 海捞胶俊绰 货肺款 概流加己阑 眠啊茄促.
	{
		m_UserItem[sSourceSlot].tMagic[iUpgradeCount + 1] = m_UserItem[sMaterialSlot[0]].tMagic[0];
		m_UserItem[sSourceSlot].sLevel += m_UserItem[sMaterialSlot[0]].sLevel;		// EBody 绰 诀弊饭捞靛 瞪锭付促 饭骇捞 鞍捞 棵扼埃促.
        if(m_UserItem[sSourceSlot].tMagic[iUpgradeCount + 1] ==16)//大于或等于8改提示
		{
         CString sysMsg;
         sysMsg.Format("玩家 %s 将[%s]机械在第 %d 改强化上力量10转伤害! ",this->m_strUserID,g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_strName ,iUpgradeCount + 1);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
		if(m_UserItem[sSourceSlot].tMagic[iUpgradeCount + 1] ==24)//大于或等于8改提示
		{
         CString sysMsg;
         sysMsg.Format("玩家 %s 将[%s]机械在第 %d 改强化上敏捷10转伤害! ",this->m_strUserID,g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_strName ,iUpgradeCount + 1);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
		if(m_UserItem[sSourceSlot].tMagic[iUpgradeCount + 1] ==27)//大于或等于8改提示
		{
         CString sysMsg;
         sysMsg.Format("玩家 %s 将[%s]机械在第 %d 改强化上智慧10转魔伤! ",this->m_strUserID,g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_strName ,iUpgradeCount + 1);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}
		if(m_UserItem[sSourceSlot].tMagic[iUpgradeCount + 1] ==14)//大于或等于8改提示
		{
         CString sysMsg;
         sysMsg.Format("玩家 %s 将[%s]机械在第 %d 改强化上释放2锆石! ",this->m_strUserID,g_arItemTable[m_UserItem[sSourceSlot].sSid]->m_strName ,iUpgradeCount + 1);
         SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		}

		MakeItemLog(&m_UserItem[sSourceSlot], ITEMLOG_EBODY_UPGRADE_SUCCESS );
		MakeItemLog(&m_UserItem[sMaterialSlot[0]], ITEMLOG_EBODY_ADDER_DELETE );
		if(iCount == 2)
			MakeItemLog(&m_UserItem[sMaterialSlot[1]], ITEMLOG_EBODY_ADDER_DELETE );

		ReSetItemSlot(&m_UserItem[sMaterialSlot[0]]);
		if(iCount == 2)
			ReSetItemSlot(&m_UserItem[sMaterialSlot[1]]);

		iWeight += g_arItemTable[sMaterialSid]->m_byWeight;		// 公霸函拳甫 眉农茄促.

		arMaterial.Add((BYTE)sSourceSlot);
		arMaterial.Add((BYTE)sMaterialSlot[0]);
		if(iCount==2)
			arMaterial.Add((BYTE)sMaterialSlot[1]);
	}

	FlushItemLog( TRUE );

	if( m_dwDN <= EBODY_UPGRADE_COST ) m_dwDN = 0;
	else m_dwDN = m_dwDN - EBODY_UPGRADE_COST;
	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();							// 捣函版 沥焊甫 焊辰促.

	TempBuf.Add(UPGRADE_ITEM_RESULT);
	index = arMaterial.GetSize();

	if(iSuccess == 1) TempBuf.Add((BYTE)0x01);	// EBody Upgrade 己傍
	else TempBuf.Add((BYTE)0x00);				// EBody Upgrade 角菩

	TempBuf.Add((BYTE)index);

	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		bySlot = (BYTE)arMaterial[i];
	
		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();							// 雀汗加档 眉农...

	Send(TempBuf, TempBuf.GetLength());

	arMaterial.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////////////
//	犬牢且 荐 乐绰 EBody 府胶飘甫 父电促.
//
void USER::EBodyIdentifyOpen(int iStoreID)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg( IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);
		return;
	}

	int i;
	short sStoreID = iStoreID;

	int iStart = EQUIP_ITEM_NUM;
	int iEnd = TOTAL_INVEN_MAX;

	CStore* pStore = GetStore(sStoreID);
	if(pStore == NULL) return;

	CBufferEx CostBuf;
	BYTE tSlot = 0;
	short sCount = 0;
	for(i = iStart; i < iEnd; i++)
	{
		if(m_UserItem[i].sSid == 908)	// 固犬牢 惑怕狼 EBody 狼 Sid
		{
			tSlot = (BYTE)i;
			sCount++;
			CostBuf.Add(tSlot);
		}
	}

	CBufferEx TempBuf;
	TempBuf.Add(EBODY_IDENTIFY_OPEN);
	TempBuf.Add(sStoreID);
	TempBuf.Add(sCount);
	if(sCount > 0) TempBuf.AddData(CostBuf, CostBuf.GetLength());

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	鉴定雪魂
//
void USER::EBodyIdentifyReq(TCHAR* pBuf)
{
	int index = 0, i, j;
	int iRandom = 0;
	DWORD dwIdentifyCost = 0;
	DWORD dwNeedXP = 0;
	CByteArray	arSlot;
	ItemList MyItem[TOTAL_INVEN_MAX - EQUIP_ITEM_NUM];

	short sSid = -1, sEBodySid = -1;
	BYTE tSlot = 0;

	short sStoreID = GetShort(pBuf, index);
	if(sStoreID < 0) return;
	
	if(sStoreID == 26)//3排合成4排 
	{
		Add4PaiReq(pBuf);
		return;
	}


	// 捞亥飘 困摹甫 八荤 -----------------------------------------//
	CPoint pt = ConvertToClient(m_curx, m_cury);
    if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, REPAIR_BLOCK)) return;

	short sCount = GetShort(pBuf, index);

	if(sCount <= 0 || sCount >= TOTAL_INVEN_MAX - EQUIP_ITEM_NUM) return;

	for(i = 0; i < sCount; i++)
	{
		tSlot = GetByte(pBuf, index);
		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) return;	// 牢亥俊辑父 犬牢 啊瓷
		if(m_UserItem[tSlot].sCount != 1) return;						// 般磨 荐 绝绰 酒捞袍捞促.

		sSid = m_UserItem[tSlot].sSid;
		if (sSid != 908)
		{
			return ;
		}
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return;

		dwIdentifyCost += EBODY_IDENTIFY_COST;
		dwNeedXP += EBODY_IDENTIFY_XP;

		if(dwIdentifyCost > m_dwDN)
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_DINA1, SYSTEM_ERROR, TO_ME);
			return;
		}
		if(dwNeedXP > m_dwXP)
		{
			SendSystemMsg( IDS_XP_ERROR, SYSTEM_ERROR, TO_ME);
			return;
		}

		arSlot.Add((BYTE)tSlot);
		MyItem[i] = m_UserItem[tSlot];
	}

	for(i = 0; i < arSlot.GetSize(); i++)
	{
		tSlot = arSlot[i];
		sSid = m_UserItem[tSlot].sSid;

		iRandom = myrand(0, g_arEBodyIdentifyTable.GetSize() -1);
		sEBodySid = g_arEBodyIdentifyTable[iRandom]->m_sSid;
		if(sEBodySid < 0 || sEBodySid > g_arItemTable.GetSize()) 
		{
			for(j = 0; j < arSlot.GetSize(); j++)
			{
				m_UserItem[arSlot[j]] = MyItem[j];
				return;
			}
		}

		m_UserItem[tSlot].sLevel	= g_arItemTable[sEBodySid]->m_byRLevel;
		m_UserItem[tSlot].sSid		= g_arItemTable[sEBodySid]->m_sSid;
		m_UserItem[tSlot].sDuration = g_arItemTable[sEBodySid]->m_sDuration;
		m_UserItem[tSlot].sBullNum	= g_arItemTable[sEBodySid]->m_sBullNum;
		m_UserItem[tSlot].sCount	= 1;
		for(j =0; j < MAGIC_NUM; j++) 
		{
			if(j == 0)	m_UserItem[tSlot].tMagic[j] = g_arItemTable[sEBodySid]->m_bySpecial;
			else		m_UserItem[tSlot].tMagic[j] = 0;
		}
		m_UserItem[tSlot].tIQ = MAGIC_ITEM; 
	}

	//-----------------[ Send Packet ]---------------------------------------//
	if(m_dwDN<=dwIdentifyCost)
		m_dwDN = 0;
	else
		m_dwDN = m_dwDN - dwIdentifyCost;
	

	m_dwXP = m_dwXP - dwNeedXP;
	if(m_dwXP < 0) m_dwXP = 0;

	UpdateUserItemDN();							// 酒捞袍捞聪瘪 官肺 利侩茄促.
	SendMoneyChanged();

	SendXP();

	CBufferEx TempBuf;

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add((BYTE)0x01);				// 己傍
	TempBuf.Add((BYTE)0x00);				// 老馆 酒捞袍
	TempBuf.Add((BYTE)arSlot.GetSize());	// 函拳等 浇吩狼 荐

	for(i = 0;  i < arSlot.GetSize(); i++)
	{
		tSlot = arSlot[i];
		TempBuf.Add((BYTE)tSlot);
		TempBuf.Add(m_UserItem[tSlot].sLevel);
		TempBuf.Add(m_UserItem[tSlot].sSid);
		TempBuf.Add(m_UserItem[tSlot].sDuration);
		TempBuf.Add(m_UserItem[tSlot].sBullNum);
		TempBuf.Add(m_UserItem[tSlot].sCount);
		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[tSlot].tMagic[j]);
		TempBuf.Add(m_UserItem[tSlot].tIQ); 
	}

	Send(TempBuf, TempBuf.GetLength());

	arSlot.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////////////
//	Finito - Sends server date and tiem to  client for screen capture.
//
void USER::ScreenCaptureReq()
{
	CBufferEx TempBuf;

    SYSTEMTIME st;
    GetSystemTime(&st);

	TempBuf.Add(SCREEN_CAPTURE_REQ);
	TempBuf.Add(st.wYear);
	TempBuf.Add(st.wMonth);
	TempBuf.Add(st.wDay);
	TempBuf.Add(st.wHour);
	TempBuf.Add(st.wMinute);
	TempBuf.Add(st.wSecond);

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	Finito - Sends Expression data to client
//
void USER::ExpressionReq(TCHAR* pBuf)
{
	int index = 0;
	short sExpression;

	sExpression = GetShort(pBuf, index);

	CBufferEx TempBuf;

	TempBuf.Add(EXPRESSION_REQ);
	TempBuf.Add((BYTE)0);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(sExpression);

	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	Finito - Opens expression dialog
//
void USER::ExpressionOpen()
{
	CBufferEx TempBuf;

	TempBuf.Add(EXPRESSION_OPEN);
	TempBuf.Add((short)1);
	TempBuf.Add((short)77);

	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////
//	Cures HP, PP, SP and restores energy / bullets for level 10 and below
//
void USER::SendHelperCure()
{
	BYTE result = SUCCESS;

	CBufferEx TempBuf;

	// 捞亥飘 困摹甫 八荤 ----------------------------------------//
	CPoint pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, CURE_BLOCK)) return;

	if(m_sHP >= m_sMagicMaxHP && m_sPP >= m_sMagicMaxPP && m_sSP >= m_sMagicMaxSP)
	{
		if(m_UserItem[RIGHT_HAND].sBullNum >= 250 && m_byClass == EDGED || m_byClass == FIREARMS && m_UserItem[RIGHT_HAND].sSid != -1)
		{		
			SendSystemMsg( IDS_HELPER_NOT_NEED_CURE, SYSTEM_NORMAL, TO_ME);
			result = FAIL;
			goto go_result;
		}
		else if(m_UserItem[RIGHT_HAND].sSid == -1)
		{	
			SendSystemMsg( IDS_HELPER_NOT_NEED_CURE, SYSTEM_NORMAL, TO_ME);
			result = FAIL;
			goto go_result;
		}
		else if(m_UserItem[RIGHT_HAND].sSid != -1 &&  m_byClass == BRAWL || m_byClass == STAFF)
		{
			SendSystemMsg( IDS_HELPER_NOT_NEED_CURE, SYSTEM_NORMAL, TO_ME);
			result = FAIL;
			goto go_result;
		}
	}

	m_sHP = m_sMagicMaxHP;
	m_sPP = m_sMagicMaxPP;
	m_sSP = m_sMagicMaxSP;


	if ((m_byClass == EDGED) || (m_byClass == FIREARMS))
	{
		if(m_UserItem[RIGHT_HAND].sSid != -1)
		{
			m_UserItem[RIGHT_HAND].sBullNum = 250; 
		}
	}

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

	Send(TempBuf, TempBuf.GetLength());
	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// 滚叼吝捞搁 促弗 滚叼盔俊霸 朝赴促.
	
	if ((m_byClass == EDGED) || (m_byClass == FIREARMS))
	{
		if (m_UserItem[RIGHT_HAND].sSid != -1)
		{
			SendBullNum(RIGHT_HAND);
		}
	}
	if (m_byClass == FIREARMS)
	{
		SendSystemMsg( IDS_HELPER_CURE_COMPLETED, SYSTEM_NORMAL, TO_ME);
		if (m_UserItem[RIGHT_HAND].sSid == -1)
		{
			SendSystemMsg( IDS_HELPER_CURE_COMPLETED2, SYSTEM_NORMAL, TO_ME);
		}
		else
		{
			TCHAR strHealMsg[128];
			::ZeroMemory(strHealMsg, sizeof(strHealMsg));
			wsprintf(strHealMsg, _ID(IDS_HELPER_CURE_COMPLETED2_CLASS), "Rifle");
			SendSystemMsg( strHealMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
	else if (m_byClass == EDGED)
	{
		SendSystemMsg( IDS_HELPER_CURE_COMPLETED, SYSTEM_NORMAL, TO_ME);
		if (m_UserItem[RIGHT_HAND].sSid == -1)
		{
			SendSystemMsg( IDS_HELPER_CURE_COMPLETED2, SYSTEM_NORMAL, TO_ME);
		}
		else
		{
			TCHAR strHealMsg[128];
			::ZeroMemory(strHealMsg, sizeof(strHealMsg));
			wsprintf(strHealMsg, _ID(IDS_HELPER_CURE_COMPLETED2_CLASS), "Sabre");
			SendSystemMsg( strHealMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
	else
	{
		SendSystemMsg( IDS_HELPER_CURE_COMPLETED, SYSTEM_NORMAL, TO_ME);
		SendSystemMsg( IDS_HELPER_CURE_COMPLETED2, SYSTEM_NORMAL, TO_ME);
	}
}

void USER::TogglePKButton(TCHAR* pBuf)  //机甲的ctrl+z切换功能
{
	CBufferEx TempBuf;

	int index=0;
//	short Slot = GetShort(pBuf, index);
	BYTE Slot = GetByte(pBuf, index);
	



	
    if(pBuf[0] == 3)
	{
		if(pBuf[1] == 1)
		{
			JJSpeed();
		} 
		if(pBuf[1] == 2){
			m_iSkin=0 , m_iHair=0;
			CheckMagicItemMove();		
            SendMyInfo( TO_INSIGHT, INFO_MODIFY );	
			SendEventMsg("您的状态已恢复!");

		}
			if(m_UserItem[39].sSid == 1184 && m_iSkin !=0){

			if(m_dwDN > 200000)
			{											
				if(pBuf[1] == 3){				
					m_UserItem[39].sCount = 1;					
					SendCharData();
					SendMyInfo(TO_INSIGHT, INFO_MODIFY);
					CheckMagicItemMove();					
					RobDN(200000);
				}else if(pBuf[1] == 4){				
					m_UserItem[39].sCount = 2;	
					SendMyInfo(TO_INSIGHT, INFO_MODIFY);
					SendCharData();
					CheckMagicItemMove();
					RobDN(200000);
				}
			}
		}		
		return;
	}/*else if(pBuf[0] == 17){ //shop点开元宝商店
		BOOL op = OpenOnShop();
		return;
	}*/
	
	if(pBuf[0] == 0x05)
	{
		TempBuf.Add((BYTE)0x3E);
		TempBuf.Add((BYTE)0x05);
		
		if(pBuf[1] == 0x00){
			TempBuf.Add((BYTE)0x00);
		    m_isUser = false;
			m_bPkKeyDown = FALSE;

		}
		if(pBuf[1] == 0x01)
		{
			m_bPkKeyDown = TRUE;

			if (!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
			{
				SendSystemMsg( "解除密码保护后才可以打开PK键", SYSTEM_ERROR, TO_ME);
				return;
			}
		//	 if(m_dwBHTime !=0 )
		//	 {
		//		 SendEventMsg("请先到天医院找NPC解除闭关后在操作!");
          //        return;
		//	}
			TempBuf.Add((BYTE)0x01);
			m_isUser = true;
        }
		Send(TempBuf, TempBuf.GetLength());
	    
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(pBuf[0] == 0x05)
	{
		TempBuf.Add((BYTE)0x3E);
		TempBuf.Add((BYTE)0x05);
		
		if(pBuf[1] == 0x00){
			TempBuf.Add((BYTE)0x00);
		    m_isUser = false;
			m_bPkKeyDown = FALSE;

		}
		if(pBuf[1] == 0x01)
		{
			m_bPkKeyDown = TRUE;

			if (m_PKStart== FALSE && m_curz == 67)
			{
			//	SendSystemMsg( "20:05分才能开启PK按钮,比赛正式开启!", SYSTEM_ERROR, TO_ME);
				return;
			}
			TempBuf.Add((BYTE)0x01);
			m_isUser = true;
        }
		Send(TempBuf, TempBuf.GetLength());
	    
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//查看装备
	if(pBuf[0] == 0x4f)
	{
		int uid = *(int*)&pBuf[1];
		shuxXING(uid);//查询状态
		ViewUserInfo(uid);//查看装备
		
	}
//	BYTE bCmd = GetByte(pBuf,index);
	switch(Slot)
	{
		case 0x0d://140级技能
		{
			if(m_sSkillPoint_ <= 0 || m_sSkillPoint_ > 70) 
				return ;
			BYTE bCmd1 = GetByte(pBuf,index);
			BYTE bCmd2 = GetByte(pBuf,index);
			if(bCmd1 == 1&&m_sLevel<150)
				return ;
			if(bCmd1 == 0 && m_sLevel<140)
				return ;
			m_sSkillPoint_ --;
			m_UserSkill140[bCmd1].sSid = bCmd2;
			m_UserSkill140[bCmd1].tLevel = 1;
			CheckMagicItemMove();
			UpdateUserData();

			CString strMsg;
			if(bCmd1 ==0)
				strMsg.Format("请选择您的140级超能");
			else
				strMsg.Format("请选择您的150级超能");

			CBufferEx TempBuf;
			TempBuf.Add(SYSTEM_MSG);
			TempBuf.Add((BYTE)0x01);
			TempBuf.Add(strMsg.GetBuffer(0),strMsg.GetLength());
			Send(TempBuf, TempBuf.GetLength());
			break;
		}
		case 0x25://喂baby 吃肉肉
		{//0x3e 0x25 0x95 0xa7 0x16 0x00 
			short sReceive = GetShort(pBuf,index);
			short sSolt = GetShort(pBuf,index);
			GiveBabyItem(sSolt);
			break;
		}
	}
}

BOOL USER::GivePSI(BYTE tType, int iTime)  /////给VIP时间
{
	CString sysMsg;
	CBufferEx TempBuf;
	switch (tType)
	{
	case PSIONIC_SHIELD:
		{
			SetShield(iTime);
			return TRUE;
		}
		break;
	case 30:
		{
			SetBigShield(iTime);
			return TRUE;
		}
		break;
	case 100://兴奋效果
		 m_dwHiExpTime = iTime*1000;
		m_dwLastHiExpTime = GetTickCount();
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		AddAbnormalInfo(ABNORMAL_HIEXP);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);	
		Send(TempBuf, TempBuf.GetLength());
		return TRUE;
		break;

		
	case 101://幸运效果
		m_dwMagicFindTime = iTime*1000;
		//-----------------------------------------------------------------------------------------
		m_dwLastMagicFindTime = GetTickCount();
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
		AddAbnormalInfo(ABNORMAL_MAGICFIND);
		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		Send(TempBuf, TempBuf.GetLength());
		return TRUE;
		break;

		case 102://变身效果
		m_sDramageUp = true;					// 打开伤害提高
		m_dwBFindTime = iTime*1000;
        m_dwLastBFindTime = GetTickCount();	
		AddStateInfo(STATE_2);//显示图标
        SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// 发送自己信息
	    SendUserStatusSkill();					// 发送用户属性数据
        CheckMagicItemMove();
		return TRUE;
		break;

		case 103://幻灵
		 m_dwHtExpTime = iTime*1000;
		m_dwLastHtExpTime = GetTickCount();	
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
        TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);	
		Send(TempBuf, TempBuf.GetLength());
		CheckMagicItemMove();
		return TRUE;
		break;

		case 104://幻晶
		 m_dwMagicFtTime = iTime*1000;
		m_dwLastMagicFtTime = GetTickCount();	
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
	    TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);	
		Send(TempBuf, TempBuf.GetLength());
		CheckMagicItemMove();
		return TRUE;

		case 105://天使双倍
		sysMsg.Format("『%s』领取天使双倍，与兴奋效果，幸运效果叠加，享受4倍经验与爆率",this->m_strUserID );
		 SendSystemMsg(sysMsg.GetBuffer(sysMsg.GetLength()),SYSTEM_ANNOUNCE,TO_ALL);
		m_dwBHTime = iTime*1000;
		m_dwLastBHTime = GetTickCount();	  
		//AddStateInfo(STATE_7);//显示图标
		//AddAbnormalInfo(ABNORMAL_FASTRUN);
		TempBuf.Add(SET_USER_STATE);
		TempBuf.Add(m_uid + USER_BAND);
	    TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);	
		Send(TempBuf, TempBuf.GetLength());
		CheckMagicItemMove();
		return TRUE;
		break;
	default:
		break;
	}

	return FALSE;
}
/////////////////////////脚本还原变身
BOOL USER::ChangeSkin()
{
	if(m_iSkin != 0 && m_dwBFindTime !=0 )
	{
		HuanYuanBianShen();	
	    return TRUE;
  }
	return FALSE;
}
BOOL USER::ChangeBh()
{
	if( m_dwBHTime !=0 )
	{
	//	HuanYuanBH();	
	  //  return TRUE;
  }
	return FALSE;
}
///////////////////////////////////////////
void USER::PersonalShopReq(TCHAR* pBuf)
{
	int uid = 0, index = 0, i = 0, nNameLength = 0;
	BYTE byType;
	ItemList tempItem;
	ShopItem* pNewItem;
	
	if(m_bNowTrading == TRUE || m_bNoItemMove == TRUE) return;

	byType = GetByte(pBuf, index);

	switch(byType)
	{
	case 0x00:
		break;
	case 0x01:
		{
			USER* pUser = NULL;
									
			if(m_iPShopViewuid != -1)
			{
				pUser = GetUser(m_iPShopViewuid);
				if(pUser==NULL) return ;
				if(pUser != NULL || pUser->m_state == STATE_GAMESTARTED)
				{
					pUser->m_bViewingAShop = FALSE;
				}	
			}
			m_bPShopView = 0;
			m_iPShopViewuid = -1;
			m_bPShopOpen = FALSE; // Set FALSE because User now has no personal shop

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

			::ZeroMemory(m_PersonalShopName, sizeof(m_PersonalShopName));

			CBufferEx TempBuf;
			TempBuf.Add((BYTE)0xEF);
			TempBuf.Add((BYTE)0x07); // Shop Closed
			Send(TempBuf, TempBuf.GetLength());
			PersonalShopClose();
		}
		break;
	case 0x02:
		{
			
			//
			/////////////////////交易密码
			_tcscpy_s(m_strTradeUser, _T(""));//bug  交易bug  test
			m_bNowTrading = m_bTradeWaiting = FALSE;
			m_bExchangeOk = m_bTradeItem	= FALSE;// 
			m_iTradeUid = -1;
			m_TradeMoney = 0;				// 

			for(i = 0; i < TOTAL_ITEM_NUM; i++) // 
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

			///////////////////////////////NPC附近不允许开设商店/////////////////////////////////
			int dir[9][2];
			int ix, iy;
			int nTarget = 0;
			CNpc* pNpc = NULL;
			MAP* pMap = g_zone[m_ZoneIndex];
			if(!pMap) return;
			dir[0][0]  =  0;	dir[0][1] =  0;
			dir[1][0]  = -1;	dir[1][1] =  0; 
			dir[2][0]  = -1;	dir[2][1] =  1;	
			dir[3][0]  =  0;	dir[3][1] =  1;
			dir[4][0]  =  1;	dir[4][1] =  1;
			dir[5][0]  =  1;	dir[5][1] =  0;
			dir[6][0]  =  1;	dir[6][1] = -1;
			dir[7][0]  =  0;	dir[7][1] = -1;
			dir[8][0]  = -1;	dir[8][1] = -1; 
			for(i = 1; i < 9; i++)
			{
				ix = m_curx + dir[i][0];
				iy = m_cury + dir[i][1];

				if(ix < 0) ix = 0;
				if(iy < 0) iy = 0;
				if(ix >= pMap->m_sizeMap.cx) ix = pMap->m_sizeMap.cx - 1;
				if(iy >= pMap->m_sizeMap.cy) iy = pMap->m_sizeMap.cy - 1;

				nTarget = pMap->m_pMap[ix][iy].m_lUser;
				if(nTarget >= NPC_BAND)
				{
					pNpc = GetNpc(nTarget - NPC_BAND);
					if(pNpc->m_byType == 1)
					{
						SendSystemMsg("在NPC人物附近无法开启商店.",SYSTEM_ERROR,TO_ME);
						m_bPShopOpen = FALSE;
						return;
					}
				}
			}
			//////////////////////////////
			m_bPShopOpen = TRUE; // Set TRUE because User now has a personal shop open
			if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1)
			{
				SendSystemMsg("物品保护尚未解除,请按HOME在内挂解除",SYSTEM_ERROR,TO_ME);
				m_bPShopOpen = FALSE;
				return;
			}      ///neo版本
		    if(!IsCity())
			{
				SendSystemMsg( "无法在该地区开启商店", SYSTEM_NORMAL, TO_ME);
				m_bPShopOpen = FALSE;
				return;
			}
			
			////////////////////////////////
			if(m_bNowTrading == TRUE)
			{
				SendSystemMsg( "在交易状态无法开启商店", SYSTEM_NORMAL, TO_ME);
				m_bPShopOpen = FALSE;
				return;
			}
			m_bPShopOpen = TRUE; // Set TRUE because User now has a personal shop open
			::ZeroMemory(m_PersonalShopName, sizeof(m_PersonalShopName));
			nNameLength = GetVarString( sizeof( m_PersonalShopName ), m_PersonalShopName, pBuf, index);

			//////////////////////////////////////////////////////////////////////
			/*if(m_dwNoChatTime == 1) //用元宝替换这段
			{
				int solt = GetEmptySlot( INVENTORY_SLOT );

				if(solt == -1)
				{
					SendEventMsg("您的包裹已满,请清空包裹栏");
					m_bPShopOpen = FALSE;
					return;
				}
				CString tmp = _T("");
				tmp.Format("[元宝]%s",m_PersonalShopName);
				strcpy(m_PersonalShopName,tmp);
			}*/
				if(m_dwNoChatTime == 1)
			{
				int solt = GetEmptySlot( INVENTORY_SLOT );

				if(solt == -1)
				{
					SendEventMsg("您的包裹已满,请清空包裹栏");
					m_bPShopOpen = FALSE;
					return;
				}
				CString tmp = _T("");
				tmp.Format("[标志]%s",m_PersonalShopName);
				strcpy(m_PersonalShopName,tmp);
			}

			else if (m_dwNoChatTime == 0)
			{
				int solt = GetEmptySlot( INVENTORY_SLOT );

				if(solt == -1)
				{
					SendEventMsg("您的包裹已满,请清空包裹栏");
					m_bPShopOpen = FALSE;
					return;
				}
				CString tmp = _T("");
				tmp.Format("[决战币]%s",m_PersonalShopName);
				strcpy(m_PersonalShopName,tmp);
			}
			/////////////////////////////////////////////////////////////////////////
			if(nNameLength == 0 || nNameLength > SHOP_NAME_LENGTH)	
			{
				SendSystemMsg( IDS_USER_INVALID_SHOP_NAME, SYSTEM_NORMAL, TO_ME);
				m_bPShopOpen = FALSE; // Set FALSE because User now has no personal shop
				return;
			}
			short ItemAmount;
			ItemAmount = GetShort(pBuf, index);
			if (ItemAmount > TOTAL_PERSONALSHOP_ITEM_NUM)
			{
				SendSystemMsg( IDS_USER_INVALID_SHOP_ITEMAMOUNT, SYSTEM_NORMAL, TO_ME);
				m_bPShopOpen = FALSE; // Set FALSE because User now has no personal shop
				return;
			}
		//	  int dwItemPriceTotal = 0;


			// Get Item data into ShopItem Array
			for(i=0;i< ItemAmount;i++)
			{
				short sItemSlot;
				short sItemCount;
				DWORD dwItemPrice;
				sItemSlot = GetShort(pBuf, index);
				if(sItemSlot<10||sItemSlot>34)//修复开商店卖身上物品出错BUG
				{
					SendEventMsg( "装备中的物品无法贩卖!");
					m_bPShopOpen = FALSE; 
					return ;
				}
				sItemCount = GetShort(pBuf, index);
				dwItemPrice = GetDWORD(pBuf, index);

				if (m_UserItem[sItemSlot].sSid != -1)
				{
					if (sItemCount > m_UserItem[sItemSlot].sCount) 
					{
						SendSystemMsg( IDS_USER_INVALID_SHOP_ITEMCOUNT, SYSTEM_NORMAL, TO_ME);
						m_bPShopOpen = FALSE; // Set FALSE because User now has no personal shop
						return;
					}
					///////////////////////////////////////////////////
					if(m_UserItem[sItemSlot].sDuration == 0)
					{
						for(i = 0; i < m_arShopItem.GetSize(); i++)
						{
							if (m_UserItem[sItemSlot].sSid == m_arShopItem[i]->ShopList.sSid)
							{
								SendSystemMsg( "请勿重复添加消耗品", SYSTEM_ERROR, TO_ME);
								m_bPShopOpen = FALSE;
								SoftClose();
								return;
							}
						}
					}

					////////////////////////////////////////////////////
			//		INT64 price = dwItemPrice;
			//		INT64 tempNum = price * m_UserItem[sItemSlot].sCount;
			//		if (tempNum + dwItemPriceTotal > 2100000000)
			//		{
			//			SendSystemMsg("物品总价不能超过21E", SYSTEM_NORMAL, TO_ME);
			//			m_bPShopOpen = FALSE; //"没有开启个人商店.因为商店物品数量与玩家的物品数量不符合."
			//			return;
			//		}
			//		dwItemPriceTotal += tempNum;

					pNewItem = new ShopItem;
					pNewItem->sSlot = sItemSlot;
					pNewItem->dwItemPrice = dwItemPrice;
					pNewItem->ShopList.sLevel = m_UserItem[sItemSlot].sLevel;
					pNewItem->ShopList.sSid = m_UserItem[sItemSlot].sSid;
					pNewItem->ShopList.sDuration = m_UserItem[sItemSlot].sDuration;
					pNewItem->ShopList.sBullNum = m_UserItem[sItemSlot].sBullNum;
					pNewItem->ShopList.sCount = m_UserItem[sItemSlot].sCount;
					for(int j =0; j < MAGIC_NUM; j++)
						pNewItem->ShopList.tMagic[j] = m_UserItem[sItemSlot].tMagic[j];
					pNewItem->ShopList.tIQ = m_UserItem[sItemSlot].tIQ;
					pNewItem->ShopList.iItemSerial = m_UserItem[sItemSlot].iItemSerial;

					EnterCriticalSection( &m_CS_ShopItem );
					m_arShopItem.Add(pNewItem);
					LeaveCriticalSection( &m_CS_ShopItem );
				}
				else
				{
					m_bPShopOpen = FALSE; // Set FALSE because User now has no personal shop
					return;
				}
			}
			if(m_bPShopOpen == TRUE)
			{
				CBufferEx TempBuf;
				TempBuf.Add((BYTE)0xEF);
				TempBuf.Add((BYTE)0x08);
				Send(TempBuf, TempBuf.GetLength());
				PersonalShopOpen();	
				 }
			break;
		}
	case 0x03:
		if(m_bPShopOpen == FALSE && m_bViewingAShop == FALSE)
		{
			uid = GetInt(pBuf, index);	
			ViewPersonalShop(0x03, uid);
		}
		break;
	case 0x04:
		PersonalShopBuy(pBuf + index);
		break;
	case 0x05:
		{
			USER* pUser = NULL;
			uid = GetInt(pBuf , index);

			if(uid < 0 || uid >= INVALID_BAND) return;
			pUser = GetUser(uid - USER_BAND);
			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
			
			pUser->m_bPShopView = 0;
			pUser->m_iPShopViewuid = -1;	
			m_bViewingAShop = FALSE;
		}
		break;
	case 0x06:
		ViewPersonalShop(0x0B, (m_uid+USER_BAND));
		break;
	default:
		break;
	}
}



void USER::PersonalShopOpen()
{       /////neo版本
	/*char szDest[20];
	ZeroMemory(szDest,sizeof(szDest));
	strcpy(szDest,m_PersonalShopName);
	if(m_dwNoChatTime == 0)
	{
		char jzb[20] = "[决战币]";
		sprintf(m_PersonalShopName,"%s%s",jzb,szDest);
	}
	else if(m_dwNoChatTime == 1)
	{
		char yb[20] = "[元宝]";
		sprintf(m_PersonalShopName,"%s%s",yb,szDest);
	} */
    CBufferEx TempBuf;

	TempBuf.Add((BYTE)0xEF);
	TempBuf.Add((BYTE)0x09);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_PersonalShopName);
	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());
}

void USER::PersonalShopClose()
{			
	CBufferEx TempBuf;

	TempBuf.Add((BYTE)0xEF);
	TempBuf.Add((BYTE)0x0A);
	TempBuf.Add(m_uid + USER_BAND);
	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength());
}
/////////////////////////////////////////////////////////////////////
//PK竞赛
//
void USER::CheckPKTime() 
{
	SYSTEMTIME messageTime;
	GetLocalTime(&messageTime);

	DWORD dwCurrTick = GetTickCount();					

	COleDateTime CurrTime = COleDateTime(messageTime); //当前时间



	if( messageTime.wHour == 20 && messageTime.wMinute < 6) //8点05分关
	{
		if (m_bNowBuddy == TRUE || m_sLevel < 60)
		{
			//SendSystemMsg("参加PK大赛不能组队,请退出组队再进入！", SYSTEM_ERROR, TO_ME);
			SendEventMsg("参加PK大赛不能组队或者等级大于60级！");
			return;
		}
		else if(m_isUser)	
		{
			//SendSystemMsg("进入PK场前不能打开PK状态，请关闭后再进入！", SYSTEM_ERROR, TO_ME);
            SendEventMsg("进入PK场前不能打开PK状态，请关闭后再进入！");
			return;
		}
		else
		ZoneMoveReq( 67, 21, 45);
	}
	else
	{
		//SendSystemMsg("PK王赛场已关闭,晚上20:00-20:05开放进入!", SYSTEM_ERROR, TO_ME);
          SendEventMsg("PK王赛场已关闭,晚上20:00-20:05开放进入!");
	}
}
/////////////////////////////////////////////////////////
//PK杀人狂
//
void USER::CheckPKShaRen() 
{
	SYSTEMTIME messageTime;
	GetLocalTime(&messageTime);

	DWORD dwCurrTick = GetTickCount();					

	COleDateTime CurrTime = COleDateTime(messageTime); //当前时间



	if( messageTime.wHour == 20 && messageTime.wMinute>=20  && messageTime.wMinute < 50) //8点50分关
	{
		if (m_bNowBuddy == TRUE || m_sLevel < 60)
		{
			//SendSystemMsg("参加PK杀人狂大赛,请退出组队再进入！", SYSTEM_ERROR, TO_ME);
			SendEventMsg("参加PK杀人狂大赛,请退出组队再进入或者等级大于60级！");
			return;
		}
		else
		{
			int x; int y; int random;
			random = myrand( 1,10);
			switch(random)
			{
			case	1:
				x =14,y=36;
				break;
			case	2:
				x =4,y=48;
				break;
			case	3:
				x =10,y=62;
				break;
			case	4:
				x =34,y=58;
				break;
			case	5:
				x =46,y=42;
				break;
			case	6:
				x =34,y=28;
				break;
			case	7:
				x =26,y=40;
				break;
			case	8:
				x =19,y=51;
				break;
			case	9:
				x =13,y=57;
				break;
			case	10:
				x =29,y=51;
				break;
			}
			ZoneMoveReq( 66, x, y);
		}
			
		
	}
	else
	{
		//SendSystemMsg("杀人狂大赛已关闭,晚上20:20-20:50开放进入!", SYSTEM_ERROR, TO_ME);
		SendEventMsg("杀人狂大赛已关闭,晚上20:20-20:50开放进入!");
	}
}
void USER::ViewYuanBaoShop(int start)
{
	//CString strTemp;
	//strTemp.Format( "您的元宝数量为: %d ", m_dwShopPingDN);
	//SendSystemMsg( strTemp.GetBuffer(), SYSTEM_NORMAL, TO_ME);
	
	int k = 0;
	m_dwYuanBaoStart =start;
	CBufferEx TempBuf;
	CBufferEx TempBuf2;
	DWORD dwTemp = 0;
	int end = start + 7;
	if(end > g_arSxOnlineShopTable.GetSize())
		end = g_arSxOnlineShopTable.GetSize();
		for(int i = start; i < end ;i++)
		{
			if(g_arSxOnlineShopTable[i]->m_iSid==0)
			{
				continue;
			}
			TempBuf2.Add((DWORD)g_arSxOnlineShopTable[i]->m_prices);
			TempBuf2.Add((short)g_arSxOnlineShopTable[i]->m_sLevel);
			TempBuf2.Add((short)g_arSxOnlineShopTable[i]->m_iSid);
			TempBuf2.Add((short)g_arSxOnlineShopTable[i]->m_sDuration);
			TempBuf2.Add((short)g_arSxOnlineShopTable[i]->m_sBullNum);
			TempBuf2.Add((short)g_arSxOnlineShopTable[i]->m_iNum);
			TempBuf2.Add((short)g_arSxOnlineShopTable[i]->m_oSid);

			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx1);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx2);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx3);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx4);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx5);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_upgrade);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx6);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx7);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx8);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx9);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_sx10);
			TempBuf2.Add((BYTE)g_arSxOnlineShopTable[i]->m_tIQ);
			k++;
		}
	
	TempBuf.Add((BYTE)0xEF);
	TempBuf.Add((BYTE)0x03);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((BYTE)k);
	TempBuf.Add((BYTE)0x00);
	TempBuf.AddData(TempBuf2, TempBuf2.GetLength());
	Send(TempBuf, TempBuf.GetLength());
	return;
}
void USER::ViewPersonalShop(BYTE mode, int uid)
{
	CBufferEx TempBuf;
	CBufferEx TempBuf2;
	USER* pUser = NULL;
    int i, n;
	n = 0;
	
	if(uid < 0 || uid >= INVALID_BAND) return;

	pUser = GetUser(uid - USER_BAND);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	if(mode == 0x03)
	{
		if ( pUser->m_UserStatus == 0 && pUser->m_UserFlag == FALSE )
		{
			goto fail_result;
		}	

		if( strcmp(pUser->m_PersonalShopName, "") == 0) 
		{
			goto fail_result;
		}

		if(pUser->m_iPShopViewuid != -1)
		{
			USER* pUser2 = NULL;
			pUser2 = GetUser(pUser->m_iPShopViewuid);
			if(pUser2 == NULL || pUser2->m_state != STATE_GAMESTARTED)
			{
				pUser->m_bPShopView = FALSE;
				pUser->m_iPShopViewuid = -1;
				return;	//修复人不在线导致掉端
			}
			else if(pUser2->m_bViewingAShop == FALSE)
			{
				pUser->m_bPShopView = FALSE;
				pUser->m_iPShopViewuid = -1;
			}
		}

		if(pUser->m_bPShopView == TRUE)
		{
			SendSystemMsg( IDS_USER_INVALID_SHOP_BEINGVIEWED, SYSTEM_NORMAL, TO_ME);
			return;
		}

		if(InterlockedCompareExchange((LONG*)&pUser->m_bPShopView, (long)1, (long)0) == (long)0)
		{
			InterlockedExchange((LONG*)&pUser->m_iPShopViewuid, (LONG)m_uid); // Set uid so that can keep track of who is viewing shop
			m_bViewingAShop = TRUE;
		}

		if(pUser->m_bPShopView == FALSE)
		{
			goto fail_result;
		}
	}

	EnterCriticalSection( &pUser->m_CS_ShopItem );
	for(i = 0; i < pUser->m_arShopItem.GetSize(); i++)
	{
		if ( pUser->m_arShopItem[i] == NULL ) continue;

		TempBuf2.Add(pUser->m_arShopItem[i]->dwItemPrice);
		TempBuf2.Add(pUser->m_arShopItem[i]->ShopList.sLevel);
		TempBuf2.Add(pUser->m_arShopItem[i]->ShopList.sSid);
		TempBuf2.Add(pUser->m_arShopItem[i]->ShopList.sDuration);
		TempBuf2.Add(pUser->m_arShopItem[i]->ShopList.sBullNum);
		TempBuf2.Add(pUser->m_arShopItem[i]->ShopList.sCount);
		TempBuf2.Add(pUser->m_arShopItem[i]->sSlot);

		for(int j =0; j < MAGIC_NUM; j++)
			TempBuf2.Add(pUser->m_arShopItem[i]->ShopList.tMagic[j]);
		TempBuf2.Add(pUser->m_arShopItem[i]->ShopList.tIQ);

		n++;
	}
	LeaveCriticalSection(&pUser->m_CS_ShopItem);

	TempBuf.Add((BYTE)0xEF);
	TempBuf.Add((BYTE)mode);
	TempBuf.Add(uid);
	TempBuf.Add((BYTE)n);
	TempBuf.Add((BYTE)0x00);
	TempBuf.AddData(TempBuf2, TempBuf2.GetLength());
	Send(TempBuf, TempBuf.GetLength());
	return;

fail_result:
	pUser->m_bPShopView = 0;
	pUser->m_iPShopViewuid = -1;
	m_bViewingAShop = FALSE;
}

void USER::PersonalShopBuy(TCHAR* pBuf)  ///neo版本
{
	CString strMsg;

	CBufferEx TempBuf;
	CBufferEx TempBuf2;
	USER* pUser = NULL;
	BYTE result = FAIL;
	DWORD TotalCost = 0;
	ShopItem* ShopBackupItem[TOTAL_PERSONALSHOP_ITEM_NUM];
	short sShopAmount, sUserSlot, sShopCount, sShopSid = -1;
	ItemList		TempItem;
	ItemList		BuyerItem[TOTAL_ITEM_NUM], ShopItem[TOTAL_ITEM_NUM];
	ItemListArray	arItemListBuyer, arItemListShop;
	CWordArray		arEmptySlotBuyer, arSameSlotBuyer, arEmptySlotShop, arSameSlotShop;
	
	int BiaoZhiNum = 0;
	BiaoZhiNum = FindItem(724);//检测标志

    int i,k, j, n, index = 0, iWeight = 0, iSameSlot, iEmptySlot, iWeightOwner, ShopSize;
	n = 0;

	int uid = GetInt(pBuf, index);
	
	if(uid == m_uid + USER_BAND  ) //调用显示属性的元宝商店
	{
					
					sShopAmount = GetShort(pBuf, index);
					if(sShopAmount >TOTAL_PERSONALSHOP_ITEM_NUM)
						return ;
					for(k = 0; k < sShopAmount; k++)
					{
						
						sUserSlot = GetShort(pBuf, index);
						sShopCount = GetShort(pBuf, index);
					    DWORD ItemPrice;
						CWordArray		arEmptySlot, arSameSlot;
						int iSlot = GetEmptySlot( INVENTORY_SLOT );
			            
						if( iSlot < 0 ) 
						{
							SendSystemMsg( IDS_USER_SHOP_NO_IVEN_SLOT, SYSTEM_ERROR, TO_ME); //"没有足够的包裹空间购买这家个人商店的物品."
							return;
						}
                        if(m_UserItem[sUserSlot].sSid == -1 || g_arOnlineShopTable.GetSize() <= 0) return;//屏蔽商店发包掉端
						ReSetItemSlot( &m_UserItem[iSlot]);
						
						for(int i = m_dwYuanBaoStart; i < g_arSxOnlineShopTable.GetSize();i++)
						{
						  if(m_dwGuarDianTianShi > 0)//如果是VIP购物9折
			              {
							if(sUserSlot == g_arSxOnlineShopTable[i]->m_oSid)
							{
								TotalCost=0;
								///////////////////////////////
						if (sShopCount > g_arSxOnlineShopTable[i]->m_iNum || sShopCount <= 0)	//g_arOnlineShopTable
							             return;
                         ///////////////////////////////////
								ItemPrice =(DWORD)g_arSxOnlineShopTable[i]->m_prices;
								int iCost = (int)(g_arSxOnlineShopTable[i]->m_prices * 0.9);//VIP在原基础上X9折
								DWORD dwCast= (ItemPrice-iCost)*sShopCount;//优惠值
								TotalCost += (iCost * sShopCount);
								//if(TotalCost > m_dwShopPingDN )
								//if(TotalCost > m_dwDN )
								//{
								//	SendSystemMsg( IDS_SHOPDN_ERROR, SYSTEM_ERROR, TO_ME); //"你没有足够的 [ 元 宝 ] 购买此物品."
								//	return;
								//}
                            ////////////////////////////////////////////
								if((int)TotalCost > BiaoZhiNum)  
					           {
						          SendSystemMsg( "对不起,您的标志不足.", SYSTEM_ERROR, TO_ME);
						          return;
					           }
							//////////////////////////////////////////////////
								m_UserItem[iSlot].sLevel = g_arSxOnlineShopTable[i]->m_sLevel;
								m_UserItem[iSlot].sSid = g_arSxOnlineShopTable[i]->m_iSid;
								m_UserItem[iSlot].sCount = sShopCount;
								m_UserItem[iSlot].sDuration = g_arSxOnlineShopTable[i]->m_sDuration;
								m_UserItem[iSlot].sBullNum = g_arSxOnlineShopTable[i]->m_sBullNum;

								m_UserItem[iSlot].tMagic[0] = (BYTE)g_arSxOnlineShopTable[i]->m_sx1;
								m_UserItem[iSlot].tMagic[1] = (BYTE)g_arSxOnlineShopTable[i]->m_sx2;
								m_UserItem[iSlot].tMagic[2] = (BYTE)g_arSxOnlineShopTable[i]->m_sx3;
								m_UserItem[iSlot].tMagic[3] = (BYTE)g_arSxOnlineShopTable[i]->m_sx4;
								m_UserItem[iSlot].tMagic[4] = (BYTE)g_arSxOnlineShopTable[i]->m_sx5;
								m_UserItem[iSlot].tMagic[5] =(BYTE)g_arSxOnlineShopTable[i]->m_upgrade;
								m_UserItem[iSlot].tMagic[6] =(BYTE)g_arSxOnlineShopTable[i]->m_sx6; //增加机甲属性的显示
								m_UserItem[iSlot].tMagic[7] =(BYTE)g_arSxOnlineShopTable[i]->m_sx7;
								m_UserItem[iSlot].tMagic[8] =(BYTE)g_arSxOnlineShopTable[i]->m_sx8;
								m_UserItem[iSlot].tMagic[9] =(BYTE)g_arSxOnlineShopTable[i]->m_sx9;
								m_UserItem[iSlot].tMagic[10] =(BYTE)g_arSxOnlineShopTable[i]->m_sx10;
								m_UserItem[iSlot].tIQ = (BYTE)g_arSxOnlineShopTable[i]->m_tIQ;		//机甲为IQ：13
								m_UserItem[iSlot].iItemSerial = 0;	

								//m_dwShopPingDN -= TotalCost;
								//SendUserStatusSkill();
							//////////////////////////////
								//m_dwDN -= TotalCost;
								//UpdateUserItemDN();
								//SendMoneyChanged();
						/////////////////////////////
                                RobItem(724,TotalCost);
						        strMsg.Format("成功购买 %s ,扣除 %d 标志.",g_arSxOnlineShopTable[i]->m_iSname,TotalCost);
						        SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg,SYSTEM_ERROR,TO_ME);
						/////////////////////////////////////////////////////////
                                arEmptySlot.Add(iSlot); 
								UpdateInvenSlot(&arEmptySlot, &arSameSlot);
                                
								/*CString strMsg= "";
					            strMsg.Format("VIP购物9折,购[%s]扣%d元宝,节省%d元宝",g_arSxOnlineShopTable[i]->m_iSname,TotalCost,dwCast);
					            SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg,SYSTEM_ERROR,TO_ME);
				                CString strTemp;
					            strTemp.Format( "您的元宝剩余数量为: %d 个", m_dwShopPingDN);
					            SendSystemMsg( strTemp.GetBuffer(), SYSTEM_NORMAL, TO_ME);*/
								
								CString strMsg= "";
					            strMsg.Format("VIP购物9折,购[%s]扣%d标志,节省%d标志",g_arSxOnlineShopTable[i]->m_iSname,TotalCost,dwCast);
					            SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg,SYSTEM_ERROR,TO_ME);
				                CString strTemp;
					            strTemp.Format( "您的标志剩余数量为: %d 个", BiaoZhiNum);
					            SendSystemMsg( strTemp.GetBuffer(), SYSTEM_NORMAL, TO_ME);

							}
						  }else
							if(sUserSlot == g_arSxOnlineShopTable[i]->m_oSid)
							{
								TotalCost=0;
								
								ItemPrice =(DWORD)g_arSxOnlineShopTable[i]->m_prices;
								TotalCost += (ItemPrice * sShopCount);
								//if(TotalCost > m_dwShopPingDN )
								//if(TotalCost > m_dwDN )
                            
								//{
								//	SendSystemMsg( IDS_SHOPDN_ERROR, SYSTEM_ERROR, TO_ME); //"你没有足够的 [ 元 宝 ] 购买此物品."
								//	return;
								//}
                                
								 ////////////////////////////////////////////
								if((int)TotalCost > BiaoZhiNum)  
					           {
						          SendSystemMsg( "对不起,您的标志不足.", SYSTEM_ERROR, TO_ME);
						          return;
					           }
							//////////////////////////////////////////////////
								m_UserItem[iSlot].sLevel = g_arSxOnlineShopTable[i]->m_sLevel;
								m_UserItem[iSlot].sSid = g_arSxOnlineShopTable[i]->m_iSid;
								m_UserItem[iSlot].sCount = sShopCount;
								m_UserItem[iSlot].sDuration = g_arSxOnlineShopTable[i]->m_sDuration;
								m_UserItem[iSlot].sBullNum = g_arSxOnlineShopTable[i]->m_sBullNum;

								m_UserItem[iSlot].tMagic[0] = (BYTE)g_arSxOnlineShopTable[i]->m_sx1;
								m_UserItem[iSlot].tMagic[1] = (BYTE)g_arSxOnlineShopTable[i]->m_sx2;
								m_UserItem[iSlot].tMagic[2] = (BYTE)g_arSxOnlineShopTable[i]->m_sx3;
								m_UserItem[iSlot].tMagic[3] = (BYTE)g_arSxOnlineShopTable[i]->m_sx4;
								m_UserItem[iSlot].tMagic[4] = (BYTE)g_arSxOnlineShopTable[i]->m_sx5;
								m_UserItem[iSlot].tMagic[5] =(BYTE)g_arSxOnlineShopTable[i]->m_upgrade;
								m_UserItem[iSlot].tMagic[6] =(BYTE)g_arSxOnlineShopTable[i]->m_sx6; //增加机甲属性的显示
								m_UserItem[iSlot].tMagic[7] =(BYTE)g_arSxOnlineShopTable[i]->m_sx7;
								m_UserItem[iSlot].tMagic[8] =(BYTE)g_arSxOnlineShopTable[i]->m_sx8;
								m_UserItem[iSlot].tMagic[9] =(BYTE)g_arSxOnlineShopTable[i]->m_sx9;
								m_UserItem[iSlot].tMagic[10] =(BYTE)g_arSxOnlineShopTable[i]->m_sx10;
								m_UserItem[iSlot].tIQ = (BYTE)g_arSxOnlineShopTable[i]->m_tIQ;		//机甲为IQ：13
								m_UserItem[iSlot].iItemSerial = 0;	

								//m_dwShopPingDN -= TotalCost;
								//SendUserStatusSkill();
								/////////////////////////////////////////////
								//m_dwDN -= TotalCost;
								//UpdateUserItemDN();
								//SendMoneyChanged();

								/////////////////////////////
                                RobItem(724,TotalCost);
						        strMsg.Format("成功购买 %s ,扣除 %d 标志.",g_arSxOnlineShopTable[i]->m_iSname,TotalCost);
						        SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg,SYSTEM_ERROR,TO_ME);
						/////////////////////////////////////////////////////////
								arEmptySlot.Add(iSlot); 
								UpdateInvenSlot(&arEmptySlot, &arSameSlot);

								/*CString strMsg= "";
					            strMsg.Format("您购买 [%s] 减少%d元宝（不是VIP用户无任何折扣）",g_arSxOnlineShopTable[i]->m_iSname,TotalCost);
					            SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg,SYSTEM_ERROR,TO_ME);
					            CString strTemp;
					            strTemp.Format( "您的元宝剩余数量为: %d ", m_dwShopPingDN);
					            SendSystemMsg( strTemp.GetBuffer(), SYSTEM_NORMAL, TO_ME);	*/

								

							}

						}

					}

					return;

	}
    else				//个人商店
	{
	pUser = GetUser(uid - USER_BAND);

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	if ( pUser->m_UserStatus == 0 && pUser->m_UserFlag == FALSE ) return;

	if( strcmp(pUser->m_PersonalShopName, "") == 0) return; 
	
	for(i = 0; i < TOTAL_ITEM_NUM; i++)			// Backup Inven data
	{
		BuyerItem[i] = m_UserItem[i];
		ShopItem[i] = pUser->m_UserItem[i];
	}

	EnterCriticalSection( &pUser->m_CS_ShopItem );
	ShopSize = pUser->m_arShopItem.GetSize();
	if(ShopSize >TOTAL_PERSONALSHOP_ITEM_NUM) {
		LeaveCriticalSection( &pUser->m_CS_ShopItem );
		return ;
	}
	for(i = 0; i < ShopSize; i++)
	{
		ShopBackupItem[i] = pUser->m_arShopItem[i];
	}
	LeaveCriticalSection( &pUser->m_CS_ShopItem );
	
	sShopAmount = GetShort(pBuf, index);
	if(sShopAmount >TOTAL_PERSONALSHOP_ITEM_NUM)
		return ;

	for(k = 0; k < sShopAmount; k++)
	{
		// Source Item
		sUserSlot = GetShort(pBuf, index);
		sShopCount = GetShort(pBuf, index);

		if ( pUser->m_UserItem[sUserSlot].sSid == -1 ) goto fail_result;
		if(sUserSlot < EQUIP_ITEM_NUM || sUserSlot >= TOTAL_INVEN_MAX) goto fail_result;		
		if ( pUser->m_UserItem[sUserSlot].sCount < sShopCount ) goto fail_result;

		sShopSid = pUser->m_UserItem[sUserSlot].sSid;
		if( sShopSid < 0 || sShopSid >= g_arItemTable.GetSize() ) goto fail_result;
		if( sShopCount <= 0 ) goto fail_result;

		// Get Item Cost
		DWORD ItemPrice;
		EnterCriticalSection( &pUser->m_CS_ShopItem );
		for(int i = 0; i < pUser->m_arShopItem.GetSize(); i++)
		{
			if(pUser->m_arShopItem[i]->sSlot == sUserSlot)
			{
				ItemPrice = pUser->m_arShopItem[i]->dwItemPrice;
				break;
			}
		}
		LeaveCriticalSection(&pUser->m_CS_ShopItem);

		//Update Dina
		TotalCost += (ItemPrice * sShopCount);
		////////////////////////交易密码
		if(!m_MItemLock && o_yehuoini[0]->mimabaohu == 1 ){ SendSystemMsg( "解除[安全锁]后,才能购买这家个人商店的物品!.", SYSTEM_ERROR, TO_ME);goto fail_result;}
			
		if (pUser->m_dwNoChatTime == 0)
			{
				if(TotalCost > m_dwDN)
				{
					SendSystemMsg( IDS_USER_SHOP_NOTENOUGH_DINA, SYSTEM_ERROR, TO_ME);
					goto fail_result;//"没有足够的金钱购买这家个人商店的物品."
				}
			}
     
		/*else if(pUser->m_dwNoChatTime == 1)   //如果要用元宝，替换这段
			{
				if(TotalCost > m_dwShopPingDN)
				{
					SendSystemMsg("没有足够的元宝购买这家个人商店的物品", SYSTEM_ERROR, TO_ME);
					goto fail_result;
				}
			}*/
		else if(pUser->m_dwNoChatTime == 1)
			{
				if((int)TotalCost > BiaoZhiNum)
				{
					SendSystemMsg("没有足够的标志购买这家个人商店的物品.", SYSTEM_ERROR, TO_ME);
					goto fail_result;
				}
			}

		iWeight = 0;
		iWeight = g_arItemTable[sShopSid]->m_byWeight * sShopCount;
		if(m_iMaxWeight < m_iCurWeight + iWeight)
		{
			SendSystemMsg( IDS_USER_SHOP_NOTENOUGH_WEIGHT, SYSTEM_ERROR, TO_ME);
			goto fail_result;
		}

		ReSetItemSlot(&TempItem);
		TempItem = pUser->m_UserItem[sUserSlot];
		TempItem.sCount = sShopCount;

		if(TempItem.sSid < 0 || TempItem.sSid >= g_arItemTable.GetSize()) goto fail_result;
		///////////////////////////////////////////////
		CString strDate ="";
			SYSTEMTIME st;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d:%d [%s]购买[%s]商店的[%s], %d %d %d %d %d %d %d %d %d %d %d %d %d %d",st.wYear,st.wMonth,st.wDay ,st.wHour,
				st.wMinute,st.wSecond,m_strUserID,pUser->m_strUserID,g_arItemTable[TempItem.sSid]->m_strName,
				TempItem.sSid,sShopCount,TempItem.tMagic[5],TempItem.tIQ,TempItem.tMagic[0],TempItem.tMagic[1],TempItem.tMagic[2],
				TempItem.tMagic[3],TempItem.tMagic[4],TempItem.tMagic[6],TempItem.tMagic[7],TempItem.tMagic[8],TempItem.tMagic[9],TempItem.tMagic[10]);
			TCHAR m_Log[500];
			if(pUser->m_bSessionOnline && pUser->m_dwNoChatTime == 0 )
			{
				sprintf_s(m_Log,"%s 离线价格:%d 决战币,数量 %d个,位置:%d\r\n",strDate,ItemPrice,sShopCount,sUserSlot);
			}
			else if(pUser->m_bSessionOnline && pUser->m_dwNoChatTime == 1 )
			{
				sprintf_s(m_Log,"%s 离线价格:%d 元宝,数量 %d个,位置:%d\r\n",strDate,ItemPrice,sShopCount,sUserSlot);
			}
			
			else if( pUser->m_dwNoChatTime == 1)
			{
				sprintf_s(m_Log,"%s 价格:%d 元宝,数量 %d个,位置:%d\r\n",strDate,ItemPrice,sShopCount,sUserSlot);
			}
			
			else if( pUser->m_dwNoChatTime == 0)
			{
				sprintf_s(m_Log,"%s 价格:%d 决战币,数量 %d个,位置:%d\r\n",strDate,ItemPrice,sShopCount,sUserSlot);
			}
			WriteUserShopLog(m_Log);
		///////////////////////////////////////////////////
		iSameSlot = GetSameItem(TempItem, INVENTORY_SLOT);

		if(iSameSlot == -1)
		{
			iEmptySlot = GetEmptySlot(INVENTORY_SLOT);
			if(iEmptySlot == -1)		
			{
				SendSystemMsg( IDS_USER_SHOP_NO_IVEN_SLOT, SYSTEM_ERROR, TO_ME);
				goto fail_result;
			}

			m_UserItem[iEmptySlot] = TempItem;

			m_UserItem[iEmptySlot].sSid = TempItem.sSid;
		
			m_UserItem[iEmptySlot].sLevel  = TempItem.sLevel;
			m_UserItem[iEmptySlot].sBullNum  = TempItem.sBullNum;
			m_UserItem[iEmptySlot].sDuration  = TempItem.sDuration;
			m_UserItem[iEmptySlot].sCount  = TempItem.sCount;
			for(j =0; j < MAGIC_NUM; j++) m_UserItem[iEmptySlot].tMagic[j] = TempItem.tMagic[j];
			m_UserItem[iEmptySlot].tIQ = TempItem.tIQ; 
			m_UserItem[iEmptySlot].iItemSerial = TempItem.iItemSerial;
			iWeightOwner = (g_arItemTable[m_UserItem[iEmptySlot].sSid]->m_byWeight * sShopCount);
			arEmptySlotBuyer.Add((BYTE)iEmptySlot);
		}
		else								
		{
			m_UserItem[iSameSlot].sSid = TempItem.sSid;
			
			m_UserItem[iSameSlot].sLevel  = TempItem.sLevel;
			m_UserItem[iSameSlot].sBullNum  = TempItem.sBullNum;
			m_UserItem[iSameSlot].sDuration  = TempItem.sDuration;
			CheckMaxValue((short &)m_UserItem[iSameSlot].sCount,(short)TempItem.sCount);

			for(j =0; j < MAGIC_NUM; j++) m_UserItem[iSameSlot].tMagic[j] = TempItem.tMagic[j];
			m_UserItem[iSameSlot].tIQ = TempItem.tIQ; 
			m_UserItem[iSameSlot].iItemSerial = TempItem.iItemSerial;
			iWeightOwner = (g_arItemTable[m_UserItem[iSameSlot].sSid]->m_byWeight * sShopCount);
			arSameSlotBuyer.Add((BYTE)iSameSlot);
		}

		// Update Shop Owner weight
		
		//MakeItemLog( &m_UserItem[tSlot], ITEMLOG_EXCHANGE_GIVE, pUser->m_strUserID );

		pUser->ReSetItemSlot(&TempItem);
		TempItem = pUser->m_UserItem[sUserSlot];
		TempItem.sCount = sShopCount;

		if(TempItem.sSid < 0 || TempItem.sSid >= g_arItemTable.GetSize()) goto fail_result;
		iSameSlot = pUser->GetSameItem(TempItem, INVENTORY_SLOT);
		if(iSameSlot == -1)
		{
			//MakeItemLog( &m_UserItem[iEmptySlot], ITEMLOG_EXCHANGE_RECEIVE, pUser->m_strUserID );
			pUser->ReSetItemSlot( &pUser->m_UserItem[sUserSlot] );
			EnterCriticalSection( &pUser->m_CS_ShopItem );
			for(int l = 0; l < pUser->m_arShopItem.GetSize(); l++)
			{
				if(pUser->m_arShopItem[l]->sSlot == sUserSlot)
				{
					pUser->m_arShopItem.RemoveAt(l);
					break;
				}
			}
			LeaveCriticalSection(&pUser->m_CS_ShopItem);

			arEmptySlotShop.Add((BYTE)sUserSlot);

		}
		else								
		{
			if(pUser->m_UserItem[iSameSlot].sCount - TempItem.sCount == 0)
			{
				pUser->ReSetItemSlot( &pUser->m_UserItem[sUserSlot] );
			}
			/////////////////////////////////////////
		    else if(pUser->m_UserItem[iSameSlot].sCount < TempItem.sCount) //bug 
				{
					goto fail_result;
				}
             //////////////////////////////////////////////
			else
			{
				pUser->m_UserItem[iSameSlot].sCount -= TempItem.sCount;

				pUser->m_UserItem[iSameSlot].sSid = pUser->m_UserItem[iSameSlot].sSid;
				pUser->m_UserItem[iSameSlot].sLevel = pUser->m_UserItem[iSameSlot].sLevel;
				pUser->m_UserItem[iSameSlot].sBullNum = pUser->m_UserItem[iSameSlot].sBullNum;
				pUser->m_UserItem[iSameSlot].sDuration = pUser->m_UserItem[iSameSlot].sDuration;
				pUser->m_UserItem[iSameSlot].sCount = pUser->m_UserItem[iSameSlot].sCount;
			
				for(j =0; j < MAGIC_NUM; j++) pUser->m_UserItem[iSameSlot].tMagic[j] = pUser->m_UserItem[iSameSlot].tMagic[j];
				pUser->m_UserItem[iSameSlot].tIQ = pUser->m_UserItem[iSameSlot].tIQ; 
				pUser->m_UserItem[iSameSlot].iItemSerial = pUser->m_UserItem[iSameSlot].iItemSerial;
			}
			EnterCriticalSection( &pUser->m_CS_ShopItem );
			for(int l = 0; l < pUser->m_arShopItem.GetSize(); l++)
			{
				if(pUser->m_arShopItem[l]->sSlot == sUserSlot)
				{
					if(pUser->m_arShopItem[l]->ShopList.sCount - TempItem.sCount == 0)
					{
						pUser->m_arShopItem.RemoveAt(l);
					}
					/////////////////////////////////////////////////
					else if(pUser->m_arShopItem[l]->ShopList.sCount < TempItem.sCount)
						{
							LeaveCriticalSection(&pUser->m_CS_ShopItem);				//bug 
							goto fail_result;
						}
					//////////////////////////////////////////////////////////

					else
					{
						pUser->m_arShopItem[l]->ShopList.sCount -= TempItem.sCount;
						////////////////////////////////
						if(pUser->m_arShopItem[l]->ShopList.sCount < 0) pUser->m_arShopItem[l]->ShopList.sCount = 0;
						///////////////////////////////////////

					}
					break;
				}
			}
			LeaveCriticalSection(&pUser->m_CS_ShopItem);

			arSameSlotShop.Add((BYTE)iSameSlot);
		}
	}

	if (pUser->m_dwNoChatTime == 0)//个人金币商店
	{
		if(TotalCost>m_dwDN) return ;
		pUser->m_dwDN += TotalCost;//Update Buyer and Shop
		pUser->m_iCurWeight -= iWeightOwner;
		if(pUser->m_iCurWeight < 0) pUser->m_iCurWeight = 0;
		pUser->SendMoneyChanged();
		pUser->GetRecoverySpeed();
		pUser->UpdateInvenSlot(&arEmptySlotShop, &arSameSlotShop);
		m_dwDN -= TotalCost;
		if(m_dwDN<0) m_dwDN=0;
		m_iCurWeight += iWeight;
		SendMoneyChanged();
		GetRecoverySpeed();
		UpdateInvenSlot(&arEmptySlotBuyer, &arSameSlotBuyer);
		pUser->UpdateUserItemDN();
		UpdateUserItemDN();
		pUser->m_bPShopView = 0;
		pUser->m_iPShopViewuid = -1;
		m_bViewingAShop = FALSE;
	}
	/*else if(pUser->m_dwNoChatTime == 1)//个人元宝商店
	{
		if(TotalCost > m_dwShopPingDN) return;
		pUser->m_dwShopPingDN += TotalCost;//Update Buyer and Shop
		pUser->m_iCurWeight -= iWeightOwner;
		if(pUser->m_iCurWeight < 0) pUser->m_iCurWeight = 0;
		pUser->SendUserStatusSkill();
		pUser->GetRecoverySpeed();
		pUser->UpdateInvenSlot(&arEmptySlotShop, &arSameSlotShop);
		m_dwShopPingDN -= TotalCost;
		if(m_dwShopPingDN < 0) m_dwShopPingDN = 0;
		m_iCurWeight += iWeight;
		SendUserStatusSkill();
		GetRecoverySpeed();
		UpdateInvenSlot(&arEmptySlotBuyer, &arSameSlotBuyer);
		pUser->m_bPShopView = 0;
		pUser->m_iPShopViewuid = -1;
		m_bViewingAShop = FALSE;
	}*/
	else if(pUser->m_dwNoChatTime == 1)//标志商店
			{
				if((int)TotalCost > BiaoZhiNum) return;
				pUser->GiveItem(724,TotalCost);
				pUser->m_iCurWeight -= iWeightOwner;
				if(pUser->m_iCurWeight < 0) pUser->m_iCurWeight = 0;
				pUser->GetRecoverySpeed();
				pUser->UpdateInvenSlot(&arEmptySlotShop, &arSameSlotShop);
				RobItem(724,TotalCost);
				if(BiaoZhiNum < 0) BiaoZhiNum = 0;
				m_iCurWeight += iWeight;
				GetRecoverySpeed();
				UpdateInvenSlot(&arEmptySlotBuyer, &arSameSlotBuyer);
				pUser->m_bPShopView = 0;
				pUser->m_iPShopViewuid = -1;
				m_bViewingAShop = FALSE;
			}
	

	if(sShopAmount == 1)
		strMsg.Format(IDS_USER_SHOP_BUY_SUCCESS, "Item");
	else
		strMsg.Format(IDS_USER_SHOP_BUY_SUCCESS, "Items");

	SendSystemMsg( strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ERROR, TO_ME);

	return;

fail_result:
	for(i = 0; i < TOTAL_ITEM_NUM; i++)		// Item Information Backup
	{
		m_UserItem[i] = BuyerItem[i];
		pUser->m_UserItem[i] = ShopItem[i];
	}
	EnterCriticalSection( &pUser->m_CS_ShopItem );
	pUser->m_arShopItem.RemoveAll();
	for(i = 0; i < ShopSize; i++)
	{
		pUser->m_arShopItem.Add(ShopBackupItem[i]); 
	}
	LeaveCriticalSection( &pUser->m_CS_ShopItem );

	pUser->m_bPShopView = 0;
	pUser->m_iPShopViewuid = -1;
	m_bViewingAShop = FALSE;
	////////////////////////
	TCHAR c_Log[500];
	sprintf_s(c_Log,"[%s]在[%s]的商店购买物品失败\r\n",m_strUserID,pUser->m_strUserID);
	WriteUserShopLog(c_Log);
	////////////////////////

	if(sShopAmount == 1)
		strMsg.Format(IDS_USER_SHOP_BUY_FAIL, "Item");
	else
		strMsg.Format(IDS_USER_SHOP_BUY_FAIL, "Items");
	SendSystemMsg( strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ERROR, TO_ME);
	return;
  }
}

BOOL USER::GetTransformOption(ItemList* pItem)///删除垃圾属性      NEO版本
{
	int iMagicCount = 0;
	int nLoop = 0;
	int i = 0, j = 0, iRandom = 0;
	int iCount = 0, iTemp = 0;
	short sSid = -1;

	if(pItem == NULL) return FALSE;
	sSid = pItem->sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;

	nLoop = 4;

	//if(m_sLevel <= 20)					// 概流牢郸胶 裹困甫 饭骇俊 蝶扼 力茄茄促. 
	if(m_sLevel <= 20) iMagicCount = 42;
	else iMagicCount = 106;

	if(iMagicCount >= g_arMagicItemTable.GetSize()) iMagicCount = g_arMagicItemTable.GetSize() - 1;

	i = 0;
	srand( (unsigned)time( NULL ) );

	while(nLoop > i)										
	{
		iRandom = myrand(1, iMagicCount);

		if(!g_arMagicItemTable[iRandom]->m_tUse) continue;
		if(g_arMagicItemTable[iRandom]->m_sSubType == 32) continue;	// 郴备档 可记篮 嘿捞瘤 臼绰促.
		
		if(CheckClassWear(sSid, iRandom) == FALSE)			// Npc.cpp 狼 CheckClassItem 苞 鞍篮 扁瓷阑 荐青. User 狼 CheckClassItem 阑 龋免窍搁 救凳
		{
			if(i == 0) continue;							// 概流篮 扁夯捞 1俺
			else if(i <= 3) continue;	// 贰绢绰 扁夯捞 3俺
			else { i++; continue; }
		}

		for(j = 0; j < 4; j++)
		{
			iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sSubType;
			if(iCount != 0 && iCount == g_arMagicItemTable[iRandom]->m_sSubType)	// 加己捞 般磨荐 乐栏骨肺 捞吝 奴蔼父 急琶	
			{
				iCount = g_arMagicItemTable[pItem->tMagic[j]]->m_sChangeValue;
				if(iCount < g_arMagicItemTable[iRandom]->m_sChangeValue)
				{
					iTemp = g_arMagicItemTable[pItem->tMagic[j]]->m_tLevel;
					if(pItem->sLevel - iTemp > 0) pItem->sLevel -= iTemp;
					pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
					pItem->tMagic[j] = iRandom; 
					break;
						
				}
				else if(iCount == g_arMagicItemTable[iRandom]->m_sChangeValue) break;
			}
			if(pItem->tMagic[j] > 0) continue;										// 捞固 浇吩俊 蔼捞 乐栏搁 逞绢皑
			else
			{ 
				pItem->tMagic[j] = iRandom; i++;
				if(g_arMagicItemTable[iRandom]->m_tLevel > 0) pItem->sLevel += g_arMagicItemTable[iRandom]->m_tLevel;
				break;
			}
		}
	}
	return TRUE;
}

void USER::TransformWindow()
{
	if(FindEvent(24) == TRUE) return;
	if(m_sLevel < 20) return;

	CBufferEx TempBuf;

	TempBuf.Add((BYTE)0xEC);
	TempBuf.Add((BYTE)0x00);
	Send(TempBuf, TempBuf.GetLength());

}

void USER::TransformReq(TCHAR* pBuf)
{
	short sSourceSid = -1;
	int index = 0, j = 0;
	BYTE result = TRUE;

	if(FindEvent(24) == TRUE) return;

	if(m_sLevel < 20) return;

	// Source Item
	short sSourceSlot = GetShort(pBuf, index);			// 急琶茄 酒捞袍 浇吩锅龋
	if(sSourceSlot < EQUIP_ITEM_NUM || sSourceSlot >= TOTAL_INVEN_MAX) return;
	
	sSourceSid = m_UserItem[sSourceSlot].sSid;
	if(sSourceSid < 0 || sSourceSid >= g_arItemTable.GetSize()) return;
	
	if( sSourceSid == 669 || sSourceSid == 670 )		// 诀弊饭捞靛 窍妨绰 酒捞袍捞 臂矾弊辆幅扼搁
	{
		result = FALSE;
		goto result;
	}

	if(g_arItemTable[sSourceSid]->m_byWear < 1 && g_arItemTable[sSourceSid]->m_byWear > 5) // 公扁, 规绢备啊 酒聪搁 俺炼且 荐 绝促.
	{
		result = FALSE;
		goto result;
	}

	if(m_UserItem[sSourceSlot].tMagic[5] >= 1)			// 诀弊饭捞靛等 酒捞袍篮 俺炼且 荐 绝促.
	{
		result = FALSE;
		goto result;
	}

	if(m_UserItem[sSourceSlot].sLevel > 40)	
	{
		result = FALSE;
		goto result;
	}

	if(!GetTransformOption(&m_UserItem[sSourceSlot]))
	{
		result = FALSE;
		goto result;
	}

	m_UserItem[sSourceSlot].tIQ = RARE_ITEM;

result:
	CBufferEx TempBuf;
	TempBuf.Add((BYTE)TRANSFORM_REQ);
	TempBuf.Add(result);
	if(result == FALSE)
	{
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	AddMyEventNum(24);

	TempBuf.Add((short)sSourceSlot);						
	TempBuf.Add(m_UserItem[sSourceSlot].sLevel);
	TempBuf.Add(m_UserItem[sSourceSlot].sSid);
	TempBuf.Add(m_UserItem[sSourceSlot].sDuration);
	TempBuf.Add(m_UserItem[sSourceSlot].sBullNum);
	TempBuf.Add(m_UserItem[sSourceSlot].sCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSourceSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[sSourceSlot].tIQ);
	
	Send(TempBuf, TempBuf.GetLength());
}
BOOL USER::ChuanSongChiJiu()
{
	BYTE tSlot = -1;
	
	CBufferEx TempBuf;
		
	if(m_UserItem[6].sSid == 947 && m_UserItem[6].sDuration > 0)	tSlot = 6;
	if(m_UserItem[7].sSid == 947 && m_UserItem[7].sDuration > 0)	tSlot = 7;
	if(tSlot < 0 || tSlot > 10) return false;
	if(m_UserItem[tSlot].sDuration <= 0) return false;
		
		if((tSlot == 6 || tSlot == 7) && m_UserItem[tSlot].sDuration > 0)//减少耐久
		{			SendAccessoriDuration(947)	;return true;
		}
		else {
		SendSystemMsg("传送戒指无持久或者不存在", SYSTEM_NORMAL, TO_ME);		
		}
		return false;
}

void USER::CheckDevilTime() //恶魔广场
{
	SYSTEMTIME messageTime;
	GetLocalTime(&messageTime);
	
	DWORD dwCurrTick = GetTickCount();					

	COleDateTime CurrTime = COleDateTime(messageTime); //当前时间
	
		if( messageTime.wHour == 19 && messageTime.wMinute <= 30)
		{
			ZoneMoveReq(92, 20, 20);
		}
		else
		{
		//	SendSystemMsg("进入恶魔广场时间未到!每晚19:00--19:30开放.", SYSTEM_ERROR, TO_ME);
            SendEventMsg("进入恶魔广场时间未到!每晚19:00--19:30开放.");
		}
}
//查看装备
void USER::ViewUserInfo(int uid)
{
	
	USER *pUser = GetUser(uid- USER_BAND);
	if(pUser == NULL)
		return ;
	TCHAR  pBuf[_ITEM_DB];
	::ZeroMemory(pBuf, sizeof(pBuf));
	int index = 0;
	int i, j;
	short sCount = 0;
	for(i = 0; i < 10; i++)
	{
		sCount++;
		SetShort(pBuf,  pUser->m_UserItem[i].sLevel,	index);
		SetShort(pBuf,  pUser->m_UserItem[i].sSid,		index);
		SetShort(pBuf,  pUser->m_UserItem[i].sDuration,	index);
		SetShort(pBuf,  pUser->m_UserItem[i].sBullNum,	index);
		SetShort(pBuf,  pUser->m_UserItem[i].sCount,	index);

		for(j = 0; j < MAGIC_NUM; j++) 
			SetByte(pBuf,  pUser->m_UserItem[i].tMagic[j], index);

		SetByte(pBuf,  pUser->m_UserItem[i].tIQ, index);
	}
	//机械
	for(i = 34; i < 40; i++)
	{
		sCount++;
		SetShort(pBuf,  pUser->m_UserItem[i].sLevel,	index);
		SetShort(pBuf,  pUser->m_UserItem[i].sSid,		index);
		SetShort(pBuf,  pUser->m_UserItem[i].sDuration,	index);
		SetShort(pBuf,  pUser->m_UserItem[i].sBullNum,	index);
		SetShort(pBuf,  pUser->m_UserItem[i].sCount,	index);

		for(j = 0; j < MAGIC_NUM; j++) 
			SetByte(pBuf,  pUser->m_UserItem[i].tMagic[j], index);

		SetByte(pBuf,  pUser->m_UserItem[i].tIQ, index);
	}
	CBufferEx TempBuf;

	TempBuf.Add(PK_BUTTON_REQ);
	TempBuf.Add((BYTE)0x50);
	TempBuf.Add((short)sCount);
	TempBuf.AddData(pBuf,index);
	Send(TempBuf, TempBuf.GetLength());
	
}

//是否为130级
BOOL USER::Is130Item(int sSid)										
{
	if(sSid<=0||sSid>g_arItemTable.GetSize())
		return FALSE;
	if(g_arItemTable[sSid]->m_byRLevel == 130&&g_arItemTable[sSid]->m_byWear != 1)
		return TRUE;
	return FALSE;
}
//是否为130级
BOOL USER::Is130Wearp(int sSid)										
{
	if(sSid<=0||sSid>g_arItemTable.GetSize())
		return FALSE;
	if(g_arItemTable[sSid]->m_byRLevel == 130 &&g_arItemTable[sSid]->m_byWear == 1)
		return TRUE;
	return FALSE;
}
//是否为100级
BOOL USER::Is100Item(int sSid)										
{
	if(sSid<=0||sSid>g_arItemTable.GetSize())
		return FALSE;
	if(g_arItemTable[sSid]->m_byRLevel == 100&&g_arItemTable[sSid]->m_byWear != 1)
		return TRUE;
	return FALSE;
}
//是否为100级
BOOL USER::Is100Wearp(int sSid)										
{
	if(sSid<=0||sSid>g_arItemTable.GetSize())
		return FALSE;
	if(g_arItemTable[sSid]->m_byRLevel == 70 &&g_arItemTable[sSid]->m_byWear == 1)
		return TRUE;
	return FALSE;
}	

//添加技能点
void USER::AddMagicPoint(short sPoint)									
{
	m_DynamicUserData[MAGIC_STR_UP]+=sPoint;//装备综合值力量	
	m_DynamicUserData[MAGIC_CON_UP]+=sPoint;//装备综合值体质
	m_DynamicUserData[MAGIC_DEX_UP]+=sPoint;//装备综合值敏捷
	m_DynamicUserData[MAGIC_VOL_UP]+=sPoint;//装备综合值智慧
	m_DynamicUserData[MAGIC_WIS_UP]+=sPoint;//装备综合值智力
}
//血量百分比
void USER::AddMaxHPPoint(short sPoint)									
{
	//
	m_nHPPoint +=sPoint;
	//m_DynamicUserData[MAGIC_HP_RATING_UP]+=sPoint;
}
//130武器转换
int USER::Wearp130Convert(int sourceSolt)
{
	int i;
	int sSid = m_UserItem[sourceSolt].sSid;
	if(sSid < 0||sSid > g_arItemTable.GetSize() )
		return FALSE;
	//原物品
	switch(sSid)
	{
		//130武器1577枪,1574刀,1580杖,1583拳
	case 330://龙之怒
        m_UserItem[sourceSolt].sSid = 1578;
		break;
    case 475://强化龙之怒
	case 476://强化龙之怒
	case 477://强化龙之怒
       {
		    m_UserItem[sourceSolt].sSid = 1579;
		    break;
		}
	case 478://龙神之怒—S
	case 479://龙神之怒—S
	    m_UserItem[sourceSolt].sSid = 1580;
		break;
	case 323://反I
		m_UserItem[sourceSolt].sSid = 1572;
		break;
	case 440://反II
	case 441://反II
	case 442://反II
		{
            m_UserItem[sourceSolt].sSid = 1573;
		    break;
         }
	case 443://反III
	case 444://反III
	    m_UserItem[sourceSolt].sSid = 1574;
		break;
	case 337://龙之翼
		m_UserItem[sourceSolt].sSid = 1581;
		break;
	case 510://强化龙之翼
	case 511://强化龙之翼
	case 512://强化龙之翼
		{
            m_UserItem[sourceSolt].sSid = 1582;
		    break;
		}
	case 513://龙之翼s
	case 514://龙之翼s
	    m_UserItem[sourceSolt].sSid = 1583;
		break;
	case 316://龙神炮g2
		m_UserItem[sourceSolt].sSid = 1575;
		break;
	case 405://龙神炮g4
    case 406://龙神炮g4
	case 407://龙神炮g4
       {
            m_UserItem[sourceSolt].sSid = 1576;
			break;
		}
	case 408://龙神炮g6
    case 409://龙神炮g6
	    m_UserItem[sourceSolt].sSid = 1577;
		break;
	case 1698://龙神驭
		m_UserItem[sourceSolt].sSid = 1584;
		break;
	case 1699://强化龙神驭
		m_UserItem[sourceSolt].sSid = 1585;
		break;
	case 1700://龙神驭-S
		m_UserItem[sourceSolt].sSid = 1586;
		break;
	default:
	
        return FALSE;
		break;
	}
	//级别重新计算
	if(m_UserItem[sourceSolt].sSid<0 || m_UserItem[sourceSolt].sSid > g_arItemTable.GetSize())
		return FALSE;
	//属性级别
	m_UserItem[sourceSolt].sLevel = g_arItemTable[m_UserItem[sourceSolt].sSid]->m_byRLevel;
	m_UserItem[sourceSolt].sDuration = g_arItemTable[m_UserItem[sourceSolt].sSid]->m_sDuration;
	int itemCount = 0;
	//130级装备
	m_UserItem[sourceSolt].tIQ = 0x12;//使用18无法看到四排以后的属性
	m_UserItem[sourceSolt].tMagic[ITEM_UPGRADE_COUNT] -=2;//合成后武器掉2改
	if(m_UserItem[sourceSolt].tIQ > NORMAL_ITEM)
	{
		if(m_UserItem[sourceSolt].tIQ == UNIQUE_ITEM) 
			itemCount = MAGIC_NUM;	
		else 
			itemCount = 4;								
		for(i = 0; i < itemCount; i++)
		{
			BYTE bMagic = m_UserItem[sourceSolt].tMagic[i];
			if(bMagic >= 0 && bMagic < g_arMagicItemTable.GetSize())
			{
				m_UserItem[sourceSolt].sLevel += g_arMagicItemTable[bMagic]->m_tLevel;
			}
		}
	}
	return TRUE;
}
//装备转换
int USER::Item130Convert(int sourceSolt)
{
	int i;
	int sSid = m_UserItem[sourceSolt].sSid;
	if(sSid < 0||sSid > g_arItemTable.GetSize() )
		return FALSE;
	//原物品
	switch(sSid)
	{
	case 1053://龙牙帽
	case 1054://强化龙牙帽
	case 1055://特殊龙牙盔
	case 1056://龙牙盔S型
		{
			//拳1590,法1594,刀1598,枪1602,仲裁1606
			int s = 1590;
			switch(m_byClass)
			{
			case 0:
				break;
			case 1:
				s = 1594;
				break;
			case 2:
				s = 1598;
				break;
			case 3:
				s = 1602;
				break;
			case 4:
				s = 1606;
				break;
			}
			m_UserItem[sourceSolt].sSid = s;
			break;
		}
	case 1057://烈焰战甲
		m_UserItem[sourceSolt].sSid = 1607;
		break;
	case 1058://强化烈焰战甲
		m_UserItem[sourceSolt].sSid = 1608;
		break;
	case 1059://烈焰战甲G型
		m_UserItem[sourceSolt].sSid = 1609;
		break;
	case 1060://烈焰战甲S型
		m_UserItem[sourceSolt].sSid = 1610;
		break;
	case 1061://烈焰护腿
		m_UserItem[sourceSolt].sSid = 1611;
		break;
	case 1062://强化烈焰护腿
		m_UserItem[sourceSolt].sSid = 1612;
		break;
	case 1063://烈焰护腿G型
		m_UserItem[sourceSolt].sSid = 1613;
		break;
	case 1064://烈焰护腿S型
		m_UserItem[sourceSolt].sSid = 1614;
		break;
	case 1065://苍月之铠
		m_UserItem[sourceSolt].sSid = 1615;
		break;
	case 1066://强化苍月之铠
		m_UserItem[sourceSolt].sSid = 1616;
		break;
	case 1067://苍月之铠II型
		m_UserItem[sourceSolt].sSid = 1617;
		break;
	case 1068://苍月之铠M型
		m_UserItem[sourceSolt].sSid = 1618;
		break;
	case 1069://苍月护腿
		m_UserItem[sourceSolt].sSid = 1619;
		break;
	case 1070://强化苍月护腿
		m_UserItem[sourceSolt].sSid = 1620;
		break;
	case 1071://苍月护腿II型
		m_UserItem[sourceSolt].sSid = 1621;
		break;
	case 1072://苍月护腿M型
		m_UserItem[sourceSolt].sSid = 1622;
		break;
	case 1073://寒冰之铠
		m_UserItem[sourceSolt].sSid = 1623;
		break;
	case 1074://强化寒冰之铠
		m_UserItem[sourceSolt].sSid = 1624;
		break;
	case 1075://寒冰之铠G型
		m_UserItem[sourceSolt].sSid = 1625;
		break;
	case 1076://寒冰之铠S型
		m_UserItem[sourceSolt].sSid = 1626;
		break;
	case 1077://寒冰护腿
		m_UserItem[sourceSolt].sSid = 1627;
		break;
	case 1078://强化寒冰护腿
		m_UserItem[sourceSolt].sSid = 1628;
		break;
	case 1079://寒冰护腿G型
		m_UserItem[sourceSolt].sSid = 1629;
		break;
	case 1080://寒冰护腿S型
		m_UserItem[sourceSolt].sSid = 1630;
		break;
	case 1081://疾风战甲
		m_UserItem[sourceSolt].sSid = 1631;
		break;
	case 1082://强化疾风战甲
		m_UserItem[sourceSolt].sSid = 1632;
		break;
	case 1083://疾风战甲II型
		m_UserItem[sourceSolt].sSid = 1633;
		break;
	case 1084://疾风战甲M型
		m_UserItem[sourceSolt].sSid = 1634;
		break;
	case 1085://疾风护腿
		m_UserItem[sourceSolt].sSid = 1635;
		break;
	case 1086://强化疾风护腿
		m_UserItem[sourceSolt].sSid = 1636;
		break;
	case 1087://疾风护腿II型
		m_UserItem[sourceSolt].sSid = 1637;
		break;
	case 1088://疾风护腿M型
		m_UserItem[sourceSolt].sSid = 1638;
		break;
	case 1709://光明铠甲
		m_UserItem[sourceSolt].sSid = 1639;
		break;
	case 1710://强化光明铠甲
		m_UserItem[sourceSolt].sSid = 1640;
		break;
	case 1711://光明铠甲II型
		m_UserItem[sourceSolt].sSid = 1641;
		break;
	case 1712://光明铠甲M型
		m_UserItem[sourceSolt].sSid = 1642;
		break;
	case 1713://光明护腿
		m_UserItem[sourceSolt].sSid = 1643;
		break;
	case 1714://强化光明护腿
		m_UserItem[sourceSolt].sSid = 1644;
		break;
	case 1715://光明护腿II型
		m_UserItem[sourceSolt].sSid = 1645;
		break;
	case 1716://光明护腿M型
		m_UserItem[sourceSolt].sSid = 1646;
		break;
	case 1089://龙骨靴
	case 1090://强化龙骨靴
	case 1091://特殊龙骨靴
	case 1092://龙骨靴S型
		{
			//拳鞋1650,法鞋1654,刀鞋1658,枪鞋1662,仲裁1666
			int s = 1650;
			switch(m_byClass)
			{
			case 0:
				break;
			case 1:
				s = 1654;
				break;
			case 2:
				s = 1658;
				break;
			case 3:
				s = 1662;
				break;
			case 4:
				s = 1666;
				break;
			}
			m_UserItem[sourceSolt].sSid = s;
			break;
		}
	default:
		return FALSE;
		break;
	}
	//级别重新计算
	if(m_UserItem[sourceSolt].sSid<0 || m_UserItem[sourceSolt].sSid > g_arItemTable.GetSize())
		return FALSE;
	//属性级别
	m_UserItem[sourceSolt].sLevel = g_arItemTable[m_UserItem[sourceSolt].sSid]->m_byRLevel;
	m_UserItem[sourceSolt].sDuration = g_arItemTable[m_UserItem[sourceSolt].sSid]->m_sDuration;
	int itemCount = 0;
	//130级装备
	m_UserItem[sourceSolt].tIQ = 18;
	m_UserItem[sourceSolt].tMagic[ITEM_UPGRADE_COUNT] -=2;//合成后装备掉2改
	if(m_UserItem[sourceSolt].tIQ > NORMAL_ITEM)
	{
		if(m_UserItem[sourceSolt].tIQ == UNIQUE_ITEM) 
			itemCount = MAGIC_NUM;	
		else if(m_UserItem[sourceSolt].tIQ == 12||m_UserItem[sourceSolt].tIQ == 18)
			itemCount = 5;								
		else
			itemCount = 4;								
		for(i = 0; i < itemCount; i++)
		{
			BYTE bMagic = m_UserItem[sourceSolt].tMagic[i];
			if(bMagic >= 0 && bMagic < g_arMagicItemTable.GetSize())
			{
				m_UserItem[sourceSolt].sLevel += g_arMagicItemTable[bMagic]->m_tLevel;
			}
		}
	}
	return TRUE;
}

void USER::Item130Convert(int sourceSolt,int materialSolt1,int materialSolt2,int materialSolt3)		//百级转130级装备
{
	//判断位置
	if(sourceSolt<10||materialSolt1<10||materialSolt2<10||materialSolt3<10
		||sourceSolt>=34||materialSolt1>=34||materialSolt2>=34||materialSolt3>=34)
		return ;
	//判断ID
	int sSid = m_UserItem[sourceSolt].sSid,s1 = m_UserItem[materialSolt1].sSid,
		s2 = m_UserItem[materialSolt2].sSid,s3 = m_UserItem[materialSolt3].sSid;
	if(sSid < 0||sSid > g_arItemTable.GetSize() )
		return ;

	if(s1 < 0||s1 > g_arItemTable.GetSize() )
		return ;

	if(s2 < 0||s2 > g_arItemTable.GetSize() )
		return ;

	if(s3 < 0||s3 > g_arItemTable.GetSize() )
		return ;
	//材料ID
	if(s1 != 1674||s2 !=1675||s3 != 1676)
		return ;
	//钱数是否够
	if(m_dwDN < 30000000)
	{
		SendSystemMsg("转换130级装备需要3000万,你的金钱不足",SYSTEM_ERROR,TO_ME);
		return;
	}
	//主体是否够改数
	//武器还是装备
	if(g_arItemTable[sSid]->m_byWear == 1)
	{
		//70级武器需要10改
		if(m_UserItem[sourceSolt].tMagic[ITEM_UPGRADE_COUNT] <10)
			return ;
		//武器转换
		if(!Wearp130Convert(sourceSolt))
			return ;	
	}else 
	{
		//100级防具8改
		if(m_UserItem[sourceSolt].tMagic[ITEM_UPGRADE_COUNT] <8)
			return ;
		//防具转换
		if(!Item130Convert(sourceSolt))
			return ;
	}
	//130级tIQ=18

	m_UserItem[materialSolt1].sCount-=1;//减去材料
	if(m_UserItem[materialSolt1].sCount<=0)
		ReSetItemSlot(&m_UserItem[materialSolt1]);

	//减去材料
	m_UserItem[materialSolt2].sCount-=1;
	if(m_UserItem[materialSolt2].sCount<=0)
		ReSetItemSlot(&m_UserItem[materialSolt2]);
	//减去材料
	m_UserItem[materialSolt3].sCount-=1;
	if(m_UserItem[materialSolt3].sCount<=0)
		ReSetItemSlot(&m_UserItem[materialSolt3]);

	//减去钱
	if( m_dwDN <= 30000000 )
		m_dwDN = 0;
	else
		m_dwDN = m_dwDN - 30000000;
	UpdateUserItemDN();					
	SendMoneyChanged();
	//发送物品变化
	CByteArray arMaterial;
	arMaterial.RemoveAll();
	arMaterial.Add((BYTE)sourceSolt);
	arMaterial.Add((BYTE)materialSolt1);
	arMaterial.Add((BYTE)materialSolt2);
	arMaterial.Add((BYTE)materialSolt3);

	CBufferEx TempBuf;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	int index = arMaterial.GetSize();
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)index);
	int i,j;
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) 
			TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	FlushItemLog( TRUE );
}

//升级130级装备
void USER::Update130Item(int sourceSolt,int materialSolt1,BYTE bReceive,int nAdd1,int nAdd2,int nAdd3,int nAdd4)		
{
/*	//判断位置
	if(sourceSolt<10||materialSolt1<10||sourceSolt>=34||materialSolt1>=34)
		return ;
	//判断ID
	int sSid = m_UserItem[sourceSolt].sSid,s1 = m_UserItem[materialSolt1].sSid;
	if(sSid < 0||sSid > g_arItemTable.GetSize() )
		return ;

	if(s1 < 0||s1 > g_arItemTable.GetSize() )
		return ;
	//王者灵石1438,王者晶石1437
	if(s1 != 1438 && s1 != 1437)
		return ;
	//非130级装备
	//if(g_arItemTable[sSid]->m_byRLevel!= 130)
	//	return ;
	//非130级装备
	if(!Is130Wearp(sSid) && !Is130Item(sSid))
	{
		SendSystemMsg( "王者晶石,王者灵石只能升级130级装备与武器", SYSTEM_ERROR, TO_ME);
		return ;
	}
	//130装备且王者灵石
	if( Is130Wearp(sSid)&& s1 != 1438)
	{
		return ;
	}
	//130装备且王者晶石
	if( Is130Item(sSid)&& s1 != 1437)
	{
		return ;
	}
	//需要四个神石或一个超神
	int nAddSid,nNAddCount=0;
	if(nAdd1<10||nAdd1>=34)
	{
		goto ErrorNoMeartial;
	}
	nAddSid = m_UserItem[nAdd1].sSid;
	//超级神石986,626神石
	if(nAddSid != 986 &&nAddSid != 626)
	{
		goto ErrorNoMeartial;
	}
	nNAddCount++;
	if(nAddSid == 986)
	{
		goto SucMeartial;
	}
	//第二块神石
	if(nAdd2<10||nAdd2>=34)
	{
		goto ErrorNoMeartial ;
	}
	nAddSid = m_UserItem[nAdd2].sSid;
	if(nAddSid != 626)
	{
		goto ErrorNoMeartial;
	}
	nNAddCount++;
	//第三块神石
	if(nAdd3<10||nAdd3>=34)
	{
		goto ErrorNoMeartial ;
	}
	nAddSid = m_UserItem[nAdd3].sSid;
	if(nAddSid != 626)
	{
		goto ErrorNoMeartial;
	}
	nNAddCount++;
	//第四块神石
	if(nAdd4<10||nAdd4>=34)
	{
		goto ErrorNoMeartial ;
	}
	nAddSid = m_UserItem[nAdd4].sSid;
	if(nAddSid != 626)
	{
		goto ErrorNoMeartial;
	}
	nNAddCount++;
	goto SucMeartial;

ErrorNoMeartial:
	SendSystemMsg(IDS_USER_SHENGSHI, SYSTEM_ERROR, TO_ME);
	return ;
SucMeartial:
	int iRandom = 0;

	BYTE tCount = m_UserItem[sourceSolt].tMagic[5];
	if(tCount >= MAX_ITEM_UPGRADE_COUNT) 
		return ;		
	iRandom = myrand(1, 10000);						// 己傍伏苞 厚背困秦 罚待荐甫 备窃
	int iSuccess = 0;
	if(iRandom <= g_ItemAttUpgrade[tCount])
	{			
		iRandom = myrand(1, 10000);				// 绵酒捞袍老版快 +3诀鳖瘤 焊呈胶肺 档傈且荐 乐促
		for(int i = 0; i < 3; i++)
		{
			iSuccess += 1;						// 绵 酒捞袍俊狼秦 割锅 诀臂沁绰瘤 舅妨 霖促.

			if(iRandom <= g_ItemBlessingUpgrade[i])
				break;
			if((tCount + iSuccess) >= MAX_ITEM_UPGRADE_COUNT)
				break;
		}
	}
	BOOL bSuc = (iSuccess>=1);
	if(iSuccess >= 1)									// 秦寸 锅掳狼 己傍伏焊促 累栏搁
	{	
		if(m_UserItem[sourceSolt].tIQ==12)
			m_UserItem[sourceSolt].tMagic[ITEM_UPGRADE_COUNT]++;
		else
			m_UserItem[sourceSolt].tMagic[ITEM_UPGRADE_COUNT] = tCount + iSuccess;
		MakeItemLog( &m_UserItem[sourceSolt], ITEMLOG_BLESS_UPGRADE_SUCCESS );
	}else
	{
		int iUp = m_UserItem[sourceSolt].tMagic[ITEM_UPGRADE_COUNT];
		iRandom = myrand(1, 10000);
		//降级
		if(iRandom <= g_ItemNormalDownUpgrade[0]) 
		{
			iUp-=1;
		}
		if(iUp < 0) 
			iUp = 0;
		m_UserItem[sourceSolt].tMagic[ITEM_UPGRADE_COUNT] = iUp;
	}
	//开始清理数据
	//减去材料
	m_UserItem[materialSolt1].sCount-=1;
	if(m_UserItem[materialSolt1].sCount<=0)
		ReSetItemSlot(&m_UserItem[materialSolt1]);
	m_UserItem[nAdd1].sCount-=nNAddCount;
	if(m_UserItem[nAdd1].sCount<=0)
		ReSetItemSlot(&m_UserItem[nAdd1]);


	//发送物品变化
	CByteArray arMaterial;
	arMaterial.RemoveAll();
	arMaterial.Add((BYTE)sourceSolt);
	arMaterial.Add((BYTE)materialSolt1);
	arMaterial.Add((BYTE)nAdd1);

	CBufferEx TempBuf;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	int index = arMaterial.GetSize();
	TempBuf.Add((BYTE)bSuc);
	TempBuf.Add((BYTE)index);
	int i,j;
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) 
			TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	FlushItemLog( TRUE );
	return ;*/
}
/////////////////////////////////////////////////////////////////////////////
//	sSid,sitemmagic
// 
BOOL USER::IsMagicVSItem(int artable,int armagic)
{
	if(artable < 0 || artable >= g_arItemTable.GetSize())
		return FALSE;	

	if(armagic < 0 || armagic >= g_arMagicItemTable.GetSize())
		return FALSE;

	int iWear;

	BYTE armWear = g_arItemTable[artable]->m_byWear;			// 酒捞袍 拌凯 1: 公扁 2~8 : 馒侩酒捞袍
	BYTE tNeedClass = g_arItemTable[artable]->m_byClass;
	BYTE armMagic = g_arMagicItemTable[armagic]->m_tNeedClass;	// 概流加己 拌凯

	//15表示所有职业
	if(armMagic != ALL_JOBS)
	{
		BYTE tTemp = 1;	
		BYTE tFire = 0;
		BYTE tEdge = 0;
		BYTE tStaff = 0;
		BYTE tBrawl = 0;

		tFire	 = tTemp & tNeedClass; tTemp = 2; 
		tEdge	 = tTemp & tNeedClass; tTemp = 4;
		tStaff	 = tTemp & tNeedClass; tTemp = 8;
		tBrawl	 = tTemp & tNeedClass;

		tFire = tFire & armMagic;
		tEdge = tEdge & armMagic;
		tStaff = tStaff & armMagic;
		tBrawl = tBrawl & armMagic;

		tTemp = tFire^tEdge^tStaff^tBrawl;
		if(!tTemp)
			return FALSE;
		//		if(tNeedClass != armMagic) return FALSE;
	}

	iWear = g_arMagicItemTable[armagic]->m_tWearInfo;		// 0表示没有限制，1表示武器属性,2表示装备属性

	if(iWear == 0)
		return TRUE;
	else if(iWear == 1)											
	{														// 1锅捞搁 公扁幅俊 嘿绰促.
		if(armWear != 1)
			return FALSE;
		else return TRUE;
	}
	else if(iWear == 2)										// 2锅捞搁 公扁甫 力寇茄 馒侩酒捞袍俊 嘿绰促.
	{
		if((armWear <= 1 || armWear >= 9)&&armWear!= 143)//130级装备属性
			return FALSE;
		else 
			return TRUE;
	}
	else return FALSE;
}
//	100-130级装备改数魔防
short USER::Get100ItemPsiDefense(short sSid, int slot)          ////魔法防御计算    //////////////////////////////////NEO版本需要开放的
{
	short sDefense = 0;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) 
		return 0;
	if(slot < 0 || slot > EQUIP_ITEM_NUM) 
		return 0;
	if(m_UserItem[slot].tIQ!=12&&m_UserItem[slot].tIQ!=18)
		return 0;
	//改数转成防																
	if(m_UserItem[slot].tIQ != UNIQUE_ITEM)	
	{
		int up_count = 0;
		up_count = m_UserItem[slot].tMagic[ITEM_UPGRADE_COUNT];
		if(up_count > 0 && up_count <= MAX_ITEM_UPGRADE_COUNT)
		{
			//100级魔法防改+3,后面+5,130级魔抗6改5点,6改以上点
			if(Is130Item(sSid)&&m_UserItem[slot].tIQ==18)
			{
				if(up_count<=6)
				{
					sDefense = up_count*5;
				}
				else
				{
					sDefense = (up_count-6)*7+6*5;
				}
			}else
			{
				if(up_count<=7)
				{
					sDefense = up_count*3;
				}
				else
				{
					sDefense = (up_count-7)*5+7*3;
				}
			}
		}
	}
	//原始装备的防
	return sDefense;
} 
//////////////////////机械转换为超级机械/////////////////////////////
void USER::EbodyConvertSuperEbody(int sMaster,int sMetral1,int sMetral2)
{
	//判断位置
	short sSid;
	if(sMaster < EQUIP_ITEM_NUM || sMaster >= TOTAL_INVEN_MAX)
		return;
	sSid = m_UserItem[sMaster].sSid;
	if(sSid <0||sSid>g_arItemTable.GetSize())
		return;
	if(!IsEbodyItem(sSid))
		return;	
	if( m_UserItem[sMaster].tMagic[5]<7)
	{
		return;
	}
	if(m_UserItem[sMaster].tIQ!=2)
		return ;
	if(m_UserItem[sMaster].tMagic[MAGIC_100_ADD_POS]!=0)
		return ;

	if(sMetral1 < EQUIP_ITEM_NUM || sMetral1 >= TOTAL_INVEN_MAX)//材料1是否合法
		return;
	sSid = m_UserItem[sMetral1].sSid;
	if(sSid <0||sSid>g_arItemTable.GetSize())
		return;
	if(!IsEbodyItem(sSid))
		return;	
	if( m_UserItem[sMetral1].tMagic[5]<7)
	{
		return;
	}
	if(m_UserItem[sMetral1].tIQ!=2)
		return ;
	if(m_UserItem[sMetral1].tMagic[MAGIC_100_ADD_POS]!=1)
		return ;

	if(sMetral2 < EQUIP_ITEM_NUM || sMetral2 >= TOTAL_INVEN_MAX)//材料2是否合法
		return;
	sSid = m_UserItem[sMetral2].sSid;
	if(sSid <0||sSid>g_arItemTable.GetSize())
		return;
	if(!IsEbodyItem(sSid))
		return;	
	if( m_UserItem[sMetral2].tMagic[5]<7)
	{
		return;
	}
	if(m_UserItem[sMetral2].tIQ!=2)
		return ;
	if(m_UserItem[sMetral2].tMagic[MAGIC_100_ADD_POS]!=1)
		return ;
	//属性数是否<
	int magicount1  = 0,magicount2= 0,magicount3 = 0;
	int i ;
	for(i = 1;i<4;i++)
	{
		BYTE bMagic = m_UserItem[sMaster].tMagic[i];
		if(bMagic >0 && bMagic<g_arEBodyTable.GetSize())
			magicount1++;

		bMagic = m_UserItem[sMetral1].tMagic[i];
		if(bMagic >0 && bMagic<g_arEBodyTable.GetSize())
			magicount2++;

		bMagic = m_UserItem[sMetral2].tMagic[i];
		if(bMagic >0 && bMagic<g_arEBodyTable.GetSize())
			magicount3++;
	}
	if(magicount1<1||magicount1>magicount2 ||magicount1>magicount3)
		return;

	//是否够钱数与经验
	if(m_dwDN < 20000000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);
		return ;		
	}
	if(m_dwXP < 1500)
	{
		SendSystemMsg( "战斗经验不足", SYSTEM_ERROR, TO_ME);
		return ;		
	}
	if( m_dwDN <= 20000000 ) 
		m_dwDN = 0;
	else 
		m_dwDN = m_dwDN - 20000000;
	//战斗经验
	if( m_dwXP <= 1500 ) 
		m_dwXP = 0;
	else 
		m_dwXP = m_dwXP - 1500;

	//1247-1252头
	//1253-1267衣服
	//1268-1270手臂
	//1271-1276裤子
	//1277 头
	short sEbodyID[]={877,878,879,880,881,883,910,//头
					  884,885,886,887,888,889,890,891,892,893,890,891,892,893,
	                  894,895,896,897,898,899,987,//必杀-->超级阿奴比斯
					  901,902,903,904,905,906};
	short sConvertEbodyID[]={1247,1248,1249,1250,1251,1252,1252,//头
					  1253,1254,1255,1256,1257,1258,1259,1260,1261,1262,1263,1264,1265,1266,
	                  1268,1269,1270,1268,1269,1269,1270,
					  1271,1272,1273,1274,1275,1276};

	/*
	//死亡使者-->超级阿奴比斯
	//艾贝依多->超级阿贝伊
	//阿卡第安->超级阿卡迪安
	//阿勒敏->超级阿尔敏
	//亚克->超级阿克
	//杰罗司-->超级杰罗斯
	//阿米诺->超级阿米奴
	//戎玟-->超级罗米
	//匝金->超级拉尔
	//彤碧志->超级淘米兹
	//卜利廷->超级布尔锑
	//奥丁->超级奥仃
	//儒贝司->超级鲁贝斯
	//仲轵->超级鲁兹
	//安吉拉->超级格尔
	//玛兹->超级马兹
	//疾敏->超级吉米
	//德力马->超级雷利阿
	//加斯->超级加斯
	//玛侬->超级马奴
	//德伊阿->超级雷伊阿
	//乍巴沃克->超级扎巴沃克
	//玟利->超级姆利
	//珀伊玛->超级珀尔玛
	//凯罗贝兹->超级卡贝尔
	//疾敏->超级巴米里温
	//阿路西斯-->超级阿尔希斯
	//帕罗尼->超级巴洛尼
	//库贝->超级库贝尔
	//玖玟->
	short sEbodyID[]={987,877,888,899,902,889,896,903,885,900,904,881,890,897,905,
	880,891,898,906,955,884,895,879,886,887,878,892,893,910,883};
	short sConvertEbodyID[]={1270,1247,1257,1268,1272,1258,1265,1273,1254,1269,1274,1251,1259,1266,1275,
	1250,1260,1267,1276,1253,1264,1249,1255,1256,1248,1261,1262,1277,1252};
	*/
	int nECount = sizeof(sEbodyID)/sizeof(sEbodyID[0]);
	int j;
	//主体id
	sSid = m_UserItem[sMaster].sSid;
	for(i = 0;i<nECount;i++)
	{
		if(sSid == sEbodyID[i])
		{
			break;
		}
	}
	if(i>=nECount)
		return ;
	ReSetItemSlot(&m_UserItem[sMetral1]);
	ReSetItemSlot(&m_UserItem[sMetral2]);
	m_UserItem[sMaster].sSid = sConvertEbodyID[i];
	m_UserItem[sMaster].tIQ = SUPER_EBODY_ITEM;
	m_UserItem[sMaster].tMagic[5] -= 7;
	//级别重新计算
	if(m_UserItem[sMaster].sSid<0 || m_UserItem[sMaster].sSid > g_arItemTable.GetSize())
		return ;
	//属性级别
	m_UserItem[sMaster].sLevel = g_arItemTable[m_UserItem[sMaster].sSid]->m_byRLevel;
	m_UserItem[sMaster].sDuration = g_arItemTable[m_UserItem[sMaster].sSid]->m_sDuration;
	int itemCount = 0;
	if(m_UserItem[sMaster].tIQ > NORMAL_ITEM)
	{
		if(m_UserItem[sMaster].tIQ == UNIQUE_ITEM) 
			itemCount = MAGIC_NUM;	
		else 
			itemCount = 4;								
		for(i = 0; i < itemCount; i++)
		{
			BYTE bMagic = m_UserItem[sMaster].tMagic[i];
			if(bMagic >= 0 && bMagic < g_arEBodyTable.GetSize())
			{
				m_UserItem[sMaster].sLevel += g_arEBodyTable[bMagic]->m_tLevel;
			}
		}
	}

	//发送物品变化
	CByteArray arMaterial;
	arMaterial.RemoveAll();
	arMaterial.Add((BYTE)sMaster);
	arMaterial.Add((BYTE)sMetral1);
	arMaterial.Add((BYTE)sMetral2);

	CBufferEx TempBuf;
	TempBuf.Add(UPGRADE_ITEM_RESULT);
	int index = arMaterial.GetSize();
	TempBuf.Add((BYTE)1);
	TempBuf.Add((BYTE)index);
	for(i = 0; i < arMaterial.GetSize(); i++)
	{
		BYTE bySlot = (BYTE)arMaterial[i];
		TempBuf.Add((BYTE)bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) 
			TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	Send(TempBuf, TempBuf.GetLength());
	FlushItemLog( TRUE );
}
//转职
void USER::ChangeJob(int nNewJob,int npcsay)
{
	int i = 0;
	int nOldClass = m_byClass;
	if(m_byClass == nNewJob)
	{
		//SendSystemMsg("你已经是仲裁者,无需转职!", SYSTEM_ERROR, TO_ME);
		SendEventMsg("你已经是仲裁者,无需转职!");
        return ;
	}
	bool bHaveItem = false;
	//身上是否有装备
	for(i = 0;i<10;i++)
	{
		if(m_UserItem[i].sSid != -1)
		{
			bHaveItem = true;
			break;
		}
	}
	if(bHaveItem)
	{
	//	SendSystemMsg("请取下您身上的装备后,再来转职.", SYSTEM_ERROR, TO_ME);
		SendEventMsg("请取下您身上的装备后,再来转职.");
		return ;
	}
	//身上是否有装备
	for(i = 34;i<40;i++)
	{
		if(m_UserItem[i].sSid != -1)
		{
			bHaveItem = true;
			break;
		}
	}
	if(bHaveItem)
	{
		//SendSystemMsg("请取下您身上的机械守护后,再来转职.", SYSTEM_ERROR, TO_ME);
        SendEventMsg("请取下您身上的机械守护后,再来转职.");
		return ;
	}
	m_byClass = nNewJob;

	//技能清零
	for( i = 0; i < SKILL_COUNT; i++)
	{
		m_UserSkill[i].sSid=i;
		m_UserSkill[i].tLevel = 0;
	}
	for( i = 0; i < 12; i++)
	{
		m_UserSkill_[i].sSid=i;
		m_UserSkill_[i].tLevel = 0;
	}
    for( i = 0; i < 2; i++)
	{
		m_UserSkill140[i].sSid = 0;
		m_UserSkill140[i].tLevel = 0;
	}

	switch (m_byClass)
	{
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
		m_sDEX = 17;
		m_sVOL = 9;
		m_sWIS = 9;
		break;
	}
	DelMyEventNum(251);//120级技能
	//DelMyEventNum(300);//新手装备技能
	//清理魔法
	m_nHavePsiNum = 0;

	for(i = 0; i < TOTAL_PSI_NUM; i++)					// 檬扁拳甫 怖 秦具 茄促.
	{
		m_UserPsi[i].sSid = -1;
		m_UserPsi[i].tOnOff = FALSE;
	}

	//技能点
	m_sPA = (short)(m_sLevel-1)+8;
	int skill_point = 0;
	if( m_sLevel <= 60 )
		skill_point = (int)( (double)m_sLevel / 2 + 0.5 );
	else if( m_sLevel > 60 && m_sLevel <= 99 )
		skill_point = (int)( 30 + (double)m_sLevel - 60 + 0.5 );
	else if( m_sLevel > 99 )
		skill_point = (int)( 30 + 99 - 60 + 0.5 );
	m_sSkillPoint = (short)skill_point;

	if(m_sLevel >= 130 && o_yehuoini[0]->chaoneng == 1)//130级技能
	{
		m_sSkillPoint_= m_sLevel - 129;
		if(m_sSkillPoint_ >70)
			m_sSkillPoint_=70;
	}
	for( i = 0;i<2;i++)
	{
		m_UserSkill140[i].sSid = 0;
		m_UserSkill140[i].tLevel = 0;
	}
	//
	GetMagicItemSetting();

	SendMyInfo( TO_INSIGHT, INFO_MODIFY );	// || 

	SendUserStatusSkill();

	SendCharData();

	TCHAR szMsg[200],szOldJob[40],szNewJob[40];
	switch(nOldClass)
	{
	case 0:
		wsprintf(szOldJob,"格斗家");
		break;
	case 1:
		wsprintf(szOldJob,"魔法师");
		break;
	case 2:
		wsprintf(szOldJob,"剑武士");
		break;
	case 3:
		wsprintf(szOldJob,"枪械师");
		break;
	case 4:
		wsprintf(szOldJob,"仲裁者");
		break;
	}
	switch(nNewJob)
	{
	case 0:
		wsprintf(szNewJob,"格斗家");
		break;
	case 1:
		wsprintf(szNewJob,"魔法师");
		break;
	case 2:
		wsprintf(szNewJob,"剑武士");
		break;
	case 3:
		wsprintf(szNewJob,"枪械师");
		break;
	case 4:
		wsprintf(szNewJob,"仲裁者");
		break;
	}
	wsprintf(szMsg,"%s 转职成为 %s",szOldJob,szNewJob);
	SendSayAddStr(npcsay,szMsg);
	//SendSystemMsg("转职成功.谢谢参与", SYSTEM_ANNOUNCE, TO_ME);
}

void USER::StudyNewJobPsi(int sPsi)
{
//	if(m_byClass!=JUDGE)
//		return ;
	BYTE tResult = 1;
	for(int i = 0;i<m_nHavePsiNum;i++)
	{
		if(m_UserPsi[i].sSid == sPsi)
			return ;
	}
	m_UserPsi[m_nHavePsiNum].sSid = sPsi;
	m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
	m_nHavePsiNum++;
	CBufferEx TempBuf;

	TempBuf.Add(BUY_PSY_RESULT);
	TempBuf.Add(tResult);
	TempBuf.Add(1);

	TempBuf.Add((short)sPsi);
	Send(TempBuf, TempBuf.GetLength());
	SendCharData();
}

//判断能否给新手装备
BOOL USER::CanGiveNewerItem()
{
	if(FindEvent(300))
	{
	//	SendSystemMsg("你已经领取新手装备,请勿重复领取.", SYSTEM_ERROR, TO_ME);
		SendEventMsg("你已经领取新手装备,请勿重复领取.");
		return FALSE;
	}
	int i;
	bool bHaveItem = false;
	//身上是否有装备
	for(i = 0;i<10;i++)
	{
		if(m_UserItem[i].sSid != -1)
		{
			bHaveItem = true;
			break;
		}
	}
	if(bHaveItem)
	{
		//SendSystemMsg("请取下您身上的装备后,再来领取新手装备.", SYSTEM_ERROR, TO_ME);
		SendEventMsg("请取下您身上的装备后,再来领取新手装备.");
		return FALSE;
	}
	//身上是否有装备
	for(i = 34;i<40;i++)
	{
		if(m_UserItem[i].sSid != -1)
		{
			bHaveItem = true;
			break;
		}
	}
	if(bHaveItem)
	{
		//SendSystemMsg("请取下您身上的机械守护后,再来领取新手装备.", SYSTEM_ERROR, TO_ME);
          SendEventMsg("请取下您身上的机械守护后,再来领取新手装备.");
		return FALSE;
	}
	return TRUE;
}
//新手装备
void  USER::GiveNewerItem(int nSolt,int sSid,int sCount,int sIQ,int nUpg,int magic0,int magic1,int magic2,int magic3,int magic4)	
{
	if(nSolt<0||nSolt>40)
	{
		return ;
	}
	int itemSize = g_arItemTable.GetSize();
	if(sSid<0||sSid>itemSize)
		return ;

	if(sCount<=0)
		return ;
	if(nUpg>MAX_ITEM_UPGRADE_COUNT)
		return ;
	
	/*
	//衣裤与装备
	if(g_arItemTable[sSid]->m_byWear == 1&&(nSolt !=LEFT_HAND||nSolt!=RIGHT_HAND))
	return ;
	if(g_arItemTable[sSid]->m_byWear == 2&&nSolt != HEAD)
	return ;
	if(g_arItemTable[sSid]->m_byWear == 3&&nSolt != BODY)
	return ;
	if(g_arItemTable[sSid]->m_byWear == 4&&nSolt != PANTS)
	return ;
	if(g_arItemTable[sSid]->m_byWear == 5&&nSolt != SHOES)
	return ;
	//机械
	*/
	ReSetItemSlot(&m_UserItem[nSolt]);
	m_UserItem[nSolt].sSid = sSid;
	m_UserItem[nSolt].sCount = sCount;
	m_UserItem[nSolt].tMagic[0] = magic0;
	m_UserItem[nSolt].tMagic[1] = magic1;
	m_UserItem[nSolt].tMagic[2] = magic2;
	m_UserItem[nSolt].tMagic[3] = magic3;
	m_UserItem[nSolt].tMagic[4] = magic4;
	m_UserItem[nSolt].tMagic[5] = nUpg;
	m_UserItem[nSolt].tIQ = sIQ;

	m_UserItem[nSolt].sDuration = g_arItemTable[sSid]->m_sDuration;
	ResetItemLevelBySolt(nSolt);
}
//新手装备发送完成
void USER::GiveNewerItemFinish()
{
	GetMagicItemSetting();
	SendCharData();
	SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	return ;
}
//重置装备级别
void USER::ResetItemLevelBySolt(int sourceSolt)
{
	int i;
	if(sourceSolt < EQUIP_ITEM_NUM || sourceSolt >= TOTAL_INVEN_MAX) 
		return;
	m_UserItem[sourceSolt].sLevel = g_arItemTable[m_UserItem[sourceSolt].sSid]->m_byRLevel;

	//重置级别
	int itemCount;
	if(m_UserItem[sourceSolt].tIQ > NORMAL_ITEM)
	{
		if(m_UserItem[sourceSolt].tIQ == UNIQUE_ITEM) 
			itemCount = MAGIC_NUM;	
		else if(m_UserItem[sourceSolt].tIQ == 12
			||m_UserItem[sourceSolt].tIQ == 18)
			itemCount = 5;								
		else
			itemCount = 4;								
		for(i = 0; i < itemCount; i++)
		{
			BYTE bMagic = m_UserItem[sourceSolt].tMagic[i];
			if(bMagic >= 0 && bMagic < g_arMagicItemTable.GetSize())
			{
				m_UserItem[sourceSolt].sLevel += g_arMagicItemTable[bMagic]->m_tLevel;
			}
		}
	}
}		

//发送NPC提示出现名称
void USER::SendSayAddStr(int npcsay,TCHAR *pStrMsg)
{
	if(pStrMsg == NULL)
		return;
	DWORD dwCuttime = GetTickCount();
	static DWORD dwLastTime = 0;
	if(dwCuttime - dwLastTime<1000)
	{
		return ;
	}
	dwLastTime = dwCuttime;
	CBufferEx TempBuf, TempSayBuf;
	TempSayBuf.AddString( pStrMsg );

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)npcsay);
	TempBuf.Add((BYTE)0x01);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
	return ;
}		

//新职业需要攻击力与体质转成护法生命
void USER::GetSkillSetting()
{
	if(m_byClass != JUDGE)
	{
		return ;
	}
	//仲裁技能
	for (int i=SKILL_JUDGE_QIANGHUAHUFA;i<SKILL_COUNT;i++)
	{
		switch(i)
		{
		case SKILL_JUDGE_QIANGHUAHUFA://强化护法
			{
				break;
			}
		case SKILL_JUDGE_RENMINBUXI://生活不息
			{
				int iLevel = m_UserSkill[i].tLevel;		
				if(iLevel < 0)
					iLevel = 0;
				if(iLevel >= 1) 
					iLevel += m_DynamicUserData[MAGIC_ALL_SKILL_UP];

				if(iLevel >= g_arSkillTable[i]->m_arSuccess.GetSize()) 
					return ;
				if(iLevel >= SKILL_LEVEL) 
					iLevel = SKILL_LEVEL - 1;

				if(i >= g_arSkillTable.GetSize())
					return ;
				int iPos = g_arSkillTable[i]->m_arInc.GetAt(iLevel);
				m_DynamicUserData[MAGIC_MAX_HP_UP]+=m_sCON*iPos/100;
				break;
			}
		}
	}
}

void USER::UserJudgeSkill_ToStr(TCHAR *pBuf)
{
	int	index = 0;
	int i ;
	//另外5个新职业技能
	//0x19,0x1a,0x1b,0x1c,0x18
	for(i = 25;i<SKILL_COUNT;i++)
	{
		SetShort(pBuf, m_UserSkill[i].sSid,		index);
		SetByte (pBuf, m_UserSkill[i].tLevel,	index);
	}
	SetShort(pBuf, m_UserSkill[24].sSid,		index);
	SetByte (pBuf, m_UserSkill[24].tLevel,	index);
}

void USER::User140SkillToStr(TCHAR *pBuf)
{
	int	index = 0;
	int i ;
	//140级技能
	//0x10,0x11,0x12
	for(i = 16;i<19;i++)
    {
		SetShort(pBuf, i,	index);
		//读取出错,必须重置
		if(m_UserSkill_[i].tLevel > 20 || m_sLevel <130)
			m_UserSkill_[i].tLevel = 0;

		SetByte (pBuf, m_UserSkill_[i].tLevel,	index);
	}
}

void USER::AddHuFaStatus(DWORD dwAbnormal)
{
	m_dwJudgePsiState |= dwAbnormal;
}

void USER::DelHuFaStatus(DWORD dwAbnormal)
{

	m_dwJudgePsiState &= (~dwAbnormal);
}

BOOL USER::CheckHuFaStatus(DWORD dwAbnormal)
{
	if((m_dwJudgePsiState & dwAbnormal) == dwAbnormal) 
		return TRUE;
	else
		return FALSE;
}
//是否存在召唤道俱
 BOOL USER::SubInvernItemBySsid(int iSid)
 {
	 int tSlot ;
	 for( tSlot = EQUIP_ITEM_NUM;tSlot<TOTAL_INVEN_MAX;tSlot++)
	 {
		 if(m_UserItem[tSlot].sSid ==0xffff)
			 continue;
		if(m_UserItem[tSlot].sSid == iSid)
		{
			break;
		}
	 }
	 if(tSlot >=TOTAL_INVEN_MAX)
	 {
		 if(iSid<0||iSid>g_arItemTable.GetSize())
			 return FALSE;
		 CString strMsg;
		 strMsg.Format("你需要道具 %s ",g_arItemTable[iSid]->m_strName);
		 SendSystemMsg(strMsg.GetBuffer(0), SYSTEM_NORMAL, TO_ME);
		 return FALSE;
	 }
	 m_UserItem[tSlot].sCount--;
	 if(m_UserItem[tSlot].sCount<=0)
		 ReSetItemSlot(&m_UserItem[tSlot]);

	 CBufferEx TempBuf;
	 TempBuf.Add(ITEM_USE_RESULT);
	 TempBuf.Add(SUCCESS);
	 TempBuf.Add((BYTE)QUICK_ITEM_TELEPORT);
	 TempBuf.Add((BYTE)tSlot);
	 TempBuf.Add(m_UserItem[tSlot].sLevel);
	 TempBuf.Add(m_UserItem[tSlot].sSid);
	 TempBuf.Add(m_UserItem[tSlot].sDuration);
	 TempBuf.Add(m_UserItem[tSlot].sBullNum);
	 TempBuf.Add(m_UserItem[tSlot].sCount);
	 for(int i = 0; i < MAGIC_NUM; i++) 
		 TempBuf.Add(m_UserItem[tSlot].tMagic[i]);
	 TempBuf.Add(m_UserItem[tSlot].tIQ);
	 Send(TempBuf, TempBuf.GetLength());
	 return TRUE;
 }
 BOOL USER::SubInvernItemBySolt(short tSlot)
 {
	 if(tSlot<EQUIP_ITEM_NUM||tSlot>=TOTAL_INVEN_MAX)
		 return FALSE;
	if(m_UserItem[tSlot].sSid ==0xffff)
		return FALSE;
	int iSid = m_UserItem[tSlot].sSid;

	 if(tSlot >=TOTAL_INVEN_MAX)
	 {
		 if(iSid<0||iSid>g_arItemTable.GetSize())
			 return FALSE;
		 CString strMsg;
		 strMsg.Format("你没有道俱 %s ",g_arItemTable[iSid]->m_strName);
		 SendSystemMsg(strMsg.GetBuffer(0), SYSTEM_NORMAL, TO_ME);
		 return FALSE;
	 }
	 m_UserItem[tSlot].sCount--;
	 if(m_UserItem[tSlot].sCount<=0)
		 ReSetItemSlot(&m_UserItem[tSlot]);

	 CBufferEx TempBuf;
	 TempBuf.Add(ITEM_USE_RESULT);
	 TempBuf.Add(SUCCESS);
	 TempBuf.Add((BYTE)QUICK_ITEM_TELEPORT);//使用宠物饲料，此值为116
	 TempBuf.Add((BYTE)tSlot);
	 TempBuf.Add(m_UserItem[tSlot].sLevel);
	 TempBuf.Add(m_UserItem[tSlot].sSid);
	 TempBuf.Add(m_UserItem[tSlot].sDuration);
	 TempBuf.Add(m_UserItem[tSlot].sBullNum);
	 TempBuf.Add(m_UserItem[tSlot].sCount);
	 for(int i = 0; i < MAGIC_NUM; i++) 
		 TempBuf.Add(m_UserItem[tSlot].tMagic[i]);
	 TempBuf.Add(m_UserItem[tSlot].tIQ);
	 Send(TempBuf, TempBuf.GetLength());
	 return TRUE;
 }									

BOOL USER::CallHuFa(BYTE tType)
{
//	static DWORD dw = 1000*60*10,dwLast = 0;

//	if(m_byClass!=JUDGE)
//		return FALSE;
	if(m_dwXiShengTime !=0){
			SendSystemMsg("牺牲护法状态下不能召唤护法,请等待5分钟后召唤.", SYSTEM_ERROR, TO_ME);
			return false;
		}

/*	DWORD dwCur = GetTickCount();
	DWORD dwSpace = dwCur - dwLast;
	if(dwSpace < dw)
	{
		CString str;
		str.Format("您召唤护法还需等待%d分%d秒.",(dw - dwSpace)/60/1000,((dw - dwSpace)/1000)%60);
		SendSystemMsg(str.GetBuffer(0), SYSTEM_ERROR, TO_ME);
		return FALSE;
	}
	dwLast = dwCur;*/
	switch(tType)
	{
	case 1://伊涂罗8000+
		{
			//80级  10010
			//120级 17500
			m_nHuFaMaxHP = 16000+m_sLevel*40+50*m_sMagicWIS;
			//130级技能增加护法血
			//起点17%,每级增加2%点
			if(m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel>0&&m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel<20)
			{
				int nAdd = m_nHuFaMaxHP*(17+2*(m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel-1))/100;
				m_nHuFaMaxHP +=nAdd;
			}
			m_nHuFaHP = m_nHuFaMaxHP;
			break;
		}
	case 2://普雷塔15000+	
		{//120级 31250
			m_nHuFaMaxHP = 30000+m_sLevel*40+50*m_sMagicWIS;
			//130级技能增加护法血
			//起点17%,每级增加2%点
			if(m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel>0&&m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel<20)
			{
				int nAdd = m_nHuFaMaxHP*(17+2*(m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel-1))/100;
				m_nHuFaMaxHP +=nAdd;
			}
			m_nHuFaHP = m_nHuFaMaxHP;
			break;
		}
	case 3://
		{
			m_nHuFaMaxHP = 48000+m_sLevel*40+50*m_sMagicWIS;
			//130级技能增加护法血
			//起点17%,每级增加2%点
			if(m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel>0&&m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel<20)
			{
				int nAdd = m_nHuFaMaxHP*(17+2*(m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel-1))/100;
				m_nHuFaMaxHP +=nAdd;
			}
			m_nHuFaHP = m_nHuFaMaxHP;
			break;
		}
	case 4:
		{
			m_nHuFaMaxHP = 80000+m_sLevel*40+50*m_sMagicWIS;
			//130级技能增加护法血
			//起点17%,每级增加2%点
			if(m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel>0&&m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel<20)
			{
				int nAdd = m_nHuFaMaxHP*(17+2*(m_UserSkill_[SKILL_130_REN_MI_FU_HA].tLevel-1))/100;
				m_nHuFaMaxHP +=nAdd;
			}
			m_nHuFaHP = m_nHuFaMaxHP;
			break;
		}
	case 0:
	default:
		return FALSE;
		break;
	}
	//测试
//#ifdef _DEBUG
//		m_nHuFaMaxHP = 1000;
//		m_nHuFaHP = 1000;	
//#endif
//	m_dwLastCallHuFaTime = dwCur;
	m_dwLevealHuFaTime = 1000*60*60;
	m_tHuFaType = tType;

	CPoint pt = FindNearAvailablePoint_S(m_curx,m_cury);
	if(pt.x == -1||pt.y == -1)
		return FALSE;
	m_xHuFa = pt.x;
	m_yHuFa = pt.y;

	SendNewHufaInfo();
	//CPoint pt = FindNearAvailablePoint_S(m_curx,m_cury);
	//if(pt.x == -1||pt.y == -1)
	//	return ;
	//CPoint ptNew = ConvertToClient( pt.x, pt.y );

	//if( ptNew.x == -1 || ptNew.y == -1 ) 
	//	return;
	//
	//CBufferEx TempBuf;
	//TempBuf.Add(PK_BUTTON_REQ);
	//TempBuf.Add((BYTE)0x43);
	//TempBuf.Add((BYTE)0x01);
	//TempBuf.Add((int)m_uid+USER_BAND+USER_HUFA_BAND);
	//TempBuf.Add((BYTE)m_tHuFaType);
	//TempBuf.Add((int)m_nHuFaMaxHP);
	//TempBuf.Add((int)m_nHuFaHP);
	//TempBuf.Add((BYTE)m_tDir);
	//TempBuf.Add((int)m_curz);
	//TempBuf.Add((int)ptNew.x);
	//TempBuf.Add((int)ptNew.y);
	//Send(TempBuf, TempBuf.GetLength());

#ifdef _USE_Guo_Fu_
	SendHuFaInfo(this,TO_INSIGHT);
#endif
	return TRUE;
}
void USER::SendHuFaInfo(USER *pUser,BYTE towho)
{
	if(pUser ==NULL)
		return ;
	if(pUser->m_tHuFaType == 0)
		return ;
	CPoint pt = FindNearAvailablePoint_S(pUser->m_curx,pUser->m_cury);
	if(pt.x == -1||pt.y == -1)
		return ;

	CPoint ptNew = ConvertToClient( pt.x, pt.y );

	if( ptNew.x == -1 || ptNew.y == -1 ) 
		return;

	CBufferEx TempBuf2;

	TempBuf2.Add(PK_BUTTON_REQ);
	TempBuf2.Add((BYTE)0x44);
	TempBuf2.Add((int)pUser->m_uid+USER_BAND+USER_HUFA_BAND);
	TempBuf2.Add((BYTE)pUser->m_tHuFaType);
	TempBuf2.Add((int)pUser->m_nHuFaHP);
	TempBuf2.Add((int)pUser->m_nHuFaMaxHP);
	TempBuf2.Add((BYTE)pUser->m_tDir);
	TempBuf2.Add((int)pUser->m_curz);
	TempBuf2.Add((int)ptNew.x-1);//护法的坐标,解决挡住角色的BUG
	TempBuf2.Add((int)ptNew.y-2);//护法的坐标,解决挡住角色的BUG
	switch(towho)
	{
	case TO_ALL:
		SendAll(TempBuf2, TempBuf2.GetLength());
		break;

	case TO_ME:
		Send(TempBuf2, TempBuf2.GetLength());
		break;

	case TO_ZONE:
		SendZone(TempBuf2, TempBuf2.GetLength());
		break;

	case TO_INSIGHT:
	default:
		SendInsight(TempBuf2, TempBuf2.GetLength());
		break;
	}
	SendJudgePsiStatus();
}
//发送护法移动消息
void USER::SendHuFaMoveInfo(BYTE bMoveType)
{
	if(m_tHuFaType == 0)
		return ;
	CPoint pt;// = FindNearAvailablePoint_S(m_curx,m_cury);
	pt.x = m_curx+1;
	pt.y = m_cury+2;

	if(pt.x == -1||pt.y == -1)
		return;

	CPoint ptNew = ConvertToClient(pt.x,pt.y );
	if( ptNew.x == -1 || ptNew.y == -1 ) 
	{
		return ;
	}
	BYTE bSuc = 0x01;
	int index = 0;
	SetByte(m_TempBuf, bMoveType, index);
	SetByte(m_TempBuf, bSuc, index);
	SetInt(m_TempBuf, m_uid + USER_BAND + USER_HUFA_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	SendInsight(m_TempBuf, index);
}
//新出生护法信息
void USER::SendNewHufaInfo()
{
	if(m_tHuFaType == 0)
		return ;
	CPoint pt = FindNearAvailablePoint_S(m_curx,m_cury);
	if(pt.x == -1||pt.y == -1)
		return ;
	CPoint ptNew = ConvertToClient( pt.x, pt.y );

	if( ptNew.x == -1 || ptNew.y == -1 ) 
		return;

	CBufferEx TempBuf;
	TempBuf.Add(PK_BUTTON_REQ);
	TempBuf.Add((BYTE)0x43);
	TempBuf.Add((BYTE)0x01);
	TempBuf.Add((int)m_uid+USER_BAND+USER_HUFA_BAND);
	TempBuf.Add((BYTE)m_tHuFaType);
	TempBuf.Add((int)m_nHuFaHP);
	TempBuf.Add((int)m_nHuFaMaxHP);
	TempBuf.Add((BYTE)m_tDir);
	TempBuf.Add((int)m_curz);
	TempBuf.Add((int)ptNew.x);
	TempBuf.Add((int)ptNew.y);

	Send(TempBuf, TempBuf.GetLength());
}

//加入护法发送队列
void USER::AddHuFaDataBuf(USER *pUser,TCHAR *pData,int &nIndex)
{	
	if(pUser == NULL)
		return ;
	if(pUser->m_tHuFaType == 0)
		return ;
	CPoint pt = FindNearAvailablePoint_S(pUser->m_curx,pUser->m_cury);
	if(pt.x == -1||pt.y == -1)
		return ;
	CPoint ptNew = ConvertToClient( pt.x, pt.y );

	if( ptNew.x == -1 || ptNew.y == -1 ) 
		return;

	////0x43
	//SetByte(pData,PK_BUTTON_REQ,nIndex);
	//SetByte(pData,0x43,nIndex);
	//SetByte(pData,0x01,nIndex);
	//SetInt(pData,(int)pUser->m_uid+USER_BAND+USER_HUFA_BAND,nIndex);
	//SetByte(pData,(BYTE)pUser->m_tHuFaType,nIndex);
	//SetInt(pData,(int)pUser->m_nHuFaMaxHP,nIndex);
	//SetInt(pData,(int)pUser->m_nHuFaHP,nIndex);
	//SetByte(pData,(BYTE)pUser->m_tDir,nIndex);
	//SetInt(pData,(int)pUser->m_curz,nIndex);
	//SetInt(pData,(int)ptNew.x,nIndex);
	//SetInt(pData,(int)ptNew.y,nIndex);

	//0x44
	SetByte(pData,PK_BUTTON_REQ,nIndex);
	SetByte(pData,0x44,nIndex);
	SetInt(pData,(int)pUser->m_uid+USER_BAND+USER_HUFA_BAND,nIndex);
	SetByte(pData,(BYTE)pUser->m_tHuFaType,nIndex);
	SetInt(pData,(int)pUser->m_nHuFaHP,nIndex);
	SetInt(pData,(int)pUser->m_nHuFaMaxHP,nIndex);
	SetByte(pData,(BYTE)pUser->m_tDir,nIndex);
	SetInt(pData,(int)pUser->m_curz,nIndex);
	SetInt(pData,(int)ptNew.x,nIndex);
	SetInt(pData,(int)ptNew.y,nIndex);
}

//智力上升
void USER::SendJudgePsiStatus(BYTE bType)
{
	CBufferEx TempBuf2;

	TempBuf2.Add(PK_BUTTON_REQ);
	TempBuf2.Add((BYTE)0x17);
	TempBuf2.Add((int)m_uid+USER_BAND);
	TempBuf2.Add((int)m_dwJudgePsiState);
	TempBuf2.Add((int)m_dwJudgePsiExState);
	switch(bType)
	{
/*	case TO_ME:
		Send(TempBuf2, TempBuf2.GetLength());
	default:
		SendInsight(TempBuf2, TempBuf2.GetLength());
		break;*/

	case TO_ALL:
		SendAll(TempBuf2, TempBuf2.GetLength());
		break;

	case TO_ME:
		Send(TempBuf2, TempBuf2.GetLength());
		break;

	case TO_ZONE:
		SendZone(TempBuf2, TempBuf2.GetLength());
		break;

	case TO_INSIGHT:
	default:
		SendInsight(TempBuf2, TempBuf2.GetLength());
		break;

	}
}

//更新护法血量
//0x4c,0x5c,0x44,0x00,0x00,0x5c,0x44,0x00,0x00,
void USER::SendHuFaHPInfo()
{
	CBufferEx TempBuf2;
	TempBuf2.Add(PK_BUTTON_REQ);
	TempBuf2.Add((BYTE)0x4c);
	TempBuf2.Add(m_nHuFaHP);
	TempBuf2.Add(m_nHuFaMaxHP);
	SendInsight(TempBuf2, TempBuf2.GetLength());
}													

//护法消失
//0x3e,0x45,0xc8,0x79,0x00,0x00,
void USER::SendHuFaPostion(int nType)
{
	CBufferEx TempBuf2;
	TempBuf2.Add(PK_BUTTON_REQ);
	TempBuf2.Add((BYTE)0x45);
	TempBuf2.Add((int)m_uid+USER_BAND+USER_HUFA_BAND);
	SendInsight(TempBuf2, TempBuf2.GetLength());
	if(nType)
	{
		m_tHuFaType = 0;
		m_nHuFaHP = 0;
		m_nHuFaMaxHP =0 ;
		//SendSystemMsg("召唤的护法已经死亡",SYSTEM_NORMAL,TO_ME);
	}
}
//设置护法伤害
void USER::SetHuFaDamage(int nDamage)
{
	if(nDamage <= 0) 
		return;
	if(m_bLive == USER_DEAD)
		return;
	//用户掉血显示
	SendDamageNum(0,m_uid+USER_BAND+USER_HUFA_BAND,nDamage);	
	m_nHuFaHP -= nDamage;
	if( m_nHuFaHP <= 0 )
	{
		m_nHuFaHP = 0;
		SendHuFaPostion(HUFA_DEA);
	}
	else
	{
		SendNpcHP(m_uid+USER_BAND+USER_HUFA_BAND,m_nHuFaHP);
		SendHuFaHPInfo();	
	}
}		
//获取护法伤害攻击值
int  USER::GetHuFaDamage()										
{
	int nBase = 15;
	switch(m_tHuFaType)
	{
	case 0x01:
		nBase+=80;
		break;
	case 0x02:
		nBase+=160;
		break;
	case 0x03:
		nBase+=320;
		break;
	case 0x04:
		nBase+=480;
		break;
	}
	
	//强化护法,智力变成护法攻击力
	int iLevel = m_UserSkill[SKILL_JUDGE_QIANGHUAHUFA].tLevel;		
	if(iLevel < 0)
		iLevel = 0;
	if(iLevel >= 1) 
		iLevel += m_DynamicUserData[MAGIC_ALL_SKILL_UP];

	if(iLevel >= g_arSkillTable[SKILL_JUDGE_QIANGHUAHUFA]->m_arSuccess.GetSize()) 
		return nBase;
	if(iLevel >= SKILL_LEVEL) 
		iLevel = SKILL_LEVEL - 1;

	if(SKILL_JUDGE_QIANGHUAHUFA >= g_arSkillTable.GetSize())
		return nBase;
	int iPos = g_arSkillTable[SKILL_JUDGE_QIANGHUAHUFA]->m_arInc.GetAt(iLevel);
	nBase+=m_sWIS*iPos/100;
	return nBase;
}
//获取反击攻击
int  USER::HuFaAttack(int npcid)	
{
	int sHP = 0,sMaxHP = 0;
	DWORD dwCur = GetTickCount();
	//护法攻击2秒一次
	if(dwCur - m_dwHuFaLastAttack<2000)
		return 0;
	m_dwHuFaLastAttack = dwCur;
	//护法攻击
	if(m_tHuFaType !=0&&m_nHuFaHP>0)
	{
		int nDamage = GetHuFaDamage();
		if(nDamage<15)
			nDamage = 15;
		//护法攻击目标去血
		if(npcid >= USER_BAND && npcid < NPC_BAND)
		{
			USER *pUser = GetUser(npcid-USER_BAND);
			if(pUser)
			{
				pUser->SetDamage(nDamage);
				sHP = pUser->m_sHP;
				sMaxHP = pUser->m_sMaxHP;
			}
			//用户掉血显示
			SendDamageNum(0,npcid,nDamage);	
		}else
		{
			CNpc *pNpc = GetNpc(npcid-NPC_BAND);
			if(pNpc)
			{
				pNpc->SetHuFaFinalDamage(this,nDamage);
				sHP = pNpc->m_sHP;
				sMaxHP = pNpc->m_sMaxHP;
			}
			//npc掉血
			//SendDamageNum(1,npcid,nDamage);	

			SendNpcHP(npcid,sHP);
		}
		CBufferEx TempBuf2;

		TempBuf2.Add(ATTACK_RESULT);//国服已经修正

		TempBuf2.Add(ATTACK_SUCCESS);

		TempBuf2.Add((int)(m_uid + USER_BAND+USER_HUFA_BAND));
		TempBuf2.Add(npcid);
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
	return 0;
}
//群攻伤害
void USER::SendWideRangeAttack(int x, int y,int psi,int damage)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return;

	int dir[9][2];
	int ix, iy;
	int nTarget = 0;
	int nDamage = 0;

	USER* pUser = NULL;
	CNpc* pNpc = NULL;
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
			pUser = GetUser(nTarget - USER_BAND);			// User Pointer 甫 掘绰促.
			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;	
			if(pUser->m_bLive == USER_DEAD || pUser->m_uid == m_uid)	continue;		// Target User 啊 捞固 磷绢乐栏搁 府畔
			if(!pUser->m_bPkStatus) continue;				// 老矫利 墨坷啊 酒聪搁 傍拜捞 救等促.

			if(m_dwGuild > 0)
			{
				if(m_tGuildWar == GUILD_WARRING && pUser->m_tGuildWar == GUILD_WARRING)
				{												
					if(pUser->m_dwGuild == m_dwGuild)  return;
				}

				if(m_tFortressWar == GUILD_WARRING && pUser->m_tFortressWar == GUILD_WARRING)
				{												// 辨靛傈老锭 老矫利 墨坷绰 鞍篮辨靛盔俊霸绰 公矫
					if(pUser->m_dwGuild == m_dwGuild)  return;
				}																			
			}

			nDamage = (int)((double)damage *  ((double)m_sMagicVOL / (m_sMagicVOL + pUser->m_sMagicVOL + pUser->m_DynamicUserData[MAGIC_PSI_RESIST_UP] + \
				(int)((double)pUser->m_DynamicEBodyData[EBODY_PSI_RESIST_UP] / 100) \
				)));
			nDamage = (int)((double)nDamage/2 + 0.5);	// 单固瘤狼 50%父 甸绢埃促.

			pUser->SetDamage(nDamage);
			if(pUser->m_sHP > 0)		// 混篮 版快 傈扁单固瘤 眠啊
			{
				//				pUser->SetColdDamage();
			}
			else if(pUser->m_lDeadUsed == 1)
			{
				int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
				IsChangeCityRank(m_sCityRank, pUser);
				pUser->GetLevelDownExp(USER_PK, tempRank, FALSE,m_strUserID);			// 版氰摹客 弊寇 函拳樊甫 馆康茄促.
			}
		}
		else if(nTarget >= NPC_BAND)				// NPC
		{
			pNpc = GetNpc(nTarget - NPC_BAND);				// NPC Point 甫 掘绰促.
			if(pNpc == NULL) continue;					// 肋给等 NPC 捞搁 府畔
			if(pNpc->m_NpcState == NPC_DEAD || pNpc->m_tNpcType != NPCTYPE_MONSTER) continue;	// NPC 啊 捞固 磷绢 乐栏搁 府畔
			if(pNpc->m_sHP <= 0) continue;
			if(pNpc->m_byAX == 0 && pNpc->m_byAZ == 0 && pNpc->m_tNpcType == 0) continue;			// 傍拜瓷仿捞 绝绰 阁胶磐(泅犁 盔籍)篮 老馆 傍拜捞 登瘤 臼绰促.

			nDamage = (int)(damage *  ((double)m_sMagicVOL / (m_sMagicVOL + pNpc->m_sVOL)));
			nDamage = (int)((double)nDamage/2 + 0.5);	// 单固瘤狼 50%父 甸绢埃促.

			//			if(pNpc->SetDamage(nDamage, m_strUserID, m_uid + USER_BAND, m_pCom) == FALSE)
			if(pNpc->SetDamage(nDamage, m_uid + USER_BAND, m_pCom) == FALSE)
			{
				if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
				{
					CheckGuildHouseWarEnd();
				}

				pNpc->SendExpToUserList(m_pCom); // 版氰摹 盒硅!!
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(diffLevel <= o_yehuoini[0]->djxz || pNpc->m_sEvent >= 30000 )
				{
					if (m_sLevel>= 70)
							m_dwShopPingDN += 1;//每杀死怪物增加杀怪物数
					if (m_isDoubleBAOLV  > 0 || g_sanBaoLv == TRUE || m_dwGuarDianTianShi > 0)
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

				if(diffLevel < 40)
				{
					m_iCityValue=m_iCityValue+250;///市民等级及时刷新
					GetCityRank();
					CheckMaxValue(m_dwXP, 1);		
					SendXP();
					SendUserStatusSkill();					
				}
			}
			else									// 混篮 版快 傈扁单固瘤 眠啊
			{
				//				pNpc->SetColdDamage();
			}
			SendDamageNum(1,nTarget,nDamage);
			SendNpcHP(nTarget,pNpc->m_sHP);	
			
		}
	}
	return ;
}


//领取充值结果为物品类
/*void USER::OnlineExchange(int nErrorSay,int nSucSay)
{
	DWORD dwCuttime = GetTickCount();
	static DWORD dwLastTime = 0;
	if(dwCuttime - dwLastTime<5000)
	{
		return ;
	}
	dwLastTime = dwCuttime;
	//获取是否存在未兑换的条目
	//兑换编码
	int exCount = 0;
	typedef struct _Ex_Code_Array_
	{
		int nSID;
		int nrmbCode;
	}Ex_Code_Array;
	Ex_Code_Array exCodeArray[10] = {{0,0}};
	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	int	nDBIndex;
	int	i;
	::ZeroMemory( szSQL, sizeof(szSQL) );

	nDBIndex = 0;

	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) 
		return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS ) 
		return;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "select sid,rmbpoint from rmbshop where struserid='%s' and state=0" ),m_strUserID );
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		SQLINTEGER		iSID;
		SQLINTEGER		nCode;
		SQLINTEGER		ivd;

		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &ivd );
				SQLGetData( hstmt, i++, SQL_C_SLONG, &nCode, sizeof(nCode), &ivd );
                //最多只兑换十项
				exCodeArray[exCount].nSID = iSID;
				exCodeArray[exCount].nrmbCode = nCode;

				if(exCount++>=10)
					break;
			}
			else break;
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

	//判断更新条目是否合法
	if(exCount<=0)
	{
		SendNpcSay(0,nErrorSay);
		return ;
	}
	int nRmbCount = g_arOnlineRMBShopTable.GetSize();
	int nPos=0;
	for(i = 0;i<exCount;i++)
	{
		//判断能够兑换
		for( nPos = 0;nPos<nRmbCount;nPos++)
		{
			if(exCodeArray[i].nrmbCode == g_arOnlineRMBShopTable[nPos]->m_rmbCode)
			{
				break;
			}
		}
		if(nPos>=nRmbCount)
			return ;
		//更新兑换条目
		::ZeroMemory( szSQL, sizeof(szSQL) );
		nDBIndex = 0;
		pDB = g_DB[m_iModSid].GetDB( nDBIndex );
		if( !pDB ) 
			return;

		retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
		if( retcode != SQL_SUCCESS ) 
			return;

		_sntprintf(szSQL, sizeof(szSQL), TEXT( "update  rmbshop set state=1 where struserid='%s' and sid=%d" ),m_strUserID,exCodeArray[i].nSID );
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

		//兑换物品成功
		GiveAllItem(g_arOnlineRMBShopTable[nPos]->m_iSid,g_arOnlineRMBShopTable[nPos]->m_iNum,g_arOnlineRMBShopTable[nPos]->m_upgrade,
			g_arOnlineRMBShopTable[nPos]->m_tIQ,g_arOnlineRMBShopTable[nPos]->m_sx1,g_arOnlineRMBShopTable[nPos]->m_sx2,
			g_arOnlineRMBShopTable[nPos]->m_sx3,g_arOnlineRMBShopTable[nPos]->m_sx4);
       
		CString strMsg1 = _T("");
		strMsg1.Format( "你成功领取了 %s ",g_arOnlineRMBShopTable[nPos]->m_iText);
		CBufferEx	TempBuf,TempBuf1;
		TempBuf.Add((byte)CHAT_RESULT);
		TempBuf.Add((byte)1);
		TempBuf.Add(WHISPER_CHAT);
		TempBuf.Add((int)0x01);
		TempBuf.Add("GM", _tcslen("GM"));
		TempBuf.Add(strMsg1.GetBuffer(0), strMsg1.GetLength());
		Send(TempBuf, TempBuf.GetLength());
	}
	return ;
}*/
//领取充值结果为决战币/元宝...
void USER::OnlineExchange(int nErrorSay,int nSucSay)
{
	DWORD dwCuttime = GetTickCount();
	static DWORD dwLastTime = 0;
	CString msg;
	if(dwCuttime - dwLastTime<5000)
	{
		return ;
	}
	dwLastTime = dwCuttime;
	//获取是否存在未兑换的条目
	//兑换编码
	int exCount = 0;
	typedef struct _Ex_Code_Array_
	{
		int nSID;
		int nrmbPoint;
	}Ex_Code_Array;
	Ex_Code_Array exCodeArray[10] = {{0,0}};
	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	int	nDBIndex;
	int	i;
	::ZeroMemory( szSQL, sizeof(szSQL) );

	nDBIndex = 0;

	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) 
		return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS ) 
		return;
   //角色领取
	_sntprintf(szSQL, sizeof(szSQL), TEXT( "select sid,rmbpoint from rmbshop where struserid='%s' and state=0" ),m_strUserID);
    //帐户领取
//	_sntprintf(szSQL, sizeof(szSQL), TEXT( "select sid,rmbpoint from rmbshop where struserid='%s' and state=0" ),m_strAccount);
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		SQLINTEGER		iSID;
		SQLINTEGER		nCode;
		SQLINTEGER		ivd;

		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &ivd );
				SQLGetData( hstmt, i++, SQL_C_SLONG, &nCode, sizeof(nCode), &ivd );

				//最多只兑换十项
				exCodeArray[exCount].nSID = iSID;
				exCodeArray[exCount].nrmbPoint = nCode;

				if(exCount++>=10)
					break;
			}
			else break;
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

	//判断更新条目是否合法
	if(exCount<=0)
	{
		SendNpcSay(0,nErrorSay);
		return ;
	}
	int PointCount = 0;
	int nPos=0;
	for(i = 0;i<exCount;i++)
	{
		//更新兑换条目
		::ZeroMemory( szSQL, sizeof(szSQL) );
		nDBIndex = 0;
		pDB = g_DB[m_iModSid].GetDB( nDBIndex );
		if( !pDB ) 
			return;

		retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
		if( retcode != SQL_SUCCESS ) 
			return;
        //角色领取
		_sntprintf(szSQL, sizeof(szSQL), TEXT( "update  rmbshop set state=1 where struserid='%s' and sid=%d" ),m_strUserID,exCodeArray[i].nSID );
		//帐户领取
	//	_sntprintf(szSQL, sizeof(szSQL), TEXT( "update  rmbshop set state=1 where struserid='%s' and sid=%d" ),m_strAccount,exCodeArray[i].nSID );
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

		PointCount+=exCodeArray[i].nrmbPoint;
		//CString strMsg1 = _T("");
		//strMsg1.Format( "你成功兑换了%d",exCodeArray[i].nrmbPoint);
		//CBufferEx	TempBuf,TempBuf1;
		//TempBuf.Add((byte)CHAT_RESULT);
		//TempBuf.Add((byte)1);
		//TempBuf.Add(WHISPER_CHAT);
		//TempBuf.Add((int)0x01);
		//TempBuf.Add("充值", _tcslen("充值"));
		//TempBuf.Add(strMsg1.GetBuffer(0), strMsg1.GetLength());
		//Send(TempBuf, TempBuf.GetLength());
	}
	TCHAR szCount[40]="";
	TCHAR szVal[40]="";
	wsprintf(szCount,"%d",exCount);
	wsprintf(szVal,"%d",PointCount);
//	GiveMoney(PointCount);
	GiveItemAll(724,PointCount,0,0,0,0,0,0,0,0,0,0,0);
//	GiveShopPingDN(PointCount);//领取到元宝
	SendUserStatusSkill();//显示到属性元宝数量
//	GiveDN(PointCount);//领取到JZB
//	SendMoneyChanged();
//	UpdateUserItemDN();
	msg.Format("恭喜【%s】充值成功!获得[%d]标志",m_strUserID,PointCount);
			SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_ANNOUNCE, TO_ALL);	
	SendBoCaiAddStr(nSucSay,szCount,szVal,"");

	TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));//帐户领取时需要存储领取记录
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf(strOP,"%d-%d-%d %d:%d分 领取了帐户%s充值",time.wYear,time.wMonth,time.wDay ,time.wHour,time.wMinute,m_strAccount);
	WriteOpratorLog(strOP,CHAT_LOG);

	//CString strMsg1 = _T("");
	//strMsg1.Format( "你成功兑换了%d",exCodeArray[i].nrmbPoint);
	//CBufferEx	TempBuf,TempBuf1;
	//TempBuf.Add((byte)CHAT_RESULT);
	//TempBuf.Add((byte)1);
	//TempBuf.Add(WHISPER_CHAT);
	//TempBuf.Add((int)0x01);
	//TempBuf.Add("充值", _tcslen("充值"));
	//TempBuf.Add(strMsg1.GetBuffer(0), strMsg1.GetLength());
	//Send(TempBuf, TempBuf.GetLength());

	return ;
} 
void USER::SendBoCaiAddStr(int npcsay,TCHAR *pBase,TCHAR *szResult,TCHAR *szLoster)
{
	CBufferEx TempBuf, TempSayBuf;
	TempSayBuf.AddString( pBase );
	TempSayBuf.AddString( szResult );
	TempSayBuf.AddString( szLoster );

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)npcsay);
	TempBuf.Add((BYTE)0x03);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
	return ;
}	
void USER::ChangeAttributeItem(int srcSid,int srcCount,int srcIQ,int src1,int src2,int src3,int src4,
							   int dstSid,int dstCount,int dstIQ,int dst1,int dst2,int dst3,int dst4,int dst5)
{
	//删除物品
	if(srcSid < 0 || srcSid >= g_arItemTable.GetSize())
		return;
	//删除物品
	if(dstSid < 0 || dstSid >= g_arItemTable.GetSize())
		return;
	if(srcCount<=0 || dstCount<=0)
		return ;
	int n,i,j;
	int nCount= 0,nSolt[24] = {0};
	BOOL bTogether  = FALSE;
	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == srcSid)  //id相同
		{
			//属性相同
			if(m_UserItem[i].tMagic[0] == src1
				&&m_UserItem[i].tMagic[1] == src2
				&&m_UserItem[i].tMagic[2] == src3
				&&m_UserItem[i].tMagic[3] == src4
				&&m_UserItem[i].tIQ == dstIQ)
			{ 
				nSolt[nCount++] = i;
				if(m_UserItem[i].sCount > 1)
				{
					bTogether = TRUE;
					nCount = m_UserItem[i].sCount;
					break;
				}
			}
		}
	}
	//判断数量是否对
	if(nCount < srcCount)
		return ;
	if(m_dwDN < 1000000)
	{
	//	SendSystemMsg("钱不够!给白锆注入属性需花费100万!", SYSTEM_ERROR, TO_ME);
        SendEventMsg("钱不够!给白锆注入属性需花费100万!");
		return ;		// 傍拜侩诀弊饭捞靛厚侩焊促累栏搁
	}
	if( m_dwDN <= 1000000 )
		m_dwDN = 0;
	else
		m_dwDN = m_dwDN - 1000000;
	//打包物品
	if(bTogether)
	{
		int iSlot = nSolt[0];
		CBufferEx TempBuf;
		ItemList	TempItem;
		ReSetItemSlot(&TempItem);

		TempItem.sSid		= g_arItemTable[srcSid]->m_sSid;
		TempItem.sBullNum	= g_arItemTable[srcSid]->m_sBullNum;
		TempItem.sDuration	= g_arItemTable[srcSid]->m_sDuration;
		for(i =0; i < MAGIC_NUM; i++) 
			TempItem.tMagic[i] = 0;
		TempItem.tIQ = NORMAL_ITEM;


		if((m_UserItem[iSlot].sCount - srcCount) <= 0)				// 促澜郴牢亥甫哗霖促.		
		{														// 狞酒捞袍函拳啊乐栏搁		
			ReSetItemSlot(&m_UserItem[iSlot]);
		}
		else
		{
			m_UserItem[iSlot].sCount -= srcCount;
		}

		int iWeight = srcCount * g_arItemTable[srcSid]->m_byWeight;

		m_iCurWeight -= iWeight;
		if(m_iCurWeight < 0) 
			m_iCurWeight = 0;


		TempBuf.Add(ITEM_GIVE_RESULT);
		TempBuf.Add((BYTE)0x01);
		TempBuf.Add((BYTE)iSlot);
		TempBuf.Add(m_UserItem[iSlot].sLevel);
		TempBuf.Add(m_UserItem[iSlot].sSid);
		TempBuf.Add(m_UserItem[iSlot].sDuration);
		TempBuf.Add(m_UserItem[iSlot].sBullNum);
		TempBuf.Add(m_UserItem[iSlot].sCount);
		for(j = 0; j < MAGIC_NUM; j++) 
			TempBuf.Add(m_UserItem[iSlot].tMagic[j]);

		TempBuf.Add(m_UserItem[iSlot].tIQ);

		Send(TempBuf, TempBuf.GetLength());
	}else
	{
		for(n = 0;n<srcCount;n++)
		{
			int iSlot = nSolt[n];
			CBufferEx TempBuf;
			ItemList	TempItem;
			ReSetItemSlot(&TempItem);

			TempItem.sSid		= g_arItemTable[srcSid]->m_sSid;
			TempItem.sBullNum	= g_arItemTable[srcSid]->m_sBullNum;
			TempItem.sDuration	= g_arItemTable[srcSid]->m_sDuration;
			for(i =0; i < MAGIC_NUM; i++) 
				TempItem.tMagic[i] = 0;
			TempItem.tIQ = NORMAL_ITEM;

			if((m_UserItem[iSlot].sCount - 1) <= 0)				// 促澜郴牢亥甫哗霖促.		
			{														// 狞酒捞袍函拳啊乐栏搁		
				ReSetItemSlot(&m_UserItem[iSlot]);
			}
			else m_UserItem[iSlot].sCount -= srcCount;

			int iWeight = srcCount * g_arItemTable[srcSid]->m_byWeight;

			m_iCurWeight -= iWeight;
			if(m_iCurWeight < 0) 
				m_iCurWeight = 0;


			TempBuf.Add(ITEM_GIVE_RESULT);
			TempBuf.Add((BYTE)0x01);
			TempBuf.Add((BYTE)iSlot);
			TempBuf.Add(m_UserItem[iSlot].sLevel);
			TempBuf.Add(m_UserItem[iSlot].sSid);
			TempBuf.Add(m_UserItem[iSlot].sDuration);
			TempBuf.Add(m_UserItem[iSlot].sBullNum);
			TempBuf.Add(m_UserItem[iSlot].sCount);
			for(j = 0; j < MAGIC_NUM; j++) 
				TempBuf.Add(m_UserItem[iSlot].tMagic[j]);

			TempBuf.Add(m_UserItem[iSlot].tIQ);

			Send(TempBuf, TempBuf.GetLength());
		}
	}
	//给物品
	GiveAllItem(dstSid,dstCount,0,dstIQ,dst1,dst2,dst3,dst4,0);

	SendMoneyChanged();
	SendSystemMsg("注入成功", SYSTEM_ERROR, TO_ME);
}
/////////////////////////////////////////////////////////////////////////////////
//群攻技能---
int USER::GetQunGongAttack(int x, int y, int dDamage)	
{
	int sx = m_curx / 10;		//范围
	int sy = m_cury / 10;

	int min_x = (sx-1)*10; if( min_x < 0 ) min_x = 0;
	int max_x = (sx+2)*10;
	int min_y = (sy-1)*10; if( min_y < 0 ) min_y = 0;
	int max_y = (sy+2)*10;
	int zzzz=m_curz;
	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return -1;

	if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
	if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;

	int nTarget;
	int num = 0; //标识有多少个怪物

	CNpc* pNpc = NULL;
	int radom = myrand(0,0);
	dDamage = dDamage + radom; //随机增加攻击
	for( int i = min_x; i < max_x; i++ )
	{
		for( int j = min_y; j < max_y; j++ )
		{				
			nTarget = pMap->m_pMap[i][j].m_lUser;
			if(nTarget >= NPC_BAND)
			{

				pNpc = GetNpc(nTarget - NPC_BAND);
				if(pNpc->m_byType == 0)
				{
					num++; 
					switch(m_byClass)		//判断职业
					{
					case 0:
						SendPsiAttackResult(SUCCESS, nTarget, (BYTE)212);   //拳师使用35编号效果
						break;
					case 1:
						SendPsiAttackResult(SUCCESS, nTarget, (BYTE)8);   //法师使用122编号效果
						break;
					case 2:
						SendPsiAttackResult(SUCCESS, nTarget, (BYTE)36);   //剑师使用36编号效果
						break;
					case 3:
						SendPsiAttackResult(SUCCESS, nTarget, (BYTE)190);   //枪师使用190编号效果
						break;
					case 4:
						SendPsiAttackResult(SUCCESS, nTarget, (BYTE)41);   //仲裁使用122编号效果
						break;
					}
					short sOldNpcHP = pNpc->m_sHP;

					if(pNpc->SetDamage(dDamage, m_uid + USER_BAND, m_pCom) == FALSE)
					{
						pNpc->SendExpToUserList(m_pCom); 
						int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel); //怪物等级相差30级没经验
						if(diffLevel <= o_yehuoini[0]->djxz || pNpc->m_sEvent >= 30000 )
						{
							if (m_sLevel>= 70)
								m_dwShopPingDN += 1;//每杀死怪物增加杀怪物数
							if (m_isDoubleBAOLV  > 0 || g_sanBaoLv == TRUE || m_dwGuarDianTianShi > 0)
								
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
							
							if(diffLevel < 40)
							{
								CheckMaxValue(m_dwXP, 1);		
								SendXP();			//发送经验
							}
						}
					}
				    SendDamageNum(1,nTarget,(short)dDamage);	//发送伤害值
					SendNpcHP(nTarget,pNpc->m_sHP);				//发送血量
 
				}//if
			}
		}
	}//for
	return num;
}
//宠物信息
void USER::GetBabySetting()										
{
	if(m_tBabyCall !=1)
		return ;
	if(m_sBabyLevel>20)
		return ;
	switch(m_sBabyID)
	{
	case 256://龙
	case 512:
	case 768:
		{
			int attDown = m_sBabyLevel*3;
			if(attDown>60)
				attDown = 60;
			m_DynamicUserData[MAGIC_FINALLY_ATTACK_UP]+= attDown;
		//	m_DynamicMagicItem[5]+= attDown;
			break;
		}
	case 257://狮
	case 513:
	case 769:
		{
			int hpAdd = m_sBabyLevel*15;
			if(hpAdd>300)
				hpAdd = 300;
			m_DynamicUserData[MAGIC_MAX_HP_UP]+= hpAdd;
			break; 
		}
	default:
		return ;
	}
	//技能
	/*
	1 基本技能喂养 
	2 1阶段技能 生命恢复 
	3 1阶段技能 最终伤害减少10 
	4 2阶段技能 药水变换全系列
	5 2阶段技能 锡片变换全系列
	6 2阶段技能 致命之聪格斗家
	7 2阶段技能 生命之光格半家
	8 2阶段技能 超级罡气
	9 2阶段技能 持续魔法师
	10 2阶段技能 生命之光 魔法师
	11 2阶段技能 灵敏之慧 魔法师
	12 2阶段技能 生命之光 剑武士
	13 2阶段技能 抗斥 剑武士
	14 2阶段技能 保护罡气 剑武士
	15 2阶段技能 生命之光 枪械师
	16 2阶段技能 抗斥 枪械师
	17 2阶段技能 灵敏之慧 枪械师
	18 3阶段技能 所有能力值增加3
	19 3阶段技能 所有技能增加1
	*/
	for(int i = 1;i<4;i++)        /////宠物属性
	{
		if(m_tBabySkill[i] == 0)
			continue;
		switch(m_tBabySkill[i])
		{
			case 3:
			//	m_DynamicMagicItem[5]+= 10;
				m_DynamicUserData[MAGIC_FINALLY_ATTACK_DOWN]+= 10;
				break;
			/*case 6://致命之聪格斗家
				m_UserSkill_[0].tLevel++;
				break;
			case 7://生命之光格半家
				m_UserSkill_[1].tLevel++;
				break;
			case 8://超级罡气
				m_UserSkill_[2].tLevel++;
				break;
			case 9://持续魔法师
				m_UserSkill_[3].tLevel++;
				break;
			case 10://生命之光 魔法师
				m_UserSkill_[4].tLevel++;
				break;	
			case 11://灵敏之慧 魔法师
				m_UserSkill_[5].tLevel++;
				break;
			case 12://生命之光 剑武士
				m_UserSkill_[6].tLevel++;
				break;
			case 13://抗斥 剑武士
				//m_UserSkill_[6].tLevel++;
				break;
			case 14://保护罡气 剑武士
				m_UserSkill_[8].tLevel++;
				break;
			case 15://生命之光 枪械师
				m_UserSkill_[9].tLevel++;
				break;
			case 16://抗斥 枪械师
				m_UserSkill_[10].tLevel++;
				break;
			case 17://灵敏之慧 枪械师
				m_UserSkill_[11].tLevel++;
				break;*/
			case 18:
				AddMagicPoint(3);
				break;
			case 19:
				m_DynamicUserData[MAGIC_ALL_SKILL_UP]+= 1;
				break;
		}
	}
}
BOOL USER::UserBabyCall(int nSolt)
{
	if(m_tBabyCall ==1)
		return FALSE;
	//100级才可以召唤
	if(m_sLevel<120)
	{
		SendEventMsg("120级以上才可召唤宠物!");
		return FALSE;
	}
	if(!SubInvernItemBySsid(1490))
		return FALSE;
	int nRand = myrand(1,1000);
	m_tBabyCall = 1;
	if(nRand<500)
	{
		m_sBabyID = 256;
	}else
	{
		m_sBabyID = 257;
	}
	m_sBabyLevel = 1;
	m_sBabyNormal = 0;
	m_nBabyExp = 1000;//0;//宠当前经验值
	m_nBabyNextLevelExp = 1000;//宠升级经验值
	m_sFealty = 100;
	m_sBabyStatus = 512;
	m_sMaxHungry = 512;
	m_sHungry = 512;
	m_tBabySkill[0] = 1;
	switch(m_byClass)
	{
	case 0:
		m_tBabySkill[1] = 10;
		break;
	case 1:
		m_tBabySkill[1] = 7;
		break;
	case 2:
		m_tBabySkill[1] = 12;
		break;
	case 3:
		m_tBabySkill[1] = 15;
		break;
	case 4:
		m_tBabySkill[1] = 0;
		break;
	default:
		m_tBabySkill[1] = 0;
		break;
	}
	m_tBabySkill[1] = 3;
	m_tBabySkill[2] = 0;
	m_tBabySkill[3] = 0;
	//m_tBabySkill[2] = 18;
	//m_tBabySkill[3] = 19;
	SendMyInfo(TO_INSIGHT,INFO_MODIFY);

	SendBabyInfo();
	return TRUE;
}
void USER::UserKillBaby(int nDN,int nSucSay,int nErrSay)
{
	if(m_tBabyCall==0)
	{
		CString str;
		str.Format("你没有宠物可卖!");
		SendSayAddStr(nErrSay,str.GetBuffer(0));
		return ;
	}
	m_tBabyCall = 0;
	m_sBabyNormal = 0;
	m_sBabyLevel= 0;
	m_sBabyID = 0;
	m_sFealty = 0;
	m_sHungry = 0;
	m_sMaxHungry = 0;
	m_sBabyStatus = 0;
	m_tBabySkill[0] = 0;
	m_tBabySkill[1] = 0;
	m_tBabySkill[2] = 0;
	m_tBabySkill[3] = 0;

	SendMyInfo(TO_INSIGHT,INFO_MODIFY);
	CString str;
	str.Format("玩家[ %s ]将自己的宠物卖给雪魂鉴定中心【梅兹】获得[ %d ]决战币!",m_strUserID,nDN);
	SendSystemMsg(str.GetBuffer(0),SYSTEM_ANNOUNCE,TO_ALL);
	
	str.Format("%d",nDN);
	SendSayAddStr(nSucSay,str.GetBuffer(0));

	if(nDN<=0)
		return ;
	GiveDN(nDN);//给金币
//	GiveShopPingDN(nDN);//给元宝
	//SendUserStatusSkill();	//更新元宝数量.
	SendMoneyChanged();
	UpdateUserItemDN();

}
void USER::SendBabyInfo()
{
/*
	0x3e,0x20,0x01,0x00,0x13,0x01,0x00,0x00,0x01,
	0x04,0xb3,0xe8,0xce,0xef,//babyname
	0x04,0x66,0x64,0x73,0x66,//username
	0x00,0x00,				//baby level;
	0x01,0x00,0x00,0x00,
	0x0a,0x00,0x00,0x00,
	0x01,0x02,//babyid
	0x50,0x00,//忠诚度
	0x00,0x00,
	0x00,0x02,
	0xf4,0x01,
	0xd0,0x2a,0x4e,0x4d,
	0x00,0x00,0x00,0x00,//技能
*/
	if(m_sBabyID <= 0 )
		return ;
	CBufferEx TempBuf2;
	TempBuf2.Add(PK_BUTTON_REQ);
	TempBuf2.Add((BYTE)0x20);
	TempBuf2.Add((BYTE)0x01);
	TempBuf2.Add((BYTE)0x00);
	TempBuf2.Add((BYTE)0x13);
	TempBuf2.Add((BYTE)0x01);
	TempBuf2.Add((BYTE)0x00);
	TempBuf2.Add((BYTE)0x00);
	TempBuf2.Add((BYTE)0x01);
	TempBuf2.Add(g_szBabyName[m_sBabyID%2],strlen(g_szBabyName[m_sBabyID%2]));
	TempBuf2.Add(m_strUserID,strlen(m_strUserID));
	TempBuf2.Add((short)m_sBabyLevel);			//baby level 龙1级加3点最终伤害,狮子1级增加15点血
	TempBuf2.Add((int)m_nBabyExp);				//cur baby exp
	TempBuf2.Add((int)m_nBabyNextLevelExp);		//cur next level exp
	TempBuf2.Add((short)m_sBabyID);				//baby ssid
	TempBuf2.Add((short)m_sFealty);				//忠诚度
	TempBuf2.Add((short)m_sBabyStatus);			//状态
	TempBuf2.Add((short)m_sMaxHungry);			//饱值状态临界
	TempBuf2.Add((short)m_sHungry);				//饥饿值
	//
	SYSTEMTIME systime;
	CTime cTime(2012,12,12,0,0,0);
	cTime.GetAsSystemTime(systime);//转成systime
	double dtime;
	SystemTimeToVariantTime(&systime,&dtime);

	TempBuf2.Add((BYTE)0xe0);
	TempBuf2.Add((BYTE)0x24);
	TempBuf2.Add((BYTE)0xe4);
	TempBuf2.Add((BYTE)0x40);
	//技能
	/*
	1 基本技能喂养 
	2 1阶段技能 生命恢复 
	3 1阶段技能 最终伤害减少10 
	4 2阶段技能 药水变换全系列
	5 2阶段技能 锡片变换全系列
	6 2阶段技能 致命之聪格斗家
	7 2阶段技能 生命之光格半家
	8 2阶段技能 超级罡气
	9 2阶段技能 持续魔法师
	10 2阶段技能 生命之光 魔法师
	11 2阶段技能 灵敏之慧 魔法师
	12 2阶段技能 生命之光 剑武士
	13 2阶段技能 抗斥 剑武士
	14 2阶段技能 保护罡气 剑武士
	15 2阶段技能 生命之光 枪械师
	16 2阶段技能 抗斥 枪械师
	17 2阶段技能 灵敏之慧 枪械师
	18 3阶段技能 所有能力值增加3
	19 3阶段技能 所有技能增加1
	*/
	TempBuf2.Add((BYTE)m_tBabySkill[0]);
	TempBuf2.Add((BYTE)m_tBabySkill[1]);
	TempBuf2.Add((BYTE)m_tBabySkill[2]);
	TempBuf2.Add((BYTE)m_tBabySkill[3]);

	SendInsight(TempBuf2, TempBuf2.GetLength());
}
//喂宠物
//喂养后额外发了一个包
//0x77 0x1f 0x27 0x00 0x00 0x15 
void USER::GiveBabyItem(short sSolt)
{
	if(!SubInvernItemBySolt(sSolt))
		return ;
	BYTE bSuc = 0x01;
	//1498 1491肉
	m_sHungry+=100;
	if(m_sHungry>m_sMaxHungry)
		m_sHungry = m_sMaxHungry;
/*	if((m_sFealty + 1) < 101)
        m_sFealty+=1;
	SendEventMsg("使用饲料.宠物[忠诚度]增加[ 1 ]");*/
	SendBabyInfo();

	CBufferEx TempBuf;
	TempBuf.Add(PK_BUTTON_REQ);
	TempBuf.Add((BYTE)0x26);
	TempBuf.Add((BYTE)bSuc);
	TempBuf.Add((short)100);
	TempBuf.Add((short)150);

	Send(TempBuf, TempBuf.GetLength());
}
void USER::BabyUpgradeLevel()
{
	if(m_sLevel<100)
		return ;
	short sOldLevel = m_sBabyLevel;

	m_sBabyLevel = (m_sLevel-100)/10 +1;

	if(m_sBabyLevel>=4)
	{
		if(m_sBabyID==256||m_sBabyID == 257)
			m_sBabyID+=256;
		m_tBabySkill[2] = 18;
	}else
	{
		if(m_sBabyID==512||m_sBabyID == 513)
			m_sBabyID-=256;

		m_tBabySkill[2] = 0;
	}

	if(m_sBabyLevel>=7)
	{
		if(m_sBabyID==512||m_sBabyID == 513)
			m_sBabyID+=256;
		m_tBabySkill[3] = 19;
	}else
	{
		if(m_sBabyID==768||m_sBabyID == 769)
			m_sBabyID-=256;
		m_tBabySkill[3] = 0;
	}

	if(sOldLevel != m_sBabyLevel)
		SendBabyInfo();
}

/////////////// 
void USER::WriteUserShopLog(TCHAR *strContents)
{
	SDWORD sSLen = strlen(strContents);
	if(sSLen <= 0 || sSLen >= 500) return;
	CFile file_log;
	file_log.Open("UserShop.Log", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone );
	file_log.SeekToEnd();
	file_log.Write(strContents,sSLen );
	file_log.Close();
}
////////////////////////////////////////////////////////
//扣税 
//////////////////////////////////////////
DWORD USER::SubTaxRate(DWORD dwCost,short sStoreID)		
{
	int nPos = 2;

	CGuildFortress *pGuild= GetGuildWarWin();
	if(pGuild!= NULL)
	{
		nPos = pGuild->m_tTaxRate;
	}
    
	DWORD dwTax = ((dwCost * nPos)/100)/10;//目前税收再降10倍
	DWORD dwCast= dwCost-dwTax;
	

//	CString strMsg1 = _T("");
//	strMsg1.Format( "您交易金额%d,税率%d%% 所得税收%d,实际金额%d ",dwCost,nPos,dwTax,dwCast);
//	CBufferEx	TempBuf,TempBuf1;
//	TempBuf.Add((byte)CHAT_RESULT);
//	TempBuf.Add((byte)1);
//	TempBuf.Add(WHISPER_CHAT);
//	TempBuf.Add((int)0x01);
//	TempBuf.Add("要塞团长", _tcslen("要塞团长"));
//	TempBuf.Add(strMsg1.GetBuffer(0), strMsg1.GetLength());
//	Send(TempBuf, TempBuf.GetLength());
	
	if(pGuild!= NULL)
		AddTaxToGuildBank(dwTax,sStoreID);
	return dwCast;
}
/////////////////////////////////////////

///////////////////////////////////////////
void USER::AddTaxToGuildBank(DWORD dwTax,short sStoreID)
{
	//要塞所有者
	CGuildFortress *pGuild= GetGuildWarWin();
	if(pGuild== NULL)
	{
		return ;
	}

	int dwVal =dwTax;

	if(dwTax > 100000000)//税收超过21E返回
	{
		return ;
	}

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	int	nDBIndex;
	int	i;
	::ZeroMemory( szSQL, sizeof(szSQL) );

	nDBIndex = 0;

	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) 
		return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS ) 
		return;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "select isid,idn from guild where strmastername='%s'" ),pGuild->m_strMasterName );
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch(hstmt);

		if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
		{
			SQLINTEGER		iSID;
			SQLINTEGER		nCode;
			SQLINTEGER		ivd;
			i = 1;
			SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &ivd );
			SQLGetData( hstmt, i++, SQL_C_SLONG, &nCode, sizeof(nCode), &ivd );
			dwVal = nCode;
			CheckMaxValue(dwVal,dwTax);
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
	{
		pDB = g_DB[m_iModSid].GetDB( nDBIndex );
		if( !pDB ) 
			return;

		retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
		if( retcode != SQL_SUCCESS ) 
			return;

		_sntprintf(szSQL, sizeof(szSQL), TEXT( "update guild set idn=%d where strmastername='%s'" ),dwVal,pGuild->m_strMasterName );
		retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
		if( retcode == SQL_SUCCESS)
		{
			retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			g_DB[m_iModSid].ReleaseDB(nDBIndex);
			return ;
		}
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);

	}
}
/////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void USER::UpdateTaxToDB(CGuildFortress *pGuild,int itax)
{
	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	int	nDBIndex;
	::ZeroMemory( szSQL, sizeof(szSQL) );

	nDBIndex = 0;
	{
		pDB = g_DB[m_iModSid].GetDB( nDBIndex );
		if( !pDB ) 
			return;

		retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
		if( retcode != SQL_SUCCESS ) 
			return;

		_sntprintf(szSQL, sizeof(szSQL), TEXT( "update guild_Fortress set staxRate=%d where sFortressID=%d" ),itax,pGuild->m_sFortressID );
		retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
		if( retcode == SQL_SUCCESS)
		{
			retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			g_DB[m_iModSid].ReleaseDB(nDBIndex);
			return ;
		}
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);

	}
}

void USER::YJmzb()
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif
	if(m_sLevel<60)
      {
       SendSystemMsg("注意:60级以上才能一键卖装备", SYSTEM_ANNOUNCE, TO_ME);
		   return;
	  }
	int i, j,sSid;
for(i = EQUIP_ITEM_NUM; i < 22; i++)
	{
		sSid = m_UserItem[i].sSid;
		
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) continue;
		int tClass = g_arItemTable[sSid]->m_byWear;
		if(tClass>= 1 && tClass<= 5)
		  {
			if(m_UserItem[i].tMagic[5] <1|| m_UserItem[i].tIQ!=3)
               {
			        DWORD m_dwDN1=g_arItemTable[sSid]->m_iDN;
			        DWORD m_dwDN2=m_dwDN1/6;    
			        m_dwDN+=m_dwDN2;
                    UpdateUserItemDN();							
	                SendMoneyChanged();
				   ReSetItemSlot(&m_UserItem[i]);
       
			   int iWeight =  g_arItemTable[i]->m_byWeight;

		       m_iCurWeight -= iWeight;
		       if(m_iCurWeight < 0) 
		       m_iCurWeight = 0;
       
            CBufferEx TempBuf;
		    TempBuf.Add(ITEM_GIVE_RESULT);
		    TempBuf.Add((BYTE)0x01);
		    TempBuf.Add((BYTE)i);
		    TempBuf.Add(m_UserItem[i].sLevel);
		    TempBuf.Add(m_UserItem[i].sSid);
		    TempBuf.Add(m_UserItem[i].sDuration);
		    TempBuf.Add(m_UserItem[i].sBullNum);
		    TempBuf.Add(m_UserItem[i].sCount);
		    for(j = 0; j < MAGIC_NUM; j++) 
			TempBuf.Add(m_UserItem[i].tMagic[j]);

		    TempBuf.Add(m_UserItem[i].tIQ);

		    Send(TempBuf, TempBuf.GetLength());
			}
       }
		}
}
void USER::Diaoluo()
{
		 USER* pUser = NULL;
	if (m_dwShopPingDN == 500 || m_dwShopPingDN == 1000 || m_dwShopPingDN == 1500|| m_dwShopPingDN ==2000|| m_dwShopPingDN == 2500)
		/* for(int i=1;i< 10;i++)*/
	 {
			 /*if(m_dwShopPingDN ==500*i)*/
              
	  /* {
		int iNum2 = -1;
		 ItemList TempItem;
	     iNum2 = GetSameItem(TempItem, INVENTORY_SLOT);
		 iNum2 = GetEmptySlot(INVENTORY_SLOT);
		 if(iNum2 < 1)						
		 {

		    SendSystemMsg("包裹空间满了,请至少留1个空间!", SYSTEM_ERROR, TO_ME);
		 }
		 else */
		
			int shul = 0;
			int iRandom = 0;
			CString strMsg = _T("");
			iRandom = myrand(1, 10);
			switch(iRandom)
			{
			case 1: shul = 50;break;
			case 2: shul = 50;break;
			case 3: shul = 50;break;
			case 4: shul = 100;break;
			case 5: shul = 200;break;
			case 6: shul = 50;break;
			case 7: shul = 50;break;
			case 8: shul = 100;break;
			case 9: shul = 200;break;
			case 10: shul = 300;break;
			}

		GiveItemAll(724,shul,0,0,0,0,0,0,0,0,0,0,0);
		GiveAllItem(1718, 10, 0, 0, 0, 0, 0, 0, 0);//碎片
				SendMoneyChanged();
				UpdateUserItemDN();
			strMsg.Format( "恭喜玩家[ %s ]杀怪满:[ %d ]个,随机奖励标志[ %d ]个和10个结晶碎片", m_strUserID , m_dwShopPingDN , shul);
			SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
         
		
      
	}	
	  //  if (m_sLevel >=90)
   //   {  
		 // for(int i=0;i< 10;i++)
	  //   {
			// if(m_dwShopPingDN ==1000*i)
   //           {
	  //          int iNum2 = -1;
		 //          ItemList TempItem;
	  //             iNum2 = GetSameItem(TempItem, INVENTORY_SLOT);
		 //           iNum2 = GetEmptySlot(INVENTORY_SLOT);
		 //           if(iNum2 < 1)						
		 //          {

		 //            SendSystemMsg("包裹空间满了,请至少留1个空间解封!", SYSTEM_ERROR, TO_ME);
		 //           }
			//		else 
		 //        { 
		 //         GiveAllItem(1349, 1, 0, 0, 0, 0, 0, 0, 0);//碎片
   //              }
	  //        }
			// else return;
   //       }
		 //}
}

////////////////////////
// 3排合成4排窗口.
void USER::Add4Pai(short storeID)
{

#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif
	int i;
	short sStoreID = storeID;

	int iStart = EQUIP_ITEM_NUM;
	int iEnd = TOTAL_INVEN_MAX;

	CStore* pStore = GetStore(sStoreID);
	if(pStore == NULL) return;

	CBufferEx CostBuf;
	BYTE tSlot = 0;
	short sCount = 0;
	int sSid = -1;
	for(i = iStart; i < iEnd; i++)
	{
		sSid = m_UserItem[i].sSid;
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) continue;
		if(m_UserItem[i].tIQ == 3)
		{
			if( sSid != -1 && (g_arItemTable[sSid]->m_byWear == 2 || g_arItemTable[sSid]->m_byWear == 3 || g_arItemTable[sSid]->m_byWear == 4 || g_arItemTable[sSid]->m_byWear == 5))
			{
				tSlot = (BYTE)i;
				sCount++;
				CostBuf.Add(tSlot);
			}
		}
	}
	CBufferEx TempBuf;
	TempBuf.Add(EBODY_IDENTIFY_OPEN);
	TempBuf.Add(sStoreID);
	TempBuf.Add(sCount);
	if(sCount > 0) TempBuf.AddData(CostBuf, CostBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////
//3排增加4排属性
void USER::Add4PaiReq(TCHAR* pBuf)
{
	int index = 0, i, j = 0 ,k = 0;
	short sSid = -1;
	BYTE tSlot = 0;
	int iWeight = 0;
	int random = 0,random2 = 0,random3 = 0,success = 0;
	CByteArray arMaterial;
	arMaterial.RemoveAll();

	GetShort(pBuf, index);
	short sCount = GetShort(pBuf, index);

	if(sCount <= 0)
	{
		SendSystemMsg("没有选择任何装备,请选择你要改造的装备!",SYSTEM_ERROR,TO_ME);
		return;
	}

	if(sCount > 1)
	{
		SendSystemMsg("一次改造只能选择一件装备,请重新选择!",SYSTEM_ERROR,TO_ME);
		return;
	}
	if(( FindItem(724)) < 100)
	{
		SendSystemMsg("一次改造需要100标志!",SYSTEM_ERROR,TO_ME);
		return;
	}
	tSlot = GetByte(pBuf, index);
	if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) return;	
	if(m_UserItem[tSlot].sCount >= 2) return;
	sSid = m_UserItem[tSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return;
	if(g_arItemTable[sSid]->m_byWear < 2 || g_arItemTable[sSid]->m_byWear > 5 ) return;

	if(m_UserItem[tSlot].tMagic[2] > 0 && m_UserItem[tSlot].tMagic[3] == 0)
	{
		random = myrand(1,10000);
		
		if(m_UserItem[tSlot].tMagic[0] == m_UserItem[tSlot].tMagic[1] && m_UserItem[tSlot].tMagic[1] == m_UserItem[tSlot].tMagic[2])
		{
			BYTE magic = m_UserItem[tSlot].tMagic[0];
			if(magic < 0 || magic >= g_arMagicItemTable.GetSize()) return;
			if(g_arMagicItemTable[m_UserItem[tSlot].tMagic[0]]->m_tLevel >= 5) random = 10000;//改造装备禁止极品成功
		}
		if(random <= 4000)	//改造成功
		{
			success = 1;
			//random2 = myrand(0,2);
			//m_UserItem[tSlot].tMagic[3] = m_UserItem[tSlot].tMagic[random2];
            for(i = 0;  i < 20; i++)
		 {
			random2 = myrand(1,143);
			if(g_arMagicItemTable[random2]->m_tUse ==1) 
                {
			      if(g_arMagicItemTable[random2]->m_sSubType >= 22 || g_arMagicItemTable[random2]->m_sSubType <= 6 && g_arMagicItemTable[random2]->m_sSubType!=37 && g_arMagicItemTable[random2]->m_sSubType!=27 && g_arMagicItemTable[random2]->m_sSubType!=28 && g_arMagicItemTable[random2]->m_sSubType!=26 && g_arMagicItemTable[random2]->m_sSubType!=30 )
					 {
					   if( random2 != m_UserItem[tSlot].tMagic[0] && random2 != m_UserItem[tSlot].tMagic[1] && random2 != m_UserItem[tSlot].tMagic[2])
						 {  
						   break;
                         }
                      random2 = 82;
					 }
				 random2 = 82;
				}	
			   
			
			else  random2=101;
         }
			m_UserItem[tSlot].tMagic[3] = random2 ;

			MakeItemLog(&m_UserItem[tSlot], ITEMLOG_EBODY_UPGRADE_SUCCESS );
			iWeight += g_arItemTable[sSid]->m_byWeight;		
			arMaterial.Add((BYTE)tSlot);
		}
		else
		{
			random3 = myrand(1,10000);
			if(random3 > 4000)
			{
				MakeItemLog(&m_UserItem[tSlot], ITEMLOG_EBODY_ADDER_DELETE );
				ReSetItemSlot(&m_UserItem[tSlot]);
			}
			iWeight += g_arItemTable[sSid]->m_byWeight;	
			arMaterial.Add((BYTE)tSlot);
		}

		FlushItemLog( TRUE );
		RobItem(724,100);							

		CBufferEx TempBuf;
		TempBuf.Add(UPGRADE_ITEM_RESULT);
		index = arMaterial.GetSize();

		if(success == 1) TempBuf.Add((BYTE)0x01);	// EBody Upgrade 己傍
		else TempBuf.Add((BYTE)0x00);				// EBody Upgrade 角菩

		TempBuf.Add((BYTE)index);

		for(i = 0;  i < arMaterial.GetSize(); i++)
		{
			tSlot = arMaterial[i];
			TempBuf.Add((BYTE)tSlot);
			TempBuf.Add(m_UserItem[tSlot].sLevel);
			TempBuf.Add(m_UserItem[tSlot].sSid);
			TempBuf.Add(m_UserItem[tSlot].sDuration);
			TempBuf.Add(m_UserItem[tSlot].sBullNum);
			TempBuf.Add(m_UserItem[tSlot].sCount);
			for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[tSlot].tMagic[j]);
			TempBuf.Add(m_UserItem[tSlot].tIQ); 
		}
		m_iCurWeight -= iWeight;
		if(m_iCurWeight < 0) m_iCurWeight = 0;
		GetRecoverySpeed();	
		Send(TempBuf, TempBuf.GetLength());
		arMaterial.RemoveAll();
		if(success == 1) SendNpcSay( NULL, 5010 );
		else SendNpcSay( NULL, 5011 );
	}
	else
	{
		SendSystemMsg("对不起,改造的物品要求是3排属性.",SYSTEM_ERROR,TO_ME);
		return;
	}
}

void USER::KuaiSuZuDui()
{
	
	USER *pUser = NULL;
	CBufferEx TempBuf;
	int i ,j ;
	int diffLevel = 0;
	CString msg;
	if(m_bMakeBuddy || m_bNowBuddy || m_bBuddyMode == TRUE)
	{
		msg.Format( "你有队伍." );
		SendSystemMsg((LPTSTR)(LPCTSTR)msg, SYSTEM_NORMAL, TO_ME);
		return;	
	}

	for (i = 0; i < MAX_USER; i++ )	//5疙父 犬牢
	{
		pUser = m_pCom->GetUserUid(i);
		if(!(pUser && pUser->m_state == STATE_GAMESTARTED)) continue;	
		if(strcmp(m_strUserID, pUser->m_strUserID) == 0)  continue;	
		diffLevel = abs(m_sLevel - pUser->m_sLevel);
		if( diffLevel > 25) continue;
		if(pUser->m_bMakeBuddy) //滚叼炉烙
		{ 
			for(j = 0; j < MAX_BUDDY_USER_NUM; j++)	
			{	
				if(pUser->m_MyBuddy[j].uid == -1)
				{	
					TempBuf.AddString(m_strUserID);
					pUser->BuddyUserChange(TempBuf, 6);	
					return;
				}
            }
         }
	}
}

