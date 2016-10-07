#include <stdint.h>

uint64_t BMagicAttacks(int i, uint64_t occ)
{
    uint64_t att = 0;
    for (uint64_t u = DATA->BMask[i]; T(u); Cut(u))
        if (F(DATA->Between[i][lsb(u)] & occ))
            att |= DATA->Between[i][lsb(u)] | Bit(lsb(u));
    return att;
}

uint64_t RMagicAttacks(int i, uint64_t occ)
{
    uint64_t att = 0;
    for (uint64_t u = DATA->RMask[i]; T(u); Cut(u))
        if (F(DATA->Between[i][lsb(u)] & occ))
            att |= DATA->Between[i][lsb(u)] | Bit(lsb(u));
    return att;
}

#ifdef GULL_MAGIC_BITBOARDS
static void init_magic(void)
{
    int i, j, k, index, bits, bit_list[16];
    uint64_t u;
    
    for (i = 0; i < 64; i++)
    {
        bits = 64 - BShift[i];
        for (u = DATA->BMagicMask[i], j = 0; T(u); Cut(u), j++)
            bit_list[j] = lsb(u);
        for (j = 0; j < Bit(bits); j++)
        {
            u = 0;
            for (k = 0; k < bits; k++)
                if (Odd(j >> k)) Add(u,bit_list[k]);
            index = Convert(BOffset[i] + ((BMagic[i] * u) >> BShift[i]),int);
            DATA->MagicAttacks[index] = BMagicAttacks(i, u);
        }
        bits = 64 - RShift[i];
        for (u = DATA->RMagicMask[i], j = 0; T(u); Cut(u), j++)
            bit_list[j] = lsb(u);
        for (j = 0; j < Bit(bits); j++)
        {
            u = 0;
            for (k = 0; k < bits; k++)
                if (Odd(j >> k)) Add(u,bit_list[k]);
            index = Convert(ROffset[i] + ((RMagic[i] * u) >> RShift[i]),int);
            DATA->MagicAttacks[index] = RMagicAttacks(i, u);
        }
    }
    for (int i = 0; i < 64; i++)
    {
        DATA->BOffsetPointer[i] = DATA->MagicAttacks + BOffset[i];
        DATA->ROffsetPointer[i] = DATA->MagicAttacks + ROffset[i];
    }
    memcpy(DATA->BMagic, BMagic, sizeof(DATA->BMagic));
    memcpy(DATA->RMagic, RMagic, sizeof(DATA->RMagic));
    memcpy(DATA->BShift, BShift, sizeof(DATA->BShift));
    memcpy(DATA->RShift, RShift, sizeof(DATA->RShift));
}
#endif

#ifdef GULL_PDEP_BITBOARDS
static void init_magic(void)
{
    unsigned offset = 0;
    for (unsigned i = 0; i < 64; i++)
    {
        GAttackInfo *info = DATA->BAttacks + i;
        info->data = DATA->MagicAttacks + offset;
        uint64_t postmask = DATA->BMask[i];
        uint64_t premask = postmask & Interior;
        info->premask = premask;
        unsigned bits = (1 << popcount(premask));
        for (unsigned j = 0; j < bits; j++)
        {
            uint64_t occ = pdep(j, premask);
            uint64_t att = BMagicAttacks(i, occ);
            DATA->MagicAttacks[offset++] = (uint16_t)pext(att, postmask);
        }
    }
    for (unsigned i = 0; i < 64; i++)
    {
        GAttackInfo *info = DATA->RAttacks + i;
        info->data = DATA->MagicAttacks + offset;
        uint64_t postmask = DATA->RMask[i];
        uint64_t premask = postmask;
        if (File(i) > 0) premask &= ~DATA->File[0];
        if (Rank(i) > 0) premask &= ~DATA->Line[0];
        if (File(i) < 7) premask &= ~DATA->File[7];
        if (Rank(i) < 7) premask &= ~DATA->Line[7];
        info->premask = premask;
        unsigned bits = (1 << popcount(premask));
        for (unsigned j = 0; j < bits; j++)
        {
            uint64_t occ = pdep(j, premask);
            uint64_t att = RMagicAttacks(i, occ);
            DATA->MagicAttacks[offset++] = (uint16_t)pext(att, postmask);
        }
    }
}
#endif

static void init_misc(void)
{
    int i, j, k, l, n;
    uint64_t u;

    for (i = 0; i < 8; i++)
    {
        DATA->File[i] = FileA << i;
        DATA->Line[i] = Line0 << (8 * i);
    }
    for (i = 0; i < 64; i++) for (j = 0; j < 64; j++) if (i != j)
    {
        u = Bit(j);
        if (File(i) == File(j)) DATA->VLine[i] |= u;
        if (Rank(i) == Rank(j)) DATA->HLine[i] |= u;
        if (NDiag(i) == NDiag(j)) DATA->NDiag[i] |= u;
        if (SDiag(i) == SDiag(j)) DATA->SDiag[i] |= u;
        if (Dist(i,j) <= 2)
        {
            DATA->DArea[i] |= u;
            if (Dist(i,j) <= 1) DATA->SArea[i] |= u;
            if (Abs(Rank(i)-Rank(j)) + Abs(File(i)-File(j)) == 3)
                DATA->NAtt[i] |= u;
        }
        if (j == i + 8) DATA->PMove[0][i] |= u;
        if (j == i - 8) DATA->PMove[1][i] |= u;
        if (Abs(File(i) - File(j)) == 1)
        {
            if (Rank(j) >= Rank(i))
            {
                DATA->PSupport[1][i] |= u;
                if (Rank(j) - Rank(i) == 1) DATA->PAtt[0][i] |= u;
            }
            if (Rank(j) <= Rank(i))
            {
                DATA->PSupport[0][i] |= u;
                if (Rank(i) - Rank(j) == 1) DATA->PAtt[1][i] |= u;
            }
        } else if (File(i) == File(j))
        {
            if (Rank(j) > Rank(i)) DATA->PWay[0][i] |= u;
            else DATA->PWay[1][i] |= u;
        }
    }
    for (i = 0; i < 64; i++)
    {
        DATA->RMask[i] = DATA->HLine[i] | DATA->VLine[i];
        DATA->BMask[i] = DATA->NDiag[i] | DATA->SDiag[i];
        DATA->QMask[i] = DATA->RMask[i] | DATA->BMask[i];
#ifdef GULL_MAGIC_BITBOARDS
        DATA->BMagicMask[i] = DATA->BMask[i] & Interior;
        DATA->RMagicMask[i] = DATA->RMask[i];
        if (File(i) > 0) DATA->RMagicMask[i] &= ~DATA->File[0];
        if (Rank(i) > 0) DATA->RMagicMask[i] &= ~DATA->Line[0];
        if (File(i) < 7) DATA->RMagicMask[i] &= ~DATA->File[7];
        if (Rank(i) < 7) DATA->RMagicMask[i] &= ~DATA->Line[7];
#endif
        for (j = 0; j < 64; j++) if (DATA->NAtt[i] & DATA->NAtt[j])
            Add(DATA->NArea[i], j);
    }
    for (i = 0; i < 8; i++)
    {
        DATA->West[i] = 0;
        DATA->East[i] = 0;
        DATA->Forward[0][i] = DATA->Forward[1][i] = 0;
        DATA->PIsolated[i] = 0;
        for (j = 0; j < 8; j++)
        {
            if (i < j) DATA->Forward[0][i] |= DATA->Line[j];
            else if (i > j) DATA->Forward[1][i] |= DATA->Line[j];
            if (i < j) DATA->East[i] |= DATA->File[j];
            else if (i > j) DATA->West[i] |= DATA->File[j];
        }
        if (i > 0) DATA->PIsolated[i] |= DATA->File[i - 1];
        if (i < 7) DATA->PIsolated[i] |= DATA->File[i + 1];
    }
    for (i = 0; i < 64; i++)
    {
        for (u = DATA->QMask[i]; T(u); Cut(u))
        {
            j = lsb(u);
            k = Sgn(Rank(j)-Rank(i));
            l = Sgn(File(j)-File(i));
            for (n = i + 8 * k + l; n != j; n += (8 * k + l))
                Add(DATA->Between[i][j], n);
        }
        for (u = DATA->BMask[i]; T(u); Cut(u))
        {
            j = lsb(u);
            DATA->FullLine[i][j] = DATA->BMask[i] & DATA->BMask[j];
        }
        for (u = DATA->RMask[i]; T(u); Cut(u))
        {
            j = lsb(u);
            DATA->FullLine[i][j] = DATA->RMask[i] & DATA->RMask[j];
        }
        DATA->BishopForward[0][i] |= DATA->PWay[0][i];
        DATA->BishopForward[1][i] |= DATA->PWay[1][i];
        for (j = 0; j < 64; j++)
        {
            if ((DATA->PWay[1][j] | Bit(j)) &
                    DATA->BMask[i] & DATA->Forward[0][Rank(i)])
                DATA->BishopForward[0][i] |= Bit(j);
            if ((DATA->PWay[0][j] | Bit(j)) & DATA->BMask[i] &
                    DATA->Forward[1][Rank(i)])
                DATA->BishopForward[1][i] |= Bit(j);
        }
    }

    for (i = 0; i < 16; i++) for (j = 0; j < 16; j++)
    {
        if (j < WhitePawn) DATA->MvvLva[i][j] = 0;
        else if (j < WhiteKnight)
            DATA->MvvLva[i][j] = PawnCaptureMvvLva(i) << 26;
        else if (j < WhiteLight)
            DATA->MvvLva[i][j] = KnightCaptureMvvLva(i) << 26;
        else if (j < WhiteRook)
            DATA->MvvLva[i][j] = BishopCaptureMvvLva(i) << 26;
        else if (j < WhiteQueen)
            DATA->MvvLva[i][j] = RookCaptureMvvLva(i) << 26;
        else DATA->MvvLva[i][j] = QueenCaptureMvvLva(i) << 26;
    }

    DATA->PieceFromChar['B'] = WhiteLight;
    DATA->PieceFromChar['K'] = WhiteKing;
    DATA->PieceFromChar['N'] = WhiteKnight;
    DATA->PieceFromChar['P'] = WhitePawn;
    DATA->PieceFromChar['Q'] = WhiteQueen;
    DATA->PieceFromChar['R'] = WhiteRook;
    DATA->PieceFromChar['b'] = BlackLight;
    DATA->PieceFromChar['k'] = BlackKing;
    DATA->PieceFromChar['n'] = BlackKnight;
    DATA->PieceFromChar['p'] = BlackPawn;
    DATA->PieceFromChar['q'] = BlackQueen;
    DATA->PieceFromChar['r'] = BlackRook;

    DATA->TurnKey = rand64();
    for (i = 0; i < 8; i++) DATA->EPKey[i] = rand64();
    for (i = 0; i < 16; i++) DATA->CastleKey[i] = rand64();
    for (i = 0; i < 16; i++) for (j = 0; j < 64; j++) {
        if (i == 0) DATA->PieceKey[i][j] = 0;
        else DATA->PieceKey[i][j] = rand64();
    }
    for (i = 0; i < 16; i++)
        DATA->LogDist[i] = (int)(10.0 * log(1.01 + (double)i));

    memcpy(DATA->UpdateCastling, UpdateCastling,
        sizeof(DATA->UpdateCastling));
}

static void init_kpk(void)
{
    int turn, wp, wk, bk, to, cnt, old_cnt, un;
    uint64_t bwp, bwk, bbk, u;
    uint8_t Kpk_gen[2][64][64][64];

    memset(Kpk_gen, 0, sizeof(Kpk_gen));

    cnt = 0;
    old_cnt = 1;
start:
    if (cnt == old_cnt) goto end;
    old_cnt = cnt;
    cnt = 0;
    for (turn = 0; turn < 2; turn++)
    {
        for (wp = 0; wp < 64; wp++)
        {
            for (wk = 0; wk < 64; wk++)
            {
                for (bk = 0; bk < 64; bk++)
                {
                    if (Kpk_gen[turn][wp][wk][bk]) continue;
                    cnt++;
                    if (wp < 8 || wp >= 56) goto set_draw;
                    if (wp == wk || wk == bk || bk == wp) goto set_draw;
                    bwp = Bit(wp);
                    bwk = Bit(wk);
                    bbk = Bit(bk);
                    if (DATA->PAtt[White][wp] & bbk)
                    {
                        if (turn == White) goto set_draw;
                        else if (F(DATA->SArea[wk] & bwp)) goto set_draw;
                    }
                    un = 0;
                    if (turn == Black)
                    {
                        u = DATA->SArea[bk] &
                            (~(DATA->SArea[wk] | DATA->PAtt[White][wp]));
                        if (F(u)) goto set_draw;
                        for (; T(u); Cut(u))
                        {
                            to = lsb(u);
                            if (Kpk_gen[turn ^ 1][wp][wk][to] == 1)
                                goto set_draw;
                            else if (Kpk_gen[turn ^ 1][wp][wk][to] == 0)
                                un++;
                        }
                        if (F(un)) goto set_win;
                    }
                    else
                    {
                        for (u = DATA->SArea[wk] & (~(DATA->SArea[bk] | bwp));
                                T(u); Cut(u))
                        {
                            to = lsb(u);
                            if (Kpk_gen[turn ^ 1][wp][to][bk] == 2)
                                goto set_win;
                            else if (Kpk_gen[turn ^ 1][wp][to][bk] == 0)
                                un++;
                        }
                        to = wp + 8;
                        if (to != wk && to != bk)
                        {
                            if (to >= 56)
                            {
                                if (F(DATA->SArea[to] & bbk)) goto set_win;
                                if (DATA->SArea[to] & bwk) goto set_win;
                            }
                            else
                            {
                                if (Kpk_gen[turn ^ 1][to][wk][bk] == 2)
                                    goto set_win;
                                else if (Kpk_gen[turn ^ 1][to][wk][bk] == 0)
                                    un++;
                                if (to < 24)
                                {
                                    to += 8;
                                    if (to != wk && to != bk)
                                    {
                                        if (Kpk_gen[turn ^ 1][to][wk][bk] == 2)
                                            goto set_win;
                                        else if (
                                            Kpk_gen[turn ^ 1][to][wk][bk] == 0)
                                             un++;
                                    }
                                }
                            }
                        }
                        if (F(un)) goto set_draw;
                    }
                    continue;
                set_draw:
                    Kpk_gen[turn][wp][wk][bk] = 1;
                    continue;
                set_win:
                    Kpk_gen[turn][wp][wk][bk] = 2;
                    continue;
                }
            }
        }
    }
    if (cnt) goto start;
end:
    for (turn = 0; turn < 2; turn++)
    {
        for (wp = 0; wp < 64; wp++)
        {
            for (wk = 0; wk < 64; wk++)
            {
                DATA->Kpk[turn][wp][wk] = 0;
                for (bk = 0; bk < 64; bk++)
                {
                    if (Kpk_gen[turn][wp][wk][bk] == 2)
                        DATA->Kpk[turn][wp][wk] |= Bit(bk);
                }
            }
        }
    }
}

#define pst_idx(piece, sq)      (((piece) << 6) | (sq))
static void init_pst(void)
{
    int i, j, k, op, eg, index, r, f, d, e, distQ[4], distL[4], distM[2];

    for (i = 0; i < 64; i++)
    {
        r = Rank(i);
        f = File(i);
        d = Abs(f - r);
        e = Abs(f + r - 7);
        distQ[0] = DistC[f] * DistC[f]; distL[0] = DistC[f];
        distQ[1] = DistC[r] * DistC[r]; distL[1] = DistC[r];
        distQ[2] = RankR[d] * RankR[d] + RankR[e] * RankR[e]; distL[2] =
            RankR[d] + RankR[e];
        distQ[3] = RankR[r] * RankR[r]; distL[3] = RankR[r];
        distM[0] = DistC[f] * DistC[r]; distM[1] = DistC[f] * RankR[r];
        for (j = 2; j < 16; j += 2)
        {
            index = PieceType[j];
            op = eg = 0;
            for (k = 0; k < 2; k++)
            {
                op += Av(PstQuadMixedWeights, 4, index, (k * 2)) * distM[k];
                eg += Av(PstQuadMixedWeights, 4, index, (k * 2) + 1) * distM[k];
            }
            for (k = 0; k < 4; k++)
            {
                op += Av(PstQuadWeights,8,index,(k * 2)) * distQ[k];
                eg += Av(PstQuadWeights,8,index,(k * 2) + 1) * distQ[k];
                op += Av(PstLinearWeights,8,index,(k * 2)) * distL[k];
                eg += Av(PstLinearWeights,8,index,(k * 2) + 1) * distL[k];
            }
            DATA->Pst[pst_idx(j, i)] = Compose(op/64, eg/64);
        }
    }

    DATA->Pst[pst_idx(WhiteKnight, 56)] -= Compose(100, 0);
    DATA->Pst[pst_idx(WhiteKnight, 63)] -= Compose(100, 0);
    for (i = 0; i < 64; i++)
    {
        for (j = 3; j < 16; j+=2)
        {
            op = Opening(DATA->Pst[pst_idx(j-1, 63-i)]);
            eg = Endgame(DATA->Pst[pst_idx(j-1, 63-i)]);
            DATA->Pst[pst_idx(j, i)] = Compose(-op, -eg);
        }
    }
}

static void init_eval()
{
    int i, j, k, index;
//    memset(Mobility,0,4 * 32 * sizeof(int));
    for (i = 0; i < 4; i++) for (j = 0; j < 32; j++)
    {
        index = i * 2;
        double op =
            (double)(Av(MobilityLinear, 8, 0, index) * j) +
                log(1.01 + (double)j) * (double)(Av(MobilityLog, 8, 0, index));
        index = i * 2 + 1;
        double eg =
            (double)(Av(MobilityLinear, 8, 0, index) * j) +
                log(1.01 + (double)j) * (double)(Av(MobilityLog, 8, 0, index));
        DATA->Mobility[i][j] = Compose((int)(op/64.0), (int)(eg/64.0));
    }

    for (i = 0; i < 3; i++) for (j = 7; j >= 0; j--)
    {
        DATA->Shelter[i][j] = 0;
        if (j > 1) for (k = 1; k < Min(j, 5); k++)
            DATA->Shelter[i][j] += Av(ShelterValue, 0, 0, (i * 5) + k - 1);
        if (!j)
            DATA->Shelter[i][j] = DATA->Shelter[i][7] +
                Av(ShelterValue, 0, 0, (i * 5) + 4);
    }

    for (i = 0; i < 4; i++)
    {
        DATA->StormBlocked[i] =
            ((Sa(StormQuad, StormBlockedMul) * i * i) +
                (Sa(StormLinear, StormBlockedMul) * (i + 1))) / 100;
        DATA->StormShelterAtt[i] =
            ((Sa(StormQuad, StormShelterAttMul) * i * i) +
                (Sa(StormLinear, StormShelterAttMul) * (i + 1))) / 100;
        DATA->StormConnected[i] =
            ((Sa(StormQuad, StormConnectedMul) * i * i) +
                (Sa(StormLinear, StormConnectedMul) * (i + 1))) / 100;
        DATA->StormOpen[i] =
            ((Sa(StormQuad, StormOpenMul) * i * i) +
                (Sa(StormLinear, StormOpenMul) * (i + 1))) / 100;
        DATA->StormFree[i] =
            ((Sa(StormQuad, StormFreeMul) * i * i) +
                (Sa(StormLinear, StormFreeMul) * (i + 1))) / 100;
    }

    for (i = 0; i < 8; i++)
    {
        int l = Max(i - 2, 0);
        int q = l * l;
        DATA->PasserGeneral[i] = Compose16(
            Av(PasserQuad, 2, 0, 0) * q + Av(PasserLinear, 2, 0, 0) * l,
            Av(PasserQuad, 2, 0, 1) * q + Av(PasserLinear, 2, 0, 1) * l);
        DATA->PasserBlocked[i] = Compose16(
            Av(PasserQuad, 2, 1, 0) * q + Av(PasserLinear, 2, 1, 0) * l,
            Av(PasserQuad, 2, 1, 1) * q + Av(PasserLinear, 2, 1, 1) * l);
        DATA->PasserFree[i] = Compose16(
            Av(PasserQuad, 2, 2, 0) * q + Av(PasserLinear, 2, 2, 0) * l,
            Av(PasserQuad, 2, 2, 1) * q + Av(PasserLinear, 2, 2, 1) * l);
        DATA->PasserSupported[i] = Compose16(
            Av(PasserQuad, 2, 3, 0) * q + Av(PasserLinear, 2, 3, 0) * l,
            Av(PasserQuad, 2, 3, 1) * q + Av(PasserLinear, 2, 3, 1) * l);
        DATA->PasserProtected[i] = Compose16(
            Av(PasserQuad, 2, 4, 0) * q + Av(PasserLinear, 2, 4, 0) * l,
            Av(PasserQuad, 2, 4, 1) * q + Av(PasserLinear, 2, 4, 1) * l);
        DATA->PasserConnected[i] = Compose16(
            Av(PasserQuad, 2, 5, 0) * q + Av(PasserLinear, 2, 5, 0) * l,
            Av(PasserQuad, 2, 5, 1) * q + Av(PasserLinear, 2, 5, 1) * l);
        DATA->PasserOutside[i] = Compose16(
            Av(PasserQuad, 2, 6, 0) * q + Av(PasserLinear, 2, 6, 0) * l,
            Av(PasserQuad, 2, 6, 1) * q + Av(PasserLinear, 2, 6, 1) * l);
        DATA->PasserCandidate[i] = Compose16(
            Av(PasserQuad, 2, 7, 0) * q + Av(PasserLinear, 2, 7, 0) * l,
            Av(PasserQuad, 2, 7, 1) * q + Av(PasserLinear, 2, 7, 1) * l);
        DATA->PasserClear[i] = Compose16(
            Av(PasserQuad, 2, 8, 0) * q + Av(PasserLinear, 2, 8, 0) * l,
            Av(PasserQuad, 2, 8, 1) * q + Av(PasserLinear, 2, 8, 1) * l);
        DATA->PasserAtt[i] = Av(PasserAttDefQuad, 2, 0, 0) * q +
            Av(PasserAttDefLinear, 2, 0, 0) * l;
        DATA->PasserDef[i] = Av(PasserAttDefQuad, 2, 1, 0) * q +
            Av(PasserAttDefLinear, 2, 1, 0) * l;
        DATA->PasserAttLog[i] = Av(PasserAttDefQuad, 2, 0, 1) * q +
            Av(PasserAttDefLinear, 2, 0, 1) * l;
        DATA->PasserDefLog[i] = Av(PasserAttDefQuad, 2, 1, 1) * q +
            Av(PasserAttDefLinear, 2, 1, 1) * l;
    }
}

void calc_material(int index)
{
    int pawns[2], knights[2], light[2], dark[2], rooks[2], queens[2],
        bishops[2], major[2], minor[2], tot[2], mat[2], mul[2], quad[2],
        score, phase, me, i = index;

    queens[White] = i % 3; i /= 3;
    queens[Black] = i % 3; i /= 3;
    rooks[White] = i % 3; i /= 3;
    rooks[Black] = i % 3; i /= 3;
    light[White] = i % 2; i /= 2;
    light[Black] = i % 2; i /= 2;
    dark[White] = i % 2; i /= 2;
    dark[Black] = i % 2; i /= 2;
    knights[White] = i % 3; i /= 3;
    knights[Black] = i % 3; i /= 3;
    pawns[White] = i % 9; i /= 9;
    pawns[Black] = i % 9;
    for (me = 0; me < 2; me++)
    {
        bishops[me] = light[me] + dark[me];
        major[me] = rooks[me] + queens[me];
        minor[me] = bishops[me] + knights[me];
        tot[me] = 3 * minor[me] + 5 * rooks[me] + 9 * queens[me];
        mat[me] = mul[me] = 32;
        quad[me] = 0;
    }
    score =
        (SeeValue[WhitePawn] + Av(MatLinear, 0, 0, 0)) *
            (pawns[White] - pawns[Black]) +
        (SeeValue[WhiteKnight] + Av(MatLinear, 0, 0, 1)) *
            (knights[White] - knights[Black]) +
        (SeeValue[WhiteLight] + Av(MatLinear, 0, 0, 2)) *
            (bishops[White] - bishops[Black]) +
        (SeeValue[WhiteRook] + Av(MatLinear, 0, 0, 3)) *
            (rooks[White] - rooks[Black]) +
        (SeeValue[WhiteQueen] + Av(MatLinear, 0, 0, 4)) *
            (queens[White] - queens[Black]) +
        50 * ((bishops[White] / 2) - (bishops[Black] / 2));
    phase =
        Phase[PieceType[WhitePawn]] * (pawns[White] + pawns[Black]) +
        Phase[PieceType[WhiteKnight]] * (knights[White] + knights[Black]) +
        Phase[PieceType[WhiteLight]] * (bishops[White] + bishops[Black]) +
        Phase[PieceType[WhiteRook]] * (rooks[White] + rooks[Black]) +
        Phase[PieceType[WhiteQueen]] * (queens[White] + queens[Black]);
    DATA->Material[index].phase =
        Min((Max(phase - PhaseMin, 0) * 128) / (PhaseMax - PhaseMin), 128);
    int special = 0;
    for (me = 0; me < 2; me++)
    {
        if (queens[me] == queens[opp])
        {
            if (rooks[me] - rooks[opp] == 1)
            {
                if (knights[me] == knights[opp] &&
                        bishops[opp] - bishops[me] == 1)
                    IncV(special, Ca(MatSpecial, MatRB));
                else if (bishops[me] == bishops[opp] &&
                        knights[opp] - knights[me] == 1)
                    IncV(special, Ca(MatSpecial, MatRN));
                else if (knights[me] == knights[opp] &&
                        bishops[opp] - bishops[me] == 2)
                    DecV(special, Ca(MatSpecial, MatBBR));
                else if (bishops[me] == bishops[opp] &&
                    knights[opp] - knights[me] == 2)
                        DecV(special, Ca(MatSpecial, MatNNR));
                else if (bishops[opp] - bishops[me] == 1 &&
                        knights[opp] - knights[me] == 1)
                    DecV(special, Ca(MatSpecial, MatBNR));
            }
            else if (rooks[me] == rooks[opp] &&
                    minor[me] - minor[opp] == 1)
                IncV(special, Ca(MatSpecial, MatM));
        }
        else if (queens[me] - queens[opp] == 1)
        {
            if (rooks[opp] - rooks[me] == 2 &&
                    minor[opp] - minor[me] == 0)
                IncV(special, Ca(MatSpecial, MatQRR));
            else if (rooks[opp] - rooks[me] == 1 &&
                    knights[opp] == knights[me] &&
                    bishops[opp] - bishops[me] == 1)
                IncV(special, Ca(MatSpecial, MatQRB));
            else if (rooks[opp] - rooks[me] == 1 &&
                    knights[opp] - knights[me] == 1 &&
                    bishops[opp] == bishops[me])
                IncV(special, Ca(MatSpecial, MatQRN));
            else if ((major[opp] + minor[opp]) - (major[me] + minor[me]) >= 2)
                IncV(special, Ca(MatSpecial, MatQ3));
        }
    }
    score +=
        (Opening(special) * DATA->Material[index].phase +
            Endgame(special) * (128 - (int)DATA->Material[index].phase)) / 128;

    for (me = 0; me < 2; me++)
    {
        quad[me] += pawns[me] * (pawns[me] * TrAv(MatQuadMe, 5, 0, 0) +
            knights[me] * TrAv(MatQuadMe, 5, 0, 1) +
            bishops[me] * TrAv(MatQuadMe, 5, 0, 2) +
            rooks[me] * TrAv(MatQuadMe, 5, 0, 3) +
            queens[me] * TrAv(MatQuadMe, 5, 0, 4));
        quad[me] += knights[me] * (knights[me] * TrAv(MatQuadMe, 5, 1, 0) +
            bishops[me] * TrAv(MatQuadMe, 5, 1, 1) +
            rooks[me] * TrAv(MatQuadMe, 5, 1, 2) +
            queens[me] * TrAv(MatQuadMe, 5, 1, 3));
        quad[me] += bishops[me] * (bishops[me] * TrAv(MatQuadMe, 5, 2, 0) +
            rooks[me] * TrAv(MatQuadMe, 5, 2, 1) +
            queens[me] * TrAv(MatQuadMe, 5, 2, 2));
        quad[me] += rooks[me] * (rooks[me] * TrAv(MatQuadMe, 5, 3, 0) +
            queens[me] * TrAv(MatQuadMe, 5, 3, 1));
        quad[me] += pawns[me] * (knights[opp] * TrAv(MatQuadOpp, 4, 0, 0) +
            bishops[opp] * TrAv(MatQuadOpp, 4, 0, 1) +
            rooks[opp] * TrAv(MatQuadOpp, 4, 0, 2) +
            queens[opp] * TrAv(MatQuadOpp, 4, 0, 3));
        quad[me] += knights[me] * (bishops[opp] * TrAv(MatQuadOpp, 4, 1, 0) +
            rooks[opp] * TrAv(MatQuadOpp, 4, 1, 1) +
            queens[opp] * TrAv(MatQuadOpp, 4, 1, 2));
        quad[me] += bishops[me] * (rooks[opp] * TrAv(MatQuadOpp, 4, 2, 0) +
            queens[opp] * TrAv(MatQuadOpp, 4, 2, 1));
        quad[me] += rooks[me] * queens[opp] * TrAv(MatQuadOpp, 4, 3, 0);

        if (bishops[me] >= 2)
            quad[me] += pawns[me] * Av(BishopPairQuad, 0, 0, 0) +
                knights[me] * Av(BishopPairQuad, 0, 0, 1) +
                rooks[me] * Av(BishopPairQuad, 0, 0, 2) +
                queens[me] * Av(BishopPairQuad, 0, 0, 3) +
                pawns[opp] * Av(BishopPairQuad, 0, 0, 4) +
                knights[opp] * Av(BishopPairQuad, 0, 0, 5) +
                bishops[opp] * Av(BishopPairQuad, 0, 0, 6) +
                rooks[opp] * Av(BishopPairQuad, 0, 0, 7) +
                queens[opp] * Av(BishopPairQuad, 0, 0, 8);
    }
    score += (quad[White] - quad[Black]) / 100;

    for (me = 0; me < 2; me++)
    {
        if (tot[me] - tot[opp] <= 3)
        {
            if (!pawns[me])
            {
                if (tot[me] <= 3) mul[me] = 0;
                if (tot[me] == tot[opp] && major[me] == major[opp] &&
                        minor[me] == minor[opp])
                    mul[me] = major[me] + minor[me] <= 2 ? 0 :
                        (major[me] + minor[me] <= 3 ? 16 : 32);
                else if (minor[me] + major[me] <= 2)
                {
                    if (bishops[me] < 2)
                        mat[me] = (bishops[me] && rooks[me]) ? 8 : 1;
                    else if (bishops[opp] + rooks[opp] >= 1) mat[me] = 1;
                    else mat[me] = 32;
                }
                else if (tot[me] - tot[opp] < 3 &&
                        minor[me] + major[me] - minor[opp] - major[opp] <= 1)
                    mat[me] = 4;
                else if (minor[me] + major[me] <= 3)
                    mat[me] = 8 * (1 + bishops[me]);
                else mat[me] = 8 * (2 + bishops[me]);
            }
            if (pawns[me] <= 1)
            {
                mul[me] = Min(28, mul[me]);
                if (rooks[me] == 1 && queens[me] + minor[me] == 0 &&
                        rooks[opp] == 1)
                    mat[me] = Min(23, mat[me]);
            }
        }
        if (!major[me])
        {
            if (!minor[me])
            {
                if (!tot[me] && pawns[me] < pawns[opp])
                    DecV(score, (pawns[opp] - pawns[me]) * SeeValue[WhitePawn]);
            }
            else if (minor[me] == 1)
            {
                if (pawns[me] <= 1 && minor[opp] >= 1) mat[me] = 1;
                if (bishops[me] == 1)
                {
                    if (minor[opp] == 1 && bishops[opp] == 1 &&
                            light[me] != light[opp])
                    {
                        mul[me] = Min(mul[me], 15);
                        if (pawns[me] - pawns[opp] <= 1)
                            mul[me] = Min(mul[me], 11);
                    }
                }
            }
            else if (!pawns[me] && knights[me] == 2 && !bishops[me])
            {
                if (!tot[opp] && pawns[opp]) mat[me] = 6;
                else mul[me] = 0;
            }
        }
        if (!mul[me]) mat[me] = 0;
        if (mat[me] <= 1 && tot[me] != tot[opp]) mul[me] = Min(mul[me], 8);
    }
    if (bishops[White] == 1 && bishops[Black] == 1 &&
            light[White] != light[Black])
    {
        mul[White] = Min(mul[White], 24 + 2 * (knights[Black] + major[Black]));
        mul[Black] = Min(mul[Black], 24 + 2 * (knights[White] + major[White]));
    }
    else if (!minor[White] && !minor[Black] && major[White] == 1 &&
            major[Black] == 1 && rooks[White] == rooks[Black])
    {
        mul[White] = Min(mul[White], 25);
        mul[Black] = Min(mul[Black], 25);
    }
    for (me = 0; me < 2; me++)
    {
        DATA->Material[index].mul[me] = mul[me];
        DATA->Material[index].pieces[me] = major[me] + minor[me];
    }
    if (score > 0) score = (score * mat[White]) / 32;
    else score = (score * mat[Black]) / 32;
    DATA->Material[index].score = score;
    for (me = 0; me < 2; me++)
    {
        if (major[me] == 0 && minor[me] == bishops[me] && minor[me] <= 1)
            DATA->Material[index].flags |= VarC(FlagSingleBishop, me);
        if (((major[me] == 0 || minor[me] == 0) &&
                major[me] + minor[me] <= 1) || major[opp] + minor[opp] == 0 ||
                (!pawns[me] && major[me] == rooks[me] && major[me] == 1 &&
                 minor[me] == bishops[me] && minor[me] == 1 &&
                 rooks[opp] == 1 && !minor[opp] && !queens[opp]))
            DATA->Material[index].flags |= VarC(FlagCallEvalEndgame, me);
    }
}

static void init_material(void)
{
    for (int index = 0; index < TotalMat; index++)
        calc_material(index);
}

static void init_data(void)
{
    init_misc();
    init_magic();
    init_kpk();
    init_pst();
    init_eval();
    init_material();
}

