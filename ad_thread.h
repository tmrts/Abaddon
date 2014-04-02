#ifndef AD_THREAD_H
#define AD_THREAD_H

#include <pthread.h>
#include <setjmp.h>

#include "ad_queue.h"

typedef struct {
    void *data;
    jmp_buf error_jmp;
}ad_thread_request;

#define ERROR_JMP(r) ((r) -> error_jmp)

typedef struct {
    ad_queue_mutex *mutex;
    ad_queue_cond *cond;
    ad_queue *request_queue;
}ad_thread_parameters;

#define REQUEST_QUEUE(p) ((p) -> request_queue)

#define VOIDPTR_TO_INT(ptr) (*((int *) (ptr)))

void ad_thread_cancel_cleanup(void *mutex);

void ad_thread_handle_requests_hook(ad_thread_request *request);

void *ad_thread_handle_requests(void *thread_parameters);

#endif
