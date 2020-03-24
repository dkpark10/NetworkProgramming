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
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char msg[] = "i am a great man";
    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) ErrorMessage("socket()");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == - 1){
        ErrorMessage("bind()");
    }
    if(listen(serv_sock, 5)== -1) ErrorMessage("listen()");
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1) ErrorMessage("accept()");
    write(clnt_sock, &msg, sizeof(msg));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
