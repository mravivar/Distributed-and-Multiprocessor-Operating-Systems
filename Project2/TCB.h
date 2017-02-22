#include <ucontext.h>
#include <string.h>
#include <stdlib.h>

struct TCB_t {
    struct TCB_t *next;
    struct TCB_t *prev;
    int thread_id;
    ucontext_t context;
};

void init_TCB(struct TCB_t *tcb, void *function, void *stackP, int stack_size)
{

    memset(tcb, '\0', sizeof(struct TCB_t));
    getcontext(&tcb->context);
    // tcb->context.uc_stack.ss_sp = stackP;
    tcb->context.uc_stack.ss_sp = malloc(8192);
    // tcb->context.uc_stack.ss_size = (size_t) stack_size;
    tcb->context.uc_stack.ss_size = 8192;
    makecontext(&tcb->context, function, 0);
}
