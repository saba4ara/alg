#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



#define min(a, b) (((a) < (b)) ? (a) : (b))



// ENUMS ------------------------------------------------------------
enum Errors {
	NO_ERROR,
	INPUT_ERROR,
	ALLOC_ERROR,
	RANGE_ERROR
};

enum ComputerTypes {
	EXT,
	VAL,
	EMPTY
};



// STRUCTS ----------------------------------------------------------
typedef struct {
	unsigned int neighbours[3];
	unsigned int degree;
	unsigned int type;
	bool is_used;
} Node;



// FUNCS ------------------------------------------------------------
void append(unsigned int number, unsigned int* array) {
	unsigned int array_size = sizeof(*array) / sizeof(unsigned int);
	for (int i = 0; i < array_size; ++i) {
		if (array[i] == -1) array[i] = number;
	}
}

int parse_input(unsigned int* P, unsigned int* S, unsigned int* E, unsigned int* V, Node** graph) {
	// 1st line
	if (scanf("%u %u %u %u\n", P, S, E, V) != 4) {
		puts("ERROR: Reading 1st line input.");
		return INPUT_ERROR;
	}

	*graph = (Node*)malloc((*P+1) * sizeof(Node));
	if (!graph) exit(ALLOC_ERROR);
	
	if (*P < 3 || *P > 1000000 || *E < 1 || *V < 1) {
		puts("ERROR: 1st line input range.");
		return RANGE_ERROR;
	}

	for (int i = 1; i <= *S; ++i) {
		for (int j = 0; j < 3; ++j) {
			graph[i]->neighbours[j] = -1;
		}
		graph[i]->degree = 0;
		graph[i]->type = EMPTY;
		graph[i]->is_used = false;
	}	

	// S lines of computers' connections
	for (int i = 1; i <= *S; ++i) {
		unsigned int p1, p2;
		if (scanf("%u %u\n", &p1, &p2) != 2) {
			puts("ERROR: Reading computers' connections input.");
			return INPUT_ERROR;
		}

		if (p1 < 1 || p1 > *P || p2 < 1 || p2 > *P) {
			puts("ERROR: Computers' connections input range.");
			return RANGE_ERROR;
		}

		append(p2, graph[p1]->neighbours);
		append(p1, graph[p2]->neighbours);
		graph[p1]->degree++;		
		graph[p2]->degree++;		

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
		graph[e]->type = EXT;
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
		graph[v]->type = VAL;
	}

	return NO_ERROR;

}



// MAIN -------------------------------------------------------------
int main (int argc, char* argv[]) {
	unsigned int P, S, E, V;
	Node* graph;
	
	int res = parse_input(&P, &S, &E, &V, &graph);

	unsigned int pairs_count = 0;
	unsigned int pairs_count_upper_bound = min(E, V);

	free(graph);
	return res;
}

