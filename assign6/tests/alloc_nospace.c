/* allocation is too big to fit in available space */
#include <assert.h>
#include <stdlib.h>
#include "mem.h"

int main() {
   assert(Mem_Init(1, 0) == 0);
   //Mem_Dump();
   void* ptr = Mem_Alloc(4);
   //assert(ptr == NULL);
   Mem_Dump();

   exit(0);
}
