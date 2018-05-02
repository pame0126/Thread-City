extern void mythread_q_init(mythread_private_t *node);
extern void mythread_q_add(mythread_private_t *node);
extern void mythread_q_delete(mythread_private_t *node);
extern void mythread_q_state_display();
extern int mythread_q_len();
extern mythread_private_t * mythread_q_search(unsigned long );
