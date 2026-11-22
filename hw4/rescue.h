#ifndef RESCUE_H
#define RESCUE_H



#include "circular_q.h"
#include <stdint.h>



#define _POSIX_C_SOURCE 200809L

#ifndef FAST_READ
#define FAST_READ 1
#endif



#ifndef SAFE
#define SAFE 0
#endif

#ifndef UNSAFE
#define UNSAFE 1
#endif

#ifndef START
#define START 2
#endif

#ifndef TARGET
#define TARGET 3
#endif



// ENUMS ------------------------------------------------------------
enum Errors {
	NO_ERROR,
	INPUT_ERROR,
	ALLOC_ERROR,
	RANGE_ERROR
};

// FUNCS ------------------------------------------------------------
static inline uint8_t read_bit();

static inline uint16_t read_uint();

void alloc_matrix(uint8_t** m, uint16_t R, uint16_t C);

void fill_matrix(uint8_t* m, uint16_t R, uint16_t C, uint32_t* f_count);

void print_matrix(uint8_t* m, uint16_t R, uint16_t C);

// translate a visual coord from the task to actual one
static inline uint32_t visual_to_actual(uint32_t visual_coord);

uint8_t parse_input(uint16_t* R, uint16_t* C, uint16_t* F, uint32_t* S, uint32_t* T, uint8_t** m, uint32_t* f_count);



#endif // RESCUE_H

