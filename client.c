#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#define MAXLEN 100

int main() {

    int client = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    struct timeval timeout = {0, 30};
    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8700);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect!\n");
        exit(-1);
    }
    char *msj = calloc(MAXLEN, sizeof(char));
    char *response = calloc(MAXLEN, sizeof(char));
    int bytes_send;
    int bytes_received;

    while(1) {
        bytes_received = recv(client, response, MAXLEN, 0);
        if (bytes_received > 0) {
            printf("Bytes received = %d\n", bytes_received);
            printf("Server response is: %s\n", response);
        }

        fgets(msj, MAXLEN, stdin);
        if (msj[strlen(msj) - 1] == '\n')
            msj[strlen(msj) - 1] = '\0';
        bytes_send = send(client, msj, strlen(msj), 0);
        if (bytes_send == 0)
            break;
        printf("Bytes send = %d\n", bytes_send);
        memset(msj, 0, MAXLEN);

    }
    free(msj);
    free(response);
    return 0;
}
