## server

```c++
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024
using namespace std;
void Errormessage(const char* str)
{
    printf("%s error!!!!!!!\n", str);
}
int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) Errormessage("socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    int bindresult = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(bindresult == -1) Errormessage("bind()");
    
    if(listen(serv_sock, 15) == -1) Errormessage("listen()");

    socklen_t clnt_addr_size;
    clnt_addr_size = sizeof(clnt_addr);

    char msg[BUF_SIZE];
    int stringlen;
    for(int i=0; i<5; i++){

        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) Errormessage("accept()");
        else printf("Connected client %d\n", i + 1);

        while((stringlen=read(clnt_sock, msg, BUF_SIZE))!= 0){
            write(clnt_sock, msg, stringlen);
            printf("clnt_test %d\n", stringlen);
        }
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}
```

## client

```c++
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFSIZE 1024
using namespace std;
void Errormessage(const char* str)
{
    printf("%s error!!!!!!!\n", str);
}
int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) Errormessage("socket()");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    int connectresult = connect(sock,
    (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(connectresult == -1) Errormessage("connect()");
    else printf("connecting........\n");

    char msg[BUFSIZE];
    int stringlen, temp;
    while(1){

        fputs("input msg q is quit : ",stdout);
        fgets(msg, BUFSIZE, stdin);
        if(!strcmp(msg,"Q\n") || !strcmp(msg,"q\n")) break;

        // write(sock,msg,strlen(msg));
        // stringlen = read(sock, msg, BUF_SIZE - 1);
        // msg[stringlen] = 0;
        // printf("stringlen == %d\n", stringlen);
        // printf("Msg from server : %s\n", msg);

        stringlen = write(sock, msg, strlen(msg)); 
        printf("write len == %d\n", stringlen);
        
        int recvidx = 0;
        while(recvidx < stringlen){
            temp = read(sock, &msg[recvidx], BUFSIZE - 1);
            if(temp == -1) Errormessage("read()");
            printf("temp == %d\n", temp);
            recvidx += temp;
        }
        msg[recvidx] = 0;
        printf("msg from server == %s\n",msg);
    }
    close(sock);
    return 0;
}
```
