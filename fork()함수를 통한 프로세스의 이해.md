# 프로세스의 이해 

```c++
#include<stdio.h>
#include<cstdio>
#include<unistd.h>
using namespace std;
int gval = 0;
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

