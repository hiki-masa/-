#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Board.hpp"

/* ���͂��ꂽ���l��Ԃ� */
int entered_number(const char* display_text) {
	int x, flag = 0, tmp;
	while (flag < 1) {
		cout << display_text << endl;
		flag = scanf("%d", &x);
		/* �\���h�~�̂��܂��Ȃ� */
		tmp = getchar();
		if (flag == 0) {
			cout << "���l����͂��Ă�������" << endl;
		}
	}
	return x;
}

/*----------
�v���C���[�N���X
----------*/
class PLAYER {
private:
	COLOR Color;

public:
	bool Act;

	/*----------
	�R���X�g���N�^
	----------*/
	PLAYER(COLOR _color) {
		Color = _color;
		Act = true;
		cout << "���Ȃ���" << ((Color == BLACK) ? "��" : "��") << "�ł�" << endl;
	}

	/*----------
	�F��Ԃ�
	----------*/
	COLOR get_color() {
		return Color;
	}

	/*----------
	�΂�ݒu
	----------*/
	void play(BOARD& board) {
		/* �u����ꏊ������Ȃ�C�΂�u�� */
		if (board.CanPut(Color)) {
			int x, y;
			Act = true;
			while (true) {
				/* �u���ꏊ�̓��͎�t */
				x = entered_number("�������́H") - 1;
				y = entered_number("�c�����́H") - 1;
				/* ���͂��ꂽ�ꏊ���u����Ȃ�C���[�v�𔲂��� */
				if (board.isPuttable(x, y, Color) == OK) {
					break;
				}
				else {
					cout << "�����ɂ͒u���܂���!!" << endl;
					cout << "���L�ɒu�����Ƃ��ł��܂�" << endl;
					/* �u����ꏊ�̕\�� */
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
		/* �u����ꏊ���Ȃ��Ȃ�C�������X�L�b�v */
		else {
			cout << "�u����ꏊ���Ȃ��̂ŁC�X�L�b�v���܂�" << endl;
			Act = false;
		}
	}
};

#endif