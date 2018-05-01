/*
 * Inyection de codigo ensamblador para ayudar al mutex a ejecutarse de mejor manera
 * Se almacena automaticamente el nuevo valor en *men, y si es igual al valor anterior (oldval)
 * entonces establece cuanlquiera de los 2 y limpia el registro del otro.
 * Siempre retorna el anterior vaor de *mem
 * El intercambio "atomico" se realiza si el valor retornado es igual a oldval
 */

# define __arch_compare_and_exchange_val_64_acq(mem, newval, oldval)	\
  ({ __typeof (*mem) ret; \
    __asm __volatile ("lock\n" "cmpxchgq %q2, %1\n"		 \
		       : "=a" (ret), "=m" (*mem)		 \
		       : "r" ((long int) (newval)), "m" (*mem),	 \
			 "0" ((long int) (oldval)));		 \
     ret; })
# define compare_and_swap(mem, newval, oldval)			 \
  __arch_compare_and_exchange_val_64_acq(mem, newval, oldval)
