#include <execinfo.h>
#include <limits.h>
#include <string.h>

#include <string>

#include "debugging.h"
#include "misc.h"

using namespace std;

namespace base {

#ifdef __APPLE__

void __attribute__((noinline)) print_stack_trace(FILE* fp /* =? */) {
    const int max_trace = 1024;
    void* callstack[max_trace];
    memset(callstack, 0, sizeof(callstack));
    int frames = backtrace(callstack, max_trace);

    // note that we ignore the first frame: print_stack_trace should not be displayed
    char **str_frames = backtrace_symbols(callstack + 1, frames - 1);
    if (str_frames == nullptr) {
        fprintf(fp, "  *** failed to obtain stack trace!\n");
        return;
    }

    fprintf(fp, "  *** begin stack trace ***\n");
    // demangle C++ function names with c++filt
    auto demangle = popen("c++filt -n", "r+");
    size_t demangled_size = 1024;
    char* demangled = nullptr;
    if (demangle) {
        fprintf(fp, "  *** demangled C++ code using c++filt ***\n");
        demangled = (char *) malloc(demangled_size);
    }
    for (int i = 0; i < frames - 1; i++) {
        if (demangle) {
            string trace = str_frames[i];
            size_t idx = trace.rfind(' ');
            size_t idx2 = trace.rfind(' ', idx - 1);
            idx = trace.rfind(' ', idx2 - 1) + 1;
            string mangled = trace.substr(idx, idx2 - idx);
            string left = trace.substr(0, idx);
            string right = trace.substr(idx2);
            if (mangled.size() + 1 > demangled_size) {
                demangled_size = mangled.size() + 1;
                demangled = (char *) realloc(demangled, demangled_size);
            }
            fprintf(demangle, "%s\n", mangled.c_str());

            ssize_t n_read = ::getline(&demangled, &demangled_size, demangle);
            demangled[n_read - 1] = '\0';

            fprintf(fp, "%s%s%s\n", left.c_str(), demangled, right.c_str());
        } else {
            fprintf(fp, "%s\n", str_frames[i]);
        }
    }
    if (demangle) {
        free(demangled);
        pclose(demangle);
    }
    fprintf(fp, "  ***  end stack trace  ***\n");

    free(str_frames);
}

#else // no __APPLE__

void __attribute__((noinline)) print_stack_trace(FILE* fp /* =? */) {
    const int max_trace = 1024;
    void* callstack[max_trace];
    memset(callstack, 0, sizeof(callstack));
    int frames = backtrace(callstack, max_trace);

    // note that we ignore the first frame: print_stack_trace should not be displayed
    char **str_frames = backtrace_symbols(callstack + 1, frames - 1);
    if (str_frames == nullptr) {
        fprintf(fp, "  *** failed to obtain stack trace!\n");
        return;
    }

    fprintf(fp, "  *** begin stack trace ***\n");
    const char* exec_path = get_exec_path();
    for (int i = 0; i < frames - 1; i++) {
        bool addr2line_ok = false;
        if (exec_path != nullptr) {
            char buf[PATH_MAX];
            snprintf(buf, PATH_MAX, "addr2line %p -e %s -f -C 2>&1", callstack[i], exec_path);
            auto addr2line = popen(buf, "r");
            if (addr2line) {
                addr2line_ok = true;
                int input_counter = 0;
                while (fgets(buf, sizeof(buf), addr2line)) {
                    if (buf[0] == '?') {
                        addr2line_ok = false;
                        break;
                    }
                    buf[strlen(buf) - 1] = '\0'; // remove \n
                    if (input_counter == 0) {
                        fprintf(fp, "%-3d  %-50s", i, buf);
                    } else {
                        fprintf(fp, "  %s", buf);
                    }
                    input_counter++;
                }
                if (addr2line_ok) {
                    fprintf(fp, "\n");
                }
                pclose(addr2line);
            }
        }
        if (!addr2line_ok) {
            fprintf(fp, "%-3d %s\n", i, str_frames[i]);
        }
    }

    fprintf(fp, "  ***  end stack trace  ***\n");

    free(str_frames);
}

#endif // ifdef __APPLE__

} // namespace base
