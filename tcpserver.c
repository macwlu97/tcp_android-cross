/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main()
{
        int sock, polaczenie, odebrane_bajty , true = 1;
        char wyslana_tresc [1024] , odebrana_tresc[1024];

        struct sockaddr_in server_addr,client_addr;
        int sin_size;

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5000);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
            perror("Nie mozna polaczyc");
            exit(1);
        }

        if (listen(sock, 5) == -1) {
            perror("Listen");
            exit(1);
        }

	printf("\nTCPServer Czeka na klienta na porcie 5000");
        fflush(stdout);


        while(1)
        {

            sin_size = sizeof(struct sockaddr_in);

            polaczenie = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

            printf("\n Odebralem polaczenie z (%s , %d)",
                   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

            while (1)
            {
              printf("\n Wyslij (q lub Q aby zamknac) : ");
              gets(wyslana_tresc);

              if (strcmp(wyslana_tresc , "q") == 0 || strcmp(wyslana_tresc , "Q") == 0)
              {
                send(polaczenie, wyslana_tresc,strlen(wyslana_tresc), 0);
                close(polaczenie);
                break;
              }

              else
                 send(polaczenie, wyslana_tresc,strlen(wyslana_tresc), 0);

              odebrane_bajty = recv(polaczenie,odebrana_tresc,1024,0);

              odebrana_tresc[odebrane_bajty] = '\0';

              if (strcmp(odebrana_tresc , "q") == 0 || strcmp(odebrana_tresc , "Q") == 0)
              {
                close(polaczenie);
                break;
              }

              else
              printf("\n Odebrana tresc = %s " , odebrana_tresc);
              fflush(stdout);
            }
        }

      close(sock);
      return 0;
}
