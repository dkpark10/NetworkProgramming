# IP주소 PORT번호

### 인터넷 주소(Internet Address)

ip(internet protocol)은 데이터 송수신목적으로 컴퓨터에게 부여되는 값 </br>
포트번호는 컴퓨터에 부여하는 값이아닌 프로그램상에서 생성되는 소켓을 구분하기 위한 값 </br>

> 친구집에 놀러가면 집주소는 아이피 친구는 포트번호 </br>

IPv4기준 4바이트 ip주소는 **네트워크 주소**와 **호스트 주소**로 나뉜다. </br>
네트워크 주소란 네트워크의 구분을 위한 IP주소의 일부를 말한다. </br>
예를 들어 www.google.com의 마크주커버그에게 데이터를 전송하고 싶다면 ??? </br>
먼저 구글컴퓨터는 하나의 로컬 네트워크로 연결되어 있다. 그렇다면 먼저 www.google.com의 </br>
네트워크로 데이터를 전송하는 것이 우선이다. 즉 첨부터 4바이트 IP주소전부를 참조해 마크주커버그</br>
에게 전송되는 것이 아니라 4바이트 IP주소 중에서 네트워크 주소만을 참조해 일단 </br>
www.google.com에 네트워크로 전송된다 전송 후 해당 네트워크는 전송된 데이터의 호스트주소 </br>
를 참조하여 마크주커버그의 컴퓨터로 데이터를 전송해준다. 그림과 같다. </br>

![image](https://user-images.githubusercontent.com/43857226/77502541-7da3db00-6e9e-11ea-9c69-6b3c5e29237e.png)

이 그림에서 임의의 호스트가 **203.211.172.103**과 **203.211.217.202** 로 데이터를 전송하고 있다.</br>
그런데 이중 **203.211.172**와 **203.211.217** 이 네트워크 주소이다.</br>
따라서 해당네트워크로 데이터가 전송된다. 단 네트워크로 데이터가 전송된다는 것은 </br>
네트워크를 구성하는 라우터 또는 스위치로 데이터가 전송됨을 뜻한다. </br>
그러면 데이터를 전송받는 라우터는 데이터에 적혀있는 호스트 주소를 참조하여 호스트에게 전송해준다. </br>

### 소켓 구분에 활용되는 포트번호 

IP는 컴퓨터를 구분하기 위한 목적 때문에 IP를 가지고 데이터 전송가능하나 </br>
이것만으로 최종목적지인 응용프로그램까지 전송은 불가능하다. </br>
우리컴퓨터에는 NIC(네트워크 인터페이스 카드)라 불리는 데이터 송수신장치가 있다. </br>
IP는 NIC를 통해 컴퓨터 내부로 전송되는데 사용된다. 그러나 컴퓨터 내부로 전송된 </br>
데이터를 소켓에 적절히 분배하는 작업은 운영체제가 담당한다. </br> 
이 때 운영체제는 포트번호를 활용한다. </br>
</br>
이렇듯 포트번호는 하나의 운영체제내에서 소켓을 구분하는 목적으로 사용되기 때문에 </br>
하나의 운영체제내에서 동일한 포트번호를 할당받을 수 없다. </br> 
포트번호는 16비트로 0~ 65536 범위이고 0부터 1023까지는 잘아려진 PORT라 해서 </br>
특정 프로그램에 할당되기로 예약되어있기 때문에 이 외에 번호로 할당을 해야한다. </br>
글구 TCP와 UDP는 포트번호를 공유하지 않기 때문에 같은 번호로 할당이 가능하다. </br>


### 주소정보의 표현

아이피와 포트번호표현을 위한 구조체가 정의되어 있다. 다음과 같다. </br>

```c++
strcut sockaddr_in{
  sa_family_t       sin_family;             // 주소체계
  uint16_t          sin_port;               // 16비트 TCP/UDP PORT번호
  struct in_addr    sin_addr;               // 32비트 IP 주소
  char              sin_zero[8];            // 노사용
};
```

그리고 위 구조체 안 in_addr 구조체는 이렇게 정의되어 있다. </br>

```c++
struct in_addr{
  in_addr_t         s_addr;                 // 32비트 IPv4 인터넷 주소
};
```

생소한 자료형이 보이는데 다음 그림을 참조하자 !!!!!!!!!!!!!! </br>

![image](https://user-images.githubusercontent.com/43857226/77503530-28b59400-6ea1-11ea-9c8b-8d9b90360a78.png)

### 구조체 sockaddr_in 멤버변수 분석

1.**sin_family** </br>
프로토콜 체계마다 주소체계가 다름 IPv4는 4바이트 주소 IPv6는 16바이트 주소체계 </br>

|주소체계|의 미|
|---|---|
|AF_INET| IPv4 인터넷 주소체계|
|AF_INET6| IPv6 인터넷 주소체계|
|AF_LOCAL| 로컬통신을 위한 유닉스 주소체계|

2. **sin_port**
16비트 포트번호를 저장한다. 네트워크 바이트 순서로 저장 </br>

3. **sin_addr**
32비트 아이피 주소정보 저장 네트워크 바이트 순서로 저장 </br>
이 sin_addr 구조체의 멤버변수 in_addr은 32비트 정수자료형으로 저장되있음. </br>

### 네트워크 바이트 순서와 인터넷 주소 변환
CPU에 따라 저장방식이 달라질수 있다. 정수 1을 다음과 같이 표현된다. </br>

**00000000 00000000 00000000 00000001** </br>
**00000001 00000000 00000000 00000000** </br>

CPU가 데이터를 메모리에 저장하는 방식은 두가지로 나뉜다. </br>
빅엔디안 = 상위 바이트값을 작은번지수에 저장하는 방식 </br>
리틀엔디안 = 상위 바이트값을 큰 번지수에 저장하는 방식 </br>
4바이트 정수 Ox12345678을 빅엔디안방식으로 저장한다고 가정하면 </br>

> Ox12 Ox34 Ox56 Ox78 </br>

4바이트 정수 Ox12345678을 리틀엔디안방식으로 저장한다고 가정하면 </br>

> Ox78 Ox56 Ox34 Ox12 </br>

즉 빅엔디안 오름차순 리틀엔디안 내림차순으로 생각하면 편하다. </br>
우리가 흔히 사용하는 인텔,AMD는 리틀엔디안 방식으로 저장된다. </br>
두 컴퓨터가 데이터 통신할 때 방식을 맞춰야 한다.!!!! 아니면 당연히 값이 인식 되니까... </br>

> 빅엔디안으로 통일

### 바이트 순서의 변환

**unsigned short htons(unsigned short)** -> short형 데이터를 호스트에서 네트워크 순서로 변환 </br>
**unsigned short ntohs(unsigned short)** -> short형 데이터를 네트워크에서 호스트 순서로 변환 </br>
**unsigned short htonl(unsigned long)** -> long형 데이터를 호스트에서 네트워크 순서로 변환 </br>
**unsigned short htons(unsigned long)** -> long형 데이터를 네트워크에서 호스트 순서로 변환 </br>

```c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    unsigned short net_port, host_port = 0x1234; 
    unsigned int net_addr, host_addr = 0x12345678;

    net_port=htons(host_port); // 변수 host_port에 저장된 데이터를 네트워크 바이트 순서로 변환
    net_addr=htonl(host_addr); // 변수 host_addr에 저장된 데이터를 네트워크 바이트 순서로 변환

    printf("Host ordered port: %#x \n", host_port);        // Ox1234
    printf("Network ordered port: %#x \n", net_port);      // Ox3412
    printf("Host ordered address: %#1x \n", host_addr);    // Ox12345678
    printf("Network ordered address: %#1x \n", net_addr);  // Ox78563412

    return 0;
}
```
