#include <threading.h>


void t_init(void)
{
        // TODO
        // initialize thread by initializing various data structures our thread will use, such as stack, contexts, and current_context_idx
        for (int i = 0; i < NUM_CTX; ++i) {
                contexts[i].state = INVALID;
        }
        current_context_idx = 0;
        getcontext(&contexts[0].context);
        contexts[current_context_idx].state = VALID;

        contexts[0].context.uc_stack.ss_sp = malloc(STK_SZ);
        contexts[0].context.uc_stack.ss_size = STK_SZ;
        contexts[0].context.uc_stack.ss_flags = 0;
        contexts[0].context.uc_link = NULL;
}

int32_t t_create(fptr foo, int32_t arg1, int32_t arg2)
{
        // TODO
        // take an empty entry in the contexts array and initalize entry, and modify it to call the function passed into this func as an argument
        uint8_t empty;
        for (empty = 0; empty < NUM_CTX; ++empty) {
                if (contexts[empty].state == INVALID) break;
        }
        if (empty == NUM_CTX) return 1;

        current_context_idx = empty;
        getcontext(&contexts[empty].context);

        contexts[empty].context.uc_stack.ss_sp = (char*) malloc(STK_SZ);
        contexts[empty].context.uc_stack.ss_size = STK_SZ;
        contexts[empty].context.uc_stack.ss_flags = 0;
        contexts[empty].context.uc_link = NULL;
        contexts[empty].state = VALID;

        makecontext(&contexts[empty].context, (void (*)(void))foo, 2, arg1, arg2);

        return 0;
}

int32_t t_yield(void)
{
        // TODO
        // update current thread, then look for a valid thread in contexts and switch to it, returning the number of valid threads
        int counts = 0;
        uint8_t swap;
        for (swap = 0; swap < NUM_CTX; ++swap) {
                if (contexts[swap].state == VALID && swap != current_context_idx) break;
        }
        if (swap == NUM_CTX) return 1;
        swapcontext(&contexts[current_context_idx].context, &contexts[swap].context);
        current_context_idx = swap;
        for (int i = 0; i < NUM_CTX; ++i) {
                if (contexts[i].state == VALID) counts++;
        }
        return counts;
}

void t_finish(void)
{
        // TODO
        // free all memory used, by first freeing stack memory allocated in the current context, and then resetting the context entry to all 0s
        free(contexts[current_context_idx].context.uc_stack.ss_sp);
        memset(&contexts[current_context_idx], 0, sizeof(struct worker_context));
        contexts[current_context_idx].state = DONE;
        t_yield();
}

