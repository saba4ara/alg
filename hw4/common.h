#ifndef COMMON_H
#define COMMON_H



#include <stdint.h>



// ENUMS ------------------------------------------------------------
enum Errors {
	NO_ERROR,
	INPUT_ERROR,
	ALLOC_ERROR,
	RANGE_ERROR
};



// STRUCTS ----------------------------------------------------------
typedef struct {
	uint16_t r;
	uint16_t c;
	uint8_t fires;
} State;



#endif // COMMON_H

