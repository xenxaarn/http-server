#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
int main()
{
    // SOCKET
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket Error");
    return -1;
}
    // Address structure
    struct server_fd *bind;

    return 0;
}
