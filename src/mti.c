/*
 * mti.c
 *
 *  Created on: May 20, 2012
 *      Author: DONG NINGNING
 */

#include "mti.h"
#include "mtiimpl.h"
#include "mtithread.h"
#include <string.h>
#include <sched.h>
/*
 * MTI_Init ()
 *
 */
int num_available_threads = 0;

int MTI_Init (int *argc, char ***argv) {

      if (*argc<3) {
          MTI_geterr (ERR_ARG);
          exit (MTI_FAILURE);
      }
      /* body of Init */
      num_available_threads = atoi(*(*argv+2));
      if (num_available_threads > MTI_THREADS_MAX) {
          MTI_geterr (ERR_TOOMANYTHREADS);
          exit (MTI_FAILURE);
      }
     else {
         pthread_mutex_init (&mtimutex, NULL);
         pthread_attr_init(&mtiattr);
         pthread_mutex_init(&sndrec_mutex, NULL);

         mtisndrec_q.front = mtisndrec_q.rear = NULL;
         mtisndrec_q.currlen = 0;
         mtisndrec_q.maxlen = 0;

         pthread_attr_setdetachstate(&mtiattr, PTHREAD_CREATE_JOINABLE);
         pthread_barrier_init (&mtibarr, NULL, num_available_threads);
    }

/*  } */
  MTI_Initialized (&mtiinitializedflag);
  /* body end of Init */

  return MTI_SUCCESS;

}/* end of MTI_Init () */

/*
 * MTI_Init_thread ()
 */
int MTI_Init_thread (void *(*start_routine) (void *)) {

  MTI_check_init (&mtiinitializedflag);

  MTI_communicator_t *mticommunicator_p;

  cpu_set_t cpuset;
  int num_available_cores;
  num_available_cores = MTI_get_ncores ();

  mtilist.head = mtilist.tail = NULL;
  mticommunicator_p = NULL;
  int i;
  for (i=0; i<num_available_threads; i++) {
      CPU_ZERO (&cpuset);
      CPU_SET (i % num_available_cores, &cpuset);
      pthread_mutex_lock (&mtimutex);

      mticommunicator_p = (MTI_communicator_t *) malloc (sizeof (MTI_communicator_t));
      MTI_create_thread(&mticommunicator_p->tid, &mtiattr, start_routine, NULL);
      mticommunicator_p->comm_size      = num_available_threads;
      mticommunicator_p->rank           = i;
      mticommunicator_p->pfunc          = start_routine;

      mticommunicator_p->cid            = (i % num_available_cores);
      mticommunicator_p->tstatus        = 1;  /* not finished tasks */

      mticommunicator_p->next           = NULL;
      if (mtilist.head == NULL) {
          mtilist.head = mtilist.tail = mticommunicator_p;
      }
      else {
          mtilist.tail->next = mticommunicator_p;
          mtilist.tail       = mticommunicator_p;
      }

      pthread_setaffinity_np (mticommunicator_p->tid, sizeof(cpu_set_t), &cpuset);
      pthread_mutex_unlock (&mtimutex);


  }
  mticommunicator_p = NULL;

  return MTI_SUCCESS;
}

/*
 * MTI_Comm_rank ()
 */
int MTI_Comm_rank (MTI_Comm comm, int *rank) {
  MTI_check_init (&mtiinitializedflag);

  MTI_communicator_t *p, *q;
  p = mtilist.head;
  q = NULL;
  if (comm == MTI_COMM_WORLD) {
      pthread_mutex_lock (&mtimutex);
      while (!(pthread_equal (p->tid, pthread_self())) && p->next!=NULL){
          q = p;
          p = p->next;
      }
      *rank = p->rank;

      pthread_mutex_unlock (&mtimutex);

 }

  return MTI_SUCCESS;
}

/*
 * MTI_Comm_size ()
 */
int MTI_Comm_size (MTI_Comm comm, int *size) {
  MTI_check_init (&mtiinitializedflag);
  if (comm == MTI_COMM_WORLD) {
      *size = num_available_threads;
  }
  return MTI_SUCCESS;
}

/*
 * MTI_Comm_rank ()
 */
int MTI_Send (void *buf, int count, MTI_Datatype datatype, int dest, int tag, MTI_Comm comm) {
  MTI_check_init (&mtiinitializedflag);
  MTI_message_t_ *p;

  p = malloc (sizeof (MTI_message_t_));
  p->buf = (char *) malloc (count*datatype);
  memcpy (p->buf, buf, count*datatype);
  p->count = count;
  p->datatype = datatype;
  p->dest = 0;
  p->tag = tag;
  p->next = NULL;
  pthread_mutex_lock (&sndrec_mutex);
  MTI_enqueue (p);
  pthread_mutex_unlock (&sndrec_mutex);

  return MTI_SUCCESS;
}

/*
 * MTI_Recv ()
 */
int MTI_Recv (void *buf, int count, MTI_Datatype datatype, int source, int tag, MTI_Comm comm, MTI_Status *status) {
  MTI_check_init (&mtiinitializedflag);

  int notfound;
  notfound = 1;

  struct timespec time_sleep;
  time_sleep.tv_sec = 0;
  time_sleep.tv_nsec = 100000000;

  while (notfound) {
      pthread_mutex_lock (&sndrec_mutex);
      if (mtisndrec_q.front != NULL) {
          if (mtisndrec_q.front->tag == tag && mtisndrec_q.front->dest == source) {
              memcpy (buf, mtisndrec_q.front->buf, count*datatype);
              MTI_dequeue ();
              notfound = 0;
          }
          else {
              notfound = 1;
          }
      }
      pthread_mutex_unlock (&sndrec_mutex);
      if (notfound == 1) {
          nanosleep (&time_sleep, NULL);
          /* sleep (1); */
      }
  }
  status->count = count;
  status->tag = tag;
  status->source = source;

  return MTI_SUCCESS;
}

/*
 * MTI_Finalize ()
 */
int MTI_Finalize (void) {

      /* body of Finalize  */

  pthread_attr_destroy(&mtiattr);
  MTI_communicator_t *p, *q;
  p = mtilist.head;
  q = NULL;
  void *status;
  MTI_thread_t mtitid;
  int rc;
         
  while (p != NULL) {

      pthread_mutex_lock (&mtimutex);
      mtitid = p->tid;
      pthread_mutex_unlock (&mtimutex);

      rc = pthread_join(mtitid, &status);
      if (rc) {
          MTI_geterr (ERR_JOIN);
          exit (MTI_FAILURE);
      }
          q = p->next;
          p = q;
     }

     pthread_mutex_destroy(&mtimutex);
     pthread_barrier_destroy(&mtibarr);
     pthread_mutex_destroy(&sndrec_mutex);

     /* destroy message queue */

     MTI_destroyqueue ();

     /* delete the list */
     p = mtilist.head;
     while (p != NULL) {
         q = p->next;
         free (p);
         p = q;
     }
     mtilist.head = mtilist.tail = NULL;
      /* body end of Finalize */
     MTI_Finalized (&mtifinalizedflag);

  return MTI_SUCCESS;

}/* end of MTI_Finalize () */

/*
 * MTI_Barrier ()
 */
int MTI_Barrier (MTI_Comm comm) {
  if (comm == MTI_COMM_WORLD) {
      pthread_barrier_wait (&mtibarr);
  }
  return MTI_SUCCESS;

}

/*
 * MTI_thread_exit ()
 */
int MTI_thread_exit_ (void *status) {
      pthread_exit(status);        
  return MTI_SUCCESS;
}
