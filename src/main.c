#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <editor.h>
#include <stdio.h>
#include <err.h>

static struct termios normal_state;


void restore_state(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &normal_state);
}

static void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &normal_state);
    struct termios raw = normal_state;
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main(int argc, char** argv) {
    if (argc > 2) {
        die("Too many args!");
    }

    atexit(restore_state);
    enable_raw_mode();

    if (argc < 2) {
        run(NULL);
    } else {
        run(argv[1]); 
    }

    // Clear terminal.
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    restore_state();

    return 0;
}
