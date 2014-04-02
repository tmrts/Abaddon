#ifndef AD_QUEUE_H
#define AD_QUEUE_H

#include <pthread.h>

/* Queue data is in the form of a void ptr.
 * The queue can be used for many purposes.
 * It's the developer's responsibility to send
 * void ptr and typecast when popped from the queue. */
typedef struct node {
    void *data;
    struct node *next;
    struct node *previous;
}ad_node;

#define DATA(n)     ((n)->data)
#define NEXT(n)     ((n)->next)
#define PREVIOUS(n) ((n)->next)

typedef pthread_mutex_t ad_queue_mutex;
typedef pthread_cond_t ad_queue_cond;

typedef struct {
    int node_count;
    ad_node *head;
    ad_node *last;
    ad_queue_mutex *mutex;
    ad_queue_cond *cond;
}ad_queue;

#define NODE_COUNT(q) ((q)->node_count)
#define IS_EMPTY(q)   ((q)->node_count ? 0 : 1)
#define HEAD(q)       ((q)->head)
#define LAST(q)       ((q)->last)

#define MUTEX(q) ((q)->mutex)
#define COND(q)  ((q)->cond)

#define AD_QUEUE_LOW_LIMIT 3
#define AD_QUEUE_HIGH_LIMIT 15

ad_queue *ad_queue_construct(ad_queue_mutex *mutex, ad_queue_cond *cond_var);

void ad_queue_push(ad_queue *queue, void *data);

void *ad_queue_pop(ad_queue *queue);

int ad_queue_get_node_count(ad_queue *queue);

void ad_queue_destruct(ad_queue *queue);

#endif
