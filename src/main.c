#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <editor.h>

static struct termios normal_state;


static void restore_state(void) {
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


int main(void) {
    atexit(restore_state);
    enable_raw_mode();

    run();

    // Clear terminal.
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    return 0;
}
