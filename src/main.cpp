#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

constexpr int PORT = 3232;
constexpr int BUFFER_SIZE = 1024;
constexpr int POOL_SIZE = 10;
constexpr const char *WELCOME_MESSAGE = "Hello!\n";
constexpr const char *GOODBYE_MESSAGE = "Bye!";
constexpr const int MAX_EVENTS = 10;

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0);

void listening_thread(int *socket_fd) { auto x = *socket_fd; }

int do_something_with_fd(int _fd) {
  printf("yebro doing something with this fd:%d\n", _fd);
  return 0;
}

int handle_connection(int c) {
  char *buf = (char *)malloc(BUFFER_SIZE);
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
  free(buf);
  return 0;
}

int setnonblocking(int sock) {
  int result;
  int flags;

  flags = fcntl(sock, F_GETFL, 0);

  if (flags == -1) {
    return -1; // error
  }

  flags |= O_NONBLOCK;

  result = fcntl(sock, F_SETFL, flags);
  return result;
}

int main(int argc, char *argv[]) {
  // we need to bind to a socket
  struct sockaddr_in serverSa;
  struct sockaddr_in clientSa;

  // variables for setting up the socket
  int rc;
  socklen_t clientSaSize = sizeof(clientSa);
  int on = 1;
  int c;
  int listen_sock, conn_sock;

  // variables for epoll
  struct epoll_event ev,
      events[MAX_EVENTS]; // define `ev` for single
                          // epoll_event, and `events` for MAX_EVENTS
  int nfds, epoll_fd;

  // initialise a socket
  listen_sock = socket(PF_INET, SOCK_STREAM,
                       0); // protocol is 0 so one is chosen automatically
  // set the socket options
  rc =
      setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on); // here
  // we set some options for the socket (via the file descriptor). In this case
  // we are setting an option at the socket level, this is because we set the
  // level argument to `SOL_SOCKET`
  memset(&serverSa, 0, sizeof(serverSa));
  serverSa.sin_family = AF_INET;
  serverSa.sin_addr.s_addr = htonl(INADDR_ANY);
  serverSa.sin_port = htons(PORT);

  // bind server socket address to socket
  rc = bind(listen_sock, (struct sockaddr *)&serverSa, sizeof(serverSa));

  if (rc < 0) {
    handle_error("failed to bind");
  }

  // mark `listen_sock` as a passive socket
  rc = listen(listen_sock, 10);

  if (rc < 0) {
    handle_error("Failed to listen");
  }

  // start using epoll

  epoll_fd = epoll_create1(0);

  if (epoll_fd == -1) {
    handle_error("Failed to create epoll instance");
  }

  ev.events = EPOLLIN;
  ev.data.fd = listen_sock;

  rc = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev); // this is
  // registering the listen_sock file descriptor. We want it to be on the
  // interest list when the file is available for read operations (i.e. someone
  // has connected), the only data we want the kernel to give us is the actual
  // listen_sock file descriptor

  if (rc == -1) {
    handle_error("Failed to register file descriptor with epoll")
  }

  // main function loop
  while (1) {
    nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // returns the number
    // of file descriptors available and populates these in the events array

    if (nfds == -1) {
      handle_error("epoll_wait error");
    }

    for (int n = 0; n < nfds; n++) {
      if (events[n].data.fd == listen_sock) {
        conn_sock =
            accept(listen_sock, (struct sockaddr *)&clientSa, &clientSaSize);

        if (conn_sock < 0) {
          perror("accept failed");
          exit(1);
        }

        printf("Client address is: %s:%d\n", inet_ntoa(clientSa.sin_addr),
               clientSa.sin_port);

        setnonblocking(conn_sock);
        ev.events = EPOLLIN | EPOLLET; // edge triggered
        ev.data.fd = conn_sock;
        rc = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_sock, &ev);

        if (rc == -1) {
          handle_error(
              "Failed to register connection socket in epoll instance");
        }
      } else {
        do_something_with_fd(events[n].data.fd);
      }
    }

    c = rc;
    rc = write(c, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));

    std::thread client_thread(handle_connection, c);
  }

  shutdown(listen_sock, SHUT_RDWR);
  close(listen_sock);
  return 0;
}
