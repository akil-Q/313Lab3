#include <threading.h>


void t_init()
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

        makecontext(&contexts[empty].context, (void (*)())foo, 2, arg1, arg2);
        contexts[empty].state = VALID;
        return 0;
}

int32_t t_yield(void)
{
        int counts = 0;
        volatile int next = (current_context_idx + 1 )% NUM_CTX;

        // Save the current context
        getcontext(&contexts[current_context_idx].context);

        // Find the next valid context
        while (contexts[next].state != VALID && next != current_context_idx) {
                next = (next + 1) % NUM_CTX;
        }

        if (next != current_context_idx) {
                int orig = current_context_idx;
                current_context_idx = (uint8_t)next;
                swapcontext(&contexts[orig].context, &contexts[current_context_idx].context);
        }

        // Count valid contexts
        for (int i = 0; i < NUM_CTX; ++i) {
        if (contexts[i].state == VALID) {
                counts++;
        }
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

