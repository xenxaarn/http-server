#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main()
{
    // SOCKET
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Socket Error");
    return -1;
}
    // bind
    struct sockaddr addr;
    int bind_result = bind(socket_fd, &addr, sizeof(struct sockaddr));

    printf("Bind result is: %d\n", bind_result);
 
    return 0;
}
