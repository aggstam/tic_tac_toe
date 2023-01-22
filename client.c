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

// Auxillary function to send message acknowledgement to a connection.
// Inputs:
//      connection conn: Connnection to send to
void send_acknowledgement(connection conn) {
    char buff[BUFFSIZE];
    fflush(stdout);
    buff[0] = 'O';
    buff[1] = 'K';
    buff[2] = '\0';
    send(conn, buff, 2, 0);
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
    int received = buff[0] - '0';
    printf("%s{int} %d\n", RECEIVED_PROMPT, received);
    send_acknowledgement(conn);
    return received;
}

// Receive buffer from connection.
// Inputs:
//      connection conn: Connnection to receive from
void receive_buffer(connection conn) {
    char buff[BUFFSIZE];
    printf("%s{buffer}", RECEIVED_PROMPT);
    fflush(stdout);
    int len = recv(conn, buff, BUFFSIZE, 0);
    fflush(stdout);
    write(STDOUT_FILENO, buff, len);
    send_acknowledgement(conn);
}

// Send provided connection.
// Inputs:
//      connection conn: Connnection to receive from
// Output:
//      0 --> Execution was successfully
//     -1 --> Error durring execution
int send_choice(connection conn){
    char buff[BUFFSIZE];
    int len, resp;
    time_t begin;
    time_t end;

    resp = receive_int(conn);
    while (resp == 0) {
        // Retrieve choice
        printf("Input> ");
        begin = time(NULL);
        fflush(stdout);
        len = readln(buff, BUFFSIZE);
        end = time(NULL);
        // Check length and time
        if ((len > 0) && ((end - begin) < 60)){
            // Send choice
            if (send(conn, buff, len, 0) == -1) {
                return -1;
            }
            // Wait response
            resp = receive_int(conn);
            if (resp == 0) {
                printf("Chose another board number, that's already filled.\n");
                continue;
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
            buff[11] = '\0';
            send(conn, buff, 12, 0);
            return -1;
        }
    }

    return 0;
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
        receive_buffer(conn);
        printf("Waiting for other player...\n");
    }

    // Wait until turn starts
    turn = receive_int(conn);
    while (turn < 9) {
        printf("Turn signal received: %d\n", turn);
        receive_buffer(conn);

        printf("Chose a board number to put your move.\n");
        if (send_choice(conn) == -1) {
            turn = 10;
            break;
        }

        receive_buffer(conn);
        printf("Waiting for other player...\n");
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
        receive_buffer(conn);
        receive_buffer(conn);
    }

    send_eof(conn);

    printf("\nGame Connection Closed.\n");

    return 0;
}
