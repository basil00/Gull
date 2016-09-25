/*
 * tbconfig.h
 * (C) 2015 basil, all rights reserved,
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef TBCONFIG_H
#define TBCONFIG_H

/****************************************************************************/
/* BUILD CONFIG:                                                            */
/****************************************************************************/

/*
 * Define TB_NO_STDINT if you do not want to use <stdint.h> or it is not
 * available.
 */
/* #define TB_NO_STDINT */

/*
 * Define TB_NO_STDBOOL if you do not want to use <stdbool.h> or it is not
 * available or unnecessary (e.g. C++).
 */
#define TB_NO_STDBOOL       // Gull is C++, so no need for stdbool.h

/*
 * Define TB_NO_THREADS if your program is not multi-threaded.
 */
/* #define TB_NO_THREADS */
#define TB_NO_THREADS       // Gull does not use threads.

/***************************************************************************/
/* ENGINE INTEGRATION CONFIG                                               */
/***************************************************************************/

#ifdef LINUX
#include "Linux.h"
#endif

#ifdef WINDOWS
#include "Windows.h"
#endif

#include "data.h"

/*
 * If you are integrating tbprobe into an engine, you can replace some of
 * tbprobe's built-in functionality with that already provided by the engine.
 * This is OPTIONAL.  If no definition are provided then tbprobe will use its
 * own internal defaults.  That said, for engines it is generally a good idea
 * to avoid redundancy.
 */

/*
 * Define TB_KING_ATTACKS(square) to return the king attacks bitboard for a
 * king at `square'.
 */
//extern uint64 SArea[64];
#define TB_KING_ATTACKS(square)             DATA->SArea[(square)]

/*
 * Define TB_KNIGHT_ATTACKS(square) to return the knight attacks bitboard for
 * a knight at `square'.
 */
//extern uint64 NAtt[64];
#define TB_KNIGHT_ATTACKS(square)           DATA->NAtt[(square)]

/*
 * Define TB_ROOK_ATTACKS(square, occ) to return the rook attacks bitboard
 * for a rook at `square' assuming the given `occ' occupancy bitboard.
 */
#ifndef HNI
#define TB_ROOK_ATTACKS(square, occ)                                        \
    (*(DATA->ROffsetPointer[(square)] +                                     \
        (((DATA->RMagicMask[(square)] & (occ)) *  \
            DATA->RMagic[(square)]) >> DATA->RShift[(square)])))
#else
#define TB_ROOK_ATTACKS(square, occ)                                        \
    (*(ROffsetPointer[(square)] + _pext_u64((occ), RMagicMask[(square)])))
#endif

/*
 * Define TB_BISHOP_ATTACKS(square, occ) to return the bishop attacks bitboard
 * for a bishop at `square' assuming the given `occ' occupancy bitboard.
 */
#ifndef HNI
#define TB_BISHOP_ATTACKS(square, occ)                                      \
    (*(DATA->BOffsetPointer[(square)] +                                     \
        (((DATA->BMagicMask[(square)] & (occ)) *  \
            DATA->BMagic[(square)]) >> DATA->BShift[(square)])))
#else
#define TB_BISHOP_ATTACKS(square, occ)                                      \
    (*(BOffsetPointer[(square)] + _pext_u64((occ), BMagicMask[(square)])))
#endif

/*
 * Define TB_QUEEN_ATTACKS(square, occ) to return the queen attacks bitboard
 * for a queen at `square' assuming the given `occ' occupancy bitboard.
 * NOTE: If no definition is provided then tbprobe will use:
 *       TB_ROOK_ATTACKS(square, occ) | TB_BISHOP_ATTACKS(square, occ)
 */
#define TB_QUEEN_ATTACKS(square, occ)                                       \
    (TB_ROOK_ATTACKS(square, occ) | TB_BISHOP_ATTACKS(square, occ))

/*
 * Define TB_PAWN_ATTACKS(square, color) to return the pawn attacks bitboard
 * for a `color' pawn at `square'.
 * NOTE: This definition must work for pawns on ranks 1 and 8.  For example,
 *       a white pawn on e1 attacks d2 and f2.  A black pawn on e1 attacks
 *       nothing.  Etc.
 */
//extern uint64 PAtt[2][64];
#define TB_PAWN_ATTACKS(square, color)      DATA->PAtt[!(color)][(square)]

#endif
