
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/server"
#define BUFFER_LENGTH 250
#define FALSE 0
#define TRUE 1

void main(int argc, char *argv[])
{

    int sd = -1, rc, bytesReceived;
    char buffer[BUFFER_LENGTH];
    struct sockaddr_un serveraddr;
    int opt;

    do
    {

        sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sd < 0)
        {
            perror("socket() failed");
            break;
        }

        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sun_family = AF_UNIX;
        if (argc > 1)
            strcpy(serveraddr.sun_path, argv[1]);
        else
            strcpy(serveraddr.sun_path, SERVER_PATH);

        rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
        if (rc < 0)
        {
            perror("connect() failed");
            break;
        }
        printf("Connected to server:\n\n");

        do
        {
            printf("1. Number of INET clients connected\n");
            printf("2. Average compression time\n");
            printf("3. Number of files sent\n");
            printf("4. Number of bytes compressed\n");
            printf("5. Exit\n");
            printf("Choose an option: ");
            scanf("%d", &opt);
            printf("\n");

            if (opt == 5)
            {
                break;
            }

            if (opt < 1 || opt > 5)
            {
                printf("Wrong input, try again\n\n");
            }
            else
            {
                sprintf(buffer, "%d", opt);
                rc = send(sd, buffer, sizeof(buffer), 0);
                if (rc < 0)
                {
                    perror("send() failed");
                    break;
                }

                rc = recv(sd, &buffer[0],
                          BUFFER_LENGTH, 0);
                if (rc < 0)
                {
                    perror("recv() failed");
                    break;
                }
                else if (rc == 0)
                {
                    printf("The server closed the connection\n");
                    break;
                }
                else
                {
                    printf("%s\n", buffer);
                }
            }
        } while (1);

    } while (FALSE);

    if (sd != -1)
        close(sd);
}