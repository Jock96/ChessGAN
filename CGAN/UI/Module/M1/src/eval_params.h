//   GreKo chess engine
//   (c) 2002-2018 Vladimir Medvedev <vrm@bk.ru>
//   http://greko.su

#ifndef EVAL_PARAMS_H
#define EVAL_PARAMS_H

#include "types.h"

extern string WEIGHTS_FILE;
extern vector<int> W;

struct Line
{
	Line() : name(""), start(0), len(0) {}
	Line(const string& n, int l) : name(n), start(0), len(l) {}

	string name;
	size_t start;
	size_t len;
};

enum Param
{
	Mid_Pawn,
	Mid_PawnPassed,
	Mid_PawnPassedFree,
	Mid_PawnPassedBlocked,
	Mid_PawnPassedConnected,
	Mid_PawnPassedSquare,
	Mid_PawnPassedKingDist,
	Mid_PawnDoubledIsolated,
	Mid_PawnDoubled,
	Mid_PawnIsolated,
	Mid_PawnBlocked,
	Mid_PawnRam,
	Mid_PawnOnBishopColor,
	Mid_PawnZero,
	Mid_Knight,
	Mid_KnightPair,
	Mid_KnightStrong,
	Mid_KnightForpost,
	Mid_KnightKingDist,
	Mid_KnightAndQueen,
	Mid_Bishop,
	Mid_BishopPair,
	Mid_BishopStrong,
	Mid_BishopMobility,
	Mid_BishopKingDist,
	Mid_BishopAndRook,
	Mid_Rook,
	Mid_RookPair,
	Mid_RookMobility,
	Mid_RookOpen,
	Mid_Rook7th,
	Mid_RookConnected,
	Mid_RookKingDist,
	Mid_Queen,
	Mid_Queen7th,
	Mid_QueenKingDist,
	Mid_King,
	Mid_KingPawnShield,
	Mid_KingPawnStorm,
	Mid_KingExposed,
	Mid_AttackKingZone,
	Mid_AttackStronger,
	Mid_AttackCenter,
	Mid_Tempo,

	End_Pawn,
	End_PawnPassed,
	End_PawnPassedFree,
	End_PawnPassedBlocked,
	End_PawnPassedConnected,
	End_PawnPassedSquare,
	End_PawnPassedKingDist,
	End_PawnDoubledIsolated,
	End_PawnDoubled,
	End_PawnIsolated,
	End_PawnBlocked,
	End_PawnRam,
	End_PawnOnBishopColor,
	End_PawnZero,
	End_Knight,
	End_KnightPair,
	End_KnightStrong,
	End_KnightForpost,
	End_KnightKingDist,
	End_KnightAndQueen,
	End_Bishop,
	End_BishopPair,
	End_BishopStrong,
	End_BishopMobility,
	End_BishopKingDist,
	End_BishopAndRook,
	End_Rook,
	End_RookPair,
	End_RookMobility,
	End_RookOpen,
	End_Rook7th,
	End_RookConnected,
	End_RookKingDist,
	End_Queen,
	End_Queen7th,
	End_QueenKingDist,
	End_King,
	End_KingPawnShield,
	End_KingPawnStorm,
	End_KingExposed,
	End_AttackKingZone,
	End_AttackStronger,
	End_AttackCenter,
	End_Tempo,

	NUM_LINES
};

extern Line lines[NUM_LINES];
extern int NUM_PARAMS;

void InitParamLines();
std::string ParamNumberToName(size_t n);

void SetDefaultValues(vector<int>& x);
void SetMaterialOnlyValues(vector<int>& x);

bool ParamsFileVersionIsCorrect(const std::string& filename);
bool ReadParamsFromFile(vector<int>& x, const std::string& filename);
void WriteParamsToFile(const vector<int>& x, const std::string& filename);

#endif
