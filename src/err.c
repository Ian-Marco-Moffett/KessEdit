#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void die(const char* e) { 
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(e);
    exit(1);
}
