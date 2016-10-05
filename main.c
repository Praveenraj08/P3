#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "mem.h"

int main()
{
  int exit_loop=1;
  int memInit_return;
  int *ptr;
  // void *ptr;
void *arr[20];
int no_of_pointers=0;
// for (int i = 0; i < ARRAY_LENGTH; i++) {
//     arr[i] = ptr;
// }
  while(exit_loop==1)
  {
    int ch;
    printf("1. Init mem 2.Mem_Alloc 3.free 0.Exit\n");
    printf(">>");
    scanf("%d",&ch);
    if(ch==1)
    {
      memInit_return=Mem_Init(4095,1);
      if(memInit_return==-1)
      break;
    }
    else if(ch==2)
    {
      printf("Enter size for Mem_Alloc\n");
      int i=0;
      scanf("%d",&i);
      // printf("Sending size %d\n",i );
      ptr=Mem_Alloc(i);
      if(ptr==NULL)
      {
        printf("Rcvd NULL ptr\n");
        break;
      }

      arr[no_of_pointers]=ptr;
      printf("Rcvd ptr at %d  %p \n",no_of_pointers,arr[no_of_pointers] );
      no_of_pointers++;

    }
    else if(ch==3)
    {
      // printf("Inside free\n");
      // printf("Curr ptr is %p\n",ptr);
      srand(time(NULL));
      int number=rand() % no_of_pointers;
      int rc=Mem_Free(arr[number]);
      // int rc=Mem_Free(ptr);
      if(rc!=0)
      {
        printf("Bad ptr\n");

      }
    }
    else if(ch==4)
    {
      printf("Print Dump\n");
      Mem_Dump();
    }
    else if(ch==0)
    {
      break;
    }

    // printf("Enter 0 to exit | 1 to continue \n");
    // scanf("%d",&exit_loop);
  }

printf("Program exiting\n");
}
