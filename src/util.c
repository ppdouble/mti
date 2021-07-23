/*
 * util.c
 *
 *  Created on: Jul 30, 2012
 *      Author: hpc2
 */

#include "util.h"
/*
 * MTI_enqueue ()
 */
int MTI_enqueue (void *object) {
  MTI_message_t_ *p;
  p = (MTI_message_t_ *)object;
  if (mtisndrec_q.front == NULL) {
      mtisndrec_q.front = mtisndrec_q.rear = p;
  }
  else {
      mtisndrec_q.rear->next = p;
      mtisndrec_q.rear = p;
  }
  mtisndrec_q.currlen++;

  return MTI_SUCCESS;
}

/*
 * MTI_dequeue ()
 */
int MTI_dequeue () {
  MTI_message_t_ *p;
  if (mtisndrec_q.front == NULL) {
      return -1;
  }
  free (mtisndrec_q.front->buf);
  mtisndrec_q.front->buf = NULL;
  p = mtisndrec_q.front;
  mtisndrec_q.front = p->next;
  if (mtisndrec_q.rear == p) {
      mtisndrec_q.front = mtisndrec_q.rear = NULL;
  }
  free (p);
  p = NULL;
  mtisndrec_q.currlen--;

  return MTI_SUCCESS;
}

/*
 * MTI_destroyqueue ()
 */
int MTI_destroyqueue () {
  while (mtisndrec_q.currlen != 0) {
      MTI_dequeue ();
  }

  return MTI_SUCCESS;
}
