/*

엔디안 변환
CPU에 따라 데이터를 저장하는 방식이 다름 크게 2가지 방식으로 나눈다
빅엔디안 : 상위 바이트 값을 작은번지수에 차례대로 저장
리틀엔디안: 상위 바이트값을 높은 번지수에 차례대로 저장
ex) Ox11223344 
  빅-> 11223344
  리틀-> 44332211
  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    unsigned short host_port = 0x1234; 
    unsigned short net_port;
    unsigned int host_addr = 0x12345678;
    unsigned int net_addr;

    net_port=htons(host_port); // 변수 host_port에 저장된 데이터를 네트워크 바이트 순서로 변환
    net_addr=htonl(host_addr); // 변수 host_addr에 저장된 데이터를 네트워크 바이트 순서로 변환

    printf("Host ordered port: %#x \n", host_port);        // Ox1234
    printf("Network ordered port: %#x \n", net_port);      // Ox3412
    printf("Host ordered address: %#1x \n", host_addr);    // Ox12345678
    printf("Network ordered address: %#1x \n", net_addr);  // Ox78563412

    return 0;
}
