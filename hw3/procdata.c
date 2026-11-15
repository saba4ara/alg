#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



#ifndef FAST_READ
#define FAST_READ 1
#endif

#ifndef FIRST_NODE
#define FIRST_NODE 1
#endif

#ifndef NEIGHBOURS_COUNT
#define NEIGHBOURS_COUNT 3
#endif

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
	unsigned int neighbours[NEIGHBOURS_COUNT];
	unsigned char degree;
	unsigned char type;
} Node;

typedef struct {
	unsigned int node_id;
	unsigned int parent_id;
	unsigned char next_neighbour_idx;
	unsigned char count_E;
	unsigned char count_V;
} StackFrame;

typedef struct {
	StackFrame* data;
	int top;
} Stack;



// FUNCS ------------------------------------------------------------
inline unsigned int read_uint() {
	unsigned int res = 0;
	int c = getchar_unlocked();

	while ((c < '0' || c > '9') && c != EOF) c = getchar_unlocked();
	
	while (c >= '0' && c <= '9') {
		res = res * 10 + (c - '0');
		c = getchar_unlocked();
	}

	return res;
}

int parse_input(unsigned int* P, unsigned int* S, unsigned int* E, unsigned int* V, Node** graph) {
	// 1st line
	if (FAST_READ) {
		*P = read_uint();
		*S = read_uint();
		*E = read_uint();
		*V = read_uint();
	} else {
		if (scanf("%u %u %u %u", P, S, E, V) != 4) {
			puts("ERROR: Reading 1st line input.");
			return INPUT_ERROR;
		} 
	}


	*graph = (Node*)malloc((*P+1) * sizeof(Node));
	if (!graph) {
		puts("ERROR: Couldn't allocate graph.");
		exit(ALLOC_ERROR);
	}
	
	if (*P < 3 || *P > 1000000 || *E < 1 || *V < 1) {
		puts("ERROR: 1st line input range.");
		return RANGE_ERROR;
	}

	// initializing the graph
	for (int i = 1; i <= *P; ++i) {
		(*graph)[i].degree = 0;
		(*graph)[i].type = EMPTY;

		for (int j = 0; j < NEIGHBOURS_COUNT; ++j) {
			(*graph)[i].neighbours[j] = 0;
		}
	}	

	
	// S lines of computers' connections
	for (int i = 1; i <= *S; ++i) {
		unsigned int p1, p2;
		if (FAST_READ) {
			p1 = read_uint();
			p2 = read_uint();
		} else {
			if (scanf("%u %u", &p1, &p2) != 2) {
				puts("ERROR: Reading computers' connections input.");
				return INPUT_ERROR;
			}
		}

		if (p1 < 1 || p1 > *P || p2 < 1 || p2 > *P) {
			puts("ERROR: Computers' connections input range.");
			return RANGE_ERROR;
		}

		unsigned int p1_degree = (*graph)[p1].degree;
		(*graph)[p1].neighbours[p1_degree] = p2;
		(*graph)[p1].degree++;		
		
		unsigned int p2_degree = (*graph)[p2].degree;
		(*graph)[p2].neighbours[p2_degree] = p1;
		(*graph)[p2].degree++;		
	}

	// line with E extractors
	for (int i = 0; i < *E; ++i) {
		unsigned int e;
		if (FAST_READ) {
			e = read_uint();
		} else {
			if (scanf("%u", &e) != 1) {
				puts("ERROR: Reading extractor's value input.");
				return INPUT_ERROR;
			}
		}

		if (e < 1) {
			puts("ERROR: Extractor's value input range.");
			return RANGE_ERROR;
		}
		(*graph)[e].type = EXT;
	}

	// line with V validators
	for (int i = 0; i < *V; ++i) {
		unsigned int v;
		if (FAST_READ) {
			v = read_uint();
		} else {	
			if (scanf("%u", &v) != 1) {
				puts("ERROR: Reading validator's value input.");
				return INPUT_ERROR;
			}
		}

		if (v < 1) {
			puts("ERROR: Validator's value input range.");
			return RANGE_ERROR;
		}
		(*graph)[v].type = VAL;
	}

	return NO_ERROR;

}

unsigned int solve_dfs(unsigned int root, unsigned int P, Node* graph) {
	Stack stack;
	stack.data = (StackFrame*)malloc(P * sizeof(StackFrame));
	if (!stack.data) {
		puts("ERROR: Couldn't allocate stack.");
		exit(ALLOC_ERROR);
	}

	stack.top = -1;

	unsigned int pairs_count = 0;

	// first frame
	stack.data[++stack.top].node_id = root;
	stack.data[stack.top].parent_id = 0;
	stack.data[stack.top].next_neighbour_idx = 0;
	stack.data[stack.top].count_E = 0;
	stack.data[stack.top].count_V = 0;
	
	// stack
	while (stack.top >= 0) {
		StackFrame* current = &stack.data[stack.top];
		unsigned int u = current->node_id;
		
		// DFSing downwards
		if (current->next_neighbour_idx < graph[u].degree) {
			unsigned int v = graph[u].neighbours[current->next_neighbour_idx++];
			
			// is parent
			if (v == current->parent_id) continue;

			// is child
			stack.data[++stack.top].node_id = v;
			stack.data[stack.top].parent_id = u;	
			stack.data[stack.top].next_neighbour_idx = 0;
			stack.data[stack.top].count_E = 0;
			stack.data[stack.top].count_V = 0;
			continue;
		}

		// deciding
		unsigned int pass_E = 0;
		unsigned int pass_V = 0;

		unsigned int total_E = current->count_E;
		unsigned int total_V = current->count_V;
		if (graph[u].type == EXT) total_E++;
		if (graph[u].type == VAL) total_V++;

		if (total_E > 0 && total_V > 0) pairs_count++;
		else if (total_E > 0) pass_E = 1;
		else if (total_V > 0) pass_V = 1;

		// propagating results upwards
		stack.top--;

		if (stack.top >= 0) {
			stack.data[stack.top].count_E += pass_E;
			stack.data[stack.top].count_V += pass_V;	
		}
	}

	free(stack.data);
	return pairs_count;
}

// debuging purposes only
void print_graph(unsigned int* P, unsigned int* S, Node* graph) {
	for (int i = 1; i <= *P; ++i ) {
		printf("NODE %d\n", i);
		
		unsigned int neighbours_count = sizeof(graph[i].neighbours) / sizeof(unsigned int);
		for (int j = 0; j < neighbours_count; ++j) {
			printf("neighbour %d is %u\n", j, graph[i].neighbours[j]);
		}
		
		printf("degree = %u\n", graph[i].degree);
		
		char* type = (graph[i].type == 0) ? "EXT" : ((graph[i].type == 1) ? "VAL" : "EMPTY");
		printf("type = %s\n", type);
	}
}



// MAIN -------------------------------------------------------------
int main (int argc, char* argv[]) {
	unsigned int P, S, E, V;
	Node* graph;
	
	int res = parse_input(&P, &S, &E, &V, &graph);
	unsigned int pairs_count = solve_dfs(FIRST_NODE, P, graph);
	printf("%u \n", pairs_count);
	
	free(graph);
	return res;
}

