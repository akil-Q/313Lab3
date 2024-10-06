#include <threading.h>

void t_init()
{
        // TODO
        // initialize thread by initializing various data structures our thread will use, such as stack, contexts, and current_context_idx
        for (int i = 0; i < NUM_CTX; ++i) {
                contexts[i].state = 0;
        }
        
}

int32_t t_create(fptr foo, int32_t arg1, int32_t arg2)
{
        // TODO
        // take an empty entry in the contexts array and initalize entry, and modify it to call the function passed into this func as an argument
}

int32_t t_yield()
{
        // TODO
        // update current thread, then look for a valid thread in contexts and switch to it, returning the number of valid threads
}

void t_finish()
{
        // TODO
        // free all memory used, by first freeing stack memory allocated in the current context, and then resetting the context entry to all 0s
}
