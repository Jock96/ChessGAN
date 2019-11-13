//   GreKo chess engine
//   (c) 2002-2018 Vladimir Medvedev <vrm@bk.ru>
//   http://greko.su

#include "eval.h"
#include "learn.h"
#include "moves.h"
#include "notation.h"
#include "search.h"
#include "utils.h"
#include <winsock.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

string PROGRAM_NAME = "GreKo 2018.08";
string RELEASE_DATE = "22-Aug-2018";

string WEIGHTS_FILE = "weights.txt";

const int MIN_HASH_SIZE = 1;
const int MAX_HASH_SIZE = 1024;
const int DEFAULT_HASH_SIZE = 128;

extern Pair PSQ[14][64];
extern Pair PSQ_PP[64];
extern Pair PSQ_PP_BLOCKED[64];
extern Pair PSQ_PP_CONNECTED[64];
extern Pair PSQ_PP_SQUARE[64];

Position g_pos;
deque<string> g_queue;
FILE* g_log = NULL;

bool g_console = false;
bool g_xboard = false;
bool g_uci = true;

static string g_s;
static vector<string> g_tokens;
static bool g_force = false;

void OnZero();

void ComputeTimeLimits()
{
	if (g_sd > 0 || g_sn > 0)
		return;
	if (g_restTime == 0)
		return;

	if (g_inc > 0)
	{
		// time control with increment
		g_stHard = g_restTime / 2;
		g_stSoft = g_restTime / 40;
	}
	else if (g_restMoves == 0 && g_movesPerSession == 0)
	{
		// absolute time control
		g_stHard = g_restTime / 40;
		g_stSoft = g_restTime / 40;
	}
	else
	{
		// N moves in M minutes
		if (g_movesPerSession > 0)
		{
			int curr = g_pos.Ply() / 2 + 1;
			g_restMoves = g_movesPerSession - ((curr - 1) % g_movesPerSession);
		}
		int divisor = (g_restMoves > 1)? g_restMoves : 2;
		g_stHard = g_restTime / divisor;
		g_stSoft = g_stHard / 2;
	}

	stringstream ss;
	ss << "TIME: rest = " << g_restTime <<
		", moves = " << g_restMoves <<
		", inc = " << g_inc <<
		" ==> stHard = " << g_stHard << ", stSoft = " << g_stSoft;
	Log(ss.str());
}
////////////////////////////////////////////////////////////////////////////////

void OnAnalyze()
{
	g_sd = 0;
	g_sn = 0;
	g_stHard = 0;
	g_stSoft = 0;

	ClearHash();
	ClearHistory();
	ClearKillers();

	Position pos = g_pos;
	StartSearch(pos, MODE_ANALYZE);
}
////////////////////////////////////////////////////////////////////////////////

void OnDump()
{
	ofstream ofs("default_params.cpp");
	if (!ofs.good())
		return;
	ofs << "\tstatic const int data[" << NUM_PARAMS << "] =" << endl;
	ofs << "\t{";
	for (int i = 0; i < NUM_PARAMS; ++i)
	{
		if (i % 50 == 0)
			ofs << endl << "\t\t";
		ofs << W[i];
		if (i < NUM_PARAMS - 1)
		{
			ofs << ",";
			if (i % 50 != 49)
				ofs << " ";
		}
	}
	ofs << endl << "\t};" << endl;
}
////////////////////////////////////////////////////////////////////////////////

void OnEval()
{
	stringstream ss;
	ss << Evaluate(g_pos, -INFINITY_SCORE, INFINITY_SCORE) << endl;
	out(ss);
}
////////////////////////////////////////////////////////////////////////////////

void OnFEN()
{
	stringstream ss;
	ss << g_pos.FEN() << endl;
	out(ss);
}
////////////////////////////////////////////////////////////////////////////////

void OnFlip()
{
	g_pos.Mirror();
	g_pos.Print();
}
////////////////////////////////////////////////////////////////////////////////

void OnGoUci()
{
	U8 mode = MODE_PLAY;

	g_stHard = 0;
	g_stSoft = 0;
	g_inc = 0;
	g_sd = 0;
	g_sn = 0;
	g_restTime = 0;
	g_restMoves = 0;
	g_movesPerSession = 0;

	for (size_t i = 1; i < g_tokens.size(); ++i)
	{
		string token = g_tokens[i];
		if (token == "infinite")
		{
			mode = MODE_ANALYZE;
		}
		else if (i < g_tokens.size() - 1)
		{
			if (token == "movetime")
			{
				int t = atoi(g_tokens[i + 1].c_str());
				g_stHard = t;
				g_stSoft = t;
				++i;
			}
			else if ((token == "wtime" && g_pos.Side() == WHITE) || (token == "btime" && g_pos.Side() == BLACK))
			{
				g_restTime = atoi(g_tokens[i + 1].c_str());
				++i;
			}
			else if ((token == "winc" && g_pos.Side() == WHITE) || (token == "binc" && g_pos.Side() == BLACK))
			{
				g_inc = atoi(g_tokens[i + 1].c_str());
				++i;
			}
			else if (token == "movestogo")
			{
				g_restMoves = atoi(g_tokens[i + 1].c_str());
				++i;
			}
			else if (token == "depth")
			{
				g_sd = atoi(g_tokens[i + 1].c_str());
				++i;
			}
			else if (token == "nodes")
			{
				g_sn = atoi(g_tokens[i + 1].c_str());
				++i;
			}
		}
	}

	ComputeTimeLimits();
	Move mv = StartSearch(g_pos, mode);

	if (mv != 0)
	{
		stringstream ss;
		ss << "bestmove " << MoveToStrLong(mv);
		out(ss);
	}
}
////////////////////////////////////////////////////////////////////////////////

void OnGo()
{
	if (g_uci)
	{
		OnGoUci();
		return;
	}

	g_force = false;

	string result, comment;
	if (IsGameOver(g_pos, result, comment))
	{
		stringstream ss;
		ss << result << " " << comment << endl;
		out(ss);
		return;
	}

	ComputeTimeLimits();
	Move mv = StartSearch(g_pos, MODE_PLAY);

	if (mv != 0)
	{
		Highlight(true);
		stringstream ss;
		ss << "move " << MoveToStrLong(mv);
		
		string output = MoveToStrLong(mv);

		std::ofstream outF;
		outF.open("Output(Main).txt");

		if (outF.is_open())
		{
			outF.close();

			remove("Output(Main).txt");
			std::ofstream{ "Output(Module).txt" };

			outF.open("Output(Module).txt");

			if (outF.is_open())
				outF << output << std::endl;
		}

		/*outF.close();*/

		out(ss);
		Highlight(false);

		g_pos.MakeMove(mv);

		if (IsGameOver(g_pos, result, comment))
			out(result + " " + comment);
	}
}
////////////////////////////////////////////////////////////////////////////////

void OnIsready()
{
	out("readyok");
}
////////////////////////////////////////////////////////////////////////////////

void OnLearn(string fileName, int alg, int param2)
{
	string pgnFile, fenFile;

	if (fileName.find(".pgn") != string::npos)
	{
		pgnFile = fileName;
		size_t extPos = fileName.find(".pgn");
		if (extPos != string::npos)
			fenFile = fileName.substr(0, extPos) + ".fen";
	}
	else if (fileName.find(".fen") != string::npos)
	{
		fenFile = fileName;
		size_t extPos = fileName.find(".fen");
		if (extPos != string::npos)
			pgnFile = fileName.substr(0, extPos) + ".pgn";
	}
	else
	{
		pgnFile = fileName + ".pgn";
		fenFile = fileName + ".fen";
	}

	{
		ifstream test(fenFile.c_str());
		if (!test.good())
		{
			if (!PgnToFen(pgnFile, fenFile, 6, 999, 1))
				return;
		}
	}

	vector<int> params;
	ifstream f1("learn_params.txt");
	if (f1.good())
	{
		string s;
		while(getline(f1, s))
		{
			vector<string> tokens;
			Split(s, tokens);
			if (tokens.empty()) continue;
			string name = tokens[0];
			for (size_t i = 0; i < NUM_LINES; ++i)
			{
				if (name == lines[i].name)
				{
					for (size_t j = 0; j < lines[i].len; ++j)
						params.push_back(lines[i].start + j);
				}
			}
		}
	}

	if (params.empty())
	{
		for (int i = 0; i < NUM_PARAMS; ++i)
			params.push_back(i);
	}

	vector<int> x0 = W;
	SetStartLearnTime();

	if (alg == COORDINATE_DESCENT)
		CoordinateDescent(fenFile, x0, params, param2);
	else if (alg == RANDOM_WALK)
		RandomWalk(fenFile, x0, param2, false, params);
	else if (alg == SIMULATED_ANNEALING)
		RandomWalk(fenFile, x0, param2, true, params);
	else
	{
		stringstream ss;
		ss << "Unknown algorithm: " << alg << endl;
		out(ss);
	}
}
////////////////////////////////////////////////////////////////////////////////

void OnLearn()
{
	if (g_tokens.size() < 2)
		return;

	string fileName = g_tokens[1];
	int alg = (g_tokens.size() > 2)? atoi(g_tokens[2].c_str()) : COORDINATE_DESCENT;
	int param2 = 0;
	if (g_tokens.size() > 3)
		param2 = atoi(g_tokens[3].c_str());
	else
		param2 = (alg == COORDINATE_DESCENT)? 6 : 600;

	OnLearn(fileName, alg, param2);
}
////////////////////////////////////////////////////////////////////////////////

void OnLevel()
{
	if (g_tokens.size() > 1)
		g_movesPerSession = atoi(g_tokens[1].c_str());
	if (g_tokens.size() > 3)
		g_inc = 1000 * atoi(g_tokens[3].c_str());
}
////////////////////////////////////////////////////////////////////////////////

void OnList()
{
	MoveList mvlist;
	GenAllMoves(g_pos, mvlist);

	stringstream ss;
	for (size_t i = 0; i < mvlist.Size(); ++i)
	{
		Move mv = mvlist[i].m_mv;
		ss << MoveToStrLong(mv) << " ";
	}
	ss << " -- total: " << mvlist.Size() << endl;
	out(ss);
}
////////////////////////////////////////////////////////////////////////////////

void OnLoad()
{
	if (g_tokens.size() < 2)
		return;

	ifstream ifs(g_tokens[1].c_str());
	if (!ifs.good())
	{
		out("Can't open file: " + g_tokens[1]);
		return;
	}

	int line = 1;
	if (g_tokens.size() > 2)
	{
		line = atoi(g_tokens[2].c_str());
		if (line <= 0)
			line = 1;
	}

	string fen;
	for (int i = 0; i < line; ++i)
	{
		if(!getline(ifs, fen)) break;
	}

	if (g_pos.SetFEN(fen))
	{
		g_pos.Print();
		cout << fen << endl << endl;
	}
	else
		out("Illegal FEN");
}
////////////////////////////////////////////////////////////////////////////////

void OnMT()
{
	if (g_tokens.size() < 2)
		return;

	ifstream ifs(g_tokens[1].c_str());
	if (!ifs.good())
	{
		out("Can't open file: " + g_tokens[1]);
		return;
	}

	string s;
	while (getline(ifs, s))
	{
		Position pos;
		if (pos.SetFEN(s))
		{
			out(s);
			EVAL e1 = Evaluate(pos, -INFINITY_SCORE, INFINITY_SCORE);
			pos.Mirror();
			EVAL e2 = Evaluate(pos, -INFINITY_SCORE, INFINITY_SCORE);
			if (e1 != e2)
			{
				pos.Mirror();
				pos.Print();
				cout << "e1 = " << e1 << ", e2 = " << e2 << endl << endl;
				return;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

void OnNew()
{
	g_force = false;
	g_pos.SetInitial();

	ClearHash();
	ClearHistory();
	ClearKillers();
}
////////////////////////////////////////////////////////////////////////////////

void OnPerft()
{
	if (g_tokens.size() < 2)
		return;
	int depth = atoi(g_tokens[1].c_str());
	StartPerft(g_pos, depth);
}
////////////////////////////////////////////////////////////////////////////////

void OnPgnToFen()
{
	if (g_tokens.size() < 3)
		return;

	string pgnFile = g_tokens[1];
	string fenFile = g_tokens[2];
	int fensPerGame = 1;
	if (g_tokens.size() >= 4)
		fensPerGame = atoi(g_tokens[3].c_str());
	PgnToFen(pgnFile, fenFile, 6, 999, fensPerGame);
}
////////////////////////////////////////////////////////////////////////////////

void OnPing()
{
	if (g_tokens.size() > 1)
		out("pong " + g_tokens[1]);
}
////////////////////////////////////////////////////////////////////////////////

void OnPosition()
{
	if (g_tokens.size() < 2)
		return;

	size_t movesTag = 0;
	if (g_tokens[1] == "fen")
	{
		string fen = "";
		for (size_t i = 2; i < g_tokens.size(); ++i)
		{
			if (g_tokens[i] == "moves")
			{
				movesTag = i;
				break;
			}
			if (!fen.empty())
				fen += " ";
			fen += g_tokens[i];
		}
		g_pos.SetFEN(fen);
	}
	else if (g_tokens[1] == "startpos")
	{
		g_pos.SetInitial();
		for (size_t i = 2; i < g_tokens.size(); ++i)
		{
			if (g_tokens[i] == "moves")
			{
				movesTag = i;
				break;
			}
		}
	}

	if (movesTag)
	{
		for (size_t i = movesTag + 1; i < g_tokens.size(); ++i)
		{
			Move mv = StrToMove(g_tokens[i], g_pos);
			g_pos.MakeMove(mv);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

void OnPredict()
{
	if (g_tokens.size() < 2)
		return;

	string file = g_tokens[1];
	if (file.find(".fen") == string::npos)
		file += ".fen";

	stringstream ss;
	ss << setprecision(10) << Predict(file) << endl;
	out(ss);
}
////////////////////////////////////////////////////////////////////////////////

void OnProtover()
{
	stringstream ss;
	ss << "feature myname=\"" << PROGRAM_NAME << "\" setboard=1 analyze=1 colors=0 san=0 ping=1 name=1 done=1";
	out(ss);
}
////////////////////////////////////////////////////////////////////////////////

void OnPSQ()
{
	if (g_tokens.size() < 2)
		return;

	EVAL mid_w = 0;
	EVAL end_w = 0;
	Pair* table = NULL;

	if (g_tokens[1] == "P" || g_tokens[1] == "p")
	{
		table = PSQ[PW];
		mid_w = VAL_P;
		end_w = VAL_P;
	}
	else if (g_tokens[1] == "PP" || g_tokens[1] == "pp")
	{
		table = PSQ_PP;
		mid_w = 0;
		end_w = 0;
	}
	else if (g_tokens[1] == "PPB" || g_tokens[1] == "ppb")
	{
		table = PSQ_PP_BLOCKED;
		mid_w = 0;
		end_w = 0;
	}
	else if (g_tokens[1] == "PPC" || g_tokens[1] == "ppc")
	{
		table = PSQ_PP_CONNECTED;
		mid_w = 0;
		end_w = 0;
	}
	else if (g_tokens[1] == "PPS" || g_tokens[1] == "pps")
	{
		table = PSQ_PP_SQUARE;
		mid_w = 0;
		end_w = 0;
	}
	else if (g_tokens[1] == "N" || g_tokens[1] == "n")
	{
		table = PSQ[NW];
		mid_w = VAL_N;
		end_w = VAL_N;
	}
	else if (g_tokens[1] == "B" || g_tokens[1] == "b")
	{
		table = PSQ[BW];
		mid_w = VAL_B;
		end_w = VAL_B;
	}
	else if (g_tokens[1] == "R" || g_tokens[1] == "r")
	{
		table = PSQ[RW];
		mid_w = VAL_R;
		end_w = VAL_R;
	}
	else if (g_tokens[1] == "Q" || g_tokens[1] == "q")
	{
		table = PSQ[QW];
		mid_w = VAL_Q;
		end_w = VAL_Q;
	}
	else if (g_tokens[1] == "K" || g_tokens[1] == "k")
	{
		table = PSQ[KW];
		mid_w = VAL_K;
		end_w = VAL_K;
	}

	double sum_mid = 0, sum_end = 0;
	if (table != NULL)
	{
		cout << endl << "Midgame:" << endl << endl;
		for (FLD f = 0; f < 64; ++f)
		{
			cout << setw(4) << (table[f].mid - mid_w);
			sum_mid += table[f].mid;

			if (f < 63)
				cout << ", ";
			if (Col(f) == 7)
				cout << endl;
		}

		cout << endl << "Endgame:" << endl << endl;
		for (FLD f = 0; f < 64; ++f)
		{
			cout << setw(4) << (table[f].end - end_w);
			sum_end += table[f].end;

			if (f < 63)
				cout << ", ";
			if (Col(f) == 7)
				cout << endl;
		}
		cout << endl;
		cout << "avg_mid = " << sum_mid / 64. << ", avg_end = " << sum_end / 64. << endl;
		cout << endl;
	}
}
////////////////////////////////////////////////////////////////////////////////

void OnSD()
{
	if (g_tokens.size() < 2)
		return;

	g_sd = atoi(g_tokens[1].c_str());
	g_sn = 0;
	g_stHard = 0;
	g_stSoft = 0;
}
////////////////////////////////////////////////////////////////////////////////

void OnSelfPlay(int N)
{
	cout <<
		"Start playing " << N <<
		" games (sd = " << g_sd <<
		", stHard = " << g_stHard <<
		", stSoft = " << g_stSoft <<
		")" << endl;

	RandSeed(time(0));

	if (g_tokens.size() > 1)
		N = atoi(g_tokens[1].c_str());

	for (int games = 0; games < N; ++games)
	{
		Position pos;
		pos.SetInitial();

		string result, comment;
		string pgn, line;

		while (!IsGameOver(pos, result, comment))
		{
			Move mv = (pos.Ply() < 6)? GetRandomMove(pos) : StartSearch(pos, MODE_PLAY | MODE_SILENT);

			if (mv == 0)
			{
				result = "1/2-1/2";
				comment = "{Adjudication: zero move generated}";
				break;
			}

			MoveList mvlist;
			GenAllMoves(pos, mvlist);
			if (pos.Side() == WHITE)
			{
				stringstream ss;
				ss << pos.Ply() / 2 + 1 << ". ";
				line = line + ss.str();
			}
			line = line + MoveToStrShort(mv, pos, mvlist) + string(" ");

			pos.MakeMove(mv);

			if (line.length() > 60 && pos.Side() == WHITE)
			{
				pgn = pgn + "\n" + line;
				line.clear();
			}

			if (pos.Ply() > 600)
			{
				result = "1/2-1/2";
				comment = "{Adjudication: too long}";
				break;
			}
		}

		stringstream header;
		header << "[Event \"Self-play\"]" << endl <<
			"[Date \"" << CurrentDateStr() << "\"]" << endl <<
			"[White \"" << PROGRAM_NAME << "\"]" << endl <<
			"[Black \"" << PROGRAM_NAME << "\"]" << endl <<
			"[Result \"" << result << "\"]" << endl;

		if (!line.empty())
		{
			pgn = pgn + "\n" + line;
			line.clear();
		}

		std::ofstream ofs;
		ofs.open("games.pgn", ios::app);
		if (ofs.good())
			ofs << header.str() << pgn << endl << result << " " << comment << endl << endl;

		cout << "Playing games: " << games + 1 << " of " << N << "\r";
	}
	cout << endl;
}
////////////////////////////////////////////////////////////////////////////////

void OnSelfPlay()
{
	if (g_tokens.size() < 2)
		return;
	int numGames = atoi(g_tokens[1].c_str());

	OnSelfPlay(numGames);
}
////////////////////////////////////////////////////////////////////////////////

void OnSetboard()
{
	if (g_pos.SetFEN(g_s.c_str() + 9))
		g_pos.Print();
	else
		out("Illegal FEN");
}
////////////////////////////////////////////////////////////////////////////////

void OnSetoption()
{
	if (g_tokens.size() < 5)
		return;
	if (g_tokens[1] != "name" && g_tokens[3] != "value")
		return;

	string name = g_tokens[2];
	string value = g_tokens[4];

	if (name == "Hash")
		SetHashSize(atoi(value.c_str()));
	else if (name == "Strength")
		SetStrength(atoi(value.c_str()));
}
////////////////////////////////////////////////////////////////////////////////

void OnSN()
{
	if (g_tokens.size() < 2)
		return;

	g_sd = 0;
	g_sn = atoi(g_tokens[1].c_str());
	g_stHard = 0;
	g_stSoft = 0;
}
////////////////////////////////////////////////////////////////////////////////

void OnST()
{
	if (g_tokens.size() < 2)
		return;

	g_sd = 0;
	g_sn = 0;
	g_stHard = U32(1000 * atof(g_tokens[1].c_str()));
	g_stSoft = U32(1000 * atof(g_tokens[1].c_str()));
}
////////////////////////////////////////////////////////////////////////////////

void OnTest()
{
	g_pos.SetFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
}
////////////////////////////////////////////////////////////////////////////////

void OnTime()
{
	if (g_tokens.size() < 2)
		return;

	g_sd = 0;
	g_sn = 0;

	g_restTime = 10 * atoi(g_tokens[1].c_str());
}
////////////////////////////////////////////////////////////////////////////////

void OnTraining(int numGames, int numEpochs, int numIters)
{
	const string pgn_file = "games.pgn";
	const string fen_file = "games.fen";

	RandSeed(time(0));

	stringstream ss;
	ss << "weights_" << CountGames(pgn_file) << ".txt";
	WriteParamsToFile(W, ss.str());
	cout << "Weights saved in " << ss.str() << endl;

	for (int epoch = 0; epoch < numEpochs; ++epoch)
	{
		cout << endl << "*** EPOCH " << epoch + 1 << " of " << numEpochs << " ***" << endl;

		OnSelfPlay(numGames);
		PgnToFen(pgn_file, fen_file, 6, 999, 1);

		OnLearn(fen_file, 1, numIters);

		stringstream ss;
		ss << "weights_" << CountGames(pgn_file) << ".txt";
		WriteParamsToFile(W, ss.str());
		cout << "Weights saved in " << ss.str() << endl;
	}
}
////////////////////////////////////////////////////////////////////////////////

void OnTraining()
{
	int numGames = (g_tokens.size() > 1)? atoi(g_tokens[1].c_str()) : 10000;
	int numEpochs = (g_tokens.size() > 2)? atoi(g_tokens[2].c_str()) : 1;
	int numIters = (g_tokens.size() > 3)? atoi(g_tokens[3].c_str()) : 2;

	OnTraining(numGames, numEpochs, numIters);
}
////////////////////////////////////////////////////////////////////////////////

void OnUCI()
{
	g_console = false;
	g_xboard = false;
	g_uci = true;

	out("id name " + PROGRAM_NAME);
	out("id author Vladimir Medvedev");

	stringstream ss1;
	ss1 << "option name Hash type spin" <<
		" default " << DEFAULT_HASH_SIZE <<
		" min " << MIN_HASH_SIZE <<
		" max " << MAX_HASH_SIZE;
	out(ss1);

	stringstream ss2;
	ss2 << "option name Strength type spin" <<
		" default " << 100 <<
		" min " << 0 <<
		" max " << 100;
	out(ss2);

	out("uciok");
}
////////////////////////////////////////////////////////////////////////////////

void OnXboard()
{
	g_console = false;
	g_xboard = true;
	g_uci = false;

	cout << endl;
}
////////////////////////////////////////////////////////////////////////////////

void OnZero()
{
	std::vector<int> x;
	x.resize(NUM_PARAMS);
	if (g_tokens.size() > 1 && g_tokens[1].find("rand") == 0)
	{
		for (int i = 0; i < NUM_PARAMS; ++i)
			x[i] = Rand32() % 7 - 3;
	}
	else if (g_tokens.size() > 1 && g_tokens[1].find("mat") == 0)
	{
		SetMaterialOnlyValues(x);
	}
	WriteParamsToFile(x, "weights.txt");
	InitEval(x);
}
////////////////////////////////////////////////////////////////////////////////

SOCKET s; //Socket handle

//Send text data to a remote computer
//int PASCAL send(SOCKET, const char*, int, int);

//CONNECTTOHOST – Connects to a remote host
bool ConnectToHost(int PortNo, char* IPAddress)
{
	//Start up Winsock…
	WSADATA wsadata;

	int error = WSAStartup(0x0202, &wsadata);

	//Did something happen?
	if (error)
		return false;

	//Did we get the right Winsock version?
	if (wsadata.wVersion != 0x0202)
	{
		WSACleanup(); //Clean up Winsock
		return false;
	}

	//Fill out the information needed to initialize a socket…
	SOCKADDR_IN target; //Socket address information

	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons(PortNo); //Port to connect on
	target.sin_addr.s_addr = inet_addr(IPAddress); //Target IP

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (s == INVALID_SOCKET)
	{
		return false; //Couldn't create the socket
	}

	//Try connecting...

	if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		return false; //Couldn't connect
	}
	else
		return true; //Success
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void CloseConnection()
{
	//Close the socket if it exists
	if (s)
		closesocket(s);

	WSACleanup(); //Clean up Winsock
}

void send_int(SOCKET s, int val)
{
	char val_char[4];
	*((int*)val_char) = val;
	send(s, val_char, sizeof(int), 0);
}

void send_str(SOCKET s, const char * str)
{
	send_int(s, strlen(str));
	send(s, str, strlen(str), 0);
}

//bool TryWriteOutput(string output)
//{
//	std::ofstream out;
//	out.open("ModuleOutput.txt");
//	if (out.is_open())
//	{
//		out << output << std::endl;
//		out.close();
//		return true;
//	}
//
//	return false;
//}

void RunCommandLine()
{
	while (1)
	{
		string line;
		std::ifstream inF("Input(Main).txt");

		if (!inF.is_open())
			continue;
		else
		{
			getline(inF, line);

			inF.close();
			remove("Input(Main).txt");

			std::ofstream{ "Input(Module).txt" };
		}

		if (!g_queue.empty())
		{
			g_s = g_queue.front();
			g_queue.pop_front();
		}
		else
		{
			if (g_console)
			{
				if (g_pos.Side() == WHITE)
				{
					cout << "White(" << g_pos.Ply() / 2 + 1 << "): ";
				}
				else
				{
					cout << "Black(" << g_pos.Ply() / 2 + 1 << "): ";
				}
			}
			/*getline(cin, g_s);*/
			g_s = line;
		}
		
		if (g_s.empty())
			continue;

		Log(string("> ") + g_s);

		Split(g_s, g_tokens);
		if (g_tokens.empty())
			continue;

		string cmd = g_tokens[0];

#define ON_CMD(pattern, minLen, action) \
    if (Is(cmd, #pattern, minLen))      \
    {                                   \
      action;                           \
      continue;                         \
    }

		ON_CMD(analyze,    1, OnAnalyze())
		ON_CMD(board,      1, g_pos.Print())
		ON_CMD(dump,       1, OnDump())
		ON_CMD(eval,       2, OnEval())
		ON_CMD(fen,        2, OnFEN())
		ON_CMD(flip,       2, OnFlip())
		ON_CMD(force,      2, g_force = true)
		ON_CMD(go,         1, OnGo())
		ON_CMD(isready,    1, OnIsready())
		ON_CMD(learn,      3, OnLearn())
		ON_CMD(level,      3, OnLevel())
		ON_CMD(list,       2, OnList())
		ON_CMD(load,       2, OnLoad())
		ON_CMD(mt,         2, OnMT())
		ON_CMD(new,        1, OnNew())
		ON_CMD(perft,      2, OnPerft())
		ON_CMD(pgntofen,   2, OnPgnToFen())
		ON_CMD(ping,       2, OnPing())
		ON_CMD(position,   2, OnPosition())
		ON_CMD(predict,    3, OnPredict())
		ON_CMD(protover,   3, OnProtover())
		ON_CMD(psq,        2, OnPSQ())
		ON_CMD(quit,       1, break)
		ON_CMD(sd,         2, OnSD())
		ON_CMD(selfplay,   4, OnSelfPlay())
		ON_CMD(sp,         2, OnSelfPlay())
		ON_CMD(setboard,   3, OnSetboard())
		ON_CMD(setoption,  3, OnSetoption())
		ON_CMD(sn,         2, OnSN())
		ON_CMD(st,         2, OnST())
		ON_CMD(test,       2, OnTest())
		ON_CMD(time,       2, OnTime())
		ON_CMD(training,   2, OnTraining())
		ON_CMD(uci,        1, OnUCI())
		ON_CMD(ucinewgame, 4, OnNew())
		ON_CMD(undo,       1, g_pos.UnmakeMove())
		ON_CMD(xboard,     1, OnXboard())
		ON_CMD(zero,       1, OnZero())
#undef ON_CMD

		if (CanBeMove(cmd))
		{
			Move mv = StrToMove(cmd, g_pos);
			if (mv)
			{
				g_pos.MakeMove(mv);
				string result, comment;
				if (IsGameOver(g_pos, result, comment))
				{
					cout << result << " " << comment << endl << endl;
				}
				else if (!g_force)
				{
					OnGo();
				}
				continue;
			}
		}

		if (!g_uci)
		{
			out("Unknown command: " + cmd);

			std::ofstream outF;
			outF.open("Output(Main).txt");

			if (outF.is_open())
			{
				outF.close();

				remove("Output(Main).txt");
				std::ofstream{ "Output(Module).txt" };

				outF.open("Output(Module).txt");

				if (outF.is_open())
					outF << "Unknown command" << std::endl;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char* argv[])
{
	InitIO();

	InitBitboards();
	Position::InitHashNumbers();
	InitEval();
	g_pos.SetInitial();

	double hashMb = DEFAULT_HASH_SIZE;

	for (int i = 1; i < argc; ++i)
	{
		if ((i < argc - 1) && (!strcmp(argv[i], "-h") || !strcmp(argv[i], "-H") || !strcmp(argv[i], "-hash") || !strcmp(argv[i], "-Hash")))
		{
			hashMb = atof(argv[i + 1]);
			if (hashMb < MIN_HASH_SIZE || hashMb > MAX_HASH_SIZE)
				hashMb = DEFAULT_HASH_SIZE;
		}
		else if ((i < argc - 1) && (!strcmp(argv[i], "-s") || !strcmp(argv[i], "-S") || !strcmp(argv[i], "-strength") || !strcmp(argv[i], "-Strength")))
		{
			int level = atoi(argv[i + 1]);
			SetStrength(level);
		}
		else if (!strcmp(argv[i], "-log"))
			g_log = fopen("GreKo.log", "at");
	}

	SetHashSize(hashMb);

	/*if (IsPipe())
	{
		out(PROGRAM_NAME + " by Vladimir Medvedev");
		g_uci = true;
		g_xboard = false;
		g_console = false;
	}
	else
	{*/
		Highlight(true);
		/*out("\n" + PROGRAM_NAME + " (" + RELEASE_DATE + ")\n");
		Highlight(false);*/
		g_uci = false;
		g_xboard = false;
		g_console = true;
	/*}*/

		/*std::ofstream outF;
		outF.open("Input(None).txt");
		if (!outF.is_open())
			std::ofstream{ "Input(None).txt" };

		outF.open("Output(None).txt");
		if (!outF.is_open())
			std::ofstream{ "Output(None).txt" };*/

	RunCommandLine();

	/*remove("ModuleInput.txt");
	remove("ModuleOutput.txt");*/

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
