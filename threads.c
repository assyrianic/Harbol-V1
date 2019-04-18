#ifdef OS_WINDOWS
#	define HARBOL_LIB
#endif

#include "harbol.h"

#ifdef OS_WINDOWS
#	include <process.h>
#	include <sys/timeb.h>
#else
#	include <signal.h>
#	include <sched.h>
#	include <unistd.h>
#	include <sys/time.h>
#	include <errno.h>
#endif


#if !(defined(__STDC_VERSION__) && defined(C11)) && !defined(HARBOL_THREAD_LOCAL)
#	if defined(COMPILER_GCC) || defined(COMPILER_INTEL)
#		define HARBOL_THREAD_LOCAL __thread
#	else
#		define HARBOL_THREAD_LOCAL __declspec(thread)
#	endif
#elif defined(COMPILER_GCC) && defined(__GNUC_MINOR__) && (((__GNUC__ << 8) | __GNUC_MINOR__) < ((4 << 8) | 9))
#	define HARBOL_THREAD_LOCAL __thread
#endif

HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_init(HarbolMutex *const mutex, const enum HarbolMutexType mutex_type)
{
	if( !mutex )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	mutex->Locked = false;
	mutex->Recursive = mutex_type & HarbolMutexTypeRecursive;
	mutex->Timed = mutex_type & mutex_timed;
	if( !mutex->Timed )
		InitializeCriticalSection(&mutex->MutexHandle.CriticalSect);
	else {
		mutex->MutexHandle.Mutex = CreateMutex(NULL, 0, NULL);
		if( !mutex->MutexHandle.Mutex )
			return HarbolThreadResError;
	}
	return HarbolThreadResSuccess;
#else
	pthread_mutexattr_t *const attr = &(pthread_mutexattr_t){0};
	pthread_mutexattr_init(attr);
	if( mutex_type & HarbolMutexTypeRecursive )
		pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE_NP);
	const int res = pthread_mutex_init(mutex, attr);
	pthread_mutexattr_destroy(attr);
	return( !res ? HarbolThreadResSuccess : HarbolThreadResError );
#endif
}

HARBOL_EXPORT bool harbol_mutex_del(HarbolMutex *const mutex)
{
	if( !mutex )
		return false;
#ifdef OS_WINDOWS
	if( !mutex->Timed )
		DeleteCriticalSection(&mutex->MutexHandle.CriticalSect);
	else CloseHandle(mutex->MutexHandle.Mutex);
	memset(mutex, 0, sizeof *mutex);
#else
	pthread_mutex_destroy(mutex);
#endif
	return true;
}

HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_lock(HarbolMutex *const mutex)
{
#ifdef OS_WINDOWS
	if( !mutex->Timed ) {
		EnterCriticalSection(&mutex->MutexHandle.CriticalSect);
	} else {
		switch( WaitForSingleObject(mutex->MutexHandle.Mutex, INFINITE) ) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_ABANDONED:
			default:
				return HarbolThreadResError;
		}
	}
	if( !mutex->Recursive ) {
		while( mutex->Locked )
			Sleep(1);
		mutex->Locked = true;
	}
	return HarbolThreadResSuccess;
#else
	return !pthread_mutex_lock(mutex) ? HarbolThreadResSuccess : HarbolThreadResError;
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_timedlock(HarbolMutex *const mutex, const struct timespec *ts)
{
	if( !mutex || !ts )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	struct timespec current_ts = {0};
	uint32_t timeout_millisec = 0;
	
	if( !mutex->Timed )
		return HarbolThreadResError;
	
	timespec_get(&current_ts, TIME_UTC);
	if( (current_ts.tv_sec > ts->tv_sec) || ((current_ts.tv_sec == ts->tv_sec) && (current_ts.tv_nsec >= ts->tv_nsec)) ) {
		timeout_millisec = 0;
	} else {
		timeout_millisec = (uint32_t)(ts->tv_sec	- current_ts.tv_sec) * 1000;
		timeout_millisec += (ts->tv_nsec - current_ts.tv_nsec) / 1000000;
		timeout_millisec += 1;
	}
	
	switch( WaitForSingleObject(mutex->MutexHandle.Mutex, timeout_millisec) ) {
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			return HarbolThreadResTimedOut;
		case WAIT_ABANDONED:
		default:
			return HarbolThreadResError;
	}
	
	if( !mutex->Recursive ) {
		while( mutex->Locked )
			Sleep(1);
		mutex->Locked = true;
	}
	return HarbolThreadResSuccess;
#elif defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >= 200112L) && defined(_POSIX_THREADS) && (_POSIX_THREADS >= 200112L)
	switch( pthread_mutex_timedlock(mutex, ts) ) {
		case 0:
			return HarbolThreadResSuccess;
		case ETIMEDOUT:
			return HarbolThreadResTimedOut;
		default:
			return HarbolThreadResError;
	}
#else
	int rc = 0;
	while( (rc = pthread_mutex_trylock(mutex)) == EBUSY ) {
		struct timespec
			curr = {0},
			dur = {0}
		;
		timespec_get(&curr, TIME_UTC);
		if( (curr.tv_sec > ts->tv_sec) || ((curr.tv_sec == ts->tv_sec) && (curr.tv_nsec >= ts->tv_nsec)) )
			break;
		
		dur.tv_sec = ts->tv_sec - curr.tv_sec;
		dur.tv_nsec = ts->tv_nsec - curr.tv_nsec;
		if( dur.tv_nsec<0 ) {
			dur.tv_sec--;
			dur.tv_nsec += 1000000000;
		}
		if( (dur.tv_sec != 0) || (dur.tv_nsec > 5000000) ) {
			dur.tv_sec = 0;
			dur.tv_nsec = 5000000;
		}
		nanosleep(&dur, NULL);
	}
	
	switch( rc ) {
		case 0:
			return HarbolThreadResSuccess;
		case ETIMEDOUT: case EBUSY:
			return HarbolThreadResTimedOut;
		default:
			return HarbolThreadResError;
	}
#endif
}


HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_trylock(HarbolMutex *const mutex)
{
	if( !mutex )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	enum HarbolThreadRes res = 0;
	if( !mutex->Timed )
		res = TryEnterCriticalSection(&mutex->MutexHandle.CriticalSect) ? HarbolThreadResSuccess : HarbolThreadResBusy;
	else res = (WaitForSingleObject(mutex->MutexHandle.Mutex, 0) == WAIT_OBJECT_0) ? HarbolThreadResSuccess : HarbolThreadResBusy;
	
	if( !mutex->Recursive && res==HarbolThreadResSuccess ) {
		if( mutex->Locked ) {
			LeaveCriticalSection(&mutex->MutexHandle.CriticalSect);
			res = HarbolThreadResBusy;
		}
		else mutex->Locked = true;
	}
	return res;
#else
	return( !pthread_mutex_trylock(mutex) ? HarbolThreadResSuccess : HarbolThreadResBusy );
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_mutex_unlock(HarbolMutex *const mutex)
{
	if( !mutex )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	mutex->Locked = false;
	if( !mutex->Timed ) {
		LeaveCriticalSection(&mutex->MutexHandle.CriticalSect);
	} else {
		if( !ReleaseMutex(mutex->MutexHandle.Mutex) )
			return HarbolThreadResError;
	}
	return HarbolThreadResSuccess;
#else
	return( !pthread_mutex_unlock(mutex) ? HarbolThreadResSuccess : HarbolThreadResError );
#endif
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

#ifdef OS_WINDOWS
#	ifndef HARBOL_COND_EVENT_ONE
#		define HARBOL_COND_EVENT_ONE 0
#	endif
#	ifndef HARBOL_COND_EVENT_ALL
#		define HARBOL_COND_EVENT_ALL 1
#	endif
#endif

HARBOL_EXPORT enum HarbolThreadRes harbol_cond_init(HarbolCond *const cond)
{
	if( !cond )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	cond->CountWaiting = 0;
	InitializeCriticalSection(&cond->CountWaitingLock);
	
	cond->EventHandles[HARBOL_COND_EVENT_ONE] = CreateEvent(NULL, 0, 0, NULL);
	if( !cond->EventHandles[HARBOL_COND_EVENT_ONE] ) {
		cond->EventHandles[HARBOL_COND_EVENT_ALL] = NULL;
		return HarbolThreadResError;
	}
	
	cond->EventHandles[HARBOL_COND_EVENT_ALL] = CreateEvent(NULL, 1, 0, NULL);
	if( !cond->EventHandles[HARBOL_COND_EVENT_ALL] ) {
		CloseHandle(cond->EventHandles[HARBOL_COND_EVENT_ONE]);
		cond->EventHandles[HARBOL_COND_EVENT_ONE] = NULL;
		return HarbolThreadResError;
	}
	else return HarbolThreadResSuccess;
#else
	return( !pthread_cond_init(cond, NULL) ? HarbolThreadResSuccess : HarbolThreadResError );
#endif
}

HARBOL_EXPORT void harbol_cond_del(HarbolCond *const cond)
{
#ifdef OS_WINDOWS
	if( cond->EventHandles[HARBOL_COND_EVENT_ONE] )
		CloseHandle(cond->EventHandles[HARBOL_COND_EVENT_ONE]);
	if( cond->EventHandles[HARBOL_COND_EVENT_ALL] )
		CloseHandle(cond->EventHandles[HARBOL_COND_EVENT_ALL]);
	DeleteCriticalSection(&cond->CountWaitingLock);
	memset(cond, 0, sizeof *cond);
#else
	pthread_cond_destroy(cond);
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_cond_signal(HarbolCond *const cond)
{
#ifdef OS_WINDOWS
	EnterCriticalSection(&cond->CountWaitingLock);
	const bool has_waiting = (cond->CountWaiting > 0);
	LeaveCriticalSection(&cond->CountWaitingLock);
	
	if( has_waiting )
		if( !SetEvent(cond->EventHandles[HARBOL_COND_EVENT_ONE]) )
			return HarbolThreadResError;
	return HarbolThreadResSuccess;
#else
	return( !pthread_cond_signal(cond) ? HarbolThreadResSuccess : HarbolThreadResError );
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_cond_broadcast(HarbolCond *const cond)
{
#ifdef OS_WINDOWS
	EnterCriticalSection(&cond->CountWaitingLock);
	const bool has_waiting = (cond->CountWaiting > 0);
	LeaveCriticalSection(&cond->CountWaitingLock);
	
	if( haveWaiters )
		if( !SetEvent(cond->EventHandles[HARBOL_COND_EVENT_ALL]) )
			return HarbolThreadResError;
	
	return HarbolThreadResSuccess;
#else
	return( !pthread_cond_broadcast(cond) ? HarbolThreadResSuccess : HarbolThreadResError );
#endif
}

#ifdef OS_WINDOWS
static enum HarbolThreadRes _harbol_cond_timedwait_win32(HarbolCond *const cond, HarbolMutex *const mutex, const uint32_t timeout)
{
	EnterCriticalSection(&cond->CountWaitingLock);
	cond->CountWaiting++;
	LeaveCriticalSection(&cond->CountWaitingLock);
	harbol_mutex_unlock(mutex);
	
	const uint32_t result = WaitForMultipleObjects(2, cond->EventHandles, 0, timeout);
	if( result==WAIT_TIMEOUT ) {
		harbol_mutex_lock(mutex);
		return HarbolThreadResTimedOut;
	} else if( result==WAIT_FAILED ) {
		harbol_mutex_lock(mutex);
		return HarbolThreadResError;
	}
	
	EnterCriticalSection(&cond->CountWaitingLock);
	cond->CountWaiting--;
	const int last_wait = (result == (WAIT_OBJECT_0 + HARBOL_COND_EVENT_ALL)) && (cond->CountWaiting == 0);
	LeaveCriticalSection(&cond->CountWaitingLock);
	
	if( last_wait ) {
		if( !ResetEvent(cond->EventHandles[HARBOL_COND_EVENT_ALL]) ) {
			harbol_mutex_lock(mutex);
			return HarbolThreadResError;
		}
	}
	harbol_mutex_lock(mutex);
	return HarbolThreadResSuccess;
}
#endif

HARBOL_EXPORT enum HarbolThreadRes harbol_cond_wait(HarbolCond *const cond, HarbolMutex *const mutex)
{
#ifdef OS_WINDOWS
	return _harbol_cond_timedwait_win32(cond, mutex, INFINITE);
#else
	return !pthread_cond_wait(cond, mutex) ? HarbolThreadResSuccess : HarbolThreadResError;
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_cond_timedwait(HarbolCond *const cond, HarbolMutex *const mutex, const struct timespec *ts)
{
#ifdef OS_WINDOWS
	struct timespec now = {0};
	if( timespec_get(&now, TIME_UTC)==TIME_UTC ) {
		const uint64_t curr_millisecs = now.tv_sec * 1000 + now.tv_nsec / 1000000;
		const uint64_t ts_millisecs	= ts->tv_sec * 1000 + ts->tv_nsec / 1000000;
		const uint32_t delta = (ts_millisecs > curr_millisecs) ? (uint32_t)(ts_millisecs - curr_millisecs) : 0;
		return _harbol_cond_timedwait_win32(cond, mutex, delta);
	}
	else return HarbolThreadResError;
#else
	const int ret = pthread_cond_timedwait(cond, mutex, ts);
	if( ret==ETIMEDOUT )
		return HarbolThreadResTimedOut;
	else return !ret ? HarbolThreadResSuccess : HarbolThreadResError;
#endif
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/


#ifdef OS_WINDOWS
static uint32_t WINAPI _harbol_thread_wrapper_func(LPVOID args)
#else
static void *_harbol_thread_wrapper_func(void *const args)
#endif
{
	struct HarbolThread *const restrict thread_info = args;
	const int res = (*thread_info->Func)(thread_info->Args);
#ifdef OS_WINDOWS
	return (uint32_t)res;
#else
	return (void *)(intptr_t)res;
#endif
}

HARBOL_EXPORT struct HarbolThread *harbol_thread_new(fnHarbolThread *const func, void *args)
{
	struct HarbolThread *const restrict thread = calloc(1, sizeof *thread);
	if( !thread )
		return NULL;
	else {
		const enum HarbolThreadRes res = harbol_thread_init(thread, func, args);
		if( res==HarbolThreadResSuccess )
			return thread;
		else {
			free(thread);
			return NULL;
		}
	}
}

HARBOL_EXPORT enum HarbolThreadRes harbol_thread_init(struct HarbolThread *const restrict thr, fnHarbolThread *const func, void *const args)
{
	if( !thr )
		return HarbolThreadResError;
	thr->Func = func;
	thr->Args = args;
#ifdef OS_WINDOWS
	thr->Thread = CreateThread(NULL, 0, _harbol_thread_wrapper_func, thr, 0, NULL);
#else
	if( pthread_create(&thr->Thread, NULL, _harbol_thread_wrapper_func, thr) != 0 )
		thr->Thread = 0;
#endif
	if( !thr->Thread )
		return HarbolThreadResError;
	else return HarbolThreadResSuccess;
}

HARBOL_EXPORT HarbolThreadCtxt harbol_thread_get_curr(void)
{
#ifdef OS_WINDOWS
	return GetCurrentThread();
#else
	return pthread_self();
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_thread_detach(struct HarbolThread *const thr)
{
	if( !thr )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	return CloseHandle(thr->Thread) != 0 ? HarbolThreadResSuccess : HarbolThreadResError;
#else
	return !pthread_detach(thr->Thread) ? HarbolThreadResSuccess : HarbolThreadResError;
#endif
}

HARBOL_EXPORT bool harbol_thread_equal(struct HarbolThread *const thrA, struct HarbolThread *const thrB)
{
	if( !thrA || !thrB )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	return GetThreadId(thrA->Thread) == GetThreadId(thrB->Thread);
#else
	return pthread_equal(thrA->Thread, thrB->Thread);
#endif
}

HARBOL_EXPORT void harbol_thread_exit(const int res, void *restrict retval)
{
#ifdef OS_WINDOWS
	(void)retval;
	ExitThread((uint32_t)res);
#else
	(void)res;
	pthread_exit(retval);
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_thread_join(struct HarbolThread *const thr, int *const res)
{
	if( !thr )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	if( WaitForSingleObject(thr->Thread, INFINITE)==WAIT_FAILED )
		return HarbolThreadResError;
	
	if( res ) {
		uint32_t thread_res = 0;
		if( GetExitCodeThread(thr->Thread, &thread_res) != 0 )
			*res = (int)thread_res;
		else return HarbolThreadResError;
	}
	CloseHandle(thr->Thread);
#else
	void *ptr_res = NULL;
	if( pthread_join(thr->Thread, &ptr_res) != 0 )
		return HarbolThreadResError;
	if( res )
		*res = (int)(intptr_t)ptr_res;
#endif
	return HarbolThreadResSuccess;
}

HARBOL_EXPORT int harbol_thread_sleep(const struct timespec *const duration, struct timespec *const remaining)
{
#if !defined(OS_WINDOWS)
	const int res = nanosleep(duration, remaining);
	if( !res ) {
		return 0;
	} else if( errno==EINTR ) {
		return -1;
	} else {
		return -2;
	}
#else
	struct timespec start = {0};
	timespec_get(&start, TIME_UTC);
	const uint32_t t = SleepEx((uint32_t)(duration->tv_sec * 1000 + duration->tv_nsec / 1000000 + (((duration->tv_nsec % 1000000) == 0) ? 0 : 1)), 1);
	
	if( !t ) {
		return 0;
	} else {
		if( remaining ) {
			timespec_get(remaining, TIME_UTC);
			remaining->tv_sec -= start.tv_sec;
			remaining->tv_nsec -= start.tv_nsec;
			if( remaining->tv_nsec<0 ) {
				remaining->tv_nsec += 1000000000;
				remaining->tv_sec -= 1;
			}
		}
		return( t==WAIT_IO_COMPLETION ) ? -1 : -2;
	}
#endif
}

HARBOL_EXPORT void harbol_thread_yield(void)
{
#ifdef OS_WINDOWS
	Sleep(0);
#else
	sched_yield();
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_tss_create(struct HarbolTSS *const key, fnHarbolStorageDtor *const dtor)
{
	if( !key )
		return HarbolThreadResError;
#ifdef OS_WINDOWS
	key->Key = TlsAlloc();
	if( key->Key==TLS_OUT_OF_INDEXES )
		return HarbolThreadResError;
#else
	if( pthread_key_create(&key->Key, dtor) != 0 )
		return HarbolThreadResError;
#endif
	else return HarbolThreadResSuccess;
}

HARBOL_EXPORT void harbol_tss_delete(struct HarbolTSS *key)
{
	if( !key )
		return;
#ifdef OS_WINDOWS
	if( key->CacheDtor )
		(*key->CacheDtor)(key->CacheVal);
	TlsFree(key->Key);
#else
	pthread_key_delete(key->Key);
#endif
	memset(key, 0, sizeof *key);
}

HARBOL_EXPORT void *harbol_tss_get(struct HarbolTSS *key)
{
	if( !key )
		return NULL;
#ifdef OS_WINDOWS
	return TlsGetValue(key->Key);
#else
	return pthread_getspecific(key->Key);
#endif
}

HARBOL_EXPORT enum HarbolThreadRes harbol_tss_set(struct HarbolTSS *const restrict key, void *val)
{
#ifdef OS_WINDOWS
	if( !TlsSetValue(key->Key, val) )
		return HarbolThreadResError;
	else key->CacheVal = val;
#else
	if( pthread_setspecific(key->Key, val) != 0 )
		return HarbolThreadResError;
#endif
	return HarbolThreadResSuccess;
}
