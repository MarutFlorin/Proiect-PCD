#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

// void send_file(FILE *fp, int sockfd, int cmpOrDcmp)
// {
//     int n;
//     char data[SIZE] = {0};

//     while (fgets(data, SIZE, fp) != NULL)
//     {
//         // strcpy(data, data + 1);
//         // data[0] = cmpOrDcmp + '0';
//         if (send(sockfd, data, sizeof(data), 0) == -1)
//         {
//             perror("Error in sending file.");
//             exit(1);
//         }
//         bzero(data, SIZE);
//     }
// }

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};
 
  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
  shutdown(sockfd, SHUT_WR);
}

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "from_server";
  char buffer[SIZE];
 
  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e, opt;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed()");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e == -1)
    {
        perror("connect() failed");
        exit(1);
    }
    printf("Connected to server:\n\n");
    printf("1.Compress file\n");
    printf("2.Decompress file\n");
    printf("3.Exit\n");
    printf("Choose an option: ");
    scanf("%d", &opt);
    if (opt == 3)
    {
      close(sockfd);

      return 0;
    }
    if (opt == 1 || opt == 2)
    {
        printf("\n\nEnter path to file: \n");
        scanf("%s", filename);

        fp = fopen(filename, "r");
        if (fp == NULL)
        {
            perror("fopen() failed");
            exit(1);
        }

        send_file(fp, sockfd);

        printf("File sent succesfuly\n");

        write_file(sockfd);
        printf("Compressed file received succesfully\n");
    }
    if (opt < 1 || opt > 2)
    {
        printf("\nWrong input, try again\n\n");
    }

    close(sockfd);

    return 0;
}
