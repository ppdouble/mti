/*======================================**
 *Name        : sendmessage.c
 *Author      : Dong Ningning
 *======================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lpmti.h"

void *MTI_startroutine () {
   char message [20];
   int myrank;
   MTI_Status status;

   MTI_Comm_rank ( MTI_COMM_WORLD, &myrank );
   if (myrank == 0) {
       strcpy (message, "Hello, thread 1");
       MTI_Send (message, strlen (message)+1, MTI_CHAR, 1, 99, MTI_COMM_WORLD);
   }
   else if (myrank == 1) {
       MTI_Recv (message, 20, MTI_CHAR, 0, 99, MTI_COMM_WORLD, &status);
       printf ("received: %s\n", message);
   }
   MTI_thread_exit (myrank);
}

int main (int argc, char *argv[] ) {

  MTI_Init (&argc, &argv);
  MTI_Init_thread (MTI_startroutine);
  MTI_Finalize ();

  return 0;
}
