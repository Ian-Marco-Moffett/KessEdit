#ifndef BUF_H
#define BUF_H

#include <stddef.h>

typedef char BUF_DATA;


struct Buffer {
    BUF_DATA* data;
    size_t len;
};


struct Buffer mkbuf(void);
void destroybuf(struct Buffer* buf);
void bufinsert(struct Buffer* buf, const BUF_DATA* data, size_t len);
void bufdump(struct Buffer buf);            // Dumps buffer into stdout.

#endif
