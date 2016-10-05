#include <assert.h>
#include <stdlib.h>
#include<stdio.h>
#include "mem.h"
#include <unistd.h>

void validateNoExtraSpace() {
  void * ptr;
  ptr = Mem_Alloc(3072);
  assert(ptr != NULL);
  assert(Mem_Alloc(1) == NULL);
  assert(Mem_Free(ptr) == 0);
}
void validateColase1() {
  void * ptr[3];

  ptr[0] = Mem_Alloc(800);
  assert(ptr[0] != NULL);

  ptr[1] = Mem_Alloc(800);
  assert(ptr[1] != NULL);

  ptr[2] = Mem_Alloc(800);
  assert(ptr[2] != NULL);

 
  assert(Mem_Free(ptr[1]) == 0);
  assert(Mem_Free(ptr[2]) == 0);

  ptr[2] = Mem_Alloc(1600);
  assert(ptr[2] != NULL);

  for (int i = 0; i < 3; i++) {
    Mem_Free(ptr[i]);
  }


}

void validateColase2() {
  void * ptr[3];

  ptr[0] = Mem_Alloc(800);
  assert(ptr[0] != NULL);

  ptr[1] = Mem_Alloc(800);
  assert(ptr[1] != NULL);

  ptr[2] = Mem_Alloc(800);
  assert(ptr[2] != NULL);

  

  assert(Mem_Free(ptr[2]) == 0);
  assert(Mem_Free(ptr[1]) == 0);

  ptr[2] = Mem_Alloc(1600);
  assert(ptr[2] != NULL);
  for (int i = 0; i < 3; i++) {
    Mem_Free(ptr[i]);
  }
}
void validateColase3() {
  void * ptr[5];

  ptr[0] = Mem_Alloc(600);
  assert(ptr[0] != NULL);

  ptr[1] = Mem_Alloc(600);
  assert(ptr[1] != NULL);

  ptr[2] = Mem_Alloc(600);
  assert(ptr[2] != NULL);

  ptr[3] = Mem_Alloc(600);
  assert(ptr[3] != NULL);

  ptr[4] = Mem_Alloc(600);
  assert(ptr[4] != NULL);



  assert(Mem_Free(ptr[1]) == 0);
  assert(Mem_Free(ptr[3]) == 0);
  assert(Mem_Free(ptr[2]) == 0);

  ptr[2] = Mem_Alloc(1800);
  assert(ptr[2] != NULL);
  for (int i = 0; i < 5; i++) {
    Mem_Free(ptr[i]);
  }
}
void validateColase4() {
  void * ptr[6];

  ptr[0] = Mem_Alloc(500);
  assert(ptr[0] != NULL);

  ptr[1] = Mem_Alloc(500);
  assert(ptr[1] != NULL);

  ptr[2] = Mem_Alloc(500);
  assert(ptr[2] != NULL);

  ptr[3] = Mem_Alloc(500);
  assert(ptr[3] != NULL);

  ptr[4] = Mem_Alloc(500);
  assert(ptr[4] != NULL);

  ptr[5] = Mem_Alloc(500);
  assert(ptr[5] != NULL);



  assert(Mem_Free(ptr[1]) == 0);
  assert(Mem_Free(ptr[5]) == 0);
  assert(Mem_Free(ptr[2]) == 0);
  assert(Mem_Free(ptr[4]) == 0);
  assert(Mem_Free(ptr[3]) == 0);

  ptr[2] = Mem_Alloc(2500);
  assert(ptr[2] != NULL);

  for (int i = 0; i < 6; i++) {
    Mem_Free(ptr[i]);
  }
}

void validateColase5() {
  void * ptr[3];

  ptr[0] = Mem_Alloc(800);
  assert(ptr[0] != NULL);

  ptr[1] = Mem_Alloc(800);
  assert(ptr[1] != NULL);

  ptr[2] = Mem_Alloc(800);
  assert(ptr[2] != NULL);



  assert(Mem_Free(ptr[0]) == 0);
  assert(Mem_Free(ptr[1]) == 0);

  ptr[0] = Mem_Alloc(1600); //*************
  assert(ptr[0] != NULL);

  for (int i = 0; i < 3; i++) {
    Mem_Free(ptr[i]);
  }
}

void validateByteSizeAllocation() {
  void * ptr[3072];
  for (int i = 0; i < 3072; i++) {
    ptr[i] = Mem_Alloc(1);
    assert(ptr[i] != NULL);
  }
//  assert(Mem_Alloc(1) == NULL);
  for (int i = 0; i < 3072; i++) {
    assert(Mem_Free(ptr[i]) == 0);
  }
}

void SanityCheck() {
  char * ptr[384];
  for (int i = 0; i < 384; i++) {
    ptr[i] = (char *) Mem_Alloc(8);
    assert(ptr[i] != NULL);
    *(ptr[i]) = 'a';
  }
  for (int i = 0; i < 384; i++) {
    assert(*(ptr[i]) == 'a');
  }
//  assert(Mem_Alloc(1) == NULL);
  for (int i = 0; i < 384; i++) {
    assert(Mem_Free(ptr[i]) == 0);
  }
}



int main() {

  assert(Mem_Init(4095, 0) == 0);
  validateColase1();
  validateColase2();
  validateColase3();
  validateColase4();
  validateColase5();
  // validateNoExtraSpace();
  validateByteSizeAllocation();
  SanityCheck();
  exit(0);
}
