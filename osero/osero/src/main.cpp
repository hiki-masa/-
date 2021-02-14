#include <windows.h>
#include "../header/Board.hpp"
#include "../header/Player.hpp"
#include "../header/Computer.hpp"

int main() {
	BOARD osero;
	osero.display();

	/* プレイヤー・コンピュータの設定 */
	PLAYER player1(BLACK);
	AI_COMPUTER player2(WHITE);

	/* ゲームメインループ */
	while (player1.Act || player2.Act) {
		/* Player */
		player1.play(osero);
		osero.display();
		Sleep(1000);

		/* Computer */
		player2.play(osero);
		osero.display();
	}

	/* 最終結果の表示 */
	osero.display();
	cout << "黒 : " << osero.color_count(BLACK) << endl;
	cout << "白 : " << osero.color_count(WHITE) << endl;
}