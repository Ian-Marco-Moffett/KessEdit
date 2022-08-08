#include <term.h>
#include <sys/ioctl.h>
#include <unistd.h>

int getwinsize(uint32_t* rows, uint32_t* cols) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
}
