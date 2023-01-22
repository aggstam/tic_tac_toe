// -------------------------------------------------------------
//
// Simple Tic Tac Toe game between two players(clients) connected to a server,
// using Douglas E. Comer's CNAI Socket API.
// This code implements the client.
//
// Author: Aggelos Stamatiou, December 2015
//
// --------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <cnaiapi.h>

// Execution constants
#define BUFFSIZE        256
#define INPUT_PROMPT    "Input   > "
#define RECEIVED_PROMPT "Received> "
int len;
char buff[BUFFSIZE];

// Auxillary function to read the execution configuration.
// Inputs:
//      computer* comp: Host to set
//      appnum* a: Port number
void get_conf(computer* comp, appnum* a) {
    char compname[BUFFSIZE];
    char appname[BUFFSIZE];
    FILE *file = fopen("conf", "r");
    if (file == NULL) {
        printf("Error at opening file.");
        exit(1);
    }
    fscanf(file, "%s\n%s", compname, appname);
    fclose(file);
    *comp = cname_to_comp(compname);
    *a = (appnum)atoi(appname);
}

// Receive an integer from connection.
// Inputs:
//      connection conn: Connnection to receive from
// Output:
//      int --> Received integer
int receive_int(connection conn) {
    char buff[BUFFSIZE];
    fflush(stdout);
    int len = recv(conn, buff, BUFFSIZE, 0);
    return buff[0] - '0';
}

// Receive board from connection.
// Inputs:
//      connection conn: Connnection to receive from
void receive_board(connection conn) {
    printf(RECEIVED_PROMPT);
    char buff[BUFFSIZE];
    int len = recv(conn, buff, BUFFSIZE, 0);
    fflush(stdout);
    write(STDOUT_FILENO, buff, len);
}

// Send provided connection.
// Inputs:
//      connection conn: Connnection to receive from
// Output:
//      0 --> Execution was successfully
//     -1 --> Error durring execution
int send_choice(connection conn){
    char buff[BUFFSIZE];
    int len;
    time_t begin;
    time_t end;

    while (1) {
        // Retrieve choice
        printf(INPUT_PROMPT);
        begin = time(NULL);
        fflush(stdout);
        len = readln(buff, BUFFSIZE);
        end = time(NULL);
        // Check length and time
        if ((len > 0) && ((end - begin) < 60)){
            // Send choice
            send(conn, buff, len, 0);
            // Wait response
            if (receive_int(conn) != 0) {
                break;
            }
        } else {
            buff[0] = 'O';
            buff[1] = 'Y';
            buff[2] = 'T';
            buff[3] = ' ';
            buff[4] = 'O';
            buff[5] = 'F';
            buff[6] = ' ';
            buff[7] = 'T';
            buff[8] = 'I';
            buff[9] = 'M';
            buff[10] = 'E';
            send(conn, buff, 11, 0);
            return -1;
        }

        printf("Chose another board number, that's already filled.\n");
    }

    return 0;
}

// Receive results from connection.
// Inputs:
//      connection conn: Connnection to receive from
void receive_results(connection conn) {
    char buff[BUFFSIZE];
    int len = recv(conn, buff, BUFFSIZE, 0);
    fflush(stdout);
    write(STDOUT_FILENO, buff, len);
}

int main(int argc, char* argv[]) {
    computer comp;
    connection conn;
    appnum a;
    int id, turn;

    // Retrieve connection configuration
    get_conf(&comp, &a);

    // Connect to server
    conn = make_contact(comp, a);
    if (conn < 0) {
        exit(1);
    }

    printf("Game Connection Established.\n");

    // Check if another player is connected
    id = receive_int(conn);
    if (id == 0) {
        printf("Waiting for second player.\n");
    } else if (id == 1) {
        receive_board(conn);
    }

    // Wait until turn starts
    turn = receive_int(conn);
    while (turn < 9) {
        receive_board(conn);
        printf("Chose a board number to put your move.\n");
        if (send_choice(conn)) {
            turn = 10;
            break;
        }
        receive_board(conn);
        turn = receive_int(conn);
    }

    // Check if game failed
    if (turn == 10 || turn == 20 || turn == 21) {
        if (turn == 10) {
            printf("You were out of time or send an empty anser.\n");
        } else if (turn == 20) {
            printf("Your opponent did not manage to anser on time or send an empty anser.\n");
        } else {
            printf("Your opponent disconnected.\n");
        }
    } else {
        // Receive final board and results
        receive_board(conn);
        receive_results(conn);
        send_eof(conn);
    }

    printf("\nGame Connection Closed.\n\n");

    return 0;

}
