# 프로세스의 이해 

프로세스란 

> 메모리 공간을 차지한 상태에서 실행중인 프로그램

즉 프로그램이 실행중인 상태 

## fork함수

> pid_t fork(void);

포크함수는 호출한 프로세스의 복사본을 생성한다. 전혀 새로운 다른 프로그램을 바탕으로 프로세스를 생성하는 것이 아니라 
이미 실행중인 fork함수를 호출한 프로세스를 복사하는 것이다. 그리고 두프로세스 포크함수 이후 문장을 실행하게 된다. </br>
그런데 완전히 동일한 프로세스로 메모리 영역까지 동일하게 복사하기 때문에 이후 프로그램 흐름은 fork 함수의
반환값을 기준으로 나위도록 프로그래밍 해야한다.

```c++
#include<stdio.h>
#include<cstdio>
#include<unistd.h>
using namespace std;
int gval = 10;
int main(void)
{
  pid_t pid;
  int lval = 20;
  gval++, lval+=5;
  
  pid = fork();
  if(!pid){             // if child
    gval+=2;
    lval += 2;
  }
  else{                 // if parent
    gval-=2;
    lval -=2;
  }
  
  if(!pid) printf("child proc : %d == %d\n", gval,lval);
  else printf("parent proc : %d == %d\n", gval,lval);
  return 0;
}
```

## 좀비프로세스

프로세스의 소멸은 중요하다!!! 자식프로세스가 종료되는 상황은 다음과 같다.</br>

1. 인자를 전달하면서 exit()를 호출
2. 메인함수 리턴

이 좀비 프로세스는 언제 소멸될까???

> 해당 자식 프로세스를 생성한 부모 프로세스에게 exit 함수 인자값이나 return문의 반환값이 전달되어야 한다.

```c++
if(pid == 0 ) printf("child\n");      // if child
else{
  printf("parent\n");                 // if parent
  sleep(30);                          // 부모가 종료되야 자식이 소멸되기 때문에 일부러 지연
}
``` 
** 이 코드를 실행시키면 잠시 멈추게 되고 이 순간 자식프로세스의 아이디를 확인하면 Z+ 이다.** </br>
30초의 대기시간이 지나면 부모가 소멸되고 자식도 함께 소멸된다. 

## 좀비프로세스의 소멸

```c++
pid_t waitpid(pit_t pid, int* statloc, int options);
1. 확인하고 싶은 자식프로세스 id전달
2. ..
3. WNOHANG인자를 전달하면 종료된 자식이 없어도 블로킹에 빠지지 않고 0을반환하며 함수를 탈출한다.
```

```c++
#include<stdio.h>
#include<cstdio>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;
int main(void)
{
  pid_t pid;
  int status;
  
  pid = fork();
  if(pid == 0){                           // child
    sleep(3);
    return 128564;
  }
  else{                                   // parent
    
    while(!waitpid(pid,&status,WNOHANG)){ // 종료된자식프로세스가 없으면 0을 반환 종료될 때 까지 계속실행
      sleep(3);
      puts("3sec...);
    }
    
    if(WIFEXITED(status)){                              // 자식 정상종료시 참반환
      printf("child send %d\n", WEXITSATUS(status));    // 자식프로세스 전달 값 반환
    }
  }
  
  return 0;
}
```
