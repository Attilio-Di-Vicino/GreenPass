#ifndef TODO_H
#define TODO_H

#include "green_pass.h"

#define INVALIDATE "INV"
#define RESTORE "RES"
#define ISVALID "ISV"
#define OTHER "OTH"
#define CODE_LENGTH 17
#define TODO_SIZE 4

typedef struct toDo {
    char code[ CODE_LENGTH ];
    char todo[ TODO_SIZE ];
} todo;

#endif // TODO_H