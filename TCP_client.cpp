#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;
void ErrorMessage(const char* msg)
{
    printf("%s error uuuuuuuu\n",msg);
}
int main(int argc, char *argv[])
{
    int sock, str_len = 0, read_len = 0;
    struct sockaddr_in serv_addr;
    char msg[30];
    
    if(argc != 3){
        printf("Usage %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) ErrorMessage("socket()");
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == - 1){
        ErrorMessage("connect()");
    }
    int idx = 0;
    while(read_len = read(sock, &msg[idx++], 1)){
        if(read_len == - 1) ErrorMessage("read()");
        str_len += read_len;
    }
    printf("msg from server : %s\n", msg);
    printf("function read call count: %d\n", str_len);
    close(sock);
    return 0;
}
