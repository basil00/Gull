
#ifdef MACOSX
#define LINUX
#endif

#define W32_BUILD
#undef W32_BUILD

#ifdef W32_BUILD
#define NTDDI_VERSION 0x05010200
#define _WIN32_WINNT 0x0501
#endif

#define CPU_TIMING
#undef CPU_TIMING

#define EXPLAIN_EVAL
#undef EXPLAIN_EVAL

#define LARGE_PAGES
//#undef LARGE_PAGES

#define MP_NPS
//#undef MP_NPS

#define TIME_TO_DEPTH
//#undef TIME_TO_DEPTH

#ifdef LINUX
#include "Linux.h"
#endif

#ifdef WINDOWS
#include "Windows.h"
#endif

#include "data.h"

#define STRING2(x)  #x
#define STRING(x)   STRING2(x)

#define Convert(x,type) ((type)(x))

#define Abs(x) ((x) > 0 ? (x) : (-(x)))
#define Sgn(x) ((x) == 0 ? 0 : ((x) > 0 ? 1 : (-1)))
#define Min(x,y) ((x) < (y) ? (x) : (y))
#define Max(x,y) ((x) > (y) ? (x) : (y))
#define Sqr(x) ((x) * (x))
#define T(x) ((x) != 0)
#define F(x) ((x) == 0)
#define Even(x) F((x) & 1)
#define Odd(x) T((x) & 1)
#define Combine(x,y) ((x) | ((y) << 16))
#define Compose(x,y) ((x) + ((y) << 16))
#define Compose16(x,y) Compose((x)/16,(y)/16)
#define Compose64(x,y) Compose((x)/64,(y)/64)
#define Compose256(x,y) Compose((x)/256,(y)/256)
#define Opening(x) Convert((x) & 0xFFFF,int16_t)
#define Endgame(x) ((((x) >> 15) & 1) + Convert((x) >> 16,int16_t))

#define File(x) ((x) & 7)
#define Rank(x) ((x) >> 3)
#define CRank(me,x) ((me) ? (7 - Rank(x)) : Rank(x))
#define NDiag(x) (7 - File(x) + Rank(x))
#define SDiag(x) (File(x) + Rank(x))
#define Dist(x,y) Max(Abs(Rank(x)-Rank(y)),Abs(File(x)-File(y)))
#define VarC(var,me) ((me) ? (var##_b) : (var##_w))
#define PVarC(prefix,var,me) ((me) ? (prefix.var##_b) : (prefix.var##_w))

#define Bit(x) (Convert(1,uint64_t) << (x))
#ifndef GULL_PDEP_BITBOARDS
#define Cut(x) (x &= (x) - 1)
#else
#define Cut(x) (x = _blsr_u64(x))
#endif
#define Multiple(x) T((x) & ((x) - 1))
#define Single(x) F((x) & ((x) - 1))
#define Add(x,b) (x |= Bit(b))

#define From(move) (((move) >> 6) & 0x3f)
#define To(move) ((move) & 0x3f)
#define SetScore(move,score) ((move) = (((move) & 0xFFFF) | ((score) << 16)))
#define BitFrom(move) Bit(From(move))
#define BitTo(move) Bit(To(move))
#define MakeMove(from,to) ((from) << 6) | (to))
#define MakeMoveF(from,to,flags) ((from) << 6) | (to) | (flags))
#define MakeMoveFS(from,to,flags,score) ((from) << 6) | (to) | (flags) | (score))
#define PieceAtOrigin(move) Square(From(move))
#define Target(move) Square(To(move)) 

#define Empty Convert(0,uint64_t)
#define Filled (~Empty)
#define Interior Convert(0x007E7E7E7E7E7E00,uint64_t)
#define Boundary (~Interior)
#define WhiteArea Convert(0x00000000FFFFFFFF,uint64_t)
#define BlackArea (~WhiteArea)
#define LightArea Convert(0x55AA55AA55AA55AA,uint64_t)
#define DarkArea (~LightArea)
#define FileA Convert(0x0101010101010101,uint64_t)
#define Line0 Convert(0x00000000000000FF,uint64_t)

#define High32(x) ((x) >> 32)
#define Low32(x) Convert(x,uint32_t)

#define White 0
#define Black 1
#define WhitePawn 2
#define BlackPawn 3
#define WhiteKnight 4
#define BlackKnight 5
#define WhiteLight 6
#define BlackLight 7
#define WhiteDark 8
#define BlackDark 9
#define WhiteRook 10
#define BlackRook 11
#define WhiteQueen 12
#define BlackQueen 13
#define WhiteKing 14
#define BlackKing 15

#define IsSlider(x) T(0x3FC0 & Bit(x))

#define CanCastle_OO 1
#define CanCastle_oo 2
#define CanCastle_OOO 4
#define CanCastle_ooo 8

#define FlagCastling 0x1000
#define FlagEP 0x2000
#define FlagPKnight 0x4000
#define FlagPLight 0x6000
#define FlagPDark 0x8000
#define FlagPRook 0xA000
#define FlagPQueen 0xC000

#define IsPromotion(move) T((move) & 0xC000)
#define IsCastling(move) T((move) & 0x1000)
#define IsEP(move) (((move) & 0xF000) == 0x2000)
#define Promotion(move,side) ((side) + (((move) & 0xF000) >> 12))

const uint8_t UpdateCastling[64] =
{
    0xFF^CanCastle_OOO,0xFF,0xFF,0xFF,0xFF^(CanCastle_OO|CanCastle_OOO),
        0xFF,0xFF,0xFF^CanCastle_OO,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF^CanCastle_ooo,0xFF,0xFF,0xFF,0xFF^(CanCastle_oo|CanCastle_ooo),
        0xFF,0xFF,0xFF^CanCastle_oo
};

#ifdef GULL_MAGIC_BITBOARDS

static const uint64_t BMagic[64] =
{
    0x0048610528020080, 0x00c4100212410004, 0x0004180181002010,
    0x0004040188108502, 0x0012021008003040, 0x0002900420228000,
    0x0080808410c00100, 0x000600410c500622, 0x00c0056084140184,
    0x0080608816830050, 0x00a010050200b0c0, 0x0000510400800181,
    0x0000431040064009, 0x0000008820890a06, 0x0050028488184008,
    0x00214a0104068200, 0x004090100c080081, 0x000a002014012604,
    0x0020402409002200, 0x008400c240128100, 0x0001000820084200,
    0x0024c02201101144, 0x002401008088a800, 0x0003001045009000,
    0x0084200040981549, 0x0001188120080100, 0x0048050048044300,
    0x0008080000820012, 0x0001001181004003, 0x0090038000445000,
    0x0010820800a21000, 0x0044010108210110, 0x0090241008204e30,
    0x000c04204004c305, 0x0080804303300400, 0x00a0020080080080,
    0x0000408020220200, 0x0000c08200010100, 0x0010008102022104,
    0x0008148118008140, 0x0008080414809028, 0x0005031010004318,
    0x0000603048001008, 0x0008012018000100, 0x0000202028802901,
    0x004011004b049180, 0x0022240b42081400, 0x00c4840c00400020,
    0x0084009219204000, 0x000080c802104000, 0x0002602201100282,
    0x0002040821880020, 0x0002014008320080, 0x0002082078208004,
    0x0009094800840082, 0x0020080200b1a010, 0x0003440407051000,
    0x000000220e100440, 0x00480220a4041204, 0x00c1800011084800,
    0x000008021020a200, 0x0000414128092100, 0x0000042002024200,
    0x0002081204004200
};

static const uint64_t RMagic[64] =
{
    0x00800011400080a6, 0x004000100120004e, 0x0080100008600082,
    0x0080080016500080, 0x0080040008000280, 0x0080020005040080,
    0x0080108046000100, 0x0080010000204080, 0x0010800424400082,
    0x00004002c8201000, 0x000c802000100080, 0x00810010002100b8,
    0x00ca808014000800, 0x0002002884900200, 0x0042002148041200,
    0x00010000c200a100, 0x00008580004002a0, 0x0020004001403008,
    0x0000820020411600, 0x0002120021401a00, 0x0024808044010800,
    0x0022008100040080, 0x00004400094a8810, 0x0000020002814c21,
    0x0011400280082080, 0x004a050e002080c0, 0x00101103002002c0,
    0x0025020900201000, 0x0001001100042800, 0x0002008080022400,
    0x000830440021081a, 0x0080004200010084, 0x00008000c9002104,
    0x0090400081002900, 0x0080220082004010, 0x0001100101000820,
    0x0000080011001500, 0x0010020080800400, 0x0034010224009048,
    0x0002208412000841, 0x000040008020800c, 0x001000c460094000,
    0x0020006101330040, 0x0000a30010010028, 0x0004080004008080,
    0x0024000201004040, 0x0000300802440041, 0x00120400c08a0011,
    0x0080006085004100, 0x0028600040100040, 0x00a0082110018080,
    0x0010184200221200, 0x0040080005001100, 0x0004200440104801,
    0x0080800900220080, 0x000a01140081c200, 0x0080044180110021,
    0x0008804001001225, 0x00a00c4020010011, 0x00001000a0050009,
    0x0011001800021025, 0x00c9000400620811, 0x0032009001080224,
    0x001400810044086a
};

const int32_t BShift[64] =
{
    58, 59, 59, 59, 59, 59, 59, 58,
    59, 59, 59, 59, 59, 59, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 59, 59, 59, 59, 59, 59,
    58, 59, 59, 59, 59, 59, 59, 58
};

const int32_t BOffset[64] =
{
    0, 64, 96, 128, 160, 192, 224, 256,
    320, 352, 384, 416, 448, 480, 512, 544,
    576, 608, 640, 768, 896, 1024, 1152, 1184,
    1216, 1248, 1280, 1408, 1920, 2432, 2560, 2592,
    2624, 2656, 2688, 2816, 3328, 3840, 3968, 4000,
    4032, 4064, 4096, 4224, 4352, 4480, 4608, 4640,
    4672, 4704, 4736, 4768, 4800, 4832, 4864, 4896,
    4928, 4992, 5024, 5056, 5088, 5120, 5152, 5184
};

const int32_t RShift[64] =
{
    52, 53, 53, 53, 53, 53, 53, 52,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    52, 53, 53, 53, 53, 53, 53, 52
};

const int32_t ROffset[64] =
{
    5248, 9344, 11392, 13440, 15488, 17536, 19584, 21632,
    25728, 27776, 28800, 29824, 30848, 31872, 32896, 33920,
    35968, 38016, 39040, 40064, 41088, 42112, 43136, 44160,
    46208, 48256, 49280, 50304, 51328, 52352, 53376, 54400,
    56448, 58496, 59520, 60544, 61568, 62592, 63616, 64640,
    66688, 68736, 69760, 70784, 71808, 72832, 73856, 74880,
    76928, 78976, 80000, 81024, 82048, 83072, 84096, 85120,
    87168, 91264, 93312, 95360, 97408, 99456, 101504, 103552
};
#endif

#define QueenAttacks(sq, occ)   (BishopAttacks(sq, occ) | RookAttacks(sq, occ))

#define FlagUnusualMaterial (1 << 30)

const int MatCode[16] = {0,0,MatWP,MatBP,MatWN,MatBN,MatWL,MatBL,MatWD,MatBD,MatWR,MatBR,MatWQ,MatBQ,0,0};

#define opp (1 ^ (me))

#define IPawn(me) (WhitePawn | (me))
#define IKnight(me) (WhiteKnight | (me))
#define ILight(me) (WhiteLight | (me))
#define IDark(me) (WhiteDark | (me))
#define IRook(me) (WhiteRook | (me))
#define IQueen(me) (WhiteQueen | (me))
#define IKing(me) (WhiteKing | (me))

#define BB(i) Board->bb[i]
#define Pawn(me) (BB(WhitePawn | (me)))
#define Knight(me) (BB(WhiteKnight | (me)))
#define Bishop(me) (BB(WhiteLight | (me)) | BB(WhiteDark | (me)))
#define Rook(me) (BB(WhiteRook | (me)))
#define Queen(me) (BB(WhiteQueen | (me)))
#define King(me) (BB(WhiteKing | (me)))
#define Piece(me) (BB(me))
#define NonPawn(me) (Piece(me) ^ Pawn(me))
#define NonPawnKing(me) (NonPawn(me) ^ King(me))
#define BSlider(me) (Bishop(me) | Queen(me))
#define RSlider(me) (Rook(me) | Queen(me))
#define Major(me) RSlider(me)
#define Minor(me) (Knight(me) | Bishop(me))
#define Slider(me) (BSlider(me) | RSlider(me))
#define PieceAll (Piece(White) | Piece(Black))
#define SliderAll (Slider(White) | Slider(Black))
#define PawnAll (Pawn(White) | Pawn(Black))
#define NonPawnKingAll (NonPawnKing(White) | NonPawnKing(Black))
#define KingPos(me) (lsb(King(me)))

#define ShiftNW(target) (((target) & (~(DATA->File[0] | DATA->Line[7]))) << 7)
#define ShiftNE(target) (((target) & (~(DATA->File[7] | DATA->Line[7]))) << 9)
#define ShiftSE(target) (((target) & (~(DATA->File[7] | DATA->Line[0]))) >> 7)
#define ShiftSW(target) (((target) & (~(DATA->File[0] | DATA->Line[0]))) >> 9)
#define ShiftW(me,target) ((me) ? ShiftSW(target) : ShiftNW(target))
#define ShiftE(me,target) ((me) ? ShiftSE(target) : ShiftNE(target))
#define ShiftN(target) ((target) << 8)
#define ShiftS(target) ((target) >> 8)
#define Shift(me,target) ((me) ? ShiftS(target) : ShiftN(target))
#define PushW(me) ((me) ? (-9) : (7))
#define PushE(me) ((me) ? (-7) : (9))
#define Push(me) ((me) ? (-8) : (8))
#define Dir(me) ((me) ? (-1) : (1))
#define IsGreater(me,x,y) ((me) ? ((x) < (y)) : ((x) > (y)))

#define Line(me,n) ((me) ? DATA->Line[7 - n] : DATA->Line[n])
#define Square(sq) Board->square[sq]
#define AddMove(from,to,flags,score) { *list = ((from) << 6) | (to) | (flags) | (score); list++; }
#define AddCapture(from,to,flags) AddMove(from,to,flags,DATA->MvvLva[Square(from)][Square(to)])
#define AddCaptureP(piece,from,to,flags) AddMove(from,to,flags,DATA->MvvLva[piece][Square(to)])
#define AddHistoryP(piece,from,to,flags) AddMove(from,to,flags,HistoryP(piece,from,to))
#define AddHistory(from,to) AddMove(from,to,0,History(from,to))
#define AddDeltaP(piece,from,to,flags) AddMove(from,to,flags,Convert(DeltaScore(piece,from,to)+(int16_t)0x4000,int) << 16)
#define AddDelta(from,to) AddMove(from,to,0,Convert(Delta(from,to)+(int16_t)0x4000,int) << 16)
#define AddCDeltaP(piece,from,to,flags) {if (DeltaScore(piece,from,to) >= Current->margin) AddMove(from,to,flags,Convert(DeltaScore(piece,from,to)+(int16_t)0x4000,int) << 16)}
#define AddCDelta(from,to) {if (Delta(from,to) >= Current->margin) AddMove(from,to,0,Convert(Delta(from,to)+(int16_t)0x4000,int) << 16)}

#define Check(me) T(Current->att[(me) ^ 1] & King(me))
#define IsIllegal(me,move) ((T(Current->xray[opp] & Bit(From(move))) && F(Bit(To(move)) & DATA->FullLine[lsb(King(me))][From(move)])) \
    || (IsEP(move) && T(DATA->Line[Rank(From(move))] & King(me)) && T(DATA->Line[Rank(From(move))] & Major(opp)) && \
    T(RookAttacks(lsb(King(me)),PieceAll ^ Bit(From(move)) ^ Bit(Current->ep_square - Push(me))) & Major(opp))))
#define IsRepetition(margin,move) ((margin) > 0 && Current->ply >= 2 && (Current-1)->move == ((To(move) << 6) | From(move)) && F(Square(To(move))) && F((move) & 0xF000))

#define IncV(var,x) (me ? (var -= (x)) : (var += (x)))
#define DecV(var,x) IncV(var,-(x))

#define KpkValue 300
#define EvalValue 30000
#define MateValue 32760
#define TBMateValue 30000
#define TBCursedMateValue 3
const int TbValues[5] =
    {-TBMateValue, -TBCursedMateValue, 0, TBCursedMateValue, TBMateValue};
#define TbDepth (depth+6>=127?127:depth+6)

/* 
general move:
0 - 11: from & to
12 - 15: flags
16 - 23: history
24 - 25: spectial moves: killers, refutations...
26 - 30: MvvLva
delta move:
0 - 11: from & to
12 - 15: flags
16 - 31: int16_t delta + (int16_t)0x4000
*/

const int MvvLvaVictim[16] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3}; 
const int MvvLvaAttacker[16] =
    {0, 0, 5, 5, 4, 4, 3, 3, 3, 3, 2, 2, 1, 1, 6, 6};
const int MvvLvaAttackerKB[16] =
    {0, 0, 9, 9, 7, 7, 5, 5, 5, 5, 3, 3, 1, 1, 11, 11};
#define PawnCaptureMvvLva(attacker) (MvvLvaAttacker[attacker])
#define MaxPawnCaptureMvvLva (MvvLvaAttacker[15]) // 6
#define KnightCaptureMvvLva(attacker) (MaxPawnCaptureMvvLva + MvvLvaAttackerKB[attacker]) 
#define MaxKnightCaptureMvvLva (MaxPawnCaptureMvvLva + MvvLvaAttackerKB[15]) // 17
#define BishopCaptureMvvLva(attacker) (MaxPawnCaptureMvvLva + MvvLvaAttackerKB[attacker] + 1)
#define MaxBishopCaptureMvvLva (MaxPawnCaptureMvvLva + MvvLvaAttackerKB[15] + 1) // 18
#define RookCaptureMvvLva(attacker) (MaxBishopCaptureMvvLva + MvvLvaAttacker[attacker])
#define MaxRookCaptureMvvLva (MaxBishopCaptureMvvLva + MvvLvaAttacker[15]) // 24
#define QueenCaptureMvvLva(attacker) (MaxRookCaptureMvvLva + MvvLvaAttacker[attacker])

#define MvvLvaPromotion (DATA->MvvLva[WhiteQueen][BlackQueen])
#define MvvLvaPromotionKnight (DATA->MvvLva[WhiteKnight][BlackKnight])
#define MvvLvaPromotionCap(capture) (DATA->MvvLva[((capture) < WhiteRook) ? WhiteRook : ((capture) >= WhiteQueen ? WhiteKing : WhiteKnight)][BlackQueen])
#define MvvLvaPromotionKnightCap(capture) (DATA->MvvLva[WhiteKing][capture])
#define MvvLvaXray (DATA->MvvLva[WhiteQueen][WhitePawn])
#define MvvLvaXrayCap(capture) (DATA->MvvLva[WhiteKing][capture])
#define RefOneScore ((0xFF << 16) | (3 << 24))
#define RefTwoScore ((0xFF << 16) | (2 << 24))
#define KillerOneScore ((0xFF << 16) | (1 << 24))
#define KillerTwoScore (0xFF << 16)

#define halt_check if ((Current - Data) >= 126) {evaluate(); return Current->score;} \
    if (Current->ply >= 100) return 0; \
    for (i = 4; i <= Current->ply; i+= 2) if (Stack[sp-i] == Current->key) return 0
#define ExtFlag(ext) ((ext) << 16)
#define Ext(flags) (((flags) >> 16) & 0xF)
#define FlagHashCheck (1 << 20) // first 20 bits are reserved for the hash killer and extension
#define FlagHaltCheck (1 << 21)
#define FlagCallEvaluation (1 << 22)
#define FlagDisableNull (1 << 23)
#define FlagNeatSearch (FlagHashCheck | FlagHaltCheck | FlagCallEvaluation)
#define FlagNoKillerUpdate (1 << 24)
#define FlagReturnBestMove (1 << 25)

#define MSBZ(x) ((x) ? msb(x) : 63)
#define LSBZ(x) ((x) ? lsb(x) : 0)
#define NB(me, x) ((me) ? msb(x) : lsb(x))
#define NBZ(me, x) ((me) ? MSBZ(x) : LSBZ(x))

typedef struct {
    uint64_t bb[16];
    uint8_t square[64];
} GBoard;
__align(64) GBoard Board[1];
uint64_t Stack[2048];
int sp, save_sp;
GBoard SaveBoard[1];

typedef struct {
    uint64_t key, pawn_key;
    uint16_t move;
    uint8_t turn, castle_flags, ply, ep_square, piece, capture;
    uint8_t square[64];
    int pst, material;
} GPosData;
typedef struct {
    uint64_t key, pawn_key, eval_key, att[2], patt[2], passer, xray[2], pin[2], threat, mask;
    uint8_t turn, castle_flags, ply, ep_square, capture, gen_flags, piece, stage, mul, dummy;
    int16_t score;
    uint16_t move, killer[3], ref[2];
    int best;
    int material, pst;
    int margin, *start, *current;
    int moves[230];
} GData;
__align(64) GData Data[256];
GData *Current = Data;
#define FlagSort (1 << 0)
#define FlagNoBcSort (1 << 1)
GData SaveData[1];

enum {
    stage_search, s_hash_move, s_good_cap, s_special, s_quiet, s_bad_cap, s_none,
    stage_evasion, e_hash_move, e_ev, e_none, 
    stage_razoring, r_hash_move, r_cap, r_checks, r_none
};
#define StageNone ((1 << s_none) | (1 << e_none) | (1 << r_none))

typedef struct {
    uint32_t key;
    uint16_t date;
    uint16_t move;
    int16_t low;
    int16_t high;
    uint16_t flags;
    uint8_t low_depth;
    uint8_t high_depth;
} GEntry;
#define initial_hash_size (1024 * 1024)
int64_t hash_size = initial_hash_size;
uint64_t hash_mask = (initial_hash_size - 4);

typedef struct {
    uint64_t key:48;
    uint8_t shelter[2], passer[2], draw[2];
    int score;
} GPawnEntry;
#define pawn_hash_size (1024 * 1024)
#define pawnHashSize    (pawn_hash_size * sizeof(GPawnEntry))
#define pawn_hash_mask (pawn_hash_size - 1)

typedef struct {
    uint32_t key;
    uint16_t date;
    uint16_t move;
    int16_t value;
    int16_t exclusion;
    uint8_t depth;
    uint8_t ex_depth;
    int knodes;
    int ply;
} GPVEntry;
#define pv_hash_size (1024 * 1024)
#define pv_cluster_size 4
#define pv_hash_mask (pv_hash_size - pv_cluster_size)
#define pvHashSize (pv_hash_size * sizeof(GPVEntry))

int RootList[256];

#define prefetch(a,mode) _mm_prefetch(a,mode)

#define FlagSingleBishop_w (1 << 0)
#define FlagSingleBishop_b (1 << 1)
#define FlagCallEvalEndgame_w (1 << 2)
#define FlagCallEvalEndgame_b (1 << 3)

#define Pst(piece,sq) DATA->Pst[((piece) << 6) | (sq)]

int16_t History[16 * 64]; 
#define HistoryScore(piece,from,to) History[((piece) << 6) | (to)]
#define HistoryP(piece,from,to) ((Convert(HistoryScore(piece,from,to) & 0xFF00,int)/Convert(HistoryScore(piece,from,to) & 0x00FF,int)) << 16)
#define History(from,to) HistoryP(Square(from),from,to)
#define HistoryM(move) HistoryScore(Square(From(move)),From(move),To(move))
#define HistoryInc(depth) Min(((depth) >> 1) * ((depth) >> 1), 64)
#define HistoryGood(move) if ((HistoryM(move) & 0x00FF) >= 256 - HistoryInc(depth)) \
    HistoryM(move) = ((HistoryM(move) & 0xFEFE) >> 1) + ((HistoryInc(depth) << 8) | HistoryInc(depth)); \
    else HistoryM(move) += ((HistoryInc(depth) << 8) | HistoryInc(depth))
#define HistoryBad(move) if ((HistoryM(move) & 0x00FF) >= 256 - HistoryInc(depth)) \
    HistoryM(move) = ((HistoryM(move) & 0xFEFE) >> 1) + HistoryInc(depth); else HistoryM(move) += HistoryInc(depth)

int16_t Delta[16 * 4096];
#define DeltaScore(piece,from,to) Delta[((piece) << 12) | ((from) << 6) | (to)]
#define Delta(from,to) DeltaScore(Square(from),from,to)
#define DeltaM(move) Delta(From(move),To(move))
#define UpdateDelta if (F(Current->capture) && T(Current->move) && F(Current->move & 0xE000) && Current > Data) { \
    if (DeltaScore(Current->piece,From(Current->move),To(Current->move)) <= -Current->score - ((Current - 1)->score)) \
    DeltaScore(Current->piece,From(Current->move),To(Current->move)) = -Current->score - ((Current - 1)->score); \
    else DeltaScore(Current->piece,From(Current->move),To(Current->move))--; }
#define DeltaMarginP(piece,from,to) (DeltaScore(piece,from,to) >= Current->margin)
#define DeltaMargin(from,to) (Delta(from,to) >= Current->margin)

typedef struct {
    uint16_t ref[2];
    uint16_t check_ref[2];
} GRef;
GRef Ref[16 * 64];
#define RefPointer(piece,from,to) Ref[((piece) << 6) | (to)]
#define RefM(move) RefPointer(Square(To(move)),From(move),To(move))
#define UpdateRef(ref_move) if (T(Current->move) && RefM(Current->move).ref[0] != (ref_move)) { \
    RefM(Current->move).ref[1] = RefM(Current->move).ref[0]; RefM(Current->move).ref[0] = (ref_move); }
#define UpdateCheckRef(ref_move) if (T(Current->move) && RefM(Current->move).check_ref[0] != (ref_move)) { \
    RefM(Current->move).check_ref[1] = RefM(Current->move).check_ref[0]; RefM(Current->move).check_ref[0] = (ref_move); }

uint64_t seed = 1;
// int MultiPV[256];
// int pvp;
// int pv_length;
int LastDepth, LastTime, LastValue, LastExactValue, PrevMove, InstCnt;
int64_t LastSpeed;
int PVN, PVHashing = 1, SearchMoves, SMPointer, Previous;
typedef struct {
    int Bad, Change, Singular, Early, FailLow, FailHigh;
} GSearchInfo;
GSearchInfo CurrentSI[1], BaseSI[1];
#ifdef CPU_TIMING
int CpuTiming = 0, UciMaxDepth = 0, UciMaxKNodes = 0, UciBaseTime = 1000, UciIncTime = 5;
int GlobalTime[2] = { 0, 0 };
int GlobalInc[2] = { 0, 0 };
int GlobalTurn = 0;
#define CyclesPerMSec Convert(3400000, int64_t)
#endif
int Aspiration = 1, LargePages = 1;
#define TimeSingTwoMargin 20
#define TimeSingOneMargin 30
#define TimeNoPVSCOMargin 60
#define TimeNoChangeMargin 70
#define TimeRatio 120
#define PonderRatio 120
#define MovesTg 30
#define InfoLag 5000
#define InfoDelay 1000
int64_t InfoTime;
uint16_t SMoves[256];

#define ExclSingle(depth) 8
#define ExclDouble(depth) 16
#define ExclSinglePV(depth) 8
#define ExclDoublePV(depth) 16

// EVAL

const int8_t DistC[8] = {3, 2, 1, 0, 0, 1, 2, 3};
const int8_t RankR[8] = {-3, -2, -1, 0, 1, 2, 3, 4};

const int SeeValue[16] =
    {0, 0, 90, 90, 325, 325, 325, 325, 325, 325, 510, 510, 975, 975,
        30000, 30000};
const int PieceType[16] = {0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5};

#define V(x) (x)

#define ArrayIndex(width,row,column) (((row) * (width)) + (column))
#define Av(x,width,row,column) (*((x) + ArrayIndex(width,row,column)))
#define TrAv(x,w,r,c) Av(x,0,0,(((r)*(2*(w)-(r)+1))/2)+(c))

#define Sa(x,y) Av(x,0,0,y)
#define Ca(x,y) Compose(Av(x,0,0,((y) * 2)),Av(x,0,0,((y) * 2)+1))

#include <stdint.h>
#include <popcntintrin.h>
static inline int popcount(uint64_t x)
{
    return _mm_popcnt_u64(x);
}

#include "tbprobe.h"

// EVAL WEIGHTS

// tuner: start
enum { // tuner: enum
    IMatLinear,
    IMatQuadMe = IMatLinear + 5,
    IMatQuadOpp = IMatQuadMe + 14,
    IBishopPairQuad = IMatQuadOpp + 10,
    IMatSpecial = IBishopPairQuad + 9,
    IPstQuadWeights = IMatSpecial + 20,
    IPstLinearWeights = IPstQuadWeights + 48,
    IPstQuadMixedWeights = IPstLinearWeights + 48,
    IMobilityLinear = IPstQuadMixedWeights + 24,
    IMobilityLog = IMobilityLinear + 8,
    IShelterValue = IMobilityLog + 8,
    IStormQuad = IShelterValue + 15,
    IStormLinear = IStormQuad + 5,
    IStormHof = IStormLinear + 5,
    IPasserQuad = IStormHof + 2,
    IPasserLinear = IPasserQuad + 18,
    IPasserAttDefQuad = IPasserLinear + 18,
    IPasserAttDefLinear = IPasserAttDefQuad + 4,
    IPasserSpecial = IPasserAttDefLinear + 4,
    IIsolated = IPasserSpecial + 4,
    IUnprotected = IIsolated + 10,
    IBackward = IUnprotected + 6,
    IDoubled = IBackward + 4,
    IRookSpecial = IDoubled + 4,
    ITactical = IRookSpecial + 20,
    IKingDefence = ITactical + 12,
    IPawnSpecial = IKingDefence + 8,
    IBishopSpecial = IPawnSpecial + 8,
    IKnightSpecial = IBishopSpecial + 4,
    IPin = IKnightSpecial + 10,
    IKingRay = IPin + 10,
    IKingAttackWeight = IKingRay + 6
};

const int Phase[5] = {
    0, 325, 325, 510, 975
};
#define MaxPhase (16 * Phase[0] + 4 * Phase[1] + 4 * Phase[2] + 4 * Phase[3] + 2 * Phase[4])
#define PhaseMin (2 * Phase[3] + Phase[1] + Phase[2])
#define PhaseMax (MaxPhase - Phase[1] - Phase[2])

const int MatLinear[5] = { // tuner: type=array, var=50, active=0
    3, 0, 3, 19, 0
};
// pawn, knight, bishop, rook, queen
const int MatQuadMe[14] = { // tuner: type=array, var=1000, active=0
    -33, 17, -23, -155, -247,
    15, 296, -105, -83,
    -162, 327, 315,
    -861, -1013
};
const int MatQuadOpp[10] = { // tuner: type=array, var=1000, active=0
    -14, 47, -20, -278,
    35, 39, 49,
    9, -2,
    75
};
const int BishopPairQuad[9] = { // tuner: type=array, var=1000, active=0
    -38, 164, 99, 246, -84, -57, -184, 88, -186
};

enum { MatRB, MatRN, MatQRR, MatQRB, MatQRN, MatQ3, MatBBR, MatBNR, MatNNR, MatM };
const int MatSpecial[20] = { // tuner: type=array, var=30, active=0
    13, -13, 10, -9, 8, 12, 4, 6, 5, 9, -3, -8, -4, 7, 2, 0, 0, -6, 1, 3
};

// piece type (6) * direction (4: h center dist, v center dist, diag dist, rank) * phase (2)
const int PstQuadWeights[48] = { // tuner: type=array, var=100, active=0
    -15, -19, -70, -13, 33, -20, 0, 197, -36, -122, 0, -60, -8, -3, -17, -28,
    -27, -63, -17, -7, 14, 0, -24, -5, -64, -2, 0, -38, -8, 0, 77, 11,
    -67, 3, -4, -92, -2, 12, -13, -42, -62, -84, -175, -42, -2, -17, 40, -19
};
const int PstLinearWeights[48] = { // tuner: type=array, var=500, active=0
    -107, 67, -115, 83, -55, 67, 92, 443, -177, 5, -82, -61, -106, -104, 273, 130,
    0, -145, -105, -58, -99, -37, -133, 14, -185, -43, -67, -53, 53, -65, 174, 134,
    -129, 7, 98, -231, 107, -40, -27, 311, 256, -117, 813, -181, 2, -215, -44, 344
};
// piece type (6) * type (2: h * v, h * rank) * phase (2)
const int PstQuadMixedWeights[24] = { // tuner: type=array, var=100, active=0
    14, -6, 1, -4, -8, -2, 4, -4,
    1, -7, -12, 0, -2, -1, -5, 4,
    5, -10, 0, 4, -2, 5, 4, -2
};
// piece type (4) * phase (2)
const int MobilityLinear[8] = { // tuner: type=array, var=300, active=0
    328, 171, 311, 102, 284, 164, 155, 288
};
const int MobilityLog[8] = { // tuner: type=array, var=500, active=0
    485, -21, 388, 389, -168, 313, 438, -276
};

// file type (3) * distance from 2d rank/open (5)
const int ShelterValue[15] = {  // tuner: type=array, var=10, active=0
    2, 9, 11, 0, 0, 12, 18, 11, 0, 2, 24, 7, 8, 0, 0
};

enum { StormBlockedMul, StormShelterAttMul, StormConnectedMul, StormOpenMul, StormFreeMul };
const int StormQuad[5] = { // tuner: type=array, var=250, active=0
    126, 328, 463, 215, 89
};
const int StormLinear[5] = { // tuner: type=array, var=500, active=0
    83, 156, 438, 321, 12
};
enum { StormHofValue, StormOfValue };
const int StormHof[2] = { // tuner: type=array, var=20, active=1
    0, 22
};

// type (7: general, blocked, free, supported, protected, connected, outside, candidate, clear) * phase (2)
const int PasserQuad[18] = { // tuner: type=array, var=50, active=0
    19, 13, 21, 3, -24, 126, 0, 65, 32, 56, 27, -5, 32, -16, 13, 4, 1, 1
};
const int PasserLinear[18] = { // tuner: type=array, var=200, active=0
    41, 2, 111, 86, 178, 113, 202, 15, -61, 21, 93, 166, 86, 92, 27, 34, -18, -7
};
// type (2: att, def) * scaling (2: linear, log) 
const int PasserAttDefQuad[4] = { // tuner: type=array, var=500, active=0
    191, 51, 83, 19
};
const int PasserAttDefLinear[4] = { // tuner: type=array, var=500, active=0
    634, 4, 233, 66
};
enum { PasserOnePiece, PasserOpKingControl, PasserOpMinorControl, PasserOpRookBlock };
const int PasserSpecial[4] = { // tuner: type=array, var=100, active=0
    0, 0, 0, 13
};

enum { IsolatedOpen, IsolatedClosed, IsolatedBlocked, IsolatedDoubledOpen, IsolatedDoubledClosed };
const int Isolated[10] = { // tuner: type=array, var=10, active=0
    6, 6, 8, 2, -8, 0, -1, 10, 7, 9
};
enum { UpBlocked, PasserTarget, ChainRoot };
const int Unprotected[6] = { // tuner: type=array, var=10, active=0
    4, 5, -5, -1, 9, -1
};
enum { BackwardOpen, BackwardClosed };
const int Backward[4] = { // tuner: type=array, var=10, active=0
    17, 10, 4, 1
};
enum { DoubledOpen, DoubledClosed };
const int Doubled[4] = { // tuner: type=array, var=10, active=0
    3, 0, 1, 0
};

enum { RookHof, RookHofWeakPAtt, RookOf, RookOfOpen, RookOfMinorFixed, RookOfMinorHaging, RookOfKingAtt, Rook7th, Rook7thK8th, Rook7thDoubled };
const int RookSpecial[20] = { // tuner: type=array, var=10, active=0
    8, 0, 2, 0, 11, 8, -1, 2, -1, -1, 14, -1, 5, -5, -5, 0, -6, 8, -7, 31
};

enum { TacticalMajorPawn, TacticalMinorPawn, TacticalMajorMinor, TacticalMinorMinor, TacticalThreat, TacticalDoubleThreat };
const int Tactical[12] = { // tuner: type=array, var=20, active=0
    -1, 5, 0, 5, 11, 29, 23, 32, 19, 11, 41, 12
};

enum { KingDefKnight, KingDefBishop, KingDefRook, KingDefQueen };
const int KingDefence[8] = { // tuner: type=array, var=5, active=0
    2, 0, 0, 1, 0, 0, 4, 0
};

enum { PawnChainLinear, PawnChain, PawnBlocked, PawnFileSpan };
const int PawnSpecial[8] = { // tuner: type=array, var=10, active=0
    11, 9, 9, 4, 0, 9, 1, 1
};

enum { BishopNonForwardPawn, BishopPawnBlock };
const int BishopSpecial[4] = { // tuner: type=array, var=5, active=0
    0, 0, 0, 3
};

const uint64_t Outpost[2] = { Convert(0x00007E7E3C000000, uint64_t), Convert(0x0000003C7E7E0000, uint64_t) };
enum { KnightOutpost, KnightOutpostProtected, KnightOutpostPawnAtt, KnightOutpostBishopAtt, KnightOutpostKingAtt };
const int KnightSpecial[10] = { // tuner: type=array, var=10, active=0
    11, 7, 23, 0, 13, 6, 1, 5, 26, 6
};

enum { WeakPin, StrongPin, ThreatPin, SelfPawnPin, SelfPiecePin };
const int Pin[10] = { // tuner: type=array, var=20, active=0
    21, 39, 6, 80, 45, 29, 8, 9, 48, 27
};

enum { QKingRay, RKingRay, BKingRay };
const int KingRay[6] = { // tuner: type=array, var=20, active=0
    4, 8, -4, 11, 11, -3
};

const int KingAttackWeight[7] = { // tuner: type=array, var=20, active=0
    17, 14, 22, 45, 48, 64, 64
};
#define KingNAttack Compose(1, Av(KingAttackWeight, 0, 0, 0))
#define KingBAttack Compose(1, Av(KingAttackWeight, 0, 0, 1))
#define KingRAttack Compose(1, Av(KingAttackWeight, 0, 0, 2))
#define KingQAttack Compose(1, Av(KingAttackWeight, 0, 0, 3))
#define KingAttack Compose(1, 0)
#define KingAttackSquare Av(KingAttackWeight, 0, 0, 4)
#define KingNoMoves Av(KingAttackWeight, 0, 0, 5)
#define KingShelterQuad Av(KingAttackWeight, 0, 0, 6)

const int KingAttackScale[16] = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 64, 64, 64, 64, 64, 64, 64 };
// tuner: stop

// END EVAL WEIGHTS

typedef struct {
    GPosData Position[1];
    uint64_t stack[100];
    uint16_t killer[16][2];
    int sp, date;
} GPos;

#define FlagClaimed (1 << 1)
#define FlagFinished (1 << 2)

#define STOPPED 0

typedef struct
{
    volatile bool stop;
    bool newGame;
    size_t nodes;
    size_t tbHits;
    int pid;
    int id;
    int depth;
    int selDepth;
    int bestMove;
    int bestScore;
#ifdef WINDOWS
    GEvent goEvent;
#endif
    int PV[];
} GThreadInfo;
#define MAX_PV_LEN      ((PAGE_SIZE - sizeof(GThreadInfo)) / sizeof(int))

typedef struct
{
    unsigned numThreads;
    unsigned syzygyProbeDepth;
    size_t hashSize;
    unsigned parentPid;
} GSettings;

typedef struct
{
    int state;
    int init;
    uint16_t date;
    GMutex mutex;
#ifdef LINUX
    GCondVar goCondVar;
    GCondVar initCondVar;
#endif
#ifdef WINDOWS
    GEvent initEvent;
#endif
    unsigned rootDepth;
    unsigned depthLimit;
    uint64_t startTime;
    uint64_t softTimeLimit;
    uint64_t hardTimeLimit;
    volatile bool ponder;
    GBoard rootBoard;
    GData rootData;
    unsigned rootSp;
    uint64_t rootStack[1024];
} GSharedInfo;

static const uint32_t Schedule[] =
{
    0x00FFFFFF,
    0x00555555,
    0x00AAAAAA,
    0x00333333,
    0x00666666,
    0x00CCCCCC,
    0x001C71C7,
    0x0038E38E,
    0x0071C71C,
    0x00E38E38,
    0x00C71C71,
    0x008E38E3,
    0x000F0F0F,
    0x001E1E1E,
    0x003C3C3C,
    0x00787878,
    0x00F0F0F0,
    0x00E1E1E1,
    0x00C3C3C3,
    0x00878787
};

extern GThreadInfo INFO[];
extern GSettings   SETTINGS[];
extern GSharedInfo SHARED[];
extern GGlobalData DATA[];
extern GPawnEntry  PAWNHASH[];
extern GPVEntry    PVHASH[];
#ifndef WINDOWS
extern GEntry      HASH[];
#else
#define HASH        ((GEntry *)0x8000000)
#endif

jmp_buf CheckJump;

inline int lsb(uint64_t x);
inline int msb(uint64_t x);
inline int popcount(uint64_t x);
inline int MinF(int x, int y);
inline int MaxF(int x, int y);
inline double MinF(double x, double y);
inline double MaxF(double x, double y);
uint16_t rand16(void);
uint64_t rand64(void);
void init_search(int clear_hash);
void setup_board();
const char *get_board(const char fen[]);
void move_to_string(int move, char string[]);
int move_from_string(char string[]);
void pick_pv();
template <bool me> void do_move(int move);
template <bool me> void undo_move(int move);
void do_null();
void undo_null();
inline void evaluate();
template <bool me> int is_legal(int move);
template <bool me> int is_check(int move);
void hash_high(int value, int depth);
void hash_low(int move, int value, int depth);
void hash_exact(int move, int value, int depth, int exclusion, int ex_depth, int knodes);
inline int pick_move();
template <bool me, bool root> int get_move();
template <bool me> int see(int move, int margin);
template <bool me> void gen_root_moves();
template <bool me> int * gen_captures(int * list);
template <bool me> int * gen_evasions(int * list);
void mark_evasions(int * list);
template <bool me> int * gen_quiet_moves(int * list);
template <bool me> int * gen_checks(int * list);
template <bool me> int * gen_delta_moves(int * list);
template <bool me, bool pv> int q_search(int alpha, int beta, int depth, int flags);
template <bool me, bool pv> int q_evasion(int alpha, int beta, int depth, int flags);
template <bool me, bool exclusion> int search(int beta, int depth, int flags);
template <bool me, bool exclusion> int search_evasion(int beta, int depth, int flags);
template <bool me, bool root> int pv_search(int alpha, int beta, int depth, int flags);
template <bool me> void root();
template <bool me> int multipv(int depth);
void send_pv(int depth, int alpha, int beta, int score);
static void send_pv(const int *PV, size_t nodes, size_t tbHits, int depth,
    int selDepth, int bestScore, int bestMove, uint64_t startTime);
void send_multipv(int depth, int curr_number);
static void send_curr_move(int move, int cnt);
static void send_best_move();
static void send_best_move(const int *PV, size_t nodes, size_t tbHits,
    int bestScore, int bestMove, uint64_t startTime);
int64_t get_time();
int time_to_stop(GSearchInfo * SI, int time, int searching);
static void emergency_stop(void);
void uci();
void send_position(GPos * Pos);
void retrieve_position(GPos * Pos, int copy_stack);
static void nuke_children(void);
static void create_children(size_t numThreads, size_t syzygyProbeDepth,
    size_t hashSize, const char *tbPath);
static void reset(size_t numThreads, size_t syzygyProbeDepth, size_t hashSize,
    const char *tbPath);

#include "tbprobe.h"

#ifdef LINUX
#include "Linux.cpp"
#endif

#ifdef WINDOWS
#include "Windows.cpp"
#endif

#include "data.c"

#define HASH_SIZE(n)    ((n) == 0? (1 << 20): Bit(msb((size_t)(n) * (1 << 20))))

#ifndef W32_BUILD
inline int lsb(uint64_t x) {
    register unsigned long long y;
    __asm__("bsfq %1, %0": "=r"(y): "rm"(x));
    return y;
}

inline int msb(uint64_t x) {
    register unsigned long long y;
    __asm__("bsrq %1, %0": "=r"(y): "rm"(x));
    return y;
}

//inline int popcount(uint64_t x) {
//    x = x - ((x >> 1) & 0x5555555555555555);
//    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
//    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
//    return (x * 0x0101010101010101) >> 56;
//}
#else
inline int lsb(uint64_t x) {
    _asm {
        mov eax, dword ptr x[0]
            test eax, eax
            jz l_high
            bsf eax, eax
            jmp l_ret
        l_high : bsf eax, dword ptr x[4]
                 add eax, 20h
             l_ret :
    }
}

inline int msb(uint64_t x) {
    _asm {
        mov eax, dword ptr x[4]
            test eax, eax
            jz l_low
            bsr eax, eax
            add eax, 20h
            jmp l_ret
        l_low : bsr eax, dword ptr x[0]
            l_ret :
    }
}

//inline int popcount(uint64_t x) {
//    unsigned int x1, x2;
//    x1 = (unsigned int)(x & 0xFFFFFFFF);
//    x1 -= (x1 >> 1) & 0x55555555;
//    x1 = (x1 & 0x33333333) + ((x1 >> 2) & 0x33333333);
//    x1 = (x1 + (x1 >> 4)) & 0x0F0F0F0F;
//    x2 = (unsigned int)(x >> 32);
//    x2 -= (x2 >> 1) & 0x55555555;
//    x2 = (x2 & 0x33333333) + ((x2 >> 2) & 0x33333333);
//    x2 = (x2 + (x2 >> 4)) & 0x0F0F0F0F;
//    return ((x1 * 0x01010101) >> 24) + ((x2 * 0x01010101) >> 24);
//}

inline int popcount(uint64_t x) {
    return __popcount((int)x) + __popcount(x >> 32);
}
#endif

inline int MinF(int x, int y) { return Min(x, y); }
inline int MaxF(int x, int y) { return Max(x, y); }
inline double MinF(double x, double y) { return Min(x, y); }
inline double MaxF(double x, double y) { return Max(x, y); }

uint16_t rand16(void)
{
    seed = (seed * Convert(6364136223846793005,uint64_t)) + Convert(1442695040888963407,uint64_t);
    return Convert((seed >> 32) & 0xFFFF,uint16_t);
}

uint64_t rand64(void)
{
    uint64_t key = Convert(rand16(),uint64_t); key <<= 16;
    key |= Convert(rand16(),uint64_t); key <<= 16;
    key |= Convert(rand16(),uint64_t); key <<= 16;
    return key | Convert(rand16(),uint64_t);
}

static char SyzygyPath[PATH_MAX] = {'\0'};
#define MAX_THREADS     (PAGE_SIZE / sizeof(GThreadInfo *))
static GThreadInfo *THREADS[MAX_THREADS] = {NULL};

static void stop(void)
{
    for (unsigned i = 0; i < SETTINGS->numThreads; i++)
        THREADS[i]->stop = true;
}

static void go(void)
{
    if (popcount(PieceAll) <= TB_LARGEST)
    {
        unsigned res = tb_probe_root(Piece(White), Piece(Black),
            King(White) | King(Black),
            Queen(White) | Queen(Black),
            Rook(White) | Rook(Black),
            Bishop(White) | Bishop(Black),
            Knight(White) | Knight(Black),
            Pawn(White) | Pawn(Black),
            Current->ply,
            Current->castle_flags,
            Current->ep_square,
            (Current->turn == White), NULL);
        if (res != TB_RESULT_FAILED)
        {
            int bestScore = TbValues[TB_GET_WDL(res)];
            int flags = 0;
            unsigned to = TB_GET_TO(res);
            switch (TB_GET_PROMOTES(res))
            {
                case TB_PROMOTES_QUEEN:
                    flags |= FlagPQueen; break;
                case TB_PROMOTES_ROOK:
                    flags |= FlagPRook; break;
                case TB_PROMOTES_BISHOP:
                    flags |= (Bit(to) & LightArea? FlagPLight: FlagPDark);
                case TB_PROMOTES_KNIGHT:
                    flags |= FlagPKnight; break;
                default:
                    break;
            }
            int bestMove = (TB_GET_FROM(res) << 6) | to | flags;
            char movStr[16];
            move_to_string(bestMove, movStr);
            char line[IOSIZE];
            int len = snprintf(line, sizeof(line)-1, "info depth 1 seldepth 1 "
                "score cp %d nodes 1 tbhits 1 pv %s\n", bestScore, movStr);
            if (len > 0 && len < sizeof(line)-1)
                put_line(line, len);
            len = snprintf(line, sizeof(line)-1, "bestmove %s\n", movStr);
            if (len > 0 && len < sizeof(line)-1)
                put_line(line, len);
            return;
        }
    }

    SHARED->date++;
    assert(PVN == 1);       // Multi-PV NYI.
    memcpy(&SHARED->rootBoard, Board, sizeof(GBoard));
    memcpy(&SHARED->rootData, Current, sizeof(GData));
    memcpy(&SHARED->rootStack, Stack, sp * sizeof(uint64_t));
    SHARED->rootSp = sp;
    for (int i = 0; i < SETTINGS->numThreads; i++)
    {
        THREADS[i]->stop      = false;
        THREADS[i]->nodes     = 0;
        THREADS[i]->tbHits    = 0;
        THREADS[i]->depth     = 0;
        THREADS[i]->selDepth  = 0;
        THREADS[i]->bestMove  = 0;
        THREADS[i]->bestScore = 0;
        THREADS[i]->PV[0]     = 0;
    }
    SHARED->state = SETTINGS->numThreads;
#ifdef LINUX
    cond_broadcast(&SHARED->goCondVar);
#endif
#ifdef WINDOWS
    for (int i = 0; i < SETTINGS->numThreads; i++)
        event_signal(&THREADS[i]->goEvent);
#endif
}

static void wait_for_go(void)
{
#ifdef LINUX
    cond_wait(&SHARED->goCondVar, &SHARED->mutex);
#endif
#ifdef WINDOWS
    event_wait(&INFO->goEvent, &SHARED->mutex);
    mutex_lock(&SHARED->mutex);
#endif
}

static void wait_for_stop(void)
{
    const uint64_t maxStopTime = 300;       // Max 300ms to stop all threads;
    mutex_unlock(&SHARED->mutex);
    uint64_t stopTime = get_time(), currTime = stopTime;
    while (true)
    {
        msleep(1);
        currTime = get_time();
        if (mutex_lock(&SHARED->mutex,
                (currTime - stopTime > maxStopTime? 5:
                    maxStopTime - (currTime - stopTime))))
        {
            emergency_stop();
            return;
        }
        if (SHARED->state == STOPPED)
            break;
        if (currTime - stopTime > maxStopTime)
        {
            emergency_stop();
            return;
        }
        mutex_unlock(&SHARED->mutex);
    }
}

static void emergency_stop(void)
{
    // Something is wrong -- threads are not stopping, or have crashed.
    // It is unspecified if the global mutex is locked.  We no longer care
    // about races.
    // Reset everthing.  We may lose on time, but so be it.
    bool go = (SHARED->state != STOPPED);
    int bestMove = 0;
    for (unsigned i = 0; go && bestMove == 0 && i < SETTINGS->numThreads; i++)
        bestMove = THREADS[i]->bestMove;
    // Log this incident:
    log("warning: threads crashed or deadlocked; initiating emergency reset\n");
    reset(SETTINGS->numThreads, SETTINGS->syzygyProbeDepth, SETTINGS->hashSize, 
        SyzygyPath);
    mutex_lock(&SHARED->mutex);
    if (!go)
        return;
    char line[32], moveStr[16];
    move_to_string(bestMove, moveStr);
    int len = snprintf(line, sizeof(line)-1, "bestmove %s\n", moveStr);
    if (len > 0 && len < sizeof(line)-1)
        put_line(line, len);
    else
    {
        // Giveup:
        char reply[] = "bestmove 0\n";
        put_line(reply, sizeof(reply));
    }
}

static inline void check_for_stop(void)
{
    if (!INFO->stop)
        return;
    longjmp(CheckJump, 1);
}

void init_search(int clear_hash) {
    memset(History,1,16 * 64 * sizeof(int16_t));
    memset(Delta,0,16 * 4096 * sizeof(int16_t));
    memset(Ref,0,16 * 64 * sizeof(GRef));
    memset(Data + 1, 0, 127 * sizeof(GData));
    if (clear_hash) {
        SHARED->date = 1;
        memset(HASH,0,SETTINGS->hashSize);
        memset(PVHASH,0,pvHashSize);
        memset(PAWNHASH,0,pawnHashSize);
    }
    hash_size = SETTINGS->hashSize / sizeof(GEntry);
    assert((hash_size & (hash_size - 1)) == 0);
    hash_mask = hash_size - 4;

    get_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    LastTime = LastValue = LastExactValue = InstCnt = 0;
    LastSpeed = 0;
    PVN = 1;
    SearchMoves = 0;
    LastDepth = 128;
    memset(CurrentSI,0,sizeof(GSearchInfo));
    memset(BaseSI,0,sizeof(GSearchInfo));
#ifdef CPU_TIMING
    GlobalTime[GlobalTurn] = UciBaseTime;
    GlobalInc[GlobalTurn] = UciIncTime;
#endif
}

void setup_board() {
    int i;
    uint64_t occ;
    GEntry * Entry;
    GPVEntry * PVEntry;

    occ = 0;
    sp = 0;
    SHARED->date++;
    if (SHARED->date > 0x8000) { // musn't ever happen
        SHARED->date = 2;
        // now GUI must wait for readyok... we have plenty of time :)
        for (Entry = HASH, i = 0; i < hash_size; i++, Entry++) Entry->date = 1;
        for (PVEntry = PVHASH, i = 0; i < pv_hash_size; i++, PVEntry++) PVEntry->date = 1;
    }
    Current->material = 0;
    Current->pst = 0;
    Current->key = DATA->PieceKey[0][0];
    if (Current->turn) Current->key ^= DATA->TurnKey;
    Current->key ^= DATA->CastleKey[Current->castle_flags];
    if (Current->ep_square) Current->key ^= DATA->EPKey[File(Current->ep_square)];
    Current->pawn_key = 0;
    Current->pawn_key ^= DATA->CastleKey[Current->castle_flags];
    for (i = 0; i < 16; i++) BB(i) = 0;
    for (i = 0; i < 64; i++) {
        if (Square(i)) {
            Add(BB(Square(i)),i);
            Add(BB(Square(i) & 1),i);
            Add(occ,i);
            Current->key ^= DATA->PieceKey[Square(i)][i];
            if (Square(i) < WhiteKnight) Current->pawn_key ^= DATA->PieceKey[Square(i)][i];
            if (Square(i) < WhiteKing) Current->material += MatCode[Square(i)];
            else Current->pawn_key ^= DATA->PieceKey[Square(i)][i];
            Current->pst += Pst(Square(i),i);
        }
    }
    if (popcount(BB(WhiteKnight)) > 2 || popcount(BB(WhiteLight)) > 1 || popcount(BB(WhiteDark)) > 1 
        || popcount(BB(WhiteRook)) > 2 || popcount(BB(WhiteQueen)) > 2) Current->material |= FlagUnusualMaterial; 
    if (popcount(BB(BlackKnight)) > 2 || popcount(BB(BlackLight)) > 1 || popcount(BB(BlackDark)) > 1 
        || popcount(BB(BlackRook)) > 2 || popcount(BB(BlackQueen)) > 2) Current->material |= FlagUnusualMaterial; 
    Current->capture = 0;
    Current->killer[1] = Current->killer[2] = 0;
    Current->ply = 0;
    Stack[sp] = Current->key;
}

const char *get_board(const char fen[]) {
    int pos, i, j;
    unsigned char c;

    Current = Data;
    memset(Board,0,sizeof(GBoard));
    memset(Current,0,sizeof(GData));
    pos = 0;
    c = fen[pos];
    while (c == ' ') c = fen[++pos];
    for (i = 56; i >= 0; i -= 8) {
        for (j = 0; j <= 7; ) {
            if (c <= '8') j += c - '0';
            else {
                Square(i+j) = DATA->PieceFromChar[c];
                if (Even(SDiag(i+j)) && (Square(i+j)/2) == 3) Square(i+j) += 2;
                j++;
            }
            c = fen[++pos];
        }
        c = fen[++pos];
    }
    if (c == 'b') Current->turn = 1;
    c = fen[++pos]; c = fen[++pos];
    if (c == '-') c = fen[++pos];
    if (c == 'K') { Current->castle_flags |= CanCastle_OO; c = fen[++pos]; }
    if (c == 'Q') { Current->castle_flags |= CanCastle_OOO; c = fen[++pos]; }
    if (c == 'k') { Current->castle_flags |= CanCastle_oo; c = fen[++pos]; }
    if (c == 'q') { Current->castle_flags |= CanCastle_ooo; c = fen[++pos]; }
    c = fen[++pos];
    if (c == '-') c = fen[++pos];
    else
    {
        i = c + fen[++pos] * 8 - 489;
        j = i ^ 8;
        if (Square(i) != 0) i = 0;
        else if (Square(j) != (3 - Current->turn)) i = 0;
        else if (Square(j-1) != (Current->turn+2) && Square(j+1) != (Current->turn+2)) i = 0;
        Current->ep_square = i;
    }
    c = fen[++pos];
    char numStr[4] = {'\0'};
    numStr[0] = c;
    for (i = 1; i < 3 && isdigit(c = fen[++pos]); i++)
        numStr[i] = c;
    Current->ply = atoi(numStr);
    memset(numStr, 0, sizeof(numStr));
    c = fen[++pos];
    numStr[0] = c;
    for (i = 1; i < 3 && isdigit(c = fen[++pos]); i++)
        numStr[i] = c;
    SHARED->rootDepth = atoi(numStr);
    setup_board();
    return fen + pos;
}

static void dump_board(void)
{
    for (int i = 56; i >= 0; i -= 8)
        for (int j = 0; j < 8; j++)
        {
            int piece = Square(i+j);
            switch (piece)
            {
                case WhiteKing: putc('K', stderr); break;
                case BlackKing: putc('k', stderr); break;
                case WhiteQueen: putc('Q', stderr); break;
                case BlackQueen: putc('q', stderr); break;
                case WhiteRook: putc('R', stderr); break;
                case BlackRook: putc('r', stderr); break;
                case WhiteLight: putc('B', stderr); break;
                case BlackLight: putc('b', stderr); break;
                case WhiteDark: putc('B', stderr); break;
                case BlackDark: putc('b', stderr); break;
                case WhiteKnight: putc('N', stderr); break;
                case BlackKnight: putc('n', stderr); break;
                case WhitePawn: putc('P', stderr); break;
                case BlackPawn: putc('p', stderr); break;
                default: putc('.', stderr); break;
            }
            putc(' ', stderr);
            if (j+1 == 8) putc('\n', stderr);
        }
    putc('\n', stderr);
}

inline GEntry * probe_hash() {
    for (GEntry * Entry = HASH + (High32(Current->key) & hash_mask); Entry < (HASH + (High32(Current->key) & hash_mask)) + 4; Entry++) if (Low32(Current->key) == Entry->key) {
        Entry->date = SHARED->date;
        return Entry;
    }
    return NULL;
}

inline GPVEntry * probe_pv_hash() {
    for (GPVEntry * PVEntry = PVHASH + (High32(Current->key) & pv_hash_mask); PVEntry < PVHASH + (High32(Current->key) & pv_hash_mask) + pv_cluster_size; PVEntry++) if (Low32(Current->key) == PVEntry->key) {
        PVEntry->date = SHARED->date;
        return PVEntry;
    }
    return NULL;
}

void move_to_string(int move, char string[]) { 
    int pos = 0;
    string[pos++] = ((move >> 6) & 7) + 'a';
    string[pos++] = ((move >> 9) & 7) + '1';
    string[pos++] = (move & 7) + 'a';
    string[pos++] = ((move >> 3) & 7) + '1';
    if (IsPromotion(move)) {
        if ((move & 0xF000) == FlagPQueen)  string[pos++] = 'q';
        else if ((move & 0xF000) == FlagPRook)   string[pos++] = 'r';
        else if ((move & 0xF000) == FlagPLight || (move & 0xF000) == FlagPDark) string[pos++] = 'b';
        else if ((move & 0xF000) == FlagPKnight) string[pos++] = 'n';
    }
    string[pos] = 0;
}

int move_from_string(char string[]) { 
    int from, to, move;
    from = ((string[1] - '1') * 8) + (string[0] - 'a');
    to  = ((string[3] - '1') * 8) + (string[2] - 'a');
    move = (from << 6) | to;
    if (Board->square[from] >= WhiteKing && Abs(to - from) == 2) move |= FlagCastling;
    if (T(Current->ep_square) && to == Current->ep_square) move |= FlagEP;
    if (string[4] != 0) {
        if (string[4] == 'q') move |= FlagPQueen;
        else if (string[4] == 'r') move |= FlagPRook;
        else if (string[4] == 'b') {
            if (Odd(to ^ Rank(to))) move |= FlagPLight;
            else move |= FlagPDark;
        } else if (string[4] == 'n') move |= FlagPKnight;
    }
    return move;
}

void pick_pv(unsigned pvPtr, unsigned pvLen)
{
    GEntry *Entry;
    GPVEntry *PVEntry;
    int i, depth, move;
    if (pvPtr >= Min(pvLen, MAX_PV_LEN))
    {
        INFO->PV[pvPtr] = 0;
        return;
    }
    move = 0;
    depth = -256;
    if ((Entry = probe_hash()) && T(Entry->move) && Entry->low_depth > depth)
    {
        depth = Entry->low_depth;
        move = Entry->move;
    }
    if ((PVEntry = probe_pv_hash()) && T(PVEntry->move) && PVEntry->depth > depth)
    {
        depth = PVEntry->depth;
        move = PVEntry->move;
    }
    evaluate();
    if (Current->att[Current->turn] & King(Current->turn ^ 1))
        INFO->PV[pvPtr] = 0;
    else if (move && (Current->turn ? is_legal<1>(move) : is_legal<0>(move)))
    {
        INFO->PV[pvPtr] = move;
        pvPtr++;
        if (Current->turn) do_move<1>(move); else do_move<0>(move);
        if (Current->ply >= 100) goto finish;
        for (i = 4; i <= Current->ply; i+= 2)
        {
            if (Stack[sp-i] == Current->key)
            {
                INFO->PV[pvPtr] = 0;
                goto finish;
            }
        }
        pick_pv(pvPtr, pvLen);
finish:
        if (Current->turn ^ 1) undo_move<1>(move); else undo_move<0>(move);
    }
    else
        INFO->PV[pvPtr] = 0;
}

template <bool me> int draw_in_pv() {
    if ((Current - Data) >= 126) return 1;
    if (Current->ply >= 100) return 1;
    for (int i = 4; i <= Current->ply; i += 2) if (Stack[sp - i] == Current->key) return 1;
    if (GPVEntry * PVEntry = probe_pv_hash()) {
        if (!PVEntry->value) return 1;
        if (int move = PVEntry->move) {
            do_move<me>(move);
            int value = draw_in_pv<opp>();
            undo_move<me>(move);
            return value;
        }
    }
    return 0;
}

template <bool me> void do_move(int move) {
    GEntry * Entry;
    GPawnEntry * PawnEntry;
    int from, to, piece, capture;
    GData * Next;
    uint64_t u, mask_from, mask_to;

    to = To(move);
    Next = Current + 1;
    Next->ep_square = 0;
    capture = Square(to);
    if (F(capture)) {
        Next->capture = 0;
        goto non_capture;
    }
    from = From(move);
    piece = Square(from);
    Next->turn = opp;
    Next->capture = capture;
    Square(from) = 0;
    Square(to) = piece;
    Next->piece = piece;
    mask_from = Bit(from);
    mask_to = Bit(to);
    BB(piece) ^= mask_from;
    Piece(me) ^= mask_from;
    BB(capture) ^= mask_to;
    Piece(opp) ^= mask_to;
    BB(piece) |= mask_to;
    Piece(me) |= mask_to;
    Next->castle_flags = Current->castle_flags & DATA->UpdateCastling[to] & DATA->UpdateCastling[from];
    Next->pst = Current->pst + Pst(piece,to) - Pst(piece,from) - Pst(capture,to);
    Next->key = Current->key ^ DATA->PieceKey[piece][from] ^ DATA->PieceKey[piece][to] ^ DATA->PieceKey[capture][to] ^ DATA->CastleKey[Current->castle_flags] ^ DATA->CastleKey[Next->castle_flags];
    if (capture != IPawn(opp)) Next->pawn_key = Current->pawn_key ^ DATA->CastleKey[Current->castle_flags] ^ DATA->CastleKey[Next->castle_flags]; // of course we can put a lot of operations inside this "if {}" but the speedup won't be worth the effort
    else Next->pawn_key = Current->pawn_key ^ DATA->PieceKey[IPawn(opp)][to] ^ DATA->CastleKey[Current->castle_flags] ^ DATA->CastleKey[Next->castle_flags];
    Next->material = Current->material - MatCode[capture];
    if (T(Current->material & FlagUnusualMaterial) && capture >= WhiteKnight) {
        if (popcount(BB(WhiteQueen)) <= 2 && popcount(BB(BlackQueen)) <= 2) {
            if (popcount(BB(WhiteLight)) <= 1 && popcount(BB(BlackLight)) <= 1 && popcount(BB(WhiteKnight)) <= 2
                && popcount(BB(BlackKnight)) <= 2 && popcount(BB(WhiteRook)) <= 2 && popcount(BB(BlackRook)) <= 2)
                Next->material ^= FlagUnusualMaterial;
        }
    }
    if (piece == IPawn(me)) {
        Next->pawn_key ^= DATA->PieceKey[IPawn(me)][from] ^ DATA->PieceKey[piece][to];
        if (IsPromotion(move)) {
            piece = Promotion(move,me);
            Square(to) = piece;
            Next->material += MatCode[piece] - MatCode[IPawn(me)];
            if (piece < WhiteRook) {
                if (piece >= WhiteLight && T(BB(piece))) Next->material |= FlagUnusualMaterial;
                if (Multiple(BB(piece))) Next->material |= FlagUnusualMaterial;
            } else if (Multiple(BB(piece))) Next->material |= FlagUnusualMaterial;
            Pawn(me) ^= mask_to;
            BB(piece) |= mask_to;
            Next->pst += Pst(piece,to) - Pst(IPawn(me),to);
            Next->key ^= DATA->PieceKey[piece][to] ^ DATA->PieceKey[IPawn(me)][to];
            Next->pawn_key ^= DATA->PieceKey[IPawn(me)][to];
        }
        PawnEntry = PAWNHASH + (Next->pawn_key & pawn_hash_mask);
        prefetch((char *)PawnEntry,_MM_HINT_NTA);
    } else if (piece >= WhiteKing) {
        Next->pawn_key ^= DATA->PieceKey[piece][from] ^ DATA->PieceKey[piece][to];
        PawnEntry = PAWNHASH + (Next->pawn_key & pawn_hash_mask);
        prefetch((char *)PawnEntry,_MM_HINT_NTA);
    } else if (capture < WhiteKnight) {
        PawnEntry = PAWNHASH + (Next->pawn_key & pawn_hash_mask);
        prefetch((char *)PawnEntry,_MM_HINT_NTA);
    }
    if (F(Next->material & FlagUnusualMaterial)) prefetch((char *)(DATA->Material + Next->material), _MM_HINT_NTA); 
    if (Current->ep_square) Next->key ^= DATA->EPKey[File(Current->ep_square)];
    Next->turn = Current->turn ^ 1;
    Next->key ^= DATA->TurnKey;
    Entry = HASH + (High32(Next->key) & hash_mask);
    prefetch((char *)Entry,_MM_HINT_NTA);
    Next->ply = 0;
    goto finish;
non_capture:
    from = From(move);
    Next->ply = Current->ply + 1;
    piece = Square(from);
    Square(from) = 0;
    Square(to) = piece;
    Next->piece = piece;
    mask_from = Bit(from);
    mask_to = Bit(to);
    BB(piece) ^= mask_from;
    Piece(me) ^= mask_from;
    BB(piece) |= mask_to;
    Piece(me) |= mask_to;
    Next->castle_flags = Current->castle_flags & DATA->UpdateCastling[to] & DATA->UpdateCastling[from];
    Next->pst = Current->pst + Pst(piece,to) - Pst(piece,from);
    Next->key = Current->key ^ DATA->PieceKey[piece][to] ^ DATA->PieceKey[piece][from] ^ DATA->CastleKey[Current->castle_flags] ^ DATA->CastleKey[Next->castle_flags];
    Next->material = Current->material;
    if (piece == IPawn(me)) {
        Next->ply = 0;
        Next->pawn_key = Current->pawn_key ^ DATA->PieceKey[IPawn(me)][to] ^ DATA->PieceKey[IPawn(me)][from] ^ DATA->CastleKey[Current->castle_flags] ^ DATA->CastleKey[Next->castle_flags];
        if (IsEP(move)) {
            Square(to ^ 8) = 0;
            u = Bit(to ^ 8);
            Next->key ^= DATA->PieceKey[IPawn(opp)][to ^ 8];
            Next->pawn_key ^= DATA->PieceKey[IPawn(opp)][to ^ 8];
            Next->pst -= Pst(IPawn(opp),to ^ 8);
            Pawn(opp) &= ~u;
            Piece(opp) &= ~u;
            Next->material -= MatCode[IPawn(opp)];
        } else if (IsPromotion(move)) {
            piece = Promotion(move,me);
            Square(to) = piece;
            Next->material += MatCode[piece] - MatCode[IPawn(me)];
            if (piece < WhiteRook) {
                if (piece >= WhiteLight && T(BB(piece))) Next->material |= FlagUnusualMaterial;
                if (Multiple(BB(piece))) Next->material |= FlagUnusualMaterial;
            } else if (Multiple(BB(piece))) Next->material |= FlagUnusualMaterial;
            Pawn(me) ^= mask_to;
            BB(piece) |= mask_to;
            Next->pst += Pst(piece,to) - Pst(IPawn(me),to);
            Next->key ^= DATA->PieceKey[piece][to] ^ DATA->PieceKey[IPawn(me)][to];
            Next->pawn_key ^= DATA->PieceKey[IPawn(me)][to];
        } else if ((to ^ from) == 16) {
            if (DATA->PAtt[me][(to + from) >> 1] & Pawn(opp)) {
                Next->ep_square = (to + from) >> 1;
                Next->key ^= DATA->EPKey[File(Next->ep_square)];
            }
        }
        PawnEntry = PAWNHASH + (Next->pawn_key & pawn_hash_mask);
        prefetch((char *)PawnEntry,_MM_HINT_NTA);
    } else {
        if (piece < WhiteKing) Next->pawn_key = Current->pawn_key ^ DATA->CastleKey[Current->castle_flags] ^ DATA->CastleKey[Next->castle_flags];
        else {
            Next->pawn_key = Current->pawn_key ^ DATA->PieceKey[piece][to] ^ DATA->PieceKey[piece][from] ^ DATA->CastleKey[Current->castle_flags] ^ DATA->CastleKey[Next->castle_flags];
            PawnEntry = PAWNHASH + (Next->pawn_key & pawn_hash_mask);
            prefetch((char *)PawnEntry,_MM_HINT_NTA);
        }
        if (IsCastling(move)) {
            int rold, rnew;
            Next->ply = 0;
            if (to == 6) {
                rold = 7; 
                rnew = 5;
            } else if (to == 2) {
                rold = 0; 
                rnew = 3;
            } else if (to == 62) {
                rold = 63;
                rnew = 61;
            } else if (to == 58) {
                rold = 56; 
                rnew = 59;
            }
            Add(mask_to,rnew);
            Square(rold) = 0;
            Square(rnew) = IRook(me);
            BB(IRook(me)) ^= Bit(rold);
            Piece(me) ^= Bit(rold);
            BB(IRook(me)) |= Bit(rnew);
            Piece(me) |= Bit(rnew);
            Next->pst += Pst(IRook(me),rnew) - Pst(IRook(me),rold);
            Next->key ^= DATA->PieceKey[IRook(me)][rnew] ^ DATA->PieceKey[IRook(me)][rold];
        }
    }

    if (Current->ep_square) Next->key ^= DATA->EPKey[File(Current->ep_square)];
    Next->turn = opp;
    Next->key ^= DATA->TurnKey;
    Entry = HASH + (High32(Next->key) & hash_mask);
    prefetch((char *)Entry,_MM_HINT_NTA);
finish:
    sp++;
    Stack[sp] = Next->key;
    Next->move = move;
    Next->gen_flags = 0;
    Current++;
    INFO->nodes++;
}

template <bool me> void undo_move(int move) {
    int to, from, piece;
    from = From(move);
    to = To(move);
    if (IsPromotion(move)) {
        BB(Square(to)) ^= Bit(to);
        piece = IPawn(me);
    } else piece = Square(to);
    Square(from) = piece;
    BB(piece) |= Bit(from);
    Piece(me) |= Bit(from);
    BB(piece) &= ~Bit(to);
    Piece(me) ^= Bit(to);
    Square(to) = Current->capture;
    if (Current->capture) {
        BB(Current->capture) |= Bit(to);
        Piece(opp) |= Bit(to);
    } else {
        if (IsCastling(move)) {
            int rold, rnew;
            if (to == 6) {
                rold = 7; 
                rnew = 5;
            } else if (to == 2) {
                rold = 0; 
                rnew = 3;
            } else if (to == 62) {
                rold = 63;
                rnew = 61;
            } else if (to == 58) {
                rold = 56; 
                rnew = 59;
            }
            Square(rnew) = 0;
            Square(rold) = IRook(me);
            Rook(me) ^= Bit(rnew);
            Piece(me) ^= Bit(rnew);
            Rook(me) |= Bit(rold);
            Piece(me) |= Bit(rold);
        } else if (IsEP(move)) {
            to = to ^ 8;
            piece = IPawn(opp);
            Square(to) = piece;
            Piece(opp) |= Bit(to);
            Pawn(opp) |= Bit(to);
        }
    }
    Current--;
    sp--;
}

void do_null() {
    GData * Next;
    GEntry * Entry;

    Next = Current + 1;
    Next->key = Current->key ^ DATA->TurnKey;
    Entry = HASH + (High32(Next->key) & hash_mask);
    prefetch((char *)Entry,_MM_HINT_NTA);
    Next->pawn_key = Current->pawn_key;
    Next->eval_key = 0;
    Next->turn = Current->turn ^ 1;
    Next->material = Current->material;
    Next->pst = Current->pst;
    Next->ply = 0;
    Next->castle_flags = Current->castle_flags;
    Next->ep_square = 0;
    Next->capture = 0;
    if (Current->ep_square) Next->key ^= DATA->EPKey[File(Current->ep_square)];
    sp++;    
    Next->att[White] = Current->att[White];
    Next->att[Black] = Current->att[Black];
    Next->patt[White] = Current->patt[White];
    Next->patt[Black] = Current->patt[Black];
    Next->xray[White] = Current->xray[White];
    Next->xray[Black] = Current->xray[Black];
    Next->pin[White] = Current->pin[White];
    Next->pin[Black] = Current->pin[Black];
    Stack[sp] = Next->key;
    Next->threat = Current->threat;
    Next->passer = Current->passer;
    Next->score = -Current->score;
    Next->move = 0;
    Next->gen_flags = 0;
    Current++;
    INFO->nodes++;
}

void undo_null() {
    Current--;
    sp--;
}

template <bool me> int krbkrx() {
    if (King(opp) & Interior) return 1;
    return 16;
}
template <bool me> int kpkx() {
    uint64_t u;
    if (me == White) u = DATA->Kpk[Current->turn][lsb(Pawn(White))][lsb(King(White))] & Bit(lsb(King(Black)));
    else u = DATA->Kpk[Current->turn ^ 1][63 - lsb(Pawn(Black))][63 - lsb(King(Black))] & Bit(63 - lsb(King(White)));
    if (u) return 32;
    else if (Piece(opp) ^ King(opp)) return 1;
    else return 0;
}
template <bool me> int knpkx() {
    if (Pawn(me) & Line(me, 6) & (DATA->File[0] | DATA->File[7])) {
        int sq = lsb(Pawn(me));
        if (DATA->SArea[sq] & King(opp) & (Line(me, 6) | Line(me, 7))) return 0;
        if (Square(sq + Push(me)) == IKing(me) && (DATA->SArea[lsb(King(me))] && DATA->SArea[lsb(King(opp))] & Line(me, 7))) return 0;
    } else if (Pawn(me) & Line(me, 5) & (DATA->File[0] | DATA->File[7])) {
        int sq = lsb(Pawn(me));
        if (Square(sq + Push(me)) == IPawn(opp)) {
            if (DATA->SArea[sq + Push(me)] & King(opp) & Line(me, 7)) return 0;
            if ((DATA->SArea[sq + Push(me)] & DATA->SArea[lsb(King(opp))] & Line(me, 7)) && (!(DATA->NAtt[sq + Push(me)] & Knight(me)) || Current->turn == opp)) return 0;
        }
    }
    return 32;
}
template <bool me> int krpkrx() {
    int mul = 32;
    int sq = lsb(Pawn(me));
    int rrank = CRank(me, sq);
    int o_king = lsb(King(opp));
    int o_rook = lsb(Rook(opp));
    int m_king = lsb(King(me));
    int add_mat = T(Piece(opp) ^ King(opp) ^ Rook(opp));
    int clear = F(add_mat) || F((DATA->PWay[opp][sq] | DATA->PIsolated[File(sq)]) & DATA->Forward[opp][Rank(sq + Push(me))] & (Piece(opp) ^ King(opp) ^ Rook(opp)));

    if (!clear) return 32;
    if (!add_mat && !(Pawn(me) & (DATA->File[0] | DATA->File[7]))) {
        int m_rook = lsb(Rook(me));
        if (CRank(me, o_king) < CRank(me, m_rook) && CRank(me, m_rook) < rrank && CRank(me, m_king) >= rrank - 1 && CRank(me, m_king) > CRank(me, m_rook)
            && ((DATA->SArea[m_king] & Pawn(me)) || (Current->turn == me && Abs(File(sq) - File(m_king)) <= 1 && Abs(rrank - CRank(me, m_king)) <= 2))) return 128;
        if (DATA->SArea[m_king] & Pawn(me)) {
            if (rrank >= 4) {
                if ((File(sq) < File(m_rook) && File(m_rook) < File(o_king)) || (File(sq) > File(m_rook) && File(m_rook) > File(o_king))) return 128;
            } else if (rrank >= 2) {
                if (!(Pawn(me) & (DATA->File[1] | DATA->File[6])) && rrank + Abs(File(sq) - File(m_rook)) > 4
                    && ((File(sq) < File(m_rook) && File(m_rook) < File(o_king)) || (File(sq) > File(m_rook) && File(m_rook) > File(o_king)))) return 128;
            }
        }
    }

    if (DATA->PWay[me][sq] & King(opp)) {
        if (Pawn(me) & (DATA->File[0] | DATA->File[7])) mul = Min(mul, add_mat << 3);
        if (rrank <= 3) mul = Min(mul, add_mat << 3);
        if (rrank == 4 && CRank(me, m_king) <= 4 && CRank(me, o_rook) == 5 && T(King(opp) & (Line(me, 6) | Line(me, 7)))
            && (Current->turn != me || F(DATA->PAtt[me][sq] & RookAttacks(lsb(Rook(me)), PieceAll) & (~DATA->SArea[o_king])))) mul = Min(mul, add_mat << 3);
        if (rrank >= 5 && CRank(me, o_rook) <= 1 && (Current->turn != me || Check(me) || Dist(m_king, sq) >= 2)) mul = Min(mul, add_mat << 3);
        if (T(King(opp) & (DATA->File[1] | DATA->File[2] | DATA->File[6] | DATA->File[7])) && T(Rook(opp) & Line(me, 7)) && T(DATA->Between[o_king][o_rook] & (DATA->File[3] | DATA->File[4])) && F(Rook(me) & Line(me, 7))) mul = Min(mul, add_mat << 3);
        return mul;
    } else if (rrank == 6 && (Pawn(me) & (DATA->File[0] | DATA->File[7])) && ((DATA->PSupport[me][sq] | DATA->PWay[opp][sq]) & Rook(opp)) && CRank(me, o_king) >= 6) {
        int dist = Abs(File(sq) - File(o_king));
        if (dist <= 3)  mul = Min(mul, add_mat << 3);
        if (dist == 4 && ((DATA->PSupport[me][o_king] & Rook(me)) || Current->turn == opp)) mul = Min(mul, add_mat << 3);
    }

    if (DATA->SArea[o_king] & DATA->PWay[me][sq] & Line(me, 7)) {
        if (rrank <= 4 && CRank(me, m_king) <= 4 && CRank(me, o_rook) == 5) mul = Min(mul, add_mat << 3);
        if (rrank == 5 && CRank(me, o_rook) <= 1 && Current->turn != me || (F(DATA->SArea[m_king] & DATA->PAtt[me][sq] & (~DATA->SArea[o_king])) && (Check(me) || Dist(m_king, sq) >= 2)))
            mul = Min(mul, add_mat << 3);
    }

    if (T(DATA->PWay[me][sq] & Rook(me)) && T(DATA->PWay[opp][sq] & Rook(opp))) {
        if (King(opp) & (DATA->File[0] | DATA->File[1] | DATA->File[6] | DATA->File[7]) & Line(me, 6)) mul = Min(mul, add_mat << 3);
        else if ((Pawn(me) & (DATA->File[0] | DATA->File[7])) && (King(opp) & (Line(me, 5) | Line(me, 6))) && Abs(File(sq) - File(o_king)) <= 2 && File(sq) != File(o_king)) mul = Min(mul, add_mat << 3);
    }

    if (Abs(File(sq) - File(o_king)) <= 1 && Abs(File(sq) - File(o_rook)) <= 1 && CRank(me, o_rook) > rrank && CRank(me, o_king) > rrank) mul = Min(mul, (Pawn(me) & (DATA->File[3] | DATA->File[4])) ? 12 : 16);

    return mul;
}
template <bool me> int krpkbx() {
    if (!(Pawn(me) & Line(me, 5))) return 32;
    int sq = lsb(Pawn(me));
    if (!(DATA->PWay[me][sq] & King(opp))) return 32;
    int diag_sq = NB(me, DATA->BMask[sq + Push(me)]);
    if (CRank(me, diag_sq) > 1) return 32;
    uint64_t mdiag = DATA->FullLine[sq + Push(me)][diag_sq] | Bit(sq + Push(me)) | Bit(diag_sq);
    int check_sq = NB(me, DATA->BMask[sq - Push(me)]);
    uint64_t cdiag = DATA->FullLine[sq - Push(me)][check_sq] | Bit(sq - Push(me)) | Bit(check_sq);
    if ((mdiag | cdiag) & (Piece(opp) ^ King(opp) ^ Bishop(opp))) return 32;
    if (cdiag & Bishop(opp)) return 0;
    if ((mdiag & Bishop(opp)) && (Current->turn == opp || !(King(me) & DATA->PAtt[opp][sq + Push(me)]))) return 0;
    return 32;
}
template <bool me> int kqkp() {
    if (F(DATA->SArea[lsb(King(opp))] & Pawn(opp) & Line(me, 1) & (DATA->File[0] | DATA->File[2] | DATA->File[5] | DATA->File[7]))) return 32;
    if (DATA->PWay[opp][lsb(Pawn(opp))] & (King(me) | Queen(me))) return 32;
    if (Pawn(opp) & (DATA->File[0] | DATA->File[7])) return 1;
    else return 4;
}
template <bool me> int kqkrpx() {
    int rsq = lsb(Rook(opp));
    uint64_t pawns = DATA->SArea[lsb(King(opp))] & DATA->PAtt[me][rsq] & Pawn(opp) & Interior & Line(me, 6);
    if (pawns && CRank(me, lsb(King(me))) <= 4) return 0;
    return 32;
}
template <bool me> int krkpx() {
    if (T(DATA->SArea[lsb(King(opp))] & Pawn(opp) & Line(me, 1)) & F(DATA->PWay[opp][NB(me, Pawn(opp))] & King(me))) return 0;
    return 32;
}
template <bool me> int krppkrpx() {
    if (Current->passer & Pawn(me)) {
        if (Single(Current->passer & Pawn(me))) {
            int sq = lsb(Current->passer & Pawn(me));
            if (DATA->PWay[me][sq] & King(opp) & (DATA->File[0] | DATA->File[1] | DATA->File[6] | DATA->File[7])) {
                int opp_king = lsb(King(opp));
                if (DATA->SArea[opp_king] & Pawn(opp)) {
                    int king_file = File(opp_king);
                    if (!((~(DATA->File[king_file] | DATA->PIsolated[king_file])) & Pawn(me))) return 1;
                }
            }
        }
        return 32;
    }
    if (F((~(DATA->PWay[opp][lsb(King(opp))] | DATA->PSupport[me][lsb(King(opp))])) & Pawn(me))) return 0;
    return 32;
}
template <bool me> int krpppkrppx() {
    if (T(Current->passer & Pawn(me)) || F((DATA->SArea[lsb(Pawn(opp))] | DATA->SArea[msb(Pawn(opp))]) & Pawn(opp))) return 32;
    if (F((~(DATA->PWay[opp][lsb(King(opp))] | DATA->PSupport[me][lsb(King(opp))])) & Pawn(me))) return 0;
    return 32;
}
template <bool me> int kbpkbx() {
    int sq = lsb(Pawn(me));
    uint64_t u;
    if ((T(Board->bb[ILight(me)]) && T(Board->bb[IDark(opp)])) || (T(Board->bb[IDark(me)]) && T(Board->bb[ILight(opp)]))) {
        if (CRank(me, sq) <= 4) return 0;
        if (T(DATA->PWay[me][sq] & King(opp)) && CRank(me, sq) <= 5) return 0;
        for (u = Bishop(opp); T(u); Cut(u)) {
            if (CRank(me, lsb(u)) <= 4 && T(BishopAttacks(lsb(u), PieceAll) & DATA->PWay[me][sq])) return 0;
            if (Current->turn == opp && T(BishopAttacks(lsb(u), PieceAll) & Pawn(me))) return 0;
        }
    } else if (T(DATA->PWay[me][sq] & King(opp)) && T(King(opp) & LightArea) != T(Bishop(me) & LightArea)) return 0;
    return 32;
}
template <bool me> int kbpknx() {
    uint64_t u;
    if (T(DATA->PWay[me][lsb(Pawn(me))] & King(opp)) && T(King(opp) & LightArea) != T(Bishop(me) & LightArea)) return 0;
    if (Current->turn == opp)
    for (u = Knight(opp); T(u); Cut(u))
    if (DATA->NAtt[lsb(u)] & Pawn(me)) return 0;
    return 32;
}
template <bool me> int kbppkbx() {
    int sq1 = NB(me, Pawn(me));
    int sq2 = NB(opp, Pawn(me));
    int o_king = lsb(King(opp));
    int o_bishop = lsb(Bishop(opp));

    if (File(sq1) == File(sq2)) {
        if (CRank(me, sq2) <= 3) return 0;
        if (T(DATA->PWay[me][sq2] & King(opp)) && CRank(me, sq2) <= 5) return 0;
    } else if (DATA->PIsolated[File(sq1)] & Pawn(me)) {
        if (T(King(opp) & LightArea) != T(Bishop(me) & LightArea)) {
            if (T((DATA->SArea[o_king] | King(opp)) & Bit(sq2 + Push(me))) && T(BishopAttacks(o_bishop, PieceAll) & Bit(sq2 + Push(me))))
            if (T((DATA->SArea[o_king] | King(opp)) & Bit((sq2 & 0xFFFFFFF8) | File(sq1))) && T(BishopAttacks(o_bishop, PieceAll) & Bit((sq2 & 0xFFFFFFF8) | File(sq1)))) return 0;
        }
    }
    return 32;
}
template <bool me> int krppkrx() {
    int sq1 = NB(me, Pawn(me));
    int sq2 = NB(opp, Pawn(me));

    if ((Piece(opp) ^ King(opp) ^ Rook(opp)) & DATA->Forward[me][Rank(sq1 - Push(me))]) return 32;
    if (File(sq1) == File(sq2)) {
        if (T(DATA->PWay[me][sq2] & King(opp))) return 16;
        return 32;
    }
    if (T(DATA->PIsolated[File(sq2)] & Pawn(me)) && T((DATA->File[0] | DATA->File[7]) & Pawn(me)) && T(King(opp) & Shift(me, Pawn(me)))) {
        if (CRank(me, sq2) == 5 && CRank(me, sq1) == 4 && T(Rook(opp) & (Line(me, 5) | Line(me, 6)))) return 10;
        else if (CRank(me, sq2) < 5) return 16;
    }
    return 32;
}
typedef struct {
    int king_w, king_b, score;
    uint64_t patt_w, patt_b, double_att_w, double_att_b;
} GPawnEvalInfo;

template <bool me> inline void eval_pawns(GPawnEntry * PawnEntry, GPawnEvalInfo &PEI) {
    int kf = File(PVarC(PEI, king, me));
    int kr = Rank(PVarC(PEI, king, me));
    int start, inc;
    if (kf <= 3) {
        start = Max(kf - 1, 0);
        inc = 1;
    } else {
        start = Min(kf + 1, 7);
        inc = -1;
    }
    int shelter = 0;
    uint64_t mpawns = Pawn(me) & DATA->Forward[me][me ? Min(kr + 1, 7) : Max(kr - 1, 0)];
    for (int file = start, i = 0; i < 3; file += inc, i++) {
        shelter += DATA->Shelter[i][CRank(me, NBZ(me, mpawns & DATA->File[file]))];
        int rank;
        if (Pawn(opp) & DATA->File[file]) {
            int sq = NB(me, Pawn(opp) & DATA->File[file]);
            if ((rank = CRank(opp, sq)) < 6) {
                if (rank >= 3) shelter += DATA->StormBlocked[rank - 3];
                if (uint64_t u = (DATA->PIsolated[File(sq)] & DATA->Forward[opp][Rank(sq)] & Pawn(me))) {
                    int square = NB(opp, u);
                    uint64_t att_sq = DATA->PAtt[me][square] & DATA->PWay[opp][sq]; // may be zero
                    if ((DATA->File[File(square)] | DATA->PIsolated[File(square)]) & King(me)) if (!(PVarC(PEI, double_att, me) & att_sq) || (Current->patt[opp] & att_sq)) {
                        if (DATA->PWay[opp][square] & Pawn(me)) continue;
                        if (!(PawnAll & DATA->PWay[opp][sq] & DATA->Forward[me][Rank(square)])) {
                            if (rank >= 3) {
                                shelter += DATA->StormShelterAtt[rank - 3];
                                if (PVarC(PEI, patt, opp) & Bit(sq + Push(opp))) shelter += DATA->StormConnected[rank - 3];
                                if (!(DATA->PWay[opp][sq] & PawnAll)) shelter += DATA->StormOpen[rank - 3];
                            }
                            if (!((DATA->File[File(sq)] | DATA->PIsolated[File(sq)]) & King(opp)) && rank <= 4) shelter += DATA->StormFree[rank - 1];
                        }
                    }
                }
            }
        } else {
            shelter += Sa(StormHof, StormHofValue);
            if (!(Pawn(me) & DATA->File[file])) shelter += Sa(StormHof, StormOfValue);
        }
    }
    PawnEntry->shelter[me] = (shelter > UINT8_MAX? UINT8_MAX: shelter);
    PawnEntry->passer[me] = 0;

    uint64_t b;
    int min_file = 7, max_file = 0;
    for (uint64_t u = Pawn(me); T(u); u ^= b) {
        int sq = lsb(u);
        b = Bit(sq);
        int rank = Rank(sq);
        int rrank = CRank(me, sq);
        int file = File(sq);
        uint64_t way = DATA->PWay[me][sq];
        int next = Square(sq + Push(me));
        if (file < min_file) min_file = file;
        if (file > max_file) max_file = file;

        int isolated = !(Pawn(me) & DATA->PIsolated[file]);
        int doubled = T(Pawn(me) & (DATA->File[file] ^ b));
        int open = !(PawnAll & way);
        int up = !(PVarC(PEI, patt, me) & b);

        if (isolated) {
            if (open) DecV(PEI.score, Ca(Isolated, IsolatedOpen));
            else {
                DecV(PEI.score, Ca(Isolated, IsolatedClosed));
                if (next == IPawn(opp)) DecV(PEI.score, Ca(Isolated, IsolatedBlocked));
            }
            if (doubled) {
                if (open) DecV(PEI.score, Ca(Isolated, IsolatedDoubledOpen));
                else DecV(PEI.score, Ca(Isolated, IsolatedDoubledClosed));
            }
        } else {
            if (doubled) {
                if (open) DecV(PEI.score, Ca(Doubled, DoubledOpen));
                else DecV(PEI.score, Ca(Doubled, DoubledClosed));
            }
            if (rrank >= 3 && (b & (DATA->File[2] | DATA->File[3] | DATA->File[4] | DATA->File[5])) && next != IPawn(opp) && (DATA->PIsolated[file] & DATA->Line[rank] & Pawn(me)))
                IncV(PEI.score, Ca(PawnSpecial, PawnChainLinear) * (rrank - 3) + Ca(PawnSpecial, PawnChain));
        }
        int backward = 0;
        if (!(DATA->PSupport[me][sq] & Pawn(me))) {
            if (isolated) backward = 1;
            else if (uint64_t v = (PawnAll | PVarC(PEI, patt, opp)) & way) if (IsGreater(me, NB(me, PVarC(PEI, patt, me) & way), NB(me, v))) backward = 1;
        }
        if (backward) {
            if (open) DecV(PEI.score, Ca(Backward, BackwardOpen));
            else DecV(PEI.score, Ca(Backward, BackwardClosed));
        } else if (open) if (!(Pawn(opp) & DATA->PIsolated[file]) || popcount(Pawn(me) & DATA->PIsolated[file]) >= popcount(Pawn(opp) & DATA->PIsolated[file])) IncV(PEI.score,DATA->PasserCandidate[rrank]); // IDEA: more precise pawn counting for the case of, say, white e5 candidate with black pawn on f5 or f4...
        if (up && next == IPawn(opp)) {
            DecV(PEI.score, Ca(Unprotected, UpBlocked));
            if (backward) {
                if (rrank <= 2) { // IDEA (based on weird passer target tuning result): may be score unprotected/backward depending on rank/file?
                    DecV(PEI.score, Ca(Unprotected, PasserTarget));
                    if (rrank <= 1) DecV(PEI.score, Ca(Unprotected, PasserTarget));
                }
                for (uint64_t v = DATA->PAtt[me][sq] & Pawn(me); v; Cut(v)) if ((DATA->PSupport[me][lsb(v)] & Pawn(me)) == b) {
                    DecV(PEI.score, Ca(Unprotected, ChainRoot));
                    break;
                }
            }
        }
        if (open && !(DATA->PIsolated[file] & DATA->Forward[me][rank] & Pawn(opp))) {
            PawnEntry->passer[me] |= (uint8_t)(1 << file);
            if (rrank <= 2) continue;
            IncV(PEI.score, DATA->PasserGeneral[rrank]);
            int dist_att = Dist(PVarC(PEI, king, opp), sq + Push(me)); // IDEA: average the distance with the distance to the promotion square? or just use the latter?
            int dist_def = Dist(PVarC(PEI, king, me), sq + Push(me));
            IncV(PEI.score, Compose256(0, dist_att * (int)DATA->PasserAtt[rrank] + DATA->LogDist[dist_att] * (int)DATA->PasserAttLog[rrank] - dist_def * (int)DATA->PasserDef[rrank] - (int)DATA->LogDist[dist_def] * (int)DATA->PasserDefLog[rrank]));
            if (PVarC(PEI, patt, me) & b) IncV(PEI.score, DATA->PasserProtected[rrank]);
            if (!(Pawn(opp) & DATA->West[file]) || !(Pawn(opp) & DATA->East[file])) IncV(PEI.score, DATA->PasserOutside[rrank]);
        }
    }
    uint64_t files = 0;
    for (int i = 1; i < 7; i++) files |= (Pawn(me) >> (i << 3)) & 0xFF;
    int file_span = (files ? (msb(files) - lsb(files)) : 0);
    IncV(PEI.score, Ca(PawnSpecial, PawnFileSpan) * file_span);
    PawnEntry->draw[me] = (7 - file_span) * Max(5 - popcount(files), 0);
}

void eval_pawn_structure(GPawnEntry * PawnEntry) {
    GPawnEvalInfo PEI;
    for (int i = 0; i < sizeof(GPawnEntry) / sizeof(int); i++) *(((int*)PawnEntry) + i) = 0;
    PawnEntry->key = (Current->pawn_key >> 16);

    PEI.patt_w = ShiftW(White, Pawn(White)) | ShiftE(White, Pawn(White));
    PEI.patt_b = ShiftW(Black, Pawn(Black)) | ShiftE(Black, Pawn(Black));
    PEI.double_att_w = ShiftW(White, Pawn(White)) & ShiftE(White, Pawn(White));
    PEI.double_att_b = ShiftW(Black, Pawn(Black)) & ShiftE(Black, Pawn(Black));
    PEI.king_w = lsb(King(White));
    PEI.king_b = lsb(King(Black));
    PEI.score = 0;

    eval_pawns<White>(PawnEntry, PEI);
    eval_pawns<Black>(PawnEntry, PEI);

    PawnEntry->score = PEI.score;
}

typedef struct {
    int score, king_w, king_b, mul;
    uint64_t occ, area_w, area_b, free_w, free_b;
    uint32_t king_att_w, king_att_b;
    GPawnEntry * PawnEntry;
    const GMaterial * material;
} GEvalInfo;

template <bool me> inline void eval_queens(GEvalInfo &EI) {
    uint64_t u, b;
    for (u = Queen(me); T(u); u ^= b) {
        int sq = lsb(u);
        b = Bit(sq);
        uint64_t att = QueenAttacks(sq,EI.occ);
        Current->att[me] |= att;
        if (DATA->QMask[sq] & King(opp)) if (uint64_t v = DATA->Between[PVarC(EI,king,opp)][sq] & EI.occ) if (Single(v)) {
            Current->xray[me] |= v;
            uint64_t square = lsb(v); int piece = Square(square); int katt = 0;
            if (piece == IPawn(me)) {
                if (!Square(square + Push(me))) IncV(EI.score, Ca(Pin, SelfPawnPin));
            } else if ((piece & 1) == me) {
                IncV(EI.score, Ca(Pin, SelfPiecePin));
                katt = 1;
            } else if (piece != IPawn(opp) && !(((DATA->BMask[sq] & Bishop(opp)) | (DATA->RMask[sq] & Rook(opp)) | Queen(opp)) & v)) {
                IncV(EI.score, Ca(Pin, WeakPin));
                if (!(Current->patt[opp] & v)) katt = 1;
            }
            if (katt && !(att & PVarC(EI, area, opp))) PVarC(EI, king_att, me) += KingAttack;
        } else if (v == (v & Minor(opp))) IncV(EI.score, Ca(KingRay, QKingRay));
        if (att & PVarC(EI, area, opp)) {
            PVarC(EI, king_att, me) += KingQAttack;
            for (uint64_t v = att & PVarC(EI, area, opp); T(v); Cut(v))
            if (DATA->FullLine[sq][lsb(v)] & att & ((Rook(me) & DATA->RMask[sq]) | (Bishop(me) & DATA->BMask[sq]))) PVarC(EI, king_att, me)++;
        }
        IncV(EI.score,DATA->Mobility[PieceType[WhiteQueen] - 1][popcount(att & PVarC(EI,free,me))]);
        if (att & PVarC(EI, free, me) & Pawn(opp)) IncV(EI.score, Ca(Tactical, TacticalMajorPawn));
        if (att & PVarC(EI, free, me) & Minor(opp)) IncV(EI.score, Ca(Tactical, TacticalMajorMinor));
        if (att & PVarC(EI, area, me)) IncV(EI.score, Ca(KingDefence, KingDefQueen));
    }
}
template <bool me> inline void eval_rooks(GEvalInfo &EI) {
    uint64_t u, b;
    for (u = Rook(me); T(u); u ^= b) {
        int sq = lsb(u);
        b = Bit(sq);
        uint64_t att = RookAttacks(sq,EI.occ);
        Current->att[me] |= att;
        if (DATA->RMask[sq] & King(opp)) if (uint64_t v = DATA->Between[PVarC(EI, king, opp)][sq] & EI.occ) if (Single(v)) {
            Current->xray[me] |= v;
            uint64_t square = lsb(v); int piece = Square(square); int katt = 0;
            if (piece == IPawn(me)) {
                if (!Square(square + Push(me))) IncV(EI.score, Ca(Pin, SelfPawnPin));
            } else if ((piece & 1) == me) {
                IncV(EI.score, Ca(Pin, SelfPiecePin));
                katt = 1;
            } else if (piece != IPawn(opp)) {
                if (piece < IRook(opp)) {
                    IncV(EI.score, Ca(Pin, WeakPin));
                    if (!(Current->patt[opp] & v)) katt = 1;
                } else if (piece == IQueen(opp)) IncV(EI.score, Ca(Pin, ThreatPin));
            }
            if (katt && !(att & PVarC(EI, area, opp))) PVarC(EI, king_att, me) += KingAttack;
        } else if (v == (v & (Minor(opp) | Queen(opp)))) IncV(EI.score, Ca(KingRay, RKingRay));
        if (att & PVarC(EI, area, opp)) {
            PVarC(EI, king_att, me) += KingRAttack;
            for (uint64_t v = att & PVarC(EI, area, opp); T(v); Cut(v))
            if (DATA->FullLine[sq][lsb(v)] & att & Major(me)) PVarC(EI, king_att, me)++;
        }
        IncV(EI.score,DATA->Mobility[PieceType[WhiteRook] - 1][popcount(att & PVarC(EI,free,me))]);
        if (att & PVarC(EI, free, me) & Pawn(opp)) IncV(EI.score, Ca(Tactical, TacticalMajorPawn));
        if (att & PVarC(EI, free, me) & Minor(opp)) IncV(EI.score, Ca(Tactical, TacticalMajorMinor));
        if (att & PVarC(EI, area, me)) IncV(EI.score, Ca(KingDefence, KingDefRook));
        Current->threat |= att & Queen(opp);
        if (!(DATA->PWay[me][sq] & Pawn(me))) {
            IncV(EI.score, Ca(RookSpecial, RookHof));
            int hof_score = 0;
            if (!(DATA->PWay[me][sq] & Pawn(opp))) {
                IncV(EI.score, Ca(RookSpecial, RookOf));
                if (att & Line(me, 7)) hof_score += Ca(RookSpecial, RookOfOpen);
                else if (uint64_t target = att & DATA->PWay[me][sq] & Minor(opp)) {
                    if (!(Current->patt[opp] & target)) {
                        hof_score += Ca(RookSpecial, RookOfMinorHaging);
                        if (DATA->PWay[me][sq] & King(opp)) hof_score += Ca(RookSpecial, RookOfKingAtt);
                    } else hof_score += Ca(RookSpecial, RookOfMinorFixed);
                }
            } else if (att & DATA->PWay[me][sq] & Pawn(opp)) {
                uint64_t square = lsb(att & DATA->PWay[me][sq] & Pawn(opp));
                if (!(DATA->PSupport[opp][square] & Pawn(opp))) hof_score += Ca(RookSpecial, RookHofWeakPAtt);
            }
            IncV(EI.score, hof_score);
            if (DATA->PWay[opp][sq] & att & Major(me)) IncV(EI.score, hof_score);
        }
        if ((b & Line(me, 6)) && ((King(opp) | Pawn(opp)) & (Line(me, 6) | Line(me, 7)))) {
            IncV(EI.score, Ca(RookSpecial, Rook7th));
            if (King(opp) & Line(me, 7)) IncV(EI.score, Ca(RookSpecial, Rook7thK8th));
            if (Major(me) & att & Line(me, 6)) IncV(EI.score, Ca(RookSpecial, Rook7thDoubled));
        }
    }
}
template <bool me> inline void eval_bishops(GEvalInfo &EI) {
    uint64_t u, b;
    for (u = Bishop(me); T(u); u ^= b) {
        int sq = lsb(u);
        b = Bit(sq);
        uint64_t att = BishopAttacks(sq, EI.occ);
        Current->att[me] |= att;
        if (DATA->BMask[sq] & King(opp)) if (uint64_t v = DATA->Between[PVarC(EI, king, opp)][sq] & EI.occ) if (Single(v)) {
            Current->xray[me] |= v;
            uint64_t square = lsb(v); int piece = Square(square); int katt = 0;
            if (piece == IPawn(me)) {
                if (!Square(square + Push(me))) IncV(EI.score, Ca(Pin, SelfPawnPin));
            } else if ((piece & 1) == me) {
                IncV(EI.score, Ca(Pin, SelfPiecePin));
                katt = 1;
            } else if (piece != IPawn(opp)) {
                if (piece < ILight(opp)) {
                    IncV(EI.score, Ca(Pin, StrongPin));
                    if (!(Current->patt[opp] & v)) katt = 1;
                } else if (piece >= IRook(opp)) IncV(EI.score, Ca(Pin, ThreatPin));
            }
            if (katt && !(att & PVarC(EI, area, opp))) PVarC(EI, king_att, me) += KingAttack;
        } else if (v == (v & (Knight(opp) | Major(opp)))) IncV(EI.score, Ca(KingRay, BKingRay));
        if (att & PVarC(EI, area, opp)) PVarC(EI, king_att, me) += KingBAttack;
        IncV(EI.score, DATA->Mobility[PieceType[WhiteLight] - 1][popcount(att & PVarC(EI, free, me))]);
        if (att & PVarC(EI, free, me) & Pawn(opp)) IncV(EI.score, Ca(Tactical, TacticalMinorPawn));
        if (att & PVarC(EI, free, me) & Knight(opp)) IncV(EI.score, Ca(Tactical, TacticalMinorMinor));
        if (att & PVarC(EI, area, me)) IncV(EI.score, Ca(KingDefence, KingDefBishop));
        Current->threat |= att & Major(opp);
        if (b & LightArea) {
            for (uint64_t v = ((~DATA->BishopForward[me][sq]) | (att & DATA->Forward[me][Rank(sq)])) & Pawn(opp) & (~Current->patt[opp]) & LightArea; v; Cut(v)) {
                uint64_t square = lsb(v);
                if (!((DATA->PSupport[opp][square] | DATA->PWay[opp][square]) & Pawn(opp))) IncV(EI.score, Ca(BishopSpecial, BishopNonForwardPawn));
            }
            uint64_t v = DATA->BishopForward[me][sq] & Pawn(me) & LightArea;
            v |= (v & (DATA->File[2] | DATA->File[3] | DATA->File[4] | DATA->File[5] | DATA->BMask[sq])) >> 8;
            DecV(EI.score, Ca(BishopSpecial, BishopPawnBlock) * popcount(v));
        } else {
            for (uint64_t v = ((~DATA->BishopForward[me][sq]) | (att & DATA->Forward[me][Rank(sq)])) & Pawn(opp) & (~Current->patt[opp]) & DarkArea; v; Cut(v)) {
                uint64_t square = lsb(v);
                if (!((DATA->PSupport[opp][square] | DATA->PWay[opp][square]) & Pawn(opp))) IncV(EI.score, Ca(BishopSpecial, BishopNonForwardPawn));
            }
            uint64_t v = DATA->BishopForward[me][sq] & Pawn(me) & DarkArea;
            v |= (v & (DATA->File[2] | DATA->File[3] | DATA->File[4] | DATA->File[5] | DATA->BMask[sq])) >> 8;
            DecV(EI.score, Ca(BishopSpecial, BishopPawnBlock) * popcount(v));
        }
    }
}
template <bool me> inline void eval_knights(GEvalInfo &EI) {
    uint64_t u, b;
    for (u = Knight(me); T(u); u ^= b) {
        int sq = lsb(u);
        b = Bit(sq);
        uint64_t att = DATA->NAtt[sq];
        Current->att[me] |= att;
        if (att & PVarC(EI, area, opp)) PVarC(EI, king_att, me) += KingNAttack;
        IncV(EI.score, DATA->Mobility[PieceType[WhiteKnight] - 1][popcount(att & PVarC(EI, free, me))]);
        if (att & PVarC(EI, free, me) & Pawn(opp)) IncV(EI.score, Ca(Tactical, TacticalMinorPawn));
        if (att & PVarC(EI, free, me) & Bishop(opp)) IncV(EI.score, Ca(Tactical, TacticalMinorMinor));
        if (att & PVarC(EI, area, me)) IncV(EI.score, Ca(KingDefence, KingDefKnight));
        Current->threat |= att & Major(opp);
        if ((b & Outpost[me]) && !(Pawn(opp) & DATA->PIsolated[File(sq)] & DATA->Forward[me][Rank(sq)])) {
            IncV(EI.score, Ca(KnightSpecial, KnightOutpost));
            if (Current->patt[me] & b) {
                IncV(EI.score, Ca(KnightSpecial, KnightOutpostProtected));
                if (att & PVarC(EI, free, me) & Pawn(opp)) IncV(EI.score, Ca(KnightSpecial, KnightOutpostPawnAtt));
                if (att & PVarC(EI, free, me) & Bishop(opp)) IncV(EI.score, Ca(KnightSpecial, KnightOutpostBishopAtt));
            }
        }
    }
}
template <bool me> inline void eval_king(GEvalInfo &EI) {
    int cnt = Opening(PVarC(EI, king_att, me));
    int score = Endgame(PVarC(EI, king_att, me));
    if (cnt >= 2 && T(Queen(me))) {
        score += (EI.PawnEntry->shelter[opp] * KingShelterQuad)/64;
        if (uint64_t u = Current->att[me] & PVarC(EI, area, opp) & (~Current->att[opp])) score += popcount(u) * KingAttackSquare;
        if (!(DATA->SArea[PVarC(EI, king, opp)] & (~(Piece(opp) | Current->att[me])))) score += KingNoMoves;
    }
    int adjusted = ((score * KingAttackScale[cnt]) >> 3) + EI.PawnEntry->shelter[opp];
    if (!Queen(me)) adjusted /= 2;
    IncV(EI.score, adjusted);
}
template <bool me> inline void eval_passer(GEvalInfo &EI) {
    for (uint64_t u = EI.PawnEntry->passer[me]; T(u); Cut(u)) {
        int file = lsb(u);
        uint64_t passer = DATA->File[file] & Pawn(me);
        if (passer == 0)
            continue;
        int sq = NB(opp, passer);
        int rank = CRank(me, sq);
        Current->passer |= Bit(sq);
        if (rank <= 2) continue;
        if (!Square(sq + Push(me))) IncV(EI.score, DATA->PasserBlocked[rank]);
        uint64_t way = DATA->PWay[me][sq];
        int connected = 0, supported = 0, hooked = 0, unsupported = 0, free = 0;
        if (!(way & Piece(opp))) {
            IncV(EI.score, DATA->PasserClear[rank]);
            if (DATA->PWay[opp][sq] & Major(me)) {
                int square = NB(opp, DATA->PWay[opp][sq] & Major(me));
                if (F(DATA->Between[sq][square] & EI.occ)) supported = 1;
            }
            if (DATA->PWay[opp][sq] & Major(opp)) {
                int square = NB(opp, DATA->PWay[opp][sq] & Major(opp));
                if (F(DATA->Between[sq][square] & EI.occ)) hooked = 1;
            }
            for (uint64_t v = DATA->PAtt[me][sq - Push(me)] & Pawn(me); T(v); Cut(v)) {
                int square = lsb(v);
                if (F(Pawn(opp) & (DATA->File[File(square)] | DATA->PIsolated[File(square)]) & DATA->Forward[me][Rank(square)])) connected++;
            }
            if (connected) IncV(EI.score, DATA->PasserConnected[rank]);
            if (!hooked && !(Current->att[opp] & way)) {
                IncV(EI.score, DATA->PasserFree[rank]);
                free = 1;
            } else {
                uint64_t attacked = Current->att[opp] | (hooked ? way : 0);
                if (supported || (!hooked && connected) || (!(Major(me) & way) && !(attacked & (~Current->att[me])))) IncV(EI.score, DATA->PasserSupported[rank]);
                else unsupported = 1;
            }
        }
        if (rank == 6) {
            if ((way & Rook(me)) && !Minor(me) && !Queen(me) && Single(Rook(me))) DecV(EI.score, Compose(0, Sa(PasserSpecial, PasserOpRookBlock)));
            if (!Major(opp) && (!NonPawnKing(opp) || Single(NonPawnKing(opp)))) {
                IncV(EI.score, Compose(0, Sa(PasserSpecial, PasserOnePiece)));
                if (!free) {
                    if (!(DATA->SArea[sq + Push(me)] & King(opp))) IncV(EI.score, Compose(0, Sa(PasserSpecial, PasserOpMinorControl)));
                    else IncV(EI.score, Compose(0, Sa(PasserSpecial, PasserOpKingControl)));
                }
            }
        }
    }
}
template <bool me> inline void eval_pieces(GEvalInfo &EI) {
    Current->threat |= Current->att[opp] & (~Current->att[me]) & Piece(me);
    if (uint64_t u = Current->threat & Piece(me)) {
        DecV(EI.score, Ca(Tactical, TacticalThreat));
        Cut(u);
        if (u) {
            DecV(EI.score, Ca(Tactical, TacticalThreat) + Ca(Tactical, TacticalDoubleThreat));
            for (Cut(u); u; Cut(u)) DecV(EI.score, Ca(Tactical, TacticalThreat));
        }
    }
}
template <bool me> void eval_endgame(GEvalInfo &EI) {
    if ((EI.material->flags & VarC(FlagSingleBishop, me)) && Pawn(me)) {
        int sq = (Board->bb[ILight(me)] ? (me ? 0 : 63) : (Board->bb[IDark(me)] ? (me ? 7 : 56) : (File(lsb(King(opp))) <= 3 ? (me ? 0 : 56) : (me ? 7 : 63))));
        if (!(Pawn(me) & (~DATA->PWay[opp][sq]))) {
            if ((DATA->SArea[sq] | Bit(sq)) & King(opp)) EI.mul = 0;
            else if ((DATA->SArea[sq] & DATA->SArea[lsb(King(opp))] & Line(me, 7)) && Square(sq - Push(me)) == IPawn(opp) && Square(sq - 2 * Push(me)) == IPawn(me)) EI.mul = 0;
        } else if ((King(opp) & Line(me, 6) | Line(me, 7)) && Abs(File(sq) - File(lsb(King(opp)))) <= 3 && !(Pawn(me) & (~DATA->PSupport[me][sq])) && (Pawn(me) & Line(me, 5) & Shift(opp, Pawn(opp)))) EI.mul = 0;
        if (Single(Pawn(me))) {
            if (!Bishop(me)) {
                EI.mul = MinF(EI.mul, kpkx<me>());
                if (Piece(opp) == King(opp) && EI.mul == 32) IncV(Current->score, KpkValue);
            } else {
                sq = lsb(Pawn(me));
                if ((Pawn(me) & (DATA->File[1] | DATA->File[6]) & Line(me, 5)) && Square(sq + Push(me)) == IPawn(opp) && ((DATA->PAtt[me][sq + Push(me)] | DATA->PWay[me][sq + Push(me)]) & King(opp))) EI.mul = 0;
            }
        }
        if (Bishop(opp) && Single(Bishop(opp)) && T(BB(ILight(me))) != T(BB(ILight(opp)))) {
            int pcnt = 0;
            if (T(King(opp) & LightArea) == T(Bishop(opp) & LightArea)) {
                for (uint64_t u = Pawn(me); u; Cut(u)) {
                    if (pcnt >= 2) goto check_for_partial_block;
                    pcnt++;
                    int sq = lsb(u);
                    if (!(DATA->PWay[me][sq] & (DATA->PAtt[me][PVarC(EI, king, opp)] | DATA->PAtt[opp][PVarC(EI, king, opp)]))) {
                        if (!(DATA->PWay[me][sq] & Pawn(opp))) goto check_for_partial_block;
                        int bsq = lsb(Bishop(opp));
                        uint64_t att = BishopAttacks(bsq, EI.occ);
                        if (!(att & DATA->PWay[me][sq] & Pawn(opp))) goto check_for_partial_block;
                        if (!(DATA->BishopForward[me][bsq] & att & DATA->PWay[me][sq] & Pawn(opp)) && popcount(DATA->FullLine[lsb(att & DATA->PWay[me][sq] & Pawn(opp))][bsq] & att) <= 2)  goto check_for_partial_block;
                    }
                }
                EI.mul = 0;
                return;
            }
        check_for_partial_block:
            if (pcnt <= 2 && Multiple(Pawn(me)) && !Pawn(opp) && !(Pawn(me) & Boundary) && EI.mul) {
                int sq1 = lsb(Pawn(me));
                int sq2 = msb(Pawn(me));
                int fd = Abs(File(sq2) - File(sq1));
                if (fd >= 5) EI.mul = 32;
                else if (fd >= 4) EI.mul = 26;
                else if (fd >= 3) EI.mul = 20;
            }
            if ((DATA->SArea[PVarC(EI, king, opp)] | Current->patt[opp]) & Bishop(opp)) {
                uint64_t push = Shift(me, Pawn(me));
                if (!(push & (~(Piece(opp) | Current->att[opp]))) && (King(opp) & (Board->bb[ILight(opp)] ? LightArea : DarkArea))) {
                    EI.mul = Min(EI.mul, 8);
                    int bsq = lsb(Bishop(opp));
                    uint64_t att = BishopAttacks(bsq, EI.occ);
                    uint64_t prp = (att | DATA->SArea[PVarC(EI, king, opp)]) & Pawn(opp) & (Board->bb[ILight(opp)] ? LightArea : DarkArea);
                    uint64_t patt = ShiftW(opp, prp) | ShiftE(opp, prp);
                    if ((DATA->SArea[PVarC(EI, king, opp)] | patt) & Bishop(opp)) {
                        uint64_t double_att = (DATA->SArea[PVarC(EI, king, opp)] & patt) | (patt & att) | (DATA->SArea[PVarC(EI, king, opp)] & att);
                        if (!(push & (~(King(opp) | Bishop(opp) | prp | double_att)))) {
                            EI.mul = 0;
                            return;
                        }
                    }
                }
            }
        }
    }
    if (F(Major(me))) {
        if (T(Bishop(me)) && F(Knight(me)) && Single(Bishop(me)) && T(Pawn(me))) {
            int number = popcount(Pawn(me));
            if (number == 1) {
                if (Bishop(opp)) EI.mul = MinF(EI.mul, kbpkbx<me>());
                else if (Knight(opp)) EI.mul = MinF(EI.mul, kbpknx<me>());
            } else if (number == 2 && T(Bishop(opp))) EI.mul = MinF(EI.mul, kbppkbx<me>());
        } else if (!Bishop(me) && Knight(me) && Single(Knight(me)) && Pawn(me) && Single(Pawn(me))) EI.mul = MinF(EI.mul, knpkx<me>());
    } else if (F(Minor(me))) {
        if (F(Pawn(me)) && F(Rook(me)) && T(Queen(me)) && T(Pawn(opp))) {
            if (F(NonPawnKing(opp)) && Single(Pawn(opp))) EI.mul = MinF(EI.mul, kqkp<me>());
            else if (Rook(opp)) EI.mul = MinF(EI.mul, kqkrpx<me>());
        } else if (F(Queen(me)) && T(Rook(me)) && Single(Rook(me))) {
            int number = popcount(Pawn(me));
            if (number <= 3) {
                if (number == 0) {
                    if (Pawn(opp)) EI.mul = MinF(EI.mul, krkpx<me>());
                } else if (Rook(opp)) {
                    if (number == 1) {
                        int new_mul = krpkrx<me>();
                        EI.mul = (new_mul <= 32 ? Min(EI.mul, new_mul) : new_mul);
                    } else {
                        if (number == 2) EI.mul = MinF(EI.mul, krppkrx<me>());
                        if (Pawn(opp)) {
                            if (number == 2) EI.mul = MinF(EI.mul, krppkrpx<me>());
                            else if (Multiple(Pawn(opp))) EI.mul = MinF(EI.mul, krpppkrppx<me>());
                        }
                    }
                } else if (number == 1 && Bishop(opp)) EI.mul = MinF(EI.mul, krpkbx<me>());
            }
        }
    } else if (!Pawn(me) && Single(Rook(me)) && !Queen(me) && Single(Bishop(me)) && !Knight(me) && Rook(opp)) EI.mul = MinF(EI.mul, krbkrx<me>());
    if (F(NonPawnKing(opp)) && Current->turn == opp && F(Current->att[me] & King(opp)) && !(DATA->SArea[PVarC(EI, king, opp)] & (~(Current->att[me] | Piece(opp))))
        && F(Current->patt[opp] & Piece(me)) && F(Shift(opp, Pawn(opp)) & (~EI.occ)))
        EI.mul = 0;
}
void eval_unusual_material(GEvalInfo &EI) {
    int wp, bp, wlight, blight, wr, br, wq, bq;
    wp = popcount(Pawn(White));
    bp = popcount(Pawn(Black));
    wlight = popcount(Minor(White));
    blight = popcount(Minor(Black));
    wr = popcount(Rook(White));
    br = popcount(Rook(Black));
    wq = popcount(Queen(White));
    bq = popcount(Queen(Black));
    int phase = Min(24, (wlight + blight) + 2 * (wr + br) + 4 * (wq + bq));
    int mat_score = SeeValue[WhitePawn] * (wp - bp) + SeeValue[WhiteKnight] * (wlight - blight) + SeeValue[WhiteRook] * (wr - br) + SeeValue[WhiteQueen] * (wq - bq);
    mat_score = Compose(mat_score,mat_score);
    Current->score = (((Opening(mat_score + EI.score) * phase) + (Endgame(mat_score + EI.score) * (24 - phase)))/24);
    if (Current->turn) Current->score = -Current->score;
    UpdateDelta
}

void evaluation() {
    GEvalInfo EI;
    
    if (Current->eval_key == Current->key) return;
    Current->eval_key = Current->key;

    EI.king_w = lsb(King(White));
    EI.king_b = lsb(King(Black));
    EI.occ = PieceAll;
    Current->patt[White] = ShiftW(White,Pawn(White)) | ShiftE(White,Pawn(White));
    Current->patt[Black] = ShiftW(Black,Pawn(Black)) | ShiftE(Black,Pawn(Black));
    EI.area_w = (DATA->SArea[EI.king_w] | King(White)) & ((~Current->patt[White]) | Current->patt[Black]);
    EI.area_b = (DATA->SArea[EI.king_b] | King(Black)) & ((~Current->patt[Black]) | Current->patt[White]);
    Current->att[White] = Current->patt[White];
    Current->att[Black] = Current->patt[Black];
    Current->passer = 0;
    Current->threat = (Current->patt[White] & NonPawn(Black)) | (Current->patt[Black] & NonPawn(White));
    EI.score = Current->pst;

#define me White
    Current->xray[me] = 0;
    PVarC(EI, free, me) = Queen(opp) | King(opp) | (~(Current->patt[opp] | Pawn(me) | King(me)));
    DecV(EI.score, popcount(Shift(opp, EI.occ) & Pawn(me)) * Ca(PawnSpecial, PawnBlocked));
    if (Current->patt[me] & PVarC(EI, area, opp)) PVarC(EI, king_att, me) = KingAttack;
    else PVarC(EI, king_att, me) = 0;
    eval_queens<me>(EI);
    PVarC(EI, free, me) |= Rook(opp);
    eval_rooks<me>(EI);
    PVarC(EI, free, me) |= Minor(opp);
    eval_bishops<me>(EI);
    eval_knights<me>(EI);
#undef me
#define me Black
    Current->xray[me] = 0;
    PVarC(EI, free, me) = Queen(opp) | King(opp) | (~(Current->patt[opp] | Pawn(me) | King(me)));
    DecV(EI.score, popcount(Shift(opp, EI.occ) & Pawn(me)) * Ca(PawnSpecial, PawnBlocked));
    if (Current->patt[me] & PVarC(EI, area, opp)) PVarC(EI, king_att, me) = KingAttack;
    else PVarC(EI, king_att, me) = 0;
    eval_queens<me>(EI);
    PVarC(EI, free, me) |= Rook(opp);
    eval_rooks<me>(EI);
    PVarC(EI, free, me) |= Minor(opp);
    eval_bishops<me>(EI);
    eval_knights<me>(EI);
#undef me

    EI.PawnEntry = PAWNHASH + (Current->pawn_key & pawn_hash_mask);
    if ((Current->pawn_key >> 16) != EI.PawnEntry->key) eval_pawn_structure(EI.PawnEntry);
    EI.score += EI.PawnEntry->score;

    eval_king<White>(EI);
    eval_king<Black>(EI);
    Current->att[White] |= DATA->SArea[EI.king_w];
    Current->att[Black] |= DATA->SArea[EI.king_b];

    eval_passer<White>(EI);
    eval_pieces<White>(EI);
    eval_passer<Black>(EI);
    eval_pieces<Black>(EI);

    if (Current->material & FlagUnusualMaterial) {
        eval_unusual_material(EI);
        return;
    }
    EI.material = &DATA->Material[Current->material];
    Current->score = EI.material->score + (((Opening(EI.score) * EI.material->phase) + (Endgame(EI.score) * (128 - (int)EI.material->phase)))/128);

    if (Current->ply >= 50) Current->score /= 2;
    if (Current->score > 0) {
        EI.mul = EI.material->mul[White];
        if (EI.material->flags & FlagCallEvalEndgame_w) eval_endgame<White>(EI);
        Current->score -= (Min(Current->score, 100) * (int)EI.PawnEntry->draw[White]) / 64;
    } else if (Current->score < 0) {
        EI.mul = EI.material->mul[Black];
        if (EI.material->flags & FlagCallEvalEndgame_b) eval_endgame<Black>(EI);
        Current->score += (Min(-Current->score, 100) * (int)EI.PawnEntry->draw[Black]) / 64;
    } else EI.mul = Min(EI.material->mul[White], EI.material->mul[Black]);
    Current->score = (Current->score * EI.mul)/32;

    if (Current->turn) Current->score = -Current->score;
    UpdateDelta
}

inline void evaluate() {
    evaluation();
}

template <bool me> int is_legal(int move) {
    int from, to, piece, capture;
    uint64_t u, occ;

    from = From(move);
    to = To(move);
    piece = Board->square[from];
    capture = Board->square[to];
    if (piece == 0) return 0;
    if ((piece & 1) != Current->turn) return 0;
    if (capture) {
        if ((capture & 1) == (piece & 1)) return 0;
        if (capture >= WhiteKing) return 0;
    }
    occ = PieceAll;
    u = Bit(to);
    if (piece >= WhiteLight && piece < WhiteKing) {
        if ((DATA->QMask[from] & u) == 0) return 0;
        if (DATA->Between[from][to] & occ) return 0;
    }
    if (IsEP(move)) {
        if (piece >= WhiteKnight) return 0;
        if (Current->ep_square != to) return 0;
        return 1;
    }
    if (IsCastling(move) && Board->square[from] < WhiteKing) return 0;
    if (IsPromotion(move) && Board->square[from] >= WhiteKnight) return 0;
    if (piece == IPawn(me)) {
        if (u & DATA->PMove[me][from]) {
            if (capture) return 0;
            if (T(u & Line(me,7)) && !IsPromotion(move)) return 0;
            return 1;
        } else if (to == (from + 2 * Push(me))) {
            if (capture) return 0;
            if (Square(to - Push(me))) return 0;
            if (F(u & Line(me,3))) return 0;
            return 1;
        } else if (u & DATA->PAtt[me][from]) {
            if (capture == 0) return 0;
            if (T(u & Line(me,7)) && !IsPromotion(move)) return 0;
            return 1;
        } else return 0;
    } else if (piece == IKing(me)) {
        if (me == White) {
            if (IsCastling(move)) {
                if (u & 0x40) {
                    if (((Current->castle_flags) & CanCastle_OO) == 0) return 0;
                    if (occ & 0x60) return 0;
                    if (Current->att[Black] & 0x70) return 0;
                } else {
                    if (((Current->castle_flags) & CanCastle_OOO) == 0) return 0;
                    if (occ & 0xE) return 0;
                    if (Current->att[Black] & 0x1C) return 0;
                }
                return 1;
            }
        } else {
            if (IsCastling(move)) {
                if (u & 0x4000000000000000) {
                    if (((Current->castle_flags) & CanCastle_oo) == 0) return 0;
                    if (occ & 0x6000000000000000) return 0;
                    if (Current->att[White] & 0x7000000000000000) return 0;
                } else {
                    if (((Current->castle_flags) & CanCastle_ooo) == 0) return 0;
                    if (occ & 0x0E00000000000000) return 0;
                    if (Current->att[White] & 0x1C00000000000000) return 0;
                }
                return 1;
            }
        }
        if (F(DATA->SArea[from] & u)) return 0;
        if (Current->att[opp] & u) return 0;
        return 1;
    }
    piece = (piece >> 1) - 2;
    if (piece == 0) {
        if (u & DATA->NAtt[from]) return 1;
        else return 0;
    } else {
        if (piece <= 2) {
            if (DATA->BMask[from] & u) return 1;
        } else if (piece == 3) {
            if (DATA->RMask[from] & u) return 1;
        } else return 1;
        return 0;
    }
}

template <bool me> int is_check(int move) { // doesn't detect castling and ep checks
    uint64_t king;
    int from, to, piece, king_sq;

    from = From(move);
    to = To(move);
    king = King(opp);
    king_sq = lsb(king);
    piece = Square(from);
    if (T(Bit(from) & Current->xray[me]) && F(DATA->FullLine[king_sq][from] & Bit(to))) return 1;
    if (piece < WhiteKnight) {
        if (DATA->PAtt[me][to] & king) return 1;
        if (T(Bit(to) & Line(me, 7)) && T(king & Line(me, 7)) && F(DATA->Between[to][king_sq] & PieceAll)) return 1;
    } else if (piece < WhiteLight) {
        if (DATA->NAtt[to] & king) return 1;
    } else if (piece < WhiteRook) {
        if (DATA->BMask[to] & king) if (F(DATA->Between[king_sq][to] & PieceAll)) return 1;
    } else if (piece < WhiteQueen) {
        if (DATA->RMask[to] & king) if (F(DATA->Between[king_sq][to] & PieceAll)) return 1;
    } else if (piece < WhiteKing) {
        if (DATA->QMask[to] & king) if (F(DATA->Between[king_sq][to] & PieceAll)) return 1;
    }
    return 0;
}

void hash_high(int value, int depth) {
    int i, score, min_score;
    GEntry *best, *Entry;

    min_score = 0x70000000;
    for (i = 0, best = Entry = HASH + (High32(Current->key) & hash_mask); i < 4; i++, Entry++) {
        if (Entry->key == Low32(Current->key)) {
            Entry->date = SHARED->date;
            if (depth > Entry->high_depth || (depth == Entry->high_depth && value < Entry->high)) {
                if (Entry->low <= value) { 
                    Entry->high_depth = depth;
                    Entry->high = value;
                } else if (Entry->low_depth < depth) {
                    Entry->high_depth = depth;
                    Entry->high = value;
                    Entry->low = value;
                }
            }
            return;
        } else score = (Convert(Entry->date,int) << 3) + Convert(Max(Entry->high_depth, Entry->low_depth),int);
        if (score < min_score) {
            min_score = score;
            best = Entry;
        }
    }
    best->date = SHARED->date;
    best->key = Low32(Current->key);
    best->high = value;
    best->high_depth = depth;
    best->low = 0;
    best->low_depth = 0;
    best->move = 0;
    best->flags = 0;
    return;
}

void hash_low(int move, int value, int depth) {
    int i, score, min_score;
    GEntry *best, *Entry;

    min_score = 0x70000000;
    move &= 0xFFFF;
    for (i = 0, best = Entry = HASH + (High32(Current->key) & hash_mask); i < 4; i++, Entry++) {
        if (Entry->key == Low32(Current->key)) {
            Entry->date = SHARED->date;
            if (depth > Entry->low_depth || (depth == Entry->low_depth && value > Entry->low)) {
                if (move) Entry->move = move;
                if (Entry->high >= value) {
                    Entry->low_depth = depth;
                    Entry->low = value;
                } else if (Entry->high_depth < depth) {
                    Entry->low_depth = depth;
                    Entry->low = value;
                    Entry->high = value;
                }
            } else if (F(Entry->move)) Entry->move = move;
            return;
        } else score = (Convert(Entry->date,int) << 3) + Convert(Max(Entry->high_depth, Entry->low_depth),int);
        if (score < min_score) {
            min_score = score;
            best = Entry;
        }
    }
    best->date = SHARED->date;
    best->key = Low32(Current->key);
    best->high = 0;
    best->high_depth = 0;
    best->low = value;
    best->low_depth = depth;
    best->move = move;
    best->flags = 0;
    return;
}

void hash_exact(int move, int value, int depth, int exclusion, int ex_depth, int knodes) {
    int i, score, min_score;
    GPVEntry *best;
    GPVEntry * PVEntry;

    min_score = 0x70000000;
    for (i = 0, best = PVEntry = PVHASH + (High32(Current->key) & pv_hash_mask); i < pv_cluster_size; i++, PVEntry++) {
        if (PVEntry->key == Low32(Current->key)) {
            PVEntry->date = SHARED->date;
            PVEntry->knodes += knodes;
            if (PVEntry->depth <= depth) {
                PVEntry->value = value;
                PVEntry->depth = depth;
                PVEntry->move = move;
                PVEntry->ply = Current->ply;
                if (ex_depth) {
                    PVEntry->exclusion = exclusion;
                    PVEntry->ex_depth = ex_depth;
                }
            }
            return;
        }
        score = (Convert(PVEntry->date,int) << 3) + Convert(PVEntry->depth,int);
        if (score < min_score) {
            min_score = score;
            best = PVEntry;
        }
    }
    best->key = Low32(Current->key);
    best->date = SHARED->date;
    best->value = value;
    best->depth = depth;
    best->move = move;
    best->exclusion = exclusion;
    best->ex_depth = ex_depth;
    best->knodes = knodes;
    best->ply = Current->ply;
}

template <bool pv> inline int extension(int move, int depth) {
    register int ext = 0;
    if (pv) {
        if (T(Current->passer & Bit(From(move))) && CRank(Current->turn, From(move)) >= 5 && depth < 16) ext = 2;
    } else {
        if (T(Current->passer & Bit(From(move))) && CRank(Current->turn, From(move)) >= 5 && depth < 16) ext = 1; 
    }
    return ext;
}

void sort(int * start, int * finish) {
    for (int * p = start; p < finish - 1; p++) {
        int * best = p;
        int value = *p;
        int previous = *p;
        for (int * q = p + 1; q < finish; q++) if ((*q) > value) {
            value = *q;
            best = q;
        }
        *best = previous;
        *p = value;
    }
}

void sort_moves(int * start, int * finish) {
    for (int * p = start + 1; p < finish; p++) for (int * q = p - 1; q >= start; q--) if (((*q) >> 16) < ((*(q+1)) >> 16)) {
        int move = *q;
        *q = *(q+1);
        *(q+1)=move;
    }
}

inline int pick_move() {
    register int move, *p, *best;
    move = *(Current->current);
    if (F(move)) return 0;
    best = Current->current;
    for (p = Current->current + 1; T(*p); p++) {
        if ((*p) > move) {
            best = p;
            move = *p;
        }
    }
    *best = *(Current->current);
    *(Current->current) = move;
    Current->current++;
    return move & 0xFFFF;
}

template <bool me> void gen_next_moves() {
    int *p, *q, *r;
    Current->gen_flags &= ~FlagSort;
    switch (Current->stage) {
    case s_hash_move: case r_hash_move: case e_hash_move:
        Current->moves[0] = Current->killer[0];
        Current->moves[1] = 0;
        return;
    case s_good_cap: 
        Current->mask = Piece(opp);
        r = gen_captures<me>(Current->moves);
        for (q = r - 1, p = Current->moves; q >= p;) {
            int move = (*q) & 0xFFFF;
            if (!see<me>(move,0)) {
                int next = *p;
                *p = *q;
                *q = next;
                p++;
            } else q--;
        }
        Current->start = p;
        Current->current = p;
        sort(p, r);
        return;
    case s_special:
        Current->current = Current->start;
        p = Current->start;
        if (Current->killer[1]) {*p = Current->killer[1]; p++;}
        if (Current->killer[2]) {*p = Current->killer[2]; p++;}
        if (Current->ref[0] && Current->ref[0] != Current->killer[1] && Current->ref[0] != Current->killer[2]) {*p = Current->ref[0]; p++;}
        if (Current->ref[1] && Current->ref[1] != Current->killer[1] && Current->ref[1] != Current->killer[2]) {*p = Current->ref[1]; p++;}
        *p = 0;
        return;
    case s_quiet: 
        gen_quiet_moves<me>(Current->start);
        Current->current = Current->start;
        Current->gen_flags |= FlagSort;
        return;
    case s_bad_cap:
        *(Current->start) = 0;
        Current->current = Current->moves;
        if (!(Current->gen_flags & FlagNoBcSort)) sort(Current->moves, Current->start);
        return;
    case r_cap:
        r = gen_captures<me>(Current->moves);
        Current->current = Current->moves;
        sort(Current->moves, r);
        return;
    case r_checks:
        r = gen_checks<me>(Current->moves);
        Current->current = Current->moves; 
        sort(Current->moves, r);
        return;
    case e_ev:
        Current->mask = Filled;
        r = gen_evasions<me>(Current->moves);
        mark_evasions(Current->moves);
        sort(Current->moves, r);
        Current->current = Current->moves;
        return;
    }
}

template <bool me, bool root> int get_move() {
    int move;
    
    if (root) {
        move = (*Current->current) & 0xFFFF;
        Current->current++;
        return move;
    }
start:
    if (F(*Current->current)) {
        Current->stage++;
        if ((1 << Current->stage) & StageNone) return 0;
        gen_next_moves<me>();
        goto start;
    }
    if (Current->gen_flags & FlagSort) move = pick_move();
    else {
        move = (*Current->current) & 0xFFFF;
        Current->current++;
    }
    if (Current->stage == s_quiet) { 
        if (move == Current->killer[1] || move == Current->killer[2] || move == Current->ref[0] || move == Current->ref[1]) goto start;
    } else if (Current->stage == s_special && (Square(To(move)) || !is_legal<me>(move))) goto start;
    return move;
}

template <bool me> int see(int move, int margin) {
    int from, to, piece, capture, delta, sq, pos;
    uint64_t clear, def, att, occ, b_area, r_slider_att, b_slider_att, r_slider_def, b_slider_def, r_area, u, new_att, my_bishop, opp_bishop;
    from = From(move);
    to = To(move);
    piece = SeeValue[Square(from)];
    capture = SeeValue[Square(to)];
    delta = piece - capture;
    if (delta <= -margin) return 1;
    if (piece == SeeValue[WhiteKing]) return 1;
    if (Current->xray[me] & Bit(from)) return 1;
    if (T(Current->pin[me] & Bit(from)) && piece <= SeeValue[WhiteDark]) return 1;
    if (piece > (SeeValue[WhiteKing] >> 1)) return 1;
    if (IsEP(move)) return 1;
    if (F(Current->att[opp] & Bit(to))) return 1;
    att = DATA->PAtt[me][to] & Pawn(opp);
    if (T(att) && delta + margin > SeeValue[WhitePawn]) return 0;
    clear = ~Bit(from);
    def = DATA->PAtt[opp][to] & Pawn(me) & clear;
    if (T(def) && delta + SeeValue[WhitePawn] + margin <= 0) return 1;
    att |= DATA->NAtt[to] & Knight(opp);
    if (T(att) && delta > SeeValue[WhiteDark] - margin) return 0;
    occ = PieceAll & clear;
    b_area = BishopAttacks(to,occ);
    opp_bishop = Bishop(opp);
    if (delta > SeeValue[IDark(me)] - margin) if (b_area & opp_bishop) return 0;
    my_bishop = Bishop(me);
    b_slider_att = DATA->BMask[to] & (opp_bishop | Queen(opp));
    r_slider_att = DATA->RMask[to] & Major(opp);
    b_slider_def = DATA->BMask[to] & (my_bishop | Queen(me)) & clear;
    r_slider_def = DATA->RMask[to] & Major(me) & clear;
    att |= (b_slider_att & b_area);
    def |= DATA->NAtt[to] & Knight(me) & clear;
    r_area = RookAttacks(to,occ);
    att |= (r_slider_att & r_area);
    def |= (b_slider_def & b_area);
    def |= (r_slider_def & r_area);
    att |= DATA->SArea[to] & King(opp);
    def |= DATA->SArea[to] & King(me) & clear;
    while (true) {
        if (u = (att & Pawn(opp))) {
            capture -= piece;
            piece = SeeValue[WhitePawn];
            sq = lsb(u);
            occ ^= Bit(sq);
            att ^= Bit(sq);
            for (new_att = DATA->FullLine[to][sq] & b_slider_att & occ & (~att); T(new_att); Cut(new_att)) {
                pos = lsb(new_att);
                if (F(DATA->Between[to][pos] & occ)) {
                    Add(att,pos);
                    break;
                }
            }
        } else if (u = (att & Knight(opp))) {
            capture -= piece;
            piece = SeeValue[WhiteKnight];
            att ^= (~(u-1)) & u;
        } else if (u = (att & opp_bishop)) {
            capture -= piece;
            piece = SeeValue[WhiteDark];
            sq = lsb(u);
            occ ^= Bit(sq);
            att ^= Bit(sq);
            for (new_att = DATA->FullLine[to][sq] & b_slider_att & occ & (~att); T(new_att); Cut(new_att)) {
                pos = lsb(new_att);
                if (F(DATA->Between[to][pos] & occ)) {
                    Add(att,pos);
                    break;
                }
            }
        } else if (u = (att & Rook(opp))) {
            capture -= piece;
            piece = SeeValue[WhiteRook];
            sq = lsb(u);
            occ ^= Bit(sq);
            att ^= Bit(sq);
            for (new_att = DATA->FullLine[to][sq] & r_slider_att & occ & (~att); T(new_att); Cut(new_att)) {
                pos = lsb(new_att);
                if (F(DATA->Between[to][pos] & occ)) {
                    Add(att,pos);
                    break;
                }
            }
        } else if (u = (att & Queen(opp))) {
            capture -= piece;
            piece = SeeValue[WhiteQueen];
            sq = lsb(u);
            occ ^= Bit(sq);
            att ^= Bit(sq);
            for (new_att = DATA->FullLine[to][sq] & (r_slider_att | b_slider_att) & occ & (~att); T(new_att); Cut(new_att)) {
                pos = lsb(new_att);
                if (F(DATA->Between[to][pos] & occ)) {
                    Add(att,pos);
                    break;
                }
            }
        } else if (u = (att & King(opp))) {
            capture -= piece;
            piece = SeeValue[WhiteKing];
        } else return 1;
        if (capture < -(SeeValue[WhiteKing] >> 1)) return 0;
        if (piece + capture < margin) return 0;
        if (u = (def & Pawn(me))) {
            capture += piece;
            piece = SeeValue[WhitePawn];
            sq = lsb(u);
            occ ^= Bit(sq);
            def ^= Bit(sq);
            for (new_att = DATA->FullLine[to][sq] & b_slider_def & occ & (~att); T(new_att); Cut(new_att)) {
                pos = lsb(new_att);
                if (F(DATA->Between[to][pos] & occ)) {
                    Add(def,pos);
                    break;
                }
            }
        } else if (u = (def & Knight(me))) {
            capture += piece;
            piece = SeeValue[WhiteKnight];
            def ^= (~(u-1)) & u;
        } else if (u = (def & my_bishop)) {
            capture += piece;
            piece = SeeValue[WhiteDark];
            sq = lsb(u);
            occ ^= Bit(sq);
            def ^= Bit(sq);
            for (new_att = DATA->FullLine[to][sq] & b_slider_def & occ & (~att); T(new_att); Cut(new_att)) {
                pos = lsb(new_att);
                if (F(DATA->Between[to][pos] & occ)) {
                    Add(def,pos);
                    break;
                }
            }
        } else if (u = (def & Rook(me))) {
            capture += piece;
            piece = SeeValue[WhiteRook];
            sq = lsb(u);
            occ ^= Bit(sq);
            def ^= Bit(sq);
            for (new_att = DATA->FullLine[to][sq] & r_slider_def & occ & (~att); T(new_att); Cut(new_att)) {
                pos = lsb(new_att);
                if (F(DATA->Between[to][pos] & occ)) {
                    Add(def,pos);
                    break;
                }
            }
        } else if (u = (def & Queen(me))) {
            capture += piece;
            piece = SeeValue[WhiteQueen];
            sq = lsb(u);
            occ ^= Bit(sq);
            def ^= Bit(sq);
            for (new_att = DATA->FullLine[to][sq] & (r_slider_def | b_slider_def) & occ & (~att); T(new_att); Cut(new_att)) {
                pos = lsb(new_att);
                if (F(DATA->Between[to][pos] & occ)) {
                    Add(def,pos);
                    break;
                }
            }
        } else if (u = (def & King(me))) {
            capture += piece;
            piece = SeeValue[WhiteKing];
        } else return 0;
        if (capture > (SeeValue[WhiteKing] >> 1)) return 1;
        if (capture - piece >= margin) return 1;
    }
}

template <bool me> void gen_root_moves() {
    int i, *p, killer, depth = -256, move;
    GEntry * Entry;
    GPVEntry * PVEntry;

    killer = 0;
    if (Entry = probe_hash()) {
        if (T(Entry->move) && Entry->low_depth > depth) {
            depth = Entry->low_depth;
            killer = Entry->move;
        }
    }
    if (PVEntry = probe_pv_hash()) {
        if (PVEntry->depth > depth && T(PVEntry->move)) {
            depth = PVEntry->depth;
            killer = PVEntry->move;
        }
    }

    Current->killer[0] = killer;
    if (Check(me)) Current->stage = stage_evasion;
    else {
        Current->stage = stage_search;
        Current->ref[0] = RefM(Current->move).ref[0];
        Current->ref[1] = RefM(Current->move).ref[1];
    }
    Current->gen_flags = 0;
    p = RootList;
    Current->current = Current->moves;
    Current->moves[0] = 0;
    while (move = get_move<me,0>()) {
        if (IsIllegal(me,move)) continue;
        if (p > RootList && move == killer) continue;
        if (SearchMoves) {
            for (i = 0; i < SMPointer; i++)
                if (SMoves[i] == move) goto keep_move;
            continue;
        }
keep_move:
        *p = move;
        p++;
    }
    *p = 0;
}

template <bool me> int * gen_captures(int * list) {
    uint64_t u, v;

    if (Current->ep_square)
        for (v = DATA->PAtt[opp][Current->ep_square] & Pawn(me); T(v); Cut(v)) AddMove(lsb(v),Current->ep_square,FlagEP,DATA->MvvLva[IPawn(me)][IPawn(opp)])
    for (u = Pawn(me) & Line(me,6); T(u); Cut(u))
        if (F(Square(lsb(u) + Push(me)))) {
            AddMove(lsb(u),lsb(u) + Push(me),FlagPQueen,MvvLvaPromotion)
            if (DATA->NAtt[lsb(King(opp))] & Bit(lsb(u) + Push(me))) AddMove(lsb(u),lsb(u) + Push(me),FlagPKnight,MvvLvaPromotionKnight)
        }
    for (v = ShiftW(opp,Current->mask) & Pawn(me) & Line(me,6); T(v); Cut(v)) {
        AddMove(lsb(v),lsb(v)+PushE(me),FlagPQueen,MvvLvaPromotionCap(Square(lsb(v)+PushE(me))))
        if (DATA->NAtt[lsb(King(opp))] & Bit(lsb(v) + PushE(me))) AddMove(lsb(v),lsb(v)+PushE(me),FlagPKnight,MvvLvaPromotionKnightCap(Square(lsb(v)+PushE(me))))
    }
    for (v = ShiftE(opp,Current->mask) & Pawn(me) & Line(me,6); T(v); Cut(v)) {
        AddMove(lsb(v),lsb(v)+PushW(me),FlagPQueen,MvvLvaPromotionCap(Square(lsb(v)+PushW(me))))
        if (DATA->NAtt[lsb(King(opp))] & Bit(lsb(v) + PushW(me))) AddMove(lsb(v),lsb(v)+PushW(me),FlagPKnight,MvvLvaPromotionKnightCap(Square(lsb(v)+PushW(me))))
    }
    if (F(Current->att[me] & Current->mask)) goto finish;
    for (v = ShiftW(opp,Current->mask) & Pawn(me) & (~Line(me,6)); T(v); Cut(v)) AddCaptureP(IPawn(me),lsb(v),lsb(v)+PushE(me),0)
    for (v = ShiftE(opp,Current->mask) & Pawn(me) & (~Line(me,6)); T(v); Cut(v)) AddCaptureP(IPawn(me),lsb(v),lsb(v)+PushW(me),0)
    for (v = DATA->SArea[lsb(King(me))] & Current->mask & (~Current->att[opp]); T(v); Cut(v)) AddCaptureP(IKing(me),lsb(King(me)),lsb(v),0)
    for (u = Knight(me); T(u); Cut(u))
        for (v = DATA->NAtt[lsb(u)] & Current->mask; T(v); Cut(v)) AddCaptureP(IKnight(me),lsb(u),lsb(v),0)
    for (u = Bishop(me); T(u); Cut(u))
        for (v = BishopAttacks(lsb(u),PieceAll) & Current->mask; T(v); Cut(v)) AddCapture(lsb(u),lsb(v),0)
    for (u = Rook(me); T(u); Cut(u))
        for (v = RookAttacks(lsb(u),PieceAll) & Current->mask; T(v); Cut(v)) AddCaptureP(IRook(me),lsb(u),lsb(v),0)
    for (u = Queen(me); T(u); Cut(u))
        for (v = QueenAttacks(lsb(u),PieceAll) & Current->mask; T(v); Cut(v)) AddCaptureP(IQueen(me),lsb(u),lsb(v),0)
finish:
    *list = 0;
    return list;
}

template <bool me> int * gen_evasions(int * list) {
    int king, att_sq, from;
    uint64_t att, esc, b, u;

    king = lsb(King(me));
    att = (DATA->NAtt[king] & Knight(opp)) | (DATA->PAtt[me][king] & Pawn(opp));
    for (u = (DATA->BMask[king] & BSlider(opp)) | (DATA->RMask[king] & RSlider(opp)); T(u); u ^= b) {
        b = Bit(lsb(u));
        if (F(DATA->Between[king][lsb(u)] & PieceAll)) att |= b;
    }
    att_sq = lsb(att);
    esc = DATA->SArea[king] & (~(Piece(me) | Current->att[opp])) & Current->mask;
    if (Square(att_sq) >= WhiteLight) esc &= ~DATA->FullLine[king][att_sq];
    Cut(att);
    if (att) {
        att_sq = lsb(att);
        if (Square(att_sq) >= WhiteLight) esc &= ~DATA->FullLine[king][att_sq];
        for (; T(esc); Cut(esc)) AddCaptureP(IKing(me),king,lsb(esc),0)
        *list = 0;
        return list;
    }
    if (Bit(att_sq) & Current->mask) {
        if (T(Current->ep_square) && Current->ep_square == att_sq + Push(me))
            for (u = DATA->PAtt[opp][att_sq + Push(me)] & Pawn(me); T(u); Cut(u)) AddMove(lsb(u),att_sq + Push(me),FlagEP,DATA->MvvLva[IPawn(me)][IPawn(opp)])
    }
    for (u = DATA->PAtt[opp][att_sq] & Pawn(me); T(u); Cut(u)) {
        from = lsb(u);
        if (Bit(att_sq) & Line(me,7)) AddMove(from,att_sq,FlagPQueen,MvvLvaPromotionCap(Square(att_sq)))
        else if (Bit(att_sq) & Current->mask) AddCaptureP(IPawn(me),from,att_sq,0)
    }
    for ( ; T(esc); Cut(esc)) AddCaptureP(IKing(me),king,lsb(esc),0)
    att = DATA->Between[king][att_sq];
    for (u = Shift(opp,att) & Pawn(me); T(u); Cut(u)) {
        from = lsb(u);
        if (Bit(from) & Line(me,6)) AddMove(from,from + Push(me),FlagPQueen,MvvLvaPromotion)
        else if (F(~Current->mask)) AddMove(from,from + Push(me),0,0)
    }
    if (F(~Current->mask)) {
        for (u = Shift(opp,Shift(opp,att)) & Line(me, 1) & Pawn(me); T(u); Cut(u))
            if (F(Square(lsb(u)+Push(me)))) AddMove(lsb(u),lsb(u) + 2 * Push(me),0,0)
    }
    att |= Bit(att_sq);
    for (u = Knight(me); T(u); Cut(u))
        for (esc = DATA->NAtt[lsb(u)] & att; T(esc); esc ^= b) {
            b = Bit(lsb(esc));
            if (b & Current->mask) AddCaptureP(IKnight(me),lsb(u),lsb(esc),0)
        }
    for (u = Bishop(me); T(u); Cut(u))
        for (esc = BishopAttacks(lsb(u),PieceAll) & att; T(esc); esc ^= b) {
            b = Bit(lsb(esc));
            if (b & Current->mask) AddCapture(lsb(u),lsb(esc),0)
        }
    for (u = Rook(me); T(u); Cut(u))
        for (esc = RookAttacks(lsb(u),PieceAll) & att; T(esc); esc ^= b) {
            b = Bit(lsb(esc));
            if (b & Current->mask) AddCaptureP(IRook(me),lsb(u),lsb(esc),0)
        }
    for (u = Queen(me); T(u); Cut(u))
        for (esc = QueenAttacks(lsb(u),PieceAll) & att; T(esc); esc ^= b) {
            b = Bit(lsb(esc));
            if (b & Current->mask) AddCaptureP(IQueen(me),lsb(u),lsb(esc),0)
        }
    *list = 0;
    return list;
}

void mark_evasions(int * list) {
    for (; T(*list); list++) {
        register int move = (*list) & 0xFFFF;
        if (F(Square(To(move))) && F(move & 0xE000)) {
            if (move == Current->ref[0]) *list |= RefOneScore;
            else if (move == Current->ref[1]) *list |= RefTwoScore;
            else if (move == Current->killer[1]) *list |= KillerOneScore;
            else if (move == Current->killer[2]) *list |= KillerTwoScore;
            else *list |= HistoryP(Square(From(move)),From(move),To(move));
        }
    }
}

template <bool me> int * gen_quiet_moves(int * list) {
    int to;
    uint64_t u, v, free, occ;

    occ = PieceAll;
    free = ~occ;
    if (me == White) {
        if (T(Current->castle_flags & CanCastle_OO) && F(occ & 0x60) && F(Current->att[Black] & 0x70)) AddHistoryP(IKing(White),4,6,FlagCastling)
        if (T(Current->castle_flags & CanCastle_OOO) && F(occ & 0xE) && F(Current->att[Black] & 0x1C)) AddHistoryP(IKing(White),4,2,FlagCastling)
    } else {
        if (T(Current->castle_flags & CanCastle_oo) && F(occ & 0x6000000000000000) && F(Current->att[White] & 0x7000000000000000)) AddHistoryP(IKing(Black),60,62,FlagCastling)
        if (T(Current->castle_flags & CanCastle_ooo) && F(occ & 0x0E00000000000000) && F(Current->att[White] & 0x1C00000000000000)) AddHistoryP(IKing(Black),60,58,FlagCastling)
    }
    for (v = Shift(me,Pawn(me)) & free & (~Line(me,7)); T(v); Cut(v)) {
        to = lsb(v);
        if (T(Bit(to) & Line(me,2)) && F(Square(to + Push(me)))) AddHistoryP(IPawn(me),to - Push(me),to + Push(me),0)
        AddHistoryP(IPawn(me),to - Push(me),to,0)
    }
    for (u = Knight(me); T(u); Cut(u))
        for (v = free & DATA->NAtt[lsb(u)]; T(v); Cut(v)) AddHistoryP(IKnight(me),lsb(u),lsb(v),0)
    for (u = Bishop(me); T(u); Cut(u))
        for (v = free & BishopAttacks(lsb(u),occ); T(v); Cut(v)) AddHistory(lsb(u),lsb(v))
    for (u = Rook(me); T(u); Cut(u))
        for (v = free & RookAttacks(lsb(u),occ); T(v); Cut(v)) AddHistoryP(IRook(me),lsb(u),lsb(v),0)
    for (u = Queen(me); T(u); Cut(u))
        for (v = free & QueenAttacks(lsb(u),occ); T(v); Cut(v)) AddHistoryP(IQueen(me),lsb(u),lsb(v),0)
    for (v = DATA->SArea[lsb(King(me))] & free & (~Current->att[opp]); T(v); Cut(v)) AddHistoryP(IKing(me),lsb(King(me)),lsb(v),0)
    *list = 0;
    return list;
}

template <bool me> int * gen_checks(int * list) {
    int king, from;
    uint64_t u, v, target, b_target, r_target, clear, xray;

    clear = ~(Piece(me) | Current->mask);
    king = lsb(King(opp));
    for (u = Current->xray[me] & Piece(me); T(u); Cut(u)) {
        from = lsb(u);
        target = clear & (~DATA->FullLine[king][from]);
        if (Square(from) == IPawn(me)) {
            if (F(Bit(from + Push(me)) & Line(me,7))) {
                if (T(Bit(from + Push(me)) & target) && F(Square(from + Push(me)))) AddMove(from,from + Push(me),0,MvvLvaXray)
                for (v = DATA->PAtt[me][from] & target & Piece(opp); T(v); Cut(v)) AddMove(from,lsb(v),0,MvvLvaXrayCap(Square(lsb(v))))
            }
        } else {
            if (Square(from) < WhiteLight) v = DATA->NAtt[from] & target;
            else if (Square(from) < WhiteRook) v = BishopAttacks(from,PieceAll) & target;
            else if (Square(from) < WhiteQueen) v = RookAttacks(from,PieceAll) & target;
            else if (Square(from) < WhiteKing) v = QueenAttacks(from,PieceAll) & target;
            else v = DATA->SArea[from] & target & (~Current->att[opp]);
            for ( ; T(v); Cut(v)) AddMove(from,lsb(v),0,MvvLvaXrayCap(Square(lsb(v))))
        }
    }
    xray = ~(Current->xray[me] & Board->bb[me]);
    for (u = Knight(me) & DATA->NArea[king] & xray; T(u); Cut(u))
        for (v = DATA->NAtt[king] & DATA->NAtt[lsb(u)] & clear; T(v); Cut(v)) AddCaptureP(IKnight(me),lsb(u),lsb(v),0)
    for (u = DATA->DArea[king] & Pawn(me) & (~Line(me,6)) & xray; T(u); Cut(u)) {
        from = lsb(u);
        for (v = DATA->PAtt[me][from] & DATA->PAtt[opp][king] & clear & Piece(opp); T(v); Cut(v)) AddCaptureP(IPawn(me),from,lsb(v),0)
        if (F(Square(from + Push(me))) && T(Bit(from + Push(me)) & DATA->PAtt[opp][king])) AddMove(from,from + Push(me),0,0)
    }
    b_target = BishopAttacks(king,PieceAll) & clear;
    r_target = RookAttacks(king,PieceAll) & clear;
    for (u = (Odd(king ^ Rank(king)) ? Board->bb[WhiteLight | me] : Board->bb[WhiteDark | me]) & xray; T(u); Cut(u))
        for (v = BishopAttacks(lsb(u),PieceAll) & b_target; T(v); Cut(v)) AddCapture(lsb(u),lsb(v),0)
    for (u = Rook(me) & xray; T(u); Cut(u)) 
        for (v = RookAttacks(lsb(u),PieceAll) & r_target; T(v); Cut(v)) AddCaptureP(IRook(me),lsb(u),lsb(v),0)
    for (u = Queen(me) & xray; T(u); Cut(u)) 
        for (v = QueenAttacks(lsb(u),PieceAll) & (b_target | r_target); T(v); Cut(v)) AddCaptureP(IQueen(me),lsb(u),lsb(v),0)
    *list = 0;
    return list;
}

template <bool me> int * gen_delta_moves(int * list) {
    int to;
    uint64_t u, v, free, occ;

    occ = PieceAll;
    free = ~occ;
    if (me == White) {
        if (T(Current->castle_flags & CanCastle_OO) && F(occ & 0x60) && F(Current->att[Black] & 0x70)) AddCDeltaP(IKing(White),4,6,FlagCastling)
        if (T(Current->castle_flags & CanCastle_OOO) && F(occ & 0xE) && F(Current->att[Black] & 0x1C)) AddCDeltaP(IKing(White),4,2,FlagCastling)
    } else {
        if (T(Current->castle_flags & CanCastle_oo) && F(occ & 0x6000000000000000) && F(Current->att[White] & 0x7000000000000000)) AddCDeltaP(IKing(Black),60,62,FlagCastling)
        if (T(Current->castle_flags & CanCastle_ooo) && F(occ & 0x0E00000000000000) && F(Current->att[White] & 0x1C00000000000000)) AddCDeltaP(IKing(Black),60,58,FlagCastling)
    }
    for (v = Shift(me,Pawn(me)) & free & (~Line(me,7)); T(v); Cut(v)) {
        to = lsb(v);
        if (T(Bit(to) & Line(me,2)) && F(Square(to + Push(me)))) AddCDeltaP(IPawn(me),to - Push(me),to + Push(me),0)
        AddCDeltaP(IPawn(me),to - Push(me),to,0)
    }
    for (u = Knight(me); T(u); Cut(u))
        for (v = free & DATA->NAtt[lsb(u)]; T(v); Cut(v)) AddCDeltaP(IKnight(me),lsb(u),lsb(v),0)
    for (u = Bishop(me); T(u); Cut(u))
        for (v = free & BishopAttacks(lsb(u),occ); T(v); Cut(v)) AddCDelta(lsb(u),lsb(v))
    for (u = Rook(me); T(u); Cut(u))
        for (v = free & RookAttacks(lsb(u),occ); T(v); Cut(v)) AddCDeltaP(IRook(me),lsb(u),lsb(v),0)
    for (u = Queen(me); T(u); Cut(u))
        for (v = free & QueenAttacks(lsb(u),occ); T(v); Cut(v)) AddCDeltaP(IQueen(me),lsb(u),lsb(v),0)
    for (v = DATA->SArea[lsb(King(me))] & free & (~Current->att[opp]); T(v); Cut(v)) AddCDeltaP(IKing(me),lsb(King(me)),lsb(v),0)
    *list = 0;
    return list;
}

template <bool me> int singular_extension(int ext, int prev_ext, int margin_one, int margin_two, int depth, int killer) {
    int value = -MateValue;
    int singular = 0;
    if (ext < 1 + (prev_ext < 1)) {
        if (Check(me)) value = search_evasion<me, 1>(margin_one, depth, killer); 
        else value = search<me, 1>(margin_one, depth, killer); 
        if (value < margin_one) singular = 1;
    }
    if (value < margin_one && ext < 2 + (prev_ext < 1) - (prev_ext >= 2)) {
        if (Check(me)) value = search_evasion<me, 1>(margin_two, depth, killer); 
        else value = search<me, 1>(margin_two, depth, killer); 
        if (value < margin_two) singular = 2;
    }
    return singular;
}

template <bool me> inline void capture_margin(int alpha, int &score) {
    if (Current->score + 200 < alpha) {
        if (Current->att[me] & Pawn(opp)) {
            Current->mask ^= Pawn(opp);
            score = Current->score + 200;
        }
        if (Current->score + 500 < alpha) {
            if (Current->att[me] & Minor(opp)) {
                Current->mask ^= Minor(opp);
                score = Current->score + 500;
            }
            if (Current->score + 700 < alpha) {
                if (Current->att[me] & Rook(opp)) {
                    Current->mask ^= Rook(opp);
                    score = Current->score + 700;
                }
                if (Current->score + 1400 < alpha && (Current->att[me] & Queen(opp))) {
                    Current->mask ^= Queen(opp);
                    score = Current->score + 1400;
                }
            }
        }
    }
}

template <bool me, bool pv> int q_search(int alpha, int beta, int depth, int flags) {
    int i, value, score, move, hash_move, hash_depth, cnt;
    GEntry * Entry;

    if (flags & FlagHaltCheck) halt_check;
    if (flags & FlagCallEvaluation) evaluate();
    if (Check(me)) return q_evasion<me, pv>(alpha, beta, depth, FlagHashCheck);
    score = Current->score + 3;
    if (score > alpha) {
        alpha = score;
        if (score >= beta) return score;
    }

    hash_move = hash_depth = 0;
    if (flags & FlagHashCheck) {
        for (i = 0, Entry = HASH + (High32(Current->key) & hash_mask); i < 4; Entry++, i++) {
            if (Low32(Current->key) == Entry->key) {
                if (T(Entry->low_depth)) {
                    if (Entry->low >= beta && !pv) return Entry->low;
                    if (Entry->low_depth > hash_depth && T(Entry->move)) {
                        hash_move = Entry->move;
                        hash_depth = Entry->low_depth;
                    }
                }
                if (T(Entry->high_depth) && Entry->high <= alpha && !pv) return Entry->high;
                break;
            }
        }
    }

    Current->mask = Piece(opp);
    capture_margin<me>(alpha, score);

    cnt = 0;
    if (T(hash_move)) {
        if (F(Bit(To(hash_move)) & Current->mask) && F(hash_move & 0xE000) && (depth < -8 || (Current->score + DeltaM(hash_move) <= alpha && F(is_check<me>(hash_move))))) goto skip_hash_move;
        if (is_legal<me>(move = hash_move)) {
            if (IsIllegal(me,move)) goto skip_hash_move;
            if (SeeValue[Square(To(move))] > SeeValue[Square(From(move))]) cnt++;
            do_move<me>(move);
            value = -q_search<opp, pv>(-beta, -alpha, depth - 1, FlagNeatSearch);
            undo_move<me>(move);
            if (value > score) {
                score = value;
                if (value > alpha) {
                    alpha = value;
                    if (value >= beta) goto cut;
                }
            }
            if (F(Bit(To(hash_move)) & Current->mask) && F(hash_move & 0xE000) && (depth < -2 || depth <= -1 && Current->score + 50 < alpha) && alpha >= beta - 1 && !pv) return alpha;
        }
    }
skip_hash_move:
    gen_captures<me>(Current->moves);
    Current->current = Current->moves;
    while (move = pick_move()) {
        if (move == hash_move) continue;
        if (IsIllegal(me,move)) continue;
        if (F(see<me>(move,-50))) continue;
        if (SeeValue[Square(To(move))] > SeeValue[Square(From(move))]) cnt++;
        do_move<me>(move);
        value = -q_search<opp, pv>(-beta, -alpha, depth - 1, FlagNeatSearch);
        undo_move<me>(move);
        if (value > score) {
            score = value;
            if (value > alpha) {
                alpha = value;
                if (value >= beta) goto cut;
            }
        }
    }

    if (depth < -2) goto finish;
    if (depth <= -1 && Current->score + 50 < alpha) goto finish;
    gen_checks<me>(Current->moves);
    Current->current = Current->moves;
    while (move = pick_move()) {
        if (move == hash_move) continue;
        if (IsIllegal(me,move)) continue;
        if (IsRepetition(alpha + 1,move)) continue;
        if (F(see<me>(move,-50))) continue;
        do_move<me>(move);
        value = -q_evasion<opp, pv>(-beta, -alpha, depth - 1, FlagNeatSearch);
        undo_move<me>(move);
        if (value > score) {
            score = value;
            if (value > alpha) {
                alpha = value;
                if (value >= beta) goto cut;
            }
        }
    }

    if (T(cnt) || Current->score + 30 < alpha || T(Current->threat & Piece(me)) || T((Current->xray[opp] | Current->pin[opp]) & NonPawn(opp)) 
        || T(Pawn(opp) & Line(me, 1) & Shift(me,~PieceAll))) goto finish;
    Current->margin = alpha - Current->score + 6;
    gen_delta_moves<me>(Current->moves);
    Current->current = Current->moves;
    while (move = pick_move()) {
        if (move == hash_move) continue;
        if (IsIllegal(me,move)) continue;
        if (IsRepetition(alpha + 1,move)) continue;
        if (F(see<me>(move,-50))) continue;
        cnt++;
        do_move<me>(move);
        value = -q_search<opp, pv>(-beta, -alpha, depth - 1, FlagNeatSearch);
        undo_move<me>(move);
        if (value > score) {
            score = value;
            if (value > alpha) {
                alpha = value;
                if (value >= beta) {
                    if (Current->killer[1] != move) {
                        Current->killer[2] = Current->killer[1];
                        Current->killer[1] = move;
                    }
                    goto cut;
                }
            }
        }
        if (cnt >= 3) break; 
    }

finish:
    if (depth >= -2 && (depth >= 0 || Current->score + 50 >= alpha)) hash_high(score, 1);
    return score;
cut:
    hash_low(move, score, 1);
    return score;
}

template <bool me, bool pv> int q_evasion(int alpha, int beta, int depth, int flags) {
    int i, value, pext, score, move, cnt, hash_move, hash_depth;
    int *p;
    GEntry * Entry;

    score = Convert((Current - Data),int) - MateValue;
    if (flags & FlagHaltCheck) halt_check;

    hash_move = hash_depth = 0;
    if (flags & FlagHashCheck) {
        for (i = 0, Entry = HASH + (High32(Current->key) & hash_mask); i < 4; Entry++, i++) {
            if (Low32(Current->key) == Entry->key) {
                if (T(Entry->low_depth)) {
                    if (Entry->low >= beta && !pv) return Entry->low;
                    if (Entry->low_depth > hash_depth && T(Entry->move)) {
                        hash_move = Entry->move;
                        hash_depth = Entry->low_depth;
                    }
                }
                if (T(Entry->high_depth) && Entry->high <= alpha && !pv) return Entry->high;
                break;
            }
        }
    }

    if (flags & FlagCallEvaluation) evaluate();
    Current->mask = Filled;
    if (Current->score - 10 <= alpha && !pv) {
        Current->mask = Piece(opp);
        score = Current->score - 10;
        capture_margin<me>(alpha, score);
    }

    alpha = Max(score, alpha);
    pext = 0;
    gen_evasions<me>(Current->moves);
    Current->current = Current->moves;
    if (F(Current->moves[0])) return score;
    if (F(Current->moves[1])) pext = 1;
    else {
        Current->ref[0] = RefM(Current->move).check_ref[0];
        Current->ref[1] = RefM(Current->move).check_ref[1];
        mark_evasions(Current->moves);
        if (T(hash_move) && (T(Bit(To(hash_move)) & Current->mask) || T(hash_move & 0xE000))) {
            for (p = Current->moves; T(*p); p++) {
                if (((*p) & 0xFFFF) == hash_move) {
                    *p |= 0x7FFF0000;
                    break;
                }
            }
        }
    }
    cnt = 0;
    while (move = pick_move()) {
        if (IsIllegal(me,move)) continue;
        cnt++;
        if (IsRepetition(alpha + 1,move)) {
            score = Max(0, score);
            continue;
        }
        if (F(Square(To(move))) && F(move & 0xE000)) {
            if (cnt > 3 && F(is_check<me>(move)) && !pv) continue;
            if ((value = Current->score + DeltaM(move) + 10) <= alpha && !pv) {
                score = Max(value, score);
                continue;
            }
        }
        do_move<me>(move);
        value = -q_search<opp, pv>(-beta, -alpha, depth - 1 + pext, FlagNeatSearch);
        undo_move<me>(move);
        if (value > score) {
            score = value;
            if (value > alpha) {
                alpha = value;
                if (value >= beta) goto cut;
            }
        }
    }
    return score;
cut:
    return score;
}

void send_position(GPos * Pos) {
    Pos->Position->key = Current->key;
    Pos->Position->pawn_key = Current->pawn_key;
    Pos->Position->move = Current->move;
    Pos->Position->capture = Current->capture;
    Pos->Position->turn = Current->turn;
    Pos->Position->castle_flags = Current->castle_flags;
    Pos->Position->ply = Current->ply;
    Pos->Position->ep_square = Current->ep_square;
    Pos->Position->piece = Current->piece;
    Pos->Position->pst = Current->pst;
    Pos->Position->material = Current->material;
    for (int i = 0; i < 64; i++) Pos->Position->square[i] = Board->square[i];
    Pos->date = SHARED->date;
    Pos->sp = sp;
    for (int i = 0; i <= Current->ply; i++) Pos->stack[i] = Stack[sp - i];
    for (int i = 0; i < Min(16, 126 - (int)(Current - Data)); i++) {
        Pos->killer[i][0] = (Current + i + 1)->killer[1];
        Pos->killer[i][1] = (Current + i + 1)->killer[2];
    }
    for (int i = Min(16, 126 - (int)(Current - Data)); i < 16; i++) Pos->killer[i][0] = Pos->killer[i][1] = 0;
}

void retrieve_board(GPos * Pos) {
    for (int i = 0; i < 16; i++) Board->bb[i] = 0;
    for (int i = 0; i < 64; i++) {
        int piece = Pos->Position->square[i];
        Board->square[i] = piece;
        if (piece) {
            Board->bb[piece & 1] |= Bit(i);
            Board->bb[piece] |= Bit(i);
        }
    }
}

void retrieve_position(GPos * Pos, int copy_stack) {
    Current->key = Pos->Position->key;
    Current->pawn_key = Pos->Position->pawn_key;
    Current->move = Pos->Position->move;
    Current->capture = Pos->Position->capture;
    Current->turn = Pos->Position->turn;
    Current->castle_flags = Pos->Position->castle_flags;
    Current->ply = Pos->Position->ply;
    Current->ep_square = Pos->Position->ep_square;
    Current->piece = Pos->Position->piece;
    Current->pst = Pos->Position->pst;
    Current->material = Pos->Position->material;
    retrieve_board(Pos);
    SHARED->date = Pos->date;
    if (copy_stack) {
        sp = Current->ply;
        for (int i = 0; i <= sp; i++) Stack[sp - i] = Pos->stack[i];
    } else sp = Pos->sp;
    for (int i = 0; i < 16; i++) {
        (Current + i + 1)->killer[1] = Pos->killer[i][0];
        (Current + i + 1)->killer[2] = Pos->killer[i][1];
    }
}

template <bool me, bool exclusion> int search(int beta, int depth, int flags) {
    int i, value, cnt, flag, moves_to_play, check, score, move, ext, margin, hash_move, singular, played,
        high_depth, high_value, hash_value, new_depth, move_back, hash_depth, *p;
    int height = (int)(Current - Data);

    check_for_stop();

    if (depth <= 1) return q_search<me, 0>(beta - 1, beta, 1, flags);
    if (flags & FlagHaltCheck) {
        if (height - MateValue >= beta) return beta;
        if (MateValue - height < beta) return beta - 1;
        halt_check;
    }

    if (exclusion) {
        cnt = high_depth = singular = played = 0;
        flag = 1;
        score = beta - 1;
        high_value = MateValue; 
        hash_value = -MateValue;
        hash_depth = -1;
        hash_move = flags & 0xFFFF;
        goto skip_hash_move;
    }

    if (flags & FlagCallEvaluation) evaluate();
    if (Check(me)) return search_evasion<me, 0>(beta, depth, flags & (~(FlagHaltCheck | FlagCallEvaluation)));

    if ((value = Current->score - 90 - (depth << 3) - (Max(depth - 5, 0) << 5)) >= beta && F(Pawn(opp) & Line(me, 1) & Shift(me,~PieceAll)) && T(NonPawnKing(me)) && F(flags & (FlagReturnBestMove | FlagDisableNull)) && depth <= 13) return value;
    if ((value = Current->score + 50) < beta && depth <= 3) return MaxF(value, q_search<me, 0>(beta - 1, beta, 1, FlagHashCheck | (flags & 0xFFFF)));

    high_depth = 0;
    high_value = MateValue;
    hash_value = -MateValue;
    hash_depth = -1;
    Current->best = hash_move = flags & 0xFFFF;
    if (GEntry * Entry = probe_hash()) {
        if (Entry->high_depth > high_depth) {
            high_depth = Entry->high_depth;
            high_value = Entry->high;
        }
        if (Entry->high < beta && Entry->high_depth >= depth) return Entry->high;
        if (T(Entry->move) && Entry->low_depth > hash_depth) {
            Current->best = hash_move = Entry->move;
            hash_depth = Entry->low_depth;
            if (Entry->low_depth) hash_value = Entry->low;
        }
        if (Entry->low >= beta && Entry->low_depth >= depth) {
            if (Entry->move) {
                Current->best = Entry->move;
                if (F(Square(To(Entry->move))) && F(Entry->move & 0xE000)) {
                    if (Current->killer[1] != Entry->move && F(flags & FlagNoKillerUpdate)) {
                        Current->killer[2] = Current->killer[1];
                        Current->killer[1] = Entry->move;
                    }
                    UpdateRef(Entry->move);
                }
                return Entry->low;
            }
            if (F(flags & FlagReturnBestMove)) return Entry->low;
        }
    }

    if (hash_depth < 0 && depth >= SETTINGS->syzygyProbeDepth &&
            TB_LARGEST > 0 && popcount(PieceAll) <= TB_LARGEST)
    {
        unsigned res = tb_probe_wdl(Piece(White), Piece(Black),
            King(White) | King(Black),
            Queen(White) | Queen(Black),
            Rook(White) | Rook(Black),
            Bishop(White) | Bishop(Black),
            Knight(White) | Knight(Black),
            Pawn(White) | Pawn(Black),
            Current->ply,
            Current->castle_flags,
            Current->ep_square,
            (me == White));
        if (res != TB_RESULT_FAILED)
        {
            INFO->tbHits++;
            hash_high(TbValues[res], TbDepth);
            hash_low(0, TbValues[res], TbDepth);
            return TbValues[res];
        }
    }

    if (depth >= 20) if (GPVEntry * PVEntry = probe_pv_hash()) {
        hash_low(PVEntry->move,PVEntry->value,PVEntry->depth);
        hash_high(PVEntry->value,PVEntry->depth);
        if (PVEntry->depth >= depth) {
            if (PVEntry->move) Current->best = PVEntry->move;
            if (F(flags & FlagReturnBestMove) && ((Current->ply <= 50 && PVEntry->ply <= 50) || (Current->ply >= 50 && PVEntry->ply >= 50))) return PVEntry->value;
        }
        if (T(PVEntry->move) && PVEntry->depth > hash_depth) {
            Current->best = hash_move = PVEntry->move;
            hash_depth = PVEntry->depth;
            hash_value = PVEntry->value;
        }
    }
    if (depth < 10) score = height - MateValue;
    else score = beta - 1;
    if (depth >= 12 && (F(hash_move) || hash_value < beta || hash_depth < depth - 12) && (high_value >= beta || high_depth < depth - 12) && F(flags & FlagDisableNull)) {
        new_depth = depth - 8;
        value = search<me, 0>(beta, new_depth, FlagHashCheck | FlagNoKillerUpdate | FlagDisableNull | FlagReturnBestMove | hash_move);
        if (value >= beta) {
            if (Current->best) hash_move = Current->best;
            hash_depth = new_depth;
            hash_value = beta;
        }
    }
    if (depth >= 4 && Current->score + 3 >= beta && F(flags & (FlagDisableNull | FlagReturnBestMove))
        && (high_value >= beta || high_depth < depth - 10) && (depth < 12 || (hash_value >= beta && hash_depth >= depth - 12)) && beta > -EvalValue && T(NonPawnKing(me))) {
        new_depth = depth - 8;
        do_null();
        value = -search<opp, 0>(1 - beta, new_depth, FlagHashCheck);
        undo_null();
        if (value >= beta) {
            if (depth < 12) hash_low(0, value, depth);
            return value;
        }
    }

    cnt = flag = singular = played = 0;
    if (T(hash_move) && is_legal<me>(move = hash_move)) {
        if (IsIllegal(me,move)) goto skip_hash_move;
        cnt++;
        check = is_check<me>(move);
        if (check) ext = 1 + (depth < 16);
        else ext = extension<0>(move, depth);
        if (depth >= 16 && hash_value >= beta && hash_depth >= (new_depth = depth - Min(12, depth/2))) {
            int margin_one = beta - ExclSingle(depth);
            int margin_two = beta - ExclDouble(depth);
            int prev_ext = Ext(flags);
            singular = singular_extension<me>(ext,prev_ext,margin_one,margin_two,new_depth,hash_move);
            if (singular) ext = Max(ext, singular + (prev_ext < 1) - (singular >= 2 && prev_ext >= 2));
        }
        if (depth < 16 && To(move) == To(Current->move) && T(Square(To(move)))) ext = Max(ext, 2);
        new_depth = depth - 2 + ext;
        do_move<me>(move);
        value = -search<opp, 0>(1 - beta, new_depth, FlagNeatSearch | ((hash_value >= beta && hash_depth >= depth - 12) ? FlagDisableNull : 0) | ExtFlag(ext));
        undo_move<me>(move);
        played++;
        if (value > score) {
            score = value;
            if (value >= beta) goto cut;
        }
    }
skip_hash_move:
    Current->killer[0] = 0;
    Current->stage = stage_search;
    Current->gen_flags = 0;
    Current->ref[0] = RefM(Current->move).ref[0];
    Current->ref[1] = RefM(Current->move).ref[1];
    move_back = 0;
    if (beta > 0 && Current->ply >= 2) {
        if (F((Current - 1)->move & 0xF000)) {
            move_back = (To((Current - 1)->move) << 6) | From((Current - 1)->move);
            if (Square(To(move_back))) move_back = 0;
        }
    }
    moves_to_play = 3 + (depth * depth)/6;
    margin = Current->score + 70 + (depth << 3) + (Max(depth - 7, 0) << 5);
    if ((value = margin) < beta && depth <= 19) {
        flag = 1;
        score = Max(value, score);
        Current->stage = stage_razoring;
        Current->mask = Piece(opp);
        if ((value = Current->score + 200 + (depth << 5)) < beta) {
            score = Max(value, score);
            Current->mask ^= Pawn(opp);
        }
    }
    Current->current = Current->moves;
    Current->moves[0] = 0;
    if (depth <= 5) Current->gen_flags |= FlagNoBcSort;
    
    while (move = get_move<me,0>()) {
        if (move == hash_move) continue;
        if (IsIllegal(me,move)) continue;
        cnt++;
        if (move == move_back) {
            score = Max(0, score);
            continue;
        }
        if (Current->stage == r_checks) check = 1;
        else check = is_check<me>(move);
        if (T(check) && T(see<me>(move, 0))) ext = 1 + (depth < 16);
        else ext = extension<0>(move, depth);
        new_depth = depth - 2 + ext;
        if (F(Square(To(move))) && F(move & 0xE000)) {
            if (move != Current->killer[1] && move != Current->killer[2]) {
                if (F(check) && cnt > moves_to_play) {
                    Current->gen_flags &= ~FlagSort;
                    continue;
                }
                if (depth >= 6) {
                    int reduction = msb(cnt);
                    if (move == Current->ref[0] || move == Current->ref[1]) reduction = Max(0, reduction - 1);
                    if (reduction >= 2 && !(Queen(White) | Queen(Black)) && popcount(NonPawnKingAll) <= 4) reduction += reduction / 2;
                    if (new_depth - reduction > 3)
                        if (F(see<me>(move, -50))) reduction += 2;
                    if (T(reduction) && reduction < 2 && new_depth - reduction > 3) {
                        if (cnt > 3) reduction = 2;
                        else reduction = 0;
                    }
                    new_depth = Max(3, new_depth - reduction);
                }
            }
            if (F(check)) {
                if ((value = Current->score + DeltaM(move) + 10) < beta && depth <= 3) {
                    score = Max(value, score);
                    continue;
                }
                if (cnt > 7 && (value = margin + DeltaM(move) - 25 * msb(cnt)) < beta && depth <= 19) {
                    score = Max(value, score);
                    continue;
                }
            }
            if (depth <= 9 && T(NonPawnKing(me)) && F(see<me>(move,-50))) continue;
        } else {
            if (Current->stage == r_cap) {
                if (F(check) && depth <= 9 && F(see<me>(move,-50))) continue;
            } else if (Current->stage == s_bad_cap && F(check) && depth <= 5) continue;
        }
make_move:
        do_move<me>(move);
        value = -search<opp, 0>(1 - beta, new_depth, FlagNeatSearch | ExtFlag(ext));
        if (value >= beta && new_depth < depth - 2 + ext) value = -search<opp, 0>(1 - beta, depth - 2 + ext, FlagNeatSearch | FlagDisableNull | ExtFlag(ext));
        undo_move<me>(move);
        played++;
        if (value > score) {
            score = value;
            if (value >= beta) goto cut;
        }
    }
    if (F(cnt) && F(flag)) {
        hash_high(0, 127);
        hash_low(0, 0, 127);
        return 0;
    }
    if (F(exclusion)) hash_high(score, depth);
    return score;
cut:
    if (exclusion) return score;
    Current->best = move;
    if (depth >= 10) score = Min(beta, score);
    hash_low(move, score, depth);
    if (F(Square(To(move))) && F(move & 0xE000)) {
        if (Current->killer[1] != move && F(flags & FlagNoKillerUpdate)) {
            Current->killer[2] = Current->killer[1];
            Current->killer[1] = move;
        }
        HistoryGood(move);
        if (move != hash_move && Current->stage == s_quiet) for (p = Current->start; p < (Current->current - 1); p++) HistoryBad(*p);
        UpdateRef(move);
    }
    return score;
}

template <bool me, bool exclusion> int search_evasion(int beta, int depth, int flags) {
    int i, value, score, pext, move, cnt, hash_value = -MateValue, hash_depth, hash_move, new_depth, ext, check, moves_to_play;
    int height = (int)(Current - Data);

    if (depth <= 1) return q_evasion<me, 0>(beta - 1, beta, 1, flags);
    score = height - MateValue;
    if (flags & FlagHaltCheck) {
        if (score >= beta) return beta;
        if (MateValue - height < beta) return beta - 1;
        halt_check;
    }

    hash_depth = -1;
    hash_move = flags & 0xFFFF;
    if (exclusion) {
        cnt = pext = 0;
        score = beta - 1;
        gen_evasions<me>(Current->moves);
        if (F(Current->moves[0])) return score;
        goto skip_hash_move;
    }
    Current->best = hash_move;
    if (GEntry * Entry = probe_hash()) {
        if (Entry->high < beta && Entry->high_depth >= depth) return Entry->high;
        if (T(Entry->move) && Entry->low_depth > hash_depth) {
            Current->best = hash_move = Entry->move;
            hash_depth = Entry->low_depth;
        }
        if (Entry->low >= beta && Entry->low_depth >= depth) {
            if (Entry->move) {
                Current->best = Entry->move;
                if (F(Square(To(Entry->move))) && F(Entry->move & 0xE000)) UpdateCheckRef(Entry->move);
            }
            return Entry->low;
        }
        if (Entry->low_depth >= depth - 8 && Entry->low > hash_value) hash_value = Entry->low;
    }

    if (depth >= 20) if (GPVEntry * PVEntry  = probe_pv_hash()) {
        hash_low(PVEntry->move,PVEntry->value,PVEntry->depth);
        hash_high(PVEntry->value,PVEntry->depth);
        if (PVEntry->depth >= depth) {
            if (PVEntry->move) Current->best = PVEntry->move;
            return PVEntry->value;
        }
        if (T(PVEntry->move) && PVEntry->depth > hash_depth) {
            Current->best = hash_move = PVEntry->move;
            hash_depth = PVEntry->depth;
            hash_value = PVEntry->value;
        }
    }

    if (hash_depth < 0 && depth >= SETTINGS->syzygyProbeDepth &&
            TB_LARGEST > 0 && popcount(PieceAll) <= TB_LARGEST)
    {
        unsigned res = tb_probe_wdl(Piece(White), Piece(Black),
            King(White) | King(Black),
            Queen(White) | Queen(Black),
            Rook(White) | Rook(Black),
            Bishop(White) | Bishop(Black),
            Knight(White) | Knight(Black),
            Pawn(White) | Pawn(Black),
            Current->ply,
            Current->castle_flags,
            Current->ep_square,
            (me == White));
        if (res != TB_RESULT_FAILED)
        {
            INFO->tbHits++;
            hash_high(TbValues[res], TbDepth);
            hash_low(0, TbValues[res], TbDepth);
            return TbValues[res];
        }
    }

    if (hash_depth >= depth && hash_value > -EvalValue) score = Min(beta - 1, Max(score, hash_value));
    if (flags & FlagCallEvaluation) evaluate();

    Current->mask = Filled;
    if (Current->score - 10 < beta && depth <= 3) {
        Current->mask = Piece(opp);
        score = Current->score - 10;
        capture_margin<me>(beta, score);
    }
    cnt = 0;
    pext = 0;
    gen_evasions<me>(Current->moves);
    if (F(Current->moves[0])) return score;
    if (F(Current->moves[1])) pext = 2;

    if (T(hash_move) && is_legal<me>(move = hash_move)) {
        if (IsIllegal(me,move)) goto skip_hash_move;
        cnt++;
        check = is_check<me>(move);
        if (check) ext = Max(pext, 1 + (depth < 16));
        else ext = MaxF(pext, extension<0>(move, depth));
        if (depth >= 16 && hash_value >= beta && hash_depth >= (new_depth = depth - Min(12, depth/2))) {
            int margin_one = beta - ExclSingle(depth);
            int margin_two = beta - ExclDouble(depth);
            int prev_ext = Ext(flags);
            int singular = singular_extension<me>(ext,prev_ext,margin_one,margin_two,new_depth,hash_move);
            if (singular) ext = Max(ext, singular + (prev_ext < 1) - (singular >= 2 && prev_ext >= 2));
        }
        new_depth = depth - 2 + ext;
        do_move<me>(move);
        evaluate();
        if (Current->att[opp] & King(me)) {
            undo_move<me>(move);
            cnt--;
            goto skip_hash_move;
        }
        value = -search<opp, 0>(1 - beta, new_depth, FlagHaltCheck | FlagHashCheck | ((hash_value >= beta && hash_depth >= depth - 12) ? FlagDisableNull : 0) | ExtFlag(ext));
        undo_move<me>(move);
        if (value > score) {
            score = value;
            if (value >= beta) goto cut;
        }
    }
skip_hash_move:
    moves_to_play = 3 + ((depth * depth) / 6); 
    Current->ref[0] = RefM(Current->move).check_ref[0];
    Current->ref[1] = RefM(Current->move).check_ref[1];
    mark_evasions(Current->moves);
    Current->current = Current->moves;
    while (move = pick_move()) {
        if (move == hash_move) continue;
        if (IsIllegal(me,move)) continue;
        cnt++;
        if (IsRepetition(beta,move)) {
            score = Max(0, score);
            continue;
        }
        check = is_check<me>(move);
        if (check) ext = Max(pext, 1 + (depth < 16));
        else ext = MaxF(pext, extension<0>(move, depth));
        new_depth = depth - 2 + ext;
        if (F(Square(To(move))) && F(move & 0xE000)) {
            if (F(check)) {
                if (cnt > moves_to_play) continue;
                if ((value = Current->score + DeltaM(move) + 10) < beta && depth <= 3) {
                    score = Max(value, score);
                    continue;
                }
            }
            if (depth >= 6 && cnt > 3) {
                int reduction = msb(cnt);
                if (reduction >= 2 && !(Queen(White) | Queen(Black)) && popcount(NonPawnKingAll) <= 4) reduction += reduction / 2;
                new_depth = Max(3, new_depth - reduction);
            }
        }
        do_move<me>(move);
        value = -search<opp, 0>(1 - beta, new_depth, FlagNeatSearch | ExtFlag(ext));
        if (value >= beta && new_depth < depth - 2 + ext) value = -search<opp, 0>(1 - beta, depth - 2 + ext, FlagNeatSearch | FlagDisableNull | ExtFlag(ext));
        undo_move<me>(move);
        if (value > score) {
            score = value;
            if (value >= beta) goto cut;
        }
    }
    if (F(exclusion)) hash_high(score, depth);
    return score;
cut:
    if (exclusion) return score;
    Current->best = move;
    hash_low(move, score, depth);
    if (F(Square(To(move))) && F(move & 0xE000)) UpdateCheckRef(move);
    return score;
}

template <bool me, bool root> int pv_search(int alpha, int beta, int depth, int flags) {
    int i, value, move, cnt, pext = 0, ext, check, hash_value = -MateValue, margin, singular = 0, played = 0, new_depth, hash_move, hash_depth, old_alpha = alpha, old_best, ex_depth = 0, ex_value = 0, start_knodes = (INFO->nodes >> 10);
    int height = (int)(Current - Data);

    if (root) {
        depth = Max(depth, 2);
        flags |= ExtFlag(1);
        if (F(RootList[0])) return 0;
        if (INFO->id == 0)
        {
            char line[128];
            int len = snprintf(line, sizeof(line)-1, "info depth %d\n",
                depth/2);
            if (len > 0 && len < sizeof(line)-1)
            {
                mutex_lock(&SHARED->mutex);
                put_line(line, len);
                mutex_unlock(&SHARED->mutex);
            }
        }
        int * p;
        for (p = RootList; *p; p++);
        sort_moves(RootList,p);
        for (p = RootList; *p; p++) *p &= 0xFFFF;
        SetScore(RootList[0],2);
        goto check_hash;
    }
    if (depth <= 1) return q_search<me, 1>(alpha, beta, 1, FlagNeatSearch);
    if (Convert((Current - Data),int) - MateValue >= beta) return beta;
    if (MateValue - Convert((Current - Data),int) <= alpha) return alpha;
    halt_check;

check_hash:
    hash_depth = -1;
    Current->best = hash_move = 0;
    if (GPVEntry * PVEntry = probe_pv_hash()) {
        hash_low(PVEntry->move,PVEntry->value,PVEntry->depth);
        hash_high(PVEntry->value,PVEntry->depth);
        if (PVEntry->depth >= depth && T(PVHashing)) {
            if (PVEntry->move) Current->best = PVEntry->move;
            if ((Current->ply <= 50 && PVEntry->ply <= 50) || (Current->ply >= 50 && PVEntry->ply >= 50)) if (!PVEntry->value || !draw_in_pv<me>()) return PVEntry->value;
        }
        if (T(PVEntry->move) && PVEntry->depth > hash_depth) {
            Current->best = hash_move = PVEntry->move;
            hash_depth = PVEntry->depth;
            hash_value = PVEntry->value;
        }
    }
    if (GEntry * Entry = probe_hash()) {
        if (T(Entry->move) && Entry->low_depth > hash_depth) {
            Current->best = hash_move = Entry->move;
            hash_depth = Entry->low_depth;
            if (Entry->low_depth) hash_value = Entry->low;
        }
    }

    if (hash_depth < 0 && TB_LARGEST > 0 && popcount(PieceAll) <= TB_LARGEST)
    {
        unsigned res = tb_probe_wdl(Piece(White), Piece(Black),
            King(White) | King(Black),
            Queen(White) | Queen(Black),
            Rook(White) | Rook(Black),
            Bishop(White) | Bishop(Black),
            Knight(White) | Knight(Black),
            Pawn(White) | Pawn(Black),
            Current->ply,
            Current->castle_flags,
            Current->ep_square,
            (me == White));
        if (res != TB_RESULT_FAILED)
        {
            INFO->tbHits++;
            hash_high(TbValues[res], TbDepth);
            hash_low(0, TbValues[res], TbDepth);
            return TbValues[res];
        }
    }

    if (root) {
        hash_move = RootList[0];
        hash_value = Previous;
        hash_depth = Max(0, depth - 2);
    }

    evaluate();

    if (F(root) && depth >= 6 && (F(hash_move) || hash_value <= alpha || hash_depth < depth - 8)) {
        if (F(hash_move)) new_depth = depth - 2;
        else new_depth = depth - 4;
        value = pv_search<me, 0>(alpha, beta, new_depth, hash_move);
        if (value > alpha) {
hash_move_found:
            if (Current->best) hash_move = Current->best;
            hash_depth = new_depth;
            hash_value = value;
            goto skip_iid;
        } else {
            i = 0;        
            new_depth = depth - 8;
iid_loop:
            margin = alpha - (8 << i);
            if (T(hash_move) && hash_depth >= Min(new_depth, depth - 8) && hash_value >= margin) goto skip_iid;
            value = search<me, 0>(margin, new_depth, FlagHashCheck | FlagNoKillerUpdate | FlagDisableNull | FlagReturnBestMove | hash_move);
            if (value >= margin) goto hash_move_found;
            i++;
            if (i < 5) goto iid_loop;
        }
    }
skip_iid:
    if (F(root) && Check(me)) {
        alpha = Max(Convert((Current - Data),int) - MateValue, alpha);
        Current->mask = Filled;
        gen_evasions<me>(Current->moves);
        if (F(Current->moves[0])) return Convert((Current - Data),int) - MateValue; 
        if (F(Current->moves[1])) pext = 2;
    }

    cnt = 0;
    if (hash_move && is_legal<me>(move = hash_move)) {
        cnt++;
        if (root)
        {
            memset(Data + 1, 0, 127 * sizeof(GData));
            send_curr_move(move, cnt);
        }
        check = is_check<me>(move);
        if (check) ext = 2;
        else ext = MaxF(pext, extension<1>(move, depth));
        if (depth >= 12 && hash_value > alpha && hash_depth >= (new_depth = depth - Min(12,depth/2))) {
            int margin_one = hash_value - ExclSinglePV(depth);
            int margin_two = hash_value - ExclDoublePV(depth);
            int prev_ext = Ext(flags);
            singular = singular_extension<me>(root ? 0 : ext,root ? 0 : prev_ext,margin_one,margin_two,new_depth,hash_move);
            if (singular) {
                ext = Max(ext, singular + (prev_ext < 1) - (singular >= 2 && prev_ext >= 2));
                if (root) CurrentSI->Singular = singular;
                ex_depth = new_depth;
                ex_value = (singular >= 2 ? margin_two : margin_one) - 1;
            }
        }
        new_depth = depth - 2 + ext;
        do_move<me>(move);
        if (SETTINGS->numThreads > 1) {         // XXX
            evaluate();
            if (Current->att[opp] & King(me)) {
                undo_move<me>(move);
                cnt--;
                goto skip_hash_move;
            }
        }
        value = -pv_search<opp, 0>(-beta, -alpha, new_depth, ExtFlag(ext));
        undo_move<me>(move);
        played++;
        if (value > alpha) {
            if (root) {
                CurrentSI->FailLow = 0;
                INFO->bestMove = move;
                INFO->bestScore = value;
                hash_low(move,value,depth);
                if (depth >= 14)
                    send_pv(depth/2, old_alpha, beta, value);
            }
            alpha = value;
            Current->best = move;
            if (value >= beta) goto cut;
        } else if (root) {
            CurrentSI->FailLow = 1;
            CurrentSI->FailHigh = 0;
            CurrentSI->Singular = 0;
            if (depth >= 14)
                send_pv(depth/2, old_alpha, beta, value);
        }
    }
skip_hash_move:
    Current->gen_flags = 0;
    if (F(Check(me))) {
        Current->stage = stage_search;
        Current->ref[0] = RefM(Current->move).ref[0];
        Current->ref[1] = RefM(Current->move).ref[1];
    } else {
        Current->stage = stage_evasion;
        Current->ref[0] = RefM(Current->move).check_ref[0];
        Current->ref[1] = RefM(Current->move).check_ref[1];
    }
    Current->killer[0] = 0;
    Current->moves[0] = 0;
    if (root) Current->current = RootList + 1;
    else Current->current = Current->moves;

    while (move = get_move<me,root>()) {
        if (move == hash_move) continue;
        if (IsIllegal(me,move)) continue;
        cnt++;
        if (root) {
            memset(Data + 1, 0, 127 * sizeof(GData));
            send_curr_move(move, cnt);
        }
        if (IsRepetition(alpha + 1,move)) continue;
        check = is_check<me>(move);
        if (check) ext = 2;
        else ext = MaxF(pext, extension<1>(move, depth));
        new_depth = depth - 2 + ext;
        if (depth >= 6 && F(move & 0xE000) && F(Square(To(move))) && (T(root) || (move != Current->killer[1] && move != Current->killer[2]) || T(Check(me))) && cnt > 3) {
            int reduction = msb(cnt) - 1;
            if (move == Current->ref[0] || move == Current->ref[1]) reduction = Max(0, reduction - 1);
            if (reduction >= 2 && !(Queen(White) | Queen(Black)) && popcount(NonPawnKingAll) <= 4) reduction += reduction / 2;
            new_depth = Max(3, new_depth - reduction);
        }
make_move:
        do_move<me>(move);
        if (new_depth <= 1) value = -pv_search<opp, 0>(-beta, -alpha, new_depth, ExtFlag(ext));
        else value = -search<opp, 0>(-alpha, new_depth, FlagNeatSearch | ExtFlag(ext));
        if (value > alpha && new_depth > 1) {
            if (root) {
                SetScore(RootList[cnt - 1],1);
                CurrentSI->Early = 0;
                old_best = INFO->bestMove;
                INFO->bestMove = move;
            }
            new_depth = depth - 2 + ext;
            value = -pv_search<opp, 0>(-beta, -alpha, new_depth, ExtFlag(ext));
            if (T(root) && value <= alpha) INFO->bestMove = old_best;
        }
        undo_move<me>(move);
        played++;
        if (value > alpha) {
            if (root) {
                SetScore(RootList[cnt - 1],cnt + 3);
                CurrentSI->Change = 1;
                CurrentSI->FailLow = 0;
                INFO->bestMove = move;
                INFO->bestScore = value;
                if (depth >= 14)
                    send_pv(depth/2, old_alpha, beta, value);
            }
            alpha = value;
            Current->best = move;
            if (value >= beta) goto cut;
        }
    }
    if (F(cnt) && F(Check(me))) {
        hash_high(0, 127);
        hash_low(0, 0, 127);
        hash_exact(0, 0, 127, 0, 0, 0);
        return 0;
    }
    if (F(root) || F(SearchMoves)) hash_high(alpha, depth);
    if (alpha > old_alpha) {
        hash_low(Current->best,alpha,depth); 
        if (Current->best != hash_move) ex_depth = 0;
        if (F(root) || F(SearchMoves)) hash_exact(Current->best,alpha,depth,ex_value,ex_depth,Convert(INFO->nodes >> 10,int) - start_knodes); 
    }
    return alpha;
cut:
    hash_low(move, alpha, depth);
    return alpha;
}

template <bool me> int multipv(int depth) {
    fprintf(stderr, "NYI...\n");
    abort();
#if 0
    int move, low = MateValue, value, i, cnt, ext, new_depth = depth;
    fprintf(stdout,"info depth %d\n",(depth/2)); fflush(stdout);
    for (cnt = 0; cnt < PVN && T(move = (MultiPV[cnt] & 0xFFFF)); cnt++) {
        MultiPV[cnt] = move;
        move_to_string(move,score_string);
        if (T(Print)) sprintf(info_string,"info currmove %s currmovenumber %d\n",score_string,cnt + 1);
        new_depth = depth - 2 + (ext = extension<1>(move, depth));
        do_move<me>(move);
        value = -pv_search<opp, 0>(-MateValue,MateValue,new_depth,ExtFlag(ext));
        MultiPV[cnt] |= value << 16;
        if (value < low) low = value;
        undo_move<me>(move);
        for (i = cnt - 1; i >= 0; i--) {
            if ((MultiPV[i] >> 16) < value) {
                MultiPV[i + 1] = MultiPV[i];
                MultiPV[i] = move | (value << 16);
            }
        }
        INFO->bestMove = MultiPV[0] & 0xFFFF;
        Current->score = MultiPV[0] >> 16;
        send_multipv((depth/2), cnt);
    }
    for (;T(move = (MultiPV[cnt] & 0xFFFF)); cnt++) {
        MultiPV[cnt] = move;
        move_to_string(move,score_string);
        if (T(Print)) sprintf(info_string,"info currmove %s currmovenumber %d\n",score_string,cnt + 1);
        new_depth = depth - 2 + (ext = extension<1>(move, depth));
        do_move<me>(move);
        value = -search<opp, 0>(-low, new_depth, FlagNeatSearch | ExtFlag(ext));
        if (value > low) value = -pv_search<opp, 0>(-MateValue,-low,new_depth,ExtFlag(ext));
        MultiPV[cnt] |= value << 16;
        undo_move<me>(move);
        if (value > low) {
            for (i = cnt; i >= PVN; i--) MultiPV[i] = MultiPV[i - 1];
            MultiPV[PVN - 1] = move | (value << 16);
            for (i = PVN - 2; i >= 0; i--) {
                if ((MultiPV[i] >> 16) < value) {
                    MultiPV[i + 1] = MultiPV[i];
                    MultiPV[i] = move | (value << 16);
                }
            }
            INFO->bestMove = MultiPV[0] & 0xFFFF;
            Current->score = MultiPV[0] >> 16;
            low = MultiPV[PVN - 1] >> 16;
            send_multipv((depth/2), cnt);
        }
    }
    return Current->score;
#endif
}

void send_multipv(int depth, int curr_number) {
    fprintf(stderr, "NYI...\n");
    abort();
#if 0
    int i, j, pos, move, score;
    int64_t nps, snodes, tbhits = 0;
    if (F(Print)) return;
    for (j = 0; j < PVN && T(MultiPV[j]); j++) {
        pv_length = 63;
        pvp = 0;
        move = MultiPV[j] & 0xFFFF;
        score = MultiPV[j] >> 16;
        memset(PV,0,64 * sizeof(uint16_t));
        if (Current->turn) do_move<1>(move);
        else do_move<0>(move);
        pick_pv();
        if (Current->turn ^ 1) undo_move<1>(move);
        else undo_move<0>(move);
        for (i = 63; i > 0; i--) PV[i] = PV[i - 1];
        PV[0] = move;
        pos = 0;
        for (i = 0; i < 64 && T(PV[i]); i++) {
            if (pos > 0) { 
                pv_string[pos] = ' '; 
                pos++; 
            }
            move = PV[i];
            pv_string[pos++] = ((move >> 6) & 7) + 'a';
            pv_string[pos++] = ((move >> 9) & 7) + '1';
            pv_string[pos++] = (move & 7) + 'a';
            pv_string[pos++] = ((move >> 3) & 7) + '1';
            if (IsPromotion(move)) {
                if ((move & 0xF000) == FlagPQueen)  pv_string[pos++] = 'q';
                else if ((move & 0xF000) == FlagPRook)   pv_string[pos++] = 'r';
                else if ((move & 0xF000) == FlagPLight || (move & 0xF000) == FlagPDark) pv_string[pos++] = 'b';
                else if ((move & 0xF000) == FlagPKnight) pv_string[pos++] = 'n';
            }
            pv_string[pos] = 0;
        }
        score_string[0] = 'c';
        score_string[1] = 'p';
        if (score > EvalValue) {
            strcpy(score_string,"mate ");
            score = (MateValue - score + 1)/2;
            score_string[6] = 0;
        } else if (score < -EvalValue) {
            strcpy(score_string,"mate ");
            score = -(score + MateValue + 1)/2;
            score_string[6] = 0;
        } else {
            score_string[0] = 'c';
            score_string[1] = 'p';
            score_string[2] = ' ';
            score_string[3] = 0;
        }
        nps = get_time() - StartTime;
        snodes = 0;
        for (int i = 0; i < SETTINGS->numThreads; i++)
        {
            snodes += THREADS[i]->nodes;
            tbhits += THREADS[i]->tbHits;
        }
        if (nps) nps = (snodes * 1000)/nps; 
        fprintf(stdout,"info multipv %d depth %d score %s%d nodes %lld nps %lld tbhits %lld pv %s\n",j + 1,(j <= curr_number ? depth : depth - 1),score_string,score,snodes,nps,tbhits,pv_string);
        fflush(stdout);
    }
#endif
}

int time_to_stop(GSearchInfo * SI, int time, int searching) {
    if (SHARED->ponder) return 0;
    if (time > SHARED->hardTimeLimit) return 1;
    if (searching) return 0;
    if (2 * time > SHARED->hardTimeLimit /*&& F(MoveTime)*/) return 1;
    if (SI->Bad) return 0;
    if (time > SHARED->softTimeLimit) return 1;
    if (T(SI->Change) || T(SI->FailLow)) return 0;
    if (time * 100 > SHARED->softTimeLimit * TimeNoChangeMargin) return 1;
    if (F(SI->Early)) return 0;
    if (time * 100 > SHARED->softTimeLimit * TimeNoPVSCOMargin) return 1;
    if (SI->Singular < 1) return 0;
    if (time * 100 > SHARED->softTimeLimit * TimeSingOneMargin) return 1;
    if (SI->Singular < 2) return 0;
    if (time * 100 > SHARED->softTimeLimit * TimeSingTwoMargin) return 1;
    return 0;
}

static void worker(void)
{
    setjmp(CheckJump);
    while (true)
    {
        Current = Data;
        mutex_lock(&SHARED->mutex);
        assert(INFO->stop);
        while (INFO->stop)
        {
            if (SHARED->state > STOPPED)
            {
                SHARED->state--;
                if (SHARED->state == STOPPED && !SHARED->ponder)
                    send_best_move();
            }
            wait_for_go();
        }

        bool newGame = INFO->newGame;
        INFO->newGame = false;

        mutex_unlock(&SHARED->mutex);

        if (newGame)
            init_search(false);
 
        memcpy(Board, &SHARED->rootBoard, sizeof(GBoard));
        memcpy(Current, &SHARED->rootData, sizeof(GData));
        memcpy(Stack, &SHARED->rootStack, SHARED->rootSp * sizeof(uint64_t));
        sp = SHARED->rootSp;
        Stack[sp] = Current->key;

        if (Current->turn == White)
            root<0>();
        else
            root<1>();
    }
}

template <bool me> void root(void)
{
    int i, depth, value, alpha, beta, delta, start_depth = 2, hash_depth = 0, hash_value, store_time = 0, time_est, ex_depth = 0, ex_value, prev_time = 0, knodes = 0;
    int64_t time;
    GPVEntry * PVEntry;

    evaluate();
    gen_root_moves<me>();
    if (PVN > 1) {
//        memset(MultiPV,0,128 * sizeof(int));
//        for (i = 0; MultiPV[i] = RootList[i]; i++);
        fprintf(stderr, "NYI...\n");
        abort();
    }
    INFO->bestMove = RootList[0];
    if (F(INFO->bestMove))
    {
        mutex_lock(&SHARED->mutex);
        INFO->stop = true;
        mutex_unlock(&SHARED->mutex);
        return;
    }
    if (!RootList[1])
    {
        value = pv_search<me, 1>(-MateValue, MateValue, 4, FlagNeatSearch);
        LastDepth = 128;
        send_pv(6, -MateValue, MateValue, value);
        mutex_lock(&SHARED->mutex);
        INFO->stop = true;
        mutex_unlock(&SHARED->mutex);
        return;
    }

    memset(CurrentSI,0,sizeof(GSearchInfo));
    memset(BaseSI,0,sizeof(GSearchInfo));
    Previous = -MateValue;
    if (PVEntry = probe_pv_hash())
    {
        if (is_legal<me>(PVEntry->move) &&
                PVEntry->move == INFO->bestMove &&
                PVEntry->depth > hash_depth)
        {
            hash_depth = PVEntry->depth;
            hash_value = PVEntry->value;
            ex_depth = PVEntry->ex_depth;
            ex_value = PVEntry->exclusion;
            knodes = PVEntry->knodes;
        }
    }
    if (T(hash_depth) && PVN == 1)
    {
        Previous = INFO->bestScore = hash_value;
        depth = hash_depth;
        if (PVHashing)
        {
            send_pv(depth/2, -MateValue, MateValue, INFO->bestScore);
            start_depth = (depth + 2) & (~1);
        }
        if ((depth >= LastDepth - 8 || T(store_time)) &&
                LastValue >= LastExactValue &&
                hash_value >= LastExactValue &&
                T(LastTime) && T(LastSpeed))
        {
            time = SHARED->softTimeLimit;
            if (ex_depth >= depth - Min(12, depth/2) &&
                    ex_value <= hash_value - ExclSinglePV(depth))
            {
                BaseSI->Early = 1;
                BaseSI->Singular = 1;
                if (ex_value <= hash_value - ExclDoublePV(depth))
                {
                    time = (time * TimeSingTwoMargin)/100;
                    BaseSI->Singular = 2;
                }
                else time = (time * TimeSingOneMargin)/100;
            }
            time_est = Min(LastTime,(knodes << 10)/LastSpeed);
            time_est = Max(time_est, store_time);
set_prev_time:
            LastTime = prev_time = time_est;
#if 0
            // This is broken so disable for now...
            if (prev_time >= time)
            {
                InstCnt++;
                if (time_est <= store_time) InstCnt = 0;
                if (InstCnt > 2)
                {
                    if (T(store_time) && store_time < time_est)
                    {
                        time_est = store_time;
                        goto set_prev_time;
                    }
                    LastSpeed = 0;
                    LastTime = 0;
                    prev_time = 0;
                    goto set_jump;
                }
                if (hash_value > 0 && Current->ply >= 2 &&
                        F(Square(To(INFO->bestMove))) &&
                        F(INFO->bestMove & 0xF000) &&
                        PrevMove ==
                            ((To(INFO->bestMove) << 6) | From(INFO->bestMove)))
                     goto set_jump;
                do_move<me>(INFO->bestMove);
                if (Current->ply >= 100)
                {
                    undo_move<me>(INFO->bestMove);
                    goto set_jump;
                }
                for (i = 4; i <= Current->ply; i+=2)
                {
                    if (Stack[sp-i] == Current->key)
                    {
                        undo_move<me>(INFO->bestMove);
                        goto set_jump;
                    }
                }
                undo_move<me>(INFO->bestMove);
                LastDepth = depth;
                LastTime = prev_time;
                LastValue = LastExactValue = hash_value;
                mutex_lock(&SHARED->mutex);
                INFO->stop = true;
                mutex_unlock(&SHARED->mutex);
                return;
            }
            else
#endif
                goto set_jump;
        }
    }
    LastTime = 0;

set_jump:
    memcpy(SaveBoard,Board,sizeof(GBoard));
    memcpy(SaveData,Data,sizeof(GData));
    save_sp = sp;
    for (depth = start_depth; depth < SHARED->depthLimit && depth < 256;
            depth += 2)
    {
        INFO->depth = depth;
        uint32_t sched =
            Schedule[INFO->id % (sizeof(Schedule) / sizeof(Schedule[0]))];
        unsigned idx = (SHARED->rootDepth + depth / 2) % 24;
        if (((sched >> idx) & 0x1) == 0)
            continue;

        memset(Data + 1, 0, 127 * sizeof(GData));
        CurrentSI->Early = 1;
        CurrentSI->Change = CurrentSI->FailHigh = CurrentSI->FailLow =
            CurrentSI->Singular = 0;
        if (PVN > 1)
            value = multipv<me>(depth);
        else if ((depth/2) < 7 || F(Aspiration))
            LastValue = LastExactValue = value =
                pv_search<me, 1>(-MateValue, MateValue, depth, FlagNeatSearch);
        else
        {
            delta = 8;
            alpha = Previous - delta;
            beta = Previous + delta;
loop:
            if (delta >= 16 * 32)
            {
                LastValue = LastExactValue = value =
                    pv_search<me, 1>(-MateValue, MateValue, depth,
                        FlagNeatSearch);
                goto finish;
            }
            value = pv_search<me, 1>(alpha, beta, depth, FlagNeatSearch);
            if (value <= alpha)
            {
                CurrentSI->FailHigh = 0;
                CurrentSI->FailLow = 1;
                alpha -= delta;
                delta *= 2;
                LastValue = value;
                memcpy(BaseSI,CurrentSI,sizeof(GSearchInfo));
                goto loop;
            }
            else if (value >= beta)
            {
                CurrentSI->FailHigh = 1;
                CurrentSI->FailLow = 0;
                CurrentSI->Early = 1;
                CurrentSI->Change = 0;
                CurrentSI->Singular = Max(CurrentSI->Singular, 1);
                beta += delta;
                delta *= 2;
                LastDepth = depth;
                LastTime = MaxF(prev_time,get_time() - SHARED->startTime);
                LastSpeed = INFO->nodes/Max(LastTime, 1);
                if (depth + 2 < SHARED->depthLimit) depth += 2;
                InstCnt = 0;
                if (INFO->id == 0 && time_to_stop(CurrentSI, LastTime, false))
                {
                    mutex_lock(&SHARED->mutex);
                    stop();
                    mutex_unlock(&SHARED->mutex);
                    goto stop;
                }
                memset(Data + 1, 0, 127 * sizeof(GData));
                LastValue = value;
                memcpy(BaseSI,CurrentSI,sizeof(GSearchInfo));
                goto loop;
            } else
                LastValue = LastExactValue = value;
        }
finish:
        if (value < Previous - 50)
            CurrentSI->Bad = 1;
        else
            CurrentSI->Bad = 0;
        memcpy(BaseSI,CurrentSI,sizeof(GSearchInfo));
        LastDepth = depth;
        LastTime = MaxF(prev_time,get_time() - SHARED->startTime);
        LastSpeed = INFO->nodes/Max(LastTime, 1);
        Previous = value;
        InstCnt = 0;
        if (INFO->id == 0 && time_to_stop(CurrentSI, LastTime, false))
        {
            mutex_lock(&SHARED->mutex);
            stop();
            mutex_unlock(&SHARED->mutex);
            goto stop;
        }
    }

stop:

    Current = Data;
    memcpy(Board,SaveBoard,sizeof(GBoard));
    memcpy(Data,SaveData,sizeof(GData));
    sp = save_sp;
    if (!INFO->stop)
    {
        mutex_lock(&SHARED->mutex);
        INFO->stop = true;
        mutex_unlock(&SHARED->mutex);
    }
}

void send_pv(int depth, int alpha, int beta, int score)
{
    int sel_depth;
    for (sel_depth = 1; sel_depth < 127 && T((Data + sel_depth)->att[0]);
        sel_depth++);
    sel_depth--;
    int move = (INFO->bestMove == 0? RootList[0]: INFO->bestMove);
    mutex_lock(&SHARED->mutex);
    INFO->selDepth = sel_depth;
    INFO->PV[0] = move;
    if (Current->turn) do_move<1>(move); else do_move<0>(move);
    unsigned pvPtr = 1, pvLen = 64;
    pick_pv(pvPtr, pvLen);
    if (Current->turn ^ 1) undo_move<1>(move); else undo_move<0>(move);
    if (INFO->id != 0)
    {
        mutex_unlock(&SHARED->mutex);
        return;
    }

    size_t nodes = 0, tbHits = 0;
    for (unsigned i = 0; i < SETTINGS->numThreads; i++)
    {
        nodes  += THREADS[i]->nodes;
        tbHits += THREADS[i]->tbHits;
    }
    send_pv(THREADS[0]->PV, nodes, tbHits, THREADS[0]->depth,
        THREADS[0]->selDepth, THREADS[0]->bestScore,
        THREADS[0]->bestMove, SHARED->startTime);
    mutex_unlock(&SHARED->mutex);
}

static void send_pv(const int *PV, size_t nodes, size_t tbHits, int depth,
    int selDepth, int bestScore, int bestMove, uint64_t startTime)
{
    const char *scoreType = "mate";
    if (bestScore > EvalValue)
        bestScore = (MateValue - bestScore + 1) / 2;
    else if (bestScore < -EvalValue)
        bestScore = -(bestScore + MateValue + 1) / 2;
    else
        scoreType = "cp";

    uint64_t currTime = get_time();
    uint64_t elapsedTime = currTime - startTime;
    if (elapsedTime == 0)
        elapsedTime = 1;

    size_t nps = (nodes * 1000) / elapsedTime;

    char pvStr[IOSIZE];
    unsigned pvPos = 0;
    for (unsigned i = 0; i < MAX_PV_LEN && PV[i] != 0; i++)
    {
        if (pvPos >= sizeof(pvStr) - 32)
            break;
        int move = PV[i];
        pvStr[pvPos++] = ' ';
        pvStr[pvPos++] = ((move >> 6) & 7) + 'a';
        pvStr[pvPos++] = ((move >> 9) & 7) + '1';
        pvStr[pvPos++] = (move & 7) + 'a';
        pvStr[pvPos++] = ((move >> 3) & 7) + '1';
        if (IsPromotion(move))
        {
            if ((move & 0xF000) == FlagPQueen)
                pvStr[pvPos++] = 'q';
            else if ((move & 0xF000) == FlagPRook)
                pvStr[pvPos++] = 'r';
            else if ((move & 0xF000) == FlagPLight ||
                     (move & 0xF000) == FlagPDark)
                pvStr[pvPos++] = 'b';
            else if ((move & 0xF000) == FlagPKnight)
                pvStr[pvPos++] = 'n';
        }
    }
    pvStr[pvPos++] = '\0';

    char line[IOSIZE];
    int len = snprintf(line, sizeof(line)-1, "info depth %d seldepth %d score "
        "%s %d multipv 1 nodes %" SIZE_T " nps %" SIZE_T " tbhits %" SIZE_T
        " time %ld pv%s\n",
        depth/2, selDepth/2, scoreType, bestScore, nodes, nps, tbHits,
        currTime - startTime, pvStr);
    if (len < 0 || len >= sizeof(line)-1)
        return; 
    put_line(line, len);
}

static void send_curr_move(int move, int cnt)
{
    if (INFO->id != 0)
        return;
    uint64_t currTime = get_time();
    uint64_t diffTime = currTime - SHARED->startTime;
    if (currTime - SHARED->startTime <= InfoLag ||
            currTime - InfoTime <= InfoDelay)
        return;
    InfoTime = currTime;
    char moveStr[16], line[IOSIZE];
    move_to_string(move, moveStr);
    int len = snprintf(line, sizeof(line)-1, 
        "info currmove %s currmovenumber %d\n", moveStr, cnt);
    if (len > 0 && len < sizeof(line)-1)
    {
        mutex_lock(&SHARED->mutex);
        put_line(line, len);
        mutex_unlock(&SHARED->mutex);
    }
}

static void send_best_move(void)
{
    size_t nodes = THREADS[0]->nodes, tbHits = THREADS[0]->tbHits;
    GThreadInfo *bestThread = THREADS[0];

    for (unsigned i = 1; i < SETTINGS->numThreads; i++)
    {
        nodes  += THREADS[i]->nodes;
        tbHits += THREADS[i]->tbHits;
        if (THREADS[i]->depth > bestThread->depth && bestThread->depth == 0)
            bestThread = THREADS[i];
    }
    send_best_move(bestThread->PV, nodes, tbHits, bestThread->bestScore,
        bestThread->bestMove, SHARED->startTime);
}

static void send_best_move(const int *PV, size_t nodes, size_t tbHits,
    int bestScore, int bestMove, uint64_t startTime)
{
    uint64_t stopTime = get_time();

    if (nodes == 0)
        nodes = 1;
    const char *scoreType = "mate";
    if (bestScore > EvalValue)
        bestScore = (MateValue - bestScore + 1) / 2;
    else if (bestScore < -EvalValue)
        bestScore = -(bestScore + MateValue + 1) / 2;
    else
        scoreType = "cp";

    char line[IOSIZE];
    uint64_t time = stopTime - startTime;
    uint64_t nps = (nodes / Max(time / 1000, 1));
    int len = snprintf(line, sizeof(line)-1, "info nodes %" SIZE_T
        " tbhits %" SIZE_T " time %ld nps %ld score %s %d\n", nodes, tbHits,
        time, nps, scoreType, bestScore);
    if (len > 0 && len < sizeof(line)-1)
        put_line(line, len);

    char moveStr[16], ponderStr[16];
    int ponder = (PV[0] == 0? 0: PV[1]);
    move_to_string(bestMove, moveStr);
    if (ponder != 0)
    {
        move_to_string(ponder, ponderStr);
        len = snprintf(line, sizeof(line)-1, "bestmove %s ponder %s\n",
            moveStr, ponderStr);
    }
    else
        len = snprintf(line, sizeof(line)-1, "bestmove %s\n", moveStr);
    if (len > 0 && len < sizeof(line)-1)
        put_line(line, len);
    else
    {
        // Giveup:
        char reply[] = "bestmove 0\n";
        put_line(reply, sizeof(reply));
    }
}

static inline int64_t get_number(const char *token)
{
    if (token == NULL)
        return 0;
    return (int64_t)atoll(token);
}

void uci(void)
{
    char line[4 * IOSIZE];
    uint64_t currTime = get_time();

    mutex_lock(&SHARED->mutex);
    while (true)
    {
        while (true)
        {
            uint64_t timeout = UINT64_MAX;
            if (SHARED->state != STOPPED)
                timeout = 50;           // 50ms
            mutex_unlock(&SHARED->mutex);

            bool timedout = get_line(line, sizeof(line)-1, timeout);
            currTime = get_time();

            if (mutex_lock(&SHARED->mutex, 500))
                emergency_stop();
            if (SHARED->state != STOPPED &&
                    currTime >= SHARED->startTime + SHARED->hardTimeLimit / 2)
            {
                stop();
                wait_for_stop();
            }
            if (!timedout)
                break;
        }
   
        if (line[0] == EOF)
        {
            nuke_children();
            exit(EXIT_SUCCESS);
        }
        char *saveptr = NULL;
        char *token = strtok_r(line, " ", &saveptr);
        if (token == NULL)
            /* NOP */ ;
        else if (strcmp(token, "go") == 0)
        {
            if (SHARED->state != STOPPED)
                continue;
            unsigned binc = 0, btime = 0, depth = 256, movestogo = 0,
                winc = 0, wtime = 0, movetime = 0;
            bool infinite = false, ponder = false;
            while ((token = strtok_r(NULL, " ", &saveptr)) != NULL)
            {
                if (strcmp(token, "binc") == 0)
                    binc = get_number(strtok_r(NULL, " ", &saveptr));
                else if (strcmp(token, "btime") == 0)
                    btime = get_number(strtok_r(NULL, " ", &saveptr));
                else if (strcmp(token, "winc") == 0)
                    winc = get_number(strtok_r(NULL, " ", &saveptr));
                else if (strcmp(token, "wtime") == 0)
                    wtime = get_number(strtok_r(NULL, " ", &saveptr));
                else if (strcmp(token, "movetime") == 0)
                    movetime = get_number(strtok_r(NULL, " ", &saveptr));
                else if (strcmp(token, "movetogo") == 0)
                    movestogo = get_number(strtok_r(NULL, " ", &saveptr));
                else if (strcmp(token, "depth") == 0)
                    depth = get_number(strtok_r(NULL, " ", &saveptr));
                else if (strcmp(token, "infinite") == 0)
                    infinite = true;
                else if (strcmp(token, "ponder") == 0)
                    ponder = true;
            }
            unsigned time = (Current->turn == White? wtime: btime);
            unsigned inc  = (Current->turn == White? winc: binc);
            if (movetime == 0 && time == 0 && inc == 0)
                infinite = true;
            if (movestogo != 0)
                movestogo = Max(movestogo-1, 1);
            int time_max = Max(time - Min(1000, time/2), 0);
            int nmoves;
            int exp_moves = MovesTg - 1;
            if (movestogo != 0)
                nmoves = movestogo;
            else
            {
                nmoves = MovesTg - 1;
                if (Current->ply > 40)
                    nmoves += Min(Current->ply - 40, (100 - Current->ply)/2);
                exp_moves = nmoves;
            }
            uint64_t softTimeLimit = 0, hardTimeLimit = 0;
            softTimeLimit = Min(time_max,
                (time_max + (Min(exp_moves, nmoves) * inc)) /
                    Min(exp_moves, nmoves));
            hardTimeLimit = Min(time_max,
                (time_max + (Min(exp_moves, nmoves) * inc)) /
                    Min(3, Min(exp_moves, nmoves)));
            softTimeLimit = Min(time_max, (softTimeLimit * TimeRatio)/100);
            if (movetime != 0)
            {
                hardTimeLimit = movetime;
                softTimeLimit = UINT32_MAX;
            }
            else if (infinite)
                hardTimeLimit = softTimeLimit = UINT32_MAX;
            SHARED->ponder = ponder;
            SHARED->softTimeLimit = softTimeLimit;
            SHARED->hardTimeLimit = hardTimeLimit;
            SHARED->depthLimit = 2 * depth + 2;
            SHARED->startTime = currTime;
            go();
        }
        else if (strcmp(token, "isready") == 0)
        {
            char reply[] = "readyok\n";
            put_line(reply, sizeof(reply)-1);
        }
        else if (strcmp(token, "stop") == 0)
        {
            if (SHARED->state == STOPPED)
                continue;
            stop();
            wait_for_stop();
        }
        else if (strcmp(token, "ponderhit") == 0)
        {
            SHARED->ponder = false;
            if (SHARED->state == STOPPED)
                send_best_move();
        }
        else if (strcmp(token, "position") == 0)
        {
            token = strtok_r(NULL, " ", &saveptr);
            if (token == NULL)
                goto bad_command;
            char *moves;
            if (strcmp(token, "fen") == 0)
            {
                char *fen = token + strlen(token) + 1;
                moves = (char *)get_board(fen);
            }
            else if (strcmp(token, "startpos") == 0)
            {
                moves = saveptr;
                get_board(
                    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            }
            else
                goto bad_command;
            token = strtok_r(moves, " ", &saveptr);
            if (token != NULL && strcmp(token, "moves") == 0)
            {
                while ((token = strtok_r(NULL, " ", &saveptr)) != NULL)
                {
                    int move = move_from_string(token);
                    if (Current->turn) do_move<1>(move); else do_move<0>(move);
                    memcpy(Data, Current, sizeof(GData));
                    Current = Data;
                    SHARED->rootDepth++;
                }
            }
            memcpy(Stack, Stack + sp - Current->ply,
                (Current->ply + 1) * sizeof(uint64_t));
            sp = Current->ply;
        }
        else if (strcmp(token, "setoption") == 0)
        {
            token = strtok_r(NULL, " ", &saveptr);
            if (token == NULL || strcmp(token, "name") != 0)
                goto bad_command;
            const char *name = strtok_r(NULL, " ", &saveptr);
            if (name == NULL)
                goto bad_command;
            token = strtok_r(NULL, " ", &saveptr);
            if (token == NULL || strcmp(token, "value") != 0)
                goto bad_command;
            if (token == NULL)
                goto bad_command;
            unsigned numThreads = SETTINGS->numThreads;
            unsigned syzygyProbeDepth = SETTINGS->syzygyProbeDepth;
            size_t hashSize = SETTINGS->hashSize;
            int64_t n = 0;
            if (strcmp(name, "Hash") == 0)
            {
                uint64_t n = get_number(strtok_r(NULL, " ", &saveptr));
                hashSize = HASH_SIZE(n);
            }
            else if (strcmp(name, "Threads") == 0)
                numThreads = get_number(strtok_r(NULL, " ", &saveptr));
            else if (strcmp(name, "SyzygyPath") == 0)
            {
                if (saveptr != NULL && strlen(saveptr) < sizeof(SyzygyPath)-1)
                   memcpy(SyzygyPath, saveptr, strlen(saveptr)+1);
            }
            else if (strcmp(name, "SyzygyProbeDepth") == 0)
                syzygyProbeDepth = get_number(strtok_r(NULL, " ", &saveptr));
            else
                goto bad_command;
            reset(numThreads, syzygyProbeDepth, hashSize, SyzygyPath);
            mutex_lock(&SHARED->mutex);
        }
        else if (strcmp(token, "ucinewgame") == 0)
        {
            if (SHARED->state != STOPPED)
                continue;
            init_search(true);
            for (unsigned i = 0; i < SETTINGS->numThreads; i++)
                THREADS[i]->newGame = true;
        }
        else if (strcmp(token, "uci") == 0)
        {
            char reply[] =
                "id name LazyGull (" STRING(VERSION) ")\n"
                "id author ThinkingALot\n"
                "option name Hash type spin min 1 max 8388608 default 128\n"
                "option name Threads type spin min 1 max 64 default 4\n"
                "option name SyzygyPath type string default <empty>\n"
                "option name SyzygyProbeDepth type spin min 0 max 64 "
                    "default 1\n"
                "uciok\n";
            put_line(reply, sizeof(reply)-1);
        }
        else if (strcmp(token, "quit") == 0)
        {
            nuke_children();
            exit(EXIT_SUCCESS);
        }
        else
        {
bad_command:
            log("warning: unknown command\n");
        }
    }
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "child") == 0)
    {
        if (argc != 10)
        {
            usage:
            fprintf(stderr, "usage: %s\n", argv[0]);
            fprintf(stderr, "       %s \"bench\" <depth> FEN1 [FEN2 ...]\n",
                argv[0]);
            exit(EXIT_FAILURE);
        }

        // CHILD:
        const char
            *hashStr = argv[2],
            *pvHashStr = argv[3],
            *pawnHashStr = argv[4],
            *dataStr = argv[5],
            *settingsStr = argv[6],
            *sharedStr = argv[7],
            *infoStr = argv[8],
            *tbPath = argv[9];
        init_object(dataStr, sizeof(GGlobalData), DATA, false, true, true,
            NULL);
        init_object(settingsStr, sizeof(GSettings), SETTINGS, false,
            true, true, NULL);
        init_object(sharedStr, sizeof(GSharedInfo), SHARED, false, false,
            true, NULL);
        init_object(hashStr, SETTINGS->hashSize, HASH, false, false, true,
            NULL);
        init_object(pvHashStr, pvHashSize, PVHASH, false, false, true,
            NULL);
        init_object(pawnHashStr, pawnHashSize, PAWNHASH, false, false,
            true, NULL);
        init_object(infoStr, sizeof(GThreadInfo), INFO, false, false, true,
            NULL);
        INFO->pid = get_pid();
        tb_init(tbPath);
        for (unsigned i = 0; i < SETTINGS->numThreads; i++)
        {
            if (i == INFO->id)
            {
                THREADS[i] = INFO;
                continue;
            }
            char infoName[256] = {0};
            init_object_name(infoName, sizeof(infoName)-1, "INFO",
                SETTINGS->parentPid, i);
            THREADS[i] = (GThreadInfo *)init_object(infoName,
                sizeof(GThreadInfo), NULL, false, false, true, NULL);
        }
        init_search(false);

        // Signal that we are initialized.
        mutex_lock(&SHARED->mutex);
        SHARED->init--;
        if (SHARED->init == 0)
        {
#ifdef LINUX
            cond_signal(&SHARED->initCondVar);
#endif
#ifdef WINDOWS
            event_signal(&SHARED->initEvent);
#endif
        }
        mutex_unlock(&SHARED->mutex);

        worker();   // Wait for work from parent.
    }
    else if (argc > 2 && strcmp(argv[1], "bench") == 0)
    {
        const int benchDepth = atoi(argv[2]);
        init_object(NULL, sizeof(GGlobalData), DATA, true, false, true, NULL);
        init_data();
        GSettings settings;
        memset(&settings, 0, sizeof(settings));
        settings.numThreads = 1;
        settings.hashSize   = 8 * (1 << 20);        // 8MB
        init_object(NULL, sizeof(GSettings), SETTINGS, true, true, true,
            &settings);
        init_object(NULL, sizeof(GSharedInfo), SHARED, true, false, true,
            NULL);
        mutex_init(&SHARED->mutex);
#ifdef LINUX
        cond_init(&SHARED->goCondVar);
#endif
        init_object(NULL, SETTINGS->hashSize, HASH, true, false, true, NULL);
        init_object(NULL, pvHashSize, PVHASH, true, false, true, NULL);
        init_object(NULL, pawnHashSize, PAWNHASH, true, false, true, NULL);
        init_object(NULL, sizeof(GThreadInfo), INFO, true, false, true, NULL);
        INFO->pid = get_pid();
        THREADS[0] = INFO;

        uint64_t t0 = get_time(), nodes = 0;
        for (int i = 3; i < argc; i++)
        {
            init_search(true);
            get_board(argv[i]);
            INFO->stop = false;
            SHARED->depthLimit = 2 * benchDepth + 2;
            SHARED->softTimeLimit = UINT32_MAX;
            SHARED->hardTimeLimit = UINT32_MAX;
            SHARED->startTime = t0;
            if (Current->turn == White) root<0>(); else root<1>();
            send_best_move();
            nodes += INFO->nodes;
        }
        uint64_t t1 = get_time();
        printf("TIME : %ldms\n", t1 - t0);
        printf("NODES: %lu\n", nodes);
        exit(EXIT_SUCCESS);
    }
    else if (argc > 1)
        goto usage;

    // PARENT:
    printf("LazyGull (" STRING(VERSION) ")\n");
    init_os();

    // Read override parameters from the environment (useful for debugging)
    unsigned numThreads = get_num_cpus();
    size_t hashSize = 128 * (1 << 20);     // 128MB
    size_t syzygyProbeDepth = 1;
    SyzygyPath[0] = '\0';
    const char *val;
    if ((val = getenv("LAZYGULL_HASH")) != NULL)
        hashSize = HASH_SIZE((size_t)atoi(val));
    if ((val = getenv("LAZYGULL_THREADS")) != NULL)
        numThreads = atoi(val);
    if ((val = getenv("LAZYGULL_SYZYGY_PATH")) != NULL)
        strncpy(SyzygyPath, val, sizeof(SyzygyPath)-1);
    if ((val = getenv("LAZYGULL_SYZYGY_PROBE_DEPTH")) != NULL)
        syzygyProbeDepth = atoi(val);

    PVN = 1;        // XXX NYI

    create_children(numThreads, syzygyProbeDepth, hashSize, SyzygyPath);
    init_search(false);

    while (true)
        uci();
    return 0;
}

/*
 * Create the child processes.
 */
static void create_children(size_t numThreads, size_t syzygyProbeDepth,
    size_t hashSize, const char *tbPath)
{
    const size_t maxHashSize = 8 * ((size_t)1 << 40);   // 8TB
    const size_t maxNumThreads = 256;
    const size_t maxSyzygyProbeDepth = 64;
    hashSize = Min(maxHashSize, hashSize);
    numThreads = Min(maxNumThreads, numThreads);
    syzygyProbeDepth = Min(maxSyzygyProbeDepth, syzygyProbeDepth);
    unsigned pid = get_pid();

    log("settings: numThreads=%" SIZE_T ", hashSize=%" SIZE_T ", "
            "syzygyProbeDepth=%" SIZE_T ", syzygyPath=\"%s\"\n",
            numThreads, hashSize, syzygyProbeDepth, tbPath);

    // Create shared objects:
    char dataName[256] = {0};
    init_object_name(dataName, sizeof(dataName)-1, "DATA", pid, 0);
    init_object(dataName, sizeof(GGlobalData), DATA, true, false, true, NULL);
    init_data();

    char settingsName[256] = {0};
    init_object_name(settingsName, sizeof(settingsName)-1, "SETTINGS", pid, 0);
    GSettings settings;
    settings.numThreads = numThreads;
    settings.syzygyProbeDepth = syzygyProbeDepth;
    settings.hashSize = hashSize;
    settings.parentPid = pid;
    init_object(settingsName, sizeof(settings), SETTINGS, true, true, true,
        &settings);

    char hashName[256] = {0};
    init_object_name(hashName, sizeof(hashName)-1, "HASH", pid, 0);
    init_object(hashName, hashSize, HASH, true, false, true, NULL);

    char pvHashName[256] = {0};
    init_object_name(pvHashName, sizeof(pvHashName)-1, "PVHASH", pid, 0);
    init_object(pvHashName, pvHashSize, PVHASH, true, false, true, NULL);

    char pawnHashName[256] = {0};
    init_object_name(pawnHashName, sizeof(pawnHashName)-1, "PAWNHASH", pid, 0);
    init_object(pawnHashName, pawnHashSize, PAWNHASH, true, false, true,
        NULL);

    char sharedName[256] = {0};
    init_object_name(sharedName, sizeof(sharedName)-1, "SHARED", pid, 0);
    init_object(sharedName, sizeof(GSharedInfo), SHARED, true, false, true,
        NULL);
    SHARED->init = numThreads;
    mutex_init(&SHARED->mutex);
#ifdef LINUX
    cond_init(&SHARED->goCondVar);
    cond_init(&SHARED->initCondVar);
#endif
#ifdef WINDOWS
    event_init(&SHARED->initEvent);
#endif

    // Create children: 
    for (size_t i = 0; i < numThreads; i++)
    {
        char infoName[256] = {0};
        init_object_name(infoName, sizeof(infoName)-1, "INFO", pid, i);
        THREADS[i] = (GThreadInfo *)init_object(infoName, sizeof(GThreadInfo),
            NULL, true, false, true, NULL);
        THREADS[i]->stop = true;
        THREADS[i]->newGame = true;
        THREADS[i]->id = (unsigned)i;
#ifdef WINDOWS
        event_init(&THREADS[i]->goEvent);
#endif
    }
    for (size_t i = 0; i < numThreads; i++)
    {
        char infoName[256] = {0};
        init_object_name(infoName, sizeof(infoName)-1, "INFO", pid, i);
        create_child(hashName, pvHashName, pawnHashName, dataName,
            settingsName, sharedName, infoName, tbPath);
    }

    init_object(NULL, sizeof(GThreadInfo), INFO, true, false, true, NULL);
    tb_init(SyzygyPath);

    // Wait for threads to finish initializing:
    mutex_lock(&SHARED->mutex);
    while (SHARED->init != 0)
    {
#ifdef LINUX
        cond_wait(&SHARED->initCondVar, &SHARED->mutex);
#endif
#ifdef WINDOWS
        event_wait(&SHARED->initEvent, &SHARED->mutex);
        mutex_lock(&SHARED->mutex);
#endif
    }
    mutex_unlock(&SHARED->mutex);

    // Cleanup:
    remove_object(dataName);
    remove_object(settingsName);
    remove_object(hashName);
    remove_object(pvHashName);
    remove_object(pawnHashName);
    remove_object(sharedName);
    for (size_t i = 0; i < numThreads; i++)
    {
        char infoName[256] = {0};
        init_object_name(infoName, sizeof(infoName)-1, "INFO", pid, i);
        remove_object(infoName);
    }
}

/*
 * Kill all children ungracefully.
 */
static void nuke_children(void)
{
    for (size_t i = 0; i < SETTINGS->numThreads; i++)
        nuke_child(THREADS[i]->pid);
    delete_object(HASH, SETTINGS->hashSize);
    delete_object(PVHASH, pvHashSize);
    delete_object(PAWNHASH, pawnHashSize);
    mutex_free(&SHARED->mutex);
#ifdef LINUX
    cond_free(&SHARED->goCondVar);
    cond_free(&SHARED->initCondVar);
#endif
    for (size_t i = 0; i < SETTINGS->numThreads; i++)
    {
#ifdef WINDOWS
        event_free(&THREADS[i]->goEvent);
#endif
        delete_object(THREADS[i], sizeof(GThreadInfo));
    }
    delete_object(DATA, sizeof(GGlobalData));
    delete_object(SETTINGS, sizeof(GSettings));
    delete_object(SHARED, sizeof(GSharedInfo));
    delete_object(INFO, sizeof(GThreadInfo));
}

/*
 * Reset (e.g. change of parameters).
 */
static void reset(size_t numThreads, size_t syzygyProbeDepth, size_t hashSize,
    const char *tbPath)
{
    nuke_children();
    create_children(numThreads, syzygyProbeDepth, hashSize, tbPath);
}

