#include "server.h"

bool is_get(char *buffer)
{
    if (strncmp(buffer, "GET", 3) == 0)
        return true;
    return false;
}

char *get_root(char *buffer)
{
    char *root = malloc(sizeof(char) + 1);
    int i = 0;
    int size = 0;

    for (i = 0; buffer[i] != '/'; i++);
    for (int j = i; buffer[j + 1] != 'H'; j++) {
        if (size >= 1)
            root = realloc(root, sizeof(char) * size + 2);
        root[size++] = buffer[j];
    }
    root[size] = '\0';
    return root;
}

char *get_file_type(char *file)
{
    char *dot = strrchr(file, '.');

    if(!dot)
        return NULL;
    return dot + 1;
}

const char *get_content_type(const char *file)
{
    if (file == NULL)
        return "text/html";
    else if (strcmp(file, "html") == 0 || strcmp(file, "htm") == 0)
        return "text/html";
    else if (strcmp(file, "txt") == 0)
        return "text/plain";
    else if (strcmp(file, "jpg") == 0 || strcmp(file, "jpeg") == 0)
        return "image/jpeg";
    else if (strcmp(file, "png") == 0)
        return "image/png";
    else
        return "application/octet-stream";
}

char *build_header(const char *type)
{
    const char *mime_type = get_content_type(type);
    char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "\r\n",
             mime_type);
    return header;
}

int get_fd(const char *file)
{
    char *path = malloc(sizeof(char) * (strlen("./src") + strlen(file)) + 1);
    strcpy(path, "./src");
    strcat(path, file);
    int fd = open(path, O_RDONLY);
    free(path);
    return fd;
}

char *get_response(const char *header, int fd, int *len_response)
{
    struct stat file_stat;
    fstat(fd, &file_stat);
    off_t file_size = file_stat.st_size;
    char *response = malloc(sizeof(char) * (file_size + strlen(header)) + 1);
    char *tmp = malloc(sizeof(char) * (file_size) + 1);
    read(fd, tmp, file_size);
    strcpy(response, header);
    memcpy(response + strlen(header), tmp, sizeof(char) * file_size);
    (*len_response) += file_size;
    free(tmp);
    close(fd);
    return response;
}

char *build_http_response(const char *path, const char *type,
int *len_response)
{
    char *response = NULL;
    char *header = build_header(type);
    int fd = 0;

    if (strcmp(path, "/") == 0 && type == NULL) {
        fd = open("./src/main.html", O_RDONLY);
    } else {
        fd = get_fd(path);
    }
    if (fd == -1) {
        response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "404 Not Found";
        (*len_response) = strlen(response);
        return response;
    }
    (*len_response) += strlen(header);
    response = get_response(header, fd, len_response);
    return response;
}

void *handle_client(void *arg)
{
    int client_fd = *(int *)arg;
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int len_response = 0;

    ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_read > 0 && is_get(buffer) && client_fd > 0) {
        buffer[bytes_read] = '\0';
        char *root = get_root(buffer);
        char *file_type = get_file_type(root);
        char *response = build_http_response(root, file_type, &len_response);
        printf("\n\rGET %s\n", root);
        write(client_fd, response, len_response);
        close(client_fd);
        if (response != NULL)
            free(response);
    }
    if (buffer != NULL)
        free(buffer);
    close(client_fd);
    return NULL;
}
