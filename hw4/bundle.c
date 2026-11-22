#define _POSIX_C_SOURCE 200809L



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



#ifndef FAST_READ
#define FAST_READ 1
#endif

#ifndef CQ_SIZE
#define CQ_SIZE 1024
#endif



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


typedef struct {
	State* content;

	unsigned int head;
	unsigned int tail;
	
	unsigned int size;
	unsigned int capacity;
} CQueue;



// CIRCULAR QUEUE FUNCS ---------------------------------------------
void init(CQueue* cq) {
	cq->head = cq->tail = cq->size = 0;
	cq->capacity = CQ_SIZE;	

	cq->content = (State*)malloc(cq->capacity * sizeof(State));
	if (!cq->content) exit(ALLOC_ERROR);
}

void resize(CQueue* cq) {
	State* tmp = (State*)malloc(2 * cq->capacity * sizeof(State));
	if (!tmp) exit(ALLOC_ERROR);
	
	// 2x memset
	for (unsigned int i = 0; i < cq->size; ++i) {
		tmp[i] = cq->content[(cq->head + i) % cq->capacity];
	}

	cq->capacity *= 2;
	
	cq->head = 0;
	cq->tail = cq->size;
	free(cq->content);
	cq->content = tmp;
}

void push(CQueue* cq, State x) {
	if (cq->size == cq->capacity) resize(cq);
	
	cq->content[cq->tail] = x;
	
	cq->tail = (cq->tail + 1) % cq->capacity;
	cq->size++;
}

// size > 0
State pop(CQueue* cq) {
	State tmp = cq->content[cq->head];
	cq->head = (cq->head + 1) % cq->capacity;
	cq->size--;
	
	return tmp;
}

// size > 0
State get_tail(CQueue* cq) {
	return cq->content[(cq->head + cq->size - 1) % cq->capacity];
}

// size > 0
State get_head(CQueue* cq) {
	return cq->content[cq->head];
}

void destroy(CQueue* cq) {
	free(cq->content);
	cq->content = NULL;
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

	return NO_ERROR;

}

uint32_t solve_bfs(uint8_t* m, uint16_t R, uint16_t C, uint16_t F, uint32_t* S, uint32_t* T) {
	size_t grid_size = (size_t)R * C;
	uint8_t* min_fires = (uint8_t*)malloc(grid_size * sizeof(uint8_t));
	if (!min_fires) {
		fputs("ERROR: min_fires allocation.\n", stderr);
		exit(ALLOC_ERROR);
	}

	memset(min_fires, (int)(F + 1), grid_size);

	uint32_t start_idx = S[0] * C + S[1];
	uint32_t target_idx = T[0] * C + T[1];

	if (start_idx == target_idx) {
		free(min_fires);
		return 0;
	}

	CQueue cq;
	init(&cq);

	min_fires[start_idx] = 0;
	State start = {(uint16_t)S[0], (uint16_t)S[1], 0};
	push(&cq, start);
	
	static const int dr[4] = {-1, 1, 0, 0};
	static const int dc[4] = {0, 0, -1, 1};

	uint32_t distance = 0;

	while (cq.size > 0) {
		unsigned int layer_size = cq.size;
		distance++;

		for (unsigned int i = 0; i < layer_size; ++i) {
			State curr = pop(&cq);
			uint32_t curr_idx = (uint32_t)curr.r * C + curr.c;

			if (curr.fires > min_fires[curr_idx]) continue;

			for (int d = 0; d < 4; ++d) {
				int nr = (int)curr.r + dr[d];
				int nc = (int)curr.c + dc[d];

				if (nr < 0 || nr >= (int)R || nc < 0 || nc >= (int)C) continue;

				uint32_t next_idx = (uint32_t)nr * C + (uint32_t)nc;
				uint8_t next_fires = (uint8_t)(curr.fires + m[next_idx]);

				if (next_fires < min_fires[next_idx]) {
					if (next_idx == target_idx) {
						free(min_fires);
						destroy(&cq);
						return distance;
					}

					min_fires[next_idx] = next_fires;
					State next_state = {(uint16_t)nr, (uint16_t)nc, next_fires};
					push(&cq, next_state);
				}
			}
		}
	}

	free(min_fires);
	destroy(&cq);
	return 0;
}


 
// MAIN -------------------------------------------------------------
int main(int argc, char* argv[]) {
	uint16_t R = 0, C = 0, F = 0;
	uint32_t S[2] = {0}, T[2] = {0};
	uint8_t* m;
	
	uint32_t f_count = 0;
	uint32_t best_path = 0;

	uint8_t res = 0;
	res = parse_input(&R, &C, &F, S, T, &m, &f_count);
	
	// easy edge cases
	uint32_t min_path = ((T[0] >= S[0]) ? (T[0] - S[0]) : (S[0] - T[0])) + ((T[1] >= S[1]) ? (T[1] - S[1]) : (S[1] - T[1]));
	
	if (min_path <= F || f_count <= F) {
		printf("%u\n", min_path);
		free(m);
		return res;
	}

	best_path = solve_bfs(m, R, C, F, S, T);
	printf("%u\n", best_path);

	free(m);
	return res;
}

