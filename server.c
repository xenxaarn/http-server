#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  // socket
  struct sockaddr_in my_addr;
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Socket Error");
    return -1;
  }

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(8080);                        // port
  my_addr.sin_addr.s_addr = inet_addr("192.168.18.255"); // ip
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  // bind
  struct sockaddr addr;
  int bind_result =
      bind(socket_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));
  printf("Bind result is: %d\n", bind_result);

  if (bind_result < 0) {
    perror("Bind error");
    return 1;
  }

  // listening
  int backlog = 15;
  int listen_res = listen(socket_fd, backlog);

  printf("Listen result is: %d\n", listen_res);

  close(socket_fd);

  return 0;
}
