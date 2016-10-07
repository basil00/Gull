#ifndef __DATA_H
#define __DATA_H

#include <stdint.h>

typedef struct
{
    int16_t score;
    uint8_t phase;
    uint8_t flags;
    uint8_t mul[2];
    uint8_t pieces[2];
} GMaterial;
#define MatWQ 1
#define MatBQ 3
#define MatWR (3 * 3)
#define MatBR (3 * 3 * 3)
#define MatWL (3 * 3 * 3 * 3)
#define MatBL (3 * 3 * 3 * 3 * 2)
#define MatWD (3 * 3 * 3 * 3 * 2 * 2)
#define MatBD (3 * 3 * 3 * 3 * 2 * 2 * 2)
#define MatWN (3 * 3 * 3 * 3 * 2 * 2 * 2 * 2)
#define MatBN (3 * 3 * 3 * 3 * 2 * 2 * 2 * 2 * 3)
#define MatWP (3 * 3 * 3 * 3 * 2 * 2 * 2 * 2 * 3 * 3)
#define MatBP (3 * 3 * 3 * 3 * 2 * 2 * 2 * 2 * 3 * 3 * 9)
#define TotalMat ((2*(MatWQ+MatBQ)+MatWL+MatBL+MatWD+MatBD+2*(MatWR+MatBR+MatWN+MatBN)+8*(MatWP+MatBP)) + 1)

#define MagicSize   107648

#ifdef GULL_MAGIC_BITBOARDS
#define BishopAttacks(sq, occ)                                          \
    (*(DATA->BOffsetPointer[sq] +                                       \
        (((DATA->BMagicMask[sq] & (occ)) *                              \
            DATA->BMagic[sq]) >> DATA->BShift[sq])))
#define RookAttacks(sq, occ)                                            \
    (*(DATA->ROffsetPointer[sq] +                                       \
        (((DATA->RMagicMask[sq] & (occ)) *                              \
            DATA->RMagic[sq]) >> DATA->RShift[sq])))
#endif  /* GULL_MAGIC_BITBOARDS */

#ifdef GULL_PDEP_BITBOARDS
typedef struct
{
    const uint16_t *data;
    uint64_t premask;
} GAttackInfo;

#include <x86intrin.h>

#define pdep(x, mask)           _pdep_u64((x), (mask))
#define pext(x, mask)           _pext_u64((x), (mask))

#define BishopAttacks(sq, occ)                                          \
    pdep(DATA->BAttacks[sq].data[pext(occ, DATA->BAttacks[sq].premask)],\
        DATA->BMask[sq])
#define RookAttacks(sq, occ)                                            \
    pdep(DATA->RAttacks[sq].data[pext(occ, DATA->RAttacks[sq].premask)],\
         DATA->RMask[sq])

#endif  /* GULL_PDEP_BITBOARDS */

typedef struct
{
    uint64_t Forward[2][8];
    uint64_t West[8];
    uint64_t East[8];
    uint64_t PIsolated[8];
    uint64_t HLine[64];
    uint64_t VLine[64];
    uint64_t NDiag[64];
    uint64_t SDiag[64];
    uint64_t RMask[64];
    uint64_t BMask[64];
    uint64_t QMask[64];
    uint64_t NAtt[64];
    uint64_t SArea[64];
    uint64_t DArea[64];
    uint64_t NArea[64];
    uint64_t BishopForward[2][64];
    uint64_t PAtt[2][64];
    uint64_t PMove[2][64];
    uint64_t PWay[2][64];
    uint64_t PSupport[2][64];
    uint64_t Between[64][64];
    uint64_t FullLine[64][64];
    uint64_t File[8];
    uint64_t Line[8];
    uint8_t UpdateCastling[64];
    int32_t Mobility[4][32];
    int32_t Shelter[3][8];
    int32_t StormHof[2];
    int32_t StormBlocked[4];
    int32_t StormShelterAtt[4];
    int32_t StormConnected[4];
    int32_t StormOpen[4];
    int32_t StormFree[4];
    int32_t LogDist[16];
    int32_t PasserSpecial[4];
    int32_t PasserGeneral[8];
    int32_t PasserBlocked[8];
    int32_t PasserFree[8];
    int32_t PasserSupported[8];
    int32_t PasserProtected[8];
    int32_t PasserConnected[8];
    int32_t PasserOutside[8];
    int32_t PasserCandidate[8];
    int32_t PasserClear[8];
    int32_t PasserAtt[8];
    int32_t PasserDef[8];
    int32_t PasserAttLog[8];
    int32_t PasserDefLog[8];
    int32_t Isolated[10];
    int32_t Unprotected[6];
    int32_t Backward[4];
    int32_t Doubled[4];
    int32_t RookSpecial[20];
    int32_t Tactical[12];
    int32_t KingDefence[8];
    int32_t PawnSpecial[8];
    int32_t BishopSpecial[4];
    uint64_t Outpost[2];
    int32_t KnightSpecial[10];
    int32_t Pin[10];
    int32_t KingRay[6];
    int32_t KingAttackWeight[7];
    int32_t KingAttackScale[16];
    uint64_t TurnKey;
    uint64_t PieceKey[16][64];
    uint64_t CastleKey[16];
    uint64_t EPKey[8];
    uint8_t PieceFromChar[256];
    uint64_t Kpk[2][64][64];
    int32_t Pst[16 * 64];
    int32_t MvvLva[16][16];
    GMaterial Material[TotalMat];
#ifdef GULL_MAGIC_BITBOARDS
    uint64_t BMagic[64];
    uint64_t RMagic[64];
    uint32_t BShift[64];
    uint32_t RShift[64];
    uint64_t BMagicMask[64];
    uint64_t RMagicMask[64];
    uint64_t *BOffsetPointer[64];
    uint64_t *ROffsetPointer[64];
    uint64_t MagicAttacks[MagicSize];
#endif
#ifdef GULL_PDEP_BITBOARDS
    GAttackInfo BAttacks[64];
    GAttackInfo RAttacks[64];
    uint16_t MagicAttacks[MagicSize];
#endif
} GGlobalData;

#endif
