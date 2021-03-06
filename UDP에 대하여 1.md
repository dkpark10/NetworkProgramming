# UDP에 대하여 1 

### UDP소켓의 특성

UDP는 편지를 쓰고 받는 사람과 주소를 적고 우체통에 넣는 것과 같다. 신뢰성은 없다</br>
TCP와 다르게 3hand shaking과정을 거치지 않는다. TCP는 신뢰성 없는 IP 프로토콜을 기반으로 신뢰성 있는 데이터 송수신을 위해 흐름제어를 하는데 **UDP는 흐름제어를 하지 않는다.**

![image](https://user-images.githubusercontent.com/43857226/79253116-a88db780-7ebd-11ea-874c-96ba0cff6e0b.png)

위 그림에서 보이듯 호스트 B를 떠난 패킷이 A에게 전달되도록 하는 것은 IP의 역할이다.
이렇게 전달된 UDP패킷을 호스트 A내 소켓에게로 최종 전달하는 것은 IP역할이 아니다.</br>
**유디피의 역할은 호스트로 수신된 패킷을 포트정보를 참조하여 최종목적지인 유디피 소켓에 전달하는 것이다.**

### UDP의 효율적 사용

압축파일은 TCP가 효율적 한개만 손실되도 치명적이기 때문 UDP는 스트리밍 서비스에 최적화 되어있다. TCP가 UDP에 비해 느린 이유는 다음을 들 수 있다.
</br>
1. 데이터 송수신 이전 이후에 거치는 연결설정 및 해제과정</br>
2. 데이터 송수신 과정에서 거치는 신뢰성 보장을 위한 흐름제어 </br>
</br>
따라서 송수신 데이터 양은 작으면서 잦은 연결이 필요한 경우 UDP가 더 효율적 

### 구현

UDP는 서로 연결되있지 않은 비연결지향 프로토콜이다. 따라서 연결설정과정이 필요없고 listen 함수와 accept함수가 불필요하다.

> UDP에서는 서버나 클라나 단 하나의 소켓만 필요하다

TCP는 1대1관계다. 10개의 클라이언트에게 서비스를 제공하려면 문지기 소켓을 제외한 열개의 소켓이 더 필요하다. 
그러나 UDP는 서버나 클라나 하나만 필요하다. 우체통 하나만 있음 어디로든 편지를 전달할 수 있다. 

![image](https://user-images.githubusercontent.com/43857226/79253129-b0e5f280-7ebd-11ea-81be-bbb4003009fa.png)

하나의 유디피 소켓으로 여러 호스트들이랑 데이터 송수신할 수 있다.

### 데이터 입출력 함수

티씨피 경우 주소 정보를 추가하는 과정이 필요없다. 티씨피는 목적지에 해당하는 소켓과 연결된
상태이기 때문이다. 즉 티씨피는 목적지의 주소를 이미 알고 있다. 하지만 유디피는 연결상태를 
유지하지 않으므로 데이터를 전송할 때 마다 **목적지의 주소정보를 별도로 추가해야한다.!!!**

```c++
ssize_t sentto(int sock, void*buff, size_t nbytes, int flags,
				struct sockaddr* to, socklen_t addrlen); 
				// 성공시 전송된 바이트 수 실패시 -1
```

sock = 소켓 디스크립터</br>
buff = 데이터</br>
nbytes = 전송할 데이터 크기</br>
flags = 옵션 </br>
to = 목적지 주소정보 담고있는 sockaddr 구조체 변수  주소값 전달</br>
addrlen = 매개변수 to로 전달된 주소 값 구조체 변수 크기</br>
</br>
다음은 받는 함수이다.</br>

```c++
ssize_t recvfrom(int sock, void*buff, size_t nbytes, int flags,
					struct sockaddr* from, socklen_t* addrlen);
				// 성공시 수신한 바이트 수 실패시 -1
```

sock = 소켓 디스크립터</br>
buff = 데이터</br>
nbytes = 수신할 최대 바이트 수 buff가 가리키는 버퍼의 크기를 넘을 수 없다</br>
flags = 옵션 </br>
to = 발신지 정보를 채워넣을 구조체 변수 주소 값 전달</br>
addrlen = 매개변수 from으로 전달된 주소에 해당하는 구조체 변수의 크기정보를 담고있는 주소값</br>

### UDP server

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
    int servsock, stringlen;
    char msg[BUFSIZE];
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_addrsize;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    servsock = socket(PF_INET, SOCK_DGRAM, 0);
    if(servsock == -1) Errormessage("socket()");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(servsock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        Errormessage("bine()");

    while(1){
        clnt_addrsize = sizeof(clnt_adr);
        stringlen = recvfrom(servsock, msg,BUFSIZE, 0, 
        (struct sockaddr*)&clnt_adr, &clnt_addrsize);
        
        sendto(servsock,msg,stringlen,0,
        (struct sockaddr*)&clnt_adr, clnt_addrsize);
    }
    close(servsock);
    return 0;
}
```

### UDP client

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
    int sock, stringlen;
    char msg[BUFSIZE];
    struct sockaddr_in serv_adr, from_adr;
    socklen_t addrsize;;

    if(argc != 3){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) Errormessage("socket()");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    while(1){

        fputs("input message ('q' OR 'Q' quit): ",stdout);
        fgets(msg, sizeof(msg),stdin);
        if(!strcmp(msg,"q\n") || !strcmp(msg,"Q\n")) break;

        sendto(sock,msg,strlen(msg),0,
        (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        addrsize = sizeof(from_adr);

        stringlen = recvfrom(sock, msg,BUFSIZE, 0, 
        (struct sockaddr*)&from_adr, &addrsize);
        msg[stringlen] = 0;
        printf("msg from server : %s\n", msg);
    }
    close(sock);
    return 0;
}
```
