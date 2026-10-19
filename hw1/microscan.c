#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define abs(a) (((a) >= (0)) ? (a) : (-(a)))

#define DEBUG_PRINTS 0



typedef struct {
	int N;
	int K;
	char* content;
} Matrix;

typedef struct {
	int N;
	int K;
	int* content;
} PrefMatrix;



static inline char read_bit();

static inline int read_int();

void alloc_matrix(Matrix* m);

void alloc_pref_matrix(PrefMatrix* p);

void fill_matrix(Matrix* m);

void print_matrix(Matrix* m);

void print_pref_matrix(PrefMatrix* p);

static inline bool is_valid_segment(Matrix* m, int x1, int y1);

void prefix_sum_inplace(Matrix* m, PrefMatrix* p);

static inline int region_sum(PrefMatrix* p, int i, int j, int k, int l);



int main(int argc, char* argv[]) {
	Matrix m;
	PrefMatrix p;	

	m.N = read_int();
	m.K = read_int();

	p.N = m.N + 1;

	alloc_matrix(&m);
	alloc_pref_matrix(&p);

	fill_matrix(&m);	
	prefix_sum_inplace(&m, &p);

	if (DEBUG_PRINTS) {
		printf("N = %d, K = %d\n", m.N, m.K);
 		printf("\n");
 		print_matrix(&m);
 		printf("\n");
 		print_pref_matrix(&p);
 		printf("\n");
	}

	int max_segments = 0;
	for (int r = 0; r <= m.N - m.K; ++r) {
		for (int c = 0; c <= m.N - m.K; ++c) {
			int tmp_res = region_sum(&p, r, c, r+m.K-1, c+m.K-1);
			if (tmp_res > max_segments) max_segments = tmp_res;
		}
	}

	printf("%d\n", max_segments);

	free(m.content);
	free(p.content);
	
	return 0;
}



static inline char read_bit() {
	int c;
	while ((c = getchar_unlocked()) != '0' && c != '1' && c != EOF);
	return (char)(c - '0');
}

static inline int read_int() {
	int res = 0;
	int c = getchar_unlocked();

	while ((c < '0' || c > '9') && c != EOF) c = getchar_unlocked();
	
	while (c >= '0' && c <= '9') {
		res = res * 10 + (c - '0');
		c = getchar_unlocked();
	}

	return res;
}

void alloc_matrix(Matrix* m) {
	m->content = (char*)calloc(m->N * m->N, sizeof(char));
	if (!m->content) {
		perror("Allocation Error");
		return;
	}
}

void alloc_pref_matrix(PrefMatrix* p) {
	p->content = (int*)calloc(p->N * p->N, sizeof(int));
	if (!p->content) {
		perror("Allocation Error");
		return;
	}
}

void fill_matrix(Matrix* m) {
	for (int r = 0; r < m->N; ++r) {
		for (int c = 0; c < m->N; ++c) {
			m->content[r * m->N + c] = read_bit();
		}
	}
}

void print_matrix(Matrix* m) {
	for (int r = 0; r < m->N; ++r) {
		for (int c = 0; c < m->N; ++c) {
			printf("%d ", m->content[r * m->N + c]);
		}
		printf("\n");
	}
}

void print_pref_matrix(PrefMatrix* p) {
	for (int r = 0; r < p->N; ++r) {
		for (int c = 0; c < p->N; ++c) {
			printf("%d ", p->content[r * p->N + c]);
		}
		printf("\n");
	}
}

static inline bool is_valid_segment(Matrix* m, int x1, int y1) {
	int x2 = x1+1;
	int y2 = y1+1;

	if (x2 >= m->N || y2 >= m->N || m->content[x1 * m->N + y1] != 1 || m->content[x2 * m->N + y2] != 1) return false;

	int x1_start = max(x1-1, 0);
	int y1_start = max(y1-1, 0);
		
	int x2_end = min(x2+1, m->N-1);
	int y2_end = min(y2+1, m->N-1);

	for (int r = x1_start; r <= x2_end; ++r) {
		for (int c = y1_start; c <= y2_end; ++c) {
			if ((r == x1 && c == y1) || (r == x2 && c == y2) || 
                           (r == x1-1 && c == y1+2) || 
		           (r == x1+2 && c == y1-1)) continue;
			if (m->content[r * m->N + c] == 1) return false;
		}
	}

	return true;
	
}

void prefix_sum_inplace(Matrix* m, PrefMatrix* p) {
	for (int r = 0; r < m->N; ++r) {
		for (int c = 0; c < m->N; ++c) {
			int val = is_valid_segment(m, r, c) ? 1 : 0;

			int p_r = r+1;
			int p_c = c+1;
			
			p->content[p_r * p->N + p_c] = val + p->content[r * p->N + p_c] + p->content[p_r * p->N + c] - p->content[r * p->N + c];
		}
	}	
}

static inline int region_sum(PrefMatrix* p, int i, int j, int k, int l) {
	return p->content[k * p->N + l] - p->content[i * p->N + l] - p->content[k * p->N + j] + p->content[i * p->N + j];
}

