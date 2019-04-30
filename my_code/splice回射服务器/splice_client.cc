#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string>
#include <iostream>


int main( int argc, char* argv[] )
{
    if ( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    struct sockaddr_in peer_server;
    bzero( &peer_server, sizeof( peer_server ) );
    peer_server.sin_family = AF_INET;
    inet_pton ( AF_INET, ip, &peer_server.sin_addr );
    peer_server.sin_port = htons( port );

    int sock = socket( PF_INET, SOCK_STREAM, 0 );
    assert( sock >= 0 );

    while (connect( sock, ( struct sockaddr* )&peer_server, sizeof( peer_server ) ) != -1)
    {
        std::string buffer1;
        char buffer2[1024] = { 0 };
        std::cout << "mmc-pc@server>> ";
        std::cin >> buffer1;
        send( sock, buffer1.c_str(), buffer1.size(), 0 );
        recv( sock, buffer2, 1023, 0 );
        std::cout << "server@mmc-pc>> " << buffer2 << std::endl;
    }
    close( sock );
    return 0;
}
