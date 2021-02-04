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
			while (1) {
				/* �u���ꏊ�̓��͎�t */
				cout << "�������́H" << endl;
				scanf("%d", &x);
				cout << "�c�����́H" << endl;
				scanf("%d", &y);
				/* ���͂��ꂽ�ꏊ���u����Ȃ�C���[�v�𔲂��� */
				if (board.isPuttable(x, y, Color) == OK) {
					break;
				}
				else {
					cout << "�����ɂ͒u���܂���!!" << endl;
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