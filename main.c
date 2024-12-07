#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define PORT 4711
#define REPLY_MESSAGE "Reply"
#define MAXLINE 4096

// HTTP-Statuscodes und Antworten definieren
#define RESPONSE_400 "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n"
#define RESPONSE_404 "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n"
#define RESPONSE_501 "HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\n\r\n"

int main (int argc, char **argv)
{
    //Deklaration von Variablen

    int                 listenfd, connfd;
    struct sockaddr_in  servaddr;
    uint8_t             buff[MAXLINE+1];
    uint8_t             recvline[MAXLINE+1];

    // Aufgabe 2.1
    // TCP-Socket erstellen und konfigurieren
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // Die Adresse und Port definieren
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family        = AF_INET;               // Familie des Sockets
    servaddr.sin_addr.s_addr   = htonl(INADDR_ANY);     // IP-Adresse binden
    servaddr.sin_port          = htons (PORT);         // Portnummer binden

    // Bind (zum Binden des Sockets an eine Adresse)
    if ((bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    // Listen (um auf eingehende Verbindungen zu warten)
    if((listen(listenfd, 10)) < 0) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    // setsockopt SO_REUSEADDR muss noch implementiert werden

    // Aufgabe 2.2 Anfragen beantworten
    // Bei jedem empfangenen Paket mit dem String "Reply" antworten
    // accept(annehmen), recv(empfangen), send(senden) implementieren

    // Aufgabe 2.3 Pakete erkennen
    // Auf jedes empfangene HTTP-Paket soll mit dem String "Reply\r\n\r\n" geantwortet werden

    // Server startet und wartet auf eingehende Verbindungen
    printf("Server läuft auf Port %d...\n", PORT);

    while (1) {
        // Verbindung akzeptieren
        if ((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) < 0) {
            perror("accept error");
            continue;
        }

        // Daten vom Client empfangen
        int n = recv(connfd, recvline, MAXLINE, 0);
        if (n < 0) {
            perror("recv error");
            close(connfd);
            continue;
        }

        recvline[n] = 0; // Null-terminieren
        printf("Empfangene Anfrage: \n%s\n", recvline);

        // Aufgabe 2.4 Syntaktisch korrekte Antworten
        // Auf jede Anfrage soll mit dem Statuscode 400: Bad Request geantwortet werden
        char method[16], uri[256], version[16];
        if (sscanf((char*)recvline, "%15s %255s %15s", method, uri, version) != 3) {
            printf("Ungültige Anfrage, sende 400 Bad Request\n");
            send(connfd, RESPONSE_400, strlen(RESPONSE_400), 0);
            close(connfd);
            continue;
        }

        // Aufgabe 2.5 Semantisch korrekte Antworten
        // Empfangene Anfragen als HTTP-Pakete parsen
        if (strcmp(method, "GET") == 0) {
            // Wenn Methode GET ist, aber die Ressource unbekannt
            if (strcmp(uri, "/static/foo") != 0) {
                printf("GET-Anfrage für unbekannte Ressource, sende 404 Not Found\n");
                send(connfd, RESPONSE_404, strlen(RESPONSE_404), 0);
            } else {
                printf("GET-Anfrage für bekannte Ressource\n");
                const char* response_200 = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nFoo";
                send(connfd, response_200, strlen(response_200), 0);
            }
        } else {
            // Alle anderen Methoden außer GET sind nicht implementiert
            printf("Nicht unterstützte Methode: %s, sende 501 Not Implemented\n", method);
            send(connfd, RESPONSE_501, strlen(RESPONSE_501), 0);
        }

        close(connfd);
    }

    close(listenfd);
    return 0;
}
