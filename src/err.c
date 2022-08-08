#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void restore_state(void);

void die(const char* e) { 
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    restore_state();
    printf("ERROR: %s\n", e);
    exit(1);
}
