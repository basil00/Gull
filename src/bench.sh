#!/bin/sh

if [ $# != 2 ]
then
    echo "usage: $0 <prog> <depth>" >&2
    exit 1
fi

$1 bench $2 \
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" \
    "r1bq1rk1/4bppp/p2p1n2/npp1p3/3PP3/2P2N1P/PPB2PP1/RNBQR1K1 b - d3 0 11" \
    "rnbqkb1r/1p3ppp/p2ppn2/6B1/3NPP2/2N5/PPP3PP/R2QKB1R b KQkq f3 0 7" \
    "r1bqr1k1/pppn1pbp/3p1np1/4p3/2PP4/5NP1/PPQ1PPBP/RNBR2K1 w - - 2 9" \
    "r1r3k1/1p1n1pp1/2p1pqn1/p2p3p/N2PPP2/3P2P1/PPR2QPN/5RK1 b - - 3 22" \
    "r1bqkbnr/pp1ppppp/2n5/2p5/2P5/5N2/PP1PPPPP/RNBQKB1R w KQkq - 1 3" \
    "r6r/p1pq2k1/b2p4/P1pP1pp1/2P1p3/2P1P1P1/Q4P2/1R2RBK1 w - - 4 30" \
    "1r2kb1r/pp1b2pp/4pq2/4np2/2p2B2/2N3P1/PPQ1PPBP/3R1RK1 b k - 1 15" \
    "2rr2k1/p1q2p1p/1p4pB/1B1bb3/P7/2P2QP1/1P2RP2/4R1K1 w - - 10 28" \
    "R7/3kqp2/3n3Q/2pPp2P/2P1P3/1r4P1/8/7K b - - 0 52" \
    "8/2p5/p3k3/2p1b2p/P2pK2p/1P1P3P/3B2P1/8 b - - 4 77" \
    "5k1B/5p2/1pp2P2/3n3P/8/8/P1B5/6K1 b - - 0 49" \
    "8/1p6/p3k1pp/P4p2/1P3PP1/4K1nP/3N4/8 b - - 1 43" \
    "3Q4/p7/1kp5/8/3Pq1BP/4P1KP/8/5r2 b - - 5 41" \
    "4r2k/1p3rp1/p1p1n3/3pP2N/6R1/4P3/PP5P/6RK w - - 7 28" \
    "4k3/3b4/8/3N4/1PK1P3/3n1r2/8/1R6 w - - 1 50" \
    "1r1bb1k1/2q2p1p/1p1p2r1/pP1P4/NpP1PP2/3BB2p/5Q2/1R3R1K w - - 1 32" \
    "r1b5/pppk3N/8/5RQ1/7P/P1P1B3/1P6/2K4q w - - 1 44" \
    "8/8/8/7p/k4P1P/2b2KP1/8/8 w - - 0 82" \
    "5r2/5pk1/6p1/7p/7P/6P1/5PK1/5R2 w - - 0 1" \
    "8/8/8/8/4k3/2Q4p/1K4pq/8 w - - 0 1" \
    "5k2/3p4/2p4p/1p6/1P6/P1PP4/2KP4/8 w - - 0 1" \
    "8/8/1R5p/4pk2/p7/1P3pK1/5r2/8 b - - 1 57"

