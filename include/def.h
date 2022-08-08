#ifndef DEF_H
#define DEF_H

#define CTRL_KEY(k) ((k) & 0x1F)
#define KESSEDIT_VERSION "0.0.1"

typedef enum {
    LEFT_ARROW = 'a',
    RIGHT_ARROW = 'd',
    UP_ARROW = 'w',
    DOWN_ARROW = 's'
} KEY;

#endif
