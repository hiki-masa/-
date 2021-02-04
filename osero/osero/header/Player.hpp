#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Board.hpp"

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
			while (1) {
				/* 置く場所の入力受付 */
				cout << "横方向は？" << endl;
				scanf("%d", &x);
				cout << "縦方向は？" << endl;
				scanf("%d", &y);
				/* 入力された場所が置けるなら，ループを抜ける */
				if (board.isPuttable(x, y, Color) == OK) {
					break;
				}
				else {
					cout << "そこには置けません!!" << endl;
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