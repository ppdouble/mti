/*
 * mtiimpl.h
 *
 *  Created on: May 23, 2012
 *      Author: DONG NINGNING
 */

#ifndef MTIIMPL_H_
#define MTIIMPL_H_


#include "mti.h"
#include "mtithread.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

struct MTI_communicator {
  MTI_thread_t tid;             /* thread id */
  unsigned short cid;           /* one core per thread */
  int comm_size;
  int rank;
  void *pfunc;                  /* point to function */
  int tstatus;                  /* 0 finish its job, 1 not */
  struct MTI_communicator *next;
};
typedef struct MTI_communicator MTI_communicator_t;
struct MTI_communicator_list {
  MTI_communicator_t *head;
  MTI_communicator_t *tail;
}mtilist;

/* snd_rec queue */
struct MTI_message {
  int dest;
  int tag;
  MTI_Comm comm;
  void *buf;
  int count;
  MTI_Datatype datatype;
  struct MTI_message *next;
};
typedef struct MTI_message MTI_message_t_;

struct _MTI_q_sndrec {
  MTI_message_t_ *front;            /* first queue element */
  MTI_message_t_ *rear;             /* last queue element */
  int      currlen;           /* current length */
  int      maxlen;            /* maximum since beginning of run */
}mtisndrec_q;

/* end of snd_rec queue */

/* MTI thread synchronization */
pthread_mutex_t mtimutex;
pthread_mutex_t sndrec_mutex;
pthread_attr_t mtiattr;
pthread_barrier_t mtibarr;

int mtiinitializedflag; /* 1 initialized, 0 not initialized */
int mtifinalizedflag;   /* 1 finalized, 0 not finalized */

int MTI_Initialized (int *flag);
int MTI_Finalized (int *flag);

/* handle errs */
/* define error number */
#define ERR_REINIT 0
#define ERR_REFINALIZE 1
#define ERR_NOTINIT 2
#define ERR_NOTFINALIZE 3
#define ERR_TOOMANYTHREADS 4
#define ERR_ARG 5
#define ERR_JOIN 6

int MTI_geterr (int errno);

int MTI_check_init (int *flag);

int MTI_get_ncores (void);

int MTI_sched (void);
#endif /* MTIIMPL_H_ */
