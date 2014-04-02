#include <stdio.h>
#include <stdlib.h>

#include "ad_queue.h"

ad_queue *ad_queue_construct(ad_queue_mutex *mutex, ad_queue_cond *cond_var)
{
    ad_queue *queue = malloc(sizeof(ad_queue));

    NODE_COUNT(queue) = 0;
    HEAD(queue)  = NULL;
    LAST(queue)  = NULL;
    MUTEX(queue) = mutex;
    COND(queue)  = cond_var;

    return queue;
}

void ad_queue_push(ad_queue *queue, void *data)
{
    ad_node *node = malloc(sizeof(ad_node)); 

    DATA(node) = data;
    NEXT(node) = NULL;

    pthread_mutex_lock(MUTEX(queue));

    if(!IS_EMPTY(queue))
    {
        NEXT(LAST(queue)) = node;
        LAST(queue) = node;
    }
    else
    {
        HEAD(queue) = node;
        LAST(queue) = node;
    }

    NODE_COUNT(queue)++;

    pthread_mutex_unlock(MUTEX(queue));
    
    /* New node in the queue to be handled. Send the signal */
    pthread_cond_signal(COND(queue));
}

void *ad_queue_pop(ad_queue *queue)
{
    void *data = NULL;
    ad_node *node = NULL;

    pthread_mutex_lock(MUTEX(queue));

    node = HEAD(queue);

    if(!IS_EMPTY(queue))
    {
        if (NODE_COUNT(queue) == 1)
        {
            LAST(queue) = NULL;
        }
        HEAD(queue) = NEXT(HEAD(queue));
        NODE_COUNT(queue)--;
    }

    pthread_mutex_unlock(MUTEX(queue));

    if(node)
    {
        data = DATA(node);
        free(node);
    }

    return data;
}

int ad_queue_get_node_count(ad_queue *queue)
{
    int node_count;

    pthread_mutex_lock(MUTEX(queue));
    node_count = NODE_COUNT(queue);
    pthread_mutex_unlock(MUTEX(queue));

    return node_count;
}

void ad_queue_destruct(ad_queue *queue)
{
    void *data;

    while(data = ad_queue_pop(queue))
    {
        free(data);
    }

    free(queue);
}
