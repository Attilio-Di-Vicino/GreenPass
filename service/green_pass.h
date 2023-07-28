#ifndef GREEN_PASS_H
#define GREEN_PASS_H

#include <time.h>

#define CODE_LENGTH 16
// Definizione del periodo di validità (6 mesi in secondi)
#define VALIDITY_PERIOD (6 * 30 * 24 * 60 * 60)
// #define WRITE_GP 0
// #define CHECK_GP 1
// #define VALIDATION_GP 2

typedef struct green_pass {
    char code[ CODE_LENGTH ];
    time_t valid_from;
    time_t valid_until;
    int service;
} GreenPass;

#endif // GREEN_PASS_H