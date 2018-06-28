#include <stdlib.h>
#include <stdio.h>
#include <cnaiapi.h>
#define BUFFSIZE 256
#define BOARDSIZE 59
#define RECEIVED_PROMPT	"Received> "

void create_board(char board[BOARDSIZE]);
void send_board(connection conn, char board[BOARDSIZE]);
char receive_choice(connection conn);
void put_choice_on_board(char board[BOARDSIZE], char choice, char c);
int check_for_winner(char board[BOARDSIZE]);
void send_results(connection conn, char p);
int send_int(connection conn, int k);
int is_correct(char, char board[BOARDSIZE]);

int main(int argc, char *argv[]) {
	char pX = '-';
	char pY = '-';
	char c = 'X';
	char choice;
	char board[BOARDSIZE];
	int turn = 0;
	char buff[BUFFSIZE];
	connection	conn0;
	connection	conn1;
	connection	connX;
	connection	connY;
	appnum a = 20000;

	printf("Game Server Waiting For Connections.\n");

	conn0 = await_contact(a);
	if (conn0 < 0)
		exit(1);
	connX = conn0;
	int id = 0;
	send_int(connX, id);
	printf("First Player Connection Established.\n");

	printf("Game Server Waiting For Second Player.\n");

	conn1 = await_contact(a);
	if (conn1 < 0)
		exit(1);
	connY = conn1;
	id = 1;
	send_int(connY, id);
	printf("Second Player Connection Established.\n");

	create_board(board);

	send_board(connY, board);

	send_int(connX, turn);
	while (turn < 9) {
		send_board(connX, board);

		int ic = 0;
		send_int(connX, ic);
		while (ic == 0) {
			choice = receive_choice(connX);
			ic = is_correct(choice, board);
			send_int(connX, ic);
		}
		if (ic == -1) {
			turn = 20;
			break;
		}

		put_choice_on_board(board, choice, c);
		send_board(connX, board);
		if ((turn == 0) || (turn == 2) || (turn == 4) || (turn == 6) || (turn == 8))
		{
			connX = conn1;
			connY = conn0;
			c = 'O';
		}
		else
		{
			connX = conn0;
			connY = conn1;
			c = 'X';
		}
		if (check_for_winner(board) == 1) {
			if ((turn == 0) || (turn == 2) || (turn == 4) || (turn == 6) || (turn == 8)) {
				pX = 'L';
				pY = 'W';
			}
			else {
				pY = 'W';
				pX = 'L';
			}

			break;
		}
		turn = turn + 1;
		if (send_int(connX, turn) == -1) {
			turn = 21;
			break;
		}
		
	}
	if (turn == 20 || turn == 21) {
		send_int(connY, turn);
	}
	else {
		if (turn != 9) {
			turn = 9;
			send_int(connX, turn);
			send_int(connY, turn);
		}
		else {
			if (check_for_winner(board) == 1) {
				pY = 'W';
				pX = 'L';
			}
			send_int(connY, turn);
		}

		send_board(connX, board);
		send_board(connY, board);
		send_results(connX, pX);
		send_results(connY, pY);
		send_eof(connX);
		send_eof(connY);
		printf("\nGame Connection Closed.\n\n");
	}
	return 0;
}

void create_board(char board[BOARDSIZE]) {

	board[0] = '\n';
	board[1] = ' ';
	board[2] = '0';
	board[3] = ' ';
	board[4] = '|';
	board[5] = ' ';
	board[6] = '1';
	board[7] = ' ';
	board[8] = '|';
	board[9] = ' ';
	board[10] = '2';
	board[11] = '\n';
	board[12] = '-';
	board[13] = '-';
	board[14] = '-';
	board[15] = '+';
	board[16] = '-';
	board[17] = '-';
	board[18] = '-';
	board[19] = '+';
	board[20] = '-';
	board[21] = '-';
	board[22] = '-';
	board[23] = '\n';
	board[24] = ' ';
	board[25] = '3';
	board[26] = ' ';
	board[27] = '|';
	board[28] = ' ';
	board[29] = '4';
	board[30] = ' ';
	board[31] = '|';
	board[32] = ' ';
	board[33] = '5';
	board[34] = '\n';
	board[35] = '-';
	board[36] = '-';
	board[37] = '-';
	board[38] = '+';
	board[39] = '-';
	board[40] = '-';
	board[41] = '-';
	board[42] = '+';
	board[43] = '-';
	board[44] = '-';
	board[45] = '-';
	board[46] = '\n';
	board[47] = ' ';
	board[48] = '6';
	board[49] = ' ';
	board[50] = '|';
	board[51] = ' ';
	board[52] = '7';
	board[53] = ' ';
	board[54] = '|';
	board[55] = ' ';
	board[56] = '8';
	board[57] = '\n';
	board[58] = '\0';

}

void send_board(connection conn, char board[BOARDSIZE]) {
	int i;
	int len = BOARDSIZE;
	char buff[BUFFSIZE];

	fflush(stdout);
	for (i = 0; i < BOARDSIZE; i++) {
		buff[i] = board[i];
	}
	write(STDOUT_FILENO, buff, len);
	send(conn, buff, len, 0);

}

char receive_choice(connection conn) {
	int len;
	char buff[BUFFSIZE];

	printf(RECEIVED_PROMPT);
	fflush(stdout);
	len = recv(conn, buff, BUFFSIZE, 0);
	write(STDOUT_FILENO, buff, len);
	return buff[0];

}

void put_choice_on_board(char board[BOARDSIZE], char choice, char c) {
	if (choice == '0')
		board[2] = c;
	if (choice == '1')
		board[6] = c;
	if (choice == '2')
		board[10] = c;
	if (choice == '3')
		board[25] = c;
	if (choice == '4')
		board[29] = c;
	if (choice == '5')
		board[33] = c;
	if (choice == '6')
		board[48] = c;
	if (choice == '7')
		board[52] = c;
	if (choice == '8')
		board[56] = c;

}

int check_for_winner(char board[BOARDSIZE]) {

	if (board[2] == board[6] && board[6] == board[10])
		return 1;
	if (board[25] == board[29] && board[29] == board[33])
		return 1;
	if (board[48] == board[52] && board[52] == board[56])
		return 1;
	if (board[2] == board[25] && board[25] == board[48])
		return 1;
	if (board[6] == board[29] && board[29] == board[52])
		return 1;
	if (board[10] == board[33] && board[33] == board[56])
		return 1;
	if (board[2] == board[29] && board[29] == board[56])
		return 1;
	if (board[10] == board[29] && board[29] == board[48])
		return 1;

	return 0;
}

void send_results(connection conn, char p) {
	int i;
	char buff[BUFFSIZE];

	char wm[9];
	wm[0] = 'Y';
	wm[1] = 'o';
	wm[2] = 'u';
	wm[3] = ' ';
	wm[4] = 'w';
	wm[5] = 'o';
	wm[6] = 'n';
	wm[7] = '!';
	wm[8] = '\0';

	char lm[10];
	lm[0] = 'Y';
	lm[1] = 'o';
	lm[2] = 'u';
	lm[3] = ' ';
	lm[4] = 'l';
	lm[5] = 'o';
	lm[6] = 's';
	lm[7] = 't';
	lm[8] = '.';
	lm[9] = '\0';

	char is[6];
	is[0] = 'D';
	is[1] = 'r';
	is[2] = 'a';
	is[3] = 'w';
	is[4] = '.';
	is[5] = '\0';

	fflush(stdout);
	if (p == 'W') {
		for (i = 0; i < 9; i++) {
			buff[i] = wm[i];
		}
	}
	else if (p == 'L') {
		for (i = 0; i < 10; i++) {
			buff[i] = lm[i];
		}
	}
	else {
		for (i = 0; i < 6; i++) {
			buff[i] = is[i];
		}
	}
	send(conn, buff, i, 0);

}

int send_int(connection conn, int k) {
	char	buff[BUFFSIZE];

	buff[0] = k + '0';
	buff[1] = '\0';
	(void)fflush(stdout);
	if (send(conn, buff, 2, 0) != -1) {
		return 1;
	}
	else
		return -1;
}

int is_correct(char choice, char board[BOARDSIZE]) {
	if (choice == '0' || choice == '1' || choice == '2' || choice == '3' || choice == '4' || choice == '5' || choice == '6' || choice == '7' || choice == '8') {
		if (choice == '0') {
			if (board[2] == 'O' || board[2] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '1') {
			if (board[6] == 'O' || board[6] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '2') {
			if (board[10] == 'O' || board[10] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '3') {
			if (board[25] == 'O' || board[25] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '4') {
			if (board[29] == 'O' || board[29] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '5') {
			if (board[33] == 'O' || board[33] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '6') {
			if (board[48] == 'O' || board[48] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '7') {
			if (board[52] == 'O' || board[52] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (choice == '8') {
			if (board[56] == 'O' || board[56] == 'X') {
				return 0;
			}
			else {
				return 1;
			}
		}

	}
	else {
		return -1;
	}

}
