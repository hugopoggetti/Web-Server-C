#include "server.h"

server *create_socket(void)
{ 
    server *s = malloc(sizeof(server));
    int opt = 1;
    // create socket
    s->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s->socket_fd < 0) {
        perror("socket cration failed");
        exit(84);
    }
    // use setsockopt to avoid time laps during lunching server
    if (setsockopt(
    s->socket_fd,
    SOL_SOCKET,
    SO_REUSEADDR,
    &opt,
    sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // IPv4 Internet protocols
    s->server_addr.sin_family = AF_INET;
    // define PORT for the server with big endien
    s->server_addr.sin_port = htons(PORT);
    // accept connection from any interface
    s->server_addr.sin_addr.s_addr = INADDR_ANY;
    return s;
}

void bind_server(server *s)
{
    if ((bind(s->socket_fd, (struct sockaddr *)&s->server_addr, sizeof(s->server_addr))) < 0) {
        perror("bind failed");
        exit(SERVER_ERROR);
    }
}

void listen_server(server *s)
{
    if (listen(s->socket_fd, 10) < 0) {
        perror("listen failed");
        exit(SERVER_ERROR);
    }

}

void waiting_for_clients(server *s)
{
    printf("WAIING FOR CONNECTION ON PORT %s%d\n\n", "http://localhost:", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client_fd;

        if ((client_fd = accept(s->socket_fd,
        (struct sockaddr *)&client_addr, &addrlen)) < 0) {
            perror("accept failed");
            continue;
        }
        //create thread to handle client connection
        pthread_t thread_id;
        pthread_create(
        &thread_id,
        NULL,
        handle_client,
        (void *)&client_fd);
        pthread_detach(thread_id);
    }
    close(s->socket_fd);
}

int main(void)
{
    server *s = create_socket();
    bind_server(s);
    listen_server(s);
    waiting_for_clients(s);
    free(s);
    return 0;
}

