#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

static bool stop = false;
static void handle_term(int sig)
{
    stop = true;
}

class TcpApi{
public:
    TcpApi(const char* ip, int port, int backlog):
        _ip(ip),
        _port(port),
        _backlog(backlog)
    {}

    void start()
    {
        int sock = socket( PF_INET, SOCK_STREAM, 0 );
        assert( sock >= 0 );
        
        struct sockaddr_in address;
        bzero( &address, sizeof(address) );
        address.sin_family = AF_INET;
        inet_pton( AF_INET, _ip, &address.sin_addr );
        address.sin_port = htons( _port );

        //绑定
        int ret = bind( sock, ( struct sockaddr* )&address, sizeof( address ) );
        assert( ret != -1 );

        //监听
        ret = listen( sock, _backlog );
        assert( ret != -1 );

        /*连接*/
        while (!stop)
        {
            sleep(1);
        }
        close( sock );
    }
private:
    const char* _ip;
    int _port;
    int _backlog;
};

int main(int argc, char* argv[])
{
    signal( SIGTERM, handle_term );
    if (argc <= 3)
    {
        //basename：作用就是只打印./之后的字符串
        printf( "usage: %s ip_address port_number backlog\n", basename( argv[0] ));
        return 1;
    }
    TcpApi Ping(argv[1], atoi(argv[2]), atoi(argv[3]));
    Ping.start();
    return 0;
}
