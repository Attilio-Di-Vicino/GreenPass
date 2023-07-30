#ifndef TODO_H
#define TODO_H

#include "green_pass.h"

#define INVALIDATE "INV"
#define RESTORE "RES"
#define ISVALID "ISV"
#define OTHER "OTH"

typedef struct toDo {
    char code[ 17 ];
    char todo[ 4 ];
} todo;

#endif // TODO_H