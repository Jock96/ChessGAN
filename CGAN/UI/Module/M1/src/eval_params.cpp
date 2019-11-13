//   GreKo chess engine
//   (c) 2002-2018 Vladimir Medvedev <vrm@bk.ru>
//   http://greko.su

#include "eval_params.h"
#include "notation.h"
#include "utils.h"

extern string PROGRAM_NAME;
extern string RELEASE_DATE;

vector<int> W;
int NUM_PARAMS = 0;

Line lines[NUM_LINES] =
{
	Line("Mid_Pawn", 6),
	Line("Mid_PawnPassed", 6),
	Line("Mid_PawnPassedFree", 6),
	Line("Mid_PawnPassedBlocked", 6),
	Line("Mid_PawnPassedConnected", 6),
	Line("Mid_PawnPassedSquare", 6),
	Line("Mid_PawnPassedKingDist", 2),
	Line("Mid_PawnDoubledIsolated", 6),
	Line("Mid_PawnDoubled", 6),
	Line("Mid_PawnIsolated", 6),
	Line("Mid_PawnBlocked", 6),
	Line("Mid_PawnRam", 6),
	Line("Mid_PawnOnBishopColor", 1),
	Line("Mid_PawnZero", 1),
	Line("Mid_Knight", 6),
	Line("Mid_KnightPair", 1),
	Line("Mid_KnightStrong", 6),
	Line("Mid_KnightForpost", 6),
	Line("Mid_KnightKingDist", 2),
	Line("Mid_KnightAndQueen", 1),
	Line("Mid_Bishop", 6),
	Line("Mid_BishopPair", 1),
	Line("Mid_BishopStrong", 6),
	Line("Mid_BishopMobility", 2),
	Line("Mid_BishopKingDist", 2),
	Line("Mid_BishopAndRook", 1),
	Line("Mid_Rook", 6),
	Line("Mid_RookPair", 1),
	Line("Mid_RookMobility", 2),
	Line("Mid_RookOpen", 1),
	Line("Mid_Rook7th", 1),
	Line("Mid_RookConnected", 1),
	Line("Mid_RookKingDist", 2),
	Line("Mid_Queen", 6),
	Line("Mid_Queen7th", 1),
	Line("Mid_QueenKingDist", 2),
	Line("Mid_King", 6),
	Line("Mid_KingPawnShield", 2),
	Line("Mid_KingPawnStorm", 2),
	Line("Mid_KingExposed", 2),
	Line("Mid_AttackKing", 2),
	Line("Mid_AttackStronger", 1),
	Line("Mid_AttackCenter", 1),
	Line("Mid_Tempo", 1),

	Line("End_Pawn", 6),
	Line("End_PawnPassed", 6),
	Line("End_PawnPassedFree", 6),
	Line("End_PawnPassedBlocked", 6),
	Line("End_PawnPassedConnected", 6),
	Line("End_PawnPassedSquare", 6),
	Line("End_PawnPassedKingDist", 2),
	Line("End_PawnDoubledIsolated", 6),
	Line("End_PawnDoubled", 6),
	Line("End_PawnIsolated", 6),
	Line("End_PawnBlocked", 6),
	Line("End_PawnRam", 6),
	Line("End_PawnOnBishopColor", 1),
	Line("End_PawnZero", 1),
	Line("End_Knight", 6),
	Line("End_KnightPair", 1),
	Line("End_KnightStrong", 6),
	Line("End_KnightForpost", 6),
	Line("End_KnightKingDist", 2),
	Line("End_KnightAndQueen", 1),
	Line("End_Bishop", 6),
	Line("End_BishopPair", 1),
	Line("End_BishopStrong", 6),
	Line("End_BishopMobility", 2),
	Line("End_BishopKingDist", 2),
	Line("End_BishopAndRook", 1),
	Line("End_Rook", 6),
	Line("End_RookPair", 1),
	Line("End_RookMobility", 2),
	Line("End_RookOpen", 1),
	Line("End_Rook7th", 1),
	Line("End_RookConnected", 1),
	Line("End_RookKingDist", 2),
	Line("End_Queen", 6),
	Line("End_Queen7th", 1),
	Line("End_QueenKingDist", 2),
	Line("End_King", 6),
	Line("End_KingPawnShield", 2),
	Line("End_KingPawnStorm", 2),
	Line("End_KingExposed", 2),
	Line("End_AttackKing", 2),
	Line("End_AttackStronger", 1),
	Line("End_AttackCenter", 1),
	Line("End_Tempo", 1)
};

void InitParamLines()
{
	for (int i = 1; i < NUM_LINES; ++i)
		lines[i].start = lines[i - 1].start + lines[i - 1].len;
	NUM_PARAMS = lines[NUM_LINES - 1].start + lines[NUM_LINES - 1].len;
}
////////////////////////////////////////////////////////////////////////////////

void SetMaterialOnlyValues(vector<int>& x)
{
	x.resize(NUM_PARAMS);
}
////////////////////////////////////////////////////////////////////////////////

void SetDefaultValues(vector<int>& x)
{
	x.resize(NUM_PARAMS);
	static const int data[300] =
	{
		-27, 9, -14, -3, -11, -38, 17, -4, 61, 33, 8, 3, 7, -5, 53, 33, -5, -20, 6, 10, 32, 11, -3, -13, -2, 2, 83, 41, 11, 7, 38, -25, 29, 3, 3, -31, -18, -30, -11, 11, 21, 45, -10, -2, 34, -6, -36, 37, -54, 2,
		-5, 3, 9, 13, -5, -8, -13, 9, -8, 3, -1, -6, -3, 2, -20, -2, 6, 1, -3, -11, -18, -14, -30, -1, -21, -130, -5, 1, 0, -66, 15, 30, 29, 35, -14, 40, 16, 14, -5, -2, -30, -6, -10, 3, -21, -2, 0, -135, -1, 7,
		-11, -3, 19, -13, 26, -54, 20, 33, -9, 8, -12, -1, 43, 29, -2, -283, -14, 0, 23, 22, -8, -5, 30, -38, -13, 16, -3, 0, -6, -476, -16, -3, -20, -26, 6, -19, -29, -28, 13, -45, -13, -80, -81, -24, -22, -4, 7, 42, 3, 24,
		-7, -3, 11, 25, -10, -10, 10, -1, -13, 52, -12, 28, 8, 12, 21, 33, 2, 11, -15, 11, -70, -6, -25, -1, -14, 7, 2, 35, -6, 9, 7, 7, 73, 70, 1, 9, -18, 35, -9, -1, 8, -14, 2, -18, 1, -11, -11, 14, 5, -2,
		0, -2, 2, -9, 8, -8, 18, -5, -4, 2, -2, -6, -2, 1, 14, -17, 31, 4, -4, 11, -11, -27, -42, 23, -13, -126, 10, -34, 27, -17, 14, 16, 1, -47, 6, -17, -58, 14, 15, -24, 5, 16, 16, -28, -2, 26, -31, -114, 31, 28,
		-33, -2, -48, 48, 20, -2, 35, -14, 6, -3, 14, -3, -12, 14, 0, -127, -8, -31, 30, -1, 26, 2, -5, 19, -4, -30, 0, -1, 22, -329, 25, 32, -139, -39, -2, -45, 27, -9, 6, -3, -8, -18, 40, -59, -6, 8, 17, 46, 1, 8
	};
	memcpy(&(x[0]), data, 300 * sizeof(int));
}
////////////////////////////////////////////////////////////////////////////////

std::string ParamNumberToName(size_t n)
{
	for (size_t i = 0; i < NUM_LINES; ++i)
	{
		if (n >= lines[i].start && n < lines[i].start + lines[i].len)
		{
			stringstream ss;
			ss << lines[i].name << "[" << n - lines[i].start + 1 << "]";
			return ss.str();
		}
	}
	return "unknown";
}
////////////////////////////////////////////////////////////////////////////////

bool ParamsFileVersionIsCorrect(const std::string& filename)
{
	ifstream ifs(filename.c_str());
	if (!ifs.good())
		return false;

	string s;
	getline(ifs, s);
	return (s.find(PROGRAM_NAME) != string::npos);
}
////////////////////////////////////////////////////////////////////////////////

bool ReadParamsFromFile(vector<int>& x, const std::string& filename)
{
	x.resize(NUM_PARAMS);

	ifstream ifs(filename.c_str());
	if (!ifs.good())
		return false;

	string s;
	while (getline(ifs, s))
	{
		vector<string> tokens;
		Split(s, tokens, " ");
		if (tokens.size() < 2)
			continue;

		for (int i = 0; i < NUM_LINES; ++i)
		{
			const Line& line = lines[i];
			if (tokens[0] == line.name && tokens.size() >= line.len + 1)
			{
				for (size_t j = 0; j < line.len; ++j)
					x[line.start + j] = atoi(tokens[1 + j].c_str());
			}
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////

void WriteParamsToFile(const vector<int>& x, const std::string& filename)
{
	ofstream ofs(filename.c_str());
	if (!ofs.good())
		return;

	ofs << PROGRAM_NAME << " evaluation weights file" << endl << endl;

	for (int i = 0; i < NUM_LINES; ++i)
	{
		const Line& line = lines[i];
		ofs << line.name;
		for (size_t j = line.start; j < line.start + line.len; ++j)
			ofs << " " << x[j];
		ofs << endl;
	}
}
////////////////////////////////////////////////////////////////////////////////
