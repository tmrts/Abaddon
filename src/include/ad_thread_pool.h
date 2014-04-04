/**************************************************************************
 *  Abaddon HTTP Server
 *  Copyright (C) 2014  Tamer Tas
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/

#ifndef AD_THREAD_POOL_H
#define AD_THREAD_POOL_H

#include <pthread.h>

#include "ad_queue.h"
#include "ad_thread.h"

#define AD_THREAD_POOL_MIN_THREADS 5
#define AD_THREAD_POOL_MAX_THREADS 15

/* If we ever need to turn pthread_t into a 
 * struct containing info like tid, etc.
 */
typedef pthread_t ad_thread;
typedef pthread_attr_t ad_thread_attr;

typedef pthread_mutex_t ad_thread_pool_mutex;
typedef pthread_cond_t ad_thread_pool_cond;

typedef struct {
    ad_thread *pool_maintainer;
    ad_queue *thread_queue;
    ad_queue *request_queue;
}ad_thread_pool;

#define MAINTAINER(p)   ((p)->pool_maintainer)
#define THREAD_QUEUE(p) ((p)->thread_queue)

ad_thread_pool *ad_thread_pool_construct(ad_thread_pool_mutex *mutex, 
                                         ad_thread_pool_cond *cond_var,
                                         ad_queue *request_queue);

void ad_thread_pool_create_thread(ad_thread_pool *thread_pool);

int ad_thread_pool_delete_thread(ad_thread_pool *thread_pool);

int ad_thread_pool_get_thread_count(ad_thread_pool *thread_pool);

void ad_thread_pool_destruct(ad_thread_pool *thread_pool);

void *ad_thread_pool_maintain(void *thread_pool);

#endif
