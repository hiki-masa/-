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
�΂̐F
----------*/
typedef enum color {
	BLACK,
	WHITE,
	EMPTY,
}COLOR;

/*----------
�n���ꂽ�F���瑊��̐΂̐F�𔻒f����
----------*/
COLOR other_color(COLOR my_color) {
	COLOR opponent;
	/* ����̐΂̐F�𔻒f���� */
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
�΂��u���邩�ǂ����̔���
----------*/
typedef enum put {
	OK,
	NG,
}PUT;

/*----------
�Ֆʂ̃N���X
----------*/
class BOARD {
private:
	/* �Ֆʂ̏�Ԃ��i�[����z�� */
	COLOR board[WIDTH][HEIGHT];

	/* �I�Z����\�� */
	void display_stone(COLOR color) {
		switch (color) {
		case BLACK:
			cout << "��"; break;
		case WHITE:
			cout << "��"; break;
		case EMPTY:
			cout << "  "; break;
		default:
			cout << "ERROR HAPPEN!" << endl; exit(0);
		}
	}

public:
	/*----------
	�R���X�g���N�^
	----------*/
	BOARD() {
		for (int x = 0, y; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				board[x][y] = EMPTY;
			}
		}
		/* �Ֆʂ̐^�񒆂̐΂̐ݒu */
		board[WIDTH / 2][HEIGHT / 2] = BLACK;
		board[WIDTH / 2 - 1][HEIGHT / 2 - 1] = BLACK;
		board[WIDTH / 2][HEIGHT / 2 - 1] = WHITE;
		board[WIDTH / 2 - 1][HEIGHT / 2] = WHITE;
	}

	/*----------
	�\��
	----------*/
	void display() {
		/* ��ʂ̃N���A */
		system("cls");

		/* �\�� */
		for (int x, y = 0; y < HEIGHT; y++) {
			/* �������̃}�X�ԍ���\�� */
			if (y == 0) {
				cout << " ";
				for (x = 0; x < WIDTH; x++) {
					cout << x + 1 << " ";
				}
				cout << endl;
			}

			/* �΂̔z�u��\�� */
			for (x = 0; x < WIDTH; x++) {
				/* �c�����̃}�X�ԍ���\�� */
				if (x == 0) {
					cout << y + 1;
				}

				/* x, y�ɑΉ�����΂�\�� */
				display_stone(board[x][y]);
			}
			cout << endl;
		}
	}

	/*----------
	�w��̏ꏊ���ǂ̏�ԉ���Ԃ�
	----------*/
	COLOR check_board(int x, int y) {
		return board[x][y];
	}

	/*----------
	�΂̐ݒu
	----------*/
	void put_stone(int x, int y, COLOR color) {
		/* opponent : ����̐΂̐F */
		COLOR opponent = other_color(color);

		/* �S�����ɑ΂��āC���񂾐΂��Ђ�����Ԃ� */
		for (int x_vectol, y_vectol = -1; y_vectol <= 1; y_vectol++) {
			for (x_vectol = -1; x_vectol <= 1; x_vectol++) {
				/* �m�F�������Ȃ��ꍇ�́C�������΂� */
				if (x_vectol == 0 && y_vectol == 0) {
					continue;
				}

				/* �m�F�����ɑ���̐΂�������΁C�������΂� */
				if (board[x + x_vectol][y + y_vectol] != opponent) {
					continue;
				}

				/* �u�����Ƃ��Ă���}�X����C1�}�X���m�F */
				for (int step = 2; step < SIZE; step++) {
					/* �ՖʊO�̃}�X�͏������Ȃ� */
					if (x + x_vectol * step >= 0 && x + x_vectol * step < WIDTH
						&& y + y_vectol * step >= 0 && y + y_vectol * step < HEIGHT) {
						/* �����̐΂�������O�ɁC�󂫂�����ꍇ�͒u�������邱�Ƃ��o���Ȃ��̂ŁC�������΂� */
						if (board[x + x_vectol * step][y + y_vectol * step] == EMPTY) {
							break;
						}

						/* �����Ɏ����̐΂�����΁C�������̐΂�u�������� */
						if (board[x + x_vectol * step][y + y_vectol * step] == color) {
							/* �ړI�̏ꏊ�ɐ΂�z�u */
							board[x][y] = color;
							/* ���񂾂������̐΂��Ђ�����Ԃ� */
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
	�w�肳�ꂽ�ꏊ�ɐ΂��u���邩�ǂ����̊m�F
	----------*/
	PUT isPuttable(int x, int y, COLOR color) {
		/* ���łɐ΂��u����Ă���ꍇ��NG */
		if (board[x][y] != EMPTY) {
			return NG;
		}

		/* opponent : ����̐΂̐F */
		COLOR opponent = other_color(color);

		/* �u����������J�E���g */
		int count = 0;
		for (int x_vectol, y_vectol = -1; y_vectol < 2; y_vectol++) {
			for (x_vectol = -1; x_vectol < 2; x_vectol++) {
				/* �m�F�������Ȃ��ꍇ�́C�������΂� */
				if (x_vectol == 0 && y_vectol == 0) {
					continue;
				}

				/* �m�F�����ɑ���̐΂�������΁C�������΂� */
				if (board[x + x_vectol][y + y_vectol] != opponent) {
					continue;
				}

				/* �u�����Ƃ��Ă���}�X����C1�}�X���m�F */
				for (int step = 2; step < SIZE; step++) {
					/* �ՖʊO�̃}�X�͏������Ȃ� */
					if (x + x_vectol * step >= 0 && x + x_vectol * step < WIDTH
						&& y + y_vectol * step >= 0 && y + y_vectol * step < HEIGHT) {
						/* �����̐΂�������O�ɁC�󂫂�����ꍇ�͒u�������邱�Ƃ��o���Ȃ��̂ŁC�������΂� */
						if (board[x + x_vectol * step][y + y_vectol * step] == EMPTY) {
							break;
						}

						/* �����Ɏ����̐΂�����΁C�������̐΂�u�������� */
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
	�΂�u����ꏊ�����邩�ǂ����𔻒f
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
	�w�肵���F�̐΂������邩�𐔂���
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