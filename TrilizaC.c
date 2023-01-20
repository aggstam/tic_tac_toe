#include <stdlib.h>
#include <stdio.h>
#include <cnaiapi.h>
#include <time.h>

#define BUFFSIZE 256
#define INPUT_PROMPT "Input   > "
#define RECEIVED_PROMPT    "Received> "
int        len;
char    buff[BUFFSIZE];

int recvln(connection, char *, int);
int readln(char *, int);
void receive_board(connection conn);
int send_choice(connection conn, int ic);
int receive_int(connection conn);
void receive_results(connection conn);
void get_conf(computer *comp, appnum *a);

int main(int argc, char *argv[]) {
    computer comp;
    connection conn;
    appnum a;
    int id;

    get_conf(&comp, &a);

    conn = make_contact(comp, a);
    if (conn < 0)
        exit(1);
    printf("Game Connection Established.\n");

    id = receive_int(conn);
    if (id == 0)
        printf("Waiting for second player.\n");
    if (id == 1)
        receive_board(conn);

    int ic;
    int turn = receive_int(conn);
    while (turn < 9) {
        receive_board(conn);
        printf("Chose a board number to put your move.\n");
        if (send_choice(conn, ic)) {
            turn = 10;
            break;
        }
        receive_board(conn);
        turn = receive_int(conn);
    }

    if (turn == 10 || turn == 20 || turn == 21) {
        if (turn == 10) {
            printf("You were out of time or send an empty anser.\n");
        }
        else if (turn == 20) {
            printf("Your opponent did not manage to anser on time or send an empty anser.\n");
        }
        else {
            printf("Your opponent disconnected.\n");
        }
    }
    else {
        receive_board(conn);
        receive_results(conn);
        send_eof(conn);
    }
    printf("\nGame Connection Closed.\n\n");

    return 0;

}

void receive_board(connection conn) {
    int len;
    char buff[BUFFSIZE];

    printf(RECEIVED_PROMPT);
    len = recv(conn, buff, BUFFSIZE, 0);
    fflush(stdout);
    write(STDOUT_FILENO, buff, len);

}

void receive_results(connection conn) {
    int len;
    char buff[BUFFSIZE];

    len = recv(conn, buff, BUFFSIZE, 0);
    fflush(stdout);
    write(STDOUT_FILENO, buff, len);

}

int send_choice(connection conn,int ic){
    char    buff[BUFFSIZE];
    int     len;
    time_t begin;
    time_t end;


    ic = receive_int(conn);
    while (ic == 0) {
        printf(INPUT_PROMPT);
        begin = time(NULL);
        fflush(stdout);
        len = readln(buff, BUFFSIZE);
        end = time(NULL);
        if((len > 0) && ((end - begin) < 10)){
            send(conn, buff, len, 0);
            ic = receive_int(conn);
            if (ic == 0) {
                printf("Chose another board number, that's already filled.\n");
            }
        }
        else {
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
            ic = -1;
            return -1;
            break;
        }
    }

    return 0;

}

int receive_int(connection conn) {
    int k;
    int len;
    char buff[BUFFSIZE];

    fflush(stdout);
    len = recv(conn, buff, BUFFSIZE, 0);
    k = buff[0] - '0';
    return k;
}

void get_conf(computer *comp, appnum *a) {

    char compname[BUFFSIZE];
    char appname[BUFFSIZE];

    FILE *file = fopen("triliza.conf", "r");
    if (file == NULL) {
        printf("Error at opening file.");
        exit(1);
    }

    fscanf(file, "%s\n%s", compname, appname);
    fclose(file);

    *comp = cname_to_comp(compname);
    *a =(appnum) atoi(appname);

}
