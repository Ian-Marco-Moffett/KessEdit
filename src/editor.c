#include <editor.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include <def.h>
#include <err.h>
#include <buf.h>
#include <term.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct Editor {
    uint32_t termrows;
    uint32_t termcols;
    uint16_t cxpos;             // Cursor x position.
    uint16_t cypos;             // Cursor y position.
    struct Buffer row;
    size_t n_rows;
} ed;


/*
 *  The 'b' prefix means buffered as it only
 *  appends the escape codes to move the cursor
 *  into the buffer.
 *
 */

static void bmove_cursor(struct Buffer* cur_buf, uint16_t x, uint16_t y) {
    char buf[50];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y + 1, x + 1);
    bufinsert(cur_buf, buf, strlen(buf));
}

static void draw_welcome_msg(struct Buffer* buf) {
    static uint8_t can_draw = 1;                            // If zero, the welcome message will not be written to stdout.

    if (can_draw) {
        can_draw = 0;
    } else {
        return;
    }

    char welcome[120];
    uint32_t wlen = snprintf(welcome, sizeof(welcome), "KessEdit -- v%s\r\n", KESSEDIT_VERSION);

    // Ensure wlen <= termcols.
    if (wlen > ed.termcols) {
        wlen = ed.termcols;
    }

    // Add padding to center our welcome message.
    uint32_t padding_len = (ed.termcols - wlen) / 2;
            
    bufinsert(buf, "~", 1);
    while (padding_len--) {
        bufinsert(buf, " ", 1);
    }

    bufinsert(buf, welcome, wlen);
}


static void draw_rows(struct Buffer* buf) {
    for (uint32_t i = 0; i < ed.termrows; ++i) {
        bufinsert(buf, "\x1b[K", 3);            // Clear one line at a time.

        if (i == ed.termrows / 3) {
            draw_welcome_msg(buf);
        } else if (i > ed.n_rows) {
            bufinsert(buf, "~\r\n", 3);
        } else if (i < ed.n_rows) {
            bufinsert(buf, ed.row.data, ed.row.len);
            bufinsert(buf, "\r\n", 2);              
        }
    }
}


static void refresh_screen(void) {
    struct Buffer buf = mkbuf();
    bufinsert(&buf, "\x1b[?25l", 6);        // Hide cursor.
    bufinsert(&buf, "\x1b[H", 3);
    draw_rows(&buf);
    bufinsert(&buf, "\x1b[H", 3);
    bufinsert(&buf, "\x1b[?25h", 6);       // Show cursor.
    bmove_cursor(&buf, ed.cxpos, ed.cypos);
    
    // Now write the whole buffer and destroy it.
    bufdump(buf);
    destroybuf(&buf);
}


static char read_key(void) {
    int readret;
    char c;

    while ((readret = read(STDIN_FILENO, &c, 1)) != 1) {
        if (readret < 0 && errno != EAGAIN) {
            die("read");
        }
    }

    if (c == '\x1b') {
        char seq[3];
        read(STDIN_FILENO, &seq[0], 1);
        read(STDIN_FILENO, &seq[1], 1);

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A':
                    return UP_ARROW;
                case 'B':
                    return DOWN_ARROW;
                case 'C':
                    return RIGHT_ARROW;
                case 'D':
                    return LEFT_ARROW;
            }
        }
    }

    return c;
}


static void process_keystroke(char c) {
    switch (c) {
        case UP_ARROW:
            if (ed.cypos > 0) {
                --ed.cypos;
            }

            break;
        case DOWN_ARROW:
            if (ed.cypos < ed.termrows - 2) {
                ++ed.cypos;
            }

            break;
        case LEFT_ARROW:
            if (ed.cxpos > 0) {
                --ed.cxpos;
            }

            break;
        case RIGHT_ARROW:
            if (ed.cxpos < ed.termcols - 1) {
                ++ed.cxpos;
            }

            break;
    }
}

void editor_open(void) {
    ed.n_rows = 1;
    ed.row = mkbuf();
    const char* line = "Hello, World!";
    bufinsert(&ed.row, line, strlen(line));
    write(STDOUT_FILENO, ed.row.data, ed.row.len);

    // Ensure the cursor starts at (0, 0).
    struct Buffer buf = mkbuf();
    bmove_cursor(&buf, 0, 0);
    bufdump(buf);
    destroybuf(&buf);
}


void run(void) {
    ed.cxpos = ed.cypos = 0;
    
    getwinsize(&ed.termrows, &ed.termcols);
    refresh_screen();

    editor_open();

    char c;
    while ((c = read_key()) != CTRL_KEY('q')) {
        process_keystroke(c);
        refresh_screen();
    }
}
