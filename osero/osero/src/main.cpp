#include <windows.h>
#include "../header/Board.hpp"
#include "../header/Player.hpp"
#include "../header/Computer.hpp"

int main() {
	BOARD osero;
	osero.display();

	/* �v���C���[�E�R���s���[�^�̐ݒ� */
	PLAYER player1(BLACK);
	AI_COMPUTER player2(WHITE);

	/* �Q�[�����C�����[�v */
	while (player1.Act || player2.Act) {
		/* Player */
		player1.play(osero);
		osero.display();
		Sleep(1000);

		/* Computer */
		player2.play(osero);
		osero.display();
	}

	/* �ŏI���ʂ̕\�� */
	osero.display();
	cout << "�� : " << osero.color_count(BLACK) << endl;
	cout << "�� : " << osero.color_count(WHITE) << endl;
}