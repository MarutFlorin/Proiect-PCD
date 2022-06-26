#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <pthread.h>
#include "miniz_lib.c"
#include <sys/un.h>
#include <time.h>


// for INET socket
#define SERVER_PORT 8080
#define BUFSIZE 1024
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100

// for UNIX socket
#define SERVER_PATH "/tmp/server"
#define UN_BUFFER_LENGTH 250

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;
typedef struct sockaddr_un SA_UN;

void * handle_connection(int);
void * start_inet_server(void *arg);
void * open_unix_socket(void *arg);
void write_file(int sockfd);
void send_file(FILE *fp, int sockfd);
int accept_new_connection(int server_socket);
int setup_server(short port, int backlog);
int check(int exp, const char *msg, int exit_program);

int no_inet = 0;
int no_files = 0;
int avg_compression = 0;
double compression_time_sum = 0;
long no_bytes_compressed = 0;

pthread_t inet_thread, unix_thread;

int main (int argc, char *argv[])
{
  unlink(SERVER_PATH);
  check(pthread_create(&unix_thread, NULL, open_unix_socket, NULL), "Error: Couldn't open the UNIX socket", 0);
  check(pthread_create(&inet_thread, NULL, start_inet_server, NULL), "Error: Couldn't open the INET socket", 1);

  pthread_join(unix_thread, NULL);
  pthread_join(inet_thread, NULL);

  unlink(SERVER_PATH);

  return 0;
}

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "from_client";
  char buffer[BUFSIZE];
 
  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, BUFSIZE, 0);
    if (n <= 0){
      break;
      return;
    }

    no_bytes_compressed += n;

    fprintf(fp, "%s", buffer);
    bzero(buffer, BUFSIZE);
  }

  FILE *fp2;
  char *filename2 = "result";

  clock_t begin = clock();
  mess_file(filename, filename2, "c", '1');
  clock_t end = clock();

  double time_spent = (double)(end-begin)/CLOCKS_PER_SEC;

  no_files++;

  compression_time_sum += time_spent;
  avg_compression = (int) compression_time_sum/no_files;

  fp2 = fopen(filename2, "r");
  send_file(fp2, sockfd);
  return;
}

void send_file(FILE *fp, int sockfd){
  int n;
  char data[BUFSIZE] = {0};
 
  while(fgets(data, BUFSIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    //printf("Sent: %s\n", data);
    bzero(data, BUFSIZE);
  }
  shutdown(sockfd, SHUT_WR);
}

void *open_unix_socket(void *arg)
{
  int server_socket, client_socket, length, rc;
  SA_UN server_addr;

  char buffer[UN_BUFFER_LENGTH];

  check((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)), "(UNIX socket) Failed to create", 0);

  // initialize the address struct
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path, SERVER_PATH);

  check(bind(server_socket, (SA*)&server_addr, SUN_LEN(&server_addr)), "(UNIX socket) Bind failed", 0);
  fprintf(stdout, "(UNIX socket) Binded successfully\n");
  
  check(listen(server_socket, 10), "(UNIX socket) Listen failed", 0);
  fprintf(stdout, "Unix socket listening\n");

  while(1)
  {
    check(client_socket = accept(server_socket, NULL, NULL), "(UNIX socket) accept failed", 0);
    fprintf(stdout, "Unix socket is accepting clients\n");

    length = UN_BUFFER_LENGTH;
    check(setsockopt(client_socket, SOL_SOCKET, SO_RCVLOWAT,
                      (char *)&length, sizeof(length)), "setsockopt(SO_RCVLOWAT) failed", 0);


    while(1)
    {
      check(rc = recv(client_socket, buffer, sizeof(buffer), 0), "(UNIX socket) recv failed", 0);
    
        if (rc == 0 ||
            rc < sizeof(buffer))
            {
                printf("The client closed the connection\n");
                break;
            }

      int option = atoi(buffer);

      char *response_buffer = (char*) malloc(sizeof(char)*UN_BUFFER_LENGTH);
      if(option == 1)
      {
        snprintf(response_buffer, UN_BUFFER_LENGTH, "Number of INET clients connected: %d", no_inet);
        check(rc = send(client_socket, response_buffer, sizeof(buffer), 0), "(UNIX socket) failed to send data back to client\n", 0);
      }
      else if(option == 2)
      {
        snprintf(response_buffer, UN_BUFFER_LENGTH, "Average compression time: %d", avg_compression);
        check(rc = send(client_socket, response_buffer, sizeof(buffer), 0), "(UNIX socket) failed to send data back to client\n", 0);
      }

      else if(option == 3)
      {
        snprintf(response_buffer, UN_BUFFER_LENGTH, "Number of files sent: %d", no_files);
        check(rc = send(client_socket, response_buffer, sizeof(buffer), 0), "(UNIX socket) failed to send data back to client\n", 0);
      }

      else if (option == 4)
      {
        snprintf(response_buffer, UN_BUFFER_LENGTH, "Number of bytes compressed: %d", no_bytes_compressed);
        check(rc = send(client_socket, response_buffer, sizeof(buffer), 0), "(UNIX socket) failed to send data back to client\n", 0);
      }
    }
  }

  return EXIT_SUCCESS;
}

void* start_inet_server(void* arg)
{
  int server_socket = setup_server(SERVER_PORT, SERVER_BACKLOG);

  fd_set current_sockets, ready_sockets;
  FD_ZERO(&current_sockets);
  FD_SET(server_socket, &current_sockets);

  while (1)
  {
    //because select is destructive
    ready_sockets = current_sockets;

    check(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL), "Error in select", 1);


    for (int i=0; i<FD_SETSIZE; i++)
    {
      if (FD_ISSET(i, &ready_sockets))
      {
        if (i == server_socket)
        {
          //this is new connection
          int client_socket = accept_new_connection(server_socket);
          FD_SET(client_socket, &current_sockets);
          no_inet++;
        }
        else
        {
          //connection already exists
          handle_connection(i);
          FD_CLR(i, &current_sockets);
        }
      }
    }

    // printf("Waiting for connections...\n");

    // int client_socket = accept_new_connection(server_socket);

    // handle_connection(client_socket);
  }

  return EXIT_SUCCESS;
}

int setup_server(short port, int backlog)
{
  int server_socket;
  SA_IN server_addr;

  check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create INET socket", 1);

  // initialize the address struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(port);

  check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "(INET socket) Bind failed", 1);
  fprintf(stdout, "(INET socket) Binded successfully\n");
  
  check(listen(server_socket, backlog), "(INET socket) Listen failed", 1);
  fprintf(stdout, "Listening on port %d\n", port);

  return server_socket;
}

int accept_new_connection(int server_socket)
{
  int addr_size = sizeof(SA_IN);
  int client_socket;
  SA_IN client_addr;
  check(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size), "(INET socket) Accept failed", 1);
  
  return client_socket;
}

int check(int exp, const char *msg, int exit_program)
{
  if(exp < 0)
  {
    perror(msg);
    if(exit_program)
      exit(1);
  }

  return exp;
}

void * handle_connection(int client_socket)
{
  write_file(client_socket);
}