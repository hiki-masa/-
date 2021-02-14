#pragma once

#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include "Board.hpp"

/*----------
COMPUTER�N���X
�u����ꏊ�ɂƂ肠�����u����bCOM
----------*/
class NORMAL_COMPUTER {
protected:
	COLOR Color;

	/*----------
	�u����ꏊ���������ꍇ�̏���
	----------*/
	void can_put(int set_x, int set_y, BOARD& board) {
		cout << "COM ��(" << set_x << ", " << set_y << ")�ɐ΂�u���܂���" << endl;
		board.put_stone(set_x, set_y, Color);
	}

	/*----------
	�u����ꏊ���Ȃ������ꍇ�̏���
	----------*/
	void cant_put() {
		cout << "�u����ꏊ���Ȃ��̂ŁC�X�L�b�v���܂���" << endl;
		Act = false;
	}

public:
	bool Act;

	/*----------
	�R���X�g���N�^
	----------*/
	NORMAL_COMPUTER(COLOR _color) {
		Color = _color;
		Act = true;
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
			Act = true;
			/* �u����ꏊ�̑I�� */
			int set_x, set_y;
			for (int x, y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					if (board.isPuttable(x, y, Color) == OK) {
						set_x = x;
						set_y = y;
					}
				}
			}
			/* �I���ӏ��ɐ΂�z�u */
			can_put(set_x, set_y, board);
		}
		/* �u����ꏊ���Ȃ��Ȃ�C�������X�L�b�v */
		else {
			cant_put();
		}
	}
};


/*----------
GREEDY_COMPUTER�N���X
��ԑ����Ƃ��ꏊ�ɐ΂�u��COM
----------*/
class GREEDY_COMPUTER : public NORMAL_COMPUTER {
public:
	/*----------
	�R���X�g���N�^
	----------*/
	GREEDY_COMPUTER(COLOR _color) : NORMAL_COMPUTER(_color) { ; }

	/*----------
	�΂�ݒu
	----------*/
	void play(BOARD& board) {
		/* �u����ꏊ������Ȃ�C�΂�u�� */
		if (board.CanPut(Color)) {
			Act = true;
			/* �u����ꏊ�̑I�� */
			int set_x, set_y;
			int get_count = INT_MIN;
			for (int x, y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					if (board.isPuttable(x, y, Color) == OK) {
						/* ����΂̐����ő�̉ӏ���I������ */
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
			/* �I���ӏ��ɐ΂�z�u */
			can_put(set_x, set_y, board);
		}
		/* �u����ꏊ���Ȃ��Ȃ�C�������X�L�b�v */
		else {
			cant_put();
		}
	}
};


/*----------
PREDICT_COMPUTER�N���X
�������܂ōl�����Đ΂�u��COM
----------*/
class PREDICT_COMPUTER : public NORMAL_COMPUTER {
private:
	/*----------
	�����܂œǂނ������߂�
	----------*/
	int Depth = 6;

	/*----------
	��̎��ǂރv���O����
	----------*/
	void deep_thinking(BOARD& board, COLOR color, int depth) {
		/* �ǂގ萔�����ȉ��Ȃ�C���ʂ�\������ */
		if (depth < Depth) {
			for (int x, y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					BOARD virtual_board = board;
					if (virtual_board.isPuttable(x, y, color) == OK) {
						virtual_board.put_stone(x, y, color);
						/* ���̎�̐F�����߂� */
						COLOR opponent = other_color(color);
						deep_thinking(virtual_board, opponent, depth + 1);
					}
				}
			}
		}
	}

public:
	/*----------
	�R���X�g���N�^
	----------*/
	PREDICT_COMPUTER(COLOR _color) : NORMAL_COMPUTER(_color) {
		Depth;
	}

	/*----------
	�΂�ݒu
	----------*/
	void play(BOARD& board) {
		/* �u����ꏊ������Ȃ�C�΂�u�� */
		if (board.CanPut(Color)) {
			Act = true;
			/* �u����ꏊ�̑I�� */
			int set_x, set_y;
			int get_count = INT_MIN;

			#pragma omp parallel for schedule(dynamic, 1)
			for (int y = 0; y < HEIGHT; y++) {
				for (int x = 0; x < WIDTH; x++) {
					if (board.isPuttable(x, y, Color) == OK) {
						BOARD virtual_board = board;
						/* ���݂̐΂̌��𐔂��� */
						int before_count = virtual_board.color_count(Color);
						virtual_board.put_stone(x, y, Color);
						deep_thinking(virtual_board, Color, 0);
						/* �\���ŋ��߂����ʂ̐΂̌��𐔂��� */
						int after_count = virtual_board.color_count(Color);
						/* �l���ł�������ő�ƂȂ鎞�̎���L�����Ă��� */
						if (get_count < after_count - before_count) {
							set_x = x;
							set_y = y;
							get_count = after_count - before_count;
						}
					}
				}
			}
			/* �I���ӏ��ɐ΂�z�u */
			can_put(set_x, set_y, board);
		}
		/* �u����ꏊ���Ȃ��Ȃ�C�������X�L�b�v */
		else {
			cant_put();
		}
	}
};


/*----------
AI_COMPUTER�N���X
�w�K�f�[�^���u���΂̈ʒu�����߂�COM
----------*/
#include "Read_txt.hpp"
class AI_COMPUTER : public NORMAL_COMPUTER {
private:
	/*----------
	�}�X���Ƃ̏d�v�x���i�[�����z��
	----------*/
	int Weight[WIDTH][HEIGHT];

	/*----------
	�n���ꂽ�Ֆʂ̏�Ԃ�]������
	----------*/
	int weight_evaluation(BOARD board) {
		int weight_sum = 0;
		for (int x = 0, y; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				/* �����̐΂��u����Ă���ꍇ�́C�d�݂����Z */
				if (board.check_board(x, y) == Color) {
					weight_sum += Weight[x][y];
				}
				/* ����̐΂��u����Ă���ꍇ�́C�d�݂����Z */
				else if (board.check_board(x, y) == other_color(Color)) {
					weight_sum -= Weight[x][y];
				}
			}
		}
		return weight_sum;
	}

public:
	/*----------
	�R���X�g���N�^
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
	�΂�ݒu
	----------*/
	void play(BOARD& board) {
		/* �u����ꏊ������Ȃ�C�΂�u�� */
		if (board.CanPut(Color)) {
			Act = true;
			/* �u����ꏊ�̑I�� */
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
			/* �I���ӏ��ɐ΂�z�u */
			can_put(set_x, set_y, board);
		}
		/* �u����ꏊ���Ȃ��Ȃ�C�������X�L�b�v */
		else {
			cant_put();
		}
	}
};

#endif