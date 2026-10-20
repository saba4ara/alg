#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// #define max(a, b) (((a) > (b)) ? (a) : (b))
// #define min(a, b) (((a) < (b)) ? (a) : (b))
#define abs(a) (((a) >= (0)) ? (a) : (-(a)))

#define DEBUG_PRINTS 1

typedef struct {
	char R;
	char C;
	char* content;
} Matrix;

typedef enum orientation {
	VERTICAL,
	HORIZONTAL
} Orient;



static inline char read_bit();

void alloc_matrix(Matrix* m);

void fill_matrix(Matrix* m);

void print_matrix(Matrix* m);

// (r, c) is left upper stone of the triple
char calculate_triple(Matrix* m, char r, char c, Orient orient);

// (r1, c1) and (r2, c2) are left upper stones of the triples
bool triples_overlap(Matrix* m, char r1, char c1, Orient o1, char r2, char c2, Orient o2);

void debug_prints(Matrix* m);



int main (int argc, char* argv[]) {
	Matrix m;
	m.R = read_bit();
	m.C = read_bit();

	if (m.R < 3 || m.R > 10 || m.C < 3 || m.C > 10) {
		perror("Invalid R or C value");
		return 1;
	}

	alloc_matrix(&m);
	fill_matrix(&m);

	if (DEBUG_PRINTS) debug_prints(&m);

	

	free(m.content);

	return 0;
}



static inline char read_bit() {
	int c;
	while (((c = getchar_unlocked()) < '0' || c > '9' ) && c != EOF);
	return (char)(c - '0');
}

void alloc_matrix(Matrix* m) {
	m->content = (char*)calloc(m->R * m->C, sizeof(char));
	if (!m->content) {
		perror("Allocation Error");
		return;
	}
}

void fill_matrix(Matrix* m) {
	for (char r = 0; r < m->R; ++r) {
		for (char c = 0; c < m->C; ++c) {
			m->content[r * m->C + c] = read_bit();
		}
	}
}

void print_matrix(Matrix* m) {
	for (char r = 0; r < m->R; ++r) {
		for (char c = 0; c < m->C; ++c) {
			printf("%d ", m->content[r * m->C + c]); 
		}
		printf("\n");
	}
}

char calculate_triple(Matrix* m, char r, char c, Orient o) {
	char res = 0;	

	if (r < 0 || r >= m->R || c < 0 || c >= m->C) return -1;

	char first_stone, second_stone, third_stone;
	first_stone = second_stone = third_stone = m->content[r * m->C + c];

	switch (o) {
		case VERTICAL:
			if ((r+2) >= m->R) return -1;

			first_stone = m->content[r * m->C + c];
			second_stone = m->content[(r+1) * m->C + c];
			third_stone = m->content[(r+2) * m->C + c];
			
			break;
		case HORIZONTAL:
			if ((c+2) >= m->C) return -1;

			first_stone = m->content[r * m->C + c];
		 	second_stone = m->content[r * m->C + (c+1)];
		 	third_stone = m->content[r * m->C + (c+2)];
			
			break;
	}	

	if (first_stone == second_stone && second_stone == third_stone) res = 3;
	else if ((first_stone == second_stone) || (second_stone == third_stone) || (first_stone == third_stone)) res = 1;
	
	return res;
}

bool triples_overlap(Matrix* m, char r1, char c1, Orient o1, char r2, char c2, Orient o2) {
	char end_r1 = r1 + (o1 == VERTICAL ? 2 : 0);
	char end_c1 = c1 + (o1 == VERTICAL ? 0 : 2);
	char end_r2 = r2 + (o2 == VERTICAL ? 2 : 0);
	char end_c2 = c2 + (o2 == VERTICAL ? 0 : 2);

	bool rows_overlap = (r1 <= end_r2) && (r2 <= end_r1);
	bool cols_overlap = (c1 <= end_c2) && (c2 <= end_c1);
	
	return rows_overlap && cols_overlap;
}

void debug_prints(Matrix* m) {
	printf("%d %d\n", m->R, m->C);
	print_matrix(m);
	printf("\n");

	for (char r = 0; r < m->R; ++r) {
		for (char c = 0; c < m->C; ++c) {
			char ver_res = calculate_triple(m, r, c, VERTICAL);
			//if (ver_res == 3) printf("vertical triple on (%d, %d) is %d\n", r, c, ver_res);

			char hor_res = calculate_triple(m, r, c, HORIZONTAL);
			//if (hor_res == 3) printf("horizontal triple on (%d, %d) is %d\n", r, c, hor_res);
		}
	}

	srand(time(NULL));
	int i = 0;
	while (i < 10) {
		char r1 = rand() % m->R;
		char c1 = rand() % m->C;
		char r2 = rand() % m->R;
		char c2 = rand() % m->C;
		
		char o1 = rand() % 2;
		char o2 = rand() % 2;

		if (r1 + 2 >= m->R || r2 + 2 >= m->R || c1 + 2 >= m->C || c2 + 2 >= m->C) continue;

		//printf("(%d, %d) and (%d, %d) are random triple\n", r1, c1, r2, c2);
		printf("do (%d, %d) %s and (%d, %d) %s collide? - %s\n", r1, c1, o1 == 0 ? "VERTICAL" : "HORIZONTAL", r2, c2, o2 == 0 ? "VERTICAL" : "HORIZONTAL", triples_overlap(m, r1, c1, o1, r2, c2, o2) ? "YES" : "NO");
		printf("\n");
		i++;
	} 
}

