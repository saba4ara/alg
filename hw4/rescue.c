#include "rescue.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



// MAIN -------------------------------------------------------------
int main(int argc, char* argv[]) {
	uint16_t R = 0, C = 0, F = 0;
	uint32_t S[2] = {0}, T[2] = {0};
	uint8_t* m;
	
	uint32_t f_count = 0;
	uint32_t best_path_len = 0;

	uint8_t res = 0;
	res = parse_input(&R, &C, &F, S, T, &m, &f_count);
	
	// printf("DEBUG: S = (%u, %u); T = (%u, %u)\n", S[0], S[1], T[0], T[1]);
	// printf("DEBUG: fire count: %hu \n", f_count);
	
	// easy edge cases
	uint32_t min_path = ((T[0] >= S[0]) ? (T[0] - S[0]) : (S[0] - T[0])) + ((T[1] >= S[1]) ? (T[1] - S[1]) : (S[1] - T[1]));
	
	if (min_path <= F || f_count <= F) {
		printf("%u \n", min_path);
		free(m);
		return res;
	}

	

	CQueue cq;
	init(&cq);

	// 



	destroy(&cq);
	free(m);
	return res;
}



// FUNCS ------------------------------------------------------------
static inline uint8_t read_bit() {
	int c;
	while ((c = getchar_unlocked()) != '0' && c != '1' && c != EOF);
	return (uint8_t)(c - '0');
}

static inline uint16_t read_uint() {
	uint16_t res = 0;
	int c = getchar_unlocked();

	while ((c < '0' || c > '9') && c != EOF) c = getchar_unlocked();
	
	while (c >= '0' && c <= '9') {
		res = res * 10 + (c - '0');
		c = getchar_unlocked();
	}

	return res;
}

void alloc_matrix(uint8_t** m, uint16_t R, uint16_t C) {
	*m = (uint8_t*)malloc(R * C * sizeof(uint8_t));
	if (!(*m)) {
		fputs("ERROR: Matrix allocation.\n", stderr);
		exit(ALLOC_ERROR);
	}
}

void fill_matrix(uint8_t* m, uint16_t R, uint16_t C, uint32_t* f_count) {
	for (uint32_t r = 0; r < R; ++r) {
		for (uint32_t c = 0; c < C; ++c) {
			uint32_t coord = r * C + c;
			m[coord] = read_bit(); 
			if (m[coord]) (*f_count)++;
		}
	}
}

void print_matrix(uint8_t* m, uint16_t R, uint16_t C) {
	for (uint16_t r = 0; r < R; ++r) {
		for (uint16_t c = 0; c < C; ++c) {
			printf("%hhu ", m[r * C + c]); 
		}
		printf("\n");
	}
}

// translate a visual coord from the task to actual one
static inline uint32_t visual_to_actual(uint32_t visual_coord) {
	return visual_coord - 1;
}

uint8_t parse_input(uint16_t* R, uint16_t* C, uint16_t* F, uint32_t* S, uint32_t* T, uint8_t** m, uint32_t* f_count) {
	// 1st line input 
	if (FAST_READ) {
		*R = read_uint();
		*C = read_uint();
		*F = read_uint();
	} else {
		if (scanf("%hu %hu %hu", R, C, F) != 3) {
			fputs("ERROR: Reading 1st line input.\n", stderr);
			exit(INPUT_ERROR);
		} 
	}

	// 1st line range check	
	if (*R < 1 || *R > 750 || *C < 1 || *C > 750 || *F < 1 || *F > 30) {
		fputs("ERROR: 1st line input range.\n", stderr);
		exit(RANGE_ERROR);
	}
	

	
	// 2nd line input
	if (FAST_READ) {
		S[0] = read_uint();
		S[1] = read_uint();
	} else {
		if (scanf("%u %u", &S[0], &S[1]) != 2) {
			fputs("ERROR: Reading 2nd line input.\n", stderr);
			exit(INPUT_ERROR);
		} 
	}

	// 2nd line range check
	if (S[0] < 1 || S[0] > *R || S[1] < 1 || S[1] > *C) {
		fputs("ERROR: 2nd line input range.\n", stderr);
		exit(RANGE_ERROR);
	}
	
	// 3rd line input
	if (FAST_READ) {
		T[0] = read_uint();
		T[1] = read_uint();
	} else {
		if (scanf("%u %u", &T[0], &T[1]) != 2) {
			fputs("ERROR: Reading 3rd line input.\n", stderr);
			exit(INPUT_ERROR);
		} 
	}

	// 3rd line range check
	if (T[0] < 1 || T[0] > *R || T[1] < 1 || T[1] > *C) {
		fputs("ERROR: 3rd line input range.\n", stderr);
		exit(RANGE_ERROR);
	}

	S[0] = visual_to_actual(S[0]);
	S[1] = visual_to_actual(S[1]);
	T[0] = visual_to_actual(T[0]);
	T[1] = visual_to_actual(T[1]);



	alloc_matrix(m, *R, *C);
	fill_matrix(*m, *R, *C, f_count);
	// (*m)[visual_to_actual(S[0]) * *C + visual_to_actual(S[1])] = START;
	// (*m)[visual_to_actual(T[0]) * *C + visual_to_actual(T[1])] = TARGET;

	return NO_ERROR;

}

