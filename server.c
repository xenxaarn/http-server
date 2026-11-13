#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

int main(void) {
  signal(SIGPIPE, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);

  struct sockaddr_in my_addr;
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("socket");
    return EXIT_FAILURE;
  }

  int opt = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);

  memset(&my_addr, 0, sizeof my_addr);
  my_addr.sin_family = AF_INET;   //ipv4
  my_addr.sin_port = htons(8080); //port
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY); //any-ip

  // bind
  if (bind(socket_fd, (struct sockaddr *)&my_addr, sizeof my_addr) < 0) {
    perror("bind");
    close(socket_fd);
    return EXIT_FAILURE;
  }
  // listening
  if (listen(socket_fd, 15) < 0) {
    perror("listen");
    close(socket_fd);
    return EXIT_FAILURE;
  }
  printf("Listening on port 8080 ...\n");

  while (1) {
    printf("Waiting for connections ...\n");

    // accept
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    int new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);

    if (new_fd < 0) {
      if (errno == EINTR) continue;
      perror("accept");
      continue;
    }
    printf("Client connected\n");

    // send
    struct timeval tv = {.tv_sec = 5, .tv_usec = 0};
    setsockopt(new_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);

    char buffer[2048];
    ssize_t bytes_received = recv(new_fd, buffer, sizeof buffer - 1, 0);

    if (bytes_received > 0) {
      buffer[bytes_received] = '\0';
      printf("Client: %s\n", buffer);

      const char *body = "<html><body><h1>Hello, World!!</h1></body></html>";
      char response[512];
      int n = snprintf(response, sizeof response,
                       "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-Length: %zu\r\n"
                       "Connection: close\r\n"
                       "\r\n"
                       "%s",
                       strlen(body), body);

      size_t sent = 0, len = (size_t)n;
      while (sent < len) {
        ssize_t w = send(new_fd, response + sent, len - sent, 0);
        if (w <= 0) {
          if (w < 0 && errno == EINTR) continue;
          break;
        }
        sent += (size_t)w;
      }
    } else if (bytes_received == 0) {
      printf("Client: closed without sending data\n");
    } else {
      if (errno != EAGAIN && errno != EWOULDBLOCK) perror("recv");
    }

    close(new_fd);
  }
  close(socket_fd);
  return EXIT_SUCCESS;
}
