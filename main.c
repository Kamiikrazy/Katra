#include <stdio.h>
#include <stdlib.h>
#include "./src/kvector.h"

int main() {
    kvector kv;
    kvector_init(&kv);

    kvector_add(&kv, "Wassup gangalang");
    for(i=0; i<kvector_total(&kv); i++) {
        printf()
    }
}