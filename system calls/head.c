#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
head(int fd, int lines)
{
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0 && lines>0) {
    for(int i=0;i<n;i++){
      if(buf[i]=='\n') lines--;
      if(lines<=0) n=i;
    }
    if (write(1, buf, n) != n) {
      printf(1, "head: write error\n");
      exit();
    }
  }
  if(n < 0){
    printf(1, "head: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    // head(0);
    exit();
  }
  int lines = atoi(argv[1]);
  for(i = 2; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "head: cannot open %s\n", argv[i]);
      exit();
    }
    printf(1,"-----%s----- \n",argv[i]);
    head(fd,lines);
    printf(1,"\n");
    close(fd);
  }
  exit();
}