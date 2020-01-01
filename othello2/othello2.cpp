﻿#include<iostream>
#include<vector>
#include<string>
using namespace std;
struct Vector2i
{
	int i,j;
	Vector2i(int i0=0, int j0=0)
	{
		i = i0;
		j = j0;
	}
	bool operator== (const Vector2i& a)
	{
		if (this->i == a.i && this->j == a.j)
			return true;
		return false;
	}
};


//const Vector2i size = Vector2i(8, 8);
const int ninzu = 2;
const string stone[ninzu]
= { "●","○" };
void title();
void start(vector<vector<int> >& board);
Vector2i Input(vector<vector<int> >& board, int junban);
int check_dir(int tate, int yoko, const vector<vector<int> >& board,
	int junban, int dtate, int dyoko);
int check(const vector<vector<int> >& board, int tate, int yoko, int junban);
void putStone(vector<vector<int> >& board, int tate, int yoko, int junban);
void drawBoard(const vector<vector<int> >& board);
int checkGameState(const vector<vector<int> >& board);
void reverse(int tate, int yoko, vector<vector<int> >& board, int junban);
void reverse_dir(int tate, int yoko, vector<vector<int> >& board,
	int junban, int dtate, int dyoko);
int main()
{
	//タイトル
	title();
	//初期化
	const int height = 8;
	const int width = 8;
	vector<vector<int> > board(height, vector<int>(width, -1)); //盤面を-1で初期化
	vector<Vector2i> putAble;//おける場所リスト
	start(board);
	int junban = 0;
	drawBoard(board);
	while (1)
	{
		putAble.clear();
		for (int i = 0; i < board.size(); i++)
		{
			for (int j = 0; j < board[0].size(); j++)
			{
				if (check(board, i, j, junban) == 1)
					putAble.push_back(Vector2i(i, j));
			}
		}
		Vector2i input;
		while (true)
		{
			input=Input(board, junban);
			int flag = 0;
			for (int i = 0; i < putAble.size(); i++)
			{
				if (input == putAble[i])
				{
					flag = 1;
					break;
				}
			}
			if (flag == 1)
			{
				break;
			}
			cout << "そこには置けません" << endl;
		}
		
		putStone(board, input.i, input.j, junban);
		drawBoard(board);
		//順番交代
		junban++;
		junban %= ninzu;
		/*if (checkGameState(board) >= size.x * size.y)
		{
			cout << "ゲーム終了";
		}*/
	}

	return 0;
}
//タイトル
void title()
{
	for (int i = 0; i < 5; i++)
	{
		cout << "\n";
	}

	cout << "  OOOOOO  TTTTTT  HH   HH  EEEEEE  LL       LL       OOOOOOO\n"
		<< "  OOOOOO  TTTTTT  HH   HH  EE      LL       LL       OOOOOOO\n"
		<< "  OO  OO    TT    HHHHHHH  EEEEEE  LL       LL       OO   OO\n"
		<< "  OO  OO    TT    HHHHHHH  EEEEEE  LL       LL       OO   OO\n"
		<< "  OOOOOO    TT    HH   HH  EE      LLLLLLL  LLLLLLL  OOOOOOO\n"
		<< "  OOOOOO    TT    HH   HH  EEEEEE  LLLLLLL  LLLLLLL  OOOOOOO\n";

	for (int i = 0; i < 2; i++)
	{
		cout << "\n";
	}

	cout << "  GGGGGGG  AAAAAAA  MM      MM  EEEEEE\n"
		<< "  GG   GG  AAAAAAA  MM      MM  EE\n"
		<< "  GG       AA   AA  M MM  MM M  EEEEEE\n"
		<< "  GG GGGG  AAAAAAA  M MM  MM M  EEEEEE\n"
		<< "  GG   GG  AA   AA  M   MM   M  EE\n"
		<< "  GGGGGGG  AA   AA  M   MM   M  EEEEEE\n";

	for (int i = 0; i < 4; i++)
	{
		cout << "\n";
	}
}

//初期化
void start(vector<vector<int> >& board)
{
	board[3][3] = board[4][4] = 0;
	board[3][4] = board[4][3] = 1;
}
//入力
Vector2i Input(vector<vector<int> >& board, int junban)
{
	cout << stone[junban] << "の番です。";

	int tate, yoko;
		cout << "石を置く位置を指定してください\n";
		//縦の入力
		cout << "縦の位置は？\n";
		cin >> tate;
		//横の入力
		cout << "横の位置は？\n";
		cin >> yoko;
		//入力は1から8、配列の添字は0から7なので座標を合わせる
		tate--;
		yoko--;
	return Vector2i(tate, yoko);
}

//チェック
int check(const vector<vector<int> >& board, int tate, int yoko, int junban)
{
	int height = board.size();
	int width = board[0].size();
	if (tate < 0 || tate >= height)
	{
		//cout << "縦は1から" << height <<  "迄の整数です。\n";
		return 0;
	}
	if (yoko<0 || yoko>width)
	{
		//cout << "横は1から" << width << "迄の整数です。\n";
		return 0;
	}
	//そこに石があったらダメ
	if (board[tate][yoko] != -1)
	{
		//cout << "そこには置けません。\n";
		return 0;
	}
	//ある方向に移動できるか
	for (int i = 0; i < 8; i++)//右から時計回り
	{
		int dyoko, dtate;
		switch (i)
		{
		case 0:
			dyoko = 1, dtate = 0;
			break;
		case 1:
			dyoko = 1, dtate = 1;
			break;
		case 2:
			dyoko = 0, dtate = 1;
			break;
		case 3:
			dyoko = -1, dtate = 1;
			break;
		case 4:
			dyoko = -1, dtate = 0;
			break;
		case 5:
			dyoko = -1, dtate = -1;
			break;
		case 6:
			dyoko = 0, dtate = -1;
			break;
		case 7:
			dyoko = 1, dtate = -1;
			break;
		}
		if (check_dir(tate, yoko, board, junban, dtate, dyoko))
			return 1;

	}
	//cout << "そこには置けません\n";
	return 0;
}

//1方向について置けるかチェック
int check_dir(int tate, int yoko, const vector<vector<int> >& board,
	int junban, int dtate, int dyoko)
{
	int height = board.size();
	int width = board[0].size();
	tate += dtate;  // 隣に移動
	yoko += dyoko;  // 隣に移動

	if (tate < 0 || tate >= height || yoko < 0 || yoko >= width) {
		return 0; // 枠外で即アウト
	}

	if (board[tate][yoko] == -1) {
		return  0; // 隣が空なのでアウト
	}

	if (board[tate][yoko] == junban) {
		return  0;  // 隣が自分の色なのでアウト
	}

	while (1) {

		tate += dtate; // さらに隣に移動
		yoko += dyoko; // さらに隣に移動

		if (tate < 0 || tate >= height || yoko < 0 || yoko >= width) {
			return 0; // 枠外でアウト
		}

		if (board[tate][yoko] == -1) {
			return 0;  // 空のマスにぶつかってアウト
		}

		if (board[tate][yoko] == junban) {
			return 1; // 自分の色の石にぶつかってOK
		}
	}
}


//石を置く
void putStone(vector<vector<int> >& board, int tate, int yoko, int junban)
{
	board[tate][yoko] = junban;
	reverse(tate, yoko, board, junban);
}
//石をひっくり返す
void reverse(int tate, int yoko, vector<vector<int> >& board, int junban) {

	if (check_dir(tate, yoko, board, junban, 1, 1) == 1) {
		reverse_dir(tate, yoko, board, junban, 1, 1);
	}
	if (check_dir(tate, yoko, board, junban, 1, 0) == 1) {
		reverse_dir(tate, yoko, board, junban, 1, 0);
	}
	if (check_dir(tate, yoko, board, junban, 1, -1) == 1) {
		reverse_dir(tate, yoko, board, junban, 1, -1);
	}
	if (check_dir(tate, yoko, board, junban, 0, 1) == 1) {
		reverse_dir(tate, yoko, board, junban, 0, 1);
	}
	if (check_dir(tate, yoko, board, junban, 0, -1) == 1) {
		reverse_dir(tate, yoko, board, junban, 0, -1);
	}
	if (check_dir(tate, yoko, board, junban, -1, 1) == 1) {
		reverse_dir(tate, yoko, board, junban, -1, 1);
	}
	if (check_dir(tate, yoko, board, junban, -1, 0) == 1) {
		reverse_dir(tate, yoko, board, junban, -1, 0);
	}
	if (check_dir(tate, yoko, board, junban, -1, -1) == 1) {
		reverse_dir(tate, yoko, board, junban, -1, -1);
	}
}

//1方向についてひっくり返す
void reverse_dir(int tate, int yoko, vector<vector<int> >& board,
	int junban, int dtate, int dyoko)
{

	tate += dtate;  // 隣に移動
	yoko += dyoko;  // 隣に移動

	board[tate][yoko] = junban; // 隣は自分の色に置き換え(はさめることは確認済み

	while (1) {

		tate += dtate; // さらに隣に移動
		yoko += dyoko; // さらに隣に移動

		if (board[tate][yoko] == junban) {
			return; // 自分の色の石にぶつかったら終了
		}

		board[tate][yoko] = junban; // 自分の色に置き換え

	}
}

//オセロ盤表示
void drawBoard(const vector<vector<int> >& board)
{
	cout << "  　１　２　３　４　５　６　７　８\n"
		<< "  ＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋\n";
	for (int i = 1; i <= 8; i++)
	{
		cout << " " << i << "｜";
		for (int j = 1; j <= 8; j++)
		{
			if (board[i - 1][j - 1] == -1)
				cout << "　";
			else
				cout << stone[board[i - 1][j - 1]];
			cout << "｜";
		}
		cout << "\n  ＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋\n";
	}
}

//数えて表示 ゲーム終了なら1 続行なら0
int checkGameState(const vector<vector<int> >& board)
{
	int black = 0, white = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 1)
				white++;
			else if (board[i][j] == 2)
				black++;
		}
	}
	cout << "白" << white << "枚\t";
	cout << "黒" << black << "枚\t";
	cout << "合計" << white + black << "枚\n\n";
	if (white + black == 64)
	{
		if (black > white)
			cout << "黒の勝ちです。\n";
		else if (white > black)
			cout << "白の勝ちです。\n";
		else
			cout << "引き分けです。\n";
		return 1;
	}
	return 0;
}
