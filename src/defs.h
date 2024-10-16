#ifndef KVECTOR_H
#define KVECTOR_H

#define KVECTOR_CAPACITY 4

#define KVECTOR_INIT(kvec) kvector kvec; kvector_init(&kvec)
#define KVECTOR_ADD(kvec, elem) kvector_add(&kvec, (void *) item)
#define KVECTOR_SET(kvec, id, elem) kvector_set(&kvec, id, (void *) elem)
#define KVECTOR_GET(kvec, type, id) (type) kvector_get(&kvec, id)
#define KVECTOR_GET_BY_INDEX(kvec, elem) kvector_get_by_index(&kvec, (void *) elem)
#define KVECTOR_DELETE(kvec, id) kvector_delete(&kvec, id)
#define KVECTOR_TOTAL(kvec) kvector_total(&kvec)
#define KVECTOR_FREE(kvec) kvector_free(&kvec)
#include "tokens.h" 

// struct KToken {
//     TokenType type;
//     const char * curChar;
//     int curPos, curLine;
// }

typedef struct kvector {
    void **items;
    int capacity;
    int total;
} kvector;

void kvector_init(kvector *);
int kvector_total(kvector *);
static void kvector_resize(kvector *, int);
void kvector_add(kvector *, void *);
void kvector_set(kvector *, int, void *);
void *kvector_get(kvector *, int);
void* kvector_get_by_index(kvector *, void *);
void kvector_delete(kvector *, int);
void kvector_free(kvector *);

#endif
