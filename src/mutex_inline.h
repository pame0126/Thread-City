#include "mutex_assam.h"

static __inline__ int __mutex_down(int *contador)
{
	int ovalor, valor = *contador;

	if (valor < 0){
		return valor;
	}

	ovalor = compare_and_swap(contador, valor-1, valor);

	if (ovalor == valor){
		return valor-1;
	}

	return -1;
}

static __inline__ int __mutex_up(int *contador)
{
	int ovalor, valor = *contador;

	ovalor = compare_and_swap(contador, valor+1, valor);

	return (ovalor == valor && ovalor == 0);
}

static __inline__ int my_mutex_unlock(int *contador)
{
	int valor = *contador;

	int ovalor = compare_and_swap(contador, valor+1, valor);

	return ovalor;
}

static __inline__ int my_mutex_trylock(int *contador)
{
	int ovalor, valor = *contador;

	ovalor = compare_and_swap(contador, valor-1, valor);

	return (ovalor == valor) && (valor+1 < 0);
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
