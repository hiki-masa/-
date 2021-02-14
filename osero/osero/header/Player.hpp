#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Board.hpp"

/* 入力された数値を返す */
int entered_number(const char* display_text) {
	int x, flag = 0, tmp;
	while (flag < 1) {
		cout << display_text << endl;
		flag = scanf("%d", &x);
		/* 暴走防止のおまじない */
		tmp = getchar();
		if (flag == 0) {
			cout << "数値を入力してください" << endl;
		}
	}
	return x;
}

/*----------
プレイヤークラス
----------*/
class PLAYER {
private:
	COLOR Color;

public:
	bool Act;

	/*----------
	コンストラクタ
	----------*/
	PLAYER(COLOR _color) {
		Color = _color;
		Act = true;
		cout << "あなたは" << ((Color == BLACK) ? "黒" : "白") << "です" << endl;
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
			int x, y;
			Act = true;
			while (true) {
				/* 置く場所の入力受付 */
				x = entered_number("横方向は？") - 1;
				y = entered_number("縦方向は？") - 1;
				/* 入力された場所が置けるなら，ループを抜ける */
				if (board.isPuttable(x, y, Color) == OK) {
					break;
				}
				else {
					cout << "そこには置けません!!" << endl;
					cout << "下記に置くことができます" << endl;
					/* 置ける場所の表示 */
					for (y = 0; y < HEIGHT; y++) {
						for (x = 0; x < WIDTH; x++) {
							if (board.isPuttable(x, y, Color) == OK) {
								cout << "(" << x + 1 << ", " << y + 1 << ")" << endl;
							}
						}
					}
				}
			}
			board.put_stone(x, y, Color);
		}
		/* 置ける場所がないなら，処理をスキップ */
		else {
			cout << "置ける場所がないので，スキップします" << endl;
			Act = false;
		}
	}
};

#endif