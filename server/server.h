#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <unistd.h>
#include <complex.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PORT 8000
#define SERVER_ERROR 84
#define BUFFER_SIZE 104857600

#pragma once


typedef struct server {
    int socket_fd;
    struct sockaddr_in server_addr;
} server;

server *create_socket(void);
void bind_server(server *s);
void listen_server(server *s);
void waiting_for_clients(server *s);
void *handle_client(void *arg);
