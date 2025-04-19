#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr int PORT = 3232;
constexpr int BUFFER_SIZE = 1024;
constexpr const char *WELCOME_MESSAGE = "Hello!\n";
constexpr const char *GOODBYE_MESSAGE = "Bye!";

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main(int argc, char *argv[]) {
  // we need to bind to a socket
  struct sockaddr_in serverSa;
  struct sockaddr_in clientSa;

  int rc;
  socklen_t clientSaSize = sizeof(clientSa);
  int on = 1;
  int c;

  char *buf = (char *)malloc(BUFFER_SIZE);

  int s = socket(PF_INET, SOCK_STREAM,
                 0); // protocol is 0 so one is chosen automatically
  rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on); // here
  // we set some options for the socket (via the file descriptor). In this case
  // we are setting an option at the socket level, this is because we set the
  // level argument to `SOL_SOCKET`
  memset(&serverSa, 0, sizeof(serverSa));
  serverSa.sin_family = AF_INET;
  serverSa.sin_addr.s_addr = htonl(INADDR_ANY);
  serverSa.sin_port = htons(PORT);
  rc = bind(s, (struct sockaddr *)&serverSa, sizeof(serverSa));
  if (rc < 0) {
    handle_error("failed to bind");
  }

  // main function loop
  while (1) {
    rc = listen(s, 10);

    if (rc < 0) {
      handle_error("Failed to listen");
    }

    rc = accept(s, (struct sockaddr *)&clientSa, &clientSaSize);

    if (rc < 0) {
      perror("accept failed");
      exit(1);
    }

    printf("Client address is: %s:%d\n", inet_ntoa(clientSa.sin_addr),
           clientSa.sin_port);

    c = rc;
    rc = write(c, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));

    while (1) {
      memset(buf, 0, BUFFER_SIZE);
      int read_bytes = read(c, buf, BUFFER_SIZE - 1);
      printf("<-- %s\n", buf);
      printf("--> %s\n", buf);
      write(c, buf, read_bytes);
      if (strncmp((char *)buf, GOODBYE_MESSAGE, strlen(GOODBYE_MESSAGE)) == 0) {
        printf("Goodbye received from client!\n");
        close(c);
        printf("Closed socket for client\n");
        break;
      }
    }
  }

  free(buf);
  shutdown(s, SHUT_RDWR);
  close(s);
  return 0;
}
