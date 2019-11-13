//   GreKo chess engine
//   (c) 2002-2018 Vladimir Medvedev <vrm@bk.ru>
//   http://greko.su

#include "eval.h"
#include "eval_params.h"
#include "utils.h"

static const int g_pawnHashSize = 32768;
static PawnHashEntry g_pawnHash[g_pawnHashSize];

Pair PSQ[14][64];
Pair PSQ_PP[64];
Pair PSQ_PP_BLOCKED[64];
Pair PSQ_PP_FREE[64];
Pair PSQ_PP_CONNECTED[64];
Pair PSQ_PP_SQUARE[64];

Pair PAWN_DOUBLED_ISOLATED[64];
Pair PAWN_DOUBLED[64];
Pair PAWN_ISOLATED[64];
Pair PAWN_BLOCKED[64];
Pair PAWN_RAM[64];
Pair PAWN_ON_BISHOP_COLOR;
Pair PAWN_ZERO;

Pair KNIGHT_PAIR;
Pair KNIGHT_STRONG[64];
Pair KNIGHT_FORPOST[64];
Pair KNIGHT_KING_DIST[10];
Pair KNIGHT_AND_QUEEN;

Pair BISHOP_PAIR;
Pair BISHOP_STRONG[64];
Pair BISHOP_MOBILITY[14];
Pair BISHOP_KING_DIST[10];
Pair BISHOP_AND_ROOK;

Pair ROOK_PAIR;
Pair ROOK_OPEN;
Pair ROOK_7TH;
Pair ROOK_MOBILITY[15];
Pair ROOK_CONNECTED;
Pair ROOK_KING_DIST[10];

Pair QUEEN_7TH;
Pair QUEEN_KING_DIST[10];

Pair KING_PAWN_SHIELD[10];
Pair KING_PAWN_STORM[10];
Pair KING_PASSED_DIST[10];
Pair KING_EXPOSED[28];

Pair ATTACK_KING[8];
Pair ATTACK_STRONGER;
Pair ATTACK_CENTER;

Pair TEMPO;

int Distance(FLD f1, FLD f2)
{
	static const int dist[100] =
	{
		0, 1, 1, 1, 2, 2, 2, 2, 2, 3,
		3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 9, 9, 9, 9, 9, 9, 9, 9, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9
	};

	int drow = Row(f1) - Row(f2);
	int dcol = Col(f1) - Col(f2);
	return dist[drow * drow + dcol * dcol];
}
////////////////////////////////////////////////////////////////////////////////

int PawnShieldPenalty(const PawnHashEntry& pEntry, int fileK, COLOR side)
{
	static const int delta[2][8] =
	{
		{ 3, 3, 3, 3, 2, 1, 0, 3 },
		{ 3, 0, 1, 2, 3, 3, 3, 3 }
	};

	int penalty = 0;
	for (int file = fileK - 1; file < fileK + 2; ++file)
	{
		int rank = pEntry.m_ranks[file][side];
		penalty += delta[side][rank];
	}

	if (penalty < 0)
		penalty = 0;
	if (penalty > 9)
		penalty = 9;

	return penalty;
}
////////////////////////////////////////////////////////////////////////////////

int PawnStormPenalty(const PawnHashEntry& pEntry, int fileK, COLOR side)
{
	COLOR opp = side ^ 1;

	static const int delta[2][8] =
	{
		{ 0, 0, 0, 1, 2, 3, 0, 0 },
		{ 0, 0, 3, 2, 1, 0, 0, 0 }
	};

	int penalty = 0;
	for (int file = fileK - 1; file < fileK + 2; ++file)
	{
		int rank = pEntry.m_ranks[file][opp];
		penalty += delta[side][rank];
	}

	if (penalty < 0)
		penalty = 0;
	if (penalty > 9)
		penalty = 9;

	return penalty;
}
////////////////////////////////////////////////////////////////////////////////

bool IsDrawKBPK(const Position& pos)
{
	if (pos.Endgame_KBP(WHITE) && pos.Endgame_K(BLACK))
	{
		FLD f = LSB(pos.Bits(PW));
		if (Col(f) == 0)
		{
			// A8
			if (pos.Bits(BW) & BB_BLACK_FIELDS)
				if (pos.Bits(KB) & LL(0xc0c0000000000000))
					return true;
		}
		else if (Col(f) == 7)
		{
			// H8
			if (pos.Bits(BW) & BB_WHITE_FIELDS)
				if (pos.Bits(KB) & LL(0x0303000000000000))
					return true;
		}
	}
	else if (pos.Endgame_KBP(BLACK) && pos.Endgame_K(WHITE))
	{
		FLD f = LSB(pos.Bits(PB));
		if (Col(f) == 0)
		{
			// A1
			if (pos.Bits(BB) & BB_WHITE_FIELDS)
				if (pos.Bits(KW) & LL(0x000000000000c0c0))
					return true;
		}
		else if (Col(f) == 7)
		{
			// H1
			if (pos.Bits(BB) & BB_BLACK_FIELDS)
				if (pos.Bits(KW) & LL(0x0000000000000303))
					return true;
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////

bool IsDrawKPK(const Position& pos)
{
	if (pos.Endgame_KP(WHITE) && pos.Endgame_K(BLACK))
	{
		U64 drawZone = 0;
		FLD f = LSB(pos.Bits(PW));
		if (Col(f) == 0)
			drawZone |= BB_DIR[f][DIR_U] | BB_SINGLE[B8] | BB_SINGLE[B7];
		else if (Col(f) == 7)
			drawZone |= BB_DIR[f][DIR_U] | BB_SINGLE[G8] | BB_SINGLE[G7];
		else
		{
			drawZone |= BB_SINGLE[f - 8];
			if (Row(f) > 2)
				drawZone |= BB_SINGLE[f - 16];
		}
		if (pos.Bits(KB) & drawZone)
			return true;
	}
	else if(pos.Endgame_KP(BLACK) && pos.Endgame_K(WHITE))
	{
		U64 drawZone = 0;
		FLD f = LSB(pos.Bits(PB));
		if (Col(f) == 0)
			drawZone |= BB_DIR[f][DIR_D] | BB_SINGLE[B1] | BB_SINGLE[B2];
		else if (Col(f) == 7)
			drawZone |= BB_DIR[f][DIR_D] | BB_SINGLE[G1] | BB_SINGLE[G2];
		else
		{
			drawZone |= BB_SINGLE[f + 8];
			if (Row(f) < 5)
				drawZone |= BB_SINGLE[f + 16];
		}
		if (pos.Bits(KW) & drawZone)
			return true;
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////

bool IsDrawLowMaterial(const Position& pos)
{
	if (pos.Count(PW) == 0 && pos.Count(PB) == 0)
	{
		if (pos.MatIndex(WHITE) < 5 && pos.MatIndex(BLACK) < 5)
			return true;
		if (pos.MatIndex(WHITE) == 6 && pos.Count(NW) == 2 && pos.MatIndex(BLACK) < 5)
			return true;
		if (pos.MatIndex(BLACK) == 6 && pos.Count(NB) == 2 && pos.MatIndex(WHITE) < 5)
			return true;		
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////

EVAL EvalMate(const Position& pos)
{
	EVAL e =
		VAL_P * (pos.Count(PW) - pos.Count(PB)) +
		VAL_N * (pos.Count(NW) - pos.Count(NB)) +
		VAL_B * (pos.Count(BW) - pos.Count(BB)) +
		VAL_R * (pos.Count(RW) - pos.Count(RB)) +
		VAL_Q * (pos.Count(QW) - pos.Count(QB));

	static const EVAL CENTER[64] =
	{
		-30, -20, -10,   0,   0, -10, -20, -30,
		-20, -10,   0,  10,  10,   0, -10, -20,
		-10,   0,  10,  20,  20,  10,   0, -10,
		  0,  10,  20,  30,  30,  20,  10,   0,
		  0,  10,  20,  30,  30,  20,  10,   0,
		-10,   0,  10,  20,  20,  10,   0, -10,
		-20, -10,   0,  10,  10,   0, -10, -20,
		-30, -20, -10,   0,   0, -10, -20, -30
	};

	static const EVAL MATE_ON_A1_H8[64] =
	{
		 0, 10, 20, 30, 40, 50, 60, 70,
		10, 10, 20, 30, 40, 50, 60, 60,
		20, 20, 20, 30, 40, 50, 50, 50,
		30, 30, 30, 30, 40, 40, 40, 40,
		40, 40, 40, 40, 30, 30, 30, 30,
		50, 50, 50, 40, 30, 20, 20, 20,
		60, 60, 50, 40, 30, 20, 10, 10,
		70, 60, 50, 40, 30, 20, 10,  0
	};

	static const EVAL MATE_ON_A8_H1[64] =
	{
		70, 60, 50, 40, 30, 20, 10,  0,
		60, 60, 50, 40, 30, 20, 10, 10,
		50, 50, 50, 40, 30, 20, 20, 20,
		40, 40, 40, 40, 30, 30, 30, 30,
		30, 30, 30, 30, 40, 40, 40, 40,
		20, 20, 20, 30, 40, 50, 50, 50,
		10, 10, 20, 30, 40, 50, 60, 60,
		 0, 10, 20, 30, 40, 50, 60, 70
	};

	if (pos.MatIndex(WHITE) > 3)
	{
		// stronger is white
		FLD K = pos.King(BLACK);
		if (pos.MatIndex(WHITE) == 6 && pos.Count(BW) == 1 && pos.Count(NW) == 1)
		{
			if (pos.Bits(BW) & BB_WHITE_FIELDS)
				e += MATE_ON_A8_H1[K];
			else
				e += MATE_ON_A1_H8[K];
		}
		else
			e -= CENTER[K];
		e -= Distance(pos.King(WHITE), K);
	}
	else
	{
		// stronger is black
		FLD K = pos.King(WHITE);
		if (pos.MatIndex(BLACK) == 6 && pos.Count(BB) == 1 && pos.Count(NB) == 1)
		{
			if (pos.Bits(BB) & BB_WHITE_FIELDS)
				e -= MATE_ON_A8_H1[K];
			else
				e -= MATE_ON_A1_H8[K];
		}
		else
			e += CENTER[K];
		e += Distance(pos.King(BLACK), K);
	}

	return (pos.Side() == WHITE)? e : -e;
}
////////////////////////////////////////////////////////////////////////////////

EVAL Evaluate(const Position& pos, EVAL alpha, EVAL beta)
{
	if (IsDrawKPK(pos) || IsDrawKBPK(pos) || IsDrawLowMaterial(pos))
		return DRAW_SCORE;
	if (pos.MatIndex(WHITE) > 3 && pos.Endgame_K(BLACK))
		return EvalMate(pos);
	if (pos.MatIndex(BLACK) > 3 && pos.Endgame_K(WHITE))
		return EvalMate(pos);

	int mid = pos.MatIndex(WHITE) + pos.MatIndex(BLACK);
	int end = 64 - mid;

	Pair score = pos.Score();

	EVAL lazy = (score.mid * mid + score.end * end) / 64;
	if (pos.Side() == BLACK)
		lazy = -lazy;
	if (lazy < alpha - 250)
		return alpha;
	if (lazy > beta + 250)
		return beta;

	U64 x, y, occ = pos.BitsAll();
	FLD f;

	U64 KingZone[2] = { BB_KING_ATTACKS[pos.King(WHITE)], BB_KING_ATTACKS[pos.King(BLACK)] };
	int attK[2] = { 0, 0 };

	//
	//   PAWNS
	//

	if (pos.Count(PW) == 0)
		score += PAWN_ZERO;
	if (pos.Count(PB) == 0)
		score -= PAWN_ZERO;

	if (pos.Count(BW) == 1)
	{
		U64 mask = (pos.Bits(BW) & BB_WHITE_FIELDS)? BB_WHITE_FIELDS : BB_BLACK_FIELDS;
		score += CountBits(pos.Bits(PW) & mask) * PAWN_ON_BISHOP_COLOR;
	}

	if (pos.Count(BB) == 1)
	{
		U64 mask = (pos.Bits(BB) & BB_WHITE_FIELDS)? BB_WHITE_FIELDS : BB_BLACK_FIELDS;
		score -= CountBits(pos.Bits(PB) & mask) * PAWN_ON_BISHOP_COLOR;
	}

	int index = pos.PawnHash() % g_pawnHashSize;
	PawnHashEntry& ps = g_pawnHash[index];
	if (ps.m_pawnHash != pos.PawnHash())
		ps.Read(pos);

	// passed
	x = ps.m_passedPawns[WHITE];
	while (x)
	{
		f = PopLSB(x);
		score += PSQ_PP[f];
		if (pos[f - 8] != NOPIECE)
			score += PSQ_PP_BLOCKED[f];
		else if ((BB_DIR[f][DIR_U] & occ) == 0)
			score += PSQ_PP_FREE[f];

		if (BB_PAWN_CONNECTED[f] & ps.m_passedPawns[WHITE])
			score += PSQ_PP_CONNECTED[f];

		score += KING_PASSED_DIST[Distance(f - 8, pos.King(BLACK))];
		score -= KING_PASSED_DIST[Distance(f - 8, pos.King(WHITE))];

		if ((BB_PAWN_SQUARE[f][WHITE] & pos.Bits(KB)) == 0)
			score += PSQ_PP_SQUARE[f];
	}
	x = ps.m_passedPawns[BLACK];
	while (x)
	{
		f = PopLSB(x);
		score -= PSQ_PP[FLIP[f]];
		if (pos[f + 8] != NOPIECE)
			score -= PSQ_PP_BLOCKED[FLIP[f]];
		else if ((BB_DIR[f][DIR_D] & occ) == 0)
			score -= PSQ_PP_FREE[FLIP[f]];

		if (BB_PAWN_CONNECTED[f] & ps.m_passedPawns[BLACK])
			score -= PSQ_PP_CONNECTED[FLIP[f]];

		score -= KING_PASSED_DIST[Distance(f + 8, pos.King(WHITE))];
		score += KING_PASSED_DIST[Distance(f + 8, pos.King(BLACK))];

		if ((BB_PAWN_SQUARE[f][BLACK] & pos.Bits(KW)) == 0)
			score -= PSQ_PP_SQUARE[FLIP[f]];
	}

	// doubled and isolated
	x = ps.m_doubledPawns[WHITE] & ps.m_isolatedPawns[WHITE];
	while (x)
	{
		f = PopLSB(x);
		score += PAWN_DOUBLED_ISOLATED[f];
	}
	x = ps.m_doubledPawns[BLACK] & ps.m_isolatedPawns[BLACK];
	while (x)
	{
		f = PopLSB(x);
		score -= PAWN_DOUBLED_ISOLATED[FLIP[f]];
	}

	// doubled
	x = ps.m_doubledPawns[WHITE] & ~ps.m_isolatedPawns[WHITE];
	while (x)
	{
		f = PopLSB(x);
		score += PAWN_DOUBLED[f];
	}
	x = ps.m_doubledPawns[BLACK] & ~ps.m_isolatedPawns[BLACK];
	while (x)
	{
		f = PopLSB(x);
		score -= PAWN_DOUBLED[FLIP[f]];
	}

	// isolated
	x = ps.m_isolatedPawns[WHITE] & ~ps.m_doubledPawns[WHITE];
	while (x)
	{
		f = PopLSB(x);
		score += PAWN_ISOLATED[f];
	}
	x = ps.m_isolatedPawns[BLACK] & ~ps.m_doubledPawns[BLACK];
	while (x)
	{
		f = PopLSB(x);
		score -= PAWN_ISOLATED[FLIP[f]];
	}

	// blocked
	x = pos.Bits(PW) & Down(occ);
	while (x)
	{
		f = PopLSB(x);
		score += PAWN_BLOCKED[f];
	}
	x = pos.Bits(PB) & Up(occ);
	while (x)
	{
		f = PopLSB(x);
		score -= PAWN_BLOCKED[FLIP[f]];
	}

	// rams
	x = pos.Bits(PW) & Down(pos.Bits(PB));
	while (x)
	{
		f = PopLSB(x);
		score += PAWN_RAM[f];
	}
	x = pos.Bits(PB) & Up(pos.Bits(PW));
	while (x)
	{
		f = PopLSB(x);
		score -= PAWN_RAM[FLIP[f]];
	}

	// attacks
	{
		x = pos.Bits(PW);
		y = UpRight(x) | UpLeft(x);
		U64 y1 = y & (pos.Bits(NB) | pos.Bits(BB) | pos.Bits(RB) | pos.Bits(QB));
		score += CountBits(y1) * ATTACK_STRONGER;
		U64 y2 = y & BB_CENTER[WHITE];
		score += CountBits(y2) * ATTACK_CENTER;
	}
	{
		x = pos.Bits(PB);
		y = DownRight(x) | DownLeft(x);
		U64 y1 = y & (pos.Bits(NW) | pos.Bits(BW) | pos.Bits(RW) | pos.Bits(QW));
		score -= CountBits(y1) * ATTACK_STRONGER;
		U64 y2 = y & BB_CENTER[BLACK];
		score -= CountBits(y2) * ATTACK_CENTER;
	}

	//
	//   KNIGHTS
	//

	if (pos.Count(NW) == 2)
		score += KNIGHT_PAIR;
	if (pos.Count(NB) == 2)
		score -= KNIGHT_PAIR;

	if (pos.Count(NW) > 0 && pos.Count(QW) > 0)
		score += KNIGHT_AND_QUEEN;
	if (pos.Count(NB) > 0 && pos.Count(QB) > 0)
		score -= KNIGHT_AND_QUEEN;

	x = pos.Bits(NW);
	while (x)
	{
		f = PopLSB(x);

		int dist = Distance(f, pos.King(BLACK));
		score += KNIGHT_KING_DIST[dist];
		
		if (BB_SINGLE[f] & ps.m_strongFields[WHITE])
		{
			score += KNIGHT_STRONG[f];
			int file = Col(f) + 1;
			if (ps.m_ranks[file][WHITE] == 0)
			{
				if (BB_DIR[f][DIR_D] & pos.Bits(RW))
				{
					score += KNIGHT_FORPOST[f];
				}
			}
		}

		y = BB_KNIGHT_ATTACKS[f];
		if (y & KingZone[BLACK])
			attK[WHITE] += 1;
		U64 y1 = y & (pos.Bits(RB) | pos.Bits(QB));
		score += CountBits(y1) * ATTACK_STRONGER;
		U64 y2 = y & BB_CENTER[WHITE];
		score += CountBits(y2) * ATTACK_CENTER;
	}

	x = pos.Bits(NB);
	while (x)
	{
		f = PopLSB(x);

		int dist = Distance(f, pos.King(WHITE));
		score -= KNIGHT_KING_DIST[dist];
		
		if (BB_SINGLE[f] & ps.m_strongFields[BLACK])
		{
			score -= KNIGHT_STRONG[FLIP[f]];

			int file = Col(f) + 1;
			if (ps.m_ranks[file][BLACK] == 7)
			{
				if (BB_DIR[f][DIR_U] & pos.Bits(RB))
				{
					score -= KNIGHT_FORPOST[FLIP[f]];
				}
			}
		}

		y = BB_KNIGHT_ATTACKS[f];
		if (y & KingZone[WHITE])
			attK[BLACK] += 1;
		U64 y1 = y & (pos.Bits(RW) | pos.Bits(QW));
		score -= CountBits(y1) * ATTACK_STRONGER;
		U64 y2 = y & BB_CENTER[BLACK];
		score -= CountBits(y2) * ATTACK_CENTER;
	}

	//
	//   BISHOPS
	//

	if (pos.Count(BW) == 2)
		score += BISHOP_PAIR;
	if (pos.Count(BB) == 2)
		score -= BISHOP_PAIR;

	if (pos.Count(BW) > 0 && pos.Count(RW) > 0)
		score += BISHOP_AND_ROOK;
	if (pos.Count(BB) > 0 && pos.Count(RB) > 0)
		score -= BISHOP_AND_ROOK;

	x = pos.Bits(BW);
	while (x)
	{
		f = PopLSB(x);

		int dist = Distance(f, pos.King(BLACK));
		score += BISHOP_KING_DIST[dist];
		
		if (BB_SINGLE[f] & ps.m_strongFields[WHITE])
			score += BISHOP_STRONG[f];
		y = BishopAttacks(f, occ);
		score += BISHOP_MOBILITY[CountBits(y)];
		if (y & KingZone[BLACK])
			attK[WHITE] += 1;
		U64 y1 = y & (pos.Bits(RB) | pos.Bits(QB));
		score += CountBits(y1) * ATTACK_STRONGER;
		U64 y2 = y & BB_CENTER[WHITE];
		score += CountBits(y2) * ATTACK_CENTER;
	}

	x = pos.Bits(BB);
	while (x)
	{
		f = PopLSB(x);

		int dist = Distance(f, pos.King(WHITE));
		score -= BISHOP_KING_DIST[dist];
		
		if (BB_SINGLE[f] & ps.m_strongFields[BLACK])
			score -= BISHOP_STRONG[FLIP[f]];
		y = BishopAttacks(f, occ);
		score -= BISHOP_MOBILITY[CountBits(y)];
		if (y & KingZone[WHITE])
			attK[BLACK] += 1;
		U64 y1 = y & (pos.Bits(RW) | pos.Bits(QW));
		score -= CountBits(y1) * ATTACK_STRONGER;
		U64 y2 = y & BB_CENTER[BLACK];
		score -= CountBits(y2) * ATTACK_CENTER;
	}

	//
	//   ROOKS
	//

	if (pos.Count(RW) == 2)
		score += ROOK_PAIR;
	if (pos.Count(RB) == 2)
		score -= ROOK_PAIR;

	x = pos.Bits(RW);
	while (x)
	{
		f = PopLSB(x);

		int dist = Distance(f, pos.King(BLACK));
		score += ROOK_KING_DIST[dist];

		y = RookAttacks(f, occ & ~pos.Bits(RW));
		score += ROOK_MOBILITY[CountBits(y)];

		if (y & KingZone[BLACK])
			attK[WHITE] += 1;

		if (y & pos.Bits(RW))
			score += ROOK_CONNECTED;

		U64 y1 = y & pos.Bits(QB);
		score += CountBits(y1) * ATTACK_STRONGER;
		U64 y2 = y & BB_CENTER[WHITE];
		score += CountBits(y2) * ATTACK_CENTER;

		int file = Col(f) + 1;
		if (ps.m_ranks[file][WHITE] == 0)
			score += ROOK_OPEN;

		if (Row(f) == 1)
		{
			if ((pos.Bits(PB) & LL(0x00ff000000000000)) || Row(pos.King(BLACK)) == 0)
				score += ROOK_7TH;
		}
	}

	x = pos.Bits(RB);
	while (x)
	{
		f = PopLSB(x);

		int dist = Distance(f, pos.King(WHITE));
		score -= ROOK_KING_DIST[dist];

		y = RookAttacks(f, occ & ~pos.Bits(RB));
		score -= ROOK_MOBILITY[CountBits(y)];

		if (y & KingZone[WHITE])
			attK[BLACK] += 1;

		if (y & pos.Bits(RB))
			score -= ROOK_CONNECTED;

		U64 y1 = y & pos.Bits(QW);
		score -= CountBits(y1) * ATTACK_STRONGER;
		U64 y2 = y & BB_CENTER[BLACK];
		score -= CountBits(y2) * ATTACK_CENTER;

		int file = Col(f) + 1;
		if (ps.m_ranks[file][BLACK] == 7)
			score -= ROOK_OPEN;

		if (Row(f) == 6)
		{
			if ((pos.Bits(PW) & LL(0x000000000000ff00)) || Row(pos.King(WHITE)) == 7)
				score -= ROOK_7TH;
		}
	}

	//
	//   QUEENS
	//

	x = pos.Bits(QW);
	while (x)
	{
		f = PopLSB(x);
		int dist = Distance(f, pos.King(BLACK));
		score += QUEEN_KING_DIST[dist];
		y = QueenAttacks(f, occ);
		if (y & KingZone[BLACK])
			attK[WHITE] += 1;

		U64 y2 = y & BB_CENTER[WHITE];
		score += CountBits(y2) * ATTACK_CENTER;

		if (Row(f) == 1)
		{
			if ((pos.Bits(PB) & LL(0x00ff000000000000)) || Row(pos.King(BLACK)) == 0)
				score += QUEEN_7TH;
		}
	}

	x = pos.Bits(QB);
	while (x)
	{
		f = PopLSB(x);
		int dist = Distance(f, pos.King(WHITE));
		score -= QUEEN_KING_DIST[dist];
		y = QueenAttacks(f, occ);
		if (y & KingZone[WHITE])
			attK[BLACK] += 1;

		U64 y2 = y & BB_CENTER[BLACK];
		score -= CountBits(y2) * ATTACK_CENTER;

		if (Row(f) == 6)
		{
			if ((pos.Bits(PW) & LL(0x000000000000ff00)) || Row(pos.King(WHITE)) == 7)
				score -= QUEEN_7TH;
		}
	}

	//
	//   KINGS
	//

	{
		f = pos.King(WHITE);
		int fileK = Col(f) + 1;
		int shield = PawnShieldPenalty(ps, fileK, WHITE);
		score += KING_PAWN_SHIELD[shield];
		int storm = PawnStormPenalty(ps, fileK, WHITE);
		score += KING_PAWN_STORM[storm];

		y = QueenAttacks(f, occ);
		int n = CountBits(y);
		score += KING_EXPOSED[n];
	}
	{
		f = pos.King(BLACK);
		int fileK = Col(f) + 1;
		int shield = PawnShieldPenalty(ps, fileK, BLACK);
		score -= KING_PAWN_SHIELD[shield];
		int storm = PawnStormPenalty(ps, fileK, BLACK);
		score -= KING_PAWN_STORM[storm];

		y = QueenAttacks(f, occ);
		int n = CountBits(y);
		score -= KING_EXPOSED[n];
	}

	//
	//   ATTACKS
	//

	if (attK[WHITE] > 7)
		attK[WHITE] = 7;

	if (attK[BLACK] > 7)
		attK[BLACK] = 7;

	score += ATTACK_KING[attK[WHITE]];
	score -= ATTACK_KING[attK[BLACK]];

	if (pos.Side() == WHITE)
		score += TEMPO;
	else
		score -= TEMPO;

	EVAL e = (score.mid * mid + score.end * end) / 64;

	if (e > 0 && pos.MatIndex(WHITE) < 5 && pos.Count(PW) == 0)
		e = 0;
	if (e < 0 && pos.MatIndex(BLACK) < 5 && pos.Count(PB) == 0)
		e = 0;

	if (pos.Side() == BLACK)
		e = -e;

	return e;
}
////////////////////////////////////////////////////////////////////////////////

int Q6(int tag, double x, double y)
{
	// A*x*x + B*x + C*y*y + D*y + E*x*y + F

	int* ptr = &(W[lines[tag].start]);

	int A = ptr[0];
	int B = ptr[1];
	int C = ptr[2];
	int D = ptr[3];
	int E = ptr[4];
	int F = ptr[5];

	double val = A * x * x + B * x + C * y * y + D * y + E * x * y + F;
	return int(val);
}
////////////////////////////////////////////////////////////////////////////////

void InitEval(const vector<int>& x)
{
	W = x;

	#define P(name, index) W[lines[name].start + index]

	for (FLD f = 0; f < 64; ++f)
	{
		double x = ((Col(f) > 3)? Col(f) - 3.5 : 3.5 - Col(f)) / 3.5;
		double y = (3.5 - Row(f)) / 3.5;

		if (Row(f) != 0 && Row(f) != 7)
		{
			PSQ[PW][f].mid = VAL_P + Q6(Mid_Pawn, x, y);
			PSQ[PW][f].end = VAL_P + Q6(End_Pawn, x, y);

			PSQ_PP[f].mid = Q6(Mid_PawnPassed, x, y);
			PSQ_PP[f].end = Q6(End_PawnPassed, x, y);

			PSQ_PP_FREE[f].mid = Q6(Mid_PawnPassedFree, x, y);
			PSQ_PP_FREE[f].end = Q6(End_PawnPassedFree, x, y);

			PSQ_PP_BLOCKED[f].mid = Q6(Mid_PawnPassedBlocked, x, y);
			PSQ_PP_BLOCKED[f].end = Q6(End_PawnPassedBlocked, x, y);

			PSQ_PP_CONNECTED[f].mid = Q6(Mid_PawnPassedConnected, x, y);
			PSQ_PP_CONNECTED[f].end = Q6(End_PawnPassedConnected, x, y);

			PSQ_PP_SQUARE[f].mid = Q6(Mid_PawnPassedSquare, x, y);
			PSQ_PP_SQUARE[f].end = Q6(End_PawnPassedSquare, x, y);

			PAWN_DOUBLED_ISOLATED[f].mid = Q6(Mid_PawnDoubledIsolated, x, y);
			PAWN_DOUBLED_ISOLATED[f].end = Q6(End_PawnDoubledIsolated, x, y);

			PAWN_DOUBLED[f].mid = Q6(Mid_PawnDoubled, x, y);
			PAWN_DOUBLED[f].end = Q6(End_PawnDoubled, x, y);

			PAWN_ISOLATED[f].mid = Q6(Mid_PawnIsolated, x, y);
			PAWN_ISOLATED[f].end = Q6(End_PawnIsolated, x, y);

			PAWN_BLOCKED[f].mid = Q6(Mid_PawnBlocked, x, y);
			PAWN_BLOCKED[f].end = Q6(End_PawnBlocked, x, y);

			PAWN_RAM[f].mid = Q6(Mid_PawnRam, x, y);
			PAWN_RAM[f].end = Q6(End_PawnRam, x, y);
		}
		else
		{
			PSQ[PW][f] = VAL_P;
			PSQ_PP[f] = 0;
			PSQ_PP_FREE[f] = 0;
			PSQ_PP_BLOCKED[f] = 0;
			PSQ_PP_CONNECTED[f] = 0;
			PSQ_PP_SQUARE[f] = 0;
			PAWN_DOUBLED[f] = 0;
			PAWN_ISOLATED[f] = 0;
			PAWN_BLOCKED[f] = 0;
			PAWN_RAM[f] = 0;
		}

		PSQ[NW][f].mid = VAL_N + Q6(Mid_Knight, x, y);
		PSQ[NW][f].end = VAL_N + Q6(End_Knight, x, y);

		PSQ[BW][f].mid = VAL_B + Q6(Mid_Bishop, x, y);
		PSQ[BW][f].end = VAL_B + Q6(End_Bishop, x, y);

		PSQ[RW][f].mid = VAL_R + Q6(Mid_Rook, x, y);
		PSQ[RW][f].end = VAL_R + Q6(End_Rook, x, y);

		PSQ[QW][f].mid = VAL_Q + Q6(Mid_Queen, x, y);
		PSQ[QW][f].end = VAL_Q + Q6(End_Queen, x, y);

		PSQ[KW][f].mid = VAL_K + Q6(Mid_King, x, y);
		PSQ[KW][f].end = VAL_K + Q6(End_King, x, y);

		PSQ[PB][FLIP[f]] = -PSQ[PW][f];
		PSQ[NB][FLIP[f]] = -PSQ[NW][f];
		PSQ[BB][FLIP[f]] = -PSQ[BW][f];
		PSQ[RB][FLIP[f]] = -PSQ[RW][f];
		PSQ[QB][FLIP[f]] = -PSQ[QW][f];
		PSQ[KB][FLIP[f]] = -PSQ[KW][f];

		KNIGHT_STRONG[f].mid = Q6(Mid_KnightStrong, x, y);
		KNIGHT_STRONG[f].end = Q6(End_KnightStrong, x, y);

		KNIGHT_FORPOST[f].mid = Q6(Mid_KnightForpost, x, y);
		KNIGHT_FORPOST[f].end = Q6(End_KnightForpost, x, y);

		BISHOP_STRONG[f].mid = Q6(Mid_BishopStrong, x, y);
		BISHOP_STRONG[f].end = Q6(End_BishopStrong, x, y);
	}

	PAWN_ON_BISHOP_COLOR = Pair(P(Mid_PawnOnBishopColor, 0), P(End_PawnOnBishopColor, 0));
	PAWN_ZERO = Pair(P(Mid_PawnZero, 0), P(End_PawnZero, 0));
	KNIGHT_PAIR = Pair(P(Mid_KnightPair, 0), P(End_KnightPair, 0));
	BISHOP_PAIR = Pair(P(Mid_BishopPair, 0), P(End_BishopPair, 0));
	ROOK_PAIR = Pair(P(Mid_RookPair, 0), P(End_RookPair, 0));

	KNIGHT_AND_QUEEN = Pair(P(Mid_KnightAndQueen, 0), P(End_KnightAndQueen, 0));
	BISHOP_AND_ROOK = Pair(P(Mid_BishopAndRook, 0), P(End_BishopAndRook, 0));

	BISHOP_MOBILITY[0] = 0;
	ROOK_MOBILITY[0] = 0;
	ROOK_MOBILITY[1] = 0;

	ROOK_CONNECTED = Pair(P(Mid_RookConnected, 0), P(End_RookConnected, 0));

	for (int m = 1; m < 14; ++m)
	{
		double z = (m - 7) / 6.0;

		BISHOP_MOBILITY[m].mid = EVAL(P(Mid_BishopMobility, 0) * z * z + P(Mid_BishopMobility, 1) * z);
		BISHOP_MOBILITY[m].end = EVAL(P(End_BishopMobility, 0) * z * z + P(End_BishopMobility, 1) * z);
	}

	for (int m = 2; m < 15; ++m)
	{
		double z = (m - 8) / 6.0;

		ROOK_MOBILITY[m].mid = EVAL(P(Mid_RookMobility, 0) * z * z + P(Mid_RookMobility, 1) * z);
		ROOK_MOBILITY[m].end = EVAL(P(End_RookMobility, 0) * z * z + P(End_RookMobility, 1) * z);
	}

	ROOK_OPEN = Pair(P(Mid_RookOpen, 0), P(End_RookOpen, 0));
	ROOK_7TH = Pair(P(Mid_Rook7th, 0), P(End_Rook7th, 0));

	QUEEN_7TH = Pair(P(Mid_Queen7th, 0), P(End_Queen7th, 0));

	for (int d = 0; d < 10; ++d)
	{
		double z = (d - 4.5) / 4.5;

		KNIGHT_KING_DIST[d].mid = EVAL(P(Mid_KnightKingDist, 0) * z * z + P(Mid_KnightKingDist, 1) * z);
		KNIGHT_KING_DIST[d].end = EVAL(P(End_KnightKingDist, 0) * z * z + P(End_KnightKingDist, 1) * z);
		BISHOP_KING_DIST[d].mid = EVAL(P(Mid_BishopKingDist, 0) * z * z + P(Mid_BishopKingDist, 1) * z);
		BISHOP_KING_DIST[d].end = EVAL(P(End_BishopKingDist, 0) * z * z + P(End_BishopKingDist, 1) * z);
		ROOK_KING_DIST[d].mid = EVAL(P(Mid_RookKingDist, 0) * z * z + P(Mid_RookKingDist, 1) * z);
		ROOK_KING_DIST[d].end = EVAL(P(End_RookKingDist, 0) * z * z + P(End_RookKingDist, 1) * z);
		QUEEN_KING_DIST[d].mid = EVAL(P(Mid_QueenKingDist, 0) * z * z + P(Mid_QueenKingDist, 1) * z);
		QUEEN_KING_DIST[d].end = EVAL(P(End_QueenKingDist, 0) * z * z + P(End_QueenKingDist, 1) * z);

		KING_PASSED_DIST[d].mid = EVAL(P(Mid_PawnPassedKingDist, 0) * z * z + P(Mid_PawnPassedKingDist, 1) * z);
		KING_PASSED_DIST[d].end = EVAL(P(End_PawnPassedKingDist, 0) * z * z + P(End_PawnPassedKingDist, 1) * z);
	}

	for (int p = 0; p < 10; ++p)
	{
		double z = p / 9.0;

		KING_PAWN_SHIELD[p].mid = EVAL(P(Mid_KingPawnShield, 0) * z * z + P(Mid_KingPawnShield, 1) * z);
		KING_PAWN_SHIELD[p].end = EVAL(P(End_KingPawnShield, 0) * z * z + P(End_KingPawnShield, 1) * z);

		KING_PAWN_STORM[p].mid = EVAL(P(Mid_KingPawnStorm, 0) * z * z + P(Mid_KingPawnStorm, 1) * z);
		KING_PAWN_STORM[p].end = EVAL(P(End_KingPawnStorm, 0) * z * z + P(End_KingPawnStorm, 1) * z);
	}

	for (int exposed = 0; exposed < 28; ++exposed)
	{
		double z = exposed / 28.0;

		KING_EXPOSED[exposed].mid = EVAL(P(Mid_KingExposed, 0) * z * z + P(Mid_KingExposed, 1) * z);
		KING_EXPOSED[exposed].end = EVAL(P(End_KingExposed, 0) * z * z + P(End_KingExposed, 1) * z);
	}

	ATTACK_STRONGER = Pair(P(Mid_AttackStronger, 0), P(End_AttackStronger, 0));
	ATTACK_CENTER = Pair(P(Mid_AttackCenter, 0), P(End_AttackCenter, 0));

	for (int att = 0; att < 8; ++att)
	{
		double z = (att - 3.5) / 3.5;

		ATTACK_KING[att].mid = EVAL(P(Mid_AttackKingZone, 0) * z * z + P(Mid_AttackKingZone, 1) * z);
		ATTACK_KING[att].end = EVAL(P(End_AttackKingZone, 0) * z * z + P(End_AttackKingZone, 1) * z);
	}

	TEMPO = Pair(P(Mid_Tempo, 0), P(End_Tempo, 0));
}
////////////////////////////////////////////////////////////////////////////////

void InitEval()
{
	InitParamLines();

	if (!ParamsFileVersionIsCorrect(WEIGHTS_FILE) || !ReadParamsFromFile(W, WEIGHTS_FILE))
	{
		SetDefaultValues(W);
		WriteParamsToFile(W, WEIGHTS_FILE);
	}

	InitEval(W);
}
////////////////////////////////////////////////////////////////////////////////

void PawnHashEntry::Read(const Position& pos)
{
	m_pawnHash = pos.PawnHash();

	m_passedPawns[WHITE] = m_passedPawns[BLACK] = 0;
	m_doubledPawns[WHITE] = m_doubledPawns[BLACK] = 0;
	m_isolatedPawns[WHITE] = m_isolatedPawns[BLACK] = 0;
	m_strongFields[WHITE] = m_strongFields[BLACK] = 0;

	for (int file = 0; file < 10; ++file)
	{
		m_ranks[file][WHITE] = 0;
		m_ranks[file][BLACK] = 7;
	}

	U64 x, y;
	FLD f;

	// first pass

	x = pos.Bits(PW);
	m_strongFields[WHITE] = UpRight(x) | UpLeft(x);

	while (x)
	{
		f = PopLSB(x);
		int file = Col(f) + 1;
		int rank = Row(f);
		if (rank > m_ranks[file][WHITE])
			m_ranks[file][WHITE] = rank;
	}

	x = pos.Bits(PB);
	m_strongFields[BLACK] = DownRight(x) | DownLeft(x);

	while (x)
	{
		f = PopLSB(x);
		int file = Col(f) + 1;
		int rank = Row(f);
		if (rank < m_ranks[file][BLACK])
			m_ranks[file][BLACK] = rank;
	}

	// second pass

	x = pos.Bits(PW);
	while (x)
	{
		f = PopLSB(x);
		int file = Col(f) + 1;
		int rank = Row(f);
		if (rank <= m_ranks[file][WHITE] && rank < m_ranks[file][BLACK])
		{
			if (rank <= m_ranks[file - 1][BLACK] && rank <= m_ranks[file + 1][BLACK])
				m_passedPawns[WHITE] |= BB_SINGLE[f];
		}
		if (rank != m_ranks[file][WHITE])
			m_doubledPawns[WHITE] |= BB_SINGLE[f];
		if (m_ranks[file - 1][WHITE] == 0 && m_ranks[file + 1][WHITE] == 0)
			m_isolatedPawns[WHITE] |= BB_SINGLE[f];

		y = BB_DIR[f][DIR_U];
		y = Left(y) | Right(y);
		m_strongFields[BLACK] &= ~y;
	}

	x = pos.Bits(PB);
	while (x)
	{
		f = PopLSB(x);
		int file = Col(f) + 1;
		int rank = Row(f);
		if (rank >= m_ranks[file][BLACK] && rank > m_ranks[file][WHITE])
		{
			if (rank >= m_ranks[file - 1][WHITE] && rank >= m_ranks[file + 1][WHITE])
				m_passedPawns[BLACK] |= BB_SINGLE[f];
		}
		if (rank != m_ranks[file][BLACK])
			m_doubledPawns[BLACK] |= BB_SINGLE[f];
		if (m_ranks[file - 1][BLACK] == 7 && m_ranks[file + 1][BLACK] == 7)
			m_isolatedPawns[BLACK] |= BB_SINGLE[f];

		y = BB_DIR[f][DIR_D];
		y = Left(y) | Right(y);
		m_strongFields[WHITE] &= ~y;
	}
}
////////////////////////////////////////////////////////////////////////////////
