#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
using namespace std;
const int port = 8888;
int main( int argc, char* argv[] )
{
    if (argc < 0)
    {
        printf("need two canshu\n");
        return 1;
    }
    int sock;
    int connfd;
    struct sockaddr_in sever_address;
    bzero( &sever_address, sizeof(sever_address) );
    sever_address.sin_family = PF_INET;
    sever_address.sin_addr.s_addr = htons(INADDR_ANY);
    sever_address.sin_port = htons(8888);

    sock = socket( AF_INET, SOCK_STREAM, 0 );

    assert( sock >= 0 );
    int ret = bind( sock, (struct sockaddr*)&sever_address, sizeof(sever_address) );
    assert( ret != -1 );
    ret = listen( sock, 1 );
    assert( ret != -1 );

    while (1)
    {
        struct sockaddr_in client;
        socklen_t client_addrlength = sizeof(client);
        connfd = accept( sock, (struct sockaddr*)&client, &client_addrlength );
        if (connfd < 0)
        {
            printf ( "errno\n" );
        }
        else
        {
            char request[1024];
            recv( connfd, request, 1024, 0 );
            request[strlen(request)+1] = '\0';
            printf( "%s\n", request );
            printf( "successeful!\n" );
            char buf[520] = "HTTP/1.1 200 ok\r\nconnection:close\r\n\r\n";
            int s = send( connfd, buf, strlen(buf), 0 );
            int fd = open( "hello.html", O_RDONLY );
            sendfile( connfd, fd, NULL, 2500 );
            close( fd );
            close( connfd );
        }
    }
    return 0;
}
