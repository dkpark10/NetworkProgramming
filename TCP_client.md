# TCP클라이언트 예제

## 코드

```c++
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
    while(read_len = read(sock, &msg[idx++], 1)){                               // TCP의 특성 read함수를 여러변 호출하고 있다.
        if(read_len == - 1) ErrorMessage("read()");                             // TCP는 read write함수의 호출횟수에 상관이 없다.
        str_len += read_len;                                                    // 잘 읽어들이면 read함수는 항상 1을 반환하기 때문에 
    }                                                                           // 1을 매번 호출시 증감시켜 몇번 호출되는지 확인한다. 
    printf("msg from server : %s\n", msg);
    printf("function read call count: %d\n", str_len);
    close(sock);
    return 0;
}
```

## 데이터 경계 확인

TCP의 특성이 무엇인가 ?? 그것은 바로 데이터의 경계가 존재하지 않는 것. </br>
내부 배열이 꽉 차지 않는 이상 언제든지 꺼내먹어도 되는 아이스크림 냉장고 같은것 </br>
