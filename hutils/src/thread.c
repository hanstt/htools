/*
 * Copyright (c) 2015 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <hutils/thread.h>
#include <hutils/memory.h>

#if defined(_MSC_VER)

# include <windows.h>
# include <process.h>

struct Mutex {
	CRITICAL_SECTION	cs;
};
struct Starter {
	void	(*func)(void *);
	void	*data;
};
struct Thread {
	HANDLE	handle;
};

static unsigned int __stdcall	run(void *);

unsigned int __stdcall
run(void *a_data)
{
	struct Starter starter;

	memcpy(&starter, a_data, sizeof starter);
	FREE(a_data);
	starter.func(starter.data);
	return 0;
}

struct Thread *
thread_create(void (*const a_func)(void *), void *const a_data)
{
	struct Starter *starter;
	struct Thread *thread;
	uintptr_t ret;

	CALLOC(starter, 1);
	starter->func = a_func;
	starter->data = a_data;
	ret = _beginthreadex(NULL, 0, run, starter, 0, NULL);
	if (-1 == ret) {
		FREE(starter);
		return NULL;
	}
	CALLOC(thread, 1);
	thread->handle = (HANDLE)ret;
	return thread;
}

void
thread_free(struct Thread **const a_thread)
{
	struct Thread *thread;

	thread = *a_thread;
	if (NULL == thread) {
		return;
	}
	WaitForSingleObject(thread->handle, INFINITE);
	CloseHandle(thread->handle);
	FREE(*a_thread);
}

struct Mutex *
thread_mutex_create()
{
	struct Mutex *mutex;

	CALLOC(mutex, 1);
	if (!InitializeCriticalSectionAndSpinCount(&mutex->cs, 0x00000400)) {
		return NULL;
	}
	return mutex;
}

void
thread_mutex_free(struct Mutex **const a_mutex)
{
	struct Mutex *mutex;

	mutex = *a_mutex;
	if (NULL == mutex) {
		return;
	}
	DeleteCriticalSection(&mutex->cs);
	FREE(*a_mutex);
}

void 
thread_mutex_lock(struct Mutex *const a_mutex)
{
	EnterCriticalSection(&a_mutex->cs);
}

void
thread_mutex_unlock(struct Mutex *const a_mutex)
{
	LeaveCriticalSection(&a_mutex->cs);
}

#elif defined(__linux__) || defined(__OpenBSD__)

# include <pthread.h>

struct Mutex {
	pthread_mutex_t	mutex;
};
struct Starter {
	void	(*func)(void *);
	void	*data;
};
struct Thread {
	pthread_t	thread;
};

static void	*run(void *);

void *
run(void *a_data)
{
	struct Starter starter;

	memcpy(&starter, a_data, sizeof starter);
	FREE(a_data);
	starter.func(starter.data);
	return NULL;
}

struct Thread *
thread_create(void (*const a_func)(void *), void *const a_data)
{
	struct Starter *starter;
	struct Thread *thread;
	int ret;

	CALLOC(starter, 1);
	starter->func = a_func;
	starter->data = a_data;

	CALLOC(thread, 1);
	ret = pthread_create(&thread->thread, NULL, run, starter);
	if (0 != ret) {
		FREE(thread);
		return NULL;
	}
	return thread;
}

void
thread_free(struct Thread **const a_thread)
{
	struct Thread *thread;

	thread = *a_thread;
	if (NULL == thread) {
		return;
	}
	pthread_join(thread->thread, NULL);
	FREE(*a_thread);
}

struct Mutex *
thread_mutex_create()
{
	struct Mutex *mutex;
	int ret;

	CALLOC(mutex, 1);
	ret = pthread_mutex_init(&mutex->mutex, NULL);
	if (0 != ret) {
		FREE(mutex);
		return NULL;
	}
	return mutex;
}

void
thread_mutex_free(struct Mutex **const a_mutex)
{
	struct Mutex *mutex;
	int ret;

	mutex = *a_mutex;
	if (NULL == mutex) {
		return;
	}
	ret = pthread_mutex_destroy(&mutex->mutex);
	if (0 != ret) {
		fprintf(stderr, "Could not destroy mutex.\n");
	}
	FREE(*a_mutex);
}

void 
thread_mutex_lock(struct Mutex *const a_mutex)
{
	pthread_mutex_lock(&a_mutex->mutex);
}

void
thread_mutex_unlock(struct Mutex *const a_mutex)
{
	pthread_mutex_unlock(&a_mutex->mutex);
}

#else
# error "Platform not supported!"
#endif
