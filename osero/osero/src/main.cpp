#include "../header/Board.hpp"
#include "../header/Player.hpp"
#include "../header/Computer.hpp"

int main() {
	BOARD osero;
	osero.display();

	PLAYER player1(BLACK);
	AI_COMPUTER player2(WHITE);

	while (player1.Act || player2.Act) {
		/* Player */
		player1.play(osero);
		osero.display();

		/* Computer */
		player2.play(osero);
		osero.display();
	}
	osero.display();
	cout << "• : " << osero.color_count(BLACK) << endl;
	cout << "”’ : " << osero.color_count(WHITE) << endl;
}