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
    perror("Socket Error\n");
    return -1;
  }

  my_addr.sin_family = AF_INET;         // ipv4
  my_addr.sin_port = htons(8080);       // port
  my_addr.sin_addr.s_addr = INADDR_ANY; // any ip
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  // bind
  struct sockaddr addr;
  int bind_result =
      bind(socket_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));
  printf("Bind result is: %d\n", bind_result);

  if (bind_result < 0) {
    perror("Bind error\n");
    return -1;
  }

  // listening
  int backlog = 15;
  int listen_res = listen(socket_fd, backlog);

  printf("Listen result is: %d\n", listen_res);

  while (1) {
    printf("Waiting for connections ...\n");

    // accept
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    int new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);

    if (new_fd < 0) {
      perror("Accept Error\n");
      continue;
    }
    printf("Client Connected\n");

    // send
    const char *http_response = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/html\r\n"
                                "Content-Length: 46\r\n"
                                "Connection: close\r\n"
                                "\r\n"
                                "<html><body><h1>Hello!!</h1></body></html>";

    char buffer[2048] = {0};
    ssize_t bytes_received = recv(new_fd, buffer, sizeof(buffer) - 1, 0);
    // recv
    if (bytes_received > 0) {
      printf("Client :%s\n", buffer);
      send(new_fd, http_response, strlen(http_response), 0);
    }
    close(new_fd);
  }
  close(socket_fd);
  return 0;
}
