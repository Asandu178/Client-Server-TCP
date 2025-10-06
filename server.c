#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define BACKLOG 10
#define MAXLEN 100
#define POOL 20

void handle_connection(int client_socket) {
    char *buf = calloc(100, sizeof(char));
    char *hello = "hi there partner";
    send(client_socket, hello, strlen(hello), 0);
    while(1) {
        int bytes_read = recv(client_socket, buf, MAXLEN, 0);
        if (bytes_read <= 0)
            break;
        printf("%s\n", buf);
        memset(buf, 0, MAXLEN);
    }
    free(buf);

}

/*pthread_t *init_threads(int num) {
    pthread_t *threads = malloc(num * sizeof(threads));
    if (!threads)
        return NULL;
    for (int i = 0 ; i < num ; i++) {
        pthread_create(&threads[i], NULL, handle_connection, client_socket);
    }
}*/

int main() {
    int server = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8700);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (server == -1) {
        printf("Error creating socket!\n");
        exit(-1);
    }

    if (bind(server, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Couldnt bind!\n");
        exit(-1);
    }

    if (listen(server, BACKLOG) == 0) {
        printf("Listening on %d\n", ntohs(server_addr.sin_port));
    } else {
        return -1;
    }

    int client_socket;
    char *greet = "Hello from server!";

    while(1) {
        printf("Awaiting connections...\n");
        client_socket = accept(server, NULL, NULL);
        if (client_socket < 0) {
            printf("Connection failed!\n");
        } else {
            printf("Connection succesful!\n");
        }
        handle_connection(client_socket);
    }


    shutdown(server, SHUT_RD);
    return 0;
}
