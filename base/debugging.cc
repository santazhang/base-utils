#include <execinfo.h>
#include <string.h>

#include "debugging.h"

namespace base {

void __attribute__((noinline)) print_stack_trace(FILE* fp /* =? */) {
    void* callstack[1024];
    memset(callstack, 0, sizeof(callstack));
    int frames = backtrace(callstack, 1024);

    fprintf(fp, "  *** begin stack trace ***\n");
    // note that we ignore the first frame: print_stack_trace should not be displayed
    backtrace_symbols_fd(callstack + 1, frames - 1, fileno(fp));
    fprintf(fp, "  ***  end stack trace  ***\n");
}

} // namespace base
