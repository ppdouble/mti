/*
 * mti.h
 *
 *  Created on: May 20, 2012
 *      Author: DONG NINGNING
 */
#define _GNU_SOURCE

#ifndef MTI_H_
#define MTI_H_

#define MTI_SUCCESS 0
#define MTI_FAILURE 1
/* MTI_THREADS_MAX , it should be changed when linux kernel or hardware changed */
#define MTI_THREADS_MAX 60000

#define MTI_COMM_WORLD 0        
#define MTI_COMM_SELF 1

typedef int MTI_Comm;

/* MTI datatype */

#define MTI_CHAR        sizeof (char) /* char */
#define MTI_SHORT       sizeof (short) /* short */
#define MTI_INT         sizeof (int) /* int */
#define MTI_LONG_INT    sizeof (long int) /* long int */
#define MTI_FLOAT       sizeof (float) /* float */
#define MTI_DOUBLE      sizeof (double) /* double */
typedef int MTI_Datatype;

struct MTI_Status_ {
    int count;
    int source;
    int tag;
};
typedef struct MTI_Status_ MTI_Status;
/* functions */

int MTI_Init (int *argc, char ***argv);
int MTI_Init_thread (void *(*start_routine) (void *));
int MTI_Comm_rank (MTI_Comm comm, int *rank);
int MTI_Comm_size (MTI_Comm comm, int *size);
int MTI_Send (void *buf, int count, MTI_Datatype datatype, int dest, int tag, MTI_Comm comm);
int MTI_Recv (void *buf, int count, MTI_Datatype datatype, int source, int tag, MTI_Comm comm, MTI_Status *status);
int MTI_Finalize (void);
int MTI_Barrier (MTI_Comm comm);
int MTI_thread_exit_ (void *status);
#define MTI_thread_exit(status) MTI_thread_exit_((void *)status)

/*
int MTI_Fork ();
int MTI_Join ();
*/

#endif /* MTI_H_ */
