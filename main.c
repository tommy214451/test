#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 80
#define REPLY_MESSAGE "Reply"
#define MAXLINE 4096

int main (int argc, char **argv)
{
    //Deklaration von Variablen

    int                 listenfd, connfd, n;
    struct sockaddr_in  servaddr;
    uint8_t             buff[MAXLINE+1];
    uint8_t             recvline[MAXLINE+1];

    //Aufgabe 2.1
    // TCP-Socket erstellen und kofigurieren
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0))< 0 )
        perror("socket error.");
        exit(EXIT_FAILURE);

    //Die Adressse und Port definieren
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family        = AF_INET;               //Familie des Sockets
    servaddr.sin_addr.s_addr   = htonl(INADDR_ANY);     //IP-Adresse binden
    servaddr.sin_port          = htons (PORT);   //Portnummer binden


    //Bind(zum Binden des Sockets an eine Adresse)
    if ((bind(listenfd,(struct sockaddr*)&servaddr, sizeof(servaddr)))< 0)
        perror("bind error.");
        exit(EXIT_FAILURE);

    //Listen(um auf eingehende Verbindungen zu warten)
    if((listen(listenfd, 10)) < 0)
        perror("listen error.");
        exit(EXIT_FAILURE);

    //setsockopt SO_REUSEADDR muss noch implementiert werden

    //Aufgabe 2.2 Anfragen beantworten
    //Bei jedem empfangenen Paket mit dem String "Reply" antworten
    // accept(annehmen), recv(empfangen), send(senden) implementieren

    //Aufgabe 2.3 Pakete erkennen
    //Auf jedes empfangende HTTP-Paket soll mit dem String "Reply\r\n\r\n" geantwortet werden

    //Aufgabe 2.4 Syntaktisch korrekte Antworten
    //Auf jede Anfrage soll mit dem Statuscode 400: Bad Request geantwortet werden

    //Aufgabe 2.5 Semantisch korrekte Antworten
    //Empfangende Anfragen als HTTP-Pakte Parsen
    //400: inkorrekte Anfragen, 404: GET-Anfragen, 501: alle anderen Anfragen

    //Aufgabe 2.6 Statischer Inhalt
    //Um Antworten die konkreten Inhalt(einen Body)enthalten erweitern
    //static/ foo:"Foo", bar: "Bar", baz:"Baz"

    //Aufgabe 2.7 Dynamischer Inhalt
    //Methoden aus dem HTTP-protokoll, welche die referenzierte Ressource verändert.
    //Den Inhalt verändert(PUT), den Inhalt löscht(DELETE)
    //PUT: Status Created(201), No Content(204), Andere Pfade Forbidden 403

}