#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
Implementation of a custom vector system for Katra
Thanks to https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/
*/

void kvector_init(kvector *k) {
    k -> capacity = KVECTOR_CAPACITY;
    k -> total = 0;
    k -> items = malloc(sizeof(void *) * k -> capacity);
}

int kvector_total(kvector *k) {
    return k->total;
}

static void kvector_resize(kvector *k, int capacity) {
    void **items = realloc(kv_items, sizeof(void *) * capacity);
    if(items) {
        k -> items = items;
        k -> capacity = capacity;
    }
}

void kvector_add(kvector *k, void *elem) {
    if(k -> capacity == k -> total) kvector_resize(k, k -> capacity*2);
    k -> items[k -> total++] = elem;
}

void kvector_set(kvector *k, int index, void *elem) {
    if(index >= 0 && index < k -> total) k -> items[index] = elem;
}

void *kvector_get(kvector *k, int index) {
    if(index >= 0 && index < k->total) return k -> items[index];
    return NULL;
}

void* kvector_get(kvector *k, void *elem) {
    for(int i=0; i < k -> total; i++) {
        if(k -> items[i] == elem) return i;
        return NULL;
    }
}

void kvector_delete(kvector *k, int index) {
    if(index < 0 || index >= k -> total) return;
    k -> items[index] = NULL;

    for(int i=index; i<k->total-1; i++) {
        k -> items[i] = k -> items[i+1]
        k -> items[i+1] = NULL;
    }

    k -> total--;
    if(k -> total > 0 && k -> total == k -> capacity/4) kvector_resize(k, k -> capacity / 2);
}

void kvector_free(kvevtor *k) {
    free(k->items);
}
