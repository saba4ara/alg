#include <stdio.h>
#include <stdbool.h>



#define min(a, b) (((a) < (b)) ? (a) : (b))



// ENUMS ------------------------------------------------------------
enum Errors {
	NO_ERROR,
	INPUT_ERROR,
	RANGE_ERROR
};

enum ComputerTypes {
	E,
	V,
	EMPTY
};



// STRUCTS ----------------------------------------------------------
typedef struct {
	unsigned int router_neighbour;
	int type;
	// TODO
} ComputerNode;

typedef struct {
	unsigned int neighbours[3];
	bool is_used;
	// TODO
} RouterNode;



// FUNCS ------------------------------------------------------------
int parse_input(unsigned int* P, unsigned int* S, unsigned int* E, unsigned int* V) {
	// 1st line
	if (scanf("%u %u %u %u\n", P, S, E, V) != 4) {
		puts("ERROR: Reading 1st line input.");
		return INPUT_ERROR;
	}
	
	if (*P < 3 || *P > 1000000 || *E < 1 || *V < 1) {
		puts("ERROR: 1st line input range.");
		return RANGE_ERROR;
	}

	// S lines of computers' connections
	for (int i = 0; i < *S; ++i) {
		unsigned int p1, p2;
		if (scanf("%u %u\n", &p1, &p2) != 2) {
			puts("ERROR: Reading computers' connections input.");
			return INPUT_ERROR;
		}

		if (p1 < 1 || p1 > *P || p2 < 1 || p2 > *P) {
			puts("ERROR: Computers' connections input range.");
			return RANGE_ERROR;
		}
	}

	// line with E extractors
	for (int i = 0; i < *E; ++i) {
		unsigned int e;
		if (scanf("%u", &e) != 1) {
			puts("ERROR: Reading extractor's value input.");
			return INPUT_ERROR;
		}

		if (e < 1) {
			puts("ERROR: Extractor's value input range.");
			return RANGE_ERROR;
		}
	}

	// line with V validators
	for (int i = 0; i < *V; ++i) {
		unsigned int v;
		if (scanf("%u", &v) != 1) {
			puts("ERROR: Reading validator's value input.");
			return INPUT_ERROR;
		}

		if (v < 1) {
			puts("ERROR: Validator's value input range.");
			return RANGE_ERROR;
		}
	}

	return NO_ERROR;

}



// MAIN -------------------------------------------------------------
int main (int argc, char* argv[]) {
	unsigned int P, S, E, V;
	int res = parse_input(&P, &S, &E, &V);

	unsigned int pairs_count = 0;
	unsigned int pairs_count_upper_bound = min(E, V);

	return res;
}

