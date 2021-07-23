/*======================================**
 *Name        : onetomany.c
 *Author      : Dong Ningning
 *======================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lpmti.h"

void *MTI_startroutine () {
   int num[2];
   int myrank;
   MTI_Status status;

   MTI_Comm_rank ( MTI_COMM_WORLD, &myrank );
   if (myrank == 0) {
       num[0] = 16;
       num[1] = 28;
       printf ("Myrank: %d, I send two integer number(%d, %d) to others.\n", myrank, num[0], num[1]);
       int i;
       for (i=1; i<100; i++) {
           MTI_Send (num, 2, MTI_INT, 1, 99, MTI_COMM_WORLD);
       }
   }
   else {
       MTI_Recv (num, 2, MTI_INT, 0, 99, MTI_COMM_WORLD, &status);
       printf ("Myrank: %d, received: %d, %d.\n", myrank, num[0], num[1]);
   }

   MTI_thread_exit (myrank);
}

int main (int argc, char *argv[] ) {

  MTI_Init (&argc, &argv);
  MTI_Init_thread (MTI_startroutine);
  MTI_Finalize ();

  return 0;
}
