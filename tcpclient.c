/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char *argv[])

{

        int sock, odebrane_bajty;
        char wyslana_tresc[1024],odebrana_tresc[1024];
        struct hostent *host;
        struct sockaddr_in server_addr;
        const char *server_ip = argc > 1 ? argv[1] : "127.0.0.1";
        host = gethostbyname(server_ip);

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5000);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(server_addr.sin_zero),8);

        if (connect(sock, (struct sockaddr *)&server_addr,
                    sizeof(struct sockaddr)) == -1)
        {
            perror("Polaczono");
            exit(1);
        }

        while(1)
        {

          odebrane_bajty=recv(sock,odebrana_tresc,1024,0);
          odebrana_tresc[odebrane_bajty] = '\0';

          if (strcmp(odebrana_tresc , "q") == 0 || strcmp(odebrana_tresc , "Q") == 0)
          {
           close(sock);
           break;
          }

          else
           printf("\nOderbana tresc = %s " , odebrana_tresc);

           printf("\nWyslij (q lub Q aby zamknac) : ");
           gets(wyslana_tresc);

          if (strcmp(wyslana_tresc , "q") != 0 && strcmp(wyslana_tresc , "Q") != 0)
           send(sock,wyslana_tresc,strlen(wyslana_tresc), 0);

          else
          {
           send(sock,wyslana_tresc,strlen(wyslana_tresc), 0);
           close(sock);
           break;
          }

        }
return 0;
}
