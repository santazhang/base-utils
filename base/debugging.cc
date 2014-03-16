#include <execinfo.h>

#include "debugging.h"

namespace base {

void __attribute__((noinline)) print_stack_trace(FILE* fp /* =? */) {
    void* callstack[1024];
    int frames = backtrace(callstack, 1024);

    // note that we ignore the first frame: print_stack_trace should not be displayed
    backtrace_symbols_fd(callstack + 1, frames - 1, fileno(fp));
}

} // namespace base
