// -------------------------------------------------------------
//
// Simple Tic Tac Toe game between two players(clients) connected to a server,
// using Douglas E. Comer's CNAI Socket API.
// This code implements the server.
//
// Author: Aggelos Stamatiou, December 2015
//
// --------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cnaiapi.h>

// Execution constants
#define BUFFSIZE        256
#define BOARDSIZE       59
#define SEND_PROMPT     "Sending> "
#define RECEIVED_PROMPT "Received> "

// Auxillary function to receive message acknowledgement from a connection.
// Inputs:
//      connection conn: Connnection to receive from
void receive_acknowledgement(connection conn)
{
	char buff[BUFFSIZE];
	fflush(stdout);
	int len = recv(conn, buff, BUFFSIZE, 0);
	fflush(stdout);
	printf("%s%c%c\n", RECEIVED_PROMPT, buff[0], buff[1]);
	if (buff[0] != 'O' && buff[0] != 'K') {
		printf("Send failed!\n");
		exit(1);
	}
}

// Send an integer to a connection.
// Inputs:
//      connection conn: Connnection to send to
//      int k: Integer to sent
// Output:
//      0 --> Execution was successfully
//     -1 --> Error durring execution
int send_int(connection conn, int k)
{
	char buff[BUFFSIZE];
	buff[0] = k + '0';
	buff[1] = '\0';
	printf("%s%c\n", SEND_PROMPT, buff[0]);
	fflush(stdout);
	if (send(conn, buff, 2, 0) == -1) {
		return -1;
	}
	receive_acknowledgement(conn);
}

// Send an board to a connection.
// Inputs:
//      connection conn: Connnection to send to
//      char board[BOARDSIZE]: board to sent
void send_board(connection conn, char board[BOARDSIZE])
{
	printf("%s%s\n", SEND_PROMPT, board);
	int len = BOARDSIZE;
	char buff[BUFFSIZE];
	fflush(stdout);
	for (int i = 0; i < BOARDSIZE; i++) {
		buff[i] = board[i];
	}
	send(conn, buff, len, 0);
	receive_acknowledgement(conn);
}

// Auxillary function to generate a new game board.
// Inputs:
//      char board[BOARDSIZE]: Board to generate
void create_board(char board[BOARDSIZE])
{
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

// Receive a choice from connection.
// Inputs:
//      connection conn: Connnection to receive from
// Output:
//      char --> Received choice
char receive_choice(connection conn)
{
	printf(RECEIVED_PROMPT);
	char buff[BUFFSIZE];
	fflush(stdout);
	int len = recv(conn, buff, BUFFSIZE, 0);
	printf("%s%c\n", RECEIVED_PROMPT, buff[0]);
	return buff[0];
}

// Check if provided choice/move is valid for the provided board
// Inputs:
//      char choice: Choice to check
//      char board[BOARDSIZE]: Current board
// Output:
//      1 --> Valid
//      0 --> Already chosen
//     -1 --> Out of bounds
int is_correct(char choice, char board[BOARDSIZE])
{
	// Check choice is a valid board position
	if (choice != '0' && choice != '1' && choice != '2'
	    && choice != '3' && choice != '4' && choice != '5'
	    && choice != '6' && choice != '7' && choice != '8') {
		return -1;
	}
	// Get choice position index
	int index = 2;
	if (choice == '1') {
		index = 6;
	} else if (choice == '2') {
		index = 10;
	} else if (choice == '3') {
		index = 25;
	} else if (choice == '4') {
		index = 29;
	} else if (choice == '5') {
		index = 33;
	} else if (choice == '6') {
		index = 48;
	} else if (choice == '7') {
		index = 52;
	} else if (choice == '8') {
		index = 56;
	}
	// Check choice validity based on current board state
	if (board[index] == 'O' || board[index] == 'X') {
		return 0;
	}

	return 1;
}

// Set provided choice to its corresponding spot in the board
// Inputs:
//      char board[BOARDSIZE]: Current board
//      char choice: Choice to check
//      char c: Chosen value
void put_choice_on_board(char board[BOARDSIZE], char choice, char c)
{
	if (choice == '0') {
		board[2] = c;
	} else if (choice == '1') {
		board[6] = c;
	} else if (choice == '2') {
		board[10] = c;
	} else if (choice == '3') {
		board[25] = c;
	} else if (choice == '4') {
		board[29] = c;
	} else if (choice == '5') {
		board[33] = c;
	} else if (choice == '6') {
		board[48] = c;
	} else if (choice == '7') {
		board[52] = c;
	} else if (choice == '8') {
		board[56] = c;
	}
}

// Check if provided board has a winner
// Inputs:
//      char board[BOARDSIZE]: Current board
// Output:
//      1 --> Winner exists
//      0 --> No winner
//     -1 --> Out of bounds
int check_for_winner(char board[BOARDSIZE])
{
	return (board[2] == board[6] && board[6] == board[10])
	    || (board[25] == board[29] && board[29] == board[33])
	    || (board[48] == board[52] && board[52] == board[56])
	    || (board[2] == board[25] && board[25] == board[48])
	    || (board[6] == board[29] && board[29] == board[52])
	    || (board[10] == board[33] && board[33] == board[56])
	    || (board[2] == board[29] && board[29] == board[56])
	    || (board[10] == board[29] && board[29] == board[48]);
}

// Send results to a connection.
// Inputs:
//      connection conn: Connnection to send to
//      char p: Outcome
void send_results(connection conn, char p)
{
	int len;
	char buff[BUFFSIZE];
	fflush(stdout);
	if (p == 'W') {
		buff[0] = '\n';
		buff[1] = 'Y';
		buff[2] = 'o';
		buff[3] = 'u';
		buff[4] = ' ';
		buff[5] = 'w';
		buff[6] = 'o';
		buff[7] = 'n';
		buff[8] = '!';
		buff[9] = '\0';
		len = 10;
	} else if (p == 'L') {
		buff[0] = '\n';
		buff[1] = 'Y';
		buff[2] = 'o';
		buff[3] = 'u';
		buff[4] = ' ';
		buff[5] = 'l';
		buff[6] = 'o';
		buff[7] = 's';
		buff[8] = 't';
		buff[9] = '.';
		buff[10] = '\0';
		len = 11;
	} else {
		buff[0] = '\n';
		buff[1] = 'D';
		buff[2] = 'r';
		buff[3] = 'a';
		buff[4] = 'w';
		buff[5] = '.';
		buff[6] = '\0';
		len = 7;
	}

	send(conn, buff, len, 0);
}

int main(int argc, char *argv[])
{
	char pX = '-';
	char pY = '-';
	char c = 'X';
	char choice;
	char board[BOARDSIZE];
	int turn = 0;
	int ic = 0;
	char buff[BUFFSIZE];
	connection conn0, conn1, connX, connY;
	appnum a = 20000;

	// Wait for clients to connect
	printf("Game Server Waiting For Connections.\n");
	conn0 = await_contact(a);
	if (conn0 < 0) {
		exit(1);
	}
	// Inform first client for successfull connection
	connX = conn0;
	send_int(connX, 0);
	printf("First Player Connection Established.\n");

	// Wait second client
	printf("Game Server Waiting For Second Player.\n");
	conn1 = await_contact(a);
	if (conn1 < 0) {
		exit(1);
	}
	// Inform second client for successfull connection
	connY = conn1;
	send_int(connY, 1);
	printf("Second Player Connection Established.\n");

	// Genrate game board
	create_board(board);

	// Send board to second client
	send_board(connY, board);

	// Inform first client that its turn has started
	send_int(connX, turn);

	// Play the game
	while (turn < 9) {
		// Send current player the board
		send_board(connX, board);

		// While they are connected
		ic = 0;
		send_int(connX, ic);
		while (ic == 0) {
			// Receive their choice
			choice = receive_choice(connX);

			// Check if its valid
			ic = is_correct(choice, board);
			// Send confirmation
			send_int(connX, ic);
		}

		// Check if responded on time
		if (ic == -1) {
			turn = 20;
			break;
		}
		// Redraw board
		put_choice_on_board(board, choice, c);

		// Send new board to current player
		send_board(connX, board);

		// Switch current player
		if ((turn == 0) || (turn == 2) || (turn == 4) || (turn == 6)
		    || (turn == 8)) {
			connX = conn1;
			connY = conn0;
			c = 'O';
		} else {
			connX = conn0;
			connY = conn1;
			c = 'X';
		}

		// Check if board has a winner
		if (check_for_winner(board) == 1) {
			// Set winner
			if ((turn == 0) || (turn == 2) || (turn == 4)
			    || (turn == 6) || (turn == 8)) {
				pX = 'L';
				pY = 'W';
			} else {
				pY = 'W';
				pX = 'L';
			}

			break;
		}
		// Send new turn signal to current player
		turn = turn + 1;
		if (send_int(connX, turn) == -1) {
			turn = 21;
			break;
		}
	}

	// Check if current client failed, to inform the other
	if (turn == 20 || turn == 21) {
		send_int(connY, turn);
		return 0;
	}
	// Notify clients to stop
	if (turn != 9) {
		turn = 9;
		send_int(connX, turn);
		send_int(connY, turn);
	} else {
		// Check results
		if (check_for_winner(board) == 1) {
			pY = 'W';
			pX = 'L';
		}
		send_int(connY, turn);
	}

	// Broadcast final bard and results
	send_board(connX, board);
	send_board(connY, board);
	send_results(connX, pX);
	send_results(connY, pY);

	// Terminate connections
	send_eof(connX);
	send_eof(connY);

	printf("\nGame Connection Closed.\n");

	return 0;
}
