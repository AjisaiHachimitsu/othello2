#include<iostream>
#include<vector>
#include<string>
#include<random>
#include<cmath>
using namespace std;
std::mt19937 mt;
struct Vector2i
{
	int i, j;
	Vector2i(int i0 = 0, int j0 = 0)
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


const int NINZU = 3;
const string stone[NINZU]
= { "●","○" ,"◎" };
void title();
void start(vector<vector<int> >& board);
vector<Vector2i> serchPutAble(const vector<vector<int> >& board, int junban);
Vector2i Input(const vector<vector<int> >& board, int junban);
int check_dir(int tate, int yoko, const vector<vector<int> >& board,
	int junban, int dtate, int dyoko);
int check(const vector<vector<int> >& board, int tate, int yoko, int junban);
void putStone(vector<vector<int> >& board, int tate, int yoko, int junban);
void drawBoard(const vector<vector<int> >& board);
vector<int> CountStone(const vector<vector<int> >& board, int ninzu, bool isShow = true);
void reverse(int tate, int yoko, vector<vector<int> >& board, int junban);
void reverse_dir(int tate, int yoko, vector<vector<int> >& board,
	int junban, int dtate, int dyoko);
void gameOver(const vector<int>& countStone);
Vector2i randomPut(const vector<Vector2i>& putAble);
Vector2i minimax(int (*f)(const vector<vector<int > >& board, int ninzu, int junban), const vector<vector<int> >& board, int ninzu, int junban, int deep);//fは評価関数
int CPU1(const vector<vector<int> >& board, int ninzu, int junban);//石の数優先
int Max(const vector<int>& data, vector<int>& maxIndex);
int Min(const vector<int>& data, vector<int>& minIndex);
int Max(const vector<int>& data);
int Min(const vector<int>& data);

int main()
{
	//タイトル
	title();
	//初期化
	const int height = 9;
	const int width = 9;
	vector<vector<int> > board(height, vector<int>(width, -1)); //盤面を-1で初期化
	start(board);
	int junban = 0;
	drawBoard(board);
	int passcount = 0;
	const int ninzu = NINZU;
	while (true)
	{
		vector<Vector2i> putAble = serchPutAble(board, junban);//おける場所リスト
		//putAble.clear();
		if (putAble.empty())
		{
			passcount++;
			cout << "おける場所がないので" << stone[junban] << "の番はスキップします。" << endl;
		}
		else {
			passcount = 0;
			Vector2i input;
			while (true)
			{
				cout << stone[junban] << "の番です。";
				/*if(junban==0)
					input = Input(board, junban);
				else*/
				input = minimax(CPU1, board, ninzu, junban, 1);
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
		}
		drawBoard(board);
		//順番交代
		junban++;
		junban %= ninzu;
		//cout << passcount << endl;
		vector<int> countstone = CountStone(board, ninzu);
		if (passcount >= ninzu)
		{
			cout << "ゲーム終了\n";
			gameOver(countstone);
			return 0;
		}
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
	board[3][3] = board[4][4] = board[5][5] = 0;
	board[3][4] = board[4][5] = board[5][3] = 1;
	board[3][5] = board[4][3] = board[5][4] = 2;
	std::random_device rd;
	mt = std::mt19937(rd());
}
vector<Vector2i> serchPutAble(const vector<vector<int> >& board, int junban)
{
	vector<Vector2i>putAble;
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[0].size(); j++)
		{
			if (check(board, i, j, junban) == 1)
				putAble.push_back(Vector2i(i, j));
		}
	}
	return putAble;
}
//入力
Vector2i Input(const vector<vector<int> >& board, int junban)
{

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

	board[tate][yoko] = junban; // 隣は自分の色に置き換え(はさめることは確認済み)

	while (1) {

		tate += dtate; // さらに隣に移動
		yoko += dyoko; // さらに隣に移動

		if (board[tate][yoko] == junban) {
			return; // 自分の色の石にぶつかったら終了
		}

		board[tate][yoko] = junban; // 自分の色に置き換え

	}
}

void gameOver(const vector<int>& countStone)
{

	vector<int> winner;
	int max = Max(countStone, winner);
	for (int i = 0; i < winner.size(); i++)
	{
		cout << stone[winner[i]];
		if (i != winner.size() - 1)
			cout << "と";
	}
	cout << "の勝ちです。" << endl;
}

Vector2i randomPut(const vector<Vector2i>& putAble)
{
	return putAble[mt() % putAble.size()];
}

Vector2i minimax(int(*f)(const vector<vector<int>>& board, int ninzu, int junban), const vector<vector<int> >& board, int ninzu, int junban, int deep)
{
	//ゲーム木構築
	Vector2i put;
	vector<vector<vector<vector<vector<int> > > > > gameKi(1);
	vector<vector<vector<vector<int> > > >oneturn;//1ターンのゲーム情報
	gameKi[0] = (vector<vector<vector<vector<int> > > >(1, vector<vector<vector<int> > >(1, board)));
	int junban2 = junban;
	for (int i = 0; i < 3*deep+1; i++)
	{
		vector<vector<vector<int > > >a;
		for (int j = 0; j < gameKi[i].size(); j++)
		{
			for (int k = 0; k < gameKi[i][j].size(); k++)
			{
				vector<Vector2i> putAble = serchPutAble(gameKi[i][j][k], junban2);
				for (int l = 0; l < putAble.size(); l++)
				{
					vector<vector<int> > board2 = gameKi[i][j][k];
					putStone(board2, putAble[k].i, putAble[k].j, junban2);
					a.push_back(board2);
				}
				oneturn.push_back(a);
			}
		}
		gameKi.push_back(oneturn);
		junban2++;
		junban2 %= ninzu;
	}
	cout << gameKi.size() << endl << gameKi[0].size() << endl << gameKi[0][0].size();

	vector<vector<vector<int> > > gamekiValue(gameKi.size());
	int End = gameKi.size() - 1;
	gamekiValue[End].resize(gameKi[End].size());
	for (int j = 0; j < gameKi[End].size(); j++)
	{
		gamekiValue[End][j].resize(gameKi[End][j].size);
		vector<int>maxindex;
		for (int k = 0; k < gameKi[End][j].size(); k++)
		{
			gamekiValue[End][j][k] = f(gameKi[End][j][k], ninzu, junban);
		}
	}
	Max(gamekiValue[End][j]);
	for (int i = gameKi.size() - 1; i >= 0; i--)
	{
		for (int j = 0; j < gameKi[i-1].size(); j++)
		{
			for (int k = 0; k < gameKi[i - 1][j].size(); k++)
			{

			}
		}
	}
	return Vector2i();
}

int CPU1(const vector<vector<int>>& board, int ninzu, int junban)
{
	int value = 0;
	auto countstone = CountStone(board, ninzu, false);
	for (int i = 0; i < countstone.size(); i++)
	{
		if (i == junban)
			value += countstone[i];
		else
			value -= countstone[i];
	}
	return value;
}

//オセロ盤表示
void drawBoard(const vector<vector<int> >& board)
{
	cout << "  　１　２　３　４　５　６　７　８　９\n"
		<< "  ＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋\n";
	for (int i = 1; i <= board.size(); i++)
	{
		cout << " " << i << "｜";
		for (int j = 1; j <= board[0].size(); j++)
		{
			if (board[i - 1][j - 1] == -1)
				cout << "　";
			else
				cout << stone[board[i - 1][j - 1]];
			cout << "｜";
		}
		cout << "\n  ＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋ー＋\n";
	}
}

//数えて表示 
vector<int> CountStone(const vector<vector<int> >& board, int ninzu, bool isShow)
{
	vector<int> countStone(ninzu, 0);
	//int black = 0, white = 0;
	int sum = 0;
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[0].size(); j++)
		{
			if (board[i][j] != -1)
			{
				countStone[board[i][j]]++;
				sum++;
			}
		}
	}
	if (isShow)
	{
		for (int i = 0; i < ninzu; i++)
		{
			cout << stone[i] << countStone[i] << "枚" << endl;
		}

		cout << "合計" << sum << "枚\n\n";
	}
	return countStone;
}

int Max(const vector<int>& data, vector<int>& maxIndex)
{
	int max = data[0];
	for (int i = 1; i < data.size(); i++)
	{
		if (max < data[i])
			max = data[i];
	}
	maxIndex.clear();
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == max)
			maxIndex.push_back(i);
	}
	return max;
}

int Min(const vector<int>& data, vector<int>& minIndex)
{
	int min = data[0];
	for (int i = 1; i < data.size(); i++)
	{
		if (min > data[i])
			min = data[i];
	}
	minIndex.clear();
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == min)
			minIndex.push_back(i);
	}
	return min;
}

int Max(const vector<int>& data)
{
	int max = data[0];
	for (int i = 1; i < data.size(); i++)
	{
		if (max < data[i])
			max = data[i];
	}
	return max;
}
int Min(const vector<int>& data)
{
	int min = data[0];
	for (int i = 1; i < data.size(); i++)
	{
		if (min > data[i])
			min = data[i];
	}
	return min;
}