#include "mutex_assam.h"

static __inline__ int __mutex_down(int *counter)
{
	int oval, val = *counter;

	if (val < 0){
		return val;
	}
	
	oval = compare_and_swap(counter, val-1, val);
  
	if (oval == val){ 
		return val-1;
	}
	
	return -1;
}

static __inline__ int __mutex_up(int *counter)
{
	int oval, val = *counter;

	oval = compare_and_swap(counter, val+1, val);

	return (oval == val && oval == 0);
}

static __inline__ int my_mutex_unlock(int *counter)
{
	int val = *counter;

	int oval = compare_and_swap(counter, val+1, val);
  
	return oval;
}

static __inline__ int my_mutex_trylock(int *counter)
{
	int oval, val = *counter;

	oval = compare_and_swap(counter, val-1, val);

	return (oval == val) && (val+1 < 0);
}

/* se bloquea y envia los escrito (datos) antes de activar
 * el semaforo
 */
static __inline__ void my_mutex_lock(void)
{
#ifdef __i386__
	__asm__ __volatile__ ("lock; addl $0,0(%%esp)": : :"memory");
#endif
}
