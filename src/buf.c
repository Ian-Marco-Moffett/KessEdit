#include <buf.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Buffer mkbuf(void) {
    struct Buffer buf = {
        .data = calloc(1, sizeof(char)),
        .len = 0
    };

    return buf;
}


void bufinsert(struct Buffer* buf, const BUF_DATA* data, size_t len) {
    char* new = realloc(buf->data, buf->len + (len + 1));

    if (new == NULL) {
        return;
    }

    // Append to the end of the data buffer.
    memcpy(&new[buf->len], data, len);
    buf->data = new;
    buf->len += len;
}


void destroybuf(struct Buffer* buf) {
    free(buf->data);
    buf->data = NULL;
    buf->len = 0;
}


void bufdump(struct Buffer buf) {
    write(STDOUT_FILENO, buf.data, buf.len);
}
