#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main( int argc, char* argv[] )
{
    if ( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi( argv[2] );

    struct sockaddr_in recv_server;
    bzero( &recv_server, sizeof( recv_server ) );
    recv_server.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &recv_server.sin_addr );
    recv_server.sin_port = htons( port );

    int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( sockfd >= 0 );

    if ( connect( sockfd, ( struct sockaddr* )&recv_server, sizeof(recv_server) ) < 0)
    {
        printf( "connection failed\n" );
    }
    else
    {
        char buffer[128];
        int result = 0;
        result = read( sockfd, buffer, sizeof( buffer ));
        assert( result > 0 );
        buffer[ result ] = '\0';
        printf( "this is: %s", buffer );
    }
    close(sockfd);
    return 0;
}
