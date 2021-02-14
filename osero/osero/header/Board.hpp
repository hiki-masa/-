#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <stdlib.h>
using namespace std;

#define SIZE 8
#define WIDTH SIZE
#define HEIGHT SIZE

/*----------
石の色
----------*/
typedef enum color {
	BLACK,
	WHITE,
	EMPTY,
}COLOR;

/*----------
渡された色から相手の石の色を判断する
----------*/
COLOR other_color(COLOR my_color) {
	COLOR opponent;
	/* 相手の石の色を判断する */
	switch (my_color) {
	case BLACK:
		opponent = WHITE; break;
	case WHITE:
		opponent = BLACK; break;
	default:
		cout << "ERROR HAPPEN!" << endl; exit(0);
	}
	return opponent;
}

/*----------
石が置けるかどうかの判定
----------*/
typedef enum put {
	OK,
	NG,
}PUT;

/*----------
盤面のクラス
----------*/
class BOARD {
private:
	/* 盤面の状態を格納する配列 */
	COLOR board[WIDTH][HEIGHT];

	/* オセロを表示 */
	void display_stone(COLOR color) {
		switch (color) {
		case BLACK:
			cout << "○"; break;
		case WHITE:
			cout << "●"; break;
		case EMPTY:
			cout << "  "; break;
		default:
			cout << "ERROR HAPPEN!" << endl; exit(0);
		}
	}

public:
	/*----------
	コンストラクタ
	----------*/
	BOARD() {
		for (int x = 0, y; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				board[x][y] = EMPTY;
			}
		}
		/* 盤面の真ん中の石の設置 */
		board[WIDTH / 2][HEIGHT / 2] = BLACK;
		board[WIDTH / 2 - 1][HEIGHT / 2 - 1] = BLACK;
		board[WIDTH / 2][HEIGHT / 2 - 1] = WHITE;
		board[WIDTH / 2 - 1][HEIGHT / 2] = WHITE;
	}

	/*----------
	表示
	----------*/
	void display() {
		/* 画面のクリア */
		system("cls");

		/* 表示 */
		for (int x, y = 0; y < HEIGHT; y++) {
			/* 横方向のマス番号を表示 */
			if (y == 0) {
				cout << " ";
				for (x = 0; x < WIDTH; x++) {
					cout << x + 1 << " ";
				}
				cout << endl;
			}

			/* 石の配置を表示 */
			for (x = 0; x < WIDTH; x++) {
				/* 縦方向のマス番号を表示 */
				if (x == 0) {
					cout << y + 1;
				}

				/* x, yに対応する石を表示 */
				display_stone(board[x][y]);
			}
			cout << endl;
		}
	}

	/*----------
	指定の場所がどの状態化を返す
	----------*/
	COLOR check_board(int x, int y) {
		return board[x][y];
	}

	/*----------
	石の設置
	----------*/
	void put_stone(int x, int y, COLOR color) {
		/* opponent : 相手の石の色 */
		COLOR opponent = other_color(color);

		/* 全方向に対して，挟んだ石をひっくり返す */
		for (int x_vectol, y_vectol = -1; y_vectol <= 1; y_vectol++) {
			for (x_vectol = -1; x_vectol <= 1; x_vectol++) {
				/* 確認方向がない場合は，処理を飛ばす */
				if (x_vectol == 0 && y_vectol == 0) {
					continue;
				}

				/* 確認方向に相手の石が無ければ，処理を飛ばす */
				if (board[x + x_vectol][y + y_vectol] != opponent) {
					continue;
				}

				/* 置こうとしているマスから，1マスずつ確認 */
				for (int step = 2; step < SIZE; step++) {
					/* 盤面外のマスは処理しない */
					if (x + x_vectol * step >= 0 && x + x_vectol * step < WIDTH
						&& y + y_vectol * step >= 0 && y + y_vectol * step < HEIGHT) {
						/* 自分の石が見つかる前に，空きがある場合は置き換えることが出来ないので，処理を飛ばす */
						if (board[x + x_vectol * step][y + y_vectol * step] == EMPTY) {
							break;
						}

						/* 方向に自分の石があれば，あいだの石を置き換える */
						if (board[x + x_vectol * step][y + y_vectol * step] == color) {
							/* 目的の場所に石を配置 */
							board[x][y] = color;
							/* 挟んだあいだの石をひっくり返す */
							for (int s = 0; s < step; s++) {
								board[x + x_vectol * s][y + y_vectol * s] = color;
							}
							break;
						}
					}
				}
			}
		}
	}

	/*----------
	指定された場所に石が置けるかどうかの確認
	----------*/
	PUT isPuttable(int x, int y, COLOR color) {
		/* すでに石が置かれている場合はNG */
		if (board[x][y] != EMPTY) {
			return NG;
		}

		/* opponent : 相手の石の色 */
		COLOR opponent = other_color(color);

		/* 置ける方向をカウント */
		int count = 0;
		for (int x_vectol, y_vectol = -1; y_vectol < 2; y_vectol++) {
			for (x_vectol = -1; x_vectol < 2; x_vectol++) {
				/* 確認方向がない場合は，処理を飛ばす */
				if (x_vectol == 0 && y_vectol == 0) {
					continue;
				}

				/* 確認方向に相手の石が無ければ，処理を飛ばす */
				if (board[x + x_vectol][y + y_vectol] != opponent) {
					continue;
				}

				/* 置こうとしているマスから，1マスずつ確認 */
				for (int step = 2; step < SIZE; step++) {
					/* 盤面外のマスは処理しない */
					if (x + x_vectol * step >= 0 && x + x_vectol * step < WIDTH
						&& y + y_vectol * step >= 0 && y + y_vectol * step < HEIGHT) {
						/* 自分の石が見つかる前に，空きがある場合は置き換えることが出来ないので，処理を飛ばす */
						if (board[x + x_vectol * step][y + y_vectol * step] == EMPTY) {
							break;
						}

						/* 方向に自分の石があれば，あいだの石を置き換える */
						if (board[x + x_vectol * step][y + y_vectol * step] == color) {
							count++;
						}
					}
				}
			}
		}
		if (count == 0) { return NG; }
		return OK;
	}

	/*----------
	石を置ける場所があるかどうかを判断
	----------*/
	bool CanPut(COLOR color) {
		for (int x, y = 0; y < HEIGHT; y++) {
			for (x = 0; x < WIDTH; x++) {
				if (isPuttable(x, y, color) == OK) {
					return true;
				}
			}
		}
		return false;
	}

	/*----------
	指定した色の石が何個あるかを数える
	----------*/
	int color_count(COLOR color) {
		int count = 0;
		for (int x, y = 0; y < HEIGHT; y++) {
			for (x = 0; x < WIDTH; x++) {
				if (board[x][y] == color) {
					count++;
				}
			}
		}
		return count;
	}
};

#endif