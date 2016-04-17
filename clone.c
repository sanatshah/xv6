#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

void* test(void *arg){
  printf(1,"function worked, in child thread");
  
}

int main(int argc, char **argv){

  void *stacks;
  void *args;

  stacks = (void*) malloc(4096);
  args = (void*) malloc(sizeof(int));

  int x=clone(test, args, stacks);

  if (x>=0){
    printf(1,"Clone was successful");

    if(x == 0){ //child process

      printf(1,"this is the child process");

    }else{ //parent process

      printf(1,"this is the parent process");
    }

  } else {
    printf(1,"clone failed");
  }

  exit();
}
