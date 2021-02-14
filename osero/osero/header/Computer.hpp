#pragma once

#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include "Board.hpp"

/*----------
COMPUTERクラス
置ける場所にとりあえず置く基礎COM
----------*/
class NORMAL_COMPUTER {
protected:
	COLOR Color;

	/*----------
	置ける場所があった場合の処理
	----------*/
	void can_put(int set_x, int set_y, BOARD& board) {
		cout << "COM が(" << set_x << ", " << set_y << ")に石を置きました" << endl;
		board.put_stone(set_x, set_y, Color);
	}

	/*----------
	置ける場所がなかった場合の処理
	----------*/
	void cant_put() {
		cout << "置ける場所がないので，スキップしました" << endl;
		Act = false;
	}

public:
	bool Act;

	/*----------
	コンストラクタ
	----------*/
	NORMAL_COMPUTER(COLOR _color) {
		Color = _color;
		Act = true;
	}

	/*----------
	色を返す
	----------*/
	COLOR get_color() {
		return Color;
	}

	/*----------
	石を設置
	----------*/
	void play(BOARD& board) {
		/* 置ける場所があるなら，石を置く */
		if (board.CanPut(Color)) {
			Act = true;
			/* 置ける場所の選択 */
			int set_x, set_y;
			for (int x, y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					if (board.isPuttable(x, y, Color) == OK) {
						set_x = x;
						set_y = y;
					}
				}
			}
			/* 選択箇所に石を配置 */
			can_put(set_x, set_y, board);
		}
		/* 置ける場所がないなら，処理をスキップ */
		else {
			cant_put();
		}
	}
};


/*----------
GREEDY_COMPUTERクラス
一番多くとれる場所に石を置くCOM
----------*/
class GREEDY_COMPUTER : public NORMAL_COMPUTER {
public:
	/*----------
	コンストラクタ
	----------*/
	GREEDY_COMPUTER(COLOR _color) : NORMAL_COMPUTER(_color) { ; }

	/*----------
	石を設置
	----------*/
	void play(BOARD& board) {
		/* 置ける場所があるなら，石を置く */
		if (board.CanPut(Color)) {
			Act = true;
			/* 置ける場所の選択 */
			int set_x, set_y;
			int get_count = INT_MIN;
			for (int x, y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					if (board.isPuttable(x, y, Color) == OK) {
						/* 取れる石の数が最大の箇所を選択する */
						BOARD virtual_board = board;
						int before_count = virtual_board.color_count(Color);
						virtual_board.put_stone(x, y, Color);
						int after_count = virtual_board.color_count(Color);
						if (get_count < after_count - before_count) {
							set_x = x;
							set_y = y;
							get_count = after_count - before_count;
						}
					}
				}
			}
			/* 選択箇所に石を配置 */
			can_put(set_x, set_y, board);
		}
		/* 置ける場所がないなら，処理をスキップ */
		else {
			cant_put();
		}
	}
};


/*----------
PREDICT_COMPUTERクラス
複数手先まで考慮して石を置くCOM
----------*/
class PREDICT_COMPUTER : public NORMAL_COMPUTER {
private:
	/*----------
	何手先まで読むかを決める
	----------*/
	int Depth = 6;

	/*----------
	先の手を読むプログラム
	----------*/
	void deep_thinking(BOARD& board, COLOR color, int depth) {
		/* 読む手数が一定以下なら，結果を予測する */
		if (depth < Depth) {
			for (int x, y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					BOARD virtual_board = board;
					if (virtual_board.isPuttable(x, y, color) == OK) {
						virtual_board.put_stone(x, y, color);
						/* 次の手の色を求める */
						COLOR opponent = other_color(color);
						deep_thinking(virtual_board, opponent, depth + 1);
					}
				}
			}
		}
	}

public:
	/*----------
	コンストラクタ
	----------*/
	PREDICT_COMPUTER(COLOR _color) : NORMAL_COMPUTER(_color) {
		Depth;
	}

	/*----------
	石を設置
	----------*/
	void play(BOARD& board) {
		/* 置ける場所があるなら，石を置く */
		if (board.CanPut(Color)) {
			Act = true;
			/* 置ける場所の選択 */
			int set_x, set_y;
			int get_count = INT_MIN;

			#pragma omp parallel for schedule(dynamic, 1)
			for (int y = 0; y < HEIGHT; y++) {
				for (int x = 0; x < WIDTH; x++) {
					if (board.isPuttable(x, y, Color) == OK) {
						BOARD virtual_board = board;
						/* 現在の石の個数を数える */
						int before_count = virtual_board.color_count(Color);
						virtual_board.put_stone(x, y, Color);
						deep_thinking(virtual_board, Color, 0);
						/* 予測で求めた結果の石の個数を数える */
						int after_count = virtual_board.color_count(Color);
						/* 獲得できる個数が最大となる時の手を記憶しておく */
						if (get_count < after_count - before_count) {
							set_x = x;
							set_y = y;
							get_count = after_count - before_count;
						}
					}
				}
			}
			/* 選択箇所に石を配置 */
			can_put(set_x, set_y, board);
		}
		/* 置ける場所がないなら，処理をスキップ */
		else {
			cant_put();
		}
	}
};


/*----------
AI_COMPUTERクラス
学習データより置く石の位置を決めるCOM
----------*/
#include "Read_txt.hpp"
class AI_COMPUTER : public NORMAL_COMPUTER {
private:
	/*----------
	マスごとの重要度を格納した配列
	----------*/
	int Weight[WIDTH][HEIGHT];

	/*----------
	渡された盤面の状態を評価する
	----------*/
	int weight_evaluation(BOARD board) {
		int weight_sum = 0;
		for (int x = 0, y; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				/* 自分の石が置かれている場合は，重みを加算 */
				if (board.check_board(x, y) == Color) {
					weight_sum += Weight[x][y];
				}
				/* 相手の石が置かれている場合は，重みを減算 */
				else if (board.check_board(x, y) == other_color(Color)) {
					weight_sum -= Weight[x][y];
				}
			}
		}
		return weight_sum;
	}

public:
	/*----------
	コンストラクタ
	----------*/
	AI_COMPUTER(COLOR _color) : NORMAL_COMPUTER(_color) {
		vector<int> _weight;
		get_value("./learn_osero.osero", _weight);
		for (int x = 0, y; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				Weight[x][y] = _weight[y * WIDTH + x];
			}
		}
	}

	/*----------
	石を設置
	----------*/
	void play(BOARD& board) {
		/* 置ける場所があるなら，石を置く */
		if (board.CanPut(Color)) {
			Act = true;
			/* 置ける場所の選択 */
			int set_x, set_y;
			int weight_sum = INT_MIN;
			for (int x, y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					if (board.isPuttable(x, y, Color) == OK) {
						BOARD virtual_board = board;
						virtual_board.put_stone(x, y, Color);
						if (weight_sum < weight_evaluation(virtual_board)) {
							set_x = x;
							set_y = y;
							weight_sum = weight_evaluation(virtual_board);
						}
					}
				}
			}
			/* 選択箇所に石を配置 */
			can_put(set_x, set_y, board);
		}
		/* 置ける場所がないなら，処理をスキップ */
		else {
			cant_put();
		}
	}
};

#endif