#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;

class SerApi {
public:
    SerApi( const char* ip, int port ):
        _ip(ip),
        _port(port)
    {}

    void start()
    {
        struct sockaddr_in address;
        bzero( &address, sizeof(address) );
        address.sin_family = AF_INET;
        address.sin_port = htons( _port );
        inet_pton( AF_INET, _ip, &(address.sin_addr) );

        int sock = socket( PF_INET, SOCK_STREAM, 0 );
        assert( sock >= 0 );

        int ret = bind( sock, ( struct sockaddr* )&address, sizeof( address ) );
        assert( ret != -1 );

        ret = listen( sock, 5 );
        assert( ret != -1 );
        sleep(30);
        struct sockaddr_in client;
        socklen_t client_addrlenth = sizeof( client );
        int connfd = accept( sock, ( struct sockaddr*)&address, &client_addrlenth );
        if (connfd < 0)
        {
            cerr << errno << endl;
        }
        else {
            char remote[INET_ADDRSTRLEN];
            printf( "connected with ip: %s and port: %d\n", 
                    inet_ntop( AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN ));
            close ( connfd );
        }
        close( sock );
    }
private:
    const char* _ip;
    int _port;
};

int main(int argc, char* argv[])
{
    if ( argc <= 2 )
    {
        printf("usage: %s ip_address port_number\n", basename( argv[0] ));
        return 1;
    }
    SerApi api(argv[1], atoi(argv[2]));
    api.start();
    return 0;
}
